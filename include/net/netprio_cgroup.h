/*
 * netprio_cgroup.h			Control Group Priority set
 *
 *
 * Authors:	Neil Horman <nhorman@tuxdriver.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#ifndef _NETPRIO_CGROUP_H
#define _NETPRIO_CGROUP_H
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
#include <linux/cgroup.h>
#include <linux/hardirq.h>
#include <linux/rcupdate.h>

<<<<<<< HEAD
<<<<<<< HEAD

#if IS_ENABLED(CONFIG_NETPRIO_CGROUP)
=======
#if IS_ENABLED(CONFIG_CGROUP_NET_PRIO)
>>>>>>> v3.18
=======
#if IS_ENABLED(CONFIG_CGROUP_NET_PRIO)
>>>>>>> v3.18
struct netprio_map {
	struct rcu_head rcu;
	u32 priomap_len;
	u32 priomap[];
};

<<<<<<< HEAD
<<<<<<< HEAD
struct cgroup_netprio_state {
	struct cgroup_subsys_state css;
};

extern void sock_update_netprioidx(struct sock *sk);

#if IS_BUILTIN(CONFIG_NETPRIO_CGROUP)
=======
void sock_update_netprioidx(struct sock *sk);
>>>>>>> v3.18
=======
void sock_update_netprioidx(struct sock *sk);
>>>>>>> v3.18

static inline u32 task_netprioidx(struct task_struct *p)
{
	struct cgroup_subsys_state *css;
	u32 idx;

	rcu_read_lock();
<<<<<<< HEAD
<<<<<<< HEAD
	css = task_subsys_state(p, net_prio_subsys_id);
=======
	css = task_css(p, net_prio_cgrp_id);
>>>>>>> v3.18
=======
	css = task_css(p, net_prio_cgrp_id);
>>>>>>> v3.18
	idx = css->cgroup->id;
	rcu_read_unlock();
	return idx;
}
<<<<<<< HEAD
<<<<<<< HEAD

#elif IS_MODULE(CONFIG_NETPRIO_CGROUP)

static inline u32 task_netprioidx(struct task_struct *p)
{
	struct cgroup_subsys_state *css;
	u32 idx = 0;

	rcu_read_lock();
	css = task_subsys_state(p, net_prio_subsys_id);
	if (css)
		idx = css->cgroup->id;
	rcu_read_unlock();
	return idx;
}
#endif

#else /* !CONFIG_NETPRIO_CGROUP */

=======
#else /* !CONFIG_CGROUP_NET_PRIO */
>>>>>>> v3.18
=======
#else /* !CONFIG_CGROUP_NET_PRIO */
>>>>>>> v3.18
static inline u32 task_netprioidx(struct task_struct *p)
{
	return 0;
}

#define sock_update_netprioidx(sk)

<<<<<<< HEAD
<<<<<<< HEAD
#endif /* CONFIG_NETPRIO_CGROUP */

=======
#endif /* CONFIG_CGROUP_NET_PRIO */
>>>>>>> v3.18
=======
#endif /* CONFIG_CGROUP_NET_PRIO */
>>>>>>> v3.18
#endif  /* _NET_CLS_CGROUP_H */
