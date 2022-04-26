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

    /**
     * @brief Gets the analog value of the given sensor, in range [-100, 100]
     *
     * @param sensor the sensor to get value of
     *
     * @return int8_t
     */
    int8_t getAxis(codal::AnalogSensor* sensor);

    /**
     * @brief checks if the joystick is oriented in the given direction
     *
     * @param direction
     * @return true
     * @return false
     */
    bool isJoystickPointingTo(const JoystickDirection direction);

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