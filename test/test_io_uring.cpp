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
    if (io_uring_queue_init(8, &ring, 0) < 0) {
        fprintf(stderr, "io_uring_queue_init failed\n");
        return 1;
    }

    /* Open test file */
#ifdef _WIN32
    int fd = _open("test.txt", _O_RDWR | _O_BINARY);
#else
    int fd = open("test.txt", O_RDWR);
#endif
    if (fd < 0) {
        perror("open");
        return 1;
    }

    printf("Opened test.txt\n");

    /* --- WRITE --- */
    const char *msg = "Hello from io_uring/IOCP!\n";
    size_t msglen = strlen(msg);

    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    io_uring_prep_write(sqe, fd, msg, msglen, 0);
    sqe->user_data = 1;

    io_uring_submit(&ring);

    struct io_uring_cqe *cqe;
    io_uring_wait_cqe(&ring, &cqe);

    if (cqe->res < 0) {
        fprintf(stderr, "write error: %d\n", cqe->res);
        return 1;
    }

    printf("Wrote %d bytes\n", cqe->res);
    io_uring_cqe_seen(&ring, cqe);

    /* --- READ BACK --- */
    char buf[128] = {0};

    sqe = io_uring_get_sqe(&ring);
    io_uring_prep_read(sqe, fd, buf, sizeof(buf)-1, 0);
    sqe->user_data = 2;

    io_uring_submit(&ring);
    io_uring_wait_cqe(&ring, &cqe);

    if (cqe->res < 0) {
        fprintf(stderr, "read error: %d\n", cqe->res);
        return 1;
    }

    printf("Read %d bytes: \"%.*s\"\n", cqe->res, cqe->res, buf);
    io_uring_cqe_seen(&ring, cqe);

    /* Cleanup */
    io_uring_queue_exit(&ring);

#ifdef _WIN32
    _close(fd);
#else
    close(fd);
#endif

    return 0;
}