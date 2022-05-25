
#include <algorithm>

#include "codal_target_hal.h"
#include "ssd1327.h"

using namespace codal;
using namespace std;

SSD1327_SPI::SSD1327_SPI(STM32SPI& spi, STM32Pin& chipSelect, STM32Pin& dataCommand, STM32Pin& reset, unsigned width,
                         unsigned height)
    : SSD1327(width, height), spi(spi), cs(chipSelect), dc(dataCommand), reset(reset)
{
    spi.setMode(3);
    spi.disableMISO();

    cs.setDigitalValue(1);
    dc.setDigitalValue(0);

    reset.setDigitalValue(0);
    target_wait(500);
    reset.setDigitalValue(1);
}

SSD1327_SPI::~SSD1327_SPI() {}

void SSD1327_SPI::writeCommand(uint8_t cmd)
{
    dc.setDigitalValue(0);
    cs.setDigitalValue(0);

    spi.write(cmd);

    cs.setDigitalValue(1);
}

void SSD1327_SPI::writeData(uint8_t* buf, unsigned len)
{
    cs.setDigitalValue(0);
    dc.setDigitalValue(1);

    spi.beginTransaction();

    for (unsigned i = 0; i < len; ++i) {
        spi.writeTransaction(buf[i]);
    }

    spi.endTransaction();

    cs.setDigitalValue(1);
}
