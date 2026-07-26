#include "include/kernel.h"
#include "include/screen.h"
#include "include/keyboard.h"
#include "include/shell.h"

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

void* memcpy(void* dest, const void* src, int n) {
    char* d = (char*)dest;
    const char* s = (const char*)src;
    for (int i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

void* memset(void* dest, int c, int n) {
    char* d = (char*)dest;
    for (int i = 0; i < n; i++) {
        d[i] = (char)c;
    }
    return dest;
}

void screen_print_dec(uint32_t num) {
    char buffer[32];
    int i = 0;
    
    if (num == 0) {
        screen_putchar('0', WHITE_ON_BLACK);
        return;
    }
    
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    for (int j = i - 1; j >= 0; j--) {
        screen_putchar(buffer[j], WHITE_ON_BLACK);
    }
}

void print(const char* str) {
    screen_print(str);
}

void print_color(const char* str, uint8_t color) {
    screen_print_color(str, color);
}

void clear_screen(void) {
    screen_clear();
}

void print_hex(uint32_t num) {
    char hex[] = "0123456789ABCDEF";
    screen_print("0x");
    for (int i = 28; i >= 0; i -= 4) {
        screen_putchar(hex[(num >> i) & 0xF], WHITE_ON_BLACK);
    }
}

void print_dec(uint32_t num) {
    screen_print_dec(num);
}

void putchar(char c) {
    screen_putchar(c, WHITE_ON_BLACK);
}

void kernel_main(void) {
    screen_init();
    keyboard_init();
    shell_init();
    shell_run();
    
    while (1) {
        __asm__ volatile("hlt");
    }
}
