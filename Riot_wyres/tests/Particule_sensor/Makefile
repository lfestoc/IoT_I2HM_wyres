# name of your application
APPLICATION = Particule_sensor

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= ../../../../RIOT-OS/RIOT

# Path to my boards
EXTERNAL_BOARD_DIRS ?= ../../boards

# If no BOARD is found in the environment, use this default:
BOARD ?= wyres-base


# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Modules to include:
USEMODULE += xtimer
USEMODULE += analog_util

FEATURES_REQUIRED += periph_adc

include $(RIOTBASE)/Makefile.include
