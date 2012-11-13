#include <iostream>
#include <queue>

using namespace std;

extern "C" queue<int> *CreateQueue()
{
	return new queue<int>;
}

extern "C" void Push(queue<int> *q, int value)
{
	q->push(value);
}

extern "C" int Pop(queue<int> *q)
{
	int rc = q->front();
	q->pop();
	return rc;
}

extern "C" void DeleteQueue(queue<int> *q)
{
	delete q;
}


