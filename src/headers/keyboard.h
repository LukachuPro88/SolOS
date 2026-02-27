#pragma once
#include <stdint.h>

// Arrow key scancodes
#define KEY_UP    0x48
#define KEY_DOWN  0x50
#define KEY_LEFT  0x4B
#define KEY_RIGHT 0x4D

// Special key return values (above ASCII range)
#define SPECIAL_UP    0x100
#define SPECIAL_DOWN  0x101
#define SPECIAL_LEFT  0x102
#define SPECIAL_RIGHT 0x103

uint8_t  keyboard_read_scancode(void);
uint16_t keyboard_handle_scancode(uint8_t sc,
                                   const unsigned char* lower,
                                   const unsigned char* upper);
uint8_t  keyboard_shift_held(void);