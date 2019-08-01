#!/bin/bash
# debug2

#Compile params
INCLUDES=" -Iext/cmsis/include -Iext/include"
CFLAGS="$CFLAGS -c -mcpu=cortex-m4 -mthumb -O0 -g3 -fdata-sections -ffunction-sections "
#CFLAGS="$CFLAGS  -MMD  -fmessage-length=0 -mfpu=fpv4-sp-d16 "
CFLAGS="$CFLAGS -Werror-implicit-function-declaration -Wno-comment -Wno-unused-function -Wall"
CFLAGS="$CFLAGS -DSTM32F407xx -mlittle-endian"
CFLAGS="$CFLAGS -ffreestanding -nostdlib"
PARAMS="$CFLAGS $INCLUDES"

#Linker params
LDFLAGS="-mcpu=cortex-m4 -mthumb -Wall -lgcc  -Tboot/stm32F407VET6_flash.ld  -N"
LDFLAGS="$LDFLAGS --specs=nano.specs" #newlib-nano 
#LDFLAGS="$LDFLAGS --specs=nosys.specs" #no lib
LDFLAGS="$LDFLAGS --specs=rdimon.specs" #semihost 
MAP=-Wl,-Map=build/Map.map

# Main
arm-none-eabi-gcc src/main.c -o build/main.o  ${PARAMS} 
# System
arm-none-eabi-gcc ext/src/system_stm32f4xx.c -o build/system.o ${PARAMS} 
# Startup
arm-none-eabi-gcc ext/src/startup_stm32f407xx.s -o build/startup.o ${PARAMS} 

#linking - Starpup provee Reset_Handler y System Systeminit
arm-none-eabi-gcc -o build/main.elf build/main.o build/startup.o build/system.o ${LDFLAGS} ${MAP}

#obj to bin/bash
arm-none-eabi-objcopy -O binary build/main.elf build/main.bin