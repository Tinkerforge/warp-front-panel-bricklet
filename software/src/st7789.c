/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * st7789.c: Driver for ST7789 lcd controller
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

#include "st7789.h"

#include "configs/config_st7789.h"

#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/utility/util_definitions.h"
#include "bricklib2/os/coop_task.h"
#include "spi.h"

#include "by25q.h"
#include "font.h"
#include "sprite.h"
#include "button.h"
#include "page_front.h"
#include "page_wifi_setup.h"
#include "display.h"
#include "status_bar.h"

CoopTask st7789_task;
ST7789 st7789;

void st7789_init_spi(void) {
    // SELECT pin configuration
    const XMC_GPIO_CONFIG_t select_pin_config = {
        .mode             = ST7789_SELECT_PIN_AF,
        .output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
    };

    // Configure slave select pin
    XMC_GPIO_Init(ST7789_SELECT_PIN, &select_pin_config);

    spi_init();
}

void st7789_task_write_byte(const uint8_t data) {
    XMC_GPIO_SetOutputHigh(ST7789_CD_PIN);
    spi_task_transceive(&data, NULL, 1, ST7789_SLAVE, 0);
}

void st7789_task_write_data(const uint8_t *data, const uint32_t length) {
    XMC_GPIO_SetOutputHigh(ST7789_CD_PIN);
    spi_task_transceive(data, NULL, length, ST7789_SLAVE, 0);
}

void st7789_task_write_display(const uint16_t *data, const uint32_t length) {
    XMC_GPIO_SetOutputHigh(ST7789_CD_PIN);
    spi_task_transceive((uint8_t*)data, NULL, length*sizeof(uint16_t), ST7789_SLAVE, SPI_TRANSCEIVE_OPTION_SWAP_U16);

    // If there is data to write to the by25q, we wait here until it is written.
    // This makes sure that there can be at least on write to the by25q per write to the display.
    // Otherwise the st7789 could block the SPI bus.
    while(by25q_has_data_to_write()) {
        coop_task_yield();
    }
}

void st7789_task_write_command(const uint8_t command) {
    XMC_GPIO_SetOutputLow(ST7789_CD_PIN);
    spi_task_transceive(&command, NULL, 1, ST7789_SLAVE, 0);
}

void st7789_task_write_command_byte(const uint8_t command, const uint8_t command_data) {
    st7789_task_write_command(command);
    st7789_task_write_byte(command_data);
}

void st7789_task_write_command_data(const uint8_t command, const uint8_t *command_data, const uint32_t length) {
    st7789_task_write_command(command);
    st7789_task_write_data(command_data, length);
}

void st7789_task_sleep(void) {
    st7789_task_write_command(ST7789_CMD_DISPOFF); // Display off
    st7789_task_write_command(ST7789_CMD_SLPIN);   // Enter sleep mode
}

void st7789_task_wakeup(void) {
    st7789_task_write_command(ST7789_CMD_SLPOUT); // Leave sleep mode
    coop_task_sleep_ms(120);
    st7789_task_write_command(ST7789_CMD_DISPON); // Display on
}

void st7789_task_reset(void) {
    XMC_GPIO_SetOutputLow(ST7789_RESET_PIN);
    coop_task_sleep_ms(10);
    XMC_GPIO_SetOutputHigh(ST7789_RESET_PIN);
    coop_task_sleep_ms(120);
}

void st7789_set_window(const uint16_t x_start, const uint16_t y_start, const uint16_t x_end, const uint16_t y_end) {
    // Column address
    st7789_task_write_command_data(ST7789_CMD_CASET, (uint8_t[]){x_start >> 8, x_start & 0xFF, x_end >> 8, x_end & 0xFF}, 4);

    // Row address set
    st7789_task_write_command_data(ST7789_CMD_RASET, (uint8_t[]){y_start >> 8, y_start & 0xFF, y_end >> 8, y_end & 0xFF}, 4);

    // Write to RAM
    st7789_task_write_command(ST7789_CMD_RAMWR);
}

