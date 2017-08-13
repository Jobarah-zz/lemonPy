#include <stdio.h>
#include <iostream>

#define BIN 0
#define DEC 1
#define HEX 2

int toBinary(int decNum) {

    return decNum == 0 ? 0 : (decNum % 2 + 10 * toBinary(decNum / 2));
}

void printFormat(int number, int format) {
    switch(format) {
        case BIN:
            printf("%d\n", toBinary(number));
        break;
        case HEX:
            printf("%X\n", number);
        break;
        case DEC:
            printf("%d\n", number);
        break;
    }
}