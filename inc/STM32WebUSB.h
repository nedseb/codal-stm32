#ifndef __STM32USBSERIAL_H__
#define __STM32USBSERIAL_H__

#ifdef USBCON

#include <cstdint>

#include "STM32Pin.h"
#include "Serial.h"

namespace codal {
class STM32WebUSB : public Serial {
  public:
    STM32WebUSB();
    ~STM32WebUSB() {}

    int init() final override;
    int putc(char c) final override;
    int getc() final override { return this->getChar(ASYNC); }

    bool isConnected();

  protected:
    int enableInterrupt(SerialInterruptType t);
    int disableInterrupt(SerialInterruptType t) final override { return DEVICE_OK; }
    int setBaudrate(uint32_t baudrate) final override { return DEVICE_OK; }
    int configurePins(Pin& tx, Pin& rx) final override { return DEVICE_OK; }

  private:
    STM32Pin nonePin;
    static STM32WebUSB* instance;

    static void receivedCallback();
};
}  // namespace codal

#endif /* USBCON */

#endif