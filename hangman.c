#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

static char *dict[] = {
    "applejuice",
    "telephone",
    "future",
    "objectivec",
    "iphone",
    "graph",
};

int main()
{
    srand(time(NULL));

    int life = 7;		// 7 tries
    int idx = rand() % sizeof(dict) / sizeof(int);
    int len = strlen(dict[idx]);
    char *ans = dict[idx];
    char *c, input, guess[len + 1];	// VLA
    char ibuf[27] = {0}, lbuf[100];

    // init
    memset(guess, '_', len);
    guess[len] = 0;

    while(life) {
	// input
	printf("\n[%s] (life:%d)\n", guess, life);
	scanf("%c", &input);
	fgets(lbuf, 100, stdin);
	if(!islower(input)) {
	    continue;
	}

	// check duplicate
	if(strchr(ibuf, input)) {
	    continue;
	}

	// make input sequence
	ibuf[strlen(ibuf)] = input;

	// check answer
	if(!(c = strchr(ans, input))) {
	    life--;
	    continue;
	}

	// apply answer
	do {
	    guess[c - ans] = input;
	    c++;
	} while((c = strchr(c, input)));

	// success
	if(!strchr(guess, '_')) {
	    printf("[%s] congratulation!!\n", guess);
	    exit(0);
	}
    }
    printf("failed..\n");

    return 0;
}
