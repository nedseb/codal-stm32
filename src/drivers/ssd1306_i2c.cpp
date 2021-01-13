#include "ssd1306.h"

using namespace codal;

SSD1306_I2C::SSD1306_I2C( STM32I2C i2c, uint16_t address, unsigned width, unsigned height, bool externalVCC ) :
    SSD1306( width, height, externalVCC ), i2c(i2c), address(address) {

}

SSD1306_I2C::~SSD1306_I2C(){ }

int SSD1306_I2C::writeCommand( uint8_t cmd ){
    uint8_t tmp[2] = { 0x80, cmd }; // Co = 1, D/C# = 0
    return i2c.write(address, tmp, 2);
}

int SSD1306_I2C::writeData( uint8_t* buf, unsigned len )
{
    uint8_t tmp[len+1];
    tmp[0] = 0x40;  // Co = 0, D/C# = 1
    
    memcpy( tmp + 1, buf, len );

    return i2c.write(address, buf, len+1 );
}
