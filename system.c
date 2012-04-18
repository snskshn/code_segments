#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

/* Note: command must include absoulte path */
int run_command(const char *command)
{
	int pid;
	char *argv[100], *env[] = {NULL};
	char *token, *saveptr, buf[100];
	int i;

	/* parsing */
	strncpy(buf, command, 100);

	argv[0] = strtok_r(buf, " \t", &saveptr);
	
	for (i = 0; argv[i]; i++) {
		argv[i + 1] = strtok_r(NULL, " \t", &saveptr);
	}

	argv[i] = NULL;

	/* forking */
	pid = fork();
	assert(pid >= 0);

	if (pid == 0) {
		execve(argv[0], argv, env);
	}

	return pid;
}

int main(int argc, char *argv[])
{
#if 0
    system("ls -al");
#else
    //int pid = run_command("/bin/busybox ftpget -ussson -pfuture01 10.51.51.1 test.pcap test.pcap");
    int pid = run_command(argv[1]);
    waitpid(pid, NULL, 0);
#endif

    return 0;
}
