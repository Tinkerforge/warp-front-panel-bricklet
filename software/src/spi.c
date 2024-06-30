/* warp-front-panel-bricklet
 * Copyright (C) 2024 Olaf Lüke <olaf@tinkerforge.com>
 *
 * spi.c: SPI functions shared between ST7789 and Flash/PSRAM
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

#include "spi.h"

#include <string.h>

#include "configs/config_st7789.h"

#include "bricklib2/os/coop_task.h"
#include "bricklib2/utility/util_definitions.h"

#include "xmc_gpio.h"

SPI spi;

#define st7789_rx_irq_handler IRQ_Hdlr_11
#define st7789_tx_irq_handler IRQ_Hdlr_12

#define st7789_tx_irq_handler_fast IRQ_Hdlr_14

// Use local pointer to save the time for accessing the struct
volatile const uint32_t *ST7789_USIC_OUTR_PTR = &ST7789_USIC->OUTR;
volatile uint32_t       *ST7789_USIC_IN_PTR   =  ST7789_USIC->IN;

// Set pointers to read/write buffer
// With this the compiler can properly optimize the access!
uint8_t *spi_data_read      = spi.data;
uint8_t *spi_data_write     = spi.data;
uint8_t *spi_data_write_end = spi.data;

void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) st7789_rx_irq_handler(void) {
	// Max fill level is 16.
	const uint8_t num = XMC_USIC_CH_RXFIFO_GetLevel(ST7789_USIC);
	switch(num) {
		case 16: *spi_data_read = *ST7789_USIC_OUTR_PTR; spi_data_read++;
		case 15: *spi_data_read = *ST7789_USIC_OUTR_PTR; spi_data_read++;
		case 14: *spi_data_read = *ST7789_USIC_OUTR_PTR; spi_data_read++;
		case 13: *spi_data_read = *ST7789_USIC_OUTR_PTR; spi_data_read++;
		case 12: *spi_data_read = *ST7789_USIC_OUTR_PTR; spi_data_read++;
		case 11: *spi_data_read = *ST7789_USIC_OUTR_PTR; spi_data_read++;
		case 10: *spi_data_read = *ST7789_USIC_OUTR_PTR; spi_data_read++;
		case 9:  *spi_data_read = *ST7789_USIC_OUTR_PTR; spi_data_read++;
		case 8:  *spi_data_read = *ST7789_USIC_OUTR_PTR; spi_data_read++;
		case 7:  *spi_data_read = *ST7789_USIC_OUTR_PTR; spi_data_read++;
		case 6:  *spi_data_read = *ST7789_USIC_OUTR_PTR; spi_data_read++;
		case 5:  *spi_data_read = *ST7789_USIC_OUTR_PTR; spi_data_read++;
		case 4:  *spi_data_read = *ST7789_USIC_OUTR_PTR; spi_data_read++;
		case 3:  *spi_data_read = *ST7789_USIC_OUTR_PTR; spi_data_read++;
		case 2:  *spi_data_read = *ST7789_USIC_OUTR_PTR; spi_data_read++;
		case 1:  *spi_data_read = *ST7789_USIC_OUTR_PTR; spi_data_read++;
	}
}

void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) st7789_tx_irq_handler(void) {
	// Max fill level is 16.
	const uint8_t to_send    = spi_data_write_end - spi_data_write;
	const uint8_t fifo_level = 16 - XMC_USIC_CH_TXFIFO_GetLevel(ST7789_USIC);
	const uint8_t num        = MIN(to_send, fifo_level);

	switch(num) {
		case 16: ST7789_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 15: ST7789_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 14: ST7789_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 13: ST7789_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 12: ST7789_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 11: ST7789_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 10: ST7789_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 9:  ST7789_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 8:  ST7789_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 7:  ST7789_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 6:  ST7789_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 5:  ST7789_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 4:  ST7789_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 3:  ST7789_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 2:  ST7789_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 1:  ST7789_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
	}

	if(spi_data_write >= spi_data_write_end) {
		XMC_USIC_CH_TXFIFO_DisableEvent(ST7789_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
	}
}

void spi_task_transceive(const uint8_t *data, const uint32_t length, XMC_SPI_CH_SLAVE_SELECT_t slave, const uint8_t option) {
	if(!(option & SPI_TRANSCEIVE_OPTION_NO_SELECT)) {
		while(spi.tranceive_ongoing) {
			coop_task_yield();
		}
	}

	spi.tranceive_ongoing = true;

	if(option & SPI_TRANSCEIVE_OPTION_SWAP_U16) {
		for(uint16_t i = 0; i < length; i+=2) {
			spi.data[i] = data[i+1];
			spi.data[i+1] = data[i];
		}
	} else {
		memcpy(spi.data, data, length);
	}
	spi.data_length = length;
	spi_data_read = spi.data;
	spi_data_write = spi.data;
	spi_data_write_end = spi.data + length;

	if(!(option & SPI_TRANSCEIVE_OPTION_NO_SELECT)) {
		XMC_SPI_CH_ClearStatusFlag(ST7789_USIC, XMC_SPI_CH_STATUS_FLAG_MSLS);
		XMC_SPI_CH_EnableSlaveSelect(ST7789_USIC, slave);
	}

	// If possible write and read data in this coop task.
	// Only if other tasks take too much time we go through the interrupts.
	// This may seem a bit redundant, but in profiling tests we found that this approach
	// is by far the most efficient.
	// Even if we transfer at a full 1.5MHz rate, the interrupt will only be called about once
	// per ms. The rest of the data is transferred through this loop, which will be called
	// by the scheduler anyway.
	while(spi_data_read < spi.data + spi.data_length) {
		// Disable TX IRQ to avoid race condition and write as much data by
		// hand as possible.
		XMC_USIC_CH_TXFIFO_DisableEvent(ST7789_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
		while((!XMC_USIC_CH_TXFIFO_IsFull(ST7789_USIC)) && (spi_data_write < spi_data_write_end)) {
			ST7789_USIC_IN_PTR[0] = *spi_data_write;
			spi_data_write++;
		}
		NVIC_ClearPendingIRQ(ST7789_IRQ_TX);
		if(spi_data_write < spi_data_write_end) {
			// If we have not yet written everything here, we turn the interrupt on.
			// If the bootloader or other tasks now takes too long the rest of the data can
			// be send through the interrupt.
			// Otherwise the we will run the code above again and send data in this loop.
			XMC_USIC_CH_TXFIFO_EnableEvent(ST7789_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
		}

		// Disable RX IRQ to avoid race condition and read as much data by
		// hand as possible.
		// If there are less then 8 bytes left in the FIFO and the SPI transfer
		// is finished, we read the rest of the bytes here (the RX IRQ is only called if 8 or
		// more bytes are in the FIFO).
		NVIC_DisableIRQ(ST7789_IRQ_RX);
		while(!XMC_USIC_CH_RXFIFO_IsEmpty(ST7789_USIC)) {
			*spi_data_read = *ST7789_USIC_OUTR_PTR;
			spi_data_read++;
		}
		// We have to clear the IRQ here. There might have been an IRQ between the
		// disable and the first read.
		NVIC_ClearPendingIRQ(ST7789_IRQ_RX);
		NVIC_EnableIRQ(ST7789_IRQ_RX);

		coop_task_yield();
	}

	if(!(option & SPI_TRANSCEIVE_OPTION_NO_DESELECT)) {
		XMC_SPI_CH_DisableSlaveSelect(ST7789_USIC);
		spi.tranceive_ongoing = false;
	}
}

void spi_init(void) {
	memset(&spi, 0, sizeof(SPI));

	// ST7789, RAM and Flash use same SPI configuration, so we can use the defines from either of them.

	// USIC channel configuration
	const XMC_SPI_CH_CONFIG_t channel_config = {
		.baudrate       = ST7789_SPI_BAUDRATE,
		.bus_mode       = XMC_SPI_CH_BUS_MODE_MASTER,
		.selo_inversion = XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS,
		.parity_mode    = XMC_USIC_CH_PARITY_MODE_NONE
	};

	// MOSI pin configuration
	const XMC_GPIO_CONFIG_t mosi_pin_config = {
		.mode             = ST7789_MOSI_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// MISO pin configuration
	const XMC_GPIO_CONFIG_t miso_pin_config = {
		.mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
		.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
	};

	// SCLK pin configuration
	const XMC_GPIO_CONFIG_t sclk_pin_config = {
		.mode             = ST7789_SCLK_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// Configure MISO pin
	XMC_GPIO_Init(ST7789_MISO_PIN, &miso_pin_config);

	// Initialize USIC channel in SPI master mode
	XMC_SPI_CH_Init(ST7789_USIC, &channel_config);
	ST7789_USIC->SCTR &= ~USIC_CH_SCTR_PDL_Msk; // Set passive data level to 0
//	ST7789_USIC->PCR_SSCMode &= ~USIC_CH_PCR_SSCMode_TIWEN_Msk; // Disable time between bytes

	XMC_SPI_CH_SetSlaveSelectPolarity(ST7789_USIC, XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS);
	XMC_SPI_CH_SetBitOrderMsbFirst(ST7789_USIC);

	XMC_SPI_CH_SetWordLength(ST7789_USIC, (uint8_t)8U);
	XMC_SPI_CH_SetFrameLength(ST7789_USIC, (uint8_t)64U);

	XMC_SPI_CH_SetTransmitMode(ST7789_USIC, XMC_SPI_CH_MODE_STANDARD);

	// Configure the clock polarity and clock delay
	XMC_SPI_CH_ConfigureShiftClockOutput(ST7789_USIC,
									     XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_1_DELAY_DISABLED,
									     XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK);
/*	XMC_SPI_CH_ConfigureShiftClockOutput(ST7789_USIC,
										 XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_0_DELAY_ENABLED,
										 XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK);*/
	// Configure Leading/Trailing delay
	XMC_SPI_CH_SetSlaveSelectDelay(ST7789_USIC, 2);

	// Set input source path
	XMC_SPI_CH_SetInputSource(ST7789_USIC, ST7789_MISO_INPUT, ST7789_MISO_SOURCE);

	// SPI Mode: CPOL=1 and CPHA=1
	ST7789_USIC_CHANNEL->DX1CR |= USIC_CH_DX1CR_DPOL_Msk;

	// Configure transmit FIFO
	XMC_USIC_CH_TXFIFO_Configure(ST7789_USIC, 48, XMC_USIC_CH_FIFO_SIZE_16WORDS, 8);

	// Configure receive FIFO
	XMC_USIC_CH_RXFIFO_Configure(ST7789_USIC, 32, XMC_USIC_CH_FIFO_SIZE_16WORDS, 8);

	// Set service request for tx FIFO transmit interrupt
	XMC_USIC_CH_TXFIFO_SetInterruptNodePointer(ST7789_USIC, XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_STANDARD, ST7789_SERVICE_REQUEST_TX);  // IRQ ST7789_IRQ_TX

	// Set service request for rx FIFO receive interrupt
	XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(ST7789_USIC, XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_STANDARD, ST7789_SERVICE_REQUEST_RX);  // IRQ ST7789_IRQ_RX
	XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(ST7789_USIC, XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_ALTERNATE, ST7789_SERVICE_REQUEST_RX); // IRQ ST7789_IRQ_RX

	// Set priority and enable NVIC node for transmit interrupt
	NVIC_SetPriority((IRQn_Type)ST7789_IRQ_TX, ST7789_IRQ_TX_PRIORITY);
	NVIC_EnableIRQ((IRQn_Type)ST7789_IRQ_TX);

	// Set priority and enable NVIC node for receive interrupt
	NVIC_SetPriority((IRQn_Type)ST7789_IRQ_RX, ST7789_IRQ_RX_PRIORITY);
	NVIC_EnableIRQ((IRQn_Type)ST7789_IRQ_RX);

	// Start SPI
	XMC_SPI_CH_Start(ST7789_USIC);

	// Configure SCLK pin
	XMC_GPIO_Init(ST7789_SCLK_PIN, &sclk_pin_config);

	// Configure MOSI pin
	XMC_GPIO_Init(ST7789_MOSI_PIN, &mosi_pin_config);

	XMC_USIC_CH_RXFIFO_Flush(ST7789_USIC);
	XMC_USIC_CH_RXFIFO_EnableEvent(ST7789_USIC, XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD | XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE);
}
