#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    char *filename = "sensor_policy.xml";
    struct stat buf;

    stat(filename, &buf);

    printf("%lu\n", buf.st_size);

    return 0;
}
