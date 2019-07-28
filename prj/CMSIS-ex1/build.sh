#!/bin/bash

INCLUDES=" -Iext/cmsis/include -Iext/include"
CFLAGS="$CFLAGS -c -MMD -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -O0 -g3 -Wall -fmessage-length=0"
CFLAGS="$CFLAGS -Werror-implicit-function-declaration -Wno-comment -Wno-unused-function -ffunction-sections -fdata-sections"
CFLAGS="$CFLAGS -DSTM32F407xx -mlittle-endian -ffreestanding -nostdlib"

PARAMS="$CFLAGS $INCLUDES"
LDFLAGS="-mcpu=cortex-m4 -mthumb -Wall --specs=nosys.specs -lgcc  -Tboot/stm32F407VET6_flash.ld  -N"

# Main
arm-none-eabi-gcc src/main.c -o build/main.o  ${PARAMS} 
# System
arm-none-eabi-gcc ext/src/system_stm32f4xx.c -o build/system.o ${PARAMS} 
# Startup
arm-none-eabi-gcc ext/src/startup_stm32f407xx.s -o build/startup.o ${PARAMS} 

#linking - Starpup provee Reset_Handler y System Systeminit
arm-none-eabi-gcc -o build/main.elf build/main.o build/startup.o build/system.o ${LDFLAGS}

#obj to bin/bash
arm-none-eabi-objcopy -O binary build/main.elf build/main.bin