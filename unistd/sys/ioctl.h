// sys/ioctl.h
// Libunistd Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef sys_ioctl_h
#define sys_ioctl_h

#include "../cfunc.h"

CFUNC int ioctl(int fd, unsigned long request, ...);

#endif
