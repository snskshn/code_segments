#include <stdio.h>
#include <string.h>

int main()
{
    char *str = "aa_bb_cc_dd_ee_";
    char *c;

    c = str;

    c = strchr(++c, '_');
    printf("%s\n", c);
    c = strchr(++c, '_');
    printf("%s\n", c);
    c = strchr(++c, '_');
    printf("%s\n", c);
    c = strchr(++c, '_');
    printf("%s\n", c);

    return 0;
}
