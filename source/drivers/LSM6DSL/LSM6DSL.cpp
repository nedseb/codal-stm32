#include "LSM6DSL.h"

using namespace codal;
using namespace std;

constexpr float SENSITIVITY_GYRO_245_DPS  = 0.00875f;
constexpr float SENSITIVITY_GYRO_500_DPS  = 0.01750f;
constexpr float SENSITIVITY_GYRO_1000_DPS = 0.03500f;
constexpr float SENSITIVITY_GYRO_2000_DPS = 0.07000f;

constexpr float SENSITIVITY_ACCEL_2G  = 0.000061f;
constexpr float SENSITIVITY_ACCEL_4G  = 0.000122f;
constexpr float SENSITIVITY_ACCEL_8G  = 0.000244f;
constexpr float SENSITIVITY_ACCEL_16G = 0.000488f;

LSM6DSL::LSM6DSL(STM32I2C* i2c, uint8_t address)
    : i2c(i2c), address(address), isGyroscopeEnable(false), isAccelerometerEnable(false)
{
}

void LSM6DSL::init(LSM6DSL_ACC_GYRO_ODR_G_t gyroscopeOutputDataRate,
                   LSM6DSL_ACC_GYRO_ODR_XL_t accelerometerOutputDataRate, LSM6DSL_ACC_GYRO_FS_G_t gyroscopeFullscale,
                   LSM6DSL_ACC_GYRO_FS_XL_t accelerometerFullscale)
{
    gyroODR  = gyroscopeOutputDataRate;
    accelODR = accelerometerOutputDataRate;

    disableGyroscope();
    disableAccelrometer();

    /* Enable register address automatically incremented during a multiple byte
    access with a serial interface. */
    LSM6DSL_ACC_GYRO_W_IF_Addr_Incr((void*)this, LSM6DSL_ACC_GYRO_IF_INC_ENABLED);

    /* Enable BDU */
    LSM6DSL_ACC_GYRO_W_BDU((void*)this, LSM6DSL_ACC_GYRO_BDU_BLOCK_UPDATE);

    /* FIFO mode selection */
    LSM6DSL_ACC_GYRO_W_FIFO_MODE((void*)this, LSM6DSL_ACC_GYRO_FIFO_MODE_BYPASS);

    setAccelerometerFullScale(accelerometerFullscale);
    setGyroscopeFullScale(gyroscopeFullscale);

    enableGyroscope();
    enableAccelrometer();
}

void LSM6DSL::enableGyroscope()
{
    isGyroscopeEnable = true;
    setGyroscopeOutputDataRate(gyroODR);
}

void LSM6DSL::disableGyroscope()
{
    isGyroscopeEnable = false;
    LSM6DSL_ACC_GYRO_W_ODR_G((void*)this, LSM6DSL_ACC_GYRO_ODR_G_POWER_DOWN);
}

void LSM6DSL::enableAccelrometer()
{
    isAccelerometerEnable = true;
    setAccelerometerOutputDataRate(accelODR);
}

void LSM6DSL::disableAccelrometer()
{
    isAccelerometerEnable = false;
    LSM6DSL_ACC_GYRO_W_ODR_XL((void*)this, LSM6DSL_ACC_GYRO_ODR_XL_POWER_DOWN);
}

void LSM6DSL::setAccelerometerFullScale(LSM6DSL_ACC_GYRO_FS_XL_t accelerometerFullscale)
{
    accelFS = accelerometerFullscale;
    LSM6DSL_ACC_GYRO_W_FS_XL((void*)this, accelerometerFullscale);
}

void LSM6DSL::setGyroscopeFullScale(LSM6DSL_ACC_GYRO_FS_G_t gyroscopeFullscale)
{
    gyroFS = gyroscopeFullscale;
    LSM6DSL_ACC_GYRO_W_FS_G((void*)this, gyroscopeFullscale);
}

void LSM6DSL::setAccelerometerOutputDataRate(LSM6DSL_ACC_GYRO_ODR_XL_t accelerometerOutputDataRate)
{
    accelODR = accelerometerOutputDataRate;

    if (isAccelerometerEnable) {
        LSM6DSL_ACC_GYRO_W_ODR_XL((void*)this, accelODR);
    }
}

void LSM6DSL::setGyroscopeOutputDataRate(LSM6DSL_ACC_GYRO_ODR_G_t gyroscopeOutputDataRate)
{
    gyroODR = gyroscopeOutputDataRate;

    if (isGyroscopeEnable) {
        LSM6DSL_ACC_GYRO_W_ODR_G((void*)this, gyroODR);
    }
}

