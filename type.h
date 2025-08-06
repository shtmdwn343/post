#pragma once

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#define true 1
#define false 0
typedef char bool;

typedef struct {
    u8* data;
    u64 size;
} string;