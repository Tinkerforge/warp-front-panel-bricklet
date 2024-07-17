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
#include "display.h"
#include "qrcode.h"
#include "font.h"
#include "status_bar.h"

#include "bricklib2/os/coop_task.h"

#define PAGE_WIFI_SETUP_IP_ADDRESS_STR "IP Address"
#define PAGE_WIFI_SETUP_SSID_STR       "SSID/Hostname"
#define PAGE_WIFI_SETUP_PASSWORD_STR   "Passphrase"

#define PAGE_WIFI_SETUP_MARGIN_TOP     5
#define PAGE_WIFI_SETUP_MARGIN_BOTTOM  5
#define PAGE_WIFI_SETUP_MARGIN_LEFT    5
#define PAGE_WIFI_SETUP_MARGIN_RIGHT   5

#define PAGE_WIFI_SETUP_FONT1          FONT_19PX_FREEMONO_WHITE_ON_BLACK
#define PAGE_WIFI_SETUP_FONT2          FONT_24PX_FREEMONO_BLACK_ON_WHITE
#define PAGE_WIFI_SETUP_FONT1_HEIGHT   font_list[PAGE_WIFI_SETUP_FONT1].height
#define PAGE_WIFI_SETUP_FONT2_HEIGHT   font_list[PAGE_WIFI_SETUP_FONT2].height
#define PAGE_WIFI_SETUP_TEXT_MARGIN    font_list[PAGE_WIFI_SETUP_FONT2].height

#define PAGE_WIFI_SETUP_QR_MODULE_SIZE 4
#define PAGE_WIFI_SETUP_QR_MODULE_NUM  33
#define PAGE_WIFI_SETUP_QR_BORDER_SIZE 5

PageWifiSetup page_wifi_setup;

// version 1 -> 18x18, 41 byte
// version 2 -> 21x21, 56 byte
// version 3 -> 26x26, 85 byte
// version 4 -> 33x33, 137 byte
// version 5 -> 42x42, 221 byte
// version 6 -> 53x53, 352 byte
// version 7 -> 66x66, 545 byte

void page_wifi_setup_task_draw_qrcode(char *str, uint16_t x_start, uint16_t y_start, uint8_t module_size) {
    QRCode qrcode;
    uint8_t data[qrcode_getBufferSize(4)]; // 137 byte, 33x33
    uint16_t pixel_white[module_size*module_size];
    uint16_t pixel_black[module_size*module_size];
    display_fill_u16(ST7789_COLOR_WHITE, pixel_white, module_size*module_size);
    display_fill_u16(ST7789_COLOR_BLACK, pixel_black, module_size*module_size);

    qrcode_initText(&qrcode, data, 4, 1, str);

    for(uint8_t y = 0; y < qrcode.size; y++) {
        for(uint8_t x = 0; x < qrcode.size; x++) {
            display_task_draw_image(
                qrcode_getModule(&qrcode, x, y) ? pixel_black : pixel_white,
                x_start + x*module_size,
                y_start + y*module_size,
                x_start + (x+1)*module_size-1,
                y_start + (y+1)*module_size-1
            );
            coop_task_yield();
        }
    }
}

void page_wifi_setup_init(void) {
    memset(&page_wifi_setup, 0, sizeof(PageWifiSetup));

    // TODO: This is testing data, to be removed
    strcpy(page_wifi_setup.ip_address, "10.0.0.1");
    strcpy(page_wifi_setup.ssid,       "wem2-Swq");
    strcpy(page_wifi_setup.password,   "MkoC-UwBq-rm7a-hrgH");
}

char* page_wifi_setup_get_qr_string(void) {
    #define BUFFER_SIZE (64+49+sizeof("WIFI:T:WPA;S:;P:;;"))
    static char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    strcat(buffer, "WIFI:T:WPA;S:");
    strcat(buffer, page_wifi_setup.ssid);
    strcat(buffer, ";P:");
    strcat(buffer, page_wifi_setup.password);
    strcat(buffer, ";;");

    return buffer;
}

