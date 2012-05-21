#include <arpa/inet.h>
#include <stdio.h>

int main()
{
	char str[16] = "10.86.123.30";
	int value;
	inet_pton(AF_INET, str, &value);

	printf("%s\n", inet_ntop(AF_INET, &value, str, sizeof(str)));

	return 0;
}
