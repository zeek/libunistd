// sys/file.h
// Copyright (c) 2022/10/19 Tomer Lev <tomerlev@microsoft.com>
// License open source MIT

#ifndef sys_file_h
#define sys_file_h

#include "../cfunc.h"

enum 
{	F_LOCK=1,
	F_TLOCK,
	F_ULOCK,
	F_TEST 
};

#define LOCK_SH 1
#define LOCK_EX 2
#define LOCK_NB 4
#define LOCK_UN 8

CFUNC int flock(int fd, int operation);

#endif
