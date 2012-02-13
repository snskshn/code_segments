#include <sys/types.h>
#include <stdio.h>
#include <regex.h>

int main()
{
    regex_t rx;
    regmatch_t matches[3] = {{0}};

    regcomp(&rx, "[a-zA-Z]+[ \t]+[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+", REG_EXTENDED);

    printf("%d\n", regexec(&rx, "   SERVER_IP	 10.51.51.1", 3, matches, 0));

    printf("start offset: %d, end offset: %d\n", matches[0].rm_so, matches[0].rm_eo);
    printf("start offset: %d, end offset: %d\n", matches[1].rm_so, matches[1].rm_eo);
    printf("start offset: %d, end offset: %d\n", matches[2].rm_so, matches[2].rm_eo);

    return 0;
}

