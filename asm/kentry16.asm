.code16
.text
.global __kstart
.global check_longmode
.global GDT_REGISTER
.global IDT_REGISTER
.global SYSTEM_INFO
.extern __kpagetable_p4
__kstart:
  cli
  mov $0xFF, %al // disable pic
  out %al, $0xA1 // slave
  out %al, $0x21 // master
  nop // wait some seconds
  nop // wait some seconds

  pop %bx
  call   enable_A20
  mov %cs, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %ss
  mov %ax, %fs
  mov %ax, %gs
  mov $__stack_top, %esp
  mov %esp, %ebp
  cld
  call kmain16
  cmp $0x00, %al
  jne .loop

  xor %eax, %eax
  add $IDT_REGISTER, %eax
  lidt (%eax)

  xor %eax, %eax
  add $GDT_REGISTER, %eax
  lgdt (%eax)

  mov %cr4, %eax
  bts $0x5, %eax //pae
  bts $0x7, %eax //pge
  mov %eax, %cr4

  xor %eax, %eax
  mov %ds, %ax
  shl $0x04, %ax
  add $__kpagetable_p4, %eax
  mov %eax, %cr3

  mov $0xC0000080, %ecx
  rdmsr
  bts $0x8, %eax
  wrmsr

  mov SYSTEM_INFO, %edx
  xor %eax, %eax
  mov %cs, %ax
  shl $0x04, %ax
  mov %ax, %bx
  add $long_mode, %ax // fix long_mode address
  pushw $0x08 // gdt cs segment
  push %ax
  mov %bx, %ax
  add $fix_cs, %ax // fix_cs absolute address
  pushw $0x0
  push %ax
  retf // change cs=0x0, eip=fix_cs
fix_cs:
  mov $0x0, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs

  mov %cr0, %eax
  bts $0x0, %eax //pe
  bts $0x10, %eax //wp
  bts $0x1F, %eax //pg
  mov %eax, %cr0

  retf // change cs and go long mode

long_mode: // i386 as do not know how to jump so hard coded
.byte 0x48, 0xb8, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00 // mov $0x20000,%rax
.byte 0xff, 0xe0 // jmp *%rax

.loop:
  hlt
  jmp .loop


enable_A20:
  call   enable_A20.a20wait
  mov    $0xad,%al
  out    %al,$0x64

  call   enable_A20.a20wait
  mov    $0xd0,%al
  out    %al,$0x64

  call   enable_A20.a20wait2
  in     $0x60,%al
  push   %eax

  call   enable_A20.a20wait
  mov    $0xd1,%al
  out    %al,$0x64

  call   enable_A20.a20wait
  pop    %eax
  or     $0x2,%al
  out    %al,$0x60

  call   enable_A20.a20wait
  mov    $0xae,%al
  out    %al,$0x64

  call   enable_A20.a20wait
  ret

enable_A20.a20wait:
  in     $0x64,%al
  test   $0x2,%al
  jne    enable_A20.a20wait
  ret

enable_A20.a20wait2:
  in     $0x64,%al
  test   $0x1,%al
  je     enable_A20.a20wait2
  ret

check_longmode:
  pushf
  pop    %eax
  mov    %eax, %ecx
  xor    $0x00200000, %eax
  push   %eax
  popf
  pushf
  pop    %eax
  xor    %ecx, %eax
  shr    $0x15, %eax
  and    $0x1, %eax
  push   %ecx
  popf
  test   %eax, %eax
  jz     check_longmode.no_longmode //no cpuid
  mov    $0x80000000, %eax
  cpuid
  cmp    $0x80000001, %eax
  jb     check_longmode.no_longmode //no extended cpuid
  mov    $0x80000001, %eax
  cpuid
  test   $0x20000000, %edx
  jz     check_longmode.no_longmode //no long mode
  test   $0x800, %edx
  jz     check_longmode.no_longmode //no nxe support
  mov    $0x0, %eax
  ret
check_longmode.no_longmode:
  mov    $0x1, %eax
  ret

.bss
.align 4
GDT_REGISTER:
.byte 0x00, 0x00
.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
.align 4
IDT_REGISTER:
.byte 0x00, 0x00
.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
SYSTEM_INFO:
.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
