// sys/ioctl.c
// Libunistd Copyright 2026 Robin.Rowe@CinePaint.org
// License open source MIT

#include "../unistd.h"
#include "../termios.h"
#include "ioctl.h"
// #include "../net/if.h" // if you need struct ifreq 

// int ioctl(int fd, unsigned long request, ...)

int ioctl(int fd, unsigned long request, void *arg) 
{   if (request == TIOCGWINSZ) 
    {   struct winsize *ws = (struct winsize *)arg;
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (!GetConsoleScreenBufferInfo(h, &csbi))
            return -1;
        ws->ws_col = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
        ws->ws_row = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;
        ws->ws_xpixel = 0;
        ws->ws_ypixel = 0;
        return 0;
    }
    errno = ENOTTY;
    return -1;
}

