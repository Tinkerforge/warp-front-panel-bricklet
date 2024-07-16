/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * page_wifi_setup.h: Display page for WIFI setup
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

#ifndef PAGE_WIFI_SETUP_H
#define PAGE_WIFI_SETUP_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool redraw_everything;
    bool redraw_background;
    bool redraw_qrcode;

    char ip_address[15+1];
    char ssid[49+1];
    char password[64+1];
} PageWifiSetup;

extern PageWifiSetup page_wifi_setup;

void page_wifi_setup_init(void);
void page_wifi_setup_task_tick(void);


#endif