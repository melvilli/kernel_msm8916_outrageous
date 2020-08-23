<<<<<<< HEAD
# kbuild trick to avoid linker error. Can be omitted if a module is built.
obj- := dummy.o

=======
>>>>>>> v3.18
# List of programs to build
hostprogs-y := getdelays

# Tell kbuild to always build the programs
always := $(hostprogs-y)

HOSTCFLAGS_getdelays.o += -I$(objtree)/usr/include
