#ifndef SCREEN_H
#define SCREEN_H

#include "kernel.h"

void screen_init(void);
void screen_putchar(char c, uint8_t color);
void screen_print(const char* str);
void screen_print_color(const char* str, uint8_t color);
void screen_clear(void);
void screen_set_cursor(uint16_t pos);
uint16_t screen_get_cursor(void);
void screen_scroll(void);

#endif
