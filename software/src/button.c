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

    XMC_GPIO_Init(BUTTON_SWITCH_PIN, &pin_config_input);
}

void button_tick(void) {
    const bool button_pressed = !XMC_GPIO_GetInput(BUTTON_SWITCH_PIN);
    if(button_pressed && !button.is_pressed) {
        if(system_timer_is_time_elapsed_ms(button.last_pressed, 50)) {
            button.index++;
            button.last_pressed = system_timer_get_ms();
        }
    }

    button.is_pressed = !XMC_GPIO_GetInput(BUTTON_SWITCH_PIN);
}