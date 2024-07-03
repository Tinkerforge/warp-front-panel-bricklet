/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
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

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/bootloader/bootloader.h"

// Default functions
BootloaderHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);
void communication_init(void);

// Constants

#define WARP_FRONT_PANEL_FLASH_STATUS_OK 0
#define WARP_FRONT_PANEL_FLASH_STATUS_BUSY 1

#define WARP_FRONT_PANEL_BOOTLOADER_MODE_BOOTLOADER 0
#define WARP_FRONT_PANEL_BOOTLOADER_MODE_FIRMWARE 1
#define WARP_FRONT_PANEL_BOOTLOADER_MODE_BOOTLOADER_WAIT_FOR_REBOOT 2
#define WARP_FRONT_PANEL_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_REBOOT 3
#define WARP_FRONT_PANEL_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_ERASE_AND_REBOOT 4

#define WARP_FRONT_PANEL_BOOTLOADER_STATUS_OK 0
#define WARP_FRONT_PANEL_BOOTLOADER_STATUS_INVALID_MODE 1
#define WARP_FRONT_PANEL_BOOTLOADER_STATUS_NO_CHANGE 2
#define WARP_FRONT_PANEL_BOOTLOADER_STATUS_ENTRY_FUNCTION_NOT_PRESENT 3
#define WARP_FRONT_PANEL_BOOTLOADER_STATUS_DEVICE_IDENTIFIER_INCORRECT 4
#define WARP_FRONT_PANEL_BOOTLOADER_STATUS_CRC_MISMATCH 5

#define WARP_FRONT_PANEL_STATUS_LED_CONFIG_OFF 0
#define WARP_FRONT_PANEL_STATUS_LED_CONFIG_ON 1
#define WARP_FRONT_PANEL_STATUS_LED_CONFIG_SHOW_HEARTBEAT 2
#define WARP_FRONT_PANEL_STATUS_LED_CONFIG_SHOW_STATUS 3

// Function and callback IDs and structs
#define FID_SET_FLASH_INDEX 1
#define FID_GET_FLASH_INDEX 2
#define FID_SET_FLASH_DATA 3
#define FID_ERASE_FLASH_SECTOR 4
#define FID_ERASE_FLASH 5


typedef struct {
	TFPMessageHeader header;
	uint32_t page_index;
	uint8_t sub_page_index;
} __attribute__((__packed__)) SetFlashIndex;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetFlashIndex;

typedef struct {
	TFPMessageHeader header;
	uint32_t page_index;
	uint8_t sub_page_index;
} __attribute__((__packed__)) GetFlashIndex_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t data[64];
} __attribute__((__packed__)) SetFlashData;

typedef struct {
	TFPMessageHeader header;
	uint32_t next_page_index;
	uint8_t next_sub_page_index;
	uint8_t status;
} __attribute__((__packed__)) SetFlashData_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t sector_index;
} __attribute__((__packed__)) EraseFlashSector;

typedef struct {
	TFPMessageHeader header;
	uint8_t status;
} __attribute__((__packed__)) EraseFlashSector_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) EraseFlash;

typedef struct {
	TFPMessageHeader header;
	uint8_t status;
} __attribute__((__packed__)) EraseFlash_Response;


// Function prototypes
BootloaderHandleMessageResponse set_flash_index(const SetFlashIndex *data);
BootloaderHandleMessageResponse get_flash_index(const GetFlashIndex *data, GetFlashIndex_Response *response);
BootloaderHandleMessageResponse set_flash_data(const SetFlashData *data, SetFlashData_Response *response);
BootloaderHandleMessageResponse erase_flash_sector(const EraseFlashSector *data, EraseFlashSector_Response *response);
BootloaderHandleMessageResponse erase_flash(const EraseFlash *data, EraseFlash_Response *response);

// Callbacks


#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 0
#define COMMUNICATION_CALLBACK_LIST_INIT \


#endif
