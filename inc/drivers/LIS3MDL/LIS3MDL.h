#ifndef __LIS3MDL_H__
#define __LIS3MDL_H__

#include <array>

#include "Compass.h"
#include "lis3mdl_driver.h"
#include "STM32I2C.h"

namespace codal {
 
    class LIS3MDL {

        public:
            /**
             * @brief Constructor
             * 
             * @param i2c object of an helper class which handles the I2C peripheral
             * @param address the I2C sensor's address (default : 0x38)
             */
            LIS3MDL( STM32I2C& i2c, uint8_t address = 0x38 );
            
            /**
             * @brief Initialise the sensor
             * 
             * @param fullscale the fullscale configuration value (default: 4Ga)
             * @param dataRate the output datarate configuration value (default: 80Hz)
             */
            void init( LIS3MDL_MAG_FS_t fullscale = LIS3MDL_MAG_FS_4Ga, LIS3MDL_MAG_DO_t dataRate = LIS3MDL_MAG_DO_80Hz );

            /**
             * @brief Enable the sensor
             * 
             */
            void enable();

            /**
             * @brief Disable the sensor
             * 
             */
            void disable();

            /**
             * @brief Get the measure from the sensor
             * 
             * @return std::array<uint16_t, 3> The computed measures (mGauss) on 3 axis (x, y, z) 
             */
            std::array<uint32_t, 3> getMeasure();

            /**
             * @brief Get the raw measure from the sensor
             * 
             * @return std::array<uint16_t, 3> The raw measures on 3 axis (x, y, z)
             */
            std::array<uint16_t, 3> getRawMeasure();
            
            /**
             * @brief Get the Sensitivity value
             * 
             * @return float the factor to multiply with raw measure
             */
            float getSensitivity();

            /**
             * @brief Set the Output Data Rate value
             * 
             * @param dataRate the data rate configuration value
             */
            void setOutputDataRate( LIS3MDL_MAG_DO_t dataRate );

            /**
             * @brief Set the Full Scale value
             * 
             * @param fullscale the fullscale configuration value
             */
            void setFullScale( LIS3MDL_MAG_FS_t fullscale );

            /**
             * @brief Write data to the sensor
             * 
             * @param WriteAddr buffer address to write
             * @param pBuffer value to write
             * @param nBytesToWrite number of byte to write
             * @return uint8_t 0 if success, error code otherwise
             */
            uint8_t IO_Write( uint8_t WriteAddr, uint8_t *pBuffer, uint16_t nBytesToWrite );

            /**
             * @brief Read data from the sensor
             * 
             * @param ReadAddr buffer address to read
             * @param pBuffer buffer to write value
             * @param nBytesToRead number of byte to read
             * @return uint8_t 0 if succes, error code otherwise
             */
            uint8_t IO_Read( uint8_t ReadAddr, uint8_t *pBuffer, uint16_t nBytesToRead );

        protected:
            STM32I2C& i2c;
            uint8_t address;
            LIS3MDL_MAG_FS_t fullscale;
            LIS3MDL_MAG_DO_t dataRate;
    };

}

#ifdef __cplusplus
extern "C" {
#endif
uint8_t LIS3MDL_IO_Write( void *handle, uint8_t WriteAddr, uint8_t *pBuffer, uint16_t nBytesToWrite );
uint8_t LIS3MDL_IO_Read( void *handle, uint8_t ReadAddr, uint8_t *pBuffer, uint16_t nBytesToRead );
#ifdef __cplusplus
}
#endif

#endif