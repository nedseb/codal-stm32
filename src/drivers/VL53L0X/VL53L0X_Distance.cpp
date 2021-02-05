#include "VL53L0X_Distance.h"

using namespace codal;

VL53L0X_Distance::VL53L0X_Distance( uint8_t id, STM32I2C& i2c, STM32Pin& shutPin, uint8_t address, uint16_t sensitivity, uint16_t samplePeriod )
    : VL53L0X( i2c, shutPin, address ), Sensor( id, sensitivity, samplePeriod ), isInitialized(false)
{
    configure();
}

void VL53L0X_Distance::configure(){
    VL53L0X::init();
    
    isInitialized = true;
}