#include "STM32I2C.h"
#include "PeripheralPins.h"

using namespace codal;

STM32I2C::STM32I2C(STM32Pin& sda, STM32Pin& scl) : I2C(sda, scl) {
    IC2x_Init_Handler(&i2c);

    void * inst = 0;
    inst = codal_setup_pin((PinName)sda.name, inst, PinMap_I2C_SDA);
    inst = codal_setup_pin((PinName)scl.name, inst, PinMap_I2C_SCL);
    i2c.Instance = (I2C_TypeDef *)inst;

    i2c.Init.Timing = 100000;

    I2Cx_Init(&i2c);
}

int STM32I2C::setFrequency(uint32_t frequency) {
    return DEVICE_OK;
}

int STM32I2C::write(uint16_t address, uint8_t* data, int len, bool repeated) {

    if (data == NULL || len <= 0)
        return DEVICE_INVALID_PARAMETER;

    int ret = DEVICE_I2C_ERROR;

    auto res = HAL_I2C_Master_Transmit(&i2c, address, data, len, HAL_MAX_DELAY);
    if (res == HAL_OK)
        return DEVICE_OK;
    
    return ret;
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


void * STM32I2C::codal_setup_pin(PinName pin, void * prev, const PinMap *map)
{
    void * tmp = pinmap_peripheral(pin, map);
    pin_function(pin, pinmap_function(pin, map));
    //pin_mode(pin, PullNone);
    //CODAL_ASSERT(!prev || prev == tmp, 99);
    return tmp;
}