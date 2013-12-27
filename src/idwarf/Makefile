# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# $Id$
#
# On command line:
#
# make all = Make all library versions.
#
# make hub = Make hub version.
#
# make hubBox = Make hubBox version.
#
# make sensor = Make sensor version.
#
# make clean = Clean out built project files.
#
# To rebuild project do "make clean" then "make all".
#----------------------------------------------------------------------------

# choose your configuration here!

# Optimization level, can be [0, 1, 2, 3, s]. 
#     0 = turn off optimization. s = optimize for size.
#     (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s

# Symbols to be removed in the chip45 object files 
HUB_SYMBOLS        = HubFindNextFreeDeviceId
HUB_SYMBOLS       += ResetDeviceIdTable
IDWARFHUB_SYMBOLS  = rfDeleteSensor
IDWARFHUB_SYMBOLS += rfGetMaximumDeviceId

#---
# hub setup 
HUB_FCPU	= 7372800
HUB_CHIP45_OBJ = main.o nvram.o protocol.o radio.o spi.o
HUB_UPGRADE_OBJ = hub.o iDwaRF_hub.o
HUB_SRC = hardware_m168.c timer_m168_t0.c fram_sram.c cypress.c
HUB_CPPSRC = 
HUB_ASRC = 
HUB_LIBS = 
# Place -D or -U options here for C sources
HUB_CDEFS = -DHUB
# Place -D or -U options here for C++ sources
HUB_CPPDEFS = -DHUB
# Place -D or -U options here for assembler sources
HUB_ASDEFS = -DHUB
# iDwaRF - version. MCU is hardcoded inside of the sourcecode.
# Value below should absolutely not be changed.
HUB_MCU   = atmega168
#-

#---
# hubBox setup
HUBBOX_FCPU	= 7372800
HUBBOX_CHIP45_OBJ = main.o nvram.o protocol.o radio.o spi.o
HUBBOX_UPGRADE_OBJ = hub.o iDwaRF_hub.o
HUBBOX_SRC = hardware_m128.c timer_m128_t0.c fram_sram.c cypress.c
HUBBOX_CPPSRC = 
HUBBOX_ASRC = 
HUBBOX_LIBS = 
# Place -D or -U options here for C sources
HUBBOX_CDEFS = -DHUB
# Place -D or -U options here for C++ sources
HUBBOX_CPPDEFS = -DHUB
# Place -D or -U options here for assembler sources
HUBBOX_ASDEFS = -DHUB
# iDwaRF - version. MCU is hardcoded inside of the sourcecode.
# Value below should absolutely not be changed.
HUBBOX_MCU   = atmega128
#-

#---
# sensor setup
SENSOR_FCPU	= 7372800
SENSOR_CHIP45_OBJ = Sensor.o iDwaRF_sensor.o main.o nvram.o protocol.o radio.o spi.o
SENSOR_SRC = hardware_m168.c timer_m168_t0.c fram_sram.c
SENSOR_CPPSRC = 
SENSOR_ASRC =
SENSOR_LIBS =
# Place -D or -U options here for C sources
SENSOR_CDEFS = -DSENSOR
# Place -D or -U options here for C++ sources
SENSOR_CPPDEFS = -DSENSOR
# Place -D or -U options here for assembler sources
SENSOR_ASDEFS = -DSENSOR
# iDwaRF - version. MCU is hardcoded inside of the sourcecode.
# Value below should absolutely not be changed.
SENSOR_MCU   = atmega168
#-

# Debugging format.
#     Native formats for AVR-GCC's -g are dwarf-2 [default] or stabs.
#     AVR Studio 4.10 requires dwarf-2.
#     AVR [Extended] COFF format requires stabs, plus an avr-objcopy run.
DEBUG = dwarf-2

# Directory where libraries will be placed.
DESTDIR = lib

# Directory where the object files will be placed.
OBJDIR = obj

# Directory where the dependency files will be placed.
DEPDIR = .dep

# Directory containing the object files of CHIP45.
CHIP45_OBJDIR = chip45

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRA_INCDIRS = 

# Compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
CSTANDARD = -std=gnu99

