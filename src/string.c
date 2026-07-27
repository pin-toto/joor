#include "include/string.h"

char* strcpy(char* dest, const char* src) {
    char* orig = dest;
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    return orig;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

int strncmp(const char* s1, const char* s2, int n) {
    while (n-- && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

int strlen(const char* s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}

char* strcat(char* dest, const char* src) {
    char* orig = dest;
    while (*dest) dest++;
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    return orig;
}
