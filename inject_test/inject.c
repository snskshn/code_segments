#include <stdio.h>
#include <pcap.h>
#include <unistd.h>
#include <byteswap.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/ether.h>

#include "ieee802_11.h"
#include "ieee802_11_radio.h"

#define BUF_SIZE 100

pcap_t *init_pcap(char *dev)
{
    char errbuf[BUF_SIZE];
    pcap_t *fd;

    if(!(fd = pcap_create(dev, errbuf))) {
        perror("pcap_create");
    }   
    pcap_activate(fd);

    return fd;
}

// copy frame contents to buf and return its length
int make_beacon(unsigned char *buf, unsigned char *da, unsigned char *sa, unsigned char *bssid)
{
    static struct ieee80211_radiotap_header radiotap_header = {0};
    static struct mgmt_header_t beacon_header = {0};

    radiotap_header.it_len = bswap_16(htons(8));        // convert to little endian
    beacon_header.fc = 0x80;                            // no endian dependency (single byte)
    memcpy(beacon_header.da, da, 6); 
    memcpy(beacon_header.sa, sa, 6); 
    memcpy(beacon_header.bssid, bssid, 6); 

    // radiotap_header and beacon_header is aligned, so copy at once
    memcpy(buf, &radiotap_header, sizeof(radiotap_header) + sizeof(beacon_header));

    return sizeof(struct ieee80211_radiotap_header) + sizeof(struct mgmt_header_t);
}

// generate beacon infinitely. input argument is pcap descriptor.
void inject_beacon(pcap_t *fd)
{
    unsigned char buf[BUF_SIZE] = {0};
    int len;

    // make frame contents
    unsigned char da[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    unsigned char sa[6] = {0xf, 0xe, 0xd, 0xc, 0xb, 0xa};
    unsigned char bssid[6] = {0x22, 0x22, 0x33, 0x44, 0x55, 0x66};

    len = make_beacon(buf, da, sa, bssid);

    // inject main loop
    while(1) {
        sleep(1);
        if(pcap_inject(fd, buf, len) < 0) {
            perror("cannot write frame");
        }
	printf("writing beacon.. \n");
    }   
}

int main(int argc, char **argv)
{
    if(argc != 2) {
	printf("Usage: %s dev\n", argv[0]);
	exit(1);
    }

    pcap_t *fd;

    fd = init_pcap(argv[1]);
    inject_beacon(fd);

    return 0;
}
