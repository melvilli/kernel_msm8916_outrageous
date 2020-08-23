#ifndef __LINUX_VMPRESSURE_H
#define __LINUX_VMPRESSURE_H

#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/workqueue.h>
#include <linux/gfp.h>
#include <linux/types.h>
#include <linux/cgroup.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/eventfd.h>
>>>>>>> v3.18
=======
#include <linux/eventfd.h>
>>>>>>> v3.18

struct vmpressure {
	unsigned long scanned;
	unsigned long reclaimed;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long stall;
	/* The lock is used to keep the scanned/reclaimed above in sync. */
	struct mutex sr_lock;
=======
	/* The lock is used to keep the scanned/reclaimed above in sync. */
	struct spinlock sr_lock;
>>>>>>> v3.18
=======
	/* The lock is used to keep the scanned/reclaimed above in sync. */
	struct spinlock sr_lock;
>>>>>>> v3.18

	/* The list of vmpressure_event structs. */
	struct list_head events;
	/* Have to grab the lock on events traversal or modifications. */
	struct mutex events_lock;

	struct work_struct work;
};

struct mem_cgroup;

<<<<<<< HEAD
<<<<<<< HEAD
extern int vmpressure_notifier_register(struct notifier_block *nb);
extern int vmpressure_notifier_unregister(struct notifier_block *nb);
=======
#ifdef CONFIG_MEMCG
>>>>>>> v3.18
=======
#ifdef CONFIG_MEMCG
>>>>>>> v3.18
extern void vmpressure(gfp_t gfp, struct mem_cgroup *memcg,
		       unsigned long scanned, unsigned long reclaimed);
extern void vmpressure_prio(gfp_t gfp, struct mem_cgroup *memcg, int prio);

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_MEMCG
extern void vmpressure_init(struct vmpressure *vmpr);
extern struct vmpressure *memcg_to_vmpressure(struct mem_cgroup *memcg);
extern struct cgroup_subsys_state *vmpressure_to_css(struct vmpressure *vmpr);
extern struct vmpressure *css_to_vmpressure(struct cgroup_subsys_state *css);
extern int vmpressure_register_event(struct cgroup *cg, struct cftype *cft,
				     struct eventfd_ctx *eventfd,
				     const char *args);
extern void vmpressure_unregister_event(struct cgroup *cg, struct cftype *cft,
					struct eventfd_ctx *eventfd);
#else
static inline struct vmpressure *memcg_to_vmpressure(struct mem_cgroup *memcg)
{
	return NULL;
}
=======
=======
>>>>>>> v3.18
extern void vmpressure_init(struct vmpressure *vmpr);
extern void vmpressure_cleanup(struct vmpressure *vmpr);
extern struct vmpressure *memcg_to_vmpressure(struct mem_cgroup *memcg);
extern struct cgroup_subsys_state *vmpressure_to_css(struct vmpressure *vmpr);
extern int vmpressure_register_event(struct mem_cgroup *memcg,
				     struct eventfd_ctx *eventfd,
				     const char *args);
extern void vmpressure_unregister_event(struct mem_cgroup *memcg,
					struct eventfd_ctx *eventfd);
#else
static inline void vmpressure(gfp_t gfp, struct mem_cgroup *memcg,
			      unsigned long scanned, unsigned long reclaimed) {}
static inline void vmpressure_prio(gfp_t gfp, struct mem_cgroup *memcg,
				   int prio) {}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif /* CONFIG_MEMCG */
#endif /* __LINUX_VMPRESSURE_H */
