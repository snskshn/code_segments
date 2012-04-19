#include <stdio.h>

int main()
{
	char *buf = "abcde  345";
	char str[100], str2[100];
	int integer;

	sscanf(buf, "%4s %1s %d", str, str2, &integer);

	printf("[%s], [%s], [%d]\n", str, str2, integer);

	return 0;
}

