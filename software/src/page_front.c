/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * page_front.c: First display page
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

#include "page_front.h"

#include <string.h>

#include "display.h"
#include "sprite.h"
#include "font.h"

#define ICON_HEIGHT 70

#define ICON_ROW1_START 0
#define ICON_ROW2_START 161

#define ICON_TEXT_AFTER 80

PageFront page_front;
PageFrontIconList page_front_icon_list[12];

const PageFrontIconPosition page_front_icon_position[6] = {
    {0, 30, ICON_TEXT_AFTER, 10+30, ICON_TEXT_AFTER, 10+30+30}, // top left
    {0, 30+ICON_HEIGHT, ICON_TEXT_AFTER, 10+30+ICON_HEIGHT, ICON_TEXT_AFTER, 10+30+30+ICON_HEIGHT}, // mid left
    {0, 30+ICON_HEIGHT*2, ICON_TEXT_AFTER, 10+30+ICON_HEIGHT*2, ICON_TEXT_AFTER, 10+30+30+ICON_HEIGHT*2}, // bottom left
    {ICON_ROW2_START, 30, ICON_ROW2_START+ICON_TEXT_AFTER, 10+30, ICON_ROW2_START+ICON_TEXT_AFTER, 10+30+30}, // top right
    {ICON_ROW2_START, 30+ICON_HEIGHT, ICON_ROW2_START+ICON_TEXT_AFTER, 10+30+ICON_HEIGHT, ICON_ROW2_START+ICON_TEXT_AFTER, 10+30+30+ICON_HEIGHT}, // mid right
    {ICON_ROW2_START, 30+ICON_HEIGHT*2, ICON_ROW2_START+ICON_TEXT_AFTER, 10+30+ICON_HEIGHT*2, ICON_ROW2_START+ICON_TEXT_AFTER, 10+30+30+ICON_HEIGHT*2} // bottom right
};

void page_front_init(void) {
    memset(&page_front, 0, sizeof(PageFront));

    // Redraw display on startup
    page_front.redraw_everything = true;
}

void page_front_task_tick(void) {
    if(page_front.redraw_everything) {
        page_front.redraw_background = true;
        page_front.redraw_everything = false;
    }

    if(page_front.redraw_background) {
        display_task_draw_background();
        page_front.redraw_background = false;
    }

    for(uint8_t i = 0; i < 6; i++) {
        if(page_front_icon_list[i].redraw) {
            if(page_front_icon_list[i].active) {
                sprite_task_draw(page_front_icon_list[i].sprite_index, page_front_icon_position[i].icon_x, page_front_icon_position[i].icon_y);
                font_task_draw_string(page_front_icon_list[i].text_1, 7, page_front_icon_list[i].font_index_1, page_front_icon_position[i].text1_x, page_front_icon_position[i].text1_y);
                font_task_draw_string(page_front_icon_list[i].text_2, 7, page_front_icon_list[i].font_index_2, page_front_icon_position[i].text2_x, page_front_icon_position[i].text2_y);
            } else {
                // TODO
            }
        }

    }
}