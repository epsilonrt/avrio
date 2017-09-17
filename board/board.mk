# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# $Id$
# Current BOARD list :
#
# ------------- manufactured by ATMEL
#  * STK200
#     STK200 Starter Kit
#     http://www.atmel.com/dyn/resources/prod_documents/doc1107.pdf
#  * USBKEY
#     AT90USBKey - AT90USB Evaluation Kit
#     http://www.atmel.com/dyn/resources/prod_documents/doc7627.pdf
#  * BUTTERFLY
#     AVR Butterfly Evaluation Kit
#     http://www.atmel.com/dyn/resources/prod_documents/doc4271.pdf
#  * DVK90CAN1
#     DVK90CAN1 - AT90CAN128 Development Kit
#     http://www.atmel.com/dyn/resources/prod_documents/doc4381.pdf
#
# ------------- manufactured by ARDUINO
#  * UNO
#     Arduino UNO
#     http://arduino.cc/en/Main/arduinoBoardUno
#
# ------------- manufactured by EGNITE
#  * ETHERNUT1
#     Ethernut 1.3
#     http://www.ethernut.de/pdf/enhwm13e.pdf
#
# ------------- manufactured by OLIMEX
#  * AVRUSBSTK
#     AVR-USB-STK development board
#     http://www.olimex.com/dev/pdf/AVR/AVR-USB-STK.pdf
#  * AVRUSBSTK_M32U2
#     AVR-USB-STK development board modified with ATmega32U2 MCU
#     http://www.olimex.com/dev/pdf/AVR/AVR-USB-STK.pdf
#  * AVRCAN
#     AVR-CAN development board
#     http://www.olimex.com/dev/pdf/AVR/AVR-CAN.pdf
#
# ------------- manufactured by chip45
#  * IDWARF_NODE
#     iDwaRF-328 V1.2 board
#     http://download.chip45.com/iDwaRF-328_V1.2_infosheet.pdf
#  * IDWARF_BOX
#     iDwaRF-BOX V1.2 board
#     http://download.chip45.com/iDwaRF-BOX_V1.2_infosheet.pdf
#
# ------------- manufactured by epsilonRT
#  * MAVRIX
#  * MAVRIX_RELOADED
#  * SOLARIX
#  * DMNET_HUB
#  * DMNET_STK200
#  * DMNET_SERVER_IHM
#  * TOUERIS_IHM
#  * GIFAM_TESTER
#  * GIFAM_CHIP
#  * XNODE
#  * CHIPIO_XNET
#  * CHIPIO_SOLARPI
#  * SOLARPI_PRESS
#  * SOLARPI_LUM
#  * SOLARPI_HUM
#  * SOLARPI_ETEMP
#  * SOLARPI_ITEMP
#  * SOLARPI_TX
#  * SOLARPI_RX
#  * Solar PI Mission 2014
#  - APRS_TRACKER
#  - APRS_SHIELD
#  - P1_HUM
#  - P6_LUM
#  - P8_PRESS
#  - P9_TEMPE
#  - P10_TEMPI
#
# Note:
# If BOARD is empty in the Makefile, the user can describe his board in a
# board.mk file that will be in the same directory as the Makefile.
#----------------------------------------------------------------------------

ifeq ($(BOARD),)
#----------------------------------------------------------------------------
# BOARD not defined
#############################################################################
#                               USER BOARD                                  #
#############################################################################
# if BOARD not defined, the board is user defined
$(if $(wildcard $(PROJECT_TOPDIR)/board.mk),,\
  $(error BOARD is not defined in the Makefile, it requires a board.mk file to define the board's user  in the current directory !))

include $(PROJECT_TOPDIR)/board.mk
BOARD = USER

else
#----------------------------------------------------------------------------
# BOARD defined

ifeq ($(USER_PROJECT),)
#----------------------------------------------------------------------------
# USER_PROJECT not defined

#############################################################################
#                              ATMEL BOARDS                                 #
#############################################################################

#----------------------------------------------------------------------------
ifeq ($(BOARD),STK200)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/atmel/stk200

# MCU name
ifeq ($(MCU),)
# MCU = atmega8535
MCU = atmega32
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

#---------------- External Memory Options ----------------

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff

EXTMEMOPTS =

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),USBKEY)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/atmel/usbkey

