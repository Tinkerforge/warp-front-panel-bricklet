
/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * led.c: Driver for RG LED
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "led.h"

#include "configs/config_led.h"
#include "bricklib2/hal/ccu4_pwm/ccu4_pwm.h"
#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/logging/logging.h"
#include "communication.h"
#include "bricklib2/utility/util_definitions.h"

#include "xmc_gpio.h"
#include "xmc_ccu4.h"
#include "xmc_scu.h"

#include "cie1931.h"

LED led;

XMC_CCU4_SLICE_t *const slice[4] = {
	CCU41_CC40,
	CCU41_CC41,
	CCU41_CC42,
	CCU41_CC43,
};

// Changes period. This does not do shadow transfer, always call set_duty_cycle afterwards!
void led_ccu4_pwm_set_period(const uint8_t ccu4_slice_number, const uint16_t period_value) {
    XMC_CCU4_SLICE_SetTimerPeriodMatch(slice[ccu4_slice_number], period_value);
}

uint16_t led_ccu4_pwm_get_period(const uint8_t ccu4_slice_number) {
    return XMC_CCU4_SLICE_GetTimerPeriodMatch(slice[ccu4_slice_number]);
}

// Compare value is a value from 0 to period_value (^= 0 to 100% duty cycle)
void led_ccu4_pwm_set_duty_cycle(const uint8_t ccu4_slice_number, const uint16_t compare_value) {
	XMC_CCU4_SLICE_SetTimerCompareMatch(slice[ccu4_slice_number], compare_value);
    XMC_CCU4_EnableShadowTransfer(CCU41, (XMC_CCU4_SHADOW_TRANSFER_SLICE_0 << (ccu4_slice_number*4)) |
    		                             (XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_0 << (ccu4_slice_number*4)));
}

uint16_t led_ccu4_pwm_get_duty_cycle(const uint8_t ccu4_slice_number) {
	return XMC_CCU4_SLICE_GetTimerCompareMatch(slice[ccu4_slice_number]);
}

// Period value is the amount of clock cycles per period
void led_ccu4_pwm_init(XMC_GPIO_PORT_t *const port, const uint8_t pin, const uint8_t ccu4_slice_number, const uint16_t period_value) {
	const XMC_CCU4_SLICE_COMPARE_CONFIG_t compare_config = {
		.timer_mode          = XMC_CCU4_SLICE_TIMER_COUNT_MODE_EA,
		.monoshot            = false,
		.shadow_xfer_clear   = 0,
		.dither_timer_period = 0,
		.dither_duty_cycle   = 0,
		.prescaler_mode      = XMC_CCU4_SLICE_PRESCALER_MODE_NORMAL,
		.mcm_enable          = 0,
		.prescaler_initval   = 0,
		.float_limit         = 0,
		.dither_limit        = 0,
		.passive_level       = ccu4_slice_number == 1 ? XMC_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_LOW : XMC_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_HIGH,
		.timer_concatenation = 0
	};

	const XMC_GPIO_CONFIG_t gpio_out_config	= {
		.mode                = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT9,
		.input_hysteresis    = XMC_GPIO_INPUT_HYSTERESIS_STANDARD,
		.output_level        = XMC_GPIO_OUTPUT_LEVEL_LOW,
	};

	const XMC_CCU4_SLICE_EVENT_CONFIG_t event_config = {
		 .duration = XMC_CCU4_SLICE_EVENT_FILTER_5_CYCLES,
		 .edge = XMC_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_RISING_EDGE,
		 .level = XMC_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_HIGH,
		 .mapped_input = XMC_CCU4_SLICE_INPUT_AI
	};

    XMC_CCU4_Init(CCU41, XMC_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR);
    XMC_CCU4_StartPrescaler(CCU41);
    XMC_CCU4_SLICE_CompareInit(slice[ccu4_slice_number], &compare_config);

    // Set the period and compare register values
    XMC_CCU4_SLICE_SetTimerPeriodMatch(slice[ccu4_slice_number], period_value);
    XMC_CCU4_SLICE_SetTimerCompareMatch(slice[ccu4_slice_number], 0);

    XMC_CCU4_EnableShadowTransfer(CCU41, (XMC_CCU4_SHADOW_TRANSFER_SLICE_0 << (ccu4_slice_number*4)) |
    		                             (XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_0 << (ccu4_slice_number*4)));

    XMC_GPIO_Init(port, pin, &gpio_out_config);

    XMC_CCU4_SLICE_ConfigureEvent(slice[ccu4_slice_number], XMC_CCU4_SLICE_EVENT_1, &event_config);
    XMC_CCU4_SLICE_StartConfig(slice[ccu4_slice_number], XMC_CCU4_SLICE_EVENT_1, XMC_CCU4_SLICE_START_MODE_TIMER_START_CLEAR);

    XMC_CCU4_EnableClock(CCU41, ccu4_slice_number);
    XMC_CCU4_SLICE_StartTimer(slice[ccu4_slice_number]);
}

