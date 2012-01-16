#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv, char **env)
{
    int i;

    for(i = 0; i < argc; i++) {
	printf("%d: %s\n", i, argv[i]);
    }

    i = 0;
    while(1) {
	if(env[i] == NULL) {
	    break;
	}
	printf("%s\n", env[i++]);
    }


    int test[3] = {1, 2, 3};
    printf("%d\n", 2[test]);
    test[2] == *(test + 2);
    2[test] == *(2 + test);

    struct {
	int i;
    } test2[10];

    return 0;
}
