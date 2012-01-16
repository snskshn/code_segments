#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ELETYPE int
ELETYPE *ar;
unsigned size;
unsigned num;
unsigned growby;

void init_array(unsigned asize, unsigned agrowby)
{
    size = asize;
    growby = agrowby;
    num = 0;
    ar = (ELETYPE *)malloc(size * sizeof(ELETYPE));
}

void insert(int idx, ELETYPE value)
{
    unsigned need;

    need = num + 1;
    if(need > size) {
	size = need + growby;
	ar = (ELETYPE *)realloc(ar, size * sizeof(ELETYPE));
    }
    memmove(ar + idx + 1, ar + idx, (num - idx)*sizeof(ELETYPE));
    ar[idx] = value;
    num++;
}

void delete(int idx)
{
    memmove(ar + idx, ar + idx + 1, (num - idx - 1)*sizeof(ELETYPE));
    num--;
}

void append(ELETYPE value)
{
    insert(num, value);
}

void uninit_array()
{
    free(ar);
}

void dump_array(char *s_mark)
{
    unsigned i;
    printf("%16s => size=%02d, num=%02d : ", s_mark, size, num);
    for(i = 0; i < num; i++) {
	printf("%2d ", ar[i]);
    }
    printf("\n");
}

int main()
{
    int i;

    init_array(10, 5); dump_array("initial");
    for(i = 1; i <= 8; i++) {
	append(i);
    }
    dump_array("append 8");
    insert(3, 10); dump_array("insert 10");
    insert(3, 11); dump_array("insert 11");
    insert(3, 12); dump_array("insert 12");
    delete(7); dump_array("delete 7");

    uninit_array();

    return 0;
}
