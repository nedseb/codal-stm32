#include "MP34DT01.h"

#include <cmath>
#include <list>

using namespace codal;

struct MP34DT01HandlerMapping {
    MP34DT01* instance;
    DFSDM_Channel_HandleTypeDef* hdfsdmChannel;
    DFSDM_Filter_HandleTypeDef* hdfsdmFilter;
    DMA_HandleTypeDef* dma;
};

constexpr uint32_t rightShiftBit                  = 8;
std::list<MP34DT01HandlerMapping>* handlerMapping = nullptr;

void MSP_ChannelInit_callback(DFSDM_Channel_HandleTypeDef* hdfsdm_channel);
void MSP_ChannelDeinit_callback(DFSDM_Channel_HandleTypeDef* hdfsdm_channel);
void MSP_FilterInit_callback(DFSDM_Filter_HandleTypeDef* hdfsdm_filter);
void MSP_FilterDeinit_callback(DFSDM_Filter_HandleTypeDef* hdfsdm_filter);
void MSP_FilterRegConvHalfCplt_callback(DFSDM_Filter_HandleTypeDef* hdfsdm_filter);
void MSP_FilterRegConvCplt_callback(DFSDM_Filter_HandleTypeDef* hdfsdm_filter);

MP34DT01::MP34DT01()
{
    if (handlerMapping == nullptr) {
        handlerMapping = new std::list<MP34DT01HandlerMapping>();
    }

    measuredSquaredRMS = 0;
    hdfsdm_channel     = (DFSDM_Channel_HandleTypeDef*)malloc(sizeof(DFSDM_Channel_HandleTypeDef));
    hdfsdm_filter      = (DFSDM_Filter_HandleTypeDef*)malloc(sizeof(DFSDM_Filter_HandleTypeDef));
    hdma_dfsdm         = (DMA_HandleTypeDef*)malloc(sizeof(DMA_HandleTypeDef));

    hdfsdm_channel->Instance                      = DFSDM1_Channel2;
    hdfsdm_channel->Init.OutputClock.Activation   = ENABLE;
    hdfsdm_channel->Init.OutputClock.Selection    = DFSDM_CHANNEL_OUTPUT_CLOCK_SYSTEM;
    hdfsdm_channel->Init.OutputClock.Divider      = 75;
    hdfsdm_channel->Init.Input.Multiplexer        = DFSDM_CHANNEL_EXTERNAL_INPUTS;
    hdfsdm_channel->Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE;
    hdfsdm_channel->Init.Input.Pins               = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
    hdfsdm_channel->Init.SerialInterface.Type     = DFSDM_CHANNEL_SPI_RISING;
    hdfsdm_channel->Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
    hdfsdm_channel->Init.Awd.FilterOrder          = DFSDM_CHANNEL_FASTSINC_ORDER;
    hdfsdm_channel->Init.Awd.Oversampling         = 1;
    hdfsdm_channel->Init.Offset                   = 0;
    hdfsdm_channel->Init.RightBitShift            = rightShiftBit;
    hdfsdm_channel->MspInitCallback               = MSP_ChannelInit_callback;
    hdfsdm_channel->MspDeInitCallback             = MSP_ChannelDeinit_callback;

    hdfsdm_filter->Instance                         = DFSDM1_Filter0;
    hdfsdm_filter->Init.RegularParam.Trigger        = DFSDM_FILTER_SW_TRIGGER;
    hdfsdm_filter->Init.RegularParam.FastMode       = ENABLE;
    hdfsdm_filter->Init.RegularParam.DmaMode        = ENABLE;
    hdfsdm_filter->Init.FilterParam.SincOrder       = DFSDM_FILTER_SINC3_ORDER;
    hdfsdm_filter->Init.FilterParam.Oversampling    = 250;
    hdfsdm_filter->Init.FilterParam.IntOversampling = 1;
    hdfsdm_filter->MspInitCallback                  = MSP_FilterInit_callback;
    hdfsdm_filter->MspDeInitCallback                = MSP_FilterDeinit_callback;

    handlerMapping->push_back((MP34DT01HandlerMapping){
        .instance = this, .hdfsdmChannel = hdfsdm_channel, .hdfsdmFilter = hdfsdm_filter, .dma = hdma_dfsdm});
}

bool MP34DT01::init()
{
    RCC_PeriphCLKInitTypeDef PeriphClkInit;
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_DFSDM1;
    PeriphClkInit.Dfsdm1ClockSelection = RCC_DFSDM1CLKSOURCE_PCLK;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        return false;
    }

    __HAL_RCC_DMA1_CLK_ENABLE();
    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

    if (HAL_DFSDM_ChannelInit(hdfsdm_channel) != HAL_OK) {
        return false;
    }

    if (HAL_DFSDM_FilterInit(hdfsdm_filter) != HAL_OK) {
        deinit();
        return false;
    }
    hdfsdm_filter->RegConvCpltCallback     = MSP_FilterRegConvCplt_callback;
    hdfsdm_filter->RegConvHalfCpltCallback = MSP_FilterRegConvHalfCplt_callback;

    if (HAL_DFSDM_FilterConfigRegChannel(hdfsdm_filter, DFSDM_CHANNEL_2, DFSDM_CONTINUOUS_CONV_ON) != HAL_OK) {
        deinit();
        return false;
    }

    if (HAL_DFSDM_FilterRegularStart_DMA(hdfsdm_filter, audioBuffer, MP34DT01_AUDIO_BUFFER) != HAL_OK) {
        deinit();
        return false;
    }

    return true;
}

