#include "square.h"

int main(int argc, char **argv)
{
	CLIENT *cl;
	square_in in;
	square_out *outp;

	if (argc != 3)
		clnt_perror(cl, "usage: client <hostname> <interger_value>");
	cl = clnt_create(argv[1], SQUARE_PROG, SQUAREPROG, "tcp");
	in.arg1 = (long)atol(argv[2]);
	if ((outp = square_proc_1(&in, cl)) == NULL)
		clnt_perror(cl, argv[1]);
	printf("result: %ld\n", outp->res1);

	return 0;
}
