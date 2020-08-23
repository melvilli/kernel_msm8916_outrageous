Linux Kernel Selftests

The kernel contains a set of "self tests" under the tools/testing/selftests/
directory. These are intended to be small unit tests to exercise individual
code paths in the kernel.

<<<<<<< HEAD
<<<<<<< HEAD
Running the selftests
=====================
=======
=======
>>>>>>> v3.18
On some systems, hot-plug tests could hang forever waiting for cpu and
memory to be ready to be offlined. A special hot-plug target is created
to run full range of hot-plug tests. In default mode, hot-plug tests run
in safe mode with a limited scope. In limited mode, cpu-hotplug test is
run on a single cpu as opposed to all hotplug capable cpus, and memory
hotplug test is run on 2% of hotplug capable memory instead of 10%.

Running the selftests (hotplug tests are run in limited mode)
=============================================================
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

To build the tests:

  $ make -C tools/testing/selftests


To run the tests:

  $ make -C tools/testing/selftests run_tests

- note that some tests will require root privileges.

<<<<<<< HEAD
<<<<<<< HEAD

To run only tests targetted for a single subsystem:
=======
To run only tests targeted for a single subsystem: (including
hotplug targets in limited mode)
>>>>>>> v3.18
=======
To run only tests targeted for a single subsystem: (including
hotplug targets in limited mode)
>>>>>>> v3.18

  $  make -C tools/testing/selftests TARGETS=cpu-hotplug run_tests

See the top-level tools/testing/selftests/Makefile for the list of all possible
targets.

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
Running the full range hotplug selftests
========================================

To build the tests:

  $ make -C tools/testing/selftests hotplug

To run the tests:

  $ make -C tools/testing/selftests run_hotplug

- note that some tests will require root privileges.
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

Contributing new tests
======================

In general, the rules for for selftests are

 * Do as much as you can if you're not root;

 * Don't take too long;

 * Don't break the build on any architecture, and

 * Don't cause the top-level "make run_tests" to fail if your feature is
   unconfigured.
