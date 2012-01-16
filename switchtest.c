#include <stdio.h>

int main()
{
    int DOS = 0x00010;

    int test = 0x00011;

    switch(test) {
	case test & DOS:
	    printf("success\n");
	    break;
    }

    return 0;
}
