MKDIR      := mkdir -p
RMDIR      := rm -rf

CC         := gcc

ROOT_DIR   := ../../..
PLATFORM   := jetson

include $(ROOT_DIR)/submodules/iRfe/makelist.mke

BIN_DIR    := ./bin
OBJ_DIR    := ./obj
PTFRM_DIR  := $(ROOT_DIR)/submodules/platform-jetson/src
PMIC_DIR   := $(ROOT_DIR)/pmic_power
BS_DIR     := $(ROOT_DIR)/boardspecific

APP_DIR    := $(subst $(realpath $(ROOT_DIR))/,,$(realpath .))
APP_SRCS   := $(wildcard ./*.c)
APP_OBJS   := $(patsubst ./%,$(OBJ_DIR)/$(APP_DIR)/%.o,$(APP_SRCS))

PTFRM_SRCS := $(wildcard $(PTFRM_DIR)/*.c)
BS_SRCS    := $(wildcard $(BS_DIR)/*.c)

PMIC_SRCS  := $(wildcard $(PMIC_DIR)/*.c)


SRCS       := $(SRC_SRCS) $(WRP_SRCS) $(FW_SRC) $(PTFRM_SRCS) $(PMIC_SRCS) $(BS_SRCS) $(C_FILES)
OBJS       := $(patsubst $(ROOT_DIR)/%,$(OBJ_DIR)/%.o,$(SRCS))

BIN        := $(BIN_DIR)/$(APP_NAME)

DEFINES      += $(CTRX_DEF)
ifdef DEBUG
    DEFINES += DEBUG=$(DEBUG)
endif
DEFINE_FLAGS := $(shell for DEF in $(DEFINES); do echo -D$$DEF; done)

CFLAGS     += -Wall $(DEFINE_FLAGS)

IRFE_INCLUDES := $(addprefix -I, $(INCLUDE_DIRS))

IFLAGS     := -I. -I$(PTFRM_DIR)/ -I$(BS_DIR)/ $(IRFE_INCLUDES) -I$(ROOT_DIR)/tests/unit_tests/support/ -I$(PMIC_DIR)

LDFLAGS    := -lm -lgpiod -li2c

.PHONY: all info clean

all: $(BIN)

$(BIN): $(APP_OBJS) $(OBJS)
	@$(MKDIR) $(@D)
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.c.o: $(ROOT_DIR)/%.c
	@$(MKDIR) $(@D)
	$(CC) -c $(IFLAGS) $(CFLAGS) -o $@ $<

info:
	@echo "======= MAIN ======="
	@echo $(APP_DIR)
	@echo $(APP_SRCS)
	@echo $(APP_OBJS)
	@echo "======= SOURCES ======="
	@echo $(SRCS)
	@echo "======= OBJECTS ======="
	@echo $(OBJS)
	@echo "========== CTRX_DEF ==========="
	@echo $(CTRX_DEF)
	@echo "========== CTRX_INFO ==========="
	@echo $(CHIP_TYPE)
	@echo $(CTRX_FW_DIR)
	@echo $(CTRX_DEF)
	@echo $(FW_INC_DIR)
	@echo $(FW_SRC)
	@echo "====================="

clean:
	$(RMDIR) $(BIN_DIR) $(OBJ_DIR)