#============================================================================
#============================================================================
#============================================================================
# Set the operating system id: Linux,windows32,MINGW32
OS := $(firstword $(subst _, ,$(shell uname -s)))

# binaries Path
ifeq ($(BINDIR),)
BINDIR := $(realpath ../../bin)
endif

ifeq ($(OS),windows32)
# Windows: set the path to find sh
export PATH := $(subst /,\\,$(BINDIR)/win32);${PATH}
else ifeq ($(OS),MINGW32)
# MinGW: set the path to find sh
export PATH := $(subst /,\\,$(BINDIR)/win32);${PATH}
endif

HUB_OBJDIR = $(OBJDIR)/hub
HUBBOX_OBJDIR = $(OBJDIR)/hubBox
SENSOR_OBJDIR = $(OBJDIR)/sensor
HUB_DEPDIR = $(DEPDIR)/hub
HUBBOX_DEPDIR = $(DEPDIR)/hubBox
SENSOR_DEPDIR = $(DEPDIR)/sensor

#---------------- Compiler Options C ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS = -g$(DEBUG)
CFLAGS += -O$(OPT)
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -Wall
CFLAGS += -Wstrict-prototypes
#CFLAGS += -mshort-calls
#CFLAGS += -fno-unit-at-a-time
#CFLAGS += -Wundef
#CFLAGS += -Wunreachable-code
#CFLAGS += -Wsign-compare
CFLAGS += $(patsubst %,-I%,$(EXTRA_INCDIRS))
CFLAGS += $(CSTANDARD)
# iDwaRF addon: these defines should be available in the code, too
# CFLAGS += -D$(PRODUCT) -D$(VERSION)
# no more pointer signedness warnings
CFLAGS += -Wno-pointer-sign
CFLAGS += -Wno-strict-aliasing
CFLAGS += -I. 

HUB_CFLAGS = -mmcu=$(HUB_MCU) $(HUB_CDEFS) -DF_CPU=$(HUB_FCPU)UL $(CFLAGS) -Wa,-adhlns=$(<:%.c=$(HUB_OBJDIR)/%.lst) -MMD -MP -MF $(HUB_DEPDIR)/$(@F).d
HUBBOX_CFLAGS = -mmcu=$(HUBBOX_MCU) $(HUBBOX_CDEFS) -DF_CPU=$(HUBBOX_FCPU)UL $(CFLAGS) -Wa,-adhlns=$(<:%.c=$(HUBBOX_OBJDIR)/%.lst) -MMD -MP -MF $(HUBBOX_DEPDIR)/$(@F).d 
SENSOR_CFLAGS = -mmcu=$(SENSOR_MCU) $(SENSOR_CDEFS) -DF_CPU=$(SENSOR_FCPU)UL $(CFLAGS) -Wa,-adhlns=$(<:%.c=$(SENSOR_OBJDIR)/%.lst) -MMD -MP -MF $(SENSOR_DEPDIR)/$(@F).d

#---------------- Compiler Options C++ ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CPPFLAGS = -g$(DEBUG)
CPPFLAGS += -O$(OPT)
CPPFLAGS += -funsigned-char
CPPFLAGS += -funsigned-bitfields
CPPFLAGS += -fpack-struct
CPPFLAGS += -fshort-enums
CPPFLAGS += -fno-exceptions
CPPFLAGS += -Wall
CFLAGS += -Wundef
#CPPFLAGS += -mshort-calls
#CPPFLAGS += -fno-unit-at-a-time
#CPPFLAGS += -Wstrict-prototypes
#CPPFLAGS += -Wunreachable-code
#CPPFLAGS += -Wsign-compare
CPPFLAGS += $(patsubst %,-I%,$(EXTRA_INCDIRS))
CPPFLAGS += -I. -x c++

