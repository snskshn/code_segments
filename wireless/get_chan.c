#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>
#include <linux/nl80211.h>
#include <netlink/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <net/if.h>

static int bitrate_cb(struct nl_msg *msg, void *arg)
{
	int *rate = (int *)arg;

	struct nlattr *tb[NL80211_ATTR_MAX + 1];
	struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(msg));
	struct nlattr *sinfo[NL80211_STA_INFO_MAX + 1];
	struct nlattr *rinfo[NL80211_RATE_INFO_MAX + 1];

	static struct nla_policy stats_policy[NL80211_STA_INFO_MAX + 1] = {
		[NL80211_STA_INFO_TX_BITRATE] = { .type = NLA_NESTED },
	};

	static struct nla_policy rate_policy[NL80211_RATE_INFO_MAX + 1] = {
		[NL80211_RATE_INFO_BITRATE] = { .type = NLA_U16 },
		[NL80211_RATE_INFO_MCS] = { .type = NLA_U8 },
		[NL80211_RATE_INFO_40_MHZ_WIDTH] = { .type = NLA_FLAG },
		[NL80211_RATE_INFO_SHORT_GI] = { .type = NLA_FLAG },
	};

	nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0),
			genlmsg_attrlen(gnlh, 0), NULL);

	if (!tb[NL80211_ATTR_STA_INFO]) {
		*rate = 0;
		return NL_STOP;
	}
	if (nla_parse_nested(sinfo, NL80211_STA_INFO_MAX,
				tb[NL80211_ATTR_STA_INFO],
				stats_policy)) {
		*rate = 0;
		return NL_STOP;
	}

	if (sinfo[NL80211_STA_INFO_TX_BITRATE]) {
		if (nla_parse_nested(rinfo, NL80211_RATE_INFO_MAX,
					sinfo[NL80211_STA_INFO_TX_BITRATE], rate_policy)) {
			*rate = 0;
		} else {
			if (rinfo[NL80211_RATE_INFO_BITRATE]) {
				*rate = nla_get_u16(rinfo[NL80211_RATE_INFO_BITRATE]) * 100;
			}
		}
	}
	return NL_STOP;
}

int main(int argc, char *argv[])
{
	struct nl_sock *sock;
	struct nl_msg *msg;
	int family;
	int rate = 3;

	if (argc != 2) {
		printf("usage: %s dev\n", argv[0]);
		exit(EXIT_SUCCESS);
	}

	sock = nl_socket_alloc();
	genl_connect(sock);
	family = genl_ctrl_resolve(sock, "nl80211");
	msg = nlmsg_alloc();

	genlmsg_put(msg, NL_AUTO_PID, NL_AUTO_SEQ, family, 0, 0,
			NL80211_CMD_GET_STATION, 0);
	nla_put_u32(msg, NL80211_ATTR_IFINDEX, if_nametoindex(argv[1]));
	//nla_put_u32(msg, NL80211_ATTR_WIPHY_FREQ, 2417);
	//nla_put_u32(msg, NL80211_ATTR_WIPHY_CHANNEL_TYPE, NL80211_CHAN_HT20);

	if (nl_send_auto_complete(sock, msg) < 0)
		goto cleanup;

	nl_socket_modify_cb(sock, NL_CB_VALID, NL_CB_CUSTOM, bitrate_cb, &rate);
	nl_recvmsgs_default(sock);

	printf("%d\n", rate);

cleanup:
	nlmsg_free(msg);
	nl_socket_free(sock);

	return 0;
}
