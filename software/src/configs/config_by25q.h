/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config_by25q.h: Configuration for BY25Q flash
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

#ifndef CONFIG_BY25Q_H
#define CONFIG_BY25Q_H

#include "xmc_gpio.h"

// SPI is shared with ST7789, we only define the BY25Q specific pins here

#define BY25Q_SELECT_PIN           P4_7
#define BY25Q_SELECT_PIN_AF        (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7 | P4_7_AF_U1C0_SELO0)
#define BY25Q_SLAVE                XMC_SPI_CH_SLAVE_SELECT_0

#define BY25Q_NWP_PIN              P2_1
#define BY25Q_NHOLD_PIN            P2_13

#endif
