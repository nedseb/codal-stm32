#include "joystick.h"

#include "AnalogSensor.h"
#include "Button.h"
#include "Event.h"
#include "EventModel.h"
#include "STM32Pin.h"

using namespace codal;

Joystick::Joystick(STM32Pin& horizontalAxisPin, STM32Pin& verticalAxisPin, STM32Pin& buttonPin, uint8_t deadzone)
    : directionUserEvents({nullptr}), buttonUserEvents({nullptr})
{
    horizontalSensor = new AnalogSensor(horizontalAxisPin, 6666);
    verticalSensor   = new AnalogSensor(verticalAxisPin, 9999);
    button           = new Button(buttonPin, 7777);
    setDeadzone(deadzone);
}

void Joystick::setDeadzone(uint8_t newDeadzone)
{
    if (newDeadzone > 100) {
        newDeadzone = 100;
    }
    deadzone = round((float)newDeadzone / 100 * 512);
    setThresholds();
    return;
}

uint8_t Joystick::getDeadzone()
{
    return round((float)deadzone / 512 * 100);
}

int8_t Joystick::getAxis(JoystickAxis axis)
{
    uint16_t axisValue;
    if (axis == JoystickAxis::Horizontal)
        axisValue = horizontalSensor->readValue();
    else if (axis == JoystickAxis::Vertical)
        axisValue = verticalSensor->readValue();
    else
        return 0;

    if (axisValue > 512 + deadzone) {
        axisValue = (axisValue - 512 + deadzone) * (JoystickAxisRange - 0) / (1023 - 512 + deadzone) + 1;
        return axisValue;
    }
    else if (axisValue < 512 - deadzone) {
        axisValue = (axisValue - 0) * (0 - (-JoystickAxisRange)) / (512 - deadzone - 0) - 100;
        return axisValue;
    }
    else {
        return 0;
    }
}

bool Joystick::isJoystickPointingTo(const JoystickDirection direction)
{
    if (direction == JoystickDirection::Left && getAxis(JoystickAxis::Horizontal) > 0)
        return true;
    else if (direction == JoystickDirection::Top && getAxis(JoystickAxis::Vertical) > 0)
        return true;
    else if (direction == JoystickDirection::Right && getAxis(JoystickAxis::Horizontal) < 0)
        return true;
    else if (direction == JoystickDirection::Bottom && getAxis(JoystickAxis::Vertical) < 0)
        return true;
    else
        return false;
}

bool Joystick::isButtonPressed()
{
    return button->isPressed();
}

void Joystick::registerDirectionEvent(JoystickDirection direction, JoystickUserEvent handler)
{
    switch (direction) {
        case JoystickDirection::Left:
            listenToAxisEvent(JoystickDirection::Left, ANALOG_THRESHOLD_LOW, handler);
            break;
        case JoystickDirection::Top:
            listenToAxisEvent(JoystickDirection::Top, ANALOG_THRESHOLD_LOW, handler);
            break;
        case JoystickDirection::Right:
            listenToAxisEvent(JoystickDirection::Right, ANALOG_THRESHOLD_HIGH, handler);
            break;
        case JoystickDirection::Bottom:
            listenToAxisEvent(JoystickDirection::Bottom, ANALOG_THRESHOLD_HIGH, handler);
            break;
        default:
            break;
    }
}

void Joystick::registerButtonEvent(ButtonEvent btnEvent, JoystickUserEvent handler)
{
    switch (btnEvent) {
        case ButtonEvent::Click:
            listenToButtonEvent(ButtonEvent::Click, DEVICE_BUTTON_EVT_CLICK, handler);
            break;
        case ButtonEvent::LongClick:
            listenToButtonEvent(ButtonEvent::LongClick, DEVICE_BUTTON_EVT_LONG_CLICK, handler);
            break;
        case ButtonEvent::Up:
            listenToButtonEvent(ButtonEvent::Up, DEVICE_BUTTON_EVT_UP, handler);
            break;
        case ButtonEvent::Down:
            listenToButtonEvent(ButtonEvent::Down, DEVICE_BUTTON_EVT_DOWN, handler);
            break;
        case ButtonEvent::Hold:
            listenToButtonEvent(ButtonEvent::Hold, DEVICE_BUTTON_EVT_HOLD, handler);
            break;
        case ButtonEvent::DoubleClick:
            listenToButtonEvent(ButtonEvent::DoubleClick, DEVICE_BUTTON_EVT_DOUBLE_CLICK, handler);
            break;
        default:
            break;
    }
}

