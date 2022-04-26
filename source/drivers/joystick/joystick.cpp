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