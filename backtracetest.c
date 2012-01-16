#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <signal.h>

void back_trace()
{
    perror("normal exit");
}

/* addr2line -e [executable] [hex address] */
static void sighandler(int num)
{
    void *array[20];	// only care about last 17 functions (3 taken with tracing support)
    size_t size;
    char **strings;
    size_t i;

    size = backtrace(array, 20);
    strings = backtrace_symbols(array, size);	// need -rdynamic gcc (linker) flag for this to work

    for(i = 0; i < size; i++) {
	printf("%s\n", strings[i]);	// ERROR
    }

    free(strings);

    signal(SIGABRT, SIG_DFL);
    abort();	// hopefully will give core
}

int main()
{
    /* error processing */
    if(atexit(back_trace) != 0) {
	perror("atexit");
    }

    signal(SIGSEGV, sighandler);
    char *ptr = NULL;

    *ptr = 100;	// error here!

    return 0;
}
