#include "STM32WebUSB.h"

#include "usbd_webusb_if.h"

using namespace codal;



STM32WebUSB::STM32WebUSB() {
}


void STM32WebUSB::init(){
    webUSB_init();
}

void STM32WebUSB::deInit(){
    webUSB_deInit();
}

