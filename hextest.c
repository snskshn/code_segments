#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    char str[1024] = {0};
    char *ptr = str;
    int *a;

    a = (void *)ptr;
    *a = htonl(0x12345678);

    FILE *fd = fopen("test.txt", "w");
    fwrite(str, 1, sizeof(str), fd);

    fclose(fd);

    return 0;
}




