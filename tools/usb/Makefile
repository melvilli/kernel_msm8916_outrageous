# Makefile for USB tools

CC = $(CROSS_COMPILE)gcc
PTHREAD_LIBS = -lpthread
WARNINGS = -Wall -Wextra
<<<<<<< HEAD
CFLAGS = $(WARNINGS) -g $(PTHREAD_LIBS) -I../include

all: testusb ffs-test
%: %.c
	$(CC) $(CFLAGS) -o $@ $^
=======
CFLAGS = $(WARNINGS) -g -I../include
LDFLAGS = $(PTHREAD_LIBS)

all: testusb ffs-test
%: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
>>>>>>> v3.18

clean:
	$(RM) testusb ffs-test
