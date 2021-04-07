#include "STM32LowLevelTimer.h"

#include "CodalDmesg.h"

using codal::STM32LowLevelTimer;

STM32LowLevelTimer* instances[5] = {0};

void codal::timer_irq_handler(uint8_t index)
{
    if (instances[index] == NULL) return;

    uint16_t channel_bitmsk = 0;

    TIM_HandleTypeDef* timHandle = &instances[index]->TimHandle;

    if (__HAL_TIM_GET_IT_SOURCE(timHandle, TIM_FLAG_CC1) == SET) {
        channel_bitmsk |= (1 << 0);
        __HAL_TIM_CLEAR_IT(timHandle, TIM_IT_CC1);
    }

    if (__HAL_TIM_GET_IT_SOURCE(timHandle, TIM_FLAG_CC2) == SET) {
        channel_bitmsk |= (1 << 1);
        __HAL_TIM_CLEAR_IT(timHandle, TIM_IT_CC2);
    }

    if (__HAL_TIM_GET_IT_SOURCE(timHandle, TIM_FLAG_CC3) == SET) {
        channel_bitmsk |= (1 << 2);
        __HAL_TIM_CLEAR_IT(timHandle, TIM_IT_CC3);
    }

    if (__HAL_TIM_GET_IT_SOURCE(timHandle, TIM_FLAG_CC4) == SET) {
        channel_bitmsk |= (1 << 3);
        __HAL_TIM_CLEAR_IT(timHandle, TIM_IT_CC4);
    }

    instances[index]->timer_pointer(channel_bitmsk);
}

void enable_tim_clk(uint32_t tim);

STM32LowLevelTimer::~STM32LowLevelTimer() {}

STM32LowLevelTimer::STM32LowLevelTimer(TIM_TypeDef* timer, IRQn_Type irqn) : LowLevelTimer(4)
{
    enable_tim_clk((uint32_t)timer);

    this->timer_instance = timer;
    this->irqN           = irqn;
    memset(&TimHandle, 0, sizeof(TIM_HandleTypeDef));

    disableIRQ();  // otherwise it might hit us while we're initializing

    DMESG("SYS CLK: %d %d", SystemCoreClock, (uint32_t)((SystemCoreClock / 1000000)));

    TimHandle.Instance           = this->timer_instance;
    TimHandle.Init.Period        = 0xFFFFFFFF;
    TimHandle.Init.Prescaler     = (uint32_t)((SystemCoreClock / 1000000) - 1);
    TimHandle.Init.ClockDivision = 0;
    TimHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;
    HAL_TIM_OC_Init(&TimHandle);

    // all timers run in at least 16 bit mode, so lets use it as a default.
    setBitMode(BitMode16);

    uint8_t instance_index = 0;

    if (timer == TIM2) instance_index = 1;
#ifdef TIM3
    if (timer == TIM3) instance_index = 2;
#endif
#ifdef TIM4
    if (timer == TIM4) instance_index = 3;
#endif
#ifdef TIM5
    if (timer == TIM5) instance_index = 4;
#endif

    instances[instance_index] = this;

    setIRQPriority(2);
}

int STM32LowLevelTimer::setIRQPriority(int priority)
{
    NVIC_SetPriority((IRQn_Type)this->irqN, priority);
    return DEVICE_OK;
}

int STM32LowLevelTimer::enable()
{
    NVIC_ClearPendingIRQ((IRQn_Type)this->irqN);
    enableIRQ();
    HAL_TIM_OC_Start_IT(&TimHandle, TIM_CHANNEL_1);
    status |= STM_LOW_LEVEL_TIMER_STATUS_ENABLED;
    return DEVICE_OK;
}

int STM32LowLevelTimer::enableIRQ()
{
    NVIC_EnableIRQ((IRQn_Type)this->irqN);
    return DEVICE_OK;
}

int STM32LowLevelTimer::disable()
{
    disableIRQ();
    HAL_TIM_OC_Stop_IT(&TimHandle, TIM_CHANNEL_1);
    // HAL_TIM_Base_Stop_IT(&TimHandle);
    status &= ~STM_LOW_LEVEL_TIMER_STATUS_ENABLED;
    return DEVICE_OK;
}

int STM32LowLevelTimer::disableIRQ()
{
    NVIC_DisableIRQ((IRQn_Type)this->irqN);
    return DEVICE_OK;
}

int STM32LowLevelTimer::reset()
{
    disableIRQ();
    __HAL_TIM_SET_COUNTER(&TimHandle, 0);
    enableIRQ();
    return DEVICE_OK;
}

int STM32LowLevelTimer::setMode(codal::TimerMode t)
{
    // only support timer mode.
    return DEVICE_OK;
}

int STM32LowLevelTimer::setCompare(uint8_t channel, uint32_t value)
{
    if (channel > getChannelCount()) return DEVICE_INVALID_PARAMETER;

    uint32_t hal_channel = TIM_CHANNEL_1;
    uint32_t hal_int     = TIM_IT_CC1;

    if (channel == 1) {
        hal_channel = TIM_CHANNEL_2;
        hal_int     = TIM_IT_CC2;
    }

    if (channel == 2) {
        hal_channel = TIM_CHANNEL_3;
        hal_int     = TIM_IT_CC3;
    }

    if (channel == 3) {
        hal_channel = TIM_CHANNEL_4;
        hal_int     = TIM_IT_CC4;
    }

    __HAL_TIM_DISABLE_IT(&TimHandle, hal_int);
    __HAL_TIM_CLEAR_IT(&TimHandle, hal_int);
    __HAL_TIM_SET_COMPARE(&TimHandle, hal_channel, value);
    __HAL_TIM_ENABLE_IT(&TimHandle, hal_int);

    return DEVICE_OK;
}

