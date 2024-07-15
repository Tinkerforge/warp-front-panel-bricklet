/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * sprite.h: Draw sprites to screen from flash
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

#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>
#include <stdbool.h>

#include "sprite_defines.h"

typedef struct {
    uint32_t start_address;
    uint16_t width;
    uint16_t height;
} SpriteList;

typedef struct {
} Sprite;

extern SpriteList sprite_list[];
extern Sprite sprite;

void sprite_init(void);
void sprite_tick(void);
void sprite_task_draw(const uint8_t index, const uint16_t x_start, const uint16_t y_start);

#endif