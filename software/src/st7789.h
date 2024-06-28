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

#define LCD_MAX_ROWS 8
#define LCD_MAX_COLUMNS 128

typedef struct {
	bool initialize;
	bool reset;
	bool new_invert;
	bool new_backlight;
	bool new_contrast;

	uint8_t display_user[LCD_MAX_ROWS][LCD_MAX_COLUMNS];
	uint8_t display_user_save[LCD_MAX_ROWS][LCD_MAX_COLUMNS];
	uint8_t display_gui[LCD_MAX_ROWS][LCD_MAX_COLUMNS];

	uint8_t display_write[LCD_MAX_ROWS][LCD_MAX_COLUMNS];
	uint8_t display_mask_write[LCD_MAX_ROWS][LCD_MAX_COLUMNS];

	bool display_user_changed;
	bool display_gui_changed;

	uint8_t display_configuration_contrast;
	uint8_t display_configuration_backlight;
	bool display_configuration_invert;

	bool redraw_all;
	bool automatic_draw;

	uint16_t read_chunk_offset;
} ST7789;

extern ST7789 st7789;

void st7789_init(void);
void st7789_tick(void);

// Commands from ST7789VW Spec V1.0 page 156ff
#define ST778_CMD_SWRESET    0x01 // Software reset
#define ST778_CMD_RDDID      0x04 // Read display ID
#define ST778_CMD_RDDST      0x09 // Read display status
#define ST778_CMD_RDDPM      0x0A // Read display power
#define ST778_CMD_RDD_MADCTL 0x0B // Read display
#define ST778_CMD_RDD_COLMOD 0x0C // Read display pixel
#define ST778_CMD_RDDIM      0x0D // Read display image
#define ST778_CMD_RDDSM      0x0E // Read display signal
#define ST778_CMD_RDDSDR     0x0F // Read display self-diagnostic result
#define ST778_CMD_SLPIN      0x10 // Sleep in
#define ST778_CMD_SLPOUT     0x11 // Sleep out
#define ST778_CMD_PTLON      0x12 // Partial mode on
#define ST778_CMD_NORON      0x13 // Partial off (normal)
#define ST778_CMD_INVOFF     0x20 // Display inversion off
#define ST778_CMD_INVON      0x21 // Display inversion on
#define ST778_CMD_GAMSET     0x26 // Gamma set
#define ST778_CMD_DISPOFF    0x28 // Display off
#define ST778_CMD_DISPON     0x29 // Display on
#define ST778_CMD_CASET      0x2A // Column address set
#define ST778_CMD_RASET      0x2B // Row address set
#define ST778_CMD_RAMWR      0x2C // Memory write
#define ST778_CMD_RAMRD      0x2E // Memory read
#define ST778_CMD_PTLAR      0x30 // Partial start/end address set
#define ST778_CMD_VSCRDEF    0x33 // Vertical scrolling definition
#define ST778_CMD_TEOFF      0x34 // Tearing effect line off
#define ST778_CMD_TEON       0x35 // Tearing effect line on
#define ST778_CMD_MADCTL     0x36 // Memory data access control
#define ST778_CMD_VSCRSADD   0x37 // Vertical scrolling start address
#define ST778_CMD_IDMOFF     0x38 // Idle mode off
#define ST778_CMD_IDMON      0x39 // Idle mode on
#define ST778_CMD_COLMOD     0x3A // Interface pixel format
#define ST778_CMD_RAMWRC     0x3C // Memory write continue
#define ST778_CMD_RAMRDC     0x3E // Memory read continue
#define ST778_CMD_TESCAN     0x44 // Set tear scanline
#define ST778_CMD_RDTESCAN   0x45 // Get tear scanline
#define ST778_CMD_WRDISBV    0x51 // Write display brightness
#define ST778_CMD_RDDISBV    0x52 // Read display brightness
#define ST778_CMD_WRCTRLD    0x53 // Write CTRL display
#define ST778_CMD_RDCTRLD    0x54 // Read CTRL display
#define ST778_CMD_WRCACE     0x55 // Write content adaptive brightness control and color enhancement
#define ST778_CMD_RDCABC     0x56 // Read content adaptive brightness control
#define ST778_CMD_WRCABCMB   0x5E // Write CABC minimum brightness
#define ST778_CMD_RDCABCMB   0x5F // Read CABC minimum brightness
#define ST778_CMD_RDABCSDR   0x68 // Read automatic brightness control self-diagnostic result
#define ST778_CMD_RDID1      0xDA // Read ID1
#define ST778_CMD_RDID2      0xDB // Read ID2
#define ST778_CMD_RDID3      0xDC // Read ID3

#endif
