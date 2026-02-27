#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

// Port I/O
uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);
void outw(uint16_t port, uint16_t value);

// Utils
int strcmp(const char* a, const char* b);

void srand(uint32_t new_seed);
uint32_t get_tsc(void);
uint32_t rand(void);

#endif