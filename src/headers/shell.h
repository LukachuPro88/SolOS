/*
 * SolOS
 * Copyright (C) 2026 Luka Saarivirta
 *
 * Licensed under the GNU GPL v2.
 * See LICENSE file in project root for full license text.
*/

//-----------------------------------------
// headers/shell.h
// Simple shell interface for kernel
// Provides function to start shell prompt
//-----------------------------------------

#ifndef SHELL_H
#define SHELL_H

//-----------------------------------------
// Shell Functions
//-----------------------------------------

/**
 * Start the shell prompt.
 * Handles user input and command processing.
 */
void shell_prompt(void);

#endif // SHELL_H