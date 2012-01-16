#include <stdio.h>

int main()
{
    FILE *fp = fopen("test.txt", "w");

    fwrite("abcde", sizeof(int), 
