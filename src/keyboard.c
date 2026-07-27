#include "include/keyboard.h"

static uint8_t keyboard_map[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0
};

static uint8_t keyboard_shift_map[] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0, 0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0, 0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0
};

static int shift_pressed = 0;

void keyboard_init(void) {
    shift_pressed = 0;
}

char keyboard_read_char(void) {
    uint8_t scancode;
    
    while (1) {
        if (inb(KEYBOARD_STATUS_PORT) & 1) {
            scancode = inb(KEYBOARD_DATA_PORT);
            break;
        }
        __asm__ volatile("nop");
    }
    
    if (scancode & 0x80) {
        uint8_t key = scancode & 0x7F;
        if (key == KEY_SHIFT) shift_pressed = 0;
        return 0;
    }
    
    if (scancode == KEY_SHIFT) {
        shift_pressed = 1;
        return 0;
    }
    
    if (scancode == KEY_ENTER) return '\n';
    if (scancode == KEY_BACKSPACE) return '\b';
    if (scancode == KEY_ESC) return 27;
    
    char c = 0;
    if (scancode < 0x60) {
        if (shift_pressed) {
            c = keyboard_shift_map[scancode];
        } else {
            c = keyboard_map[scancode];
        }
    }
    
    return c;
}
