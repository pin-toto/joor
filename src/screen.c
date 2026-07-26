#include "include/screen.h"

static uint16_t* vga_buffer = (uint16_t*)VGA_ADDRESS;
static uint16_t cursor_pos = 0;
static uint8_t default_color = WHITE_ON_BLACK;

void screen_init(void) {
    screen_clear();
    screen_set_cursor(0);
}

void screen_putchar(char c, uint8_t color) {
    if (c == '\n') {
        cursor_pos = (cursor_pos / MAX_COLS + 1) * MAX_COLS;
    } else if (c == '\r') {
        cursor_pos = (cursor_pos / MAX_COLS) * MAX_COLS;
    } else if (c == '\t') {
        cursor_pos = (cursor_pos + 4) & ~3;
    } else if (c == '\b') {
        if (cursor_pos > 0) {
            cursor_pos--;
            vga_buffer[cursor_pos] = (color << 8) | ' ';
        }
    } else {
        vga_buffer[cursor_pos] = (color << 8) | c;
        cursor_pos++;
    }
    
    if (cursor_pos >= MAX_ROWS * MAX_COLS) {
        screen_scroll();
    }
    
    screen_set_cursor(cursor_pos);
}

void screen_print(const char* str) {
    screen_print_color(str, default_color);
}

void screen_print_color(const char* str, uint8_t color) {
    while (*str) {
        screen_putchar(*str++, color);
    }
}

void screen_clear(void) {
    for (int i = 0; i < MAX_ROWS * MAX_COLS; i++) {
        vga_buffer[i] = (default_color << 8) | ' ';
    }
    cursor_pos = 0;
    screen_set_cursor(0);
}

void screen_set_cursor(uint16_t pos) {
    cursor_pos = pos;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

uint16_t screen_get_cursor(void) {
    return cursor_pos;
}

void screen_scroll(void) {
    for (int i = 0; i < (MAX_ROWS - 1) * MAX_COLS; i++) {
        vga_buffer[i] = vga_buffer[i + MAX_COLS];
    }
    for (int i = (MAX_ROWS - 1) * MAX_COLS; i < MAX_ROWS * MAX_COLS; i++) {
        vga_buffer[i] = (default_color << 8) | ' ';
    }
    cursor_pos = (MAX_ROWS - 1) * MAX_COLS;
}
