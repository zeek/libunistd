// sys/uio.h
// Copyright 3 Feb 2026 Robin.Rowe@CinePaint.org
// License Open Source MIT

#ifndef sys_uio_h
#define sys_uio_h

#include <stddef.h>  // size_t
#include <sys/types.h> // ssize_t, if you define it there
#include "posix_types.h"
#include "cfunc.h"

struct iovec 
{   void  *iov_base;   /* Starting address */
    size_t iov_len;    /* Number of bytes to transfer */
};

CFUNC ssize_t readv(int fd, const struct iovec *iov, int iovcnt);
CFUNC ssize_t writev(int fd, const struct iovec *iov, int iovcnt);

#endif