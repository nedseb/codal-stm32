#pragma once

#ifdef STM32L4xx
#include "MP34DT01.h"
#include "Sensor.h"

namespace codal {

class MP34DT01_dBMeter : public MP34DT01, public Sensor {
  public:
    MP34DT01_dBMeter(uint8_t id, uint16_t sensitivity = SENSOR_DEFAULT_SENSITIVITY,
                     uint16_t samplePeriod = SENSOR_DEFAULT_SAMPLE_PERIOD)
        : MP34DT01(), Sensor(id, sensitivity, samplePeriod)
    {
        configure();
    }

    /**
     * @brief Configure the sensor
     *
     */
    void configure()
    {
        MP34DT01::init();
        isInitialized = true;
    }

    /**
     * @brief Read the value from underlying hardware
     *
     * @return int
     */
    virtual int readValue()
    {
        if (!isInitialized) {
            configure();
        }

        return getDecibel();
    }

  private:
    bool isInitialized;
};

}  // namespace codal

#endif
