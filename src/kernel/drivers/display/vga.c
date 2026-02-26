/*
 * SolOS
 * Copyright (C) 2026 Luka Saarivirta
 *
 * Licensed under the GNU GPL v2.
 */

#include "headers/vga.h"

#include <stdint.h>

#include "headers/utils.h"  // inb / outb

//-----------------------------------------
// Internal helpers
//-----------------------------------------
void update_hw_cursor(void) {
  if (cursor < 0 || cursor >= VGA_WIDTH * VGA_HEIGHT) return;

  uint16_t pos = (uint16_t)cursor;
  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint8_t)(pos & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint8_t)(pos >> 8));
}

static void scroll(void) {
  for (int i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++) {
    vga[i] = vga[i + VGA_WIDTH];
  }

  for (int i = VGA_WIDTH * (VGA_HEIGHT - 1); i < VGA_WIDTH * VGA_HEIGHT; i++) {
    vga[i] = (color << 8) | ' ';
  }

  cursor -= VGA_WIDTH;
}

//-----------------------------------------
// Public VGA API
//-----------------------------------------
void vga_disable_blink(void) {
  inb(0x3DA);
  outb(0x3C0, 0x10);
  uint8_t val = inb(0x3C1);
  val &= ~0x08;
  outb(0x3C0, 0x10);
  outb(0x3C0, val);
}

void enable_hw_cursor(uint8_t start, uint8_t end) {
  outb(0x3D4, 0x0A);
  uint8_t val = inb(0x3D5);
  val &= ~0x20;
  val = (val & 0xC0) | (start & 0x1F);
  outb(0x3D4, 0x0A);
  outb(0x3D5, val);

  outb(0x3D4, 0x0B);
  outb(0x3D5, end & 0x1F);

  update_hw_cursor();
}

void putchar(unsigned char c) {
  if (c == '\n') {
    cursor += VGA_WIDTH - (cursor % VGA_WIDTH);
  } else {
    vga[cursor] = (color << 8) | c;
    cursor++;
  }

  if (cursor >= VGA_WIDTH * VGA_HEIGHT) {
    scroll();
  }

  update_hw_cursor();
}

void print(const char* s) {
  while (*s) {
    putchar((unsigned char)*s++);
  }
}

void clear_screen(void) {
  for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
    vga[i] = (color << 8) | ' ';
  }

  cursor = 0;
  update_hw_cursor();
}