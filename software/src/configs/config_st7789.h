/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config_st7789.h: Configuration for ST7789x lcd controller
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

#ifndef CONFIG_ST7789_H
#define CONFIG_ST7789_H

#include "xmc_gpio.h"

#define ST7789_SPI_BAUDRATE         1000000
#define ST7789_USIC_CHANNEL         USIC1_CH0
#define ST7789_USIC                 XMC_SPI1_CH0

#define ST7789_SCLK_PIN             P4_6
#define ST7789_SCLK_PIN_AF          (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7 | P4_6_AF_U1C0_SCLKOUT)

#define ST7789_SELECT_PIN           P4_7
#define ST7789_SELECT_PIN_AF        (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT6 | P4_7_AF_U1C0_SELO0)

#define ST7789_MOSI_PIN             P4_5
#define ST7789_MOSI_PIN_AF          (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT6 | P4_5_AF_U1C0_DOUT0)

#define ST7789_MISO_PIN             P4_4
#define ST7789_MISO_INPUT           XMC_USIC_CH_INPUT_DX0
#define ST7789_MISO_SOURCE          0b010 // DX0C

#define ST7789_SERVICE_REQUEST_RX   2
#define ST7789_SERVICE_REQUEST_TX   3

#define ST7789_IRQ_RX               11
#define ST7789_IRQ_RX_PRIORITY      0
#define ST7789_IRQCTRL_RX           XMC_SCU_IRQCTRL_USIC1_SR2_IRQ11
#define ST7789_IRQ_TX               12
#define ST7789_IRQ_TX_PRIORITY      1
#define ST7789_IRQCTRL_TX           XMC_SCU_IRQCTRL_USIC1_SR3_IRQ12

#define ST7789_CLOCK_PASSIVE_LEVEL  XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_1_DELAY_DISABLED
#define ST7789_CLOCK_OUTPUT         XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK
#define ST7789_SLAVE                XMC_SPI_CH_SLAVE_SELECT_0

#define ST7789_RX_FIFO_DATA_POINTER 0
#define ST7789_TX_FIFO_DATA_POINTER 16
#define ST7789_RX_FIFO_SIZE         XMC_USIC_CH_FIFO_SIZE_16WORDS
#define ST7789_TX_FIFO_SIZE         XMC_USIC_CH_FIFO_SIZE_16WORDS

#define ST7789_RESET_PIN            P1_0
#define ST7789_BACKLIGHT_PIN        P1_1
#define ST7789_CD_PIN               P0_9

#endif
