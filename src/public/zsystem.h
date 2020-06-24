#ifndef __ZSYSTEM_H__
#define __ZSYSTEM_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef WIN32
typedef __int8 s8;
typedef __int16 s16;
typedef __int32 s32;
typedef __int64 s64;
typedef unsigned __int8 u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;
#else
typedef unsigned char u8;
typedef unsigned short u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef char s8;
typedef short s16;
typedef int32_t s32;
typedef int64_t s64;
#endif // WIN32


#ifdef __cplusplus
extern "C" {
#endif
    void zassertion(const char *file, int line, const char *function, const char *debug);
#ifdef __cplusplus
};
#endif

#ifdef DEBUG_MODE
#define zassert(p, format, ...) {\
    char debug[4096] = {0}; \
    snprintf(debug, sizeof(debug), format, ##__VA_ARGS__); \
    ((p) ? (void)0 : (void)zassertion(__FILE__, __LINE__, __FUNCTION__, debug)); \
}
#else
#define zassert
#endif



#endif // __ZSYSTEM_H__