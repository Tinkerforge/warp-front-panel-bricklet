/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * led.h: Driver for RG LED
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

#ifndef LED_H
#define LED_H

#include <stdint.h>
#include <stdbool.h>

#define LED_BLINK_DURATION_ON   250 // in ms
#define LED_BLINK_DURATION_OFF  250 // in ms

typedef struct {
	uint8_t pattern;
	uint8_t color;

	uint8_t set_r;
	uint8_t set_g;

	uint32_t blink_num;
	uint32_t blink_count;
	bool blink_on;
	uint32_t blink_last_time;

	uint32_t breathing_time;
	int16_t breathing_index;
	bool breathing_up;
} LED;

extern LED led;

void led_init(void);
void led_tick(void);

#endif
