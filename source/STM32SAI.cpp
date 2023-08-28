#include "STM32SAI.h"

#if defined(STM32WBxx)
#include "PortNames.h"

using namespace std;
using namespace codal;

struct SAIRegister {
    SAI_HandleTypeDef* hsai;
    STM32SAI* instance;
};

vector<SAIRegister*> SAI_Instances;
uint8_t initInstance = 0;

STM32SAI::STM32SAI(STM32Pin* data, STM32Pin* clock, uint8_t alternate, uint16_t bufferSize)
    : data{data},
      clock{clock},
      hsai{new SAI_HandleTypeDef()},
      dma{new STM32DMA()},
      alternate{alternate},
      bufferSize{bufferSize},
      buffer{new int32_t[bufferSize]},
      cbOnComplete{nullptr},
      cbOnError{nullptr}
{
    for (uint16_t i = 0; i < bufferSize; i++) {
        buffer[i] = 0x00000000;
    }

    hsai->Instance            = SAI1_Block_A;
    hsai->Init.AudioMode      = SAI_MODEMASTER_RX;
    hsai->Init.AudioFrequency = SAI_AUDIO_FREQUENCY_48K;
    hsai->Init.Synchro        = SAI_ASYNCHRONOUS;
    hsai->Init.SynchroExt     = SAI_SYNCEXT_DISABLE;
    hsai->Init.MckOutput      = SAI_MCK_OUTPUT_ENABLE;
    hsai->Init.MonoStereoMode = SAI_STEREOMODE;

    hsai->Init.PdmInit.Activation  = FunctionalState::ENABLE;
    hsai->Init.PdmInit.MicPairsNbr = 1;
    hsai->Init.PdmInit.ClockEnable = SAI_PDM_CLOCK1_ENABLE;

    SAI_Instances.push_back(new SAIRegister{hsai : hsai, instance : this});
}

STM32SAI::~STM32SAI()
{
    delete[] buffer;
    delete dma;

    for (size_t i = 0; i < SAI_Instances.size(); ++i) {
        if (SAI_Instances[i]->instance == this) {
            SAI_Instances.erase(SAI_Instances.begin() + i);
        }
    }
}

bool STM32SAI::init()
{
    disableClock();
    return HAL_SAI_InitProtocol(hsai, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_16BITEXTENDED, 2) == HAL_OK;
}

void STM32SAI::deinit()
{
    stopListening();
    HAL_SAI_DeInit(hsai);
    dma->deinit();
}

void STM32SAI::startListening()
{
    enableClock();
    HAL_SAI_Receive_DMA(hsai, (uint8_t*)buffer, bufferSize);
}

void STM32SAI::stopListening()
{
    disableClock();
    HAL_SAI_DMAStop(hsai);
}

void STM32SAI::pauseDMA()
{
    HAL_SAI_DMAPause(hsai);
}

void STM32SAI::resumeDMA()
{
    HAL_SAI_DMAResume(hsai);
}

void STM32SAI::onReceiveData(function<void(const int32_t*)> callback)
{
    cbOnComplete = callback;
}

void STM32SAI::onError(function<void(const uint32_t)> callback)
{
    cbOnError = callback;
}

void STM32SAI::mspInit()
{
    if (initInstance == 0) __HAL_RCC_SAI1_CLK_ENABLE();
    initInstance++;

    initGPIO();
    dma->init(DMA1_Channel4, DMA_Request::SAI_1_A, DMA_Direction::PERIPH_TO_MEMORY);

    hsai->hdmarx             = dma->getHandle();
    hsai->hdmatx             = dma->getHandle();
    dma->getHandle()->Parent = hsai;

    dma->enableIRQ(0, 0);
}

void STM32SAI::mspDeinit()
{
    if (initInstance == 1) {
        __HAL_RCC_SAI1_CLK_DISABLE();
        dma->disableIRQ();
    }
    initInstance--;

    deinit();
}

void STM32SAI::rxCpltCallback()
{
    pauseDMA();
    if (cbOnComplete != nullptr) {
        cbOnComplete(buffer);
    }
    resumeDMA();
}

void STM32SAI::errorCallback()
{
    pauseDMA();
    if (cbOnError != nullptr) {
        cbOnError(hsai->ErrorCode);
    }
    resumeDMA();
}

void STM32SAI::initGPIO()
{
    set_GPIO_Port_Clock(STM_PORT(data->name));
    set_GPIO_Port_Clock(STM_PORT(clock->name));

    GPIO_InitTypeDef gpioData;
    GPIO_InitTypeDef gpioClock;

    gpioData.Pin       = STM_GPIO_PIN(data->name);
    gpioData.Mode      = GPIO_MODE_AF_PP;
    gpioData.Pull      = GPIO_NOPULL;
    gpioData.Speed     = GPIO_SPEED_FREQ_LOW;
    gpioData.Alternate = alternate;

    gpioClock.Pin       = STM_GPIO_PIN(clock->name);
    gpioClock.Mode      = GPIO_MODE_AF_PP;
    gpioClock.Pull      = GPIO_NOPULL;
    gpioClock.Speed     = GPIO_SPEED_FREQ_LOW;
    gpioClock.Alternate = alternate;

    HAL_GPIO_Init(get_GPIO_Port(STM_PORT(data->name)), &gpioData);
    HAL_GPIO_Init(get_GPIO_Port(STM_PORT(clock->name)), &gpioClock);
}

void STM32SAI::enableClock()
{
    RCC_PeriphCLKInitTypeDef saiClk;

    saiClk.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
    saiClk.Sai1ClockSelection   = RCC_SAI1CLKSOURCE_PLLSAI1;

    saiClk.PLLSAI1.PLLN            = 6;
    saiClk.PLLSAI1.PLLP            = RCC_PLLP_DIV4;
    saiClk.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK;

    HAL_RCCEx_PeriphCLKConfig(&saiClk);
}

void STM32SAI::disableClock()
{
    __HAL_RCC_PLLCLKOUT_DISABLE(RCC_PLL_SAI1CLK);
}

STM32SAI* findRegisterBySAI(SAI_HandleTypeDef* hsai)
{
    for (size_t i = 0; i < SAI_Instances.size(); i++) {
        if (SAI_Instances[i]->hsai == hsai) {
            return SAI_Instances[i]->instance;
        }
    }

    return nullptr;
}

void HAL_SAI_MspInit(SAI_HandleTypeDef* obj)
{
    auto reg = findRegisterBySAI(obj);

    if (reg != nullptr) {
        reg->mspInit();
    }
}

void HAL_SAI_MspDeInit(SAI_HandleTypeDef* obj)
{
    auto reg = findRegisterBySAI(obj);

    if (reg != nullptr) {
        reg->mspInit();
    }
}

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef* obj) {}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef* obj) {}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef* obj)
{
    auto inst = findRegisterBySAI(obj);
    if (inst != nullptr) {
        inst->rxCpltCallback();
    }
}

void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef* obj) {}

void HAL_SAI_ErrorCallback(SAI_HandleTypeDef* obj)
{
    auto inst = findRegisterBySAI(obj);
    if (inst != nullptr) {
        inst->errorCallback();
    }
}
#endif