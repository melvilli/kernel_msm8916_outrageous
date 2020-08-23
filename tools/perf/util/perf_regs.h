#ifndef __PERF_REGS_H
#define __PERF_REGS_H

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef HAVE_PERF_REGS
#include <perf_regs.h>
#else
#define PERF_REGS_MASK	0
=======
=======
>>>>>>> v3.18
#include <linux/types.h>

struct regs_dump;

#ifdef HAVE_PERF_REGS_SUPPORT
#include <perf_regs.h>

int perf_reg_value(u64 *valp, struct regs_dump *regs, int id);

#else
#define PERF_REGS_MASK	0
#define PERF_REGS_MAX	0
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

static inline const char *perf_reg_name(int id __maybe_unused)
{
	return NULL;
}
<<<<<<< HEAD
<<<<<<< HEAD
#endif /* HAVE_PERF_REGS */
=======
=======
>>>>>>> v3.18

static inline int perf_reg_value(u64 *valp __maybe_unused,
				 struct regs_dump *regs __maybe_unused,
				 int id __maybe_unused)
{
	return 0;
}
#endif /* HAVE_PERF_REGS_SUPPORT */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif /* __PERF_REGS_H */
