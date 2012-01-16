#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    int answer;
    int guess;
    int count = 0;

    // make answer
    srand(time(NULL));
    answer = rand() % 1000;

    // guess
    printf("input:");
    scanf("%d", &guess);
    if(guess <= 0 || guess >= 1000) {
	printf("invalid input\n");
	return -1;
    }

    // decision
    if(answer / 100 == guess / 100) count++;
    if(answer / 10 == guess / 10) count++;
    if(answer / 1 == guess / 1) count++;

    // output
    printf("[guess:%03d] [answer:%d] %d strikes, %d balls\n",
	    guess, answer, count, 3 - count);

    return 0;
}
