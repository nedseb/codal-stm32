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

#ifndef CODAL_STM32_I2C_H
#define CODAL_STM32_I2C_H

#include <vector>

#include "CodalConfig.h"
#include "ErrorNo.h"
#include "I2C.h"
#include "STM32Pin.h"
#include "pinmap.h"
#include "twi.h"

namespace codal {

class STM32I2C : public codal::I2C {
  private:
    i2c_t i2c;

    uint16_t currentAddress;
    std::vector<uint8_t> dataToSent;
    bool isOnTransmission;

  public:
    STM32I2C(STM32Pin& sda, STM32Pin& scl);

    /** Set the frequency of the I2C interface
     *
     * @param frequency The bus frequency in hertz
     */
    int setFrequency(uint32_t frequency) final override;

    /** Get the I2C buffer size
     *
     */
    unsigned getBufferSize() { return I2C_TXRX_BUFFER_SIZE; }

    /**
     * @brief Start I2C transmission with a slave.
     *
     * @param address The slave address
     */
    void beginTransmission(uint16_t address);

    /**
     * @brief Terminate the I2C transmission. Sending all data to the slave.
     *
     * @param sendStop If True sending the STOP signal. (default: True)
     */
    void endTransmission(bool sendStop = true);

    /**
     * @brief Write single byte to TX buffer.
     *
     * @param data byte to write to buffer
     * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the the write request failed.
     */
    int write(uint8_t data) final override;

    /**
     * @brief Write bytes to TX buffer
     *
     * @param data pointer to the bytes to write
     * @param len the number of bytes to write
     * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the the write request failed.
     */
    int write(uint8_t* data, size_t len);

    /**
     * @brief Write byte to the slave's register
     *
     * @param reg register address
     * @param value byte to send
     */
    void writeRegister(uint8_t reg, uint8_t value);

    /**
     * @brief Read data from slave
     *
     * @param address the slave addres
     * @param len amount data to read
     * @param sendStop If True sending the STOP signal. (default: True)
     * @return bytes received from slave
     */
    std::vector<uint8_t> read(uint8_t address, size_t len, bool sendStop = true);

    /**
     * @brief Read value from slave's register
     *
     * @param address the slave address
     * @param reg the register address
     * @param len amount of data to read
     * @param sendStop If True sending the STOP signal. (default: True)
     * @return bytes received from slave
     */
    std::vector<uint8_t> readRegister(uint8_t address, uint8_t reg, size_t len, bool sendStop = true);

  protected:
    /**
     * Issues a START condition on the I2C bus
     * @return DEVICE_OK on success, or an error code
     */
    int start() final override { return DEVICE_NOT_IMPLEMENTED; }

    /**
     * Issues a STOP condition on the I2C bus
     * @return DEVICE_OK on success, or an error code
     */
    int stop() final override { return DEVICE_NOT_IMPLEMENTED; }

    /**
     * Reads a single byte from the I2C bus.
     * The CPU will busy wait until the transmission is complete.
     *
     * @return the byte read from the I2C bus, or DEVICE_I2C_ERROR if the the write request failed.
     */
    int read(AcknowledgeType ack = ACK) final override { return DEVICE_NOT_IMPLEMENTED; }

    /**
     * Issues a standard, I2C command write to the I2C bus.
     * This consists of:
     *  - Asserting a Start condition on the bus
     *  - Selecting the Slave address (as an 8 bit address)
     *  - Writing a number of raw data bytes provided
     *  - Asserting a Stop condition on the bus
     *
     * The CPU will busy wait until the transmission is complete.
     *
     * @param address The 8bit I2C address of the device to write to
     * @param data pointer to the bytes to write
     * @param len the number of bytes to write
     * @param repeated Suppresses the generation of a STOP condition if set. Default: false;
     *
     * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the the write request failed.
     */
    int write(uint16_t address, uint8_t* data, int len, bool repeated = false) final override {
        return DEVICE_NOT_IMPLEMENTED;
    }

    /**
     * Performs a typical register write operation to the I2C slave device provided.
     * This consists of:
     *  - Asserting a Start condition on the bus
     *  - Selecting the Slave address (as an 8 bit address)
     *  - Writing the 8 bit register address provided
     *  - Writing the 8 bit value provided
     *  - Asserting a Stop condition on the bus
     *
     * The CPU will busy wait until the transmission is complete..
     *
     * @param address 8bit address of the device to write to
     * @param reg The 8bit address of the register to write to.
     * @param value The value to write.
     *
     * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the the write request failed.
     */
    int writeRegister(uint16_t address, uint8_t reg, uint8_t value) final override { return DEVICE_NOT_IMPLEMENTED; }

    /**
     * Issues a standard, 2 byte I2C command read to the I2C bus.
     * This consists of:
     *  - Asserting a Start condition on the bus
     *  - Selecting the Slave address (as an 8 bit address)
     *  - reading "len" bytes of raw 8 bit data into the buffer provided
     *  - Asserting a Stop condition on the bus
     *
     * The CPU will busy wait until the transmission is complete.
     *
     * @param address The 8bit I2C address of the device to read from
     * @param data pointer to store the the bytes read
     * @param len the number of bytes to read into the buffer
     * @param repeated Suppresses the generation of a STOP condition if set. Default: false;
     *
     * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the the read request failed.
     */
    int read(uint16_t address, uint8_t* data, int len, bool repeated = false) final override {
        return DEVICE_NOT_IMPLEMENTED;
    }

    /**
     * Performs a typical register read operation to the I2C slave device provided.
     * This consists of:
     *  - Asserting a Start condition on the bus
     *  - Selecting the Slave address (as an 8 bit address, I2C WRITE)
     *  - Selecting a RAM register address in the slave
     *  - Asserting a Stop condition on the bus
     *  - Asserting a Start condition on the bus
     *  - Selecting the Slave address (as an 8 bit address, I2C READ)
     *  - Performing an 8 bit read operation (of the requested register)
     *  - Asserting a Stop condition on the bus
     *
     * The CPU will busy wait until the transmission is complete..
     *
     * @param address 8bit I2C address of the device to read from
     * @param reg The 8bit register address of the to read.
     * @param data A pointer to a memory location to store the result of the read operation
     * @param length The number of mytes to read
     * @param repeated Use a repeated START/START/STOP transaction if true, or independent
     * START/STOP/START/STOP transactions if fasle. Default: true
     *
     * @return DEVICE_OK or DEVICE_I2C_ERROR if the the read request failed.
     */
    int readRegister(uint16_t address, uint8_t reg, uint8_t* data, int length, bool repeated = true) final override {
        return DEVICE_NOT_IMPLEMENTED;
    }

    /**
     * @brief Set the XferOptions of i2c object
     *
     * @param sendStop If True sending the STOP signal.
     */
    void setXferOptions(bool sendStop);
};
}  // namespace codal

#endif /*CODAL_STM32_I2C_H*/