void led_init(void) {
	const XMC_GPIO_CONFIG_t output_config = {
		.mode         = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
		.output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
	};

	XMC_GPIO_Init(LED_R_PIN, &output_config);
	XMC_GPIO_Init(LED_G_PIN, &output_config);

	led_ccu4_pwm_init(LED_R_PIN, LED_R_CCU4_SLICE, LED_PERIOD_VALUE-1);
	led_ccu4_pwm_init(LED_G_PIN, LED_G_CCU4_SLICE, LED_PERIOD_VALUE-1);
	XMC_SCU_SetCcuTriggerHigh(SCU_GENERAL_CCUCON_GSC41_Msk);

	memset(&led, 0, sizeof(LED));

	// Use yellow during startup until ESP boots
	led.pattern = WARP_FRONT_PANEL_LED_PATTERN_ON;
	led.color   = WARP_FRONT_PANEL_LED_COLOR_YELLOW;
}

void led_set_color_value(const uint8_t value) {
	if(led.color == WARP_FRONT_PANEL_LED_COLOR_RED) {
		led.set_r = value;
		led.set_g = 0;
	} else if(led.color == WARP_FRONT_PANEL_LED_COLOR_GREEN) {
		led.set_r = 0;
		led.set_g = value;
	} else if(led.color == WARP_FRONT_PANEL_LED_COLOR_YELLOW) {
		led.set_r = value*203/255; // 203 is 50% pwm, which is maximum possible for yellow
		led.set_g = value*203/255;
	}
}

void led_tick(void) {
	static uint8_t last_r = 0;
	static uint8_t last_g = 0;

	if(led.pattern == WARP_FRONT_PANEL_LED_PATTERN_OFF) {
		led_set_color_value(0);
	} else if(led.pattern == WARP_FRONT_PANEL_LED_PATTERN_ON) {
		led_set_color_value(255);
	} else if(led.pattern == WARP_FRONT_PANEL_LED_PATTERN_BLINKING) {
		if(led.blink_on) {
			if(system_timer_is_time_elapsed_ms(led.blink_last_time, LED_BLINK_DURATION_ON)) {
				led.blink_last_time = system_timer_get_ms();
				led.blink_on = false;
				led.blink_count++;

				led_set_color_value(0);
			} else {
				return;
			}
		} else {
			if(system_timer_is_time_elapsed_ms(led.blink_last_time, LED_BLINK_DURATION_OFF)) {
				led.blink_last_time = system_timer_get_ms();
				led.blink_on = true;

				led_set_color_value(255);
			} else {
				return;
			}
		}
	} else if(led.pattern == WARP_FRONT_PANEL_LED_PATTERN_BREATHING) {
		if(!system_timer_is_time_elapsed_ms(led.breathing_time, 5)) {
			return;
		}
		led.breathing_time = system_timer_get_ms();

		if(led.breathing_up) {
			led.breathing_index += 1;
		} else {
			led.breathing_index -= 1;
		}
		led.breathing_index = BETWEEN(0, led.breathing_index, 255);

		if(led.breathing_index == 0) {
			led.breathing_up = true;
		} else if(led.breathing_index == 255) {
			led.breathing_up = false;
		}
		led_set_color_value(led.breathing_index);
	} else {
		logw("Unknown pattern: %d\n\r", led.pattern);
	}

	if(led.set_r != last_r) {
		led_ccu4_pwm_set_duty_cycle(LED_R_CCU4_SLICE, 65535 - cie1931[led.set_r]);
		last_r = led.set_r;
	}
	if(led.set_g != last_g) {
		led_ccu4_pwm_set_duty_cycle(LED_G_CCU4_SLICE, cie1931[led.set_g]);
		last_g = led.set_g;
	}
}
