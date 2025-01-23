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

#include "st7789.h"
#include "display.h"
#include "sprite.h"
#include "font.h"
#include "status_bar.h"

#define PAGE_FRONT_ICON_WIDTH 69
#define PAGE_FRONT_ICON_HEIGHT 70
#define PAGE_FRONT_TEXT_WIDTH 90
#define PAGE_FRONT_TEXT_HEIGHT 70
#define PAGE_FRONT_DIVIDER_WIDTH 2
#define PAGE_FRONT_DIVIDER_HEIGHT 240
#define PAGE_FRONT_TEXT_BETWEEN_MARGIN 0
#define PAGE_FRONT_RIGHT_SIDE_START_X (PAGE_FRONT_ICON_WIDTH + PAGE_FRONT_TEXT_WIDTH + PAGE_FRONT_DIVIDER_WIDTH)

PageFront page_front;
PageFrontIconList page_front_icon_list[12];

void page_front_get_icon_position(const uint8_t icon_index, PageFrontIconPosition *pfip) {
    const uint8_t text_margin_top = (PAGE_FRONT_TEXT_HEIGHT - font_list[page_front_icon_list[icon_index].font_index_1].height - font_list[page_front_icon_list[icon_index].font_index_2].height - PAGE_FRONT_TEXT_BETWEEN_MARGIN)/2;

    switch(icon_index) {
        case PAGE_FRONT_INDEX_LEFT_TOP: {
            pfip->icon_x  = 0;
            pfip->icon_y  = 0*PAGE_FRONT_ICON_HEIGHT + STATUS_BAR_HEIGHT;
            pfip->text1_x = PAGE_FRONT_ICON_WIDTH;
            pfip->text1_y = 0*PAGE_FRONT_ICON_HEIGHT + STATUS_BAR_HEIGHT + text_margin_top;
            pfip->text2_x = PAGE_FRONT_ICON_WIDTH;
            pfip->text2_y = 0*PAGE_FRONT_ICON_HEIGHT + STATUS_BAR_HEIGHT + text_margin_top + font_list[page_front_icon_list[icon_index].font_index_2].height + PAGE_FRONT_TEXT_BETWEEN_MARGIN;
            break;
        }
        case PAGE_FRONT_INDEX_LEFT_MID: {
            pfip->icon_x  = 0;
            pfip->icon_y  = 1*PAGE_FRONT_ICON_HEIGHT + STATUS_BAR_HEIGHT;
            pfip->text1_x = PAGE_FRONT_ICON_WIDTH;
            pfip->text1_y = 1*PAGE_FRONT_ICON_HEIGHT + STATUS_BAR_HEIGHT + text_margin_top;
            pfip->text2_x = PAGE_FRONT_ICON_WIDTH;
            pfip->text2_y = 1*PAGE_FRONT_ICON_HEIGHT + STATUS_BAR_HEIGHT + text_margin_top + font_list[page_front_icon_list[icon_index].font_index_2].height + PAGE_FRONT_TEXT_BETWEEN_MARGIN;
            break;
        }
        case PAGE_FRONT_INDEX_LEFT_BOTTOM: {
            pfip->icon_x  = 0;
            pfip->icon_y  = 2*PAGE_FRONT_ICON_HEIGHT + STATUS_BAR_HEIGHT;
            pfip->text1_x = PAGE_FRONT_ICON_WIDTH;
            pfip->text1_y = 2*PAGE_FRONT_ICON_HEIGHT + STATUS_BAR_HEIGHT + text_margin_top;
            pfip->text2_x = PAGE_FRONT_ICON_WIDTH;
            pfip->text2_y = 2*PAGE_FRONT_ICON_HEIGHT + STATUS_BAR_HEIGHT + text_margin_top + font_list[page_front_icon_list[icon_index].font_index_2].height + PAGE_FRONT_TEXT_BETWEEN_MARGIN;
            break;
        }
        case PAGE_FRONT_INDEX_RIGHT_TOP: {
            pfip->icon_x  = PAGE_FRONT_RIGHT_SIDE_START_X;
            pfip->icon_y  = 0*PAGE_FRONT_ICON_HEIGHT + STATUS_BAR_HEIGHT;
            pfip->text1_x = PAGE_FRONT_RIGHT_SIDE_START_X + PAGE_FRONT_ICON_WIDTH;
            pfip->text1_y = 0*PAGE_FRONT_ICON_HEIGHT + STATUS_BAR_HEIGHT + text_margin_top;
            pfip->text2_x = PAGE_FRONT_RIGHT_SIDE_START_X + PAGE_FRONT_ICON_WIDTH;
            pfip->text2_y = 0*PAGE_FRONT_ICON_HEIGHT + STATUS_BAR_HEIGHT + text_margin_top + font_list[page_front_icon_list[icon_index].font_index_2].height + PAGE_FRONT_TEXT_BETWEEN_MARGIN;
            break;
        }
        case PAGE_FRONT_INDEX_RIGHT_MID: {
            pfip->icon_x  = PAGE_FRONT_RIGHT_SIDE_START_X;
            pfip->icon_y  = 1*PAGE_FRONT_ICON_HEIGHT + STATUS_BAR_HEIGHT;
            pfip->text1_x = PAGE_FRONT_RIGHT_SIDE_START_X + PAGE_FRONT_ICON_WIDTH;
            pfip->text1_y = 1*PAGE_FRONT_ICON_HEIGHT + STATUS_BAR_HEIGHT + text_margin_top;
            pfip->text2_x = PAGE_FRONT_RIGHT_SIDE_START_X + PAGE_FRONT_ICON_WIDTH;
            pfip->text2_y = 1*PAGE_FRONT_ICON_HEIGHT + STATUS_BAR_HEIGHT + text_margin_top + font_list[page_front_icon_list[icon_index].font_index_2].height + PAGE_FRONT_TEXT_BETWEEN_MARGIN;
            break;
        }
        case PAGE_FRONT_INDEX_RIGHT_BOTTOM: {
            pfip->icon_x  = PAGE_FRONT_RIGHT_SIDE_START_X;
            pfip->icon_y  = 2*PAGE_FRONT_ICON_HEIGHT + STATUS_BAR_HEIGHT;
            pfip->text1_x = PAGE_FRONT_RIGHT_SIDE_START_X + PAGE_FRONT_ICON_WIDTH;
            pfip->text1_y = 2*PAGE_FRONT_ICON_HEIGHT + STATUS_BAR_HEIGHT + text_margin_top;
            pfip->text2_x = PAGE_FRONT_RIGHT_SIDE_START_X + PAGE_FRONT_ICON_WIDTH;
            pfip->text2_y = 2*PAGE_FRONT_ICON_HEIGHT + STATUS_BAR_HEIGHT + text_margin_top + font_list[page_front_icon_list[icon_index].font_index_2].height + PAGE_FRONT_TEXT_BETWEEN_MARGIN;
            break;
        }
    }
}

