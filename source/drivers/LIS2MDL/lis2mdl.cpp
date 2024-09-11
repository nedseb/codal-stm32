#include "lis2mdl.h"

#include <vector>

using namespace std;
using namespace codal;

constexpr uint8_t WHO_AM_I   = 0x4F;
constexpr uint8_t CFG_REG_A  = 0x60;
constexpr uint8_t CFG_REG_B  = 0x61;
constexpr uint8_t CFG_REG_C  = 0x62;
constexpr uint8_t OUTX_L_REG = 0x68;
constexpr uint8_t OUTX_H_REG = 0x69;
constexpr uint8_t OUTY_L_REG = 0x6A;
constexpr uint8_t OUTY_H_REG = 0x6B;
constexpr uint8_t OUTZ_L_REG = 0x6C;
constexpr uint8_t OUTZ_H_REG = 0x6D;

constexpr float SENSITIVITY = 1.5;

LIS2MDL::LIS2MDL(STM32I2C* i2c, uint16_t address) : i2c(i2c), address(address) {}

void LIS2MDL::init()
{
    LisCfgRegA regA = getCfgRegA();
    LisCfgRegC regC = getCfgRegC();

    regA.MD        = 0;
    regA.ODR       = uint8_t(LIS2_ODR::F_100_HZ);
    regC.Self_test = 0;
    regC.BDU       = 1;

    setCfgRegA(regA);
    setCfgRegC(regC);
}

uint8_t LIS2MDL::whoAmI()
{
    return readRegister(WHO_AM_I);
}

void LIS2MDL::setODR(LIS2_ODR odr)
{
    LisCfgRegA regA = getCfgRegA();
    regA.ODR        = uint8_t(odr);
    setCfgRegA(regA);
}

void LIS2MDL::setLowPowerMode(bool enable)
{
    LisCfgRegA regA = getCfgRegA();
    regA.LP         = enable ? 1 : 0;
    setCfgRegA(regA);
}

void LIS2MDL::setLowPassFilter(bool enable)
{
    LisCfgRegB regB = getCfgRegB();
    regB.LPF        = enable ? 1 : 0;
    setCfgRegB(regB);
}

Lis2Data LIS2MDL::readData()
{
    vector<uint8_t> raw = i2c->readRegister(address, OUTX_L_REG, 6);

    int16_t rawX = (int16_t(raw[1]) << 8) | raw[0];
    int16_t rawY = (int16_t(raw[3]) << 8) | raw[2];
    int16_t rawZ = (int16_t(raw[5]) << 8) | raw[4];

    return Lis2Data{x : float(rawX) * SENSITIVITY, y : float(rawY) * SENSITIVITY, z : float(rawZ) * SENSITIVITY};
}

void LIS2MDL::sendRegister(uint8_t reg, uint8_t value)
{
    i2c->beginTransmission(address);
    i2c->writeRegister(reg, value);
    i2c->endTransmission();
}
uint8_t LIS2MDL::readRegister(uint8_t reg)
{
    return i2c->readRegister(address, reg, 1)[0];
}

LisCfgRegA LIS2MDL::getCfgRegA()
{
    LisCfgRegA reg;
    reg.reg = readRegister(CFG_REG_A);

    return reg;
}

void LIS2MDL::setCfgRegA(LisCfgRegA reg)
{
    sendRegister(CFG_REG_A, reg.reg);
}

LisCfgRegB LIS2MDL::getCfgRegB()
{
    LisCfgRegB reg;
    reg.reg = readRegister(CFG_REG_B);

    return reg;
}

void LIS2MDL::setCfgRegB(LisCfgRegB reg)
{
    sendRegister(CFG_REG_B, reg.reg);
}

LisCfgRegC LIS2MDL::getCfgRegC()
{
    LisCfgRegC reg;
    reg.reg = readRegister(CFG_REG_C);

    return reg;
}

void LIS2MDL::setCfgRegC(LisCfgRegC reg)
{
    sendRegister(CFG_REG_C, reg.reg);
}
