/*
 * SolOS
 * Copyright (C) 2026 Luka Saarivirta
 *
 * Licensed under the GNU GPL v2.
 */

//-----------------------------------------
// drivers/navigation/keyboard.c
// Handles keyboard navigation and other functions.
//-----------------------------------------

#include "headers/keyboard.h"

#include <stdint.h>
#include "headers/vga.h"

static uint8_t shift = 0;

uint8_t keyboard_read_scancode(void) {
  uint8_t status, sc;

  while (1) {
    __asm__ volatile("inb $0x64, %0" : "=a"(status));
    if (status & 0x01) break;
  }

  __asm__ volatile("inb $0x60, %0" : "=a"(sc));
  return sc;
}

uint16_t keyboard_handle_scancode(uint8_t sc, const unsigned char* lower,
                                  const unsigned char* upper) {
  // Shift press/release
  if (sc == 0x2A || sc == 0x36) {
    shift = 1;
    return 0;
  }
  if (sc == 0xAA || sc == 0xB6) {
    shift = 0;
    return 0;
  }

  // Key release — ignore
  if (sc & 0x80) return 0;

  // Arrow keys
  if (sc == KEY_UP) return SPECIAL_UP;
  if (sc == KEY_DOWN) return SPECIAL_DOWN;
  if (sc == KEY_LEFT) return SPECIAL_LEFT;
  if (sc == KEY_RIGHT) return SPECIAL_RIGHT;

  update_hw_cursor();

  // Normal character
  if (sc < 60) {
    return shift ? upper[sc] : lower[sc];
  }

  return 0;
}

uint8_t keyboard_shift_held(void) {
  return shift;
}
