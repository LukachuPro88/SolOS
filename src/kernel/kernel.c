/*
 * SolOS
 * Copyright (C) 2026 Luka Saarivirta
 *
 * Licensed under the GNU GPL v2.
 * See LICENSE file in project root for full license text.
 */

//-----------------------------------------
// headers/utils.h
// Kernel utility functions and hardware access
// Provides VGA output, port I/O, string utilities, and random numbers
//-----------------------------------------

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

//-----------------------------------------
// VGA / Display Variables
//-----------------------------------------
extern uint16_t* vga;  // Pointer to VGA memory (text mode buffer 0xB8000)
extern uint8_t color;  // Current text color attribute
extern int cursor;     // Current cursor position (0-1999 for 80x25 screen)

//-----------------------------------------
// Port I/O Functions
//-----------------------------------------

/**
 * Write a byte to an I/O port
 * @param port I/O port number
 * @param val  Value to write
 */
static inline void outb(uint16_t port, uint8_t val) {
  __asm__ volatile("outb %0, %1" ::"a"(val), "Nd"(port));
}

/**
 * Read a byte from an I/O port
 * @param port I/O port number
 * @return Value read
 */
static inline uint8_t inb(uint16_t port) {
  uint8_t val;
  __asm__ volatile("inb %1, %0" : "=a"(val) : "Nd"(port));
  return val;
}

//-----------------------------------------
// VGA / Screen Functions
//-----------------------------------------
void vga_disable_blink(void);  // Disable blinking text
void update_hw_cursor(void);   // Update hardware cursor
void enable_hw_cursor(uint8_t start,
                      uint8_t end);  // Enable cursor with scanlines
void putchar(unsigned char c);       // Print single character at cursor
void print(const char* s);           // Print null-terminated string
void clear_screen(void);             // Clear the screen and reset cursor

//-----------------------------------------
// Utility Functions
//-----------------------------------------
int strcmp(const char* a, const char* b);  // Compare two strings
uint32_t rand(void);                       // Return pseudo-random 32-bit number

#endif  // UTILS_H

//-----------------------------------------
// kernel/kernel.c
// Main kernel implementation
// Initializes screen, prints boot messages, and runs shell
//-----------------------------------------

#include "headers/shell.h"
#include "headers/utils.h"

//-----------------------------------------
// Kernel Entry Point
//-----------------------------------------

/**
 * Main entry point for the kernel.
 * Called by kernel_entry.asm after switching to protected mode.
 * Initializes the display and starts the shell loop.
 */
void kernel_main(void) {
  // Clear the screen and reset cursor
  clear_screen();

  // Print boot messages
  print("SolOS v0.1 booted successfully.\n");
  print("Type 'help' for available commands.\n");

  // Enter infinite shell loop
  while (1) {
    shell_prompt();
  }
}