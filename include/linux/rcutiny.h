/*
 * Read-Copy Update mechanism for mutual exclusion, the Bloatwatch edition.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
<<<<<<< HEAD
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
=======
 * along with this program; if not, you can access it online at
 * http://www.gnu.org/licenses/gpl-2.0.html.
>>>>>>> v3.18
 *
 * Copyright IBM Corporation, 2008
 *
 * Author: Paul E. McKenney <paulmck@linux.vnet.ibm.com>
 *
 * For detailed explanation of Read-Copy Update mechanism see -
 *		Documentation/RCU
 */
#ifndef __LINUX_TINY_H
#define __LINUX_TINY_H

#include <linux/cache.h>

<<<<<<< HEAD
static inline void rcu_init(void)
{
=======
static inline unsigned long get_state_synchronize_rcu(void)
{
	return 0;
}

static inline void cond_synchronize_rcu(unsigned long oldstate)
{
	might_sleep();
>>>>>>> v3.18
}

static inline void rcu_barrier_bh(void)
{
	wait_rcu_gp(call_rcu_bh);
}

static inline void rcu_barrier_sched(void)
{
	wait_rcu_gp(call_rcu_sched);
}

<<<<<<< HEAD
#ifdef CONFIG_TINY_RCU

=======
>>>>>>> v3.18
static inline void synchronize_rcu_expedited(void)
{
	synchronize_sched();	/* Only one CPU, so pretty fast anyway!!! */
}

static inline void rcu_barrier(void)
{
	rcu_barrier_sched();  /* Only one CPU, so only one list of callbacks! */
}

<<<<<<< HEAD
#else /* #ifdef CONFIG_TINY_RCU */

void synchronize_rcu_expedited(void);

static inline void rcu_barrier(void)
{
	wait_rcu_gp(call_rcu);
}

#endif /* #else #ifdef CONFIG_TINY_RCU */

=======
>>>>>>> v3.18
static inline void synchronize_rcu_bh(void)
{
	synchronize_sched();
}

static inline void synchronize_rcu_bh_expedited(void)
{
	synchronize_sched();
}

static inline void synchronize_sched_expedited(void)
{
	synchronize_sched();
}

static inline void kfree_call_rcu(struct rcu_head *head,
				  void (*func)(struct rcu_head *rcu))
{
	call_rcu(head, func);
}

<<<<<<< HEAD
#ifdef CONFIG_TINY_RCU

static inline void rcu_preempt_note_context_switch(void)
{
}

static inline int rcu_needs_cpu(int cpu, unsigned long *delta_jiffies)
{
	*delta_jiffies = ULONG_MAX;
	return 0;
}

#else /* #ifdef CONFIG_TINY_RCU */

void rcu_preempt_note_context_switch(void);
int rcu_preempt_needs_cpu(void);

static inline int rcu_needs_cpu(int cpu, unsigned long *delta_jiffies)
{
	*delta_jiffies = ULONG_MAX;
	return rcu_preempt_needs_cpu();
}

#endif /* #else #ifdef CONFIG_TINY_RCU */

static inline void rcu_note_context_switch(int cpu)
{
	rcu_sched_qs(cpu);
	rcu_preempt_note_context_switch();
=======
static inline void rcu_note_context_switch(int cpu)
{
	rcu_sched_qs();
>>>>>>> v3.18
}

/*
 * Take advantage of the fact that there is only one CPU, which
 * allows us to ignore virtualization-based context switches.
 */
static inline void rcu_virt_note_context_switch(int cpu)
{
}

/*
 * Return the number of grace periods.
 */
static inline long rcu_batches_completed(void)
{
	return 0;
}

/*
 * Return the number of bottom-half grace periods.
 */
static inline long rcu_batches_completed_bh(void)
{
	return 0;
}

static inline void rcu_force_quiescent_state(void)
{
}

static inline void rcu_bh_force_quiescent_state(void)
{
}

static inline void rcu_sched_force_quiescent_state(void)
{
}

<<<<<<< HEAD
=======
static inline void show_rcu_gp_kthreads(void)
{
}

>>>>>>> v3.18
static inline void rcu_cpu_stall_reset(void)
{
}

<<<<<<< HEAD
#ifdef CONFIG_DEBUG_LOCK_ALLOC
extern int rcu_scheduler_active __read_mostly;
extern void rcu_scheduler_starting(void);
=======
static inline void exit_rcu(void)
{
}

#ifdef CONFIG_DEBUG_LOCK_ALLOC
extern int rcu_scheduler_active __read_mostly;
void rcu_scheduler_starting(void);
>>>>>>> v3.18
#else /* #ifdef CONFIG_DEBUG_LOCK_ALLOC */
static inline void rcu_scheduler_starting(void)
{
}
#endif /* #else #ifdef CONFIG_DEBUG_LOCK_ALLOC */

<<<<<<< HEAD
=======
#if defined(CONFIG_DEBUG_LOCK_ALLOC) || defined(CONFIG_RCU_TRACE)

static inline bool rcu_is_watching(void)
{
	return __rcu_is_watching();
}

#else /* defined(CONFIG_DEBUG_LOCK_ALLOC) || defined(CONFIG_RCU_TRACE) */

static inline bool rcu_is_watching(void)
{
	return true;
}


#endif /* #else defined(CONFIG_DEBUG_LOCK_ALLOC) || defined(CONFIG_RCU_TRACE) */

>>>>>>> v3.18
#endif /* __LINUX_RCUTINY_H */
