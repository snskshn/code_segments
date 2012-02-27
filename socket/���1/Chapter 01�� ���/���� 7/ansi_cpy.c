#include <stdio.h>
#define BUF_SIZE  30

int main(void)
{
	char buf[BUF_SIZE];
	int readCnt;

	FILE * src=fopen("src.dat", "rb");
	FILE * des=fopen("dst.dat", "wb");
	
	if(src==NULL || des==NULL)
	{
		puts("file open error");
		return -1;
	}

	while(1)
	{
		readCnt=fread((void*)buf, 1, BUF_SIZE, src);

		if(readCnt<BUF_SIZE)
		{
			if(feof(src)!=0)
			{
				fwrite((void*)buf, 1, readCnt, des);
				break;
			}
			else
				puts("file cpy error()");

			break;
		}
		fwrite((void*)buf, 1, BUF_SIZE, des);
	}

	fclose(src);
	fclose(des);
	return 0;
}
