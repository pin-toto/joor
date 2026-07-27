#ifndef FS_H
#define FS_H

#include "kernel.h"
#include "shell.h"

#define MAX_FILES 10
#define MAX_FILENAME 32
#define MAX_FILE_SIZE 256

void fs_init(void);
void fs_list(void);
void fs_delete(const char* name);
void fs_read_file(const char* name, char* buffer);
void fs_write_file(const char* name, char* data, int size);

#endif
