#include <stdio.h>
#include <string.h>

int main()
{
    char test[100] = "abcde";
    static int count;

    //strncat(test, "df", strlen("df"));
    sprintf(test, "%s%d.pcap\n", test, count++);
    sprintf(test, "%s%d.pcap\n", test, count++);
    sprintf(test, "%s%d.pcap\n", test, count++);
    sprintf(test, "%s%d.pcap\n", test, count++);
    sprintf(test, "%s%d.pcap\n", test, count++);
    printf("%s\n", test);

    return 0;
}
