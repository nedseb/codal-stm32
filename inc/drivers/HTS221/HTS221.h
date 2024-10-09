#ifndef __SSH221_H__
#define __SSH221_H__

#include "STM32I2C.h"

namespace codal {

enum HTS221_AVCONF_T : uint8_t {
    T_AVG_2   = 0b0000'0000,
    T_AVG_4   = 0b0000'1000,
    T_AVG_8   = 0b0001'0000,
    T_AVG_16  = 0b0001'1000,
    T_AVG_32  = 0b0010'0000,
    T_AVG_64  = 0b0010'1000,
    T_AVG_128 = 0b0011'0000,
    T_AVG_256 = 0b0011'1000
};

enum HTS221_AVCONF_H : uint8_t {
    H_AVG_4   = 0b0000,
    H_AVG_8   = 0b0001,
    H_AVG_16  = 0b0010,
    H_AVG_32  = 0b0011,
    H_AVG_64  = 0b0100,
    H_AVG_128 = 0b0101,
    H_AVG_256 = 0b0110,
    H_AVG_512 = 0b0111
};

enum HTS221_OUTPUT_RATE : uint8_t { ONE_SHOT = 0x00, RATE_1HZ = 0x01, RATE_7HZ = 0x02, RATE_12HZ = 0x03 };

class HTS221 {
  public:
    HTS221(STM32I2C* i2c, uint16_t address);
    ~HTS221() {}

    /**
     * @brief Initialise the sensor
     *          - Get the calibration value for interpolation methods
     *          - Power On the device
     *
     */
    void init();

    /**
     * @brief Device authentication
     *
     * @return uint8_t The device identifier
     */
    uint8_t whoAmI();

    /**
     * @brief Set the register to configure the number of average samples (humidity and temperature)
     *
     * @param avg_temp Number of average temperature samples (default AVG_16)
     * @param avg_hum Number of average humidity samples (default AVG_32)
     */
    void setAverageConfiguration(HTS221_AVCONF_T avg_temp = T_AVG_16, HTS221_AVCONF_H avg_hum = H_AVG_32);

    /**
     * @brief Turn on the sensor
     *
     */
    void powerOn();

    /**
     * @brief Turn off the sensor
     *
     */
    void powerOff();

    /**
     * @brief Enable the continuous update. Sensor doesn't wait user read data before update. Enable by default
     *
     */
    void enableContinuousUpdate();

    /**
     * @brief Disable the continuous update. Sensor waits user read data before update. Enable by default
     *
     */
    void disableContinuousUpdate();

    /**
     * @brief Set the output rate value (default one shot)
     *
     * @param rate The rate value (default ONE_SHOT)
     */
    void setOutputRate(HTS221_OUTPUT_RATE rate = HTS221_OUTPUT_RATE::ONE_SHOT);

    /**
     * @brief Enable internal heater
     *
     */
    void enableHeater();

    /**
     * @brief Disable internal heater
     *
     */
    void disableHeater();

    /**
     * @brief Start a new conversion from de sensor on One Shot mode.
     *
     */
    void startOneShotAcquisition();

    /**
     * @brief Enable and configure the data ready output signal. Disable by default.
     *
     * @param useOpenDrain Use open drain if true otherwise use push-pull (default)
     * @param activeLow Signal is active at low level if true otherwise active at high level (default)
     */
    void enableDataReadyOutputSignal(bool useOpenDrain = false, bool activeLow = false);

    /**
     * @brief Disable the data ready output signal. Disable by default.
     *
     */
    void disableDataReadyOutputSignal();

    /**
     * @brief Get if the temperature data is available
     *
     * @return TRUE if the data is available
     */
    bool isTemperatureDataAvailable();

    /**
     * @brief Get if the humidity data is available
     *
     * @return TRUE if the data is available
     */
    bool isHumidityDataAvailable();

    /**
     * @brief Get the Temperature value
     *
     * @return Temperature measure by the sensor
     */
    float getTemperature();

    /**
     * @brief Get the Humidity value
     *
     * @return Humidity measure by the sensor
     */
    float getHumidity();

  private:
    STM32I2C* i2c;
    uint16_t address;

    float T0Deg;
    float T1Deg;
    float H0Rh;
    float H1Rh;

    int16_t T0Out;
    int16_t T1Out;
    int16_t H0Out;
    int16_t H1Out;

    uint8_t readRegister(uint8_t reg);
    void readCalibrationValue();

    float interpolateTemperature(int16_t raw)
    {
        return (float)(raw - T0Out) * (T1Deg - T0Deg) / (float)(T1Out - T0Out) + T0Deg;
    }

    float interpolateHumidity(int16_t raw)
    {
        return (float)(raw - H0Out) * (H1Rh - H0Rh) / (float)(H1Out - H0Out) + H0Rh;
    }
};

}  // namespace codal

#endif