HUB_CPPFLAGS    = -mmcu=$(HUB_MCU) $(HUB_CPPDEFS) -DF_CPU=$(HUB_FCPU)UL $(CPPFLAGS) -Wa,-adhlns=$(<:%.cpp=$(HUB_OBJDIR)/%.lst) -MMD -MP -MF $(HUB_DEPDIR)/$(@F).d
HUBBOX_CPPFLAGS = -mmcu=$(HUBBOX_MCU) $(HUBBOX_CPPDEFS) -DF_CPU=$(HUBBOX_FCPU)UL $(CPPFLAGS) -Wa,-adhlns=$(<:%.cpp=$(HUBBOX_OBJDIR)/%.lst) -MMD -MP -MF $(HUBBOX_DEPDIR)/$(@F).d 
SENSOR_CPPFLAGS = -mmcu=$(SENSOR_MCU) $(SENSOR_CPPDEFS) -DF_CPU=$(SENSOR_FCPU)UL $(CPPFLAGS) -Wa,-adhlns=$(<:%.cpp=$(SENSOR_OBJDIR)/%.lst) -MMD -MP -MF $(SENSOR_DEPDIR)/$(@F).d

#---------------- Assembler Options ----------------
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -ahlms:    create listing
#  -gstabs:   have the assembler create line number information; note that
#             for use in COFF files, additional information about filenames
#             and function names needs to be present in the assembler source
#             files -- see avr-libc docs [FIXME: not yet described there]
#  -listing-cont-lines: Sets the maximum number of continuation lines of hex 
#       dump that will be displayed for a given single line of source input.
ASFLAGS  = -I. -x assembler-with-cpp

HUB_ASFLAGS = -mmcu=$(HUB_MCU) $(HUB_ASDEFS) -DF_CPU=$(HUB_FCPU)UL $(ASFLAGS) -Wa,-adhlns=$(<:.S=$(HUB_OBJDIR)/%.lst),-gstabs,--listing-cont-lines=100
HUBBOX_ASFLAGS = -mmcu=$(HUBBOX_MCU) $(HUBBOX_ASDEFS) -DF_CPU=$(HUBBOX_FCPU)UL $(ASFLAGS) -Wa,-adhlns=$(<:.S=$(HUBBOX_OBJDIR)/%.lst),-gstabs,--listing-cont-lines=100
SENSOR_ASFLAGS = -mmcu=$(SENSOR_MCU) $(SENSOR_ASDEFS) -DF_CPU=$(SENSOR_FCPU)UL $(ASFLAGS) -Wa,-adhlns=$(<:.S=$(SENSOR_OBJDIR)/%.lst),-gstabs,--listing-cont-lines=100

#============================================================================

# Define all object files.
HUB_ORIGINAL_OBJ = $(patsubst %,$(CHIP45_OBJDIR)/hub/%,$(HUB_CHIP45_OBJ))
HUBBOX_ORIGINAL_OBJ = $(patsubst %,$(CHIP45_OBJDIR)/hubBox/%,$(HUBBOX_CHIP45_OBJ))
SENSOR_ORIGINAL_OBJ = $(patsubst %,$(CHIP45_OBJDIR)/sensor/%,$(SENSOR_CHIP45_OBJ))

HUB_OBJ = $(HUB_ORIGINAL_OBJ) $(patsubst %,$(HUB_OBJDIR)/%,$(HUB_UPGRADE_OBJ)) $(HUB_SRC:%.c=$(HUB_OBJDIR)/%.o) $(HUB_CPPSRC:%.cpp=$(HUB_OBJDIR)/%.o) $(HUB_ASRC:%.S=$(HUB_OBJDIR)/%.o) 
HUBBOX_OBJ = $(HUBBOX_ORIGINAL_OBJ) $(patsubst %,$(HUBBOX_OBJDIR)/%,$(HUBBOX_UPGRADE_OBJ)) $(HUBBOX_SRC:%.c=$(HUBBOX_OBJDIR)/%.o) $(HUBBOX_CPPSRC:%.cpp=$(HUBBOX_OBJDIR)/%.o) $(HUBBOX_ASRC:%.S=$(HUBBOX_OBJDIR)/%.o) 
SENSOR_OBJ = $(SENSOR_ORIGINAL_OBJ) $(SENSOR_SRC:%.c=$(SENSOR_OBJDIR)/%.o) $(SENSOR_CPPSRC:%.cpp=$(SENSOR_OBJDIR)/%.o) $(SENSOR_ASRC:%.S=$(SENSOR_OBJDIR)/%.o) 


