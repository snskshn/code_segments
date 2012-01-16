#include <stdio.h>

void printfloat(float f)
{
    unsigned t;
    char temp[35], bin[35];

    t = *(unsigned *)&f;
    itoa(t, bin, 2);
    memset(temp, '0', 35);
    strcpy(temp + 32 - strlen(bin), bin);

