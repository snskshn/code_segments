#include <stdio.h>
#include <unistd.h>

int main()
{
    int fd[2];
    char buf[30];

    pipe(fd);

    while(1);

    return 0;
}
