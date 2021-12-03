.extern isr_default_int,do_syscall

.macro SAVE_REGS
    pushal
    push %ds 
    push %es 
    push %fs 
    push %gs 
    mov $0x10,%eax
    mov %eax,%ds 
.endm

.macro RESTORE_REGS
    pop %gs
	pop %fs
	pop %es
	pop %ds
	popal
.endm

.macro INTERRUPT num
.global _asm_int_\num
_asm_int_\num:
    SAVE_REGS
    push \num
    call isr_default_int
    pop %eax
    mov $0x20,%al
	out %al,$0x20
    RESTORE_REGS
    iret
.endm


.extern isr_GP_exc, isr_PF_exc 
.global _asm_syscalls, _asm_exc_GP, _asm_exc_PF


.global _asm_syscalls
_asm_syscalls:
    SAVE_REGS
    push %eax
    call do_syscall
    pop %eax
    RESTORE_REGS
    iret

_asm_exc_GP:
	SAVE_REGS
	call isr_GP_exc
	RESTORE_REGS
	iret

_asm_exc_PF:
	SAVE_REGS
	call isr_PF_exc
	RESTORE_REGS
	add $4,%esp
	iret

.global _asm_schedule
_asm_schedule:
	SAVE_REGS
	call isr_schedule_int
    mov $0x20,%al
	out %al,$0x20
	RESTORE_REGS
	iret

INTERRUPT 1

