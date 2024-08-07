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


void st7789_task_tick(void) {
    st7789_task_reset();
    st7789_task_init();

    // Read top-left pixel of WIFI symbol for status bar background color
    by25q_task_read((uint8_t*)&status_bar.background_color, 2, sprite_list[SPRITE_STATUS_ICON_WIFI].start_address);

#if 0
    fill_u16(ST7789_COLOR_BLACK, circle, CIRCLE_SIZE*CIRCLE_SIZE);
    st7789_draw_circle(circle, CIRCLE_SIZE);
#endif

#if 0
    st7789_task_draw_filled_rect(ST7789_COLOR_WHITE, 0, 0, 319, 239);
    st7789_task_draw_filled_rect(ST7789_COLOR_BLACK, 1, 1, 318, 238);
    uint16_t y = 1;
    uint16_t x = 0;
    int16_t dir = 1;
#endif

#if 0
    font_task_draw_string("!\"#$%%&'()*+,-./01234567", 24, 0, 0, 0);
    font_task_draw_string("89:;<=>?@ABCDEFGHIJKLMNO", 24, 0, 0, 18);
    font_task_draw_string("PQRSTUVWXYZ[\\]^_`abcdefg", 24, 0, 0, 36);
    font_task_draw_string("hijklmnopqrstuvwxyz{|}~", 24, 0, 0, 54);
    char special_chars[32] = {'\0'};
    for(uint8_t i = 16; i < 33; i++) {
        special_chars[i-16] = i;
    }
    font_task_draw_string(special_chars, strlen(special_chars), 0, 0, 72);
#endif

    while(true) {
        display_task_tick();

#if 0
        if(button.is_pressed) {
            font_task_draw_string(str_pressed, 10, 0, 100, 100);
        } else {
            font_task_draw_string(str_unpressed, 10, 0, 100, 100);
        }
#endif
#if 0
        for(uint32_t add = 0; add < 2; add++) {
            st7789_task_draw_from_by25q(add*320*240*sizeof(uint16_t), 0, 0, 319, 239);
            coop_task_yield();
            coop_task_sleep_ms(2500);
        }
#endif
#if 0
        if(by25q.to_write_index >= 0) {
            st7789_task_write_display((uint16_t*)by25q.data_write, 256/2);
            by25q.to_write_index = -1;
        }
#endif
#if 0
        for(x = 0; x < 320-CIRCLE_SIZE; x++) {
            if((y >= 240-CIRCLE_SIZE) || (y <= 0)){
                dir = -dir;
            }
            y += dir;
            st7789_task_draw_image(circle, x, y, x+CIRCLE_SIZE-1, y+CIRCLE_SIZE-1);
            coop_task_yield();
        }
        for(x = 320-CIRCLE_SIZE-1; x > 0; x--) {
            if((y >= 240-CIRCLE_SIZE) || (y <= 0)){
                dir = -dir;
            }
            y += dir;
            st7789_task_draw_image(circle, x, y, x+CIRCLE_SIZE-1, y+CIRCLE_SIZE-1);
            coop_task_yield();
        }
        uint16_t col = (y>>5)*77;
        st7789_task_draw_filled_rect(col, 0, 0, 319, 239);
#endif
        coop_task_yield();
    }
}

void st7789_init(void) {
    memset(&st7789, 0, sizeof(ST7789));
    st7789_init_spi();

    const XMC_GPIO_CONFIG_t pin_config_output_high = {
        .mode             = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
        .output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
    };

    const XMC_GPIO_CONFIG_t pin_config_output_low = {
        .mode             = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
        .output_level     = XMC_GPIO_OUTPUT_LEVEL_LOW
    };

    const XMC_GPIO_CONFIG_t input_pin_config = {
        .mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
        .input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
    };

    XMC_GPIO_Init(ST7789_MISO2_PIN, &input_pin_config); // unused
    XMC_GPIO_Init(ST7789_RESET_PIN, &pin_config_output_high);
    XMC_GPIO_Init(ST7789_CD_PIN, &pin_config_output_high);
    XMC_GPIO_Init(ST7789_BACKLIGHT_PIN, &pin_config_output_high);
    coop_task_init(&st7789_task, st7789_task_tick);
}

void st7789_tick(void) {
    coop_task_tick(&st7789_task);
}
