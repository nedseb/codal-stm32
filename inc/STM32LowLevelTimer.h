#pragma once

#include "LowLevelTimer.h"
#include "codal-stm32/source/stm32/timer.h"
#include "stm32yyxx_hal_conf.h"

#define STM_LOW_LEVEL_TIMER_STATUS_ENABLED 0x02

namespace codal {

class STM32LowLevelTimer : public LowLevelTimer {
  private:
    uint8_t irqN;
    TIM_TypeDef* timer_instance;
    TIM_HandleTypeDef TimHandle;

  public:
    STM32LowLevelTimer(TIM_TypeDef* timer, uint8_t irqn);

    virtual int enable();

    virtual int enableIRQ();

    virtual int disable();

    virtual int disableIRQ();

    virtual int reset();

    virtual int setMode(TimerMode t);

    virtual int setCompare(uint8_t channel, uint32_t value);

    virtual int offsetCompare(uint8_t channel, uint32_t value);

    virtual int clearCompare(uint8_t channel);

    virtual uint32_t captureCounter();

    virtual int setClockSpeed(uint32_t speedKHz);

    virtual int setBitMode(TimerBitMode t);

    friend void timer_irq_handler(uint8_t index);
};

timer_index_t get_timer_index(TIM_TypeDef* instance);

void timer_irq_handler(uint8_t index);
}  // namespace codal
