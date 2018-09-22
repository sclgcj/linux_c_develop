TARGET=data_structure

#CROSS_COMPILE ?= arm-none-eabi-

export CC 			= gcc
export CPP 			= g++
export AS 			= as
export LD 			= ld
export OBJCOPY 		= objcopy

TOP = $(shell pwd)

				
#-ffunction-sections -fdata-sections 
# 

cur_date=$(shell date)

CPU = #-mcpu=cortex-m4
FPU = #-mfpu=fpv4-sp-d16
FLOAT-ABI = #-mfloat-abi=hard

CHIP = stm32f3xx
HAL_CONF = build_conf/hal_conf
SRC_SHELL = src_select.sh
HAL_SRC_CMD = $(SRC_SHELL) $(CHIP) $(HAL_CONF)

SRC_SUFFIX = c
COMM_SRC_CONF = build_conf/src_conf
COMM_SRC_SHELL = comm_src_select.sh
COMM_SRC_CMD = $(COMM_SRC_SHELL) $(COMM_SRC_CONF)

ASM_SUFFIX = S
ASM_CMD = $(COMM_SRC_SHELL) $(COMM_SRC_CONF) $(ASM_SUFFIX)

HEAD_SUFFIX = h
HEAD_CONF = build_conf/head_conf
HEAD_CMD = $(COMM_SRC_SHELL) $(HEAD_CONF) $(HEAD_SUFFIX)

LIB_SUFFIX = l
LIB_CONF = build_conf/lib_conf
LIB_CMD = cat $(LIB_CONF) | tr "\n" "\0"

ifeq ($(LINK_FILE),)
	LINK_FILE=m_system/arch/linker/STM32F303RETx_FLASH.ld
endif

MAP_FILE=stm32f303retx.map

C_DEFS = -DUSE_HAL_DRIVER -DSTM32F303xE

MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

INC_FLAGS = $(shell $(HEAD_CMD))	

export CFLAGS = -g $(CPU) $(FPU) $(FLOAT-ABI) -Os  -fsigned-char -ffunction-sections -fdata-sections  $(C_DEFS) -DARM_MATH_CM0PLUS \
	-ffunction-sections -fdata-sections $(INC_FLAGS) -MMD -MP -MF"$(@:%.o=%.cdep)" -MT"$(@)"

export CPPFLAGS = -g $(CPU) $(FPU) $(FLOAT-ABI) -Os -fsigned-char -ffunction-sections -fdata-sections $(c_DEFS) \
	-ffunction-sections -fdata-sections $(INC_FLAGS) -fabi-version=0 -MMD -MP -MF"$(@:%.o=%.cdep)" -MT"$(@)"

ASFLAGS = -W -Wall

LDFLAGS = $(CPU) $(FPU) $(FLOAT-ABI) -Os -fsigned-char -fmessage-length=0 -ffunction-sections -fdata-sections  -Xlinker --gc-sections -Wl,-Map,"$(MAP_FILE)" ########################################################################
LDFLAGS += $(shell $(LIB_CMD) 2>/dev/null)
#C_SRC = $(shell find ./ -name '*.c' | grep -v erpc | grep -v mcu2mpu_client)
#C_SRC = $(shell $(HAL_SRC_CMD) 2>/dev/null)
C_SRC += $(shell $(COMM_SRC_CMD) 2>/dev/null)
C_OBJ = $(C_SRC:%.c=%.o)
C_DEP = $(C_SRC:%.c=%.cdep)

#CPP_SRC = $(shell find ./ -name '*.cpp' | grep -v erpc | grep -v mcu2mpu_client)
#CPP_OBJ = $(CPP_SRC:%.cpp=%.o)
#CPP_DEP = $(CPP_SRC:%.cpp=%.cdep)

#ASM_SRC = $(shell find ./ -name '*.S')
#ASM_SRC = $(shell $(ASM_CMD) 2>/dev/null)
#ASM_OBJ = $(ASM_SRC:%.S=%.o)
#ASM_DEP = $(ASM_SRC:%.S=%.adep)

########################################################################
.PHONY: all clean
#-ffunction-sections -fdata-sections 
#all:$(CPP_DEP)  $(C_DEP) $(ASM_DEP) $(CPP_OBJ) $(C_OBJ) $(ASM_OBJ)
all:  $(ASM_OBJ) $(CPP_OBJ) $(C_OBJ)

	$(CC) $(LDFLAGS) -o "$(TARGET).elf" $(ASM_OBJ) $(C_OBJ) $(CPP_OBJ)

	$(OBJCOPY) $(TARGET).elf $(TARGET).bin -Obinary
	$(OBJCOPY) $(TARGET).elf $(TARGET).hex -Oihex

########################################################################
$(C_OBJ):%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ 

########################################################################
$(CPP_OBJ):%.o:%.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@ 


########################################################################
$(ASM_OBJ):%.o:%.S
	$(AS) -c $< -o $@ $(ASFLAGS)

########################################################################
clean:
	@rm -rf $(TARGET).bin
	@rm -rf $(TARGET).elf
	@rm -rf $(TARGET).hex
	@rm -rf $(MAP_FILE)
	@for i in $(shell find ./ -name '*.o'); do if [ -e $${i} ];then rm $${i};fi;done
	@for i in $(shell find ./ -name '*.cdep'); do if [ -e $${i} ]; then rm $${i};fi;done
	@for i in $(shell find ./ -name '*.adep'); do if [ -e $${i} ]; then rm $${i};fi; done
	@rm -rf $(TARGET) $(TARGET).elf $(TARGET).hex

