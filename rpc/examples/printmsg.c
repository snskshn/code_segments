/* printmsg.c: print a message on the console */
#include <stdio.h>
 
main(argc, argv)
        int argc;
        char *argv[];
{
        char *message;
 
        if (argc != 2) {
                fprintf(stderr, "usage: %s <message>\n", argv[0]);
                exit (1);
        }
        message = argv[1];
 
        if (!printmessage(message)) {
                fprintf(stderr, "%s: couldn't print your message\n",
                        argv[0]);
                exit (1);
        }
        printf("Message Delivered!\n");
        exit (0);
}
/*
 * Print a message to the console.  Return a Boolean showing result.
 */
printmessage(msg)
        char *msg;
{
        FILE *f;
 
        f = fopen("/dev/console", "w");
        if (f == NULL) {
                return (0);
        }
        fprintf(f, "%s\n", msg);
        fclose(f);
        return (1);
}


