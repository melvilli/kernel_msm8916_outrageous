#ifndef __PERF_STATS_H
#define __PERF_STATS_H

<<<<<<< HEAD
<<<<<<< HEAD
#include "types.h"
=======
#include <linux/types.h>
>>>>>>> v3.18
=======
#include <linux/types.h>
>>>>>>> v3.18

struct stats
{
	double n, mean, M2;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	u64 max, min;
>>>>>>> v3.18
=======
	u64 max, min;
>>>>>>> v3.18
};

void update_stats(struct stats *stats, u64 val);
double avg_stats(struct stats *stats);
double stddev_stats(struct stats *stats);
double rel_stddev_stats(double stddev, double avg);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
static inline void init_stats(struct stats *stats)
{
	stats->n    = 0.0;
	stats->mean = 0.0;
	stats->M2   = 0.0;
	stats->min  = (u64) -1;
	stats->max  = 0;
}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif
