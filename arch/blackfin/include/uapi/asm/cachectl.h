/*
 * based on the mips/cachectl.h
 *
 * Copyright 2010 Analog Devices Inc.
 * Copyright (C) 1994, 1995, 1996 by Ralf Baechle
 *
 * Licensed under the GPL-2 or later.
 */

<<<<<<< HEAD
<<<<<<< HEAD
#ifndef	_ASM_CACHECTL
#define	_ASM_CACHECTL
=======
#ifndef _UAPI_ASM_CACHECTL
#define _UAPI_ASM_CACHECTL
>>>>>>> v3.18
=======
#ifndef _UAPI_ASM_CACHECTL
#define _UAPI_ASM_CACHECTL
>>>>>>> v3.18

/*
 * Options for cacheflush system call
 */
#define	ICACHE	(1<<0)		/* flush instruction cache        */
#define	DCACHE	(1<<1)		/* writeback and flush data cache */
#define	BCACHE	(ICACHE|DCACHE)	/* flush both caches              */

<<<<<<< HEAD
<<<<<<< HEAD
#endif	/* _ASM_CACHECTL */
=======
#endif /* _UAPI_ASM_CACHECTL */
>>>>>>> v3.18
=======
#endif /* _UAPI_ASM_CACHECTL */
>>>>>>> v3.18
