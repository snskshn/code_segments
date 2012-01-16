#include <stdio.h>
#include <pthread.h>

void *handler(void *arg)
{
    while(1) {
	printf("%d\n", *(int *)arg);
    }
}

int main()
{
    pthread_t tid[10];
    int i;

    for(i = 0; i < 10; i++) {
	pthread_create(&tid[i], NULL, handler, &i);
    }

    while(1);

    return 0;
}
