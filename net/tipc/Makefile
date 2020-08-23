#
# Makefile for the Linux TIPC layer
#

obj-$(CONFIG_TIPC) := tipc.o

tipc-y	+= addr.o bcast.o bearer.o config.o \
<<<<<<< HEAD
<<<<<<< HEAD
	   core.o handler.o link.o discover.o msg.o  \
	   name_distr.o  subscr.o name_table.o net.o  \
	   netlink.o node.o node_subscr.o port.o ref.o  \
	   socket.o log.o eth_media.o

tipc-$(CONFIG_TIPC_MEDIA_IB)	+= ib_media.o
=======
=======
>>>>>>> v3.18
	   core.o link.o discover.o msg.o  \
	   name_distr.o  subscr.o name_table.o net.o  \
	   netlink.o node.o node_subscr.o \
	   socket.o log.o eth_media.o server.o

tipc-$(CONFIG_TIPC_MEDIA_IB)	+= ib_media.o
tipc-$(CONFIG_SYSCTL)		+= sysctl.o
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
