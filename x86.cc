#include "x86.h"

#include "io.h"
extern "C" {

Gdtr kgdtr{.limit = GDTSIZE * 8, .base = GDTBASE};
GdtDesc* kgdt = reinterpret_cast<GdtDesc*>(GDTBASE);

Idtr kidtr{.limit = IDTSIZE * 8, .base = IDTBASE};
IdtDesc* kidt = reinterpret_cast<IdtDesc*>(IDTBASE);

Tss default_tss;

GdtDesc new_gdt_desc(u32 base, u32 limit, u8 access, u8 other) {
  return GdtDesc{.lim0_15 = limit & 0xffff,
                 .base0_15 = base & 0xffff,
                 .base16_23 = (base >> 16) & 0xff,
                 .acces = access,
                 .lim16_19 = (limit >> 16) & 0xf,
                 .other = other,
                 .base24_31 = (base >> 24) & 0xff};
}

void init_gdt() {

  // for cpu switch state
  default_tss.debug_flag = 0x00;
  default_tss.io_map = 0x00;
  default_tss.esp0 = 0x1FFF0;
  default_tss.ss0 = 0x18;

  kgdt[0] = new_gdt_desc(0, 0, 0, 0);
  kgdt[1] = new_gdt_desc(0x0, 0xFFFFF, 0x9B, 0x0D);  // code
  kgdt[2] = new_gdt_desc(0x0, 0xFFFFF, 0x93, 0x0D);  // data
  kgdt[3] = new_gdt_desc(0x0, 0x0, 0x97, 0x0D);

  kgdt[7] = new_gdt_desc((u32)&default_tss, 0x67, 0xE9, 0x00);  // tss

  asm volatile("lgdtl (kgdtr)");

  /* initiliaz the segments */
  asm("   movw $0x10, %ax	\n \
            movw %ax, %ds	\n \
            movw %ax, %es	\n \
            movw %ax, %fs	\n \
            movw %ax, %gs	\n \
            ljmp $0x08, $next	\n \
            next:		\n");
}

IdtDesc new_idt_desc(u16 select, u32 offset, u16 type) {
  return IdtDesc{.offset0_15 = offset & 0xffff,
                 .select = select,
                 .type = type,
                 .offset16_31 = (offset >> 16) & 0xffff};
}

void init_idt() {
  // init irq
  for (int i = 0; i < IDTSIZE; ++i) {
    kidt[i] = new_idt_desc(0x08, (u32)_asm_schedule, INTGATE);
  }
  // exception
  kidt[13] = new_idt_desc(0x08, (u32)_asm_exc_GP, INTGATE);
  kidt[14] = new_idt_desc(0x08, (u32)_asm_exc_PF, INTGATE);
  kidt[32] = new_idt_desc(0x08, (u32)_asm_schedule, INTGATE);
  kidt[33] = new_idt_desc(0x08, (u32)_asm_int_1, INTGATE);
  kidt[48] = new_idt_desc(0x08, (u32)_asm_syscalls, TRAPGATE);
  kidt[128] = new_idt_desc(0x08, (u32)_asm_syscalls, TRAPGATE);

  asm("lidtl (kidtr)");
}

void init_pic(void) {
  /* Initialization of ICW1 */
  io.outb(0x20, 0x11);
  io.outb(0xA0, 0x11);
  /* Initialization of ICW2 */
  io.outb(0x21, 0x20); /* start vector = 32 */
  io.outb(0xA1, 0x70); /* start vector = 96 */
  /* Initialization of ICW3 */
  io.outb(0x21, 0x04);
  io.outb(0xA1, 0x02);
  /* Initialization of ICW4 */
  io.outb(0x21, 0x01);
  io.outb(0xA1, 0x01);
  /* mask interrupts */
  io.outb(0x21, 0x0);
  io.outb(0xA1, 0x0);
}

void isr_default_int(int id) {
  // io.print("interrupt");
  char chs[] = {id + '0',0};
  // io.print("interrupt");
  io.print(chs);
  static int tic = 0;
  static int sec = 0;
  switch (id) {
    case 1:
      // isr_kbd_int();
      break;
    default:
      return;
  }
  io.outb(0x20, 0x20);
  io.outb(0xA0, 0x20);
}

void isr_GP_exc(void) {
  io.print("General protection fault !");
  // if (arch.pcurrent!=NULL){
  // 	io.print("The processus %s have to be killed
  // !\n\n",(arch.pcurrent)->getName()); 	(arch.pcurrent)->exit();
  // schedule();
  // }
  // else{
  // 	io.print("The kernel have to be killed !\n\n");
  // 	asm("hlt");
  // }
  
}

void isr_PF_exc(void) {
  // u32 faulting_addr, code;
  // u32 eip;
  // struct page *pg;
  // u32 stack;
  // asm(" 	movl 60(%%ebp), %%eax	\n \
  //   		mov %%eax, %0		\n \
	// 	mov %%cr2, %%eax	\n \
	// 	mov %%eax, %1		\n \
 	// 	movl 56(%%ebp), %%eax	\n \
  //   		mov %%eax, %2"
  // 	: "=m"(eip), "=m"(faulting_addr), "=m"(code));
  //  asm("mov %%ebp, %0": "=m"(stack):);

  // //io.print("#PF : %x \n",faulting_addr);

  // //for (;;);
  // 	if (arch.pcurrent==NULL)
  // 		return;

  // 	process_st* current=arch.pcurrent->getPInfo();

  // if (faulting_addr >= USER_OFFSET && faulting_addr <= USER_STACK) {
  // 	pg = (struct page *) kmalloc(sizeof(struct page));
  // 	pg->p_addr = get_page_frame();
  // 	pg->v_addr = (char *) (faulting_addr & 0xFFFFF000);
  // 	list_add(&pg->list, &current->pglist);
  // 	pd_add_page(pg->v_addr, pg->p_addr, PG_USER, current->pd);
  // }
  // else {
  // 	io.print("\n");
  // 	io.print("No autorized memory acces on : %p (eip:%p,code:%p)\n",
  // faulting_addr,eip,  code); 	io.print("heap=%x, heap_limit=%x,
  // stack=%x\n",kern_heap,KERN_HEAP_LIM,stack);

  // 	if (arch.pcurrent!=NULL){
  // 		io.print("The processus %s have to be killed
  // !\n\n",(arch.pcurrent)->getName()); 		(arch.pcurrent)->exit();
  // schedule();
  // 	}
  // 	else{
  // 		io.print("The kernel have to be killed !\n\n");
  // 		asm("hlt");
  // 	}
  // }
  // io.print("interrupt");
}
void isr_schedule_int() {
  // static int tic = 0;
  // static int sec = 0;
  // 	tic++;
  // 	if (tic % 100 == 0) {
  // 	sec++;
  // 	tic = 0;
  // }
  // schedule();
  io.outb(0x20, 0x20);
  io.outb(0xA0, 0x20);
  // io.print("interrupt");
}
void do_syscall(int num) { io.print("syscall"); }
}