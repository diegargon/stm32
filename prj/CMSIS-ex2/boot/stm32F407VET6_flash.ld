/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = 0x2001FFFF;    /* end of RAM */

/* 2000000 + 128.000 = 536998912  2001 F400‬ ¿Cambiar a 2001F400 */

/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x200;;      /* required amount of heap  */
_Min_Stack_Size = 0x400;; /* required amount of stack */

/* Specify the memory areas */
MEMORY
{
  FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 512K
  RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 128K
  CCMRAM (rw)     : ORIGIN = 0x10000000, LENGTH = 64K
}
/* _estack = ORIGIN(RAM) + LENGTH(RAM);ORIGIN(RAM) + LENGTH(RAM); */
INCLUDE boot/stm32.ld


