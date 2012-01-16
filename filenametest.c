#include <stdio.h>

int main()
{
    char *filename = "test.txt";
    char *directory = "policy";
    FILE *fp = fopen(directory"/"filename, "wr");


    return 0;
}
