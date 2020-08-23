/*
 * Linker script for x32 vDSO.
 * We #include the file to define the layout details.
<<<<<<< HEAD
<<<<<<< HEAD
 * Here we only choose the prelinked virtual address.
 *
 * This file defines the version script giving the user-exported symbols in
 * the DSO.  We can define local symbols here called VDSO* to make their
 * values visible using the asm-x86/vdso.h macros from the kernel proper.
 */

#define VDSO_PRELINK 0
=======
=======
>>>>>>> v3.18
 *
 * This file defines the version script giving the user-exported symbols in
 * the DSO.
 */

#define BUILD_VDSOX32

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#include "vdso-layout.lds.S"

/*
 * This controls what userland symbols we export from the vDSO.
 */
VERSION {
	LINUX_2.6 {
	global:
		__vdso_clock_gettime;
		__vdso_gettimeofday;
		__vdso_getcpu;
		__vdso_time;
	local: *;
	};
}
<<<<<<< HEAD
<<<<<<< HEAD

VDSOX32_PRELINK = VDSO_PRELINK;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
