#ifndef _ASM_ALPHA_PARAM_H
#define _ASM_ALPHA_PARAM_H

#include <uapi/asm/param.h>

<<<<<<< HEAD
<<<<<<< HEAD
#define HZ		CONFIG_HZ
#define USER_HZ		HZ
# define CLOCKS_PER_SEC	HZ	/* frequency at which times() counts */
=======
=======
>>>>>>> v3.18
# undef HZ
# define HZ		CONFIG_HZ
# define USER_HZ	1024
# define CLOCKS_PER_SEC	USER_HZ	/* frequency at which times() counts */

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif /* _ASM_ALPHA_PARAM_H */
