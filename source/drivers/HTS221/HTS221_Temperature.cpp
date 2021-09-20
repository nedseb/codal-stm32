#include "HTS221_Temperature.h"

using namespace codal;

HTS221_Temperature::HTS221_Temperature(uint8_t id, STM32I2C* i2c, uint16_t address, uint16_t sensitivity,
                                       uint16_t samplePeriod)
    : HTS221(i2c, address), Sensor(id, sensitivity, samplePeriod), isInitialized(false)
{
    configure();
}

void HTS221_Temperature::configure()
{
    HTS221::init();
    setOutputRate(HTS221_OUTPUT_RATE::RATE_7HZ);

    isInitialized = true;
}
