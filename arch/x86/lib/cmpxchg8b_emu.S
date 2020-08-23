/*
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; version 2
 *	of the License.
 *
 */

#include <linux/linkage.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/alternative-asm.h>
#include <asm/frame.h>
#include <asm/dwarf2.h>


=======
#include <asm/dwarf2.h>

>>>>>>> v3.18
=======
#include <asm/dwarf2.h>

>>>>>>> v3.18
.text

/*
 * Inputs:
 * %esi : memory location to compare
 * %eax : low 32 bits of old value
 * %edx : high 32 bits of old value
 * %ebx : low 32 bits of new value
 * %ecx : high 32 bits of new value
 */
ENTRY(cmpxchg8b_emu)
CFI_STARTPROC

#
# Emulate 'cmpxchg8b (%esi)' on UP except we don't
# set the whole ZF thing (caller will just compare
# eax:edx with the expected value)
#
<<<<<<< HEAD
<<<<<<< HEAD
cmpxchg8b_emu:
	pushfl
	cli

	cmpl  (%esi), %eax
	jne not_same
	cmpl 4(%esi), %edx
	jne half_same
=======
=======
>>>>>>> v3.18
	pushfl_cfi
	cli

	cmpl  (%esi), %eax
	jne .Lnot_same
	cmpl 4(%esi), %edx
	jne .Lhalf_same
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	movl %ebx,  (%esi)
	movl %ecx, 4(%esi)

<<<<<<< HEAD
<<<<<<< HEAD
	popfl
	ret

 not_same:
	movl  (%esi), %eax
 half_same:
	movl 4(%esi), %edx

	popfl
=======
=======
>>>>>>> v3.18
	CFI_REMEMBER_STATE
	popfl_cfi
	ret

	CFI_RESTORE_STATE
.Lnot_same:
	movl  (%esi), %eax
.Lhalf_same:
	movl 4(%esi), %edx

	popfl_cfi
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	ret

CFI_ENDPROC
ENDPROC(cmpxchg8b_emu)