# Define programs and commands.
SHELL = sh
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
AR = avr-ar rcs
EXTRACT = avr-ar x
NM = avr-nm
STRIP = avr-strip
AVRDUDE = avrdude
REMOVE = rm -f
REMOVEDIR = rm -rf
COPY = cp
MAKEDIR = mkdir -p

# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_COFF = Converting to AVR COFF:
MSG_EXTENDED_COFF = Converting to AVR Extended COFF:
MSG_FLASH = Creating load file for Flash:
MSG_EEPROM = Creating load file for EEPROM:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling C:
MSG_COMPILING_CPP = Compiling C++:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:
MSG_CREATING_LIBRARY = Creating library:
MSG_STRIP_OBJECT = Removing unwanted symbols:

# Default target.
all: begin gccversion build end
build: hub hubBox sensor
rebuild: begin gccversion clean_list build end

# for convenience with AVRStudio ...
idwarf.elf: begin gccversion hub hubBox sensor end
	touch idwarf.elf

#build: lib
hub: $(DESTDIR)/libiDwaRF_hub.a
hubBox: $(DESTDIR)/libiDwaRF_hubBox.a
sensor: $(DESTDIR)/libiDwaRF_sensor.a

# Eye candy.
# AVR Studio 3.x does not check make's exit code but relies on
# the following magic strings to be generated by the compile job.
begin:
	@echo $(MSG_BEGIN)
#	@echo BINDIR=$(BINDIR)
#	@echo PATH=${PATH}
#	@echo HUB_OBJ=$(HUB_OBJ)
#	@echo HUBBOX_OBJ=$(HUBBOX_OBJ)

end:
	@echo $(MSG_END)

# Display compiler version information.
gccversion : 
	@$(CC) --version

# Create libraries from object files.
$(DESTDIR)/libiDwaRF_hub.a: $(HUB_OBJ)
	@echo $(MSG_CREATING_LIBRARY) $@
	$(AR) $@ $(HUB_OBJ)
	$(STRIP) -g $@
	
$(DESTDIR)/libiDwaRF_hubBox.a: $(HUBBOX_OBJ)
	@echo $(MSG_CREATING_LIBRARY) $@
	$(AR) $@ $(HUBBOX_OBJ)
	$(STRIP) -g $@
	
$(DESTDIR)/libiDwaRF_sensor.a: $(SENSOR_OBJ)
	@echo $(MSG_CREATING_LIBRARY) $@
	$(AR) $@ $(SENSOR_OBJ)
	$(STRIP) -g $@

# extract original objects from original libs
$(CHIP45_OBJDIR)/hub/%.o: $(CHIP45_OBJDIR)/libiDwaRF_hub.a
	(cd $(CHIP45_OBJDIR)/hub; $(EXTRACT) ../$(notdir $<) $(notdir $@); cd -)
	
	
$(CHIP45_OBJDIR)/hubBox/%.o: $(CHIP45_OBJDIR)/libiDwaRF_hubBox.a
	(cd $(CHIP45_OBJDIR)/hubBox; $(EXTRACT) ../$(notdir $<) $(notdir $@); cd -)
	
$(CHIP45_OBJDIR)/sensor/%.o: $(CHIP45_OBJDIR)/libiDwaRF_sensor.a
	(cd $(CHIP45_OBJDIR)/sensor; $(EXTRACT) ../$(notdir $<) $(notdir $@); cd -)

# strip unwanted symbols
$(HUB_OBJDIR)/hub.o: $(CHIP45_OBJDIR)/hub/hub.o
	@echo $(MSG_STRIP_OBJECT) $<
	$(STRIP) $(patsubst %,-N %,$(HUB_SYMBOLS)) -o $@ $<

$(HUB_OBJDIR)/iDwaRF_hub.o: $(CHIP45_OBJDIR)/hub/iDwaRF_hub.o
	@echo $(MSG_STRIP_OBJECT) $<
	$(STRIP) $(patsubst %,-N %,$(IDWARFHUB_SYMBOLS)) -o $@ $<

$(HUBBOX_OBJDIR)/hub.o: $(CHIP45_OBJDIR)/hubBox/hub.o
	@echo $(MSG_STRIP_OBJECT) $<
	$(STRIP) $(patsubst %,-N %,$(HUB_SYMBOLS)) -o $@ $<

