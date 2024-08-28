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

StatusBar status_bar;

void status_bar_init(void) {
    memset(&status_bar, 0, sizeof(StatusBar));

    // Redraw status bar on startup
    status_bar.redraw_everything = true;
}

void status_bar_task_draw_status_bar_string(const char *str) {
    font_task_draw_string(
        str,
        STATUS_BAR_STATUS_CHARS_MAX,
        STATUS_BAR_FONT_INDEX,
        STATUS_BAR_END_X - font_list[STATUS_BAR_FONT_INDEX].width*STATUS_BAR_STATUS_CHARS_MAX - STATUS_BAR_ICON_RIGHT_MARGIN,
        (STATUS_BAR_HEIGHT - font_list[STATUS_BAR_FONT_INDEX].height)/2
    );
}

void status_bar_task_tick(void) {
    if(status_bar.redraw_everything) {
        status_bar.redraw_background = true;
        status_bar.redraw_wifi       = true;
        status_bar.redraw_ethernet   = true;
        status_bar.redraw_clock      = true;
        status_bar.redraw_status     = true;
        status_bar.redraw_everything = false;
    }

    if(status_bar.redraw_background) {
        display_task_draw_filled_rect(status_bar.background_color, STATUS_BAR_START_X, STATUS_BAR_START_Y, STATUS_BAR_END_X, STATUS_BAR_END_Y);
        status_bar.redraw_background = false;
    }

    if(status_bar.redraw_wifi) {
        const uint16_t status = status_bar.wifi_status >> 16;
        const int16_t  rssi   = (status_bar.wifi_status & 0xFFFF) - 127;
        uint8_t icon_wifi = SPRITE_STATUS_ICON_NO_WIFI;
        if(status == 3) { // 3 = connected
            if(rssi > -65) {
                icon_wifi = SPRITE_STATUS_ICON_WIFI_3BAR;
            } else if(rssi > -80) {
                icon_wifi = SPRITE_STATUS_ICON_WIFI_2BAR;
            } else {
                icon_wifi = SPRITE_STATUS_ICON_WIFI_1BAR;
            }
        }
        sprite_task_draw(icon_wifi, STATUS_BAR_ICON_LEFT_MARGIN, STATUS_BAR_START_Y);
        status_bar.redraw_wifi = false;
    }

    if(status_bar.redraw_ethernet) {
        // 3 = connected
        const uint8_t icon_ethernet = status_bar.ethernet_status == 3 ? SPRITE_STATUS_ICON_ETHERNET : SPRITE_STATUS_ICON_NO_ETHERNET;
        sprite_task_draw(icon_ethernet, STATUS_BAR_ICON_LEFT_MARGIN + sprite_list[SPRITE_STATUS_ICON_WIFI_3BAR].width, STATUS_BAR_START_Y);
        status_bar.redraw_ethernet = false;
    }

    if(strlen(status_bar.status) > 0) {
        if(status_bar.redraw_status) {
            status_bar_task_draw_status_bar_string(status_bar.status);
            status_bar.redraw_status = false;
        }
    } else if(status_bar.redraw_clock) {
        char clock[STATUS_BAR_STATUS_CHARS_MAX] = "        00:00:00";
        clock[STATUS_BAR_STATUS_CHARS_MAX - STATUS_BAR_CLOCK_CHARS + 0] += status_bar.hours   / 10;
        clock[STATUS_BAR_STATUS_CHARS_MAX - STATUS_BAR_CLOCK_CHARS + 1] += status_bar.hours   % 10;
        clock[STATUS_BAR_STATUS_CHARS_MAX - STATUS_BAR_CLOCK_CHARS + 3] += status_bar.minutes / 10;
        clock[STATUS_BAR_STATUS_CHARS_MAX - STATUS_BAR_CLOCK_CHARS + 4] += status_bar.minutes % 10;
        clock[STATUS_BAR_STATUS_CHARS_MAX - STATUS_BAR_CLOCK_CHARS + 6] += status_bar.seconds / 10;
        clock[STATUS_BAR_STATUS_CHARS_MAX - STATUS_BAR_CLOCK_CHARS + 7] += status_bar.seconds % 10;

        status_bar_task_draw_status_bar_string(clock);
        status_bar.redraw_clock = false;
    }
}

void status_bar_set_status(const char *str) {
    if(strlen(str) == 0) {
        // If no status is set, fill with zeroes -> clock will be shown
        memset(status_bar.status, 0, STATUS_BAR_STATUS_CHARS_MAX);
    } else {
        // If status is set, fill front with spaces -> new text will always overwrite old text completely
        memset(status_bar.status, ' ', STATUS_BAR_STATUS_CHARS_MAX-strlen(str));
        strcpy(status_bar.status + STATUS_BAR_STATUS_CHARS_MAX-strlen(str), str);
    }
}