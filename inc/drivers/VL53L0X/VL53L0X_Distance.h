#ifndef __VL53L0X_DISTANCE_H__
#define __VL53L0X_DISTANCE_H__

#include "Sensor.h"
#include "VL53L0X.h"

namespace codal {

class VL53L0X_Distance : public VL53L0X, public Sensor {
  public:
    VL53L0X_Distance(uint8_t id, STM32I2C* i2c, STM32Pin* shutPin, uint8_t address,
                     uint16_t sensitivity  = SENSOR_DEFAULT_SENSITIVITY,
                     uint16_t samplePeriod = SENSOR_DEFAULT_SAMPLE_PERIOD);

    /**
     * @brief Configure the sensor
     *
     */
    void configure();

    /**
     * Read the value from underlying hardware.
     */
    virtual int readValue()
    {
        if (!isInitialized) {
            configure();
        }
        return getDistance();
    };

  private:
    bool isInitialized;
};

}  // namespace codal

#endif