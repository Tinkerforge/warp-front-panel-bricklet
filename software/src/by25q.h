/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * by25q.h: Driver for BY25Q SPI NOR flash
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

#ifndef BY25Q_H
#define BY25Q_H

#include <stdint.h>
#include <stdbool.h>

#define BY25Q_INSTRUCTION_SIZE 1
#define BY25Q_ADDRESS_SIZE 3
#define BY25Q_PAGE_SIZE 256
#define BY25Q_SECTOR_SIZE 4096

typedef struct {
    uint32_t version;
    uint32_t length;
    uint32_t checksum;
} __attribute__((__packed__)) BY25QMetadata;

typedef struct {
    BY25QMetadata metadata;

    uint32_t page_index;
    uint8_t sub_page_index;

    int32_t to_write_index;
    int32_t to_erase_sector_index;
    bool to_erase_full;

    uint8_t data_write[BY25Q_PAGE_SIZE];
    uint8_t data_read[BY25Q_PAGE_SIZE];

    uint8_t manufacturer_id;
    uint8_t device_id;

    bool rwe_in_progress;
} BY25Q;

extern BY25Q by25q;

void by25q_init(void);
void by25q_tick(void);
void by25q_task_read(uint8_t *data, const uint16_t length, const uint32_t address);
bool by25q_has_data_to_write(void);

// Configuration and status instructions
#define BY25Q_OPCODE_WRITE_ENABLE                 0x06
#define BY25Q_OPCODE_WRITE_ENABLE_FOR_VOLATILE    0x50
#define BY25Q_OPCODE_WRITE_DISABLE                0x04
#define BY25Q_OPCODE_READ_STATUS1                 0x05
#define BY25Q_OPCODE_READ_STATUS2                 0x35
#define BY25Q_OPCODE_READ_STATUS3                 0x15
#define BY25Q_OPCODE_WRITE_STATUS1                0x01
#define BY25Q_OPCODE_WRITE_STATUS2                0x31
#define BY25Q_OPCODE_WRITE_STATUS3                0x11
#define BY25Q_OPCODE_ENABLE_RESET                 0x66
#define BY25Q_OPCODE_RESET_DEVICE                 0x99

// Read instructions
#define BY25Q_OPCODE_READ_DATA                    0x03
#define BY25Q_OPCODE_FAST_READ                    0x0B
#define BY25Q_OPCODE_DUAL_OUTPUT_FAST_READ        0x3B
#define BY25Q_OPCODE_QUAD_OUTPUT_FAST_READ        0x6B
#define BY25Q_OPCODE_DUAL_IO_FAST_READ            0xBB
#define BY25Q_OPCODE_QUAD_IO_FAST_READ            0xEB
#define BY25Q_OPCODE_QUAD_IO_WORD_FAST_READ       0xE7
#define BY25Q_OPCODE_SET_BURST_WITH_WRAP          0x77

// ID and security instructions
#define BY25Q_OPCODE_READ_MANU_DEVICE_ID          0x90
#define BY25Q_OPCODE_DUAL_IO_READ_MANU_DEVICE_ID  0x92
#define BY25Q_OPCODE_QUAD_IO_READ_MANU_DEVICE_ID  0x94
#define BY25Q_OPCODE_READ_JDEC_ID                 0x9F
#define BY25Q_OPCODE_READ_UNIQUE_ID               0x4B
#define BY25Q_OPCODE_DEEP_POWER_DOWN              0xB9
#define BY25Q_OPCODE_RELESE_FROM_DEEP_POWER_DOWN  0xAB
#define BY25Q_OPCODE_READ_SECURITY_REGISTERS      0x48
#define BY25Q_OPCODE_ERASE_SECURITY_REGISTERS     0x44
#define BY25Q_OPCODE_PROGRAM_SECURITY_REGISTERS   0x42
#define BY25Q_OPCODE_READ_SERIAL_FLASH_DISC_PARAM 0x5A

// Program and erase instructions
#define BY25Q_OPCODE_PAGE_PROGRAM                 0x02
#define BY25Q_OPCODE_QUAD_PAGE_PROGRAM            0x32
#define BY25Q_OPCODE_SECTOR_ERASE                 0x20
#define BY25Q_OPCODE_32KB_BLOCK_ERASE             0x52
#define BY25Q_OPCODE_564KB_BLOCK_ERASE            0xD8
#define BY25Q_OPCODE_CHIP_ERASE1                  0x60
#define BY25Q_OPCODE_CHIP_ERASE2                  0xC7
#define BY25Q_OPCODE_ERASE_SUSPEND                0x75
#define BY25Q_OPCODE_ERASE_RESUME                 0x7A

#endif
