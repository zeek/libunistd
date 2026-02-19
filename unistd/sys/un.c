// sys/un.c
// Copyright 3 Feb 2026 Robin.Rowe@CinePaint.org
// License Open Source MIT

#include <socket.h>
#include "sys_un.h"

int af_socket(int domain, int type, int protocol) {
    if (domain == AF_UNIX) {
        int fd = allocate_fd();
        struct unix_socket *u = &unix_table[fd];
        memset(u, 0, sizeof(*u));
        u->is_server = 0;
        u->handle = INVALID_HANDLE_VALUE;
        return fd;
    }
    return _socket(domain, type, protocol);
}

int af_bind(int fd, const struct sockaddr *sa, socklen_t len) {
    if (sa->sa_family != AF_UNIX)
        return real_bind(fd, sa, len);

    struct unix_socket *u = &unix_table[fd];
    const struct sockaddr_un *sun = (const struct sockaddr_un *)sa;

    snprintf(u->path, UNIX_PATH_MAX, "\\\\.\\pipe\\%s", sun->sun_path);
    u->is_server = 1;

    return 0;
}

int af_listen(int fd, int backlog) {
    struct unix_socket *u = &unix_table[fd];

    if (!u->is_server)
        return -1;

    u->handle = CreateNamedPipeA(
        u->path,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        65536, 65536,
        0,
        NULL
    );

    return (u->handle == INVALID_HANDLE_VALUE) ? -1 : 0;
}

int af_accept(int fd, struct sockaddr *sa, socklen_t *len) {
    struct unix_socket *u = &unix_table[fd];

    if (!u->is_server)
        return -1;

    if (!ConnectNamedPipe(u->handle, NULL)) {
        if (GetLastError() != ERROR_PIPE_CONNECTED)
            return -1;
    }

    int newfd = allocate_fd();
    struct unix_socket *c = &unix_table[newfd];
    memset(c, 0, sizeof(*c));
    c->handle = u->handle;

    return newfd;
}

int af_connect(int fd, const struct sockaddr *sa, socklen_t len) {
    const struct sockaddr_un *sun = (const struct sockaddr_un *)sa;

    char pipename[UNIX_PATH_MAX + 10];
    snprintf(pipename, sizeof(pipename), "\\\\.\\pipe\\%s", sun->sun_path);

    HANDLE h = CreateFileA(
        pipename,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (h == INVALID_HANDLE_VALUE)
        return -1;

    struct unix_socket *u = &unix_table[fd];
    u->handle = h;

    return 0;
}

ssize_t af_write(int fd, const void *buf, size_t len) {
    struct unix_socket *u = &unix_table[fd];
    DWORD written = 0;

    if (!WriteFile(u->handle, buf, (DWORD)len, &written, NULL))
        return -1;

    return written;
}

ssize_t af_read(int fd, void *buf, size_t len) {
    struct unix_socket *u = &unix_table[fd];
    DWORD readn = 0;

    if (!ReadFile(u->handle, buf, (DWORD)len, &readn, NULL))
        return -1;

    return readn;
}