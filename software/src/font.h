/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * font.h: Draw text to screen from font in flash
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

#ifndef FONT_H
#define FONT_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t start_address;
    uint8_t width;
    uint8_t height;
} FontCharset;

typedef struct {
} Font;

extern Font font;

void font_init(void);
void font_tick(void);
void font_task_draw_string(const char *str, const uint8_t str_length, const uint8_t index, const uint16_t x_start, const uint16_t y_start);

#endif