/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * button.c: Driver for button with LED
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

#include "button.h"

#include "configs/config_button.h"
#include "bricklib2/hal/system_timer/system_timer.h"

#include <string.h>
#include "xmc_gpio.h"

Button button;

void button_init(void) {
    memset(&button, 0, sizeof(Button));

    const XMC_GPIO_CONFIG_t pin_config_input = {
        .mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
        .input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
    };
    const XMC_GPIO_CONFIG_t pin_config_output_low = {
        .mode             = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
        .output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
    };
    const XMC_GPIO_CONFIG_t pin_config_output_high = {
        .mode             = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
        .output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
    };

    XMC_GPIO_Init(BUTTON_SWITCH_PIN, &pin_config_input);
    XMC_GPIO_Init(BUTTON_LED1_PIN,   &pin_config_output_low);
    XMC_GPIO_Init(BUTTON_LED2_PIN,   &pin_config_output_high);
}

uint32_t test = 0;
uint32_t i = 0;

void button_tick(void) {
    const bool button_pressed = !XMC_GPIO_GetInput(BUTTON_SWITCH_PIN);
    if(button_pressed && !button.is_pressed) {
        if(system_timer_is_time_elapsed_ms(button.last_pressed, 50)) {
            button.index++;
            button.last_pressed = system_timer_get_ms();
        }
    }
    button.is_pressed = !XMC_GPIO_GetInput(BUTTON_SWITCH_PIN);
    if(system_timer_is_time_elapsed_ms(test, 1000)) {
        test = system_timer_get_ms();
        i++;
        switch(i%3) {
            case 0:
                XMC_GPIO_SetOutputHigh(BUTTON_LED1_PIN);
                XMC_GPIO_SetOutputHigh(BUTTON_LED2_PIN);
                break;
            case 1:
                XMC_GPIO_SetOutputLow(BUTTON_LED1_PIN);
                XMC_GPIO_SetOutputHigh(BUTTON_LED2_PIN);
                break;
            case 2:
                XMC_GPIO_SetOutputHigh(BUTTON_LED1_PIN);
                XMC_GPIO_SetOutputLow(BUTTON_LED2_PIN);
                break;
        }
    }
}