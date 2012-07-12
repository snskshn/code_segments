/*
 * msg.x: Remote message printing protocol
 */
 
program MESSAGEPROG {
        version MESSAGEVERS {
                int PRINTMESSAGE(string) = 1;
        } = 1;
} = 0x20000099;

