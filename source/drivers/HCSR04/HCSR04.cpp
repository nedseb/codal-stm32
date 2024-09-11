#include "HCSR04.h"

#include "STM32Pin.h"
#include "wiring_time.h"

using namespace codal;

float HCSR04::getDistance(HCSR04Unit type)
{
    switch (type) {
        case HCSR04Unit::Mm:
            return getDistanceMilli();
        case HCSR04Unit::Cm:
            return getDistanceMilli() / 10.0;
        case HCSR04Unit::Dm:
            return getDistanceMilli() / 100.0;
        case HCSR04Unit::M:
            return getDistanceMilli() / 1000.0;
        default:
            break;
    }
    return 0;
}

float HCSR04::getTime(HCSR04TimeUnit type)
{
    switch (type) {
        case HCSR04TimeUnit::Us:
            return getTimeMicroSeconds();
        case HCSR04TimeUnit::Ms:
            return getTimeMicroSeconds() / 1000.0;
        case HCSR04TimeUnit::S:
            return getTimeMicroSeconds() / 1000000.0;
        default:
            break;
    }
    return 0;
}

float HCSR04::getDistanceMilli()
{
    return (343000.0 * getTimeMicroSeconds() / 1000000.0) / 2.0;
}

float HCSR04::getTimeMicroSeconds()
{
    trig.setDigitalValue(1);
    delay(2);
    trig.setDigitalValue(0);

    int pulse     = 1;
    uint32_t tick = micros();
    uint64_t maxd = 200000;
    while (echo.getDigitalValue() != pulse) {
        if (micros() - tick > maxd) return 0;
    }
    uint32_t start = micros();
    while (echo.getDigitalValue() == pulse) {
        if (micros() - tick > maxd) return 0;
    }
    uint32_t end = micros();

    return static_cast<float>(end - start);
}
