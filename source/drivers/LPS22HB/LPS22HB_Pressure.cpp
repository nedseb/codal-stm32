#include "LPS22HB_Pressure.h"

using namespace codal;

LPS22HB_Pressure::LPS22HB_Pressure(uint8_t id, STM32I2C* i2c, uint8_t address, uint16_t sensitivity,
                                   uint16_t samplePeriod)
    : LPS22HB(i2c, address), Sensor(id, sensitivity, samplePeriod)
{
    configure();
}

void LPS22HB_Pressure::configure()
{
    LPS22HB::init();

    isInitialized = true;
}
