#ifndef __LSM6DSL_H__
#define __LSM6DSL_H__

#include <array>

#include "LSM6DSL_ACC_GYRO_Driver.h"
#include "STM32I2C.h"

namespace codal{

class LSM6DSL{

    public:
        /**
         * @brief Constructor
         * 
         * @param i2c object of an helper class which handles the I2C peripheral
         * @param address the address of the component's instance
         */
        LSM6DSL( STM32I2C& i2c, uint8_t address );

        /**
         * @brief Initialize the sensor
         * 
         * @param gyroscopeOutputDataRate The gyroscope output data rate value
         * @param accelerometerOutputDataRate The accelerometer output data rate value
         * @param gyroscopeFullscale The gyroscope full scale value
         * @param accelerometerFullscale The accelerometer full scale value
         */
        void init(  LSM6DSL_ACC_GYRO_ODR_G_t gyroscopeOutputDataRate = LSM6DSL_ACC_GYRO_ODR_G_13Hz,
                    LSM6DSL_ACC_GYRO_ODR_XL_t accelerometerOutputDataRate = LSM6DSL_ACC_GYRO_ODR_XL_13Hz,
                    LSM6DSL_ACC_GYRO_FS_G_t gyroscopeFullscale = LSM6DSL_ACC_GYRO_FS_G_245dps,
                    LSM6DSL_ACC_GYRO_FS_XL_t accelerometerFullscale = LSM6DSL_ACC_GYRO_FS_XL_2g);
        
        /**
         * @brief Enable the Gyroscope part
         * 
         */
        void enableGyroscope();

        /**
         * @brief Disable the gyroscope part
         * 
         */
        void disableGyroscope();

        /**
         * @brief Enable the accelerometer part
         * 
         */
        void enableAccelrometer();

        /**
         * @brief Disable the accelerometer part
         * 
         */
        void disableAccelrometer();

        /**
         * @brief Set the Accelerometer Full Scale
         * 
         * @param accelerometerFullscale the full scale value
         */
        void setAccelerometerFullScale(LSM6DSL_ACC_GYRO_FS_XL_t accelerometerFullscale);
        
        /**
         * @brief Set the Gyroscope Full Scale
         * 
         * @param gyroscopeFullscale the fullscale value
         */
        void setGyroscopeFullScale(LSM6DSL_ACC_GYRO_FS_G_t gyroscopeFullscale);

        /**
         * @brief Set the Accelerometer Output Data Rate
         * 
         * @param accelerometerOutputDataRate the output data rate value
         */
        void setAccelerometerOutputDataRate(LSM6DSL_ACC_GYRO_ODR_XL_t accelerometerOutputDataRate);
        
        /**
         * @brief Set the Gyroscope Output Data Rate
         * 
         * @param gyroscopeOutputDataRate the output data rate value
         */
        void setGyroscopeOutputDataRate(LSM6DSL_ACC_GYRO_ODR_G_t gyroscopeOutputDataRate);
        
        /**
         * @brief Get the Accelerometer Sensitivity for the current accelerometer full scale value
         * 
         * @return float 
         */
        float getAccelerometerSensitivity();

        /**
         * @brief Get the Gyroscope Sensitivity for the current gyroscope full scale value
         * 
         * @return float 
         */
        float getGyroscopeSensitivity();

        /**
         * @brief Get the Accelerometer Raw Measure
         * 
         * @return std::array<uint16_t, 3> the X, Y and Z raw measure
         */
        std::array<int16_t, 3> getAccelerometerRawMeasure();

        /**
         * @brief Get the Accelerometer Measure
         * 
         * @return std::array<float, 3> the X, Y and Z measure
         */
        std::array<float, 3> getAccelerometerMeasure();

        /**
         * @brief Get the Gyroscope Raw Measure
         * 
         * @return std::array<uint16_t, 3> the X, Y and Z raw measure
         */
        std::array<int16_t, 3> getGyroscopeRawMeasure();

        /**
         * @brief Get the Gyroscope Measure
         * 
         * @return std::array<float, 3> the X, Y and Z measure
         */
        std::array<float, 3> getGyroscopeMeasure();

        uint8_t IO_Write( uint8_t WriteAddr, uint8_t *pBuffer, uint16_t nBytesToWrite );
        uint8_t IO_Read( uint8_t ReadAddr, uint8_t *pBuffer, uint16_t nBytesToRead );
        
    protected:
        STM32I2C& i2c;
        uint8_t address;
        bool isGyroscopeEnable;
        bool isAccelerometerEnable;

        LSM6DSL_ACC_GYRO_ODR_G_t gyroODR;
        LSM6DSL_ACC_GYRO_ODR_XL_t accelODR;
        LSM6DSL_ACC_GYRO_FS_G_t gyroFS;
        LSM6DSL_ACC_GYRO_FS_XL_t accelFS;


};

}

#ifdef __cplusplus
extern "C" {
#endif
uint8_t LSM6DSL_IO_Write( void *handle, uint8_t WriteAddr, uint8_t *pBuffer, uint16_t nBytesToWrite );
uint8_t LSM6DSL_IO_Read( void *handle, uint8_t ReadAddr, uint8_t *pBuffer, uint16_t nBytesToRead );
#ifdef __cplusplus
}
#endif

#endif