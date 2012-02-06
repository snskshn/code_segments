#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int b;
    int c;
} *NODE;

int main()
{
    NODE ptr;

    printf("%d\n", sizeof(ptr));
    printf("%d\n", sizeof(*ptr));

    ptr = malloc(sizeof(*NODE));

    ptr->b = 3;
    ptr->c = 4;

    printf("%d, %d\n", ptr->b, ptr->c);

    free(ptr);

    return 0;
}
