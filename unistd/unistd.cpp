/* unistd.h: replaces *nix header of same name
// Windows emulation of common *nix functions
// Copyright Nov 10, 2002, Robin.Rowe@CinePaint.org
// License MIT (http://opensource.org/licenses/mit-license.php)
*/

#include <vector>
#include "unistd.h"
#include <ws2tcpip.h>
#include <io.h>
#include <stdint.h>
#include "sys/posix_types.h"

pid_t getpgrp() /* POSIX.1 version */
{	STUB_0(getpgrp);
}

pid_t getpgrp(pid_t pid) /* BSD version */
{	(void)pid;
	STUB_0(getpgrp);
}

int setpgrp() /* System V version */
{	STUB_0(setpgrp);
}

int setpgrp(pid_t pid, pid_t pgid) /* BSD version */
{	(void)pid;
	(void)pgid;
	STUB_0(setpgrp);
}

#pragma warning(disable : 4996)

struct WinsockData
{   std::vector<SOCKET> socket;
	WinsockData()
	{   WSADATA wsa;
        int err = WSAStartup(MAKEWORD(2, 2), &wsa);
        if (err != 0)
		{	puts("ERROR: WSAStartup failed");
    }   }
    ~WinsockData() {
        WSACleanup();
    }
};

static WinsockData winsock_data;

static inline
SOCKET get_socket(int fd)
{   if (fd < 0 || fd >= winsock_data.socket.size() )
	{	return INVALID_SOCKET;
	}
    return winsock_data.socket[fd];
}

int posix_socket(int domain, int type, int protocol) {
    SOCKET s = WSASocket(domain, type, protocol, NULL, 0, 0);
    if (s == INVALID_SOCKET)
    {   return -1;
	}
    int fd = _open_osfhandle((intptr_t)s, 0);
    if (fd < 0)
	{   closesocket(s);
        return -1;
    }
    winsock_data.socket.push_back(s);
    return fd;
}

int posix_read(int fd,void* buf,unsigned len)
{   SOCKET s = get_socket(fd);
	if(s != INVALID_SOCKET)
	{   int ret = recv(s,(char*) buf, (int)len, 0);
        if (ret == SOCKET_ERROR)
		{   errno = WSAGetLastError() == WSAEWOULDBLOCK ? EAGAIN : EIO;
            return -1;
        }
        return ret;
    }
    return _read(fd, buf, (unsigned)len);
}

ssize_t posix_write(int fd, const void *buf, size_t len)
{   SOCKET s = get_socket(fd);
	if(s != INVALID_SOCKET)
	{   int ret = send(s, (char*) buf, (int)len, 0);
        if (ret == SOCKET_ERROR)
		{   errno = WSAGetLastError() == WSAEWOULDBLOCK ? EAGAIN : EIO;
            return -1;
        }
        return ret;
    }
	return _write(fd, buf, (unsigned)len);
}

#if 0
int pipe(int pipes[2])
{	return _pipe((pipes), 8*1024, _O_BINARY);
}
#endif

int snprintb(char *buf, size_t buflen, const char *fmt, uint64_t val)
{	(void)buf;
	(void)buflen;
	(void)fmt;
	(void)val;
	STUB_0(snprintb);
}

int snprintb_m(char *buf, size_t buflen, const char *fmt, uint64_t val,size_t max)
{	(void)buf;
	(void)buflen;
	(void)fmt;
	(void)val;
	(void)max;
	STUB_0(snprintb_m);
}

size_t unistd_safe_strlen(const char* s)
{	if(!s)
	{	puts("ERROR: strlen(null)");
		return 0;
	}
	return (s ? strlen(s):0);
}

int uni_sscanf(char* input,const char* format,...)
{	if(!input || !*input || !format)
	{	return 0;
	}
	const size_t length = strlen(input);
	va_list argList;
	va_start(argList,format);// BUG/BROKEN: should be count of args, not format
#pragma warning(disable:4996)
	const int retval = _snscanf(input,length,format,argList);
	va_end(argList);
	input[length-1]=0;
	return retval;
}

