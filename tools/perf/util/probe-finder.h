#ifndef _PROBE_FINDER_H
#define _PROBE_FINDER_H

#include <stdbool.h>
#include "util.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "intlist.h"
>>>>>>> v3.18
=======
#include "intlist.h"
>>>>>>> v3.18
#include "probe-event.h"

#define MAX_PROBE_BUFFER	1024
#define MAX_PROBES		 128
<<<<<<< HEAD
<<<<<<< HEAD
=======
#define MAX_PROBE_ARGS		 128
>>>>>>> v3.18
=======
#define MAX_PROBE_ARGS		 128
>>>>>>> v3.18

static inline int is_c_varname(const char *name)
{
	/* TODO */
	return isalpha(name[0]) || name[0] == '_';
}

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef DWARF_SUPPORT
=======
#ifdef HAVE_DWARF_SUPPORT
>>>>>>> v3.18
=======
#ifdef HAVE_DWARF_SUPPORT
>>>>>>> v3.18

#include "dwarf-aux.h"

/* TODO: export debuginfo data structure even if no dwarf support */

/* debug information structure */
struct debuginfo {
	Dwarf		*dbg;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	Dwfl_Module	*mod;
>>>>>>> v3.18
=======
	Dwfl_Module	*mod;
>>>>>>> v3.18
	Dwfl		*dwfl;
	Dwarf_Addr	bias;
};

<<<<<<< HEAD
<<<<<<< HEAD
extern struct debuginfo *debuginfo__new(const char *path);
extern struct debuginfo *debuginfo__new_online_kernel(unsigned long addr);
extern void debuginfo__delete(struct debuginfo *self);

/* Find probe_trace_events specified by perf_probe_event from debuginfo */
extern int debuginfo__find_trace_events(struct debuginfo *self,
=======
=======
>>>>>>> v3.18
/* This also tries to open distro debuginfo */
extern struct debuginfo *debuginfo__new(const char *path);
extern void debuginfo__delete(struct debuginfo *dbg);

/* Find probe_trace_events specified by perf_probe_event from debuginfo */
extern int debuginfo__find_trace_events(struct debuginfo *dbg,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
					struct perf_probe_event *pev,
					struct probe_trace_event **tevs,
					int max_tevs);

/* Find a perf_probe_point from debuginfo */
<<<<<<< HEAD
<<<<<<< HEAD
extern int debuginfo__find_probe_point(struct debuginfo *self,
=======
extern int debuginfo__find_probe_point(struct debuginfo *dbg,
>>>>>>> v3.18
=======
extern int debuginfo__find_probe_point(struct debuginfo *dbg,
>>>>>>> v3.18
				       unsigned long addr,
				       struct perf_probe_point *ppt);

/* Find a line range */
<<<<<<< HEAD
<<<<<<< HEAD
extern int debuginfo__find_line_range(struct debuginfo *self,
				      struct line_range *lr);

/* Find available variables */
extern int debuginfo__find_available_vars_at(struct debuginfo *self,
=======
=======
>>>>>>> v3.18
extern int debuginfo__find_line_range(struct debuginfo *dbg,
				      struct line_range *lr);

/* Find available variables */
extern int debuginfo__find_available_vars_at(struct debuginfo *dbg,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
					     struct perf_probe_event *pev,
					     struct variable_list **vls,
					     int max_points, bool externs);

struct probe_finder {
	struct perf_probe_event	*pev;		/* Target probe event */

	/* Callback when a probe point is found */
	int (*callback)(Dwarf_Die *sc_die, struct probe_finder *pf);

	/* For function searching */
	int			lno;		/* Line number */
	Dwarf_Addr		addr;		/* Address */
	const char		*fname;		/* Real file name */
	Dwarf_Die		cu_die;		/* Current CU */
	Dwarf_Die		sp_die;
<<<<<<< HEAD
<<<<<<< HEAD
	struct list_head	lcache;		/* Line cache for lazy match */
=======
	struct intlist		*lcache;	/* Line cache for lazy match */
>>>>>>> v3.18
=======
	struct intlist		*lcache;	/* Line cache for lazy match */
>>>>>>> v3.18

	/* For variable searching */
#if _ELFUTILS_PREREQ(0, 142)
	Dwarf_CFI		*cfi;		/* Call Frame Information */
#endif
	Dwarf_Op		*fb_ops;	/* Frame base attribute */
	struct perf_probe_arg	*pvar;		/* Current target variable */
	struct probe_trace_arg	*tvar;		/* Current result variable */
};

struct trace_event_finder {
	struct probe_finder	pf;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	Dwfl_Module		*mod;		/* For solving symbols */
>>>>>>> v3.18
=======
	Dwfl_Module		*mod;		/* For solving symbols */
>>>>>>> v3.18
	struct probe_trace_event *tevs;		/* Found trace events */
	int			ntevs;		/* Number of trace events */
	int			max_tevs;	/* Max number of trace events */
};

struct available_var_finder {
	struct probe_finder	pf;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	Dwfl_Module		*mod;		/* For solving symbols */
>>>>>>> v3.18
=======
	Dwfl_Module		*mod;		/* For solving symbols */
>>>>>>> v3.18
	struct variable_list	*vls;		/* Found variable lists */
	int			nvls;		/* Number of variable lists */
	int			max_vls;	/* Max no. of variable lists */
	bool			externs;	/* Find external vars too */
	bool			child;		/* Search child scopes */
};

struct line_finder {
	struct line_range	*lr;		/* Target line range */

	const char		*fname;		/* File name */
	int			lno_s;		/* Start line number */
	int			lno_e;		/* End line number */
	Dwarf_Die		cu_die;		/* Current CU */
	Dwarf_Die		sp_die;
	int			found;
};

<<<<<<< HEAD
<<<<<<< HEAD
#endif /* DWARF_SUPPORT */
=======
#endif /* HAVE_DWARF_SUPPORT */
>>>>>>> v3.18
=======
#endif /* HAVE_DWARF_SUPPORT */
>>>>>>> v3.18

#endif /*_PROBE_FINDER_H */
