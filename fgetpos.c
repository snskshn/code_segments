#include <stdio.h>

int main()
{
	FILE *fp;
	fpos_t pos;
	char buf[1024];

	fp = fopen("test.txt", "w+");

	if (fp == NULL)
		puts("cannot create file");
	else {
		fputs("abcde", fp);
#if 0
		fgetpos(fp, &pos);
		printf("position of file pointer: %ld\n", pos.__pos);
#else
		fseek(fp, 0, SEEK_SET);
		printf("position of file pointer: %ld\n", ftell(fp));
#endif
		fread(buf, 1024, 1, fp);
		puts(buf);
		fclose(fp);
	}

	return 0;
}
