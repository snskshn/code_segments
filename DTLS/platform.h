#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#ifndef WIN32
# define DIRSEP "/"
# define CP "cp"
# define _mkdir(filename) mkdir(filename, 0755)
# define _snprintf(...) snprintf(__VA_ARGS__)
# define iswalpah(ch) isalpha(ch)
# define iswupper(ch) isupper(ch)
# define Sleep(t) usleep(1000 * t)
# define CALLBACK
# define SOCKET int
# define SOCKADDR_IN struct sockaddr_in
# define INVALID_SOCKET (-1)
# define SOCKET_ERROR (-1)
# define setsockopt(sockfd, level, optname, optval, optlen) \
    setsockopt((int)sockfd, level, optname, (void *)optval, (socklen_t)optlen)
# define closesocket(sd) close(sd)
# define recvfrom(s, buf, len, flags, from, fromlen) \
    recvfrom((int)s, (void *)buf, (size_t)len, flags, from, (socklen_t *)fromlen)
#else
# define DIRSEP "\\"
# define CP "copy"
# pragma warning(disable:4127; disable:4100; disable:4127)
#endif

#endif	/* __PLATFORM_H__ */
