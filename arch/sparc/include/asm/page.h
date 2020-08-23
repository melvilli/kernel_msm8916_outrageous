#ifndef ___ASM_SPARC_PAGE_H
#define ___ASM_SPARC_PAGE_H
<<<<<<< HEAD
<<<<<<< HEAD
=======

#define page_to_phys(page)	(page_to_pfn(page) << PAGE_SHIFT)

>>>>>>> v3.18
=======

#define page_to_phys(page)	(page_to_pfn(page) << PAGE_SHIFT)

>>>>>>> v3.18
#if defined(__sparc__) && defined(__arch64__)
#include <asm/page_64.h>
#else
#include <asm/page_32.h>
#endif
#endif
