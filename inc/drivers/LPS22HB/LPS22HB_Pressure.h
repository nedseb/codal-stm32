#ifndef __LPS22HB_PRESSURE_H__
#define __LPS22HB_PRESSURE_H__


#include "LPS22HB.h"
#include "Sensor.h"

namespace codal{

class LPS22HB_Pressure : public LPS22HB, public Sensor{
    public:
        LPS22HB_Pressure( uint8_t id, STM32I2C& i2c, uint8_t address = 0xB8, uint16_t sensitivity = SENSOR_DEFAULT_SENSITIVITY, uint16_t samplePeriod = SENSOR_DEFAULT_SAMPLE_PERIOD );

        /**
         * @brief Configure the sensor
         * 
         */
        void configure();

        /**
         * Read the value from underlying hardware.
         */
        virtual int readValue( ){
            if(!isInitialized){ configure(); }
            return getPressure();
        };

    private:
        bool isInitialized;
};

}

#endif