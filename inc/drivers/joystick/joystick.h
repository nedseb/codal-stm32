#pragma once

#include "AnalogSensor.h"
#include "Button.h"
#include "STM32Pin.h"

namespace codal {
typedef void (*handler)();

enum JoystickDirection { Left = 0, Top = 1, Right = 2, Bottom = 3 };
enum JoystickAxis { Horizontal, Vertical };
enum ButtonEvent { Click = 0, LongClick = 1, Up = 2, Down = 3, Hold = 4, DoubleClick = 5 };

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

    /**
     * @brief checks if the joystick's button is pressed
     *
     * @return true
     * @return false
     */
    bool isButtonPressed() { return button->isPressed(); }

    /**
     * @brief Registers a new event that triggers when the joystick is pointed in a specified direction
     *
     * @param direction the direction that triggers the event
     * @param handler the user function to execute when the event is triggered
     */
    void registerDirectionEvent(JoystickDirection direction, handler handler);

  private:
    codal::AnalogSensor* horizontalSensor;
    codal::AnalogSensor* verticalSensor;
    codal::Button* button;
    uint8_t deadzone;
    handler directionHandlers[4];
    handler buttonHandlers[6];

    void onEvent(codal::Event event);

    /** @brief Utility function used to factorize the joystick's axis registering method
     *
     * @param direction the type of event that will be registered
     * @param listenValue the value parameter of the listen function
     * @param handler the user function that will be executed when the event is triggered
     */
    void listenToAxisEvent(JoystickDirection direction, uint8_t listenValue, handler handler);

    /**
     * @brief Sets the Thresholds of the AnalogSensors
     *
     */
    void setThresholds();
};
}  // namespace codal