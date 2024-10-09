#include "LPS22HB.h"

using namespace codal;

LPS22HB::LPS22HB(STM32I2C* i2c, uint8_t address) : dev_i2c(i2c), address(address) {};

void LPS22HB::init()
{
    Last_ODR  = 25.0f;
    isEnabled = false;

    LPS22HB_Set_PowerMode((void*)this, LPS22HB_LowPower);
    LPS22HB_Set_Odr((void*)this, LPS22HB_ODR_ONE_SHOT);
    LPS22HB_Set_LowPassFilter((void*)this, LPS22HB_DISABLE);
    LPS22HB_Set_LowPassFilterCutoff((void*)this, LPS22HB_ODR_9);
    LPS22HB_Set_Bdu((void*)this, LPS22HB_BDU_NO_UPDATE);
    LPS22HB_Set_AutomaticIncrementRegAddress((void*)this, LPS22HB_DISABLE);
    LPS22HB_Set_PowerMode((void*)this, LPS22HB_LowPower);

    enable();
}

void LPS22HB::enable(void)
{
    if (isEnabled) {
        return;
    }

    setODR_When_Enabled(Last_ODR);

    isEnabled = true;
}

void LPS22HB::disable(void)
{
    if (!isEnabled) {
        return;
    }

    LPS22HB_Set_Odr((void*)this, LPS22HB_ODR_ONE_SHOT);

    isEnabled = false;
}

uint8_t LPS22HB::readID()
{
    uint8_t val = 0x00;

    LPS22HB_Get_DeviceID((void*)this, &val);

    return val;
}

void LPS22HB::reset(void)
{
    LPS22HB_MemoryBoot((void*)this);
}

float LPS22HB::getPressure()
{
    int32_t int32data = 0;

    LPS22HB_Get_Pressure((void*)this, &int32data);

    return (float)int32data / 100.0f;
}

float LPS22HB::getTemperature()
{
    int16_t int16data = 0;

    LPS22HB_Get_Temperature((void*)this, &int16data);

    return (float)int16data / 10.0f;
}

float LPS22HB::getODR()
{
    LPS22HB_Odr_et odr_low_level;

    LPS22HB_Get_Odr((void*)this, &odr_low_level);

    switch (odr_low_level) {
        case LPS22HB_ODR_ONE_SHOT:
            return 0.0f;

        case LPS22HB_ODR_1HZ:
            return 1.0f;

        case LPS22HB_ODR_10HZ:
            return 10.0f;

        case LPS22HB_ODR_25HZ:
            return 25.0f;

        case LPS22HB_ODR_50HZ:
            return 50.0f;

        case LPS22HB_ODR_75HZ:
            return 75.0f;

        default:
            return -1.0f;
    }
}

void LPS22HB::setODR(float odr)
{
    if (isEnabled) {
        setODR_When_Enabled(odr);
    }
    else {
        setODR_When_Disabled(odr);
    }
}

bool LPS22HB::setODR_When_Enabled(float odr)
{
    LPS22HB_Odr_et new_odr;

    new_odr = (odr <= 1.0f)    ? LPS22HB_ODR_1HZ
              : (odr <= 10.0f) ? LPS22HB_ODR_10HZ
              : (odr <= 25.0f) ? LPS22HB_ODR_25HZ
              : (odr <= 50.0f) ? LPS22HB_ODR_50HZ
                               : LPS22HB_ODR_75HZ;

    if (LPS22HB_Set_Odr((void*)this, new_odr) == LPS22HB_ERROR) {
        return false;
    }

    return true;
}

bool LPS22HB::setODR_When_Disabled(float odr)
{
    Last_ODR = (odr <= 1.0f) ? 1.0f : (odr <= 10.0f) ? 10.0f : (odr <= 25.0f) ? 25.0f : (odr <= 50.0f) ? 50.0f : 75.0f;

    return true;
}

uint8_t LPS22HB::readReg(uint8_t reg)
{
    uint8_t data;

    LPS22HB_ReadReg((void*)this, reg, 1, &data);

    return data;
}

bool LPS22HB::writeReg(uint8_t reg, uint8_t data)
{
    if (LPS22HB_WriteReg((void*)this, reg, 1, &data) == LPS22HB_ERROR) {
        return false;
    }

    return true;
}

uint8_t LPS22HB_IO_Write(void* handle, uint8_t WriteAddr, uint8_t* pBuffer, uint16_t nBytesToWrite)
{
    return ((LPS22HB*)handle)->IO_Write(pBuffer, WriteAddr, nBytesToWrite);
}

uint8_t LPS22HB_IO_Read(void* handle, uint8_t ReadAddr, uint8_t* pBuffer, uint16_t nBytesToRead)
{
    return ((LPS22HB*)handle)->IO_Read(pBuffer, ReadAddr, nBytesToRead);
}
