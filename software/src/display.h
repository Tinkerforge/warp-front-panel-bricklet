/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * display.h: Display main loop and utility functions
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

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t last_index;
} Display;

extern Display display;

void display_init(void);
void display_task_tick(void);

void display_fill_u16(const uint16_t color, uint16_t *data, const uint16_t length);
void display_fill_circle(const uint16_t color, uint16_t *data, const uint16_t length);

void display_task_draw_filled_rect(const uint16_t color, const uint16_t x_start, const uint16_t y_start, const uint16_t x_end, const uint16_t y_end);
void display_task_draw_image(uint16_t *image, const uint16_t x_start, const uint16_t y_start, const uint16_t x_end, const uint16_t y_end);
void display_task_draw_from_by25q(const uint32_t address_start, const uint16_t x_start, const uint16_t y_start, const uint16_t x_end, const uint16_t y_end);
void display_task_draw_background(void);

#endif