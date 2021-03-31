#include "timer.h"
#include "stm32yyxx_ll_tim.h"
#include "LowLevelTimer.h"

#ifndef STM32_LOW_LEVEL_TIMER_H
#define STM32_LOW_LEVEL_TIMER_H

#define STM_LOW_LEVEL_TIMER_STATUS_ENABLED 0x02

namespace codal {
class STM32LowLevelTimer : public LowLevelTimer {
  private:
    IRQn_Type irqN;
    TIM_TypeDef* timer_instance;
    TIM_HandleTypeDef TimHandle;

  public:

    STM32LowLevelTimer(TIM_TypeDef* timer, IRQn_Type irqn);
    
    virtual ~STM32LowLevelTimer();

    virtual int setIRQPriority(int) override final;

    virtual int enable() override final;

    virtual int enableIRQ() override final;

    virtual int disable() override final;

    virtual int disableIRQ() override final;

    virtual int reset() override final;

    virtual int setMode(TimerMode t) override final;

    virtual int setCompare(uint8_t channel, uint32_t value) override final;

    virtual int offsetCompare(uint8_t channel, uint32_t value) override final;

    virtual int clearCompare(uint8_t channel) override final;

    virtual uint32_t captureCounter() override final;

    virtual int setClockSpeed(uint32_t speedKHz) override final;

    virtual int setBitMode(TimerBitMode t) override final;
    
    friend void timer_irq_handler(uint8_t index);

};
void timer_irq_handler(uint8_t index);
} // namespace codal

#endif
