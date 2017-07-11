# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# $Id$
#----------------------------------------------------------------------------

# Upper to lower case, _ to - macro
lc = $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$(subst _,-,$1)))))))))))))))))))))))))))
# Lower to Upper case, - to _ macro
uc = $(subst a,A,$(subst b,B,$(subst c,C,$(subst d,D,$(subst e,E,$(subst f,F,$(subst g,G,$(subst h,H,$(subst i,I,$(subst j,J,$(subst k,K,$(subst l,L,$(subst m,M,$(subst n,N,$(subst o,O,$(subst p,P,$(subst q,Q,$(subst r,R,$(subst s,S,$(subst t,T,$(subst u,U,$(subst v,V,$(subst w,W,$(subst x,X,$(subst y,Y,$(subst z,Z,$(subst -,_,$1)))))))))))))))))))))))))))
# Set the operating system id: Linux,windows32,MINGW32
OS := $(firstword $(subst _, ,$(shell uname -s)))

ifeq ($(AVRIO_TOPDIR),)
#----------------------------------------------------------------------------
#                               ~~~~AVRIO~~~~
# AVRIO not defined
AVRIODEFS =
AVRIOINCDIR =
AVRIOSRCDIR =
AVRIOBRDDIR =
AVRXLIB = libavrx

else
#----------------------------------------------------------------------------
#AVRIO_TOPDIR_NOBACKSLASH := $(call adjust-path-mixed, $(AVRIO_TOPDIR))
#AVRIO_TOPDIR = $(AVRIO_TOPDIR_NOBACKSLASH)
#$(warning AVRIO_TOPDIR=$(AVRIO_TOPDIR))

# scripts Path
ifeq ($(SCRIPTDIR),)
SCRIPTDIR := $(AVRIO_TOPDIR)/script
endif

# binaries Path
ifeq ($(BINDIR),)
BINDIR := $(AVRIO_TOPDIR)/bin
endif

# Windows: force l'utilisation de la version d'avr-gcc fournie avec AvrIO et
# fournie un bash pour les scripts, le bash et les utilitaires systèmes sont
# prioritaires.
ifeq ($(OS),windows32) 
export PATH := $(subst /,\,$(BINDIR)/win32/utils/bin);${PATH}
else ifeq ($(OS),MINGW32)
export PATH := $(subst /,\,$(BINDIR)/win32/utils/bin);${PATH}
endif

ifneq ($(F_CPU),)
F_CPU_CUSTOM = $(F_CPU)
endif

ifneq ($(MCU),)
MCU_CUSTOM = $(MCU)
endif

# AVRIO defined
include $(AVRIO_TOPDIR)/board/board.mk

# AVRIO Config
AVRIO_CONFIG += __AVRIO__
AVRIO_CONFIG += AVRIO_BOARD_$(BOARD)
ifeq ($(DELAY_LOOPS_DISABLE),ON)
AVRIO_CONFIG += AVRIO_FAST_SIM
endif

ifeq ($(AVRX),ON)
AVRIO_CONFIG += AVRIO_AVRX_ENABLE
AVRXLIB_TARGET = $(AVRXLIB).a
endif

# AVRIO Defs
AVRIODEFS =   $(patsubst %,-D%,$(AVRIO_CONFIG))

# AVRIO Paths
AVRIOINCDIR = $(AVRIO_TOPDIR)/src
AVRIOSRCDIR = $(AVRIO_TOPDIR)/src

