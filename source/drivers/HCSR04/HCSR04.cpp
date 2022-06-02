#include "HCSR04.h"

#include "STM32Pin.h"
#include "wiring_time.h"

using namespace codal;

HCSR04::HCSR04(STM32Pin& trig, STM32Pin& echo) : trig(trig), echo(echo) {}

HCSR04::~HCSR04(){}

int HCSR04::getDistance(HCSR04Unit type) {
    switch (type)
    {
    case HCSR04Unit::Mm :
        return getDistanceMilli();
    case HCSR04Unit::Cm :
        return getDistanceMilli() / 10;
    case HCSR04Unit::Dm :
        return getDistanceMilli() / 100;
    case HCSR04Unit::M :
        return getDistanceMilli() / 1000;
    default:
        break;
    }
    return 0;
}

int HCSR04::getDistanceMilli() { 
    trig.setDigitalValue(1);
    delay(2);
    trig.setDigitalValue(0);

    int pulse = 1;
    uint32_t tick = micros();
    uint64_t maxd = 200000;
    while(echo.getDigitalValue() != pulse) {
        if (micros() - tick > maxd)
            return 0;
    }
    uint32_t start = micros();
    while(echo.getDigitalValue() == pulse) {
        if (micros() - tick > maxd)
            return 0;
    }
    uint32_t end = micros();

    // Return the distance in centimeter
    return (343000 * (end - start) / 1000000) / 2;
}

