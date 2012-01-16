#include <stdio.h>

int main()
{
    //unsigned int test:1, test2:1, test3:1, test4:1;

    struct test
    {
	int wlan_wep:4,
	wlan_retry:2,
	wlan_retry2:3,
	wlan_retry3:29;
    };


    //test.wlan_retry8 = 2;

    //printf("%d\n", test.wlan_retry8);

    return 0;
}
