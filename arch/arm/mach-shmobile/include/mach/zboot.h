#ifndef ZBOOT_H
#define ZBOOT_H

<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/mach-types.h>
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
#include <mach/zboot_macros.h>

/**************************************************
 *
 *		board specific settings
 *
 **************************************************/

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_MACH_AP4EVB
#define MACH_TYPE	MACH_TYPE_AP4EVB
#include "mach/head-ap4evb.txt"
#elif defined(CONFIG_MACH_MACKEREL)
#define MACH_TYPE	MACH_TYPE_MACKEREL
#include "mach/head-mackerel.txt"
=======
=======
>>>>>>> v3.18
#ifdef CONFIG_MACH_MACKEREL
#define MEMORY_START	0x40000000
#include "mach/head-mackerel.txt"
#elif defined(CONFIG_MACH_KZM9G) || defined(CONFIG_MACH_KZM9G_REFERENCE)
#define MEMORY_START	0x43000000
#include "mach/head-kzm9g.txt"
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#else
#error "unsupported board."
#endif

#endif /* ZBOOT_H */
