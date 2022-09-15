#include "VL53L0X.h"

using namespace codal;

VL53L0X::VL53L0X(STM32I2C* i2c, STM32Pin* shutPin, uint8_t address)
    : baseDriver(i2c, shutPin, address), address(address)
{
}

void VL53L0X::init()
{
    baseDriver.InitSensor(address);
    baseDriver.StartMeasurementSimplified(range_continuous_polling, NULL);
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
    VL53L0X_RangingMeasurementData_t pRangingMeasurementData;
    baseDriver.GetMeasurementSimplified(range_continuous_polling, &pRangingMeasurementData);
    return pRangingMeasurementData.RangeMilliMeter;
}