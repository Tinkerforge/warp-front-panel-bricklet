/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * page_front.c: First display page
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

#include "page_front.h"

#include <string.h>

#include "st7789.h"

PageFront page_front;

void page_front_init(void) {
	memset(&page_front, 0, sizeof(PageFront));
}

void page_front_task_tick(void) {
	if(page_front.redraw_everything) {
		page_front.redraw_background = true;
		page_front.redraw_everything = false;
	}

	if(page_front.redraw_background) {
		st7789_task_draw_background();
		page_front.redraw_background = false;
	}
}