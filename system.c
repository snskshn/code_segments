#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int run_command(char *const argv[])
{
	int pid = fork();
	char *env[] = {NULL};

	assert(pid >= 0);

	if (pid == 0) {
		execve(argv[0], argv, env);
	}

	return pid;
}

int main()
{
#if 0
    system("ls -al");
#else
    char *const argv[] = {"/bin/ls", "-al", NULL};
    int pid = run_command(argv);
    waitpid(pid, NULL, 0);
#endif

    return 0;
}
