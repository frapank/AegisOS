#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>

int kstrncmp(const char* str1, const char* str2, size_t n);
int kstreql(const char* ch1, const char* ch2);
size_t kstrncpy(char* dest, const char* src, size_t n);
size_t strlen(const char* ch);

#endif
