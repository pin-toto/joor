#include "include/fs.h"
#include "include/string.h"

static file_t files[MAX_FILES];
static int file_count = 0;

void fs_init(void) {
    file_count = 0;
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

void fs_read_file(const char* name, char* buffer) {
    for (int i = 0; i < file_count; i++) {
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
            strcpy(buffer, files[i].data);
            return;
        }
    }
    buffer[0] = '\0';
}

void fs_write_file(const char* name, char* data, int size) {
    for (int i = 0; i < file_count; i++) {
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
            int k = 0;
            while (data[k] && k < MAX_FILE_SIZE - 1) {
                files[i].data[k] = data[k];
                k++;
            }
            files[i].data[k] = '\0';
            files[i].size = k;
            return;
        }
    }
    
    if (file_count < MAX_FILES) {
        int i = 0;
        while (name[i] && i < MAX_FILENAME - 1) {
            files[file_count].name[i] = name[i];
            i++;
        }
        files[file_count].name[i] = '\0';
        
        int k = 0;
        while (data[k] && k < MAX_FILE_SIZE - 1) {
            files[file_count].data[k] = data[k];
            k++;
        }
        files[file_count].data[k] = '\0';
        files[file_count].size = k;
        file_count++;
    }
}