float LSM6DSL::getAccelerometerSensitivity()
{
    switch (accelFS) {
        case LSM6DSL_ACC_GYRO_FS_XL_2g:
            return SENSITIVITY_ACCEL_2G;

        case LSM6DSL_ACC_GYRO_FS_XL_4g:
            return SENSITIVITY_ACCEL_4G;

        case LSM6DSL_ACC_GYRO_FS_XL_8g:
            return SENSITIVITY_ACCEL_8G;

        case LSM6DSL_ACC_GYRO_FS_XL_16g:
            return SENSITIVITY_ACCEL_16G;

        default:
            return 0.0f;
    }
}

float LSM6DSL::getGyroscopeSensitivity()
{
    switch (accelFS) {
        case LSM6DSL_ACC_GYRO_FS_G_245dps:
            return SENSITIVITY_GYRO_245_DPS;

        case LSM6DSL_ACC_GYRO_FS_G_500dps:
            return SENSITIVITY_GYRO_500_DPS;

        case LSM6DSL_ACC_GYRO_FS_G_1000dps:
            return SENSITIVITY_GYRO_1000_DPS;

        case LSM6DSL_ACC_GYRO_FS_G_2000dps:
            return SENSITIVITY_GYRO_2000_DPS;

        default:
            return 0.0f;
    }
}

array<int16_t, 3> LSM6DSL::getAccelerometerRawMeasure()
{
    uint8_t rawValues[6] = {0};
    array<int16_t, 3> result;

    LSM6DSL_ACC_GYRO_GetRawAccData((void*)this, rawValues);

    /* Format the data. */
    result[0] = ((int16_t)rawValues[1] << 8) + rawValues[0];
    result[1] = ((int16_t)rawValues[3] << 8) + rawValues[2];
    result[2] = ((int16_t)rawValues[5] << 8) + rawValues[4];

    return result;
}

array<float, 3> LSM6DSL::getAccelerometerMeasure()
{
    array<float, 3> result;
    auto dataRaw      = getAccelerometerRawMeasure();
    float sensitivity = getAccelerometerSensitivity();

    result[0] = dataRaw[0] * sensitivity;
    result[1] = dataRaw[1] * sensitivity;
    result[2] = dataRaw[2] * sensitivity;

    return result;
}

array<int16_t, 3> LSM6DSL::getGyroscopeRawMeasure()
{
    uint8_t rawValues[6] = {0};
    array<int16_t, 3> result;

    LSM6DSL_ACC_GYRO_GetRawGyroData((void*)this, rawValues);

    /* Format the data. */
    result[0] = ((int16_t)rawValues[1] << 8) + rawValues[0];
    result[1] = ((int16_t)rawValues[3] << 8) + rawValues[2];
    result[2] = ((int16_t)rawValues[5] << 8) + rawValues[4];

    return result;
}

array<float, 3> LSM6DSL::getGyroscopeMeasure()
{
    array<float, 3> result;
    auto rawValues    = getGyroscopeRawMeasure();
    float sensitivity = getGyroscopeSensitivity();

    result[0] = rawValues[0] * sensitivity;
    result[1] = rawValues[1] * sensitivity;
    result[2] = rawValues[2] * sensitivity;

    return result;
}

uint8_t LSM6DSL::IO_Write(uint8_t WriteAddr, uint8_t* pBuffer, uint16_t nBytesToWrite)
{
    i2c->beginTransmission(address);
    i2c->write(WriteAddr);
    i2c->write(pBuffer, nBytesToWrite);
    i2c->endTransmission();

    return 0;
}

uint8_t LSM6DSL::IO_Read(uint8_t ReadAddr, uint8_t* pBuffer, uint16_t nBytesToRead)
{
    auto result = i2c->readRegister(address, ReadAddr, nBytesToRead);
    memcpy(pBuffer, result.data(), nBytesToRead);

    return 0;
}

uint8_t LSM6DSL_IO_Write(void* handle, uint8_t WriteAddr, uint8_t* pBuffer, uint16_t nBytesToWrite)
{
    return ((LSM6DSL*)handle)->IO_Write(WriteAddr, pBuffer, nBytesToWrite);
}

uint8_t LSM6DSL_IO_Read(void* handle, uint8_t ReadAddr, uint8_t* pBuffer, uint16_t nBytesToRead)
{
    return ((LSM6DSL*)handle)->IO_Read(ReadAddr, pBuffer, nBytesToRead);
}
