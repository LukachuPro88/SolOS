/*
 * SolOS
 * Copyright (C) 2026 Luka Saarivirta
 *
 * Licensed under the GNU GPL v2.
 * See LICENSE file in project root for full license text.
*/

//-----------------------------------------
// headers/vga.h
// VGA functions 
// Provides cursor functions, screen clearing, text reading and writing
//-----------------------------------------

#ifndef VGA_H
#define VGA_H

#include <stdint.h>

static uint16_t* vga = (uint16_t*)0xB8000;
static uint8_t color = 0x0F;
static int cursor = 0;

// VGA text mode constants
#define VGA_WIDTH  80
#define VGA_HEIGHT 25

// VGA API
void vga_disable_blink(void);
void enable_hw_cursor(uint8_t start, uint8_t end);
void update_hw_cursor(void);
void clear_screen(void);
void putchar(unsigned char c);
void print(const char* s);

#endif