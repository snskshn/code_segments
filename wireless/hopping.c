#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_ether.h>
#include <linux/if.h>
#include <linux/wireless.h>

int main(int argc, char *argv[])
{
	static struct iwreq iwr;
	int fd, i;
	/* 1 ~ 11 */
	static const int chans[] = {2412, 2417, 2422, 2427, 2432, 2437, 2442,
		2447, 2452, 2457, 2462};

	fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	strncpy(iwr.ifr_name, "wlan0", IFNAMSIZ);
	iwr.u.freq.e = 6;

	while (1) {
		iwr.u.freq.m = chans[i++ % (sizeof(chans) >> 2)];
		if (ioctl(fd, SIOCSIWFREQ, &iwr) < 0)
			perror("ioctl set");
		usleep(atoi(argv[1]));
	}

	return 0;
}