$(HUBBOX_OBJDIR)/iDwaRF_hub.o: $(CHIP45_OBJDIR)/hubBox/iDwaRF_hub.o
	@echo $(MSG_STRIP_OBJECT) $<
	$(STRIP) $(patsubst %,-N %,$(IDWARFHUB_SYMBOLS)) -o $@ $<

# Compile: create object files from C source files.
$(HUB_OBJDIR)/%.o : %.c
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(HUB_CFLAGS) $< -o $@ 

$(HUBBOX_OBJDIR)/%.o : %.c
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(HUBBOX_CFLAGS) $< -o $@ 

$(SENSOR_OBJDIR)/%.o : %.c
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(SENSOR_CFLAGS) $< -o $@ 

# Compile: create object files from C++ source files.
$(HUB_OBJDIR)/%.o : %.cpp
	@echo $(MSG_COMPILING_CPP) $<
	$(CC) -c $(HUB_CPPFLAGS) $< -o $@ 
	
$(HUBBOX_OBJDIR)/%.o : %.cpp
	@echo $(MSG_COMPILING_CPP) $<
	$(CC) -c $(HUBBOX_CPPFLAGS) $< -o $@ 

$(SENSOR_OBJDIR)/%.o : %.cpp
	@echo $(MSG_COMPILING_CPP) $<
	$(CC) -c $(SENSOR_CPPFLAGS) $< -o $@ 

# Assemble: create object files from assembler source files.
$(HUB_OBJDIR)/%.o : %.S
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(HUB_ASFLAGS) $< -o $@

$(HUBBOX_OBJDIR)/%.o : %.S
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(HUBBOX_ASFLAGS) $< -o $@

$(SENSOR_OBJDIR)/%.o : %.S
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(SENSOR_ASFLAGS) $< -o $@

# Target: clean project.
clean: begin clean_list end
distclean: begin distclean_list clean_list end

clean_list :
	@echo $(MSG_CLEANING)
	$(REMOVE) $(DESTDIR)/*.a
	$(REMOVEDIR) $(DEPDIR)
	$(REMOVEDIR) $(OBJDIR)
	$(REMOVEDIR) $(CHIP45_OBJDIR)/hub
	$(REMOVEDIR) $(CHIP45_OBJDIR)/hubBox
	$(REMOVEDIR) $(CHIP45_OBJDIR)/sensor

distclean_list :
	$(REMOVE) *.bak
	$(REMOVE) *~

$(HUB_OBJ): | $(CHIP45_OBJDIR)/hub $(HUB_OBJDIR) $(HUB_DEPDIR) $(DESTDIR)
$(HUBBOX_OBJ): | $(CHIP45_OBJDIR)/hubBox $(HUBBOX_OBJDIR) $(HUBBOX_DEPDIR) $(DESTDIR)
$(SENSOR_OBJ): | $(CHIP45_OBJDIR)/sensor $(SENSOR_OBJDIR) $(SENSOR_DEPDIR) $(DESTDIR)

$(HUB_OBJDIR):
	-$(MAKEDIR) $@
$(HUBBOX_OBJDIR):
	-$(MAKEDIR) $@
$(SENSOR_OBJDIR):
	-$(MAKEDIR) $@
$(HUB_DEPDIR):
	-$(MAKEDIR) $@
$(HUBBOX_DEPDIR):
	-$(MAKEDIR) $@
$(SENSOR_DEPDIR):
	-$(MAKEDIR) $@
$(DESTDIR):
	-$(MAKEDIR) $@
$(CHIP45_OBJDIR)/hub:
	-$(MAKEDIR) $@
$(CHIP45_OBJDIR)/hubBox:
	-$(MAKEDIR) $@
$(CHIP45_OBJDIR)/sensor:
	-$(MAKEDIR) $@

# Include the dependency files.
-include $(wildcard $(HUB_DEPDIR)/*)
-include $(wildcard $(HUBBOX_DEPDIR)/*)
-include $(wildcard $(SENSOR_DEPDIR)/*)

# Listing of phony targets.
.PHONY : all begin build rebuild finish end gccversion hub hubBox sensor clean clean_list
