<<<<<<< HEAD
# kbuild trick to avoid linker error. Can be omitted if a module is built.
obj- := dummy.o

# List of programs to build
hostprogs-y := timestamping
=======
# To compile, from the source root
#
#    make headers_install
#    make M=documentation

# List of programs to build
hostprogs-y := hwtstamp_config timestamping txtimestamp
>>>>>>> v3.18

# Tell kbuild to always build the programs
always := $(hostprogs-y)

HOSTCFLAGS_timestamping.o += -I$(objtree)/usr/include
<<<<<<< HEAD

clean:
	rm -f timestamping
=======
HOSTCFLAGS_txtimestamp.o += -I$(objtree)/usr/include
HOSTCFLAGS_hwtstamp_config.o += -I$(objtree)/usr/include
>>>>>>> v3.18
