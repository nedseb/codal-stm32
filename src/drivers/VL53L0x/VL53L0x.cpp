#include "VL53L0x.h"

using namespace codal;


VL53L0x::VL53L0x( STM32I2C& i2c, STM32Pin& shutPin, uint8_t address ) : baseDriver( i2c, shutPin, address ), address(address) {

}

void VL53L0x::init(){
    baseDriver.InitSensor(address);
}

void VL53L0x::enable(){
    baseDriver.VL53L0X_On();
}

void VL53L0x::disable(){
    baseDriver.VL53L0X_Off();
}

uint32_t VL53L0x::getDistance(){
    uint32_t distance;
    baseDriver.GetDistance(&distance);
    return distance;
}