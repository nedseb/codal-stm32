#include "STM32SingleWireSerial.h"

using namespace std;
using namespace codal;

STM32SingleWireSerial::STM32SingleWireSerial(STM32Pin& pin, uint16_t bufferSize, uint16_t id)
    : SingleWireSerial{pin, id},
      pin{pin},
      nc_pin{0, PinName::NC, PinCapability::PIN_CAPABILITY_ALL},
      serial{new STM32Serial(pin, nc_pin)}
{
}

void STM32SingleWireSerial::init(uint32_t baudrate, LengthSerial databits, ParitySerial parity, StopBitSerial stopBit)
{
    serial->init(baudrate, databits, parity, stopBit);
}

int STM32SingleWireSerial::putc(char c)
{
    return serial->putc(c);
}

int STM32SingleWireSerial::getc()
{
    return serial->getc();
}

int STM32SingleWireSerial::send(uint8_t* buf, int len)
{
    return serial->send(buf, len);
}

int STM32SingleWireSerial::receive(uint8_t* buf, int len)
{
    return serial->read(buf, len);
}

int STM32SingleWireSerial::setBaud(uint32_t baud)
{
    return serial->setBaud(baud);
}

uint32_t STM32SingleWireSerial::getBaud()
{
    return serial->getBaud();
}

int STM32SingleWireSerial::getBytesReceived()
{
    return serial->rxBufferedSize();
}

int STM32SingleWireSerial::getBytesTransmitted()
{
    return serial->txBufferedSize();
}

int STM32SingleWireSerial::sendBreak()
{
    return 0;
}

void STM32SingleWireSerial::configureRxInterrupt(int enable)
{
    return;
}

int STM32SingleWireSerial::configureTx(int enable)
{
    if (enable) {
        serial->enableHalfDuplexTransmitter();
        return 1;
    }

    return 0;
}

int STM32SingleWireSerial::configureRx(int enable)
{
    if (enable) {
        serial->enableHalfDuplexReceiver();
        return 1;
    }

    return 0;
}
