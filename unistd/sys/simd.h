// simd_detect.h — portable SIMD feature detection for C/C++
// Copyright 3 Feb 2026 Robin.Rowe@CinePaint.org
// License MIT (http://opensource.org/licenses/mit-license.php);
// Works on GCC, Clang, MSVC, ICC, MinGW, Clang-CL

#ifndef simd_h
#define simd_h

// =========================
//  SSE2
// =========================
#if defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#   define HAVE_SSE2 1
#else
#   define HAVE_SSE2 0
#endif

// =========================
//  SSE4.1
// =========================
#if defined(__SSE4_1__) || defined(__AVX__) || defined(__AVX2__)
#   define HAVE_SSE41 1
#else
#   define HAVE_SSE41 0
#endif

// =========================
//  AVX
// =========================
#if defined(__AVX__) || defined(__AVX2__)
#   define HAVE_AVX 1
#else
#   define HAVE_AVX 0
#endif

// =========================
//  AVX2
// =========================
#if defined(__AVX2__)
#   define HAVE_AVX2 1
#else
#   define HAVE_AVX2 0
#endif

// =========================
//  AVX-512 (basic)
// =========================
#if defined(__AVX512F__)
#   define HAVE_AVX512 1
#else
#   define HAVE_AVX512 0
#endif

#if HAVE_AVX2
#   define POSIX_SIMD_LEVEL 5
#elif HAVE_AVX
#   define POSIX_SIMD_LEVEL 4
#elif HAVE_SSE41
#   define POSIX_SIMD_LEVEL 3
#elif HAVE_SSE2
#   define POSIX_SIMD_LEVEL 2
#else
#   define POSIX_SIMD_LEVEL 0
#endif

#endif
