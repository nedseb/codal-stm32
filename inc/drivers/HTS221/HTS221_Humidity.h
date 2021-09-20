#ifndef __SSH221_HUMIDITY_H__
#define __SSH221_HUMIDITY_H__

#include "HTS221.h"
#include "STM32I2C.h"
#include "Sensor.h"

namespace codal {

class HTS221_Humidity : public HTS221, public Sensor {
  public:
    HTS221_Humidity(uint8_t id, STM32I2C* i2c, uint16_t address, uint16_t sensitivity = SENSOR_DEFAULT_SENSITIVITY,
                    uint16_t samplePeriod = SENSOR_DEFAULT_SAMPLE_PERIOD);

    /**
     * @brief Configure the sensor
     *
     */
    void configure();

    /**
     * Read the value from underlying hardware.
     */
    virtual int readValue() override final
    {
        if (!isInitialized) configure();
        return (int)getHumidity();
    }

  private:
    bool isInitialized;
};

}  // namespace codal

#endif