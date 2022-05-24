ifdef LINUX
unexport LINUX

CC = /usr/bin/gcc

# Create a static library
AR = ar csrv
RL = ranlib

CFLAGS += -fpic -fstack-protector-strong -Werror -Wall -Wextra -pedantic -g3 -Og -std=gnu11 -fstack-protector-all -ffunction-sections -fwrapv -Wstrict-overflow -fno-common -fdata-sections -Wmissing-include-dirs -fno-delete-null-pointer-checks -fdiagnostics-color -Wstrict-prototypes -Wold-style-definition -gz -Wformat=2 -Wformat-overflow -Wformat-truncation
ifdef DEBUG
	CFLAGS += -g
endif
ifdef TEST
CFLAGS += -DTESTING
endif
CPPFLAGS += -D__LINUX__

BIN_DIR = bin/linux-x86_64
OBJ_DIR = bin/linux-x86_64/obj_cunit
LIB_DIR = bin/linux-x86_64/lib_cunit
$(info $$BIN_DIR is [$(BIN_DIR)])
$(info $$OBJ_DIR is [$(OBJ_DIR)])
$(info $$LIB_DIR is [$(LIB_DIR)])

LDFLAGS += $(LIB_DIR)
LIBS += cunit

MKDIR_BIN_CUNIT := mkdir -p $(BIN_DIR)
MKDIR_OBJ_CUNIT := mkdir -p $(OBJ_DIR)
MKDIR_LIB_CUNIT := mkdir -p $(LIB_DIR)

INC_DIR = include
SRC_DIR = .
$(info $$INC_DIR is [$(INC_DIR)])
$(info $$SRC_DIR is [$(SRC_DIR)])

# Flags
CFLAGS += -I$(INC_DIR)
$(info $$CFLAGS is [$(CFLAGS)])

TARGETS = directories_cunit $(LIB_DIR)/libcunit
$(info rule is [$(INC_DIR)/cUnit.h])

all : $(TARGETS)

directories_cunit:
	$(MKDIR_BIN_CUNIT)
	$(MKDIR_OBJ_CUNIT)
	$(MKDIR_LIB_CUNIT)

# Build cUnit as static library
$(LIB_DIR)/libcunit : $(OBJ_DIR)/cUnit.o
	$(AR) $@.a $^
	$(RL) $@.a

$(OBJ_DIR)/cUnit.o : $(SRC_DIR)/cUnit.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/cUnit.o : $(INC_DIR)/cUnit.h

ifdef TEST
$(BIN_DIR)/cunit : $(OBJ_DIR)/main.o
	$(CC) $< -o $@ $(addprefix -L, $(LDFLAGS)) $(addprefix -l, $(LIBS))
# Build main
$(OBJ_DIR)/main.o : $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o : $(INC_DIR)/cUnit.h
endif

endif

ifdef RIOT
unexport RIOT

APPLICATION = cunit

BOARD ?= native

RIOTBASE ?= /home/phantom/CP_Systems/RTOS/RIOT

DEVHELP ?= 1

# For using assertions
DEVELHELP ?= 1

QUIET ?= 1

CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-parameter
CFLAGS += -D__RIOT__
ifdef TEST
CFLAGS += -DTESTING
endif


endif