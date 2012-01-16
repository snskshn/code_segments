#define paster(n) printf("token" #n " = %d\n", token##n)
#define stringer(x) printf(#x "\n")
#define array_cnt(x) sizeof(x) / sizeof(x[0])
#define str(x) #x

enum {
    SIZE,
};

#include <stdio.h>

int main()
{
    printf("%s\n", str(SIZE));
    int token9 = 9;
    paster(9);
    stringer(In quotes in the printf function call\n);

    int array[20];
    printf("%d\n", array_cnt(array));

    return 0;
}
