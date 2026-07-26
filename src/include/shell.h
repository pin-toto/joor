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

// دستورات (همگی با همین فرمت)
void cmd_help(void);
void cmd_echo(char* text);
void cmd_info(void);
void cmd_ls(void);
void cmd_rm(char* filename);
void cmd_clear(void);
void cmd_reboot(void);
void cmd_off(void);

// سیستم فایل
void fs_init(void);
void fs_list(void);
void fs_delete(const char* name);
void fs_create(const char* name, const char* content);

#endif
