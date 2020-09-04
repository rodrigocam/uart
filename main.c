
#include <string.h>
#include <stdio.h>

int hex2int(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return -1;
}

int main() {
    char msg[5] = {hex2int(0xA1), '1', '3', '9', '9'};
    /* strcat(msg, "1399"); */
    for(int i=0; i<5; i++) {
        printf("%c\n", msg[i]);
    }
}
