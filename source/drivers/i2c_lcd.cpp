
#include "i2c_lcd.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "codal_target_hal.h"

using namespace codal;

constexpr uint8_t COMMAND_CLEAR          = 0x01;
constexpr uint8_t COMMAND_HOME           = 0x02;
constexpr uint8_t COMMAND_ENTRY_MODE     = 0x04;
constexpr uint8_t COMMAND_DISPLAY        = 0x08;
constexpr uint8_t COMMAND_FUNCTION       = 0x20;
constexpr uint8_t COMMAND_SET_DDRAM_ADDR = 0x80;

constexpr uint8_t DISPLAY_SCREEN_ON = 0x04;
constexpr uint8_t DISPLAY_CURSOR_ON = 0x02;
constexpr uint8_t DISPLAY_BLINK_ON  = 0x01;

constexpr uint8_t FUNCTION_8_BITS    = 0x10;
constexpr uint8_t FUNCTION_2_LINES   = 0x08;
constexpr uint8_t FUNCTION_5X10_DOTS = 0x04;

constexpr uint8_t PIN_BACKLIGHT = 0x08;
constexpr uint8_t PIN_E         = 0x04;
constexpr uint8_t PIN_RW        = 0x02;
constexpr uint8_t PIN_RS        = 0x01;

constexpr uint8_t ROWS_OFFSET[4] = {0x00, 0x40, 0x14, 0x54};

I2C_LCD::I2C_LCD(STM32I2C& i2c, uint16_t address, uint8_t cols, uint8_t rows)
    : i2c(i2c), address(address), cols(cols), rows(rows)
{
    entryMode      = 0x02;
    displayControl = 0x00;
    functionSet    = FUNCTION_2_LINES;
    backlight      = false;
}

void I2C_LCD::init()
{
    target_wait(1000);

    write4bits(0x03, true);
    target_wait_us(4500);

    write4bits(0x03, true);
    target_wait_us(4500);

    write4bits(0x03, true);
    target_wait_us(150);

    write4bits(0x02, true);

    sendCommand(COMMAND_FUNCTION | functionSet);

    displayOn();
    clear();
    sendCommand(COMMAND_ENTRY_MODE | entryMode);
    home();
}

void I2C_LCD::displayOn()
{
    displayControl |= DISPLAY_SCREEN_ON;
    sendCommand(COMMAND_DISPLAY | displayControl);
}

void I2C_LCD::displayOff()
{
    displayControl &= ~DISPLAY_SCREEN_ON;
    sendCommand(COMMAND_DISPLAY | displayControl);
}

void I2C_LCD::cursorOn()
{
    displayControl |= DISPLAY_CURSOR_ON;
    sendCommand(COMMAND_DISPLAY | displayControl);
}

void I2C_LCD::cursorOff()
{
    displayControl &= ~DISPLAY_CURSOR_ON;
    sendCommand(COMMAND_DISPLAY | displayControl);
}

void I2C_LCD::blinkOn()
{
    displayControl |= DISPLAY_BLINK_ON;
    sendCommand(COMMAND_DISPLAY | displayControl);
}

void I2C_LCD::blinkOff()
{
    displayControl &= ~DISPLAY_CURSOR_ON;
    sendCommand(COMMAND_DISPLAY | displayControl);
}

void I2C_LCD::backlightOn()
{
    backlight = true;
    writeExpender(0x00 | PIN_BACKLIGHT);
}

void I2C_LCD::backlightOff()
{
    backlight = false;
    writeExpender(0x00 & ~PIN_BACKLIGHT);
}

void I2C_LCD::clear()
{
    sendCommand(COMMAND_CLEAR);
    target_wait(5);
}

void I2C_LCD::home()
{
    sendCommand(COMMAND_HOME);
    target_wait(5);
}

void I2C_LCD::setCursor(uint8_t col, uint8_t row)
{
    if (row > rows) {
        row = rows - 1;  // we count rows starting w/0
    }

    sendCommand(COMMAND_SET_DDRAM_ADDR | (col + ROWS_OFFSET[row]));
}

void I2C_LCD::print(char c)
{
    send((uint8_t)c, false);
}

void I2C_LCD::print(const char* str)
{
    for (size_t i = 0; i < strlen(str); i++) {
        print(str[i]);
    }
}

void I2C_LCD::toBinString(char* str, uint32_t value, uint8_t size)
{
    bool skip   = true;
    uint8_t pos = 0;
    for (int8_t i = size - 1; i >= 0; --i) {
        if ((value & (1 << i)) == 0) {
            if (!skip) str[pos++] = '0';
        }
        else {
            str[pos++] = '1';
            skip       = false;
        }
    }

    str[pos] = '\0';
}

void I2C_LCD::print(float value, uint8_t decimal)
{
    char tmp[32] = {0};
    sprintf(tmp, "%.*f", decimal, value);
    print(tmp);
}

void I2C_LCD::send(uint8_t value, bool isCommand)
{
    uint8_t high = (value >> 4) & 0x0F;
    uint8_t low  = value & 0x0F;

    write4bits(high, isCommand);
    write4bits(low, isCommand);
}
void I2C_LCD::write4bits(uint8_t data, bool isCommand)
{
    uint8_t payload = (data & 0x0F) << 4;
    payload |= (backlight ? PIN_BACKLIGHT : 0);
    payload |= (isCommand ? 0 : PIN_RS);

    writeExpender(payload & ~PIN_E);
    target_wait_us(1);

    writeExpender(payload | PIN_E);
    target_wait_us(1);

    writeExpender(payload & ~PIN_E);
    target_wait_us(50);
}

void I2C_LCD::writeExpender(uint8_t data)
{
    i2c.beginTransmission(address);
    i2c.write(data);
    i2c.endTransmission();
}
