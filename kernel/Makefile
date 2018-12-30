include ../Makefile.config

KERNEL_OBJECTS=boot/kernelmain.o support/string.o support/serial.o support/console.o main.o

basekernel.img: kernel
	i686-elf-gcc -T linker.ld -o gegix.bin -ffreestanding -O2 -nostdlib ${KERNEL_OBJECTS} -lgcc
	mkdir -p ../isodir/boot/grub
	cp gegix.bin ../isodir/boot/gegix.bin
	cp boot/grub_file.txt ../isodir/boot/grub/grub.cfg
	grub-mkrescue -o basekernel.iso ../isodir

kernel: kernel.elf
	${OBJCOPY} -O binary $< $@

bootblock: bootblock.elf
	${OBJCOPY} -O binary $< $@

kernel.elf: ${KERNEL_OBJECTS}
	${LD} ${KERNEL_LDFLAGS} -Ttext 0x10000 ${KERNEL_OBJECTS} -o $@

bootblock.elf: bootblock.o
	${LD} ${KERNEL_LDFLAGS} -Ttext 0 $< -o $@

%.o: %.c
	${CC} ${KERNEL_CCFLAGS} -I ../include $< -o $@

%.o: %.S
	${CC} ${KERNEL_CCFLAGS} -I ../include $< -o $@

clean:
	rm -rf basekernel.img *.o *.elf kernel bootblock bootblock.o boot/*.o support/*.o gegix.bin
