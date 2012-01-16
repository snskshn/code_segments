#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main()
{
    char answer[4] = {0};
    char guess[4] = {0};

    // make answer
    srand(time(NULL));
    snprintf(answer, 4, "%03d", rand() % 1000);
    printf("answer: %s\n", answer);

    // guess
    printf("input: ");
    scanf("%03s", guess);

    // decision
    int i, j;
    int strikes = 0, balls = 0;
    for(i = 0; i < 3; i++) {
	for(j = 0; j < 3; j++) {
	    if(answer[i] == guess[j]) {
		if(i == j) {
		    strikes++;
		}
		else {
		    balls++;
		}
	    }
	}
    }

    // output
    printf("[guess:%03s] [answer:%s] %d strikes, %d balls\n",
	    guess, answer, strikes, balls);


    return 0;
}
