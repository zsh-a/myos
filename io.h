#pragma once
#include "types.h"
class IO {
 public:
  // IO(){

  // }
  void outb(u32 ad, u8 v) { asm volatile("outb %%al,%%dx" ::"d"(ad), "a"(v)); }
  void outw(u32 ad, u16 v) { asm volatile("outw %%ax,%%dx" ::"d"(ad), "a"(v)); }
  void outl(u32 ad, u32 v) {
    asm volatile("outl %%eax,%%dx" ::"d"(ad), "a"(v));
  }

  u8 inb(u32 ad) {
    u8 v;

    __asm__ volatile("inb %%dx,%%al" : "=a"(v) : "d"(ad));
    return v;
  }

  u16 inw(u32 ad) {
    u16 v;
    asm volatile("inw %%dx,%%ax" : "=a"(v) : "d"(ad));
    return v;
  }
  u32 inl(u32 ad) {
    u32 v;
    asm volatile("inl %%dx,%%eax" : "=a"(v) : "d"(ad));
    return v;
  }

  u8 x{0};
  u8 y{0};
  u16* SCREEN_ADDRESS = (u16*)(0xb8000);
  // void print(const char *str) {
  //   u16* SCREEN_ADDRESS = (u16*)(0xb8000);
  //   for (int i = 0; str[i] != '\0'; ++i) {
  //     //写入字符串，取或0xff00的意思是我们需要把屏幕高四位拉低，否则就是黑色的字体，黑色的字体黑色的屏幕是啥也看不到的
  //     SCREEN_ADDRESS[i] = (SCREEN_ADDRESS[i] & 0xff00) | str[i];
  //   }
  // }
  void print(const char* str) {
    for (int i = 0; str[i] != '\0'; ++i, ++y) {
      if (y >= SCREEN_COL) {
        y = 0;
        ++x;
      }
      SCREEN_ADDRESS[x * SCREEN_COL + y] =
          (SCREEN_ADDRESS[x * SCREEN_COL + y] & 0xff00) | str[i];
    }
  }

 private:
  static constexpr u8 SCREEN_ROW = 25;
  static constexpr u8 SCREEN_COL = 80;
  // screen attributes
};

extern IO io;