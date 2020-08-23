# Makefile for vm tools
#
TARGETS=page-types slabinfo

<<<<<<< HEAD
<<<<<<< HEAD
LK_DIR = ../lib/lk
LIBLK = $(LK_DIR)/liblk.a

CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra -I../lib/
LDFLAGS = $(LIBLK)

$(TARGETS): liblk

liblk:
	make -C $(LK_DIR)
=======
=======
>>>>>>> v3.18
LIB_DIR = ../lib/api
LIBS = $(LIB_DIR)/libapikfs.a

CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra -I../lib/
LDFLAGS = $(LIBS)

$(TARGETS): $(LIBS)

$(LIBS):
	make -C $(LIB_DIR)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	$(RM) page-types slabinfo
<<<<<<< HEAD
<<<<<<< HEAD
	make -C ../lib/lk clean
=======
	make -C $(LIB_DIR) clean
>>>>>>> v3.18
=======
	make -C $(LIB_DIR) clean
>>>>>>> v3.18