#undef MAX_PRIORITY /* remove winspool.h warning */

int kill(pid_t p, int x)
{	(void)p;
	(void)x;
	return -1;
}

int S_ISCHR(int v)
{	(void)v;
	return 0;
}

int S_ISBLK(int v)
{	(void)v;
	return 0;
}

int S_ISFIFO(int v)
{	(void)v;
	return 0;
}

int S_ISSOCK(int v)
{	(void)v;
	return 0;
}

pid_t gettid()
{	HANDLE h = GetCurrentThread();
	return (intptr_t) h;
}

int setgid(gid_t g)
{	(void)g;
	return -1;
}

int setuid(uid_t g)
{	(void)g;
	return -1;
}

const char* getsysconfdir()
{	STUB_0(getsysconfdir);
}

int mkstemp(char *filename)
{	char buffer[MAX_PATH];
	unsigned filenameNo = GetTempFileNameA(".",filename,0,buffer);
	return (int) filenameNo;
}

int fchmod(int a, mode_t b)
{	(void)a;
	(void)b;
	STUB_NEG(fchmod);
}

uid_t getuid()
{	STUB_NEG(getuid);
}

uid_t geteuid()
{	STUB_NEG(geteuid);
}

gid_t getgid()
{	STUB_NEG(getgid);
}

gid_t getegid()
{	STUB_NEG(getegid);
}

char* realpath(const char *path, char *resolved_path)
{	if(!resolved_path)
	{	return 0;
	}
	const DWORD  err = GetFullPathNameA(path,(DWORD) PATH_MAX,resolved_path,0);
	if(err == 0)
	{	return 0;
	}
	return resolved_path;
}

ssize_t readlink(const char *path, char *buf, size_t bufsize)
{	(void)path;
	(void)buf;
	(void)bufsize;
	STUB_0(readlink);
}

#if _MSC_VER <= 1900

int gethostname(char *name, size_t len)
{   DWORD bufsize = (DWORD) len;
    BOOL ok = GetComputerNameA(name,&bufsize);
    return ok? 0:-1;
}

#endif

char *getlogin()
{	STUB_0(getlogin);
}

int getlogin_r(char *buf, size_t len)
{   DWORD bufsize = (DWORD) len;
    BOOL ok = GetUserNameA(buf,&bufsize);
    return ok? 0:-1;
}

void PrintDirectory()
{	const char* path = _getcwd(0,0);
	if(!path)
	{	perror("getcwd() error");
		return;
	}
	printf("pwd = %s\n", path);
	free((void*)path);
}

unsigned int alarm(unsigned int seconds)
{	(void)seconds;
	STUB_0(alarm);
}

int chown(const char *path, uid_t owner, gid_t group)
{	(void)path;
	(void)owner;
	(void)group;
	STUB_0(chown);
}
#if 0
int chmod(const char *path, mode_t mode)
{   (void) path;
	(void) mode;
	STUB_0(chmod);
}
#endif
int fchown(int fd, uid_t owner, gid_t group)
{	(void)fd;
	(void)owner;
	(void)group;
	STUB_0(fchown);
}

int lchown(const char *path, uid_t owner, gid_t group)
{	(void)path;
	(void)owner;
	(void)group;
	STUB_0(lchown);
}

int chroot(const char *path)
{	(void)path;
	STUB_NEG(chroot);
}

size_t confstr(int name, char *buf, size_t len)
{	(void)name;
	(void)buf;
	(void)len;
	STUB_0(confstr);
}

const char *ctermid(char *s)
{	const char* term = "/dev/tty";
	if(s)
	{	strcpy(s,term);
		return s;
	}
	return term;
}

/*
int chdir(const char *path)
{	return _chdir(path);
}*/

int fchdir(int fd)
{	(void)fd;
	STUB_NEG(fchdir);
}

