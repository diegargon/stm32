# Diego Garcia Gonzalez (diegargon@gmail.com)
# Makefile for STM32 / STM32F407xx projects

CC = arm-none-eabi-gcc
GDB = arm-none-eabi-gdb
OBJCOPY =  arm-none-eabi-objcopy -O binary
MCPU = -mcpu=cortex-m4
DMCPU = -DSTM32F407xx
MAIN_PROG = main
CFLAGS  = -Wall -mthumb -O0 -fdata-sections -ffunction-sections -g3 -Werror-implicit-function-declaration -Wno-comment -Wno-unused-function -ffreestanding -nostdlib -mlittle-endian

LDFLAGS = -mthumb -Wall -lgcc  --specs=nano.specs --specs=rdimon.specs -Tboot/stm32F407VET6_flash.ld  -N
MAP = -Wl,-Map=build/$(MAIN_PROG).map
LIBS = 
INCLUDES = -I./ext/cmsis/include -I./ext/include -I./include
BUILD_DIR = build
SRCS_DIR = src
SRCS = $(wildcard $(SRCS_DIR)/*.c)

OBJS = $(SRCS:$(SRCS_DIR)%c=$(BUILD_DIR)%o)

EXT_SRCS_DIR = ext/src
EXT_SRCS = $(wildcard $(EXT_SRCS_DIR)/*.c)
EXT_SRCS_S = $(wildcard $(EXT_SRCS_DIR)/*.s)

EXT_OBJ = $(EXT_SRCS:$(EXT_SRCS_DIR)/%c=$(BUILD_DIR)/%o)
EXT_OBJ_S = $(EXT_SRCS_S:$(EXT_SRCS_DIR)/%s=$(BUILD_DIR)/%o) 

GDB_ARGS = -ex "target extended-remote localhost:3333" -ex "monitor arm semihosting enable" -ex "monitor init" -ex  "monitor reset halt" -ex "monitor targets" -ex "tui enable"
GDB_ARGS_FLASH = $(GDB_ARGS) -ex "monitor flash write_image erase $(BUILD_DIR)/$(MAIN_PROG).bin 0x08000000" -ex "monitor targets"


default: all

all: $(MAIN_PROG)
	@echo Doing $(MAIN_PROG)

#Make main 
$(MAIN_PROG):  ext $(OBJS) 
	$(CC) $(MCPU) $(LDFLAGS) $(MAP) -o $(BUILD_DIR)/$(MAIN_PROG).elf $(OBJS) $(EXT_OBJ) $(EXT_OBJ_S)

$(OBJS): $(SRCS) 
	$(CC) $(MCPU) $(DMCPU) $(CFLAGS) $(INCLUDES) $(LIBS) -o $(OBJS) -c $<	

#Make main y obj incluyendo el codigo semihosting
semi: clean ext semi-objs
	$(CC) $(MCPU) $(LDFLAGS) $(MAP) -o $(BUILD_DIR)/$(MAIN_PROG).elf $(OBJS) $(EXT_OBJ) $(EXT_OBJ_S)
	
semi-objs: $(SRCS) 
	$(CC) $(MCPU) $(DMCPU) $(CFLAGS) $(INCLUDES) $(LIBS) -DSEMIHOSTING -o $(OBJS) -c $<	
	
#Conectarse al servidor gdb (necesita openocd server iniciado)
debug: $(MAIN_PROG)
	$(GDB) $(BUILD_DIR)/$(MAIN_PROG).elf $(GDB_ARGS)

#Connectarse al servidor gdb y flashear (necesita openocd server iniciado)
debug-flash: $(MAIN_PROG) binary
	$(GDB) $(BUILD_DIR)/$(MAIN_PROG).elf $(GDB_ARGS_FLASH)

#Crea los obj externos startup/system	
ext: $(EXT_OBJ) $(EXT_OBJ_S)
	
$(EXT_OBJ): $(EXT_SRCS)
	$(CC) $(MCPU) $(DMCPU) $(CFLAGS) $(INCLUDES) $(LIBS) -o $(EXT_OBJ) -c $<

$(EXT_OBJ_S): $(EXT_SRCS_S)
	$(CC) $(MCPU) $(DMCPU) $(CFLAGS) $(INCLUDES) $(LIBS) -o $(EXT_OBJ_S) -c $<	

#Convierte el ejecutable main.elf a main.bin (binario)
binary:
	$(OBJCOPY) $(BUILD_DIR)/$(MAIN_PROG).elf $(BUILD_DIR)/$(MAIN_PROG).bin

#Arranca el servidor openocd en modo semihosting 	
openocd:
	openocd -f ../../config/stm32f4-black.cfg -c "init; arm semihosting enable ; reset halt ; targets"

#Arranca el servidor openocd en modo semihosting y flasea	
openocd-flash: binary
	openocd -f ../../config/stm32f4-black.cfg -c "init; arm semihosting enable ; reset halt ; flash write_image erase $(BUILD_DIR)/$(MAIN_PROG).bin 0x08000000 ; targets"

#Flasea usando openocd y sale (no puede estar el servidor openocd iniciado)
flash: binary $(MAIN_PROG)	
	openocd -f ../../config/stm32f4-black.cfg -c "init; reset halt ; flash write_image erase $(BUILD_DIR)/$(MAIN_PROG).bin 0x08000000 ; reset run ; targets ; exit"
#flasea a traves de telnet y sale
telnet-flash: halt binary $(MAIN_PROG)
	echo "flash write_image erase $(BUILD_DIR)/$(MAIN_PROG).bin 0x08000000" | telnet localhost 4444 || true
#pone la placa en modo run	
run: 
	echo "reset run" | telnet localhost 4444 || true
	echo "targets" | telnet localhost 4444 || true
#pone la placa en modo halt
halt:
	echo "reset halt" | telnet localhost 4444 || true
	echo "targets" | telnet localhost 4444 || true
#Limpia el directorio build
clean: 
	rm -f $(BUILD_DIR)/*
