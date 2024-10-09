#pragma once

#include <vector>

#include "STM32I2C.h"

namespace codal {

enum class WSEN_PADS_ODR : uint8_t {
    SINGLE      = 0b000,
    RATE_1_HZ   = 0b001,
    RATE_10_HZ  = 0b010,
    RATE_25_HZ  = 0b011,
    RATE_50_HZ  = 0b100,
    RATE_75_HZ  = 0b101,
    RATE_100_HZ = 0b110,
    RATE_200_HZ = 0b111,
};

class WSEN_PADS {
  public:
    /**
     * @brief Construct a new wsen pads object
     *
     * @param i2c
     * @param address
     */
    WSEN_PADS(STM32I2C& i2c, uint16_t address = 0xB8) : i2c(i2c), address(address) {};

    ~WSEN_PADS() {}

    /**
     * @brief Initialise the sensor
     *
     */
    void init();

    uint8_t whoAmI();

    /**
     * @brief Change the Output Data Rate
     *
     * @param odr The rate to apply (default 10 Hz)
     */
    void setODR(WSEN_PADS_ODR odr)
    {
        this->odr = odr;
        on();
    }

    /**
     * @brief Set the Low Noise Mode
     *
     * @param enable if TRUE the low Noise mode will be enable (default), otherwise the low power mode will be enable
     */
    void setLowNoiseMode(bool enable);

    /**
     * @brief Set the second Low Pass Filter
     *
     * @param enable if TRUE the second lowpass filter will be enable, otherwise FALSE will disable it.
     */
    void setLowPassFilter(bool enable);

    /**
     * @brief Set the configuration Low Pass Filter
     *
     * @param enable if TRUE the second lowpass filter will be enable, otherwise FALSE will disable it.
     */
    void setLowPassFilterConfig(bool enable);

    /**
     * @brief Set the Block data update feature. The BDU blocks the data register to be updated until the register are
     * readed.
     *
     * @param enable if TRUE the BDU will be enable, otherwise FALSE will disable it.
     */
    void setBDU(bool enable);

    /**
     * @brief Stop measuring and enter in power-down mode
     *
     */
    void off();

    /**
     * @brief Start measuring (only if ODR not equal to SINGLE)
     *
     */
    void on();

    /**
     * @brief When the ODR is equal to SINGLE, use this fonction to start a measure.
     *
     */
    void oneShotMeasure();

    /**
     * @brief Get the Pressure
     *
     * @return float the pressure value in kilo Pascal (kPa)
     */
    float getPressure();

    /**
     * @brief Get the Temperature
     *
     * @return float the temperature value in Celsius (C)
     */
    float getTemperature();

  private:
    STM32I2C& i2c;
    WSEN_PADS_ODR odr;
    uint16_t address;

    void setRegister(uint8_t regAddr, uint8_t value);
    uint8_t getRegister(uint8_t regAddr) { return i2c.readRegister(address, regAddr, 1)[0]; }
    void setBitRegister(uint8_t regAddr, uint8_t bit);
    void clearBitRegister(uint8_t regAddr, uint8_t bit);
};
}  // namespace codal