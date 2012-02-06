#include <stdio.h>
#include "list.h"

struct people 
{
    int age; 
    char name[20]; 
    struct list_head list; 
}; 

int main()
{
    //struct list_head head = { &head, &head };
    LIST_HEAD(head); 
    struct people p1 = { 34, "kim", }; 
    struct people p2 = { 22, "park", }; 
    struct people p3 = { 23, "park", }; 
    struct people p4 = { 24, "park", }; 
    struct people p5 = { 25, "park", }; 
    struct people p6 = { 26, "park", }; 
    struct list_head *temp; 
    struct people *p; 

    list_add_tail( &p1.list , &head ); 
    list_add_tail( &p2.list , &head ); 
    list_add_tail( &p3.list , &head ); 
    list_add_tail( &p4.list , &head ); 
    list_add_tail( &p5.list , &head ); 
    list_add_tail( &p6.list , &head ); 

    printf("%p\n", &head);

    //for( temp = head.prev; temp != &head; temp = temp->prev )
    list_for_each( temp, &p6.list )
    {
	p = list_entry( temp, struct people, list); 
	printf("name:%s, age:%d\n", p->name, p->age ); 
    }

    return 0; 
}

#if 0
int main()
{
    struct people p1 = { 34, "kim", }; 

    printf("%d\n", &((struct people*)0)->list ); 

    printf("name:%s\n", 
	    ((struct people*)
	     ((char *)&p1.list - (int)&((struct people*)0)->list))->name); 

    printf("name:%s\n", 
	    list_entry(&p1.list, struct people, list)->name ); 

    return 0; 
}
#endif
