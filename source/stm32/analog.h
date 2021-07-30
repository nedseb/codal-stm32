/**
 ******************************************************************************
 * @file    analog.h
 * @author  WI6LABS
 * @version V1.0.0
 * @date    01-August-2016
 * @brief   Header for analog module
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ANALOG_H
#define __ANALOG_H

/* Includes ------------------------------------------------------------------*/
#include "PeripheralPins.h"
#include "stm32_def.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TIMER_DISABLED,  // == TIM_OCMODE_TIMING           no output, useful for only-interrupt
    // Output Compare
    TIMER_OUTPUT_COMPARE,           // == Obsolete, use TIMER_DISABLED instead. Kept for compatibility reason
    TIMER_OUTPUT_COMPARE_ACTIVE,    // == TIM_OCMODE_ACTIVE           pin is set high when counter == channel compare
    TIMER_OUTPUT_COMPARE_INACTIVE,  // == TIM_OCMODE_INACTIVE         pin is set low when counter == channel compare
    TIMER_OUTPUT_COMPARE_TOGGLE,    // == TIM_OCMODE_TOGGLE           pin toggles when counter == channel compare
    TIMER_OUTPUT_COMPARE_PWM1,  // == TIM_OCMODE_PWM1             pin high when counter < channel compare, low otherwise
    TIMER_OUTPUT_COMPARE_PWM2,  // == TIM_OCMODE_PWM2             pin low when counter < channel compare, high otherwise
    TIMER_OUTPUT_COMPARE_FORCED_ACTIVE,    // == TIM_OCMODE_FORCED_ACTIVE    pin always high
    TIMER_OUTPUT_COMPARE_FORCED_INACTIVE,  // == TIM_OCMODE_FORCED_INACTIVE  pin always low

    // Input capture
    TIMER_INPUT_CAPTURE_RISING,    // == TIM_INPUTCHANNELPOLARITY_RISING
    TIMER_INPUT_CAPTURE_FALLING,   // == TIM_INPUTCHANNELPOLARITY_FALLING
    TIMER_INPUT_CAPTURE_BOTHEDGE,  // == TIM_INPUTCHANNELPOLARITY_BOTHEDGE

    // Used 2 channels for a single pin. One channel in TIM_INPUTCHANNELPOLARITY_RISING another channel in
    // TIM_INPUTCHANNELPOLARITY_FALLING. Channels must be used by pair: CH1 with CH2, or CH3 with CH4 This mode is very
    // useful for Frequency and Dutycycle measurement
    TIMER_INPUT_FREQ_DUTY_MEASUREMENT,

    TIMER_NOT_USED = 0xFFFF  // This must be the last item of this enum
} TimerModes_t;

typedef enum {
    TICK_FORMAT,  // default
    MICROSEC_FORMAT,
    HERTZ_FORMAT,
} TimerFormat_t;

typedef enum {
    RESOLUTION_1B_COMPARE_FORMAT = 1,  // used for Dutycycle: [0 .. 1]
    RESOLUTION_2B_COMPARE_FORMAT,      // used for Dutycycle: [0 .. 3]
    RESOLUTION_3B_COMPARE_FORMAT,      // used for Dutycycle: [0 .. 7]
    RESOLUTION_4B_COMPARE_FORMAT,      // used for Dutycycle: [0 .. 15]
    RESOLUTION_5B_COMPARE_FORMAT,      // used for Dutycycle: [0 .. 31]
    RESOLUTION_6B_COMPARE_FORMAT,      // used for Dutycycle: [0 .. 63]
    RESOLUTION_7B_COMPARE_FORMAT,      // used for Dutycycle: [0 .. 127]
    RESOLUTION_8B_COMPARE_FORMAT,      // used for Dutycycle: [0 .. 255]
    RESOLUTION_9B_COMPARE_FORMAT,      // used for Dutycycle: [0 .. 511]
    RESOLUTION_10B_COMPARE_FORMAT,     // used for Dutycycle: [0 .. 1023]
    RESOLUTION_11B_COMPARE_FORMAT,     // used for Dutycycle: [0 .. 2047]
    RESOLUTION_12B_COMPARE_FORMAT,     // used for Dutycycle: [0 .. 4095]
    RESOLUTION_13B_COMPARE_FORMAT,     // used for Dutycycle: [0 .. 8191]
    RESOLUTION_14B_COMPARE_FORMAT,     // used for Dutycycle: [0 .. 16383]
    RESOLUTION_15B_COMPARE_FORMAT,     // used for Dutycycle: [0 .. 32767]
    RESOLUTION_16B_COMPARE_FORMAT,     // used for Dutycycle: [0 .. 65535]

    TICK_COMPARE_FORMAT = 0x80,  // default
    MICROSEC_COMPARE_FORMAT,
    HERTZ_COMPARE_FORMAT,
    PERCENT_COMPARE_FORMAT,  // used for Dutycycle
} TimerCompareFormat_t;

/* Exported functions ------------------------------------------------------- */
void dac_write_value(PinName pin, uint32_t value, uint8_t do_init);
void dac_stop(PinName pin);
uint16_t adc_read_value(PinName pin, uint32_t resolution);
#if defined(HAL_TIM_MODULE_ENABLED) && !defined(HAL_TIM_MODULE_ONLY)
void pwm_start(PinName pin, uint32_t clock_freq, uint32_t value, TimerCompareFormat_t resolution);
void pwm_stop(PinName pin);
#endif
uint32_t get_pwm_channel(PinName pin);

#ifdef __cplusplus
}
#endif

#endif /* __ANALOG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
