#include <stdio.h>
#include <string.h>

char ar[16] = "abcdef";

void insert(int idx, char ch)
{
    memmove(ar + idx + 1, ar + idx, strlen(ar) - idx + 1);
    ar[idx] = ch;
}

void delete(int idx)
{
    memmove(ar + idx, ar + idx + 1, strlen(ar) - idx);
}

void append(char ch)
{
    insert(strlen(ar), ch);
}

int main()
{
    printf("initial: %s\n", ar);
    insert(3, 't'); printf("insert t: %s\n", ar);
    delete(1); printf("delete b: %s\n", ar);
    append('s'); printf("append s: %s\n", ar);

    return 0;
}
