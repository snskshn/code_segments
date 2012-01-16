#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum)
{
    if(signum == SIGUSR1) {
	printf("get signal\n");
    }
}

int main()
{
    //signal(SIGUSR1, handler);

    struct sigaction sig;

    sig.sa_handler = handler;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = 0;

    if(sigaction(SIGUSR1, &sig, 0) == -1) {
	printf("signal(SIGUSR1) error\n");
	return -1;
    }

    kill(getpid(), SIGUSR1);
    kill(getpid(), SIGUSR1);
    kill(getpid(), SIGUSR1);
    kill(getpid(), SIGUSR1);
    kill(getpid(), SIGUSR1);
    kill(getpid(), SIGUSR1);

    return 0;
}
