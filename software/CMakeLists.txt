CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_config_custom.txt)

SET(PROJECT_NAME warp-front-panel-bricklet)
SET(CHIP_FAMILY XMC1)
SET(MCU cortex-m0\ -mthumb)
PROJECT(${PROJECT_NAME})

# Optimization level, can be [0, 1, 2, 3, s].
# 0 = turn off optimization. s = optimize for size.
#SET(OPTIMIZATION_LEVEL 0)
SET(DEBUG "-g -ggdb")

SET(OPTIMIZATION_LEVEL 2)
#SET(DEBUG "")

SET(CMAKE_BUILD_TYPE None)
ENABLE_LANGUAGE(C ASM)

INCLUDE_DIRECTORIES(
	"${PROJECT_SOURCE_DIR}/src/"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/xmclib/XMCLib/inc/"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/xmclib/CMSIS/Include/"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/xmclib/CMSIS/Infineon/XMC1400_series/Include/"
	"${PROJECT_SOURCE_DIR}/src/qrcode/"
)

# find source files
SET(SOURCES
	"${PROJECT_SOURCE_DIR}/src/main.c"
	"${PROJECT_SOURCE_DIR}/src/communication.c"
	"${PROJECT_SOURCE_DIR}/src/spi.c"
	"${PROJECT_SOURCE_DIR}/src/st7789.c"
	"${PROJECT_SOURCE_DIR}/src/by25q.c"
	"${PROJECT_SOURCE_DIR}/src/button.c"
	"${PROJECT_SOURCE_DIR}/src/led.c"
	"${PROJECT_SOURCE_DIR}/src/psram.c"
	"${PROJECT_SOURCE_DIR}/src/font.c"
	"${PROJECT_SOURCE_DIR}/src/sprite.c"
	"${PROJECT_SOURCE_DIR}/src/page_front.c"
	"${PROJECT_SOURCE_DIR}/src/page_wifi_setup.c"
	"${PROJECT_SOURCE_DIR}/src/status_bar.c"
	"${PROJECT_SOURCE_DIR}/src/display.c"
	"${PROJECT_SOURCE_DIR}/src/qrcode/qrcode.c"

	"${PROJECT_SOURCE_DIR}/src/bricklib2/hal/uartbb/uartbb.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/hal/system_timer/system_timer.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/hal/startup/startup_xmc1.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/hal/startup/startup_xmc1400.S"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/os/coop_task.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/os/coop_task_pendsv_handler.S"

	"${PROJECT_SOURCE_DIR}/src/bricklib2/bootloader/bootloader.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/bootloader/bootloader_xmc_irq.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/protocols/tfp/tfp.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/logging/logging.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/utility/ringbuffer.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/utility/pearson_hash.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/utility/communication_callback.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/xmclib/XMCLib/src/xmc_gpio.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/xmclib/XMCLib/src/xmc1_gpio.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/xmclib/XMCLib/src/xmc_spi.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/xmclib/XMCLib/src/xmc_usic.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/xmclib/XMCLib/src/xmc1_scu.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/xmclib/XMCLib/src/xmc1_flash.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/xmclib/XMCLib/src/xmc_ccu4.c"
)

MESSAGE(STATUS "\nFound following source files:\n ${SOURCES}\n")

# define executable
include(${CMAKE_CURRENT_SOURCE_DIR}/src/bricklib2/cmake/configs/config_define_executable.txt)

# add aeabi aeabi_idiv, aeabi_uidiv, aeabi_idivmod and aeabi_uidivmod
include(${CMAKE_CURRENT_SOURCE_DIR}/src/bricklib2/cmake/configs/config_comcu_add_aeabi_libraries.txt)

# define standard compile and linker flags
SET(LINKER_SCRIPT_NAME xmc1_firmware_with_brickletboot.ld)
SET(FLASH_ORIGIN 0x10003000) # Move flash origin above the bootloader
SET(FLASH_EEPROM_LENGTH 1024) # Flash used for EEPROM emulation at end of flash (multiple of page size (256 byte))
MATH(EXPR FLASH_LENGTH "${CHIP_FLASH_SIZE} - 8192 - ${FLASH_EEPROM_LENGTH}") # Remove bootloader size from flash size
include(${CMAKE_CURRENT_SOURCE_DIR}/src/bricklib2/cmake/configs/config_comcu_add_standard_flags.txt)

# add custom build commands
include(${CMAKE_CURRENT_SOURCE_DIR}/src/bricklib2/cmake/configs/config_comcu_add_standard_custom_commands.txt)

# add preprocessor defines
include(${CMAKE_CURRENT_SOURCE_DIR}/src/bricklib2/cmake/configs/config_xmc1_add_preprocessor_defines.txt)
