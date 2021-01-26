#include <algorithm>

#include "ssd1306.h"

using namespace codal;
using namespace std;

SSD1306_I2C::SSD1306_I2C( STM32I2C i2c, uint16_t address, unsigned width, unsigned height, bool externalVCC ) :
    SSD1306( width, height, externalVCC ), i2c(i2c), address(address) {

}

SSD1306_I2C::~SSD1306_I2C(){ }

void SSD1306_I2C::writeCommand( uint8_t cmd ){

    /*i2c.begin();
    
    uint8_t tmp[2] = { 0x80, cmd }; // Co = 1, D/C# = 0
    return i2c.write(address, tmp, 2);

    i2c.end();*/

    i2c.beginTransmission(address);
        i2c.write( 0x80 );
        i2c.write( cmd );
    i2c.endTransmission();

}

void SSD1306_I2C::writeData( uint8_t* buf, unsigned len )
{

    unsigned byteToSend = 1;

    i2c.beginTransmission(address);
    i2c.write(0x40);

    for( unsigned i = 0; i < len; i++ ){

        if( byteToSend >= i2c.getBufferSize() ){
            i2c.endTransmission();
            i2c.beginTransmission(address);
            i2c.write(0x40);

            byteToSend = 1;
        }

        i2c.write( buf[i] );
        ++byteToSend;
    }

    i2c.endTransmission();
}