void st7789_task_init(void) {
    coop_task_sleep_ms(100);

    // Colormode to 16bit RGB (565)
    st7789_task_write_command_byte(ST7789_CMD_COLMOD, 0x55);

    // Screen refresh left to right, top to bottom, color RGB
    st7789_task_write_command_byte(ST7789_CMD_MADCTL, 0b01100000); // MX=1, MV=1

    // Porch Control
    st7789_task_write_command_data(ST7789_CMD_PORCTRL, (uint8_t[]){0x0C, 0x0C, 0x00, 0x33, 0x33}, 5);

    // LCD voltage generator settings
    st7789_task_write_command_byte(ST7789_CMD_GCTRL, 0x44);    // Gate control -> VGL=-9.6V, VGH=12.2V
    st7789_task_write_command_byte(ST7789_CMD_VCOMS, 0x24);    // VCOM setting-> 1.0V
    st7789_task_write_command_byte(ST7789_CMD_LCMCTRL, 0x2C);  // LCM control -> XOR RGB setting, XOR MX setting and reverse source output order
    st7789_task_write_command_byte(ST7789_CMD_VDVVRHEN, 0x01); // VDV and VRH command enable
    st7789_task_write_command_byte(ST7789_CMD_VRHS, 0x13);     // VRH -> 4.5V
    st7789_task_write_command_byte(ST7789_CMD_VDVS, 0x20);     // VDV -> 0V
    st7789_task_write_command_byte(ST7789_CMD_FRCTRL2, 0x0F);  // Frame rate -> 60Hz

    // Power control: AVDD -> 6.8V, AVCL -> -4.8V, VDDS > 2.3V
    st7789_task_write_command_data(ST7789_CMD_PWCTRL1, (uint8_t[]){0xA4, 0xA1}, 2);

    // Positive and negative voltage gamma control (values from EastRising example)
    st7789_task_write_command_data(ST7789_CMD_PVGAMCTRL, (uint8_t[]){0xF0, 0x00, 0x04, 0x04, 0x04, 0x05, 0x29, 0x33, 0x3E, 0x38, 0x12, 0x12, 0x28, 0x30}, 14);
    st7789_task_write_command_data(ST7789_CMD_NVGAMCTRL, (uint8_t[]){0xF0, 0x07, 0x0A, 0x0D, 0x0B, 0x07, 0x28, 0x33, 0x3E, 0x36, 0x14, 0x14, 0x29, 0x32}, 14);

    // Show display
    st7789_task_write_command(ST7789_CMD_INVON);  // Display inversion
    st7789_task_write_command(ST7789_CMD_SLPOUT); // Leave sleep mode
    coop_task_sleep_ms(120);
    st7789_task_write_command(ST7789_CMD_DISPON); // Display on
    coop_task_sleep_ms(50);
}

void st7789_task_display_off(void) {
    st7789_task_write_command(ST7789_CMD_DISPOFF);
    coop_task_sleep_ms(50);
}

void st7789_task_display_on(void) {
    st7789_task_write_command(ST7789_CMD_DISPON);
}

void st7789_task_tick(void) {
    st7789_task_reset();
    st7789_task_init();

    // Read top-left pixel of WIFI symbol for status bar background color
    by25q_task_read((uint8_t*)&status_bar.background_color, 2, sprite_list[SPRITE_STATUS_ICON_WIFI_3BAR].start_address);

    bool was_active = true;
    while(true) {
        display_task_tick();
        coop_task_yield();

        if(!display.is_active && was_active) {
            st7789_task_display_off();
            was_active = false;
        } else if(display.is_active && !was_active) {
            st7789_task_display_on();
            was_active = true;
        }
    }
}

void st7789_init(void) {
    memset(&st7789, 0, sizeof(ST7789));
    st7789_init_spi();

    const XMC_GPIO_CONFIG_t pin_config_output_high = {
        .mode             = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
        .output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
    };

    XMC_GPIO_Init(ST7789_RESET_PIN, &pin_config_output_high);
    XMC_GPIO_Init(ST7789_CD_PIN, &pin_config_output_high);
    XMC_GPIO_Init(ST7789_BACKLIGHT_PIN, &pin_config_output_high);
    coop_task_init(&st7789_task, st7789_task_tick);
}

void st7789_tick(void) {
    coop_task_tick(&st7789_task);
}
