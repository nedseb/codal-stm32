#include "LIS3MDL.h"

#include "CodalFiber.h"

using namespace codal;
using namespace std;

LIS3MDL::LIS3MDL(STM32I2C* i2c, uint8_t address) : i2c(i2c), address(address) {}

void LIS3MDL::init(LIS3MDL_MAG_FS_t fullscale, LIS3MDL_MAG_DO_t dataRate)
{
    this->fullscale = fullscale;
    this->dataRate  = dataRate;

    LIS3MDL_MAG_W_SoftRST((void*)this, LIS3MDL_MAG_SOFT_RST_YES);
    fiber_sleep(50);
    LIS3MDL_MAG_W_Reboot((void*)this, LIS3MDL_MAG_REBOOT_YES);
    fiber_sleep(50);

    disable();

    LIS3MDL_MAG_W_BlockDataUpdate((void*)this, LIS3MDL_MAG_BDU_ENABLE);
    setOutputDataRate(dataRate);
    setFullScale(fullscale);
    /* X and Y axes Operating mode selection */
    LIS3MDL_MAG_W_OperatingModeXY((void*)this, LIS3MDL_MAG_OM_HIGH);
    /* Temperature sensor disable - temp. sensor not used */
    LIS3MDL_MAG_W_TemperatureSensor((void*)this, LIS3MDL_MAG_TEMP_EN_DISABLE);

    enable();
}

void LIS3MDL::enable()
{
    LIS3MDL_MAG_W_SystemOperatingMode((void*)this, LIS3MDL_MAG_MD_CONTINUOUS);
}

void LIS3MDL::disable()
{
    LIS3MDL_MAG_W_SystemOperatingMode((void*)this, LIS3MDL_MAG_MD_POWER_DOWN);
}

array<int32_t, 3> LIS3MDL::getMeasure()
{
    array<int32_t, 3> finalValues;
    auto values       = getRawMeasure();
    float sensitivity = getSensitivity();

    finalValues[0] = (int32_t)((float)values[0] / sensitivity);
    finalValues[1] = (int32_t)((float)values[1] / sensitivity);
    finalValues[2] = (int32_t)((float)values[2] / sensitivity);

    return finalValues;
}

array<int16_t, 3> LIS3MDL::getRawMeasure()
{
    uint8_t rawValues[6] = {0};
    array<int16_t, 3> values;

    LIS3MDL_MAG_Get_Magnetic((void*)this, rawValues);

    values[0] = (int16_t)(rawValues[1] << 8) | rawValues[0];
    values[1] = (int16_t)(rawValues[3] << 8) | rawValues[2];
    values[2] = (int16_t)(rawValues[5] << 8) | rawValues[4];

    return values;
}

float LIS3MDL::getSensitivity()
{
    LIS3MDL_MAG_FS_t fullscale;
    LIS3MDL_MAG_R_FullScale((void*)this, &fullscale);

    // See page 8 of ST Datasheet "Sensitivity"
    switch (fullscale) {
        case LIS3MDL_MAG_FS_4Ga:
            return 6.842;

        case LIS3MDL_MAG_FS_8Ga:
            return 3.421;

        case LIS3MDL_MAG_FS_12Ga:
            return 2.281;

        case LIS3MDL_MAG_FS_16Ga:
            return 1.711;

        default:
            return 0; /* error */
    }
}

void LIS3MDL::setOutputDataRate(LIS3MDL_MAG_DO_t dataRate)
{
    this->dataRate = dataRate;
    LIS3MDL_MAG_W_OutputDataRate((void*)this, dataRate);
}

void LIS3MDL::setFullScale(LIS3MDL_MAG_FS_t fullscale)
{
    this->fullscale = fullscale;
    LIS3MDL_MAG_W_FullScale((void*)this, fullscale);
}

uint8_t LIS3MDL::IO_Write(uint8_t WriteAddr, uint8_t* pBuffer, uint16_t nBytesToWrite)
{
    i2c->beginTransmission(address);
    i2c->write(WriteAddr);
    i2c->write(pBuffer, nBytesToWrite);
    i2c->endTransmission();
    return 0;
}

uint8_t LIS3MDL::IO_Read(uint8_t ReadAddr, uint8_t* pBuffer, uint16_t nBytesToRead)
{
    auto result = i2c->readRegister(address, ReadAddr, nBytesToRead);
    memcpy(pBuffer, result.data(), nBytesToRead);
    return 0;
}

uint8_t LIS3MDL_IO_Write(void* handle, uint8_t WriteAddr, uint8_t* pBuffer, uint16_t nBytesToWrite)
{
    return ((LIS3MDL*)handle)->IO_Write(WriteAddr, pBuffer, nBytesToWrite);
}

uint8_t LIS3MDL_IO_Read(void* handle, uint8_t ReadAddr, uint8_t* pBuffer, uint16_t nBytesToRead)
{
    return ((LIS3MDL*)handle)->IO_Read(ReadAddr, pBuffer, nBytesToRead);
}
