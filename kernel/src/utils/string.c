#include "utils/string.h"

int kstrncmp(const char* str1, const char* str2, size_t n){
    for (size_t i = 0; i < n; i++) {
        if (str1[i] == 0 || str2[i] == 0) return (int)(str1[i] - str2[i]);
        if (str1[i] != str2[i]) return (int)(str1[i] - str2[i]);
    }
    return 0;
}

size_t kstrncpy(char* dest, const char* src, size_t n){

    size_t src_len = 0;
    const char *s;

    for (s = src; *s; ++s) ++src_len;

    if (n == 0) return src_len;

    size_t to_copy = (src_len >= n) ? (n - 1) : src_len;

    for (size_t i = 0; i < to_copy; ++i) dest[i] = src[i];

    dest[to_copy] = '\0';
    return src_len;
}

size_t kstrlength(const char* ch){
    size_t i = 0;
    while (ch[i] != 0) i++;
    return i;
}

int kstreql(const char* ch1, const char* ch2){
    size_t size = kstrlength(ch1);

    if (size != kstrlength(ch2)) return 0;
    
    for (size_t i = 0; i <= size; i++) {
        if (ch1[i] != ch2[i]) return 0;
    }

    return 1;
}

