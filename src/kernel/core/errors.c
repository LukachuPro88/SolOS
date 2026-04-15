/*
 * Copyright (c) 2026 Luka Saarivirta
 *
 * Licensed under GNU GPL v2.
 */

#include "headers/vga.h"

void kernel_panic(const char *message) {
    // Disable interrupts
    asm volatile("cli");
    
    // Print the panic message
    print("KERNEL PANIC: ")
    print(message);
    
    // Halt the CPU
    while (1) {
        asm volatile("hlt");
    }
}