# MCU name
MCU = at90usb1287

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

#---------------- External Memory Options ----------------

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff

EXTMEMOPTS =

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),BUTTERFLY)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/atmel/butterfly

# MCU name
MCU = atmega169

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

#---------------- External Memory Options ----------------

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff

EXTMEMOPTS =

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),DVK90CAN1)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/atmel/dvk90can1

# MCU name
ifeq ($(MCU),)
MCU = at90can128
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

#---------------- External Memory Options ----------------

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff

EXTMEMOPTS =

endif

#############################################################################
#                             ARDUINO BOARDS                                #
#############################################################################

#----------------------------------------------------------------------------
ifeq ($(BOARD),UNO)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/arduino/uno

# MCU name
ifeq ($(MCU),)
MCU = atmega328p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 16000000
endif

ARDUINO_VARIANT = standard

# Arduino OnBoard Programmer
ifeq ($(AVRDUDE_PROGRAMMER),)
AVRDUDE_PROGRAMMER ?= arduino
endif
ifeq ($(AVRDUDE_PORT),)
AVRDUDE_PORT ?= /dev/ttyACM0
endif
#AVRDUDE_PORT ?= COM4
#AVRDUDE_BAUDRATE ?= 115200

endif

#############################################################################
#                             EGNITE BOARDS                                 #
#############################################################################

#----------------------------------------------------------------------------
ifeq ($(BOARD),ETHERNUT1)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/egnite/ethernut

# MCU name
MCU = atmega128

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 14745600
endif

#---------------- External Memory Options ----------------

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--section-sta  rt,.data=0x801100,--defsym=__heap_end=0x80ffff
# ETHERNUT1 Memory Map
# 0x0000 - 0x001F   CPU Registers (Note 1)
# 0x0020 - 0x005F   I/O Registers (Note 1)
# 0x0060 - 0x00FF   Extended I/O Registers (Note 1)
# 0x0100 - 0x10FF   Fast Internal RAM, no wait states required
# 0x1100 - 0x7FFF   External RAM, 1 wait state is recommended
# 0x8000 - 0x82FF   Unused, Available for Custom Extensions
# 0x8300 - 0x831F   Ethernet Controller Registers (Note 2)
# 0x8320 - 0xFFFF   Unused, Available for Custom Extensions
EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x807fff

# Enable XMEM interface module in AvrIO
AVRIO_CONFIG += AVRIO_XMEM_ENABLE

endif

#############################################################################
#                             OLIMEX BOARDS                                 #
#############################################################################

#----------------------------------------------------------------------------
ifeq ($(BOARD),AVRUSBSTK)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/olimex/avrusbstk

# MCU name
MCU = at90usb162

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),AVRUSBSTK_M32U2)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/olimex/avrusbstk/m32u2

# MCU name
MCU = atmega32u2

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),AVRCAN)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/olimex/avrcan

# MCU name
MCU = at90can128

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 16000000
endif

#---------------- External Memory Options ----------------

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff

EXTMEMOPTS =

endif

#############################################################################
#                              CHIP45 BOARDS                                #
#############################################################################

#----------------------------------------------------------------------------
ifeq ($(BOARD),IDWARF_NODE)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/chip45/node

# MCU name
ifeq ($(MCU),)
MCU = atmega328p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 7372800
endif

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),IDWARF_BOX)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/chip45/box

# MCU name
MCU = atmega128

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 7372800
endif

#---------------- External Memory Options ----------------

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff

EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x8090ff

endif

#############################################################################
#                            EPSILONRT BOARDS                               #
#############################################################################

#----------------------------------------------------------------------------
ifeq ($(BOARD),MAVRIX)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/mavrix

