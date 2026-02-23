/* unistd.h: replaces *nix header of same name
// Windows emulation of common *nix functions
// Copyright Nov 10, 2002, Robin.Rowe@CinePaint.org
// License MIT (http://opensource.org/licenses/mit-license.php);
*/

#ifndef unistd_h
#define unistd_h

#if !defined(_WINSOCK2API_) && defined(_WINSOCKAPI_)
#error unistd.h must be included before Winsock2.h or Windows.h!
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <winnt.h>
#include <corecrt_io.h>
#include <vcruntime.h>
#undef socklen_t
#include <WS2tcpip.h>
#include <math.h>
#include <fcntl.h>
#include <process.h> // getpid()
#include <io.h>
#include <malloc.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <winerror.h>
#include <memory.h>
#include <Ws2tcpip.h>//for InetNtop
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <sys/utime.h>
#include <sys/stat.h>
#include <sys/random.h>
#include <assert.h>
#include <inttypes.h>
#include "bsd/string.h"
#include "sigaction.h"
#include "gettimeofday.h"
#include "clock_gettime.h"
#include "cfunc.h"
#include "stub.h"
#include "sys/posix_types.h"
#include "sys/posix_stdio.h"

inline
const char* GetUnistdVersion()
{	return "v1.4 (3 Feb 2026)"; // git tag -a v1.4 -m "16 Feb 2026"
}

// Disable gcc function signature extension:
#ifndef __has_attribute
#define   __attribute__(x)
#endif
//__attribute__((format (printf, 1, 2)))

#undef MAX_PRIORITY /* remove winspool.h warning */
#define SSIZE_MAX SIZE_MAX
#define SIGTRAP 23

CFUNC const char* optarg;
CFUNC int optind;
CFUNC int opterr;
CFUNC int optopt;

typedef long long useconds_t;
typedef unsigned int uint;

#ifdef _BSD_SOURCE
CFUNC pid_t getpgrp(pid_t pid); /* BSD version */
//CFUNC int fcntl(int handle,int mode,int mode2);
#else
CFUNC pid_t getpgrp(); /* POSIX.1 version */
//CFUNC int fcntl(int handle, int mode,...);
#endif

CFUNC int setpgrp(pid_t pid, pid_t pgid);
CFUNC int pipe(int pipes[2]);
//CFUNC int uni_open(const char* filename,unsigned oflag,int mode);
//CFUNC int uni_open(const char* filename, unsigned oflag,...);
//CFUNC int fcntl(int handle, int mode,...);
//CFUNC int fcntl(int handle,int mode,int mode2);
CFUNC int setpgrp(pid_t pid, pid_t pgid);
CFUNC int mkdir2(const char* path, int mask);
CFUNC int snprintb(char *buf, size_t buflen, const char *fmt, uint64_t val);
CFUNC int snprintb_m(char *buf, size_t buflen, const char *fmt, uint64_t val,size_t max);
CFUNC size_t unistd_safe_strlen(const char* s);
CFUNC int uni_sscanf(char* input,const char* format,...);
//CFUNC FILE *popen(const char *command, const char *type);
//CFUNC int pclose(FILE *stream);
CFUNC int kill(pid_t p, int x);
CFUNC int S_ISCHR(int v);
CFUNC int S_ISBLK(int v);
CFUNC int S_ISFIFO(int v);
CFUNC int S_ISSOCK(int v);
CFUNC pid_t gettid();
CFUNC int setgid(gid_t g);
CFUNC int setuid(uid_t g);
CFUNC const char* getsysconfdir();
CFUNC int mkstemp(char *filename);
CFUNC int fchmod(int a, mode_t b);
CFUNC uid_t getuid();
CFUNC uid_t geteuid();
CFUNC gid_t getgid();
CFUNC gid_t getegid();
CFUNC char* realpath(const char *path, char *resolved_path);
CFUNC ssize_t readlink(const char *path, char *buf, size_t bufsize);
CFUNC char *getlogin();
CFUNC int getlogin_r(char *buf, size_t len);
CFUNC int getopt(int argc, char * const argv[],const char *optstring);
CFUNC void PrintDirectory();
CFUNC unsigned int alarm(unsigned int seconds);
CFUNC int chown(const char *path, uid_t owner, gid_t group);
CFUNC int fchown(int fd, uid_t owner, gid_t group);
CFUNC int lchown(const char *path, uid_t owner, gid_t group);
CFUNC int chroot(const char *path);
CFUNC size_t confstr(int name, char *buf, size_t len);
CFUNC const char *ctermid(char *s);
CFUNC int fchdir(int fd);
CFUNC pid_t fork();
CFUNC int getdtablesize();
CFUNC int fsync (int fd);
CFUNC void sync();
CFUNC int syncfs(int fd);
CFUNC int fdatasync(int fd);
CFUNC long fpathconf(int fd, int name);
CFUNC long pathconf(const char *path, int name);
CFUNC long gethostid();
CFUNC int sethostid(long hostid);
CFUNC int getpagesize();
CFUNC char *getpass(const char *prompt);
CFUNC int setpgid(pid_t pid, pid_t pgid);
CFUNC pid_t getpgid(pid_t pid);
CFUNC pid_t getppid();
CFUNC int link(const char *oldpath, const char *newpath);
CFUNC int lockf(int fd, int cmd, off_t len);
CFUNC int nice(int inc);
CFUNC int pause();
CFUNC int brk(void *addr);
CFUNC void *sbrk(intptr_t increment);
CFUNC int setreuid(uid_t ruid, uid_t euid);
CFUNC int setregid(gid_t rgid, gid_t egid);
CFUNC int setsid();
CFUNC int symlink(const char *target, const char *linkpath);
CFUNC long sysconf(int name);
CFUNC pid_t tcgetpgrp(int fd);
CFUNC int tcsetpgrp(int fd, pid_t pgrp);
CFUNC char *ttyname(int fd);
CFUNC int ttyname_r(int fd, char *buf, size_t buflen);
CFUNC useconds_t ualarm(useconds_t usecs, useconds_t interval);
CFUNC pid_t vfork();
/* between 0.0 and 1.0 */
CFUNC double drand48();
CFUNC void srand48(long int seedval);
CFUNC long int random(void);
CFUNC void srandom(unsigned int seed);
CFUNC int setenv(const char *name, const char *value, int overwrite);
CFUNC int unsetenv(const char *name);
CFUNC int truncate(const char *path, off_t length);
CFUNC int ftruncate(int fd, off_t length);
CFUNC int fseeko(FILE *stream, off_t offset, int whence);
CFUNC off_t ftello(FILE *stream);
CFUNC char* strptime(const char* s, const char* format,struct tm* tm);
CFUNC ssize_t getline(char** lineptr, size_t* n,FILE* stream);
CFUNC ssize_t getdelim(char** lineptr, size_t* n,int delim, FILE* stream);
CFUNC int vasprintf(char **strp, const char *fmt, va_list ap);

