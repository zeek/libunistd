#include <stdio.h>
#include <string.h>
#include "mini_io_uring.h"

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#else
#include <unistd.h>
#include <fcntl.h>
#endif

int main() {
    struct io_uring ring;
    if (io_uring_queue_init(32, &ring, 0) < 0) {
        fprintf(stderr, "io_uring_queue_init failed\n");
        return 1;
    }

#ifdef _WIN32
    int fd = _open("test.txt", _O_RDWR | _O_BINARY);
#else
    int fd = open("test.txt", O_RDWR | O_CREAT, 0644);
#endif
    if (fd < 0) {
        perror("open");
        return 1;
    }

    const char *msg = "Hello from io_uring-lite!\n";
    size_t msglen = strlen(msg);
    char buf[128] = {0};

    /* WRITE + TIMEOUT in same submit */
    struct io_uring_sqe *sqe;

    sqe = io_uring_get_sqe(&ring);
    io_uring_prep_write(sqe, fd, msg, (unsigned)msglen, 0);
    sqe->user_data = 1;

    sqe = io_uring_get_sqe(&ring);
    io_uring_prep_timeout(sqe, 5000, 0, 0); /* 5s timeout */
    sqe->user_data = 2;

    io_uring_submit(&ring);

    struct io_uring_cqe *cqe;
    for (int i = 0; i < 2; i++) {
        if (io_uring_wait_cqe(&ring, &cqe) < 0) {
            fprintf(stderr, "wait_cqe failed\n");
            return 1;
        }
        printf("CQE: user_data=%llu res=%d\n",
               (unsigned long long)cqe->user_data, cqe->res);
        io_uring_cqe_seen(&ring, cqe);
    }

    /* READ back */
    sqe = io_uring_get_sqe(&ring);
    io_uring_prep_read(sqe, fd, buf, sizeof(buf)-1, 0);
    sqe->user_data = 3;

    io_uring_submit(&ring);
    if (io_uring_wait_cqe(&ring, &cqe) < 0) {
        fprintf(stderr, "wait_cqe failed\n");
        return 1;
    }

    if (cqe->res >= 0) {
        printf("READ: %d bytes: \"%.*s\"\n", cqe->res, cqe->res, buf);
    } else {
        printf("READ error: %d\n", cqe->res);
    }
    io_uring_cqe_seen(&ring, cqe);

    io_uring_queue_exit(&ring);

#ifdef _WIN32
    _close(fd);
#else
    close(fd);
#endif

    return 0;
}