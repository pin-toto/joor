#include "include/shell.h"
#include "include/string.h"

static char command_buffer[MAX_COMMAND_LENGTH];
static int buffer_index = 0;
int show_welcome = 1;

// ======== تاریخچه دستورات ========
#define MAX_HISTORY 10
static char history[MAX_HISTORY][MAX_COMMAND_LENGTH];
static int history_count = 0;
static int history_pos = -1;

void add_to_history(const char* cmd) {
    if (cmd[0] == '\0') return;
    if (history_count > 0 && strcmp(history[history_count - 1], cmd) == 0) return;
    
    if (history_count < MAX_HISTORY) {
        strcpy(history[history_count++], cmd);
    } else {
        for (int i = 0; i < MAX_HISTORY - 1; i++) {
            strcpy(history[i], history[i + 1]);
        }
        strcpy(history[MAX_HISTORY - 1], cmd);
    }
    history_pos = history_count;
}

// ======== تابع اصلی خواندن خط (فقط با تاریخچه) ========
void terminal_readline(char* buffer, int maxlen) {
    int i = 0;
    buffer[0] = '\0';
    
    while (1) {
        char c = keyboard_read_char();
        
        if (c == 0) {
            uint8_t sc = inb(KEYBOARD_DATA_PORT);
            
            if (sc == 0x48) {  // فلش بالا
                if (history_pos > 0) {
                    // پاک کردن خط فعلی
                    while (i > 0) {
                        screen_putchar('\b', WHITE_ON_BLACK);
                        i--;
                    }
                    history_pos--;
                    strcpy(buffer, history[history_pos]);
                    i = strlen(buffer);
                    print(buffer);
                }
                continue;
            } else if (sc == 0x50) {  // فلش پایین
                if (history_pos < history_count - 1) {
                    while (i > 0) {
                        screen_putchar('\b', WHITE_ON_BLACK);
                        i--;
                    }
                    history_pos++;
                    strcpy(buffer, history[history_pos]);
                    i = strlen(buffer);
                    print(buffer);
                } else if (history_pos == history_count - 1) {
                    while (i > 0) {
                        screen_putchar('\b', WHITE_ON_BLACK);
                        i--;
                    }
                    history_pos = history_count;
                    buffer[0] = '\0';
                    i = 0;
                }
                continue;
            }
            continue;
        }
        
        if (c == '\n') {
            buffer[i] = '\0';
            screen_putchar('\n', WHITE_ON_BLACK);
            break;
        } else if (c == '\b' && i > 0) {
            i--;
            screen_putchar('\b', WHITE_ON_BLACK);
        } else if (c >= 32 && c <= 126) {
            buffer[i++] = c;
            screen_putchar(c, WHITE_ON_BLACK);
            if (i >= maxlen - 1) {
                buffer[i] = '\0';
                screen_putchar('\n', WHITE_ON_BLACK);
                break;
            }
        }
    }
}

// ======== دستورات ========
void cmd_help(void) {
    print("\nCommands:\n");
    print("  help    - Show this help\n");
    print("  ls      - List files\n");
    print("  rm      - Delete a file\n");
    print("  cat     - Show file content\n");
    print("  cout    - Print text\n");
    print("  clear   - Clear screen\n");
    print("  cls     - Clear screen\n");
    print("  info    - System info\n");
    print("  reboot  - Reboot\n");
    print("  off     - Shutdown\n");
    print("  edit    - Edit a file\n");
    print("  show_welcome:on/off - Toggle welcome message\n");
}

void cmd_echo(char* text) {
    print(text);
    print("\n");
}

void cmd_info(void) {
    print_color("\n=== Joor OS v0.2 ===\n", COLOR_LIGHT_GREEN);
    print_color("x86 32-bit\n", COLOR_LIGHT_CYAN);
}

void cmd_ls(void) {
    fs_list();
}

void cmd_rm(char* filename) {
    if (!filename || filename[0] == '\0') {
        print("Usage: rm <filename>\n");
        return;
    }
    fs_delete(filename);
}

