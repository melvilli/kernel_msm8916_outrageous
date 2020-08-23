#ifndef ___ASM_SPARC_AUXIO_H
#define ___ASM_SPARC_AUXIO_H
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18

#ifndef __ASSEMBLY__

extern void __iomem *auxio_register;

#endif /* ifndef __ASSEMBLY__ */

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#if defined(__sparc__) && defined(__arch64__)
#include <asm/auxio_64.h>
#else
#include <asm/auxio_32.h>
#endif
#endif
