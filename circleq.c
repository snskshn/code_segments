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
    } *n1, *n2, *n3, *np, lookup_table[10];

    CIRCLEQ_INIT(&head);                /* Initialize the circular queue. */

    // filling lookup_table
    int i;
    for(i = 0; i < 10; i++) {
	lookup_table[i].a = lookup_table[i].b = i + 10;
	CIRCLEQ_INSERT_TAIL(&head, &lookup_table[i], entries);
    }

    n1 = malloc(sizeof(struct entry));
    n1->a = 1;
    n1->b = 1;
    CIRCLEQ_INSERT_TAIL(&head, n1, entries);

    n2 = malloc(sizeof(struct entry));
    n2->a = 2;
    n2->b = 2;
    CIRCLEQ_INSERT_TAIL(&head, n2, entries);

    n3 = malloc(sizeof(struct entry));
    n3->a = 3;
    n3->b = 3;
    CIRCLEQ_INSERT_TAIL(&head, n3, entries);

    struct entry *p = CIRCLEQ_FIRST(&head);
    p = CIRCLEQ_LOOP_NEXT(&head, n1, entries);
    //printf("a:%d, b:%d\n", p->a, p->b);
    p = CIRCLEQ_LOOP_NEXT(&head, n2, entries);
    //printf("a:%d, b:%d\n", p->a, p->b);
    p = CIRCLEQ_LOOP_NEXT(&head, n3, entries);
    //printf("a:%d, b:%d\n", p->a, p->b);
    printf("orig: %p, %p\n", lookup_table[5].entries.cqe_next,
	    lookup_table[5].entries.cqe_prev);
    CIRCLEQ_REMOVE(&head, &lookup_table[5], entries);
    printf("after: %p, %p\n", lookup_table[5].entries.cqe_next,
	    lookup_table[5].entries.cqe_prev);

    for(i = 0; i < 30; i++) {
	p = CIRCLEQ_LOOP_NEXT(&head, p, entries);
	printf("a:%d, b:%d\n", p->a, p->b);
    }

    puts("---------");



#if 0
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
#endif
    return 0;
}
