#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

// Port I/O
uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);

// Utils
int strcmp(const char* a, const char* b);
uint32_t rand(void);

#endif