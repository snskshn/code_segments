#include <stdio.h>

int main()
{
    int pid;

    __asm__("movl $20, %%eax\n\t"
	    "int $0x80\n\t"
	    "movl %%eax, %0"
	    : "=m" (pid)
	   );

    printf("pid: %d\n", pid);

    return 0;
}