void page_front_draw_inactive(const uint8_t icon_index) {
    switch(icon_index) {
        case PAGE_FRONT_INDEX_LEFT_TOP: {
            display_task_draw_filled_rect(
                PAGE_FRONT_BACKGROUND_COLOR,
                0,
                STATUS_BAR_HEIGHT + PAGE_FRONT_ICON_HEIGHT*0,
                PAGE_FRONT_ICON_WIDTH + PAGE_FRONT_TEXT_WIDTH - 1,
                STATUS_BAR_HEIGHT + PAGE_FRONT_ICON_HEIGHT*1 - 1);
            break;
        }
        case PAGE_FRONT_INDEX_LEFT_MID: {
            display_task_draw_filled_rect(
                PAGE_FRONT_BACKGROUND_COLOR,
                0,
                STATUS_BAR_HEIGHT + PAGE_FRONT_ICON_HEIGHT*1,
                PAGE_FRONT_ICON_WIDTH + PAGE_FRONT_TEXT_WIDTH - 1,
                STATUS_BAR_HEIGHT + PAGE_FRONT_ICON_HEIGHT*2 - 1);
            break;
        }


        case PAGE_FRONT_INDEX_LEFT_BOTTOM: {
            display_task_draw_filled_rect(
                PAGE_FRONT_BACKGROUND_COLOR,
                0,
                STATUS_BAR_HEIGHT + PAGE_FRONT_ICON_HEIGHT*2,
                PAGE_FRONT_ICON_WIDTH + PAGE_FRONT_TEXT_WIDTH - 1,
                STATUS_BAR_HEIGHT + PAGE_FRONT_ICON_HEIGHT*3 - 1);
            break;
        }
        case PAGE_FRONT_INDEX_RIGHT_TOP: {
            display_task_draw_filled_rect(
                PAGE_FRONT_BACKGROUND_COLOR,
                PAGE_FRONT_RIGHT_SIDE_START_X,
                STATUS_BAR_HEIGHT + PAGE_FRONT_ICON_HEIGHT*0,
                PAGE_FRONT_RIGHT_SIDE_START_X + PAGE_FRONT_ICON_WIDTH + PAGE_FRONT_TEXT_WIDTH - 1,
                STATUS_BAR_HEIGHT + PAGE_FRONT_ICON_HEIGHT*1 - 1);
            break;
        }
        case PAGE_FRONT_INDEX_RIGHT_MID: {
            display_task_draw_filled_rect(
                PAGE_FRONT_BACKGROUND_COLOR,
                PAGE_FRONT_RIGHT_SIDE_START_X,
                STATUS_BAR_HEIGHT + PAGE_FRONT_ICON_HEIGHT*1,
                PAGE_FRONT_RIGHT_SIDE_START_X + PAGE_FRONT_ICON_WIDTH + PAGE_FRONT_TEXT_WIDTH - 1,
                STATUS_BAR_HEIGHT + PAGE_FRONT_ICON_HEIGHT*2 - 1);
            break;
        }


        case PAGE_FRONT_INDEX_RIGHT_BOTTOM: {
            display_task_draw_filled_rect(
                PAGE_FRONT_BACKGROUND_COLOR,
                PAGE_FRONT_RIGHT_SIDE_START_X,
                STATUS_BAR_HEIGHT + PAGE_FRONT_ICON_HEIGHT*2,
                PAGE_FRONT_RIGHT_SIDE_START_X + PAGE_FRONT_ICON_WIDTH + PAGE_FRONT_TEXT_WIDTH - 1,
                STATUS_BAR_HEIGHT + PAGE_FRONT_ICON_HEIGHT*3 - 1);
            break;
        }
    }
}

