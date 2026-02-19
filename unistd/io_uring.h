#ifndef MINI_IO_URING_H
#define MINI_IO_URING_H

#ifdef _WIN32

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
    IORING_OP_NOP = 0,
    IORING_OP_READ,
    IORING_OP_WRITE
};

struct io_uring_sqe {
    int op;
    HANDLE h;
    void *buf;
    unsigned nbytes;
    long long offset;
    unsigned long long user_data;
};

struct io_uring_cqe {
    int res;
    unsigned flags;
    unsigned long long user_data;
};

struct io_uring {
    HANDLE iocp;
    struct io_uring_sqe *sqes;
    int sq_size;
    int sq_count;

    struct io_uring_cqe cqe;
    int cqe_ready;
};

static inline int io_uring_queue_init(unsigned entries,
                                      struct io_uring *ring,
                                      unsigned flags)
{
    (void)flags;
    ring->iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (!ring->iocp) return -1;

    ring->sqes = (struct io_uring_sqe*)calloc(entries, sizeof(*ring->sqes));
    ring->sq_size = entries;
    ring->sq_count = 0;
    ring->cqe_ready = 0;
    return 0;
}

static inline void io_uring_queue_exit(struct io_uring *ring)
{
    if (ring->iocp) CloseHandle(ring->iocp);
    free(ring->sqes);
}

static inline struct io_uring_sqe *io_uring_get_sqe(struct io_uring *ring)
{
    if (ring->sq_count >= ring->sq_size)
        return NULL;
    return &ring->sqes[ring->sq_count++];
}

static inline void io_uring_prep_read(struct io_uring_sqe *sqe,
                                      int fd, void *buf,
                                      unsigned nbytes, long long offset)
{
    sqe->op = IORING_OP_READ;
    sqe->h = (HANDLE)_get_osfhandle(fd);
    sqe->buf = buf;
    sqe->nbytes = nbytes;
    sqe->offset = offset;
}

static inline void io_uring_prep_write(struct io_uring_sqe *sqe,
                                       int fd, const void *buf,
                                       unsigned nbytes, long long offset)
{
    sqe->op = IORING_OP_WRITE;
    sqe->h = (HANDLE)_get_osfhandle(fd);
    sqe->buf = (void*)buf;
    sqe->nbytes = nbytes;
    sqe->offset = offset;
}

static inline int io_uring_submit(struct io_uring *ring)
{
    for (int i = 0; i < ring->sq_count; i++) {
        struct io_uring_sqe *sqe = &ring->sqes[i];

        OVERLAPPED *ov = (OVERLAPPED*)calloc(1, sizeof(OVERLAPPED));
        LARGE_INTEGER off;
        off.QuadPart = sqe->offset;
        ov->Offset = off.LowPart;
        ov->OffsetHigh = off.HighPart;

        CreateIoCompletionPort(sqe->h, ring->iocp, 0, 0);

        BOOL ok;
        DWORD flags = 0;

        if (sqe->op == IORING_OP_READ) {
            ok = ReadFile(sqe->h, sqe->buf, sqe->nbytes, NULL, ov);
        } else if (sqe->op == IORING_OP_WRITE) {
            ok = WriteFile(sqe->h, sqe->buf, sqe->nbytes, NULL, ov);
        } else {
            free(ov);
            continue;
        }

        if (!ok && GetLastError() != ERROR_IO_PENDING) {
            ring->cqe.res = -(int)GetLastError();
            ring->cqe.user_data = sqe->user_data;
            ring->cqe_ready = 1;
            free(ov);
        }
    }

    ring->sq_count = 0;
    return 1;
}

static inline int io_uring_wait_cqe(struct io_uring *ring,
                                    struct io_uring_cqe **cqe_ptr)
{
    if (ring->cqe_ready) {
        *cqe_ptr = &ring->cqe;
        return 0;
    }

    DWORD bytes;
    ULONG_PTR key;
    OVERLAPPED *ovp;

    BOOL ok = GetQueuedCompletionStatus(ring->iocp,
                                        &bytes,
                                        &key,
                                        &ovp,
                                        INFINITE);

    if (!ok)
        ring->cqe.res = -(int)GetLastError();
    else
        ring->cqe.res = (int)bytes;

    ring->cqe.user_data = 0;
    ring->cqe.flags = 0;
    ring->cqe_ready = 1;

    free(ovp);
    *cqe_ptr = &ring->cqe;
    return 0;
}

static inline void io_uring_cqe_seen(struct io_uring *ring,
                                     struct io_uring_cqe *cqe)
{
    (void)cqe;
    ring->cqe_ready = 0;
}

#ifdef __cplusplus
}
#endif

#else   /* Linux */

#include <liburing.h>

#endif /* _WIN32 */

#endif /* MINI_IO_URING_H */