void page_wifi_setup_task_tick(void) {
    if(page_wifi_setup.redraw_everything) {
        page_wifi_setup.redraw_background = true;
        page_wifi_setup.redraw_qrcode     = true;
        page_wifi_setup.redraw_ip_address = true;
        page_wifi_setup.redraw_ssid       = true;
        page_wifi_setup.redraw_password   = true;
        page_wifi_setup.redraw_everything = false;
    }

    if(page_wifi_setup.redraw_background) {
        display_task_draw_background();
        page_wifi_setup.redraw_background = false;
    }

    if(page_wifi_setup.redraw_qrcode) {
        display_task_draw_filled_rect(
            ST7789_COLOR_WHITE,
            DISPLAY_END_X - PAGE_WIFI_SETUP_MARGIN_RIGHT - PAGE_WIFI_SETUP_QR_MODULE_SIZE*PAGE_WIFI_SETUP_QR_MODULE_NUM - 2*PAGE_WIFI_SETUP_QR_BORDER_SIZE,
            STATUS_BAR_HEIGHT,
            DISPLAY_END_X,
            STATUS_BAR_HEIGHT + PAGE_WIFI_SETUP_MARGIN_TOP + PAGE_WIFI_SETUP_MARGIN_BOTTOM + PAGE_WIFI_SETUP_QR_MODULE_SIZE*PAGE_WIFI_SETUP_QR_MODULE_NUM + 2*PAGE_WIFI_SETUP_QR_BORDER_SIZE - 1
        );
        page_wifi_setup_task_draw_qrcode(
            page_wifi_setup_get_qr_string(),
            DISPLAY_END_X - PAGE_WIFI_SETUP_MARGIN_RIGHT - PAGE_WIFI_SETUP_QR_MODULE_SIZE*PAGE_WIFI_SETUP_QR_MODULE_NUM,
            STATUS_BAR_HEIGHT + PAGE_WIFI_SETUP_MARGIN_TOP + PAGE_WIFI_SETUP_QR_BORDER_SIZE,
            PAGE_WIFI_SETUP_QR_MODULE_SIZE
        );
        page_wifi_setup.redraw_qrcode = false;
    }

    if(page_wifi_setup.redraw_ip_address) {
        font_task_draw_string(
            PAGE_WIFI_SETUP_IP_ADDRESS_STR,
            strlen(PAGE_WIFI_SETUP_IP_ADDRESS_STR),
            PAGE_WIFI_SETUP_FONT1,
            PAGE_WIFI_SETUP_MARGIN_LEFT,
            STATUS_BAR_HEIGHT + PAGE_WIFI_SETUP_MARGIN_TOP + PAGE_WIFI_SETUP_FONT1_HEIGHT*0 + PAGE_WIFI_SETUP_FONT2_HEIGHT*0 + PAGE_WIFI_SETUP_TEXT_MARGIN*0
        );
        font_task_draw_string(
            page_wifi_setup.ip_address,
            strlen(page_wifi_setup.ip_address),
            PAGE_WIFI_SETUP_FONT2,
            PAGE_WIFI_SETUP_MARGIN_LEFT,
            STATUS_BAR_HEIGHT + PAGE_WIFI_SETUP_MARGIN_TOP + PAGE_WIFI_SETUP_FONT1_HEIGHT*1 + PAGE_WIFI_SETUP_FONT2_HEIGHT*0 + PAGE_WIFI_SETUP_TEXT_MARGIN*0
        );
        page_wifi_setup.redraw_ip_address = false;
    }

    if(page_wifi_setup.redraw_ssid) {
        font_task_draw_string(
            PAGE_WIFI_SETUP_SSID_STR,
            strlen(PAGE_WIFI_SETUP_SSID_STR),
            PAGE_WIFI_SETUP_FONT1,
            PAGE_WIFI_SETUP_MARGIN_LEFT,
            STATUS_BAR_HEIGHT + PAGE_WIFI_SETUP_MARGIN_TOP + PAGE_WIFI_SETUP_FONT1_HEIGHT*1 + PAGE_WIFI_SETUP_FONT2_HEIGHT*1 + PAGE_WIFI_SETUP_TEXT_MARGIN*1
        );
        font_task_draw_string(
            page_wifi_setup.ssid,
            strlen(page_wifi_setup.ssid),
            PAGE_WIFI_SETUP_FONT2,
            PAGE_WIFI_SETUP_MARGIN_LEFT,
            STATUS_BAR_HEIGHT + PAGE_WIFI_SETUP_MARGIN_TOP + PAGE_WIFI_SETUP_FONT1_HEIGHT*2 + PAGE_WIFI_SETUP_FONT2_HEIGHT*1 + PAGE_WIFI_SETUP_TEXT_MARGIN*1
        );
        page_wifi_setup.redraw_ssid = false;
    }

    if(page_wifi_setup.redraw_password) {
        font_task_draw_string(
            PAGE_WIFI_SETUP_PASSWORD_STR,
            strlen(PAGE_WIFI_SETUP_PASSWORD_STR),
            PAGE_WIFI_SETUP_FONT1,
            PAGE_WIFI_SETUP_MARGIN_LEFT,
            STATUS_BAR_HEIGHT + PAGE_WIFI_SETUP_MARGIN_TOP + PAGE_WIFI_SETUP_FONT1_HEIGHT*2 + PAGE_WIFI_SETUP_FONT2_HEIGHT*2 + PAGE_WIFI_SETUP_TEXT_MARGIN*2
        );
        font_task_draw_string(
            page_wifi_setup.password,
            strlen(page_wifi_setup.password),
            PAGE_WIFI_SETUP_FONT2,
            PAGE_WIFI_SETUP_MARGIN_LEFT,
            STATUS_BAR_HEIGHT + PAGE_WIFI_SETUP_MARGIN_TOP + PAGE_WIFI_SETUP_FONT1_HEIGHT*3 + PAGE_WIFI_SETUP_FONT2_HEIGHT*2 + PAGE_WIFI_SETUP_TEXT_MARGIN*2
        );
        page_wifi_setup.redraw_password = false;
    }
}