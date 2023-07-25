#include "WSEN-PADS.h"

using namespace codal;

constexpr uint8_t INT_CFG        = 0x0B;
constexpr uint8_t THR_P_L        = 0x0C;
constexpr uint8_t THR_P_H        = 0x0D;
constexpr uint8_t INTERFACE_CTRL = 0x0E;
constexpr uint8_t DEVICE_ID      = 0x0F;
constexpr uint8_t CTRL_1         = 0x10;
constexpr uint8_t CTRL_2         = 0x11;
constexpr uint8_t CTRL_3         = 0x12;
constexpr uint8_t FIFO_CTRL      = 0x13;
constexpr uint8_t FIFO_WTM       = 0x14;
constexpr uint8_t REF_P_L        = 0x15;
constexpr uint8_t REF_P_H        = 0x16;
constexpr uint8_t OPC_P_L        = 0x18;
constexpr uint8_t OPC_P_H        = 0x19;
constexpr uint8_t INT_SOURCE     = 0x24;
constexpr uint8_t FIFO_STATUS_1  = 0x25;
constexpr uint8_t FIFO_STATUS_2  = 0x26;
constexpr uint8_t STATUS         = 0x27;
constexpr uint8_t DATA_P_XL      = 0x28;
constexpr uint8_t DATA_P_L       = 0x29;
constexpr uint8_t DATA_P_H       = 0x2A;
constexpr uint8_t DATA_T_L       = 0x2B;
constexpr uint8_t DATA_T_H       = 0x2C;
constexpr uint8_t FIFO_DATA_P_XL = 0x78;
constexpr uint8_t FIFO_DATA_P_L  = 0x79;
constexpr uint8_t FIFO_DATA_P_H  = 0x7A;
constexpr uint8_t FIFO_DATA_T_L  = 0x7B;
constexpr uint8_t FIFO_DATA_T_H  = 0x7C;

void WSEN_PADS::init()
{
    off();
    setLowNoiseMode(true);
    setLowPassFilter(true);
    setLowPassFilterConfig(true);
    setBDU(true);

    setODR(WSEN_PADS_ODR::RATE_50_HZ);
}

uint8_t WSEN_PADS::whoAmI()
{
    return getRegister(DEVICE_ID);
}

void WSEN_PADS::setLowNoiseMode(bool enable)
{
    if (enable)
        setBitRegister(CTRL_2, 1);
    else
        clearBitRegister(CTRL_2, 1);
}

void WSEN_PADS::setLowPassFilter(bool enable)
{
    if (enable)
        setBitRegister(CTRL_1, 3);
    else
        clearBitRegister(CTRL_1, 3);
}

void WSEN_PADS::setLowPassFilterConfig(bool enable)
{
    if (enable)
        setBitRegister(CTRL_1, 2);
    else
        clearBitRegister(CTRL_1, 2);
}

void WSEN_PADS::setBDU(bool enable)
{
    if (enable)
        setBitRegister(CTRL_1, 1);
    else
        clearBitRegister(CTRL_1, 1);
}

void WSEN_PADS::off()
{
    auto reg = getRegister(CTRL_1);
    reg &= ~(0b111 << 4);
    setRegister(CTRL_1, reg);
}

void WSEN_PADS::on()
{
    auto reg = getRegister(CTRL_1);
    reg &= ~(0b111 << 4);
    reg |= (uint8_t)odr << 4;
    setRegister(CTRL_1, reg);
}

void WSEN_PADS::oneShotMeasure()
{
    setBitRegister(CTRL_2, 0);
}

float WSEN_PADS::getPressure()
{
    uint8_t xl     = getRegister(DATA_P_XL);
    uint8_t l      = getRegister(DATA_P_L);
    uint8_t h      = getRegister(DATA_P_H);
    int32_t result = (int32_t)xl;
    result |= ((int32_t)l) << 8;
    result |= ((int32_t)h) << 16;

    return (float)result / 40960.0f;
}

float WSEN_PADS::getTemperature()
{
    uint8_t l      = getRegister(DATA_T_L);
    uint8_t h      = getRegister(DATA_T_H);
    int16_t result = (int16_t)l;
    result |= ((int16_t)h) << 8;

    return (float)result / 100.0f;
}

void WSEN_PADS::setRegister(uint8_t regAddr, uint8_t value)
{
    i2c.beginTransmission(address);
    i2c.writeRegister(regAddr, value);
    i2c.endTransmission();
}

void WSEN_PADS::setBitRegister(uint8_t regAddr, uint8_t bit)
{
    if (bit > 15) return;

    uint8_t reg = getRegister(regAddr);
    reg |= 1 << bit;
    setRegister(regAddr, reg);
}

void WSEN_PADS::clearBitRegister(uint8_t regAddr, uint8_t bit)
{
    if (bit > 15) return;

    uint8_t reg = getRegister(regAddr);
    reg &= ~(1 << bit);
    setRegister(regAddr, reg);
}
