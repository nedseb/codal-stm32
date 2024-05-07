#ifndef __STM32SERIAL_H__
#define __STM32SERIAL_H__

#include <map>

#include "STM32Pin.h"
#include "Serial.h"
#include "uart.h"
// required for gcc-6 (but not 7!)
#undef putc
#undef getc

namespace codal {

enum class LengthSerial : uint32_t { SERIAL_8_BITS = UART_WORDLENGTH_8B, SERIAL_9_BITS = UART_WORDLENGTH_9B };

enum class ParitySerial : uint32_t {
    SERIAL_NONE_PARITY = UART_PARITY_NONE,
    SERIAL_ODD_PARITY  = UART_PARITY_ODD,
    SERIAL_EVEN_PARITY = UART_PARITY_EVEN
};

enum class StopBitSerial : uint32_t { SERIAL_STOP_1 = UART_STOPBITS_1, SERIAL_STOP_2 = UART_STOPBITS_2 };

class STM32Serial : public Serial {
  private:
    static std::map<serial_t*, STM32Serial*> mapSerialInstance;

    STM32Pin& rxPin;
    STM32Pin& txPin;
    LengthSerial databits;
    ParitySerial parity;
    StopBitSerial stopBit;

  protected:
    serial_t serial;
    uint32_t baudrate;

    int enableInterrupt(SerialInterruptType t) final override;
    int disableInterrupt(SerialInterruptType t) final override;
    int setBaudrate(uint32_t baudrate) final override;
    int configurePins(Pin& tx, Pin& rx) final override;

    static void RxIRQ(serial_t* obj);

  public:
    STM32Serial(STM32Pin& txPin, STM32Pin& rxPin);
    ~STM32Serial();

    void init(uint32_t baudrate, LengthSerial databits = LengthSerial::SERIAL_8_BITS,
              ParitySerial parity   = ParitySerial::SERIAL_NONE_PARITY,
              StopBitSerial stopBit = StopBitSerial::SERIAL_STOP_1);

    int putc(char c) final override;
    int getc() final override;

    uint32_t getBaud() { return baudrate; }

    void enableHalfDuplexTransmitter();
    void enableHalfDuplexReceiver();
};

extern STM32Serial* default_serial_debug;
}  // namespace codal

#endif