# MCU name
MCU = atmega128

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 14745600
endif

#---------------- External Memory Options ----------------

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff

EXTMEMOPTS =

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),MAVRIX_RELOADED)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/mavrix/reloaded

# MCU name
MCU = atmega128

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 7372800
endif


#---------------- External Memory Options ----------------

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff
EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x807fff

# Enable XMEM interface module in AvrIO
AVRIO_CONFIG += AVRIO_XMEM_ENABLE

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),SOLARIX)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/solarix

# MCU name
MCU = atmega32

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 16000000
endif

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),DMNET_HUB)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/dmnet/dmnet-hub

# MCU name
ifeq ($(MCU),)
MCU = atmega328p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 7372800
endif

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),DMNET_STK200)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/dmnet/dmnet-stk200

# MCU name
ifeq ($(MCU),)
MCU = atmega168
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 7372800
endif

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),DMNET_SERVER_IHM)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/dmnet/dmnet-server-ihm

# MCU name
MCU = atmega32

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),TOUERIS_IHM)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/toueris/toueris-ihm

# MCU name
ifeq ($(MCU),)
MCU = atmega168
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),GIFAM_TESTER)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/gifam-tester

# MCU name
ifeq ($(MCU),)
MCU = atmega168
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),GIFAM_CHIP)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/gifam-chip
# MCU name
ifeq ($(MCU),)
MCU = attiny45
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

endif


#----------------------------------------------------------------------------
ifeq ($(BOARD),USBKEY_MSL)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/usbkey-msl

# MCU name
MCU = at90usb1287

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

#---------------- External Memory Options ----------------

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff

EXTMEMOPTS =

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),DVK90CAN1_XBEE)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/dvk90can1-xbee

# MCU name
ifeq ($(MCU),)
MCU = at90can128
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 16000000
endif

#---------------- External Memory Options ----------------

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff

EXTMEMOPTS =
#---------------- Programming Options (avrdude) ----------------
# Programming hardware
# Type: avrdude -c ?
# to get a full listing.
#
# STK200 programmer on parallel port
#AVRDUDE_PROGRAMMER ?= stk200
#AVRDUDE_PORT ?= lpt1

# AVRPROG programmer on serial port
#AVRDUDE_PROGRAMMER ?= avr109
#AVRDUDE_PORT ?= com1
#AVRDUDE_BAUD = 38400

# JTAG ICE MkII
#AVRDUDE_PROGRAMMER ?= jtag2
#AVRDUDE_PORT ?= usb

# AVR Dragon
AVRDUDE_PROGRAMMER ?= dragon_jtag
AVRDUDE_PORT ?= usb

# Fuses and lock for fuse target
#AVRDUDE_LFUSE ?= 0xE2
#AVRDUDE_HFUSE ?= 0xDB
#AVRDUDE_EFUSE ?= 0x07
#AVRDUDE_LOCK  ?= 0x0F

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),XPLBEE)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/xplbee

# MCU name
ifeq ($(MCU),)
# !! You must change the following setting to match your target board !!
MCU = atmega1284p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
# !! You must change the following setting to match your target board !!
F_CPU = 16000000
endif

#---------------- External Memory Options ----------------

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff

EXTMEMOPTS =

#---------------- Programming Options (avrdude) ----------------
# Programming hardware
# Type: avrdude -c ?
# to get a full listing.
#

# !! You must change and enable the setting below to match your programmer !!

# "-I" skips interrupts which make debugging easier while a timer is running

# AVR Dragon ISP
#AVRDUDE_PROGRAMMER ?= dragon_isp
#AVRDUDE_PORT ?= usb

# AVR Dragon JTAG
AVRDUDE_PROGRAMMER ?= dragon_jtag
AVRDUDE_PORT ?= usb
AVARICE_OPT ?= --dragon 

# JTAG ICE MkII
#AVRDUDE_PROGRAMMER ?= jtag2
#AVRDUDE_PORT ?= usb
#AVARICE_OPT ?= --mkII

