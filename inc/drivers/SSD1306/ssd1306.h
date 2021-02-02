#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <cstdint>

#include "STM32I2C.h"
#include "frameBuffer.h"

namespace codal{
    
    class SSD1306 : public FrameBuffer{
        public:
            SSD1306(unsigned width, unsigned height, bool externalVCC = false);
            ~SSD1306();

            void init();
            void powerOff();
            void powerOn();
            void contrast( uint8_t contrast);
            void invert( bool invert );
            void show();

            virtual void writeCommand( uint8_t cmd ) = 0;
            virtual void writeData( uint8_t* buf, unsigned size ) = 0;

        private:
            bool externalVCC;
    };

    class SSD1306_I2C : public SSD1306 {
        public:
            SSD1306_I2C( STM32I2C i2c, uint16_t address, unsigned width, unsigned height, bool externalVCC = false );
            ~SSD1306_I2C();

            void writeCommand( uint8_t cmd );
            void writeData( uint8_t* buf, unsigned len );

        private:
            STM32I2C i2c;
            uint16_t address;
    };

}

#endif