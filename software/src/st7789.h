/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * st7789.h: Driver for ST7789 lcd controller
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

#ifndef ST7789_H
#define ST7789_H

#include <stdint.h>
#include <stdbool.h>


typedef struct {
} ST7789;

extern ST7789 st7789;

void st7789_init(void);
void st7789_tick(void);

void st7789_task_write_byte(const uint8_t data);
void st7789_task_write_data(const uint8_t *data, const uint32_t length);
void st7789_task_write_display(const uint16_t *data, const uint32_t length);
void st7789_task_write_command(const uint8_t command);
void st7789_task_write_command_byte(const uint8_t command, const uint8_t command_data);
void st7789_task_write_command_data(const uint8_t command, const uint8_t *command_data, const uint32_t length);
void st7789_task_sleep(void);
void st7789_task_wakeup(void);
void st7789_task_reset(void);
void st7789_set_window(const uint16_t x_start, const uint16_t const y_start, const uint16_t x_end, const uint16_t y_end);

// Commands from ST7789VW Spec V1.0 page 156ff
#define ST7789_CMD_SWRESET    0x01 // Software reset
#define ST7789_CMD_RDDID      0x04 // Read display ID
#define ST7789_CMD_RDDST      0x09 // Read display status
#define ST7789_CMD_RDDPM      0x0A // Read display power
#define ST7789_CMD_RDD_MADCTL 0x0B // Read display
#define ST7789_CMD_RDD_COLMOD 0x0C // Read display pixel
#define ST7789_CMD_RDDIM      0x0D // Read display image
#define ST7789_CMD_RDDSM      0x0E // Read display signal
#define ST7789_CMD_RDDSDR     0x0F // Read display self-diagnostic result
#define ST7789_CMD_SLPIN      0x10 // Sleep in
#define ST7789_CMD_SLPOUT     0x11 // Sleep out
#define ST7789_CMD_PTLON      0x12 // Partial mode on
#define ST7789_CMD_NORON      0x13 // Partial off (normal)
#define ST7789_CMD_INVOFF     0x20 // Display inversion off
#define ST7789_CMD_INVON      0x21 // Display inversion on
#define ST7789_CMD_GAMSET     0x26 // Gamma set
#define ST7789_CMD_DISPOFF    0x28 // Display off
#define ST7789_CMD_DISPON     0x29 // Display on
#define ST7789_CMD_CASET      0x2A // Column address set
#define ST7789_CMD_RASET      0x2B // Row address set
#define ST7789_CMD_RAMWR      0x2C // Memory write
#define ST7789_CMD_RAMRD      0x2E // Memory read
#define ST7789_CMD_PTLAR      0x30 // Partial start/end address set
#define ST7789_CMD_VSCRDEF    0x33 // Vertical scrolling definition
#define ST7789_CMD_TEOFF      0x34 // Tearing effect line off
#define ST7789_CMD_TEON       0x35 // Tearing effect line on
#define ST7789_CMD_MADCTL     0x36 // Memory data access control
#define ST7789_CMD_VSCRSADD   0x37 // Vertical scrolling start address
#define ST7789_CMD_IDMOFF     0x38 // Idle mode off
#define ST7789_CMD_IDMON      0x39 // Idle mode on
#define ST7789_CMD_COLMOD     0x3A // Interface pixel format
#define ST7789_CMD_RAMWRC     0x3C // Memory write continue
#define ST7789_CMD_RAMRDC     0x3E // Memory read continue
#define ST7789_CMD_TESCAN     0x44 // Set tear scanline
#define ST7789_CMD_RDTESCAN   0x45 // Get tear scanline
#define ST7789_CMD_WRDISBV    0x51 // Write display brightness
#define ST7789_CMD_RDDISBV    0x52 // Read display brightness
#define ST7789_CMD_WRCTRLD    0x53 // Write CTRL display
#define ST7789_CMD_RDCTRLD    0x54 // Read CTRL display
#define ST7789_CMD_WRCACE     0x55 // Write content adaptive brightness control and color enhancement
#define ST7789_CMD_RDCABC     0x56 // Read content adaptive brightness control
#define ST7789_CMD_WRCABCMB   0x5E // Write CABC minimum brightness
#define ST7789_CMD_RDCABCMB   0x5F // Read CABC minimum brightness
#define ST7789_CMD_RDABCSDR   0x68 // Read automatic brightness control self-diagnostic result
#define ST7789_CMD_RAMCTRL    0xB0
#define ST7789_CMD_RGBCTRL    0xB1
#define ST7789_CMD_PORCTRL    0xB2
#define ST7789_CMD_FRCTRL1    0xB3
#define ST7789_CMD_PARCTRL    0xB5
#define ST7789_CMD_GCTRL      0xB7
#define ST7789_CMD_GTADJ      0xB8
#define ST7789_CMD_DGMEN      0xBA
#define ST7789_CMD_VCOMS      0xBB
#define ST7789_CMD_POWSAVE    0xBC
#define ST7789_CMD_DLPOFFSAVE 0xBD
#define ST7789_CMD_LCMCTRL    0xC0
#define ST7789_CMD_IDSET      0xC1
#define ST7789_CMD_VDVVRHEN   0xC2
#define ST7789_CMD_VRHS       0xC3
#define ST7789_CMD_VDVS       0xC4
#define ST7789_CMD_VCMOFSET   0xC5
#define ST7789_CMD_FRCTRL2    0xC6
#define ST7789_CMD_CABCCTRL   0xC7
#define ST7789_CMD_REGSEL1    0xC8
#define ST7789_CMD_REGSEL2    0xCA
#define ST7789_CMD_PWMFRSEL   0xCC
#define ST7789_CMD_PWCTRL1    0xD0
#define ST7789_CMD_VAPVANEN   0xD2
#define ST7789_CMD_RDID1      0xDA // Read ID1
#define ST7789_CMD_RDID2      0xDB // Read ID2
#define ST7789_CMD_RDID3      0xDC // Read ID3
#define ST7789_CMD_CMD2EN     0xDF
#define ST7789_CMD_PVGAMCTRL  0xE0
#define ST7789_CMD_NVGAMCTRL  0xE1
#define ST7789_CMD_DGMLUTR    0xE2
#define ST7789_CMD_DGMLUTB    0xE3
#define ST7789_CMD_GATECTRL   0xE4
#define ST7789_CMD_SPI2EN     0xE7
#define ST7789_CMD_PWCTRL2    0xE8
#define ST7789_CMD_EQCTRL     0xE9
#define ST7789_CMD_PROMCTRL   0xEC
#define ST7789_CMD_PROMEN     0xFA
#define ST7789_CMD_NVMSET     0xFC
#define ST7789_CMD_PROMACT    0xFE

// RGB565 colors
#define ST7789_COLOR_WHITE    0xFFFF
#define ST7789_COLOR_BLACK    0x0000
#define ST7789_COLOR_BLUE     0x001F
#define ST7789_COLOR_RED      0xF800
#define ST7789_COLOR_MAGENTA  0xF81F
#define ST7789_COLOR_GREEN    0x07E0
#define ST7789_COLOR_CYAN     0x7FFF
#define ST7789_COLOR_YELLOW   0xFFE0
#define ST7789_COLOR_GRAY     0X8430

#endif
