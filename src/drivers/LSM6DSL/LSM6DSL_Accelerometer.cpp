#include "LSM6DSL_Accelerometer.h"

using namespace codal;

LSM6DSL_Accelerometer::LSM6DSL_Accelerometer( STM32I2C& i2c, uint8_t address, CoordinateSpace& coordinateSpace, uint16_t id)
    : LSM6DSL(i2c, address), Accelerometer( coordinateSpace, id) {

}

int LSM6DSL_Accelerometer::configure(){

    LSM6DSL::init();
    disableGyroscope();

    LSM6DSL_ACC_GYRO_ODR_XL_t odr = getBestODR(samplePeriod);
    LSM6DSL_ACC_GYRO_FS_XL_t fs = getBestFullScale(sampleRange);

    setAccelerometerFullScale(fs);
    setAccelerometerOutputDataRate(odr);

    isConfigured = true;

    return DEVICE_OK;
}

int LSM6DSL_Accelerometer::requestUpdate(){
    if( !isConfigured ) configure();

    auto result = getGyroscopeMeasure();
    sampleENU.x = result[0];
    sampleENU.y = result[1];
    sampleENU.z = result[2];

    return update();
}


LSM6DSL_ACC_GYRO_FS_XL_t LSM6DSL_Accelerometer::getBestFullScale(float value){

    if     ( value <= 2 ) return LSM6DSL_ACC_GYRO_FS_XL_2g;
    else if( value <= 4 ) return LSM6DSL_ACC_GYRO_FS_XL_4g;
    else if( value <= 8 ) return LSM6DSL_ACC_GYRO_FS_XL_8g;
    else                  return LSM6DSL_ACC_GYRO_FS_XL_16g;
}

LSM6DSL_ACC_GYRO_ODR_XL_t LSM6DSL_Accelerometer::getBestODR(float ms){

    float freq = 1 / (ms * 1000);

    if     ( freq <= 12.5 ) return LSM6DSL_ACC_GYRO_ODR_XL_13Hz;
    else if( freq <= 26 )   return LSM6DSL_ACC_GYRO_ODR_XL_26Hz;
    else if( freq <= 52 )   return LSM6DSL_ACC_GYRO_ODR_XL_52Hz;
    else if( freq <= 104 )  return LSM6DSL_ACC_GYRO_ODR_XL_104Hz;
    else if( freq <= 208 )  return LSM6DSL_ACC_GYRO_ODR_XL_208Hz;
    else if( freq <= 416 )  return LSM6DSL_ACC_GYRO_ODR_XL_416Hz;
    else if( freq <= 833 )  return LSM6DSL_ACC_GYRO_ODR_XL_833Hz;
    else if( freq <= 1666 ) return LSM6DSL_ACC_GYRO_ODR_XL_1660Hz;
    else if( freq <= 3332 ) return LSM6DSL_ACC_GYRO_ODR_XL_3330Hz;
    else                    return LSM6DSL_ACC_GYRO_ODR_XL_6660Hz;
}
