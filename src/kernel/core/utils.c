/*
 * SolOS
 * Copyright (C) 2026 Luka Saarivirta
 *
 * Licensed under the GNU GPL v2.
 */

#include "headers/utils.h"
#include <stdint.h>

//-----------------------------------------
// Port I/O Functions
//-----------------------------------------
uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

void outw(uint16_t port, uint16_t value) {
    __asm__ volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}

//-----------------------------------------
// String utilities
//-----------------------------------------
int strcmp(const char* a, const char* b) {
    while (*a && *b && *a == *b) {
        a++;
        b++;
    }
    return *a - *b;
}

//-----------------------------------------
// Pseudo-random number generator
//-----------------------------------------
static uint32_t seed = 12345;

void srand(uint32_t new_seed) {
    seed = new_seed;
}

uint32_t get_tsc(void) {
    uint32_t lo;
    __asm__ volatile("rdtsc" : "=a"(lo) : : "edx");
    return lo;
}

uint32_t rand(void) {
    seed = seed * 1664525 + 1013904223;
    return seed;
}