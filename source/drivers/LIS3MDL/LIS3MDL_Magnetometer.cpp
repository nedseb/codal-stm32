#include "LIS3MDL_Magnetometer.h"

using namespace codal;

LIS3MDL_Magnetometer::LIS3MDL_Magnetometer(STM32I2C* i2c, uint8_t address, CoordinateSpace& coordinateSpace,
                                           uint16_t id)
    : LIS3MDL(i2c, address), Compass(coordinateSpace, id)
{
}

int LIS3MDL_Magnetometer::configure()
{
    LIS3MDL::init();

    // set freq mHz
    float freq = 1.0 / (samplePeriod / 1000.0);

    if (freq <= 0.625f) {
        setOutputDataRate(LIS3MDL_MAG_DO_0_625Hz);
    }
    else if (freq <= 1.250f) {
        setOutputDataRate(LIS3MDL_MAG_DO_1_25Hz);
    }
    else if (freq <= 2.500f) {
        setOutputDataRate(LIS3MDL_MAG_DO_2_5Hz);
    }
    else if (freq <= 5.000f) {
        setOutputDataRate(LIS3MDL_MAG_DO_5Hz);
    }
    else if (freq <= 10.000f) {
        setOutputDataRate(LIS3MDL_MAG_DO_10Hz);
    }
    else if (freq <= 20.000f) {
        setOutputDataRate(LIS3MDL_MAG_DO_20Hz);
    }
    else if (freq <= 40.000f) {
        setOutputDataRate(LIS3MDL_MAG_DO_40Hz);
    }
    else {
        setOutputDataRate(LIS3MDL_MAG_DO_80Hz);
    }

    return DEVICE_OK;
}

int LIS3MDL_Magnetometer::requestUpdate()
{
    auto values    = getMeasure();
    auto rawValues = getRawMeasure();

    sample.x       = values[0];
    sample.y       = values[1];
    sample.z       = values[2];

    sampleENU.x    = rawValues[0];
    sampleENU.y    = rawValues[1];
    sampleENU.z    = rawValues[2];

    update();

    return DEVICE_OK;
}