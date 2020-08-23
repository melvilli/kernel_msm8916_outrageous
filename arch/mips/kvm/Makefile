# Makefile for KVM support for MIPS
#

common-objs = $(addprefix ../../../virt/kvm/, kvm_main.o coalesced_mmio.o)

EXTRA_CFLAGS += -Ivirt/kvm -Iarch/mips/kvm

<<<<<<< HEAD
<<<<<<< HEAD
kvm-objs := $(common-objs) kvm_mips.o kvm_mips_emul.o kvm_locore.o \
	    kvm_mips_int.o kvm_mips_stats.o kvm_mips_commpage.o \
	    kvm_mips_dyntrans.o kvm_trap_emul.o

obj-$(CONFIG_KVM)	+= kvm.o
obj-y			+= kvm_cb.o kvm_tlb.o
=======
=======
>>>>>>> v3.18
kvm-objs := $(common-objs) mips.o emulate.o locore.o \
	    interrupt.o stats.o commpage.o \
	    dyntrans.o trap_emul.o

obj-$(CONFIG_KVM)	+= kvm.o
obj-y			+= callback.o tlb.o
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
