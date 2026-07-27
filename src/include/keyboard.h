#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "kernel.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#define KEY_ENTER 0x1C
#define KEY_BACKSPACE 0x0E
#define KEY_ESC 0x01
#define KEY_SHIFT 0x2A
#define KEY_UP 0x48
#define KEY_DOWN 0x50
#define KEY_PAGEUP 0x49
#define KEY_PAGEDOWN 0x51

void keyboard_init(void);
char keyboard_read_char(void);
uint8_t keyboard_read_scancode(void);

#endif
