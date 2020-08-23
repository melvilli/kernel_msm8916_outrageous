# Makefile for USB tools

CC = $(CROSS_COMPILE)gcc
PTHREAD_LIBS = -lpthread
WARNINGS = -Wall -Wextra
<<<<<<< HEAD
<<<<<<< HEAD
CFLAGS = $(WARNINGS) -g $(PTHREAD_LIBS) -I../include

all: testusb ffs-test
%: %.c
	$(CC) $(CFLAGS) -o $@ $^
=======
=======
>>>>>>> v3.18
CFLAGS = $(WARNINGS) -g -I../include
LDFLAGS = $(PTHREAD_LIBS)

all: testusb ffs-test
%: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

clean:
	$(RM) testusb ffs-test
