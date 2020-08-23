/*
 * builtin-record.c
 *
 * Builtin record command: Record the profile of a workload
 * (or a CPU, or a PID) into the perf.data output file - for
 * later analysis via perf report.
 */
#include "builtin.h"

#include "perf.h"

#include "util/build-id.h"
#include "util/util.h"
#include "util/parse-options.h"
#include "util/parse-events.h"

<<<<<<< HEAD
=======
#include "util/callchain.h"
#include "util/cgroup.h"
>>>>>>> v3.18
#include "util/header.h"
#include "util/event.h"
#include "util/evlist.h"
#include "util/evsel.h"
#include "util/debug.h"
#include "util/session.h"
#include "util/tool.h"
#include "util/symbol.h"
#include "util/cpumap.h"
#include "util/thread_map.h"
<<<<<<< HEAD
=======
#include "util/data.h"
>>>>>>> v3.18

#include <unistd.h>
#include <sched.h>
#include <sys/mman.h>

<<<<<<< HEAD
#ifndef HAVE_ON_EXIT
#ifndef ATEXIT_MAX
#define ATEXIT_MAX 32
#endif
static int __on_exit_count = 0;
typedef void (*on_exit_func_t) (int, void *);
static on_exit_func_t __on_exit_funcs[ATEXIT_MAX];
static void *__on_exit_args[ATEXIT_MAX];
static int __exitcode = 0;
static void __handle_on_exit_funcs(void);
static int on_exit(on_exit_func_t function, void *arg);
#define exit(x) (exit)(__exitcode = (x))

static int on_exit(on_exit_func_t function, void *arg)
{
	if (__on_exit_count == ATEXIT_MAX)
		return -ENOMEM;
	else if (__on_exit_count == 0)
		atexit(__handle_on_exit_funcs);
	__on_exit_funcs[__on_exit_count] = function;
	__on_exit_args[__on_exit_count++] = arg;
	return 0;
}

static void __handle_on_exit_funcs(void)
{
	int i;
	for (i = 0; i < __on_exit_count; i++)
		__on_exit_funcs[i] (__exitcode, __on_exit_args[i]);
}
#endif

enum write_mode_t {
	WRITE_FORCE,
	WRITE_APPEND
};

struct perf_record {
	struct perf_tool	tool;
	struct perf_record_opts	opts;
	u64			bytes_written;
	const char		*output_name;
	struct perf_evlist	*evlist;
	struct perf_session	*session;
	const char		*progname;
	int			output;
	unsigned int		page_size;
	int			realtime_prio;
	enum write_mode_t	write_mode;
	bool			no_buildid;
	bool			no_buildid_cache;
	bool			force;
	bool			file_new;
	bool			append_file;
	long			samples;
	off_t			post_processing_offset;
};

static void advance_output(struct perf_record *rec, size_t size)
{
	rec->bytes_written += size;
}

