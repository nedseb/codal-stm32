#include "STM32I2C.h"
#include "PeripheralPins.h"

using namespace codal;

STM32I2C::STM32I2C(STM32Pin& sda, STM32Pin& scl) : I2C(sda, scl) {

    //if( sda.name != PinNumber::I2C2_SDA ){ return; }

    /*IC2x_Init_Handler(&i2c);

    void * inst = 0;
    inst = codal_setup_pin((PinName)sda.name, inst, PinMap_I2C_SDA);
    inst = codal_setup_pin((PinName)scl.name, inst, PinMap_I2C_SCL);
    i2c.Instance = (I2C_TypeDef *)inst;

    i2c.Init.Timing = 100000;

    I2Cx_Init(&i2c);*/
    
    _i2c.sda = (PinName)sda.name;
    _i2c.scl = (PinName)scl.name;
    _i2c.isMaster = 1;
    _i2c.generalCall = 0;

    //i2c_init(&_i2c);

    /**Configure Analogue filter */
    // HAL_I2CEx_ConfigAnalogFilter(&_i2c.handle, I2C_ANALOGFILTER_ENABLE);  
}

void STM32I2C::begin(){
    i2c_init(&_i2c);
    //i2c_custom_init(&_i2c, 100000, I2C_ADDRESSINGMODE_7BIT, 0x33);
}

void STM32I2C::end(){
    i2c_deinit(&_i2c);
}

int STM32I2C::setFrequency(uint32_t frequency) {
    return DEVICE_OK;
}

int STM32I2C::write(uint16_t address, uint8_t* data, int len, bool repeated) {

    // int ret = DEVICE_I2C_ERROR;

    // auto res = HAL_I2C_Master_Transmit(&(_i2c.handle), address, data, len, HAL_MAX_DELAY);
    // if (res == HAL_OK)
    //     return DEVICE_OK;
    
    // return ret;

    // uint32_t XferOptions = _i2c.handle.XferOptions;
    // HAL_I2C_Master_Seq_Transmit_IT(&(_i2c.handle), address, data, len, XferOptions);

    // return len;

    _i2c.handle.XferOptions = I2C_OTHER_AND_LAST_FRAME;
    auto res = i2c_master_write(&_i2c, address, data, len);

    if( res != I2C_OK ){
       return DEVICE_I2C_ERROR;
    }

    return len;
}

int STM32I2C::read(uint16_t address, uint8_t* data, int len, bool repeated) {
    if (data == NULL || len <= 0)
        return DEVICE_INVALID_PARAMETER;

    int ret = DEVICE_I2C_ERROR;

    return ret;
}

int STM32I2C::readRegister(uint16_t address, uint8_t reg, uint8_t* data, int length, bool repeated) {
    if (data == NULL || length <= 0)
        return DEVICE_INVALID_PARAMETER;
    return 0;
}

int STM32I2C::writeRegister(uint16_t address, uint8_t reg, uint8_t value) {
    return 0;
}

void * STM32I2C::setup_pin(PinName pin, const PinMap *map)
{
    void * tmp = pinmap_peripheral(pin, map);
    pin_function(pin, pinmap_function(pin, map));
    return tmp;
}