# Arduino OnBoard Programmer
#AVRDUDE_PROGRAMMER ?= arduino
#AVRDUDE_PORT ?= /dev/ttyACM0
#AVRDUDE_PORT ?= COM4

# STK200 programmer on parallel port
#AVRDUDE_PROGRAMMER ?= stk200
#AVRDUDE_PORT ?= lpt1

# Fuses and lock for fuse target
# !! You may change the following setting to match your target board !!
AVRDUDE_LFUSE ?= 0xEF
AVRDUDE_HFUSE ?= 0x31
AVRDUDE_EFUSE ?= 0xFF
#AVRDUDE_LOCK  ?= 0xFF

# Set the DEBUG_UI to either gdb or insight.
#DEBUG_UI = gdb
DEBUG_UI = cgdb
#DEBUG_UI = insight

# Set the debugging back-end to either avarice, simulavr.
DEBUG_BACKEND = avarice
#DEBUG_BACKEND = simulavr

JTAG_DEV = usb
AVARICE_BITRATE ?= --jtag-bitrate 2MHz

#----------------------------------------------------------------------------
endif

#############################################################################
#                               MTO BOARD                                   #
#############################################################################

#----------------------------------------------------------------------------
ifeq ($(BOARD),MTO_TEMPLATE)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/mto/template

# MCU name
ifeq ($(MCU),)
MCU = attiny841
endif

# Processor frequency.
ifeq ($(F_CPU),)
# !! You must change the following setting to match your target board !!
F_CPU = 8000000
endif

#---------------- Programming Options (avrdude) ----------------
# Programming hardware
# Type: avrdude -c ?
# to get a full listing.
#

# !! You must change and enable the setting below to match your programmer !!

# User
AVRDUDE_PROGRAMMER ?= dragon_isp
AVRDUDE_PORT ?= usb
#AVRDUDE_FLAGS += -B 1.0MHz

# Fuses and lock for fuse target
# !! You may change the following setting to match your target board !!
AVRDUDE_LFUSE ?= 0xC2
AVRDUDE_HFUSE ?= 0xD7
AVRDUDE_EFUSE ?= 0xFF
#AVRDUDE_LOCK  ?= 0xFF

# Set the DEBUG_UI to either gdb or insight.
#DEBUG_UI = gdb
DEBUG_UI = cgdb
#DEBUG_UI = insight

# Set the debugging back-end to either avarice, simulavr.
DEBUG_BACKEND = avarice
#DEBUG_BACKEND = simulavr

JTAG_DEV = usb
AVARICE_BITRATE ?= --jtag-bitrate 2MHz
AVARICE_OPT ?= --dragon  
#AVARICE_OPT ?= --mkII

#----------------------------------------------------------------------------
endif

#############################################################################
#                             SOLARPI BOARD                                 #
#############################################################################

#----------------------------------------------------------------------------
ifeq ($(BOARD),SOLARPI_HUM)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/solarpi/sensor-hum

# MCU name
ifeq ($(MCU),)
MCU = atmega168p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

# AVR Dragon
AVRDUDE_PROGRAMMER ?= dragon_isp
AVRDUDE_PORT ?= usb

# Fuses and lock for fuse target
AVRDUDE_LFUSE ?= 0xE2
AVRDUDE_HFUSE ?= 0xDD
#AVRDUDE_EFUSE ?= 0x01
#AVRDUDE_LOCK  ?= 0x0F

#----------------------------------------------------------------------------
endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),SOLARPI_LUM)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/solarpi/sensor-lum

# MCU name
ifeq ($(MCU),)
MCU = atmega168p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

# AVR Dragon
AVRDUDE_PROGRAMMER ?= dragon_isp
AVRDUDE_PORT ?= usb

# Fuses and lock for fuse target
AVRDUDE_LFUSE ?= 0xE2
AVRDUDE_HFUSE ?= 0xDD
#AVRDUDE_EFUSE ?= 0x01
#AVRDUDE_LOCK  ?= 0x0F

