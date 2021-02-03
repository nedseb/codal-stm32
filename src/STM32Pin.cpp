#include "STM32Pin.h"

#include "PeripheralPins.h"
#include "PinConfigured.h"
#include "analog.h"
#include "digital_io.h"
#include "pins_arduino.h"

using namespace codal;

#ifndef DEVICE_DEFAULT_PWM_PERIOD
#define DEVICE_DEFAULT_PWM_PERIOD 20000
#endif

static uint16_t adc_offset = 0;

#if !defined(ADC_RESOLUTION_16B)
#define MAX_ADC_RESOLUTION 12
#else
#define MAX_ADC_RESOLUTION 16
#endif
#define MAX_PWM_RESOLUTION 16

static int _readResolution = ADC_RESOLUTION;
static int _internalReadResolution =
#if ADC_RESOLUTION > MAX_ADC_RESOLUTION
    MAX_ADC_RESOLUTION
#else

#ifdef ADC_RESOLUTION_12B

#if ADC_RESOLUTION <= 6 && defined(ADC_RESOLUTION_6B)
    6
#elif ADC_RESOLUTION <= 8
    8
#elif ADC_RESOLUTION <= 10
    10
#elif ADC_RESOLUTION <= 12
    12
#elif ADC_RESOLUTION <= 14 && defined(ADC_RESOLUTION_14B)
    14
#elif defined(ADC_RESOLUTION_16B)
    16
#endif
#else  /* ADC_RESOLUTION_12B */
    12
#endif /* ADC_RESOLUTION_12B */
#endif /* ADC_RESOLUTION > MAX_ADC_RESOLUTION */
    ;

static int _writeResolution = PWM_RESOLUTION;
static int _internalWriteResolution =
#if PWM_RESOLUTION > MAX_PWM_RESOLUTION
    MAX_PWM_RESOLUTION
#else
    PWM_RESOLUTION
#endif
    ;

static uint32_t _writeFreq = PWM_FREQUENCY;

extern uint32_t g_anOutputPinConfigured[MAX_NB_PORT];

static inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to)
{
    if (from != to) {
        if (from > to) {
            value = (value < (uint32_t)(1 << (from - to))) ? 0 : ((value + 1) >> (from - to)) - 1;
        }
        else {
            if (value != 0) {
                value = ((value + 1) << (to - from)) - 1;
            }
        }
    }
    return value;
}

STM32Pin::STM32Pin(int id, PinNumber name, PinCapability capability) : codal::Pin(id, name, capability)
{
    this->pullMode = DEVICE_DEFAULT_PULLMODE;

    // Power up in a disconnected, low power state.
    // If we're unused, this is how it will stay...
    this->status = 0x00;
}

inline int map(codal::PullMode pinMode)
{
    switch (pinMode) {
        case PullMode::Up:
            return GPIO_PULLUP;
        case PullMode::Down:
            return GPIO_PULLDOWN;
        case PullMode::None:
            return GPIO_NOPULL;
    }

    return GPIO_NOPULL;
}

inline PinName to_pinName(PinNumber pin)
{
    return (PinName)pin;
}

void STM32Pin::disconnect()
{
    PinName p = to_pinName(this->name);
    if (p == NC) return;

// If the pin that support PWM or DAC output, we need to turn it off
#if (defined(HAL_DAC_MODULE_ENABLED) && !defined(HAL_DAC_MODULE_ONLY)) || \
    (defined(HAL_TIM_MODULE_ENABLED) && !defined(HAL_TIM_MODULE_ONLY))
    if (is_pin_configured(p, g_anOutputPinConfigured)) {
#if defined(HAL_DAC_MODULE_ENABLED) && !defined(HAL_DAC_MODULE_ONLY)
        if (pin_in_pinmap(p, PinMap_DAC)) {
            dac_stop(p);
        }
        else
#endif  // HAL_DAC_MODULE_ENABLED && !HAL_DAC_MODULE_ONLY

#if defined(HAL_TIM_MODULE_ENABLED) && !defined(HAL_TIM_MODULE_ONLY)
            if (pin_in_pinmap(p, PinMap_PWM)) {
            pwm_stop(p);
        }
#endif  // HAL_TIM_MODULE_ENABLED && !HAL_TIM_MODULE_ONLY

        reset_pin_configured(p, g_anOutputPinConfigured);
#endif
        status = 0;
    }
}

