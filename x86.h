#pragma once
#include "types.h"

constexpr auto GDTSIZE = 7;
constexpr u32 GDTBASE = 0x00000800;

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
  u16 limit;
  u32 base;
} __attribute__((packed));

struct IdtDesc {
  u16 offset0_15;
  u16 select;
  u16 type;
  u16 offset16_31;
} __attribute__((packed));

struct Idtr {
  u16 limit;
  u32 base;
} __attribute__((packed));

extern "C" {

extern GdtDesc* kgdt;
void init_gdt();
}