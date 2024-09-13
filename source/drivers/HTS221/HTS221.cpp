#include "HTS221.h"

using namespace codal;

// --- Configuration & Status Registers ---
constexpr uint8_t WHO_AM_I   = 0x0F;
constexpr uint8_t AV_CONF    = 0x10;
constexpr uint8_t CTRL_REG1  = 0x20;
constexpr uint8_t CTRL_REG2  = 0x21;
constexpr uint8_t CTRL_REG3  = 0x22;
constexpr uint8_t STATUE_REG = 0x27;

// --- Data Registers ---
constexpr uint8_t HUMIDITY_OUT_L = 0x28;
constexpr uint8_t HUMIDITY_OUT_H = 0x29;
constexpr uint8_t TEMP_OUT_L     = 0x2A;
constexpr uint8_t TEMP_OUT_H     = 0x2B;

// --- Calibration registers ---
constexpr uint8_t H0_RH_X2      = 0x30;
constexpr uint8_t H1_RH_X2      = 0x31;
constexpr uint8_t T0_DEGC_X8    = 0x32;
constexpr uint8_t T1_DEGC_X8    = 0x33;
constexpr uint8_t T0_T1_MSB     = 0x35;
constexpr uint8_t H0_T0_OUT_LSB = 0x36;
constexpr uint8_t H0_T0_OUT_MSB = 0x37;
constexpr uint8_t H1_T0_OUT_LSB = 0x3A;
constexpr uint8_t H1_T0_OUT_MSB = 0x3B;
constexpr uint8_t T0_OUT_LSB    = 0x3C;
constexpr uint8_t T0_OUT_MSB    = 0x3D;
constexpr uint8_t T1_OUT_LSB    = 0x3E;
constexpr uint8_t T1_OUT_MSB    = 0x3F;

// --- Bits for CTRL_REG_1 ---
constexpr uint8_t BIT_PD   = 0x80;
constexpr uint8_t BIT_BDU  = 0x04;
constexpr uint8_t MASK_ODR = 0x03;

// --- Bits for CTRL_REG_2 ---
constexpr uint8_t BIT_HEATER   = 0x02;
constexpr uint8_t BIT_ONE_SHOT = 0x01;

// --- Bits for CTRL_REG_3 ---
constexpr uint8_t BIT_DRDY_H_L = 0x80;
constexpr uint8_t BIT_PP_OD    = 0x40;
constexpr uint8_t BIT_DRDY     = 0x04;

// --- Bits for STATUS_REG ---
constexpr uint8_t BIT_H_DA = 0x02;
constexpr uint8_t BIT_T_DA = 0x01;

// -- Mask for calibration registers ---
constexpr uint8_t MASK_T0_MSB = 0x03;
constexpr uint8_t MASK_T1_MSB = 0x0C;

HTS221::HTS221(STM32I2C* i2c, uint16_t address) : i2c(i2c), address(address) {}

void HTS221::init()
{
    readCalibrationValue();
    powerOn();
}

uint8_t HTS221::whoAmI()
{
    return readRegister(WHO_AM_I);
}

void HTS221::setAverageConfiguration(HTS221_AVCONF_T avg_temp, HTS221_AVCONF_H avg_hum)
{
    i2c->beginTransmission(address);
    i2c->writeRegister(AV_CONF, static_cast<uint8_t>(avg_temp) | static_cast<uint8_t>(avg_hum));
    i2c->endTransmission();
}

void HTS221::powerOn()
{
    uint8_t ctrlReg = readRegister(CTRL_REG1);
    ctrlReg |= BIT_PD;

    i2c->beginTransmission(address);
    i2c->writeRegister(CTRL_REG1, ctrlReg);
    i2c->endTransmission();
}

void HTS221::powerOff()
{
    uint8_t ctrlReg = readRegister(CTRL_REG1);
    ctrlReg &= ~BIT_PD;

    i2c->beginTransmission(address);
    i2c->writeRegister(CTRL_REG1, ctrlReg);
    i2c->endTransmission();
}

void HTS221::enableContinuousUpdate()
{
    uint8_t ctrlReg = readRegister(CTRL_REG1);
    ctrlReg &= ~BIT_BDU;

    i2c->beginTransmission(address);
    i2c->writeRegister(CTRL_REG1, ctrlReg);
    i2c->endTransmission();
}

void HTS221::disableContinuousUpdate()
{
    uint8_t ctrlReg = readRegister(CTRL_REG1);
    ctrlReg |= BIT_BDU;

    i2c->beginTransmission(address);
    i2c->writeRegister(CTRL_REG1, ctrlReg);
    i2c->endTransmission();
}

