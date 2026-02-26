/*
 * SolOS
 * Copyright (C) 2026 Luka Saarivirta
 *
 * Licensed under the GNU GPL v2.
 * See LICENSE file in project root for full license text.
*/

//-----------------------------------------
// kernel/kernel.c
// Simple kernel shell for SolOS
// Handles keyboard input, commands, and basic output
//-----------------------------------------

#include "headers/shell.h"
#include "headers/vga.h"

void kernel_main(void) {
  clear_screen();
  print("SolOS v0.1 booted successfully.\n");
  print("Type 'help' for available commands.\n");

  while (1) {
    shell_prompt();
  }
}