int STM32LowLevelTimer::offsetCompare(uint8_t channel, uint32_t value)
{
    if (channel > getChannelCount()) return DEVICE_INVALID_PARAMETER;

    uint32_t hal_channel = TIM_CHANNEL_1;
    uint32_t hal_int     = TIM_IT_CC1;

    if (channel == 1) {
        hal_channel = TIM_CHANNEL_2;
        hal_int     = TIM_IT_CC2;
    }

    if (channel == 2) {
        hal_channel = TIM_CHANNEL_3;
        hal_int     = TIM_IT_CC3;
    }

    if (channel == 3) {
        hal_channel = TIM_CHANNEL_4;
        hal_int     = TIM_IT_CC4;
    }

    __HAL_TIM_DISABLE_IT(&TimHandle, hal_int);

    __HAL_TIM_SET_COMPARE(&TimHandle, hal_channel, (uint32_t)(__HAL_TIM_GET_COMPARE(&TimHandle, hal_channel) + value));
    __HAL_TIM_ENABLE_IT(&TimHandle, hal_int);

    return DEVICE_OK;
}

int STM32LowLevelTimer::clearCompare(uint8_t channel)
{
    if (channel > getChannelCount()) return DEVICE_INVALID_PARAMETER;

    uint32_t hal_int = TIM_IT_CC1;

    if (channel == 1) hal_int = TIM_IT_CC2;

    if (channel == 2) hal_int = TIM_IT_CC3;

    if (channel == 3) hal_int = TIM_IT_CC4;

    __HAL_TIM_DISABLE_IT(&TimHandle, hal_int);

    return DEVICE_OK;
}

uint32_t STM32LowLevelTimer::captureCounter()
{
    return __HAL_TIM_GET_COUNTER(&TimHandle);
}

int STM32LowLevelTimer::setClockSpeed(uint32_t speedKHz)
{
    return DEVICE_OK;
}

int STM32LowLevelTimer::setBitMode(codal::TimerBitMode t)
{
    this->bitMode = t;
    return DEVICE_OK;
}

void enable_tim_clk(uint32_t tim)
{
    // Enable TIM clock
#if defined(TIM1_BASE)
    if (tim == TIM1_BASE) {
        __HAL_RCC_TIM1_CLK_ENABLE();
    }
#endif
#if defined(TIM2_BASE)
    if (tim == TIM2_BASE) {
        __HAL_RCC_TIM2_CLK_ENABLE();
    }
#endif
#if defined(TIM3_BASE)
    if (tim == TIM3_BASE) {
        __HAL_RCC_TIM3_CLK_ENABLE();
    }
#endif
#if defined(TIM4_BASE)
    if (tim == TIM4_BASE) {
        __HAL_RCC_TIM4_CLK_ENABLE();
    }
#endif
#if defined(TIM5_BASE)
    if (tim == TIM5_BASE) {
        __HAL_RCC_TIM5_CLK_ENABLE();
    }
#endif
#if defined(TIM8_BASE)
    if (tim == TIM8_BASE) {
        __HAL_RCC_TIM8_CLK_ENABLE();
    }
#endif
#if defined(TIM9_BASE)
    if (tim == TIM9_BASE) {
        __HAL_RCC_TIM9_CLK_ENABLE();
    }
#endif
#if defined(TIM10_BASE)
    if (tim == TIM10_BASE) {
        __HAL_RCC_TIM10_CLK_ENABLE();
    }
#endif
#if defined(TIM11_BASE)
    if (tim == TIM11_BASE) {
        __HAL_RCC_TIM11_CLK_ENABLE();
    }
#endif
#if defined(TIM12_BASE)
    if (tim == TIM12_BASE) {
        __HAL_RCC_TIM12_CLK_ENABLE();
    }
#endif
#if defined(TIM13_BASE)
    if (tim == TIM13_BASE) {
        __HAL_RCC_TIM13_CLK_ENABLE();
    }
#endif
#if defined(TIM14_BASE)
    if (tim == TIM14_BASE) {
        __HAL_RCC_TIM14_CLK_ENABLE();
    }
#endif
#if defined(TIM15_BASE)
    if (tim == TIM15_BASE) {
        __HAL_RCC_TIM15_CLK_ENABLE();
    }
#endif
#if defined(TIM16_BASE)
    if (tim == TIM16_BASE) {
        __HAL_RCC_TIM16_CLK_ENABLE();
    }
#endif
#if defined(TIM17_BASE)
    if (tim == TIM17_BASE) {
        __HAL_RCC_TIM17_CLK_ENABLE();
    }
#endif
#if defined(TIM18_BASE)
    if (tim == TIM18_BASE) {
        __HAL_RCC_TIM18_CLK_ENABLE();
    }
#endif
#if defined(TIM19_BASE)
    if (tim == TIM19_BASE) {
        __HAL_RCC_TIM19_CLK_ENABLE();
    }
#endif
#if defined(TIM20_BASE)
    if (tim == TIM20_BASE) {
        __HAL_RCC_TIM20_CLK_ENABLE();
    }
#endif
#if defined(TIM21_BASE)
    if (tim == TIM21_BASE) {
        __HAL_RCC_TIM21_CLK_ENABLE();
    }
#endif
#if defined(TIM22_BASE)
    if (tim == TIM22_BASE) {
        __HAL_RCC_TIM22_CLK_ENABLE();
    }
#endif
}
