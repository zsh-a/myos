GCCPARAMS = -m32 -O2 -W -fno-use-cxa-atexit -fno-stack-protector -ffreestanding -nostdlib -fno-builtin -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPPARAMS = --32
LDPARAMS = -melf_i386
GCC = g++
ASM = as
LINKER = ld
CFLAGS = -o $@ -c $<
ASMFLAGS = -o $@ $<
LINKERFLAGS = -T $< -o $@ 

objects = boot.o kernel.o x86.o

%.o: %.cc
	$(GCC) $(GCCPARAMS) $(CFLAGS)
	
%.o: %.s
	$(ASM) $(ASPPARAMS) $(ASMFLAGS)
	
kernel.bin: linker.ld $(objects)
	$(LINKER) $(LINKERFLAGS) $(objects)
	
all: kernel.bin
	echo "build successed"
clean: 
	rm -rf *.o
	rm -rf *.out
	rm -rf iso
	rm -rf *.iso
	rm -rf *.bin
	
rebuild: clean all
	echo "rebuild"
	
install: kernel_lab.bin
	sudo cp $< /boot/kernel_lab.bin

kernel.iso: kernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot
	cp boot/grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o $@ iso
	rm -rf iso

qemu: kernel.iso
	qemu-system-x86_64 kernel.iso

qemu-gdb: kernel.iso
	qemu-system-x86_64 -s -S kernel.iso