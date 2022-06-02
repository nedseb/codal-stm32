#pragma once

#include "STM32Pin.h"


namespace codal {
using namespace codal;
enum class HCSR04Unit : uint8_t {M = 0, Dm = 1, Cm = 2, Mm = 3};


class HCSR04 {
  public:
    HCSR04(STM32Pin& trig, STM32Pin& echo);
    ~HCSR04();

    int getDistance(HCSR04Unit type);


  private:
    STM32Pin& trig;
    STM32Pin& echo;

    int getDistanceMilli();
};

}  // namespace codal
