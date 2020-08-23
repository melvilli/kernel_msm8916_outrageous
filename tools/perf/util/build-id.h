#ifndef PERF_BUILD_ID_H_
#define PERF_BUILD_ID_H_ 1

#define BUILD_ID_SIZE 20

#include "tool.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "types.h"
=======
#include <linux/types.h>
>>>>>>> v3.18
=======
#include <linux/types.h>
>>>>>>> v3.18

extern struct perf_tool build_id__mark_dso_hit_ops;
struct dso;

int build_id__sprintf(const u8 *build_id, int len, char *bf);
<<<<<<< HEAD
<<<<<<< HEAD
char *dso__build_id_filename(struct dso *self, char *bf, size_t size);
=======
char *dso__build_id_filename(const struct dso *dso, char *bf, size_t size);
>>>>>>> v3.18
=======
char *dso__build_id_filename(const struct dso *dso, char *bf, size_t size);
>>>>>>> v3.18

int build_id__mark_dso_hit(struct perf_tool *tool, union perf_event *event,
			   struct perf_sample *sample, struct perf_evsel *evsel,
			   struct machine *machine);
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif
