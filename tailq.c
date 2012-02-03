#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

int main()
{
    TAILQ_HEAD(tailhead, entry) head;
    struct tailhead *headp;                 /* Tail queue head. */
    struct entry {
	int a;
	int b;
	    TAILQ_ENTRY(entry) entries;         /* Tail queue. */
    } *n1, *n2, *np;

    TAILQ_INIT(&head);                      /* Initialize the queue. */

    n1 = malloc(sizeof(struct entry));      /* Insert at the head. */
    n1->a = 3;
    n1->b = 4;
    TAILQ_INSERT_HEAD(&head, n1, entries);

    n1 = malloc(sizeof(struct entry));      /* Insert at the tail. */
    n1->a = 5;
    n1->b = 6;
    TAILQ_INSERT_TAIL(&head, n1, entries);

    n2 = malloc(sizeof(struct entry));      /* Insert after. */
    n2->a = 7;
    n2->b = 8;
    TAILQ_INSERT_AFTER(&head, n1, n2, entries);
    /* Forward traversal. */
    for (np = head.tqh_first; np != NULL; np = np->entries.tqe_next) {
	printf("a:%d, b:%d\n", np->a, np->b);
    }
    /* Delete. */
    while (head.tqh_first != NULL)
	TAILQ_REMOVE(&head, head.tqh_first, entries);

    return 0;
}
