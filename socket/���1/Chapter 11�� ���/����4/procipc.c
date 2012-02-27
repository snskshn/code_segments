#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[])
{
	int fds1[2], fds2[2];
	char str1[]="Do you like coffee?";
	char str2[]="I like coffee";
	char str3[]="I like bread";
	char * str_arr[]={str1, str2, str3};
	char buf[BUF_SIZE];
	pid_t pid;
	int i;
	
	pipe(fds1), pipe(fds2);
	pid=fork();
	
	if(pid==0)
	{
		for(i=0; i<3; i++)
		{
			write(fds1[1], str_arr[i], strlen(str_arr[i])+1);
			read(fds2[0], buf, BUF_SIZE);
			printf("Child proc output: %s \n",  buf);
		}
	}
	else
	{
		for(i=0; i<3; i++)
		{
			read(fds1[0], buf, BUF_SIZE);
			printf("Parent proc output: %s \n", buf);
			write(fds2[1], str_arr[i], strlen(str_arr[i])+1);
		}
	}
	return 0;
}
