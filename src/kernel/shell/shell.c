/*
 * SolOS
 * Copyright (C) 2026 Luka Saarivirta
 *
 * Licensed under the GNU GPL v2.
 * See LICENSE file in project root for full license text.
*/

//-----------------------------------------
// kernel/shell.c
// Simple kernel shell for SolOS
// Handles keyboard input, commands, and basic output
//-----------------------------------------

#include "headers/shell.h"
#include "headers/utils.h"
#include "headers/vga.h"
#include <stdint.h>

//-----------------------------------------
// Internal buffers and keyboard layouts
//-----------------------------------------
static char input_buf[256];
static int input_len = 0;

// Standard US keyboard mappings
static const unsigned char sc_ascii_lower[] = {
  0, 0, '1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',
  'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
  'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
  'z','x','c','v','b','n','m',',','.','/',0,'*',0,' '
};

static const unsigned char sc_ascii_upper[] = {
  0,0,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t',
  'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,
  'A','S','D','F','G','H','J','K','L',':','"','~',0,'|',
  'Z','X','C','V','B','N','M','<','>', '?',0,'*',0,' '
};

// Finnish keyboard layout (lowercase)
static const unsigned char sc_ascii_lower_fi[] = {
  0,0,'1','2','3','4','5','6','7','8','9','0','+',
  0,'\b','\t','q','w','e','r','t','y','u','i','o','p',
  0x86,0xA8,'\n',0,'a','s','d','f','g','h','j','k','l',
  0x94,0x84,0xBD,0,'\'','z','x','c','v','b','n','m',',',
  '.','-',0,'*',0,' '
};

// Finnish keyboard layout (uppercase)
static const unsigned char sc_ascii_upper_fi[] = {
  0,0,'!','"','#',0xA4,'%','&','/','(',')','=',
  '?',0,'\b','\t','Q','W','E','R','T','Y','U','I','O','P',
  0x8F,0xA8,'\n',0,'A','S','D','F','G','H','J','K','L',
  0x99,0x8E,0xBD,0,'*','Z','X','C','V','B','N','M',';',':','_',0,'*',0,' '
};

// Keyboard layout struct
typedef struct {
  const unsigned char* lower;
  const unsigned char* upper;
  const char* name;
} keyboard_layout_t;

static const keyboard_layout_t layouts[] = {
  {sc_ascii_lower, sc_ascii_upper, "US"},
  {sc_ascii_lower_fi, sc_ascii_upper_fi, "FI"}
};

static int current_layout = 0;

//-----------------------------------------
// Joke system
//-----------------------------------------
typedef struct {
  const char* question;
  const char* punchline;
} Joke;

static Joke jokes[] = {
  {"Why is the programmer scared of nature?", "Too many bugs!"},
  {"Why do programmers prefer dark mode?", "Because light attracts bugs!"},
  {"Why did the scarecrow win an award?", "Because he was outstanding in his field!"},
  {"Why can't skeletons fight each other?", "They don't have the guts!"},
  {"Why did the bicycle fall over?", "Because it was two-tired!"}
};

//-----------------------------------------
// Internal helpers
//-----------------------------------------
static unsigned char read_char(void);
static int is_write_command(const char* input);
static void parse_input(void);

//-----------------------------------------
// Commands
//-----------------------------------------
static void cmd_help(void) {
  print("\nCommands: help, clear, hello, about, joke, settings\n");
  print("Usage: write \"message\"\n");
}
static void cmd_clear(void) { clear_screen(); }
static void cmd_hello(void) { print("\nHello from SolOS!\n"); }
static void cmd_about(void) { print("\nSolOS v0.1 - A hobby OS\n"); }
static void cmd_joke(void) {
  int num_jokes = sizeof(jokes) / sizeof(Joke);
  int joke_index = rand() % num_jokes;
  print("\n");
  print(jokes[joke_index].question);
  print("\n");
  print(jokes[joke_index].punchline);
  print("\n");
}
static void cmd_settings(void) {
  clear_screen();
  print("KEYBOARD LAYOUT: ");
  print(layouts[current_layout].name);
  print("\nPress c to cycle layout, q to quit\n");

  while (1) {
    unsigned char c = read_char();
    if (!c) continue;

    if (c == 'c') {
      current_layout = (current_layout + 1) % 2;
      clear_screen();
      print("KEYBOARD LAYOUT: ");
      print(layouts[current_layout].name);
      print("\nPress c to cycle layout, q to quit\n");
    } else if (c == 'q') {
      clear_screen();
      break;
    }
  }
}
static void cmd_write(void) {
  const char* usage = "\nUsage: write \"message\"\n";
  const char* p = input_buf + 5;
  while (*p == ' ') p++;

  if (*p != '"') { print(usage); return; }
  p++;
  const char* message_start = p;

  while (*p && *p != '"') p++;
  if (*p != '"') { print(usage); return; }
  const char* message_end = p;
  p++;
  while (*p == ' ') p++;
  if (*p != '\0') { print(usage); return; }

  print("\n");
  for (const char* it = message_start; it < message_end; it++)
    putchar((unsigned char)*it);
  print("\n");
}

// Command struct
typedef struct {
  const char* name;
  void (*handler)(void);
} Command;

static Command commands[] = {
  {"help", cmd_help}, {"clear", cmd_clear}, {"hello", cmd_hello},
  {"about", cmd_about}, {"joke", cmd_joke}, {"settings", cmd_settings},
  {"write", cmd_write}
};

// Check if command is write
static int is_write_command(const char* input) {
  return input[0]=='w' && input[1]=='r' && input[2]=='i' &&
         input[3]=='t' && input[4]=='e' && 
         (input[5]=='\0' || input[5]==' ');
}

//-----------------------------------------
// Input parsing
//-----------------------------------------
static void parse_input(void) {
  if (is_write_command(input_buf)) { cmd_write(); return; }

  int num_commands = sizeof(commands) / sizeof(Command);
  for (int i=0; i<num_commands; i++) {
    if (strcmp(input_buf, commands[i].name) == 0) {
      commands[i].handler();
      return;
    }
  }

  if (input_len > 0) {
    print("\nUnknown command: ");
    print(input_buf);
    print("\n");
  }
}

//-----------------------------------------
// Keyboard reading
//-----------------------------------------
static unsigned char read_char(void) {
  static uint8_t shift = 0;
  uint8_t sc, status;

  while (1) {
    __asm__ volatile("inb $0x64, %0" : "=a"(status));
    if (status & 0x01) break;
  }

  __asm__ volatile("inb $0x60, %0" : "=a"(sc));

  // Shift press/release handling
  if (sc==0x2A || sc==0x36) { shift=1; return 0; }
  if (sc==0xAA || sc==0xB6) { shift=0; return 0; }
  if (sc & 0x80) return 0;

  if (sc < sizeof(sc_ascii_lower)) {
    return shift ? layouts[current_layout].upper[sc] : layouts[current_layout].lower[sc];
  }
  return 0;
}

//-----------------------------------------
// Shell prompt
//-----------------------------------------
void shell_prompt(void) {
  print("\nSolOS> ");
  input_len = 0;

  while (1) {
    unsigned char c = read_char();
    if (!c) continue;

    if (c=='\n') {
      input_buf[input_len] = '\0';
      parse_input();
      return;
    } else if (c=='\b') {
      if (input_len>0) {
        input_len--;
        cursor--;
        vga[cursor] = (color<<8)|' ';
        update_hw_cursor();
      }
    } else if (input_len<255) {
      putchar(c);
      input_buf[input_len++] = c;
    }
  }
}