/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * main.c: Initialization for WARP Front Panel Bricklet
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

#include <stdio.h>
#include <stdbool.h>

#include "configs/config.h"

#include "bricklib2/bootloader/bootloader.h"
#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/logging/logging.h"
#include "communication.h"

#include "st7789.h"
#include "by25q.h"
#include "button.h"
#include "led.h"
#include "psram.h"
#include "font.h"
#include "sprite.h"
#include "page_front.h"
#include "page_wifi_setup.h"
#include "status_bar.h"
#include "display.h"

int main(void) {
    logging_init();
    logd("Start WARP Front Panel Bricklet\n\r");

    communication_init();
    st7789_init();
    by25q_init();
    button_init();
    led_init();
    psram_init();
    font_init();
    sprite_init();
    page_front_init();
    page_wifi_setup_init();
    status_bar_init();
    display_init();

    while(true) {
        bootloader_tick();
        communication_tick();
        st7789_tick();
        by25q_tick();
        button_tick();
        led_tick();
        psram_tick();
    }
}
