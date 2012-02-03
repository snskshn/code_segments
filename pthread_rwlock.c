#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

pthread_rwlock_t lock;

void *func(void* arg)
{
    int i = *(int *)arg;

    while(1) {
	pthread_rwlock_wrlock(&lock);
	printf("i'm %d\n", i);
	pthread_rwlock_unlock(&lock);
	sleep(1);
    }

    return NULL;
}

int main()
{
    pthread_t t[2];
    pthread_attr_t attr;
    int arg[2] = {1, 2};
    pthread_rwlock_init(&lock, NULL);

    // set the thread attribute to detached state
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);


	pthread_rwlock_wrlock(&lock);
    // make two detached state threads
    pthread_create(&t[0], &attr, func, &arg[0]);
    pthread_create(&t[1], &attr, func, &arg[1]);

    while(1);

    return 0;
}


#if 0
    pthread_rwlock_t test;
    pthread_rwlock_t buf;	// ground truth

    pthread_rwlock_init(&test, NULL);
    memcpy(&buf, &test, sizeof(buf));

    pthread_rwlock_wrlock(&test);
    if(memcmp(&buf, &test, sizeof(buf) - offsetof(pthread_rwlock_t, __data.__flags))) {
	puts("differ after rwlock");

	int i;
	for(i = 0; i < 56; i++) {
	    printf("%x->%x\n", test.__size[i], buf.__size[i]);
	}
	//printf("%x->%x\n", test.__align, buf.__align);
    }

    pthread_rwlock_unlock(&test);
    if(memcmp(&buf, &test, sizeof(buf) - offsetof(pthread_rwlock_t, __data.__flags))) {
	puts("differ after unlock");
    }

    pthread_rwlock_rdlock(&test);
    if(memcmp(&buf, &test, sizeof(buf) - offsetof(pthread_rwlock_t, __data.__flags))) {
	puts("differ after rdlock");
    }
#endif

