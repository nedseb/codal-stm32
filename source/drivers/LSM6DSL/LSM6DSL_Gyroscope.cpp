#include "LSM6DSL_Gyroscope.h"

using namespace std;
using namespace codal;

LSM6DSL_Gyroscope::LSM6DSL_Gyroscope(STM32I2C* i2c, uint8_t address, CoordinateSpace& coordinateSpace, uint16_t id)
    : LSM6DSL(i2c, address), Gyroscope(coordinateSpace, id), isConfigured(false)
{
}

int LSM6DSL_Gyroscope::configure()
{
    LSM6DSL::init();

    LSM6DSL_ACC_GYRO_ODR_G_t odr = getBestODR(samplePeriod);
    LSM6DSL_ACC_GYRO_FS_G_t fs   = getBestFullScale(sampleRange);

    setGyroscopeFullScale(fs);
    setGyroscopeOutputDataRate(odr);

    isConfigured = true;

    return DEVICE_OK;
}

int LSM6DSL_Gyroscope::requestUpdate()
{
    if (!isConfigured) configure();

    auto result = getGyroscopeMeasure();
    Sample3D tmpSample(result[0], result[1], result[2]);

    return update(tmpSample);
}

LSM6DSL_ACC_GYRO_FS_G_t LSM6DSL_Gyroscope::getBestFullScale(float value)
{
    if (value <= 2)
        return LSM6DSL_ACC_GYRO_FS_G_245dps;
    else if (value <= 4)
        return LSM6DSL_ACC_GYRO_FS_G_500dps;
    else if (value <= 8)
        return LSM6DSL_ACC_GYRO_FS_G_1000dps;
    else
        return LSM6DSL_ACC_GYRO_FS_G_2000dps;
}

LSM6DSL_ACC_GYRO_ODR_G_t LSM6DSL_Gyroscope::getBestODR(float ms)
{
    float freq = 1 / (ms * 1'000);

    if (freq <= 12.5)
        return LSM6DSL_ACC_GYRO_ODR_G_13Hz;
    else if (freq <= 26)
        return LSM6DSL_ACC_GYRO_ODR_G_26Hz;
    else if (freq <= 52)
        return LSM6DSL_ACC_GYRO_ODR_G_52Hz;
    else if (freq <= 104)
        return LSM6DSL_ACC_GYRO_ODR_G_104Hz;
    else if (freq <= 208)
        return LSM6DSL_ACC_GYRO_ODR_G_208Hz;
    else if (freq <= 416)
        return LSM6DSL_ACC_GYRO_ODR_G_416Hz;
    else if (freq <= 833)
        return LSM6DSL_ACC_GYRO_ODR_G_833Hz;
    else if (freq <= 1'666)
        return LSM6DSL_ACC_GYRO_ODR_G_1660Hz;
    else if (freq <= 3'332)
        return LSM6DSL_ACC_GYRO_ODR_G_3330Hz;
    else
        return LSM6DSL_ACC_GYRO_ODR_G_6660Hz;
}