void Joystick::onEvent(Event event)
{
    if (event.source == horizontalSensor->id) {
        switch (event.value) {
            case ANALOG_THRESHOLD_LOW:
                directionUserEvents[static_cast<uint8_t>(JoystickDirection::Right)]();
                break;
            case ANALOG_THRESHOLD_HIGH:
                directionUserEvents[static_cast<uint8_t>(JoystickDirection::Left)]();
            default:
                break;
        }
    }
    else if (event.source == verticalSensor->id) {
        switch (event.value) {
            case ANALOG_THRESHOLD_LOW:
                directionUserEvents[static_cast<uint8_t>(JoystickDirection::Top)]();
                break;
            case ANALOG_THRESHOLD_HIGH:
                directionUserEvents[static_cast<uint8_t>(JoystickDirection::Bottom)]();
                break;
            default:
                break;
        }
    }
    else if (event.source == button->id) {
        switch (event.value) {
            case DEVICE_BUTTON_EVT_CLICK:
                buttonUserEvents[static_cast<uint8_t>(ButtonEvent::Click)]();
                break;
            case DEVICE_BUTTON_EVT_LONG_CLICK:
                buttonUserEvents[static_cast<uint8_t>(ButtonEvent::LongClick)]();
                break;
            case DEVICE_BUTTON_EVT_UP:
                buttonUserEvents[static_cast<uint8_t>(ButtonEvent::Up)]();
                break;
            case DEVICE_BUTTON_EVT_DOWN:
                buttonUserEvents[static_cast<uint8_t>(ButtonEvent::Down)]();
                break;
            case DEVICE_BUTTON_EVT_HOLD:
                buttonUserEvents[static_cast<uint8_t>(ButtonEvent::Hold)]();
                break;
            case DEVICE_BUTTON_EVT_DOUBLE_CLICK:
                buttonUserEvents[static_cast<uint8_t>(ButtonEvent::DoubleClick)]();
            default:
                break;
        }
    }
    else
        return;
}

void Joystick::setThresholds()
{
    horizontalSensor->setHighThreshold(512 + deadzone);
    horizontalSensor->setLowThreshold(512 - deadzone);
    verticalSensor->setHighThreshold(512 + deadzone);
    verticalSensor->setLowThreshold(512 - deadzone);
}

void Joystick::listenToButtonEvent(ButtonEvent enumEvent, uint8_t listenValue, JoystickUserEvent handler)
{
    if (buttonUserEvents[static_cast<uint8_t>(enumEvent)] == nullptr) {
        EventModel::defaultEventBus->listen(button->id, listenValue, this, &Joystick::onEvent);
    }
    buttonUserEvents[static_cast<uint8_t>(enumEvent)] = handler;
}

void Joystick::listenToAxisEvent(JoystickDirection direction, uint8_t listenValue, JoystickUserEvent handler)
{
    if (buttonUserEvents[static_cast<uint8_t>(direction)] == nullptr) {
        switch (direction) {
            case JoystickDirection::Left:
            case JoystickDirection::Right:
                EventModel::defaultEventBus->listen(horizontalSensor->id, listenValue, this, &Joystick::onEvent);
                break;
            case JoystickDirection::Bottom:
            case JoystickDirection::Top:
                EventModel::defaultEventBus->listen(verticalSensor->id, listenValue, this, &Joystick::onEvent);
                break;
            default:
                break;
        }
    }
    directionUserEvents[static_cast<uint8_t>(direction)] = handler;
}