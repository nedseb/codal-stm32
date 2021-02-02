/*
The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef CODAL_STM32_SPI_H
#define CODAL_STM32_SPI_H

#include <optional>

#include "CodalConfig.h"
#include "ErrorNo.h"
#include "SPI.h"

#include "STM32Pin.h"
#include "pinmap.h"

namespace codal {

class STM32SPI;
struct SPI_HandleWithParent : public SPI_HandleTypeDef
{
    STM32SPI *stm32spi_parent;
};

/**
 * Class definition for SPI service, derived from ARM mbed.
 */
class STM32SPI : public codal::SPI, public codal::CodalComponent
{
public:
    static void _complete(uint32_t instance);
    static void _irq(uint32_t instance);

    /**
     * Initialize SPI instance with given pins.
     *
     * Default setup is 1 MHz, 8 bit, mode 0.
     * If `cs` is specified, the SPI is run in slave mode.
     */
    STM32SPI(codal::Pin &mosi, codal::Pin &miso, codal::Pin &sclk, std::optional<codal::Pin> cs = std::nullopt);


    virtual ~STM32SPI() = default;

    /** Set the frequency of the SPI interface
     *
     * @param frequency The bus frequency in hertz
     */
    virtual int setFrequency(uint32_t frequency) override;

    /** Set the mode of the SPI interface
     *
     * @param mode Clock polarity and phase mode (0 - 3)
     * @param bits Number of bits per SPI frame (4 - 16)
     *
     * @code
     * mode | POL PHA
     * -----+--------
     *   0  |  0   0
     *   1  |  0   1
     *   2  |  1   0
     *   3  |  1   1
     * @endcode
     */
    virtual int setMode(int mode, int bits = 8) override;

    /**
     * Writes the given byte to the SPI bus.
     *
     * The CPU will wait until the transmission is complete.
     *
     * @param data The data to write.
     * @return Response from the SPI slave or DEVICE_SPI_ERROR if the the write request failed.
     */
    virtual int write(int data) override;

    /**
     * Writes and reads from the SPI bus concurrently. Waits un-scheduled for transfer to finish.
     *
     * Either buffer can be NULL.
     */
    virtual int transfer(const uint8_t *txBuffer, uint32_t txSize, uint8_t *rxBuffer,
                         uint32_t rxSize) override;

    virtual int startTransfer(const uint8_t *txBuffer, uint32_t txSize, uint8_t *rxBuffer,
                              uint32_t rxSize, PVoidCallback doneHandler, void *arg) override;
private:
    Pin *mosi, *miso, *sclk;
    Pin *cs;
    
    bool needsInit;
    bool isSlave;
    bool hasRx, hasTx;
    
    void complete();
    void init_internal();
};
#define CODAL_SPI_SLAVE_SUPPORTED 1
}  // namespace codal

#endif /*CODAL_STM32_SPI_H*/