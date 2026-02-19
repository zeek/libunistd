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
    IORING_OP_WRITE,
    IORING_OP_TIMEOUT
};

struct io_uring_sqe {
    int op;
    HANDLE h;
    void *buf;
    unsigned nbytes;
    long long offset;
    unsigned long long user_data;
    unsigned long long timeout_ms; /* for TIMEOUT */
};

struct io_uring_cqe {
    int res;
    unsigned flags;
    unsigned long long user_data;
};

#define MINI_IOURING_DEPTH  32

struct io_uring {
    HANDLE iocp;

    struct io_uring_sqe sq[MINI_IOURING_DEPTH];
    unsigned sq_head;
    unsigned sq_tail;

    struct io_uring_cqe cq[MINI_IOURING_DEPTH];
    unsigned cq_head;
    unsigned cq_tail;

    int have_timeout;
    unsigned long long timeout_expire; /* GetTickCount64() + ms */
    unsigned long long timeout_user_data;
};

static inline int io_uring_queue_init(unsigned entries,
                                      struct io_uring *ring,
                                      unsigned flags)
{
    (void)entries; (void)flags;
    memset(ring, 0, sizeof(*ring));
    ring->iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    return ring->iocp ? 0 : -1;
}

static inline void io_uring_queue_exit(struct io_uring *ring)
{
    if (ring->iocp) CloseHandle(ring->iocp);
}

static inline unsigned mini_ring_next(unsigned v)
{
    return (v + 1) & (MINI_IOURING_DEPTH - 1);
}

static inline int mini_ring_full(unsigned head, unsigned tail)
{
    return mini_ring_next(tail) == head;
}

static inline int mini_ring_empty(unsigned head, unsigned tail)
{
    return head == tail;
}

static inline struct io_uring_sqe *io_uring_get_sqe(struct io_uring *ring)
{
    if (mini_ring_full(ring->sq_head, ring->sq_tail))
        return NULL;
    struct io_uring_sqe *sqe = &ring->sq[ring->sq_tail];
    memset(sqe, 0, sizeof(*sqe));
    ring->sq_tail = mini_ring_next(ring->sq_tail);
    return sqe;
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

static inline void io_uring_prep_timeout(struct io_uring_sqe *sqe,
                                         unsigned long long timeout_ms,
                                         unsigned count,
                                         unsigned flags)
{
    (void)count; (void)flags;
    sqe->op = IORING_OP_TIMEOUT;
    sqe->timeout_ms = timeout_ms;
}

static inline void mini_cq_push(struct io_uring *ring,
                                int res,
                                unsigned long long user_data)
{
    if (mini_ring_full(ring->cq_head, ring->cq_tail))
        return; /* drop on overflow */
    struct io_uring_cqe *cqe = &ring->cq[ring->cq_tail];
    cqe->res = res;
    cqe->flags = 0;
    cqe->user_data = user_data;
    ring->cq_tail = mini_ring_next(ring->cq_tail);
}

static inline int io_uring_submit(struct io_uring *ring)
{
    unsigned idx = ring->sq_head;
    while (!mini_ring_empty(idx, ring->sq_tail)) {
        struct io_uring_sqe *sqe = &ring->sq[idx];

        if (sqe->op == IORING_OP_TIMEOUT) {
            ring->have_timeout = 1;
            ring->timeout_expire = GetTickCount64() + sqe->timeout_ms;
            ring->timeout_user_data = sqe->user_data;
        } else if (sqe->op == IORING_OP_READ || sqe->op == IORING_OP_WRITE) {
            OVERLAPPED *ov = (OVERLAPPED*)calloc(1, sizeof(OVERLAPPED));
            LARGE_INTEGER off;
            off.QuadPart = sqe->offset;
            ov->Offset = off.LowPart;
            ov->OffsetHigh = off.HighPart;

            CreateIoCompletionPort(sqe->h, ring->iocp, (ULONG_PTR)sqe->user_data, 0);

            BOOL ok;
            if (sqe->op == IORING_OP_READ) {
                ok = ReadFile(sqe->h, sqe->buf, sqe->nbytes, NULL, ov);
            } else {
                ok = WriteFile(sqe->h, sqe->buf, sqe->nbytes, NULL, ov);
            }

            if (!ok && GetLastError() != ERROR_IO_PENDING) {
                mini_cq_push(ring, -(int)GetLastError(), sqe->user_data);
                free(ov);
            }
        }

        idx = mini_ring_next(idx);
    }
    ring->sq_head = ring->sq_tail;
    return 0;
}

static inline int io_uring_wait_cqe(struct io_uring *ring,
                                    struct io_uring_cqe **cqe_ptr)
{
    if (!mini_ring_empty(ring->cq_head, ring->cq_tail)) {
        *cqe_ptr = &ring->cq[ring->cq_head];
        return 0;
    }

    DWORD timeout_ms = INFINITE;
    if (ring->have_timeout) {
        unsigned long long now = GetTickCount64();
        if (now >= ring->timeout_expire) {
            mini_cq_push(ring, -10060, ring->timeout_user_data); /* ETIMEDOUT-ish */
            ring->have_timeout = 0;
            *cqe_ptr = &ring->cq[ring->cq_head];
            return 0;
        } else {
            unsigned long long diff = ring->timeout_expire - now;
            timeout_ms = (DWORD)(diff > 0xFFFFFFFFULL ? 0xFFFFFFFFUL : diff);
        }
    }

    DWORD bytes;
    ULONG_PTR key;
    OVERLAPPED *ovp;

    BOOL ok = GetQueuedCompletionStatus(ring->iocp,
                                        &bytes,
                                        &key,
                                        &ovp,
                                        timeout_ms);

    if (!ok) {
        DWORD err = GetLastError();
        if (err == WAIT_TIMEOUT && ring->have_timeout) {
            mini_cq_push(ring, -10060, ring->timeout_user_data);
            ring->have_timeout = 0;
        } else if (err != WAIT_TIMEOUT) {
            mini_cq_push(ring, -(int)err, (unsigned long long)key);
        }
    } else {
        mini_cq_push(ring, (int)bytes, (unsigned long long)key);
    }

    if (ovp) free(ovp);

    if (mini_ring_empty(ring->cq_head, ring->cq_tail))
        return -1;

    *cqe_ptr = &ring->cq[ring->cq_head];
    return 0;
}

static inline void io_uring_cqe_seen(struct io_uring *ring,
                                     struct io_uring_cqe *cqe)
{
    (void)cqe;
    ring->cq_head = mini_ring_next(ring->cq_head);
}

#ifdef __cplusplus
}
#endif

#else  /* Linux: thin wrappers over liburing */

#include <liburing.h>

static inline void io_uring_prep_timeout(struct io_uring_sqe *sqe,
                                         unsigned long long timeout_ms,
                                         unsigned count,
                                         unsigned flags)
{
    struct __kernel_timespec ts;
    ts.tv_sec = timeout_ms / 1000;
    ts.tv_nsec = (timeout_ms % 1000) * 1000000ULL;
    io_uring_prep_timeout(sqe, &ts, count, flags);
}

#endif /* _WIN32 */

#endif /* MINI_IO_URING_H */