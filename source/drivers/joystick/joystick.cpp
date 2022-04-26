#include "joystick.h"

#include "AnalogSensor.h"
#include "Button.h"
#include "STM32Pin.h"

using namespace codal;

Joystick::Joystick(codal::STM32Pin& horizontalAxisPin, codal::STM32Pin& verticalAxisPin, codal::STM32Pin& buttonPin,
                   uint8_t deadzone)
{
    horizontalSensor = new AnalogSensor(horizontalAxisPin, 6666);
    verticalSensor   = new AnalogSensor(verticalAxisPin, 9999);
    button           = new Button(buttonPin, 7777);
}

void Joystick::setDeadzone(uint8_t newDeadzone)
{
    if (newDeadzone > 100) {
        newDeadzone = 100;
    }
    this->deadzone = round((float)newDeadzone / 100 * 512);
    setThresholds();
    return;
}

uint8_t Joystick::getDeadzone()
{
    return round((float)this->deadzone / 512 * 100);
}

int8_t Joystick::getAxis(codal::AnalogSensor* sensor)
{
    uint16_t axisValue = sensor->readValue();
    if (axisValue > 512 + this->deadzone) {
        axisValue = (axisValue - 512 + this->deadzone) * (100 - 0) / (1023 - 512 + this->deadzone) + 1;
        return axisValue;
    }
    else if (axisValue < 512 - Joystick::deadzone) {
        axisValue = (axisValue - 0) * (0 - (-100)) / (512 - this->deadzone - 0) - 100;
        return axisValue;
    }
    else {
        return 0;
    }
}

bool Joystick::isJoystickPointingTo(const JoystickDirection direction)
{
    switch (direction) {
        case JoystickDirection::Left:
            if (getAxis(horizontalSensor) > 0) return true;
            break;
        case JoystickDirection::Top:
            if (getAxis(verticalSensor) > 0) return true;
            break;
        case JoystickDirection::Right:
            if (getAxis(horizontalSensor) < 0) return true;
            break;
        case JoystickDirection::Bottom:
            if (getAxis(verticalSensor) < 0) return true;
            break;
        default:
            break;
    }
    return false;
}

void Joystick::setThresholds()
{
    this->horizontalSensor->setHighThreshold(512 + deadzone);
    this->horizontalSensor->setLowThreshold(512 - deadzone);
    this->verticalSensor->setHighThreshold(512 + deadzone);
    this->verticalSensor->setLowThreshold(512 - deadzone);
}