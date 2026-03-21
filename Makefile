#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

TOPDIR ?= $(CURDIR)

# This contains the rest of the make rules needed to build this project
include $(DEVKITARM)/3ds_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing header files
#---------------------------------------------------------------------------------
TARGET		:=	$(notdir $(CURDIR))
BUILD		:=	build

include $(TOPDIR)/sources.mk

INCLUDES	:=	include \
				include/vendor/MK7-Memory/include \
				include/vendor/MK7-Memory/vendor/lms/Include \
				include/vendor/MK7-Memory/vendor/nnheaders/include \
				include/vendor/MK7-Memory/vendor/nw4c/include \
				include/vendor/MK7-Memory/vendor/sead/include \
				include/vendor/MK7-Memory/vendor/libc/include
				
PATCH		:= patch.py

ADDRESSES_DIR := addresses

#---------------------------------------------------------------------------------
# game version selection
#---------------------------------------------------------------------------------

# Defaults to E3 2010
VERSION ?= e3_2010

include $(TOPDIR)/versions.mk

ADDRESSES_FILE := $(TOPDIR)/$(ADDRESSES_DIR)/$(VERSION)/addresses.ld
SYMBOLS_FILE   := $(TOPDIR)/$(ADDRESSES_DIR)/$(VERSION)/symbols.ld

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------

LINKER_SCRIPT 	:= $(TOPDIR)/mk7.ld

ARCH	:=	-march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft -mfpu=vfpv2

CFLAGS	:=	-g -Wall -O2 -mword-relocations \
			-ffunction-sections -fomit-frame-pointer \
			$(ARCH) -DGAME_VERSION=$(VERSION_SYM)

# -DNNSDK is for sead
CFLAGS	+=	$(INCLUDE) -D__3DS__ -DNNSDK

CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++23 -Wno-invalid-offsetof \
							-Wno-array-bounds -fno-unwind-tables -fno-asynchronous-unwind-tables

ASFLAGS := -g $(ARCH) -DGAME_VERSION=$(VERSION_SYM) $(INCLUDE)
LDFLAGS	=	-g $(ARCH) -Wl,-Map,$(notdir $*.map) \
			-T $(LINKER_SCRIPT) \
			-T $(ADDRESSES_FILE) \
			-T $(SYMBOLS_FILE) \
			-Wl,--no-demangle \
			-DGAME_VERSION=$(VERSION_SYM)

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:= 


#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)
export TOPDIR	:=	$(CURDIR)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES_SOURCES 	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export OFILES := $(OFILES_SOURCES)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

export _3DSXDEPS	:=	$(if $(NO_SMDH),,$(OUTPUT).smdh)

.PHONY: all clean $(BUILD)

#---------------------------------------------------------------------------------
all: $(BUILD)

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	$(MAKE) -C include/vendor/MK7-Memory
	@$(MAKE) -C $(BUILD) -f $(CURDIR)/Makefile
	@if python3 $(PATCH) $(OUTPUT).elf $(VERSION); then \
		echo "created code.ips" ; \
	else \
		python $(PATCH) $(OUTPUT).elf $(VERSION); \
		echo "created code.ips" ; \
	fi

ifneq ($(DEPSDIR),$(BUILD))
$(DEPSDIR):
	@mkdir -p $@
endif

#---------------------------------------------------------------------------------

clean:
	@echo clean ...
	@$(MAKE) -C include/vendor/MK7-Memory clean
	@rm -fr $(BUILD) $(TARGET).elf code.ips exheader.bin $(OUTPUT).elf

else

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).elf : $(OFILES) $(LINKER_SCRIPT) $(ADDRESSES_FILE) $(SYMBOLS_FILE)

-include $(DEPSDIR)/*.d

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
