#include <stdio.h>
#include <signal.h>

void handler(int sig)
{
    puts("hi");
}

int main()
{
    int i;
    for(i = 10; i < 30; i++) {
	signal(i, handler);
    }
    while(1);

    return 0;
}
