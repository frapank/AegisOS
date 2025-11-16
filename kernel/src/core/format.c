#include "core/format.h"

void int_to_ascii(int n, char* buffer)
{

    char temp[10];
    int i = 0;

    do {
        int ret = n % 10;
        temp[i++] = ret + '0';
        n /= 10;
    } while(n > 0);

    for(int j = 0; j < i; j++)
        buffer[j] = temp[i - j - 1];

    buffer[i] = 0;
}

void uint_to_hex(u32 n, char* buffer) {
    buffer[0] = '0';
    buffer[1] = 'x';

    for (int i = 0; i < 8; i++) {
        u8 nibble = (n >> (28 - i * 4)) & 0xF;
        if (nibble < 10)
            buffer[2 + i] = '0' + nibble;
        else
            buffer[2 + i] = 'A' + (nibble - 10);
    }

    buffer[10] = '\0';
}
