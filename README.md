# libunistd Windows POSIX/pthreads

[Libunistd](https://github.com/robinrowe/libunistd/) is a lightweight [POSIX](https://en.wikipedia.org/wiki/POSIX) and [pthreads](https://en.wikipedia.org/wiki/Pthreads) library for Windows that enables ordinary Linux C/C++ source code to build using Microsoft Visual Studio pretty much as-is. If you're developing C/C++ code across Windows, Linux and MacOS, use libunistd to make a single codebase strategy work. Libunistd supports all the common Linux pthreads POSIX calls, except fork(). Instead of fork, use C/C++threads or pthreads. 

Robin Rowe created libunistd, the Windows port of the Linux system calls, while porting CinePaint to Windows .Using libunstd provides common BSD and SysV calls under Windows. Using libunstd is quite lightweight, mainly headers, and works with VC++. Unlike the alternative [Cygwin](https://en.wikipedia.org/wiki/Cygwin), that brings the Linux development evironment including gcc to Windows. Cygwin is so much that it includes its own package management system.

## Usage

Set the include path in your VC++ project to include the directory of libunistd/unistd (where the unistd.h file is). In Windows, where you encounter any Linux-based code that won't compile,  include the following at top of your header or source file:

	#include <unistd.h>

Doing this will include the magic to make basic Linux APIs work in Windows. Because unistd.h exists in Linux as a standard header file, making this change for Windows will not break Linux code. 

No \#ifdef \_WIN32 ugliness in your code. Simply write C++ code as you would for Linux, and have it build in Visual Studio for Windows.

When porting your Linux program to Windows for the first time, you may have to include more POSIX headers than were necessary in your Linux code. In Linux, some standard headers include other standard header files. As a result, sometimes Linux code will compile that's not strictly legit, that it hasn't explicitly included all necessary header files. If you port Linux code to FreeBSD or Mac OS X, you may have encountered this before, that the compiler sometimes objects to unknown POSIX functions. Simple solution. Look up in the Linux man page the "unknown" function and include all the header files that the Linux man page says it requires.

## CMake Build Automation with Cmaker

Libunistd has a CMake build system. If you have a Linux codebase that uses `make`, that is not the right stuff for maintaining a platform-independent single codebase. Use my Cmake build automation software [Cmaker](https://gitlab.com/heroic-robots/cmaker) to create a new cmake build system automatically. Cmaker creates a recursive cmake build system without reference to anything except what C/C++ files it finds in the source directory and below. Will create a cmake recursive build system from scratch, within seconds.

## History

Libunistd ([github repo](https://github.com/robinrowe/libunistd)) is an API wrapper for Windows that maps POSIX/pthreads/BSD/System V \*nix system calls to call into their Windows conterparts. Port Linux-style C/C++ programs to Windows rapidly. Simply set build include path to point to magic Windows headers that have the same names as unistd.h and other common Linux header files.

Originally developed in 2002 while porting CinePaint to Windows, libunistd has been continually improved. Using libunistd makes it much easier to maintain the same C/C++ codebase for Linux/MacOS/Windows. Another benefit of libunistd, building \*nix code in Visual Studio is access to the excellent static code checker in that IDE. Detect buffer overruns and pointer issues at compile time. Great for legacy code clean-up.

Besides CinePaint, libunistd is used in other systems. For example, to build a Windows version of a Linux traffic controller. Sequences U.S. traffic lights upon which lives depend, a safety-critical, real-time, Linux embedded system that’s national critical infrastructure. Although the controller would only ship with the embedded version using the USDOT-approved version of gcc, I built the same code in gcc, clang and VC++. Each compiler reports slightly different warnings and errors. Achieving a clean compile in all 3 compilers, with no warnings or errors, made for better code.

## Version 1.3
Tagged 19 Oct 2025

Changes:

- vasprintf()
- getline()
- gethostbyname2()
- getrusage()
- getrandom()
- flock()
- memmem()
- fnmatch()
- vsyslog()
- getopt()
- timwoj merge
- Wojtulewicz merge

bash
```
% git log --tags --simplify-by-decoration --pretty="format:%ai %d"

2018-10-28 22:36:18 -0700  (tag: v1.2)
2017-04-15 15:49:13 -0700  (tag: v1.0)
2016-09-16 18:12:01 -0700  (origin/devel)
2016-02-20 16:52:48 -0800  (tag: v1.1)
2015-06-12 20:09:48 -0700 
```

## Code Contributors

- robinrowe
- Wojtulewicz
- timwoj

## License

Open Source MIT/BSD. In other words, you may do pretty much whatever you want with libunistd. Not copyleft GNU.

## Project Director

Robin.Rowe@cinepaint.org 
Baltimore, Maryland 323-535-0952

