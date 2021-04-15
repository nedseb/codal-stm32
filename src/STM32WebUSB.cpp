
#ifdef USBCON

#include "STM32WebUSB.h"

#include "PinNumber.h"
#include "usbd_webusb_if.h"

using namespace codal;

STM32WebUSB* STM32WebUSB::instance = nullptr;

STM32WebUSB::STM32WebUSB() : Serial(nonePin, nonePin), nonePin(0, PinNumber::NC, PIN_CAPABILITY_ALL)
{
    if (instance == nullptr) instance = this;
}

int STM32WebUSB::init()
{
    webUSB_init();
    webUSB_OnDataReceived(receivedCallback);

    return DEVICE_OK;
}

int STM32WebUSB::putc(char c)
{
    uint8_t value = c;

    CDC_TransmitQueue_Enqueue(&WebUSB_TransmitQueue, &value, 1);

    if (webUSB_connected()) {
        webUSB_continue_transmit();
    }

    return DEVICE_OK;
}

void STM32WebUSB::receivedCallback()
{
    int length = CDC_ReceiveQueue_ReadSize(&WebUSB_ReceiveQueue);

    for (int i = 0; i < length; ++i) {
        int byte = CDC_ReceiveQueue_Dequeue(&WebUSB_ReceiveQueue);
        webUSB_resume_receive();

        uint16_t pos = (unsigned int)(instance->rxBuffHead + 1) % instance->rxBuffSize;

        if (pos != instance->rxBuffTail) {
            instance->rxBuff[instance->rxBuffHead] = (uint8_t)byte;
            instance->rxBuffHead                   = pos;
        }
    }
}

int STM32WebUSB::enableInterrupt(SerialInterruptType t)
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

bool STM32WebUSB::isConnected()
{
    return webUSB_connected();
}

#endif /* USBCON */
