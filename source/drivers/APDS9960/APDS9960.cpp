#include "APDS9960.h"

constexpr uint8_t REG_ENABLE  = 0x80;
constexpr uint8_t REG_ATIME   = 0x81;
constexpr uint8_t REG_WTIME   = 0x82;
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
    setBit(REG_ENABLE, 0);
    setBit(REG_ENABLE, 3);
    setRegister(REG_WTIME, 246);

    setBit(REG_ENABLE, 1);
    setRegister(REG_ATIME, 219);
    setGain(APDS9960Gain::GAIN_x4);
}

array<uint16_t, 4> APDS9960::getColors(bool wait)
{
    while (getBit(REG_STATUS, 0) == 0)
        ;

    uint16_t c;
    uint16_t r;
    uint16_t g;
    uint16_t b;

    c = readRegister(REG_CDATA_L) | (readRegister(REG_CDATA_H) << 8);
    r = readRegister(REG_RDATA_L) | (readRegister(REG_RDATA_H) << 8);
    g = readRegister(REG_GDATA_L) | (readRegister(REG_GDATA_H) << 8);
    b = readRegister(REG_BDATA_L) | (readRegister(REG_BDATA_H) << 8);

    return {r, g, b, c};
}

void APDS9960::setGain(APDS9960Gain gain)
{
    this->gain  = gain;
    uint8_t reg = readRegister(REG_CONTROL);
    reg &= 0b11111100;
    reg |= (uint8_t)gain;
    setRegister(REG_CONTROL, reg);
}

uint8_t APDS9960::readRegister(uint8_t reg)
{
    return i2c.readRegister(address, reg, 1)[0];
}

void APDS9960::setRegister(uint8_t reg, uint8_t value)
{
    i2c.beginTransmission(address);
    i2c.writeRegister(reg, value);
    i2c.endTransmission();
}
