#pragma once

#include "STM32I2C.h"
#include "STM32Pin.h"
#include "vl53l1x_class.h"

namespace codal {

class VL53L1X {
  private:
    VL53L1X_base baseDriver;
    uint8_t address;

  public:
    /**
     * @brief Constructor
     *
     * @param i2c the i2c interface to use to communicate with the sensor
     * @param pin the shut pin
     * @param address the sensor address
     */
    VL53L1X(STM32I2C* i2c, STM32Pin* pin = nullptr, uint8_t address = VL53L1X_DEFAULT_DEVICE_ADDRESS);

    /**
     * @brief Init the sensor (and enable it)
     *
     */
    void init();

    /**
     * @brief Enable the sensor
     *
     */
    void enable();

    /**
     * @brief Disable the sensor
     *
     */
    void disable();

    /**
     * @brief Get the measured distance (in mm)
     *
     * @return uint32_t the measure
     */
    uint32_t getDistance();
};

}  // namespace codal
