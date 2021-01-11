#include "HardwareTimer.h"
#include "LowLevelTimer.h"

#ifndef STM32_LOW_LEVEL_TIMER_H
#define STM32_LOW_LEVEL_TIMER_H

#define STM_LOW_LEVEL_TIMER_STATUS_ENABLED 0x02

namespace codal {

class STM32LowLevelTimer : public LowLevelTimer {
  private:
    IRQn_Type irqN;
    HardwareTimer hardwareTimer;
    TimerMode mode;

  public:
    STM32LowLevelTimer(TIM_TypeDef* timer, IRQn_Type irqn);
    virtual ~STM32LowLevelTimer();

    int setIRQPriority(int) final override;
    int setIRQPriority(uint32_t preemptPriority, uint32_t subPriority);

    int enable() final override;
    int enableIRQ() final override;
    int disable() final override;
    int disableIRQ() final override;
    int reset() final override;
    int setMode(TimerMode t) final override;
    int setCompare(uint8_t channel, uint32_t value) final override;
    int offsetCompare(uint8_t channel, uint32_t value) final override;
    int clearCompare(uint8_t channel) final override;
    uint32_t captureCounter() final override;
    int setClockSpeed(uint32_t speedKHz) final override;
    int setBitMode(TimerBitMode t) final override;
    friend void timer_irq_handler(uint8_t index);
};

void timer_irq_handler(uint8_t index);
}  // namespace codal

#endif