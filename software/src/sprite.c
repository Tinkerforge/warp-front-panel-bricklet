/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * sprite.c: Draw sprites to screen from flash
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


#include "sprite.h"

#include <string.h>

#include "display.h"

Sprite sprite;
SpriteList sprite_list[] = {
    #include "sprite_list.h"
};

void sprite_task_draw(const uint8_t index, const uint16_t x_start, const uint16_t y_start) {
    display_task_draw_from_by25q(
        sprite_list[index].start_address,
        x_start,
        y_start,
        x_start + sprite_list[index].width - 1,
        y_start + sprite_list[index].height - 1
    );
}

void sprite_init(void) {
	memset(&sprite, 0, sizeof(Sprite));
}

void sprite_tick(void) {

}