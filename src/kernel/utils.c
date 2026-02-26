/*
 * SolOS
 * Copyright (C) 2026 Luka Saarivirta
 *
 * Licensed under the GNU GPL v2.
 * See LICENSE file in project root for full license text.
*/

//-----------------------------------------
// kernel/utils.c
// Kernel utility functions
// Handles VGA output, hardware cursor, string comparison, and random numbers
//-----------------------------------------

#include "headers/utils.h"
#include <stdint.h>

//-----------------------------------------
// VGA / Display Variables
//-----------------------------------------
uint16_t* vga = (uint16_t*)0xB8000; // VGA text buffer
uint8_t color = 0x0F;               // Default text color
int cursor = 0;                      // Current cursor position

//-----------------------------------------
// VGA / Hardware Cursor Functions
//-----------------------------------------

/**
 * Disable blinking text in VGA
 */
void vga_disable_blink(void) {
  inb(0x3DA);
  outb(0x3C0, 0x10);
  uint8_t val = inb(0x3C1);
  val &= ~0x08;
  outb(0x3C0, 0x10);
  outb(0x3C0, val);
}

/**
 * Update hardware cursor to match `cursor` variable
 */
void update_hw_cursor(void) {
  if (cursor < 0 || cursor >= 80 * 25) return;
  uint16_t pos = (uint16_t)cursor;
  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint8_t)(pos & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint8_t)(pos >> 8));
}

/**
 * Enable hardware cursor with custom scanline start/end
 */
void enable_hw_cursor(uint8_t start, uint8_t end) {
  outb(0x3D4, 0x0A);
  uint8_t val = inb(0x3D5);
  val &= ~0x20;
  val = (val & 0xC0) | (start & 0x1F);
  outb(0x3D4, 0x0A);
  outb(0x3D5, val);
  outb(0x3D4, 0x0B);
  outb(0x3D5, (uint8_t)(end & 0x1F));
  update_hw_cursor();
}

//-----------------------------------------
// Text Output Functions
//-----------------------------------------

/**
 * Print a single character to the screen
 */
void putchar(unsigned char c) {
  if (c == '\n') {
    cursor += 80 - (cursor % 80);
  } else {
    vga[cursor] = (color << 8) | c;
    cursor++;
  }
  update_hw_cursor();
}

/**
 * Print a null-terminated string
 */
void print(const char* s) {
  while (*s) putchar((unsigned char)*s++);
}

/**
 * Clear the screen and reset cursor
 */
void clear_screen(void) {
  for (int i = 0; i < 80 * 25; i++) vga[i] = (color << 8) | ' ';
  cursor = 0;
  update_hw_cursor();
}

//-----------------------------------------
// String / Utility Functions
//-----------------------------------------

/**
 * Compare two strings
 * Returns 0 if equal, else difference of first differing char
 */
int strcmp(const char* a, const char* b) {
  while (*a && *b && *a == *b) {
    a++;
    b++;
  }
  return *a - *b;
}

//-----------------------------------------
// Pseudo-random Number Generator
//-----------------------------------------
static uint32_t seed = 12345;

/**
 * Return a pseudo-random 32-bit number
 */
uint32_t rand(void) {
  seed = seed * 1664525 + 1013904223;
  return seed;
}