%{
    enum {
	NONE = 0,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
    };

    int number;
    void action();
%}

%%
oneone	{ number = SEVEN; action(); }
one |
ONE	{ number = ONE; action(); }
two |
TWO	{ number = TWO; action(); }
three |
THREE	{ number = THREE; action(); }
four |
FOUR	{ number = FOUR; action(); }
five |
FIVE	{ number = FIVE; action(); }
six |
SIX	{ number = SIX; action(); }
[a-zA-Z0-9]+	{ number = NONE; action(); }
%%

void action()
{
    if(number != NONE) {
	printf("%s is %d\n", yytext, number);
    }
    else {
	printf("%s?? I don't learn it.\n", yytext);
    }
}

int main()
{
    yylex();

    return 0;
}


