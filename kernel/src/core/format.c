#include "core/format.h"

void int_to_ascii(int n, char* buffer){
    char temp[10];
    int i = 0;

    do {
        int ret = n % 10;
        temp[i++] = ret + '0';
        n /= 10;
    } while(n > 0);

    for(int j = 0; j < i; j++){
        buffer[j] = temp[i - j - 1];
    }
    buffer[i] = 0;
}

