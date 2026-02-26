/*
 * SolOS
 * Copyright (C) 2026 Luka Saarivirta
 *
 * Licensed under the GNU GPL v2.
 * See LICENSE file in project root for full license text.
*/

//-----------------------------------------
// headers/kernel.h
// Kernel entry point declaration
// Provides interface for the main kernel function
//-----------------------------------------

#ifndef KERNEL_H
#define KERNEL_H

//-----------------------------------------
// Kernel Functions
//-----------------------------------------

/**
 * Main entry point for the kernel.
 * Called by kernel_entry.asm after switching to protected mode.
 */
void kernel_main(void);

#endif // KERNEL_H