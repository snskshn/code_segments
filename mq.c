#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <sys/mman.h>
#include <errno.h>

static const char *MQ = "/my_mq";
typedef struct
{
	int len;
	char *ptr;
} MSG;

void *writer(void *arg)
{
	char *filename;
	int fd;
	struct stat buf;
	char *content;
	mqd_t mqdes;
	MSG msg;
	char errbuf[128];

	filename = (char *)arg;
	fd = open(filename, O_RDONLY);
	mqdes = mq_open(MQ, O_WRONLY);

	stat(filename, &buf);
	content = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	msg.len = buf.st_size;
	msg.ptr = content;

	if (mq_send(mqdes, (char *)&msg, sizeof(msg), 0) == -1)
		printf("[%s] mqdes: %d, error: %s\n", __FUNCTION__, mqdes,
				strerror_r(errno, errbuf, sizeof(errbuf)));

	mq_close(mqdes);
	close(fd);
	return NULL;
}

void *reader(void *arg)
{
	char *filename;
	int fd;
	mqd_t mqdes;
	MSG msg;
	char errbuf[128];

	sleep(1);

	filename = (char *)arg;
	fd = open(filename, O_WRONLY | O_CREAT);
	mqdes = mq_open(MQ, O_RDONLY);

	if (mq_receive(mqdes, (char *)&msg, sizeof(msg), 0) == -1)
		printf("[%s] mqdes: %d, error: %s\n", __FUNCTION__, mqdes,
				strerror_r(errno, errbuf, sizeof(errbuf)));
	//write(fd, msg.ptr, msg.len);
	printf("ptr: %s, len: %d\n", msg.ptr, msg.len);

	mq_close(mqdes);
	munmap(msg.ptr, msg.len);
	close(fd);
	return NULL;
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "%s src dst\n", argv[0]);
		exit(1);
	}

	if (!strcmp(argv[1], argv[2])) {
		fprintf(stderr, "src == dst\n");
		exit(1);
	}

	pthread_t t[2];
	struct mq_attr attr = {.mq_maxmsg = 40, .mq_msgsize = 8};

	mq_unlink(MQ);
	mq_close(mq_open(MQ, O_CREAT, 0660, &attr));

	pthread_create(&t[0], NULL, writer, argv[1]);
	pthread_create(&t[1], NULL, reader, argv[2]);

	pthread_join(t[1], NULL);
	pthread_join(t[0], NULL);

	return 0;
}
