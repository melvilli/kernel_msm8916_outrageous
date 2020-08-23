#ifndef CEPH_CRUSH_MAPPER_H
#define CEPH_CRUSH_MAPPER_H

/*
 * CRUSH functions for find rules and then mapping an input to an
 * output set.
 *
 * LGPL2
 */

#include <linux/crush/crush.h>

extern int crush_find_rule(const struct crush_map *map, int ruleset, int type, int size);
extern int crush_do_rule(const struct crush_map *map,
			 int ruleno,
			 int x, int *result, int result_max,
<<<<<<< HEAD
<<<<<<< HEAD
			 const __u32 *weights);
=======
			 const __u32 *weights, int weight_max,
			 int *scratch);
>>>>>>> v3.18
=======
			 const __u32 *weights, int weight_max,
			 int *scratch);
>>>>>>> v3.18

#endif
