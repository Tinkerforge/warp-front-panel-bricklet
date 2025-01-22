/* warp-front-panel-bricklet
 * Copyright (C) 2025 Olaf Lüke <olaf@tinkerforge.com>
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

#define WARP_FRONT_PANEL_LED_PATTERN_OFF 0
#define WARP_FRONT_PANEL_LED_PATTERN_ON 1
#define WARP_FRONT_PANEL_LED_PATTERN_BLINKING 2
#define WARP_FRONT_PANEL_LED_PATTERN_BREATHING 3

#define WARP_FRONT_PANEL_LED_COLOR_GREEN 0
#define WARP_FRONT_PANEL_LED_COLOR_RED 1
#define WARP_FRONT_PANEL_LED_COLOR_YELLOW 2

#define WARP_FRONT_PANEL_DISPLAY_OFF 0
#define WARP_FRONT_PANEL_DISPLAY_AUTOMATIC 1

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
#define FID_SET_STATUS_BAR 6
#define FID_GET_STATUS_BAR 7
#define FID_SET_DISPLAY_PAGE_INDEX 8
#define FID_GET_DISPLAY_PAGE_INDEX 9
#define FID_SET_DISPLAY_FRONT_PAGE_ICON 10
#define FID_GET_DISPLAY_FRONT_PAGE_ICON 11
#define FID_SET_DISPLAY_WIFI_SETUP_1 12
#define FID_GET_DISPLAY_WIFI_SETUP_1 13
#define FID_SET_DISPLAY_WIFI_SETUP_2 14
#define FID_GET_DISPLAY_WIFI_SETUP_2 15
#define FID_SET_LED_STATE 16
#define FID_GET_LED_STATE 17
#define FID_SET_DISPLAY 18
#define FID_GET_DISPLAY 19
#define FID_GET_FLASH_METADATA 20

#define FID_CALLBACK_FLASH_DATA_DONE 21

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

typedef struct {
    TFPMessageHeader header;
    uint32_t ethernet_status;
    uint32_t wifi_status;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} __attribute__((__packed__)) SetStatusBar;

typedef struct {
    TFPMessageHeader header;
} __attribute__((__packed__)) GetStatusBar;

typedef struct {
    TFPMessageHeader header;
    uint32_t ethernet_status;
    uint32_t wifi_status;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} __attribute__((__packed__)) GetStatusBar_Response;

typedef struct {
    TFPMessageHeader header;
    uint32_t page_index;
} __attribute__((__packed__)) SetDisplayPageIndex;

typedef struct {
    TFPMessageHeader header;
} __attribute__((__packed__)) GetDisplayPageIndex;

typedef struct {
    TFPMessageHeader header;
    uint32_t page_index;
} __attribute__((__packed__)) GetDisplayPageIndex_Response;

typedef struct {
    TFPMessageHeader header;
    uint32_t icon_index;
    bool active;
    uint32_t sprite_index;
    char text_1[6];
    uint8_t font_index_1;
    char text_2[6];
    uint8_t font_index_2;
} __attribute__((__packed__)) SetDisplayFrontPageIcon;

typedef struct {
    TFPMessageHeader header;
    uint32_t icon_index;
} __attribute__((__packed__)) GetDisplayFrontPageIcon;

typedef struct {
    TFPMessageHeader header;
    bool active;
    uint32_t sprite_index;
    char text_1[10];
    uint8_t font_index_1;
    char text_2[10];
    uint8_t font_index_2;
} __attribute__((__packed__)) GetDisplayFrontPageIcon_Response;

typedef struct {
    TFPMessageHeader header;
    char ip_address[15];
    char ssid[49];
} __attribute__((__packed__)) SetDisplayWifiSetup1;

typedef struct {
    TFPMessageHeader header;
} __attribute__((__packed__)) GetDisplayWifiSetup1;

typedef struct {
    TFPMessageHeader header;
    char ip_address[15];
    char ssid[49];
} __attribute__((__packed__)) GetDisplayWifiSetup1_Response;

typedef struct {
    TFPMessageHeader header;
    char password[64];
} __attribute__((__packed__)) SetDisplayWifiSetup2;

typedef struct {
    TFPMessageHeader header;
} __attribute__((__packed__)) GetDisplayWifiSetup2;

typedef struct {
    TFPMessageHeader header;
    char password[64];
} __attribute__((__packed__)) GetDisplayWifiSetup2_Response;

typedef struct {
    TFPMessageHeader header;
    uint8_t pattern;
    uint8_t color;
} __attribute__((__packed__)) SetLEDState;

typedef struct {
    TFPMessageHeader header;
} __attribute__((__packed__)) GetLEDState;

typedef struct {
    TFPMessageHeader header;
    uint8_t pattern;
    uint8_t color;
} __attribute__((__packed__)) GetLEDState_Response;

typedef struct {
    TFPMessageHeader header;
    uint8_t display;
} __attribute__((__packed__)) SetDisplay;

typedef struct {
    TFPMessageHeader header;
} __attribute__((__packed__)) GetDisplay;

typedef struct {
    TFPMessageHeader header;
    uint8_t display;
    uint32_t countdown;
} __attribute__((__packed__)) GetDisplay_Response;

typedef struct {
    TFPMessageHeader header;
} __attribute__((__packed__)) GetFlashMetadata;

typedef struct {
    TFPMessageHeader header;
    uint32_t version_flash;
    uint32_t version_expected;
    uint32_t length_flash;
    uint32_t length_expected;
    uint32_t checksum_flash;
    uint32_t checksum_expected;
} __attribute__((__packed__)) GetFlashMetadata_Response;

typedef struct {
    TFPMessageHeader header;
} __attribute__((__packed__)) FlashDataDone_Callback;


// Function prototypes
BootloaderHandleMessageResponse set_flash_index(const SetFlashIndex *data);
BootloaderHandleMessageResponse get_flash_index(const GetFlashIndex *data, GetFlashIndex_Response *response);
BootloaderHandleMessageResponse set_flash_data(const SetFlashData *data, SetFlashData_Response *response);
BootloaderHandleMessageResponse erase_flash_sector(const EraseFlashSector *data, EraseFlashSector_Response *response);
BootloaderHandleMessageResponse erase_flash(const EraseFlash *data, EraseFlash_Response *response);
BootloaderHandleMessageResponse set_status_bar(const SetStatusBar *data);
BootloaderHandleMessageResponse get_status_bar(const GetStatusBar *data, GetStatusBar_Response *response);
BootloaderHandleMessageResponse set_display_page_index(const SetDisplayPageIndex *data);
BootloaderHandleMessageResponse get_display_page_index(const GetDisplayPageIndex *data, GetDisplayPageIndex_Response *response);
BootloaderHandleMessageResponse set_display_front_page_icon(const SetDisplayFrontPageIcon *data);
BootloaderHandleMessageResponse get_display_front_page_icon(const GetDisplayFrontPageIcon *data, GetDisplayFrontPageIcon_Response *response);
BootloaderHandleMessageResponse set_display_wifi_setup_1(const SetDisplayWifiSetup1 *data);
BootloaderHandleMessageResponse get_display_wifi_setup_1(const GetDisplayWifiSetup1 *data, GetDisplayWifiSetup1_Response *response);
BootloaderHandleMessageResponse set_display_wifi_setup_2(const SetDisplayWifiSetup2 *data);
BootloaderHandleMessageResponse get_display_wifi_setup_2(const GetDisplayWifiSetup2 *data, GetDisplayWifiSetup2_Response *response);
BootloaderHandleMessageResponse set_led_state(const SetLEDState *data);
BootloaderHandleMessageResponse get_led_state(const GetLEDState *data, GetLEDState_Response *response);
BootloaderHandleMessageResponse set_display(const SetDisplay *data);
BootloaderHandleMessageResponse get_display(const GetDisplay *data, GetDisplay_Response *response);
BootloaderHandleMessageResponse get_flash_metadata(const GetFlashMetadata *data, GetFlashMetadata_Response *response);

// Callbacks
bool handle_flash_data_done_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 1
#define COMMUNICATION_CALLBACK_LIST_INIT \
    handle_flash_data_done_callback, \


#endif
