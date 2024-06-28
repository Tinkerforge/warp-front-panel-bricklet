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
#include "bricklib2/os/coop_task.h"
#include "spi.h"

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

void st7789_task_write_data_byte(const uint8_t data) {
	XMC_GPIO_SetOutputHigh(ST7789_CD_PIN);
	spi_task_transceive(&data, 1, XMC_SPI_CH_SLAVE_SELECT_0);
}

void st7789_task_write_command_byte(const uint8_t command) {
	XMC_GPIO_SetOutputLow(ST7789_CD_PIN);
	spi_task_transceive(&command, 1, XMC_SPI_CH_SLAVE_SELECT_0);
}

void st7789_task_write_data(const uint8_t *data, const uint32_t length) {
	XMC_GPIO_SetOutputHigh(ST7789_CD_PIN);
	spi_task_transceive(data, length, XMC_SPI_CH_SLAVE_SELECT_0);
}

void st7789_task_write_command(const uint8_t *command, const uint32_t length) {
	XMC_GPIO_SetOutputLow(ST7789_CD_PIN);
	spi_task_transceive(command, length, XMC_SPI_CH_SLAVE_SELECT_0);
}


void st7789_set_reset(const bool reset) {
	if(reset) {
		XMC_GPIO_SetOutputLow(ST7789_RESET_PIN);
	} else {
		XMC_GPIO_SetOutputHigh(ST7789_RESET_PIN);
	}
}

#define X_SHIFT 0
#define Y_SHIFT 0
#define WHITE       0xFFFF
#define BLACK       0x0000
#define BLUE        0x001F
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define GRAY        0X8430
#define BRED        0XF81F
#define GRED        0XFFE0
#define GBLUE       0X07FF
#define BROWN       0XBC40
#define BRRED       0XFC07
#define DARKBLUE    0X01CF
#define LIGHTBLUE   0X7D7C
#define GRAYBLUE    0X5458

#define LIGHTGREEN  0X841F
#define LGRAY       0XC618
#define LGRAYBLUE   0XA651
#define LBBLUE      0X2B12

void st7789_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	uint16_t x_start = x0 + X_SHIFT, x_end = x1 + X_SHIFT;
	uint16_t y_start = y0 + Y_SHIFT, y_end = y1 + Y_SHIFT;

	/* Column Address set */
	st7789_task_write_command_byte(ST778_CMD_CASET);
	{
		uint8_t data[] = {x_start >> 8, x_start & 0xFF, x_end >> 8, x_end & 0xFF};
		st7789_task_write_data(data, sizeof(data));
	}

	/* Row Address set */
	st7789_task_write_command_byte(ST778_CMD_RASET);
	{
		uint8_t data[] = {y_start >> 8, y_start & 0xFF, y_end >> 8, y_end & 0xFF};
		st7789_task_write_data(data, sizeof(data));
	}
	/* Write to RAM */
	st7789_task_write_command_byte(ST778_CMD_RAMWR);
}

/* Page Address Order ('0': Top to Bottom, '1': the opposite) */
#define ST7789_MADCTL_MY  0x80
/* Column Address Order ('0': Left to Right, '1': the opposite) */
#define ST7789_MADCTL_MX  0x40
/* Page/Column Order ('0' = Normal Mode, '1' = Reverse Mode) */
#define ST7789_MADCTL_MV  0x20
/* Line Address Order ('0' = LCD Refresh Top to Bottom, '1' = the opposite) */
#define ST7789_MADCTL_ML  0x10
/* RGB/BGR Order ('0' = RGB, '1' = BGR) */
#define ST7789_MADCTL_RGB 0x00

