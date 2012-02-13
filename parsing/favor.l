%{
    /*
     * My favorite...
     */
%}
%%

[\t ]+	;
rain |
rose	{ printf("%s!! Tha is my favorite.\n", yytext); }
love |
story	{ printf("%s!! I like a love story. ^_~\n", yytext); }
"potato chip" |
potato |
chip	{ printf("%s...!! now and then, I have a potato chip.\n", yytext); }

[A-Za-z]+ { printf("%s..., what is that?\n", yytext); }

.|\n	{ ECHO; }

%%

int main()
{
    yylex();

    return 0;
}