#----------------------------------------------------------------------------
endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),SOLARPI_PRESS)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/solarpi/sensor-press

# MCU name
ifeq ($(MCU),)
MCU = atmega168p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

# AVR Dragon
AVRDUDE_PROGRAMMER ?= dragon_isp
AVRDUDE_PORT ?= usb

# Fuses and lock for fuse target
AVRDUDE_LFUSE ?= 0xE2
AVRDUDE_HFUSE ?= 0xDD
#AVRDUDE_EFUSE ?= 0x01
#AVRDUDE_LOCK  ?= 0x0F

#----------------------------------------------------------------------------
endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),SOLARPI_ETEMP)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/solarpi/sensor-etemp

# MCU name
ifeq ($(MCU),)
MCU = atmega168p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

# AVR Dragon
AVRDUDE_PROGRAMMER ?= dragon_isp
AVRDUDE_PORT ?= usb

# Fuses and lock for fuse target
AVRDUDE_LFUSE ?= 0xE2
AVRDUDE_HFUSE ?= 0xDD
#AVRDUDE_EFUSE ?= 0x01
#AVRDUDE_LOCK  ?= 0x0F

#----------------------------------------------------------------------------
endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),SOLARPI_ITEMP)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/solarpi/sensor-itemp

# MCU name
ifeq ($(MCU),)
MCU = atmega168p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

# AVR Dragon
AVRDUDE_PROGRAMMER ?= dragon_isp
AVRDUDE_PORT ?= usb

# Fuses and lock for fuse target
AVRDUDE_LFUSE ?= 0xE2
AVRDUDE_HFUSE ?= 0xDD
#AVRDUDE_EFUSE ?= 0x01
#AVRDUDE_LOCK  ?= 0x0F

#----------------------------------------------------------------------------
endif


#----------------------------------------------------------------------------
ifeq ($(BOARD),SOLARPI_TX)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/solarpi/radio-tx

# MCU name
ifeq ($(MCU),)
MCU = atmega168p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 7372800
endif

CDEFS += -DSERIAL_BAUDRATE_MAX=115200

# AVR Dragon
ifeq ($(AVRDUDE_PROGRAMMER),)
AVRDUDE_PROGRAMMER ?= dragon_isp
AVRDUDE_PORT ?= usb
endif

# Fuses and lock for fuse target
AVRDUDE_LFUSE ?= 0xCD
AVRDUDE_HFUSE ?= 0xDF
#AVRDUDE_EFUSE ?= 0x01
#AVRDUDE_LOCK  ?= 0x0F

#----------------------------------------------------------------------------
endif


#----------------------------------------------------------------------------
ifeq ($(BOARD),SOLARPI_RX)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/solarpi/radio-rx

# MCU name
ifeq ($(MCU),)
MCU = atmega328p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 16000000
endif

CDEFS += -DSERIAL_BAUDRATE_MAX=76800

ifeq ($(AVRDUDE_PROGRAMMER),)
# AVR Dragon
#AVRDUDE_PROGRAMMER ?= dragon_isp
# Arduino OnBoard Programmer
AVRDUDE_PROGRAMMER ?= arduino
#AVRDUDE_BAUD ?= 115200
endif

ifeq ($(AVRDUDE_PORT),)
#AVRDUDE_PORT ?= usb
#AVRDUDE_PORT ?= /dev/ttyACM0
AVRDUDE_PORT ?= COM4
endif

# Fuses and lock for fuse target
#AVRDUDE_LFUSE ?= 0xCD
#AVRDUDE_HFUSE ?= 0xDF
#AVRDUDE_EFUSE ?= 0x01
#AVRDUDE_LOCK  ?= 0x0F

endif


#----------------------------------------------------------------------------
ifeq ($(BOARD),XNET_NODE)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/xnet-node

# MCU name
ifeq ($(MCU),)
MCU = atmega328p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

