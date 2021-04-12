#ifndef __STM32WEBUSB_H__
#define __STM32WEBUSB_H__

#ifdef USBCON

#include "CodalComponent.h"

namespace codal {
class STM32WebUSB : public CodalComponent {
  public:
    STM32WebUSB();
    ~STM32WebUSB() {}

    int init();
    void deInit();
};
}  // namespace codal

#endif /* USBCON */

#endif /* __STM32WEBUSB_H__ */