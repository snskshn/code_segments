#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <execinfo.h>

/* get REG_EIP from ucontext.h */
#define __USE_GNU
#include <ucontext.h>

/* self debugging module.
 *
 * - Implemented using documented SA_SIGINFO option and undocumented
 *   third "secret" argument in signal handler.
 * - Handler may misbehave on high-level compiler optimization.
 * - Inline or Macro functions may not be printed.
 * - backtrace_symbols() can fail if the memory heap is corrupted.
 *   in that case, use backtrace_symbols_fd() instead.
 * - print the exact line with "addr2line -fe [executable] [addr]"
 * - tested on i386 and ELF binery format only.
 *
 *
 * http://www.linuxjournal.com/article/6391
 * */
void bt_sighandler(int sig, siginfo_t *info, void *secret)
{
    void *trace[16];
    char **messages = (char **)NULL;
    int i, trace_size = 0;
    ucontext_t *uc = (ucontext_t *)secret;

    /* Do something useful with siginfo_t */
    if(sig == SIGSEGV) {
	printf("Got signal %d, faulty addfess is %p, "
		"from %p\n", sig, info->si_addr,
		(void *)uc->uc_mcontext.gregs[REG_EIP]);
    }
    else {
	printf("Got signal %d\n", sig);
    }

    trace_size = backtrace(trace, 16);
    /* overwrite sigaction with caller's address */
    trace[1] = (void *)uc->uc_mcontext.gregs[REG_EIP];

    messages = backtrace_symbols(trace, trace_size);
    /* skip first stack frame (points here) */
    printf("[bt] Execution path:\n");
    for(i = 1; i < trace_size; ++i) {
	printf("[bt] %s\n", messages[i]);
    }

    free(messages);
    //exit(0);
}

int func_a(int a, char b)
{
    char *p = (char *)0xdeadbeef;

    a = a + b;
    *p = 10;	/* CRASH here!! */

    return 2 * a;
}

int func_b()
{
    int res, a = 5;

    res = 5 + func_a(a, 't');

    return res;
}

int main()
{
    /* Install our signal handler */
    struct sigaction sa;

    sa.sa_sigaction = (void *)bt_sighandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_SIGINFO;

    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);
    /* ... add any otehr signal here */

    /* Do something */
    printf("%d\n", func_b());

    return 0;
}
