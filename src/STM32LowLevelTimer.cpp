#include "STM32LowLevelTimer.h"
using namespace codal;

uint32_t getCaptureCompareIT(uint8_t channel) {
    uint32_t hal_int = TIM_IT_CC1;

    if (channel == 1)
        hal_int = TIM_IT_CC2;

    if (channel == 2)
        hal_int = TIM_IT_CC3;

    if (channel == 3)
        hal_int = TIM_IT_CC4;

    return hal_int;
}

STM32LowLevelTimer::STM32LowLevelTimer(TIM_TypeDef* timerInstance, uint8_t irqN)
    : LowLevelTimer(TIMER_CHANNELS),
      irqN(irqN),
      hardwareTimer(timerInstance),
      mode(TimerModeTimer) {
}

int STM32LowLevelTimer::setIRQPriority(uint32_t preemptPriority, uint32_t subPriority) {
    hardwareTimer.setInterruptPriority(preemptPriority, subPriority);
    return DEVICE_OK;
}

int STM32LowLevelTimer::setIRQPriority(int priority) {
    uint32_t preemptPriority;
    uint32_t subPriority;
    uint32_t priorityGroup = NVIC_GetPriorityGrouping();
    NVIC_DecodePriority(priority, priorityGroup, &preemptPriority, &subPriority);
    return setIRQPriority(preemptPriority, subPriority);
}

int STM32LowLevelTimer::enable() {
    hardwareTimer.resume();
    status |= STM_LOW_LEVEL_TIMER_STATUS_ENABLED;
    return DEVICE_OK;
}

int STM32LowLevelTimer::enableIRQ() {
    NVIC_EnableIRQ((IRQn_Type)this->irqN);
    return DEVICE_OK;
}

int STM32LowLevelTimer::disable() {
    hardwareTimer.pause();
    status &= ~STM_LOW_LEVEL_TIMER_STATUS_ENABLED;
    return DEVICE_OK;
}

int STM32LowLevelTimer::disableIRQ() {
    NVIC_DisableIRQ((IRQn_Type)this->irqN);
    return DEVICE_OK;
}

int STM32LowLevelTimer::reset() {
    hardwareTimer.setCount(0, TICK_FORMAT);
    return DEVICE_OK;
}

int STM32LowLevelTimer::setMode(TimerMode mode) {
    this->mode = mode;
    // only support timer mode.

    return DEVICE_OK;
}

int STM32LowLevelTimer::setCompare(uint8_t channel, uint32_t value) {
    if (channel > getChannelCount())
        return DEVICE_INVALID_PARAMETER;

    __HAL_TIM_DISABLE_IT(hardwareTimer.getHandle(), getCaptureCompareIT(channel));
    hardwareTimer.setCaptureCompare(channel+1, value);
    __HAL_TIM_ENABLE_IT(hardwareTimer.getHandle(), getCaptureCompareIT(channel));
    return DEVICE_OK;
}

int STM32LowLevelTimer::offsetCompare(uint8_t channel, uint32_t value) {
    if (channel > getChannelCount())
        return DEVICE_INVALID_PARAMETER;

    __HAL_TIM_DISABLE_IT(hardwareTimer.getHandle(), getCaptureCompareIT(channel));
    hardwareTimer.setCaptureCompare(channel+1, hardwareTimer.getCaptureCompare(channel) + value);
    __HAL_TIM_ENABLE_IT(hardwareTimer.getHandle(), getCaptureCompareIT(channel));

    return DEVICE_OK;
}

int STM32LowLevelTimer::clearCompare(uint8_t channel) {
    if (channel > getChannelCount())
        return DEVICE_INVALID_PARAMETER;

    __HAL_TIM_DISABLE_IT(hardwareTimer.getHandle(), getCaptureCompareIT(channel));

    return DEVICE_OK;
}

uint32_t STM32LowLevelTimer::captureCounter() {
    return hardwareTimer.getCount();
}

int STM32LowLevelTimer::setClockSpeed(uint32_t speedKHz) {
    return DEVICE_OK;
}

int STM32LowLevelTimer::setBitMode(TimerBitMode bitMode) {
    this->bitMode = bitMode;
    return DEVICE_OK;
}
