#pragma once
using u8 = unsigned char;
using u16 = unsigned short ;
using u32 = unsigned int ;
using u64 = unsigned long long ;

using s8 = signed char ;
using s16=  signed short ;
using s32 = signed int ;
using s64 =  signed long long ;

static_assert(sizeof(u8) == 1);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(u32) == 4);
static_assert(sizeof(u64) == 8);