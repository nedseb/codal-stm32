#include "CodalConfig.h"
#include "STM32SPI.h"
#include "ErrorNo.h"
#include "CodalDmesg.h"

using namespace codal;
using namespace std;

STM32SPI::STM32SPI( STM32Pin& miso, STM32Pin& mosi, STM32Pin& sclk, uint32_t freq, int mode, bool msbFirst )
    : SPI(), frequency(freq), mode(modeToSpiMode(mode)), isMsbFirst(msbFirst), isOnTransaction(false) {
    spi.pin_miso = (PinName)miso.name;
    spi.pin_mosi = (PinName)mosi.name;
    spi.pin_sclk = (PinName)sclk.name;
    spi.pin_ssel = NC;
}

void STM32SPI::beginTransaction(){
    if(isOnTransaction) return;

    buffer.clear();
    isOnTransaction = true;
}

vector<uint8_t> STM32SPI::endTransaction(uint32_t timeout, bool skipReceive){
    if(!isOnTransaction) return vector<uint8_t>();

    auto result = vector<uint8_t>( buffer.size() );

    spi_init(&spi, frequency, mode, isMsbFirst);

    spi_transfer( &spi, buffer.data(), result.data(), buffer.size(), timeout, skipReceive );

    spi_deinit(&spi);

    isOnTransaction = false;

    return result;
}

void STM32SPI::writeTransaction(uint8_t data){
    if( !isOnTransaction ) return;
    buffer.push_back(data);
}

int STM32SPI::write(int data){

    uint8_t sendData = data;
    uint8_t result = 0;

    spi_init(&spi, frequency, mode, isMsbFirst);
    spi_transfer( &spi, &sendData, &result, 1, 1000, false );
    spi_deinit(&spi);

    return (int)result;
}

spi_mode_e STM32SPI::modeToSpiMode(int mode){
    switch (mode)
    {
        case 0:
            return SPI_MODE_0;

        case 1:
            return SPI_MODE_1;

        case 2:
            return SPI_MODE_2;

        case 3:
            return SPI_MODE_3;

        default:
            return SPI_MODE_0;
    }
}