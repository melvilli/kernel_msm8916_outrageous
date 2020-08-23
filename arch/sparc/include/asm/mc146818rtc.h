#ifndef ___ASM_SPARC_MC146818RTC_H
#define ___ASM_SPARC_MC146818RTC_H
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18

#include <linux/spinlock.h>

extern spinlock_t rtc_lock;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#if defined(__sparc__) && defined(__arch64__)
#include <asm/mc146818rtc_64.h>
#else
#include <asm/mc146818rtc_32.h>
#endif
#endif
