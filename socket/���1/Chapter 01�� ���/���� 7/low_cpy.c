#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define BUF_SIZE 100

int main(int argc, char *argv[]) 
{
	int src, dst;
	int read_cnt;
	char buf[BUF_SIZE];
	
	src=open("src.dat", O_RDONLY);
	dst=open("dst.dat", O_CREAT|O_WRONLY|O_TRUNC);
	if(src==-1||dst==-1)
	{
		puts("file open error");
		return -1;
	}
	
	while((read_cnt=read(src, buf, BUF_SIZE))!=0)
		write(dst, buf, read_cnt);

	close(src);
	close(dst);
	return 0;
}
