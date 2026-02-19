// strings.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef strings_h
#define strings_h

#include <stddef.h>
#include <string.h>
#include <memory.h>
#include "stub.h"
#include "cfunc.h"

inline
int ffs(int i)
{   STUB_NEG(ffs);
}

inline
int ffsl(long int i)
{   STUB_NEG(ffsl);
}

inline
int ffsll(long long int i)
{   STUB_NEG(ffsll);
}

inline
int strcasecmp(const char *s1, const char *s2)
{	return _stricmp(s1,s2);
}

inline
int strncasecmp(const char *s1, const char *s2, size_t n)
{	return _strnicmp(s1,s2,n);
}

inline
int bcmp(const void* s1, const void* s2, size_t n)
{	return memcmp(s1,s2,n);
}

inline
void bcopy(const void* src,void* dest,size_t n)
{	memcpy(dest,src,n);
}

#endif
