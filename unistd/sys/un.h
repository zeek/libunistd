// sys/un.h
// Copyright 3 Feb 2026 Robin.Rowe@CinePaint.org
// License Open Source MIT

#ifndef sys_un_h
#define sys_un_h

#include <errno.h>
#include "../cfunc.h"
#include "posix_types.h"
#include "../grp.h"

#define AF_LOCAL AF_UNIX
#define UNIX_PATH_MAX 108

//typedef unsigned short sa_family_t;

struct sockaddr_un 
{   sa_family_t sun_family;
    char sun_path[UNIX_PATH_MAX];
};

CFUNC int af_socket(int domain, int type, int protocol);
CFUNC int af_bind(int fd, const struct sockaddr *sa, socklen_t len);
CFUNC int af_listen(int fd, int backlog);
CFUNC int af_accept(int fd, struct sockaddr *sa, socklen_t *len);
CFUNC int af_connect(int fd, const struct sockaddr *sa, socklen_t len);
CFUNC ssize_t af_write(int fd, const void *buf, size_t len);
CFUNC ssize_t af_read(int fd, void *buf, size_t len);

#define socket af_socket
#define bind af_bind
#define listen af_listen
#define accept af_accept
#define connect af_connect
#define write af_write
#define read af_read

#endif
