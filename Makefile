# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile


ARDUINO_DIR       = /opt/arduino/
ARDMK_DIR         = /home/ashitaka/Repos/Arduino-Makefile/
AVR_TOOLS_DIR     = /opt/arduino/hardware/tools/avr/

SPARKFUN_DIR      = /home/ashitaka/Repos/Sparkfun-Arduino-Boards



# ARDUINO_PORT      = /dev/ttyACM0

# BOARD_TAG         = promicro
# BOARD_SUB		  = 16MHzatmega32U4

# BOARDS_TXT        = $(SPARKFUN_DIR)/sparkfun/avr/boards.txt
# BOOTLOADER_PARENT = $(SPARKFUN_DIR)/sparkfun/avr/bootloaders
# ARDUINO_VAR_PATH  = $(SPARKFUN_DIR)/sparkfun/avr/variants

# BOOTLOADER_PATH   = caterina
# BOOTLOADER_FILE   = Caterina-promicro16.hex

# USB_PID			  = 0x9204


BOARD_TAG    = mega
BOARD_SUB    = atmega2560
MONITOR_PORT = /dev/ttyACM0


ARDUINO_LIBS      = Adafruit-GFX-Library Adafruit_SSD1306 Firmata HX711 SPI Wire qpn


#ISP_PROG     	  = usbasp

# Show progress bars
#AVRDUDE_OPTS 	  = -v

include /home/ashitaka/Repos/Arduino-Makefile/Arduino.mk

