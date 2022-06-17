PREFIX := "$(GNU_INSTALL_ROOT)"arm-none-eabi-
PROJ_DIR := ./
OUTPUT_DIR := _build
SRC_DIR := src
LINKER_SCRIPT:= linker.ld

# Source files common to all targets
SRC_FILES += \
  startup.c \
  main.c \
  printf.c \
  uart.c

OBJ_FILES = $(SRC_FILES:.c=.o)

# Include folders common to all targets
INC_FOLDERS += \
  $(PROJ_DIR)/include \
  $(PROJ_DIR)/nRF5_SDK_17/modules/nrfx/hal \
  $(PROJ_DIR)/nRF5_SDK_17/components/drivers_nrf/nrf_soc_nosd \
  $(PROJ_DIR)/nRF5_SDK_17/modules/nrfx/drivers/include \
  $(PROJ_DIR)/nRF5_SDK_17/components/toolchain/cmsis/include \
  $(PROJ_DIR)/nRF5_SDK_17/components/libraries/delay \
  $(PROJ_DIR)/nRF5_SDK_17/components \
  $(PROJ_DIR)/nRF5_SDK_17/integration/nrfx \
  $(PROJ_DIR)/nRF5_SDK_17/components/libraries/util \
  $(PROJ_DIR)/nRF5_SDK_17/modules/nrfx/mdk \
  $(PROJ_DIR)/nRF5_SDK_17/modules/nrfx \
  $(PROJ_DIR)/nRF5_SDK_17/integration/nrfx/legacy \
  $(PROJ_DIR)/nRF5_SDK_17/config/nrf52840/config

INCLUDES = $(INC_FOLDERS:%=-I%)
CFLAGS += -g3 -O0
CFLAGS += ${INCLUDES}
CFLAGS += -DNRF52840_XXAA
CFLAGS += -DARM_MATH_CM4
CFLAGS += -Wall
CFLAGS += -fno-builtin
CFLAGS += -mthumb
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mabi=aapcs
CFLAGS += -mfloat-abi=hard
CFLAGS += -mfpu=fpv4-sp-d16
CFLAGS += -fsingle-precision-constant

LDFLAGS += ${CFLAGS}
LDFLAGS += -nostartfiles
LDFLAGS += -T$(LINKER_SCRIPT)

# let linker dump unused sections
LDFLAGS += -Wl,--gc-sections,-Map=${OUTPUT_DIR}/build.map
# use newlib in nano version
LDFLAGS += --specs=nano.specs

LIB_FILES += -lc -lnosys -lm

all: ${OUTPUT_DIR}/build.hex ${OUTPUT_DIR}/build.elf

${OUTPUT_DIR}/%.o: ${SRC_DIR}/%.c
	@${PREFIX}gcc ${CFLAGS} -c $< -o $@
	@echo "CC $<"


${OUTPUT_DIR}/build.elf: $(OBJ_FILES:%=${OUTPUT_DIR}/%)
	@${PREFIX}gcc ${LDFLAGS} $^ -o $@ ${LIB_FILES}
	@${PREFIX}size $@

${OUTPUT_DIR}/build.hex: ${OUTPUT_DIR}/build.elf
	@${PREFIX}objcopy -O ihex $< $@
	@echo "Preparing $@"

.PHONY: clean flash erase

clean:
	rm -rf _build/*

# Flash the program
flash: ${OUTPUT_DIR}/build.hex
	@echo Flashing: $<
	nrfjprog -f nrf52 --program $< --sectorerase
	nrfjprog -f nrf52 --reset

erase:
	nrfjprog -f nrf52 --eraseall