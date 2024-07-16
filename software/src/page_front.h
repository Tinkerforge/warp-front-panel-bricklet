/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * page_front.h: First display page
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

#ifndef PAGE_FRONT_H
#define PAGE_FRONT_H

#include <stdint.h>
#include <stdbool.h>

#include "st7789.h"

#define PAGE_FRONT_INDEX_LEFT_TOP     0
#define PAGE_FRONT_INDEX_LEFT_MID     1
#define PAGE_FRONT_INDEX_LEFT_BOTTOM  2
#define PAGE_FRONT_INDEX_RIGHT_TOP    3
#define PAGE_FRONT_INDEX_RIGHT_MID    4
#define PAGE_FRONT_INDEX_RIGHT_BOTTOM 5

#define PAGE_FRONT_BACKGROUND_COLOR   ST7789_COLOR_BLACK

typedef struct {
    uint16_t icon_x;
    uint16_t icon_y;
    uint16_t text1_x;
    uint16_t text1_y;
    uint16_t text2_x;
    uint16_t text2_y;
} PageFrontIconPosition;

typedef struct {
    bool redraw_everything;
    bool redraw_sprite;
    bool redraw_text_1;
    bool redraw_text_2;
    bool active;
    uint32_t sprite_index;
    char text_1[10];
    uint8_t font_index_1;
    char text_2[10];
    uint8_t font_index_2;
} PageFrontIconList;

typedef struct {
    bool redraw_everything;
    bool redraw_background;
} PageFront;

extern PageFrontIconList page_front_icon_list[12];
extern PageFront page_front;

void page_front_init(void);
void page_front_task_tick(void);


#endif