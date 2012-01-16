#include <stdio.h>

void func(int (*ar)[3], int size)
{
    int i, j;

    for(i = 0; i < size; i++) {
	for(j = 0; j < 3; j++) {
	    printf("ar[%d][%d] = %d\n", i, j, ar[i][j]);
	}
    }
    puts("");
}

int main()
{
    int ar1[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int ar2[3][3] = {{7, 8, 9}, {10, 11, 12}, {13, 14, 15}};

    func(ar1, 2);
    func(ar2, 3);

    return 0;
}


