/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * font.c: Draw text to screen from font in flash
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

#include "font.h"

#include <string.h>

#include "st7789.h"

Font font;
FontCharset font_charset[4] = {
    {0, 10, 17},
};

void font_task_draw_string(const char *str, const uint8_t str_length, const uint8_t index, const uint16_t x_start, const uint16_t y_start) {
    const uint32_t char_offset = font_charset[index].width * font_charset[index].height*sizeof(uint16_t);
    const uint32_t y_end       = y_start + font_charset[index].height -1;
    for(uint16_t i = 0; i < str_length; i++) {
        const uint32_t address      = font_charset[index].start_address + char_offset*((uint32_t)str[i]);
        const uint32_t x_start_next = x_start      + font_charset[index].width*i;
        const uint32_t x_end        = x_start_next + font_charset[index].width -1;
        st7789_task_draw_from_by25q(address, x_start_next, y_start, x_end, y_end);
    }
}

void font_init(void) {
	memset(&font, 0, sizeof(Font));
}

void font_tick(void) {

}