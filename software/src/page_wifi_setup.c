/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * page_wifi_setup.c: Display page for WIFI setup
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

#include "page_wifi_setup.h"

#include <string.h>

#include "st7789.h"
#include "qrcode.h"

#include "bricklib2/os/coop_task.h"

PageWifiSetup page_wifi_setup;

// version 1 -> 18x18, 41 byte
// version 2 -> 21x21, 56 byte
// version 3 -> 26x26, 85 byte
// version 4 -> 33x33, 137 byte
// version 5 -> 42x42, 221 byte
// version 6 -> 53x53, 352 byte
// version 7 -> 66x66, 545 byte

void page_wifi_setup_task_draw_qrcode(char *str, uint16_t x_start, uint16_t y_start, uint8_t pixel_size) {
    QRCode qrcode;
    uint8_t data[qrcode_getBufferSize(4)]; // 137 byte, 33x33
    uint16_t pixel_white[pixel_size*pixel_size];
    uint16_t pixel_black[pixel_size*pixel_size];
	st7789_fill_u16(ST7789_COLOR_WHITE, pixel_white, pixel_size*pixel_size);
	st7789_fill_u16(ST7789_COLOR_BLACK, pixel_black, pixel_size*pixel_size);

    qrcode_initText(&qrcode, data, 4, 1, str);

    for(uint8_t y = 0; y < qrcode.size; y++) {
        for(uint8_t x = 0; x < qrcode.size; x++) {
            st7789_task_draw_image(
                qrcode_getModule(&qrcode, x, y) ? pixel_black : pixel_white,
                x_start + x*pixel_size,
                y_start + y*pixel_size,
                x_start + (x+1)*pixel_size-1,
                y_start + (y+1)*pixel_size-1
            );
            coop_task_yield();
        }
    }
}

void page_wifi_setup_init(void) {
	memset(&page_wifi_setup, 0, sizeof(PageWifiSetup));
}

void page_wifi_setup_task_tick(void) {
	if(page_wifi_setup.redraw_everything) {
		page_wifi_setup.redraw_background = true;
		page_wifi_setup.redraw_qrcode = true;
		page_wifi_setup.redraw_everything = false;
	}

	if(page_wifi_setup.redraw_background) {
		st7789_task_draw_background();
		page_wifi_setup.redraw_background = false;
	}

	if(page_wifi_setup.redraw_qrcode) {
		page_wifi_setup_task_draw_qrcode("Test", 100, 100, 3);
		page_wifi_setup.redraw_qrcode = false;
	}
}