pid_t fork()
{	STUB_NEG(fork);
}

inline
int getdtablesize()
{	STUB_0(getdtablesize);
}

int fsync (int fd)
{	HANDLE h = (HANDLE) _get_osfhandle(fd);
	if (h == INVALID_HANDLE_VALUE)
	{	return -1;
	}
	if (!FlushFileBuffers (h))
	{	return -1;
	}
	return 0;
}

void sync()
{	_flushall();
}

int syncfs(int fd)
{	fsync(fd);
	return 0;
}

int fdatasync(int fd)
{	(void)fd;
	STUB_NEG(fdatasync);
}

long fpathconf(int fd, int name)
{	(void)fd;
	(void)name;
	STUB_NEG(fpathconf);
}

long pathconf(const char *path, int name)
{	(void)path;
	(void)name;
	STUB_NEG(pathconf);
}

long gethostid()
{	STUB_NEG(gethostid);
}

int sethostid(long hostid)
{	(void)hostid;
	STUB_NEG(sethostid);
}

int getpagesize()
{	STUB_0(getpagesize);
}

char *getpass(const char *prompt)
{	(void)prompt;
	STUB_0(getpass);
}

int setpgid(pid_t pid, pid_t pgid)
{	(void)pid;
	(void)pgid;
	STUB_0(setpgid);
}

pid_t getpgid(pid_t pid)
{	(void)pid;
	STUB_0(getpgid);
}

pid_t getppid()
{	STUB_0(getppid);
}

int link(const char *oldpath, const char *newpath)
{	(void)oldpath;
	(void)newpath;
	STUB_NEG(link);
}

int lockf(int fd, int cmd, off_t len)
{	(void)fd;
	(void)cmd;
	(void)len;
	STUB_NEG(lockf);
}

int nice(int inc)
{	(void)inc;
	STUB_NEG(nice);
}

int pause()
{	STUB_NEG(pause);
}

int brk(void *addr)
{	(void)addr;
	STUB_NEG(brk);
}

void *sbrk(intptr_t increment)
{	(void)increment;
	STUB_0(sbrk);
}

int setreuid(uid_t ruid, uid_t euid)
{	(void)ruid;
	(void)euid;
	STUB_NEG(setreuid);
}

int setregid(gid_t rgid, gid_t egid)
{	(void)rgid;
	(void)egid;
	STUB_NEG(setregid);
}

int setsid()
{	STUB_NEG(setsid);
}

int symlink(const char *target, const char *linkpath)
{	(void)target;
	(void)linkpath;
	STUB_NEG(symlink);
}

long sysconf(int name)
{	(void)name;
	STUB_NEG(sysconf);
}

pid_t tcgetpgrp(int fd)
{	(void)fd;
	STUB_NEG(tcgetpgrp);
}

int tcsetpgrp(int fd, pid_t pgrp)
{	(void)fd;
	(void)pgrp;
	STUB_NEG(tcsetpgrp);
}

char *ttyname(int fd)
{	(void)fd;
	STUB_0(ttyname);
}

int ttyname_r(int fd, char *buf, size_t buflen)
{	(void)fd;
	(void)buf;
	(void)buflen;
	STUB_NEG(ttyname_r);
}

useconds_t ualarm(useconds_t usecs, useconds_t interval)
{	(void)usecs;
	(void)interval;
	STUB_0(ualarm);
}

pid_t vfork()
{	STUB_NEG(vfork);
}

/* between 0.0 and 1.0 */
double drand48(void)
{	double r=(double)rand();
	r/=RAND_MAX;
	return r > 1.0? 1.0:r;
}

/*double srand48(time_t);*/

void srand48(long int seedval)
{	srand(seedval);
}

long int random()
{	return rand();
}
void srandom(unsigned int seed)
{ 	srand(seed);
}

#if 0
int sleep(useconds_t seconds)
{	Sleep((DWORD)(1000*seconds));
	return 0;
}