static int write_output(struct perf_record *rec, void *buf, size_t size)
{
	while (size) {
		int ret = write(rec->output, buf, size);

		if (ret < 0) {
			pr_err("failed to write\n");
			return -1;
		}

		size -= ret;
		buf += ret;

		rec->bytes_written += ret;
	}

=======

struct record {
	struct perf_tool	tool;
	struct record_opts	opts;
	u64			bytes_written;
	struct perf_data_file	file;
	struct perf_evlist	*evlist;
	struct perf_session	*session;
	const char		*progname;
	int			realtime_prio;
	bool			no_buildid;
	bool			no_buildid_cache;
	long			samples;
};

static int record__write(struct record *rec, void *bf, size_t size)
{
	if (perf_data_file__write(rec->session->file, bf, size) < 0) {
		pr_err("failed to write perf data, error: %m\n");
		return -1;
	}

	rec->bytes_written += size;
>>>>>>> v3.18
	return 0;
}

static int process_synthesized_event(struct perf_tool *tool,
				     union perf_event *event,
				     struct perf_sample *sample __maybe_unused,
				     struct machine *machine __maybe_unused)
{
<<<<<<< HEAD
	struct perf_record *rec = container_of(tool, struct perf_record, tool);
	if (write_output(rec, event, event->header.size) < 0)
		return -1;

	return 0;
}

static int perf_record__mmap_read(struct perf_record *rec,
				   struct perf_mmap *md)
{
	unsigned int head = perf_mmap__read_head(md);
	unsigned int old = md->prev;
	unsigned char *data = md->base + rec->page_size;
=======
	struct record *rec = container_of(tool, struct record, tool);
	return record__write(rec, event, event->header.size);
}

static int record__mmap_read(struct record *rec, int idx)
{
	struct perf_mmap *md = &rec->evlist->mmap[idx];
	unsigned int head = perf_mmap__read_head(md);
	unsigned int old = md->prev;
	unsigned char *data = md->base + page_size;
>>>>>>> v3.18
	unsigned long size;
	void *buf;
	int rc = 0;

	if (old == head)
		return 0;

	rec->samples++;

	size = head - old;

	if ((old & md->mask) + size != (head & md->mask)) {
		buf = &data[old & md->mask];
		size = md->mask + 1 - (old & md->mask);
		old += size;

<<<<<<< HEAD
		if (write_output(rec, buf, size) < 0) {
=======
		if (record__write(rec, buf, size) < 0) {
>>>>>>> v3.18
			rc = -1;
			goto out;
		}
	}

	buf = &data[old & md->mask];
	size = head - old;
	old += size;

<<<<<<< HEAD
	if (write_output(rec, buf, size) < 0) {
=======
	if (record__write(rec, buf, size) < 0) {
>>>>>>> v3.18
		rc = -1;
		goto out;
	}

	md->prev = old;
<<<<<<< HEAD
	perf_mmap__write_tail(md, old);

=======
	perf_evlist__mmap_consume(rec->evlist, idx);
>>>>>>> v3.18
out:
	return rc;
}

static volatile int done = 0;
static volatile int signr = -1;
static volatile int child_finished = 0;

static void sig_handler(int sig)
{
	if (sig == SIGCHLD)
		child_finished = 1;
<<<<<<< HEAD

	done = 1;
	signr = sig;
}

static void perf_record__sig_exit(int exit_status __maybe_unused, void *arg)
{
	struct perf_record *rec = arg;
	int status;

	if (rec->evlist->workload.pid > 0) {
		if (!child_finished)
			kill(rec->evlist->workload.pid, SIGTERM);

		wait(&status);
		if (WIFSIGNALED(status))
			psignal(WTERMSIG(status), rec->progname);
	}

	if (signr == -1 || signr == SIGUSR1)
		return;

	signal(signr, SIG_DFL);
	kill(getpid(), signr);
}

static bool perf_evlist__equal(struct perf_evlist *evlist,
			       struct perf_evlist *other)
{
	struct perf_evsel *pos, *pair;

	if (evlist->nr_entries != other->nr_entries)
		return false;

	pair = perf_evlist__first(other);

	list_for_each_entry(pos, &evlist->entries, node) {
		if (memcmp(&pos->attr, &pair->attr, sizeof(pos->attr) != 0))
			return false;
		pair = perf_evsel__next(pair);
	}

	return true;
}

static int perf_record__open(struct perf_record *rec)
=======
	else
		signr = sig;

	done = 1;
}

static void record__sig_exit(void)
{
	if (signr == -1)
		return;

	signal(signr, SIG_DFL);
	raise(signr);
}

static int record__open(struct record *rec)
>>>>>>> v3.18
{
	char msg[512];
	struct perf_evsel *pos;
	struct perf_evlist *evlist = rec->evlist;
	struct perf_session *session = rec->session;
<<<<<<< HEAD
	struct perf_record_opts *opts = &rec->opts;
=======
	struct record_opts *opts = &rec->opts;
>>>>>>> v3.18
	int rc = 0;

	perf_evlist__config(evlist, opts);

<<<<<<< HEAD
	list_for_each_entry(pos, &evlist->entries, node) {
=======
	evlist__for_each(evlist, pos) {
>>>>>>> v3.18
try_again:
		if (perf_evsel__open(pos, evlist->cpus, evlist->threads) < 0) {
			if (perf_evsel__fallback(pos, errno, msg, sizeof(msg))) {
				if (verbose)
					ui__warning("%s\n", msg);
				goto try_again;
			}

			rc = -errno;
			perf_evsel__open_strerror(pos, &opts->target,
						  errno, msg, sizeof(msg));
			ui__error("%s\n", msg);
			goto out;
		}
	}

	if (perf_evlist__apply_filters(evlist)) {
		error("failed to set filter with %d (%s)\n", errno,
<<<<<<< HEAD
			strerror(errno));
=======
			strerror_r(errno, msg, sizeof(msg)));
>>>>>>> v3.18
		rc = -1;
		goto out;
	}

	if (perf_evlist__mmap(evlist, opts->mmap_pages, false) < 0) {
		if (errno == EPERM) {
			pr_err("Permission error mapping pages.\n"
			       "Consider increasing "
			       "/proc/sys/kernel/perf_event_mlock_kb,\n"
			       "or try again with a smaller value of -m/--mmap_pages.\n"
<<<<<<< HEAD
			       "(current value: %d)\n", opts->mmap_pages);
			rc = -errno;
		} else if (!is_power_of_2(opts->mmap_pages) &&
			   (opts->mmap_pages != UINT_MAX)) {
			pr_err("--mmap_pages/-m value must be a power of two.");
			rc = -EINVAL;
		} else {
			pr_err("failed to mmap with %d (%s)\n", errno, strerror(errno));
=======
			       "(current value: %u)\n", opts->mmap_pages);
			rc = -errno;
		} else {
			pr_err("failed to mmap with %d (%s)\n", errno,
				strerror_r(errno, msg, sizeof(msg)));
>>>>>>> v3.18
			rc = -errno;
		}
		goto out;
	}

<<<<<<< HEAD
	if (rec->file_new)
		session->evlist = evlist;
	else {
		if (!perf_evlist__equal(session->evlist, evlist)) {
			fprintf(stderr, "incompatible append\n");
			rc = -1;
			goto out;
		}
 	}

=======
	session->evlist = evlist;
>>>>>>> v3.18
	perf_session__set_id_hdr_size(session);
out:
	return rc;
}

<<<<<<< HEAD
static int process_buildids(struct perf_record *rec)
{
	u64 size = lseek(rec->output, 0, SEEK_CUR);

	if (size == 0)
		return 0;

	rec->session->fd = rec->output;
	return __perf_session__process_events(rec->session, rec->post_processing_offset,
					      size - rec->post_processing_offset,
					      size, &build_id__mark_dso_hit_ops);
}

static void perf_record__exit(int status, void *arg)
{
	struct perf_record *rec = arg;

	if (status != 0)
		return;

	if (!rec->opts.pipe_output) {
		rec->session->header.data_size += rec->bytes_written;

		if (!rec->no_buildid)
			process_buildids(rec);
		perf_session__write_header(rec->session, rec->evlist,
					   rec->output, true);
		perf_session__delete(rec->session);
		perf_evlist__delete(rec->evlist);
		symbol__exit();
	}
}

=======
static int process_buildids(struct record *rec)
{
	struct perf_data_file *file  = &rec->file;
	struct perf_session *session = rec->session;
	u64 start = session->header.data_offset;

	u64 size = lseek(file->fd, 0, SEEK_CUR);
	if (size == 0)
		return 0;

	return __perf_session__process_events(session, start,
					      size - start,
					      size, &build_id__mark_dso_hit_ops);
}

>>>>>>> v3.18
static void perf_event__synthesize_guest_os(struct machine *machine, void *data)
{
	int err;
	struct perf_tool *tool = data;
	/*
	 *As for guest kernel when processing subcommand record&report,
	 *we arrange module mmap prior to guest kernel mmap and trigger
	 *a preload dso because default guest module symbols are loaded
	 *from guest kallsyms instead of /lib/modules/XXX/XXX. This
	 *method is used to avoid symbol missing when the first addr is
	 *in module instead of in guest kernel.
	 */
	err = perf_event__synthesize_modules(tool, process_synthesized_event,
					     machine);
	if (err < 0)
		pr_err("Couldn't record guest kernel [%d]'s reference"
		       " relocation symbol.\n", machine->pid);

	/*
	 * We use _stext for guest kernel because guest kernel's /proc/kallsyms
	 * have no _text sometimes.
	 */
	err = perf_event__synthesize_kernel_mmap(tool, process_synthesized_event,
<<<<<<< HEAD
						 machine, "_text");
	if (err < 0)
		err = perf_event__synthesize_kernel_mmap(tool, process_synthesized_event,
							 machine, "_stext");
=======
						 machine);
>>>>>>> v3.18
	if (err < 0)
		pr_err("Couldn't record guest kernel [%d]'s reference"
		       " relocation symbol.\n", machine->pid);
}

