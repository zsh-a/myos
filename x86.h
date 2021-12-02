#pragma once
#include "types.h"

constexpr auto GDTSIZE = 7;




struct GdtDesc {
  u16 lim0_15;
  u16 base0_15;
  u8 base16_23;
  u8 acces;
  u8 lim16_19 : 4;
  u8 other : 4;
  u8 base24_31;
} __attribute__((packed));

struct Gdtr {
  u16 limite;
  u32 base;
} __attribute__((packed));


extern "C"{

    extern GdtDesc kgdt[];
    void init_gdt();

}