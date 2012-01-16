#include <stdio.h>

#define _(x) #x
char *print_frame_type(int type, int subtype)
{
    static char *types[4][16] = {
	{
	    _(ASSOC_REQUEST),
	    _(ASSOC_RESPONSE),
	    _(REASSOC_REQUEST),
	    _(REASSOC_RESPONSE),
	    _(PROBE_REQUEST),
	    _(PROBE_RESPONSE),
	    _(RESERVED),
	    _(RESERVED),
	    _(BEACON),
	    _(ATIM),
	    _(DISASSOC),
	    _(AUTH),
	    _(DEAUTH),
	    _(ACTION),
	    _(ACTION_NO_ACK),
	    _(RESERVED),
	},
	{
	    _(RESERVED),
	    _(RESERVED),
	    _(RESERVED),
	    _(RESERVED),
	    _(RESERVED),
	    _(RESERVED),
	    _(RESERVED),
	    _(CONTROL_WRAPPER),
	    _(BAR),
	    _(BA),
	    _(PS_POLL),
	    _(RTS),
	    _(CTS),
	    _(ACK),
	    _(CF_END),
	    _(END_ACK),
	},
	{
	    _(DATA),
	    _(DATA_CF_ACK),
	    _(DATA_CF_POLL),
	    _(DATA_CF_ACK_POLL),
	    _(NODATA),
	    _(NODATA_CF_ACK),
	    _(NODATA_CF_POLL),
	    _(NODATA_CF_ACK_POLL),
	    _(QOS_DATA),
	    _(QOS_DATA_CF_ACK),
	    _(QOS_DATA_CF_POLL),
	    _(QOS_DATA_CF_ACK_POLL),
	    _(QOS_NODATA),
	    _(RESERVED),
	    _(QOS_CF_POLL_NODATA),
	    _(QOS_CF_ACK_POLL_NODATA),
	},
	{
	    _(RESERVED),
	    _(RESERVED),
	    _(RESERVED),
	    _(RESERVED),
	    _(RESERVED),
	    _(RESERVED),
	    _(RESERVED),
	    _(RESERVED),
	    _(RESERVED),
	    _(RESERVED),
	    _(RESERVED),
	    _(RESERVED),
	    _(RESERVED),
	    _(RESERVED),
	    _(RESERVED),
	    _(RESERVED),
	}};

    return types[type][subtype];
}

int main()
{
    int i;

    for(i = 0; i < 63; i++) {
	printf("%s\n", print_frame_type(i / 16, i % 16));
    }

    return 0;
}