int usleep(useconds_t usec)
{	LARGE_INTEGER time1;
	LARGE_INTEGER time2;
	LARGE_INTEGER freq;
	time1.QuadPart = 0;
	time2.QuadPart = 0;
	freq.QuadPart = 0;
	QueryPerformanceCounter(&time1);
	QueryPerformanceFrequency(&freq);
	do
	{	QueryPerformanceCounter(&time2);
	} while((time2.QuadPart-time1.QuadPart) < usec);
	return 0;
}
#endif

int ftruncate(int fd, off_t length)
{	return _chsize(fd,length);
}

int fseeko(FILE *stream, off_t offset, int whence)
{	return fseek(stream,offset,whence);
}

off_t ftello(FILE *stream)
{	return ftell(stream);
}

int vasprintf(char **strp, const char *fmt, va_list ap)
{   va_list ap_copy;
    va_copy(ap_copy, ap);
    int size = vsnprintf(NULL, 0, fmt, ap_copy);
    va_end(ap_copy);
    if (size < 0)
	{	return -1; // Error
    }
    *strp = (char *)malloc(size + 1); // +1 for the null terminator
    if (*strp == NULL)
	{	return -1; // Memory allocation failed
    }
    // format the string into the allocated buffer
    int result = vsnprintf(*strp, size + 1, fmt, ap);
    if (result < 0)
	{   free(*strp);
        *strp = NULL;
        return -1; // Error
    }
	return result;
}

int lstat(const char *path, struct stat *statbuf)
{   HANDLE hFile;
    BY_HANDLE_FILE_INFORMATION fileInfo;
    if (!path || !statbuf)
	{   errno = EINVAL;
        return -1;
    }
    // Open with FILE_FLAG_OPEN_REPARSE_POINT to not follow symlinks
    hFile = CreateFileA(path,
                        0,  // No access needed
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
                        NULL);
    if (hFile == INVALID_HANDLE_VALUE)
	{   errno = ENOENT;
        return -1;
    }
    if (!GetFileInformationByHandle(hFile, &fileInfo))
	{   CloseHandle(hFile);
        errno = EIO;
        return -1;
    }
    CloseHandle(hFile);
    memset(statbuf, 0, sizeof(struct _stat));
    DWORD fileAttr = fileInfo.dwFileAttributes;
    // Set file mode
    if (fileAttr & FILE_ATTRIBUTE_DIRECTORY) {
        statbuf->st_mode = _S_IFDIR | 0755;
    } else if (fileAttr & FILE_ATTRIBUTE_REPARSE_POINT) {
        statbuf->st_mode = _S_IFLNK | 0777;  // Symlink
    } else {
        statbuf->st_mode = _S_IFREG | 0644;
    }
    // Set file size
    statbuf->st_size = (((__int64)fileInfo.nFileSizeHigh) << 32)
                        + fileInfo.nFileSizeLow;
    // Convert FILETIME to time_t
    ULARGE_INTEGER ull;
    ull.LowPart = fileInfo.ftLastWriteTime.dwLowDateTime;
    ull.HighPart = fileInfo.ftLastWriteTime.dwHighDateTime;
    statbuf->st_mtime = (time_t)(ull.QuadPart / 10000000ULL - 11644473600ULL);
    ull.LowPart = fileInfo.ftLastAccessTime.dwLowDateTime;
    ull.HighPart = fileInfo.ftLastAccessTime.dwHighDateTime;
    statbuf->st_atime = (time_t)(ull.QuadPart / 10000000ULL - 11644473600ULL);
    ull.LowPart = fileInfo.ftCreationTime.dwLowDateTime;
    ull.HighPart = fileInfo.ftCreationTime.dwHighDateTime;
    statbuf->st_ctime = (time_t)(ull.QuadPart / 10000000ULL - 11644473600ULL);
    // Set number of links (Windows doesn't really support this)
    statbuf->st_nlink = 1;
    return 0;
}

