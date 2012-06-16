#include <stdio.h>

#define FSM		for(;;)
#define STATE(x)	x##_s
#define NEXTSTATE(x)	goto x##_s

int main()
{
	FSM {
STATE(s1):
		puts("state 1");
		NEXTSTATE(s2);

STATE(s2):
		puts("state 2");
		NEXTSTATE(s3);

STATE(s3):
		puts("state 3");
		break;
	}

	return 0;
}
