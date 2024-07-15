/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * status_bar.c: Status bar drawing functions
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

#include "status_bar.h"

#include <string.h>

#include "by25q.h"
#include "display.h"
#include "sprite.h"
#include "font.h"

#define STATUS_BAR_FONT 8

StatusBar status_bar;

void status_bar_init(void) {
    memset(&status_bar, 0, sizeof(StatusBar));

    // Redraw status bar on startup
    status_bar.redraw_everything = true;
}

void status_bar_task_tick(void) {
    if(status_bar.redraw_everything) {
        status_bar.redraw_background = true;
        status_bar.redraw_wifi       = true;
        status_bar.redraw_ethernet   = true;
        status_bar.redraw_clock      = true;
        status_bar.redraw_everything = false;
    }

    if(status_bar.redraw_background) {
        display_task_draw_filled_rect(status_bar.background_color, 0, 0, 319, 29);
        status_bar.redraw_background = false;
    }

    if(status_bar.redraw_wifi) {
        sprite_task_draw(SPRITE_STATUS_ICON_WIFI, 5, 0);
        status_bar.redraw_wifi = false;
    }

    if(status_bar.redraw_ethernet) {
        sprite_task_draw(SPRITE_STATUS_ICON_ETHERNET, 5 + sprite_list[SPRITE_STATUS_ICON_WIFI].width, 0);
        status_bar.redraw_ethernet = false;
    }

    if(status_bar.redraw_clock) {
        char clock[9] = "00:00:00";
        clock[0] += status_bar.hours   / 10;
        clock[1] += status_bar.hours   % 10;
        clock[3] += status_bar.minutes / 10;
        clock[4] += status_bar.minutes % 10;
        clock[6] += status_bar.seconds / 10;
        clock[7] += status_bar.seconds % 10;

        font_task_draw_string(clock, 8, STATUS_BAR_FONT, 319-font_list[STATUS_BAR_FONT].width*8-5, (30 - font_list[STATUS_BAR_FONT].height)/2);
        status_bar.redraw_clock = false;
    }
}