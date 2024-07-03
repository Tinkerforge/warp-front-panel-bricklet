/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * by25q.c: Driver for BY25Q SPI NOR flash
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

#include "by25q.h"

#include "configs/config_by25q.h"

#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/utility/util_definitions.h"
#include "bricklib2/os/coop_task.h"
#include "bricklib2/logging/logging.h"
#include "spi.h"

CoopTask by25q_task;
BY25Q by25q;

void by25q_init_spi(void) {
	// SELECT pin configuration
	const XMC_GPIO_CONFIG_t select_pin_config = {
		.mode             = BY25Q_SELECT_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// Configure slave select pin
	XMC_GPIO_Init(BY25Q_SELECT_PIN, &select_pin_config);
}

void by25q_task_read_manufacturer_and_device_id(void) {
	uint8_t data[6] = {BY25Q_OPCODE_READ_MANU_DEVICE_ID, 0, 0, 0, 0, 0};
	spi_task_transceive(data, data, 6, BY25Q_SLAVE, false);
	by25q.manufacturer_id = data[4];
	by25q.device_id       = data[5];
}

// bit 0 = WIP (write in progress)
// bit 1 = WEL (write enable latch)
uint8_t by25q_task_read_status(void) {
	uint8_t data[2] = {BY25Q_OPCODE_READ_STATUS1, 0};
	spi_task_transceive(data, data, 2, BY25Q_SLAVE, 0);
	return data[1];
}

bool by25q_task_is_write_in_progress(void) {
	return by25q_task_read_status() & 1;
}

bool by25q_task_is_write_latch_enabled(void) {
	return by25q_task_read_status() & 2;
}

bool by25q_task_is_anything_in_progress(void) {
	if(by25q.rwe_in_progress) {
		return true;
	}

	return by25q_task_read_status() & (1 | 2);
}

void by25q_task_write_page(const uint8_t *data, const uint32_t address) {
	const uint8_t write_enable = BY25Q_OPCODE_WRITE_ENABLE;
	uint8_t instruction_and_address[BY25Q_INSTRUCTION_SIZE + BY25Q_ADDRESS_SIZE] = {
		BY25Q_OPCODE_PAGE_PROGRAM,
		(address >> 16) & 0xFF,
		(address >>  8) & 0xFF,
		(address >>  0) & 0xFF
	};

	while(by25q_task_is_anything_in_progress()) {
		coop_task_yield();
	}

	by25q.rwe_in_progress = true;
	// Write enable (chip select and deselect)
	spi_task_transceive(&write_enable, NULL, 1, BY25Q_SLAVE, 0);
	// Write instruction and address (only chip select)
	spi_task_transceive(instruction_and_address, NULL, BY25Q_INSTRUCTION_SIZE + BY25Q_ADDRESS_SIZE, BY25Q_SLAVE, SPI_TRANSCEIVE_OPTION_NO_DESELECT);
	// Write page (already selected, only chip deselect)
	spi_task_transceive(data, NULL, BY25Q_PAGE_SIZE, BY25Q_SLAVE, SPI_TRANSCEIVE_OPTION_NO_SELECT);
	by25q.rwe_in_progress = false;
}

void by25q_task_erase_sector(const uint32_t address) {
	const uint8_t write_enable = BY25Q_OPCODE_WRITE_ENABLE;
	uint8_t instruction_and_address[BY25Q_INSTRUCTION_SIZE + BY25Q_ADDRESS_SIZE] = {
		BY25Q_OPCODE_SECTOR_ERASE,
		(address >> 16) & 0xFF,
		(address >>  8) & 0xFF,
		(address >>  0) & 0xFF
	};

	while(by25q_task_is_anything_in_progress()) {
		coop_task_yield();
	}

	by25q.rwe_in_progress = true;
	spi_task_transceive(&write_enable, NULL, 1, BY25Q_SLAVE, 0);
	spi_task_transceive(instruction_and_address, NULL, BY25Q_INSTRUCTION_SIZE + BY25Q_ADDRESS_SIZE, BY25Q_SLAVE, 0);
	by25q.rwe_in_progress = false;
}

void by25q_task_chip_erase(void) {
	const uint8_t write_enable = BY25Q_OPCODE_WRITE_ENABLE;
	const uint8_t instruction = BY25Q_OPCODE_CHIP_ERASE1;

	while(by25q_task_is_anything_in_progress()) {
		coop_task_yield();
	}

	by25q.rwe_in_progress = true;
	spi_task_transceive(&write_enable, NULL, 1, BY25Q_SLAVE, 0);
	spi_task_transceive(&instruction, NULL, 1, BY25Q_SLAVE, 0);
	by25q.rwe_in_progress = false;
}

void by25q_task_read(uint8_t *data, const uint16_t length, const uint32_t address) {
	const uint8_t instruction_and_address[BY25Q_INSTRUCTION_SIZE + BY25Q_ADDRESS_SIZE] = {
		BY25Q_OPCODE_READ_DATA,
		(address >> 16) & 0xFF,
		(address >>  8) & 0xFF,
		(address >>  0) & 0xFF
	};

	while(by25q_task_is_anything_in_progress()) {
		coop_task_yield();
	}

	by25q.rwe_in_progress = true;

	// Read instruction and address (only chip select)
	spi_task_transceive(instruction_and_address, NULL, BY25Q_INSTRUCTION_SIZE + BY25Q_ADDRESS_SIZE, BY25Q_SLAVE, SPI_TRANSCEIVE_OPTION_NO_DESELECT);
	// Read page (already selected, only chip deselect)
	spi_task_transceive(NULL, data, length, BY25Q_SLAVE, SPI_TRANSCEIVE_OPTION_NO_SELECT);

	by25q.rwe_in_progress = false;
}

void by25q_task_init(void) {

}

void by25q_task_tick(void) {
	by25q_task_init();
	by25q_task_read_manufacturer_and_device_id();
	logd("BY25Q Manufacturer ID: %d, Device ID: %d\n\r", by25q.manufacturer_id, by25q.device_id);

	while(true) {
		if(by25q.to_erase_full) {
			by25q_task_chip_erase();
			by25q.to_erase_full = false;
		}
		if(by25q.to_erase_sector_index >= 0) {
			by25q_task_erase_sector(by25q.to_erase_sector_index*BY25Q_SECTOR_SIZE);
			by25q.to_erase_sector_index = -1;
		}
		if(by25q.to_write_index >= 0) {
			by25q_task_write_page(by25q.data_write, by25q.to_write_index*BY25Q_PAGE_SIZE);
			by25q.to_write_index = -1;
		}
		coop_task_yield();
	}
}

void by25q_init(void) {
	memset(&by25q, 0, sizeof(BY25Q));
	by25q.to_write_index = -1;
	by25q.to_erase_sector_index = -1;

	by25q_init_spi();

	const XMC_GPIO_CONFIG_t input_pin_config = {
		.mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
		.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
	};

	const XMC_GPIO_CONFIG_t pin_config_output_high = {
		.mode             = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	XMC_GPIO_Init(BY25Q_NWP_PIN,   &pin_config_output_high);
	XMC_GPIO_Init(BY25Q_NHOLD_PIN, &pin_config_output_high);

	coop_task_init(&by25q_task, by25q_task_tick);
}

void by25q_tick(void) {
	coop_task_tick(&by25q_task);
}