inline
void bzero(void *s, size_t n)
{	memset(s, 0, n);
}

inline
double pow10(double x)
{	return pow(x,10);
}

inline
char *strdup(const char *s)
{	return _strdup(s);
}

//Already in Win32: CFUNC int chmod(const char *path, mode_t mode);
CFUNC ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset);
CFUNC int setlinebuf(FILE *stream);
CFUNC int vasprintf(char **strp, const char *fmt, va_list ap);
CFUNC int aprintf(char **ret, const char *format, ...);

/* use with limits.h */
#define LONG_LONG_MAX LLONG_MAX
#define LONG_LONG_MIN LLONG_MIN

inline
int unlink(const char *path)
{	return _unlink(path);
}

inline
int rmdir(const char *path)
{	return _rmdir(path);
}

inline
int isatty(int fd)
{	return _isatty(fd);
}

inline
char *getcwd(char* buf, int size)
{	return _getcwd(buf,size);
}

inline
int dup(int oldfd)
{	return _dup(oldfd);
}

inline
int dup2(int oldfd, int newfd)
{	return _dup2(oldfd,newfd);
}

inline
int chdir(const char *path)
{	return _chdir(path);
}


inline
int access(const char *path, int mode)
{	return _access(path,mode);
}

inline
int pipe(int* pipes)
{	return _pipe((pipes),8*1024,_O_BINARY);
}

inline
int open(const char *filename, int oflag, ...)
{	return _open(filename, oflag, 0); //mode is third arg
}

inline
int close(int fd)
{	return _close(fd);
}

CFUNC int lstat(const char *path,struct stat *statbuf);
CFUNC int fstat(int fd, struct stat* st);

#define spawnvpe _spawnvpe
#define spawnvp _spawnvp
#define spawnve _spawnve
#define spawnv _spawnv
#define strcmpi _stricmp
#define strcasecmp _stricmp
#define isascii(c) ((unsigned)(c) < 0x80)

// causes issues with math.h:
//#define rint(x) floor ((x) + 0.5)
//#define lround floor
//#define roundl floor

// The POSIX name for this item is deprecated by MSVC:

// Defined unsupported macro as empty.
#define __builtin_unreachable()

// Remove defines that cause collisions.
#undef min
#undef max
#undef close
//#undef CONST
//#undef ERROR
#undef IGNORE
#undef STATUS_INVALID_HANDLE
#undef STATUS_INVALID_PARAMETER
#undef Yield
#undef CompareString
#undef NO_ERROR

#if _MSC_VER < 1930
// Workaround negative character values that caused asserts on VS 2019 and below
#define isalpha(ch) isalpha((unsigned char)(ch))
#define isupper(ch) isupper((unsigned char)(ch))
#define islower(ch) islower((unsigned char)(ch))
#define isdigit(ch) isdigit((unsigned char)(ch))
#define isspace(ch) isspace((unsigned char)(ch))
#define ispunct(ch) ispunct((unsigned char)(ch))
#define isblank(ch) isblank((unsigned char)(ch))
#define isalnum(ch) isalnum((unsigned char)(ch))
#define isprint(ch) isprint((unsigned char)(ch))
#define isgraph(ch) isgraph((unsigned char)(ch))
#define iscntrl(ch) iscntrl((unsigned char)(ch))
#endif

#endif
