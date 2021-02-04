#ifndef __LIS3MDL_H__
#define __LIS3MDL_H__

#include <array>

#include "Compass.h"
#include "lis3mdl_driver.h"
#include "STM32I2C.h"

namespace codal {
 
    class LIS3MDL : public Compass {

        public:
            /**
             * @brief Constructor
             * 
             * @param i2c object of an helper class which handles the I2C peripheral
             * @param address the I2C sensor's address (default : 0x38)
             */
            LIS3MDL( CoordinateSpace& coordinateSpace, STM32I2C& i2c, uint8_t address = 0x38 );
            
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

            /**
             * Configures the device for the sample rate defined
             * in this object. The nearest values are chosen to those defined
             * that are supported by the hardware. The instance variables are then
             * updated to reflect reality.
             *
             * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the compass could not be configured.
             */
            virtual int configure() override final;

            /**
             * Attempts to set the sample rate of the compass to the specified period value (in ms).
             *
             * @param period the requested time between samples, in milliseconds.
             * @return DEVICE_OK on success, DEVICE_I2C_ERROR is the request fails.
             *
             * @note The requested rate may not be possible on the hardware. In this case, the
             * nearest lower rate is chosen.
             *
             * @note This method should be overriden (if supported) by specific magnetometer device drivers.
             */
            virtual int setPeriod(int period) override final;

            /**
             * Poll to see if new data is available from the hardware. If so, update it.
             * n.b. it is not necessary to explicitly call this function to update data
             * (it normally happens in the background when the scheduler is idle), but a check is performed
             * if the user explicitly requests up to date data.
             *
             * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the update fails.
             *
             * @note This method should be overidden by the hardware driver to implement the requested
             * changes in hardware.
             */
            virtual int requestUpdate() override final;

            /**
             * A periodic callback invoked by the fiber scheduler idle thread.
             *
             * Internally calls updateSample().
             */
            virtual void idleCallback() override final { requestUpdate(); }

            /**
             * Puts the component in (or out of) sleep (low power) mode.
             */
            virtual int setSleep(bool doSleep) override final {
                doSleep ? disable() : enable();
                return DEVICE_OK;
            }


        private:
            STM32I2C& i2c;
            uint8_t address;
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