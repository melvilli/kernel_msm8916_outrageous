TARGETS = breakpoints
TARGETS += cpu-hotplug
TARGETS += efivarfs
TARGETS += kcmp
<<<<<<< HEAD
<<<<<<< HEAD
=======
TARGETS += memfd
>>>>>>> v3.18
=======
TARGETS += memfd
>>>>>>> v3.18
TARGETS += memory-hotplug
TARGETS += mqueue
TARGETS += mount
TARGETS += net
TARGETS += ptrace
<<<<<<< HEAD
<<<<<<< HEAD
TARGETS += vm
=======
=======
>>>>>>> v3.18
TARGETS += timers
TARGETS += vm
TARGETS += powerpc
TARGETS += user
TARGETS += sysctl
TARGETS += firmware
TARGETS += ftrace

TARGETS_HOTPLUG = cpu-hotplug
TARGETS_HOTPLUG += memory-hotplug
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

all:
	for TARGET in $(TARGETS); do \
		make -C $$TARGET; \
	done;

run_tests: all
	for TARGET in $(TARGETS); do \
		make -C $$TARGET run_tests; \
	done;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
hotplug:
	for TARGET in $(TARGETS_HOTPLUG); do \
		make -C $$TARGET; \
	done;

run_hotplug: hotplug
	for TARGET in $(TARGETS_HOTPLUG); do \
		make -C $$TARGET run_full_test; \
	done;

clean_hotplug:
	for TARGET in $(TARGETS_HOTPLUG); do \
		make -C $$TARGET clean; \
	done;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
clean:
	for TARGET in $(TARGETS); do \
		make -C $$TARGET clean; \
	done;
