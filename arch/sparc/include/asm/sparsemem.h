#ifndef _SPARC64_SPARSEMEM_H
#define _SPARC64_SPARSEMEM_H

#ifdef __KERNEL__

<<<<<<< HEAD
<<<<<<< HEAD
#define SECTION_SIZE_BITS       30
#define MAX_PHYSADDR_BITS       42
#define MAX_PHYSMEM_BITS        42
=======
=======
>>>>>>> v3.18
#include <asm/page.h>

#define SECTION_SIZE_BITS       30
#define MAX_PHYSADDR_BITS       MAX_PHYS_ADDRESS_BITS
#define MAX_PHYSMEM_BITS        MAX_PHYS_ADDRESS_BITS
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

#endif /* !(__KERNEL__) */

#endif /* !(_SPARC64_SPARSEMEM_H) */
