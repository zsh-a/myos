.set MAGIC, 0x1badb002
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multboot
	.long MAGIC
	.long FLAGS
	.long CHECKSUM
.section .text
.extern kernel_main,system_constructors
.global laoder

loader:
	mov $kernel_stack, %esp
	call system_constructors
	push %eax
	push %ebx
	call kernel_main

stop:
	cli
	hlt
	jmp stop

.section .bss
.space 2*1024*1024
kernel_stack:
