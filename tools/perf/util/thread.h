#ifndef __PERF_THREAD_H
#define __PERF_THREAD_H

#include <linux/rbtree.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <unistd.h>
#include <sys/types.h>
#include "symbol.h"
=======
=======
>>>>>>> v3.18
#include <linux/list.h>
#include <unistd.h>
#include <sys/types.h>
#include "symbol.h"
#include <strlist.h>
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

struct thread {
	union {
		struct rb_node	 rb_node;
		struct list_head node;
	};
<<<<<<< HEAD
<<<<<<< HEAD
	struct map_groups	mg;
	pid_t			pid;
	char			shortname[3];
	bool			comm_set;
	char			*comm;
=======
=======
>>>>>>> v3.18
	struct map_groups	*mg;
	pid_t			pid_; /* Not all tools update this */
	pid_t			tid;
	pid_t			ppid;
	int			cpu;
	char			shortname[3];
	bool			comm_set;
	bool			dead; /* if set thread has exited */
	struct list_head	comm_list;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	int			comm_len;

	void			*priv;
};

struct machine;
<<<<<<< HEAD
<<<<<<< HEAD

struct thread *thread__new(pid_t pid);
void thread__delete(struct thread *self);

int thread__set_comm(struct thread *self, const char *comm);
int thread__comm_len(struct thread *self);
void thread__insert_map(struct thread *self, struct map *map);
int thread__fork(struct thread *self, struct thread *parent);
size_t thread__fprintf(struct thread *thread, FILE *fp);

static inline struct map *thread__find_map(struct thread *self,
					   enum map_type type, u64 addr)
{
	return self ? map_groups__find(&self->mg, type, addr) : NULL;
}

=======
=======
>>>>>>> v3.18
struct comm;

struct thread *thread__new(pid_t pid, pid_t tid);
int thread__init_map_groups(struct thread *thread, struct machine *machine);
void thread__delete(struct thread *thread);
static inline void thread__exited(struct thread *thread)
{
	thread->dead = true;
}

int __thread__set_comm(struct thread *thread, const char *comm, u64 timestamp,
		       bool exec);
static inline int thread__set_comm(struct thread *thread, const char *comm,
				   u64 timestamp)
{
	return __thread__set_comm(thread, comm, timestamp, false);
}

int thread__comm_len(struct thread *thread);
struct comm *thread__comm(const struct thread *thread);
struct comm *thread__exec_comm(const struct thread *thread);
const char *thread__comm_str(const struct thread *thread);
void thread__insert_map(struct thread *thread, struct map *map);
int thread__fork(struct thread *thread, struct thread *parent, u64 timestamp);
size_t thread__fprintf(struct thread *thread, FILE *fp);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
void thread__find_addr_map(struct thread *thread, struct machine *machine,
			   u8 cpumode, enum map_type type, u64 addr,
			   struct addr_location *al);

void thread__find_addr_location(struct thread *thread, struct machine *machine,
				u8 cpumode, enum map_type type, u64 addr,
<<<<<<< HEAD
<<<<<<< HEAD
				struct addr_location *al,
				symbol_filter_t filter);
=======
=======
>>>>>>> v3.18
				struct addr_location *al);

void thread__find_cpumode_addr_location(struct thread *thread,
					struct machine *machine,
					enum map_type type, u64 addr,
					struct addr_location *al);

static inline void *thread__priv(struct thread *thread)
{
	return thread->priv;
}

static inline void thread__set_priv(struct thread *thread, void *p)
{
	thread->priv = p;
}

static inline bool thread__is_filtered(struct thread *thread)
{
	if (symbol_conf.comm_list &&
	    !strlist__has_entry(symbol_conf.comm_list, thread__comm_str(thread))) {
		return true;
	}

	return false;
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif	/* __PERF_THREAD_H */
