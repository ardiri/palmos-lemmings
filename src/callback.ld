OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(callback)
 SEARCH_DIR(/usr/local/arm-elf/lib);
SECTIONS
{
  .text 0x00000000 : { *(.text) *(.rodata) *(.rodata.str1.4) }
}

