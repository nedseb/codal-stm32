#include "VL53L1X.h"

using namespace codal;

VL53L1X::VL53L1X(STM32I2C* i2c, STM32Pin* shutPin, uint8_t address)
    : baseDriver(i2c, shutPin, address), address(address)
{
}

void VL53L1X::init()
{
    baseDriver.InitSensor(address);
    baseDriver.VL53L1X_SetDistanceMode(2);
    baseDriver.VL53L1X_SetTimingBudgetInMs(100);
    baseDriver.VL53L1X_SetInterMeasurementInMs(100);
    baseDriver.VL53L1X_StartRanging();
}

void VL53L1X::enable()
{
    baseDriver.VL53L1X_On();
}

void VL53L1X::disable()
{
    baseDriver.VL53L1X_Off();
}

uint32_t VL53L1X::getDistance()
{
    uint32_t distance;
    baseDriver.GetDistance(&distance);
    return distance;
}