#include "x86.h"

extern "C" {

GdtDesc kgdt[GDTSIZE];

inline GdtDesc new_gdt_desc(u32 base, u32 limit, u8 access, u8 other) {
  return GdtDesc{.lim0_15 = limit & 0xffff,
                 .base0_15 = base & 0xffff,
                 .base16_23 = (base >> 16) & 0xff,
                 .acces = access,
                 .lim16_19 = (limit >> 16) & 0xf,
                 .other = other,
                 .base24_31 = (base >> 24) & 0xff};
}

void init_gdt() {
    kgdt[0] = new_gdt_desc(0,0,0,0);
    kgdt[1] = new_gdt_desc(0,3,4,12);
}
}