#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#ifndef WIN32
# define _snprintf(...) snprintf(__VA_ARGS__)
# define Sleep(t) usleep(1000 * t)
# define CALLBACK
# define SOCKET int
# define SOCKADDR_IN struct sockaddr_in
# define closesocket(sd) close(sd)
# define recvfrom(s, buf, len, flags, from, fromlen) \
    recvfrom((int)s, (void *)buf, (size_t)len, flags, from, (socklen_t *)fromlen)
#else
#pragma warning(disable:4127; disable:4100; disable:4127)
#endif

#endif	/* __PLATFORM_H__ */