# AVRIO Sources
AVRIO_LCD_SRC  = avrio/lcd.c
#AVRIO_LCD_SRC += $(addprefix avrio/lcd/io/, $(notdir $(shell ls $(AVRIOSRCDIR)/avrio/lcd/io/*.c)))
#AVRIO_LCD_SRC += $(addprefix avrio/lcd/ctrl/, $(notdir $(shell ls $(AVRIOSRCDIR)/avrio/lcd/ctrl/*.c)))
AVRIO_LCD_SRC += $(addprefix avrio/lcd/io/, $(notdir $(wildcard $(AVRIOSRCDIR)/avrio/lcd/io/*.c)))
AVRIO_LCD_SRC += $(addprefix avrio/lcd/ctrl/, $(notdir $(wildcard $(AVRIOSRCDIR)/avrio/lcd/ctrl/*.c)))

ifeq ($(AVRIO_ARDUINO),ON)
# The target needs libarduino...
include $(AVRIO_TOPDIR)/src/arduino/arduino.mk
ARDUINO_LIBTARGET = $(ARDUINO_LIB).a
MATH_LIB_ENABLE = ON
CPPSRC += arduino/main.cpp
endif

# AVRX Paths and goals
AVRXLIBDIR = $(AVRIOSRCDIR)/avrx/$(call lc,$(MCU))
AVRXLIB = $(AVRXLIBDIR)/libavrx

ARDUINO_DESTDIR = $(AVRIOSRCDIR)/arduino/$(call lc,$(ARDUINO_VARIANT))
ARDUINO_LIB = $(ARDUINO_DESTDIR)/libarduino

VPATH+=:$(AVRIOSRCDIR)
#                               ~~~~AVRIO~~~~
#----------------------------------------------------------------------------
endif

ifeq ($(ARDUINO_TOPDIR),)
#----------------------------------------------------------------------------
#                              ~~~~ARDUINO~~~~
else
#----------------------------------------------------------------------------
# AVRIO_ARDUINO defined
ARDUINO_INCDIR = $(shell find $(ARDUINO_LIBDIR) -maxdepth 1 -mindepth 1 -type d)
ARDUINO_INCDIR += $(shell find $(ARDUINO_LIBDIR) -maxdepth 2 -mindepth 2 -type d -name utility)
ARDUINO_INCDIR += $(ARDUINO_SRCDIR)/arduino $(ARDUINO_VARDIR)/$(ARDUINO_VARIANT)
# ARDUINO Defs
ARDUINO_DEFS   +=   -DARDUINO=$(ARDUINO)

VPATH+=:$(ARDUINO_SRCDIR)
VPATH+=:$(ARDUINO_LIBDIR)

#                              ~~~~ARDUINO~~~~
#----------------------------------------------------------------------------
endif

ifeq ($(CMG_TOPDIR),)
#----------------------------------------------------------------------------
#                                ~~~~CMG~~~~
# CMG not defined
CMGDEFS     =
CMGINCDIR   =
CMGSRCDIR   =

else
#----------------------------------------------------------------------------
# CMG defined

# scripts Path
ifeq ($(SCRIPTDIR),)
SCRIPTDIR = $(CMG_TOPDIR)/script
endif

# CMG Config
CMG_CONFIG += $(CMG_PLATFORM)

# CMG Defs
CMGDEFS     =   $(patsubst %,-D%,$(CMG_CONFIG))

# CMG Paths
CMGINCDIR   = $(CMG_TOPDIR)
CMGSRCDIR   = $(CMG_TOPDIR)/CMG

# Liste des fichiers sources de CMG
-include $(CMGSRCDIR)/MakeInclude
SRC += $(CMG_SOURCES)

VPATH+=:$(CMG_TOPDIR):$(CMGSRCDIR)
#                                ~~~~CMG~~~~
#----------------------------------------------------------------------------
endif

#----------------------------------------------------------------------------
#                                ~~~~MBM~~~~
ifeq ($(MBMASTER),ON)

ifeq ($(SCRIPTDIR),)
SCRIPTDIR = $(AVRIO_TOPDIR)/script
endif

-include $(SCRIPTDIR)/mbm.mk

VPATH+=:$(MBM_SRCDIRS)
EXTRA_INCDIRS += $(MBM_INCDIRS)
SRC += $(MBM_SRCS)
endif
#                                ~~~~MBM~~~~
#----------------------------------------------------------------------------

ifeq ($(LUFA_TOPDIR),)
#----------------------------------------------------------------------------
#                                ~~~~LUFA~~~~
# LUFA not defined
LUFADEFS     =
LUFAINCDIR   =
LUFASRCDIR   =

else
#----------------------------------------------------------------------------
# LUFA defined

# LUFA Paths
LUFAINCDIR   = $(LUFA_TOPDIR)
LUFASRCDIR   = $(LUFA_TOPDIR)/LUFA

# LUFA Mandatory vars
ARCH         = AVR8
LUFA_PATH    = $(LUFASRCDIR)

# LUFA Architecture
LUFA_CONFIG += ARCH=ARCH_$(ARCH) BOARD=$(LUFA_BOARD)

# LUFA Defs
LUFADEFS     =   $(patsubst %,-D%,$(LUFA_CONFIG)) -DF_USB=$(F_USB)UL

# Liste des fichiers sources de LUFA
-include $(LUFASRCDIR)/Build/lufa_sources.mk

# Retrait du répertoire racine du chemin des fichiers sources de LUFA
SRC += $(subst $(LUFA_TOPDIR)/,,$(LUFA_SRC))

VPATH+=:$(LUFA_TOPDIR)
#                                ~~~~LUFA~~~~
#----------------------------------------------------------------------------
endif

#----------------------------------------------------------------------------
#                                ~~~~USER~~~~
ifeq ($(PROJECT_TOPDIR),)
else
VPATH+=:$(PROJECT_TOPDIR)
EXTRA_INCDIRS += $(PROJECT_TOPDIR)
endif
#                                ~~~~USER~~~~
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# Destination files directory
ifeq ($(ARDUINO_TOPDIR),)
DESTDIR = $(call lc,$(MCU))
else
DESTDIR = $(call lc,$(ARDUINO_VARIANT))
endif

# Object files directory
OBJDIR = $(DESTDIR)/obj

# Full Path of TARGET
TARGET_PATH = $(DESTDIR)/$(TARGET)
TARGET_LIB_PATH = $(DESTDIR)/lib$(TARGET)

# Compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
CSTANDARD = -std=gnu99

# Output format. (can be srec, ihex, binary)
FORMAT = ihex

# Adds include directories
EXTRA_INCDIRS += $(AVRIOINCDIR) $(AVRIOBRDDIR) $(CMGINCDIR) $(LUFAINCDIR) $(ARDUINO_INCDIR)

ifeq ($(DISABLE_DELETE_UNUSED_SECTIONS),)
DISABLE_DELETE_UNUSED_SECTIONS=OFF
endif

#---------------- Compiler Options C ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
ifeq ($(DEBUG),ON)
CFLAGS += -g$(DEBUG_FORMAT) -O$(DEBUG_OPT) -DDEBUG
else
CFLAGS += -O$(OPT) -DNDEBUG
endif

CFLAGS += -DF_CPU=$(F_CPU)UL
CFLAGS += $(CDEFS)
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -D__SIZEOF_ENUM__=1
CFLAGS += -Wall
CFLAGS += -Wstrict-prototypes
ifeq ($(DISABLE_DELETE_UNUSED_SECTIONS),OFF)
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
endif
#CFLAGS += -mshort-calls
#CFLAGS += -fno-unit-at-a-time
#CFLAGS += -Wundef
#CFLAGS += -Wunreachable-code
#CFLAGS += -Wsign-compare
CFLAGS += -Wa,-adhlns=$(addprefix $(OBJDIR)/, $*.lst)
CFLAGS += $(patsubst %,-I%,$(EXTRA_INCDIRS))
CFLAGS += $(CSTANDARD)

#---------------- Compiler Options C++ ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
ifeq ($(DEBUG),ON)
CPPFLAGS += -g$(DEBUG_FORMAT) -O$(DEBUG_OPT) -DDEBUG
else
CPPFLAGS += -O$(OPT) -DNDEBUG
endif
CPPFLAGS += -DF_CPU=$(F_CPU)UL
CPPFLAGS += $(CPPDEFS)
CPPFLAGS += -funsigned-char
CPPFLAGS += -funsigned-bitfields
CPPFLAGS += -fpack-struct
CPPFLAGS += -fshort-enums
CPPFLAGS += -D__SIZEOF_ENUM__=1
CPPFLAGS += -fno-exceptions
CPPFLAGS += -Wall
ifeq ($(DISABLE_DELETE_UNUSED_SECTIONS),OFF)
CPPFLAGS += -ffunction-sections
CPPFLAGS += -fdata-sections
endif
CPPFLAGS += -Wundef
#CPPFLAGS += -mshort-calls
#CPPFLAGS += -fno-unit-at-a-time
#CPPFLAGS += -Wstrict-prototypes
#CPPFLAGS += -Wunreachable-code
#CPPFLAGS += -Wsign-compare
CPPFLAGS += -Wa,-adhlns=$(addprefix $(OBJDIR)/, $*.lst)
CPPFLAGS += $(patsubst %,-I%,$(EXTRA_INCDIRS))
#CPPFLAGS += $(CSTANDARD)


#---------------- Assembler Options ----------------
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -adhlns:   create listing
#  -gstabs:   have the assembler create line number information; note that
#             for use in COFF files, additional information about filenames
#             and function names needs to be present in the assembler source
#             files -- see avr-libc docs [FIXME: not yet described there]
#  -listing-cont-lines: Sets the maximum number of continuation lines of hex
#       dump that will be displayed for a given single line of source input.
ASFLAGS += -DF_CPU=$(F_CPU)
ASFLAGS += $(ADEFS)
ifeq ($(DISABLE_DELETE_UNUSED_SECTIONS),OFF)
ASFLAGS += -ffunction-sections
ASFLAGS += -fdata-sections
endif
ASFLAGS +=  -Wa,-adhlns=$(addprefix $(OBJDIR)/, $*.lst),-gstabs+
ASFLAGS += $(patsubst %,-I%,$(EXTRA_INCDIRS))

#---------------- Library Options ----------------
# Minimalistic printf version
PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min

# Floating point printf version (requires MATH_LIB = -lm below)
PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt

# If this is left blank, then it will use the Standard printf version.
ifeq ($(PRINTF_VERSION),STANDARD)
PRINTF_LIB =
endif

ifeq ($(PRINTF_VERSION),MIN)
PRINTF_LIB = $(PRINTF_LIB_MIN)
endif

ifeq ($(PRINTF_VERSION),FLOAT)
PRINTF_LIB = $(PRINTF_LIB_FLOAT)
MATH_LIB_ENABLE = ON
endif


# Minimalistic scanf version
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Floating point + %[ scanf version (requires MATH_LIB = -lm below)
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt

# If this is left blank, then it will use the Standard scanf version.
ifeq ($(SCANF_VERSION),STANDARD)
SCANF_LIB =
endif

ifeq ($(SCANF_VERSION),MIN)
SCANF_LIB = $(SCANF_LIB_MIN)
endif

ifeq ($(SCANF_VERSION),FLOAT)
SCANF_LIB = $(SCANF_LIB_FLOAT)
MATH_LIB_ENABLE = ON
endif

ifeq ($(MATH_LIB_ENABLE),ON)
MATH_LIB = -lm
# AVR relocation truncations workaround 
# relocation truncated to fit: R_AVR_13_PCREL against symbol
#MATH_LIB = -nodefaultlibs -lm -lgcc -lc -lgcc
endif

#---------------- Linker Options ----------------
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS += -Wl,-Map=$(TARGET_PATH).map,--cref
LDFLAGS += $(EXTMEMOPTS)
LDFLAGS += $(patsubst %,-L%,$(EXTRA_LIBDIRS))
LDFLAGS += $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB) $(AVRX_LIB)
LDFLAGS += $(patsubst %,-l%,$(EXTRA_LIBS))
LDFLAGS += -Wl,--gc-sections -Wl,--relax
ifeq ($(DISABLE_DELETE_UNUSED_SECTIONS),OFF)
LDFLAGS += -Wl,-static 
ifneq ($(DEBUG),ON)
LDFLAGS += -Wl,-s
endif
endif
LDFLAGS += -g
#LDFLAGS += -T linker_script.x



#---------------- Programming Options (avrdude) ----------------
AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
ifeq ($(AVRDUDE_WRITE_EEPROM_ENABLE),ON)
AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep
endif

# Uncomment the following if you want avrdude's erase cycle counter.
# Note that this counter needs to be initialized first using -Yn,
# see avrdude manual.
#AVRDUDE_ERASE_COUNTER = -y

# Uncomment the following if you do /not/ wish a verification to be
# performed after programming the device.
#AVRDUDE_NO_VERIFY = -V

# Increase verbosity level.  Please use this when submitting bug
# reports about avrdude. See <http://savannah.nongnu.org/projects/avrdude>
# to submit bug reports.
#AVRDUDE_VERBOSE = -v -v

AVRDUDE_FLAGS += -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)
AVRDUDE_FLAGS += $(AVRDUDE_NO_VERIFY)
AVRDUDE_FLAGS += $(AVRDUDE_VERBOSE)
AVRDUDE_FLAGS += $(AVRDUDE_ERASE_COUNTER)

ifeq ($(AVRDUDE_BAUD),)
else
AVRDUDE_FLAGS += -b $(AVRDUDE_BAUD)
endif


ifeq ($(AVRDUDE_LFUSE),)
else
AVRDUDE_BITRATE = 8.0
AVRDUDE_WRITE_FUSES += -U lfuse:w:$(AVRDUDE_LFUSE):m
endif

ifeq ($(AVRDUDE_HFUSE),)
else
AVRDUDE_WRITE_FUSES += -U hfuse:w:$(AVRDUDE_HFUSE):m
endif

ifeq ($(AVRDUDE_EFUSE),)
else
AVRDUDE_WRITE_FUSES += -U efuse:w:$(AVRDUDE_EFUSE):m
endif

ifeq ($(AVRDUDE_LOCK),)
else
AVRDUDE_BITRATE = 8.0
AVRDUDE_WRITE_LOCKS += -U lock:w:$(AVRDUDE_LOCK):m
endif

ifeq ($(AVRDUDE_BITRATE),)
else
AVRDUDE_FLAGS += -B $(AVRDUDE_BITRATE)
endif

#---------------- Debugging Options ----------------

# For simulavr only - target MCU frequency.
DEBUG_MFREQ = $(F_CPU)

# GDB Init Filename.
GDBINIT_FILE = __avr_gdbinit

# Debugging port used to communicate between GDB / avarice / simulavr.
DEBUG_PORT = 4242

# Debugging host used to communicate between GDB / avarice / simulavr, normally
#     just set to localhost unless doing some sort of crazy debugging when
#     avarice is running on a different computer.
DEBUG_HOST = localhost

#============================================================================
# Define programs and commands.
SHELL = sh
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
AR = avr-ar rcs
NM = avr-nm
AVRDUDE = avrdude
MAKEDIR = mkdir -p
REMOVE = rm -f
REMOVEDIR = rm -r -f
COPY = cp
WINSHELL = cmd

# Define Messages
# English
MSG_ERRORS_NONE = Errors: none

# Define Messages
# English
MSG_COMPILING         = "[CC]      "
MSG_COMPILING_CPP     = "[CPP]     "
MSG_ASSEMBLING        = "[ASM]     "
MSG_LINKING           = "[LINK]    "
MSG_CREATING_LIBRARY  = "[LIB]     "
MSG_CLEANING          = "[CLEAN]   "
MSG_EXTENDED_LISTING  = "[LISTING] "
MSG_SYMBOL_TABLE      = "[SYMBOL]  "
MSG_SIZE              = "[SIZE]    "
MSG_COFF              = "[COFF]    "
MSG_EXTENDED_COFF     = "[ECOFF]   "
MSG_FLASH             = "[FLASH]   "
MSG_EEPROM            = "[EEPROM]  "

# Define all object files.
OBJ = $(addprefix $(OBJDIR)/, $(SRC:%.c=%.o) $(CPPSRC:%.cpp=%.o) $(ASRC:%.S=%.o))

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MMD -MP -MF $(@D)/dep/$(@F).d

# Generate the list of directories for object files
OBJDIRS := $(sort $(dir $(OBJ)))
DEPDIRS := $(addsuffix dep, $(OBJDIRS))
#$(info OBJ=$(OBJ))
#$(info OBJDIRS=$(OBJDIRS))
#$(info DEPDIRS=$(DEPDIRS))

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(AVRIODEFS) $(CMGDEFS) $(LUFADEFS) $(ARDUINO_DEFS) $(GENDEPFLAGS)
ALL_CPPFLAGS = -mmcu=$(MCU) -I. -x c++ $(CPPFLAGS) $(AVRIODEFS) $(CMGDEFS) $(ARDUINO_DEFS) $(LUFADEFS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS) $(AVRIODEFS) $(CMGDEFS)  $(LUFADEFS)
LD_CFLAGS = -mmcu=$(MCU)
ifeq ($(DEBUG),ON)
LD_CFLAGS += -g$(DEBUG_FORMAT)
endif

ifeq ($(VIEW_GCC_LINE),ON)
else
CC := @$(CC)
OBJCOPY := @$(OBJCOPY)
OBJDUMP := @$(OBJDUMP)
AR := @$(AR)
NM := @$(NM)
AVRDUDE := @$(AVRDUDE)
MAKEDIR := @$(MAKEDIR)
REMOVE := @$(REMOVE)
REMOVEDIR := @$(REMOVEDIR)
COPY := @$(COPY)
endif

# Include the dependency files.
DEPFILES := $(foreach dep,$(OBJ:.o=.o.d),$(dir $(dep))dep/$(notdir $(dep)))
-include $(DEPFILES)

# Default target.
all: gcc-version build sizeafter
build: elf hex eep lss sym
# bug make 4.1 sous windows (AVR_8_bit_GNU_Toolchain_3.5.4_1709)
mkdirs:
	$(MAKEDIR) $(OBJDIRS) $(DEPDIRS)
clean: clean_list
rebuild: sizebefore clean_list build sizeafter
distclean: distclean_list clean_list

elf: mkdirs version-git.h $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET_PATH).lss
sym: $(TARGET_PATH).sym

lib: mkdirs version-git.h $(TARGET_LIB_PATH).a
cleanlib: clean_list_lib
rebuildlib:  clean_list_lib $(TARGET_LIB_PATH).a
distcleanlib: distclean_list clean_list_lib


# Create the list of directories for object and dependencies files
# bug make 4.1 sous windows (AVR_8_bit_GNU_Toolchain_3.5.4_1709)
#$(OBJ): | $(OBJDIRS) $(DEPDIRS)
$(OBJ):

$(OBJDIRS):
	$(MAKEDIR) $@

$(DEPDIRS):
	$(MAKEDIR) $@

$(AVRXLIB).a:
	@$(MAKE) BOARD=$(BOARD) OPT=$(OPT) MCU=$(MCU) -C $(AVRIOSRCDIR)/avrx lib

$(ARDUINO_LIB).a:
	@$(MAKE) BOARD=$(BOARD) OPT=$(OPT) MCU=$(MCU) -C $(AVRIOSRCDIR)/arduino lib

# Display size of file.
HEXSIZE = $(SIZE) -C --mcu=$(MCU) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) -C --mcu=$(MCU) $(TARGET).elf

sizebefore:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE); $(ELFSIZE); 2>/dev/null; fi

sizeafter:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE); $(ELFSIZE); 2>/dev/null; fi

gcc-version:
#	@echo AVRIO_TOOLS_PATH=$(AVRIO_TOOLS_PATH)
	@avr-gcc --version
#	@echo PATH="${PATH}"
#	@echo AVRSTUDIO7DIR="${AVRSTUDIO7DIR}"
  
ifneq ($(F_CPU_CUSTOM),)
	@echo "<WARNING> F_CPU defined to custom value: $(F_CPU_CUSTOM) Hz"
endif
ifneq ($(MCU_CUSTOM),)
	@echo "<WARNING>  MCU defined to custom value: $(MCU_CUSTOM)"
endif

version-git.h:
ifeq ($(GIT_VERSION),ON)
	@$(AVRIO_TOPDIR)/script/version.sh $@
endif

version-git.mk:
ifeq ($(GIT_VERSION),ON)
	@$(AVRIO_TOPDIR)/script/version.sh $@
endif

har: $(TARGET).hex version-git.mk
ifeq ($(GIT_VERSION),ON)
	$(COPY) $< $(TARGET)-$(subst -,.,$(shell cat version-git.mk)).hex
endif


# Program the device.
program: $(TARGET).hex $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)

# Program the fuses device.
fuse:
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FUSES)

# Program the locks device.
lock:
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_LOCKS)

# Generate avr-gdb config/init file which does the following:
#     define the reset signal, load the target file, connect to target, and set
#     a breakpoint at main().
gdb-config:
	$(REMOVE) $(GDBINIT_FILE)
	@echo define reset >> $(GDBINIT_FILE)
	@echo SIGNAL SIGHUP >> $(GDBINIT_FILE)
	@echo end >> $(GDBINIT_FILE)
	@echo file $(TARGET).elf >> $(GDBINIT_FILE)
	@echo set remotetimeout 10 >> $(GDBINIT_FILE)
	@echo target remote $(DEBUG_HOST):$(DEBUG_PORT)  >> $(GDBINIT_FILE)
ifeq ($(DEBUG_BACKEND),simulavr)
	@echo load  >> $(GDBINIT_FILE)
endif
	@echo break main >> $(GDBINIT_FILE)

ifeq ($(OS),Linux) 
NEWSHWIN=x-terminal-emulator -e 
else
NEWSHWIN=$(WINSHELL) /c start
PAUSE=$(WINSHELL) /c pause
endif

debug-ice: gdb-config $(TARGET).elf
ifeq ($(DEBUG_BACKEND), avarice)
	@echo Starting AVaRICE - Press enter when "waiting to connect" message displays.
# The parameter "--erase --program -f*.elf" are obsolete and not used any 
# more, because avr-gdb can do :
# avarice -I --jtag usb -3 -B1800khz  :4242
# "-I" skips interrupts which make debugging easier while a timer is running
# https://sourceforge.net/p/avarice/mailman/message/31557561/
#	$(NEWSHWIN) avarice $(AVARICE_OPT) --jtag $(JTAG_DEV) --erase --program --file $(TARGET).elf $(AVARICE_BITRATE) $(DEBUG_HOST):$(DEBUG_PORT)
	$(NEWSHWIN) avarice $(AVARICE_OPT) --jtag $(JTAG_DEV) $(AVARICE_BITRATE) $(DEBUG_HOST):$(DEBUG_PORT)
	@$(PAUSE)
else
	@$(NEWSHWIN) simulavr --gdbserver --device $(MCU) --clock-freq $(DEBUG_MFREQ) --port $(DEBUG_PORT)
endif

debug: debug-ice
ifeq ($(DEBUG_UI),cgdb)
	@$(NEWSHWIN) cgdb -d avr-gdb -- --command=$(GDBINIT_FILE)
else
	@$(NEWSHWIN) avr-$(DEBUG_UI) --command=$(GDBINIT_FILE)
endif

# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT = $(OBJCOPY) --debugging
COFFCONVERT += --change-section-address .data-0x800000
COFFCONVERT += --change-section-address .bss-0x800000
COFFCONVERT += --change-section-address .noinit-0x800000
COFFCONVERT += --change-section-address .eeprom-0x810000

coff: $(TARGET).elf
	@echo "$(MSG_COFF) $(TARGET).cof"
	@$(COFFCONVERT) -O coff-avr $< $(TARGET).cof

extcoff: $(TARGET).elf
	@echo "$(MSG_EXTENDED_COFF) $(TARGET).cof"
	@$(COFFCONVERT) -O coff-ext-avr $< $(TARGET).cof

# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	@echo "$(MSG_FLASH) $@"
	$(OBJCOPY) -O $(FORMAT) -R .eeprom -R .fuse -R .lock -R .signature $< $@

%.eep: %.elf
	@echo "$(MSG_EEPROM) $@"
	@-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0

# Create extended listing file from ELF output file.
%.lss: %.elf
	@echo "$(MSG_EXTENDED_LISTING) $@"
	$(OBJDUMP) -h -S -z $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo "$(MSG_SYMBOL_TABLE) $@"
	$(NM) -n $< > $@

# Create library from object files.
.SECONDARY : $(TARGET_LIB_PATH).a
.PRECIOUS : $(OBJ)
%.a: $(OBJ)
	@echo "$(MSG_CREATING_LIBRARY) $@"
	$(AR) $@ $(OBJ)

# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ) $(AVRXLIB_TARGET) $(ARDUINO_LIBTARGET)
	@echo "$(MSG_LINKING) $@"
	$(CC) $(LD_CFLAGS) $^ --output $@ $(LDFLAGS)
ifeq ($(GIT_VERSION),ON)
	@test -s .version || $(REMOVE) version-git.h .version
endif

# Compile: create object files from C source files.
$(OBJDIR)/%.o : %.c Makefile
	@echo "$(MSG_COMPILING) $<"
	$(CC) -c $(ALL_CFLAGS) $< -o $@

# Compile: create object files from C++ source files.
$(OBJDIR)/%.o : %.cpp Makefile
	@echo "$(MSG_COMPILING_CPP) $<"
	$(CC) -c $(ALL_CPPFLAGS) $< -o $@


# Compile: create assembler files from C source files.
%.s : %.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Compile: create assembler files from C++ source files.
%.s : %.cpp
	$(CC) -S $(ALL_CPPFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
$(OBJDIR)/%.o : %.S Makefile
	@echo "$(MSG_ASSEMBLING) $<"
	$(CC) -c $(ALL_ASFLAGS) $< -o $@


# Create preprocessed source for use in sending a bug report.
%.i : %.c
	$(CC) -E -mmcu=$(MCU) -I. $(CFLAGS) $< -o $@

clean_list_lib:
	@echo "$(MSG_CLEANING) $(TARGET)"
	$(REMOVE) $(TARGET_LIB_PATH).a
	@$(REMOVEDIR) $(DESTDIR)

clean_list:
	@echo "$(MSG_CLEANING) $(TARGET)"
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).eep
	$(REMOVE) $(TARGET).cof
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).pnps
	$(REMOVE) $(TARGET).aws
	$(REMOVE) $(TARGET_PATH).map
	$(REMOVE) $(TARGET_PATH).sym
	$(REMOVE) $(TARGET_PATH).lss
	$(REMOVEDIR) $(DEPDIRS)
	$(REMOVEDIR) $(OBJDIRS)
	$(REMOVEDIR) $(DESTDIR)

distclean_list:
	$(REMOVE) *.bak
	$(REMOVE) *~
	$(REMOVE) __avr_gdbinit
	$(REMOVE)  AVRStudioConversionLog.xslt
	$(REMOVE) $(TARGET).componentinfo.xml
	$(REMOVE) ConversionLog-*.xml
	$(REMOVEDIR) Debug
	$(REMOVEDIR) bin
  
ifeq ($(GIT_VERSION),ON)
	$(REMOVE) version-git.h version-git.mk .version
endif

# Listing of phony targets.
.PHONY : all finish sizebefore sizeafter \
build version-git rebuild lib elf hex eep lss sym coff extcoff \
clean distclean cleanlib clean_list clean_list_lib program debug gdb-config \
debug-ice mkdirs

# Make docs pictures
FIG2DEV                 = fig2dev

dox: eps png pdf

eps: $(TARGET_PATH).eps
png: $(TARGET_PATH).png
pdf: $(TARGET_PATH).pdf

%.eps: %.fig
	@$(FIG2DEV) -L eps $< $@

%.pdf: %.fig
	@$(FIG2DEV) -L pdf $< $@

%.png: %.fig
	@$(FIG2DEV) -L png $< $@
