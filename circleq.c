#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

int main()
{
    CIRCLEQ_HEAD(circleq, entry) head;
    struct circleq *headp;              /* Circular queue head. */
    struct entry {
	int a;
	int b;
	    CIRCLEQ_ENTRY(entry) entries;   /* Circular queue. */
    } *n1, *n2, *np;

    CIRCLEQ_INIT(&head);                /* Initialize the circular queue. */

    n1 = malloc(sizeof(struct entry));  /* Insert at the head. */
    n1->a = 3;
    n1->b = 4;
    CIRCLEQ_INSERT_HEAD(&head, n1, entries);

    n1 = malloc(sizeof(struct entry));  /* Insert at the tail. */
    n1->a = 5;
    n1->b = 6;
    CIRCLEQ_INSERT_TAIL(&head, n1, entries);

    n2 = malloc(sizeof(struct entry));  /* Insert after. */
    n2->a = 7;
    n2->b = 8;
    CIRCLEQ_INSERT_AFTER(&head, n1, n2, entries);

    n2 = malloc(sizeof(struct entry));  /* Insert before. */
    CIRCLEQ_INSERT_BEFORE(&head, n1, n2, entries);
    /* Forward traversal. */
    for (np = head.cqh_first; np != (void *)&head;
	    np = np->entries.cqe_next) {
	printf("a:%d, b:%d\n", np->a, np->b);
    }

    /* Reverse traversal. */
    for (np = head.cqh_last; np != (void *)&head; np = np->entries.cqe_prev) {
	printf("a:%d, b:%d\n", np->a, np->b);
    }
    /* Delete. */
    while (head.cqh_first != (void *)&head)
	CIRCLEQ_REMOVE(&head, head.cqh_first, entries);

    return 0;
}
