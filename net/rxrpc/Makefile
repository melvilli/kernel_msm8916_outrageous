#
# Makefile for Linux kernel RxRPC
#

af-rxrpc-y := \
	af_rxrpc.o \
	ar-accept.o \
	ar-ack.o \
	ar-call.o \
	ar-connection.o \
	ar-connevent.o \
	ar-error.o \
	ar-input.o \
	ar-key.o \
	ar-local.o \
	ar-output.o \
	ar-peer.o \
	ar-recvmsg.o \
	ar-security.o \
	ar-skbuff.o \
	ar-transport.o

<<<<<<< HEAD
ifeq ($(CONFIG_PROC_FS),y)
af-rxrpc-y += ar-proc.o
endif
=======
af-rxrpc-$(CONFIG_PROC_FS) += ar-proc.o
af-rxrpc-$(CONFIG_SYSCTL) += sysctl.o
>>>>>>> v3.18

obj-$(CONFIG_AF_RXRPC) += af-rxrpc.o

obj-$(CONFIG_RXKAD) += rxkad.o
