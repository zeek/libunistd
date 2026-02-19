// sys/posix_stdio.h
// Copyright 4 Feb 2026 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef posix_stdio_h
#define posix_stdio_h

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

#define STDIN_FILENO _fileno(stdin)
#define STDOUT_FILENO _fileno(stdout)
#define STDERR_FILENO _fileno(stderr)

inline 
int fileno(FILE *stream)
{	return _fileno(stream);
}

inline
FILE *fdopen(int fildes, const char *mode)
{	return _fdopen(fildes,mode);
}

inline
int fcntl(int handle, int mode,...)
{	(void)handle;
	(void)mode;
	STUB_0(fcntl);
}

#if _CRT_DECLARE_NONSTDC_NAMES == 0

inline
ssize_t write(int fd,const void* buf,size_t count)
{	return _write(fd,buf,count);
}

//CFUNC int read(int fh, void* buf, unsigned count);
inline
ssize_t read(int fd,void* buf,size_t count)
{	return _read(fd,buf,count);
}

inline
void *alloca(size_t size)
{	return _alloca(size);
}

inline
off_t lseek(int fd, off_t offset, int whence)
{	return _lseek(fd,offset,whence);
}

inline
int mkdir(const char *path, mode_t mode)
{	(void) mode;
	return _mkdir(path);
}

inline
int execve(const char *path, char *const *argv,char *const *envp)
{	return _execve(path,argv,envp);
}

inline
int execv(const char *path, char *const *argv)
{	return _execv(path,argv);
}

// Cannot do struct/function name: typedef struct _stat64 stat;

typedef unsigned long nlink_t;
typedef long blksize_t;
typedef long long blkcnt_t;

#if 0
struct stat 
{// same as _stat64
    _dev_t         st_dev;
    _ino_t         st_ino;
    unsigned short st_mode;
    short          st_nlink;
    short          st_uid;
    short          st_gid;
    _dev_t         st_rdev;
    __int64        st_size;
    __time64_t     st_atime;
    __time64_t     st_mtime;
    __time64_t     st_ctime;
};
#endif

struct stat {
    dev_t     st_dev;
    ino_t     st_ino;
    mode_t    st_mode;
    nlink_t   st_nlink;
    uid_t     st_uid;
    gid_t     st_gid;
    dev_t     st_rdev;
    off_t     st_size;
    time_t    st_atime;
    time_t    st_mtime;
    time_t    st_ctime;
    blksize_t st_blksize;   
    blkcnt_t  st_blocks;    
};

inline
int stat(const char *path, struct stat *buf) 
{   return _stat64(path, (struct _stat64*)buf);
}

inline
FILE *popen(const char *command, const char *type)
{	
#ifdef _DEBUG
	printf("popen(%s,%s)\n",command,type);
#endif
	return _popen(command,type);
}

inline
int pclose(FILE *stream)
{	return stream ? _pclose(stream):-1;
}

inline 
pid_t getpid()
{   return _getpid();
}

#else
#error Cmake set: add_definitions(-D_CRT_DECLARE_NONSTDC_NAMES=0)
#define write _write
#define read _read
#define alloca _alloca
#define lseek _lseek
#define isatty _isatty
#define getcwd _getcwd
#define dup2 _dup2
#define dup _dup
#define close _close
#define chdir _chdir
#define getpid _getpid
#define RETSIGTYPE void
#define access _access
#ifndef __has_attribute
#define   __attribute__(x)
#endif
//#define mkdir mkdir2
//#define fileno _fileno
//#define open uni_open
//#define fdopen _fdopen
#define execve _execve
#define execv _execv
//#define mkdir _mkdir
#define stat _stat64
//#define strlen unistd_safe_strlen
//#define inet_ntop InetNtop
#define vsnprintf _vsnprintf
#define bzero(address,size) memset((address),0,size)
#define bcmp(s1, s2, n)	memcmp ((s1), (s2), (n))
#define bcopy(s, d, n)	memcpy ((d), (s), (n))
#define pow10(x) pow(x,10)
#define alloca _alloca
#define strdup _strdup
//#define sscanf uni_sscanf
//#define strlen unistd_safe_strlen
//#define inet_ntop InetNtop
#ifndef strcasecmp
#define strcasecmp _stricmp
#endif
#define strncasecmp _strnicmp
#define strtok_r strtok_s
#define getpid _getpid

#endif

// Define missing POSIX constants for MSVC
#ifndef S_IFLNK
#define S_IFLNK  0xA000
#endif

#ifndef S_ISLNK
#define S_ISLNK(m) (((m) & _S_IFMT) == S_IFLNK)
#endif

#ifndef S_ISREG
#define S_ISREG(m) (((m) & _S_IFMT) == _S_IFREG)
#endif

#ifndef S_ISDIR
#define S_ISDIR(m) (((m) & _S_IFMT) == _S_IFDIR)
#endif

#ifndef S_IREAD
#define S_IREAD  _S_IREAD
#endif

#ifndef S_IWRITE
#define S_IWRITE _S_IWRITE
#endif

#ifndef S_IEXEC
#define S_IEXEC  _S_IEXEC
#endif

// POSIX permission bits
#ifndef S_IRWXU
#define S_IRWXU  0000700
#define S_IRUSR  0000400
#define S_IWUSR  0000200
#define S_IXUSR  0000100
#define S_IRWXG  0000070
#define S_IRGRP  0000040
#define S_IWGRP  0000020
#define S_IXGRP  0000010
#define S_IRWXO  0000007
#define S_IROTH  0000004
#define S_IWOTH  0000002
#define S_IXOTH  0000001
#endif

//int lstat(const char *path, struct stat *statbuf);

#endif
