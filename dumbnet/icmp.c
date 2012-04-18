/* from: http://pastebin.com/kvnNRSyi */
#include <dumbnet.h>

int main()
{
    uint32_t length = IP_HDR_LEN + ICMP_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN;
    uint8_t packet[length];
    rand_t *r;
    struct addr sip, dip;
    ip_t *ip;

    if((r = rand_open()) == 0) {
	perror("rand_open()");
    }

    addr_pton("10.51.51.2", &sip);
    addr_pton("10.25.1.50", &dip);
    ip_pack_hdr(packet, 0, length, rand_uint16(r), 0, 128, IP_PROTO_ICMP,
	    sip.addr_ip, dip.addr_ip);
    icmp_pack_hdr(packet + IP_HDR_LEN, ICMP_UNREACH, ICMP_UNREACH_PORT);
    ip_checksum(packet, ntohs(((struct ip_hdr *)packet)->ip_len));
    ip_pack_hdr(packet + IP_HDR_LEN + ICMP_HDR_LEN, 0,
	    length - IP_HDR_LEN - ICMP_HDR_LEN, rand_uint16(r), 0, 128,
	    IP_PROTO_UDP, dip.addr_ip, sip.addr_ip);
    ip_checksum(packet + IP_HDR_LEN + ICMP_HDR_LEN,
	    ntohs(((struct ip_hdr *)(packet + IP_HDR_LEN + ICMP_HDR_LEN))->ip_len));
    udp_pack_hdr(packet + IP_HDR_LEN + ICMP_HDR_LEN + IP_HDR_LEN, 51949, 11111,
	    89);


    ip = ip_open();
    ip_send(ip, packet, sizeof(packet));

    return 0;
}