static int map_errno(DWORD winerr)
{
    switch (winerr) {
    case ERROR_FILE_NOT_FOUND:     return ENOENT;
    case ERROR_PATH_NOT_FOUND:     return ENOENT;
    case ERROR_ACCESS_DENIED:      return EACCES;
    case ERROR_INVALID_HANDLE:     return EBADF;
    case ERROR_NOT_ENOUGH_MEMORY:  return ENOMEM;
    case ERROR_OUTOFMEMORY:        return ENOMEM;
    case ERROR_SHARING_VIOLATION:  return EACCES;
    case ERROR_LOCK_VIOLATION:     return EACCES;
    case ERROR_ALREADY_EXISTS:     return EEXIST;
    case ERROR_FILE_EXISTS:        return EEXIST;
    case ERROR_BROKEN_PIPE:        return EPIPE;
    case ERROR_PIPE_NOT_CONNECTED: return EPIPE;
    case ERROR_INVALID_FUNCTION:   return EINVAL;
    default:
        return EIO;
    }
}

int fstat(int fd, struct stat* st)
{   if (!st)
	{   errno = EFAULT;
        return -1;
    }
    // 1. Check if this fd is a socket in your POSIX layer
    SOCKET s = get_socket(fd);
    if (s != INVALID_SOCKET)
	{   // Sockets have no real stat info on Windows
        memset(st, 0, sizeof(*st));
        st->st_mode = S_IFSOCK;
        st->st_nlink = 1;
        return 0;
    }
    // 2. Use MSVC's _fstat64 for real files
    struct _stat64 wst;
    if (_fstat64(fd, &wst) == -1)
	{	errno = map_errno(GetLastError());
        return -1;
    }
    // 3. Convert Windows _stat64 ? Linux struct stat
    memset(st, 0, sizeof(*st));
    st->st_dev     = (dev_t)wst.st_dev;
    st->st_ino     = (ino_t)wst.st_ino;
    st->st_mode    = (mode_t)wst.st_mode;
    st->st_nlink   = (nlink_t)wst.st_nlink;
    st->st_uid     = (uid_t)wst.st_uid;
    st->st_gid     = (gid_t)wst.st_gid;
    st->st_rdev    = (dev_t)wst.st_rdev;
    st->st_size    = (off_t)wst.st_size;
    st->st_atime   = (time_t)wst.st_atime;
    st->st_mtime   = (time_t)wst.st_mtime;
    st->st_ctime   = (time_t)wst.st_ctime;

    // Linux fields not present on Windows
    st->st_blksize = 4096;
    st->st_blocks  = (st->st_size + 511) / 512;

    return 0;
}

ssize_t pwrite(int fildes, const void *buf, size_t nbyte, off_t offset)
{	if (nbyte == 0)
	{	return 0;
	}
	OVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.Offset = static_cast<DWORD>(offset);
	overlapped.OffsetHigh = offset >> 32;
	DWORD written;
	if (!WriteFile((HANDLE)_get_osfhandle(fildes), buf, static_cast<DWORD>(nbyte), &written, &overlapped))
	{	return -1;
	}
	return written;
}

int setlinebuf(FILE *stream)
{	return setvbuf(stream, NULL, _IONBF, 0);
}

#if 0
int vasprintf(char **ptr, const char *format, va_list arg)
{	int n = _vscprintf(format, arg);
	if (n < 0)
		return -1;
	char *p = (char *)malloc(n+1);
	if (p == NULL)
		return -1;
	int rv = vsprintf_s(p, n+1, format, arg);
	if (rv < 0)
	{	free(p);
		return -1;
	}
	*ptr = p;
	return rv;
}
#endif

int asprintf(char **ret, const char *format, ...)
{	va_list ap;
	va_start(ap, format);
	int retval = vasprintf(ret, format, ap);
	va_end(ap);
	return retval;
}
