#ifndef SHELL_H
#define SHELL_H

#include "kernel.h"
#include "keyboard.h"
#include "screen.h"

#define MAX_COMMAND_LENGTH 256
#define MAX_ARGS 10
#define MAX_FILES 10
#define MAX_FILENAME 32
#define MAX_FILE_SIZE 256

typedef struct {
    char name[MAX_FILENAME];
    char data[MAX_FILE_SIZE];
    int size;
} file_t;

void shell_init(void);
void shell_run(void);
void terminal_readline(char* buffer, int maxlen);  // اضافه کن!
void cmd_edit(char* filename);

void fs_init(void);
void fs_list(void);
void fs_delete(const char* name);
void fs_read_file(const char* name, char* buffer);

#endif
