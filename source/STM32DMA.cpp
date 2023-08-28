#include "STM32DMA.h"

#if defined(STM32WBxx)

#include <cstdio>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace codal;

struct DMARegister {
    DMA_HandleTypeDef* hdma;
    STM32DMA* instance;
};

vector<DMARegister*> DMAInstances;
unordered_map<IRQn_Type, uint8_t> IRQnUsageCounter;

STM32DMA::STM32DMA() : hdma(new DMA_HandleTypeDef()), initialized(false)
{
    DMAInstances.push_back(new DMARegister{hdma : hdma, instance : this});
}

STM32DMA::~STM32DMA()
{
    delete hdma;

    for (size_t i = 0; i < DMAInstances.size(); ++i) {
        if (DMAInstances[i]->instance == this) {
            DMAInstances.erase(DMAInstances.begin() + i);
        }
    }
}

bool STM32DMA::init(DMA_Channel_TypeDef* instance, DMA_Request request, DMA_Direction direction,
                    DMA_PeripheralIncrementMode peripheralIncrement, DMA_MemoryIncrementMode memoryIncrement,
                    DMA_PeripheralDataSize peripheralDataAlingment, DMA_MemoryDataSize memoryDataAlingment,
                    DMA_Mode mode, DMA_Priority priority)
{
    if (initialized) {
        return false;
    }

#if defined(DMA2)
    if ((uint32_t)(instance) >= (uint32_t)(DMA2_Channel1)) {
        __HAL_RCC_DMA2_CLK_ENABLE();
    }
    else {
        __HAL_RCC_DMA1_CLK_ENABLE();
    }
#else
    __HAL_RCC_DMA1_CLK_ENABLE();
#endif

    __HAL_RCC_DMAMUX1_CLK_ENABLE();

    hdma->Instance                 = instance;
    hdma->Init.Request             = uint32_t(request);
    hdma->Init.Direction           = uint32_t(direction);
    hdma->Init.PeriphInc           = uint32_t(peripheralIncrement);
    hdma->Init.MemInc              = uint32_t(memoryIncrement);
    hdma->Init.PeriphDataAlignment = uint32_t(peripheralDataAlingment);
    hdma->Init.MemDataAlignment    = uint32_t(memoryDataAlingment);
    hdma->Init.Mode                = uint32_t(mode);
    hdma->Init.Priority            = uint32_t(priority);

    uint8_t result = HAL_DMA_Init(hdma);

    initialized = (result == HAL_OK);

    irqn = getIRQn(instance);

    printf("DMA Init: 0x%02X\r\n", result);

    return initialized;
}

void STM32DMA::deinit()
{
    initialized = false;
    HAL_DMA_DeInit(hdma);
    disableIRQ();
}

void STM32DMA::enableIRQ(uint32_t intPriority, uint32_t subPriority)
{
    if (irqn == HardFault_IRQn) return;

    HAL_NVIC_SetPriority(irqn, 0, 0);
    HAL_NVIC_EnableIRQ(irqn);

    if (IRQnUsageCounter.count(irqn) == 1) {
        IRQnUsageCounter[irqn]++;
    }
    else {
        IRQnUsageCounter.insert({irqn, 1});
    }
}

void STM32DMA::disableIRQ()
{
    if (irqn == HardFault_IRQn) return;

    if (IRQnUsageCounter.count(irqn) == 1) {
        IRQnUsageCounter[irqn]--;

        if (IRQnUsageCounter[irqn] == 0) {
            HAL_NVIC_DisableIRQ(irqn);
        }
    }
}

IRQn_Type STM32DMA::getIRQn(DMA_Channel_TypeDef* channel)
{
    if (channel == DMA1_Channel1) {
        return DMA1_Channel1_IRQn;
    }
    else if (channel == DMA1_Channel2) {
        return DMA1_Channel2_IRQn;
    }
#if defined(DMA1_Channel3)
    else if (channel == DMA1_Channel3) {
        return DMA1_Channel3_IRQn;
    }
#endif
#if defined(DMA1_Channel4)
    else if (channel == DMA1_Channel4) {
        return DMA1_Channel4_IRQn;
    }
#endif
#if defined(DMA1_Channel5)
    else if (channel == DMA1_Channel5) {
        return DMA1_Channel5_IRQn;
    }
#endif
#if defined(DMA1_Channel6)
    else if (channel == DMA1_Channel6) {
        return DMA1_Channel6_IRQn;
    }
#endif
#if defined(DMA1_Channel7)
    else if (channel == DMA1_Channel7) {
        return DMA1_Channel7_IRQn;
    }
#endif
#if defined(DMA2)
    if (channel == DMA2_Channel1) {
        return DMA2_Channel1_IRQn;
    }
    else if (channel == DMA2_Channel2) {
        return DMA2_Channel2_IRQn;
    }
#if defined(DMA2_Channel3)
    else if (channel == DMA2_Channel3) {
        return DMA2_Channel3_IRQn;
    }
#endif
#if defined(DMA2_Channel4)
    else if (channel == DMA2_Channel4) {
        return DMA2_Channel4_IRQn;
    }
#endif
#if defined(DMA2_Channel5)
    else if (channel == DMA2_Channel5) {
        return DMA2_Channel5_IRQn;
    }
#endif
#if defined(DMA2_Channel6)
    else if (channel == DMA2_Channel6) {
        return DMA2_Channel6_IRQn;
    }
#endif
#if defined(DMA2_Channel7)
    else if (channel == DMA2_Channel7) {
        return DMA2_Channel7_IRQn;
    }
#endif
#endif

    return HardFault_IRQn;
}

void callIRQHandlerDMAInstances()
{
    for (auto inst : DMAInstances) {
        if (inst->instance->isInitialized()) {
            HAL_DMA_IRQHandler(inst->hdma);
        }
    }
}

#ifdef __cplusplus
extern "C" {
#endif

void DMA1_Channel1_IRQHandler(void)
{
    callIRQHandlerDMAInstances();
}

void DMA1_Channel2_IRQHandler(void)
{
    callIRQHandlerDMAInstances();
}

void DMA1_Channel3_IRQHandler(void)
{
    callIRQHandlerDMAInstances();
}

void DMA1_Channel4_IRQHandler(void)
{
    callIRQHandlerDMAInstances();
}

void DMA1_Channel5_IRQHandler(void)
{
    callIRQHandlerDMAInstances();
}

void DMA1_Channel6_IRQHandler(void)
{
    callIRQHandlerDMAInstances();
}

void DMA1_Channel7_IRQHandler(void)
{
    callIRQHandlerDMAInstances();
}

#if defined(DMA2)

void DMA2_Channel1_IRQHandler(void)
{
    callIRQHandlerDMAInstances();
}

void DMA2_Channel2_IRQHandler(void)
{
    callIRQHandlerDMAInstances();
}

void DMA2_Channel3_IRQHandler(void)
{
    callIRQHandlerDMAInstances();
}

void DMA2_Channel4_IRQHandler(void)
{
    callIRQHandlerDMAInstances();
}

void DMA2_Channel5_IRQHandler(void)
{
    callIRQHandlerDMAInstances();
}

#endif

#ifdef __cplusplus
}
#endif

#endif