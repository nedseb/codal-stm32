#include "STM32LowLevelTimer.h"
using namespace codal;

uint32_t getTIMChannel(uint8_t channel) {
    uint32_t timChannel = TIM_CHANNEL_1;

    if (channel == 1) timChannel = TIM_CHANNEL_2;

    if (channel == 2) timChannel = TIM_CHANNEL_3;

    if (channel == 3) timChannel = TIM_CHANNEL_4;

    return timChannel;
}

uint32_t getCaptureCompareIT(uint8_t channel) {
    uint32_t timCaptureCompareIt = TIM_IT_CC1;

    if (channel == 1) timCaptureCompareIt = TIM_IT_CC2;

    if (channel == 2) timCaptureCompareIt = TIM_IT_CC3;

    if (channel == 3) timCaptureCompareIt = TIM_IT_CC4;

    return timCaptureCompareIt;
}

STM32LowLevelTimer::~STM32LowLevelTimer() {}

STM32LowLevelTimer::STM32LowLevelTimer(TIM_TypeDef* timerInstance, IRQn_Type irqN)
    : LowLevelTimer(TIMER_CHANNELS), irqN(irqN), hardwareTimer(timerInstance), mode(TimerModeTimer) {}

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
    NVIC_ClearPendingIRQ(this->irqN);

    if (enableIRQ() == DEVICE_OK) {
        HAL_TIM_OC_Start_IT(hardwareTimer.getHandle(), TIM_CHANNEL_1);
        status |= STM_LOW_LEVEL_TIMER_STATUS_ENABLED;
    }

    return DEVICE_OK;
}

int STM32LowLevelTimer::enableIRQ() {
    if (timer_pointer == nullptr) return DEVICE_INVALID_PARAMETER;

    NVIC_EnableIRQ(this->irqN);

    return DEVICE_OK;
}

int STM32LowLevelTimer::disable() {
    if (disableIRQ() == DEVICE_OK) {
        HAL_TIM_OC_Stop_IT(hardwareTimer.getHandle(), TIM_CHANNEL_1);
        status &= ~STM_LOW_LEVEL_TIMER_STATUS_ENABLED;
    }

    return DEVICE_OK;
}

int STM32LowLevelTimer::disableIRQ() {
    if (timer_pointer == nullptr) return DEVICE_INVALID_PARAMETER;
    NVIC_DisableIRQ(this->irqN);
    return DEVICE_OK;
}

int STM32LowLevelTimer::reset() {
    disableIRQ();
    __HAL_TIM_SET_COUNTER(hardwareTimer.getHandle(), 0);
    enableIRQ();

    return DEVICE_OK;
}

int STM32LowLevelTimer::setMode(TimerMode mode) {
    this->mode = mode;
    switch (this->mode) {
        case TimerModeTimer:
            // only support timer mode.
            return DEVICE_OK;

        case TimerModeCounter:
            return DEVICE_NOT_SUPPORTED;

        case TimerModeAlternateFunction:
            return DEVICE_NOT_SUPPORTED;

        default:
            return DEVICE_NOT_SUPPORTED;
    }
}

int STM32LowLevelTimer::setCompare(uint8_t channel, uint32_t value) {
    if (channel > getChannelCount()) return DEVICE_INVALID_PARAMETER;

    __HAL_TIM_DISABLE_IT(hardwareTimer.getHandle(), getCaptureCompareIT(channel));
    __HAL_TIM_CLEAR_IT(hardwareTimer.getHandle(), getCaptureCompareIT(channel));
    __HAL_TIM_SET_COMPARE(hardwareTimer.getHandle(), getTIMChannel(channel), value);
    __HAL_TIM_ENABLE_IT(hardwareTimer.getHandle(), getCaptureCompareIT(channel));

    return DEVICE_OK;
}

int STM32LowLevelTimer::offsetCompare(uint8_t channel, uint32_t value) {
    if (channel > getChannelCount()) return DEVICE_INVALID_PARAMETER;

    __HAL_TIM_DISABLE_IT(hardwareTimer.getHandle(), getCaptureCompareIT(channel));
    auto newValue = __HAL_TIM_GET_COMPARE(hardwareTimer.getHandle(), getTIMChannel(channel)) + value;
    __HAL_TIM_SET_COMPARE(hardwareTimer.getHandle(), getTIMChannel(channel), newValue);
    __HAL_TIM_ENABLE_IT(hardwareTimer.getHandle(), getCaptureCompareIT(channel));

    return DEVICE_OK;
}

int STM32LowLevelTimer::clearCompare(uint8_t channel) {
    if (channel > getChannelCount()) return DEVICE_INVALID_PARAMETER;

    __HAL_TIM_DISABLE_IT(hardwareTimer.getHandle(), getCaptureCompareIT(channel));
    __HAL_TIM_CLEAR_IT(hardwareTimer.getHandle(), getCaptureCompareIT(channel));

    return DEVICE_OK;
}

uint32_t STM32LowLevelTimer::captureCounter() {
    return __HAL_TIM_GET_COUNTER(hardwareTimer.getHandle());
}

int STM32LowLevelTimer::setClockSpeed(uint32_t speedKHz) {
    return DEVICE_OK;
}

int STM32LowLevelTimer::setBitMode(TimerBitMode bitMode) {
    this->bitMode = bitMode;
    return DEVICE_OK;
}
