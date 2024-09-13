#pragma once

#include "CodalFiber.h"
#include "HCI.h"
#include "STM32Pin.h"
#include "STM32SPI.h"
#include "clock.h"
#include "codal_target_hal.h"

class HCI_SPI : public HCI {
  public:
    HCI_SPI(codal::STM32SPI& spi, codal::STM32Pin& cs, codal::STM32Pin& irq, codal::STM32Pin& rst,
            uint32_t freq = 1000000);

    virtual ~HCI_SPI() {}

    virtual bool init() override final;
    virtual void resetHardware() override final;
    void poll();

  protected:
    virtual std::vector<uint8_t> sendCommand(OpCodeCommand command, uint8_t nbARgs, const uint8_t* args) override final;
    uint8_t available();
    uint8_t readByte();
    void readBytes(uint8_t* data, uint8_t size);
    bool writeBytes(uint8_t* data, uint8_t size);

    bool aciEnableLLOnly();
    bool aciGattInit();
    bool aciGapInit();

  private:
    codal::STM32SPI& spi;
    codal::STM32Pin& cs;
    codal::STM32Pin& irq;
    codal::STM32Pin& rst;

    void enableCS()
    {
        cs.setDigitalValue(0);
        target_wait_us(100);
    }
    void disableCS()
    {
        cs.setDigitalValue(1);
        target_wait_us(100);
    }
    void waitForInit();
};