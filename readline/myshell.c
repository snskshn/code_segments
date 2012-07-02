#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main()
{
	char *cmd, *token;

	while ((cmd = readline("admin>"))) {
		token = strtok(cmd, " \t");
		if (!token)
			continue;

		if (strcmp("quit", token) == 0)
			break;

		else if (strcmp("ls", token) == 0) {
			token = strtok(NULL, " \t");
			char buf[1024];
			sprintf(buf, "ls %s", token);
			system(buf);
		}

		add_history(cmd);
		free(cmd);
	}

	return 0;
}

