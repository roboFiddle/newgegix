include Makefile.config

KERNEL_SOURCES=$(wildcard kernel/*.[chS])

do: clean	all	run

all: basekernel.iso

run: basekernel.iso
	qemu-system-i386 -cdrom basekernel.iso -serial file:serial

library/baselib.a: $(LIBRARY_SOURCES) $(LIBRARY_HEADERS)
	cd library && make

$(USER_PROGRAMS): $(USER_SOURCES) library/baselib.a $(LIBRARY_HEADERS)
	cd user && make

kernel/basekernel.img: $(KERNEL_SOURCES)
	cd kernel && make

image: kernel/basekernel.img
	cp kernel/basekernel.iso basekernel.iso

basekernel.iso: image

clean:
	rm -rf basekernel.iso image isodir
	cd kernel && make clean
