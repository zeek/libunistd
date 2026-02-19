// sys/uio.c
// Copyright 3 Feb 2026 Robin.Rowe@CinePaint.org
// License Open Source MIT

#include "../unistd.h"
#include <io.h>        // _get_osfhandle
#include <errno.h>
#include "sys/uio.h"

#define DWORD_MAX ULONG_MAX

static HANDLE fd_to_handle(int fd) {
    intptr_t h = _get_osfhandle(fd);
    if (h == -1) {
        errno = EBADF;
        return INVALID_HANDLE_VALUE;
    }
    return (HANDLE)h;
}

ssize_t writev(int fd, const struct iovec *iov, int iovcnt) {
    if (iovcnt < 0) {
        errno = EINVAL;
        return -1;
    }

    HANDLE h = fd_to_handle(fd);
    if (h == INVALID_HANDLE_VALUE)
        return -1;

    ssize_t total = 0;

    for (int i = 0; i < iovcnt; i++) {
        const char *buf = (const char *)iov[i].iov_base;
        size_t len = iov[i].iov_len;

        while (len > 0) {
            DWORD chunk = (DWORD)(len > (size_t)DWORD_MAX ? DWORD_MAX : len);
            DWORD written = 0;

            if (!WriteFile(h, buf, chunk, &written, NULL)) {
                if (total == 0)
                    return -1;
                return total;
            }

            if (written == 0) {
                // Should not happen, but avoid infinite loop
                return total;
            }

            buf   += written;
            len   -= written;
            total += written;
        }
    }

    return total;
}

ssize_t readv(int fd, const struct iovec *iov, int iovcnt) {
    if (iovcnt < 0) {
        errno = EINVAL;
        return -1;
    }

    HANDLE h = fd_to_handle(fd);
    if (h == INVALID_HANDLE_VALUE)
        return -1;

    ssize_t total = 0;

    for (int i = 0; i < iovcnt; i++) {
        char *buf = (char *)iov[i].iov_base;
        size_t len = iov[i].iov_len;

        while (len > 0) {
            DWORD chunk = (DWORD)(len > (size_t)DWORD_MAX ? DWORD_MAX : len);
            DWORD read = 0;

            if (!ReadFile(h, buf, chunk, &read, NULL)) {
                if (total == 0)
                    return -1;
                return total;
            }

            if (read == 0) {
                // EOF
                return total;
            }

            buf   += read;
            len   -= read;
            total += read;
        }
    }

    return total;
}