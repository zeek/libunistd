// sys/socket.h

#ifndef sys_socket_h
#define sys_socket_h

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
// Windows SDKs (Vista → 11) changed rule, include windows.h first:
// <windows.h> no longer includes <winsock.h>
#include <windows.h>
#include <winsock2.h>
#include <Mstcpip.h>
#include <Ws2tcpip.h>
#undef VOID
#include <stdint.h>
#include "stub.h"
#include "cfunc.h"
#include "uio.h"
#include "posix_types.h"

struct unix_socket {
    int is_server;
    char path[UNIX_PATH_MAX];
    HANDLE handle;
};

typedef uint32_t sa_family_t;

struct msghdr
{	void* msg_name;
	socklen_t msg_namelen;
	struct iovec* msg_iov;
	int msg_iovlen;
	void* msg_control;
	socklen_t msg_controllen;
	int msg_flags;
};

typedef uint8_t* caddr_t;

// ssize_t send(int sockfd, const void *buf, size_t size, int flags)

// The ioctlsocket function and the WSAIoctl function handle socket functions that were performed by IOCTL and fcntl in BSD

inline int inet_aton(const char* cp, struct in_addr* inp)
{	return inet_pton(AF_INET, cp, inp);
}

/* FYI, how to do TCP_KEEPCNT in linux/windows:
#ifndef _WIN32
    int count = 10;
    int optlen = sizeof( int );
    //Set the time before a keepalive is sent to 60 seconds
    setsockopt( sockFD, SOL_TCP, TCP_KEEPIDLE, &idle, optlen );
    //Set the time between keepalive packets to 5 seconds
    setsockopt( sockFD, SOL_TCP, TCP_KEEPINTVL, &intvl, optlen );
    //Set the number of packets before connection is closed to 5
    setsockopt( sockFD, SOL_TCP, TCP_KEEPCNT, &count, optlen );
#endif
#ifdef _WIN32
    //WinSock structure for KeepAlive timing settings
    struct tcp_keepalive settings;
    settings.onoff = 1;
    settings.keepalivetime = idle * 1000;
    settings.keepaliveinterval = intvl * 1000;

    //Random, useless pointers for WinSock call
    DWORD bytesReturned;
    WSAOVERLAPPED overlapped;
    overlapped.hEvent = NULL;
    //Set KeepAlive settings -- I HATE WINSOCK
    WSAIoctl(
        sockFD,
        SIO_KEEPALIVE_VALS,
        &settings,
        sizeof( struct tcp_keepalive ),
        NULL,
        0,
        &bytesReturned,
        &overlapped,
        NULL
    );
#endif
*/

#endif