void HTS221::setOutputRate(HTS221_OUTPUT_RATE rate)
{
    uint8_t ctrlReg = readRegister(CTRL_REG1);
    ctrlReg &= ~MASK_ODR;
    ctrlReg |= rate;

    i2c->beginTransmission(address);
    i2c->writeRegister(CTRL_REG1, ctrlReg);
    i2c->endTransmission();
}

void HTS221::enableHeater()
{
    uint8_t ctrlReg = readRegister(CTRL_REG2);
    ctrlReg |= BIT_HEATER;

    i2c->beginTransmission(address);
    i2c->writeRegister(CTRL_REG2, ctrlReg);
    i2c->endTransmission();
}

void HTS221::disableHeater()
{
    uint8_t ctrlReg = readRegister(CTRL_REG2);
    ctrlReg &= ~BIT_HEATER;

    i2c->beginTransmission(address);
    i2c->writeRegister(CTRL_REG2, ctrlReg);
    i2c->endTransmission();
}

void HTS221::startOneShotAcquisition()
{
    uint8_t ctrlReg = readRegister(CTRL_REG2);
    ctrlReg |= BIT_ONE_SHOT;

    i2c->beginTransmission(address);
    i2c->writeRegister(CTRL_REG2, ctrlReg);
    i2c->endTransmission();
}

void HTS221::enableDataReadyOutputSignal(bool useOpenDrain, bool activeLow)
{
    uint8_t ctrlReg = readRegister(CTRL_REG3);

    ctrlReg |= BIT_DRDY;
    ctrlReg &= ~(BIT_DRDY_H_L | BIT_PP_OD);

    if (useOpenDrain) {
        ctrlReg |= BIT_PP_OD;
    }
    if (activeLow) {
        ctrlReg |= BIT_DRDY_H_L;
    }

    i2c->beginTransmission(address);
    i2c->writeRegister(CTRL_REG3, ctrlReg);
    i2c->endTransmission();
}

void HTS221::disableDataReadyOutputSignal()
{
    uint8_t ctrlReg = readRegister(CTRL_REG3);
    ctrlReg &= ~BIT_DRDY;

    i2c->beginTransmission(address);
    i2c->writeRegister(CTRL_REG3, ctrlReg);
    i2c->endTransmission();
}

bool HTS221::isTemperatureDataAvailable()
{
    uint8_t status = readRegister(STATUE_REG);
    return (status & BIT_T_DA) > 0;
}

bool HTS221::isHumidityDataAvailable()
{
    uint8_t status = readRegister(STATUE_REG);
    return (status & BIT_H_DA) > 0;
}

float HTS221::getTemperature()
{
    int16_t rawValue = (int16_t)(readRegister(TEMP_OUT_H) << 8) | readRegister(TEMP_OUT_L);
    return interpolateTemperature(rawValue);
}

float HTS221::getHumidity()
{
    int16_t rawValue = (int16_t)(readRegister(HUMIDITY_OUT_H) << 8) | readRegister(HUMIDITY_OUT_L);
    return interpolateHumidity(rawValue);
}

uint8_t HTS221::readRegister(uint8_t reg)
{
    return i2c->readRegister(address, reg, 1)[0];
}

void HTS221::readCalibrationValue()
{
    uint8_t TMsb = readRegister(T0_T1_MSB);
    int16_t T0DegRaw;
    int16_t T1DegRaw;

    T0DegRaw = TMsb & MASK_T0_MSB;
    T0DegRaw <<= 8;
    T0DegRaw |= readRegister(T0_DEGC_X8);

    T1DegRaw = TMsb & MASK_T1_MSB;
    T1DegRaw <<= 6;
    T1DegRaw |= readRegister(T1_DEGC_X8);

    T0Deg = T0DegRaw >> 3;
    T1Deg = T1DegRaw >> 3;

    H0Rh = readRegister(H0_RH_X2) >> 1;
    H1Rh = readRegister(H1_RH_X2) >> 1;

    T0Out = (int16_t)(readRegister(T0_OUT_MSB) << 8) | readRegister(T0_OUT_LSB);
    T1Out = (int16_t)(readRegister(T1_OUT_MSB) << 8) | readRegister(T1_OUT_LSB);

    H0Out = (int16_t)(readRegister(H0_T0_OUT_MSB) << 8) | readRegister(H0_T0_OUT_LSB);
    H1Out = (int16_t)(readRegister(H1_T0_OUT_MSB) << 8) | readRegister(H1_T0_OUT_LSB);
}
