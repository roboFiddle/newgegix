.set ALIGN,    1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,  1<<1             /* provide memory map */
.set FLAGS,    ALIGN | MEMINFO  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
  .skip 32768 # 16 KiB
stack_top:

.section .text
.global _start
.type _start, @function
_start:
	mov $stack_top, %esp
    jmp load_gdt
_runkernel:
  call kernel_main
  cli
1:	hlt
  jmp 1b

.align 16
.global gdt
gdt:
	.word	0,0,0,0				# seg 0 - null
	.word	0xffff, 0x0000, 0x9a00, 0x00cf	# seg 1 - kernel flat 4GB code
	.word	0xffff, 0x0000, 0x9200, 0x00cf	# seg 2 - kernel flat 4GB data
	.word	0xffff, 0x0000, 0xfa00, 0x00cf	# seg 3 - user flat 4GB code
	.word	0xffff, 0x0000, 0xf200, 0x00cf	# seg 4 - user flat 4GB data
	.word	0x0068, (tss-_start),0x8901, 0x00cf  # seg 5 - TSS

gdt_init: # to find location of gdt
	.word	gdt_init-gdt
	.long	gdt

.align 16
.global tss
tss:
	.long	0

.align 16
.global load_gdt
load_gdt:
    cli
    lgdt gdt_init
    sti
    jmp load_idt

load_idt:
    jmp _runkernel


.size _start, . - _start

