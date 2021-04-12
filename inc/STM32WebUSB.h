#ifndef __STM32WEBUSB_H__
#define __STM32WEBUSB_H__

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

#endif /* __STM32WEBUSB_H__ */