#---------------- Programming Options (avrdude) ----------------
# Programming hardware
# Type: avrdude -c ?
# to get a full listing.
#
# STK200 programmer on parallel port
#AVRDUDE_PROGRAMMER ?= stk200
#AVRDUDE_PORT ?= lpt1

# AVRPROG programmer on serial port
#AVRDUDE_PROGRAMMER ?= avr109
#AVRDUDE_PORT ?= com1
#AVRDUDE_BAUD ?= 38400

# JTAG ICE MkII
#AVRDUDE_PROGRAMMER ?= jtag2
#AVRDUDE_PORT ?= usb

# AVR Dragon
AVRDUDE_PROGRAMMER ?= dragon_isp
AVRDUDE_PORT ?= usb

# Fuses and lock for fuse target
AVRDUDE_LFUSE ?= 0xE2
AVRDUDE_HFUSE ?= 0xDB
#AVRDUDE_EFUSE ?= 0x07
#AVRDUDE_LOCK  ?= 0x0F

#----------------------------------------------------------------------------
endif


#############################################################################
#                               CHIPIO BOARD                                #
#############################################################################

#----------------------------------------------------------------------------
ifeq ($(BOARD),CHIPIO_XNET)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/chipio/xnet

# MCU name
ifeq ($(MCU),)
MCU = atmega328p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

#---------------- Programming Options (avrdude) ----------------
# Programming hardware
# Type: avrdude -c ?
# to get a full listing.
#
# STK200 programmer on parallel port
#AVRDUDE_PROGRAMMER ?= stk200
#AVRDUDE_PORT ?= lpt1

# JTAG ICE MkII
#AVRDUDE_PROGRAMMER ?= jtag2
#AVRDUDE_PORT ?= usb

# AVR Dragon
AVRDUDE_PROGRAMMER ?= dragon_isp
AVRDUDE_PORT ?= usb

# Fuses and lock for fuse target
AVRDUDE_LFUSE ?= 0xD2
AVRDUDE_HFUSE ?= 0xD9
#AVRDUDE_EFUSE ?= 0xFF
#AVRDUDE_LOCK  ?= 0xFF

#----------------------------------------------------------------------------
endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),CHIPIO_SOLARPI)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/chipio/solarpi

# MCU name
ifeq ($(MCU),)
MCU = atmega168p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 14745600
endif

#---------------- Programming Options (avrdude) ----------------
# Programming hardware
# Type: avrdude -c ?
# to get a full listing.
#
# STK200 programmer on parallel port
#AVRDUDE_PROGRAMMER ?= stk200
#AVRDUDE_PORT ?= lpt1

# JTAG ICE MkII
#AVRDUDE_PROGRAMMER ?= jtag2
#AVRDUDE_PORT ?= usb

# AVR Dragon
AVRDUDE_PROGRAMMER ?= dragon_isp
AVRDUDE_PORT ?= usb

# Fuses and lock for fuse target
AVRDUDE_LFUSE ?= 0xF6
AVRDUDE_HFUSE ?= 0xDF
AVRDUDE_EFUSE ?= 0xF9
#AVRDUDE_LOCK  ?= 0xFF

#----------------------------------------------------------------------------
endif

#############################################################################
#                    SOLARPI BOARD  (MISSION 2014)                          #
#############################################################################

#----------------------------------------------------------------------------
ifeq ($(BOARD),APRS_TRACKER)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/solarpi/mission-2014/tracker

# MCU name
ifeq ($(MCU),)
MCU = atmega168p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 14745600
endif

#---------------- Programming Options (avrdude) ----------------
# Programming hardware
# Type: avrdude -c ?
# to get a full listing.
#

# !! You must change and enable the setting below to match your programmer !!

# "-I" skips interrupts which make debugging easier while a timer is running

# AVR Dragon ISP
AVRDUDE_PROGRAMMER ?= dragon_isp
AVRDUDE_PORT ?= usb

# AVR Dragon JTAG
#AVRDUDE_PROGRAMMER ?= dragon_jtag
#AVRDUDE_PORT ?= usb

