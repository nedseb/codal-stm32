
#ifdef USBCON

    #include "STM32USBSerial.h"

    #include "PinNumber.h"
    #include "usbd_cdc_if.h"

    // required for gcc-6 (but not 7!)
    #undef putc
    #undef getc

using namespace codal;

STM32USBSerial* STM32USBSerial::instance = nullptr;

STM32USBSerial::STM32USBSerial() : Serial(nonePin, nonePin), nonePin(0, PinNumber::NC, PIN_CAPABILITY_ALL)
{
    if (instance == nullptr) instance = this;
}

int STM32USBSerial::init()
{
    CDC_init();
    CDC_OnDataReceived(receivedCallback);

    return DEVICE_OK;
}

int STM32USBSerial::putc(char c)
{
    uint8_t value = c;

    CDC_TransmitQueue_Enqueue(&TransmitQueue, &value, 1);

    if (CDC_connected()) {
        CDC_continue_transmit();
    }

    return DEVICE_OK;
}

void STM32USBSerial::receivedCallback()
{
    int length = CDC_ReceiveQueue_ReadSize(&ReceiveQueue);

    for (int i = 0; i < length; ++i) {
        int byte = CDC_ReceiveQueue_Dequeue(&ReceiveQueue);
        CDC_resume_receive();

        uint16_t pos = (unsigned int)(instance->rxBuffHead + 1) % instance->rxBuffSize;

        if (pos != instance->rxBuffTail) {
            instance->rxBuff[instance->rxBuffHead] = (uint8_t)byte;
            instance->rxBuffHead                   = pos;
        }
    }
}

int STM32USBSerial::enableInterrupt(SerialInterruptType t)
{
    if (t == RxInterrupt) {
        return DEVICE_OK;
    }
    else if (t == TxInterrupt) {
        while (txBufferedSize() > 0) {
            dataTransmitted();
        }
    }

    return DEVICE_OK;
}

bool STM32USBSerial::isConnected()
{
    return CDC_connected();
}

#endif /* USBCON */
