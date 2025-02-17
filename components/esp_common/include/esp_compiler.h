/*
 * SPDX-FileCopyrightText: 2016-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "sdkconfig.h"

#if defined(_MSC_VER)                       /* #CUSTOM@NDRS */

#include <immintrin.h>
#include <stdalign.h>
#define __attribute__(...)
#define likely(x)               (x)
#define unlikely(x)             (x) 
#define __alignof__             alignof
#define __builtin_ctz           _tzcnt_u32
#define __builtin_clz           _lzcnt_u32

static inline int __builtin_ffs(int x) {
    unsigned long index;

    if (_BitScanForward(&index, x)) {
        return index + 1; // Convert 0-based index to 1-based index
    }

    return 0; // If no bits are set, return 0
}

#ifndef __BUILTIN_FFS_DEFINED
#define __BUILTIN_FFS_DEFINED
static inline int __builtin_clzll(unsigned long long int x) {
    if (x == 0) {
        // If x is 0, return 64.
        return 64;
    }

    // Split the 64-bit integer into two 32-bit integers.
    unsigned int upper = x >> 32;
    unsigned int lower = x & 0xFFFFFFFF;

    if (upper != 0) {
        // If the upper 32 bits are not all zero, apply __builtin_clz to them.
        return __builtin_clz(upper);
    }
    else {
        // Otherwise, apply __builtin_clz to the lower 32 bits and add 32 to the result.
        return __builtin_clz(lower) + 32;
    }
}
#endif

#endif

/*
 * The likely and unlikely macro pairs:
 * These macros are useful to place when application
 * knows the majority occurrence of a decision paths,
 * placing one of these macros can hint the compiler
 * to reorder instructions producing more optimized
 * code.
 */
#if (CONFIG_COMPILER_OPTIMIZATION_PERF)
#ifndef likely
#define likely(x)      __builtin_expect(!!(x), 1)
#endif
#ifndef unlikely
#define unlikely(x)    __builtin_expect(!!(x), 0)
#endif
#else
#ifndef likely
#define likely(x)      (x)
#endif
#ifndef unlikely
#define unlikely(x)    (x)
#endif
#endif

/*
 * Utility macros used for designated initializers, which work differently
 * in C99 and C++ standards mainly for aggregate types.
 * The member separator, comma, is already part of the macro, please omit the trailing comma.
 * Usage example:
 *   struct config_t { char* pchr; char arr[SIZE]; } config = {
 *              ESP_COMPILER_DESIGNATED_INIT_AGGREGATE_TYPE_EMPTY(pchr)
 *              ESP_COMPILER_DESIGNATED_INIT_AGGREGATE_TYPE_STR(arr, "Value")
 *          };
 */
#if defined(__cplusplus) && __cplusplus >= 202002L
#define ESP_COMPILER_DESIGNATED_INIT_AGGREGATE_TYPE_STR(member, value)  .member = value,
#define ESP_COMPILER_DESIGNATED_INIT_AGGREGATE_TYPE_EMPTY(member) .member = { },
#elif defined(__cplusplus) && __cplusplus < 202002L
#define ESP_COMPILER_DESIGNATED_INIT_AGGREGATE_TYPE_STR(member, value)  { .member = value },
#define ESP_COMPILER_DESIGNATED_INIT_AGGREGATE_TYPE_EMPTY(member) .member = { },
#else
#define ESP_COMPILER_DESIGNATED_INIT_AGGREGATE_TYPE_STR(member, value)  .member = value,
#define ESP_COMPILER_DESIGNATED_INIT_AGGREGATE_TYPE_EMPTY(member)
#endif

#define __COMPILER_PRAGMA__(string) _Pragma(#string)
#define _COMPILER_PRAGMA_(string) __COMPILER_PRAGMA__(string)

#if __clang__
#define ESP_COMPILER_DIAGNOSTIC_PUSH_IGNORE(warning) \
    __COMPILER_PRAGMA__(clang diagnostic push) \
    __COMPILER_PRAGMA__(clang diagnostic ignored "-Wunknown-warning-option") \
    __COMPILER_PRAGMA__(clang diagnostic ignored warning)
#define ESP_COMPILER_DIAGNOSTIC_POP(warning) \
    __COMPILER_PRAGMA__(clang diagnostic pop)
#elif __GNUC__
#define ESP_COMPILER_DIAGNOSTIC_PUSH_IGNORE(warning) \
    __COMPILER_PRAGMA__(GCC diagnostic push) \
    __COMPILER_PRAGMA__(GCC diagnostic ignored "-Wpragmas") \
    __COMPILER_PRAGMA__(GCC diagnostic ignored warning)
#define ESP_COMPILER_DIAGNOSTIC_POP(warning) \
    __COMPILER_PRAGMA__(GCC diagnostic pop)
#else
#define ESP_COMPILER_DIAGNOSTIC_PUSH_IGNORE(warning)
#define ESP_COMPILER_DIAGNOSTIC_POP(warning)
#endif

#if __clang_analyzer__ || CONFIG_COMPILER_STATIC_ANALYZER
#define ESP_STATIC_ANALYZER_CHECK(_expr_, _ret_) do { if ((_expr_)) { return (_ret_); } } while(0)
#else
#define ESP_STATIC_ANALYZER_CHECK(_expr_, _ret_)
#endif
