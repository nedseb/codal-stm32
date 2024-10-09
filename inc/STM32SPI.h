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
    STM32SPI(STM32Pin& miso, STM32Pin& mosi, STM32Pin& sclk, uint32_t freq = 14'000'000, int mode = 0,
             bool msbFirst = true);

    ~STM32SPI();

    /** @brief Set the bit order
     *
     * @param msbFirst the order of bits on the bus
     */
    virtual int setBitsOrder(bool msbFirst) final
    {
        isMsbFirst = msbFirst;
        return DEVICE_OK;
    }

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
     * @brief Writes the given two bytes to the SPI bus.
     * The CPU will busy wait until the transmission is complete.
     *
     * @deprecated Should not be used directly. Use (begin/write/end)Transaction instead.
     *
     * @param data The data to write.
     * @return Response from the SPI slave or DEVICE_SPI_ERROR if the the write request failed.
     */
    virtual int write16(uint16_t data) final;

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
    std::vector<uint8_t> endTransaction(uint32_t timeout = 1'000, bool skipReceive = false);

    /**
     * @brief
     *
     * @param data
     */
    void writeTransaction(uint8_t data);

    /**
     * @brief
     *
     * @param data
     */
    void write16Transaction(uint8_t data);

    void disableMISO() { isMISODisabled = true; }
    void enableMISO() { isMISODisabled = false; }

  private:
    SPI_HandleTypeDef hspi;
    uint32_t frequency;
    spi_mode_e mode;
    bool isMsbFirst;
    bool isOnTransaction;
    std::vector<uint8_t> buffer;
    PinName pin_miso;
    PinName pin_mosi;
    PinName pin_sclk;
    bool isMISODisabled;

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

    uint8_t getClockPhase()
    {
        if ((mode == SPI_MODE_0) || (mode == SPI_MODE_2)) {
            return SPI_PHASE_1EDGE;
        }
        else {
            return SPI_PHASE_2EDGE;
        }
    }

    uint8_t getClockPolarity()
    {
        if ((mode == SPI_MODE_0) || (mode == SPI_MODE_1)) {
            return SPI_POLARITY_LOW;
        }
        else {
            return SPI_POLARITY_HIGH;
        }
    }

    uint8_t getPrescaler()
    {
        uint32_t spi_freq = spi_getClkFreqInst(hspi.Instance);

        if (frequency >= (spi_freq / SPI_SPEED_CLOCK_DIV2_MHZ)) {
            return SPI_BAUDRATEPRESCALER_2;
        }
        else if (frequency >= (spi_freq / SPI_SPEED_CLOCK_DIV4_MHZ)) {
            return SPI_BAUDRATEPRESCALER_4;
        }
        else if (frequency >= (spi_freq / SPI_SPEED_CLOCK_DIV8_MHZ)) {
            return SPI_BAUDRATEPRESCALER_8;
        }
        else if (frequency >= (spi_freq / SPI_SPEED_CLOCK_DIV16_MHZ)) {
            return SPI_BAUDRATEPRESCALER_16;
        }
        else if (frequency >= (spi_freq / SPI_SPEED_CLOCK_DIV32_MHZ)) {
            return SPI_BAUDRATEPRESCALER_32;
        }
        else if (frequency >= (spi_freq / SPI_SPEED_CLOCK_DIV64_MHZ)) {
            return SPI_BAUDRATEPRESCALER_64;
        }
        else if (frequency >= (spi_freq / SPI_SPEED_CLOCK_DIV128_MHZ)) {
            return SPI_BAUDRATEPRESCALER_128;
        }
        else {
            return SPI_BAUDRATEPRESCALER_256;
        }
    }

    void enableSPIClock()
    {
#if defined SPI1_BASE
        // Enable SPI clock
        if (hspi.Instance == SPI1) {
            __HAL_RCC_SPI1_CLK_ENABLE();
        }
#endif

#if defined SPI2_BASE
        if (hspi.Instance == SPI2) {
            __HAL_RCC_SPI2_CLK_ENABLE();
        }
#endif

#if defined SPI3_BASE
        if (hspi.Instance == SPI3) {
            __HAL_RCC_SPI3_CLK_ENABLE();
        }
#endif

#if defined SPI4_BASE
        if (hspi.Instance == SPI4) {
            __HAL_RCC_SPI4_CLK_ENABLE();
        }
#endif

#if defined SPI5_BASE
        if (hspi.Instance == SPI5) {
            __HAL_RCC_SPI5_CLK_ENABLE();
        }
#endif

#if defined SPI6_BASE
        if (hspi.Instance == SPI6) {
            __HAL_RCC_SPI6_CLK_ENABLE();
        }
#endif
    }

    void disableSPIClock()
    {
#if defined SPI1_BASE
        // Enable SPI clock
        if (hspi.Instance == SPI1) {
            __HAL_RCC_SPI1_CLK_DISABLE();
        }
#endif

#if defined SPI2_BASE
        if (hspi.Instance == SPI2) {
            __HAL_RCC_SPI2_CLK_DISABLE();
        }
#endif

#if defined SPI3_BASE
        if (hspi.Instance == SPI3) {
            __HAL_RCC_SPI3_CLK_DISABLE();
        }
#endif

#if defined SPI4_BASE
        if (hspi.Instance == SPI4) {
            __HAL_RCC_SPI4_CLK_DISABLE();
        }
#endif

#if defined SPI5_BASE
        if (hspi.Instance == SPI5) {
            __HAL_RCC_SPI5_CLK_DISABLE();
        }
#endif

#if defined SPI6_BASE
        if (hspi.Instance == SPI6) {
            __HAL_RCC_SPI6_CLK_DISABLE();
        }
#endif
    }
};

}  // namespace codal

#endif /*CODAL_STM32_SPI_H*/