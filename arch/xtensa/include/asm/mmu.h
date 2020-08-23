/*
<<<<<<< HEAD
 * include/asm-xtensa/mmu.h
 *
=======
>>>>>>> v3.18
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
<<<<<<< HEAD
 * Copyright (C) 2001 - 2005 Tensilica Inc.
=======
 * Copyright (C) 2001 - 2013 Tensilica Inc.
>>>>>>> v3.18
 */

#ifndef _XTENSA_MMU_H
#define _XTENSA_MMU_H

#ifndef CONFIG_MMU
#include <asm-generic/mmu.h>
#else

<<<<<<< HEAD
/* Default "unsigned long" context */
typedef unsigned long mm_context_t;
=======
typedef struct {
	unsigned long asid[NR_CPUS];
	unsigned int cpu;
} mm_context_t;
>>>>>>> v3.18

#endif /* CONFIG_MMU */
#endif	/* _XTENSA_MMU_H */
