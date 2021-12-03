#pragma once
#include "types.h"
class IO {
 public:
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
};

extern IO io;