void cmd_cat(char* filename) {
    if (!filename || filename[0] == '\0') {
        print("Usage: cat <filename>\n");
        return;
    }
    
    char content[4096] = {0};
    fs_read_file(filename, content);
    
    if (content[0] == '\0') {
        print("File not found or empty: ");
        print(filename);
        print("\n");
        return;
    }
    
    print(content);
}

void cmd_clear(void) {
    screen_clear();
}

void cmd_reboot(void) {
    print("Rebooting...\n");
    __asm__ volatile("int $0x19");
}

void cmd_off(void) {
    print("Shutting down...\n");
    __asm__ volatile(
        "mov $0x1000, %ax\n"
        "mov %ax, %ss\n"
        "mov $0xf000, %sp\n"
        "mov $0x5307, %ax\n"
        "mov $0x0001, %bx\n"
        "mov $0x0003, %cx\n"
        "int $0x15"
    );
    while (1) {
        __asm__ volatile("hlt");
    }
}

// ======== شل ========
void shell_init(void) {
    screen_clear();
    fs_init();
    if (show_welcome) {
        print_color("================================\n", COLOR_LIGHT_CYAN);
        print_color("   JOOR OS v0.2\n", COLOR_LIGHT_GREEN);
        print_color("   Type 'help'\n", COLOR_LIGHT_GREEN);
        print_color("================================\n\n", COLOR_LIGHT_CYAN);
    }
}

void shell_run(void) {
    while (1) {
        print_color("Joor> ", COLOR_LIGHT_CYAN);
        buffer_index = 0;
        command_buffer[0] = '\0';
        
        terminal_readline(command_buffer, MAX_COMMAND_LENGTH);
        
        if (command_buffer[0] != '\0') {
            add_to_history(command_buffer);
        }
        
        // ======== پردازش دستورات ========
        if (strcmp(command_buffer, "help") == 0) {
            cmd_help();
        } else if (strcmp(command_buffer, "ls") == 0) {
            cmd_ls();
        } else if (strcmp(command_buffer, "cat") == 0) {
            cmd_cat("");
        } else if (strcmp(command_buffer, "clear") == 0) {
            cmd_clear();
            shell_init();
        } else if (strcmp(command_buffer, "cls") == 0) {
            cmd_clear();
            shell_init();
        } else if (strcmp(command_buffer, "info") == 0) {
            cmd_info();
        } else if (strcmp(command_buffer, "reboot") == 0) {
            cmd_reboot();
        } else if (strcmp(command_buffer, "off") == 0) {
            cmd_off();
        } else if (strcmp(command_buffer, "show_welcome:off") == 0) {
            show_welcome = 0;
        } else if (strcmp(command_buffer, "show_welcome:on") == 0) {
            show_welcome = 1;
        } else if (command_buffer[0] == 'r' && command_buffer[1] == 'm' && command_buffer[2] == ' ') {
            cmd_rm(command_buffer + 3);
        } else if (command_buffer[0] == 'c' && command_buffer[1] == 'o' && command_buffer[2] == 'u' && command_buffer[3] == 't' && command_buffer[4] == ' ') {
            cmd_echo(command_buffer + 5);
        } else if (command_buffer[0] == 'c' && command_buffer[1] == 'a' && command_buffer[2] == 't' && command_buffer[3] == ' ') {
            cmd_cat(command_buffer + 4);
        } else if (command_buffer[0] == 'e' && command_buffer[1] == 'd' && command_buffer[2] == 'i' && command_buffer[3] == 't' && command_buffer[4] == ' ') {
            cmd_edit(command_buffer + 5);
        } else if (strcmp(command_buffer, "edit") == 0) {
            cmd_edit("");
        } else if (command_buffer[0] != '\0') {
            print_color("Unknown command: ", COLOR_LIGHT_RED);
            print(command_buffer);
            print("\n");
        }
    }
}
