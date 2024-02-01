# name of your application
APPLICATION = accelerometer

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= ../../../../RIOT-OS/RIOT

# Path to my boards
EXTERNAL_BOARD_DIRS ?= ../../boards

# External modules
EXTERNAL_MODULE_DIRS += ../../modules
# If no BOARD is found in the environment, use this default:
BOARD ?= wyres-base

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1


# use lis2dh12_spi for SPI-Mode and lis2dh12_i2c for I2C-Mode
DRIVER ?= lis2dh12_spi

USEMODULE += fmt
USEMODULE += ztimer
USEMODULE += ztimer_msec
USEMODULE += shell_cmds_default
USEMODULE += $(DRIVER)

# for using lis2dh12 with interrupt function
USEMODULE += lis2dh12_int

include $(RIOTBASE)/Makefile.include