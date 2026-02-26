/*
 * SolOS
 * Copyright (C) 2026 Luka Saarivirta
 *
 * Licensed under the GNU GPL v2.
 * See LICENSE file in project root for full license text.
*/

//-----------------------------------------
// linker.ld
// Linker script for SolOS kernel
// Defines memory layout and entry point
//-----------------------------------------

ENTRY(_start)          // Kernel entry point defined in kernel_entry.asm

SECTIONS {
  // Set load address for kernel
  . = 0x1000;

  //---------------------------------------
  // Text section (code)
  //---------------------------------------
  .text : {
    *(.text)
  }

  //---------------------------------------
  // Read-only data section
  //---------------------------------------
  .rodata : {
    *(.rodata)
  }

  //---------------------------------------
  // Initialized data section
  //---------------------------------------
  .data : {
    *(.data)
  }

  //---------------------------------------
  // Uninitialized data (BSS)
  //---------------------------------------
  .bss : {
    *(.bss)
  }
}