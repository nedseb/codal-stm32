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

#include <vector>

#include "SPI.h"
#include "STM32Pin.h"
#include "spi_com.h"

namespace codal {

class STM32SPI : public codal::SPI {
  public:
    /**
     * @brief Construct a new STM32SPI object
     *
     * @param miso the MISO (CIPO) pin
     * @param mosi the MOSI (COPI) pin
     * @param sclk the SCLK pin
     * @param freq the bus frequency in hertz (default: 14MHz)
     * @param mode Clock polarity and phase mode (default: 0)
     *
     * @code
     * mode | POL PHA
     * -----+--------
     *   0  |  0   0
     *   1  |  0   1
     *   2  |  1   0
     *   3  |  1   1
     * @endcode
     *
     * @param msbFirst Trus if should send data MSB First
     */
    STM32SPI(STM32Pin& miso, STM32Pin& mosi, STM32Pin& sclk, uint32_t freq = 14000000, int mode = 0,
             bool msbFirst = true);

    /** @brief Set the frequency of the SPI interface
     *
     * @param frequency The bus frequency in hertz
     */
    virtual int setFrequency(uint32_t frequency) override final
    {
        this->frequency = frequency;
        return DEVICE_OK;
    }

    /** @brief Set the mode of the SPI interface
     *
     * @param mode Clock polarity and phase mode (0 - 3)
     * @param bits [Not used] Number of bits per SPI frame (4 - 16)
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
    virtual int setMode(int mode, int bits = 8) override final
    {
        this->mode = modeToSpiMode(mode);
        return DEVICE_OK;
    }

    /**
     * @brief Writes the given byte to the SPI bus.
     * The CPU will busy wait until the transmission is complete.
     *
     * @deprecated Should not be used directly. Use (begin/write/end)Transaction instead.
     *
     * @param data The data to write.
     * @return Response from the SPI slave or DEVICE_SPI_ERROR if the the write request failed.
     */
    virtual int write(int data) override final;

    /**
     * @brief
     *
     */
    void beginTransaction();

    /**
     * @brief
     *
     * @param timeout
     * @param skipReceive
     * @return std::vector<uint8_t>
     */
    std::vector<uint8_t> endTransaction(uint32_t timeout = 1000, bool skipReceive = false);

    /**
     * @brief
     *
     * @param data
     */
    void writeTransaction(uint8_t data);

  private:
    spi_t spi;
    uint32_t frequency;
    spi_mode_e mode;
    bool isMsbFirst;
    bool isOnTransaction;
    std::vector<uint8_t> buffer;

    spi_mode_e modeToSpiMode(int mode);

    /**
     * @brief
     *
     * @param tx_buffer
     * @param rx_buffer
     * @param len
     *
     * @return
     */
    uint8_t* write(uint8_t* tx_buffer, uint8_t* rx_buffer, uint16_t len);
};

}  // namespace codal

#endif /*CODAL_STM32_SPI_H*/