#pragma once

#include "AnalogSensor.h"
#include "Button.h"
#include "STM32Pin.h"

namespace codal {
enum JoystickDirection { Left = 0, Top = 1, Right = 2, Bottom = 3 };
enum JoystickAxis { Horizontal, Vertical };

class Joystick {
  public:
    /**
     * @brief Joystick object constructor
     *
     * @param horizontalAxisPin the pin connected to the horizontal axis of the joystick
     * @param verticalAxisPin the pin connected to the horizontal axis of the joystick
     * @param buttonPin the pin connected to the button of the joystick
     * @param deadzone the joystick's deadzone, which is the amountthe joystick can move before an input is recognized
     */
    Joystick(codal::STM32Pin& horizontalAxisPin, codal::STM32Pin& verticalAxisPin, codal::STM32Pin& buttonPin,
             uint8_t deadzone = 10);

    ~Joystick() {}

    /**
     * @brief Sets the deadzone of the joystick
     *
     * @param newDeadzone deadzone's new value in range [0, 100]
     *
     * @return void
     */
    void setDeadzone(uint8_t newDeadzone);

    /**
     * @brief Get the deadzone value in range [0, 100]
     *
     * @return uint8_t
     */
    uint8_t getDeadzone();

  private:
    codal::AnalogSensor* horizontalSensor;
    codal::AnalogSensor* verticalSensor;
    codal::Button* button;
    uint8_t deadzone;

    /**
     * @brief Sets the Thresholds of the AnalogSensors
     *
     */
    void setThresholds();
};
}  // namespace codal