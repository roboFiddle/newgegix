include Makefile.config

KERNEL_SOURCES=$(wildcard kernel/*.[chS])

all: basekernel.iso

run: basekernel.iso
	qemu-system-i386 -cdrom basekernel.iso

library/baselib.a: $(LIBRARY_SOURCES) $(LIBRARY_HEADERS)
	cd library && make

$(USER_PROGRAMS): $(USER_SOURCES) library/baselib.a $(LIBRARY_HEADERS)
	cd user && make

kernel/basekernel.img: $(KERNEL_SOURCES)
	cd kernel && make

image: kernel/basekernel.img
	rm -rf image
	mkdir image image/boot image/bin
	cp kernel/basekernel.img image/boot

basekernel.iso: image
	${ISOGEN} -input-charset utf-8 -iso-level 2 -J -R -o $@ -b boot/basekernel.img image

clean:
	rm -rf basekernel.iso image
	cd kernel && make clean
