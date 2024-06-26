/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * psram.h: Driver for PSRAM
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

#ifndef PSRAM_H
#define PSRAM_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
} PSRAM;

extern PSRAM psram;

void psram_init(void);
void psram_tick(void);

#endif