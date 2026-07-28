# Project Name
TARGET = plaits

# The Patch SM's internal flash is limited to 128 KiB. Load the application
# through the Daisy bootloader so larger Plaits engines can run from SRAM.
APP_TYPE = BOOT_SRAM

# Library Locations
LIBDAISY_DIR = libDaisy
STMLIB_DIR = eurorack/stmlib
LIBDAISY_ARCHIVE = $(LIBDAISY_DIR)/build/libdaisy.a

# Sources
CPP_SOURCES = Plaits.cpp 
CC_SOURCES += $(wildcard eurorack/plaits/dsp/*.cc)
CC_SOURCES += $(wildcard eurorack/plaits/dsp/engine/*.cc)
CC_SOURCES += $(wildcard eurorack/plaits/dsp/speech/*.cc)
CC_SOURCES += $(wildcard eurorack/plaits/dsp/physical_modelling/*.cc)
CC_SOURCES += eurorack/plaits/resources.cc


CC_SOURCES += $(STMLIB_DIR)/dsp/units.cc \
$(STMLIB_DIR)/utils/random.cc \
# cv_scaler.cc \

C_INCLUDES += \
-I. \
-Iresources \
-Ieurorack \
-I../..

# Can't actually add to CFLAGS.. due to libDaisy stuff
# silences warning from stmlib JOIN macros
C_INCLUDES += -Wno-unused-local-typedefs
OPT = -Os -s

# Core location, and generic makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

# Extend libDaisy's build rules with Mutable Instruments' .cc sources.
CC_OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(CC_SOURCES:.cc=.o)))
OBJECTS += $(CC_OBJECTS)
vpath %.cc $(sort $(dir $(CC_SOURCES)))

$(BUILD_DIR)/%.o: %.cc Makefile | $(BUILD_DIR)
	$(CXX) -c $(CPPFLAGS) $(CPP_STANDARD) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cc=.lst)) $< -o $@

# GCC requires interrupt handlers to avoid floating-point registers.
$(BUILD_DIR)/startup_stm32h750xx.o: CFLAGS += -mgeneral-regs-only

$(LIBDAISY_ARCHIVE):
	$(MAKE) -C $(LIBDAISY_DIR)

# Add project-specific prerequisites without replacing libDaisy's link recipe.
$(BUILD_DIR)/$(TARGET).elf: $(CC_OBJECTS) $(LIBDAISY_ARCHIVE)
