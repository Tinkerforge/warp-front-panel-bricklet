/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * status_bar.h: Status bar drawing functions
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

#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool redraw_everything;
    bool redraw_background;
    bool redraw_wifi;
    bool redraw_ethernet;
    bool redraw_clock;

    uint16_t background_color;

    uint32_t ethernet_status;
	uint32_t wifi_status;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
} StatusBar;

extern StatusBar status_bar;

void status_bar_init(void);
void status_bar_task_tick(void);


#endif