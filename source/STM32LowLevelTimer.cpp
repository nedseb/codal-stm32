#include "STM32LowLevelTimer.h"

#include "CodalDmesg.h"

/* Private Defines */
#define PIN_NOT_USED 0xFF
#define MAX_RELOAD   ((1 << 16) - 1)  // Currently even 32b timers are used as 16b to have generic behavior

using namespace codal;

STM32LowLevelTimer* instances[TIMER_NUM] = {nullptr};

void codal::timer_irq_handler(uint8_t index)
{
    if (instances[index] == NULL) return;

    uint16_t channel_bitmsk      = 0;

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

extern "C" void TIM1_IRQHandler()
{
    timer_irq_handler(0);
}
extern "C" void TIM2_IRQHandler()
{
    timer_irq_handler(1);
}
extern "C" void TIM3_IRQHandler()
{
    timer_irq_handler(2);
}
extern "C" void TIM4_IRQHandler()
{
    timer_irq_handler(3);
}
extern "C" void TIM5_IRQHandler()
{
    timer_irq_handler(4);
}

STM32LowLevelTimer::STM32LowLevelTimer(TIM_TypeDef* timer, uint8_t irqn) : LowLevelTimer(4)
{
    uint32_t index = get_timer_index(timer);
    if (index == UNKNOWN_TIMER) {
        Error_Handler();
    }
    this->timer_instance = timer;
    this->irqN           = irqn;

    memset(&TimHandle, 0, sizeof(TIM_HandleTypeDef));
    TimHandle.Instance = timer;
    TimHandle.Channel  = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
    TimHandle.hdma[0]  = NULL;
    TimHandle.hdma[1]  = NULL;
    TimHandle.hdma[2]  = NULL;
    TimHandle.hdma[3]  = NULL;
    TimHandle.hdma[4]  = NULL;
    TimHandle.hdma[5]  = NULL;
    TimHandle.hdma[6]  = NULL;
    TimHandle.Lock     = HAL_UNLOCKED;
    TimHandle.State    = HAL_TIM_STATE_RESET;

    enableTimerClock(&TimHandle);

    /* Configure timer with some default values */
    TimHandle.Init.Prescaler     = (uint32_t)((SystemCoreClock / 1'000'000) - 1);
    TimHandle.Init.Period        = MAX_RELOAD;
    TimHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;
    TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
#if defined(TIM_RCR_REP)
    TimHandle.Init.RepetitionCounter = 0;
#endif
    TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    HAL_TIM_Base_Init(&TimHandle);

    // all timers run in at least 16 bit mode, so lets use it as a default.
    setBitMode(BitMode16);

    uint8_t instance_index    = get_timer_index(timer);

    instances[instance_index] = this;
}

int STM32LowLevelTimer::enable()
{
    NVIC_SetPriority((IRQn_Type)this->irqN, 2);
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

int STM32LowLevelTimer::setMode(TimerMode t)
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
    uint32_t elapsed = 0;

    elapsed          = __HAL_TIM_GET_COUNTER(&TimHandle);
    return elapsed;
}

int STM32LowLevelTimer::setClockSpeed(uint32_t speedKHz)
{
    // // 8000 khz
    // // TODO: Reconfigure clocks if resolution is greater than 8khz
    // if (speedKHz > 8000)
    //     return DEVICE_INVALID_PARAMETER;

    // // clock is 8khz
    // uint32_t clockSpeed = 8000;
    // uint8_t prescaleValue = 0;

    // // snap to the lowest
    // for (prescaleValue = 0; prescaleValue < PRESCALE_VALUE_MAX; prescaleValue++)
    // {
    //     if (speedKHz < (clockSpeed / prescalerDivison[prescaleValue]))
    //         continue;

    //     break;
    // }

    // tc->COUNT8.CTRLA.bit.PRESCALER = prescaleValue;

    return DEVICE_OK;
}

int STM32LowLevelTimer::setBitMode(TimerBitMode t)
{
    // disable();
    // if (HAL_TIM_OC_DeInit(&TimHandle) != HAL_OK)
    //     CODAL_ASSERT(0);

    // switch (t)
    // {
    //     case BitMode8:
    //         TimHandle.Init.Period = 0xFF;
    //         break;
    //     case BitMode16:
    //         TimHandle.Init.Period = 0xFFFF;
    //         break;
    //     case BitMode24:
    //         TimHandle.Init.Period = 0xFFFFFF;
    //         break;
    //     case BitMode32:
    //         TimHandle.Init.Period = 0xFFFFFFFF;
    //         break;
    // }

    // // TimHandle.Init.Prescaler = (uint32_t)((SystemCoreClock / 1000000) - 1);
    // // TimHandle.Init.ClockDivision = 0;
    // // TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;

    // if (HAL_TIM_OC_Init(&TimHandle) != HAL_OK)
    //     CODAL_ASSERT(0);

    // enable();

    this->bitMode = t;
    return DEVICE_OK;
}

/**
 * @brief  return timer index from timer handle
 * @param  htim : one of the defined timer
 * @retval timer index
 */
timer_index_t codal::get_timer_index(TIM_TypeDef* instance)
{
    timer_index_t index = UNKNOWN_TIMER;

#if defined(TIM1_BASE)
    if (instance == TIM1) {
        index = TIMER1_INDEX;
    }
#endif
#if defined(TIM2_BASE)
    if (instance == TIM2) {
        index = TIMER2_INDEX;
    }
#endif
#if defined(TIM3_BASE)
    if (instance == TIM3) {
        index = TIMER3_INDEX;
    }
#endif
#if defined(TIM4_BASE)
    if (instance == TIM4) {
        index = TIMER4_INDEX;
    }
#endif
#if defined(TIM5_BASE)
    if (instance == TIM5) {
        index = TIMER5_INDEX;
    }
#endif
#if defined(TIM6_BASE)
    if (instance == TIM6) {
        index = TIMER6_INDEX;
    }
#endif
#if defined(TIM7_BASE)
    if (instance == TIM7) {
        index = TIMER7_INDEX;
    }
#endif
#if defined(TIM8_BASE)
    if (instance == TIM8) {
        index = TIMER8_INDEX;
    }
#endif
#if defined(TIM9_BASE)
    if (instance == TIM9) {
        index = TIMER9_INDEX;
    }
#endif
#if defined(TIM10_BASE)
    if (instance == TIM10) {
        index = TIMER10_INDEX;
    }
#endif
#if defined(TIM11_BASE)
    if (instance == TIM11) {
        index = TIMER11_INDEX;
    }
#endif
#if defined(TIM12_BASE)
    if (instance == TIM12) {
        index = TIMER12_INDEX;
    }
#endif
#if defined(TIM13_BASE)
    if (instance == TIM13) {
        index = TIMER13_INDEX;
    }
#endif
#if defined(TIM14_BASE)
    if (instance == TIM14) {
        index = TIMER14_INDEX;
    }
#endif
#if defined(TIM15_BASE)
    if (instance == TIM15) {
        index = TIMER15_INDEX;
    }
#endif
#if defined(TIM16_BASE)
    if (instance == TIM16) {
        index = TIMER16_INDEX;
    }
#endif
#if defined(TIM17_BASE)
    if (instance == TIM17) {
        index = TIMER17_INDEX;
    }
#endif
#if defined(TIM18_BASE)
    if (instance == TIM18) {
        index = TIMER18_INDEX;
    }
#endif
#if defined(TIM19_BASE)
    if (instance == TIM19) {
        index = TIMER19_INDEX;
    }
#endif
#if defined(TIM20_BASE)
    if (instance == TIM20) {
        index = TIMER20_INDEX;
    }
#endif
#if defined(TIM21_BASE)
    if (instance == TIM21) {
        index = TIMER21_INDEX;
    }
#endif
#if defined(TIM22_BASE)
    if (instance == TIM22) {
        index = TIMER22_INDEX;
    }
#endif
    return index;
}
