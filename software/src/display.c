/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * display.c: Display main loop and utility functions
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

#include "display.h"

#include <string.h>

#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/utility/util_definitions.h"
#include "bricklib2/os/coop_task.h"
#include "spi.h"

#include "configs/config_st7789.h"
#include "st7789.h"
#include "by25q.h"
#include "font.h"
#include "sprite.h"
#include "button.h"
#include "page_front.h"
#include "page_wifi_setup.h"
#include "status_bar.h"
#include "communication.h"
#include "xmc_gpio.h"

Display display;

void display_init(void) {
    memset(&display, 0, sizeof(Display));
    display.active    = WARP_FRONT_PANEL_DISPLAY_AUTOMATIC;
    display.is_active = true;
    display.countdown = MAX(1, system_timer_get_ms());
}

void display_fill_u16(const uint16_t color, uint16_t *data, const uint16_t length) {
    for(uint16_t i = 0; i < length; i++) {
        data[i] = color;
    }
}


void display_fill_circle(const uint16_t color, uint16_t *data, const uint16_t length) {
    for(int row = 0; row < length; row++) {
        for(int col = 0; col < length; col++) {
            int x = col - length/2;
            int y = length/2 - row;
            int sumsq = x*x + y*y;

            if((sumsq > 95) && (sumsq < 105)) {
                data[col*length + row] = color;
            }
        }
    }
}

void display_task_draw_filled_rect(const uint16_t color, const uint16_t x_start, const uint16_t y_start, const uint16_t x_end, const uint16_t y_end) {
    uint16_t data[100];
    display_fill_u16(color, data, 100);
    st7789_set_window(x_start, y_start, x_end, y_end);

    uint32_t length = (x_end - x_start + 1)*(y_end-y_start+1);
    while(length > 0) {
        uint16_t length_write = MIN(100, length);
        st7789_task_write_display(data, length_write);
        length -= length_write;
    }
}

void display_task_draw_image(uint16_t *image, const uint16_t x_start, const uint16_t y_start, const uint16_t x_end, const uint16_t y_end) {
    st7789_set_window(x_start, y_start, x_end, y_end);

    uint32_t length = (x_end - x_start + 1)*(y_end-y_start+1);
    while(length > 0) {
        uint16_t length_write = MIN(100, length);
        st7789_task_write_display(image, length_write);
        image += length_write;
        length -= length_write;
    }
}

void display_task_draw_from_by25q(const uint32_t address_start, const uint16_t x_start, const uint16_t y_start, const uint16_t x_end, const uint16_t y_end) {
    st7789_set_window(x_start, y_start, x_end, y_end);

    // window length for 8bit data
    uint32_t window_length = (x_end - x_start + 1)*(y_end-y_start+1)*sizeof(uint16_t);
    uint32_t address = address_start;
    uint8_t data[BY25Q_PAGE_SIZE] = {0};
    while(window_length > 0) {
        uint16_t length = MIN(BY25Q_PAGE_SIZE, window_length);
        by25q_task_read(data, length, address);
        st7789_task_write_display((uint16_t*)data, length/sizeof(uint16_t));
        window_length -= length;
        address       += length;
    }
}

void display_task_draw_background(void) {
    display_task_draw_filled_rect(DISPLAY_BACKGROUND_COLOR, DISPLAY_START_X, STATUS_BAR_HEIGHT, DISPLAY_END_X, DISPLAY_END_Y);
//    display_task_draw_filled_rect(status_bar.background_color, DISPLAY_WIDTH/2-1, DISPLAY_START_Y, DISPLAY_WIDTH/2, DISPLAY_END_Y);
}

void display_task_tick(void) {
    const uint8_t new_index = button.index % 2;
    const bool index_changed = new_index != display.last_index;
    display.last_index = new_index;

    if(display.active == WARP_FRONT_PANEL_DISPLAY_OFF) {
        display.is_active = false;
    } else if(system_timer_is_time_elapsed_ms(display.countdown, DISPLAY_COUNTDOWN_MS)) {
        display.is_active = false;
        display.countdown = 0;
    }

    if(index_changed) {
        status_bar.redraw_everything = true;
    }
    switch(new_index) {
        default:
        case 0: {
            if(index_changed) {
                status_bar_set_status("");
                page_front.redraw_everything = true;
            }

            status_bar_task_tick();
            page_front_task_tick();
            break;
        }

        case 1: {
            if(index_changed) {
                status_bar_set_status("Access Point");
                page_wifi_setup.redraw_everything = true;
            }

            status_bar_task_tick();
            page_wifi_setup_task_tick();
            break;
        }
    }

    if (display.is_active) {
        // Check that backlight is on if display is active
        // This turns the backlight on after a reset after the first page is drawn
        XMC_GPIO_SetOutputHigh(ST7789_BACKLIGHT_PIN);
    }
}