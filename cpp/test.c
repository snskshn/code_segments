#include <stdio.h>

typedef struct queue queue;

queue *CreateQueue();
void Push(queue *q, int value);
int Pop(queue *q);
void DeleteQueue(queue *q);

int main()
{
	queue *q = CreateQueue();

	Push(q, 1);
	Push(q, 2);
	Push(q, 3);

	printf("%d\n", Pop(q));
	printf("%d\n", Pop(q));
	printf("%d\n", Pop(q));

	DeleteQueue(q);

	return 0;
}
