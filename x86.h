#pragma once
#include "types.h"

constexpr auto GDTSIZE = 7;
constexpr u32 GDTBASE = 0x00000800;

constexpr u32 IDTBASE = 0x00000000;
constexpr auto IDTSIZE = 0xff;

// IdtDesc type
constexpr u16 INTGATE = 0x8E00; // automatically disable interrupts on entry and enable on IRET
constexpr u16 TRAPGATE = 0xEF00;

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
  u16 select; // segment selector : 0-1 bits RPL   2 bit : GDT or LDT   3-15 bits : index
  u16 type;  // Gate Type  and DPL, P
  u16 offset16_31;
} __attribute__((packed));

struct Idtr {
  u16 limit;
  u32 base;
} __attribute__((packed));


struct Tss {
	u16 previous_task, __previous_task_unused;
	u32 esp0;
	u16 ss0, __ss0_unused;
	u32 esp1;
	u16 ss1, __ss1_unused;
	u32 esp2;
	u16 ss2, __ss2_unused;
	u32 cr3;
	u32 eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
	u16 es, __es_unused;
	u16 cs, __cs_unused;
	u16 ss, __ss_unused;
	u16 ds, __ds_unused;
	u16 fs, __fs_unused;
	u16 gs, __gs_unused;
	u16 ldt_selector, __ldt_sel_unused;
	u16 debug_flag, io_map;
} __attribute__ ((packed));

extern "C" {

extern GdtDesc* kgdt;
void init_gdt();
void init_idt();
void init_pic();


// extern void _asm_int_0();
extern void _asm_int_1();
extern void _asm_syscalls();
extern void _asm_exc_GP(void);
extern void _asm_exc_PF(void);
extern void _asm_schedule();

}