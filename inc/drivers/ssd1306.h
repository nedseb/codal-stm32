#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <cstdint>

#include "STM32I2C.h"
#include "frameBuffer.h"

namespace codal{
    
    class SSD1306 : FrameBuffer{
        public:
            SSD1306(unsigned width, unsigned height, bool externalVCC = false);
            ~SSD1306();

            void initDisplay();
            void powerOff();
            void powerOn();
            void contrast( uint8_t contrast);
            void invert( bool invert );
            void show();

            virtual int writeCommand( uint8_t cmd ) = 0;
            virtual int writeData( uint8_t* buf, unsigned size ) = 0;

        private:
            bool externalVCC;
    };

    class SSD1306_I2C : SSD1306 {
        public:
            SSD1306_I2C( STM32I2C i2c, uint16_t address, unsigned width, unsigned height, bool externalVCC = false );
            ~SSD1306_I2C();

            int writeCommand( uint8_t cmd );
            int writeData( uint8_t* buf, unsigned len );

        private:
            STM32I2C i2c;
            uint16_t address;
    };

}

#endif