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

#include "st7789.h"
#include "by25q.h"

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	const uint8_t length = ((TFPMessageHeader*)message)->length;
	switch(tfp_get_fid_from_message(message)) {
		case FID_SET_EEPROM_INDEX:    return length != sizeof(SetEEPROMIndex)    ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : set_eeprom_index(message);
		case FID_GET_EEPROM_INDEX:    return length != sizeof(GetEEPROMIndex)    ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : get_eeprom_index(message, response);
		case FID_SET_EEPROM_DATA:     return length != sizeof(SetEEPROMData)     ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : set_eeprom_data(message, response);
		case FID_GET_EEPROM_DATA:     return length != sizeof(GetEEPROMData)     ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : get_eeprom_data(message, response);
		case FID_ERASE_EEPROM_SECTOR: return length != sizeof(EraseEEPROMSector) ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : erase_eeprom_sector(message);
		case FID_ERASE_EEPROM:        return length != sizeof(EraseEEPROM)       ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : erase_eeprom(message);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}


BootloaderHandleMessageResponse set_eeprom_index(const SetEEPROMIndex *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_eeprom_index(const GetEEPROMIndex *data, GetEEPROMIndex_Response *response) {
	response->header.length = sizeof(GetEEPROMIndex_Response);
	response->page_index = by25q.manufacturer_id + 1;
	response->sub_page_index = by25q.device_id + 2;
	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_eeprom_data(const SetEEPROMData *data, SetEEPROMData_Response *response) {
	response->header.length = sizeof(SetEEPROMData_Response);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_eeprom_data(const GetEEPROMData *data, GetEEPROMData_Response *response) {
	response->header.length = sizeof(GetEEPROMData_Response);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse erase_eeprom_sector(const EraseEEPROMSector *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse erase_eeprom(const EraseEEPROM *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}





void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	communication_callback_init();
}
