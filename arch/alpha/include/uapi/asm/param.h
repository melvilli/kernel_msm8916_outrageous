#ifndef _UAPI_ASM_ALPHA_PARAM_H
#define _UAPI_ASM_ALPHA_PARAM_H

<<<<<<< HEAD
<<<<<<< HEAD
/* ??? Gross.  I don't want to parameterize this, and supposedly the
   hardware ignores reprogramming.  We also need userland buy-in to the 
   change in HZ, since this is visible in the wait4 resources etc.  */

#ifndef __KERNEL__
#define HZ		1024
#endif
=======
#define HZ		1024
>>>>>>> v3.18
=======
#define HZ		1024
>>>>>>> v3.18

#define EXEC_PAGESIZE	8192

#ifndef NOGROUP
#define NOGROUP		(-1)
#endif

#define MAXHOSTNAMELEN	64	/* max length of hostname */

<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif /* _UAPI_ASM_ALPHA_PARAM_H */
