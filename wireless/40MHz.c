#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>
#include <linux/nl80211.h>
#include <netlink/socket.h>
#include <net/if.h>

int main()
{
	struct nl_sock *sock;
	struct nl_msg *msg;
	int family;

	sock = nl_socket_alloc();
	genl_connect(sock);
	family = genl_ctrl_resolve(sock, "nl80211");
	msg = nlmsg_alloc();

	genlmsg_put(msg, NL_AUTO_PID, NL_AUTO_SEQ, family, 0, 0,
			NL80211_CMD_SET_WIPHY, 0);
	nla_put_u32(msg, NL80211_ATTR_IFINDEX, if_nametoindex("wlan0"));
	nla_put_u32(msg, NL80211_ATTR_WIPHY_FREQ, 2417);
	nla_put_u32(msg, NL80211_ATTR_WIPHY_CHANNEL_TYPE, NL80211_CHAN_HT20);

	nl_send_auto_complete(sock, msg);
	nl_recvmsgs_default(sock);

	if (msg)
		nlmsg_free(msg);

	if (sock)
		nl_socket_free(sock);

	return 0;
}
