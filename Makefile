MAKE = make

AS16 = i386-elf-as
CC16 = i386-elf-gcc
LD16 = i386-elf-ld

AS64 = x86_64-elf-as
CC64 = x86_64-elf-gcc
LD64 = x86_64-elf-ld

DOCSGEN = doxygen
DOCSFILES = $(shell find . -type f -name \*.md)
DOCSCONF = docs.doxygen

M4 = m4

CCXXFLAGS = -std=gnu99 -Os -nostdlib -ffreestanding -c -Iincludes \
	-Werror -Wall \
	-mgeneral-regs-only -mno-red-zone

AS16FLAGS = --32
CC16FLAGS = -m32 -march=i386 -D___BITS=16 $(CCXXFLAGS)
LD16FLAGS = --nmagic -s

AS64FLAGS = --64
CC64FLAGS = -m64 -march=x86-64 -D___BITS=64 $(CCXXFLAGS)
LD64FLAGS = --nmagic -s

OBJDIR = output
ASOBJDIR = $(OBJDIR)/asm
CCOBJDIR = $(OBJDIR)/cc
DOCSOBJDIR = $(OBJDIR)/docs
ASSRCDIR = asm
CCSRCDIR = cc
LDSRCDIR = lds
M4SRCDIR = m4_macros
M4OBJDIR = $(CCSRCDIR)-gen


AS16SRCS = $(shell find $(ASSRCDIR) -type f -name \*16.asm)
CC16SRCS = $(shell find $(CCSRCDIR) -type f -name \*.16.c)

AS64SRCS = $(shell find $(ASSRCDIR) -type f -name \*64.asm)
CC64SRCS = $(shell find $(CCSRCDIR) -type f -name \*.64.c)

CCXXSRCS = $(shell find $(CCSRCDIR) -type f -name \*.xx.c)

LDSRCS = $(shell find $(LDSRCDIR) -type f -name \*.ld)

M4SRCS = $(shell find $(M4SRCDIR) -type f -name \*.m4)

UTILSSRCS = $(shell find $(UTILSSRCDIR) -type f -name \*.c)

ASOBJS = $(patsubst $(ASSRCDIR)/%.asm,$(ASOBJDIR)/%.o,$(ASSRCS))
CC16OBJS = $(patsubst $(CCSRCDIR)/%.16.c,$(CCOBJDIR)/%.16.o,$(CC16SRCS))
CC16OBJS += $(patsubst $(CCSRCDIR)/%.xx.c,$(CCOBJDIR)/%.xx_16.o,$(CCXXSRCS))
CC64OBJS = $(patsubst $(CCSRCDIR)/%.64.c,$(CCOBJDIR)/%.64.o,$(CC64SRCS))
CC64OBJS += $(patsubst $(CCSRCDIR)/%.xx.c,$(CCOBJDIR)/%.xx_64.o,$(CCXXSRCS))
M4OBJS = $(patsubst $(M4SRCDIR)/%.m4,$(M4OBJDIR)/%.c,$(M4SRCS))

OBJS = $(ASOBJS) $(CC16OBJS) $(CC64OBJS)
PROGS = $(OBJDIR)/bootsect16.bin \
	$(OBJDIR)/slottable.bin \
	$(OBJDIR)/stage2.bin \
	$(OBJDIR)/stage3.bin

SUBDIRS := tests utils

.PHONY: all disk clean depend $(SUBDIRS)
.PRECIOUS: $(M4OBJS)
all: docs disk

docs: $(DOCSCONF) $(DOCSFILES)
	$(DOCSGEN) $(DOCSCONF)

disk: kernel
	dd bs=512 conv=notrunc if=$(OBJDIR)/$< of=/Volumes/DATA/VirtualBox\ VMs/osdev/rawdisk0.raw

kernel: $(OBJDIR)/kernel $(SUBDIRS)
	$(OBJDIR)/formatslots.bin $< $(PROGS)

$(OBJDIR)/kernel: $(PROGS)
	cat $(PROGS) > $@

$(OBJDIR)/bootsect16.bin: $(LDSRCDIR)/bootsect.ld $(ASOBJDIR)/bootsect16.o
	$(LD16) $(LDFLAGS) --script=$<  -o $@ $(filter-out $<,$^)

$(OBJDIR)/slottable.bin: $(LDSRCDIR)/slottableprotect.ld $(ASOBJDIR)/slottableprotect16.o
	$(LD16) $(LDFLAGS) --script=$<  -o $@ $(filter-out $<,$^)

$(OBJDIR)/stage2.bin: $(LDSRCDIR)/stage2.ld $(ASOBJDIR)/kentry16.o $(CC16OBJS)
	$(LD16) $(LD16FLAGS) --script=$< -o $@ $(filter-out $<,$^)

$(OBJDIR)/stage3.bin: $(LDSRCDIR)/stage3.ld $(ASOBJDIR)/kentry64.o $(CC64OBJS)
	$(LD64) $(LD64FLAGS) -T $< -o $@ $(filter-out $<,$^)

$(CCOBJDIR)/%.16.o: $(CCSRCDIR)/%.16.c $(M4OBJS)
	$(CC16) $(CC16FLAGS) -o $@ $<

$(CCOBJDIR)/%.xx_16.o: $(CCSRCDIR)/%.xx.c $(M4OBJS)
	$(CC16) $(CC16FLAGS) -o $@ $<

$(CCOBJDIR)/%.64.o: $(CCSRCDIR)/%.64.c $(M4OBJS)
	$(CC64) $(CC64FLAGS) -o $@ $<

$(CCOBJDIR)/%.xx_64.o: $(CCSRCDIR)/%.xx.c $(M4OBJS)
	$(CC64) $(CC64FLAGS) -o $@ $<

$(ASOBJDIR)/%16.o: $(ASSRCDIR)/%16.asm
	$(AS16) $(AS16FLAGS) -o $@ $^

$(ASOBJDIR)/%64.o: $(ASSRCDIR)/%64.asm
	$(AS64) $(AS64FLAGS) -o $@ $^

$(M4OBJDIR)/%.c: $(M4SRCDIR)/%.m4
	$(M4) $^ > $@

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	rm -fr $(DOCSOBJDIR)/*
	find $(OBJDIR) -type f -delete
	rm -f .depend*
	find $(M4OBJDIR) -type f -delete

print-%: ; @echo $* = $($*)

depend: .depend16 .depend64

.depend16: $(CC16SRCS) $(CCXXSRCS)
	$(CC16) $(CC16FLAGS) -D___DEPEND_ANALYSIS -MM $^ | sed -E 's%^(.*):%'$(CCOBJDIR)'/\1:%g' > .depend16
	sed -i '' 's/xx.o:/xx_16.o:/g' .depend16

.depend64: $(CC64SRCS) $(CCXXSRCS)
	$(CC64) $(CC64FLAGS) -D___DEPEND_ANALYSIS -MM $^ | sed -E 's%^(.*):%'$(CCOBJDIR)'/\1:%g' > .depend64
	sed -i '' 's/xx.o:/xx_64.o:/g' .depend64

-include .depend16 .depend64
