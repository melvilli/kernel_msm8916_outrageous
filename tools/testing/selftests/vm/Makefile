# Makefile for vm selftests

CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall
<<<<<<< HEAD
<<<<<<< HEAD

all: hugepage-mmap hugepage-shm  map_hugetlb thuge-gen
=======
=======
>>>>>>> v3.18
BINARIES = hugepage-mmap hugepage-shm map_hugetlb thuge-gen hugetlbfstest
BINARIES += transhuge-stress

all: $(BINARIES)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
%: %.c
	$(CC) $(CFLAGS) -o $@ $^

run_tests: all
<<<<<<< HEAD
<<<<<<< HEAD
	@/bin/sh ./run_vmtests || echo "vmtests: [FAIL]"

clean:
	$(RM) hugepage-mmap hugepage-shm  map_hugetlb
=======
=======
>>>>>>> v3.18
	@/bin/sh ./run_vmtests || (echo "vmtests: [FAIL]"; exit 1)

clean:
	$(RM) $(BINARIES)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
