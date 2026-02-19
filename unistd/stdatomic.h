// atomic/stdatomic.h — portable atomic operations for C with MSVC
// Copyright 3 Feb 2026 Robin.Rowe@CinePaint.org
// License Open Source MIT

#ifndef atomic_stdatomic_h
#define atomic_stdatomic_h

#include "unistd.h"
#include "cfunc.h"

#if 1
#ifdef __cplusplus
extern "C" {
#endif

typedef struct atomic_int_impl atomic_int_t;

#ifdef __cplusplus
}
#endif
#else
// CFUNC with extern "C" necessary on forward-declared C/C++ struct in MSVC!
CFUNC typedef struct atomic_int_impl atomic_int_t;
#endif

CFUNC atomic_int_t* atomic_int_create(int value);
CFUNC void atomic_int_destroy(atomic_int_t* a);
CFUNC int  atomic_int_load(const atomic_int_t* a);
CFUNC void atomic_int_store(atomic_int_t* a, int value);
CFUNC int  atomic_int_exchange(atomic_int_t* a, int value);
CFUNC int  atomic_int_fetch_add(atomic_int_t* a, int value);
CFUNC int  atomic_int_fetch_sub(atomic_int_t* a, int value);
CFUNC int  atomic_int_compare_exchange(atomic_int_t* a, int expected, int desired);

// #undef __STDC_NO_ATOMICS__

// Emulate C11 _Atomic keyword
// #define _Atomic(T) struct { volatile T value; }
#define _Atomic volatile

// Atomic load
#define atomic_load(obj) \
    InterlockedCompareExchange(&(obj)->value, 0, 0)

// Atomic store
#define atomic_store(obj, val) \
    InterlockedExchange(&(obj)->value, (val))

// Atomic exchange
#define atomic_exchange(obj, val) \
    InterlockedExchange(&(obj)->value, (val))

// Atomic compare-exchange
#define atomic_compare_exchange_strong(obj, expected, desired) \
    (InterlockedCompareExchange(&(obj)->value, (desired), *(expected)) == *(expected))

#endif
