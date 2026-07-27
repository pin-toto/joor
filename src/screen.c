#include "include/screen.h"

static uint16_t* vga_buffer = (uint16_t*)VGA_ADDRESS;
static uint16_t cursor_pos = 0;
static uint8_t default_color = WHITE_ON_BLACK;
static int scroll_offset = 0;

static char scrollback[SCROLLBACK_LINES][SCREEN_COLS + 1];
static int scrollback_count = 0;
static int scrollback_index = 0;

void screen_init(void) {
    screen_clear();
    screen_set_cursor(0);
    scroll_offset = 0;
}

void screen_putchar(char c, uint8_t color) {
    if (c == '\n') {
        cursor_pos = (cursor_pos / SCREEN_COLS + 1) * SCREEN_COLS;
    } else if (c == '\r') {
        cursor_pos = (cursor_pos / SCREEN_COLS) * SCREEN_COLS;
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
    
    if (cursor_pos >= SCREEN_ROWS * SCREEN_COLS) {
        screen_scroll();
    }
    
    screen_set_cursor(cursor_pos);
}

void screen_print(const char* str) {
    screen_print_color(str, default_color);
}

void screen_print_color(const char* str, uint8_t color) {
    screen_save_line(str);
    
    while (*str) {
        screen_putchar(*str++, color);
    }
}

void screen_clear(void) {
    for (int i = 0; i < SCREEN_ROWS * SCREEN_COLS; i++) {
        vga_buffer[i] = (default_color << 8) | ' ';
    }
    cursor_pos = 0;
    scroll_offset = 0;
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
    for (int i = 0; i < (SCREEN_ROWS - 1) * SCREEN_COLS; i++) {
        vga_buffer[i] = vga_buffer[i + SCREEN_COLS];
    }
    for (int i = (SCREEN_ROWS - 1) * SCREEN_COLS; i < SCREEN_ROWS * SCREEN_COLS; i++) {
        vga_buffer[i] = (default_color << 8) | ' ';
    }
    cursor_pos = (SCREEN_ROWS - 1) * SCREEN_COLS;
}

void screen_save_line(const char* line) {
    int i = 0;
    while (line[i] && i < SCREEN_COLS - 1) {
        scrollback[scrollback_index][i] = line[i];
        i++;
    }
    scrollback[scrollback_index][i] = '\0';
    scrollback_index = (scrollback_index + 1) % SCROLLBACK_LINES;
    if (scrollback_count < SCROLLBACK_LINES) scrollback_count++;
}

void redraw_screen(void) {
    screen_clear();
    int start = (scrollback_index - scrollback_count + scroll_offset) % SCROLLBACK_LINES;
    if (start < 0) start += SCROLLBACK_LINES;
    
    for (int i = 0; i < SCREEN_ROWS - 1; i++) {
        int idx = (start + i) % SCROLLBACK_LINES;
        if (i < scrollback_count - scroll_offset) {
            screen_print(scrollback[idx]);
        }
    }
}

void screen_scroll_up(int lines) {
    if (scroll_offset + lines < scrollback_count) {
        scroll_offset += lines;
        redraw_screen();
    }
}

void screen_scroll_down(int lines) {
    if (scroll_offset >= lines) {
        scroll_offset -= lines;
        redraw_screen();
    }
}