int STM32Pin::setDigitalValue(int value)
{
    // Ensure we have a valid value.
    if (value < 0 || value > 1) return DEVICE_INVALID_PARAMETER;

    // Move into a Digital input state if necessary.
    if (!(this->status & IO_STATUS_DIGITAL_OUT)) {
        disconnect();
        pin_function(to_pinName(this->name), STM_PIN_DATA(STM_MODE_OUTPUT_PP, map(this->pullMode), 0));
        this->status |= IO_STATUS_DIGITAL_OUT;
    }

    digital_io_write(get_GPIO_Port(STM_PORT(this->name)), STM_LL_GPIO_PIN(this->name), value);
    return DEVICE_OK;
}

int STM32Pin::getDigitalValue()
{
    // Move into a Digital input state if necessary.
    if (!(status & (IO_STATUS_DIGITAL_IN | IO_STATUS_EVENT_ON_EDGE | IO_STATUS_EVENT_PULSE_ON_EDGE))) {
        disconnect();
        pin_function(to_pinName(this->name), STM_PIN_DATA(STM_PIN_INPUT, map(this->pullMode), 0));
        status |= IO_STATUS_DIGITAL_IN;
    }

    uint8_t level = 0;
    level         = digital_io_read(get_GPIO_Port(STM_PORT(this->name)), STM_LL_GPIO_PIN(this->name));

    return (level) ? HIGH : LOW;
}

int STM32Pin::setAnalogValue(int value)
{
    // sanitise the level value
    if (value < 0 || value > DEVICE_PIN_MAX_OUTPUT) return DEVICE_INVALID_PARAMETER;

#if defined(HAL_DAC_MODULE_ENABLED) && !defined(HAL_DAC_MODULE_ONLY)
    uint8_t do_init = 0;
#endif
    PinName p = to_pinName(this->name);
    if (p != NC) {
#if defined(HAL_DAC_MODULE_ENABLED) && !defined(HAL_DAC_MODULE_ONLY)
        if (pin_in_pinmap(p, PinMap_DAC)) {
            if (is_pin_configured(p, g_anOutputPinConfigured) == false) {
                do_init = 1;
                set_pin_configured(p, g_anOutputPinConfigured);
            }
            value = mapResolution(value, _writeResolution, DACC_RESOLUTION);
            dac_write_value(p, value, do_init);
        }
        else
#endif  // HAL_DAC_MODULE_ENABLED && !HAL_DAC_MODULE_ONLY
#if defined(HAL_TIM_MODULE_ENABLED) && !defined(HAL_TIM_MODULE_ONLY)
            if (pin_in_pinmap(p, PinMap_PWM)) {
            if (is_pin_configured(p, g_anOutputPinConfigured) == false) {
                set_pin_configured(p, g_anOutputPinConfigured);
            }
            value = mapResolution(value, _writeResolution, _internalWriteResolution);
            pwm_start(p, _writeFreq, value, (TimerCompareFormat_t)_internalWriteResolution);
        }
        else
#endif /* HAL_TIM_MODULE_ENABLED && !HAL_TIM_MODULE_ONLY */
        {
            // DIGITAL PIN ONLY
            // Defaults to digital write
            pin_function(to_pinName(this->name), STM_PIN_DATA(STM_MODE_OUTPUT_PP, map(this->pullMode), 0));
            value = mapResolution(value, _writeResolution, 8);
            if (value < 128) {
                setDigitalValue(LOW);
            }
            else {
                setDigitalValue(HIGH);
            }
        }
    }
    return DEVICE_OK;
}

