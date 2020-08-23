/*
 *  Generic process-grouping system.
 *
 *  Based originally on the cpuset system, extracted by Paul Menage
 *  Copyright (C) 2006 Google, Inc
 *
 *  Notifications support
 *  Copyright (C) 2009 Nokia Corporation
 *  Author: Kirill A. Shutemov
 *
 *  Copyright notices from the original cpuset code:
 *  --------------------------------------------------
 *  Copyright (C) 2003 BULL SA.
 *  Copyright (C) 2004-2006 Silicon Graphics, Inc.
 *
 *  Portions derived from Patrick Mochel's sysfs code.
 *  sysfs is Copyright (c) 2001-3 Patrick Mochel
 *
 *  2003-10-10 Written by Simon Derr.
 *  2003-10-22 Updates by Stephen Hemminger.
 *  2004 May-July Rework by Paul Jackson.
 *  ---------------------------------------------------
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of the Linux
 *  distribution for more details.
 */

<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> v3.18
#include <linux/cgroup.h>
#include <linux/cred.h>
#include <linux/ctype.h>
#include <linux/errno.h>
#include <linux/init_task.h>
#include <linux/kernel.h>
#include <linux/list.h>
<<<<<<< HEAD
=======
#include <linux/magic.h>
>>>>>>> v3.18
#include <linux/mm.h>
#include <linux/mutex.h>
#include <linux/mount.h>
#include <linux/pagemap.h>
#include <linux/proc_fs.h>
#include <linux/rcupdate.h>
#include <linux/sched.h>
<<<<<<< HEAD
#include <linux/backing-dev.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/magic.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/sort.h>
#include <linux/kmod.h>
#include <linux/module.h>
#include <linux/delayacct.h>
#include <linux/cgroupstats.h>
#include <linux/hashtable.h>
#include <linux/namei.h>
#include <linux/pid_namespace.h>
#include <linux/idr.h>
#include <linux/vmalloc.h> /* TODO: replace with more sophisticated array */
#include <linux/eventfd.h>
#include <linux/poll.h>
#include <linux/flex_array.h> /* used in cgroup_attach_task */
#include <linux/kthread.h>

#include <linux/atomic.h>

/* css deactivation bias, makes css->refcnt negative to deny new trygets */
#define CSS_DEACT_BIAS		INT_MIN
=======
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/rwsem.h>
#include <linux/string.h>
#include <linux/sort.h>
#include <linux/kmod.h>
#include <linux/delayacct.h>
#include <linux/cgroupstats.h>
#include <linux/hashtable.h>
#include <linux/pid_namespace.h>
#include <linux/idr.h>
#include <linux/vmalloc.h> /* TODO: replace with more sophisticated array */
#include <linux/kthread.h>
#include <linux/delay.h>

#include <linux/atomic.h>

/*
 * pidlists linger the following amount before being destroyed.  The goal
 * is avoiding frequent destruction in the middle of consecutive read calls
 * Expiring in the middle is a performance problem not a correctness one.
 * 1 sec should be enough.
 */
#define CGROUP_PIDLIST_DESTROY_DELAY	HZ

#define CGROUP_FILE_NAME_MAX		(MAX_CGROUP_TYPE_NAMELEN +	\
					 MAX_CFTYPE_NAME + 2)
>>>>>>> v3.18

/*
 * cgroup_mutex is the master lock.  Any modification to cgroup or its
 * hierarchy must be performed while holding it.
 *
<<<<<<< HEAD
 * cgroup_root_mutex nests inside cgroup_mutex and should be held to modify
 * cgroupfs_root of any cgroup hierarchy - subsys list, flags,
 * release_agent_path and so on.  Modifying requires both cgroup_mutex and
 * cgroup_root_mutex.  Readers can acquire either of the two.  This is to
 * break the following locking order cycle.
 *
 *  A. cgroup_mutex -> cred_guard_mutex -> s_type->i_mutex_key -> namespace_sem
 *  B. namespace_sem -> cgroup_mutex
 *
 * B happens only through cgroup_show_options() and using cgroup_root_mutex
 * breaks it.
 */
#ifdef CONFIG_PROVE_RCU
DEFINE_MUTEX(cgroup_mutex);
EXPORT_SYMBOL_GPL(cgroup_mutex);	/* only for task_subsys_state_check() */
#else
static DEFINE_MUTEX(cgroup_mutex);
#endif

static DEFINE_MUTEX(cgroup_root_mutex);
=======
 * css_set_rwsem protects task->cgroups pointer, the list of css_set
 * objects, and the chain of tasks off each css_set.
 *
 * These locks are exported if CONFIG_PROVE_RCU so that accessors in
 * cgroup.h can use them for lockdep annotations.
 */
#ifdef CONFIG_PROVE_RCU
DEFINE_MUTEX(cgroup_mutex);
DECLARE_RWSEM(css_set_rwsem);
EXPORT_SYMBOL_GPL(cgroup_mutex);
EXPORT_SYMBOL_GPL(css_set_rwsem);
#else
static DEFINE_MUTEX(cgroup_mutex);
static DECLARE_RWSEM(css_set_rwsem);
#endif

/*
 * Protects cgroup_idr and css_idr so that IDs can be released without
 * grabbing cgroup_mutex.
 */
static DEFINE_SPINLOCK(cgroup_idr_lock);

/*
 * Protects cgroup_subsys->release_agent_path.  Modifying it also requires
 * cgroup_mutex.  Reading requires either cgroup_mutex or this spinlock.
 */
static DEFINE_SPINLOCK(release_agent_path_lock);

#define cgroup_assert_mutex_or_rcu_locked()				\
	rcu_lockdep_assert(rcu_read_lock_held() ||			\
			   lockdep_is_held(&cgroup_mutex),		\
			   "cgroup_mutex or RCU read lock required");
>>>>>>> v3.18

/*
 * cgroup destruction makes heavy use of work items and there can be a lot
 * of concurrent destructions.  Use a separate workqueue so that cgroup
 * destruction work items don't end up filling up max_active of system_wq
 * which may lead to deadlock.
 */
static struct workqueue_struct *cgroup_destroy_wq;

/*
<<<<<<< HEAD
 * Generate an array of cgroup subsystem pointers. At boot time, this is
 * populated with the built in subsystems, and modular subsystems are
 * registered after that. The mutable section of this array is protected by
 * cgroup_mutex.
 */
#define SUBSYS(_x) [_x ## _subsys_id] = &_x ## _subsys,
#define IS_SUBSYS_ENABLED(option) IS_BUILTIN(option)
static struct cgroup_subsys *subsys[CGROUP_SUBSYS_COUNT] = {
#include <linux/cgroup_subsys.h>
};

/*
 * The "rootnode" hierarchy is the "dummy hierarchy", reserved for the
 * subsystems that are otherwise unattached - it never has more than a
 * single cgroup, and all tasks are part of that cgroup.
 */
static struct cgroupfs_root rootnode;

/*
 * cgroupfs file entry, pointed to from leaf dentry->d_fsdata.
 */
struct cfent {
	struct list_head		node;
	struct dentry			*dentry;
	struct cftype			*type;

	/* file xattrs */
	struct simple_xattrs		xattrs;
};

/*
 * CSS ID -- ID per subsys's Cgroup Subsys State(CSS). used only when
 * cgroup_subsys->use_id != 0.
 */
#define CSS_ID_MAX	(65535)
struct css_id {
	/*
	 * The css to which this ID points. This pointer is set to valid value
	 * after cgroup is populated. If cgroup is removed, this will be NULL.
	 * This pointer is expected to be RCU-safe because destroy()
	 * is called after synchronize_rcu(). But for safe use, css_tryget()
	 * should be used for avoiding race.
	 */
	struct cgroup_subsys_state __rcu *css;
	/*
	 * ID of this css.
	 */
	unsigned short id;
	/*
	 * Depth in hierarchy which this ID belongs to.
	 */
	unsigned short depth;
	/*
	 * ID is freed by RCU. (and lookup routine is RCU safe.)
	 */
	struct rcu_head rcu_head;
	/*
	 * Hierarchy of CSS ID belongs to.
	 */
	unsigned short stack[0]; /* Array of Length (depth+1) */
};

/*
 * cgroup_event represents events which userspace want to receive.
 */
struct cgroup_event {
	/*
	 * Cgroup which the event belongs to.
	 */
	struct cgroup *cgrp;
	/*
	 * Control file which the event associated.
	 */
	struct cftype *cft;
	/*
	 * eventfd to signal userspace about the event.
	 */
	struct eventfd_ctx *eventfd;
	/*
	 * Each of these stored in a list by the cgroup.
	 */
	struct list_head list;
	/*
	 * All fields below needed to unregister event when
	 * userspace closes eventfd.
	 */
	poll_table pt;
	wait_queue_head_t *wqh;
	wait_queue_t wait;
	struct work_struct remove;
};

/* The list of hierarchy roots */

static LIST_HEAD(roots);
static int root_count;

static DEFINE_IDA(hierarchy_ida);
static int next_hierarchy_id;
static DEFINE_SPINLOCK(hierarchy_id_lock);

/* dummytop is a shorthand for the dummy hierarchy's top cgroup */
#define dummytop (&rootnode.top_cgroup)

static struct cgroup_name root_cgroup_name = { .name = "/" };
=======
 * pidlist destructions need to be flushed on cgroup destruction.  Use a
 * separate workqueue as flush domain.
 */
static struct workqueue_struct *cgroup_pidlist_destroy_wq;

/* generate an array of cgroup subsystem pointers */
#define SUBSYS(_x) [_x ## _cgrp_id] = &_x ## _cgrp_subsys,
static struct cgroup_subsys *cgroup_subsys[] = {
#include <linux/cgroup_subsys.h>
};
#undef SUBSYS

/* array of cgroup subsystem names */
#define SUBSYS(_x) [_x ## _cgrp_id] = #_x,
static const char *cgroup_subsys_name[] = {
#include <linux/cgroup_subsys.h>
};
#undef SUBSYS

/*
 * The default hierarchy, reserved for the subsystems that are otherwise
 * unattached - it never has more than a single cgroup, and all tasks are
 * part of that cgroup.
 */
struct cgroup_root cgrp_dfl_root;

/*
 * The default hierarchy always exists but is hidden until mounted for the
 * first time.  This is for backward compatibility.
 */
static bool cgrp_dfl_root_visible;

/*
 * Set by the boot param of the same name and makes subsystems with NULL
 * ->dfl_files to use ->legacy_files on the default hierarchy.
 */
static bool cgroup_legacy_files_on_dfl;

/* some controllers are not supported in the default hierarchy */
static unsigned int cgrp_dfl_root_inhibit_ss_mask;

/* The list of hierarchy roots */

static LIST_HEAD(cgroup_roots);
static int cgroup_root_count;

/* hierarchy ID allocation and mapping, protected by cgroup_mutex */
static DEFINE_IDR(cgroup_hierarchy_idr);

/*
 * Assign a monotonically increasing serial number to csses.  It guarantees
 * cgroups with bigger numbers are newer than those with smaller numbers.
 * Also, as csses are always appended to the parent's ->children list, it
 * guarantees that sibling csses are always sorted in the ascending serial
 * number order on the list.  Protected by cgroup_mutex.
 */
static u64 css_serial_nr_next = 1;
>>>>>>> v3.18

/* This flag indicates whether tasks in the fork and exit paths should
 * check for fork/exit handlers to call. This avoids us having to do
 * extra work in the fork/exit path if none of the subsystems need to
 * be called.
 */
static int need_forkexit_callback __read_mostly;

<<<<<<< HEAD
static int cgroup_destroy_locked(struct cgroup *cgrp);
static int cgroup_addrm_files(struct cgroup *cgrp, struct cgroup_subsys *subsys,
			      struct cftype cfts[], bool is_add);

static int css_unbias_refcnt(int refcnt)
{
	return refcnt >= 0 ? refcnt : refcnt - CSS_DEACT_BIAS;
}

/* the current nr of refs, always >= 0 whether @css is deactivated or not */
static int css_refcnt(struct cgroup_subsys_state *css)
{
	int v = atomic_read(&css->refcnt);

	return css_unbias_refcnt(v);
}

/* convenient tests for these bits */
inline int cgroup_is_removed(const struct cgroup *cgrp)
{
	return test_bit(CGRP_REMOVED, &cgrp->flags);
}
=======
static struct cftype cgroup_dfl_base_files[];
static struct cftype cgroup_legacy_base_files[];

static int rebind_subsystems(struct cgroup_root *dst_root,
			     unsigned int ss_mask);
static int cgroup_destroy_locked(struct cgroup *cgrp);
static int create_css(struct cgroup *cgrp, struct cgroup_subsys *ss,
		      bool visible);
static void css_release(struct percpu_ref *ref);
static void kill_css(struct cgroup_subsys_state *css);
static int cgroup_addrm_files(struct cgroup *cgrp, struct cftype cfts[],
			      bool is_add);

/* IDR wrappers which synchronize using cgroup_idr_lock */
static int cgroup_idr_alloc(struct idr *idr, void *ptr, int start, int end,
			    gfp_t gfp_mask)
{
	int ret;

	idr_preload(gfp_mask);
	spin_lock_bh(&cgroup_idr_lock);
	ret = idr_alloc(idr, ptr, start, end, gfp_mask);
	spin_unlock_bh(&cgroup_idr_lock);
	idr_preload_end();
	return ret;
}

static void *cgroup_idr_replace(struct idr *idr, void *ptr, int id)
{
	void *ret;

	spin_lock_bh(&cgroup_idr_lock);
	ret = idr_replace(idr, ptr, id);
	spin_unlock_bh(&cgroup_idr_lock);
	return ret;
}

static void cgroup_idr_remove(struct idr *idr, int id)
{
	spin_lock_bh(&cgroup_idr_lock);
	idr_remove(idr, id);
	spin_unlock_bh(&cgroup_idr_lock);
}

static struct cgroup *cgroup_parent(struct cgroup *cgrp)
{
	struct cgroup_subsys_state *parent_css = cgrp->self.parent;

	if (parent_css)
		return container_of(parent_css, struct cgroup, self);
	return NULL;
}

/**
 * cgroup_css - obtain a cgroup's css for the specified subsystem
 * @cgrp: the cgroup of interest
 * @ss: the subsystem of interest (%NULL returns @cgrp->self)
 *
 * Return @cgrp's css (cgroup_subsys_state) associated with @ss.  This
 * function must be called either under cgroup_mutex or rcu_read_lock() and
 * the caller is responsible for pinning the returned css if it wants to
 * keep accessing it outside the said locks.  This function may return
 * %NULL if @cgrp doesn't have @subsys_id enabled.
 */
static struct cgroup_subsys_state *cgroup_css(struct cgroup *cgrp,
					      struct cgroup_subsys *ss)
{
	if (ss)
		return rcu_dereference_check(cgrp->subsys[ss->id],
					lockdep_is_held(&cgroup_mutex));
	else
		return &cgrp->self;
}

/**
 * cgroup_e_css - obtain a cgroup's effective css for the specified subsystem
 * @cgrp: the cgroup of interest
 * @ss: the subsystem of interest (%NULL returns @cgrp->self)
 *
 * Similar to cgroup_css() but returns the effctive css, which is defined
 * as the matching css of the nearest ancestor including self which has @ss
 * enabled.  If @ss is associated with the hierarchy @cgrp is on, this
 * function is guaranteed to return non-NULL css.
 */
static struct cgroup_subsys_state *cgroup_e_css(struct cgroup *cgrp,
						struct cgroup_subsys *ss)
{
	lockdep_assert_held(&cgroup_mutex);

	if (!ss)
		return &cgrp->self;

	if (!(cgrp->root->subsys_mask & (1 << ss->id)))
		return NULL;

	while (cgroup_parent(cgrp) &&
	       !(cgroup_parent(cgrp)->child_subsys_mask & (1 << ss->id)))
		cgrp = cgroup_parent(cgrp);

	return cgroup_css(cgrp, ss);
}

/* convenient tests for these bits */
static inline bool cgroup_is_dead(const struct cgroup *cgrp)
{
	return !(cgrp->self.flags & CSS_ONLINE);
}

struct cgroup_subsys_state *of_css(struct kernfs_open_file *of)
{
	struct cgroup *cgrp = of->kn->parent->priv;
	struct cftype *cft = of_cft(of);

	/*
	 * This is open and unprotected implementation of cgroup_css().
	 * seq_css() is only called from a kernfs file operation which has
	 * an active reference on the file.  Because all the subsystem
	 * files are drained before a css is disassociated with a cgroup,
	 * the matching css from the cgroup's subsys table is guaranteed to
	 * be and stay valid until the enclosing operation is complete.
	 */
	if (cft->ss)
		return rcu_dereference_raw(cgrp->subsys[cft->ss->id]);
	else
		return &cgrp->self;
}
EXPORT_SYMBOL_GPL(of_css);
>>>>>>> v3.18

/**
 * cgroup_is_descendant - test ancestry
 * @cgrp: the cgroup to be tested
 * @ancestor: possible ancestor of @cgrp
 *
 * Test whether @cgrp is a descendant of @ancestor.  It also returns %true
 * if @cgrp == @ancestor.  This function is safe to call as long as @cgrp
 * and @ancestor are accessible.
 */
bool cgroup_is_descendant(struct cgroup *cgrp, struct cgroup *ancestor)
{
	while (cgrp) {
		if (cgrp == ancestor)
			return true;
<<<<<<< HEAD
		cgrp = cgrp->parent;
	}
	return false;
}
EXPORT_SYMBOL_GPL(cgroup_is_descendant);

static int cgroup_is_releasable(const struct cgroup *cgrp)
{
	const int bits =
		(1 << CGRP_RELEASABLE) |
		(1 << CGRP_NOTIFY_ON_RELEASE);
	return (cgrp->flags & bits) == bits;
}
=======
		cgrp = cgroup_parent(cgrp);
	}
	return false;
}
>>>>>>> v3.18

static int notify_on_release(const struct cgroup *cgrp)
{
	return test_bit(CGRP_NOTIFY_ON_RELEASE, &cgrp->flags);
}

<<<<<<< HEAD
/*
 * for_each_subsys() allows you to iterate on each subsystem attached to
 * an active hierarchy
 */
#define for_each_subsys(_root, _ss) \
list_for_each_entry(_ss, &_root->subsys_list, sibling)

/* for_each_active_root() allows you to iterate across the active hierarchies */
#define for_each_active_root(_root) \
list_for_each_entry(_root, &roots, root_list)

static inline struct cgroup *__d_cgrp(struct dentry *dentry)
{
	return dentry->d_fsdata;
}

static inline struct cfent *__d_cfe(struct dentry *dentry)
{
	return dentry->d_fsdata;
}

static inline struct cftype *__d_cft(struct dentry *dentry)
{
	return __d_cfe(dentry)->type;
}

/**
 * cgroup_lock_live_group - take cgroup_mutex and check that cgrp is alive.
 * @cgrp: the cgroup to be checked for liveness
 *
 * On success, returns true; the mutex should be later unlocked.  On
 * failure returns false with no lock held.
 */
static bool cgroup_lock_live_group(struct cgroup *cgrp)
{
	mutex_lock(&cgroup_mutex);
	if (cgroup_is_removed(cgrp)) {
		mutex_unlock(&cgroup_mutex);
		return false;
	}
	return true;
}

/* the list of cgroups eligible for automatic release. Protected by
 * release_list_lock */
static LIST_HEAD(release_list);
static DEFINE_RAW_SPINLOCK(release_list_lock);
static void cgroup_release_agent(struct work_struct *work);
static DECLARE_WORK(release_agent_work, cgroup_release_agent);
static void check_for_release(struct cgroup *cgrp);

/* Link structure for associating css_set objects with cgroups */
struct cg_cgroup_link {
	/*
	 * List running through cg_cgroup_links associated with a
	 * cgroup, anchored on cgroup->css_sets
	 */
	struct list_head cgrp_link_list;
	struct cgroup *cgrp;
	/*
	 * List running through cg_cgroup_links pointing at a
	 * single css_set object, anchored on css_set->cg_links
	 */
	struct list_head cg_link_list;
	struct css_set *cg;
};

/* The default css_set - used by init and its children prior to any
=======
/**
 * for_each_css - iterate all css's of a cgroup
 * @css: the iteration cursor
 * @ssid: the index of the subsystem, CGROUP_SUBSYS_COUNT after reaching the end
 * @cgrp: the target cgroup to iterate css's of
 *
 * Should be called under cgroup_[tree_]mutex.
 */
#define for_each_css(css, ssid, cgrp)					\
	for ((ssid) = 0; (ssid) < CGROUP_SUBSYS_COUNT; (ssid)++)	\
		if (!((css) = rcu_dereference_check(			\
				(cgrp)->subsys[(ssid)],			\
				lockdep_is_held(&cgroup_mutex)))) { }	\
		else

/**
 * for_each_e_css - iterate all effective css's of a cgroup
 * @css: the iteration cursor
 * @ssid: the index of the subsystem, CGROUP_SUBSYS_COUNT after reaching the end
 * @cgrp: the target cgroup to iterate css's of
 *
 * Should be called under cgroup_[tree_]mutex.
 */
#define for_each_e_css(css, ssid, cgrp)					\
	for ((ssid) = 0; (ssid) < CGROUP_SUBSYS_COUNT; (ssid)++)	\
		if (!((css) = cgroup_e_css(cgrp, cgroup_subsys[(ssid)]))) \
			;						\
		else

/**
 * for_each_subsys - iterate all enabled cgroup subsystems
 * @ss: the iteration cursor
 * @ssid: the index of @ss, CGROUP_SUBSYS_COUNT after reaching the end
 */
#define for_each_subsys(ss, ssid)					\
	for ((ssid) = 0; (ssid) < CGROUP_SUBSYS_COUNT &&		\
	     (((ss) = cgroup_subsys[ssid]) || true); (ssid)++)

/* iterate across the hierarchies */
#define for_each_root(root)						\
	list_for_each_entry((root), &cgroup_roots, root_list)

/* iterate over child cgrps, lock should be held throughout iteration */
#define cgroup_for_each_live_child(child, cgrp)				\
	list_for_each_entry((child), &(cgrp)->self.children, self.sibling) \
		if (({ lockdep_assert_held(&cgroup_mutex);		\
		       cgroup_is_dead(child); }))			\
			;						\
		else

static void cgroup_release_agent(struct work_struct *work);
static void check_for_release(struct cgroup *cgrp);

/*
 * A cgroup can be associated with multiple css_sets as different tasks may
 * belong to different cgroups on different hierarchies.  In the other
 * direction, a css_set is naturally associated with multiple cgroups.
 * This M:N relationship is represented by the following link structure
 * which exists for each association and allows traversing the associations
 * from both sides.
 */
struct cgrp_cset_link {
	/* the cgroup and css_set this link associates */
	struct cgroup		*cgrp;
	struct css_set		*cset;

	/* list of cgrp_cset_links anchored at cgrp->cset_links */
	struct list_head	cset_link;

	/* list of cgrp_cset_links anchored at css_set->cgrp_links */
	struct list_head	cgrp_link;
};

/*
 * The default css_set - used by init and its children prior to any
>>>>>>> v3.18
 * hierarchies being mounted. It contains a pointer to the root state
 * for each subsystem. Also used to anchor the list of css_sets. Not
 * reference-counted, to improve performance when child cgroups
 * haven't been created.
 */
<<<<<<< HEAD

static struct css_set init_css_set;
static struct cg_cgroup_link init_css_set_link;

static int cgroup_init_idr(struct cgroup_subsys *ss,
			   struct cgroup_subsys_state *css);

/* css_set_lock protects the list of css_set objects, and the
 * chain of tasks off each css_set.  Nests outside task->alloc_lock
 * due to cgroup_iter_start() */
static DEFINE_RWLOCK(css_set_lock);
static int css_set_count;
=======
struct css_set init_css_set = {
	.refcount		= ATOMIC_INIT(1),
	.cgrp_links		= LIST_HEAD_INIT(init_css_set.cgrp_links),
	.tasks			= LIST_HEAD_INIT(init_css_set.tasks),
	.mg_tasks		= LIST_HEAD_INIT(init_css_set.mg_tasks),
	.mg_preload_node	= LIST_HEAD_INIT(init_css_set.mg_preload_node),
	.mg_node		= LIST_HEAD_INIT(init_css_set.mg_node),
};

static int css_set_count	= 1;	/* 1 for init_css_set */

/**
 * cgroup_update_populated - updated populated count of a cgroup
 * @cgrp: the target cgroup
 * @populated: inc or dec populated count
 *
 * @cgrp is either getting the first task (css_set) or losing the last.
 * Update @cgrp->populated_cnt accordingly.  The count is propagated
 * towards root so that a given cgroup's populated_cnt is zero iff the
 * cgroup and all its descendants are empty.
 *
 * @cgrp's interface file "cgroup.populated" is zero if
 * @cgrp->populated_cnt is zero and 1 otherwise.  When @cgrp->populated_cnt
 * changes from or to zero, userland is notified that the content of the
 * interface file has changed.  This can be used to detect when @cgrp and
 * its descendants become populated or empty.
 */
static void cgroup_update_populated(struct cgroup *cgrp, bool populated)
{
	lockdep_assert_held(&css_set_rwsem);

	do {
		bool trigger;

		if (populated)
			trigger = !cgrp->populated_cnt++;
		else
			trigger = !--cgrp->populated_cnt;

		if (!trigger)
			break;

		if (cgrp->populated_kn)
			kernfs_notify(cgrp->populated_kn);
		cgrp = cgroup_parent(cgrp);
	} while (cgrp);
}
>>>>>>> v3.18

/*
 * hash table for cgroup groups. This improves the performance to find
 * an existing css_set. This hash doesn't (currently) take into
 * account cgroups in empty hierarchies.
 */
#define CSS_SET_HASH_BITS	7
static DEFINE_HASHTABLE(css_set_table, CSS_SET_HASH_BITS);

static unsigned long css_set_hash(struct cgroup_subsys_state *css[])
{
<<<<<<< HEAD
	int i;
	unsigned long key = 0UL;

	for (i = 0; i < CGROUP_SUBSYS_COUNT; i++)
=======
	unsigned long key = 0UL;
	struct cgroup_subsys *ss;
	int i;

	for_each_subsys(ss, i)
>>>>>>> v3.18
		key += (unsigned long)css[i];
	key = (key >> 16) ^ key;

	return key;
}

<<<<<<< HEAD
/* We don't maintain the lists running through each css_set to its
 * task until after the first call to cgroup_iter_start(). This
 * reduces the fork()/exit() overhead for people who have cgroups
 * compiled into their kernel but not actually in use */
static int use_task_css_set_links __read_mostly;

static void __put_css_set(struct css_set *cg, int taskexit)
{
	struct cg_cgroup_link *link;
	struct cg_cgroup_link *saved_link;
	/*
	 * Ensure that the refcount doesn't hit zero while any readers
	 * can see it. Similar to atomic_dec_and_lock(), but for an
	 * rwlock
	 */
	if (atomic_add_unless(&cg->refcount, -1, 1))
		return;
	write_lock(&css_set_lock);
	if (!atomic_dec_and_test(&cg->refcount)) {
		write_unlock(&css_set_lock);
		return;
	}

	/* This css_set is dead. unlink it and release cgroup refcounts */
	hash_del(&cg->hlist);
	css_set_count--;

	list_for_each_entry_safe(link, saved_link, &cg->cg_links,
				 cg_link_list) {
		struct cgroup *cgrp = link->cgrp;
		list_del(&link->cg_link_list);
		list_del(&link->cgrp_link_list);

		/*
		 * We may not be holding cgroup_mutex, and if cgrp->count is
		 * dropped to 0 the cgroup can be destroyed at any time, hence
		 * rcu_read_lock is used to keep it alive.
		 */
		rcu_read_lock();
		if (atomic_dec_and_test(&cgrp->count) &&
		    notify_on_release(cgrp)) {
			if (taskexit)
				set_bit(CGRP_RELEASABLE, &cgrp->flags);
			check_for_release(cgrp);
		}
		rcu_read_unlock();
=======
static void put_css_set_locked(struct css_set *cset)
{
	struct cgrp_cset_link *link, *tmp_link;
	struct cgroup_subsys *ss;
	int ssid;

	lockdep_assert_held(&css_set_rwsem);

	if (!atomic_dec_and_test(&cset->refcount))
		return;

	/* This css_set is dead. unlink it and release cgroup refcounts */
	for_each_subsys(ss, ssid)
		list_del(&cset->e_cset_node[ssid]);
	hash_del(&cset->hlist);
	css_set_count--;

	list_for_each_entry_safe(link, tmp_link, &cset->cgrp_links, cgrp_link) {
		struct cgroup *cgrp = link->cgrp;

		list_del(&link->cset_link);
		list_del(&link->cgrp_link);

		/* @cgrp can't go away while we're holding css_set_rwsem */
		if (list_empty(&cgrp->cset_links)) {
			cgroup_update_populated(cgrp, false);
			check_for_release(cgrp);
		}
>>>>>>> v3.18

		kfree(link);
	}

<<<<<<< HEAD
	write_unlock(&css_set_lock);
	kfree_rcu(cg, rcu_head);
}

/*
 * refcounted get/put for css_set objects
 */
static inline void get_css_set(struct css_set *cg)
{
	atomic_inc(&cg->refcount);
}

static inline void put_css_set(struct css_set *cg)
{
	__put_css_set(cg, 0);
}

static inline void put_css_set_taskexit(struct css_set *cg)
{
	__put_css_set(cg, 1);
}

/*
 * compare_css_sets - helper function for find_existing_css_set().
 * @cg: candidate css_set being tested
 * @old_cg: existing css_set for a task
 * @new_cgrp: cgroup that's being entered by the task
 * @template: desired set of css pointers in css_set (pre-calculated)
 *
 * Returns true if "cg" matches "old_cg" except for the hierarchy
 * which "new_cgrp" belongs to, for which it should match "new_cgrp".
 */
static bool compare_css_sets(struct css_set *cg,
			     struct css_set *old_cg,
=======
	kfree_rcu(cset, rcu_head);
}

static void put_css_set(struct css_set *cset)
{
	/*
	 * Ensure that the refcount doesn't hit zero while any readers
	 * can see it. Similar to atomic_dec_and_lock(), but for an
	 * rwlock
	 */
	if (atomic_add_unless(&cset->refcount, -1, 1))
		return;

	down_write(&css_set_rwsem);
	put_css_set_locked(cset);
	up_write(&css_set_rwsem);
}

/*
 * refcounted get/put for css_set objects
 */
static inline void get_css_set(struct css_set *cset)
{
	atomic_inc(&cset->refcount);
}

/**
 * compare_css_sets - helper function for find_existing_css_set().
 * @cset: candidate css_set being tested
 * @old_cset: existing css_set for a task
 * @new_cgrp: cgroup that's being entered by the task
 * @template: desired set of css pointers in css_set (pre-calculated)
 *
 * Returns true if "cset" matches "old_cset" except for the hierarchy
 * which "new_cgrp" belongs to, for which it should match "new_cgrp".
 */
static bool compare_css_sets(struct css_set *cset,
			     struct css_set *old_cset,
>>>>>>> v3.18
			     struct cgroup *new_cgrp,
			     struct cgroup_subsys_state *template[])
{
	struct list_head *l1, *l2;

<<<<<<< HEAD
	if (memcmp(template, cg->subsys, sizeof(cg->subsys))) {
		/* Not all subsystems matched */
		return false;
	}

	/*
	 * Compare cgroup pointers in order to distinguish between
	 * different cgroups in heirarchies with no subsystems. We
	 * could get by with just this check alone (and skip the
	 * memcmp above) but on most setups the memcmp check will
	 * avoid the need for this more expensive check on almost all
	 * candidates.
	 */

	l1 = &cg->cg_links;
	l2 = &old_cg->cg_links;
	while (1) {
		struct cg_cgroup_link *cgl1, *cgl2;
		struct cgroup *cg1, *cg2;
=======
	/*
	 * On the default hierarchy, there can be csets which are
	 * associated with the same set of cgroups but different csses.
	 * Let's first ensure that csses match.
	 */
	if (memcmp(template, cset->subsys, sizeof(cset->subsys)))
		return false;

	/*
	 * Compare cgroup pointers in order to distinguish between
	 * different cgroups in hierarchies.  As different cgroups may
	 * share the same effective css, this comparison is always
	 * necessary.
	 */
	l1 = &cset->cgrp_links;
	l2 = &old_cset->cgrp_links;
	while (1) {
		struct cgrp_cset_link *link1, *link2;
		struct cgroup *cgrp1, *cgrp2;
>>>>>>> v3.18

		l1 = l1->next;
		l2 = l2->next;
		/* See if we reached the end - both lists are equal length. */
<<<<<<< HEAD
		if (l1 == &cg->cg_links) {
			BUG_ON(l2 != &old_cg->cg_links);
			break;
		} else {
			BUG_ON(l2 == &old_cg->cg_links);
		}
		/* Locate the cgroups associated with these links. */
		cgl1 = list_entry(l1, struct cg_cgroup_link, cg_link_list);
		cgl2 = list_entry(l2, struct cg_cgroup_link, cg_link_list);
		cg1 = cgl1->cgrp;
		cg2 = cgl2->cgrp;
		/* Hierarchies should be linked in the same order. */
		BUG_ON(cg1->root != cg2->root);
=======
		if (l1 == &cset->cgrp_links) {
			BUG_ON(l2 != &old_cset->cgrp_links);
			break;
		} else {
			BUG_ON(l2 == &old_cset->cgrp_links);
		}
		/* Locate the cgroups associated with these links. */
		link1 = list_entry(l1, struct cgrp_cset_link, cgrp_link);
		link2 = list_entry(l2, struct cgrp_cset_link, cgrp_link);
		cgrp1 = link1->cgrp;
		cgrp2 = link2->cgrp;
		/* Hierarchies should be linked in the same order. */
		BUG_ON(cgrp1->root != cgrp2->root);
>>>>>>> v3.18

		/*
		 * If this hierarchy is the hierarchy of the cgroup
		 * that's changing, then we need to check that this
		 * css_set points to the new cgroup; if it's any other
		 * hierarchy, then this css_set should point to the
		 * same cgroup as the old css_set.
		 */
<<<<<<< HEAD
		if (cg1->root == new_cgrp->root) {
			if (cg1 != new_cgrp)
				return false;
		} else {
			if (cg1 != cg2)
=======
		if (cgrp1->root == new_cgrp->root) {
			if (cgrp1 != new_cgrp)
				return false;
		} else {
			if (cgrp1 != cgrp2)
>>>>>>> v3.18
				return false;
		}
	}
	return true;
}

<<<<<<< HEAD
/*
 * find_existing_css_set() is a helper for
 * find_css_set(), and checks to see whether an existing
 * css_set is suitable.
 *
 * oldcg: the cgroup group that we're using before the cgroup
 * transition
 *
 * cgrp: the cgroup that we're moving into
 *
 * template: location in which to build the desired set of subsystem
 * state objects for the new cgroup group
 */
static struct css_set *find_existing_css_set(
	struct css_set *oldcg,
	struct cgroup *cgrp,
	struct cgroup_subsys_state *template[])
{
	int i;
	struct cgroupfs_root *root = cgrp->root;
	struct css_set *cg;
	unsigned long key;
=======
/**
 * find_existing_css_set - init css array and find the matching css_set
 * @old_cset: the css_set that we're using before the cgroup transition
 * @cgrp: the cgroup that we're moving into
 * @template: out param for the new set of csses, should be clear on entry
 */
static struct css_set *find_existing_css_set(struct css_set *old_cset,
					struct cgroup *cgrp,
					struct cgroup_subsys_state *template[])
{
	struct cgroup_root *root = cgrp->root;
	struct cgroup_subsys *ss;
	struct css_set *cset;
	unsigned long key;
	int i;
>>>>>>> v3.18

	/*
	 * Build the set of subsystem state objects that we want to see in the
	 * new css_set. while subsystems can change globally, the entries here
	 * won't change, so no need for locking.
	 */
<<<<<<< HEAD
	for (i = 0; i < CGROUP_SUBSYS_COUNT; i++) {
		if (root->subsys_mask & (1UL << i)) {
			/* Subsystem is in this hierarchy. So we want
			 * the subsystem state from the new
			 * cgroup */
			template[i] = cgrp->subsys[i];
		} else {
			/* Subsystem is not in this hierarchy, so we
			 * don't want to change the subsystem state */
			template[i] = oldcg->subsys[i];
=======
	for_each_subsys(ss, i) {
		if (root->subsys_mask & (1UL << i)) {
			/*
			 * @ss is in this hierarchy, so we want the
			 * effective css from @cgrp.
			 */
			template[i] = cgroup_e_css(cgrp, ss);
		} else {
			/*
			 * @ss is not in this hierarchy, so we don't want
			 * to change the css.
			 */
			template[i] = old_cset->subsys[i];
>>>>>>> v3.18
		}
	}

	key = css_set_hash(template);
<<<<<<< HEAD
	hash_for_each_possible(css_set_table, cg, hlist, key) {
		if (!compare_css_sets(cg, oldcg, cgrp, template))
			continue;

		/* This css_set matches what we need */
		return cg;
=======
	hash_for_each_possible(css_set_table, cset, hlist, key) {
		if (!compare_css_sets(cset, old_cset, cgrp, template))
			continue;

		/* This css_set matches what we need */
		return cset;
>>>>>>> v3.18
	}

	/* No existing cgroup group matched */
	return NULL;
}

<<<<<<< HEAD
static void free_cg_links(struct list_head *tmp)
{
	struct cg_cgroup_link *link;
	struct cg_cgroup_link *saved_link;

	list_for_each_entry_safe(link, saved_link, tmp, cgrp_link_list) {
		list_del(&link->cgrp_link_list);
=======
static void free_cgrp_cset_links(struct list_head *links_to_free)
{
	struct cgrp_cset_link *link, *tmp_link;

	list_for_each_entry_safe(link, tmp_link, links_to_free, cset_link) {
		list_del(&link->cset_link);
>>>>>>> v3.18
		kfree(link);
	}
}

<<<<<<< HEAD
/*
 * allocate_cg_links() allocates "count" cg_cgroup_link structures
 * and chains them on tmp through their cgrp_link_list fields. Returns 0 on
 * success or a negative error
 */
static int allocate_cg_links(int count, struct list_head *tmp)
{
	struct cg_cgroup_link *link;
	int i;
	INIT_LIST_HEAD(tmp);
	for (i = 0; i < count; i++) {
		link = kmalloc(sizeof(*link), GFP_KERNEL);
		if (!link) {
			free_cg_links(tmp);
			return -ENOMEM;
		}
		list_add(&link->cgrp_link_list, tmp);
=======
/**
 * allocate_cgrp_cset_links - allocate cgrp_cset_links
 * @count: the number of links to allocate
 * @tmp_links: list_head the allocated links are put on
 *
 * Allocate @count cgrp_cset_link structures and chain them on @tmp_links
 * through ->cset_link.  Returns 0 on success or -errno.
 */
static int allocate_cgrp_cset_links(int count, struct list_head *tmp_links)
{
	struct cgrp_cset_link *link;
	int i;

	INIT_LIST_HEAD(tmp_links);

	for (i = 0; i < count; i++) {
		link = kzalloc(sizeof(*link), GFP_KERNEL);
		if (!link) {
			free_cgrp_cset_links(tmp_links);
			return -ENOMEM;
		}
		list_add(&link->cset_link, tmp_links);
>>>>>>> v3.18
	}
	return 0;
}

/**
 * link_css_set - a helper function to link a css_set to a cgroup
<<<<<<< HEAD
 * @tmp_cg_links: cg_cgroup_link objects allocated by allocate_cg_links()
 * @cg: the css_set to be linked
 * @cgrp: the destination cgroup
 */
static void link_css_set(struct list_head *tmp_cg_links,
			 struct css_set *cg, struct cgroup *cgrp)
{
	struct cg_cgroup_link *link;

	BUG_ON(list_empty(tmp_cg_links));
	link = list_first_entry(tmp_cg_links, struct cg_cgroup_link,
				cgrp_link_list);
	link->cg = cg;
	link->cgrp = cgrp;
	atomic_inc(&cgrp->count);
	list_move(&link->cgrp_link_list, &cgrp->css_sets);
=======
 * @tmp_links: cgrp_cset_link objects allocated by allocate_cgrp_cset_links()
 * @cset: the css_set to be linked
 * @cgrp: the destination cgroup
 */
static void link_css_set(struct list_head *tmp_links, struct css_set *cset,
			 struct cgroup *cgrp)
{
	struct cgrp_cset_link *link;

	BUG_ON(list_empty(tmp_links));

	if (cgroup_on_dfl(cgrp))
		cset->dfl_cgrp = cgrp;

	link = list_first_entry(tmp_links, struct cgrp_cset_link, cset_link);
	link->cset = cset;
	link->cgrp = cgrp;

	if (list_empty(&cgrp->cset_links))
		cgroup_update_populated(cgrp, true);
	list_move(&link->cset_link, &cgrp->cset_links);

>>>>>>> v3.18
	/*
	 * Always add links to the tail of the list so that the list
	 * is sorted by order of hierarchy creation
	 */
<<<<<<< HEAD
	list_add_tail(&link->cg_link_list, &cg->cg_links);
}

/*
 * find_css_set() takes an existing cgroup group and a
 * cgroup object, and returns a css_set object that's
 * equivalent to the old group, but with the given cgroup
 * substituted into the appropriate hierarchy. Must be called with
 * cgroup_mutex held
 */
static struct css_set *find_css_set(
	struct css_set *oldcg, struct cgroup *cgrp)
{
	struct css_set *res;
	struct cgroup_subsys_state *template[CGROUP_SUBSYS_COUNT];

	struct list_head tmp_cg_links;

	struct cg_cgroup_link *link;
	unsigned long key;

	/* First see if we already have a cgroup group that matches
	 * the desired set */
	read_lock(&css_set_lock);
	res = find_existing_css_set(oldcg, cgrp, template);
	if (res)
		get_css_set(res);
	read_unlock(&css_set_lock);

	if (res)
		return res;

	res = kmalloc(sizeof(*res), GFP_KERNEL);
	if (!res)
		return NULL;

	/* Allocate all the cg_cgroup_link objects that we'll need */
	if (allocate_cg_links(root_count, &tmp_cg_links) < 0) {
		kfree(res);
		return NULL;
	}

	atomic_set(&res->refcount, 1);
	INIT_LIST_HEAD(&res->cg_links);
	INIT_LIST_HEAD(&res->tasks);
	INIT_HLIST_NODE(&res->hlist);

	/* Copy the set of subsystem state objects generated in
	 * find_existing_css_set() */
	memcpy(res->subsys, template, sizeof(res->subsys));

	write_lock(&css_set_lock);
	/* Add reference counts and links from the new css_set. */
	list_for_each_entry(link, &oldcg->cg_links, cg_link_list) {
		struct cgroup *c = link->cgrp;
		if (c->root == cgrp->root)
			c = cgrp;
		link_css_set(&tmp_cg_links, res, c);
	}

	BUG_ON(!list_empty(&tmp_cg_links));

	css_set_count++;

	/* Add this cgroup group to the hash table */
	key = css_set_hash(res->subsys);
	hash_add(css_set_table, &res->hlist, key);

	write_unlock(&css_set_lock);

	return res;
}

/*
 * Return the cgroup for "task" from the given hierarchy. Must be
 * called with cgroup_mutex held.
 */
static struct cgroup *task_cgroup_from_root(struct task_struct *task,
					    struct cgroupfs_root *root)
{
	struct css_set *css;
	struct cgroup *res = NULL;

	BUG_ON(!mutex_is_locked(&cgroup_mutex));
	read_lock(&css_set_lock);
	/*
	 * No need to lock the task - since we hold cgroup_mutex the
	 * task can't change groups, so the only thing that can happen
	 * is that it exits and its css is set back to init_css_set.
	 */
	css = task->cgroups;
	if (css == &init_css_set) {
		res = &root->top_cgroup;
	} else {
		struct cg_cgroup_link *link;
		list_for_each_entry(link, &css->cg_links, cg_link_list) {
			struct cgroup *c = link->cgrp;
			if (c->root == root) {
				res = c;
				break;
			}
		}
	}
	read_unlock(&css_set_lock);
	BUG_ON(!res);
	return res;
}

/*
 * There is one global cgroup mutex. We also require taking
 * task_lock() when dereferencing a task's cgroup subsys pointers.
 * See "The task_lock() exception", at the end of this comment.
 *
 * A task must hold cgroup_mutex to modify cgroups.
 *
 * Any task can increment and decrement the count field without lock.
 * So in general, code holding cgroup_mutex can't rely on the count
 * field not changing.  However, if the count goes to zero, then only
 * cgroup_attach_task() can increment it again.  Because a count of zero
 * means that no tasks are currently attached, therefore there is no
 * way a task attached to that cgroup can fork (the other way to
 * increment the count).  So code holding cgroup_mutex can safely
 * assume that if the count is zero, it will stay zero. Similarly, if
 * a task holds cgroup_mutex on a cgroup with zero count, it
 * knows that the cgroup won't be removed, as cgroup_rmdir()
 * needs that mutex.
 *
 * The fork and exit callbacks cgroup_fork() and cgroup_exit(), don't
 * (usually) take cgroup_mutex.  These are the two most performance
 * critical pieces of code here.  The exception occurs on cgroup_exit(),
 * when a task in a notify_on_release cgroup exits.  Then cgroup_mutex
 * is taken, and if the cgroup count is zero, a usermode call made
 * to the release agent with the name of the cgroup (path relative to
 * the root of cgroup file system) as the argument.
 *
 * A cgroup can only be deleted if both its 'count' of using tasks
 * is zero, and its list of 'children' cgroups is empty.  Since all
 * tasks in the system use _some_ cgroup, and since there is always at
 * least one task in the system (init, pid == 1), therefore, top_cgroup
 * always has either children cgroups and/or using tasks.  So we don't
 * need a special hack to ensure that top_cgroup cannot be deleted.
 *
 *	The task_lock() exception
 *
 * The need for this exception arises from the action of
 * cgroup_attach_task(), which overwrites one task's cgroup pointer with
 * another.  It does so using cgroup_mutex, however there are
 * several performance critical places that need to reference
 * task->cgroup without the expense of grabbing a system global
 * mutex.  Therefore except as noted below, when dereferencing or, as
 * in cgroup_attach_task(), modifying a task's cgroup pointer we use
 * task_lock(), which acts on a spinlock (task->alloc_lock) already in
 * the task_struct routinely used for such matters.
=======
	list_add_tail(&link->cgrp_link, &cset->cgrp_links);
}

/**
 * find_css_set - return a new css_set with one cgroup updated
 * @old_cset: the baseline css_set
 * @cgrp: the cgroup to be updated
 *
 * Return a new css_set that's equivalent to @old_cset, but with @cgrp
 * substituted into the appropriate hierarchy.
 */
static struct css_set *find_css_set(struct css_set *old_cset,
				    struct cgroup *cgrp)
{
	struct cgroup_subsys_state *template[CGROUP_SUBSYS_COUNT] = { };
	struct css_set *cset;
	struct list_head tmp_links;
	struct cgrp_cset_link *link;
	struct cgroup_subsys *ss;
	unsigned long key;
	int ssid;

	lockdep_assert_held(&cgroup_mutex);

	/* First see if we already have a cgroup group that matches
	 * the desired set */
	down_read(&css_set_rwsem);
	cset = find_existing_css_set(old_cset, cgrp, template);
	if (cset)
		get_css_set(cset);
	up_read(&css_set_rwsem);

	if (cset)
		return cset;

	cset = kzalloc(sizeof(*cset), GFP_KERNEL);
	if (!cset)
		return NULL;

	/* Allocate all the cgrp_cset_link objects that we'll need */
	if (allocate_cgrp_cset_links(cgroup_root_count, &tmp_links) < 0) {
		kfree(cset);
		return NULL;
	}

	atomic_set(&cset->refcount, 1);
	INIT_LIST_HEAD(&cset->cgrp_links);
	INIT_LIST_HEAD(&cset->tasks);
	INIT_LIST_HEAD(&cset->mg_tasks);
	INIT_LIST_HEAD(&cset->mg_preload_node);
	INIT_LIST_HEAD(&cset->mg_node);
	INIT_HLIST_NODE(&cset->hlist);

	/* Copy the set of subsystem state objects generated in
	 * find_existing_css_set() */
	memcpy(cset->subsys, template, sizeof(cset->subsys));

	down_write(&css_set_rwsem);
	/* Add reference counts and links from the new css_set. */
	list_for_each_entry(link, &old_cset->cgrp_links, cgrp_link) {
		struct cgroup *c = link->cgrp;

		if (c->root == cgrp->root)
			c = cgrp;
		link_css_set(&tmp_links, cset, c);
	}

	BUG_ON(!list_empty(&tmp_links));

	css_set_count++;

	/* Add @cset to the hash table */
	key = css_set_hash(cset->subsys);
	hash_add(css_set_table, &cset->hlist, key);

	for_each_subsys(ss, ssid)
		list_add_tail(&cset->e_cset_node[ssid],
			      &cset->subsys[ssid]->cgroup->e_csets[ssid]);

	up_write(&css_set_rwsem);

	return cset;
}

static struct cgroup_root *cgroup_root_from_kf(struct kernfs_root *kf_root)
{
	struct cgroup *root_cgrp = kf_root->kn->priv;

	return root_cgrp->root;
}

static int cgroup_init_root_id(struct cgroup_root *root)
{
	int id;

	lockdep_assert_held(&cgroup_mutex);

	id = idr_alloc_cyclic(&cgroup_hierarchy_idr, root, 0, 0, GFP_KERNEL);
	if (id < 0)
		return id;

	root->hierarchy_id = id;
	return 0;
}

static void cgroup_exit_root_id(struct cgroup_root *root)
{
	lockdep_assert_held(&cgroup_mutex);

	if (root->hierarchy_id) {
		idr_remove(&cgroup_hierarchy_idr, root->hierarchy_id);
		root->hierarchy_id = 0;
	}
}

static void cgroup_free_root(struct cgroup_root *root)
{
	if (root) {
		/* hierarhcy ID shoulid already have been released */
		WARN_ON_ONCE(root->hierarchy_id);

		idr_destroy(&root->cgroup_idr);
		kfree(root);
	}
}

static void cgroup_destroy_root(struct cgroup_root *root)
{
	struct cgroup *cgrp = &root->cgrp;
	struct cgrp_cset_link *link, *tmp_link;

	mutex_lock(&cgroup_mutex);

	BUG_ON(atomic_read(&root->nr_cgrps));
	BUG_ON(!list_empty(&cgrp->self.children));

	/* Rebind all subsystems back to the default hierarchy */
	rebind_subsystems(&cgrp_dfl_root, root->subsys_mask);

	/*
	 * Release all the links from cset_links to this hierarchy's
	 * root cgroup
	 */
	down_write(&css_set_rwsem);

	list_for_each_entry_safe(link, tmp_link, &cgrp->cset_links, cset_link) {
		list_del(&link->cset_link);
		list_del(&link->cgrp_link);
		kfree(link);
	}
	up_write(&css_set_rwsem);

	if (!list_empty(&root->root_list)) {
		list_del(&root->root_list);
		cgroup_root_count--;
	}

	cgroup_exit_root_id(root);

	mutex_unlock(&cgroup_mutex);

	kernfs_destroy_root(root->kf_root);
	cgroup_free_root(root);
}

/* look up cgroup associated with given css_set on the specified hierarchy */
static struct cgroup *cset_cgroup_from_root(struct css_set *cset,
					    struct cgroup_root *root)
{
	struct cgroup *res = NULL;

	lockdep_assert_held(&cgroup_mutex);
	lockdep_assert_held(&css_set_rwsem);

	if (cset == &init_css_set) {
		res = &root->cgrp;
	} else {
		struct cgrp_cset_link *link;

		list_for_each_entry(link, &cset->cgrp_links, cgrp_link) {
			struct cgroup *c = link->cgrp;

			if (c->root == root) {
				res = c;
				break;
			}
		}
	}

	BUG_ON(!res);
	return res;
}

/*
 * Return the cgroup for "task" from the given hierarchy. Must be
 * called with cgroup_mutex and css_set_rwsem held.
 */
static struct cgroup *task_cgroup_from_root(struct task_struct *task,
					    struct cgroup_root *root)
{
	/*
	 * No need to lock the task - since we hold cgroup_mutex the
	 * task can't change groups, so the only thing that can happen
	 * is that it exits and its css is set back to init_css_set.
	 */
	return cset_cgroup_from_root(task_css_set(task), root);
}

/*
 * A task must hold cgroup_mutex to modify cgroups.
 *
 * Any task can increment and decrement the count field without lock.
 * So in general, code holding cgroup_mutex can't rely on the count
 * field not changing.  However, if the count goes to zero, then only
 * cgroup_attach_task() can increment it again.  Because a count of zero
 * means that no tasks are currently attached, therefore there is no
 * way a task attached to that cgroup can fork (the other way to
 * increment the count).  So code holding cgroup_mutex can safely
 * assume that if the count is zero, it will stay zero. Similarly, if
 * a task holds cgroup_mutex on a cgroup with zero count, it
 * knows that the cgroup won't be removed, as cgroup_rmdir()
 * needs that mutex.
 *
 * A cgroup can only be deleted if both its 'count' of using tasks
 * is zero, and its list of 'children' cgroups is empty.  Since all
 * tasks in the system use _some_ cgroup, and since there is always at
 * least one task in the system (init, pid == 1), therefore, root cgroup
 * always has either children cgroups and/or using tasks.  So we don't
 * need a special hack to ensure that root cgroup cannot be deleted.
>>>>>>> v3.18
 *
 * P.S.  One more locking exception.  RCU is used to guard the
 * update of a tasks cgroup pointer by cgroup_attach_task()
 */

<<<<<<< HEAD
/*
 * A couple of forward declarations required, due to cyclic reference loop:
 * cgroup_mkdir -> cgroup_create -> cgroup_populate_dir ->
 * cgroup_add_file -> cgroup_create_file -> cgroup_dir_inode_operations
 * -> cgroup_mkdir.
 */

static int cgroup_mkdir(struct inode *dir, struct dentry *dentry, umode_t mode);
static struct dentry *cgroup_lookup(struct inode *, struct dentry *, unsigned int);
static int cgroup_rmdir(struct inode *unused_dir, struct dentry *dentry);
static int cgroup_populate_dir(struct cgroup *cgrp, bool base_files,
			       unsigned long subsys_mask);
static const struct inode_operations cgroup_dir_inode_operations;
static const struct file_operations proc_cgroupstats_operations;

static struct backing_dev_info cgroup_backing_dev_info = {
	.name		= "cgroup",
	.capabilities	= BDI_CAP_NO_ACCT_AND_WRITEBACK,
};

static int alloc_css_id(struct cgroup_subsys *ss,
			struct cgroup *parent, struct cgroup *child);

static struct inode *cgroup_new_inode(umode_t mode, struct super_block *sb)
{
	struct inode *inode = new_inode(sb);

	if (inode) {
		inode->i_ino = get_next_ino();
		inode->i_mode = mode;
		inode->i_uid = current_fsuid();
		inode->i_gid = current_fsgid();
		inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
		inode->i_mapping->backing_dev_info = &cgroup_backing_dev_info;
	}
	return inode;
}

static struct cgroup_name *cgroup_alloc_name(struct dentry *dentry)
{
	struct cgroup_name *name;

	name = kmalloc(sizeof(*name) + dentry->d_name.len + 1, GFP_KERNEL);
	if (!name)
		return NULL;
	strcpy(name->name, dentry->d_name.name);
	return name;
}

static void cgroup_free_fn(struct work_struct *work)
{
	struct cgroup *cgrp = container_of(work, struct cgroup, free_work);
	struct cgroup_subsys *ss;

	mutex_lock(&cgroup_mutex);
	/*
	 * Release the subsystem state objects.
	 */
	for_each_subsys(cgrp->root, ss)
		ss->css_free(cgrp);

	cgrp->root->number_of_cgroups--;
	mutex_unlock(&cgroup_mutex);

	/*
	 * We get a ref to the parent's dentry, and put the ref when
	 * this cgroup is being freed, so it's guaranteed that the
	 * parent won't be destroyed before its children.
	 */
	dput(cgrp->parent->dentry);

	ida_simple_remove(&cgrp->root->cgroup_ida, cgrp->id);

	/*
	 * Drop the active superblock reference that we took when we
	 * created the cgroup. This will free cgrp->root, if we are
	 * holding the last reference to @sb.
	 */
	deactivate_super(cgrp->root->sb);

	/*
	 * if we're getting rid of the cgroup, refcount should ensure
	 * that there are no pidlists left.
	 */
	BUG_ON(!list_empty(&cgrp->pidlists));

	simple_xattrs_free(&cgrp->xattrs);

	kfree(rcu_dereference_raw(cgrp->name));
	kfree(cgrp);
}

static void cgroup_free_rcu(struct rcu_head *head)
{
	struct cgroup *cgrp = container_of(head, struct cgroup, rcu_head);

	queue_work(cgroup_destroy_wq, &cgrp->free_work);
}

static void cgroup_diput(struct dentry *dentry, struct inode *inode)
{
	/* is dentry a directory ? if so, kfree() associated cgroup */
	if (S_ISDIR(inode->i_mode)) {
		struct cgroup *cgrp = dentry->d_fsdata;

		BUG_ON(!(cgroup_is_removed(cgrp)));
		call_rcu(&cgrp->rcu_head, cgroup_free_rcu);
	} else {
		struct cfent *cfe = __d_cfe(dentry);
		struct cgroup *cgrp = dentry->d_parent->d_fsdata;

		WARN_ONCE(!list_empty(&cfe->node) &&
			  cgrp != &cgrp->root->top_cgroup,
			  "cfe still linked for %s\n", cfe->type->name);
		simple_xattrs_free(&cfe->xattrs);
		kfree(cfe);
	}
	iput(inode);
}

static int cgroup_delete(const struct dentry *d)
{
	return 1;
}

static void remove_dir(struct dentry *d)
{
	struct dentry *parent = dget(d->d_parent);

	d_delete(d);
	simple_rmdir(parent->d_inode, d);
	dput(parent);
=======
static int cgroup_populate_dir(struct cgroup *cgrp, unsigned int subsys_mask);
static struct kernfs_syscall_ops cgroup_kf_syscall_ops;
static const struct file_operations proc_cgroupstats_operations;

static char *cgroup_file_name(struct cgroup *cgrp, const struct cftype *cft,
			      char *buf)
{
	if (cft->ss && !(cft->flags & CFTYPE_NO_PREFIX) &&
	    !(cgrp->root->flags & CGRP_ROOT_NOPREFIX))
		snprintf(buf, CGROUP_FILE_NAME_MAX, "%s.%s",
			 cft->ss->name, cft->name);
	else
		strncpy(buf, cft->name, CGROUP_FILE_NAME_MAX);
	return buf;
}

/**
 * cgroup_file_mode - deduce file mode of a control file
 * @cft: the control file in question
 *
 * returns cft->mode if ->mode is not 0
 * returns S_IRUGO|S_IWUSR if it has both a read and a write handler
 * returns S_IRUGO if it has only a read handler
 * returns S_IWUSR if it has only a write hander
 */
static umode_t cgroup_file_mode(const struct cftype *cft)
{
	umode_t mode = 0;

	if (cft->mode)
		return cft->mode;

	if (cft->read_u64 || cft->read_s64 || cft->seq_show)
		mode |= S_IRUGO;

	if (cft->write_u64 || cft->write_s64 || cft->write)
		mode |= S_IWUSR;

	return mode;
}

static void cgroup_get(struct cgroup *cgrp)
{
	WARN_ON_ONCE(cgroup_is_dead(cgrp));
	css_get(&cgrp->self);
}

static bool cgroup_tryget(struct cgroup *cgrp)
{
	return css_tryget(&cgrp->self);
}

static void cgroup_put(struct cgroup *cgrp)
{
	css_put(&cgrp->self);
}

/**
 * cgroup_refresh_child_subsys_mask - update child_subsys_mask
 * @cgrp: the target cgroup
 *
 * On the default hierarchy, a subsystem may request other subsystems to be
 * enabled together through its ->depends_on mask.  In such cases, more
 * subsystems than specified in "cgroup.subtree_control" may be enabled.
 *
 * This function determines which subsystems need to be enabled given the
 * current @cgrp->subtree_control and records it in
 * @cgrp->child_subsys_mask.  The resulting mask is always a superset of
 * @cgrp->subtree_control and follows the usual hierarchy rules.
 */
static void cgroup_refresh_child_subsys_mask(struct cgroup *cgrp)
{
	struct cgroup *parent = cgroup_parent(cgrp);
	unsigned int cur_ss_mask = cgrp->subtree_control;
	struct cgroup_subsys *ss;
	int ssid;

	lockdep_assert_held(&cgroup_mutex);

	if (!cgroup_on_dfl(cgrp)) {
		cgrp->child_subsys_mask = cur_ss_mask;
		return;
	}

	while (true) {
		unsigned int new_ss_mask = cur_ss_mask;

		for_each_subsys(ss, ssid)
			if (cur_ss_mask & (1 << ssid))
				new_ss_mask |= ss->depends_on;

		/*
		 * Mask out subsystems which aren't available.  This can
		 * happen only if some depended-upon subsystems were bound
		 * to non-default hierarchies.
		 */
		if (parent)
			new_ss_mask &= parent->child_subsys_mask;
		else
			new_ss_mask &= cgrp->root->subsys_mask;

		if (new_ss_mask == cur_ss_mask)
			break;
		cur_ss_mask = new_ss_mask;
	}

	cgrp->child_subsys_mask = cur_ss_mask;
}

/**
 * cgroup_kn_unlock - unlocking helper for cgroup kernfs methods
 * @kn: the kernfs_node being serviced
 *
 * This helper undoes cgroup_kn_lock_live() and should be invoked before
 * the method finishes if locking succeeded.  Note that once this function
 * returns the cgroup returned by cgroup_kn_lock_live() may become
 * inaccessible any time.  If the caller intends to continue to access the
 * cgroup, it should pin it before invoking this function.
 */
static void cgroup_kn_unlock(struct kernfs_node *kn)
{
	struct cgroup *cgrp;

	if (kernfs_type(kn) == KERNFS_DIR)
		cgrp = kn->priv;
	else
		cgrp = kn->parent->priv;

	mutex_unlock(&cgroup_mutex);

	kernfs_unbreak_active_protection(kn);
	cgroup_put(cgrp);
}

/**
 * cgroup_kn_lock_live - locking helper for cgroup kernfs methods
 * @kn: the kernfs_node being serviced
 *
 * This helper is to be used by a cgroup kernfs method currently servicing
 * @kn.  It breaks the active protection, performs cgroup locking and
 * verifies that the associated cgroup is alive.  Returns the cgroup if
 * alive; otherwise, %NULL.  A successful return should be undone by a
 * matching cgroup_kn_unlock() invocation.
 *
 * Any cgroup kernfs method implementation which requires locking the
 * associated cgroup should use this helper.  It avoids nesting cgroup
 * locking under kernfs active protection and allows all kernfs operations
 * including self-removal.
 */
static struct cgroup *cgroup_kn_lock_live(struct kernfs_node *kn)
{
	struct cgroup *cgrp;

	if (kernfs_type(kn) == KERNFS_DIR)
		cgrp = kn->priv;
	else
		cgrp = kn->parent->priv;

	/*
	 * We're gonna grab cgroup_mutex which nests outside kernfs
	 * active_ref.  cgroup liveliness check alone provides enough
	 * protection against removal.  Ensure @cgrp stays accessible and
	 * break the active_ref protection.
	 */
	if (!cgroup_tryget(cgrp))
		return NULL;
	kernfs_break_active_protection(kn);

	mutex_lock(&cgroup_mutex);

	if (!cgroup_is_dead(cgrp))
		return cgrp;

	cgroup_kn_unlock(kn);
	return NULL;
>>>>>>> v3.18
}

static void cgroup_rm_file(struct cgroup *cgrp, const struct cftype *cft)
{
<<<<<<< HEAD
	struct cfent *cfe;

	lockdep_assert_held(&cgrp->dentry->d_inode->i_mutex);
	lockdep_assert_held(&cgroup_mutex);

	/*
	 * If we're doing cleanup due to failure of cgroup_create(),
	 * the corresponding @cfe may not exist.
	 */
	list_for_each_entry(cfe, &cgrp->files, node) {
		struct dentry *d = cfe->dentry;

		if (cft && cfe->type != cft)
			continue;

		dget(d);
		d_delete(d);
		simple_unlink(cgrp->dentry->d_inode, d);
		list_del_init(&cfe->node);
		dput(d);

		break;
	}
}

/**
 * cgroup_clear_directory - selective removal of base and subsystem files
 * @dir: directory containing the files
 * @base_files: true if the base files should be removed
 * @subsys_mask: mask of the subsystem ids whose files should be removed
 */
static void cgroup_clear_directory(struct dentry *dir, bool base_files,
				   unsigned long subsys_mask)
{
	struct cgroup *cgrp = __d_cgrp(dir);
	struct cgroup_subsys *ss;

	for_each_subsys(cgrp->root, ss) {
		struct cftype_set *set;
		if (!test_bit(ss->subsys_id, &subsys_mask))
			continue;
		list_for_each_entry(set, &ss->cftsets, node)
			cgroup_addrm_files(cgrp, NULL, set->cfts, false);
	}
	if (base_files) {
		while (!list_empty(&cgrp->files))
			cgroup_rm_file(cgrp, NULL);
	}
}

/*
 * NOTE : the dentry must have been dget()'ed
 */
static void cgroup_d_remove_dir(struct dentry *dentry)
{
	struct dentry *parent;
	struct cgroupfs_root *root = dentry->d_sb->s_fs_info;

	cgroup_clear_directory(dentry, true, root->subsys_mask);

	parent = dentry->d_parent;
	spin_lock(&parent->d_lock);
	spin_lock_nested(&dentry->d_lock, DENTRY_D_LOCK_NESTED);
	list_del_init(&dentry->d_child);
	spin_unlock(&dentry->d_lock);
	spin_unlock(&parent->d_lock);
	remove_dir(dentry);
}

/*
 * Call with cgroup_mutex held. Drops reference counts on modules, including
 * any duplicate ones that parse_cgroupfs_options took. If this function
 * returns an error, no reference counts are touched.
 */
static int rebind_subsystems(struct cgroupfs_root *root,
			      unsigned long final_subsys_mask)
{
	unsigned long added_mask, removed_mask;
	struct cgroup *cgrp = &root->top_cgroup;
	int i;

	BUG_ON(!mutex_is_locked(&cgroup_mutex));
	BUG_ON(!mutex_is_locked(&cgroup_root_mutex));

	removed_mask = root->actual_subsys_mask & ~final_subsys_mask;
	added_mask = final_subsys_mask & ~root->actual_subsys_mask;
	/* Check that any added subsystems are currently free */
	for (i = 0; i < CGROUP_SUBSYS_COUNT; i++) {
		unsigned long bit = 1UL << i;
		struct cgroup_subsys *ss = subsys[i];
		if (!(bit & added_mask))
			continue;
		/*
		 * Nobody should tell us to do a subsys that doesn't exist:
		 * parse_cgroupfs_options should catch that case and refcounts
		 * ensure that subsystems won't disappear once selected.
		 */
		BUG_ON(ss == NULL);
		if (ss->root != &rootnode) {
			/* Subsystem isn't free */
			return -EBUSY;
		}
	}

	/* Currently we don't handle adding/removing subsystems when
	 * any child cgroups exist. This is theoretically supportable
	 * but involves complex error handling, so it's being left until
	 * later */
	if (root->number_of_cgroups > 1)
		return -EBUSY;

	/* Process each subsystem */
	for (i = 0; i < CGROUP_SUBSYS_COUNT; i++) {
		struct cgroup_subsys *ss = subsys[i];
		unsigned long bit = 1UL << i;
		if (bit & added_mask) {
			/* We're binding this subsystem to this hierarchy */
			BUG_ON(ss == NULL);
			BUG_ON(cgrp->subsys[i]);
			BUG_ON(!dummytop->subsys[i]);
			BUG_ON(dummytop->subsys[i]->cgroup != dummytop);
			cgrp->subsys[i] = dummytop->subsys[i];
			cgrp->subsys[i]->cgroup = cgrp;
			list_move(&ss->sibling, &root->subsys_list);
			ss->root = root;
			if (ss->bind)
				ss->bind(cgrp);
			/* refcount was already taken, and we're keeping it */
		} else if (bit & removed_mask) {
			/* We're removing this subsystem */
			BUG_ON(ss == NULL);
			BUG_ON(cgrp->subsys[i] != dummytop->subsys[i]);
			BUG_ON(cgrp->subsys[i]->cgroup != cgrp);
			if (ss->bind)
				ss->bind(dummytop);
			dummytop->subsys[i]->cgroup = dummytop;
			cgrp->subsys[i] = NULL;
			subsys[i]->root = &rootnode;
			list_move(&ss->sibling, &rootnode.subsys_list);
			/* subsystem is now free - drop reference on module */
			module_put(ss->module);
		} else if (bit & final_subsys_mask) {
			/* Subsystem state should already exist */
			BUG_ON(ss == NULL);
			BUG_ON(!cgrp->subsys[i]);
			/*
			 * a refcount was taken, but we already had one, so
			 * drop the extra reference.
			 */
			module_put(ss->module);
#ifdef CONFIG_MODULE_UNLOAD
			BUG_ON(ss->module && !module_refcount(ss->module));
#endif
		} else {
			/* Subsystem state shouldn't exist */
			BUG_ON(cgrp->subsys[i]);
		}
	}
	root->subsys_mask = root->actual_subsys_mask = final_subsys_mask;

	return 0;
}

static int cgroup_show_options(struct seq_file *seq, struct dentry *dentry)
{
	struct cgroupfs_root *root = dentry->d_sb->s_fs_info;
	struct cgroup_subsys *ss;

	mutex_lock(&cgroup_root_mutex);
	for_each_subsys(root, ss)
		seq_printf(seq, ",%s", ss->name);
	if (root->flags & CGRP_ROOT_SANE_BEHAVIOR)
		seq_puts(seq, ",sane_behavior");
=======
	char name[CGROUP_FILE_NAME_MAX];

	lockdep_assert_held(&cgroup_mutex);
	kernfs_remove_by_name(cgrp->kn, cgroup_file_name(cgrp, cft, name));
}

/**
 * cgroup_clear_dir - remove subsys files in a cgroup directory
 * @cgrp: target cgroup
 * @subsys_mask: mask of the subsystem ids whose files should be removed
 */
static void cgroup_clear_dir(struct cgroup *cgrp, unsigned int subsys_mask)
{
	struct cgroup_subsys *ss;
	int i;

	for_each_subsys(ss, i) {
		struct cftype *cfts;

		if (!(subsys_mask & (1 << i)))
			continue;
		list_for_each_entry(cfts, &ss->cfts, node)
			cgroup_addrm_files(cgrp, cfts, false);
	}
}

static int rebind_subsystems(struct cgroup_root *dst_root, unsigned int ss_mask)
{
	struct cgroup_subsys *ss;
	unsigned int tmp_ss_mask;
	int ssid, i, ret;

	lockdep_assert_held(&cgroup_mutex);

	for_each_subsys(ss, ssid) {
		if (!(ss_mask & (1 << ssid)))
			continue;

		/* if @ss has non-root csses attached to it, can't move */
		if (css_next_child(NULL, cgroup_css(&ss->root->cgrp, ss)))
			return -EBUSY;

		/* can't move between two non-dummy roots either */
		if (ss->root != &cgrp_dfl_root && dst_root != &cgrp_dfl_root)
			return -EBUSY;
	}

	/* skip creating root files on dfl_root for inhibited subsystems */
	tmp_ss_mask = ss_mask;
	if (dst_root == &cgrp_dfl_root)
		tmp_ss_mask &= ~cgrp_dfl_root_inhibit_ss_mask;

	ret = cgroup_populate_dir(&dst_root->cgrp, tmp_ss_mask);
	if (ret) {
		if (dst_root != &cgrp_dfl_root)
			return ret;

		/*
		 * Rebinding back to the default root is not allowed to
		 * fail.  Using both default and non-default roots should
		 * be rare.  Moving subsystems back and forth even more so.
		 * Just warn about it and continue.
		 */
		if (cgrp_dfl_root_visible) {
			pr_warn("failed to create files (%d) while rebinding 0x%x to default root\n",
				ret, ss_mask);
			pr_warn("you may retry by moving them to a different hierarchy and unbinding\n");
		}
	}

	/*
	 * Nothing can fail from this point on.  Remove files for the
	 * removed subsystems and rebind each subsystem.
	 */
	for_each_subsys(ss, ssid)
		if (ss_mask & (1 << ssid))
			cgroup_clear_dir(&ss->root->cgrp, 1 << ssid);

	for_each_subsys(ss, ssid) {
		struct cgroup_root *src_root;
		struct cgroup_subsys_state *css;
		struct css_set *cset;

		if (!(ss_mask & (1 << ssid)))
			continue;

		src_root = ss->root;
		css = cgroup_css(&src_root->cgrp, ss);

		WARN_ON(!css || cgroup_css(&dst_root->cgrp, ss));

		RCU_INIT_POINTER(src_root->cgrp.subsys[ssid], NULL);
		rcu_assign_pointer(dst_root->cgrp.subsys[ssid], css);
		ss->root = dst_root;
		css->cgroup = &dst_root->cgrp;

		down_write(&css_set_rwsem);
		hash_for_each(css_set_table, i, cset, hlist)
			list_move_tail(&cset->e_cset_node[ss->id],
				       &dst_root->cgrp.e_csets[ss->id]);
		up_write(&css_set_rwsem);

		src_root->subsys_mask &= ~(1 << ssid);
		src_root->cgrp.subtree_control &= ~(1 << ssid);
		cgroup_refresh_child_subsys_mask(&src_root->cgrp);

		/* default hierarchy doesn't enable controllers by default */
		dst_root->subsys_mask |= 1 << ssid;
		if (dst_root != &cgrp_dfl_root) {
			dst_root->cgrp.subtree_control |= 1 << ssid;
			cgroup_refresh_child_subsys_mask(&dst_root->cgrp);
		}

		if (ss->bind)
			ss->bind(css);
	}

	kernfs_activate(dst_root->cgrp.kn);
	return 0;
}

static int cgroup_show_options(struct seq_file *seq,
			       struct kernfs_root *kf_root)
{
	struct cgroup_root *root = cgroup_root_from_kf(kf_root);
	struct cgroup_subsys *ss;
	int ssid;

	for_each_subsys(ss, ssid)
		if (root->subsys_mask & (1 << ssid))
			seq_printf(seq, ",%s", ss->name);
>>>>>>> v3.18
	if (root->flags & CGRP_ROOT_NOPREFIX)
		seq_puts(seq, ",noprefix");
	if (root->flags & CGRP_ROOT_XATTR)
		seq_puts(seq, ",xattr");
<<<<<<< HEAD
	if (strlen(root->release_agent_path))
		seq_printf(seq, ",release_agent=%s", root->release_agent_path);
	if (test_bit(CGRP_CPUSET_CLONE_CHILDREN, &root->top_cgroup.flags))
		seq_puts(seq, ",clone_children");
	if (strlen(root->name))
		seq_printf(seq, ",name=%s", root->name);
	mutex_unlock(&cgroup_root_mutex);
=======

	spin_lock(&release_agent_path_lock);
	if (strlen(root->release_agent_path))
		seq_printf(seq, ",release_agent=%s", root->release_agent_path);
	spin_unlock(&release_agent_path_lock);

	if (test_bit(CGRP_CPUSET_CLONE_CHILDREN, &root->cgrp.flags))
		seq_puts(seq, ",clone_children");
	if (strlen(root->name))
		seq_printf(seq, ",name=%s", root->name);
>>>>>>> v3.18
	return 0;
}

struct cgroup_sb_opts {
<<<<<<< HEAD
	unsigned long subsys_mask;
	unsigned long flags;
=======
	unsigned int subsys_mask;
	unsigned int flags;
>>>>>>> v3.18
	char *release_agent;
	bool cpuset_clone_children;
	char *name;
	/* User explicitly requested empty subsystem */
	bool none;
<<<<<<< HEAD

	struct cgroupfs_root *new_root;

};

/*
 * Convert a hierarchy specifier into a bitmask of subsystems and flags. Call
 * with cgroup_mutex held to protect the subsys[] array. This function takes
 * refcounts on subsystems to be used, unless it returns error, in which case
 * no refcounts are taken.
 */
=======
};

>>>>>>> v3.18
static int parse_cgroupfs_options(char *data, struct cgroup_sb_opts *opts)
{
	char *token, *o = data;
	bool all_ss = false, one_ss = false;
<<<<<<< HEAD
	unsigned long mask = (unsigned long)-1;
	int i;
	bool module_pin_failed = false;

	BUG_ON(!mutex_is_locked(&cgroup_mutex));

#ifdef CONFIG_CPUSETS
	mask = ~(1UL << cpuset_subsys_id);
=======
	unsigned int mask = -1U;
	struct cgroup_subsys *ss;
	int nr_opts = 0;
	int i;

#ifdef CONFIG_CPUSETS
	mask = ~(1U << cpuset_cgrp_id);
>>>>>>> v3.18
#endif

	memset(opts, 0, sizeof(*opts));

	while ((token = strsep(&o, ",")) != NULL) {
<<<<<<< HEAD
=======
		nr_opts++;

>>>>>>> v3.18
		if (!*token)
			return -EINVAL;
		if (!strcmp(token, "none")) {
			/* Explicitly have no subsystems */
			opts->none = true;
			continue;
		}
		if (!strcmp(token, "all")) {
			/* Mutually exclusive option 'all' + subsystem name */
			if (one_ss)
				return -EINVAL;
			all_ss = true;
			continue;
		}
		if (!strcmp(token, "__DEVEL__sane_behavior")) {
			opts->flags |= CGRP_ROOT_SANE_BEHAVIOR;
			continue;
		}
		if (!strcmp(token, "noprefix")) {
			opts->flags |= CGRP_ROOT_NOPREFIX;
			continue;
		}
		if (!strcmp(token, "clone_children")) {
			opts->cpuset_clone_children = true;
			continue;
		}
		if (!strcmp(token, "xattr")) {
			opts->flags |= CGRP_ROOT_XATTR;
			continue;
		}
		if (!strncmp(token, "release_agent=", 14)) {
			/* Specifying two release agents is forbidden */
			if (opts->release_agent)
				return -EINVAL;
			opts->release_agent =
				kstrndup(token + 14, PATH_MAX - 1, GFP_KERNEL);
			if (!opts->release_agent)
				return -ENOMEM;
			continue;
		}
		if (!strncmp(token, "name=", 5)) {
			const char *name = token + 5;
			/* Can't specify an empty name */
			if (!strlen(name))
				return -EINVAL;
			/* Must match [\w.-]+ */
			for (i = 0; i < strlen(name); i++) {
				char c = name[i];
				if (isalnum(c))
					continue;
				if ((c == '.') || (c == '-') || (c == '_'))
					continue;
				return -EINVAL;
			}
			/* Specifying two names is forbidden */
			if (opts->name)
				return -EINVAL;
			opts->name = kstrndup(name,
					      MAX_CGROUP_ROOT_NAMELEN - 1,
					      GFP_KERNEL);
			if (!opts->name)
				return -ENOMEM;

			continue;
		}

<<<<<<< HEAD
		for (i = 0; i < CGROUP_SUBSYS_COUNT; i++) {
			struct cgroup_subsys *ss = subsys[i];
			if (ss == NULL)
				continue;
=======
		for_each_subsys(ss, i) {
>>>>>>> v3.18
			if (strcmp(token, ss->name))
				continue;
			if (ss->disabled)
				continue;

			/* Mutually exclusive option 'all' + subsystem name */
			if (all_ss)
				return -EINVAL;
<<<<<<< HEAD
			set_bit(i, &opts->subsys_mask);
=======
			opts->subsys_mask |= (1 << i);
>>>>>>> v3.18
			one_ss = true;

			break;
		}
		if (i == CGROUP_SUBSYS_COUNT)
			return -ENOENT;
	}

<<<<<<< HEAD
	/*
	 * If the 'all' option was specified select all the subsystems,
	 * otherwise if 'none', 'name=' and a subsystem name options
	 * were not specified, let's default to 'all'
	 */
	if (all_ss || (!one_ss && !opts->none && !opts->name)) {
		for (i = 0; i < CGROUP_SUBSYS_COUNT; i++) {
			struct cgroup_subsys *ss = subsys[i];
			if (ss == NULL)
				continue;
			if (ss->disabled)
				continue;
			set_bit(i, &opts->subsys_mask);
		}
	}

	/* Consistency checks */

	if (opts->flags & CGRP_ROOT_SANE_BEHAVIOR) {
		pr_warning("cgroup: sane_behavior: this is still under development and its behaviors will change, proceed at your own risk\n");

		if (opts->flags & CGRP_ROOT_NOPREFIX) {
			pr_err("cgroup: sane_behavior: noprefix is not allowed\n");
			return -EINVAL;
		}

		if (opts->cpuset_clone_children) {
			pr_err("cgroup: sane_behavior: clone_children is not allowed\n");
			return -EINVAL;
		}
	}

	/*
	 * Option noprefix was introduced just for backward compatibility
	 * with the old cpuset, so we allow noprefix only if mounting just
	 * the cpuset subsystem.
	 */
	if ((opts->flags & CGRP_ROOT_NOPREFIX) && (opts->subsys_mask & mask))
		return -EINVAL;


	/* Can't specify "none" and some subsystems */
	if (opts->subsys_mask && opts->none)
		return -EINVAL;
=======
	if (opts->flags & CGRP_ROOT_SANE_BEHAVIOR) {
		pr_warn("sane_behavior: this is still under development and its behaviors will change, proceed at your own risk\n");
		if (nr_opts != 1) {
			pr_err("sane_behavior: no other mount options allowed\n");
			return -EINVAL;
		}
		return 0;
	}

	/*
	 * If the 'all' option was specified select all the subsystems,
	 * otherwise if 'none', 'name=' and a subsystem name options were
	 * not specified, let's default to 'all'
	 */
	if (all_ss || (!one_ss && !opts->none && !opts->name))
		for_each_subsys(ss, i)
			if (!ss->disabled)
				opts->subsys_mask |= (1 << i);
>>>>>>> v3.18

	/*
	 * We either have to specify by name or by subsystems. (So all
	 * empty hierarchies must have a name).
	 */
	if (!opts->subsys_mask && !opts->name)
		return -EINVAL;

	/*
<<<<<<< HEAD
	 * Grab references on all the modules we'll need, so the subsystems
	 * don't dance around before rebind_subsystems attaches them. This may
	 * take duplicate reference counts on a subsystem that's already used,
	 * but rebind_subsystems handles this case.
	 */
	for (i = 0; i < CGROUP_SUBSYS_COUNT; i++) {
		unsigned long bit = 1UL << i;

		if (!(bit & opts->subsys_mask))
			continue;
		if (!try_module_get(subsys[i]->module)) {
			module_pin_failed = true;
			break;
		}
	}
	if (module_pin_failed) {
		/*
		 * oops, one of the modules was going away. this means that we
		 * raced with a module_delete call, and to the user this is
		 * essentially a "subsystem doesn't exist" case.
		 */
		for (i--; i >= 0; i--) {
			/* drop refcounts only on the ones we took */
			unsigned long bit = 1UL << i;

			if (!(bit & opts->subsys_mask))
				continue;
			module_put(subsys[i]->module);
		}
		return -ENOENT;
	}
=======
	 * Option noprefix was introduced just for backward compatibility
	 * with the old cpuset, so we allow noprefix only if mounting just
	 * the cpuset subsystem.
	 */
	if ((opts->flags & CGRP_ROOT_NOPREFIX) && (opts->subsys_mask & mask))
		return -EINVAL;

	/* Can't specify "none" and some subsystems */
	if (opts->subsys_mask && opts->none)
		return -EINVAL;
>>>>>>> v3.18

	return 0;
}

<<<<<<< HEAD
static void drop_parsed_module_refcounts(unsigned long subsys_mask)
{
	int i;
	for (i = 0; i < CGROUP_SUBSYS_COUNT; i++) {
		unsigned long bit = 1UL << i;

		if (!(bit & subsys_mask))
			continue;
		module_put(subsys[i]->module);
	}
}

static int cgroup_remount(struct super_block *sb, int *flags, char *data)
{
	int ret = 0;
	struct cgroupfs_root *root = sb->s_fs_info;
	struct cgroup *cgrp = &root->top_cgroup;
	struct cgroup_sb_opts opts;
	unsigned long added_mask, removed_mask;

	if (root->flags & CGRP_ROOT_SANE_BEHAVIOR) {
		pr_err("cgroup: sane_behavior: remount is not allowed\n");
		return -EINVAL;
	}

	mutex_lock(&cgrp->dentry->d_inode->i_mutex);
	mutex_lock(&cgroup_mutex);
	mutex_lock(&cgroup_root_mutex);
=======
static int cgroup_remount(struct kernfs_root *kf_root, int *flags, char *data)
{
	int ret = 0;
	struct cgroup_root *root = cgroup_root_from_kf(kf_root);
	struct cgroup_sb_opts opts;
	unsigned int added_mask, removed_mask;

	if (root == &cgrp_dfl_root) {
		pr_err("remount is not allowed\n");
		return -EINVAL;
	}

	mutex_lock(&cgroup_mutex);
>>>>>>> v3.18

	/* See what subsystems are wanted */
	ret = parse_cgroupfs_options(data, &opts);
	if (ret)
		goto out_unlock;

<<<<<<< HEAD
	if (opts.subsys_mask != root->actual_subsys_mask || opts.release_agent)
		pr_warning("cgroup: option changes via remount are deprecated (pid=%d comm=%s)\n",
			   task_tgid_nr(current), current->comm);
=======
	if (opts.subsys_mask != root->subsys_mask || opts.release_agent)
		pr_warn("option changes via remount are deprecated (pid=%d comm=%s)\n",
			task_tgid_nr(current), current->comm);
>>>>>>> v3.18

	added_mask = opts.subsys_mask & ~root->subsys_mask;
	removed_mask = root->subsys_mask & ~opts.subsys_mask;

	/* Don't allow flags or name to change at remount */
<<<<<<< HEAD
	if (opts.flags != root->flags ||
	    (opts.name && strcmp(opts.name, root->name))) {
		ret = -EINVAL;
		drop_parsed_module_refcounts(opts.subsys_mask);
		goto out_unlock;
	}

	/*
	 * Clear out the files of subsystems that should be removed, do
	 * this before rebind_subsystems, since rebind_subsystems may
	 * change this hierarchy's subsys_list.
	 */
	cgroup_clear_directory(cgrp->dentry, false, removed_mask);

	ret = rebind_subsystems(root, opts.subsys_mask);
	if (ret) {
		/* rebind_subsystems failed, re-populate the removed files */
		cgroup_populate_dir(cgrp, false, removed_mask);
		drop_parsed_module_refcounts(opts.subsys_mask);
		goto out_unlock;
	}

	/* re-populate subsystem files */
	cgroup_populate_dir(cgrp, false, added_mask);

	if (opts.release_agent)
		strcpy(root->release_agent_path, opts.release_agent);
 out_unlock:
	kfree(opts.release_agent);
	kfree(opts.name);
	mutex_unlock(&cgroup_root_mutex);
	mutex_unlock(&cgroup_mutex);
	mutex_unlock(&cgrp->dentry->d_inode->i_mutex);
	return ret;
}

static const struct super_operations cgroup_ops = {
	.statfs = simple_statfs,
	.drop_inode = generic_delete_inode,
	.show_options = cgroup_show_options,
	.remount_fs = cgroup_remount,
};

static void init_cgroup_housekeeping(struct cgroup *cgrp)
{
	INIT_LIST_HEAD(&cgrp->sibling);
	INIT_LIST_HEAD(&cgrp->children);
	INIT_LIST_HEAD(&cgrp->files);
	INIT_LIST_HEAD(&cgrp->css_sets);
	INIT_LIST_HEAD(&cgrp->allcg_node);
	INIT_LIST_HEAD(&cgrp->release_list);
	INIT_LIST_HEAD(&cgrp->pidlists);
	INIT_WORK(&cgrp->free_work, cgroup_free_fn);
	mutex_init(&cgrp->pidlist_mutex);
	INIT_LIST_HEAD(&cgrp->event_list);
	spin_lock_init(&cgrp->event_list_lock);
	simple_xattrs_init(&cgrp->xattrs);
}

static void init_cgroup_root(struct cgroupfs_root *root)
{
	struct cgroup *cgrp = &root->top_cgroup;

	INIT_LIST_HEAD(&root->subsys_list);
	INIT_LIST_HEAD(&root->root_list);
	INIT_LIST_HEAD(&root->allcg_list);
	root->number_of_cgroups = 1;
	cgrp->root = root;
	cgrp->name = &root_cgroup_name;
	init_cgroup_housekeeping(cgrp);
	list_add_tail(&cgrp->allcg_node, &root->allcg_list);
}

static bool init_root_id(struct cgroupfs_root *root)
{
	int ret = 0;

	do {
		if (!ida_pre_get(&hierarchy_ida, GFP_KERNEL))
			return false;
		spin_lock(&hierarchy_id_lock);
		/* Try to allocate the next unused ID */
		ret = ida_get_new_above(&hierarchy_ida, next_hierarchy_id,
					&root->hierarchy_id);
		if (ret == -ENOSPC)
			/* Try again starting from 0 */
			ret = ida_get_new(&hierarchy_ida, &root->hierarchy_id);
		if (!ret) {
			next_hierarchy_id = root->hierarchy_id + 1;
		} else if (ret != -EAGAIN) {
			/* Can only get here if the 31-bit IDR is full ... */
			BUG_ON(ret);
		}
		spin_unlock(&hierarchy_id_lock);
	} while (ret);
	return true;
}

static int cgroup_test_super(struct super_block *sb, void *data)
{
	struct cgroup_sb_opts *opts = data;
	struct cgroupfs_root *root = sb->s_fs_info;

	/* If we asked for a name then it must match */
	if (opts->name && strcmp(opts->name, root->name))
		return 0;

	/*
	 * If we asked for subsystems (or explicitly for no
	 * subsystems) then they must match
	 */
	if ((opts->subsys_mask || opts->none)
	    && (opts->subsys_mask != root->subsys_mask))
		return 0;

	return 1;
}

static struct cgroupfs_root *cgroup_root_from_opts(struct cgroup_sb_opts *opts)
{
	struct cgroupfs_root *root;

	if (!opts->subsys_mask && !opts->none)
		return NULL;

	root = kzalloc(sizeof(*root), GFP_KERNEL);
	if (!root)
		return ERR_PTR(-ENOMEM);

	if (!init_root_id(root)) {
		kfree(root);
		return ERR_PTR(-ENOMEM);
	}
	init_cgroup_root(root);

	root->subsys_mask = opts->subsys_mask;
	root->flags = opts->flags;
	ida_init(&root->cgroup_ida);
=======
	if ((opts.flags ^ root->flags) ||
	    (opts.name && strcmp(opts.name, root->name))) {
		pr_err("option or name mismatch, new: 0x%x \"%s\", old: 0x%x \"%s\"\n",
		       opts.flags, opts.name ?: "", root->flags, root->name);
		ret = -EINVAL;
		goto out_unlock;
	}

	/* remounting is not allowed for populated hierarchies */
	if (!list_empty(&root->cgrp.self.children)) {
		ret = -EBUSY;
		goto out_unlock;
	}

	ret = rebind_subsystems(root, added_mask);
	if (ret)
		goto out_unlock;

	rebind_subsystems(&cgrp_dfl_root, removed_mask);

	if (opts.release_agent) {
		spin_lock(&release_agent_path_lock);
		strcpy(root->release_agent_path, opts.release_agent);
		spin_unlock(&release_agent_path_lock);
	}
 out_unlock:
	kfree(opts.release_agent);
	kfree(opts.name);
	mutex_unlock(&cgroup_mutex);
	return ret;
}

/*
 * To reduce the fork() overhead for systems that are not actually using
 * their cgroups capability, we don't maintain the lists running through
 * each css_set to its tasks until we see the list actually used - in other
 * words after the first mount.
 */
static bool use_task_css_set_links __read_mostly;

static void cgroup_enable_task_cg_lists(void)
{
	struct task_struct *p, *g;

	down_write(&css_set_rwsem);

	if (use_task_css_set_links)
		goto out_unlock;

	use_task_css_set_links = true;

	/*
	 * We need tasklist_lock because RCU is not safe against
	 * while_each_thread(). Besides, a forking task that has passed
	 * cgroup_post_fork() without seeing use_task_css_set_links = 1
	 * is not guaranteed to have its child immediately visible in the
	 * tasklist if we walk through it with RCU.
	 */
	read_lock(&tasklist_lock);
	do_each_thread(g, p) {
		WARN_ON_ONCE(!list_empty(&p->cg_list) ||
			     task_css_set(p) != &init_css_set);

		/*
		 * We should check if the process is exiting, otherwise
		 * it will race with cgroup_exit() in that the list
		 * entry won't be deleted though the process has exited.
		 * Do it while holding siglock so that we don't end up
		 * racing against cgroup_exit().
		 */
		spin_lock_irq(&p->sighand->siglock);
		if (!(p->flags & PF_EXITING)) {
			struct css_set *cset = task_css_set(p);

			list_add(&p->cg_list, &cset->tasks);
			get_css_set(cset);
		}
		spin_unlock_irq(&p->sighand->siglock);
	} while_each_thread(g, p);
	read_unlock(&tasklist_lock);
out_unlock:
	up_write(&css_set_rwsem);
}

static void init_cgroup_housekeeping(struct cgroup *cgrp)
{
	struct cgroup_subsys *ss;
	int ssid;

	INIT_LIST_HEAD(&cgrp->self.sibling);
	INIT_LIST_HEAD(&cgrp->self.children);
	INIT_LIST_HEAD(&cgrp->cset_links);
	INIT_LIST_HEAD(&cgrp->pidlists);
	mutex_init(&cgrp->pidlist_mutex);
	cgrp->self.cgroup = cgrp;
	cgrp->self.flags |= CSS_ONLINE;

	for_each_subsys(ss, ssid)
		INIT_LIST_HEAD(&cgrp->e_csets[ssid]);

	init_waitqueue_head(&cgrp->offline_waitq);
	INIT_WORK(&cgrp->release_agent_work, cgroup_release_agent);
}

static void init_cgroup_root(struct cgroup_root *root,
			     struct cgroup_sb_opts *opts)
{
	struct cgroup *cgrp = &root->cgrp;

	INIT_LIST_HEAD(&root->root_list);
	atomic_set(&root->nr_cgrps, 1);
	cgrp->root = root;
	init_cgroup_housekeeping(cgrp);
	idr_init(&root->cgroup_idr);

	root->flags = opts->flags;
>>>>>>> v3.18
	if (opts->release_agent)
		strcpy(root->release_agent_path, opts->release_agent);
	if (opts->name)
		strcpy(root->name, opts->name);
	if (opts->cpuset_clone_children)
<<<<<<< HEAD
		set_bit(CGRP_CPUSET_CLONE_CHILDREN, &root->top_cgroup.flags);
	return root;
}

static void cgroup_drop_root(struct cgroupfs_root *root)
{
	if (!root)
		return;

	BUG_ON(!root->hierarchy_id);
	spin_lock(&hierarchy_id_lock);
	ida_remove(&hierarchy_ida, root->hierarchy_id);
	spin_unlock(&hierarchy_id_lock);
	ida_destroy(&root->cgroup_ida);
	kfree(root);
}

static int cgroup_set_super(struct super_block *sb, void *data)
{
	int ret;
	struct cgroup_sb_opts *opts = data;

	/* If we don't have a new root, we can't set up a new sb */
	if (!opts->new_root)
		return -EINVAL;

	BUG_ON(!opts->subsys_mask && !opts->none);

	ret = set_anon_super(sb, NULL);
	if (ret)
		return ret;

	sb->s_fs_info = opts->new_root;
	opts->new_root->sb = sb;

	sb->s_blocksize = PAGE_CACHE_SIZE;
	sb->s_blocksize_bits = PAGE_CACHE_SHIFT;
	sb->s_magic = CGROUP_SUPER_MAGIC;
	sb->s_op = &cgroup_ops;

	return 0;
}

static int cgroup_get_rootdir(struct super_block *sb)
{
	static const struct dentry_operations cgroup_dops = {
		.d_iput = cgroup_diput,
		.d_delete = cgroup_delete,
	};

	struct inode *inode =
		cgroup_new_inode(S_IFDIR | S_IRUGO | S_IXUGO | S_IWUSR, sb);

	if (!inode)
		return -ENOMEM;

	inode->i_fop = &simple_dir_operations;
	inode->i_op = &cgroup_dir_inode_operations;
	/* directories start off with i_nlink == 2 (for "." entry) */
	inc_nlink(inode);
	sb->s_root = d_make_root(inode);
	if (!sb->s_root)
		return -ENOMEM;
	/* for everything else we want ->d_op set */
	sb->s_d_op = &cgroup_dops;
	return 0;
=======
		set_bit(CGRP_CPUSET_CLONE_CHILDREN, &root->cgrp.flags);
}

static int cgroup_setup_root(struct cgroup_root *root, unsigned int ss_mask)
{
	LIST_HEAD(tmp_links);
	struct cgroup *root_cgrp = &root->cgrp;
	struct cftype *base_files;
	struct css_set *cset;
	int i, ret;

	lockdep_assert_held(&cgroup_mutex);

	ret = cgroup_idr_alloc(&root->cgroup_idr, root_cgrp, 1, 2, GFP_NOWAIT);
	if (ret < 0)
		goto out;
	root_cgrp->id = ret;

	ret = percpu_ref_init(&root_cgrp->self.refcnt, css_release, 0,
			      GFP_KERNEL);
	if (ret)
		goto out;

	/*
	 * We're accessing css_set_count without locking css_set_rwsem here,
	 * but that's OK - it can only be increased by someone holding
	 * cgroup_lock, and that's us. The worst that can happen is that we
	 * have some link structures left over
	 */
	ret = allocate_cgrp_cset_links(css_set_count, &tmp_links);
	if (ret)
		goto cancel_ref;

	ret = cgroup_init_root_id(root);
	if (ret)
		goto cancel_ref;

	root->kf_root = kernfs_create_root(&cgroup_kf_syscall_ops,
					   KERNFS_ROOT_CREATE_DEACTIVATED,
					   root_cgrp);
	if (IS_ERR(root->kf_root)) {
		ret = PTR_ERR(root->kf_root);
		goto exit_root_id;
	}
	root_cgrp->kn = root->kf_root->kn;

	if (root == &cgrp_dfl_root)
		base_files = cgroup_dfl_base_files;
	else
		base_files = cgroup_legacy_base_files;

	ret = cgroup_addrm_files(root_cgrp, base_files, true);
	if (ret)
		goto destroy_root;

	ret = rebind_subsystems(root, ss_mask);
	if (ret)
		goto destroy_root;

	/*
	 * There must be no failure case after here, since rebinding takes
	 * care of subsystems' refcounts, which are explicitly dropped in
	 * the failure exit path.
	 */
	list_add(&root->root_list, &cgroup_roots);
	cgroup_root_count++;

	/*
	 * Link the root cgroup in this hierarchy into all the css_set
	 * objects.
	 */
	down_write(&css_set_rwsem);
	hash_for_each(css_set_table, i, cset, hlist)
		link_css_set(&tmp_links, cset, root_cgrp);
	up_write(&css_set_rwsem);

	BUG_ON(!list_empty(&root_cgrp->self.children));
	BUG_ON(atomic_read(&root->nr_cgrps) != 1);

	kernfs_activate(root_cgrp->kn);
	ret = 0;
	goto out;

destroy_root:
	kernfs_destroy_root(root->kf_root);
	root->kf_root = NULL;
exit_root_id:
	cgroup_exit_root_id(root);
cancel_ref:
	percpu_ref_exit(&root_cgrp->self.refcnt);
out:
	free_cgrp_cset_links(&tmp_links);
	return ret;
>>>>>>> v3.18
}

static struct dentry *cgroup_mount(struct file_system_type *fs_type,
			 int flags, const char *unused_dev_name,
			 void *data)
{
<<<<<<< HEAD
	struct cgroup_sb_opts opts;
	struct cgroupfs_root *root;
	int ret = 0;
	struct super_block *sb;
	struct cgroupfs_root *new_root;
	struct inode *inode;

	/* First find the desired set of subsystems */
	mutex_lock(&cgroup_mutex);
	ret = parse_cgroupfs_options(data, &opts);
	mutex_unlock(&cgroup_mutex);
	if (ret)
		goto out_err;

	/*
	 * Allocate a new cgroup root. We may not need it if we're
	 * reusing an existing hierarchy.
	 */
	new_root = cgroup_root_from_opts(&opts);
	if (IS_ERR(new_root)) {
		ret = PTR_ERR(new_root);
		goto drop_modules;
	}
	opts.new_root = new_root;

	/* Locate an existing or new sb for this hierarchy */
	sb = sget(fs_type, cgroup_test_super, cgroup_set_super, 0, &opts);
	if (IS_ERR(sb)) {
		ret = PTR_ERR(sb);
		cgroup_drop_root(opts.new_root);
		goto drop_modules;
	}

	root = sb->s_fs_info;
	BUG_ON(!root);
	if (root == opts.new_root) {
		/* We used the new root structure, so this is a new hierarchy */
		struct list_head tmp_cg_links;
		struct cgroup *root_cgrp = &root->top_cgroup;
		struct cgroupfs_root *existing_root;
		const struct cred *cred;
		int i;
		struct css_set *cg;

		BUG_ON(sb->s_root != NULL);

		ret = cgroup_get_rootdir(sb);
		if (ret)
			goto drop_new_super;
		inode = sb->s_root->d_inode;

		mutex_lock(&inode->i_mutex);
		mutex_lock(&cgroup_mutex);
		mutex_lock(&cgroup_root_mutex);

		/* Check for name clashes with existing mounts */
		ret = -EBUSY;
		if (strlen(root->name))
			for_each_active_root(existing_root)
				if (!strcmp(existing_root->name, root->name))
					goto unlock_drop;

		/*
		 * We're accessing css_set_count without locking
		 * css_set_lock here, but that's OK - it can only be
		 * increased by someone holding cgroup_lock, and
		 * that's us. The worst that can happen is that we
		 * have some link structures left over
		 */
		ret = allocate_cg_links(css_set_count, &tmp_cg_links);
		if (ret)
			goto unlock_drop;

		ret = rebind_subsystems(root, root->subsys_mask);
		if (ret == -EBUSY) {
			free_cg_links(&tmp_cg_links);
			goto unlock_drop;
		}
		/*
		 * There must be no failure case after here, since rebinding
		 * takes care of subsystems' refcounts, which are explicitly
		 * dropped in the failure exit path.
		 */

		/* EBUSY should be the only error here */
		BUG_ON(ret);

		list_add(&root->root_list, &roots);
		root_count++;

		sb->s_root->d_fsdata = root_cgrp;
		root->top_cgroup.dentry = sb->s_root;

		/* Link the top cgroup in this hierarchy into all
		 * the css_set objects */
		write_lock(&css_set_lock);
		hash_for_each(css_set_table, i, cg, hlist)
			link_css_set(&tmp_cg_links, cg, root_cgrp);
		write_unlock(&css_set_lock);

		free_cg_links(&tmp_cg_links);

		BUG_ON(!list_empty(&root_cgrp->children));
		BUG_ON(root->number_of_cgroups != 1);

		cred = override_creds(&init_cred);
		cgroup_populate_dir(root_cgrp, true, root->subsys_mask);
		revert_creds(cred);
		mutex_unlock(&cgroup_root_mutex);
		mutex_unlock(&cgroup_mutex);
		mutex_unlock(&inode->i_mutex);
	} else {
		/*
		 * We re-used an existing hierarchy - the new root (if
		 * any) is not needed
		 */
		cgroup_drop_root(opts.new_root);

		if (root->flags != opts.flags) {
			if ((root->flags | opts.flags) & CGRP_ROOT_SANE_BEHAVIOR) {
				pr_err("cgroup: sane_behavior: new mount options should match the existing superblock\n");
				ret = -EINVAL;
				goto drop_new_super;
			} else {
				pr_warning("cgroup: new mount options do not match the existing superblock, will be ignored\n");
			}
		}

		/* no subsys rebinding, so refcounts don't change */
		drop_parsed_module_refcounts(opts.subsys_mask);
	}

	kfree(opts.release_agent);
	kfree(opts.name);
	return dget(sb->s_root);

 unlock_drop:
	mutex_unlock(&cgroup_root_mutex);
	mutex_unlock(&cgroup_mutex);
	mutex_unlock(&inode->i_mutex);
 drop_new_super:
	deactivate_locked_super(sb);
 drop_modules:
	drop_parsed_module_refcounts(opts.subsys_mask);
 out_err:
	kfree(opts.release_agent);
	kfree(opts.name);
	return ERR_PTR(ret);
}

static void cgroup_kill_sb(struct super_block *sb) {
	struct cgroupfs_root *root = sb->s_fs_info;
	struct cgroup *cgrp = &root->top_cgroup;
	int ret;
	struct cg_cgroup_link *link;
	struct cg_cgroup_link *saved_link;

	BUG_ON(!root);

	BUG_ON(root->number_of_cgroups != 1);
	BUG_ON(!list_empty(&cgrp->children));

	mutex_lock(&cgroup_mutex);
	mutex_lock(&cgroup_root_mutex);

	/* Rebind all subsystems back to the default hierarchy */
	ret = rebind_subsystems(root, 0);
	/* Shouldn't be able to fail ... */
	BUG_ON(ret);

	/*
	 * Release all the links from css_sets to this hierarchy's
	 * root cgroup
	 */
	write_lock(&css_set_lock);

	list_for_each_entry_safe(link, saved_link, &cgrp->css_sets,
				 cgrp_link_list) {
		list_del(&link->cg_link_list);
		list_del(&link->cgrp_link_list);
		kfree(link);
	}
	write_unlock(&css_set_lock);

	if (!list_empty(&root->root_list)) {
		list_del(&root->root_list);
		root_count--;
	}

	mutex_unlock(&cgroup_root_mutex);
	mutex_unlock(&cgroup_mutex);

	simple_xattrs_free(&cgrp->xattrs);

	kill_litter_super(sb);
	cgroup_drop_root(root);
=======
	struct super_block *pinned_sb = NULL;
	struct cgroup_subsys *ss;
	struct cgroup_root *root;
	struct cgroup_sb_opts opts;
	struct dentry *dentry;
	int ret;
	int i;
	bool new_sb;

	/*
	 * The first time anyone tries to mount a cgroup, enable the list
	 * linking each css_set to its tasks and fix up all existing tasks.
	 */
	if (!use_task_css_set_links)
		cgroup_enable_task_cg_lists();

	mutex_lock(&cgroup_mutex);

	/* First find the desired set of subsystems */
	ret = parse_cgroupfs_options(data, &opts);
	if (ret)
		goto out_unlock;

	/* look for a matching existing root */
	if (opts.flags & CGRP_ROOT_SANE_BEHAVIOR) {
		cgrp_dfl_root_visible = true;
		root = &cgrp_dfl_root;
		cgroup_get(&root->cgrp);
		ret = 0;
		goto out_unlock;
	}

	/*
	 * Destruction of cgroup root is asynchronous, so subsystems may
	 * still be dying after the previous unmount.  Let's drain the
	 * dying subsystems.  We just need to ensure that the ones
	 * unmounted previously finish dying and don't care about new ones
	 * starting.  Testing ref liveliness is good enough.
	 */
	for_each_subsys(ss, i) {
		if (!(opts.subsys_mask & (1 << i)) ||
		    ss->root == &cgrp_dfl_root)
			continue;

		if (!percpu_ref_tryget_live(&ss->root->cgrp.self.refcnt)) {
			mutex_unlock(&cgroup_mutex);
			msleep(10);
			ret = restart_syscall();
			goto out_free;
		}
		cgroup_put(&ss->root->cgrp);
	}

	for_each_root(root) {
		bool name_match = false;

		if (root == &cgrp_dfl_root)
			continue;

		/*
		 * If we asked for a name then it must match.  Also, if
		 * name matches but sybsys_mask doesn't, we should fail.
		 * Remember whether name matched.
		 */
		if (opts.name) {
			if (strcmp(opts.name, root->name))
				continue;
			name_match = true;
		}

		/*
		 * If we asked for subsystems (or explicitly for no
		 * subsystems) then they must match.
		 */
		if ((opts.subsys_mask || opts.none) &&
		    (opts.subsys_mask != root->subsys_mask)) {
			if (!name_match)
				continue;
			ret = -EBUSY;
			goto out_unlock;
		}

		if (root->flags ^ opts.flags)
			pr_warn("new mount options do not match the existing superblock, will be ignored\n");

		/*
		 * We want to reuse @root whose lifetime is governed by its
		 * ->cgrp.  Let's check whether @root is alive and keep it
		 * that way.  As cgroup_kill_sb() can happen anytime, we
		 * want to block it by pinning the sb so that @root doesn't
		 * get killed before mount is complete.
		 *
		 * With the sb pinned, tryget_live can reliably indicate
		 * whether @root can be reused.  If it's being killed,
		 * drain it.  We can use wait_queue for the wait but this
		 * path is super cold.  Let's just sleep a bit and retry.
		 */
		pinned_sb = kernfs_pin_sb(root->kf_root, NULL);
		if (IS_ERR(pinned_sb) ||
		    !percpu_ref_tryget_live(&root->cgrp.self.refcnt)) {
			mutex_unlock(&cgroup_mutex);
			if (!IS_ERR_OR_NULL(pinned_sb))
				deactivate_super(pinned_sb);
			msleep(10);
			ret = restart_syscall();
			goto out_free;
		}

		ret = 0;
		goto out_unlock;
	}

	/*
	 * No such thing, create a new one.  name= matching without subsys
	 * specification is allowed for already existing hierarchies but we
	 * can't create new one without subsys specification.
	 */
	if (!opts.subsys_mask && !opts.none) {
		ret = -EINVAL;
		goto out_unlock;
	}

	root = kzalloc(sizeof(*root), GFP_KERNEL);
	if (!root) {
		ret = -ENOMEM;
		goto out_unlock;
	}

	init_cgroup_root(root, &opts);

	ret = cgroup_setup_root(root, opts.subsys_mask);
	if (ret)
		cgroup_free_root(root);

out_unlock:
	mutex_unlock(&cgroup_mutex);
out_free:
	kfree(opts.release_agent);
	kfree(opts.name);

	if (ret)
		return ERR_PTR(ret);

	dentry = kernfs_mount(fs_type, flags, root->kf_root,
				CGROUP_SUPER_MAGIC, &new_sb);
	if (IS_ERR(dentry) || !new_sb)
		cgroup_put(&root->cgrp);

	/*
	 * If @pinned_sb, we're reusing an existing root and holding an
	 * extra ref on its sb.  Mount is complete.  Put the extra ref.
	 */
	if (pinned_sb) {
		WARN_ON(new_sb);
		deactivate_super(pinned_sb);
	}

	return dentry;
}

static void cgroup_kill_sb(struct super_block *sb)
{
	struct kernfs_root *kf_root = kernfs_root_from_sb(sb);
	struct cgroup_root *root = cgroup_root_from_kf(kf_root);

	/*
	 * If @root doesn't have any mounts or children, start killing it.
	 * This prevents new mounts by disabling percpu_ref_tryget_live().
	 * cgroup_mount() may wait for @root's release.
	 *
	 * And don't kill the default root.
	 */
	if (css_has_online_children(&root->cgrp.self) ||
	    root == &cgrp_dfl_root)
		cgroup_put(&root->cgrp);
	else
		percpu_ref_kill(&root->cgrp.self.refcnt);

	kernfs_kill_sb(sb);
>>>>>>> v3.18
}

static struct file_system_type cgroup_fs_type = {
	.name = "cgroup",
	.mount = cgroup_mount,
	.kill_sb = cgroup_kill_sb,
};

static struct kobject *cgroup_kobj;

/**
<<<<<<< HEAD
 * cgroup_path - generate the path of a cgroup
 * @cgrp: the cgroup in question
 * @buf: the buffer to write the path into
 * @buflen: the length of the buffer
 *
 * Writes path of cgroup into buf.  Returns 0 on success, -errno on error.
 *
 * We can't generate cgroup path using dentry->d_name, as accessing
 * dentry->name must be protected by irq-unsafe dentry->d_lock or parent
 * inode's i_mutex, while on the other hand cgroup_path() can be called
 * with some irq-safe spinlocks held.
 */
int cgroup_path(const struct cgroup *cgrp, char *buf, int buflen)
{
	int ret = -ENAMETOOLONG;
	char *start;

	if (!cgrp->parent) {
		if (strlcpy(buf, "/", buflen) >= buflen)
			return -ENAMETOOLONG;
		return 0;
	}

	start = buf + buflen - 1;
	*start = '\0';

	rcu_read_lock();
	do {
		const char *name = cgroup_name(cgrp);
		int len;

		len = strlen(name);
		if ((start -= len) < buf)
			goto out;
		memcpy(start, name, len);

		if (--start < buf)
			goto out;
		*start = '/';

		cgrp = cgrp->parent;
	} while (cgrp->parent);
	ret = 0;
	memmove(buf, start, buf + buflen - start);
out:
	rcu_read_unlock();
	return ret;
}
EXPORT_SYMBOL_GPL(cgroup_path);

/*
 * Control Group taskset
 */
struct task_and_cgroup {
	struct task_struct	*task;
	struct cgroup		*cgrp;
	struct css_set		*cg;
};

struct cgroup_taskset {
	struct task_and_cgroup	single;
	struct flex_array	*tc_array;
	int			tc_array_len;
	int			idx;
	struct cgroup		*cur_cgrp;
=======
 * task_cgroup_path - cgroup path of a task in the first cgroup hierarchy
 * @task: target task
 * @buf: the buffer to write the path into
 * @buflen: the length of the buffer
 *
 * Determine @task's cgroup on the first (the one with the lowest non-zero
 * hierarchy_id) cgroup hierarchy and copy its path into @buf.  This
 * function grabs cgroup_mutex and shouldn't be used inside locks used by
 * cgroup controller callbacks.
 *
 * Return value is the same as kernfs_path().
 */
char *task_cgroup_path(struct task_struct *task, char *buf, size_t buflen)
{
	struct cgroup_root *root;
	struct cgroup *cgrp;
	int hierarchy_id = 1;
	char *path = NULL;

	mutex_lock(&cgroup_mutex);
	down_read(&css_set_rwsem);

	root = idr_get_next(&cgroup_hierarchy_idr, &hierarchy_id);

	if (root) {
		cgrp = task_cgroup_from_root(task, root);
		path = cgroup_path(cgrp, buf, buflen);
	} else {
		/* if no hierarchy exists, everyone is in "/" */
		if (strlcpy(buf, "/", buflen) < buflen)
			path = buf;
	}

	up_read(&css_set_rwsem);
	mutex_unlock(&cgroup_mutex);
	return path;
}
EXPORT_SYMBOL_GPL(task_cgroup_path);

/* used to track tasks and other necessary states during migration */
struct cgroup_taskset {
	/* the src and dst cset list running through cset->mg_node */
	struct list_head	src_csets;
	struct list_head	dst_csets;

	/*
	 * Fields for cgroup_taskset_*() iteration.
	 *
	 * Before migration is committed, the target migration tasks are on
	 * ->mg_tasks of the csets on ->src_csets.  After, on ->mg_tasks of
	 * the csets on ->dst_csets.  ->csets point to either ->src_csets
	 * or ->dst_csets depending on whether migration is committed.
	 *
	 * ->cur_csets and ->cur_task point to the current task position
	 * during iteration.
	 */
	struct list_head	*csets;
	struct css_set		*cur_cset;
	struct task_struct	*cur_task;
>>>>>>> v3.18
};

/**
 * cgroup_taskset_first - reset taskset and return the first task
 * @tset: taskset of interest
 *
 * @tset iteration is initialized and the first task is returned.
 */
struct task_struct *cgroup_taskset_first(struct cgroup_taskset *tset)
{
<<<<<<< HEAD
	if (tset->tc_array) {
		tset->idx = 0;
		return cgroup_taskset_next(tset);
	} else {
		tset->cur_cgrp = tset->single.cgrp;
		return tset->single.task;
	}
}
EXPORT_SYMBOL_GPL(cgroup_taskset_first);
=======
	tset->cur_cset = list_first_entry(tset->csets, struct css_set, mg_node);
	tset->cur_task = NULL;

	return cgroup_taskset_next(tset);
}
>>>>>>> v3.18

/**
 * cgroup_taskset_next - iterate to the next task in taskset
 * @tset: taskset of interest
 *
 * Return the next task in @tset.  Iteration must have been initialized
 * with cgroup_taskset_first().
 */
struct task_struct *cgroup_taskset_next(struct cgroup_taskset *tset)
{
<<<<<<< HEAD
	struct task_and_cgroup *tc;

	if (!tset->tc_array || tset->idx >= tset->tc_array_len)
		return NULL;

	tc = flex_array_get(tset->tc_array, tset->idx++);
	tset->cur_cgrp = tc->cgrp;
	return tc->task;
}
EXPORT_SYMBOL_GPL(cgroup_taskset_next);

/**
 * cgroup_taskset_cur_cgroup - return the matching cgroup for the current task
 * @tset: taskset of interest
 *
 * Return the cgroup for the current (last returned) task of @tset.  This
 * function must be preceded by either cgroup_taskset_first() or
 * cgroup_taskset_next().
 */
struct cgroup *cgroup_taskset_cur_cgroup(struct cgroup_taskset *tset)
{
	return tset->cur_cgrp;
}
EXPORT_SYMBOL_GPL(cgroup_taskset_cur_cgroup);

/**
 * cgroup_taskset_size - return the number of tasks in taskset
 * @tset: taskset of interest
 */
int cgroup_taskset_size(struct cgroup_taskset *tset)
{
	return tset->tc_array ? tset->tc_array_len : 1;
}
EXPORT_SYMBOL_GPL(cgroup_taskset_size);


/*
 * cgroup_task_migrate - move a task from one cgroup to another.
 *
 * Must be called with cgroup_mutex and threadgroup locked.
 */
static void cgroup_task_migrate(struct cgroup *oldcgrp,
				struct task_struct *tsk, struct css_set *newcg)
{
	struct css_set *oldcg;
=======
	struct css_set *cset = tset->cur_cset;
	struct task_struct *task = tset->cur_task;

	while (&cset->mg_node != tset->csets) {
		if (!task)
			task = list_first_entry(&cset->mg_tasks,
						struct task_struct, cg_list);
		else
			task = list_next_entry(task, cg_list);

		if (&task->cg_list != &cset->mg_tasks) {
			tset->cur_cset = cset;
			tset->cur_task = task;
			return task;
		}

		cset = list_next_entry(cset, mg_node);
		task = NULL;
	}

	return NULL;
}

/**
 * cgroup_task_migrate - move a task from one cgroup to another.
 * @old_cgrp: the cgroup @tsk is being migrated from
 * @tsk: the task being migrated
 * @new_cset: the new css_set @tsk is being attached to
 *
 * Must be called with cgroup_mutex, threadgroup and css_set_rwsem locked.
 */
static void cgroup_task_migrate(struct cgroup *old_cgrp,
				struct task_struct *tsk,
				struct css_set *new_cset)
{
	struct css_set *old_cset;

	lockdep_assert_held(&cgroup_mutex);
	lockdep_assert_held(&css_set_rwsem);
>>>>>>> v3.18

	/*
	 * We are synchronized through threadgroup_lock() against PF_EXITING
	 * setting such that we can't race against cgroup_exit() changing the
	 * css_set to init_css_set and dropping the old one.
	 */
	WARN_ON_ONCE(tsk->flags & PF_EXITING);
<<<<<<< HEAD
	oldcg = tsk->cgroups;

	task_lock(tsk);
	rcu_assign_pointer(tsk->cgroups, newcg);
	task_unlock(tsk);

	/* Update the css_set linked lists if we're using them */
	write_lock(&css_set_lock);
	if (!list_empty(&tsk->cg_list))
		list_move(&tsk->cg_list, &newcg->tasks);
	write_unlock(&css_set_lock);

	/*
	 * We just gained a reference on oldcg by taking it from the task. As
	 * trading it for newcg is protected by cgroup_mutex, we're safe to drop
	 * it here; it will be freed under RCU.
	 */
	set_bit(CGRP_RELEASABLE, &oldcgrp->flags);
	put_css_set(oldcg);
}

/**
 * cgroup_attach_task - attach a task or a whole threadgroup to a cgroup
 * @cgrp: the cgroup to attach to
 * @tsk: the task or the leader of the threadgroup to be attached
 * @threadgroup: attach the whole threadgroup?
 *
 * Call holding cgroup_mutex and the group_rwsem of the leader. Will take
 * task_lock of @tsk or each thread in the threadgroup individually in turn.
 */
static int cgroup_attach_task(struct cgroup *cgrp, struct task_struct *tsk,
			      bool threadgroup)
{
	int retval, i, group_size;
	struct cgroup_subsys *ss, *failed_ss = NULL;
	struct cgroupfs_root *root = cgrp->root;
	/* threadgroup list cursor and array */
	struct task_struct *leader = tsk;
	struct task_and_cgroup *tc;
	struct flex_array *group;
	struct cgroup_taskset tset = { };

	/*
	 * step 0: in order to do expensive, possibly blocking operations for
	 * every thread, we cannot iterate the thread group list, since it needs
	 * rcu or tasklist locked. instead, build an array of all threads in the
	 * group - group_rwsem prevents new threads from appearing, and if
	 * threads exit, this will just be an over-estimate.
	 */
	if (threadgroup)
		group_size = get_nr_threads(tsk);
	else
		group_size = 1;
	/* flex_array supports very large thread-groups better than kmalloc. */
	group = flex_array_alloc(sizeof(*tc), group_size, GFP_KERNEL);
	if (!group)
		return -ENOMEM;
	/* pre-allocate to guarantee space while iterating in rcu read-side. */
	retval = flex_array_prealloc(group, 0, group_size, GFP_KERNEL);
	if (retval)
		goto out_free_group_list;

	i = 0;
=======
	old_cset = task_css_set(tsk);

	get_css_set(new_cset);
	rcu_assign_pointer(tsk->cgroups, new_cset);

	/*
	 * Use move_tail so that cgroup_taskset_first() still returns the
	 * leader after migration.  This works because cgroup_migrate()
	 * ensures that the dst_cset of the leader is the first on the
	 * tset's dst_csets list.
	 */
	list_move_tail(&tsk->cg_list, &new_cset->mg_tasks);

	/*
	 * We just gained a reference on old_cset by taking it from the
	 * task. As trading it for new_cset is protected by cgroup_mutex,
	 * we're safe to drop it here; it will be freed under RCU.
	 */
	put_css_set_locked(old_cset);
}

/**
 * cgroup_migrate_finish - cleanup after attach
 * @preloaded_csets: list of preloaded css_sets
 *
 * Undo cgroup_migrate_add_src() and cgroup_migrate_prepare_dst().  See
 * those functions for details.
 */
static void cgroup_migrate_finish(struct list_head *preloaded_csets)
{
	struct css_set *cset, *tmp_cset;

	lockdep_assert_held(&cgroup_mutex);

	down_write(&css_set_rwsem);
	list_for_each_entry_safe(cset, tmp_cset, preloaded_csets, mg_preload_node) {
		cset->mg_src_cgrp = NULL;
		cset->mg_dst_cset = NULL;
		list_del_init(&cset->mg_preload_node);
		put_css_set_locked(cset);
	}
	up_write(&css_set_rwsem);
}

/**
 * cgroup_migrate_add_src - add a migration source css_set
 * @src_cset: the source css_set to add
 * @dst_cgrp: the destination cgroup
 * @preloaded_csets: list of preloaded css_sets
 *
 * Tasks belonging to @src_cset are about to be migrated to @dst_cgrp.  Pin
 * @src_cset and add it to @preloaded_csets, which should later be cleaned
 * up by cgroup_migrate_finish().
 *
 * This function may be called without holding threadgroup_lock even if the
 * target is a process.  Threads may be created and destroyed but as long
 * as cgroup_mutex is not dropped, no new css_set can be put into play and
 * the preloaded css_sets are guaranteed to cover all migrations.
 */
static void cgroup_migrate_add_src(struct css_set *src_cset,
				   struct cgroup *dst_cgrp,
				   struct list_head *preloaded_csets)
{
	struct cgroup *src_cgrp;

	lockdep_assert_held(&cgroup_mutex);
	lockdep_assert_held(&css_set_rwsem);

	src_cgrp = cset_cgroup_from_root(src_cset, dst_cgrp->root);

	if (!list_empty(&src_cset->mg_preload_node))
		return;

	WARN_ON(src_cset->mg_src_cgrp);
	WARN_ON(!list_empty(&src_cset->mg_tasks));
	WARN_ON(!list_empty(&src_cset->mg_node));

	src_cset->mg_src_cgrp = src_cgrp;
	get_css_set(src_cset);
	list_add(&src_cset->mg_preload_node, preloaded_csets);
}

/**
 * cgroup_migrate_prepare_dst - prepare destination css_sets for migration
 * @dst_cgrp: the destination cgroup (may be %NULL)
 * @preloaded_csets: list of preloaded source css_sets
 *
 * Tasks are about to be moved to @dst_cgrp and all the source css_sets
 * have been preloaded to @preloaded_csets.  This function looks up and
 * pins all destination css_sets, links each to its source, and append them
 * to @preloaded_csets.  If @dst_cgrp is %NULL, the destination of each
 * source css_set is assumed to be its cgroup on the default hierarchy.
 *
 * This function must be called after cgroup_migrate_add_src() has been
 * called on each migration source css_set.  After migration is performed
 * using cgroup_migrate(), cgroup_migrate_finish() must be called on
 * @preloaded_csets.
 */
static int cgroup_migrate_prepare_dst(struct cgroup *dst_cgrp,
				      struct list_head *preloaded_csets)
{
	LIST_HEAD(csets);
	struct css_set *src_cset, *tmp_cset;

	lockdep_assert_held(&cgroup_mutex);

	/*
	 * Except for the root, child_subsys_mask must be zero for a cgroup
	 * with tasks so that child cgroups don't compete against tasks.
	 */
	if (dst_cgrp && cgroup_on_dfl(dst_cgrp) && cgroup_parent(dst_cgrp) &&
	    dst_cgrp->child_subsys_mask)
		return -EBUSY;

	/* look up the dst cset for each src cset and link it to src */
	list_for_each_entry_safe(src_cset, tmp_cset, preloaded_csets, mg_preload_node) {
		struct css_set *dst_cset;

		dst_cset = find_css_set(src_cset,
					dst_cgrp ?: src_cset->dfl_cgrp);
		if (!dst_cset)
			goto err;

		WARN_ON_ONCE(src_cset->mg_dst_cset || dst_cset->mg_dst_cset);

		/*
		 * If src cset equals dst, it's noop.  Drop the src.
		 * cgroup_migrate() will skip the cset too.  Note that we
		 * can't handle src == dst as some nodes are used by both.
		 */
		if (src_cset == dst_cset) {
			src_cset->mg_src_cgrp = NULL;
			list_del_init(&src_cset->mg_preload_node);
			put_css_set(src_cset);
			put_css_set(dst_cset);
			continue;
		}

		src_cset->mg_dst_cset = dst_cset;

		if (list_empty(&dst_cset->mg_preload_node))
			list_add(&dst_cset->mg_preload_node, &csets);
		else
			put_css_set(dst_cset);
	}

	list_splice_tail(&csets, preloaded_csets);
	return 0;
err:
	cgroup_migrate_finish(&csets);
	return -ENOMEM;
}

/**
 * cgroup_migrate - migrate a process or task to a cgroup
 * @cgrp: the destination cgroup
 * @leader: the leader of the process or the task to migrate
 * @threadgroup: whether @leader points to the whole process or a single task
 *
 * Migrate a process or task denoted by @leader to @cgrp.  If migrating a
 * process, the caller must be holding threadgroup_lock of @leader.  The
 * caller is also responsible for invoking cgroup_migrate_add_src() and
 * cgroup_migrate_prepare_dst() on the targets before invoking this
 * function and following up with cgroup_migrate_finish().
 *
 * As long as a controller's ->can_attach() doesn't fail, this function is
 * guaranteed to succeed.  This means that, excluding ->can_attach()
 * failure, when migrating multiple targets, the success or failure can be
 * decided for all targets by invoking group_migrate_prepare_dst() before
 * actually starting migrating.
 */
static int cgroup_migrate(struct cgroup *cgrp, struct task_struct *leader,
			  bool threadgroup)
{
	struct cgroup_taskset tset = {
		.src_csets	= LIST_HEAD_INIT(tset.src_csets),
		.dst_csets	= LIST_HEAD_INIT(tset.dst_csets),
		.csets		= &tset.src_csets,
	};
	struct cgroup_subsys_state *css, *failed_css = NULL;
	struct css_set *cset, *tmp_cset;
	struct task_struct *task, *tmp_task;
	int i, ret;

>>>>>>> v3.18
	/*
	 * Prevent freeing of tasks while we take a snapshot. Tasks that are
	 * already PF_EXITING could be freed from underneath us unless we
	 * take an rcu_read_lock.
	 */
<<<<<<< HEAD
	rcu_read_lock();
	do {
		struct task_and_cgroup ent;

		/* @tsk either already exited or can't exit until the end */
		if (tsk->flags & PF_EXITING)
			goto next;

		/* as per above, nr_threads may decrease, but not increase. */
		BUG_ON(i >= group_size);
		ent.task = tsk;
		ent.cgrp = task_cgroup_from_root(tsk, root);
		/* nothing to do if this task is already in the cgroup */
		if (ent.cgrp == cgrp)
			goto next;
		/*
		 * saying GFP_ATOMIC has no effect here because we did prealloc
		 * earlier, but it's good form to communicate our expectations.
		 */
		retval = flex_array_put(group, i, &ent, GFP_ATOMIC);
		BUG_ON(retval != 0);
		i++;
next:
		if (!threadgroup)
			break;
	} while_each_thread(leader, tsk);
	rcu_read_unlock();
	/* remember the number of threads in the array for later. */
	group_size = i;
	tset.tc_array = group;
	tset.tc_array_len = group_size;

	/* methods shouldn't be called if no task is actually migrating */
	retval = 0;
	if (!group_size)
		goto out_free_group_list;

	/*
	 * step 1: check that we can legitimately attach to the cgroup.
	 */
	for_each_subsys(root, ss) {
		if (ss->can_attach) {
			retval = ss->can_attach(cgrp, &tset);
			if (retval) {
				failed_ss = ss;
=======
	down_write(&css_set_rwsem);
	rcu_read_lock();
	task = leader;
	do {
		/* @task either already exited or can't exit until the end */
		if (task->flags & PF_EXITING)
			goto next;

		/* leave @task alone if post_fork() hasn't linked it yet */
		if (list_empty(&task->cg_list))
			goto next;

		cset = task_css_set(task);
		if (!cset->mg_src_cgrp)
			goto next;

		/*
		 * cgroup_taskset_first() must always return the leader.
		 * Take care to avoid disturbing the ordering.
		 */
		list_move_tail(&task->cg_list, &cset->mg_tasks);
		if (list_empty(&cset->mg_node))
			list_add_tail(&cset->mg_node, &tset.src_csets);
		if (list_empty(&cset->mg_dst_cset->mg_node))
			list_move_tail(&cset->mg_dst_cset->mg_node,
				       &tset.dst_csets);
	next:
		if (!threadgroup)
			break;
	} while_each_thread(leader, task);
	rcu_read_unlock();
	up_write(&css_set_rwsem);

	/* methods shouldn't be called if no task is actually migrating */
	if (list_empty(&tset.src_csets))
		return 0;

	/* check that we can legitimately attach to the cgroup */
	for_each_e_css(css, i, cgrp) {
		if (css->ss->can_attach) {
			ret = css->ss->can_attach(css, &tset);
			if (ret) {
				failed_css = css;
>>>>>>> v3.18
				goto out_cancel_attach;
			}
		}
	}

	/*
<<<<<<< HEAD
	 * step 2: make sure css_sets exist for all threads to be migrated.
	 * we use find_css_set, which allocates a new one if necessary.
	 */
	for (i = 0; i < group_size; i++) {
		tc = flex_array_get(group, i);
		tc->cg = find_css_set(tc->task->cgroups, cgrp);
		if (!tc->cg) {
			retval = -ENOMEM;
			goto out_put_css_set_refs;
		}
	}

	/*
	 * step 3: now that we're guaranteed success wrt the css_sets,
	 * proceed to move all tasks to the new cgroup.  There are no
	 * failure cases after here, so this is the commit point.
	 */
	for (i = 0; i < group_size; i++) {
		tc = flex_array_get(group, i);
		cgroup_task_migrate(tc->cgrp, tc->task, tc->cg);
	}
	/* nothing is sensitive to fork() after this point. */

	/*
	 * step 4: do subsystem attach callbacks.
	 */
	for_each_subsys(root, ss) {
		if (ss->attach)
			ss->attach(cgrp, &tset);
	}

	/*
	 * step 5: success! and cleanup
	 */
	retval = 0;
out_put_css_set_refs:
	if (retval) {
		for (i = 0; i < group_size; i++) {
			tc = flex_array_get(group, i);
			if (!tc->cg)
				break;
			put_css_set(tc->cg);
		}
	}
out_cancel_attach:
	if (retval) {
		for_each_subsys(root, ss) {
			if (ss == failed_ss)
				break;
			if (ss->cancel_attach)
				ss->cancel_attach(cgrp, &tset);
		}
	}
out_free_group_list:
	flex_array_free(group);
	return retval;
}

static int cgroup_allow_attach(struct cgroup *cgrp, struct cgroup_taskset *tset)
{
	struct cgroup_subsys *ss;
	int ret;

	for_each_subsys(cgrp->root, ss) {
		if (ss->allow_attach) {
			ret = ss->allow_attach(cgrp, tset);
			if (ret)
				return ret;
		} else {
			return -EACCES;
		}
	}

	return 0;
}

int subsys_cgroup_allow_attach(struct cgroup *cgrp, struct cgroup_taskset *tset)
{
	const struct cred *cred = current_cred(), *tcred;
	struct task_struct *task;

	if (capable(CAP_SYS_NICE))
		return 0;

	cgroup_taskset_for_each(task, cgrp, tset) {
		tcred = __task_cred(task);

		if (current != task && cred->euid != tcred->uid &&
		    cred->euid != tcred->suid)
			return -EACCES;
	}

	return 0;
=======
	 * Now that we're guaranteed success, proceed to move all tasks to
	 * the new cgroup.  There are no failure cases after here, so this
	 * is the commit point.
	 */
	down_write(&css_set_rwsem);
	list_for_each_entry(cset, &tset.src_csets, mg_node) {
		list_for_each_entry_safe(task, tmp_task, &cset->mg_tasks, cg_list)
			cgroup_task_migrate(cset->mg_src_cgrp, task,
					    cset->mg_dst_cset);
	}
	up_write(&css_set_rwsem);

	/*
	 * Migration is committed, all target tasks are now on dst_csets.
	 * Nothing is sensitive to fork() after this point.  Notify
	 * controllers that migration is complete.
	 */
	tset.csets = &tset.dst_csets;

	for_each_e_css(css, i, cgrp)
		if (css->ss->attach)
			css->ss->attach(css, &tset);

	ret = 0;
	goto out_release_tset;

out_cancel_attach:
	for_each_e_css(css, i, cgrp) {
		if (css == failed_css)
			break;
		if (css->ss->cancel_attach)
			css->ss->cancel_attach(css, &tset);
	}
out_release_tset:
	down_write(&css_set_rwsem);
	list_splice_init(&tset.dst_csets, &tset.src_csets);
	list_for_each_entry_safe(cset, tmp_cset, &tset.src_csets, mg_node) {
		list_splice_tail_init(&cset->mg_tasks, &cset->tasks);
		list_del_init(&cset->mg_node);
	}
	up_write(&css_set_rwsem);
	return ret;
}

/**
 * cgroup_attach_task - attach a task or a whole threadgroup to a cgroup
 * @dst_cgrp: the cgroup to attach to
 * @leader: the task or the leader of the threadgroup to be attached
 * @threadgroup: attach the whole threadgroup?
 *
 * Call holding cgroup_mutex and threadgroup_lock of @leader.
 */
static int cgroup_attach_task(struct cgroup *dst_cgrp,
			      struct task_struct *leader, bool threadgroup)
{
	LIST_HEAD(preloaded_csets);
	struct task_struct *task;
	int ret;

	/* look up all src csets */
	down_read(&css_set_rwsem);
	rcu_read_lock();
	task = leader;
	do {
		cgroup_migrate_add_src(task_css_set(task), dst_cgrp,
				       &preloaded_csets);
		if (!threadgroup)
			break;
	} while_each_thread(leader, task);
	rcu_read_unlock();
	up_read(&css_set_rwsem);

	/* prepare dst csets and commit */
	ret = cgroup_migrate_prepare_dst(dst_cgrp, &preloaded_csets);
	if (!ret)
		ret = cgroup_migrate(dst_cgrp, leader, threadgroup);

	cgroup_migrate_finish(&preloaded_csets);
	return ret;
>>>>>>> v3.18
}

/*
 * Find the task_struct of the task to attach by vpid and pass it along to the
 * function to attach either it or all tasks in its threadgroup. Will lock
<<<<<<< HEAD
 * cgroup_mutex and threadgroup; may take task_lock of task.
 */
static int attach_task_by_pid(struct cgroup *cgrp, u64 pid, bool threadgroup)
{
	struct task_struct *tsk;
	const struct cred *cred = current_cred(), *tcred;
	int ret;

	if (!cgroup_lock_live_group(cgrp))
=======
 * cgroup_mutex and threadgroup.
 */
static ssize_t __cgroup_procs_write(struct kernfs_open_file *of, char *buf,
				    size_t nbytes, loff_t off, bool threadgroup)
{
	struct task_struct *tsk;
	const struct cred *cred = current_cred(), *tcred;
	struct cgroup *cgrp;
	pid_t pid;
	int ret;

	if (kstrtoint(strstrip(buf), 0, &pid) || pid < 0)
		return -EINVAL;

	cgrp = cgroup_kn_lock_live(of->kn);
	if (!cgrp)
>>>>>>> v3.18
		return -ENODEV;

retry_find_task:
	rcu_read_lock();
	if (pid) {
		tsk = find_task_by_vpid(pid);
		if (!tsk) {
			rcu_read_unlock();
<<<<<<< HEAD
			ret= -ESRCH;
=======
			ret = -ESRCH;
>>>>>>> v3.18
			goto out_unlock_cgroup;
		}
		/*
		 * even if we're attaching all tasks in the thread group, we
		 * only need to check permissions on one of them.
		 */
		tcred = __task_cred(tsk);
		if (!uid_eq(cred->euid, GLOBAL_ROOT_UID) &&
		    !uid_eq(cred->euid, tcred->uid) &&
		    !uid_eq(cred->euid, tcred->suid)) {
<<<<<<< HEAD
			/*
			 * if the default permission check fails, give each
			 * cgroup a chance to extend the permission check
			 */
			struct cgroup_taskset tset = { };
			tset.single.task = tsk;
			tset.single.cgrp = cgrp;
			ret = cgroup_allow_attach(cgrp, &tset);
			if (ret) {
				rcu_read_unlock();
				goto out_unlock_cgroup;
			}
=======
			rcu_read_unlock();
			ret = -EACCES;
			goto out_unlock_cgroup;
>>>>>>> v3.18
		}
	} else
		tsk = current;

	if (threadgroup)
		tsk = tsk->group_leader;

	/*
	 * Workqueue threads may acquire PF_NO_SETAFFINITY and become
	 * trapped in a cpuset, or RT worker may be born in a cgroup
	 * with no rt_runtime allocated.  Just say no.
	 */
	if (tsk == kthreadd_task || (tsk->flags & PF_NO_SETAFFINITY)) {
		ret = -EINVAL;
		rcu_read_unlock();
		goto out_unlock_cgroup;
	}

	get_task_struct(tsk);
	rcu_read_unlock();

	threadgroup_lock(tsk);
	if (threadgroup) {
		if (!thread_group_leader(tsk)) {
			/*
			 * a race with de_thread from another thread's exec()
			 * may strip us of our leadership, if this happens,
			 * there is no choice but to throw this task away and
			 * try again; this is
			 * "double-double-toil-and-trouble-check locking".
			 */
			threadgroup_unlock(tsk);
			put_task_struct(tsk);
			goto retry_find_task;
		}
	}

	ret = cgroup_attach_task(cgrp, tsk, threadgroup);

	threadgroup_unlock(tsk);

	put_task_struct(tsk);
out_unlock_cgroup:
<<<<<<< HEAD
	mutex_unlock(&cgroup_mutex);
	return ret;
=======
	cgroup_kn_unlock(of->kn);
	return ret ?: nbytes;
>>>>>>> v3.18
}

/**
 * cgroup_attach_task_all - attach task 'tsk' to all cgroups of task 'from'
 * @from: attach to all cgroups of a given task
 * @tsk: the task to be attached
 */
int cgroup_attach_task_all(struct task_struct *from, struct task_struct *tsk)
{
<<<<<<< HEAD
	struct cgroupfs_root *root;
	int retval = 0;

	mutex_lock(&cgroup_mutex);
	for_each_active_root(root) {
		struct cgroup *from_cg = task_cgroup_from_root(from, root);

		retval = cgroup_attach_task(from_cg, tsk, false);
=======
	struct cgroup_root *root;
	int retval = 0;

	mutex_lock(&cgroup_mutex);
	for_each_root(root) {
		struct cgroup *from_cgrp;

		if (root == &cgrp_dfl_root)
			continue;

		down_read(&css_set_rwsem);
		from_cgrp = task_cgroup_from_root(from, root);
		up_read(&css_set_rwsem);

		retval = cgroup_attach_task(from_cgrp, tsk, false);
>>>>>>> v3.18
		if (retval)
			break;
	}
	mutex_unlock(&cgroup_mutex);

	return retval;
}
EXPORT_SYMBOL_GPL(cgroup_attach_task_all);

<<<<<<< HEAD
static int cgroup_tasks_write(struct cgroup *cgrp, struct cftype *cft, u64 pid)
{
	return attach_task_by_pid(cgrp, pid, false);
}

static int cgroup_procs_write(struct cgroup *cgrp, struct cftype *cft, u64 tgid)
{
	return attach_task_by_pid(cgrp, tgid, true);
}

static int cgroup_release_agent_write(struct cgroup *cgrp, struct cftype *cft,
				      const char *buffer)
{
	BUILD_BUG_ON(sizeof(cgrp->root->release_agent_path) < PATH_MAX);
	if (strlen(buffer) >= PATH_MAX)
		return -EINVAL;
	if (!cgroup_lock_live_group(cgrp))
		return -ENODEV;
	mutex_lock(&cgroup_root_mutex);
	strcpy(cgrp->root->release_agent_path, buffer);
	mutex_unlock(&cgroup_root_mutex);
	mutex_unlock(&cgroup_mutex);
	return 0;
}

static int cgroup_release_agent_show(struct cgroup *cgrp, struct cftype *cft,
				     struct seq_file *seq)
{
	if (!cgroup_lock_live_group(cgrp))
		return -ENODEV;
	seq_puts(seq, cgrp->root->release_agent_path);
	seq_putc(seq, '\n');
	mutex_unlock(&cgroup_mutex);
	return 0;
}

static int cgroup_sane_behavior_show(struct cgroup *cgrp, struct cftype *cft,
				     struct seq_file *seq)
{
	seq_printf(seq, "%d\n", cgroup_sane_behavior(cgrp));
	return 0;
}

/* A buffer size big enough for numbers or short strings */
#define CGROUP_LOCAL_BUFFER_SIZE 64

static ssize_t cgroup_write_X64(struct cgroup *cgrp, struct cftype *cft,
				struct file *file,
				const char __user *userbuf,
				size_t nbytes, loff_t *unused_ppos)
{
	char buffer[CGROUP_LOCAL_BUFFER_SIZE];
	int retval = 0;
	char *end;

	if (!nbytes)
		return -EINVAL;
	if (nbytes >= sizeof(buffer))
		return -E2BIG;
	if (copy_from_user(buffer, userbuf, nbytes))
		return -EFAULT;

	buffer[nbytes] = 0;     /* nul-terminate */
	if (cft->write_u64) {
		u64 val = simple_strtoull(strstrip(buffer), &end, 0);
		if (*end)
			return -EINVAL;
		retval = cft->write_u64(cgrp, cft, val);
	} else {
		s64 val = simple_strtoll(strstrip(buffer), &end, 0);
		if (*end)
			return -EINVAL;
		retval = cft->write_s64(cgrp, cft, val);
	}
	if (!retval)
		retval = nbytes;
	return retval;
}

static ssize_t cgroup_write_string(struct cgroup *cgrp, struct cftype *cft,
				   struct file *file,
				   const char __user *userbuf,
				   size_t nbytes, loff_t *unused_ppos)
{
	char local_buffer[CGROUP_LOCAL_BUFFER_SIZE];
	int retval = 0;
	size_t max_bytes = cft->max_write_len;
	char *buffer = local_buffer;

	if (!max_bytes)
		max_bytes = sizeof(local_buffer) - 1;
	if (nbytes >= max_bytes)
		return -E2BIG;
	/* Allocate a dynamic buffer if we need one */
	if (nbytes >= sizeof(local_buffer)) {
		buffer = kmalloc(nbytes + 1, GFP_KERNEL);
		if (buffer == NULL)
			return -ENOMEM;
	}
	if (nbytes && copy_from_user(buffer, userbuf, nbytes)) {
		retval = -EFAULT;
		goto out;
	}

	buffer[nbytes] = 0;     /* nul-terminate */
	retval = cft->write_string(cgrp, cft, strstrip(buffer));
	if (!retval)
		retval = nbytes;
out:
	if (buffer != local_buffer)
		kfree(buffer);
	return retval;
}

static ssize_t cgroup_file_write(struct file *file, const char __user *buf,
						size_t nbytes, loff_t *ppos)
{
	struct cftype *cft = __d_cft(file->f_dentry);
	struct cgroup *cgrp = __d_cgrp(file->f_dentry->d_parent);

	if (cgroup_is_removed(cgrp))
		return -ENODEV;
	if (cft->write)
		return cft->write(cgrp, cft, file, buf, nbytes, ppos);
	if (cft->write_u64 || cft->write_s64)
		return cgroup_write_X64(cgrp, cft, file, buf, nbytes, ppos);
	if (cft->write_string)
		return cgroup_write_string(cgrp, cft, file, buf, nbytes, ppos);
	if (cft->trigger) {
		int ret = cft->trigger(cgrp, (unsigned int)cft->private);
		return ret ? ret : nbytes;
	}
	return -EINVAL;
}

static ssize_t cgroup_read_u64(struct cgroup *cgrp, struct cftype *cft,
			       struct file *file,
			       char __user *buf, size_t nbytes,
			       loff_t *ppos)
{
	char tmp[CGROUP_LOCAL_BUFFER_SIZE];
	u64 val = cft->read_u64(cgrp, cft);
	int len = sprintf(tmp, "%llu\n", (unsigned long long) val);

	return simple_read_from_buffer(buf, nbytes, ppos, tmp, len);
}

static ssize_t cgroup_read_s64(struct cgroup *cgrp, struct cftype *cft,
			       struct file *file,
			       char __user *buf, size_t nbytes,
			       loff_t *ppos)
{
	char tmp[CGROUP_LOCAL_BUFFER_SIZE];
	s64 val = cft->read_s64(cgrp, cft);
	int len = sprintf(tmp, "%lld\n", (long long) val);

	return simple_read_from_buffer(buf, nbytes, ppos, tmp, len);
}

static ssize_t cgroup_file_read(struct file *file, char __user *buf,
				   size_t nbytes, loff_t *ppos)
{
	struct cftype *cft = __d_cft(file->f_dentry);
	struct cgroup *cgrp = __d_cgrp(file->f_dentry->d_parent);

	if (cgroup_is_removed(cgrp))
		return -ENODEV;

	if (cft->read)
		return cft->read(cgrp, cft, file, buf, nbytes, ppos);
	if (cft->read_u64)
		return cgroup_read_u64(cgrp, cft, file, buf, nbytes, ppos);
	if (cft->read_s64)
		return cgroup_read_s64(cgrp, cft, file, buf, nbytes, ppos);
	return -EINVAL;
}

/*
 * seqfile ops/methods for returning structured data. Currently just
 * supports string->u64 maps, but can be extended in future.
 */

struct cgroup_seqfile_state {
	struct cftype *cft;
	struct cgroup *cgroup;
};

static int cgroup_map_add(struct cgroup_map_cb *cb, const char *key, u64 value)
{
	struct seq_file *sf = cb->state;
	return seq_printf(sf, "%s %llu\n", key, (unsigned long long)value);
}

static int cgroup_seqfile_show(struct seq_file *m, void *arg)
{
	struct cgroup_seqfile_state *state = m->private;
	struct cftype *cft = state->cft;
	if (cft->read_map) {
		struct cgroup_map_cb cb = {
			.fill = cgroup_map_add,
			.state = m,
		};
		return cft->read_map(state->cgroup, cft, &cb);
	}
	return cft->read_seq_string(state->cgroup, cft, m);
}

static int cgroup_seqfile_release(struct inode *inode, struct file *file)
{
	struct seq_file *seq = file->private_data;
	kfree(seq->private);
	return single_release(inode, file);
}

static const struct file_operations cgroup_seqfile_operations = {
	.read = seq_read,
	.write = cgroup_file_write,
	.llseek = seq_lseek,
	.release = cgroup_seqfile_release,
};

static int cgroup_file_open(struct inode *inode, struct file *file)
{
	int err;
	struct cftype *cft;

	err = generic_file_open(inode, file);
	if (err)
		return err;
	cft = __d_cft(file->f_dentry);

	if (cft->read_map || cft->read_seq_string) {
		struct cgroup_seqfile_state *state =
			kzalloc(sizeof(*state), GFP_USER);
		if (!state)
			return -ENOMEM;
		state->cft = cft;
		state->cgroup = __d_cgrp(file->f_dentry->d_parent);
		file->f_op = &cgroup_seqfile_operations;
		err = single_open(file, cgroup_seqfile_show, state);
		if (err < 0)
			kfree(state);
	} else if (cft->open)
		err = cft->open(inode, file);
	else
		err = 0;

	return err;
}

static int cgroup_file_release(struct inode *inode, struct file *file)
{
	struct cftype *cft = __d_cft(file->f_dentry);
	if (cft->release)
		return cft->release(inode, file);
	return 0;
}

/*
 * cgroup_rename - Only allow simple rename of directories in place.
 */
static int cgroup_rename(struct inode *old_dir, struct dentry *old_dentry,
			    struct inode *new_dir, struct dentry *new_dentry)
{
	int ret;
	struct cgroup_name *name, *old_name;
	struct cgroup *cgrp;

	/*
	 * It's convinient to use parent dir's i_mutex to protected
	 * cgrp->name.
	 */
	lockdep_assert_held(&old_dir->i_mutex);

	if (!S_ISDIR(old_dentry->d_inode->i_mode))
		return -ENOTDIR;
	if (new_dentry->d_inode)
		return -EEXIST;
	if (old_dir != new_dir)
		return -EIO;

	cgrp = __d_cgrp(old_dentry);

	name = cgroup_alloc_name(new_dentry);
	if (!name)
		return -ENOMEM;

	ret = simple_rename(old_dir, old_dentry, new_dir, new_dentry);
	if (ret) {
		kfree(name);
		return ret;
	}

	old_name = cgrp->name;
	rcu_assign_pointer(cgrp->name, name);

	kfree_rcu(old_name, rcu_head);
	return 0;
}

static struct simple_xattrs *__d_xattrs(struct dentry *dentry)
{
	if (S_ISDIR(dentry->d_inode->i_mode))
		return &__d_cgrp(dentry)->xattrs;
	else
		return &__d_cfe(dentry)->xattrs;
}

static inline int xattr_enabled(struct dentry *dentry)
{
	struct cgroupfs_root *root = dentry->d_sb->s_fs_info;
	return root->flags & CGRP_ROOT_XATTR;
}

static bool is_valid_xattr(const char *name)
{
	if (!strncmp(name, XATTR_TRUSTED_PREFIX, XATTR_TRUSTED_PREFIX_LEN) ||
	    !strncmp(name, XATTR_SECURITY_PREFIX, XATTR_SECURITY_PREFIX_LEN))
		return true;
	return false;
}

static int cgroup_setxattr(struct dentry *dentry, const char *name,
			   const void *val, size_t size, int flags)
{
	if (!xattr_enabled(dentry))
		return -EOPNOTSUPP;
	if (!is_valid_xattr(name))
		return -EINVAL;
	return simple_xattr_set(__d_xattrs(dentry), name, val, size, flags);
}

static int cgroup_removexattr(struct dentry *dentry, const char *name)
{
	if (!xattr_enabled(dentry))
		return -EOPNOTSUPP;
	if (!is_valid_xattr(name))
		return -EINVAL;
	return simple_xattr_remove(__d_xattrs(dentry), name);
}

static ssize_t cgroup_getxattr(struct dentry *dentry, const char *name,
			       void *buf, size_t size)
{
	if (!xattr_enabled(dentry))
		return -EOPNOTSUPP;
	if (!is_valid_xattr(name))
		return -EINVAL;
	return simple_xattr_get(__d_xattrs(dentry), name, buf, size);
}

static ssize_t cgroup_listxattr(struct dentry *dentry, char *buf, size_t size)
{
	if (!xattr_enabled(dentry))
		return -EOPNOTSUPP;
	return simple_xattr_list(__d_xattrs(dentry), buf, size);
}

static const struct file_operations cgroup_file_operations = {
	.read = cgroup_file_read,
	.write = cgroup_file_write,
	.llseek = generic_file_llseek,
	.open = cgroup_file_open,
	.release = cgroup_file_release,
};

static const struct inode_operations cgroup_file_inode_operations = {
	.setxattr = cgroup_setxattr,
	.getxattr = cgroup_getxattr,
	.listxattr = cgroup_listxattr,
	.removexattr = cgroup_removexattr,
};

static const struct inode_operations cgroup_dir_inode_operations = {
	.lookup = cgroup_lookup,
	.mkdir = cgroup_mkdir,
	.rmdir = cgroup_rmdir,
	.rename = cgroup_rename,
	.setxattr = cgroup_setxattr,
	.getxattr = cgroup_getxattr,
	.listxattr = cgroup_listxattr,
	.removexattr = cgroup_removexattr,
};

static struct dentry *cgroup_lookup(struct inode *dir, struct dentry *dentry, unsigned int flags)
{
	if (dentry->d_name.len > NAME_MAX)
		return ERR_PTR(-ENAMETOOLONG);
	d_add(dentry, NULL);
	return NULL;
}

/*
 * Check if a file is a control file
 */
static inline struct cftype *__file_cft(struct file *file)
{
	if (file_inode(file)->i_fop != &cgroup_file_operations)
		return ERR_PTR(-EINVAL);
	return __d_cft(file->f_dentry);
}

static int cgroup_create_file(struct dentry *dentry, umode_t mode,
				struct super_block *sb)
{
	struct inode *inode;

	if (!dentry)
		return -ENOENT;
	if (dentry->d_inode)
		return -EEXIST;

	inode = cgroup_new_inode(mode, sb);
	if (!inode)
		return -ENOMEM;

	if (S_ISDIR(mode)) {
		inode->i_op = &cgroup_dir_inode_operations;
		inode->i_fop = &simple_dir_operations;

		/* start off with i_nlink == 2 (for "." entry) */
		inc_nlink(inode);
		inc_nlink(dentry->d_parent->d_inode);

		/*
		 * Control reaches here with cgroup_mutex held.
		 * @inode->i_mutex should nest outside cgroup_mutex but we
		 * want to populate it immediately without releasing
		 * cgroup_mutex.  As @inode isn't visible to anyone else
		 * yet, trylock will always succeed without affecting
		 * lockdep checks.
		 */
		WARN_ON_ONCE(!mutex_trylock(&inode->i_mutex));
	} else if (S_ISREG(mode)) {
		inode->i_size = 0;
		inode->i_fop = &cgroup_file_operations;
		inode->i_op = &cgroup_file_inode_operations;
	}
	d_instantiate(dentry, inode);
	dget(dentry);	/* Extra count - pin the dentry in core */
	return 0;
}

/**
 * cgroup_file_mode - deduce file mode of a control file
 * @cft: the control file in question
 *
 * returns cft->mode if ->mode is not 0
 * returns S_IRUGO|S_IWUSR if it has both a read and a write handler
 * returns S_IRUGO if it has only a read handler
 * returns S_IWUSR if it has only a write hander
 */
static umode_t cgroup_file_mode(const struct cftype *cft)
{
	umode_t mode = 0;

	if (cft->mode)
		return cft->mode;

	if (cft->read || cft->read_u64 || cft->read_s64 ||
	    cft->read_map || cft->read_seq_string)
		mode |= S_IRUGO;

	if (cft->write || cft->write_u64 || cft->write_s64 ||
	    cft->write_string || cft->trigger)
		mode |= S_IWUSR;

	return mode;
}

static int cgroup_add_file(struct cgroup *cgrp, struct cgroup_subsys *subsys,
			   struct cftype *cft)
{
	struct dentry *dir = cgrp->dentry;
	struct cgroup *parent = __d_cgrp(dir);
	struct dentry *dentry;
	struct cfent *cfe;
	int error;
	umode_t mode;
	char name[MAX_CGROUP_TYPE_NAMELEN + MAX_CFTYPE_NAME + 2] = { 0 };

	if (subsys && !(cgrp->root->flags & CGRP_ROOT_NOPREFIX)) {
		strcpy(name, subsys->name);
		strcat(name, ".");
	}
	strcat(name, cft->name);

	BUG_ON(!mutex_is_locked(&dir->d_inode->i_mutex));

	cfe = kzalloc(sizeof(*cfe), GFP_KERNEL);
	if (!cfe)
		return -ENOMEM;

	dentry = lookup_one_len(name, dir, strlen(name));
	if (IS_ERR(dentry)) {
		error = PTR_ERR(dentry);
		goto out;
	}

	cfe->type = (void *)cft;
	cfe->dentry = dentry;
	dentry->d_fsdata = cfe;
	simple_xattrs_init(&cfe->xattrs);

	mode = cgroup_file_mode(cft);
	error = cgroup_create_file(dentry, mode | S_IFREG, cgrp->root->sb);
	if (!error) {
		list_add_tail(&cfe->node, &parent->files);
		cfe = NULL;
	}
	dput(dentry);
out:
	kfree(cfe);
	return error;
}

static int cgroup_addrm_files(struct cgroup *cgrp, struct cgroup_subsys *subsys,
			      struct cftype cfts[], bool is_add)
{
	struct cftype *cft;
	int err, ret = 0;

	for (cft = cfts; cft->name[0] != '\0'; cft++) {
		/* does cft->flags tell us to skip this file on @cgrp? */
		if ((cft->flags & CFTYPE_INSANE) && cgroup_sane_behavior(cgrp))
			continue;
		if ((cft->flags & CFTYPE_NOT_ON_ROOT) && !cgrp->parent)
			continue;
		if ((cft->flags & CFTYPE_ONLY_ON_ROOT) && cgrp->parent)
			continue;

		if (is_add) {
			err = cgroup_add_file(cgrp, subsys, cft);
			if (err)
				pr_warn("cgroup_addrm_files: failed to add %s, err=%d\n",
					cft->name, err);
			ret = err;
=======
static ssize_t cgroup_tasks_write(struct kernfs_open_file *of,
				  char *buf, size_t nbytes, loff_t off)
{
	return __cgroup_procs_write(of, buf, nbytes, off, false);
}

static ssize_t cgroup_procs_write(struct kernfs_open_file *of,
				  char *buf, size_t nbytes, loff_t off)
{
	return __cgroup_procs_write(of, buf, nbytes, off, true);
}

static ssize_t cgroup_release_agent_write(struct kernfs_open_file *of,
					  char *buf, size_t nbytes, loff_t off)
{
	struct cgroup *cgrp;

	BUILD_BUG_ON(sizeof(cgrp->root->release_agent_path) < PATH_MAX);

	cgrp = cgroup_kn_lock_live(of->kn);
	if (!cgrp)
		return -ENODEV;
	spin_lock(&release_agent_path_lock);
	strlcpy(cgrp->root->release_agent_path, strstrip(buf),
		sizeof(cgrp->root->release_agent_path));
	spin_unlock(&release_agent_path_lock);
	cgroup_kn_unlock(of->kn);
	return nbytes;
}

static int cgroup_release_agent_show(struct seq_file *seq, void *v)
{
	struct cgroup *cgrp = seq_css(seq)->cgroup;

	spin_lock(&release_agent_path_lock);
	seq_puts(seq, cgrp->root->release_agent_path);
	spin_unlock(&release_agent_path_lock);
	seq_putc(seq, '\n');
	return 0;
}

static int cgroup_sane_behavior_show(struct seq_file *seq, void *v)
{
	seq_puts(seq, "0\n");
	return 0;
}

static void cgroup_print_ss_mask(struct seq_file *seq, unsigned int ss_mask)
{
	struct cgroup_subsys *ss;
	bool printed = false;
	int ssid;

	for_each_subsys(ss, ssid) {
		if (ss_mask & (1 << ssid)) {
			if (printed)
				seq_putc(seq, ' ');
			seq_printf(seq, "%s", ss->name);
			printed = true;
		}
	}
	if (printed)
		seq_putc(seq, '\n');
}

/* show controllers which are currently attached to the default hierarchy */
static int cgroup_root_controllers_show(struct seq_file *seq, void *v)
{
	struct cgroup *cgrp = seq_css(seq)->cgroup;

	cgroup_print_ss_mask(seq, cgrp->root->subsys_mask &
			     ~cgrp_dfl_root_inhibit_ss_mask);
	return 0;
}

/* show controllers which are enabled from the parent */
static int cgroup_controllers_show(struct seq_file *seq, void *v)
{
	struct cgroup *cgrp = seq_css(seq)->cgroup;

	cgroup_print_ss_mask(seq, cgroup_parent(cgrp)->subtree_control);
	return 0;
}

/* show controllers which are enabled for a given cgroup's children */
static int cgroup_subtree_control_show(struct seq_file *seq, void *v)
{
	struct cgroup *cgrp = seq_css(seq)->cgroup;

	cgroup_print_ss_mask(seq, cgrp->subtree_control);
	return 0;
}

/**
 * cgroup_update_dfl_csses - update css assoc of a subtree in default hierarchy
 * @cgrp: root of the subtree to update csses for
 *
 * @cgrp's child_subsys_mask has changed and its subtree's (self excluded)
 * css associations need to be updated accordingly.  This function looks up
 * all css_sets which are attached to the subtree, creates the matching
 * updated css_sets and migrates the tasks to the new ones.
 */
static int cgroup_update_dfl_csses(struct cgroup *cgrp)
{
	LIST_HEAD(preloaded_csets);
	struct cgroup_subsys_state *css;
	struct css_set *src_cset;
	int ret;

	lockdep_assert_held(&cgroup_mutex);

	/* look up all csses currently attached to @cgrp's subtree */
	down_read(&css_set_rwsem);
	css_for_each_descendant_pre(css, cgroup_css(cgrp, NULL)) {
		struct cgrp_cset_link *link;

		/* self is not affected by child_subsys_mask change */
		if (css->cgroup == cgrp)
			continue;

		list_for_each_entry(link, &css->cgroup->cset_links, cset_link)
			cgroup_migrate_add_src(link->cset, cgrp,
					       &preloaded_csets);
	}
	up_read(&css_set_rwsem);

	/* NULL dst indicates self on default hierarchy */
	ret = cgroup_migrate_prepare_dst(NULL, &preloaded_csets);
	if (ret)
		goto out_finish;

	list_for_each_entry(src_cset, &preloaded_csets, mg_preload_node) {
		struct task_struct *last_task = NULL, *task;

		/* src_csets precede dst_csets, break on the first dst_cset */
		if (!src_cset->mg_src_cgrp)
			break;

		/*
		 * All tasks in src_cset need to be migrated to the
		 * matching dst_cset.  Empty it process by process.  We
		 * walk tasks but migrate processes.  The leader might even
		 * belong to a different cset but such src_cset would also
		 * be among the target src_csets because the default
		 * hierarchy enforces per-process membership.
		 */
		while (true) {
			down_read(&css_set_rwsem);
			task = list_first_entry_or_null(&src_cset->tasks,
						struct task_struct, cg_list);
			if (task) {
				task = task->group_leader;
				WARN_ON_ONCE(!task_css_set(task)->mg_src_cgrp);
				get_task_struct(task);
			}
			up_read(&css_set_rwsem);

			if (!task)
				break;

			/* guard against possible infinite loop */
			if (WARN(last_task == task,
				 "cgroup: update_dfl_csses failed to make progress, aborting in inconsistent state\n"))
				goto out_finish;
			last_task = task;

			threadgroup_lock(task);
			/* raced against de_thread() from another thread? */
			if (!thread_group_leader(task)) {
				threadgroup_unlock(task);
				put_task_struct(task);
				continue;
			}

			ret = cgroup_migrate(src_cset->dfl_cgrp, task, true);

			threadgroup_unlock(task);
			put_task_struct(task);

			if (WARN(ret, "cgroup: failed to update controllers for the default hierarchy (%d), further operations may crash or hang\n", ret))
				goto out_finish;
		}
	}

out_finish:
	cgroup_migrate_finish(&preloaded_csets);
	return ret;
}

/* change the enabled child controllers for a cgroup in the default hierarchy */
static ssize_t cgroup_subtree_control_write(struct kernfs_open_file *of,
					    char *buf, size_t nbytes,
					    loff_t off)
{
	unsigned int enable = 0, disable = 0;
	unsigned int css_enable, css_disable, old_ctrl, new_ctrl;
	struct cgroup *cgrp, *child;
	struct cgroup_subsys *ss;
	char *tok;
	int ssid, ret;

	/*
	 * Parse input - space separated list of subsystem names prefixed
	 * with either + or -.
	 */
	buf = strstrip(buf);
	while ((tok = strsep(&buf, " "))) {
		if (tok[0] == '\0')
			continue;
		for_each_subsys(ss, ssid) {
			if (ss->disabled || strcmp(tok + 1, ss->name) ||
			    ((1 << ss->id) & cgrp_dfl_root_inhibit_ss_mask))
				continue;

			if (*tok == '+') {
				enable |= 1 << ssid;
				disable &= ~(1 << ssid);
			} else if (*tok == '-') {
				disable |= 1 << ssid;
				enable &= ~(1 << ssid);
			} else {
				return -EINVAL;
			}
			break;
		}
		if (ssid == CGROUP_SUBSYS_COUNT)
			return -EINVAL;
	}

	cgrp = cgroup_kn_lock_live(of->kn);
	if (!cgrp)
		return -ENODEV;

	for_each_subsys(ss, ssid) {
		if (enable & (1 << ssid)) {
			if (cgrp->subtree_control & (1 << ssid)) {
				enable &= ~(1 << ssid);
				continue;
			}

			/* unavailable or not enabled on the parent? */
			if (!(cgrp_dfl_root.subsys_mask & (1 << ssid)) ||
			    (cgroup_parent(cgrp) &&
			     !(cgroup_parent(cgrp)->subtree_control & (1 << ssid)))) {
				ret = -ENOENT;
				goto out_unlock;
			}

			/*
			 * @ss is already enabled through dependency and
			 * we'll just make it visible.  Skip draining.
			 */
			if (cgrp->child_subsys_mask & (1 << ssid))
				continue;

			/*
			 * Because css offlining is asynchronous, userland
			 * might try to re-enable the same controller while
			 * the previous instance is still around.  In such
			 * cases, wait till it's gone using offline_waitq.
			 */
			cgroup_for_each_live_child(child, cgrp) {
				DEFINE_WAIT(wait);

				if (!cgroup_css(child, ss))
					continue;

				cgroup_get(child);
				prepare_to_wait(&child->offline_waitq, &wait,
						TASK_UNINTERRUPTIBLE);
				cgroup_kn_unlock(of->kn);
				schedule();
				finish_wait(&child->offline_waitq, &wait);
				cgroup_put(child);

				return restart_syscall();
			}
		} else if (disable & (1 << ssid)) {
			if (!(cgrp->subtree_control & (1 << ssid))) {
				disable &= ~(1 << ssid);
				continue;
			}

			/* a child has it enabled? */
			cgroup_for_each_live_child(child, cgrp) {
				if (child->subtree_control & (1 << ssid)) {
					ret = -EBUSY;
					goto out_unlock;
				}
			}
		}
	}

	if (!enable && !disable) {
		ret = 0;
		goto out_unlock;
	}

	/*
	 * Except for the root, subtree_control must be zero for a cgroup
	 * with tasks so that child cgroups don't compete against tasks.
	 */
	if (enable && cgroup_parent(cgrp) && !list_empty(&cgrp->cset_links)) {
		ret = -EBUSY;
		goto out_unlock;
	}

	/*
	 * Update subsys masks and calculate what needs to be done.  More
	 * subsystems than specified may need to be enabled or disabled
	 * depending on subsystem dependencies.
	 */
	cgrp->subtree_control |= enable;
	cgrp->subtree_control &= ~disable;

	old_ctrl = cgrp->child_subsys_mask;
	cgroup_refresh_child_subsys_mask(cgrp);
	new_ctrl = cgrp->child_subsys_mask;

	css_enable = ~old_ctrl & new_ctrl;
	css_disable = old_ctrl & ~new_ctrl;
	enable |= css_enable;
	disable |= css_disable;

	/*
	 * Create new csses or make the existing ones visible.  A css is
	 * created invisible if it's being implicitly enabled through
	 * dependency.  An invisible css is made visible when the userland
	 * explicitly enables it.
	 */
	for_each_subsys(ss, ssid) {
		if (!(enable & (1 << ssid)))
			continue;

		cgroup_for_each_live_child(child, cgrp) {
			if (css_enable & (1 << ssid))
				ret = create_css(child, ss,
					cgrp->subtree_control & (1 << ssid));
			else
				ret = cgroup_populate_dir(child, 1 << ssid);
			if (ret)
				goto err_undo_css;
		}
	}

	/*
	 * At this point, cgroup_e_css() results reflect the new csses
	 * making the following cgroup_update_dfl_csses() properly update
	 * css associations of all tasks in the subtree.
	 */
	ret = cgroup_update_dfl_csses(cgrp);
	if (ret)
		goto err_undo_css;

	/*
	 * All tasks are migrated out of disabled csses.  Kill or hide
	 * them.  A css is hidden when the userland requests it to be
	 * disabled while other subsystems are still depending on it.  The
	 * css must not actively control resources and be in the vanilla
	 * state if it's made visible again later.  Controllers which may
	 * be depended upon should provide ->css_reset() for this purpose.
	 */
	for_each_subsys(ss, ssid) {
		if (!(disable & (1 << ssid)))
			continue;

		cgroup_for_each_live_child(child, cgrp) {
			struct cgroup_subsys_state *css = cgroup_css(child, ss);

			if (css_disable & (1 << ssid)) {
				kill_css(css);
			} else {
				cgroup_clear_dir(child, 1 << ssid);
				if (ss->css_reset)
					ss->css_reset(css);
			}
		}
	}

	kernfs_activate(cgrp->kn);
	ret = 0;
out_unlock:
	cgroup_kn_unlock(of->kn);
	return ret ?: nbytes;

err_undo_css:
	cgrp->subtree_control &= ~enable;
	cgrp->subtree_control |= disable;
	cgroup_refresh_child_subsys_mask(cgrp);

	for_each_subsys(ss, ssid) {
		if (!(enable & (1 << ssid)))
			continue;

		cgroup_for_each_live_child(child, cgrp) {
			struct cgroup_subsys_state *css = cgroup_css(child, ss);

			if (!css)
				continue;

			if (css_enable & (1 << ssid))
				kill_css(css);
			else
				cgroup_clear_dir(child, 1 << ssid);
		}
	}
	goto out_unlock;
}

static int cgroup_populated_show(struct seq_file *seq, void *v)
{
	seq_printf(seq, "%d\n", (bool)seq_css(seq)->cgroup->populated_cnt);
	return 0;
}

static ssize_t cgroup_file_write(struct kernfs_open_file *of, char *buf,
				 size_t nbytes, loff_t off)
{
	struct cgroup *cgrp = of->kn->parent->priv;
	struct cftype *cft = of->kn->priv;
	struct cgroup_subsys_state *css;
	int ret;

	if (cft->write)
		return cft->write(of, buf, nbytes, off);

	/*
	 * kernfs guarantees that a file isn't deleted with operations in
	 * flight, which means that the matching css is and stays alive and
	 * doesn't need to be pinned.  The RCU locking is not necessary
	 * either.  It's just for the convenience of using cgroup_css().
	 */
	rcu_read_lock();
	css = cgroup_css(cgrp, cft->ss);
	rcu_read_unlock();

	if (cft->write_u64) {
		unsigned long long v;
		ret = kstrtoull(buf, 0, &v);
		if (!ret)
			ret = cft->write_u64(css, cft, v);
	} else if (cft->write_s64) {
		long long v;
		ret = kstrtoll(buf, 0, &v);
		if (!ret)
			ret = cft->write_s64(css, cft, v);
	} else {
		ret = -EINVAL;
	}

	return ret ?: nbytes;
}

static void *cgroup_seqfile_start(struct seq_file *seq, loff_t *ppos)
{
	return seq_cft(seq)->seq_start(seq, ppos);
}

static void *cgroup_seqfile_next(struct seq_file *seq, void *v, loff_t *ppos)
{
	return seq_cft(seq)->seq_next(seq, v, ppos);
}

static void cgroup_seqfile_stop(struct seq_file *seq, void *v)
{
	seq_cft(seq)->seq_stop(seq, v);
}

static int cgroup_seqfile_show(struct seq_file *m, void *arg)
{
	struct cftype *cft = seq_cft(m);
	struct cgroup_subsys_state *css = seq_css(m);

	if (cft->seq_show)
		return cft->seq_show(m, arg);

	if (cft->read_u64)
		seq_printf(m, "%llu\n", cft->read_u64(css, cft));
	else if (cft->read_s64)
		seq_printf(m, "%lld\n", cft->read_s64(css, cft));
	else
		return -EINVAL;
	return 0;
}

static struct kernfs_ops cgroup_kf_single_ops = {
	.atomic_write_len	= PAGE_SIZE,
	.write			= cgroup_file_write,
	.seq_show		= cgroup_seqfile_show,
};

static struct kernfs_ops cgroup_kf_ops = {
	.atomic_write_len	= PAGE_SIZE,
	.write			= cgroup_file_write,
	.seq_start		= cgroup_seqfile_start,
	.seq_next		= cgroup_seqfile_next,
	.seq_stop		= cgroup_seqfile_stop,
	.seq_show		= cgroup_seqfile_show,
};

/*
 * cgroup_rename - Only allow simple rename of directories in place.
 */
static int cgroup_rename(struct kernfs_node *kn, struct kernfs_node *new_parent,
			 const char *new_name_str)
{
	struct cgroup *cgrp = kn->priv;
	int ret;

	if (kernfs_type(kn) != KERNFS_DIR)
		return -ENOTDIR;
	if (kn->parent != new_parent)
		return -EIO;

	/*
	 * This isn't a proper migration and its usefulness is very
	 * limited.  Disallow on the default hierarchy.
	 */
	if (cgroup_on_dfl(cgrp))
		return -EPERM;

	/*
	 * We're gonna grab cgroup_mutex which nests outside kernfs
	 * active_ref.  kernfs_rename() doesn't require active_ref
	 * protection.  Break them before grabbing cgroup_mutex.
	 */
	kernfs_break_active_protection(new_parent);
	kernfs_break_active_protection(kn);

	mutex_lock(&cgroup_mutex);

	ret = kernfs_rename(kn, new_parent, new_name_str);

	mutex_unlock(&cgroup_mutex);

	kernfs_unbreak_active_protection(kn);
	kernfs_unbreak_active_protection(new_parent);
	return ret;
}

/* set uid and gid of cgroup dirs and files to that of the creator */
static int cgroup_kn_set_ugid(struct kernfs_node *kn)
{
	struct iattr iattr = { .ia_valid = ATTR_UID | ATTR_GID,
			       .ia_uid = current_fsuid(),
			       .ia_gid = current_fsgid(), };

	if (uid_eq(iattr.ia_uid, GLOBAL_ROOT_UID) &&
	    gid_eq(iattr.ia_gid, GLOBAL_ROOT_GID))
		return 0;

	return kernfs_setattr(kn, &iattr);
}

static int cgroup_add_file(struct cgroup *cgrp, struct cftype *cft)
{
	char name[CGROUP_FILE_NAME_MAX];
	struct kernfs_node *kn;
	struct lock_class_key *key = NULL;
	int ret;

#ifdef CONFIG_DEBUG_LOCK_ALLOC
	key = &cft->lockdep_key;
#endif
	kn = __kernfs_create_file(cgrp->kn, cgroup_file_name(cgrp, cft, name),
				  cgroup_file_mode(cft), 0, cft->kf_ops, cft,
				  NULL, false, key);
	if (IS_ERR(kn))
		return PTR_ERR(kn);

	ret = cgroup_kn_set_ugid(kn);
	if (ret) {
		kernfs_remove(kn);
		return ret;
	}

	if (cft->seq_show == cgroup_populated_show)
		cgrp->populated_kn = kn;
	return 0;
}

/**
 * cgroup_addrm_files - add or remove files to a cgroup directory
 * @cgrp: the target cgroup
 * @cfts: array of cftypes to be added
 * @is_add: whether to add or remove
 *
 * Depending on @is_add, add or remove files defined by @cfts on @cgrp.
 * For removals, this function never fails.  If addition fails, this
 * function doesn't remove files already added.  The caller is responsible
 * for cleaning up.
 */
static int cgroup_addrm_files(struct cgroup *cgrp, struct cftype cfts[],
			      bool is_add)
{
	struct cftype *cft;
	int ret;

	lockdep_assert_held(&cgroup_mutex);

	for (cft = cfts; cft->name[0] != '\0'; cft++) {
		/* does cft->flags tell us to skip this file on @cgrp? */
		if ((cft->flags & __CFTYPE_ONLY_ON_DFL) && !cgroup_on_dfl(cgrp))
			continue;
		if ((cft->flags & __CFTYPE_NOT_ON_DFL) && cgroup_on_dfl(cgrp))
			continue;
		if ((cft->flags & CFTYPE_NOT_ON_ROOT) && !cgroup_parent(cgrp))
			continue;
		if ((cft->flags & CFTYPE_ONLY_ON_ROOT) && cgroup_parent(cgrp))
			continue;

		if (is_add) {
			ret = cgroup_add_file(cgrp, cft);
			if (ret) {
				pr_warn("%s: failed to add %s, err=%d\n",
					__func__, cft->name, ret);
				return ret;
			}
>>>>>>> v3.18
		} else {
			cgroup_rm_file(cgrp, cft);
		}
	}
<<<<<<< HEAD
	return ret;
}

static DEFINE_MUTEX(cgroup_cft_mutex);

static void cgroup_cfts_prepare(void)
	__acquires(&cgroup_cft_mutex) __acquires(&cgroup_mutex)
{
	/*
	 * Thanks to the entanglement with vfs inode locking, we can't walk
	 * the existing cgroups under cgroup_mutex and create files.
	 * Instead, we increment reference on all cgroups and build list of
	 * them using @cgrp->cft_q_node.  Grab cgroup_cft_mutex to ensure
	 * exclusive access to the field.
	 */
	mutex_lock(&cgroup_cft_mutex);
	mutex_lock(&cgroup_mutex);
}

static void cgroup_cfts_commit(struct cgroup_subsys *ss,
			       struct cftype *cfts, bool is_add)
	__releases(&cgroup_mutex) __releases(&cgroup_cft_mutex)
{
	LIST_HEAD(pending);
	struct cgroup *cgrp, *n;
	struct super_block *sb = ss->root->sb;

	/* %NULL @cfts indicates abort and don't bother if @ss isn't attached */
	if (cfts && ss->root != &rootnode &&
	    atomic_inc_not_zero(&sb->s_active)) {
		list_for_each_entry(cgrp, &ss->root->allcg_list, allcg_node) {
			dget(cgrp->dentry);
			list_add_tail(&cgrp->cft_q_node, &pending);
		}
	} else {
		sb = NULL;
	}

	mutex_unlock(&cgroup_mutex);

	/*
	 * All new cgroups will see @cfts update on @ss->cftsets.  Add/rm
	 * files for all cgroups which were created before.
	 */
	list_for_each_entry_safe(cgrp, n, &pending, cft_q_node) {
		struct inode *inode = cgrp->dentry->d_inode;

		mutex_lock(&inode->i_mutex);
		mutex_lock(&cgroup_mutex);
		if (!cgroup_is_removed(cgrp))
			cgroup_addrm_files(cgrp, ss, cfts, is_add);
		mutex_unlock(&cgroup_mutex);
		mutex_unlock(&inode->i_mutex);

		list_del_init(&cgrp->cft_q_node);
		dput(cgrp->dentry);
	}

	if (sb)
		deactivate_super(sb);

	mutex_unlock(&cgroup_cft_mutex);
=======
	return 0;
}

static int cgroup_apply_cftypes(struct cftype *cfts, bool is_add)
{
	LIST_HEAD(pending);
	struct cgroup_subsys *ss = cfts[0].ss;
	struct cgroup *root = &ss->root->cgrp;
	struct cgroup_subsys_state *css;
	int ret = 0;

	lockdep_assert_held(&cgroup_mutex);

	/* add/rm files for all cgroups created before */
	css_for_each_descendant_pre(css, cgroup_css(root, ss)) {
		struct cgroup *cgrp = css->cgroup;

		if (cgroup_is_dead(cgrp))
			continue;

		ret = cgroup_addrm_files(cgrp, cfts, is_add);
		if (ret)
			break;
	}

	if (is_add && !ret)
		kernfs_activate(root->kn);
	return ret;
}

static void cgroup_exit_cftypes(struct cftype *cfts)
{
	struct cftype *cft;

	for (cft = cfts; cft->name[0] != '\0'; cft++) {
		/* free copy for custom atomic_write_len, see init_cftypes() */
		if (cft->max_write_len && cft->max_write_len != PAGE_SIZE)
			kfree(cft->kf_ops);
		cft->kf_ops = NULL;
		cft->ss = NULL;

		/* revert flags set by cgroup core while adding @cfts */
		cft->flags &= ~(__CFTYPE_ONLY_ON_DFL | __CFTYPE_NOT_ON_DFL);
	}
}

static int cgroup_init_cftypes(struct cgroup_subsys *ss, struct cftype *cfts)
{
	struct cftype *cft;

	for (cft = cfts; cft->name[0] != '\0'; cft++) {
		struct kernfs_ops *kf_ops;

		WARN_ON(cft->ss || cft->kf_ops);

		if (cft->seq_start)
			kf_ops = &cgroup_kf_ops;
		else
			kf_ops = &cgroup_kf_single_ops;

		/*
		 * Ugh... if @cft wants a custom max_write_len, we need to
		 * make a copy of kf_ops to set its atomic_write_len.
		 */
		if (cft->max_write_len && cft->max_write_len != PAGE_SIZE) {
			kf_ops = kmemdup(kf_ops, sizeof(*kf_ops), GFP_KERNEL);
			if (!kf_ops) {
				cgroup_exit_cftypes(cfts);
				return -ENOMEM;
			}
			kf_ops->atomic_write_len = cft->max_write_len;
		}

		cft->kf_ops = kf_ops;
		cft->ss = ss;
	}

	return 0;
}

static int cgroup_rm_cftypes_locked(struct cftype *cfts)
{
	lockdep_assert_held(&cgroup_mutex);

	if (!cfts || !cfts[0].ss)
		return -ENOENT;

	list_del(&cfts->node);
	cgroup_apply_cftypes(cfts, false);
	cgroup_exit_cftypes(cfts);
	return 0;
}

/**
 * cgroup_rm_cftypes - remove an array of cftypes from a subsystem
 * @cfts: zero-length name terminated array of cftypes
 *
 * Unregister @cfts.  Files described by @cfts are removed from all
 * existing cgroups and all future cgroups won't have them either.  This
 * function can be called anytime whether @cfts' subsys is attached or not.
 *
 * Returns 0 on successful unregistration, -ENOENT if @cfts is not
 * registered.
 */
int cgroup_rm_cftypes(struct cftype *cfts)
{
	int ret;

	mutex_lock(&cgroup_mutex);
	ret = cgroup_rm_cftypes_locked(cfts);
	mutex_unlock(&cgroup_mutex);
	return ret;
>>>>>>> v3.18
}

/**
 * cgroup_add_cftypes - add an array of cftypes to a subsystem
 * @ss: target cgroup subsystem
 * @cfts: zero-length name terminated array of cftypes
 *
 * Register @cfts to @ss.  Files described by @cfts are created for all
 * existing cgroups to which @ss is attached and all future cgroups will
 * have them too.  This function can be called anytime whether @ss is
 * attached or not.
 *
 * Returns 0 on successful registration, -errno on failure.  Note that this
 * function currently returns 0 as long as @cfts registration is successful
 * even if some file creation attempts on existing cgroups fail.
 */
<<<<<<< HEAD
int cgroup_add_cftypes(struct cgroup_subsys *ss, struct cftype *cfts)
{
	struct cftype_set *set;

	set = kzalloc(sizeof(*set), GFP_KERNEL);
	if (!set)
		return -ENOMEM;

	cgroup_cfts_prepare();
	set->cfts = cfts;
	list_add_tail(&set->node, &ss->cftsets);
	cgroup_cfts_commit(ss, cfts, true);

	return 0;
}
EXPORT_SYMBOL_GPL(cgroup_add_cftypes);

/**
 * cgroup_rm_cftypes - remove an array of cftypes from a subsystem
 * @ss: target cgroup subsystem
 * @cfts: zero-length name terminated array of cftypes
 *
 * Unregister @cfts from @ss.  Files described by @cfts are removed from
 * all existing cgroups to which @ss is attached and all future cgroups
 * won't have them either.  This function can be called anytime whether @ss
 * is attached or not.
 *
 * Returns 0 on successful unregistration, -ENOENT if @cfts is not
 * registered with @ss.
 */
int cgroup_rm_cftypes(struct cgroup_subsys *ss, struct cftype *cfts)
{
	struct cftype_set *set;

	cgroup_cfts_prepare();

	list_for_each_entry(set, &ss->cftsets, node) {
		if (set->cfts == cfts) {
			list_del_init(&set->node);
			cgroup_cfts_commit(ss, cfts, false);
			return 0;
		}
	}

	cgroup_cfts_commit(ss, NULL, false);
	return -ENOENT;
=======
static int cgroup_add_cftypes(struct cgroup_subsys *ss, struct cftype *cfts)
{
	int ret;

	if (ss->disabled)
		return 0;

	if (!cfts || cfts[0].name[0] == '\0')
		return 0;

	ret = cgroup_init_cftypes(ss, cfts);
	if (ret)
		return ret;

	mutex_lock(&cgroup_mutex);

	list_add_tail(&cfts->node, &ss->cfts);
	ret = cgroup_apply_cftypes(cfts, true);
	if (ret)
		cgroup_rm_cftypes_locked(cfts);

	mutex_unlock(&cgroup_mutex);
	return ret;
}

/**
 * cgroup_add_dfl_cftypes - add an array of cftypes for default hierarchy
 * @ss: target cgroup subsystem
 * @cfts: zero-length name terminated array of cftypes
 *
 * Similar to cgroup_add_cftypes() but the added files are only used for
 * the default hierarchy.
 */
int cgroup_add_dfl_cftypes(struct cgroup_subsys *ss, struct cftype *cfts)
{
	struct cftype *cft;

	for (cft = cfts; cft && cft->name[0] != '\0'; cft++)
		cft->flags |= __CFTYPE_ONLY_ON_DFL;
	return cgroup_add_cftypes(ss, cfts);
}

/**
 * cgroup_add_legacy_cftypes - add an array of cftypes for legacy hierarchies
 * @ss: target cgroup subsystem
 * @cfts: zero-length name terminated array of cftypes
 *
 * Similar to cgroup_add_cftypes() but the added files are only used for
 * the legacy hierarchies.
 */
int cgroup_add_legacy_cftypes(struct cgroup_subsys *ss, struct cftype *cfts)
{
	struct cftype *cft;

	/*
	 * If legacy_flies_on_dfl, we want to show the legacy files on the
	 * dfl hierarchy but iff the target subsystem hasn't been updated
	 * for the dfl hierarchy yet.
	 */
	if (!cgroup_legacy_files_on_dfl ||
	    ss->dfl_cftypes != ss->legacy_cftypes) {
		for (cft = cfts; cft && cft->name[0] != '\0'; cft++)
			cft->flags |= __CFTYPE_NOT_ON_DFL;
	}

	return cgroup_add_cftypes(ss, cfts);
>>>>>>> v3.18
}

/**
 * cgroup_task_count - count the number of tasks in a cgroup.
 * @cgrp: the cgroup in question
 *
 * Return the number of tasks in the cgroup.
 */
<<<<<<< HEAD
int cgroup_task_count(const struct cgroup *cgrp)
{
	int count = 0;
	struct cg_cgroup_link *link;

	read_lock(&css_set_lock);
	list_for_each_entry(link, &cgrp->css_sets, cgrp_link_list) {
		count += atomic_read(&link->cg->refcount);
	}
	read_unlock(&css_set_lock);
	return count;
}

/*
 * Advance a list_head iterator.  The iterator should be positioned at
 * the start of a css_set
 */
static void cgroup_advance_iter(struct cgroup *cgrp,
				struct cgroup_iter *it)
{
	struct list_head *l = it->cg_link;
	struct cg_cgroup_link *link;
	struct css_set *cg;

	/* Advance to the next non-empty css_set */
	do {
		l = l->next;
		if (l == &cgrp->css_sets) {
			it->cg_link = NULL;
			return;
		}
		link = list_entry(l, struct cg_cgroup_link, cgrp_link_list);
		cg = link->cg;
	} while (list_empty(&cg->tasks));
	it->cg_link = l;
	it->task = cg->tasks.next;
}

/*
 * To reduce the fork() overhead for systems that are not actually
 * using their cgroups capability, we don't maintain the lists running
 * through each css_set to its tasks until we see the list actually
 * used - in other words after the first call to cgroup_iter_start().
 */
static void cgroup_enable_task_cg_lists(void)
{
	struct task_struct *p, *g;
	write_lock(&css_set_lock);
	use_task_css_set_links = 1;
	/*
	 * We need tasklist_lock because RCU is not safe against
	 * while_each_thread(). Besides, a forking task that has passed
	 * cgroup_post_fork() without seeing use_task_css_set_links = 1
	 * is not guaranteed to have its child immediately visible in the
	 * tasklist if we walk through it with RCU.
	 */
	read_lock(&tasklist_lock);
	do_each_thread(g, p) {
		task_lock(p);
		/*
		 * We should check if the process is exiting, otherwise
		 * it will race with cgroup_exit() in that the list
		 * entry won't be deleted though the process has exited.
		 */
		if (!(p->flags & PF_EXITING) && list_empty(&p->cg_list))
			list_add(&p->cg_list, &p->cgroups->tasks);
		task_unlock(p);
	} while_each_thread(g, p);
	read_unlock(&tasklist_lock);
	write_unlock(&css_set_lock);
}

/**
 * cgroup_next_descendant_pre - find the next descendant for pre-order walk
 * @pos: the current position (%NULL to initiate traversal)
 * @cgroup: cgroup whose descendants to walk
 *
 * To be used by cgroup_for_each_descendant_pre().  Find the next
 * descendant to visit for pre-order traversal of @cgroup's descendants.
 */
struct cgroup *cgroup_next_descendant_pre(struct cgroup *pos,
					  struct cgroup *cgroup)
{
	struct cgroup *next;

	WARN_ON_ONCE(!rcu_read_lock_held());

	/* if first iteration, pretend we just visited @cgroup */
	if (!pos)
		pos = cgroup;

	/* visit the first child if exists */
	next = list_first_or_null_rcu(&pos->children, struct cgroup, sibling);
=======
static int cgroup_task_count(const struct cgroup *cgrp)
{
	int count = 0;
	struct cgrp_cset_link *link;

	down_read(&css_set_rwsem);
	list_for_each_entry(link, &cgrp->cset_links, cset_link)
		count += atomic_read(&link->cset->refcount);
	up_read(&css_set_rwsem);
	return count;
}

/**
 * css_next_child - find the next child of a given css
 * @pos: the current position (%NULL to initiate traversal)
 * @parent: css whose children to walk
 *
 * This function returns the next child of @parent and should be called
 * under either cgroup_mutex or RCU read lock.  The only requirement is
 * that @parent and @pos are accessible.  The next sibling is guaranteed to
 * be returned regardless of their states.
 *
 * If a subsystem synchronizes ->css_online() and the start of iteration, a
 * css which finished ->css_online() is guaranteed to be visible in the
 * future iterations and will stay visible until the last reference is put.
 * A css which hasn't finished ->css_online() or already finished
 * ->css_offline() may show up during traversal.  It's each subsystem's
 * responsibility to synchronize against on/offlining.
 */
struct cgroup_subsys_state *css_next_child(struct cgroup_subsys_state *pos,
					   struct cgroup_subsys_state *parent)
{
	struct cgroup_subsys_state *next;

	cgroup_assert_mutex_or_rcu_locked();

	/*
	 * @pos could already have been unlinked from the sibling list.
	 * Once a cgroup is removed, its ->sibling.next is no longer
	 * updated when its next sibling changes.  CSS_RELEASED is set when
	 * @pos is taken off list, at which time its next pointer is valid,
	 * and, as releases are serialized, the one pointed to by the next
	 * pointer is guaranteed to not have started release yet.  This
	 * implies that if we observe !CSS_RELEASED on @pos in this RCU
	 * critical section, the one pointed to by its next pointer is
	 * guaranteed to not have finished its RCU grace period even if we
	 * have dropped rcu_read_lock() inbetween iterations.
	 *
	 * If @pos has CSS_RELEASED set, its next pointer can't be
	 * dereferenced; however, as each css is given a monotonically
	 * increasing unique serial number and always appended to the
	 * sibling list, the next one can be found by walking the parent's
	 * children until the first css with higher serial number than
	 * @pos's.  While this path can be slower, it happens iff iteration
	 * races against release and the race window is very small.
	 */
	if (!pos) {
		next = list_entry_rcu(parent->children.next, struct cgroup_subsys_state, sibling);
	} else if (likely(!(pos->flags & CSS_RELEASED))) {
		next = list_entry_rcu(pos->sibling.next, struct cgroup_subsys_state, sibling);
	} else {
		list_for_each_entry_rcu(next, &parent->children, sibling)
			if (next->serial_nr > pos->serial_nr)
				break;
	}

	/*
	 * @next, if not pointing to the head, can be dereferenced and is
	 * the next sibling.
	 */
	if (&next->sibling != &parent->children)
		return next;
	return NULL;
}

/**
 * css_next_descendant_pre - find the next descendant for pre-order walk
 * @pos: the current position (%NULL to initiate traversal)
 * @root: css whose descendants to walk
 *
 * To be used by css_for_each_descendant_pre().  Find the next descendant
 * to visit for pre-order traversal of @root's descendants.  @root is
 * included in the iteration and the first node to be visited.
 *
 * While this function requires cgroup_mutex or RCU read locking, it
 * doesn't require the whole traversal to be contained in a single critical
 * section.  This function will return the correct next descendant as long
 * as both @pos and @root are accessible and @pos is a descendant of @root.
 *
 * If a subsystem synchronizes ->css_online() and the start of iteration, a
 * css which finished ->css_online() is guaranteed to be visible in the
 * future iterations and will stay visible until the last reference is put.
 * A css which hasn't finished ->css_online() or already finished
 * ->css_offline() may show up during traversal.  It's each subsystem's
 * responsibility to synchronize against on/offlining.
 */
struct cgroup_subsys_state *
css_next_descendant_pre(struct cgroup_subsys_state *pos,
			struct cgroup_subsys_state *root)
{
	struct cgroup_subsys_state *next;

	cgroup_assert_mutex_or_rcu_locked();

	/* if first iteration, visit @root */
	if (!pos)
		return root;

	/* visit the first child if exists */
	next = css_next_child(NULL, pos);
>>>>>>> v3.18
	if (next)
		return next;

	/* no child, visit my or the closest ancestor's next sibling */
<<<<<<< HEAD
	while (pos != cgroup) {
		next = list_entry_rcu(pos->sibling.next, struct cgroup,
				      sibling);
		if (&next->sibling != &pos->parent->children)
			return next;

=======
	while (pos != root) {
		next = css_next_child(pos, pos->parent);
		if (next)
			return next;
>>>>>>> v3.18
		pos = pos->parent;
	}

	return NULL;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(cgroup_next_descendant_pre);

/**
 * cgroup_rightmost_descendant - return the rightmost descendant of a cgroup
 * @pos: cgroup of interest
 *
 * Return the rightmost descendant of @pos.  If there's no descendant,
 * @pos is returned.  This can be used during pre-order traversal to skip
 * subtree of @pos.
 */
struct cgroup *cgroup_rightmost_descendant(struct cgroup *pos)
{
	struct cgroup *last, *tmp;

	WARN_ON_ONCE(!rcu_read_lock_held());
=======

/**
 * css_rightmost_descendant - return the rightmost descendant of a css
 * @pos: css of interest
 *
 * Return the rightmost descendant of @pos.  If there's no descendant, @pos
 * is returned.  This can be used during pre-order traversal to skip
 * subtree of @pos.
 *
 * While this function requires cgroup_mutex or RCU read locking, it
 * doesn't require the whole traversal to be contained in a single critical
 * section.  This function will return the correct rightmost descendant as
 * long as @pos is accessible.
 */
struct cgroup_subsys_state *
css_rightmost_descendant(struct cgroup_subsys_state *pos)
{
	struct cgroup_subsys_state *last, *tmp;

	cgroup_assert_mutex_or_rcu_locked();
>>>>>>> v3.18

	do {
		last = pos;
		/* ->prev isn't RCU safe, walk ->next till the end */
		pos = NULL;
<<<<<<< HEAD
		list_for_each_entry_rcu(tmp, &last->children, sibling)
=======
		css_for_each_child(tmp, last)
>>>>>>> v3.18
			pos = tmp;
	} while (pos);

	return last;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(cgroup_rightmost_descendant);

static struct cgroup *cgroup_leftmost_descendant(struct cgroup *pos)
{
	struct cgroup *last;

	do {
		last = pos;
		pos = list_first_or_null_rcu(&pos->children, struct cgroup,
					     sibling);
=======

static struct cgroup_subsys_state *
css_leftmost_descendant(struct cgroup_subsys_state *pos)
{
	struct cgroup_subsys_state *last;

	do {
		last = pos;
		pos = css_next_child(NULL, pos);
>>>>>>> v3.18
	} while (pos);

	return last;
}

/**
<<<<<<< HEAD
 * cgroup_next_descendant_post - find the next descendant for post-order walk
 * @pos: the current position (%NULL to initiate traversal)
 * @cgroup: cgroup whose descendants to walk
 *
 * To be used by cgroup_for_each_descendant_post().  Find the next
 * descendant to visit for post-order traversal of @cgroup's descendants.
 */
struct cgroup *cgroup_next_descendant_post(struct cgroup *pos,
					   struct cgroup *cgroup)
{
	struct cgroup *next;

	WARN_ON_ONCE(!rcu_read_lock_held());

	/* if first iteration, visit the leftmost descendant */
	if (!pos) {
		next = cgroup_leftmost_descendant(cgroup);
		return next != cgroup ? next : NULL;
	}

	/* if there's an unvisited sibling, visit its leftmost descendant */
	next = list_entry_rcu(pos->sibling.next, struct cgroup, sibling);
	if (&next->sibling != &pos->parent->children)
		return cgroup_leftmost_descendant(next);

	/* no sibling left, visit parent */
	next = pos->parent;
	return next != cgroup ? next : NULL;
}
EXPORT_SYMBOL_GPL(cgroup_next_descendant_post);

void cgroup_iter_start(struct cgroup *cgrp, struct cgroup_iter *it)
	__acquires(css_set_lock)
{
	/*
	 * The first time anyone tries to iterate across a cgroup,
	 * we need to enable the list linking each css_set to its
	 * tasks, and fix up all existing tasks.
	 */
	if (!use_task_css_set_links)
		cgroup_enable_task_cg_lists();

	read_lock(&css_set_lock);
	it->cg_link = &cgrp->css_sets;
	cgroup_advance_iter(cgrp, it);
}

struct task_struct *cgroup_iter_next(struct cgroup *cgrp,
					struct cgroup_iter *it)
{
	struct task_struct *res;
	struct list_head *l = it->task;
	struct cg_cgroup_link *link;

	/* If the iterator cg is NULL, we have no tasks */
	if (!it->cg_link)
		return NULL;
	res = list_entry(l, struct task_struct, cg_list);
	/* Advance iterator to find next entry */
	l = l->next;
	link = list_entry(it->cg_link, struct cg_cgroup_link, cgrp_link_list);
	if (l == &link->cg->tasks) {
		/* We reached the end of this task list - move on to
		 * the next cg_cgroup_link */
		cgroup_advance_iter(cgrp, it);
	} else {
		it->task = l;
	}
	return res;
}

void cgroup_iter_end(struct cgroup *cgrp, struct cgroup_iter *it)
	__releases(css_set_lock)
{
	read_unlock(&css_set_lock);
}

static inline int started_after_time(struct task_struct *t1,
				     struct timespec *time,
				     struct task_struct *t2)
{
	int start_diff = timespec_compare(&t1->start_time, time);
	if (start_diff > 0) {
		return 1;
	} else if (start_diff < 0) {
		return 0;
	} else {
		/*
		 * Arbitrarily, if two processes started at the same
		 * time, we'll say that the lower pointer value
		 * started first. Note that t2 may have exited by now
		 * so this may not be a valid pointer any longer, but
		 * that's fine - it still serves to distinguish
		 * between two tasks started (effectively) simultaneously.
		 */
		return t1 > t2;
	}
}

/*
 * This function is a callback from heap_insert() and is used to order
 * the heap.
 * In this case we order the heap in descending task start time.
 */
static inline int started_after(void *p1, void *p2)
{
	struct task_struct *t1 = p1;
	struct task_struct *t2 = p2;
	return started_after_time(t1, &t2->start_time, t2);
}

/**
 * cgroup_scan_tasks - iterate though all the tasks in a cgroup
 * @scan: struct cgroup_scanner containing arguments for the scan
 *
 * Arguments include pointers to callback functions test_task() and
 * process_task().
 * Iterate through all the tasks in a cgroup, calling test_task() for each,
 * and if it returns true, call process_task() for it also.
 * The test_task pointer may be NULL, meaning always true (select all tasks).
 * Effectively duplicates cgroup_iter_{start,next,end}()
 * but does not lock css_set_lock for the call to process_task().
 * The struct cgroup_scanner may be embedded in any structure of the caller's
 * creation.
 * It is guaranteed that process_task() will act on every task that
 * is a member of the cgroup for the duration of this call. This
 * function may or may not call process_task() for tasks that exit
 * or move to a different cgroup during the call, or are forked or
 * move into the cgroup during the call.
 *
 * Note that test_task() may be called with locks held, and may in some
 * situations be called multiple times for the same task, so it should
 * be cheap.
 * If the heap pointer in the struct cgroup_scanner is non-NULL, a heap has been
 * pre-allocated and will be used for heap operations (and its "gt" member will
 * be overwritten), else a temporary heap will be used (allocation of which
 * may cause this function to fail).
 */
int cgroup_scan_tasks(struct cgroup_scanner *scan)
{
	int retval, i;
	struct cgroup_iter it;
	struct task_struct *p, *dropped;
	/* Never dereference latest_task, since it's not refcounted */
	struct task_struct *latest_task = NULL;
	struct ptr_heap tmp_heap;
	struct ptr_heap *heap;
	struct timespec latest_time = { 0, 0 };

	if (scan->heap) {
		/* The caller supplied our heap and pre-allocated its memory */
		heap = scan->heap;
		heap->gt = &started_after;
	} else {
		/* We need to allocate our own heap memory */
		heap = &tmp_heap;
		retval = heap_init(heap, PAGE_SIZE, GFP_KERNEL, &started_after);
		if (retval)
			/* cannot allocate the heap */
			return retval;
	}

 again:
	/*
	 * Scan tasks in the cgroup, using the scanner's "test_task" callback
	 * to determine which are of interest, and using the scanner's
	 * "process_task" callback to process any of them that need an update.
	 * Since we don't want to hold any locks during the task updates,
	 * gather tasks to be processed in a heap structure.
	 * The heap is sorted by descending task start time.
	 * If the statically-sized heap fills up, we overflow tasks that
	 * started later, and in future iterations only consider tasks that
	 * started after the latest task in the previous pass. This
	 * guarantees forward progress and that we don't miss any tasks.
	 */
	heap->size = 0;
	cgroup_iter_start(scan->cg, &it);
	while ((p = cgroup_iter_next(scan->cg, &it))) {
		/*
		 * Only affect tasks that qualify per the caller's callback,
		 * if he provided one
		 */
		if (scan->test_task && !scan->test_task(p, scan))
			continue;
		/*
		 * Only process tasks that started after the last task
		 * we processed
		 */
		if (!started_after_time(p, &latest_time, latest_task))
			continue;
		dropped = heap_insert(heap, p);
		if (dropped == NULL) {
			/*
			 * The new task was inserted; the heap wasn't
			 * previously full
			 */
			get_task_struct(p);
		} else if (dropped != p) {
			/*
			 * The new task was inserted, and pushed out a
			 * different task
			 */
			get_task_struct(p);
			put_task_struct(dropped);
		}
		/*
		 * Else the new task was newer than anything already in
		 * the heap and wasn't inserted
		 */
	}
	cgroup_iter_end(scan->cg, &it);

	if (heap->size) {
		for (i = 0; i < heap->size; i++) {
			struct task_struct *q = heap->ptrs[i];
			if (i == 0) {
				latest_time = q->start_time;
				latest_task = q;
			}
			/* Process the task per the caller's callback */
			scan->process_task(q, scan);
			put_task_struct(q);
		}
		/*
		 * If we had to process any tasks at all, scan again
		 * in case some of them were in the middle of forking
		 * children that didn't get processed.
		 * Not the most efficient way to do it, but it avoids
		 * having to take callback_mutex in the fork path
		 */
		goto again;
	}
	if (heap == &tmp_heap)
		heap_free(&tmp_heap);
	return 0;
}

static void cgroup_transfer_one_task(struct task_struct *task,
				     struct cgroup_scanner *scan)
{
	struct cgroup *new_cgroup = scan->data;

	mutex_lock(&cgroup_mutex);
	cgroup_attach_task(new_cgroup, task, false);
	mutex_unlock(&cgroup_mutex);
=======
 * css_next_descendant_post - find the next descendant for post-order walk
 * @pos: the current position (%NULL to initiate traversal)
 * @root: css whose descendants to walk
 *
 * To be used by css_for_each_descendant_post().  Find the next descendant
 * to visit for post-order traversal of @root's descendants.  @root is
 * included in the iteration and the last node to be visited.
 *
 * While this function requires cgroup_mutex or RCU read locking, it
 * doesn't require the whole traversal to be contained in a single critical
 * section.  This function will return the correct next descendant as long
 * as both @pos and @cgroup are accessible and @pos is a descendant of
 * @cgroup.
 *
 * If a subsystem synchronizes ->css_online() and the start of iteration, a
 * css which finished ->css_online() is guaranteed to be visible in the
 * future iterations and will stay visible until the last reference is put.
 * A css which hasn't finished ->css_online() or already finished
 * ->css_offline() may show up during traversal.  It's each subsystem's
 * responsibility to synchronize against on/offlining.
 */
struct cgroup_subsys_state *
css_next_descendant_post(struct cgroup_subsys_state *pos,
			 struct cgroup_subsys_state *root)
{
	struct cgroup_subsys_state *next;

	cgroup_assert_mutex_or_rcu_locked();

	/* if first iteration, visit leftmost descendant which may be @root */
	if (!pos)
		return css_leftmost_descendant(root);

	/* if we visited @root, we're done */
	if (pos == root)
		return NULL;

	/* if there's an unvisited sibling, visit its leftmost descendant */
	next = css_next_child(pos, pos->parent);
	if (next)
		return css_leftmost_descendant(next);

	/* no sibling left, visit parent */
	return pos->parent;
}

/**
 * css_has_online_children - does a css have online children
 * @css: the target css
 *
 * Returns %true if @css has any online children; otherwise, %false.  This
 * function can be called from any context but the caller is responsible
 * for synchronizing against on/offlining as necessary.
 */
bool css_has_online_children(struct cgroup_subsys_state *css)
{
	struct cgroup_subsys_state *child;
	bool ret = false;

	rcu_read_lock();
	css_for_each_child(child, css) {
		if (child->flags & CSS_ONLINE) {
			ret = true;
			break;
		}
	}
	rcu_read_unlock();
	return ret;
}

/**
 * css_advance_task_iter - advance a task itererator to the next css_set
 * @it: the iterator to advance
 *
 * Advance @it to the next css_set to walk.
 */
static void css_advance_task_iter(struct css_task_iter *it)
{
	struct list_head *l = it->cset_pos;
	struct cgrp_cset_link *link;
	struct css_set *cset;

	/* Advance to the next non-empty css_set */
	do {
		l = l->next;
		if (l == it->cset_head) {
			it->cset_pos = NULL;
			return;
		}

		if (it->ss) {
			cset = container_of(l, struct css_set,
					    e_cset_node[it->ss->id]);
		} else {
			link = list_entry(l, struct cgrp_cset_link, cset_link);
			cset = link->cset;
		}
	} while (list_empty(&cset->tasks) && list_empty(&cset->mg_tasks));

	it->cset_pos = l;

	if (!list_empty(&cset->tasks))
		it->task_pos = cset->tasks.next;
	else
		it->task_pos = cset->mg_tasks.next;

	it->tasks_head = &cset->tasks;
	it->mg_tasks_head = &cset->mg_tasks;
}

/**
 * css_task_iter_start - initiate task iteration
 * @css: the css to walk tasks of
 * @it: the task iterator to use
 *
 * Initiate iteration through the tasks of @css.  The caller can call
 * css_task_iter_next() to walk through the tasks until the function
 * returns NULL.  On completion of iteration, css_task_iter_end() must be
 * called.
 *
 * Note that this function acquires a lock which is released when the
 * iteration finishes.  The caller can't sleep while iteration is in
 * progress.
 */
void css_task_iter_start(struct cgroup_subsys_state *css,
			 struct css_task_iter *it)
	__acquires(css_set_rwsem)
{
	/* no one should try to iterate before mounting cgroups */
	WARN_ON_ONCE(!use_task_css_set_links);

	down_read(&css_set_rwsem);

	it->ss = css->ss;

	if (it->ss)
		it->cset_pos = &css->cgroup->e_csets[css->ss->id];
	else
		it->cset_pos = &css->cgroup->cset_links;

	it->cset_head = it->cset_pos;

	css_advance_task_iter(it);
}

/**
 * css_task_iter_next - return the next task for the iterator
 * @it: the task iterator being iterated
 *
 * The "next" function for task iteration.  @it should have been
 * initialized via css_task_iter_start().  Returns NULL when the iteration
 * reaches the end.
 */
struct task_struct *css_task_iter_next(struct css_task_iter *it)
{
	struct task_struct *res;
	struct list_head *l = it->task_pos;

	/* If the iterator cg is NULL, we have no tasks */
	if (!it->cset_pos)
		return NULL;
	res = list_entry(l, struct task_struct, cg_list);

	/*
	 * Advance iterator to find next entry.  cset->tasks is consumed
	 * first and then ->mg_tasks.  After ->mg_tasks, we move onto the
	 * next cset.
	 */
	l = l->next;

	if (l == it->tasks_head)
		l = it->mg_tasks_head->next;

	if (l == it->mg_tasks_head)
		css_advance_task_iter(it);
	else
		it->task_pos = l;

	return res;
}

/**
 * css_task_iter_end - finish task iteration
 * @it: the task iterator to finish
 *
 * Finish task iteration started by css_task_iter_start().
 */
void css_task_iter_end(struct css_task_iter *it)
	__releases(css_set_rwsem)
{
	up_read(&css_set_rwsem);
>>>>>>> v3.18
}

/**
 * cgroup_trasnsfer_tasks - move tasks from one cgroup to another
 * @to: cgroup to which the tasks will be moved
 * @from: cgroup in which the tasks currently reside
<<<<<<< HEAD
 */
int cgroup_transfer_tasks(struct cgroup *to, struct cgroup *from)
{
	struct cgroup_scanner scan;

	scan.cg = from;
	scan.test_task = NULL; /* select all tasks in cgroup */
	scan.process_task = cgroup_transfer_one_task;
	scan.heap = NULL;
	scan.data = to;

	return cgroup_scan_tasks(&scan);
=======
 *
 * Locking rules between cgroup_post_fork() and the migration path
 * guarantee that, if a task is forking while being migrated, the new child
 * is guaranteed to be either visible in the source cgroup after the
 * parent's migration is complete or put into the target cgroup.  No task
 * can slip out of migration through forking.
 */
int cgroup_transfer_tasks(struct cgroup *to, struct cgroup *from)
{
	LIST_HEAD(preloaded_csets);
	struct cgrp_cset_link *link;
	struct css_task_iter it;
	struct task_struct *task;
	int ret;

	mutex_lock(&cgroup_mutex);

	/* all tasks in @from are being moved, all csets are source */
	down_read(&css_set_rwsem);
	list_for_each_entry(link, &from->cset_links, cset_link)
		cgroup_migrate_add_src(link->cset, to, &preloaded_csets);
	up_read(&css_set_rwsem);

	ret = cgroup_migrate_prepare_dst(to, &preloaded_csets);
	if (ret)
		goto out_err;

	/*
	 * Migrate tasks one-by-one until @form is empty.  This fails iff
	 * ->can_attach() fails.
	 */
	do {
		css_task_iter_start(&from->self, &it);
		task = css_task_iter_next(&it);
		if (task)
			get_task_struct(task);
		css_task_iter_end(&it);

		if (task) {
			ret = cgroup_migrate(to, task, false);
			put_task_struct(task);
		}
	} while (task && !ret);
out_err:
	cgroup_migrate_finish(&preloaded_csets);
	mutex_unlock(&cgroup_mutex);
	return ret;
>>>>>>> v3.18
}

/*
 * Stuff for reading the 'tasks'/'procs' files.
 *
 * Reading this file can return large amounts of data if a cgroup has
 * *lots* of attached tasks. So it may need several calls to read(),
 * but we cannot guarantee that the information we produce is correct
 * unless we produce it entirely atomically.
 *
 */

/* which pidlist file are we talking about? */
enum cgroup_filetype {
	CGROUP_FILE_PROCS,
	CGROUP_FILE_TASKS,
};

/*
 * A pidlist is a list of pids that virtually represents the contents of one
 * of the cgroup files ("procs" or "tasks"). We keep a list of such pidlists,
 * a pair (one each for procs, tasks) for each pid namespace that's relevant
 * to the cgroup.
 */
struct cgroup_pidlist {
	/*
	 * used to find which pidlist is wanted. doesn't change as long as
	 * this particular list stays in the list.
	*/
	struct { enum cgroup_filetype type; struct pid_namespace *ns; } key;
	/* array of xids */
	pid_t *list;
	/* how many elements the above list has */
	int length;
<<<<<<< HEAD
	/* how many files are using the current array */
	int use_count;
=======
>>>>>>> v3.18
	/* each of these stored in a list by its cgroup */
	struct list_head links;
	/* pointer to the cgroup we belong to, for list removal purposes */
	struct cgroup *owner;
<<<<<<< HEAD
	/* protects the other fields */
	struct rw_semaphore mutex;
=======
	/* for delayed destruction */
	struct delayed_work destroy_dwork;
>>>>>>> v3.18
};

/*
 * The following two functions "fix" the issue where there are more pids
 * than kmalloc will give memory for; in such cases, we use vmalloc/vfree.
 * TODO: replace with a kernel-wide solution to this problem
 */
#define PIDLIST_TOO_LARGE(c) ((c) * sizeof(pid_t) > (PAGE_SIZE * 2))
static void *pidlist_allocate(int count)
{
	if (PIDLIST_TOO_LARGE(count))
		return vmalloc(count * sizeof(pid_t));
	else
		return kmalloc(count * sizeof(pid_t), GFP_KERNEL);
}
<<<<<<< HEAD
=======

>>>>>>> v3.18
static void pidlist_free(void *p)
{
	if (is_vmalloc_addr(p))
		vfree(p);
	else
		kfree(p);
}

/*
<<<<<<< HEAD
=======
 * Used to destroy all pidlists lingering waiting for destroy timer.  None
 * should be left afterwards.
 */
static void cgroup_pidlist_destroy_all(struct cgroup *cgrp)
{
	struct cgroup_pidlist *l, *tmp_l;

	mutex_lock(&cgrp->pidlist_mutex);
	list_for_each_entry_safe(l, tmp_l, &cgrp->pidlists, links)
		mod_delayed_work(cgroup_pidlist_destroy_wq, &l->destroy_dwork, 0);
	mutex_unlock(&cgrp->pidlist_mutex);

	flush_workqueue(cgroup_pidlist_destroy_wq);
	BUG_ON(!list_empty(&cgrp->pidlists));
}

static void cgroup_pidlist_destroy_work_fn(struct work_struct *work)
{
	struct delayed_work *dwork = to_delayed_work(work);
	struct cgroup_pidlist *l = container_of(dwork, struct cgroup_pidlist,
						destroy_dwork);
	struct cgroup_pidlist *tofree = NULL;

	mutex_lock(&l->owner->pidlist_mutex);

	/*
	 * Destroy iff we didn't get queued again.  The state won't change
	 * as destroy_dwork can only be queued while locked.
	 */
	if (!delayed_work_pending(dwork)) {
		list_del(&l->links);
		pidlist_free(l->list);
		put_pid_ns(l->key.ns);
		tofree = l;
	}

	mutex_unlock(&l->owner->pidlist_mutex);
	kfree(tofree);
}

/*
>>>>>>> v3.18
 * pidlist_uniq - given a kmalloc()ed list, strip out all duplicate entries
 * Returns the number of unique elements.
 */
static int pidlist_uniq(pid_t *list, int length)
{
	int src, dest = 1;

	/*
	 * we presume the 0th element is unique, so i starts at 1. trivial
	 * edge cases first; no work needs to be done for either
	 */
	if (length == 0 || length == 1)
		return length;
	/* src and dest walk down the list; dest counts unique elements */
	for (src = 1; src < length; src++) {
		/* find next unique element */
		while (list[src] == list[src-1]) {
			src++;
			if (src == length)
				goto after;
		}
		/* dest always points to where the next unique element goes */
		list[dest] = list[src];
		dest++;
	}
after:
	return dest;
}

<<<<<<< HEAD
=======
/*
 * The two pid files - task and cgroup.procs - guaranteed that the result
 * is sorted, which forced this whole pidlist fiasco.  As pid order is
 * different per namespace, each namespace needs differently sorted list,
 * making it impossible to use, for example, single rbtree of member tasks
 * sorted by task pointer.  As pidlists can be fairly large, allocating one
 * per open file is dangerous, so cgroup had to implement shared pool of
 * pidlists keyed by cgroup and namespace.
 *
 * All this extra complexity was caused by the original implementation
 * committing to an entirely unnecessary property.  In the long term, we
 * want to do away with it.  Explicitly scramble sort order if on the
 * default hierarchy so that no such expectation exists in the new
 * interface.
 *
 * Scrambling is done by swapping every two consecutive bits, which is
 * non-identity one-to-one mapping which disturbs sort order sufficiently.
 */
static pid_t pid_fry(pid_t pid)
{
	unsigned a = pid & 0x55555555;
	unsigned b = pid & 0xAAAAAAAA;

	return (a << 1) | (b >> 1);
}

static pid_t cgroup_pid_fry(struct cgroup *cgrp, pid_t pid)
{
	if (cgroup_on_dfl(cgrp))
		return pid_fry(pid);
	else
		return pid;
}

>>>>>>> v3.18
static int cmppid(const void *a, const void *b)
{
	return *(pid_t *)a - *(pid_t *)b;
}

<<<<<<< HEAD
=======
static int fried_cmppid(const void *a, const void *b)
{
	return pid_fry(*(pid_t *)a) - pid_fry(*(pid_t *)b);
}

static struct cgroup_pidlist *cgroup_pidlist_find(struct cgroup *cgrp,
						  enum cgroup_filetype type)
{
	struct cgroup_pidlist *l;
	/* don't need task_nsproxy() if we're looking at ourself */
	struct pid_namespace *ns = task_active_pid_ns(current);

	lockdep_assert_held(&cgrp->pidlist_mutex);

	list_for_each_entry(l, &cgrp->pidlists, links)
		if (l->key.type == type && l->key.ns == ns)
			return l;
	return NULL;
}

>>>>>>> v3.18
/*
 * find the appropriate pidlist for our purpose (given procs vs tasks)
 * returns with the lock on that pidlist already held, and takes care
 * of the use count, or returns NULL with no locks held if we're out of
 * memory.
 */
<<<<<<< HEAD
static struct cgroup_pidlist *cgroup_pidlist_find(struct cgroup *cgrp,
						  enum cgroup_filetype type)
{
	struct cgroup_pidlist *l;
	/* don't need task_nsproxy() if we're looking at ourself */
	struct pid_namespace *ns = task_active_pid_ns(current);

	/*
	 * We can't drop the pidlist_mutex before taking the l->mutex in case
	 * the last ref-holder is trying to remove l from the list at the same
	 * time. Holding the pidlist_mutex precludes somebody taking whichever
	 * list we find out from under us - compare release_pid_array().
	 */
	mutex_lock(&cgrp->pidlist_mutex);
	list_for_each_entry(l, &cgrp->pidlists, links) {
		if (l->key.type == type && l->key.ns == ns) {
			/* make sure l doesn't vanish out from under us */
			down_write(&l->mutex);
			mutex_unlock(&cgrp->pidlist_mutex);
			return l;
		}
	}
	/* entry not found; create a new one */
	l = kmalloc(sizeof(struct cgroup_pidlist), GFP_KERNEL);
	if (!l) {
		mutex_unlock(&cgrp->pidlist_mutex);
		return l;
	}
	init_rwsem(&l->mutex);
	down_write(&l->mutex);
	l->key.type = type;
	l->key.ns = get_pid_ns(ns);
	l->use_count = 0; /* don't increment here */
	l->list = NULL;
	l->owner = cgrp;
	list_add(&l->links, &cgrp->pidlists);
	mutex_unlock(&cgrp->pidlist_mutex);
=======
static struct cgroup_pidlist *cgroup_pidlist_find_create(struct cgroup *cgrp,
						enum cgroup_filetype type)
{
	struct cgroup_pidlist *l;

	lockdep_assert_held(&cgrp->pidlist_mutex);

	l = cgroup_pidlist_find(cgrp, type);
	if (l)
		return l;

	/* entry not found; create a new one */
	l = kzalloc(sizeof(struct cgroup_pidlist), GFP_KERNEL);
	if (!l)
		return l;

	INIT_DELAYED_WORK(&l->destroy_dwork, cgroup_pidlist_destroy_work_fn);
	l->key.type = type;
	/* don't need task_nsproxy() if we're looking at ourself */
	l->key.ns = get_pid_ns(task_active_pid_ns(current));
	l->owner = cgrp;
	list_add(&l->links, &cgrp->pidlists);
>>>>>>> v3.18
	return l;
}

/*
 * Load a cgroup's pidarray with either procs' tgids or tasks' pids
 */
static int pidlist_array_load(struct cgroup *cgrp, enum cgroup_filetype type,
			      struct cgroup_pidlist **lp)
{
	pid_t *array;
	int length;
	int pid, n = 0; /* used for populating the array */
<<<<<<< HEAD
	struct cgroup_iter it;
	struct task_struct *tsk;
	struct cgroup_pidlist *l;

=======
	struct css_task_iter it;
	struct task_struct *tsk;
	struct cgroup_pidlist *l;

	lockdep_assert_held(&cgrp->pidlist_mutex);

>>>>>>> v3.18
	/*
	 * If cgroup gets more users after we read count, we won't have
	 * enough space - tough.  This race is indistinguishable to the
	 * caller from the case that the additional cgroup users didn't
	 * show up until sometime later on.
	 */
	length = cgroup_task_count(cgrp);
	array = pidlist_allocate(length);
	if (!array)
		return -ENOMEM;
	/* now, populate the array */
<<<<<<< HEAD
	cgroup_iter_start(cgrp, &it);
	while ((tsk = cgroup_iter_next(cgrp, &it))) {
=======
	css_task_iter_start(&cgrp->self, &it);
	while ((tsk = css_task_iter_next(&it))) {
>>>>>>> v3.18
		if (unlikely(n == length))
			break;
		/* get tgid or pid for procs or tasks file respectively */
		if (type == CGROUP_FILE_PROCS)
			pid = task_tgid_vnr(tsk);
		else
			pid = task_pid_vnr(tsk);
		if (pid > 0) /* make sure to only use valid results */
			array[n++] = pid;
	}
<<<<<<< HEAD
	cgroup_iter_end(cgrp, &it);
	length = n;
	/* now sort & (if procs) strip out duplicates */
	sort(array, length, sizeof(pid_t), cmppid, NULL);
	if (type == CGROUP_FILE_PROCS)
		length = pidlist_uniq(array, length);
	l = cgroup_pidlist_find(cgrp, type);
=======
	css_task_iter_end(&it);
	length = n;
	/* now sort & (if procs) strip out duplicates */
	if (cgroup_on_dfl(cgrp))
		sort(array, length, sizeof(pid_t), fried_cmppid, NULL);
	else
		sort(array, length, sizeof(pid_t), cmppid, NULL);
	if (type == CGROUP_FILE_PROCS)
		length = pidlist_uniq(array, length);

	l = cgroup_pidlist_find_create(cgrp, type);
>>>>>>> v3.18
	if (!l) {
		pidlist_free(array);
		return -ENOMEM;
	}
<<<<<<< HEAD
	/* store array, freeing old if necessary - lock already held */
	pidlist_free(l->list);
	l->list = array;
	l->length = length;
	l->use_count++;
	up_write(&l->mutex);
=======

	/* store array, freeing old if necessary */
	pidlist_free(l->list);
	l->list = array;
	l->length = length;
>>>>>>> v3.18
	*lp = l;
	return 0;
}

/**
 * cgroupstats_build - build and fill cgroupstats
 * @stats: cgroupstats to fill information into
 * @dentry: A dentry entry belonging to the cgroup for which stats have
 * been requested.
 *
 * Build and fill cgroupstats so that taskstats can export it to user
 * space.
 */
int cgroupstats_build(struct cgroupstats *stats, struct dentry *dentry)
{
<<<<<<< HEAD
	int ret = -EINVAL;
	struct cgroup *cgrp;
	struct cgroup_iter it;
	struct task_struct *tsk;

	/*
	 * Validate dentry by checking the superblock operations,
	 * and make sure it's a directory.
	 */
	if (dentry->d_sb->s_op != &cgroup_ops ||
	    !S_ISDIR(dentry->d_inode->i_mode))
		 goto err;

	ret = 0;
	cgrp = dentry->d_fsdata;

	cgroup_iter_start(cgrp, &it);
	while ((tsk = cgroup_iter_next(cgrp, &it))) {
=======
	struct kernfs_node *kn = kernfs_node_from_dentry(dentry);
	struct cgroup *cgrp;
	struct css_task_iter it;
	struct task_struct *tsk;

	/* it should be kernfs_node belonging to cgroupfs and is a directory */
	if (dentry->d_sb->s_type != &cgroup_fs_type || !kn ||
	    kernfs_type(kn) != KERNFS_DIR)
		return -EINVAL;

	mutex_lock(&cgroup_mutex);

	/*
	 * We aren't being called from kernfs and there's no guarantee on
	 * @kn->priv's validity.  For this and css_tryget_online_from_dir(),
	 * @kn->priv is RCU safe.  Let's do the RCU dancing.
	 */
	rcu_read_lock();
	cgrp = rcu_dereference(kn->priv);
	if (!cgrp || cgroup_is_dead(cgrp)) {
		rcu_read_unlock();
		mutex_unlock(&cgroup_mutex);
		return -ENOENT;
	}
	rcu_read_unlock();

	css_task_iter_start(&cgrp->self, &it);
	while ((tsk = css_task_iter_next(&it))) {
>>>>>>> v3.18
		switch (tsk->state) {
		case TASK_RUNNING:
			stats->nr_running++;
			break;
		case TASK_INTERRUPTIBLE:
			stats->nr_sleeping++;
			break;
		case TASK_UNINTERRUPTIBLE:
			stats->nr_uninterruptible++;
			break;
		case TASK_STOPPED:
			stats->nr_stopped++;
			break;
		default:
			if (delayacct_is_task_waiting_on_io(tsk))
				stats->nr_io_wait++;
			break;
		}
	}
<<<<<<< HEAD
	cgroup_iter_end(cgrp, &it);

err:
	return ret;
=======
	css_task_iter_end(&it);

	mutex_unlock(&cgroup_mutex);
	return 0;
>>>>>>> v3.18
}


/*
 * seq_file methods for the tasks/procs files. The seq_file position is the
 * next pid to display; the seq_file iterator is a pointer to the pid
 * in the cgroup->l->list array.
 */

static void *cgroup_pidlist_start(struct seq_file *s, loff_t *pos)
{
	/*
	 * Initially we receive a position value that corresponds to
	 * one more than the last pid shown (or 0 on the first call or
	 * after a seek to the start). Use a binary-search to find the
	 * next pid to display, if any
	 */
<<<<<<< HEAD
	struct cgroup_pidlist *l = s->private;
	int index = 0, pid = *pos;
	int *iter;

	down_read(&l->mutex);
=======
	struct kernfs_open_file *of = s->private;
	struct cgroup *cgrp = seq_css(s)->cgroup;
	struct cgroup_pidlist *l;
	enum cgroup_filetype type = seq_cft(s)->private;
	int index = 0, pid = *pos;
	int *iter, ret;

	mutex_lock(&cgrp->pidlist_mutex);

	/*
	 * !NULL @of->priv indicates that this isn't the first start()
	 * after open.  If the matching pidlist is around, we can use that.
	 * Look for it.  Note that @of->priv can't be used directly.  It
	 * could already have been destroyed.
	 */
	if (of->priv)
		of->priv = cgroup_pidlist_find(cgrp, type);

	/*
	 * Either this is the first start() after open or the matching
	 * pidlist has been destroyed inbetween.  Create a new one.
	 */
	if (!of->priv) {
		ret = pidlist_array_load(cgrp, type,
					 (struct cgroup_pidlist **)&of->priv);
		if (ret)
			return ERR_PTR(ret);
	}
	l = of->priv;

>>>>>>> v3.18
	if (pid) {
		int end = l->length;

		while (index < end) {
			int mid = (index + end) / 2;
<<<<<<< HEAD
			if (l->list[mid] == pid) {
				index = mid;
				break;
			} else if (l->list[mid] <= pid)
=======
			if (cgroup_pid_fry(cgrp, l->list[mid]) == pid) {
				index = mid;
				break;
			} else if (cgroup_pid_fry(cgrp, l->list[mid]) <= pid)
>>>>>>> v3.18
				index = mid + 1;
			else
				end = mid;
		}
	}
	/* If we're off the end of the array, we're done */
	if (index >= l->length)
		return NULL;
	/* Update the abstract position to be the actual pid that we found */
	iter = l->list + index;
<<<<<<< HEAD
	*pos = *iter;
=======
	*pos = cgroup_pid_fry(cgrp, *iter);
>>>>>>> v3.18
	return iter;
}

static void cgroup_pidlist_stop(struct seq_file *s, void *v)
{
<<<<<<< HEAD
	struct cgroup_pidlist *l = s->private;
	up_read(&l->mutex);
=======
	struct kernfs_open_file *of = s->private;
	struct cgroup_pidlist *l = of->priv;

	if (l)
		mod_delayed_work(cgroup_pidlist_destroy_wq, &l->destroy_dwork,
				 CGROUP_PIDLIST_DESTROY_DELAY);
	mutex_unlock(&seq_css(s)->cgroup->pidlist_mutex);
>>>>>>> v3.18
}

static void *cgroup_pidlist_next(struct seq_file *s, void *v, loff_t *pos)
{
<<<<<<< HEAD
	struct cgroup_pidlist *l = s->private;
=======
	struct kernfs_open_file *of = s->private;
	struct cgroup_pidlist *l = of->priv;
>>>>>>> v3.18
	pid_t *p = v;
	pid_t *end = l->list + l->length;
	/*
	 * Advance to the next pid in the array. If this goes off the
	 * end, we're done
	 */
	p++;
	if (p >= end) {
		return NULL;
	} else {
<<<<<<< HEAD
		*pos = *p;
=======
		*pos = cgroup_pid_fry(seq_css(s)->cgroup, *p);
>>>>>>> v3.18
		return p;
	}
}

static int cgroup_pidlist_show(struct seq_file *s, void *v)
{
	return seq_printf(s, "%d\n", *(int *)v);
}

<<<<<<< HEAD
/*
 * seq_operations functions for iterating on pidlists through seq_file -
 * independent of whether it's tasks or procs
 */
static const struct seq_operations cgroup_pidlist_seq_operations = {
	.start = cgroup_pidlist_start,
	.stop = cgroup_pidlist_stop,
	.next = cgroup_pidlist_next,
	.show = cgroup_pidlist_show,
};

static void cgroup_release_pid_array(struct cgroup_pidlist *l)
{
	/*
	 * the case where we're the last user of this particular pidlist will
	 * have us remove it from the cgroup's list, which entails taking the
	 * mutex. since in pidlist_find the pidlist->lock depends on cgroup->
	 * pidlist_mutex, we have to take pidlist_mutex first.
	 */
	mutex_lock(&l->owner->pidlist_mutex);
	down_write(&l->mutex);
	BUG_ON(!l->use_count);
	if (!--l->use_count) {
		/* we're the last user if refcount is 0; remove and free */
		list_del(&l->links);
		mutex_unlock(&l->owner->pidlist_mutex);
		pidlist_free(l->list);
		put_pid_ns(l->key.ns);
		up_write(&l->mutex);
		kfree(l);
		return;
	}
	mutex_unlock(&l->owner->pidlist_mutex);
	up_write(&l->mutex);
}

static int cgroup_pidlist_release(struct inode *inode, struct file *file)
{
	struct cgroup_pidlist *l;
	if (!(file->f_mode & FMODE_READ))
		return 0;
	/*
	 * the seq_file will only be initialized if the file was opened for
	 * reading; hence we check if it's not null only in that case.
	 */
	l = ((struct seq_file *)file->private_data)->private;
	cgroup_release_pid_array(l);
	return seq_release(inode, file);
}

static const struct file_operations cgroup_pidlist_operations = {
	.read = seq_read,
	.llseek = seq_lseek,
	.write = cgroup_file_write,
	.release = cgroup_pidlist_release,
};

/*
 * The following functions handle opens on a file that displays a pidlist
 * (tasks or procs). Prepare an array of the process/thread IDs of whoever's
 * in the cgroup.
 */
/* helper function for the two below it */
static int cgroup_pidlist_open(struct file *file, enum cgroup_filetype type)
{
	struct cgroup *cgrp = __d_cgrp(file->f_dentry->d_parent);
	struct cgroup_pidlist *l;
	int retval;

	/* Nothing to do for write-only files */
	if (!(file->f_mode & FMODE_READ))
		return 0;

	/* have the array populated */
	retval = pidlist_array_load(cgrp, type, &l);
	if (retval)
		return retval;
	/* configure file information */
	file->f_op = &cgroup_pidlist_operations;

	retval = seq_open(file, &cgroup_pidlist_seq_operations);
	if (retval) {
		cgroup_release_pid_array(l);
		return retval;
	}
	((struct seq_file *)file->private_data)->private = l;
	return 0;
}
static int cgroup_tasks_open(struct inode *unused, struct file *file)
{
	return cgroup_pidlist_open(file, CGROUP_FILE_TASKS);
}
static int cgroup_procs_open(struct inode *unused, struct file *file)
{
	return cgroup_pidlist_open(file, CGROUP_FILE_PROCS);
}

static u64 cgroup_read_notify_on_release(struct cgroup *cgrp,
					    struct cftype *cft)
{
	return notify_on_release(cgrp);
}

static int cgroup_write_notify_on_release(struct cgroup *cgrp,
					  struct cftype *cft,
					  u64 val)
{
	clear_bit(CGRP_RELEASABLE, &cgrp->flags);
	if (val)
		set_bit(CGRP_NOTIFY_ON_RELEASE, &cgrp->flags);
	else
		clear_bit(CGRP_NOTIFY_ON_RELEASE, &cgrp->flags);
	return 0;
}

/*
 * When dput() is called asynchronously, if umount has been done and
 * then deactivate_super() in cgroup_free_fn() kills the superblock,
 * there's a small window that vfs will see the root dentry with non-zero
 * refcnt and trigger BUG().
 *
 * That's why we hold a reference before dput() and drop it right after.
 */
static void cgroup_dput(struct cgroup *cgrp)
{
	struct super_block *sb = cgrp->root->sb;

	atomic_inc(&sb->s_active);
	dput(cgrp->dentry);
	deactivate_super(sb);
}

/*
 * Unregister event and free resources.
 *
 * Gets called from workqueue.
 */
static void cgroup_event_remove(struct work_struct *work)
{
	struct cgroup_event *event = container_of(work, struct cgroup_event,
			remove);
	struct cgroup *cgrp = event->cgrp;

	remove_wait_queue(event->wqh, &event->wait);

	event->cft->unregister_event(cgrp, event->cft, event->eventfd);

	/* Notify userspace the event is going away. */
	eventfd_signal(event->eventfd, 1);

	eventfd_ctx_put(event->eventfd);
	kfree(event);
	cgroup_dput(cgrp);
}

/*
 * Gets called on POLLHUP on eventfd when user closes it.
 *
 * Called with wqh->lock held and interrupts disabled.
 */
static int cgroup_event_wake(wait_queue_t *wait, unsigned mode,
		int sync, void *key)
{
	struct cgroup_event *event = container_of(wait,
			struct cgroup_event, wait);
	struct cgroup *cgrp = event->cgrp;
	unsigned long flags = (unsigned long)key;

	if (flags & POLLHUP) {
		/*
		 * If the event has been detached at cgroup removal, we
		 * can simply return knowing the other side will cleanup
		 * for us.
		 *
		 * We can't race against event freeing since the other
		 * side will require wqh->lock via remove_wait_queue(),
		 * which we hold.
		 */
		spin_lock(&cgrp->event_list_lock);
		if (!list_empty(&event->list)) {
			list_del_init(&event->list);
			/*
			 * We are in atomic context, but cgroup_event_remove()
			 * may sleep, so we have to call it in workqueue.
			 */
			schedule_work(&event->remove);
		}
		spin_unlock(&cgrp->event_list_lock);
	}

	return 0;
}

static void cgroup_event_ptable_queue_proc(struct file *file,
		wait_queue_head_t *wqh, poll_table *pt)
{
	struct cgroup_event *event = container_of(pt,
			struct cgroup_event, pt);

	event->wqh = wqh;
	add_wait_queue(wqh, &event->wait);
}

/*
 * Parse input and register new cgroup event handler.
 *
 * Input must be in format '<event_fd> <control_fd> <args>'.
 * Interpretation of args is defined by control file implementation.
 */
static int cgroup_write_event_control(struct cgroup *cgrp, struct cftype *cft,
				      const char *buffer)
{
	struct cgroup_event *event = NULL;
	struct cgroup *cgrp_cfile;
	unsigned int efd, cfd;
	struct file *efile = NULL;
	struct file *cfile = NULL;
	char *endp;
	int ret;

	efd = simple_strtoul(buffer, &endp, 10);
	if (*endp != ' ')
		return -EINVAL;
	buffer = endp + 1;

	cfd = simple_strtoul(buffer, &endp, 10);
	if ((*endp != ' ') && (*endp != '\0'))
		return -EINVAL;
	buffer = endp + 1;

	event = kzalloc(sizeof(*event), GFP_KERNEL);
	if (!event)
		return -ENOMEM;
	event->cgrp = cgrp;
	INIT_LIST_HEAD(&event->list);
	init_poll_funcptr(&event->pt, cgroup_event_ptable_queue_proc);
	init_waitqueue_func_entry(&event->wait, cgroup_event_wake);
	INIT_WORK(&event->remove, cgroup_event_remove);

	efile = eventfd_fget(efd);
	if (IS_ERR(efile)) {
		ret = PTR_ERR(efile);
		goto fail;
	}

	event->eventfd = eventfd_ctx_fileget(efile);
	if (IS_ERR(event->eventfd)) {
		ret = PTR_ERR(event->eventfd);
		goto fail;
	}

	cfile = fget(cfd);
	if (!cfile) {
		ret = -EBADF;
		goto fail;
	}

	/* the process need read permission on control file */
	/* AV: shouldn't we check that it's been opened for read instead? */
	ret = inode_permission(file_inode(cfile), MAY_READ);
	if (ret < 0)
		goto fail;

	event->cft = __file_cft(cfile);
	if (IS_ERR(event->cft)) {
		ret = PTR_ERR(event->cft);
		goto fail;
	}

	/*
	 * The file to be monitored must be in the same cgroup as
	 * cgroup.event_control is.
	 */
	cgrp_cfile = __d_cgrp(cfile->f_dentry->d_parent);
	if (cgrp_cfile != cgrp) {
		ret = -EINVAL;
		goto fail;
	}

	if (!event->cft->register_event || !event->cft->unregister_event) {
		ret = -EINVAL;
		goto fail;
	}

	ret = event->cft->register_event(cgrp, event->cft,
			event->eventfd, buffer);
	if (ret)
		goto fail;

	efile->f_op->poll(efile, &event->pt);

	/*
	 * Events should be removed after rmdir of cgroup directory, but before
	 * destroying subsystem state objects. Let's take reference to cgroup
	 * directory dentry to do that.
	 */
	dget(cgrp->dentry);

	spin_lock(&cgrp->event_list_lock);
	list_add(&event->list, &cgrp->event_list);
	spin_unlock(&cgrp->event_list_lock);

	fput(cfile);
	fput(efile);

	return 0;

fail:
	if (cfile)
		fput(cfile);

	if (event && event->eventfd && !IS_ERR(event->eventfd))
		eventfd_ctx_put(event->eventfd);

	if (!IS_ERR_OR_NULL(efile))
		fput(efile);

	kfree(event);

	return ret;
}

static u64 cgroup_clone_children_read(struct cgroup *cgrp,
				    struct cftype *cft)
{
	return test_bit(CGRP_CPUSET_CLONE_CHILDREN, &cgrp->flags);
}

static int cgroup_clone_children_write(struct cgroup *cgrp,
				     struct cftype *cft,
				     u64 val)
{
	if (val)
		set_bit(CGRP_CPUSET_CLONE_CHILDREN, &cgrp->flags);
	else
		clear_bit(CGRP_CPUSET_CLONE_CHILDREN, &cgrp->flags);
	return 0;
}

/*
 * for the common functions, 'private' gives the type of file
 */
/* for hysterical raisins, we can't put this on the older files */
#define CGROUP_FILE_GENERIC_PREFIX "cgroup."
static struct cftype files[] = {
	{
		.name = "tasks",
		.open = cgroup_tasks_open,
		.write_u64 = cgroup_tasks_write,
		.release = cgroup_pidlist_release,
		.mode = S_IRUGO | S_IWUSR,
	},
	{
		.name = CGROUP_FILE_GENERIC_PREFIX "procs",
		.open = cgroup_procs_open,
		.write_u64 = cgroup_procs_write,
		.release = cgroup_pidlist_release,
		.mode = S_IRUGO | S_IWUSR,
	},
	{
		.name = "notify_on_release",
		.read_u64 = cgroup_read_notify_on_release,
		.write_u64 = cgroup_write_notify_on_release,
	},
	{
		.name = CGROUP_FILE_GENERIC_PREFIX "event_control",
		.write_string = cgroup_write_event_control,
		.mode = S_IWUGO,
	},
	{
		.name = "cgroup.clone_children",
		.flags = CFTYPE_INSANE,
=======
static u64 cgroup_read_notify_on_release(struct cgroup_subsys_state *css,
					 struct cftype *cft)
{
	return notify_on_release(css->cgroup);
}

static int cgroup_write_notify_on_release(struct cgroup_subsys_state *css,
					  struct cftype *cft, u64 val)
{
	if (val)
		set_bit(CGRP_NOTIFY_ON_RELEASE, &css->cgroup->flags);
	else
		clear_bit(CGRP_NOTIFY_ON_RELEASE, &css->cgroup->flags);
	return 0;
}

static u64 cgroup_clone_children_read(struct cgroup_subsys_state *css,
				      struct cftype *cft)
{
	return test_bit(CGRP_CPUSET_CLONE_CHILDREN, &css->cgroup->flags);
}

static int cgroup_clone_children_write(struct cgroup_subsys_state *css,
				       struct cftype *cft, u64 val)
{
	if (val)
		set_bit(CGRP_CPUSET_CLONE_CHILDREN, &css->cgroup->flags);
	else
		clear_bit(CGRP_CPUSET_CLONE_CHILDREN, &css->cgroup->flags);
	return 0;
}

/* cgroup core interface files for the default hierarchy */
static struct cftype cgroup_dfl_base_files[] = {
	{
		.name = "cgroup.procs",
		.seq_start = cgroup_pidlist_start,
		.seq_next = cgroup_pidlist_next,
		.seq_stop = cgroup_pidlist_stop,
		.seq_show = cgroup_pidlist_show,
		.private = CGROUP_FILE_PROCS,
		.write = cgroup_procs_write,
		.mode = S_IRUGO | S_IWUSR,
	},
	{
		.name = "cgroup.controllers",
		.flags = CFTYPE_ONLY_ON_ROOT,
		.seq_show = cgroup_root_controllers_show,
	},
	{
		.name = "cgroup.controllers",
		.flags = CFTYPE_NOT_ON_ROOT,
		.seq_show = cgroup_controllers_show,
	},
	{
		.name = "cgroup.subtree_control",
		.seq_show = cgroup_subtree_control_show,
		.write = cgroup_subtree_control_write,
	},
	{
		.name = "cgroup.populated",
		.flags = CFTYPE_NOT_ON_ROOT,
		.seq_show = cgroup_populated_show,
	},
	{ }	/* terminate */
};

/* cgroup core interface files for the legacy hierarchies */
static struct cftype cgroup_legacy_base_files[] = {
	{
		.name = "cgroup.procs",
		.seq_start = cgroup_pidlist_start,
		.seq_next = cgroup_pidlist_next,
		.seq_stop = cgroup_pidlist_stop,
		.seq_show = cgroup_pidlist_show,
		.private = CGROUP_FILE_PROCS,
		.write = cgroup_procs_write,
		.mode = S_IRUGO | S_IWUSR,
	},
	{
		.name = "cgroup.clone_children",
>>>>>>> v3.18
		.read_u64 = cgroup_clone_children_read,
		.write_u64 = cgroup_clone_children_write,
	},
	{
		.name = "cgroup.sane_behavior",
		.flags = CFTYPE_ONLY_ON_ROOT,
<<<<<<< HEAD
		.read_seq_string = cgroup_sane_behavior_show,
=======
		.seq_show = cgroup_sane_behavior_show,
	},
	{
		.name = "tasks",
		.seq_start = cgroup_pidlist_start,
		.seq_next = cgroup_pidlist_next,
		.seq_stop = cgroup_pidlist_stop,
		.seq_show = cgroup_pidlist_show,
		.private = CGROUP_FILE_TASKS,
		.write = cgroup_tasks_write,
		.mode = S_IRUGO | S_IWUSR,
	},
	{
		.name = "notify_on_release",
		.read_u64 = cgroup_read_notify_on_release,
		.write_u64 = cgroup_write_notify_on_release,
>>>>>>> v3.18
	},
	{
		.name = "release_agent",
		.flags = CFTYPE_ONLY_ON_ROOT,
<<<<<<< HEAD
		.read_seq_string = cgroup_release_agent_show,
		.write_string = cgroup_release_agent_write,
		.max_write_len = PATH_MAX,
=======
		.seq_show = cgroup_release_agent_show,
		.write = cgroup_release_agent_write,
		.max_write_len = PATH_MAX - 1,
>>>>>>> v3.18
	},
	{ }	/* terminate */
};

/**
<<<<<<< HEAD
 * cgroup_populate_dir - selectively creation of files in a directory
 * @cgrp: target cgroup
 * @base_files: true if the base files should be added
 * @subsys_mask: mask of the subsystem ids whose files should be added
 */
static int cgroup_populate_dir(struct cgroup *cgrp, bool base_files,
			       unsigned long subsys_mask)
{
	int err;
	struct cgroup_subsys *ss;

	if (base_files) {
		err = cgroup_addrm_files(cgrp, NULL, files, true);
		if (err < 0)
			return err;
	}

	/* process cftsets of each subsystem */
	for_each_subsys(cgrp->root, ss) {
		struct cftype_set *set;
		if (!test_bit(ss->subsys_id, &subsys_mask))
			continue;

		list_for_each_entry(set, &ss->cftsets, node)
			cgroup_addrm_files(cgrp, ss, set->cfts, true);
	}

	/* This cgroup is ready now */
	for_each_subsys(cgrp->root, ss) {
		struct cgroup_subsys_state *css = cgrp->subsys[ss->subsys_id];
		/*
		 * Update id->css pointer and make this css visible from
		 * CSS ID functions. This pointer will be dereferened
		 * from RCU-read-side without locks.
		 */
		if (css->id)
			rcu_assign_pointer(css->id->css, css);
	}

	return 0;
}

static void css_dput_fn(struct work_struct *work)
{
	struct cgroup_subsys_state *css =
		container_of(work, struct cgroup_subsys_state, dput_work);

	cgroup_dput(css->cgroup);
}

static void init_cgroup_css(struct cgroup_subsys_state *css,
			       struct cgroup_subsys *ss,
			       struct cgroup *cgrp)
{
	css->cgroup = cgrp;
	atomic_set(&css->refcnt, 1);
	css->flags = 0;
	css->id = NULL;
	if (cgrp == dummytop)
		css->flags |= CSS_ROOT;
	BUG_ON(cgrp->subsys[ss->subsys_id]);
	cgrp->subsys[ss->subsys_id] = css;

	/*
	 * css holds an extra ref to @cgrp->dentry which is put on the last
	 * css_put().  dput() requires process context, which css_put() may
	 * be called without.  @css->dput_work will be used to invoke
	 * dput() asynchronously from css_put().
	 */
	INIT_WORK(&css->dput_work, css_dput_fn);
}

/* invoke ->post_create() on a new CSS and mark it online if successful */
static int online_css(struct cgroup_subsys *ss, struct cgroup *cgrp)
{
=======
 * cgroup_populate_dir - create subsys files in a cgroup directory
 * @cgrp: target cgroup
 * @subsys_mask: mask of the subsystem ids whose files should be added
 *
 * On failure, no file is added.
 */
static int cgroup_populate_dir(struct cgroup *cgrp, unsigned int subsys_mask)
{
	struct cgroup_subsys *ss;
	int i, ret = 0;

	/* process cftsets of each subsystem */
	for_each_subsys(ss, i) {
		struct cftype *cfts;

		if (!(subsys_mask & (1 << i)))
			continue;

		list_for_each_entry(cfts, &ss->cfts, node) {
			ret = cgroup_addrm_files(cgrp, cfts, true);
			if (ret < 0)
				goto err;
		}
	}
	return 0;
err:
	cgroup_clear_dir(cgrp, subsys_mask);
	return ret;
}

/*
 * css destruction is four-stage process.
 *
 * 1. Destruction starts.  Killing of the percpu_ref is initiated.
 *    Implemented in kill_css().
 *
 * 2. When the percpu_ref is confirmed to be visible as killed on all CPUs
 *    and thus css_tryget_online() is guaranteed to fail, the css can be
 *    offlined by invoking offline_css().  After offlining, the base ref is
 *    put.  Implemented in css_killed_work_fn().
 *
 * 3. When the percpu_ref reaches zero, the only possible remaining
 *    accessors are inside RCU read sections.  css_release() schedules the
 *    RCU callback.
 *
 * 4. After the grace period, the css can be freed.  Implemented in
 *    css_free_work_fn().
 *
 * It is actually hairier because both step 2 and 4 require process context
 * and thus involve punting to css->destroy_work adding two additional
 * steps to the already complex sequence.
 */
static void css_free_work_fn(struct work_struct *work)
{
	struct cgroup_subsys_state *css =
		container_of(work, struct cgroup_subsys_state, destroy_work);
	struct cgroup *cgrp = css->cgroup;

	percpu_ref_exit(&css->refcnt);

	if (css->ss) {
		/* css free path */
		if (css->parent)
			css_put(css->parent);

		css->ss->css_free(css);
		cgroup_put(cgrp);
	} else {
		/* cgroup free path */
		atomic_dec(&cgrp->root->nr_cgrps);
		cgroup_pidlist_destroy_all(cgrp);
		cancel_work_sync(&cgrp->release_agent_work);

		if (cgroup_parent(cgrp)) {
			/*
			 * We get a ref to the parent, and put the ref when
			 * this cgroup is being freed, so it's guaranteed
			 * that the parent won't be destroyed before its
			 * children.
			 */
			cgroup_put(cgroup_parent(cgrp));
			kernfs_put(cgrp->kn);
			kfree(cgrp);
		} else {
			/*
			 * This is root cgroup's refcnt reaching zero,
			 * which indicates that the root should be
			 * released.
			 */
			cgroup_destroy_root(cgrp->root);
		}
	}
}

static void css_free_rcu_fn(struct rcu_head *rcu_head)
{
	struct cgroup_subsys_state *css =
		container_of(rcu_head, struct cgroup_subsys_state, rcu_head);

	INIT_WORK(&css->destroy_work, css_free_work_fn);
	queue_work(cgroup_destroy_wq, &css->destroy_work);
}

static void css_release_work_fn(struct work_struct *work)
{
	struct cgroup_subsys_state *css =
		container_of(work, struct cgroup_subsys_state, destroy_work);
	struct cgroup_subsys *ss = css->ss;
	struct cgroup *cgrp = css->cgroup;

	mutex_lock(&cgroup_mutex);

	css->flags |= CSS_RELEASED;
	list_del_rcu(&css->sibling);

	if (ss) {
		/* css release path */
		cgroup_idr_remove(&ss->css_idr, css->id);
	} else {
		/* cgroup release path */
		cgroup_idr_remove(&cgrp->root->cgroup_idr, cgrp->id);
		cgrp->id = -1;

		/*
		 * There are two control paths which try to determine
		 * cgroup from dentry without going through kernfs -
		 * cgroupstats_build() and css_tryget_online_from_dir().
		 * Those are supported by RCU protecting clearing of
		 * cgrp->kn->priv backpointer.
		 */
		RCU_INIT_POINTER(*(void __rcu __force **)&cgrp->kn->priv, NULL);
	}

	mutex_unlock(&cgroup_mutex);

	call_rcu(&css->rcu_head, css_free_rcu_fn);
}

static void css_release(struct percpu_ref *ref)
{
	struct cgroup_subsys_state *css =
		container_of(ref, struct cgroup_subsys_state, refcnt);

	INIT_WORK(&css->destroy_work, css_release_work_fn);
	queue_work(cgroup_destroy_wq, &css->destroy_work);
}

static void init_and_link_css(struct cgroup_subsys_state *css,
			      struct cgroup_subsys *ss, struct cgroup *cgrp)
{
	lockdep_assert_held(&cgroup_mutex);

	cgroup_get(cgrp);

	memset(css, 0, sizeof(*css));
	css->cgroup = cgrp;
	css->ss = ss;
	INIT_LIST_HEAD(&css->sibling);
	INIT_LIST_HEAD(&css->children);
	css->serial_nr = css_serial_nr_next++;

	if (cgroup_parent(cgrp)) {
		css->parent = cgroup_css(cgroup_parent(cgrp), ss);
		css_get(css->parent);
	}

	BUG_ON(cgroup_css(cgrp, ss));
}

/* invoke ->css_online() on a new CSS and mark it online if successful */
static int online_css(struct cgroup_subsys_state *css)
{
	struct cgroup_subsys *ss = css->ss;
>>>>>>> v3.18
	int ret = 0;

	lockdep_assert_held(&cgroup_mutex);

	if (ss->css_online)
<<<<<<< HEAD
		ret = ss->css_online(cgrp);
	if (!ret)
		cgrp->subsys[ss->subsys_id]->flags |= CSS_ONLINE;
	return ret;
}

/* if the CSS is online, invoke ->pre_destory() on it and mark it offline */
static void offline_css(struct cgroup_subsys *ss, struct cgroup *cgrp)
	__releases(&cgroup_mutex) __acquires(&cgroup_mutex)
{
	struct cgroup_subsys_state *css = cgrp->subsys[ss->subsys_id];
=======
		ret = ss->css_online(css);
	if (!ret) {
		css->flags |= CSS_ONLINE;
		rcu_assign_pointer(css->cgroup->subsys[ss->id], css);
	}
	return ret;
}

/* if the CSS is online, invoke ->css_offline() on it and mark it offline */
static void offline_css(struct cgroup_subsys_state *css)
{
	struct cgroup_subsys *ss = css->ss;
>>>>>>> v3.18

	lockdep_assert_held(&cgroup_mutex);

	if (!(css->flags & CSS_ONLINE))
		return;

	if (ss->css_offline)
<<<<<<< HEAD
		ss->css_offline(cgrp);

	cgrp->subsys[ss->subsys_id]->flags &= ~CSS_ONLINE;
}

/*
 * cgroup_create - create a cgroup
 * @parent: cgroup that will be parent of the new cgroup
 * @dentry: dentry of the new cgroup
 * @mode: mode to set on new inode
 *
 * Must be called with the mutex on the parent inode held
 */
static long cgroup_create(struct cgroup *parent, struct dentry *dentry,
			     umode_t mode)
{
	struct cgroup *cgrp;
	struct cgroup_name *name;
	struct cgroupfs_root *root = parent->root;
	int err = 0;
	struct cgroup_subsys *ss;
	struct super_block *sb = root->sb;

	/* allocate the cgroup and its ID, 0 is reserved for the root */
	cgrp = kzalloc(sizeof(*cgrp), GFP_KERNEL);
	if (!cgrp)
		return -ENOMEM;

	name = cgroup_alloc_name(dentry);
	if (!name)
		goto err_free_cgrp;
	rcu_assign_pointer(cgrp->name, name);

	cgrp->id = ida_simple_get(&root->cgroup_ida, 1, 0, GFP_KERNEL);
	if (cgrp->id < 0)
		goto err_free_name;

	/*
	 * Only live parents can have children.  Note that the liveliness
	 * check isn't strictly necessary because cgroup_mkdir() and
	 * cgroup_rmdir() are fully synchronized by i_mutex; however, do it
	 * anyway so that locking is contained inside cgroup proper and we
	 * don't get nasty surprises if we ever grow another caller.
	 */
	if (!cgroup_lock_live_group(parent)) {
		err = -ENODEV;
		goto err_free_id;
	}

	/* Grab a reference on the superblock so the hierarchy doesn't
	 * get deleted on unmount if there are child cgroups.  This
	 * can be done outside cgroup_mutex, since the sb can't
	 * disappear while someone has an open control file on the
	 * fs */
	atomic_inc(&sb->s_active);

	init_cgroup_housekeeping(cgrp);

	dentry->d_fsdata = cgrp;
	cgrp->dentry = dentry;

	cgrp->parent = parent;
	cgrp->root = parent->root;

	if (notify_on_release(parent))
		set_bit(CGRP_NOTIFY_ON_RELEASE, &cgrp->flags);

	if (test_bit(CGRP_CPUSET_CLONE_CHILDREN, &parent->flags))
		set_bit(CGRP_CPUSET_CLONE_CHILDREN, &cgrp->flags);

	for_each_subsys(root, ss) {
		struct cgroup_subsys_state *css;

		css = ss->css_alloc(cgrp);
		if (IS_ERR(css)) {
			err = PTR_ERR(css);
			goto err_free_all;
		}
		init_cgroup_css(css, ss, cgrp);
		if (ss->use_id) {
			err = alloc_css_id(ss, parent, cgrp);
			if (err)
				goto err_free_all;
		}
	}

	/*
	 * Create directory.  cgroup_create_file() returns with the new
	 * directory locked on success so that it can be populated without
	 * dropping cgroup_mutex.
	 */
	err = cgroup_create_file(dentry, S_IFDIR | mode, sb);
	if (err < 0)
		goto err_free_all;
	lockdep_assert_held(&dentry->d_inode->i_mutex);

	/* allocation complete, commit to creation */
	list_add_tail(&cgrp->allcg_node, &root->allcg_list);
	list_add_tail_rcu(&cgrp->sibling, &cgrp->parent->children);
	root->number_of_cgroups++;

	/* each css holds a ref to the cgroup's dentry */
	for_each_subsys(root, ss)
		dget(dentry);

	/* hold a ref to the parent's dentry */
	dget(parent->dentry);

	/* creation succeeded, notify subsystems */
	for_each_subsys(root, ss) {
		err = online_css(ss, cgrp);
		if (err)
			goto err_destroy;

		if (ss->broken_hierarchy && !ss->warned_broken_hierarchy &&
		    parent->parent) {
			pr_warning("cgroup: %s (%d) created nested cgroup for controller \"%s\" which has incomplete hierarchy support. Nested cgroups may change behavior in the future.\n",
				   current->comm, current->pid, ss->name);
			if (!strcmp(ss->name, "memory"))
				pr_warning("cgroup: \"memory\" requires setting use_hierarchy to 1 on the root.\n");
			ss->warned_broken_hierarchy = true;
		}
	}

	err = cgroup_populate_dir(cgrp, true, root->subsys_mask);
	if (err)
		goto err_destroy;

	mutex_unlock(&cgroup_mutex);
	mutex_unlock(&cgrp->dentry->d_inode->i_mutex);

	return 0;

err_free_all:
	for_each_subsys(root, ss) {
		if (cgrp->subsys[ss->subsys_id])
			ss->css_free(cgrp);
	}
	mutex_unlock(&cgroup_mutex);
	/* Release the reference count that we took on the superblock */
	deactivate_super(sb);
err_free_id:
	ida_simple_remove(&root->cgroup_ida, cgrp->id);
err_free_name:
	kfree(rcu_dereference_raw(cgrp->name));
err_free_cgrp:
	kfree(cgrp);
	return err;

err_destroy:
	cgroup_destroy_locked(cgrp);
	mutex_unlock(&cgroup_mutex);
	mutex_unlock(&dentry->d_inode->i_mutex);
	return err;
}

static int cgroup_mkdir(struct inode *dir, struct dentry *dentry, umode_t mode)
{
	struct cgroup *c_parent = dentry->d_parent->d_fsdata;

	/* the vfs holds inode->i_mutex already */
	return cgroup_create(c_parent, dentry, mode | S_IFDIR);
}

static int cgroup_destroy_locked(struct cgroup *cgrp)
	__releases(&cgroup_mutex) __acquires(&cgroup_mutex)
{
	struct dentry *d = cgrp->dentry;
	struct cgroup *parent = cgrp->parent;
	struct cgroup_event *event, *tmp;
	struct cgroup_subsys *ss;

	lockdep_assert_held(&d->d_inode->i_mutex);
	lockdep_assert_held(&cgroup_mutex);

	if (atomic_read(&cgrp->count) || !list_empty(&cgrp->children))
		return -EBUSY;

	/*
	 * Block new css_tryget() by deactivating refcnt and mark @cgrp
	 * removed.  This makes future css_tryget() and child creation
	 * attempts fail thus maintaining the removal conditions verified
	 * above.
	 */
	for_each_subsys(cgrp->root, ss) {
		struct cgroup_subsys_state *css = cgrp->subsys[ss->subsys_id];

		WARN_ON(atomic_read(&css->refcnt) < 0);
		atomic_add(CSS_DEACT_BIAS, &css->refcnt);
	}
	set_bit(CGRP_REMOVED, &cgrp->flags);

	/* tell subsystems to initate destruction */
	for_each_subsys(cgrp->root, ss)
		offline_css(ss, cgrp);

	/*
	 * Put all the base refs.  Each css holds an extra reference to the
	 * cgroup's dentry and cgroup removal proceeds regardless of css
	 * refs.  On the last put of each css, whenever that may be, the
	 * extra dentry ref is put so that dentry destruction happens only
	 * after all css's are released.
	 */
	for_each_subsys(cgrp->root, ss)
		css_put(cgrp->subsys[ss->subsys_id]);

	raw_spin_lock(&release_list_lock);
	if (!list_empty(&cgrp->release_list))
		list_del_init(&cgrp->release_list);
	raw_spin_unlock(&release_list_lock);

	/* delete this cgroup from parent->children */
	list_del_rcu(&cgrp->sibling);
	list_del_init(&cgrp->allcg_node);

	dget(d);
	cgroup_d_remove_dir(d);
	dput(d);

	set_bit(CGRP_RELEASABLE, &parent->flags);
	check_for_release(parent);

	/*
	 * Unregister events and notify userspace.
	 * Notify userspace about cgroup removing only after rmdir of cgroup
	 * directory to avoid race between userspace and kernelspace.
	 */
	spin_lock(&cgrp->event_list_lock);
	list_for_each_entry_safe(event, tmp, &cgrp->event_list, list) {
		list_del_init(&event->list);
		schedule_work(&event->remove);
	}
	spin_unlock(&cgrp->event_list_lock);

	return 0;
}

static int cgroup_rmdir(struct inode *unused_dir, struct dentry *dentry)
{
	int ret;

	mutex_lock(&cgroup_mutex);
	ret = cgroup_destroy_locked(dentry->d_fsdata);
	mutex_unlock(&cgroup_mutex);

	return ret;
}

static void __init_or_module cgroup_init_cftsets(struct cgroup_subsys *ss)
{
	INIT_LIST_HEAD(&ss->cftsets);

	/*
	 * base_cftset is embedded in subsys itself, no need to worry about
	 * deregistration.
	 */
	if (ss->base_cftypes) {
		ss->base_cftset.cfts = ss->base_cftypes;
		list_add_tail(&ss->base_cftset.node, &ss->cftsets);
	}
}

static void __init cgroup_init_subsys(struct cgroup_subsys *ss)
{
	struct cgroup_subsys_state *css;

	printk(KERN_INFO "Initializing cgroup subsys %s\n", ss->name);

	mutex_lock(&cgroup_mutex);

	/* init base cftset */
	cgroup_init_cftsets(ss);

	/* Create the top cgroup state for this subsystem */
	list_add(&ss->sibling, &rootnode.subsys_list);
	ss->root = &rootnode;
	css = ss->css_alloc(dummytop);
	/* We don't handle early failures gracefully */
	BUG_ON(IS_ERR(css));
	init_cgroup_css(css, ss, dummytop);

	/* Update the init_css_set to contain a subsys
	 * pointer to this state - since the subsystem is
	 * newly registered, all tasks and hence the
	 * init_css_set is in the subsystem's top cgroup. */
	init_css_set.subsys[ss->subsys_id] = css;

	need_forkexit_callback |= ss->fork || ss->exit;

	/* At system boot, before all subsystems have been
	 * registered, no tasks have been forked, so we don't
	 * need to invoke fork callbacks here. */
	BUG_ON(!list_empty(&init_task.tasks));

	BUG_ON(online_css(ss, dummytop));

	mutex_unlock(&cgroup_mutex);

	/* this function shouldn't be used with modular subsystems, since they
	 * need to register a subsys_id, among other things */
	BUG_ON(ss->module);
}

/**
 * cgroup_load_subsys: load and register a modular subsystem at runtime
 * @ss: the subsystem to load
 *
 * This function should be called in a modular subsystem's initcall. If the
 * subsystem is built as a module, it will be assigned a new subsys_id and set
 * up for use. If the subsystem is built-in anyway, work is delegated to the
 * simpler cgroup_init_subsys.
 */
int __init_or_module cgroup_load_subsys(struct cgroup_subsys *ss)
{
	struct cgroup_subsys_state *css;
	int i, ret;
	struct hlist_node *tmp;
	struct css_set *cg;
	unsigned long key;

	/* check name and function validity */
	if (ss->name == NULL || strlen(ss->name) > MAX_CGROUP_TYPE_NAMELEN ||
	    ss->css_alloc == NULL || ss->css_free == NULL)
		return -EINVAL;

	/*
	 * we don't support callbacks in modular subsystems. this check is
	 * before the ss->module check for consistency; a subsystem that could
	 * be a module should still have no callbacks even if the user isn't
	 * compiling it as one.
	 */
	if (ss->fork || ss->exit)
		return -EINVAL;

	/*
	 * an optionally modular subsystem is built-in: we want to do nothing,
	 * since cgroup_init_subsys will have already taken care of it.
	 */
	if (ss->module == NULL) {
		/* a sanity check */
		BUG_ON(subsys[ss->subsys_id] != ss);
		return 0;
	}

	/* init base cftset */
	cgroup_init_cftsets(ss);

	mutex_lock(&cgroup_mutex);
	subsys[ss->subsys_id] = ss;

	/*
	 * no ss->css_alloc seems to need anything important in the ss
	 * struct, so this can happen first (i.e. before the rootnode
	 * attachment).
	 */
	css = ss->css_alloc(dummytop);
	if (IS_ERR(css)) {
		/* failure case - need to deassign the subsys[] slot. */
		subsys[ss->subsys_id] = NULL;
		mutex_unlock(&cgroup_mutex);
		return PTR_ERR(css);
	}

	list_add(&ss->sibling, &rootnode.subsys_list);
	ss->root = &rootnode;

	/* our new subsystem will be attached to the dummy hierarchy. */
	init_cgroup_css(css, ss, dummytop);
	/* init_idr must be after init_cgroup_css because it sets css->id. */
	if (ss->use_id) {
		ret = cgroup_init_idr(ss, css);
		if (ret)
			goto err_unload;
	}

	/*
	 * Now we need to entangle the css into the existing css_sets. unlike
	 * in cgroup_init_subsys, there are now multiple css_sets, so each one
	 * will need a new pointer to it; done by iterating the css_set_table.
	 * furthermore, modifying the existing css_sets will corrupt the hash
	 * table state, so each changed css_set will need its hash recomputed.
	 * this is all done under the css_set_lock.
	 */
	write_lock(&css_set_lock);
	hash_for_each_safe(css_set_table, i, tmp, cg, hlist) {
		/* skip entries that we already rehashed */
		if (cg->subsys[ss->subsys_id])
			continue;
		/* remove existing entry */
		hash_del(&cg->hlist);
		/* set new value */
		cg->subsys[ss->subsys_id] = css;
		/* recompute hash and restore entry */
		key = css_set_hash(cg->subsys);
		hash_add(css_set_table, &cg->hlist, key);
	}
	write_unlock(&css_set_lock);

	ret = online_css(ss, dummytop);
	if (ret)
		goto err_unload;

	/* success! */
	mutex_unlock(&cgroup_mutex);
	return 0;

err_unload:
	mutex_unlock(&cgroup_mutex);
	/* @ss can't be mounted here as try_module_get() would fail */
	cgroup_unload_subsys(ss);
	return ret;
}
EXPORT_SYMBOL_GPL(cgroup_load_subsys);

/**
 * cgroup_unload_subsys: unload a modular subsystem
 * @ss: the subsystem to unload
 *
 * This function should be called in a modular subsystem's exitcall. When this
 * function is invoked, the refcount on the subsystem's module will be 0, so
 * the subsystem will not be attached to any hierarchy.
 */
void cgroup_unload_subsys(struct cgroup_subsys *ss)
{
	struct cg_cgroup_link *link;

	BUG_ON(ss->module == NULL);

	/*
	 * we shouldn't be called if the subsystem is in use, and the use of
	 * try_module_get in parse_cgroupfs_options should ensure that it
	 * doesn't start being used while we're killing it off.
	 */
	BUG_ON(ss->root != &rootnode);

	mutex_lock(&cgroup_mutex);

	offline_css(ss, dummytop);

	if (ss->use_id)
		idr_destroy(&ss->idr);

	/* deassign the subsys_id */
	subsys[ss->subsys_id] = NULL;

	/* remove subsystem from rootnode's list of subsystems */
	list_del_init(&ss->sibling);

	/*
	 * disentangle the css from all css_sets attached to the dummytop. as
	 * in loading, we need to pay our respects to the hashtable gods.
	 */
	write_lock(&css_set_lock);
	list_for_each_entry(link, &dummytop->css_sets, cgrp_link_list) {
		struct css_set *cg = link->cg;
		unsigned long key;

		hash_del(&cg->hlist);
		cg->subsys[ss->subsys_id] = NULL;
		key = css_set_hash(cg->subsys);
		hash_add(css_set_table, &cg->hlist, key);
	}
	write_unlock(&css_set_lock);

	/*
	 * remove subsystem's css from the dummytop and free it - need to
	 * free before marking as null because ss->css_free needs the
	 * cgrp->subsys pointer to find their state. note that this also
	 * takes care of freeing the css_id.
	 */
	ss->css_free(dummytop);
	dummytop->subsys[ss->subsys_id] = NULL;

	mutex_unlock(&cgroup_mutex);
}
EXPORT_SYMBOL_GPL(cgroup_unload_subsys);
=======
		ss->css_offline(css);

	css->flags &= ~CSS_ONLINE;
	RCU_INIT_POINTER(css->cgroup->subsys[ss->id], NULL);

	wake_up_all(&css->cgroup->offline_waitq);
}

/**
 * create_css - create a cgroup_subsys_state
 * @cgrp: the cgroup new css will be associated with
 * @ss: the subsys of new css
 * @visible: whether to create control knobs for the new css or not
 *
 * Create a new css associated with @cgrp - @ss pair.  On success, the new
 * css is online and installed in @cgrp with all interface files created if
 * @visible.  Returns 0 on success, -errno on failure.
 */
static int create_css(struct cgroup *cgrp, struct cgroup_subsys *ss,
		      bool visible)
{
	struct cgroup *parent = cgroup_parent(cgrp);
	struct cgroup_subsys_state *parent_css = cgroup_css(parent, ss);
	struct cgroup_subsys_state *css;
	int err;

	lockdep_assert_held(&cgroup_mutex);

	css = ss->css_alloc(parent_css);
	if (IS_ERR(css))
		return PTR_ERR(css);

	init_and_link_css(css, ss, cgrp);

	err = percpu_ref_init(&css->refcnt, css_release, 0, GFP_KERNEL);
	if (err)
		goto err_free_css;

	err = cgroup_idr_alloc(&ss->css_idr, NULL, 2, 0, GFP_NOWAIT);
	if (err < 0)
		goto err_free_percpu_ref;
	css->id = err;

	if (visible) {
		err = cgroup_populate_dir(cgrp, 1 << ss->id);
		if (err)
			goto err_free_id;
	}

	/* @css is ready to be brought online now, make it visible */
	list_add_tail_rcu(&css->sibling, &parent_css->children);
	cgroup_idr_replace(&ss->css_idr, css, css->id);

	err = online_css(css);
	if (err)
		goto err_list_del;

	if (ss->broken_hierarchy && !ss->warned_broken_hierarchy &&
	    cgroup_parent(parent)) {
		pr_warn("%s (%d) created nested cgroup for controller \"%s\" which has incomplete hierarchy support. Nested cgroups may change behavior in the future.\n",
			current->comm, current->pid, ss->name);
		if (!strcmp(ss->name, "memory"))
			pr_warn("\"memory\" requires setting use_hierarchy to 1 on the root\n");
		ss->warned_broken_hierarchy = true;
	}

	return 0;

err_list_del:
	list_del_rcu(&css->sibling);
	cgroup_clear_dir(css->cgroup, 1 << css->ss->id);
err_free_id:
	cgroup_idr_remove(&ss->css_idr, css->id);
err_free_percpu_ref:
	percpu_ref_exit(&css->refcnt);
err_free_css:
	call_rcu(&css->rcu_head, css_free_rcu_fn);
	return err;
}

static int cgroup_mkdir(struct kernfs_node *parent_kn, const char *name,
			umode_t mode)
{
	struct cgroup *parent, *cgrp;
	struct cgroup_root *root;
	struct cgroup_subsys *ss;
	struct kernfs_node *kn;
	struct cftype *base_files;
	int ssid, ret;

	/* Do not accept '\n' to prevent making /proc/<pid>/cgroup unparsable.
	 */
	if (strchr(name, '\n'))
		return -EINVAL;

	parent = cgroup_kn_lock_live(parent_kn);
	if (!parent)
		return -ENODEV;
	root = parent->root;

	/* allocate the cgroup and its ID, 0 is reserved for the root */
	cgrp = kzalloc(sizeof(*cgrp), GFP_KERNEL);
	if (!cgrp) {
		ret = -ENOMEM;
		goto out_unlock;
	}

	ret = percpu_ref_init(&cgrp->self.refcnt, css_release, 0, GFP_KERNEL);
	if (ret)
		goto out_free_cgrp;

	/*
	 * Temporarily set the pointer to NULL, so idr_find() won't return
	 * a half-baked cgroup.
	 */
	cgrp->id = cgroup_idr_alloc(&root->cgroup_idr, NULL, 2, 0, GFP_NOWAIT);
	if (cgrp->id < 0) {
		ret = -ENOMEM;
		goto out_cancel_ref;
	}

	init_cgroup_housekeeping(cgrp);

	cgrp->self.parent = &parent->self;
	cgrp->root = root;

	if (notify_on_release(parent))
		set_bit(CGRP_NOTIFY_ON_RELEASE, &cgrp->flags);

	if (test_bit(CGRP_CPUSET_CLONE_CHILDREN, &parent->flags))
		set_bit(CGRP_CPUSET_CLONE_CHILDREN, &cgrp->flags);

	/* create the directory */
	kn = kernfs_create_dir(parent->kn, name, mode, cgrp);
	if (IS_ERR(kn)) {
		ret = PTR_ERR(kn);
		goto out_free_id;
	}
	cgrp->kn = kn;

	/*
	 * This extra ref will be put in cgroup_free_fn() and guarantees
	 * that @cgrp->kn is always accessible.
	 */
	kernfs_get(kn);

	cgrp->self.serial_nr = css_serial_nr_next++;

	/* allocation complete, commit to creation */
	list_add_tail_rcu(&cgrp->self.sibling, &cgroup_parent(cgrp)->self.children);
	atomic_inc(&root->nr_cgrps);
	cgroup_get(parent);

	/*
	 * @cgrp is now fully operational.  If something fails after this
	 * point, it'll be released via the normal destruction path.
	 */
	cgroup_idr_replace(&root->cgroup_idr, cgrp, cgrp->id);

	ret = cgroup_kn_set_ugid(kn);
	if (ret)
		goto out_destroy;

	if (cgroup_on_dfl(cgrp))
		base_files = cgroup_dfl_base_files;
	else
		base_files = cgroup_legacy_base_files;

	ret = cgroup_addrm_files(cgrp, base_files, true);
	if (ret)
		goto out_destroy;

	/* let's create and online css's */
	for_each_subsys(ss, ssid) {
		if (parent->child_subsys_mask & (1 << ssid)) {
			ret = create_css(cgrp, ss,
					 parent->subtree_control & (1 << ssid));
			if (ret)
				goto out_destroy;
		}
	}

	/*
	 * On the default hierarchy, a child doesn't automatically inherit
	 * subtree_control from the parent.  Each is configured manually.
	 */
	if (!cgroup_on_dfl(cgrp)) {
		cgrp->subtree_control = parent->subtree_control;
		cgroup_refresh_child_subsys_mask(cgrp);
	}

	kernfs_activate(kn);

	ret = 0;
	goto out_unlock;

out_free_id:
	cgroup_idr_remove(&root->cgroup_idr, cgrp->id);
out_cancel_ref:
	percpu_ref_exit(&cgrp->self.refcnt);
out_free_cgrp:
	kfree(cgrp);
out_unlock:
	cgroup_kn_unlock(parent_kn);
	return ret;

out_destroy:
	cgroup_destroy_locked(cgrp);
	goto out_unlock;
}

/*
 * This is called when the refcnt of a css is confirmed to be killed.
 * css_tryget_online() is now guaranteed to fail.  Tell the subsystem to
 * initate destruction and put the css ref from kill_css().
 */
static void css_killed_work_fn(struct work_struct *work)
{
	struct cgroup_subsys_state *css =
		container_of(work, struct cgroup_subsys_state, destroy_work);

	mutex_lock(&cgroup_mutex);
	offline_css(css);
	mutex_unlock(&cgroup_mutex);

	css_put(css);
}

/* css kill confirmation processing requires process context, bounce */
static void css_killed_ref_fn(struct percpu_ref *ref)
{
	struct cgroup_subsys_state *css =
		container_of(ref, struct cgroup_subsys_state, refcnt);

	INIT_WORK(&css->destroy_work, css_killed_work_fn);
	queue_work(cgroup_destroy_wq, &css->destroy_work);
}

/**
 * kill_css - destroy a css
 * @css: css to destroy
 *
 * This function initiates destruction of @css by removing cgroup interface
 * files and putting its base reference.  ->css_offline() will be invoked
 * asynchronously once css_tryget_online() is guaranteed to fail and when
 * the reference count reaches zero, @css will be released.
 */
static void kill_css(struct cgroup_subsys_state *css)
{
	lockdep_assert_held(&cgroup_mutex);

	/*
	 * This must happen before css is disassociated with its cgroup.
	 * See seq_css() for details.
	 */
	cgroup_clear_dir(css->cgroup, 1 << css->ss->id);

	/*
	 * Killing would put the base ref, but we need to keep it alive
	 * until after ->css_offline().
	 */
	css_get(css);

	/*
	 * cgroup core guarantees that, by the time ->css_offline() is
	 * invoked, no new css reference will be given out via
	 * css_tryget_online().  We can't simply call percpu_ref_kill() and
	 * proceed to offlining css's because percpu_ref_kill() doesn't
	 * guarantee that the ref is seen as killed on all CPUs on return.
	 *
	 * Use percpu_ref_kill_and_confirm() to get notifications as each
	 * css is confirmed to be seen as killed on all CPUs.
	 */
	percpu_ref_kill_and_confirm(&css->refcnt, css_killed_ref_fn);
}

/**
 * cgroup_destroy_locked - the first stage of cgroup destruction
 * @cgrp: cgroup to be destroyed
 *
 * css's make use of percpu refcnts whose killing latency shouldn't be
 * exposed to userland and are RCU protected.  Also, cgroup core needs to
 * guarantee that css_tryget_online() won't succeed by the time
 * ->css_offline() is invoked.  To satisfy all the requirements,
 * destruction is implemented in the following two steps.
 *
 * s1. Verify @cgrp can be destroyed and mark it dying.  Remove all
 *     userland visible parts and start killing the percpu refcnts of
 *     css's.  Set up so that the next stage will be kicked off once all
 *     the percpu refcnts are confirmed to be killed.
 *
 * s2. Invoke ->css_offline(), mark the cgroup dead and proceed with the
 *     rest of destruction.  Once all cgroup references are gone, the
 *     cgroup is RCU-freed.
 *
 * This function implements s1.  After this step, @cgrp is gone as far as
 * the userland is concerned and a new cgroup with the same name may be
 * created.  As cgroup doesn't care about the names internally, this
 * doesn't cause any problem.
 */
static int cgroup_destroy_locked(struct cgroup *cgrp)
	__releases(&cgroup_mutex) __acquires(&cgroup_mutex)
{
	struct cgroup_subsys_state *css;
	bool empty;
	int ssid;

	lockdep_assert_held(&cgroup_mutex);

	/*
	 * css_set_rwsem synchronizes access to ->cset_links and prevents
	 * @cgrp from being removed while put_css_set() is in progress.
	 */
	down_read(&css_set_rwsem);
	empty = list_empty(&cgrp->cset_links);
	up_read(&css_set_rwsem);
	if (!empty)
		return -EBUSY;

	/*
	 * Make sure there's no live children.  We can't test emptiness of
	 * ->self.children as dead children linger on it while being
	 * drained; otherwise, "rmdir parent/child parent" may fail.
	 */
	if (css_has_online_children(&cgrp->self))
		return -EBUSY;

	/*
	 * Mark @cgrp dead.  This prevents further task migration and child
	 * creation by disabling cgroup_lock_live_group().
	 */
	cgrp->self.flags &= ~CSS_ONLINE;

	/* initiate massacre of all css's */
	for_each_css(css, ssid, cgrp)
		kill_css(css);

	/*
	 * Remove @cgrp directory along with the base files.  @cgrp has an
	 * extra ref on its kn.
	 */
	kernfs_remove(cgrp->kn);

	check_for_release(cgroup_parent(cgrp));

	/* put the base reference */
	percpu_ref_kill(&cgrp->self.refcnt);

	return 0;
};

static int cgroup_rmdir(struct kernfs_node *kn)
{
	struct cgroup *cgrp;
	int ret = 0;

	cgrp = cgroup_kn_lock_live(kn);
	if (!cgrp)
		return 0;

	ret = cgroup_destroy_locked(cgrp);

	cgroup_kn_unlock(kn);
	return ret;
}

static struct kernfs_syscall_ops cgroup_kf_syscall_ops = {
	.remount_fs		= cgroup_remount,
	.show_options		= cgroup_show_options,
	.mkdir			= cgroup_mkdir,
	.rmdir			= cgroup_rmdir,
	.rename			= cgroup_rename,
};

static void __init cgroup_init_subsys(struct cgroup_subsys *ss, bool early)
{
	struct cgroup_subsys_state *css;

	printk(KERN_INFO "Initializing cgroup subsys %s\n", ss->name);

	mutex_lock(&cgroup_mutex);

	idr_init(&ss->css_idr);
	INIT_LIST_HEAD(&ss->cfts);

	/* Create the root cgroup state for this subsystem */
	ss->root = &cgrp_dfl_root;
	css = ss->css_alloc(cgroup_css(&cgrp_dfl_root.cgrp, ss));
	/* We don't handle early failures gracefully */
	BUG_ON(IS_ERR(css));
	init_and_link_css(css, ss, &cgrp_dfl_root.cgrp);

	/*
	 * Root csses are never destroyed and we can't initialize
	 * percpu_ref during early init.  Disable refcnting.
	 */
	css->flags |= CSS_NO_REF;

	if (early) {
		/* allocation can't be done safely during early init */
		css->id = 1;
	} else {
		css->id = cgroup_idr_alloc(&ss->css_idr, css, 1, 2, GFP_KERNEL);
		BUG_ON(css->id < 0);
	}

	/* Update the init_css_set to contain a subsys
	 * pointer to this state - since the subsystem is
	 * newly registered, all tasks and hence the
	 * init_css_set is in the subsystem's root cgroup. */
	init_css_set.subsys[ss->id] = css;

	need_forkexit_callback |= ss->fork || ss->exit;

	/* At system boot, before all subsystems have been
	 * registered, no tasks have been forked, so we don't
	 * need to invoke fork callbacks here. */
	BUG_ON(!list_empty(&init_task.tasks));

	BUG_ON(online_css(css));

	mutex_unlock(&cgroup_mutex);
}
>>>>>>> v3.18

/**
 * cgroup_init_early - cgroup initialization at system boot
 *
 * Initialize cgroups at system boot, and initialize any
 * subsystems that request early init.
 */
int __init cgroup_init_early(void)
{
<<<<<<< HEAD
	int i;
	atomic_set(&init_css_set.refcount, 1);
	INIT_LIST_HEAD(&init_css_set.cg_links);
	INIT_LIST_HEAD(&init_css_set.tasks);
	INIT_HLIST_NODE(&init_css_set.hlist);
	css_set_count = 1;
	init_cgroup_root(&rootnode);
	root_count = 1;
	init_task.cgroups = &init_css_set;

	init_css_set_link.cg = &init_css_set;
	init_css_set_link.cgrp = dummytop;
	list_add(&init_css_set_link.cgrp_link_list,
		 &rootnode.top_cgroup.css_sets);
	list_add(&init_css_set_link.cg_link_list,
		 &init_css_set.cg_links);

	for (i = 0; i < CGROUP_SUBSYS_COUNT; i++) {
		struct cgroup_subsys *ss = subsys[i];

		/* at bootup time, we don't worry about modular subsystems */
		if (!ss || ss->module)
			continue;

		BUG_ON(!ss->name);
		BUG_ON(strlen(ss->name) > MAX_CGROUP_TYPE_NAMELEN);
		BUG_ON(!ss->css_alloc);
		BUG_ON(!ss->css_free);
		if (ss->subsys_id != i) {
			printk(KERN_ERR "cgroup: Subsys %s id == %d\n",
			       ss->name, ss->subsys_id);
			BUG();
		}

		if (ss->early_init)
			cgroup_init_subsys(ss);
=======
	static struct cgroup_sb_opts __initdata opts;
	struct cgroup_subsys *ss;
	int i;

	init_cgroup_root(&cgrp_dfl_root, &opts);
	cgrp_dfl_root.cgrp.self.flags |= CSS_NO_REF;

	RCU_INIT_POINTER(init_task.cgroups, &init_css_set);

	for_each_subsys(ss, i) {
		WARN(!ss->css_alloc || !ss->css_free || ss->name || ss->id,
		     "invalid cgroup_subsys %d:%s css_alloc=%p css_free=%p name:id=%d:%s\n",
		     i, cgroup_subsys_name[i], ss->css_alloc, ss->css_free,
		     ss->id, ss->name);
		WARN(strlen(cgroup_subsys_name[i]) > MAX_CGROUP_TYPE_NAMELEN,
		     "cgroup_subsys_name %s too long\n", cgroup_subsys_name[i]);

		ss->id = i;
		ss->name = cgroup_subsys_name[i];

		if (ss->early_init)
			cgroup_init_subsys(ss, true);
>>>>>>> v3.18
	}
	return 0;
}

/**
 * cgroup_init - cgroup initialization
 *
 * Register cgroup filesystem and /proc file, and initialize
 * any subsystems that didn't request early init.
 */
int __init cgroup_init(void)
{
<<<<<<< HEAD
	int err;
	int i;
	unsigned long key;

	err = bdi_init(&cgroup_backing_dev_info);
	if (err)
		return err;

	for (i = 0; i < CGROUP_SUBSYS_COUNT; i++) {
		struct cgroup_subsys *ss = subsys[i];

		/* at bootup time, we don't worry about modular subsystems */
		if (!ss || ss->module)
			continue;
		if (!ss->early_init)
			cgroup_init_subsys(ss);
		if (ss->use_id)
			cgroup_init_idr(ss, init_css_set.subsys[ss->subsys_id]);
	}
=======
	struct cgroup_subsys *ss;
	unsigned long key;
	int ssid, err;

	BUG_ON(cgroup_init_cftypes(NULL, cgroup_dfl_base_files));
	BUG_ON(cgroup_init_cftypes(NULL, cgroup_legacy_base_files));

	mutex_lock(&cgroup_mutex);
>>>>>>> v3.18

	/* Add init_css_set to the hash table */
	key = css_set_hash(init_css_set.subsys);
	hash_add(css_set_table, &init_css_set.hlist, key);
<<<<<<< HEAD
	BUG_ON(!init_root_id(&rootnode));

	cgroup_kobj = kobject_create_and_add("cgroup", fs_kobj);
	if (!cgroup_kobj) {
		err = -ENOMEM;
		goto out;
	}

	err = register_filesystem(&cgroup_fs_type);
	if (err < 0) {
		kobject_put(cgroup_kobj);
		goto out;
	}

	proc_create("cgroups", 0, NULL, &proc_cgroupstats_operations);

out:
	if (err)
		bdi_destroy(&cgroup_backing_dev_info);

	return err;
=======

	BUG_ON(cgroup_setup_root(&cgrp_dfl_root, 0));

	mutex_unlock(&cgroup_mutex);

	for_each_subsys(ss, ssid) {
		if (ss->early_init) {
			struct cgroup_subsys_state *css =
				init_css_set.subsys[ss->id];

			css->id = cgroup_idr_alloc(&ss->css_idr, css, 1, 2,
						   GFP_KERNEL);
			BUG_ON(css->id < 0);
		} else {
			cgroup_init_subsys(ss, false);
		}

		list_add_tail(&init_css_set.e_cset_node[ssid],
			      &cgrp_dfl_root.cgrp.e_csets[ssid]);

		/*
		 * Setting dfl_root subsys_mask needs to consider the
		 * disabled flag and cftype registration needs kmalloc,
		 * both of which aren't available during early_init.
		 */
		if (ss->disabled)
			continue;

		cgrp_dfl_root.subsys_mask |= 1 << ss->id;

		if (cgroup_legacy_files_on_dfl && !ss->dfl_cftypes)
			ss->dfl_cftypes = ss->legacy_cftypes;

		if (!ss->dfl_cftypes)
			cgrp_dfl_root_inhibit_ss_mask |= 1 << ss->id;

		if (ss->dfl_cftypes == ss->legacy_cftypes) {
			WARN_ON(cgroup_add_cftypes(ss, ss->dfl_cftypes));
		} else {
			WARN_ON(cgroup_add_dfl_cftypes(ss, ss->dfl_cftypes));
			WARN_ON(cgroup_add_legacy_cftypes(ss, ss->legacy_cftypes));
		}
	}

	cgroup_kobj = kobject_create_and_add("cgroup", fs_kobj);
	if (!cgroup_kobj)
		return -ENOMEM;

	err = register_filesystem(&cgroup_fs_type);
	if (err < 0) {
		kobject_put(cgroup_kobj);
		return err;
	}

	proc_create("cgroups", 0, NULL, &proc_cgroupstats_operations);
	return 0;
>>>>>>> v3.18
}

static int __init cgroup_wq_init(void)
{
	/*
	 * There isn't much point in executing destruction path in
	 * parallel.  Good chunk is serialized with cgroup_mutex anyway.
	 * Use 1 for @max_active.
	 *
	 * We would prefer to do this in cgroup_init() above, but that
	 * is called before init_workqueues(): so leave this until after.
	 */
	cgroup_destroy_wq = alloc_workqueue("cgroup_destroy", 0, 1);
	BUG_ON(!cgroup_destroy_wq);
<<<<<<< HEAD
=======

	/*
	 * Used to destroy pidlists and separate to serve as flush domain.
	 * Cap @max_active to 1 too.
	 */
	cgroup_pidlist_destroy_wq = alloc_workqueue("cgroup_pidlist_destroy",
						    0, 1);
	BUG_ON(!cgroup_pidlist_destroy_wq);

>>>>>>> v3.18
	return 0;
}
core_initcall(cgroup_wq_init);

/*
 * proc_cgroup_show()
 *  - Print task's cgroup paths into seq_file, one line for each hierarchy
 *  - Used for /proc/<pid>/cgroup.
<<<<<<< HEAD
 *  - No need to task_lock(tsk) on this tsk->cgroup reference, as it
 *    doesn't really matter if tsk->cgroup changes after we read it,
 *    and we take cgroup_mutex, keeping cgroup_attach_task() from changing it
 *    anyway.  No need to check that tsk->cgroup != NULL, thanks to
 *    the_top_cgroup_hack in cgroup_exit(), which sets an exiting tasks
 *    cgroup to top_cgroup.
 */

/* TODO: Use a proper seq_file iterator */
int proc_cgroup_show(struct seq_file *m, void *v)
{
	struct pid *pid;
	struct task_struct *tsk;
	char *buf;
	int retval;
	struct cgroupfs_root *root;

	retval = -ENOMEM;
	buf = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!buf)
		goto out;

	retval = -ESRCH;
	pid = m->private;
	tsk = get_pid_task(pid, PIDTYPE_PID);
	if (!tsk)
		goto out_free;

	retval = 0;

	mutex_lock(&cgroup_mutex);

	for_each_active_root(root) {
		struct cgroup_subsys *ss;
		struct cgroup *cgrp;
		int count = 0;

		seq_printf(m, "%d:", root->hierarchy_id);
		for_each_subsys(root, ss)
			seq_printf(m, "%s%s", count++ ? "," : "", ss->name);
=======
 */
int proc_cgroup_show(struct seq_file *m, struct pid_namespace *ns,
		     struct pid *pid, struct task_struct *tsk)
{
	char *buf, *path;
	int retval;
	struct cgroup_root *root;

	retval = -ENOMEM;
	buf = kmalloc(PATH_MAX, GFP_KERNEL);
	if (!buf)
		goto out;

	mutex_lock(&cgroup_mutex);
	down_read(&css_set_rwsem);

	for_each_root(root) {
		struct cgroup_subsys *ss;
		struct cgroup *cgrp;
		int ssid, count = 0;

		if (root == &cgrp_dfl_root && !cgrp_dfl_root_visible)
			continue;

		seq_printf(m, "%d:", root->hierarchy_id);
		for_each_subsys(ss, ssid)
			if (root->subsys_mask & (1 << ssid))
				seq_printf(m, "%s%s", count++ ? "," : "", ss->name);
>>>>>>> v3.18
		if (strlen(root->name))
			seq_printf(m, "%sname=%s", count ? "," : "",
				   root->name);
		seq_putc(m, ':');
		cgrp = task_cgroup_from_root(tsk, root);
<<<<<<< HEAD
		retval = cgroup_path(cgrp, buf, PAGE_SIZE);
		if (retval < 0)
			goto out_unlock;
		seq_puts(m, buf);
		seq_putc(m, '\n');
	}

out_unlock:
	mutex_unlock(&cgroup_mutex);
	put_task_struct(tsk);
out_free:
=======
		path = cgroup_path(cgrp, buf, PATH_MAX);
		if (!path) {
			retval = -ENAMETOOLONG;
			goto out_unlock;
		}
		seq_puts(m, path);
		seq_putc(m, '\n');
	}

	retval = 0;
out_unlock:
	up_read(&css_set_rwsem);
	mutex_unlock(&cgroup_mutex);
>>>>>>> v3.18
	kfree(buf);
out:
	return retval;
}

/* Display information about each subsystem and each hierarchy */
static int proc_cgroupstats_show(struct seq_file *m, void *v)
{
<<<<<<< HEAD
=======
	struct cgroup_subsys *ss;
>>>>>>> v3.18
	int i;

	seq_puts(m, "#subsys_name\thierarchy\tnum_cgroups\tenabled\n");
	/*
	 * ideally we don't want subsystems moving around while we do this.
	 * cgroup_mutex is also necessary to guarantee an atomic snapshot of
	 * subsys/hierarchy state.
	 */
	mutex_lock(&cgroup_mutex);
<<<<<<< HEAD
	for (i = 0; i < CGROUP_SUBSYS_COUNT; i++) {
		struct cgroup_subsys *ss = subsys[i];
		if (ss == NULL)
			continue;
		seq_printf(m, "%s\t%d\t%d\t%d\n",
			   ss->name, ss->root->hierarchy_id,
			   ss->root->number_of_cgroups, !ss->disabled);
	}
=======

	for_each_subsys(ss, i)
		seq_printf(m, "%s\t%d\t%d\t%d\n",
			   ss->name, ss->root->hierarchy_id,
			   atomic_read(&ss->root->nr_cgrps), !ss->disabled);

>>>>>>> v3.18
	mutex_unlock(&cgroup_mutex);
	return 0;
}

static int cgroupstats_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_cgroupstats_show, NULL);
}

static const struct file_operations proc_cgroupstats_operations = {
	.open = cgroupstats_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

/**
<<<<<<< HEAD
 * cgroup_fork - attach newly forked task to its parents cgroup.
 * @child: pointer to task_struct of forking parent process.
 *
 * Description: A task inherits its parent's cgroup at fork().
 *
 * A pointer to the shared css_set was automatically copied in
 * fork.c by dup_task_struct().  However, we ignore that copy, since
 * it was not made under the protection of RCU or cgroup_mutex, so
 * might no longer be a valid cgroup pointer.  cgroup_attach_task() might
 * have already changed current->cgroups, allowing the previously
 * referenced cgroup group to be removed and freed.
 *
 * At the point that cgroup_fork() is called, 'current' is the parent
 * task, and the passed argument 'child' points to the child task.
 */
void cgroup_fork(struct task_struct *child)
{
	task_lock(current);
	child->cgroups = current->cgroups;
	get_css_set(child->cgroups);
	task_unlock(current);
=======
 * cgroup_fork - initialize cgroup related fields during copy_process()
 * @child: pointer to task_struct of forking parent process.
 *
 * A task is associated with the init_css_set until cgroup_post_fork()
 * attaches it to the parent's css_set.  Empty cg_list indicates that
 * @child isn't holding reference to its css_set.
 */
void cgroup_fork(struct task_struct *child)
{
	RCU_INIT_POINTER(child->cgroups, &init_css_set);
>>>>>>> v3.18
	INIT_LIST_HEAD(&child->cg_list);
}

/**
 * cgroup_post_fork - called on a new task after adding it to the task list
 * @child: the task in question
 *
 * Adds the task to the list running through its css_set if necessary and
 * call the subsystem fork() callbacks.  Has to be after the task is
 * visible on the task list in case we race with the first call to
<<<<<<< HEAD
 * cgroup_iter_start() - to guarantee that the new task ends up on its
=======
 * cgroup_task_iter_start() - to guarantee that the new task ends up on its
>>>>>>> v3.18
 * list.
 */
void cgroup_post_fork(struct task_struct *child)
{
<<<<<<< HEAD
	int i;

	/*
	 * use_task_css_set_links is set to 1 before we walk the tasklist
	 * under the tasklist_lock and we read it here after we added the child
	 * to the tasklist under the tasklist_lock as well. If the child wasn't
	 * yet in the tasklist when we walked through it from
	 * cgroup_enable_task_cg_lists(), then use_task_css_set_links value
	 * should be visible now due to the paired locking and barriers implied
	 * by LOCK/UNLOCK: it is written before the tasklist_lock unlock
	 * in cgroup_enable_task_cg_lists() and read here after the tasklist_lock
	 * lock on fork.
	 */
	if (use_task_css_set_links) {
		write_lock(&css_set_lock);
		task_lock(child);
		if (list_empty(&child->cg_list))
			list_add(&child->cg_list, &child->cgroups->tasks);
		task_unlock(child);
		write_unlock(&css_set_lock);
=======
	struct cgroup_subsys *ss;
	int i;

	/*
	 * This may race against cgroup_enable_task_cg_lists().  As that
	 * function sets use_task_css_set_links before grabbing
	 * tasklist_lock and we just went through tasklist_lock to add
	 * @child, it's guaranteed that either we see the set
	 * use_task_css_set_links or cgroup_enable_task_cg_lists() sees
	 * @child during its iteration.
	 *
	 * If we won the race, @child is associated with %current's
	 * css_set.  Grabbing css_set_rwsem guarantees both that the
	 * association is stable, and, on completion of the parent's
	 * migration, @child is visible in the source of migration or
	 * already in the destination cgroup.  This guarantee is necessary
	 * when implementing operations which need to migrate all tasks of
	 * a cgroup to another.
	 *
	 * Note that if we lose to cgroup_enable_task_cg_lists(), @child
	 * will remain in init_css_set.  This is safe because all tasks are
	 * in the init_css_set before cg_links is enabled and there's no
	 * operation which transfers all tasks out of init_css_set.
	 */
	if (use_task_css_set_links) {
		struct css_set *cset;

		down_write(&css_set_rwsem);
		cset = task_css_set(current);
		if (list_empty(&child->cg_list)) {
			rcu_assign_pointer(child->cgroups, cset);
			list_add(&child->cg_list, &cset->tasks);
			get_css_set(cset);
		}
		up_write(&css_set_rwsem);
>>>>>>> v3.18
	}

	/*
	 * Call ss->fork().  This must happen after @child is linked on
	 * css_set; otherwise, @child might change state between ->fork()
	 * and addition to css_set.
	 */
	if (need_forkexit_callback) {
<<<<<<< HEAD
		/*
		 * fork/exit callbacks are supported only for builtin
		 * subsystems, and the builtin section of the subsys
		 * array is immutable, so we don't need to lock the
		 * subsys array here. On the other hand, modular section
		 * of the array can be freed at module unload, so we
		 * can't touch that.
		 */
		for (i = 0; i < CGROUP_BUILTIN_SUBSYS_COUNT; i++) {
			struct cgroup_subsys *ss = subsys[i];

			if (ss->fork)
				ss->fork(child);
		}
=======
		for_each_subsys(ss, i)
			if (ss->fork)
				ss->fork(child);
>>>>>>> v3.18
	}
}

/**
 * cgroup_exit - detach cgroup from exiting task
 * @tsk: pointer to task_struct of exiting process
<<<<<<< HEAD
 * @run_callback: run exit callbacks?
=======
>>>>>>> v3.18
 *
 * Description: Detach cgroup from @tsk and release it.
 *
 * Note that cgroups marked notify_on_release force every task in
 * them to take the global cgroup_mutex mutex when exiting.
 * This could impact scaling on very large systems.  Be reluctant to
 * use notify_on_release cgroups where very high task exit scaling
 * is required on large systems.
 *
<<<<<<< HEAD
 * the_top_cgroup_hack:
 *
 *    Set the exiting tasks cgroup to the root cgroup (top_cgroup).
 *
 *    We call cgroup_exit() while the task is still competent to
 *    handle notify_on_release(), then leave the task attached to the
 *    root cgroup in each hierarchy for the remainder of its exit.
 *
 *    To do this properly, we would increment the reference count on
 *    top_cgroup, and near the very end of the kernel/exit.c do_exit()
 *    code we would add a second cgroup function call, to drop that
 *    reference.  This would just create an unnecessary hot spot on
 *    the top_cgroup reference count, to no avail.
 *
 *    Normally, holding a reference to a cgroup without bumping its
 *    count is unsafe.   The cgroup could go away, or someone could
 *    attach us to a different cgroup, decrementing the count on
 *    the first cgroup that we never incremented.  But in this case,
 *    top_cgroup isn't going away, and either task has PF_EXITING set,
 *    which wards off any cgroup_attach_task() attempts, or task is a failed
 *    fork, never visible to cgroup_attach_task.
 */
void cgroup_exit(struct task_struct *tsk, int run_callbacks)
{
	struct css_set *cg;
	int i;

	/*
	 * Unlink from the css_set task list if necessary.
	 * Optimistically check cg_list before taking
	 * css_set_lock
	 */
	if (!list_empty(&tsk->cg_list)) {
		write_lock(&css_set_lock);
		if (!list_empty(&tsk->cg_list))
			list_del_init(&tsk->cg_list);
		write_unlock(&css_set_lock);
	}

	/* Reassign the task to the init_css_set. */
	task_lock(tsk);
	cg = tsk->cgroups;
	tsk->cgroups = &init_css_set;

	if (run_callbacks && need_forkexit_callback) {
		/*
		 * fork/exit callbacks are supported only for builtin
		 * subsystems, see cgroup_post_fork() for details.
		 */
		for (i = 0; i < CGROUP_BUILTIN_SUBSYS_COUNT; i++) {
			struct cgroup_subsys *ss = subsys[i];

			if (ss->exit) {
				struct cgroup *old_cgrp =
					rcu_dereference_raw(cg->subsys[i])->cgroup;
				struct cgroup *cgrp = task_cgroup(tsk, i);
				ss->exit(cgrp, old_cgrp, tsk);
			}
		}
	}
	task_unlock(tsk);

	put_css_set_taskexit(cg);
=======
 * We set the exiting tasks cgroup to the root cgroup (top_cgroup).  We
 * call cgroup_exit() while the task is still competent to handle
 * notify_on_release(), then leave the task attached to the root cgroup in
 * each hierarchy for the remainder of its exit.  No need to bother with
 * init_css_set refcnting.  init_css_set never goes away and we can't race
 * with migration path - PF_EXITING is visible to migration path.
 */
void cgroup_exit(struct task_struct *tsk)
{
	struct cgroup_subsys *ss;
	struct css_set *cset;
	bool put_cset = false;
	int i;

	/*
	 * Unlink from @tsk from its css_set.  As migration path can't race
	 * with us, we can check cg_list without grabbing css_set_rwsem.
	 */
	if (!list_empty(&tsk->cg_list)) {
		down_write(&css_set_rwsem);
		list_del_init(&tsk->cg_list);
		up_write(&css_set_rwsem);
		put_cset = true;
	}

	/* Reassign the task to the init_css_set. */
	cset = task_css_set(tsk);
	RCU_INIT_POINTER(tsk->cgroups, &init_css_set);

	if (need_forkexit_callback) {
		/* see cgroup_post_fork() for details */
		for_each_subsys(ss, i) {
			if (ss->exit) {
				struct cgroup_subsys_state *old_css = cset->subsys[i];
				struct cgroup_subsys_state *css = task_css(tsk, i);

				ss->exit(css, old_css, tsk);
			}
		}
	}

	if (put_cset)
		put_css_set(cset);
>>>>>>> v3.18
}

static void check_for_release(struct cgroup *cgrp)
{
<<<<<<< HEAD
	/* All of these checks rely on RCU to keep the cgroup
	 * structure alive */
	if (cgroup_is_releasable(cgrp) &&
	    !atomic_read(&cgrp->count) && list_empty(&cgrp->children)) {
		/*
		 * Control Group is currently removeable. If it's not
		 * already queued for a userspace notification, queue
		 * it now
		 */
		int need_schedule_work = 0;

		raw_spin_lock(&release_list_lock);
		if (!cgroup_is_removed(cgrp) &&
		    list_empty(&cgrp->release_list)) {
			list_add(&cgrp->release_list, &release_list);
			need_schedule_work = 1;
		}
		raw_spin_unlock(&release_list_lock);
		if (need_schedule_work)
			schedule_work(&release_agent_work);
	}
}

/* Caller must verify that the css is not for root cgroup */
bool __css_tryget(struct cgroup_subsys_state *css)
{
	while (true) {
		int t, v;

		v = css_refcnt(css);
		t = atomic_cmpxchg(&css->refcnt, v, v + 1);
		if (likely(t == v))
			return true;
		else if (t < 0)
			return false;
		cpu_relax();
	}
}
EXPORT_SYMBOL_GPL(__css_tryget);

/* Caller must verify that the css is not for root cgroup */
void __css_put(struct cgroup_subsys_state *css)
{
	int v;

	v = css_unbias_refcnt(atomic_dec_return(&css->refcnt));
	if (v == 0)
		queue_work(cgroup_destroy_wq, &css->dput_work);
}
EXPORT_SYMBOL_GPL(__css_put);
=======
	if (notify_on_release(cgrp) && !cgroup_has_tasks(cgrp) &&
	    !css_has_online_children(&cgrp->self) && !cgroup_is_dead(cgrp))
		schedule_work(&cgrp->release_agent_work);
}
>>>>>>> v3.18

/*
 * Notify userspace when a cgroup is released, by running the
 * configured release agent with the name of the cgroup (path
 * relative to the root of cgroup file system) as the argument.
 *
 * Most likely, this user command will try to rmdir this cgroup.
 *
 * This races with the possibility that some other task will be
 * attached to this cgroup before it is removed, or that some other
 * user task will 'mkdir' a child cgroup of this cgroup.  That's ok.
 * The presumed 'rmdir' will fail quietly if this cgroup is no longer
 * unused, and this cgroup will be reprieved from its death sentence,
 * to continue to serve a useful existence.  Next time it's released,
 * we will get notified again, if it still has 'notify_on_release' set.
 *
 * The final arg to call_usermodehelper() is UMH_WAIT_EXEC, which
 * means only wait until the task is successfully execve()'d.  The
 * separate release agent task is forked by call_usermodehelper(),
 * then control in this thread returns here, without waiting for the
 * release agent task.  We don't bother to wait because the caller of
 * this routine has no use for the exit status of the release agent
 * task, so no sense holding our caller up for that.
 */
static void cgroup_release_agent(struct work_struct *work)
{
<<<<<<< HEAD
	BUG_ON(work != &release_agent_work);
	mutex_lock(&cgroup_mutex);
	raw_spin_lock(&release_list_lock);
	while (!list_empty(&release_list)) {
		char *argv[3], *envp[3];
		int i;
		char *pathbuf = NULL, *agentbuf = NULL;
		struct cgroup *cgrp = list_entry(release_list.next,
						    struct cgroup,
						    release_list);
		list_del_init(&cgrp->release_list);
		raw_spin_unlock(&release_list_lock);
		pathbuf = kmalloc(PAGE_SIZE, GFP_KERNEL);
		if (!pathbuf)
			goto continue_free;
		if (cgroup_path(cgrp, pathbuf, PAGE_SIZE) < 0)
			goto continue_free;
		agentbuf = kstrdup(cgrp->root->release_agent_path, GFP_KERNEL);
		if (!agentbuf)
			goto continue_free;

		i = 0;
		argv[i++] = agentbuf;
		argv[i++] = pathbuf;
		argv[i] = NULL;

		i = 0;
		/* minimal command environment */
		envp[i++] = "HOME=/";
		envp[i++] = "PATH=/sbin:/bin:/usr/sbin:/usr/bin";
		envp[i] = NULL;

		/* Drop the lock while we invoke the usermode helper,
		 * since the exec could involve hitting disk and hence
		 * be a slow process */
		mutex_unlock(&cgroup_mutex);
		call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC);
		mutex_lock(&cgroup_mutex);
 continue_free:
		kfree(pathbuf);
		kfree(agentbuf);
		raw_spin_lock(&release_list_lock);
	}
	raw_spin_unlock(&release_list_lock);
	mutex_unlock(&cgroup_mutex);
=======
	struct cgroup *cgrp =
		container_of(work, struct cgroup, release_agent_work);
	char *pathbuf = NULL, *agentbuf = NULL, *path;
	char *argv[3], *envp[3];

	mutex_lock(&cgroup_mutex);

	pathbuf = kmalloc(PATH_MAX, GFP_KERNEL);
	agentbuf = kstrdup(cgrp->root->release_agent_path, GFP_KERNEL);
	if (!pathbuf || !agentbuf)
		goto out;

	path = cgroup_path(cgrp, pathbuf, PATH_MAX);
	if (!path)
		goto out;

	argv[0] = agentbuf;
	argv[1] = path;
	argv[2] = NULL;

	/* minimal command environment */
	envp[0] = "HOME=/";
	envp[1] = "PATH=/sbin:/bin:/usr/sbin:/usr/bin";
	envp[2] = NULL;

	mutex_unlock(&cgroup_mutex);
	call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC);
	goto out_free;
out:
	mutex_unlock(&cgroup_mutex);
out_free:
	kfree(agentbuf);
	kfree(pathbuf);
>>>>>>> v3.18
}

static int __init cgroup_disable(char *str)
{
<<<<<<< HEAD
	int i;
	char *token;
=======
	struct cgroup_subsys *ss;
	char *token;
	int i;
>>>>>>> v3.18

	while ((token = strsep(&str, ",")) != NULL) {
		if (!*token)
			continue;
<<<<<<< HEAD
		for (i = 0; i < CGROUP_SUBSYS_COUNT; i++) {
			struct cgroup_subsys *ss = subsys[i];

			/*
			 * cgroup_disable, being at boot time, can't
			 * know about module subsystems, so we don't
			 * worry about them.
			 */
			if (!ss || ss->module)
				continue;

=======

		for_each_subsys(ss, i) {
>>>>>>> v3.18
			if (!strcmp(token, ss->name)) {
				ss->disabled = 1;
				printk(KERN_INFO "Disabling %s control group"
					" subsystem\n", ss->name);
				break;
			}
		}
	}
	return 1;
}
__setup("cgroup_disable=", cgroup_disable);

<<<<<<< HEAD
/*
 * Functons for CSS ID.
 */

/*
 *To get ID other than 0, this should be called when !cgroup_is_removed().
 */
unsigned short css_id(struct cgroup_subsys_state *css)
{
	struct css_id *cssid;

	/*
	 * This css_id() can return correct value when somone has refcnt
	 * on this or this is under rcu_read_lock(). Once css->id is allocated,
	 * it's unchanged until freed.
	 */
	cssid = rcu_dereference_check(css->id, css_refcnt(css));

	if (cssid)
		return cssid->id;
	return 0;
}
EXPORT_SYMBOL_GPL(css_id);

unsigned short css_depth(struct cgroup_subsys_state *css)
{
	struct css_id *cssid;

	cssid = rcu_dereference_check(css->id, css_refcnt(css));

	if (cssid)
		return cssid->depth;
	return 0;
}
EXPORT_SYMBOL_GPL(css_depth);

/**
 *  css_is_ancestor - test "root" css is an ancestor of "child"
 * @child: the css to be tested.
 * @root: the css supporsed to be an ancestor of the child.
 *
 * Returns true if "root" is an ancestor of "child" in its hierarchy. Because
 * this function reads css->id, the caller must hold rcu_read_lock().
 * But, considering usual usage, the csses should be valid objects after test.
 * Assuming that the caller will do some action to the child if this returns
 * returns true, the caller must take "child";s reference count.
 * If "child" is valid object and this returns true, "root" is valid, too.
 */

bool css_is_ancestor(struct cgroup_subsys_state *child,
		    const struct cgroup_subsys_state *root)
{
	struct css_id *child_id;
	struct css_id *root_id;

	child_id  = rcu_dereference(child->id);
	if (!child_id)
		return false;
	root_id = rcu_dereference(root->id);
	if (!root_id)
		return false;
	if (child_id->depth < root_id->depth)
		return false;
	if (child_id->stack[root_id->depth] != root_id->id)
		return false;
	return true;
}

void free_css_id(struct cgroup_subsys *ss, struct cgroup_subsys_state *css)
{
	struct css_id *id = css->id;
	/* When this is called before css_id initialization, id can be NULL */
	if (!id)
		return;

	BUG_ON(!ss->use_id);

	rcu_assign_pointer(id->css, NULL);
	rcu_assign_pointer(css->id, NULL);
	spin_lock(&ss->id_lock);
	idr_remove(&ss->idr, id->id);
	spin_unlock(&ss->id_lock);
	kfree_rcu(id, rcu_head);
}
EXPORT_SYMBOL_GPL(free_css_id);

/*
 * This is called by init or create(). Then, calls to this function are
 * always serialized (By cgroup_mutex() at create()).
 */

static struct css_id *get_new_cssid(struct cgroup_subsys *ss, int depth)
{
	struct css_id *newid;
	int ret, size;

	BUG_ON(!ss->use_id);

	size = sizeof(*newid) + sizeof(unsigned short) * (depth + 1);
	newid = kzalloc(size, GFP_KERNEL);
	if (!newid)
		return ERR_PTR(-ENOMEM);

	idr_preload(GFP_KERNEL);
	spin_lock(&ss->id_lock);
	/* Don't use 0. allocates an ID of 1-65535 */
	ret = idr_alloc(&ss->idr, newid, 1, CSS_ID_MAX + 1, GFP_NOWAIT);
	spin_unlock(&ss->id_lock);
	idr_preload_end();

	/* Returns error when there are no free spaces for new ID.*/
	if (ret < 0)
		goto err_out;

	newid->id = ret;
	newid->depth = depth;
	return newid;
err_out:
	kfree(newid);
	return ERR_PTR(ret);

}

static int __init_or_module cgroup_init_idr(struct cgroup_subsys *ss,
					    struct cgroup_subsys_state *rootcss)
{
	struct css_id *newid;

	spin_lock_init(&ss->id_lock);
	idr_init(&ss->idr);

	newid = get_new_cssid(ss, 0);
	if (IS_ERR(newid))
		return PTR_ERR(newid);

	newid->stack[0] = newid->id;
	newid->css = rootcss;
	rootcss->id = newid;
	return 0;
}

static int alloc_css_id(struct cgroup_subsys *ss, struct cgroup *parent,
			struct cgroup *child)
{
	int subsys_id, i, depth = 0;
	struct cgroup_subsys_state *parent_css, *child_css;
	struct css_id *child_id, *parent_id;

	subsys_id = ss->subsys_id;
	parent_css = parent->subsys[subsys_id];
	child_css = child->subsys[subsys_id];
	parent_id = parent_css->id;
	depth = parent_id->depth + 1;

	child_id = get_new_cssid(ss, depth);
	if (IS_ERR(child_id))
		return PTR_ERR(child_id);

	for (i = 0; i < depth; i++)
		child_id->stack[i] = parent_id->stack[i];
	child_id->stack[depth] = child_id->id;
	/*
	 * child_id->css pointer will be set after this cgroup is available
	 * see cgroup_populate_dir()
	 */
	rcu_assign_pointer(child_css->id, child_id);

	return 0;
}

/**
 * css_lookup - lookup css by id
 * @ss: cgroup subsys to be looked into.
 * @id: the id
 *
 * Returns pointer to cgroup_subsys_state if there is valid one with id.
 * NULL if not. Should be called under rcu_read_lock()
 */
struct cgroup_subsys_state *css_lookup(struct cgroup_subsys *ss, int id)
{
	struct css_id *cssid = NULL;

	BUG_ON(!ss->use_id);
	cssid = idr_find(&ss->idr, id);

	if (unlikely(!cssid))
		return NULL;

	return rcu_dereference(cssid->css);
}
EXPORT_SYMBOL_GPL(css_lookup);

/*
 * get corresponding css from file open on cgroupfs directory
 */
struct cgroup_subsys_state *cgroup_css_from_dir(struct file *f, int id)
{
	struct cgroup *cgrp;
	struct inode *inode;
	struct cgroup_subsys_state *css;

	inode = file_inode(f);
	/* check in cgroup filesystem dir */
	if (inode->i_op != &cgroup_dir_inode_operations)
		return ERR_PTR(-EBADF);

	if (id < 0 || id >= CGROUP_SUBSYS_COUNT)
		return ERR_PTR(-EINVAL);

	/* get cgroup */
	cgrp = __d_cgrp(f->f_dentry);
	css = cgrp->subsys[id];
	return css ? css : ERR_PTR(-ENOENT);
}

#ifdef CONFIG_CGROUP_DEBUG
static struct cgroup_subsys_state *debug_css_alloc(struct cgroup *cont)
=======
static int __init cgroup_set_legacy_files_on_dfl(char *str)
{
	printk("cgroup: using legacy files on the default hierarchy\n");
	cgroup_legacy_files_on_dfl = true;
	return 0;
}
__setup("cgroup__DEVEL__legacy_files_on_dfl", cgroup_set_legacy_files_on_dfl);

/**
 * css_tryget_online_from_dir - get corresponding css from a cgroup dentry
 * @dentry: directory dentry of interest
 * @ss: subsystem of interest
 *
 * If @dentry is a directory for a cgroup which has @ss enabled on it, try
 * to get the corresponding css and return it.  If such css doesn't exist
 * or can't be pinned, an ERR_PTR value is returned.
 */
struct cgroup_subsys_state *css_tryget_online_from_dir(struct dentry *dentry,
						       struct cgroup_subsys *ss)
{
	struct kernfs_node *kn = kernfs_node_from_dentry(dentry);
	struct cgroup_subsys_state *css = NULL;
	struct cgroup *cgrp;

	/* is @dentry a cgroup dir? */
	if (dentry->d_sb->s_type != &cgroup_fs_type || !kn ||
	    kernfs_type(kn) != KERNFS_DIR)
		return ERR_PTR(-EBADF);

	rcu_read_lock();

	/*
	 * This path doesn't originate from kernfs and @kn could already
	 * have been or be removed at any point.  @kn->priv is RCU
	 * protected for this access.  See css_release_work_fn() for details.
	 */
	cgrp = rcu_dereference(kn->priv);
	if (cgrp)
		css = cgroup_css(cgrp, ss);

	if (!css || !css_tryget_online(css))
		css = ERR_PTR(-ENOENT);

	rcu_read_unlock();
	return css;
}

/**
 * css_from_id - lookup css by id
 * @id: the cgroup id
 * @ss: cgroup subsys to be looked into
 *
 * Returns the css if there's valid one with @id, otherwise returns NULL.
 * Should be called under rcu_read_lock().
 */
struct cgroup_subsys_state *css_from_id(int id, struct cgroup_subsys *ss)
{
	WARN_ON_ONCE(!rcu_read_lock_held());
	return idr_find(&ss->css_idr, id);
}

#ifdef CONFIG_CGROUP_DEBUG
static struct cgroup_subsys_state *
debug_css_alloc(struct cgroup_subsys_state *parent_css)
>>>>>>> v3.18
{
	struct cgroup_subsys_state *css = kzalloc(sizeof(*css), GFP_KERNEL);

	if (!css)
		return ERR_PTR(-ENOMEM);

	return css;
}

<<<<<<< HEAD
static void debug_css_free(struct cgroup *cont)
{
	kfree(cont->subsys[debug_subsys_id]);
}

static u64 cgroup_refcount_read(struct cgroup *cont, struct cftype *cft)
{
	return atomic_read(&cont->count);
}

static u64 debug_taskcount_read(struct cgroup *cont, struct cftype *cft)
{
	return cgroup_task_count(cont);
}

static u64 current_css_set_read(struct cgroup *cont, struct cftype *cft)
=======
static void debug_css_free(struct cgroup_subsys_state *css)
{
	kfree(css);
}

static u64 debug_taskcount_read(struct cgroup_subsys_state *css,
				struct cftype *cft)
{
	return cgroup_task_count(css->cgroup);
}

static u64 current_css_set_read(struct cgroup_subsys_state *css,
				struct cftype *cft)
>>>>>>> v3.18
{
	return (u64)(unsigned long)current->cgroups;
}

<<<<<<< HEAD
static u64 current_css_set_refcount_read(struct cgroup *cont,
					   struct cftype *cft)
=======
static u64 current_css_set_refcount_read(struct cgroup_subsys_state *css,
					 struct cftype *cft)
>>>>>>> v3.18
{
	u64 count;

	rcu_read_lock();
<<<<<<< HEAD
	count = atomic_read(&current->cgroups->refcount);
=======
	count = atomic_read(&task_css_set(current)->refcount);
>>>>>>> v3.18
	rcu_read_unlock();
	return count;
}

<<<<<<< HEAD
static int current_css_set_cg_links_read(struct cgroup *cont,
					 struct cftype *cft,
					 struct seq_file *seq)
{
	struct cg_cgroup_link *link;
	struct css_set *cg;

	read_lock(&css_set_lock);
	rcu_read_lock();
	cg = rcu_dereference(current->cgroups);
	list_for_each_entry(link, &cg->cg_links, cg_link_list) {
		struct cgroup *c = link->cgrp;
		const char *name;

		if (c->dentry)
			name = c->dentry->d_name.name;
		else
			name = "?";
		seq_printf(seq, "Root %d group %s\n",
			   c->root->hierarchy_id, name);
	}
	rcu_read_unlock();
	read_unlock(&css_set_lock);
=======
static int current_css_set_cg_links_read(struct seq_file *seq, void *v)
{
	struct cgrp_cset_link *link;
	struct css_set *cset;
	char *name_buf;

	name_buf = kmalloc(NAME_MAX + 1, GFP_KERNEL);
	if (!name_buf)
		return -ENOMEM;

	down_read(&css_set_rwsem);
	rcu_read_lock();
	cset = rcu_dereference(current->cgroups);
	list_for_each_entry(link, &cset->cgrp_links, cgrp_link) {
		struct cgroup *c = link->cgrp;

		cgroup_name(c, name_buf, NAME_MAX + 1);
		seq_printf(seq, "Root %d group %s\n",
			   c->root->hierarchy_id, name_buf);
	}
	rcu_read_unlock();
	up_read(&css_set_rwsem);
	kfree(name_buf);
>>>>>>> v3.18
	return 0;
}

#define MAX_TASKS_SHOWN_PER_CSS 25
<<<<<<< HEAD
static int cgroup_css_links_read(struct cgroup *cont,
				 struct cftype *cft,
				 struct seq_file *seq)
{
	struct cg_cgroup_link *link;

	read_lock(&css_set_lock);
	list_for_each_entry(link, &cont->css_sets, cgrp_link_list) {
		struct css_set *cg = link->cg;
		struct task_struct *task;
		int count = 0;
		seq_printf(seq, "css_set %pK\n", cg);
		list_for_each_entry(task, &cg->tasks, cg_list) {
			if (count++ > MAX_TASKS_SHOWN_PER_CSS) {
				seq_puts(seq, "  ...\n");
				break;
			} else {
				seq_printf(seq, "  task %d\n",
					   task_pid_vnr(task));
			}
		}
	}
	read_unlock(&css_set_lock);
	return 0;
}

static u64 releasable_read(struct cgroup *cgrp, struct cftype *cft)
{
	return test_bit(CGRP_RELEASABLE, &cgrp->flags);
=======
static int cgroup_css_links_read(struct seq_file *seq, void *v)
{
	struct cgroup_subsys_state *css = seq_css(seq);
	struct cgrp_cset_link *link;

	down_read(&css_set_rwsem);
	list_for_each_entry(link, &css->cgroup->cset_links, cset_link) {
		struct css_set *cset = link->cset;
		struct task_struct *task;
		int count = 0;

		seq_printf(seq, "css_set %p\n", cset);

		list_for_each_entry(task, &cset->tasks, cg_list) {
			if (count++ > MAX_TASKS_SHOWN_PER_CSS)
				goto overflow;
			seq_printf(seq, "  task %d\n", task_pid_vnr(task));
		}

		list_for_each_entry(task, &cset->mg_tasks, cg_list) {
			if (count++ > MAX_TASKS_SHOWN_PER_CSS)
				goto overflow;
			seq_printf(seq, "  task %d\n", task_pid_vnr(task));
		}
		continue;
	overflow:
		seq_puts(seq, "  ...\n");
	}
	up_read(&css_set_rwsem);
	return 0;
}

static u64 releasable_read(struct cgroup_subsys_state *css, struct cftype *cft)
{
	return (!cgroup_has_tasks(css->cgroup) &&
		!css_has_online_children(&css->cgroup->self));
>>>>>>> v3.18
}

static struct cftype debug_files[] =  {
	{
<<<<<<< HEAD
		.name = "cgroup_refcount",
		.read_u64 = cgroup_refcount_read,
	},
	{
=======
>>>>>>> v3.18
		.name = "taskcount",
		.read_u64 = debug_taskcount_read,
	},

	{
		.name = "current_css_set",
		.read_u64 = current_css_set_read,
	},

	{
		.name = "current_css_set_refcount",
		.read_u64 = current_css_set_refcount_read,
	},

	{
		.name = "current_css_set_cg_links",
<<<<<<< HEAD
		.read_seq_string = current_css_set_cg_links_read,
=======
		.seq_show = current_css_set_cg_links_read,
>>>>>>> v3.18
	},

	{
		.name = "cgroup_css_links",
<<<<<<< HEAD
		.read_seq_string = cgroup_css_links_read,
=======
		.seq_show = cgroup_css_links_read,
>>>>>>> v3.18
	},

	{
		.name = "releasable",
		.read_u64 = releasable_read,
	},

	{ }	/* terminate */
};

<<<<<<< HEAD
struct cgroup_subsys debug_subsys = {
	.name = "debug",
	.css_alloc = debug_css_alloc,
	.css_free = debug_css_free,
	.subsys_id = debug_subsys_id,
	.base_cftypes = debug_files,
=======
struct cgroup_subsys debug_cgrp_subsys = {
	.css_alloc = debug_css_alloc,
	.css_free = debug_css_free,
	.legacy_cftypes = debug_files,
>>>>>>> v3.18
};
#endif /* CONFIG_CGROUP_DEBUG */
