include ../../py/mkenv.mk

CROSS = 0

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# MicroPython feature configurations
MICROPY_ROM_TEXT_COMPRESSION ?= 1

# include py core make definitions
include $(TOP)/py/py.mk

CROSS_COMPILE ?= arm-none-eabi-
AS	?= arm-none-eabi-as

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)

DFU = $(TOP)/tools/dfu.py
PYDFU = $(TOP)/tools/pydfu.py
CFLAGS_CORTEX_M3 = -mtune=cortex-m3 -mcpu=cortex-m3
CFLAGS = $(INC) -Wall -std=gnu99 -nostdlib  $(CFLAGS_CORTEX_M3) $(COPT) -lm
LDFLAGS = -nostdlib -T LPC17xx.ld -Map=$@.map --cref --gc-sections


CFLAGS += -g -Os -DNDEBUG
CFLAGS += -fdata-sections -ffunction-sections

LIBS = 

SRC_C = \
	main.c \
	mhalport.c \
	lib/utils/printf.c \
	lib/utils/stdout_helpers.c \
	lib/utils/pyexec.c \
	lib/mp-readline/readline.c \
	$(BUILD)/_frozen_mpy.c \
	system_LPC17xx.c \
	core_cm3.c \
	usbcore.c \
	usbdesc.c \
	usbhw.c \
	usbuser.c \
	cdcuser.c \
	serial.c \
	led/lib_led.c \
	led/funct_led.c \
	delay.c \
	vcomdemo.c \
	
SRC_S = startup_LPC17xx.s \


SRC_C += lib/libc/string0.c


OBJ = $(PY_CORE_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o)) $(addprefix $(BUILD)/, $(SRC_S:.s=.o))
all: $(BUILD)/firmware.dfu

$(BUILD)/_frozen_mpy.c: frozentest.mpy $(BUILD)/genhdr/qstrdefs.generated.h
	$(ECHO) "MISC freezing bytecode"
	$(Q)$(TOP)/tools/mpy-tool.py -f -q $(BUILD)/genhdr/qstrdefs.preprocessed.h -mlongint-impl=none $< > $@

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(Q)$(SIZE) $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(Q)$(OBJCOPY) -O binary -j .isr_vector -j .text -j .data $^ $(BUILD)/firmware.bin

define GENERATE_HEX
	$(ECHO) "GEN $(1)"
	$(Q)$(OBJCOPY) -O ihex $(2) $(1)
endef

$(BUILD)/firmware.hex: $(BUILD)/firmware.elf
	$(call GENERATE_HEX,$@,$^)

$(BUILD)/firmware.dfu: $(BUILD)/firmware.bin $(BUILD)/firmware.hex
	$(ECHO) "Create $@"
	$(Q)$(PYTHON) $(DFU) -b 0x08000000:$(BUILD)/firmware.bin $@
	$(Q)$(PYTHON) $(DFU) -b 0x08000000:$(BUILD)/firmware.hex $@

# Run emulation build on a POSIX system with suitable terminal settings
run:
	stty raw opost -echo
	build/firmware.elf
	@echo Resetting terminal...
# This sleep is useful to spot segfaults
	sleep 1
	reset

test: $(BUILD)/firmware.elf
	$(Q)/bin/echo -e "print('hello world!', list(x+1 for x in range(10)), end='eol\\\\n')\\r\\n\\x04" | $(BUILD)/firmware.elf | tail -n2 | grep "^hello world! \\[1, 2, 3, 4, 5, 6, 7, 8, 9, 10\\]eol"

include $(TOP)/py/mkrules.mk

