#include "include/kernel.h"
#include "include/screen.h"
#include "include/keyboard.h"
#include "include/shell.h"


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

uint8_t cmos_read(uint8_t reg) {
    outb(0x70, reg);
    return inb(0x71);
}

uint8_t bcd_to_bin(uint8_t bcd) {
    return (bcd & 0x0F) + ((bcd >> 4) * 10);
}

void cmd_date() {
    uint8_t minute, hour, day, month, year;
    
    minute = cmos_read(0x02);
    hour = cmos_read(0x04);
    day = cmos_read(0x07);
    month = cmos_read(0x08);
    year = cmos_read(0x09);
    
    minute = bcd_to_bin(minute);
    hour = bcd_to_bin(hour);
    day = bcd_to_bin(day);
    month = bcd_to_bin(month);
    year = bcd_to_bin(year);
    
    hour += 3;
    minute += 30;
    
    if (minute >= 60) {
        minute -= 60;
        hour += 1;
    }
    if (hour >= 24) {
        hour -= 24;
    }
    
    print("Date: ");
    print_dec(year + 2000);
    print("-");
    if (month < 10) print("0");
    print_dec(month);
    print("-");
    if (day < 10) print("0");
    print_dec(day);
    print(" Time: ");
    if (hour < 10) print("0");
    print_dec(hour);
    print(":");
    if (minute < 10) print("0");
    print_dec(minute);
    print("\n");
}
