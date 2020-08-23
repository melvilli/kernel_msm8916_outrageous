#ifndef __PERF_TOOL_H
#define __PERF_TOOL_H

#include <stdbool.h>

struct perf_session;
union perf_event;
struct perf_evlist;
struct perf_evsel;
struct perf_sample;
struct perf_tool;
struct machine;

typedef int (*event_sample)(struct perf_tool *tool, union perf_event *event,
			    struct perf_sample *sample,
			    struct perf_evsel *evsel, struct machine *machine);

typedef int (*event_op)(struct perf_tool *tool, union perf_event *event,
			struct perf_sample *sample, struct machine *machine);

<<<<<<< HEAD
<<<<<<< HEAD
typedef int (*event_attr_op)(union perf_event *event,
			     struct perf_evlist **pevlist);
typedef int (*event_simple_op)(struct perf_tool *tool, union perf_event *event);

typedef int (*event_synth_op)(union perf_event *event,
			      struct perf_session *session);
=======
typedef int (*event_attr_op)(struct perf_tool *tool,
			     union perf_event *event,
			     struct perf_evlist **pevlist);
>>>>>>> v3.18
=======
typedef int (*event_attr_op)(struct perf_tool *tool,
			     union perf_event *event,
			     struct perf_evlist **pevlist);
>>>>>>> v3.18

typedef int (*event_op2)(struct perf_tool *tool, union perf_event *event,
			 struct perf_session *session);

struct perf_tool {
	event_sample	sample,
			read;
	event_op	mmap,
<<<<<<< HEAD
<<<<<<< HEAD
=======
			mmap2,
>>>>>>> v3.18
=======
			mmap2,
>>>>>>> v3.18
			comm,
			fork,
			exit,
			lost,
			throttle,
			unthrottle;
	event_attr_op	attr;
<<<<<<< HEAD
<<<<<<< HEAD
	event_synth_op	tracing_data;
	event_simple_op	event_type;
	event_op2	finished_round,
			build_id;
	bool		ordered_samples;
=======
=======
>>>>>>> v3.18
	event_op2	tracing_data;
	event_op2	finished_round,
			build_id;
	bool		ordered_events;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	bool		ordering_requires_timestamps;
};

#endif /* __PERF_TOOL_H */
