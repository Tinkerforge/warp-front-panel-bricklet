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
#include "bricklib2/utility/util_definitions.h"
#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/os/coop_task.h"
#include "bricklib2/hal/system_timer/system_timer.h"

#include "st7789.h"
#include "by25q.h"
#include "display.h"
#include "status_bar.h"
#include "page_front.h"
#include "page_wifi_setup.h"
#include "led.h"
#include "metadata.h"

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
    const uint8_t length = ((TFPMessageHeader*)message)->length;
    switch(tfp_get_fid_from_message(message)) {
        case FID_SET_FLASH_INDEX:             return length != sizeof(SetFlashIndex)           ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : set_flash_index(message);
        case FID_GET_FLASH_INDEX:             return length != sizeof(GetFlashIndex)           ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : get_flash_index(message, response);
        case FID_SET_FLASH_DATA:              return length != sizeof(SetFlashData)            ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : set_flash_data(message, response);
        case FID_ERASE_FLASH_SECTOR:          return length != sizeof(EraseFlashSector)        ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : erase_flash_sector(message, response);
        case FID_ERASE_FLASH:                 return length != sizeof(EraseFlash)              ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : erase_flash(message, response);
        case FID_SET_STATUS_BAR:              return length != sizeof(SetStatusBar)            ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : set_status_bar(message);
        case FID_GET_STATUS_BAR:              return length != sizeof(GetStatusBar)            ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : get_status_bar(message, response);
        case FID_SET_DISPLAY_PAGE_INDEX:      return length != sizeof(SetDisplayPageIndex)     ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : set_display_page_index(message);
        case FID_GET_DISPLAY_PAGE_INDEX:      return length != sizeof(GetDisplayPageIndex)     ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : get_display_page_index(message, response);
        case FID_SET_DISPLAY_FRONT_PAGE_ICON: return length != sizeof(SetDisplayFrontPageIcon) ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : set_display_front_page_icon(message);
        case FID_GET_DISPLAY_FRONT_PAGE_ICON: return length != sizeof(GetDisplayFrontPageIcon) ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : get_display_front_page_icon(message, response);
        case FID_SET_DISPLAY_WIFI_SETUP_1:    return length != sizeof(SetDisplayWifiSetup1)    ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : set_display_wifi_setup_1(message);
        case FID_GET_DISPLAY_WIFI_SETUP_1:    return length != sizeof(GetDisplayWifiSetup1)    ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : get_display_wifi_setup_1(message, response);
        case FID_SET_DISPLAY_WIFI_SETUP_2:    return length != sizeof(SetDisplayWifiSetup2)    ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : set_display_wifi_setup_2(message);
        case FID_GET_DISPLAY_WIFI_SETUP_2:    return length != sizeof(GetDisplayWifiSetup2)    ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : get_display_wifi_setup_2(message, response);
        case FID_SET_LED_STATE:               return length != sizeof(SetLEDState)             ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : set_led_state(message);
        case FID_GET_LED_STATE:               return length != sizeof(GetLEDState)             ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : get_led_state(message, response);
        case FID_SET_DISPLAY:                 return length != sizeof(SetDisplay)              ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : set_display(message);
        case FID_GET_DISPLAY:                 return length != sizeof(GetDisplay)              ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : get_display(message, response);
        case FID_GET_FLASH_METADATA:          return length != sizeof(GetFlashMetadata)        ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : get_flash_metadata(message, response);
        case FID_REDRAW_EVERYTHING:           return length != sizeof(RedrawEverything)        ? HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER : redraw_everything(message);
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

BootloaderHandleMessageResponse erase_flash_sector(const EraseFlashSector *data, EraseFlashSector_Response *response) {
    response->header.length = sizeof(EraseFlashSector_Response);
    if(by25q.to_erase_sector_index >= 0) {
        response->status = WARP_FRONT_PANEL_FLASH_STATUS_BUSY;
        return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
    }

    by25q.to_erase_sector_index = data->sector_index;

    return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse erase_flash(const EraseFlash *data, EraseFlash_Response *response) {
    response->header.length = sizeof(EraseFlash_Response);
    if(by25q.to_erase_full) {
        response->status = WARP_FRONT_PANEL_FLASH_STATUS_BUSY;
        return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
    }

    by25q.to_erase_full = true;

    return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_status_bar(const SetStatusBar *data) {
    if((data->hours > 23) || (data->minutes > 59) || (data->seconds > 59)) {
        return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
    }

    if(status_bar.ethernet_status != data->ethernet_status) {
        status_bar.ethernet_status = data->ethernet_status;
        status_bar.redraw_ethernet = true;
    }

    if(status_bar.wifi_status != data->wifi_status) {
        status_bar.wifi_status = data->wifi_status;
        status_bar.redraw_wifi = true;
    }

    if((status_bar.hours != data->hours) || (status_bar.minutes != data->minutes) || (status_bar.hours != data->minutes)) {
        status_bar.hours        = data->hours;
        status_bar.minutes      = data->minutes;
        status_bar.seconds      = data->seconds;
        status_bar.redraw_clock = true;
    }

    return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_status_bar(const GetStatusBar *data, GetStatusBar_Response *response) {
    response->header.length   = sizeof(GetStatusBar_Response);
    response->ethernet_status = status_bar.ethernet_status;
    response->wifi_status     = status_bar.wifi_status;
    response->hours           = status_bar.hours;
    response->minutes         = status_bar.minutes;
    response->seconds         = status_bar.seconds;

    return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_display_page_index(const SetDisplayPageIndex *data) {

    return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_display_page_index(const GetDisplayPageIndex *data, GetDisplayPageIndex_Response *response) {
    response->header.length = sizeof(GetDisplayPageIndex_Response);

    return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_display_front_page_icon(const SetDisplayFrontPageIcon *data) {
    if(data->icon_index > 12) {
        return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
    }

    if(page_front_icon_list[data->icon_index].active != data->active) {
        page_front_icon_list[data->icon_index].active = data->active;
        page_front_icon_list[data->icon_index].redraw_everything = true;
    }
    if(page_front_icon_list[data->icon_index].sprite_index != data->sprite_index) {
        page_front_icon_list[data->icon_index].sprite_index = data->sprite_index;
        page_front_icon_list[data->icon_index].redraw_sprite = true;
    }
    if((page_front_icon_list[data->icon_index].font_index_1 != data->font_index_1) || (memcmp(page_front_icon_list[data->icon_index].text_1, data->text_1, 10) != 0)) {
        page_front_icon_list[data->icon_index].font_index_1 = data->font_index_1;
        memcpy(page_front_icon_list[data->icon_index].text_1, data->text_1, PAGE_FRONT_TEXT_MAX_CHAR);
        page_front_icon_list[data->icon_index].text_1[PAGE_FRONT_TEXT_MAX_CHAR] = '\0';
        page_front_icon_list[data->icon_index].redraw_text_1 = true;
    }
    if((page_front_icon_list[data->icon_index].font_index_2 != data->font_index_2) || (memcmp(page_front_icon_list[data->icon_index].text_2, data->text_2, 10) != 0)) {
        page_front_icon_list[data->icon_index].font_index_2 = data->font_index_2;
        memcpy(page_front_icon_list[data->icon_index].text_2, data->text_2, PAGE_FRONT_TEXT_MAX_CHAR);
        page_front_icon_list[data->icon_index].text_2[PAGE_FRONT_TEXT_MAX_CHAR] = '\0';
        page_front_icon_list[data->icon_index].redraw_text_2 = true;
    }

    return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_display_front_page_icon(const GetDisplayFrontPageIcon *data, GetDisplayFrontPageIcon_Response *response) {
    response->header.length = sizeof(GetDisplayFrontPageIcon_Response);

    if(data->icon_index > 12) {
        return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
    }

    response->active       = page_front_icon_list[data->icon_index].active;
    response->sprite_index = page_front_icon_list[data->icon_index].sprite_index;
    response->font_index_1 = page_front_icon_list[data->icon_index].font_index_1;
    response->font_index_2 = page_front_icon_list[data->icon_index].font_index_2;
    memcpy(response->text_1, page_front_icon_list[data->icon_index].text_1, PAGE_FRONT_TEXT_MAX_CHAR);
    memcpy(response->text_2, page_front_icon_list[data->icon_index].text_2, PAGE_FRONT_TEXT_MAX_CHAR);

    return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_display_wifi_setup_1(const SetDisplayWifiSetup1 *data) {
    if(strncmp(page_wifi_setup.ip_address, data->ip_address, 15) != 0) {
        memcpy(page_wifi_setup.ip_address, data->ip_address, 15);
        page_wifi_setup.ip_address[15] = '\0';
        page_wifi_setup.redraw_ip_address = true;
    }

    if(strncmp(page_wifi_setup.ssid, data->ssid, 49) != 0) {
        memcpy(page_wifi_setup.ssid, data->ssid, 49);
        page_wifi_setup.ssid[49] = '\0';
        page_wifi_setup.redraw_ssid = true;
    }

    return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_display_wifi_setup_1(const GetDisplayWifiSetup1 *data, GetDisplayWifiSetup1_Response *response) {
    response->header.length = sizeof(GetDisplayWifiSetup1_Response);
    memcpy(response->ip_address, page_wifi_setup.ip_address, 15);
    memcpy(response->ssid, page_wifi_setup.ssid, 49);

    return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_display_wifi_setup_2(const SetDisplayWifiSetup2 *data) {
    if(strncmp(page_wifi_setup.password, data->password, 64) != 0) {
        memcpy(page_wifi_setup.password, data->password, 64);
        page_wifi_setup.password[64] = '\0';

        page_wifi_setup.redraw_qrcode = true;
    }

    return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_display_wifi_setup_2(const GetDisplayWifiSetup2 *data, GetDisplayWifiSetup2_Response *response) {
    response->header.length = sizeof(GetDisplayWifiSetup2_Response);
    memcpy(response->password, page_wifi_setup.password, 49);

    return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_led_state(const SetLEDState *data) {
    if(data->color > WARP_FRONT_PANEL_LED_COLOR_YELLOW) {
        return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
    }
    if(data->pattern > WARP_FRONT_PANEL_LED_PATTERN_BREATHING) {
        return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
    }

    led.color   = data->color;
    led.pattern = data->pattern;

    return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_led_state(const GetLEDState *data, GetLEDState_Response *response) {
    response->header.length = sizeof(GetLEDState_Response);
    response->color         = led.color;
    response->pattern       = led.pattern;

    return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_display(const SetDisplay *data) {
    if(data->display > WARP_FRONT_PANEL_DISPLAY_AUTOMATIC) {
        return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
    }

    display.active = data->display;
    if(display.active == WARP_FRONT_PANEL_DISPLAY_OFF) {
        display.is_active = false;
        display.countdown = 0;
    } else {
        if(!display.is_active) {
            display.is_active = true;
            display.countdown = system_timer_get_ms();
        }
    }

    return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_display(const GetDisplay *data, GetDisplay_Response *response) {
    response->header.length = sizeof(GetDisplay_Response);
    response->display       = display.active;
    if(display.countdown == 0) {
        response->countdown = 0;
    } else {
        if(system_timer_is_time_elapsed_ms(display.countdown, DISPLAY_COUNTDOWN_MS)) {
            response->countdown = 0;
        } else {
            const uint32_t diff = system_timer_get_ms() - display.countdown;
            response->countdown = DISPLAY_COUNTDOWN_MS - diff;
        }
    }

    return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_flash_metadata(const GetFlashMetadata *data, GetFlashMetadata_Response *response) {
    response->header.length     = sizeof(GetFlashMetadata_Response);
    response->length_flash      = by25q.metadata.length;
    response->length_expected   = METADATA_LENGTH;
    response->version_flash     = by25q.metadata.version;
    response->version_expected  = METADATA_VERSION;
    response->checksum_flash    = by25q.metadata.checksum;
    response->checksum_expected = METADATA_CHECKSUM;

    return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse redraw_everything(const RedrawEverything *data) {
    display.redraw_everything = true;

    return HANDLE_MESSAGE_RESPONSE_EMPTY;
}


bool handle_flash_data_done_callback(void) {
    static bool is_buffered = false;
    static FlashDataDone_Callback cb;

    if(!is_buffered) {
        if(by25q.flash_data_done) {
            by25q.flash_data_done = false;
            tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(FlashDataDone_Callback), FID_CALLBACK_FLASH_DATA_DONE);
        } else {
            return false;
        }
    }

    if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
        bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(FlashDataDone_Callback));
        is_buffered = false;
        return true;
    } else {
        is_buffered = true;
    }

    return false;
}


void communication_tick(void) {
    communication_callback_tick();
}

void communication_init(void) {
    communication_callback_init();
}
