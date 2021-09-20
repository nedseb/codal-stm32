#include "VL53L0X.h"

using namespace codal;

VL53L0X::VL53L0X(STM32I2C* i2c, STM32Pin* shutPin, uint8_t address)
    : baseDriver(i2c, shutPin, address), address(address)
{
}

void VL53L0X::init()
{
    baseDriver.InitSensor(address);
}

void VL53L0X::enable()
{
    baseDriver.VL53L0X_On();
}

void VL53L0X::disable()
{
    baseDriver.VL53L0X_Off();
}

uint32_t VL53L0X::getDistance()
{
    uint32_t distance;
    baseDriver.GetDistance(&distance);
    return distance;
}