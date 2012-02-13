#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

int main()
{
    LIST_HEAD(listhead, entry) head;
    struct listhead *headp;                 /* List head. */
    struct entry {
	int a;
	int b;
	LIST_ENTRY(entry) entries;          /* List. */
    } *n1, *n2, *np;

    LIST_INIT(&head);                       /* Initialize the list. */

    n1 = malloc(sizeof(struct entry));      /* Insert at the head. */
    n1->a = 3;
    n1->b = 4;
    LIST_INSERT_HEAD(&head, n1, entries);

    n2 = malloc(sizeof(struct entry));      /* Insert after. */
    n2->a = 5;
    n2->b = 6;
    LIST_INSERT_AFTER(n1, n2, entries);

    struct entry *temp = LIST_FIRST(&head);
    temp = LIST_NEXT(temp, entries);
    if(temp == NULL) {
	temp = LIST_FIRST(&head);
    }
    temp = LIST_NEXT(temp, entries);
    if(temp == NULL) {
	temp = LIST_FIRST(&head);
    }
    temp = LIST_NEXT(temp, entries);
    if(temp == NULL) {
	temp = LIST_FIRST(&head);
    }
    temp = LIST_NEXT(temp, entries);
    if(temp == NULL) {
	temp = LIST_FIRST(&head);
    }

    printf("a:%d, b:%d\n", temp->a, temp->b);
#if 0

    /* Forward traversal. */
    for (np = head.lh_first; np != NULL; np = np->entries.le_next) {
	printf("a:%d, b:%d\n", np->a, np->b);
    }

    while (head.lh_first != NULL) {          /* Delete. */
	LIST_REMOVE(head.lh_first, entries);
    }
#endif

    return 0;
}
