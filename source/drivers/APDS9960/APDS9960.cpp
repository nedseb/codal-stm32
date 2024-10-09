#include "APDS9960.h"

#include "stdio.h"

constexpr uint8_t REG_ENABLE  = 0x80;
constexpr uint8_t REG_ATIME   = 0x81;
constexpr uint8_t REG_WTIME   = 0x82;
constexpr uint8_t REG_CONFIG1 = 0x8D;
constexpr uint8_t REG_CONTROL = 0x8F;
constexpr uint8_t REG_STATUS  = 0x93;
constexpr uint8_t REG_CDATA_L = 0x94;
constexpr uint8_t REG_CDATA_H = 0x95;
constexpr uint8_t REG_RDATA_L = 0x96;
constexpr uint8_t REG_RDATA_H = 0x97;
constexpr uint8_t REG_GDATA_L = 0x98;
constexpr uint8_t REG_GDATA_H = 0x99;
constexpr uint8_t REG_BDATA_L = 0x9A;
constexpr uint8_t REG_BDATA_H = 0x9B;

using namespace codal;
using namespace std;

void APDS9960::init()
{
    setRegister(REG_ENABLE, 0b0000'0001);
    setRegister(REG_WTIME, 0xFF);
    setRegister(REG_ATIME, 0xFF);
    setRegister(REG_CONFIG1, 0b0110'0000);
    setGain(APDS9960Gain::GAIN_x4);
}

array<uint16_t, 4> APDS9960::getColors(bool wait)
{
    setBit(REG_ENABLE, 1);
    while (wait && getBit(REG_STATUS, 0) == 0);

    uint16_t c = read16BitRegister(REG_CDATA_L);
    uint16_t r = read16BitRegister(REG_RDATA_L);
    uint16_t g = read16BitRegister(REG_GDATA_L);
    uint16_t b = read16BitRegister(REG_BDATA_L);

    clearBit(REG_ENABLE, 1);

    return {r, g, b, c};
}

void APDS9960::setGain(APDS9960Gain gain)
{
    this->gain  = gain;
    uint8_t reg = readRegister(REG_CONTROL);
    reg &= 0b1100'1100;
    reg |= (uint8_t)gain;
    setRegister(REG_CONTROL, reg);
}

uint8_t APDS9960::readRegister(uint8_t reg)
{
    return i2c.readRegister(address, reg, 1)[0];
}

uint16_t APDS9960::read16BitRegister(uint8_t reg)
{
    auto regs = i2c.readRegister(address, reg, 2);
    return (uint16_t(regs[1]) << 8) | uint16_t(regs[0]);
}

void APDS9960::setRegister(uint8_t reg, uint8_t value)
{
    i2c.beginTransmission(address);
    i2c.writeRegister(reg, value);
    i2c.endTransmission();
}
