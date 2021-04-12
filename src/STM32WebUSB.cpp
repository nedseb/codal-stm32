#include "STM32WebUSB.h"

#include "usbd_webusb_if.h"

using namespace codal;

STM32WebUSB::STM32WebUSB() : CodalComponent(DEVICE_ID_SERIAL, 0) {}

int STM32WebUSB::init()
{
    webUSB_init();

    return DEVICE_OK;
}

void STM32WebUSB::deInit()
{
    webUSB_deInit();
}