static struct perf_event_header finished_round_event = {
	.size = sizeof(struct perf_event_header),
	.type = PERF_RECORD_FINISHED_ROUND,
};

<<<<<<< HEAD
static int perf_record__mmap_read_all(struct perf_record *rec)
{
=======
static int record__mmap_read_all(struct record *rec)
{
	u64 bytes_written = rec->bytes_written;
>>>>>>> v3.18
	int i;
	int rc = 0;

	for (i = 0; i < rec->evlist->nr_mmaps; i++) {
		if (rec->evlist->mmap[i].base) {
<<<<<<< HEAD
			if (perf_record__mmap_read(rec, &rec->evlist->mmap[i]) != 0) {
=======
			if (record__mmap_read(rec, i) != 0) {
>>>>>>> v3.18
				rc = -1;
				goto out;
			}
		}
	}

<<<<<<< HEAD
	if (perf_header__has_feat(&rec->session->header, HEADER_TRACING_DATA))
		rc = write_output(rec, &finished_round_event,
				  sizeof(finished_round_event));
=======
	/*
	 * Mark the round finished in case we wrote
	 * at least one event.
	 */
	if (bytes_written != rec->bytes_written)
		rc = record__write(rec, &finished_round_event, sizeof(finished_round_event));
>>>>>>> v3.18

out:
	return rc;
}

<<<<<<< HEAD
static int __cmd_record(struct perf_record *rec, int argc, const char **argv)
{
	struct stat st;
	int flags;
	int err, output, feat;
=======
static void record__init_features(struct record *rec)
{
	struct perf_session *session = rec->session;
	int feat;

	for (feat = HEADER_FIRST_FEATURE; feat < HEADER_LAST_FEATURE; feat++)
		perf_header__set_feat(&session->header, feat);

	if (rec->no_buildid)
		perf_header__clear_feat(&session->header, HEADER_BUILD_ID);

	if (!have_tracepoints(&rec->evlist->entries))
		perf_header__clear_feat(&session->header, HEADER_TRACING_DATA);

	if (!rec->opts.branch_stack)
		perf_header__clear_feat(&session->header, HEADER_BRANCH_STACK);
}

static volatile int workload_exec_errno;

/*
 * perf_evlist__prepare_workload will send a SIGUSR1
 * if the fork fails, since we asked by setting its
 * want_signal to true.
 */
static void workload_exec_failed_signal(int signo __maybe_unused,
					siginfo_t *info,
					void *ucontext __maybe_unused)
{
	workload_exec_errno = info->si_value.sival_int;
	done = 1;
	child_finished = 1;
}

static int __cmd_record(struct record *rec, int argc, const char **argv)
{
	int err;
	int status = 0;
>>>>>>> v3.18
	unsigned long waking = 0;
	const bool forks = argc > 0;
	struct machine *machine;
	struct perf_tool *tool = &rec->tool;
<<<<<<< HEAD
	struct perf_record_opts *opts = &rec->opts;
	struct perf_evlist *evsel_list = rec->evlist;
	const char *output_name = rec->output_name;
	struct perf_session *session;
	bool disabled = false;

	rec->progname = argv[0];

	rec->page_size = sysconf(_SC_PAGE_SIZE);

	on_exit(perf_record__sig_exit, rec);
	signal(SIGCHLD, sig_handler);
	signal(SIGINT, sig_handler);
	signal(SIGUSR1, sig_handler);

	if (!output_name) {
		if (!fstat(STDOUT_FILENO, &st) && S_ISFIFO(st.st_mode))
			opts->pipe_output = true;
		else
			rec->output_name = output_name = "perf.data";
	}
	if (output_name) {
		if (!strcmp(output_name, "-"))
			opts->pipe_output = true;
		else if (!stat(output_name, &st) && st.st_size) {
			if (rec->write_mode == WRITE_FORCE) {
				char oldname[PATH_MAX];
				snprintf(oldname, sizeof(oldname), "%s.old",
					 output_name);
				unlink(oldname);
				rename(output_name, oldname);
			}
		} else if (rec->write_mode == WRITE_APPEND) {
			rec->write_mode = WRITE_FORCE;
		}
	}

	flags = O_CREAT|O_RDWR;
	if (rec->write_mode == WRITE_APPEND)
		rec->file_new = 0;
	else
		flags |= O_TRUNC;

	if (opts->pipe_output)
		output = STDOUT_FILENO;
	else
		output = open(output_name, flags, S_IRUSR | S_IWUSR);
	if (output < 0) {
		perror("failed to create output file");
		return -1;
	}

	rec->output = output;

	session = perf_session__new(output_name, O_WRONLY,
				    rec->write_mode == WRITE_FORCE, false, NULL);
	if (session == NULL) {
		pr_err("Not enough memory for reading perf file header\n");
=======
	struct record_opts *opts = &rec->opts;
	struct perf_data_file *file = &rec->file;
	struct perf_session *session;
	bool disabled = false, draining = false;

	rec->progname = argv[0];

	atexit(record__sig_exit);
	signal(SIGCHLD, sig_handler);
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);

	session = perf_session__new(file, false, NULL);
	if (session == NULL) {
		pr_err("Perf session creation failed.\n");
>>>>>>> v3.18
		return -1;
	}

	rec->session = session;

<<<<<<< HEAD
	for (feat = HEADER_FIRST_FEATURE; feat < HEADER_LAST_FEATURE; feat++)
		perf_header__set_feat(&session->header, feat);

	if (rec->no_buildid)
		perf_header__clear_feat(&session->header, HEADER_BUILD_ID);

	if (!have_tracepoints(&evsel_list->entries))
		perf_header__clear_feat(&session->header, HEADER_TRACING_DATA);

	if (!rec->opts.branch_stack)
		perf_header__clear_feat(&session->header, HEADER_BRANCH_STACK);

	if (!rec->file_new) {
		err = perf_session__read_header(session, output);
		if (err < 0)
			goto out_delete_session;
	}

	if (forks) {
		err = perf_evlist__prepare_workload(evsel_list, &opts->target,
						    argv, opts->pipe_output,
						    true);
		if (err < 0) {
			pr_err("Couldn't run the workload!\n");
=======
	record__init_features(rec);

	if (forks) {
		err = perf_evlist__prepare_workload(rec->evlist, &opts->target,
						    argv, file->is_pipe,
						    workload_exec_failed_signal);
		if (err < 0) {
			pr_err("Couldn't run the workload!\n");
			status = err;
>>>>>>> v3.18
			goto out_delete_session;
		}
	}

<<<<<<< HEAD
	if (perf_record__open(rec) != 0) {
		err = -1;
		goto out_delete_session;
	}

	if (!evsel_list->nr_groups)
		perf_header__clear_feat(&session->header, HEADER_GROUP_DESC);

	/*
	 * perf_session__delete(session) will be called at perf_record__exit()
	 */
	on_exit(perf_record__exit, rec);

	if (opts->pipe_output) {
		err = perf_header__write_pipe(output);
		if (err < 0)
			goto out_delete_session;
	} else if (rec->file_new) {
		err = perf_session__write_header(session, evsel_list,
						 output, false);
		if (err < 0)
			goto out_delete_session;
=======
	if (record__open(rec) != 0) {
		err = -1;
		goto out_child;
	}

	if (!rec->evlist->nr_groups)
		perf_header__clear_feat(&session->header, HEADER_GROUP_DESC);

	if (file->is_pipe) {
		err = perf_header__write_pipe(file->fd);
		if (err < 0)
			goto out_child;
	} else {
		err = perf_session__write_header(session, rec->evlist,
						 file->fd, false);
		if (err < 0)
			goto out_child;
>>>>>>> v3.18
	}

	if (!rec->no_buildid
	    && !perf_header__has_feat(&session->header, HEADER_BUILD_ID)) {
		pr_err("Couldn't generate buildids. "
		       "Use --no-buildid to profile anyway.\n");
		err = -1;
<<<<<<< HEAD
		goto out_delete_session;
	}

	rec->post_processing_offset = lseek(output, 0, SEEK_CUR);

	machine = &session->machines.host;

	if (opts->pipe_output) {
=======
		goto out_child;
	}

	machine = &session->machines.host;

	if (file->is_pipe) {
>>>>>>> v3.18
		err = perf_event__synthesize_attrs(tool, session,
						   process_synthesized_event);
		if (err < 0) {
			pr_err("Couldn't synthesize attrs.\n");
<<<<<<< HEAD
			goto out_delete_session;
		}

		err = perf_event__synthesize_event_types(tool, process_synthesized_event,
							 machine);
		if (err < 0) {
			pr_err("Couldn't synthesize event_types.\n");
			goto out_delete_session;
		}

		if (have_tracepoints(&evsel_list->entries)) {
=======
			goto out_child;
		}

		if (have_tracepoints(&rec->evlist->entries)) {
>>>>>>> v3.18
			/*
			 * FIXME err <= 0 here actually means that
			 * there were no tracepoints so its not really
			 * an error, just that we don't need to
			 * synthesize anything.  We really have to
			 * return this more properly and also
			 * propagate errors that now are calling die()
			 */
<<<<<<< HEAD
			err = perf_event__synthesize_tracing_data(tool, output, evsel_list,
								  process_synthesized_event);
			if (err <= 0) {
				pr_err("Couldn't record tracing data.\n");
				goto out_delete_session;
			}
			advance_output(rec, err);
=======
			err = perf_event__synthesize_tracing_data(tool, file->fd, rec->evlist,
								  process_synthesized_event);
			if (err <= 0) {
				pr_err("Couldn't record tracing data.\n");
				goto out_child;
			}
			rec->bytes_written += err;
>>>>>>> v3.18
		}
	}

	err = perf_event__synthesize_kernel_mmap(tool, process_synthesized_event,
<<<<<<< HEAD
						 machine, "_text");
	if (err < 0)
		err = perf_event__synthesize_kernel_mmap(tool, process_synthesized_event,
							 machine, "_stext");
=======
						 machine);
>>>>>>> v3.18
	if (err < 0)
		pr_err("Couldn't record kernel reference relocation symbol\n"
		       "Symbol resolution may be skewed if relocation was used (e.g. kexec).\n"
		       "Check /proc/kallsyms permission or run as root.\n");

	err = perf_event__synthesize_modules(tool, process_synthesized_event,
					     machine);
	if (err < 0)
		pr_err("Couldn't record kernel module information.\n"
		       "Symbol resolution may be skewed if relocation was used (e.g. kexec).\n"
		       "Check /proc/modules permission or run as root.\n");

	if (perf_guest) {
		machines__process_guests(&session->machines,
					 perf_event__synthesize_guest_os, tool);
	}

<<<<<<< HEAD
	if (perf_target__has_task(&opts->target))
		err = perf_event__synthesize_thread_map(tool, evsel_list->threads,
						  process_synthesized_event,
						  machine);
	else if (perf_target__has_cpu(&opts->target))
		err = perf_event__synthesize_threads(tool, process_synthesized_event,
					       machine);
	else /* command specified */
		err = 0;

	if (err != 0)
		goto out_delete_session;
=======
	err = __machine__synthesize_threads(machine, tool, &opts->target, rec->evlist->threads,
					    process_synthesized_event, opts->sample_address);
	if (err != 0)
		goto out_child;
>>>>>>> v3.18

	if (rec->realtime_prio) {
		struct sched_param param;

		param.sched_priority = rec->realtime_prio;
		if (sched_setscheduler(0, SCHED_FIFO, &param)) {
			pr_err("Could not set realtime priority.\n");
			err = -1;
<<<<<<< HEAD
			goto out_delete_session;
=======
			goto out_child;
>>>>>>> v3.18
		}
	}

	/*
	 * When perf is starting the traced process, all the events
	 * (apart from group members) have enable_on_exec=1 set,
	 * so don't spoil it by prematurely enabling them.
	 */
<<<<<<< HEAD
	if (!perf_target__none(&opts->target))
		perf_evlist__enable(evsel_list);
=======
	if (!target__none(&opts->target) && !opts->initial_delay)
		perf_evlist__enable(rec->evlist);
>>>>>>> v3.18

	/*
	 * Let the child rip
	 */
	if (forks)
<<<<<<< HEAD
		perf_evlist__start_workload(evsel_list);
=======
		perf_evlist__start_workload(rec->evlist);

	if (opts->initial_delay) {
		usleep(opts->initial_delay * 1000);
		perf_evlist__enable(rec->evlist);
	}
>>>>>>> v3.18

	for (;;) {
		int hits = rec->samples;

<<<<<<< HEAD
		if (perf_record__mmap_read_all(rec) < 0) {
			err = -1;
			goto out_delete_session;
		}

		if (hits == rec->samples) {
			if (done)
				break;
			err = poll(evsel_list->pollfd, evsel_list->nr_fds, -1);
			waking++;
=======
		if (record__mmap_read_all(rec) < 0) {
			err = -1;
			goto out_child;
		}

		if (hits == rec->samples) {
			if (done || draining)
				break;
			err = perf_evlist__poll(rec->evlist, -1);
			/*
			 * Propagate error, only if there's any. Ignore positive
			 * number of returned events and interrupt error.
			 */
			if (err > 0 || (err < 0 && errno == EINTR))
				err = 0;
			waking++;

			if (perf_evlist__filter_pollfd(rec->evlist, POLLERR | POLLHUP) == 0)
				draining = true;
>>>>>>> v3.18
		}

		/*
		 * When perf is starting the traced process, at the end events
		 * die with the process and we wait for that. Thus no need to
		 * disable events in this case.
		 */
<<<<<<< HEAD
		if (done && !disabled && !perf_target__none(&opts->target)) {
			perf_evlist__disable(evsel_list);
=======
		if (done && !disabled && !target__none(&opts->target)) {
			perf_evlist__disable(rec->evlist);
>>>>>>> v3.18
			disabled = true;
		}
	}

<<<<<<< HEAD
	if (quiet || signr == SIGUSR1)
		return 0;

	fprintf(stderr, "[ perf record: Woken up %ld times to write data ]\n", waking);

	/*
	 * Approximate RIP event size: 24 bytes.
	 */
	fprintf(stderr,
		"[ perf record: Captured and wrote %.3f MB %s (~%" PRIu64 " samples) ]\n",
		(double)rec->bytes_written / 1024.0 / 1024.0,
		output_name,
		rec->bytes_written / 24);

	return 0;

out_delete_session:
	perf_session__delete(session);
	return err;
=======
	if (forks && workload_exec_errno) {
		char msg[STRERR_BUFSIZE];
		const char *emsg = strerror_r(workload_exec_errno, msg, sizeof(msg));
		pr_err("Workload failed: %s\n", emsg);
		err = -1;
		goto out_child;
	}

	if (!quiet) {
		fprintf(stderr, "[ perf record: Woken up %ld times to write data ]\n", waking);

		/*
		 * Approximate RIP event size: 24 bytes.
		 */
		fprintf(stderr,
			"[ perf record: Captured and wrote %.3f MB %s (~%" PRIu64 " samples) ]\n",
			(double)rec->bytes_written / 1024.0 / 1024.0,
			file->path,
			rec->bytes_written / 24);
	}

out_child:
	if (forks) {
		int exit_status;

		if (!child_finished)
			kill(rec->evlist->workload.pid, SIGTERM);

		wait(&exit_status);

		if (err < 0)
			status = err;
		else if (WIFEXITED(exit_status))
			status = WEXITSTATUS(exit_status);
		else if (WIFSIGNALED(exit_status))
			signr = WTERMSIG(exit_status);
	} else
		status = err;

	if (!err && !file->is_pipe) {
		rec->session->header.data_size += rec->bytes_written;

		if (!rec->no_buildid)
			process_buildids(rec);
		perf_session__write_header(rec->session, rec->evlist,
					   file->fd, true);
	}

out_delete_session:
	perf_session__delete(session);
	return status;
>>>>>>> v3.18
}

#define BRANCH_OPT(n, m) \
	{ .name = n, .mode = (m) }

#define BRANCH_END { .name = NULL }

struct branch_mode {
	const char *name;
	int mode;
};

static const struct branch_mode branch_modes[] = {
	BRANCH_OPT("u", PERF_SAMPLE_BRANCH_USER),
	BRANCH_OPT("k", PERF_SAMPLE_BRANCH_KERNEL),
	BRANCH_OPT("hv", PERF_SAMPLE_BRANCH_HV),
	BRANCH_OPT("any", PERF_SAMPLE_BRANCH_ANY),
	BRANCH_OPT("any_call", PERF_SAMPLE_BRANCH_ANY_CALL),
	BRANCH_OPT("any_ret", PERF_SAMPLE_BRANCH_ANY_RETURN),
	BRANCH_OPT("ind_call", PERF_SAMPLE_BRANCH_IND_CALL),
<<<<<<< HEAD
=======
	BRANCH_OPT("abort_tx", PERF_SAMPLE_BRANCH_ABORT_TX),
	BRANCH_OPT("in_tx", PERF_SAMPLE_BRANCH_IN_TX),
	BRANCH_OPT("no_tx", PERF_SAMPLE_BRANCH_NO_TX),
	BRANCH_OPT("cond", PERF_SAMPLE_BRANCH_COND),
>>>>>>> v3.18
	BRANCH_END
};

static int
parse_branch_stack(const struct option *opt, const char *str, int unset)
{
#define ONLY_PLM \
	(PERF_SAMPLE_BRANCH_USER	|\
	 PERF_SAMPLE_BRANCH_KERNEL	|\
	 PERF_SAMPLE_BRANCH_HV)

	uint64_t *mode = (uint64_t *)opt->value;
	const struct branch_mode *br;
	char *s, *os = NULL, *p;
	int ret = -1;

	if (unset)
		return 0;

	/*
	 * cannot set it twice, -b + --branch-filter for instance
	 */
	if (*mode)
		return -1;

	/* str may be NULL in case no arg is passed to -b */
	if (str) {
		/* because str is read-only */
		s = os = strdup(str);
		if (!s)
			return -1;

		for (;;) {
			p = strchr(s, ',');
			if (p)
				*p = '\0';

			for (br = branch_modes; br->name; br++) {
				if (!strcasecmp(s, br->name))
					break;
			}
			if (!br->name) {
				ui__warning("unknown branch filter %s,"
					    " check man page\n", s);
				goto error;
			}

			*mode |= br->mode;

			if (!p)
				break;

			s = p + 1;
		}
	}
	ret = 0;

	/* default to any branch */
	if ((*mode & ~ONLY_PLM) == 0) {
		*mode = PERF_SAMPLE_BRANCH_ANY;
	}
error:
	free(os);
	return ret;
}

<<<<<<< HEAD
#ifdef LIBUNWIND_SUPPORT
static int get_stack_size(char *str, unsigned long *_size)
{
	char *endptr;
	unsigned long size;
	unsigned long max_size = round_down(USHRT_MAX, sizeof(u64));

	size = strtoul(str, &endptr, 0);

	do {
		if (*endptr)
			break;

		size = round_up(size, sizeof(u64));
		if (!size || size > max_size)
			break;

		*_size = size;
		return 0;

	} while (0);

	pr_err("callchain: Incorrect stack dump size (max %ld): %s\n",
	       max_size, str);
	return -1;
}
#endif /* LIBUNWIND_SUPPORT */

int record_parse_callchain_opt(const struct option *opt,
			       const char *arg, int unset)
{
	struct perf_record_opts *opts = opt->value;
	char *tok, *name, *saveptr = NULL;
	char *buf;
	int ret = -1;

	/* --no-call-graph */
	if (unset)
		return 0;

	/* We specified default option if none is provided. */
	BUG_ON(!arg);

	/* We need buffer that we know we can write to. */
	buf = malloc(strlen(arg) + 1);
	if (!buf)
		return -ENOMEM;

	strcpy(buf, arg);

	tok = strtok_r((char *)buf, ",", &saveptr);
	name = tok ? : (char *)buf;

	do {
		/* Framepointer style */
		if (!strncmp(name, "fp", sizeof("fp"))) {
			if (!strtok_r(NULL, ",", &saveptr)) {
				opts->call_graph = CALLCHAIN_FP;
				ret = 0;
			} else
				pr_err("callchain: No more arguments "
				       "needed for -g fp\n");
			break;

#ifdef LIBUNWIND_SUPPORT
		/* Dwarf style */
		} else if (!strncmp(name, "dwarf", sizeof("dwarf"))) {
			const unsigned long default_stack_dump_size = 8192;

			ret = 0;
			opts->call_graph = CALLCHAIN_DWARF;
			opts->stack_dump_size = default_stack_dump_size;

			tok = strtok_r(NULL, ",", &saveptr);
			if (tok) {
				unsigned long size = 0;

				ret = get_stack_size(tok, &size);
				opts->stack_dump_size = size;
			}

			if (!ret)
				pr_debug("callchain: stack dump size %d\n",
					 opts->stack_dump_size);
#endif /* LIBUNWIND_SUPPORT */
		} else {
			pr_err("callchain: Unknown -g option "
			       "value: %s\n", arg);
			break;
		}

	} while (0);

	free(buf);

	if (!ret)
		pr_debug("callchain: type %d\n", opts->call_graph);

	return ret;
=======
static void callchain_debug(void)
{
	static const char *str[CALLCHAIN_MAX] = { "NONE", "FP", "DWARF" };

	pr_debug("callchain: type %s\n", str[callchain_param.record_mode]);

	if (callchain_param.record_mode == CALLCHAIN_DWARF)
		pr_debug("callchain: stack dump size %d\n",
			 callchain_param.dump_size);
}

int record_parse_callchain_opt(const struct option *opt __maybe_unused,
			       const char *arg,
			       int unset)
{
	int ret;

	callchain_param.enabled = !unset;

	/* --no-call-graph */
	if (unset) {
		callchain_param.record_mode = CALLCHAIN_NONE;
		pr_debug("callchain: disabled\n");
		return 0;
	}

	ret = parse_callchain_record_opt(arg);
	if (!ret)
		callchain_debug();

	return ret;
}

int record_callchain_opt(const struct option *opt __maybe_unused,
			 const char *arg __maybe_unused,
			 int unset __maybe_unused)
{
	callchain_param.enabled = true;

	if (callchain_param.record_mode == CALLCHAIN_NONE)
		callchain_param.record_mode = CALLCHAIN_FP;

	callchain_debug();
	return 0;
}

static int perf_record_config(const char *var, const char *value, void *cb)
{
	if (!strcmp(var, "record.call-graph"))
		var = "call-graph.record-mode"; /* fall-through */

	return perf_default_config(var, value, cb);
>>>>>>> v3.18
}

static const char * const record_usage[] = {
	"perf record [<options>] [<command>]",
	"perf record [<options>] -- <command> [<options>]",
	NULL
};

/*
<<<<<<< HEAD
 * XXX Ideally would be local to cmd_record() and passed to a perf_record__new
 * because we need to have access to it in perf_record__exit, that is called
=======
 * XXX Ideally would be local to cmd_record() and passed to a record__new
 * because we need to have access to it in record__exit, that is called
>>>>>>> v3.18
 * after cmd_record() exits, but since record_options need to be accessible to
 * builtin-script, leave it here.
 *
 * At least we don't ouch it in all the other functions here directly.
 *
 * Just say no to tons of global variables, sigh.
 */
<<<<<<< HEAD
static struct perf_record record = {
	.opts = {
=======
static struct record record = {
	.opts = {
		.sample_time	     = true,
>>>>>>> v3.18
		.mmap_pages	     = UINT_MAX,
		.user_freq	     = UINT_MAX,
		.user_interval	     = ULLONG_MAX,
		.freq		     = 4000,
		.target		     = {
			.uses_mmap   = true,
<<<<<<< HEAD
		},
	},
	.write_mode = WRITE_FORCE,
	.file_new   = true,
};

#define CALLCHAIN_HELP "do call-graph (stack chain/backtrace) recording: "

#ifdef LIBUNWIND_SUPPORT
const char record_callchain_help[] = CALLCHAIN_HELP "[fp] dwarf";
#else
const char record_callchain_help[] = CALLCHAIN_HELP "[fp]";
=======
			.default_per_cpu = true,
		},
	},
};

#define CALLCHAIN_HELP "setup and enables call-graph (stack chain/backtrace) recording: "

#ifdef HAVE_DWARF_UNWIND_SUPPORT
const char record_callchain_help[] = CALLCHAIN_HELP "fp dwarf";
#else
const char record_callchain_help[] = CALLCHAIN_HELP "fp";
>>>>>>> v3.18
#endif

/*
 * XXX Will stay a global variable till we fix builtin-script.c to stop messing
 * with it and switch to use the library functions in perf_evlist that came
<<<<<<< HEAD
 * from builtin-record.c, i.e. use perf_record_opts,
=======
 * from builtin-record.c, i.e. use record_opts,
>>>>>>> v3.18
 * perf_evlist__prepare_workload, etc instead of fork+exec'in 'perf record',
 * using pipes, etc.
 */
const struct option record_options[] = {
	OPT_CALLBACK('e', "event", &record.evlist, "event",
		     "event selector. use 'perf list' to list available events",
		     parse_events_option),
	OPT_CALLBACK(0, "filter", &record.evlist, "filter",
		     "event filter", parse_filter),
	OPT_STRING('p', "pid", &record.opts.target.pid, "pid",
		    "record events on existing process id"),
	OPT_STRING('t', "tid", &record.opts.target.tid, "tid",
		    "record events on existing thread id"),
	OPT_INTEGER('r', "realtime", &record.realtime_prio,
		    "collect data with this RT SCHED_FIFO priority"),
<<<<<<< HEAD
	OPT_BOOLEAN('D', "no-delay", &record.opts.no_delay,
=======
	OPT_BOOLEAN(0, "no-buffering", &record.opts.no_buffering,
>>>>>>> v3.18
		    "collect data without buffering"),
	OPT_BOOLEAN('R', "raw-samples", &record.opts.raw_samples,
		    "collect raw sample records from all opened counters"),
	OPT_BOOLEAN('a', "all-cpus", &record.opts.target.system_wide,
			    "system-wide collection from all CPUs"),
<<<<<<< HEAD
	OPT_BOOLEAN('A', "append", &record.append_file,
			    "append to the output file to do incremental profiling"),
	OPT_STRING('C', "cpu", &record.opts.target.cpu_list, "cpu",
		    "list of cpus to monitor"),
	OPT_BOOLEAN('f', "force", &record.force,
			"overwrite existing data file (deprecated)"),
	OPT_U64('c', "count", &record.opts.user_interval, "event period to sample"),
	OPT_STRING('o', "output", &record.output_name, "file",
		    "output file name"),
	OPT_BOOLEAN('i', "no-inherit", &record.opts.no_inherit,
		    "child tasks do not inherit counters"),
	OPT_UINTEGER('F', "freq", &record.opts.user_freq, "profile at this frequency"),
	OPT_UINTEGER('m', "mmap-pages", &record.opts.mmap_pages,
		     "number of mmap data pages"),
	OPT_BOOLEAN(0, "group", &record.opts.group,
		    "put the counters into a counter group"),
	OPT_CALLBACK_DEFAULT('g', "call-graph", &record.opts,
			     "mode[,dump_size]", record_callchain_help,
			     &record_parse_callchain_opt, "fp"),
=======
	OPT_STRING('C', "cpu", &record.opts.target.cpu_list, "cpu",
		    "list of cpus to monitor"),
	OPT_U64('c', "count", &record.opts.user_interval, "event period to sample"),
	OPT_STRING('o', "output", &record.file.path, "file",
		    "output file name"),
	OPT_BOOLEAN_SET('i', "no-inherit", &record.opts.no_inherit,
			&record.opts.no_inherit_set,
			"child tasks do not inherit counters"),
	OPT_UINTEGER('F', "freq", &record.opts.user_freq, "profile at this frequency"),
	OPT_CALLBACK('m', "mmap-pages", &record.opts.mmap_pages, "pages",
		     "number of mmap data pages",
		     perf_evlist__parse_mmap_pages),
	OPT_BOOLEAN(0, "group", &record.opts.group,
		    "put the counters into a counter group"),
	OPT_CALLBACK_NOOPT('g', NULL, &record.opts,
			   NULL, "enables call-graph recording" ,
			   &record_callchain_opt),
	OPT_CALLBACK(0, "call-graph", &record.opts,
		     "mode[,dump_size]", record_callchain_help,
		     &record_parse_callchain_opt),
>>>>>>> v3.18
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose (show counter open errors, etc)"),
	OPT_BOOLEAN('q', "quiet", &quiet, "don't print any message"),
	OPT_BOOLEAN('s', "stat", &record.opts.inherit_stat,
		    "per thread counts"),
	OPT_BOOLEAN('d', "data", &record.opts.sample_address,
		    "Sample addresses"),
	OPT_BOOLEAN('T', "timestamp", &record.opts.sample_time, "Sample timestamps"),
	OPT_BOOLEAN('P', "period", &record.opts.period, "Sample period"),
	OPT_BOOLEAN('n', "no-samples", &record.opts.no_samples,
		    "don't sample"),
	OPT_BOOLEAN('N', "no-buildid-cache", &record.no_buildid_cache,
		    "do not update the buildid cache"),
	OPT_BOOLEAN('B', "no-buildid", &record.no_buildid,
		    "do not collect buildids in perf.data"),
	OPT_CALLBACK('G', "cgroup", &record.evlist, "name",
		     "monitor event in cgroup name only",
		     parse_cgroups),
<<<<<<< HEAD
=======
	OPT_UINTEGER('D', "delay", &record.opts.initial_delay,
		  "ms to wait before starting measurement after program start"),
>>>>>>> v3.18
	OPT_STRING('u', "uid", &record.opts.target.uid_str, "user",
		   "user to profile"),

	OPT_CALLBACK_NOOPT('b', "branch-any", &record.opts.branch_stack,
		     "branch any", "sample any taken branches",
		     parse_branch_stack),

	OPT_CALLBACK('j', "branch-filter", &record.opts.branch_stack,
		     "branch filter mask", "branch stack filter modes",
		     parse_branch_stack),
	OPT_BOOLEAN('W', "weight", &record.opts.sample_weight,
		    "sample by weight (on special events only)"),
<<<<<<< HEAD
=======
	OPT_BOOLEAN(0, "transaction", &record.opts.sample_transaction,
		    "sample transaction flags (special events only)"),
	OPT_BOOLEAN(0, "per-thread", &record.opts.target.per_thread,
		    "use per-thread mmaps"),
>>>>>>> v3.18
	OPT_END()
};

int cmd_record(int argc, const char **argv, const char *prefix __maybe_unused)
{
	int err = -ENOMEM;
<<<<<<< HEAD
	struct perf_evsel *pos;
	struct perf_evlist *evsel_list;
	struct perf_record *rec = &record;
	char errbuf[BUFSIZ];

	evsel_list = perf_evlist__new();
	if (evsel_list == NULL)
		return -ENOMEM;

	rec->evlist = evsel_list;

	argc = parse_options(argc, argv, record_options, record_usage,
			    PARSE_OPT_STOP_AT_NON_OPTION);
	if (!argc && perf_target__none(&rec->opts.target))
		usage_with_options(record_usage, record_options);

	if (rec->force && rec->append_file) {
		ui__error("Can't overwrite and append at the same time."
			  " You need to choose between -f and -A");
		usage_with_options(record_usage, record_options);
	} else if (rec->append_file) {
		rec->write_mode = WRITE_APPEND;
	} else {
		rec->write_mode = WRITE_FORCE;
	}
=======
	struct record *rec = &record;
	char errbuf[BUFSIZ];

	rec->evlist = perf_evlist__new();
	if (rec->evlist == NULL)
		return -ENOMEM;

	perf_config(perf_record_config, rec);

	argc = parse_options(argc, argv, record_options, record_usage,
			    PARSE_OPT_STOP_AT_NON_OPTION);
	if (!argc && target__none(&rec->opts.target))
		usage_with_options(record_usage, record_options);
>>>>>>> v3.18

	if (nr_cgroups && !rec->opts.target.system_wide) {
		ui__error("cgroup monitoring only available in"
			  " system-wide mode\n");
		usage_with_options(record_usage, record_options);
	}

<<<<<<< HEAD
	symbol__init();
=======
	symbol__init(NULL);
>>>>>>> v3.18

	if (symbol_conf.kptr_restrict)
		pr_warning(
"WARNING: Kernel address maps (/proc/{kallsyms,modules}) are restricted,\n"
"check /proc/sys/kernel/kptr_restrict.\n\n"
"Samples in kernel functions may not be resolved if a suitable vmlinux\n"
"file is not found in the buildid cache or in the vmlinux path.\n\n"
"Samples in kernel modules won't be resolved at all.\n\n"
"If some relocation was applied (e.g. kexec) symbols may be misresolved\n"
"even with a suitable vmlinux or kallsyms file.\n\n");

	if (rec->no_buildid_cache || rec->no_buildid)
		disable_buildid_cache();

<<<<<<< HEAD
	if (evsel_list->nr_entries == 0 &&
	    perf_evlist__add_default(evsel_list) < 0) {
=======
	if (rec->evlist->nr_entries == 0 &&
	    perf_evlist__add_default(rec->evlist) < 0) {
>>>>>>> v3.18
		pr_err("Not enough memory for event selector list\n");
		goto out_symbol_exit;
	}

<<<<<<< HEAD
	err = perf_target__validate(&rec->opts.target);
	if (err) {
		perf_target__strerror(&rec->opts.target, err, errbuf, BUFSIZ);
		ui__warning("%s", errbuf);
	}

	err = perf_target__parse_uid(&rec->opts.target);
	if (err) {
		int saved_errno = errno;

		perf_target__strerror(&rec->opts.target, err, errbuf, BUFSIZ);
=======
	if (rec->opts.target.tid && !rec->opts.no_inherit_set)
		rec->opts.no_inherit = true;

	err = target__validate(&rec->opts.target);
	if (err) {
		target__strerror(&rec->opts.target, err, errbuf, BUFSIZ);
		ui__warning("%s", errbuf);
	}

	err = target__parse_uid(&rec->opts.target);
	if (err) {
		int saved_errno = errno;

		target__strerror(&rec->opts.target, err, errbuf, BUFSIZ);
>>>>>>> v3.18
		ui__error("%s", errbuf);

		err = -saved_errno;
		goto out_symbol_exit;
	}

	err = -ENOMEM;
<<<<<<< HEAD
	if (perf_evlist__create_maps(evsel_list, &rec->opts.target) < 0)
		usage_with_options(record_usage, record_options);

	list_for_each_entry(pos, &evsel_list->entries, node) {
		if (perf_header__push_event(pos->attr.config, perf_evsel__name(pos)))
			goto out_free_fd;
	}

	if (rec->opts.user_interval != ULLONG_MAX)
		rec->opts.default_interval = rec->opts.user_interval;
	if (rec->opts.user_freq != UINT_MAX)
		rec->opts.freq = rec->opts.user_freq;

	/*
	 * User specified count overrides default frequency.
	 */
	if (rec->opts.default_interval)
		rec->opts.freq = 0;
	else if (rec->opts.freq) {
		rec->opts.default_interval = rec->opts.freq;
	} else {
		ui__error("frequency and count are zero, aborting\n");
		err = -EINVAL;
		goto out_free_fd;
	}

	err = __cmd_record(&record, argc, argv);

	perf_evlist__munmap(evsel_list);
	perf_evlist__close(evsel_list);
out_free_fd:
	perf_evlist__delete_maps(evsel_list);
out_symbol_exit:
=======
	if (perf_evlist__create_maps(rec->evlist, &rec->opts.target) < 0)
		usage_with_options(record_usage, record_options);

	if (record_opts__config(&rec->opts)) {
		err = -EINVAL;
		goto out_symbol_exit;
	}

	err = __cmd_record(&record, argc, argv);
out_symbol_exit:
	perf_evlist__delete(rec->evlist);
>>>>>>> v3.18
	symbol__exit();
	return err;
}
