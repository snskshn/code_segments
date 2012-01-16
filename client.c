#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

#define BUF_SIZE 4096 
#define FILENAME "sensor_policy.xml"

typedef struct{
    uint16_t    version;
    uint16_t    type;
    uint16_t    errCode;
    int8_t      standby;
    int8_t      szEntityVersion;
    uint32_t    msgSeq;
    uint32_t    length;
    uint32_t    spi;
    unsigned char senderCID[16];
    unsigned char recipientCID[16];
    uint16_t    encAlgo;
    uint16_t    macAlgo;
}MSG_HEAD;

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_adr;

    if(argc != 3) {
	printf("Usage: %s <IP> <port>\n", argv[0]);
	exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
	error_handling("socket() error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) {
	error_handling("connect() error!");
    }
    else {
	puts("Connected..............");
    }
    // get file size
    struct stat file_info;
    stat(FILENAME, &file_info);

    // write header
    MSG_HEAD send_head;
    memset(&send_head, 'o', sizeof(MSG_HEAD));
    send_head.length = htonl(file_info.st_size + sizeof(MSG_HEAD));
    printf("file size: %d\n", file_info.st_size);
    printf("header length: %d\n", sizeof(MSG_HEAD));
    
    write(sock, &send_head, sizeof(MSG_HEAD));

    int fd = open(FILENAME, O_RDONLY);

    // write file
    while((str_len = read(fd, message, sizeof(message)))) {
	write(sock, message, str_len);
    }
    //shutdown(sock, SHUT_WR);
    str_len = read(sock, message, sizeof(message));

    MSG_HEAD head;
    memcpy(&head, message, sizeof(MSG_HEAD));

    printf("received length: %d\n", ntohl(head.length));
    printf("received type: %x\n", ntohs(head.type));
    printf("received errCode: %d\n", ntohs(head.errCode));

    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
