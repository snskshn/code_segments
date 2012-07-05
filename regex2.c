#include <sys/types.h>
#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>

#define ALPHA "[[:alpha:]]"
#define NUM "[[:digit:]]"
#define SPECIAL "[[:punct:]]"
//#define REGEXP "^\\(?=.*[[:alpha:]]\\)\\(?=.*[[:digit:]]\\)\\(?=.*[[:punct:]]\\).\\{6,50\\}$"
//#define REGEXP "\\(?=\\)\\(?=match this too\\)\\(?=oh, and this\\)"
#define STRONG "^\\(?=.*\\[A-Z\\]\\)\\(?=.*\\[a-z\\]\\)\\(?=.*\\[0-9\\]\\).\\{6,50\\}$"
#define LONGER ".{6,}"

int main(int argc, char *argv[])
{
    regex_t rx;

    if (argc != 2) {
	    printf("%s string\n", argv[0]);
	    return 0;
    }

    int rc;
    char errbuf[1024];
    //rc = regcomp(&rx, REGEXP, REG_EXTENDED);
    rc = regcomp(&rx, LONGER, REG_EXTENDED);
    regerror(rc, &rx, errbuf, 1024);
    printf("%s\n", errbuf);

    regmatch_t *match;
    match = malloc(strlen(argv[1]) * sizeof(regmatch_t) + 1);
    printf("%d\n", regexec(&rx, argv[1], strlen(argv[1]), match, 0));

    free(match);
    return 0;
}