# JTAG ICE MkII
#AVRDUDE_PROGRAMMER ?= jtag2
#AVRDUDE_PORT ?= usb
#AVARICE_OPT ?= --mkII

# Arduino OnBoard Programmer
#AVRDUDE_PROGRAMMER ?= arduino
#AVRDUDE_PORT ?= /dev/ttyACM0
#AVRDUDE_PORT ?= COM4

# STK200 programmer on parallel port
#AVRDUDE_PROGRAMMER ?= stk200
#AVRDUDE_PORT ?= lpt1

# Fuses and lock for fuse target
# !! You may change the following setting to match your target board !!
#AVRDUDE_LFUSE ?= 0xEF
#AVRDUDE_HFUSE ?= 0x31
#AVRDUDE_EFUSE ?= 0xFF
#AVRDUDE_LOCK  ?= 0xFF

# Set the DEBUG_UI to either gdb or insight.
#DEBUG_UI = gdb
DEBUG_UI = cgdb
#DEBUG_UI = insight

# Set the debugging back-end to either avarice, simulavr.
DEBUG_BACKEND = avarice
#DEBUG_BACKEND = simulavr

#JTAG_DEV = usb
AVARICE_OPT ?= --dragon 
AVARICE_BITRATE ?= --jtag-bitrate 2MHz

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),APRS_SHIELD)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/solarpi/mission-2014/tracker-shield

# MCU name
ifeq ($(MCU),)
MCU = atmega328p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 16000000
endif

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),P1_HUM)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/solarpi/mission-2014/p1-hum

# MCU name
ifeq ($(MCU),)
MCU = atmega168p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

#AVRDUDE_FLAGS ?= -B 8.0
#AVRDUDE_PROGRAMMER ?= dragon_isp
#AVRDUDE_PORT ?= usb

#----------------------------------------------------------------------------
endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),P6_LUM)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/solarpi/mission-2014/p6-lum

# MCU name
ifeq ($(MCU),)
MCU = atmega168p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

#AVRDUDE_FLAGS ?= -B 8.0
#AVRDUDE_PROGRAMMER ?= dragon_isp
#AVRDUDE_PORT ?= usb

#----------------------------------------------------------------------------
endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),P8_PRESS)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/solarpi/mission-2014/p8-press

# MCU name
ifeq ($(MCU),)
MCU = atmega168p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

#AVRDUDE_FLAGS ?= -B 8.0
#AVRDUDE_PROGRAMMER ?= dragon_isp
#AVRDUDE_PORT ?= usb

#----------------------------------------------------------------------------
endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),P9_TEMPE)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/solarpi/mission-2014/p9-tempe

# MCU name
ifeq ($(MCU),)
MCU = atmega168p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

#AVRDUDE_FLAGS ?= -B 8.0
#AVRDUDE_PROGRAMMER ?= dragon_isp
#AVRDUDE_PORT ?= usb

#----------------------------------------------------------------------------
endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),P10_TEMPI)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/solarpi/mission-2014/p10-tempi

# MCU name
ifeq ($(MCU),)
MCU = atmega168p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 8000000
endif

#AVRDUDE_FLAGS ?= -B 8.0
#AVRDUDE_PROGRAMMER ?= dragon_isp
#AVRDUDE_PORT ?= usb

#----------------------------------------------------------------------------
endif

else
#############################################################################
#                             USER PROJECT                                  #
#############################################################################
$(if $(wildcard $(PROJECT_TOPDIR)/board.mk),,\
  $(error BOARD is not defined in the Makefile, it requires a board.mk file to define the board's user  in the current directory !))
#$(warning USER_PROJECT=$(USER_PROJECT))
include $(PROJECT_TOPDIR)/board.mk

endif

#############################################################################
#                              USER BOARDS                                  #
#############################################################################
# TODO: obsolete, à remplacer par un projet utilisateur

# user's projects included in board structure
include $(AVRIO_TOPDIR)/board/user.mk

# BOARD defined
endif
#----------------------------------------------------------------------------
