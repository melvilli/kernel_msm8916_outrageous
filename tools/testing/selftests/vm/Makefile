# Makefile for vm selftests

CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall
<<<<<<< HEAD

all: hugepage-mmap hugepage-shm  map_hugetlb thuge-gen
=======
BINARIES = hugepage-mmap hugepage-shm map_hugetlb thuge-gen hugetlbfstest
BINARIES += transhuge-stress

all: $(BINARIES)
>>>>>>> v3.18
%: %.c
	$(CC) $(CFLAGS) -o $@ $^

run_tests: all
<<<<<<< HEAD
	@/bin/sh ./run_vmtests || echo "vmtests: [FAIL]"

clean:
	$(RM) hugepage-mmap hugepage-shm  map_hugetlb
=======
	@/bin/sh ./run_vmtests || (echo "vmtests: [FAIL]"; exit 1)

clean:
	$(RM) $(BINARIES)
>>>>>>> v3.18
