/**
 ******************************************************************************
 * @file    LPS22HBSensor.h
 * @author  AST
 * @version V1.0.0
 * @date    7 September 2017
 * @brief   Abstract Class of a LPS22HB Pressure sensor.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

#ifndef __LPS22HB_H__
#define __LPS22HB_H__

#include "STM32I2C.h"
#include "lps22hb_driver.h"

namespace codal {

class LPS22HB {
  public:
    /**
     * @brief Constructor
     * @param i2c object of an helper class which handles the I2C peripheral
     * @param address the address of the component's instance (default : 0xB8)
     */
    LPS22HB(STM32I2C* i2c, uint8_t address = 0xB8);

    /**
     * @brief Init the sensor
     *
     */
    void init();

    /**
     * @brief Turn on the sensor
     *
     */
    void enable(void);

    /**
     * @brief Turn off the sensor
     *
     */
    void disable(void);

    /**
     * @brief  Read ID address of LPS22HB
     * @retval The ID of the device is stored
     */
    uint8_t readID();

    /**
     * @brief  Reboot memory content of LPS22HB
     */
    void reset(void);

    /**
     * @brief  Read LPS22HB output register, and calculate the pressure in mbar
     * @retval The pressure value
     */
    float getPressure();

    /**
     * @brief  Read LPS22HB output register, and calculate the temperature
     * @retval The temperature value
     */
    float getTemperature();

    /**
     * @brief  Read LPS22HB output data rate
     * @retval The output data rate
     */
    float getODR();

    /**
     * @brief  Set ODR
     * @param  odr the output data rate to be set
     */
    void setODR(float odr);

    /**
     * @brief Read the data from register
     * @param reg register address
     * @retval The register data
     */
    uint8_t readReg(uint8_t reg);

    /**
     * @brief Write the data to register
     * @param reg register address
     * @param data register data
     * @retval True in case of success, false otherwise
     */
    bool writeReg(uint8_t reg, uint8_t data);

    /**
     * @brief Utility function to read data.
     * @param  pBuffer: pointer to data to be read.
     * @param  RegisterAddr: specifies internal address register to be read.
     * @param  NumByteToRead: number of bytes to be read.
     * @retval 0 if ok, an error code otherwise.
     */
    uint8_t IO_Read(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToRead)
    {
        auto result = dev_i2c->readRegister(address, RegisterAddr, NumByteToRead);
        memcpy(pBuffer, result.data(), NumByteToRead);

        return 0;
    }

    /**
     * @brief Utility function to write data.
     * @param  pBuffer: pointer to data to be written.
     * @param  RegisterAddr: specifies internal address register to be written.
     * @param  NumByteToWrite: number of bytes to write.
     * @retval 0 if ok, an error code otherwise.
     */
    uint8_t IO_Write(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToWrite)
    {
        dev_i2c->beginTransmission(address);
        dev_i2c->write(RegisterAddr);
        dev_i2c->write(pBuffer, NumByteToWrite);
        dev_i2c->endTransmission();

        return 0;
    }

  private:
    STM32I2C* dev_i2c;
    uint8_t address;
    bool isEnabled;
    float Last_ODR;

    /**
     * @brief Set the LPS22HB sensor output data rate when enabled
     * @param odr the functional output data rate to be set
     * @retval True in case of success, false otherwise
     */
    bool setODR_When_Enabled(float odr);

    /**
     * @brief Set the LPS22HB sensor output data rate when disabled
     * @param odr the functional output data rate to be set
     * @retval True in case of success, false otherwise
     */
    bool setODR_When_Disabled(float odr);
};
}  // namespace codal

#ifdef __cplusplus
extern "C" {
#endif
uint8_t LPS22HB_IO_Write(void* handle, uint8_t WriteAddr, uint8_t* pBuffer, uint16_t nBytesToWrite);
uint8_t LPS22HB_IO_Read(void* handle, uint8_t ReadAddr, uint8_t* pBuffer, uint16_t nBytesToRead);
#ifdef __cplusplus
}
#endif

#endif