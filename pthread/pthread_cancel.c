#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void cleanup(void *arg)
{
    printf("cleanup: %d\n", *(int *)arg);
}

void *func(void *arg)
{
    int thread_num = *(int *)arg;

    pthread_cleanup_push(cleanup, &thread_num);

    while(1) {
	sleep(1);
	printf("I'm thread %d\n", thread_num);
    }

    pthread_cleanup_pop(0);
}

int main()
{
    int arg[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    pthread_t tid[10];
    pthread_attr_t attr;
    int i;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    for(i = 0; i < 10; i++) {
	pthread_create(&tid[i], &attr, func, &arg[i]);
    }

    for(i = 0; i < 10; i++) {
	sleep(2);
	printf("canceling thread %d\n", i);
	pthread_cancel(tid[i]);
    }

    while(1);

    return 0;
}