void st7789_set_rotation(uint8_t m)
{
	st7789_task_write_command_byte(ST778_CMD_MADCTL);	// MADCTL
	switch (m) {
	case 0:
		st7789_task_write_data_byte(ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB);
		break;
	case 1:
		st7789_task_write_data_byte(ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
		break;
	case 2:
		st7789_task_write_data_byte(ST7789_MADCTL_RGB);
		break;
	case 3:
		st7789_task_write_data_byte(ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
		break;
	default:
		break;
	}
}

#define ST7789_COLOR_MODE_16bit 0x55    //  RGB565 (16bit)
void st7789_task_tick(void) {
	st7789_set_reset(true);
	coop_task_sleep_ms(10);
	st7789_set_reset(false);
	coop_task_sleep_ms(20);

    st7789_task_write_command_byte(ST778_CMD_COLMOD);		//	Set color mode
    st7789_task_write_data_byte(ST7789_COLOR_MODE_16bit);
  	st7789_task_write_command_byte(0xB2);				//	Porch control
	{
		uint8_t data[] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
		st7789_task_write_data(data, sizeof(data));
	}
	st7789_set_rotation(2);
	//ST7789_SetRotation(ST7789_ROTATION);	//	MADCTL (Display Rotation)

	/* Internal LCD Voltage generator settings */
    st7789_task_write_command_byte(0XB7);				//	Gate Control
    st7789_task_write_data_byte(0x35);			//	Default value
    st7789_task_write_command_byte(0xBB);				//	VCOM setting
    st7789_task_write_data_byte(0x19);			//	0.725v (default 0.75v for 0x20)
    st7789_task_write_command_byte(0xC0);				//	LCMCTRL
    st7789_task_write_data_byte (0x2C);			//	Default value
    st7789_task_write_command_byte (0xC2);				//	VDV and VRH command Enable
    st7789_task_write_data_byte (0x01);			//	Default value
    st7789_task_write_command_byte (0xC3);				//	VRH set
    st7789_task_write_data_byte (0x12);			//	+-4.45v (defalut +-4.1v for 0x0B)
    st7789_task_write_command_byte (0xC4);				//	VDV set
    st7789_task_write_data_byte (0x20);			//	Default value
    st7789_task_write_command_byte (0xC6);				//	Frame rate control in normal mode
    st7789_task_write_data_byte (0x0F);			//	Default value (60HZ)
    st7789_task_write_command_byte (0xD0);				//	Power control
    st7789_task_write_data_byte (0xA4);			//	Default value
    st7789_task_write_data_byte (0xA1);			//	Default value
	/**************** Division line ****************/

	st7789_task_write_command_byte(0xE0);
	{
		uint8_t data[] = {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23};
		st7789_task_write_data(data, sizeof(data));
	}

    st7789_task_write_command_byte(0xE1);
	{
		uint8_t data[] = {0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23};
		st7789_task_write_data(data, sizeof(data));
	}
    st7789_task_write_command_byte (ST778_CMD_INVON);		//	Inversion ON
	st7789_task_write_command_byte (ST778_CMD_SLPOUT);	//	Out of sleep mode
  	st7789_task_write_command_byte (ST778_CMD_NORON);		//	Normal Display on
  	st7789_task_write_command_byte (ST778_CMD_DISPON);	//	Main screen turned on

	coop_task_sleep_ms(50);
	//ST7789_Fill_Color(BLACK);				//	Fill with Black.

	uint16_t data[320];
	while(true) {
		for(uint16_t i = 0; i < 320; i++) {
			data[i] = RED;
		}
		for(uint8_t i = 0; i < 240; i++) {
			st7789_set_window(i, 0, i, 320);
			st7789_task_write_data((uint8_t*)data, sizeof(data));
		}
		coop_task_sleep_ms(250);
		for(uint16_t i = 0; i < 320; i++) {
			data[i] = GREEN;
		}
		for(uint8_t i = 0; i < 240; i++) {
			st7789_set_window(i, 0, i, 320);
			st7789_task_write_data((uint8_t*)data, sizeof(data));
		}
		coop_task_sleep_ms(250);
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

	XMC_GPIO_Init(ST7789_RESET_PIN, &pin_config_output_high);
	XMC_GPIO_Init(ST7789_CD_PIN, &pin_config_output_high);
	XMC_GPIO_Init(ST7789_BACKLIGHT_PIN, &pin_config_output_high);
	coop_task_init(&st7789_task, st7789_task_tick);
}

void st7789_tick(void) {
	coop_task_tick(&st7789_task);
}
