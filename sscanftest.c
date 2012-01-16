#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    char *str = "SENSOR_COORDINATES 1@(23.4,30.4)\n";
    char key[20];
    char value[20];
    int ip;

    sscanf(str, "%s %s\n", key, value);
#if 0
    printf("%c\n", key[0]);

    inet_aton(value, (struct in_addr *)&ip);

    printf("%s %s\n", key, inet_ntoa(*((struct in_addr *)&ip)));
#else

    printf("%s %s\n", key, value);
    int sensor_id;
    long double x, y;

    sscanf(value, "%d@(%Lf,%Lf)", &sensor_id, &x, &y);

    printf("%d, %Lf, %Lf\n", sensor_id, x, y);
#endif

    int a, b;
    char *buf = "2356777";

    sscanf(buf, "%02x%02x", &a, &b);

    printf("%x %x\n", a, b);

    return 0;
}
