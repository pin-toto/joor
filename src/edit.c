#include "include/shell.h"  // این باید باشه!
#include "include/fs.h"
#include "include/string.h"

#define MAX_LINES 100
#define MAX_LINE_LEN 256

static char editor_buffer[MAX_LINES][MAX_LINE_LEN];
static int line_count = 0;

void cmd_edit(char* filename) {
    if (!filename || filename[0] == '\0') {
        print("Usage: edit <filename>\n");
        return;
    }
    
    print("Editing: ");
    print(filename);
    print("\n");
    print("(Type ':wq' to save, ':q!' to quit)\n");
    
    // بارگذاری محتوای قبلی
    char content[4096] = {0};
    fs_read_file(filename, content);
    
    line_count = 0;
    if (content[0] != '\0') {
        char* line = content;
        while (*line) {
            char* next = line;
            while (*next && *next != '\n') next++;
            if (*next == '\n') *next = '\0';
            
            strcpy(editor_buffer[line_count++], line);
            if (line_count >= MAX_LINES) break;
            
            line = next + 1;
        }
    }
    
    // نمایش محتوای فعلی
    print("Current content:\n");
    for (int i = 0; i < line_count; i++) {
        print(editor_buffer[i]);
        print("\n");
    }
    print("---\n");
    
    char line_input[MAX_LINE_LEN];
    
    while (1) {
        print("> ");
        terminal_readline(line_input, MAX_LINE_LEN);  // از shell.h میاد
        
        if (strcmp(line_input, ":wq") == 0) {
            char new_content[4096] = {0};
            int pos = 0;
            for (int i = 0; i < line_count; i++) {
                int j = 0;
                while (editor_buffer[i][j] && pos < 4095) {
                    new_content[pos++] = editor_buffer[i][j];
                    j++;
                }
                if (pos < 4095) {
                    new_content[pos++] = '\n';
                }
            }
            new_content[pos] = '\0';
            fs_write_file(filename, new_content, pos);
            print("Saved!\n");
            break;
        } else if (strcmp(line_input, ":q!") == 0) {
            print("Exited without saving.\n");
            break;
        } else {
            if (line_count < MAX_LINES) {
                strcpy(editor_buffer[line_count++], line_input);
            } else {
                print("Buffer full!\n");
            }
        }
    }
}
