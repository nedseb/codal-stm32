#pragma once

#include "STM32Pin.h"

namespace codal {

enum class HCSR04Unit : uint8_t { M = 0, Dm = 1, Cm = 2, Mm = 3 };
enum class HCSR04TimeUnit : uint8_t { S = 0, Ms = 1, Us = 2 };

class HCSR04 {
  public:
    HCSR04(STM32Pin& trig, STM32Pin& echo) : trig(trig), echo(echo) {};
    ~HCSR04() {};

    float getDistance(HCSR04Unit type);
    float getTime(HCSR04TimeUnit type);

  private:
    STM32Pin& trig;
    STM32Pin& echo;

    float getDistanceMilli();
    float getTimeMicroSeconds();
};

}  // namespace codal
