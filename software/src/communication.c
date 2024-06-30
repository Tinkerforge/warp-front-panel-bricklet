/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
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

#include "communication.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/os/coop_task.h"

#include "st7789.h"
#include "by25q.h"

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	const uint8_t length = ((TFPMessageHeader*)message)->length;
	switch(tfp_get_fid_from_message(message)) {
		case FID_SET_FLASH_INDEX:    return length != sizeof(SetFlashIndex)    ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : set_flash_index(message);
		case FID_GET_FLASH_INDEX:    return length != sizeof(GetFlashIndex)    ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : get_flash_index(message, response);
		case FID_SET_FLASH_DATA:     return length != sizeof(SetFlashData)     ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : set_flash_data(message, response);
		case FID_ERASE_FLASH_SECTOR: return length != sizeof(EraseFlashSector) ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : erase_flash_sector(message);
		case FID_ERASE_FLASH:        return length != sizeof(EraseFlash)       ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : erase_flash(message);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}


BootloaderHandleMessageResponse set_flash_index(const SetFlashIndex *data) {
	if(by25q.sub_page_index > 3) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	by25q.page_index     = data->page_index;
	by25q.sub_page_index = data->page_index;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_flash_index(const GetFlashIndex *data, GetFlashIndex_Response *response) {
	response->header.length  = sizeof(GetFlashIndex_Response);
	response->page_index     = by25q.page_index;
	response->sub_page_index = by25q.sub_page_index;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_flash_data(const SetFlashData *data, SetFlashData_Response *response) {
	response->header.length = sizeof(SetFlashData_Response);
	while(by25q.to_write_index >= 0) {
		response->next_page_index     = by25q.page_index;
		response->next_sub_page_index = by25q.sub_page_index;
		response->status              = WARP_FRONT_PANEL_FLASH_STATUS_BUSY;

		return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
	}

	memcpy(by25q.data_write + by25q.sub_page_index*64, data->data, 64);
	by25q.sub_page_index++;
	if(by25q.sub_page_index > 3) {
		by25q.sub_page_index = 0;
		by25q.to_write_index = by25q.page_index;
		by25q.page_index++;
	}
	response->next_page_index     = by25q.page_index;
	response->next_sub_page_index = by25q.sub_page_index;
	response->status              = WARP_FRONT_PANEL_FLASH_STATUS_OK;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse erase_flash_sector(const EraseFlashSector *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse erase_flash(const EraseFlash *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}





void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	communication_callback_init();
}