int STM32Pin::setServoValue(int value, int range, int center)
{
    // check if this pin has an analogue mode...
    if (!(PIN_CAPABILITY_ANALOG & capability)) return DEVICE_NOT_SUPPORTED;

    // sanitise the servo level
    if (value < 0 || range < 1 || center < 1) return DEVICE_INVALID_PARAMETER;

    // clip - just in case
    if (value > DEVICE_PIN_MAX_SERVO_RANGE) value = DEVICE_PIN_MAX_SERVO_RANGE;

    // calculate the lower bound based on the midpoint
    int lower = (center - (range / 2)) * 1000;

    value = value * 1000;

    // add the percentage of the range based on the value between 0 and 180
    int scaled = lower + (range * (value / DEVICE_PIN_MAX_SERVO_RANGE));

    return setServoPulseUs(scaled / 1000);
}

int STM32Pin::getAnalogValue()
{
    // check if this pin has an analogue mode...
    if (!(PIN_CAPABILITY_ANALOG & capability)) return DEVICE_NOT_SUPPORTED;

    if (!(status & IO_STATUS_ANALOG_IN)) {
        disconnect();
        pin_function(to_pinName(this->name), STM_PIN_DATA(STM_PIN_ANALOG, map(this->pullMode), 0));

        status = IO_STATUS_ANALOG_IN;
    }

    uint32_t value = 0;
#if defined(HAL_ADC_MODULE_ENABLED) && !defined(HAL_ADC_MODULE_ONLY)
    PinName p = to_pinName(this->name);
    if (p != NC) {
        value = adc_read_value(p, _internalReadResolution);
        value = mapResolution(value, _internalReadResolution, _readResolution);
    }
#endif

    if (value <= 0)
        adc_offset = value;
    else if (value > 4095)
        adc_offset = value - 4095;

    return (value - adc_offset) >> 2;
}

int STM32Pin::isTouched()
{
    return DEVICE_NOT_SUPPORTED;
}

int STM32Pin::setServoPulseUs(uint32_t pulseWidthUs)
{
    // check if this pin has an analogue mode...
    if (!(PIN_CAPABILITY_ANALOG & capability)) return DEVICE_NOT_SUPPORTED;

    PinName p = to_pinName(this->name);
    if (p != NC) {
#if defined(HAL_TIM_MODULE_ENABLED) && !defined(HAL_TIM_MODULE_ONLY)
        if (pin_in_pinmap(p, PinMap_PWM)) {
            if (is_pin_configured(p, g_anOutputPinConfigured) == false) {
                set_pin_configured(p, g_anOutputPinConfigured);
            }
            pwm_start(p, _writeFreq, pulseWidthUs, TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
        }
#endif
    }

    return DEVICE_OK;
}

int STM32Pin::setAnalogPeriod(int period)
{
    return 0;
}
int STM32Pin::setAnalogPeriodUs(uint32_t period)
{
    return 0;
}
uint32_t STM32Pin::getAnalogPeriodUs()
{
    return 0;
}
int STM32Pin::setPull(PullMode pull)
{
    if (pullMode == pull) return DEVICE_OK;

    pullMode = pull;

    // have to disconnect to flush the change to the hardware
    disconnect();
    getDigitalValue();

    return DEVICE_OK;
}

int STM32Pin::eventOn(int eventType)
{
    switch (eventType) {
        case DEVICE_PIN_INTERRUPT_ON_EDGE:
        case DEVICE_PIN_EVENT_ON_EDGE:
        case DEVICE_PIN_EVENT_ON_PULSE:
            enableRiseFallEvents(eventType);
            break;

        case DEVICE_PIN_EVENT_NONE:
            disableEvents();
            break;

        default:
            return DEVICE_INVALID_PARAMETER;
    }

    return DEVICE_OK;
}

int STM32Pin::enableRiseFallEvents(int eventType)
{
    return DEVICE_NOT_IMPLEMENTED;
}

int STM32Pin::disableEvents()
{
    if (status & (IO_STATUS_EVENT_ON_EDGE | IO_STATUS_EVENT_PULSE_ON_EDGE | IO_STATUS_TOUCH_IN)) {
        disconnect();
        getDigitalValue();
    }

    return DEVICE_OK;
}