void page_front_init(void) {
    memset(&page_front, 0, sizeof(PageFront));

    // Redraw display on startup
    page_front.redraw_everything = true;
}

void page_front_task_tick(void) {
    if(page_front.redraw_everything) {
        page_front.redraw_background = true;
        for(uint8_t i = 0; i < 6; i++) {
            page_front_icon_list[i].redraw_everything = true;
        }
        page_front.redraw_everything = false;
    }

    if(page_front.redraw_background) {
        page_front.redraw_background = false;
        display_task_draw_background();
    }

    for(uint8_t i = 0; i < 6; i++) {
        PageFrontIconPosition pfip;
        page_front_get_icon_position(i, &pfip);

        if(page_front_icon_list[i].redraw_everything && page_front_icon_list[i].active) {
            page_front_icon_list[i].redraw_sprite = true;
            page_front_icon_list[i].redraw_text_1 = true;
            page_front_icon_list[i].redraw_text_2 = true;
            page_front_icon_list[i].redraw_everything = false;
        }
        if(page_front_icon_list[i].redraw_sprite && page_front_icon_list[i].active) {
            page_front_icon_list[i].redraw_sprite = false;
            sprite_task_draw(page_front_icon_list[i].sprite_index, pfip.icon_x, pfip.icon_y);
        }
        if(page_front_icon_list[i].redraw_text_1 && page_front_icon_list[i].active) {
            page_front_icon_list[i].redraw_text_1 = false;
            font_task_draw_string(page_front_icon_list[i].text_1, PAGE_FRONT_TEXT_MAX_CHAR, page_front_icon_list[i].font_index_1, pfip.text1_x, pfip.text1_y);
        }
        if(page_front_icon_list[i].redraw_text_2 && page_front_icon_list[i].active) {
            page_front_icon_list[i].redraw_text_2 = false;
            font_task_draw_string(page_front_icon_list[i].text_2, PAGE_FRONT_TEXT_MAX_CHAR, page_front_icon_list[i].font_index_2, pfip.text2_x, pfip.text2_y);
        }
        if(page_front_icon_list[i].redraw_everything && !page_front_icon_list[i].active) {
            page_front_icon_list[i].redraw_sprite = false;
            page_front_icon_list[i].redraw_text_1 = false;
            page_front_icon_list[i].redraw_text_2 = false;
            page_front_icon_list[i].redraw_everything = false;
            page_front_draw_inactive(i);
        }
    }
}
