#ifndef __VL53L0X_H__
#define __VL53L0X_H__

#include "STM32I2C.h"
#include "STM32Pin.h"

#include "vl53l0x_class.h"

namespace codal {

    class VL53L0X {

        private:
            VL53L0X_base baseDriver;
            uint8_t address;

        public:
            VL53L0X( STM32I2C& i2c, STM32Pin& pin, uint8_t address );

            void init();
            void enable();
            void disable();

            uint32_t getDistance();
    };

}

#endif