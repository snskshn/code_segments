#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#if 0
int main()
{
#if 0
	FILE *p = popen("ls", "r");
	char buf[1024];

	while (fgets(buf, 1024, p))
		printf("%s", buf);

	pclose(p);
#else
	int pid;
	int pipefd[2];
	int i = 0;

	pipe(pipefd);
	pid = fork();
	assert(pid >= 0);

	if (pid == 0) {
		close(pipefd[0]);
		dup2(pipefd[1], 1);
		char *const argv[] = {"/bin/df", "-h", NULL};
		char *env[] = {NULL};
		execve(argv[0], argv, env);
	} else {
		char buf[1024];
		close(pipefd[1]);
		FILE *df_file = fdopen(pipefd[0], "r");
		while (fgets(buf, 1024, df_file))
			//printf("%d\n", i++);
			printf("%s", buf);
		waitpid(pid, NULL, 0);
	}
#endif

	return 0;
}
#endif


FILE *open_command(const char *command)
{
	int pid;
	int pipefd[2];
	char *argv[100], *env[] = {NULL};
	char *saveptr, buf[100];
	int i;

	/* parsing */
	strncpy(buf, command, 100);

	argv[0] = strtok_r(buf, " \t", &saveptr);

	for (i = 0; argv[i]; i++) {
		argv[i + 1] = strtok_r(NULL, " \t", &saveptr);
	}

	argv[i] = NULL;

	pipe(pipefd);
	pid = fork();
	assert(pid >= 0);

	if (pid == 0) {
		close(pipefd[0]);
		dup2(pipefd[1], 1);
		execve(argv[0], argv, env);
	} else {
		close(pipefd[1]);
	}

	return fdopen(pipefd[0], "r");
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		return -1;
	}

	FILE *f = open_command(argv[1]);
	char buf[1024];

	while (fgets(buf, 1024, f))
		printf("%s", buf);

	return 0;
}
