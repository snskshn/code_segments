#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <pthread.h>

int main()
{
    pthread_rwlock_t test;
    pthread_rwlock_t buf;	// ground truth

    pthread_rwlock_init(&test, NULL);
    memcpy(&buf, &test, sizeof(buf));

    pthread_rwlock_wrlock(&test);
    if(memcmp(&buf, &test, sizeof(buf) - offsetof(pthread_rwlock_t, __data.__flags))) {
	puts("differ after rwlock");

#if 0
	int i;
	for(i = 0; i < 56; i++) {
	    printf("%x->%x\n", test.__size[i], buf.__size[i]);
	}
	//printf("%x->%x\n", test.__align, buf.__align);
#endif
    }

    pthread_rwlock_unlock(&test);
    if(memcmp(&buf, &test, sizeof(buf) - offsetof(pthread_rwlock_t, __data.__flags))) {
	puts("differ after unlock");
    }

    pthread_rwlock_rdlock(&test);
    if(memcmp(&buf, &test, sizeof(buf) - offsetof(pthread_rwlock_t, __data.__flags))) {
	puts("differ after rdlock");
    }

    return 0;
}