void MP34DT01::deinit()
{
    HAL_DFSDM_FilterRegularStop_DMA(hdfsdm_filter);
    HAL_DFSDM_FilterDeInit(hdfsdm_filter);
    HAL_DFSDM_ChannelDeInit(hdfsdm_channel);
}

void MP34DT01::mspChannelInit(DFSDM_Channel_HandleTypeDef* hdfsdm_channel)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_DFSDM1_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitStruct.Pin       = GPIO_PIN_7 | GPIO_PIN_9;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_DFSDM1;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

void MP34DT01::mspChannelDeinit(DFSDM_Channel_HandleTypeDef* hdfsdm_channel)
{
    __HAL_RCC_DFSDM1_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_7 | GPIO_PIN_9);
}

void MP34DT01::mspFilterInit(DFSDM_Filter_HandleTypeDef* hdfsdm_filter)
{
    hdma_dfsdm->Instance                 = DMA1_Channel4;
    hdma_dfsdm->Init.Request             = DMA_REQUEST_0;
    hdma_dfsdm->Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_dfsdm->Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_dfsdm->Init.MemInc              = DMA_MINC_ENABLE;
    hdma_dfsdm->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_dfsdm->Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    hdma_dfsdm->Init.Mode                = DMA_CIRCULAR;
    hdma_dfsdm->Init.Priority            = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(hdma_dfsdm) != HAL_OK) {
        // Hard fault handler ?
    }

    __HAL_LINKDMA(hdfsdm_filter, hdmaInj, *hdma_dfsdm);
    __HAL_LINKDMA(hdfsdm_filter, hdmaReg, *hdma_dfsdm);
}

void MP34DT01::mspFilterDeinit(DFSDM_Filter_HandleTypeDef* hdfsdm_filter)
{
    HAL_DMA_DeInit(hdfsdm_filter->hdmaInj);
    HAL_DMA_DeInit(hdfsdm_filter->hdmaReg);
}

void MP34DT01::mspFilterConversionHalfComplete(DFSDM_Filter_HandleTypeDef* hdfsdm_filter)
{
    return;
}

void MP34DT01::mspFilterConversionComplete(DFSDM_Filter_HandleTypeDef* hdfsdm_filter)
{
    uint64_t sum = 0;

    for (auto v : audioBuffer) {
        uint64_t tmp = abs(v);
        tmp >>= 8;
        sum += tmp * tmp;
    }

    measuredSquaredRMS = sum / (uint64_t)MP34DT01_AUDIO_BUFFER;
}

int32_t MP34DT01::getDecibel()
{
    return 10.0 * log10((double)measuredSquaredRMS);
}

// ==========================================================================
// ==================== CALLBACKS & INTERNAL FUNCTIONS ===================
// ====================================================================

MP34DT01* findInstanceByChannelHandler(DFSDM_Channel_HandleTypeDef* hdfsdm_channel)
{
    for (auto map : *handlerMapping) {
        if (map.hdfsdmChannel == hdfsdm_channel) {
            return map.instance;
        }
    }

    return nullptr;
}

MP34DT01* findInstanceByFilterHandler(DFSDM_Filter_HandleTypeDef* hdfsdm_filter)
{
    for (auto map : *handlerMapping) {
        if (map.hdfsdmFilter == hdfsdm_filter) {
            return map.instance;
        }
    }

    return nullptr;
}

void MSP_ChannelInit_callback(DFSDM_Channel_HandleTypeDef* hdfsdm_channel)
{
    auto instance = findInstanceByChannelHandler(hdfsdm_channel);
    if (instance != nullptr) instance->mspChannelInit(hdfsdm_channel);
}

void MSP_ChannelDeinit_callback(DFSDM_Channel_HandleTypeDef* hdfsdm_channel)
{
    auto instance = findInstanceByChannelHandler(hdfsdm_channel);
    if (instance != nullptr) instance->mspChannelDeinit(hdfsdm_channel);
}

void MSP_FilterInit_callback(DFSDM_Filter_HandleTypeDef* hdfsdm_filter)
{
    auto instance = findInstanceByFilterHandler(hdfsdm_filter);
    if (instance != nullptr) instance->mspFilterInit(hdfsdm_filter);
}

void MSP_FilterDeinit_callback(DFSDM_Filter_HandleTypeDef* hdfsdm_filter)
{
    auto instance = findInstanceByFilterHandler(hdfsdm_filter);
    if (instance != nullptr) instance->mspFilterDeinit(hdfsdm_filter);
}

void MSP_FilterRegConvHalfCplt_callback(DFSDM_Filter_HandleTypeDef* hdfsdm_filter)
{
    auto instance = findInstanceByFilterHandler(hdfsdm_filter);
    if (instance != nullptr) instance->mspFilterConversionHalfComplete(hdfsdm_filter);
}

void MSP_FilterRegConvCplt_callback(DFSDM_Filter_HandleTypeDef* hdfsdm_filter)
{
    auto instance = findInstanceByFilterHandler(hdfsdm_filter);
    if (instance != nullptr) instance->mspFilterConversionComplete(hdfsdm_filter);
}

// ============================================================
// ===  DMA INTERRUPT HANDLER ==============================
// ======================================================

#ifdef __cplusplus
extern "C" {
#endif

void DMA1_Channel4_IRQHandler(void)
{
    for (auto map : *handlerMapping) {
        HAL_DMA_IRQHandler(map.dma);
    }
}

#ifdef __cplusplus
}
#endif
