#include <stdio.h>
#include "list.h"

typedef struct _A
{
	int a;
	struct list_head list;
	int b;
} A;

int main()
{
	LIST_HEAD(head);

	A a1 = {.a = 1, .b = 2}, a2 = {.a = 3, .b = 4};

	INIT_LIST_HEAD(&a1.list);
	INIT_LIST_HEAD(&a2.list);

	list_add_tail(&a1.list, &head);
	list_add_tail(&a2.list, &head);
	list_add_tail(&a2.list, &head);

	A *p;

	list_for_each_entry(p, &head, list, A) {
		printf("%d %d\n", p->a, p->b);
	}

	puts("success");

	return 0;
}

