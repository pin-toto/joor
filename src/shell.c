#include "include/shell.h"

static char command_buffer[MAX_COMMAND_LENGTH];
static int buffer_index = 0;

// ======== سیستم فایل ========
static file_t files[MAX_FILES];
static int file_count = 0;

void fs_init(void) {
    file_count = 0;
    // ایجاد فایل تست
    char* content = "This is a test file for Joor OS";
    int i = 0;
    while (content[i] && i < MAX_FILE_SIZE - 1) {
        files[0].data[i] = content[i];
        i++;
    }
    files[0].data[i] = '\0';
    files[0].size = i;
    
    char* name = "tst.txt";
    i = 0;
    while (name[i] && i < MAX_FILENAME - 1) {
        files[0].name[i] = name[i];
        i++;
    }
    files[0].name[i] = '\0';
    file_count++;
}

void fs_list(void) {
    if (file_count == 0) {
        print("No files\n");
        return;
    }
    print("Files:\n");
    for (int i = 0; i < file_count; i++) {
        print("  ");
        print(files[i].name);
        print("\n");
    }
}

void fs_delete(const char* name) {
    for (int i = 0; i < file_count; i++) {
        // مقایسه ساده با حلقه
        int match = 1;
        int j = 0;
        while (name[j] && files[i].name[j]) {
            if (name[j] != files[i].name[j]) {
                match = 0;
                break;
            }
            j++;
        }
        if (match && name[j] == '\0' && files[i].name[j] == '\0') {
            for (int j = i; j < file_count - 1; j++) {
                int k = 0;
                while (files[j+1].name[k] && k < MAX_FILENAME - 1) {
                    files[j].name[k] = files[j+1].name[k];
                    k++;
                }
                files[j].name[k] = '\0';
                
                k = 0;
                while (files[j+1].data[k] && k < MAX_FILE_SIZE - 1) {
                    files[j].data[k] = files[j+1].data[k];
                    k++;
                }
                files[j].data[k] = '\0';
                files[j].size = files[j+1].size;
            }
            file_count--;
            print("Deleted: ");
            print(name);
            print("\n");
            return;
        }
    }
    print("File not found: ");
    print(name);
    print("\n");
}

// ======== دستورات ========
void cmd_help(void) {
    print("\nCommands:\n");
    print("  help    - Show this help\n");
    print("  ls      - List files\n");
    print("  rm      - Delete a file\n");
    print("  cout    - Print text\n");
    print("  clear   - Clear screen\n");
    print("  info    - System info\n");
    print("  reboot  - Reboot\n");
    print("  off     - Shutdown\n");
}

void cmd_echo(char* text) {
    print(text);
    print("\n");
}

void cmd_info(void) {
    print_color("\n=== Joor OS v1.0 ===\n", COLOR_LIGHT_GREEN);
    print_color("Built with C + ASM\n", COLOR_LIGHT_CYAN);
    print_color("x86 32-bit\n\n", COLOR_LIGHT_CYAN);
}

void cmd_ls(void) {
    fs_list();
}

void cmd_rm(char* filename) {
    fs_delete(filename);
}

void cmd_clear(void) {
    screen_clear();
}

void cmd_reboot(void) {
    print("Rebooting...\n");
    __asm__ volatile("int $0x19");
}

void cmd_off(void) {
    print_color("System halted.\n", COLOR_LIGHT_RED);
    while (1) __asm__ volatile("hlt");
}

// ======== شل ========
void shell_init(void) {
    screen_clear();
    fs_init();
    print_color("================================\n", COLOR_LIGHT_CYAN);
    print_color("   JOOR OS v1.0\n", COLOR_LIGHT_GREEN);
    print_color("   Type 'help'\n", COLOR_LIGHT_GREEN);
    print_color("================================\n\n", COLOR_LIGHT_CYAN);
}

void shell_run(void) {
    while (1) {
        print_color("Joor> ", COLOR_LIGHT_CYAN);
        
        buffer_index = 0;
        
        while (1) {
            char c = keyboard_read_char();
            
            if (c == 0) continue;
            
            if (c == '\n') {
                screen_putchar('\n', WHITE_ON_BLACK);
                command_buffer[buffer_index] = '\0';
                break;
            } else if (c == '\b') {
                if (buffer_index > 0) {
                    buffer_index--;
                    screen_putchar('\b', WHITE_ON_BLACK);
                }
            } else if (c >= 32 && c <= 126) {
                command_buffer[buffer_index++] = c;
                screen_putchar(c, WHITE_ON_BLACK);
                if (buffer_index >= MAX_COMMAND_LENGTH - 1) {
                    buffer_index = MAX_COMMAND_LENGTH - 1;
                }
            }
        }
        
        // ======== پردازش دستورات ========
        if (strcmp(command_buffer, "help") == 0) {
            cmd_help();
        } else if (strcmp(command_buffer, "ls") == 0) {
            cmd_ls();
        } else if (strcmp(command_buffer, "clear") == 0) {
            cmd_clear();
            shell_init();
        } else if (strcmp(command_buffer, "info") == 0) {
            cmd_info();
        } else if (strcmp(command_buffer, "reboot") == 0) {
            cmd_reboot();
        } else if (strcmp(command_buffer, "off") == 0) {
            cmd_off();
        } else if (command_buffer[0] == 'r' && command_buffer[1] == 'm' && command_buffer[2] == ' ') {
            cmd_rm(command_buffer + 3);
        } else if (command_buffer[0] == 'c' && command_buffer[1] == 'o' && command_buffer[2] == 'u' && command_buffer[3] == 't' && command_buffer[4] == ' ') {
            cmd_echo(command_buffer + 5);
        } else if (command_buffer[0] != '\0') {
            print_color("Unknown command: ", COLOR_LIGHT_RED);
            print(command_buffer);
            print("\n");
        }
    }
}
