#pragma once

#include <array>

#include "STM32I2C.h"

namespace codal {

enum class APDS9960Gain : uint8_t {
    GAIN_x1  = 0,
    GAIN_x4  = 1,
    GAIN_x16 = 2,
    GAIN_x64 = 3,
};

class APDS9960 {
  public:
    APDS9960(STM32I2C& i2c, uint16_t address) : i2c(i2c), address(address) {}
    ~APDS9960() {}

    void init();

    std::array<uint16_t, 4> getColors(bool wait = true);
    void setGain(APDS9960Gain gain);

  private:
    STM32I2C& i2c;
    uint16_t address;
    APDS9960Gain gain;

    uint8_t readRegister(uint8_t reg);
    uint16_t read16BitRegister(uint8_t reg);
    void setRegister(uint8_t reg, uint8_t value);
    uint8_t getBit(uint8_t reg, uint8_t bit) { return (readRegister(reg) >> bit) & 0x01; }
    void setBit(uint8_t reg, uint8_t bit)
    {
        uint8_t r = readRegister(reg);
        r |= 1 << bit;
        setRegister(reg, r);
    }

    void clearBit(uint8_t reg, uint8_t bit)
    {
        uint8_t r = readRegister(reg);
        r &= ~(1 << bit);
        setRegister(reg, r);
    }
};

}  // namespace codal