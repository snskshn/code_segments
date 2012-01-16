#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define BACKTRACE_BUFFER_SIZE 256

void func_2()
{
    int i, nptrs;
    void *buffer[BACKTRACE_BUFFER_SIZE];
    char **strings = NULL;

    nptrs = backtrace(buffer, BACKTRACE_BUFFER_SIZE);
    strings = backtrace_symbols(buffer, nptrs);
    if(!strings) {
	perror("backtrace_symbols");
	exit(EXIT_FAILURE);
    }

    for(i = 0; i < nptrs; i++) {
	printf("%s\n", strings[i]);
    }

    free(strings);
}

void func_1()
{
    func_2();
}

int main()
{
    //func_1();
    signal(SIGSEGV, func_2);

    int *test = NULL;
    *test = 0;
    return 0;
}

