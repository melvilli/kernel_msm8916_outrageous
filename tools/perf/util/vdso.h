#ifndef __PERF_VDSO__
#define __PERF_VDSO__

#include <linux/types.h>
#include <string.h>
#include <stdbool.h>

#define VDSO__MAP_NAME "[vdso]"

<<<<<<< HEAD
<<<<<<< HEAD
=======
#define DSO__NAME_VDSO "[vdso]"

>>>>>>> v3.18
=======
#define DSO__NAME_VDSO "[vdso]"

>>>>>>> v3.18
static inline bool is_vdso_map(const char *filename)
{
	return !strcmp(filename, VDSO__MAP_NAME);
}

<<<<<<< HEAD
<<<<<<< HEAD
struct dso *vdso__dso_findnew(struct list_head *head);
void vdso__exit(void);
=======
=======
>>>>>>> v3.18
struct dso;

bool dso__is_vdso(struct dso *dso);

struct machine;
struct thread;

struct dso *vdso__dso_findnew(struct machine *machine, struct thread *thread);
void vdso__exit(struct machine *machine);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

#endif /* __PERF_VDSO__ */
