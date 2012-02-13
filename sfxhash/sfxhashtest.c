#include <stdio.h>
#include "sfxhash.h"

int main()
{
    SFXHASH *t;

    // create
    t = sfxhash_new(100, 20, 20, 0, 0, NULL, NULL, 1);
    if(t == NULL) {
	perror("sfxhash_new");
    }

    // add
    sfxhash_add(t, "one", "1");
    sfxhash_add(t, "two", "2");
    sfxhash_add(t, "three", "3");

    // find 
    char *result = sfxhash_find(t, "one");

    printf("%s\n", result);

    // delete
    sfxhash_delete(t);

    return 0;
}
