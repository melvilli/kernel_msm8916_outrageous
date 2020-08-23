/*
 * Split spinlock implementation out into its own file, so it can be
 * compiled in a FTRACE-compatible way.
 */
#include <linux/kernel_stat.h>
#include <linux/spinlock.h>
#include <linux/debugfs.h>
#include <linux/log2.h>
#include <linux/gfp.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/slab.h>
>>>>>>> v3.18
=======
#include <linux/slab.h>
>>>>>>> v3.18

#include <asm/paravirt.h>

#include <xen/interface/xen.h>
#include <xen/events.h>

#include "xen-ops.h"
#include "debugfs.h"

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_XEN_DEBUG_FS
static struct xen_spinlock_stats
{
	u64 taken;
	u32 taken_slow;
	u32 taken_slow_nested;
	u32 taken_slow_pickup;
	u32 taken_slow_spurious;
	u32 taken_slow_irqenable;

	u64 released;
	u32 released_slow;
	u32 released_slow_kicked;

#define HISTO_BUCKETS	30
	u32 histo_spin_total[HISTO_BUCKETS+1];
	u32 histo_spin_spinning[HISTO_BUCKETS+1];
	u32 histo_spin_blocked[HISTO_BUCKETS+1];

	u64 time_total;
	u64 time_spinning;
=======
=======
>>>>>>> v3.18
enum xen_contention_stat {
	TAKEN_SLOW,
	TAKEN_SLOW_PICKUP,
	TAKEN_SLOW_SPURIOUS,
	RELEASED_SLOW,
	RELEASED_SLOW_KICKED,
	NR_CONTENTION_STATS
};


#ifdef CONFIG_XEN_DEBUG_FS
#define HISTO_BUCKETS	30
static struct xen_spinlock_stats
{
	u32 contention_stats[NR_CONTENTION_STATS];
	u32 histo_spin_blocked[HISTO_BUCKETS+1];
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	u64 time_blocked;
} spinlock_stats;

static u8 zero_stats;

<<<<<<< HEAD
<<<<<<< HEAD
static unsigned lock_timeout = 1 << 10;
#define TIMEOUT lock_timeout

static inline void check_zero(void)
{
	if (unlikely(zero_stats)) {
		memset(&spinlock_stats, 0, sizeof(spinlock_stats));
		zero_stats = 0;
	}
}

#define ADD_STATS(elem, val)			\
	do { check_zero(); spinlock_stats.elem += (val); } while(0)
=======
=======
>>>>>>> v3.18
static inline void check_zero(void)
{
	u8 ret;
	u8 old = ACCESS_ONCE(zero_stats);
	if (unlikely(old)) {
		ret = cmpxchg(&zero_stats, old, 0);
		/* This ensures only one fellow resets the stat */
		if (ret == old)
			memset(&spinlock_stats, 0, sizeof(spinlock_stats));
	}
}

static inline void add_stats(enum xen_contention_stat var, u32 val)
{
	check_zero();
	spinlock_stats.contention_stats[var] += val;
}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

static inline u64 spin_time_start(void)
{
	return xen_clocksource_read();
}

static void __spin_time_accum(u64 delta, u32 *array)
{
	unsigned index = ilog2(delta);

	check_zero();

	if (index < HISTO_BUCKETS)
		array[index]++;
	else
		array[HISTO_BUCKETS]++;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void spin_time_accum_spinning(u64 start)
{
	u32 delta = xen_clocksource_read() - start;

	__spin_time_accum(delta, spinlock_stats.histo_spin_spinning);
	spinlock_stats.time_spinning += delta;
}

static inline void spin_time_accum_total(u64 start)
{
	u32 delta = xen_clocksource_read() - start;

	__spin_time_accum(delta, spinlock_stats.histo_spin_total);
	spinlock_stats.time_total += delta;
}

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
static inline void spin_time_accum_blocked(u64 start)
{
	u32 delta = xen_clocksource_read() - start;

	__spin_time_accum(delta, spinlock_stats.histo_spin_blocked);
	spinlock_stats.time_blocked += delta;
}
#else  /* !CONFIG_XEN_DEBUG_FS */
<<<<<<< HEAD
<<<<<<< HEAD
#define TIMEOUT			(1 << 10)
#define ADD_STATS(elem, val)	do { (void)(val); } while(0)
=======
static inline void add_stats(enum xen_contention_stat var, u32 val)
{
}
>>>>>>> v3.18
=======
static inline void add_stats(enum xen_contention_stat var, u32 val)
{
}
>>>>>>> v3.18

static inline u64 spin_time_start(void)
{
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void spin_time_accum_total(u64 start)
{
}
static inline void spin_time_accum_spinning(u64 start)
{
}
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
static inline void spin_time_accum_blocked(u64 start)
{
}
#endif  /* CONFIG_XEN_DEBUG_FS */

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * Size struct xen_spinlock so it's the same as arch_spinlock_t.
 */
#if NR_CPUS < 256
typedef u8 xen_spinners_t;
# define inc_spinners(xl) \
	asm(LOCK_PREFIX " incb %0" : "+m" ((xl)->spinners) : : "memory");
# define dec_spinners(xl) \
	asm(LOCK_PREFIX " decb %0" : "+m" ((xl)->spinners) : : "memory");
#else
typedef u16 xen_spinners_t;
# define inc_spinners(xl) \
	asm(LOCK_PREFIX " incw %0" : "+m" ((xl)->spinners) : : "memory");
# define dec_spinners(xl) \
	asm(LOCK_PREFIX " decw %0" : "+m" ((xl)->spinners) : : "memory");
#endif

struct xen_spinlock {
	unsigned char lock;		/* 0 -> free; 1 -> locked */
	xen_spinners_t spinners;	/* count of waiting cpus */
};

static int xen_spin_is_locked(struct arch_spinlock *lock)
{
	struct xen_spinlock *xl = (struct xen_spinlock *)lock;

	return xl->lock != 0;
}

static int xen_spin_is_contended(struct arch_spinlock *lock)
{
	struct xen_spinlock *xl = (struct xen_spinlock *)lock;

	/* Not strictly true; this is only the count of contended
	   lock-takers entering the slow path. */
	return xl->spinners != 0;
}

static int xen_spin_trylock(struct arch_spinlock *lock)
{
	struct xen_spinlock *xl = (struct xen_spinlock *)lock;
	u8 old = 1;

	asm("xchgb %b0,%1"
	    : "+q" (old), "+m" (xl->lock) : : "memory");

	return old == 0;
}

static DEFINE_PER_CPU(int, lock_kicker_irq) = -1;
static DEFINE_PER_CPU(struct xen_spinlock *, lock_spinners);

/*
 * Mark a cpu as interested in a lock.  Returns the CPU's previous
 * lock of interest, in case we got preempted by an interrupt.
 */
static inline struct xen_spinlock *spinning_lock(struct xen_spinlock *xl)
{
	struct xen_spinlock *prev;

	prev = __this_cpu_read(lock_spinners);
	__this_cpu_write(lock_spinners, xl);

	wmb();			/* set lock of interest before count */

	inc_spinners(xl);

	return prev;
}

/*
 * Mark a cpu as no longer interested in a lock.  Restores previous
 * lock of interest (NULL for none).
 */
static inline void unspinning_lock(struct xen_spinlock *xl, struct xen_spinlock *prev)
{
	dec_spinners(xl);
	wmb();			/* decrement count before restoring lock */
	__this_cpu_write(lock_spinners, prev);
}

static noinline int xen_spin_lock_slow(struct arch_spinlock *lock, bool irq_enable)
{
	struct xen_spinlock *xl = (struct xen_spinlock *)lock;
	struct xen_spinlock *prev;
	int irq = __this_cpu_read(lock_kicker_irq);
	int ret;
	u64 start;

	/* If kicker interrupts not initialized yet, just spin */
	if (irq == -1)
		return 0;

	start = spin_time_start();

	/* announce we're spinning */
	prev = spinning_lock(xl);

	ADD_STATS(taken_slow, 1);
	ADD_STATS(taken_slow_nested, prev != NULL);

	do {
		unsigned long flags;

		/* clear pending */
		xen_clear_irq_pending(irq);

		/* check again make sure it didn't become free while
		   we weren't looking  */
		ret = xen_spin_trylock(lock);
		if (ret) {
			ADD_STATS(taken_slow_pickup, 1);

			/*
			 * If we interrupted another spinlock while it
			 * was blocking, make sure it doesn't block
			 * without rechecking the lock.
			 */
			if (prev != NULL)
				xen_set_irq_pending(irq);
			goto out;
		}

		flags = arch_local_save_flags();
		if (irq_enable) {
			ADD_STATS(taken_slow_irqenable, 1);
			raw_local_irq_enable();
		}

		/*
		 * Block until irq becomes pending.  If we're
		 * interrupted at this point (after the trylock but
		 * before entering the block), then the nested lock
		 * handler guarantees that the irq will be left
		 * pending if there's any chance the lock became free;
		 * xen_poll_irq() returns immediately if the irq is
		 * pending.
		 */
		xen_poll_irq(irq);

		raw_local_irq_restore(flags);

		ADD_STATS(taken_slow_spurious, !xen_test_irq_pending(irq));
	} while (!xen_test_irq_pending(irq)); /* check for spurious wakeups */

	kstat_incr_irqs_this_cpu(irq, irq_to_desc(irq));

out:
	unspinning_lock(xl, prev);
	spin_time_accum_blocked(start);

	return ret;
}

static inline void __xen_spin_lock(struct arch_spinlock *lock, bool irq_enable)
{
	struct xen_spinlock *xl = (struct xen_spinlock *)lock;
	unsigned timeout;
	u8 oldval;
	u64 start_spin;

	ADD_STATS(taken, 1);

	start_spin = spin_time_start();

	do {
		u64 start_spin_fast = spin_time_start();

		timeout = TIMEOUT;

		asm("1: xchgb %1,%0\n"
		    "   testb %1,%1\n"
		    "   jz 3f\n"
		    "2: rep;nop\n"
		    "   cmpb $0,%0\n"
		    "   je 1b\n"
		    "   dec %2\n"
		    "   jnz 2b\n"
		    "3:\n"
		    : "+m" (xl->lock), "=q" (oldval), "+r" (timeout)
		    : "1" (1)
		    : "memory");

		spin_time_accum_spinning(start_spin_fast);

	} while (unlikely(oldval != 0 &&
			  (TIMEOUT == ~0 || !xen_spin_lock_slow(lock, irq_enable))));

	spin_time_accum_total(start_spin);
}

static void xen_spin_lock(struct arch_spinlock *lock)
{
	__xen_spin_lock(lock, false);
}

static void xen_spin_lock_flags(struct arch_spinlock *lock, unsigned long flags)
{
	__xen_spin_lock(lock, !raw_irqs_disabled_flags(flags));
}

static noinline void xen_spin_unlock_slow(struct xen_spinlock *xl)
{
	int cpu;

	ADD_STATS(released_slow, 1);

	for_each_online_cpu(cpu) {
		/* XXX should mix up next cpu selection */
		if (per_cpu(lock_spinners, cpu) == xl) {
			ADD_STATS(released_slow_kicked, 1);
			xen_send_IPI_one(cpu, XEN_SPIN_UNLOCK_VECTOR);
=======
=======
>>>>>>> v3.18
struct xen_lock_waiting {
	struct arch_spinlock *lock;
	__ticket_t want;
};

static DEFINE_PER_CPU(int, lock_kicker_irq) = -1;
static DEFINE_PER_CPU(char *, irq_name);
static DEFINE_PER_CPU(struct xen_lock_waiting, lock_waiting);
static cpumask_t waiting_cpus;

static bool xen_pvspin = true;
__visible void xen_lock_spinning(struct arch_spinlock *lock, __ticket_t want)
{
	int irq = __this_cpu_read(lock_kicker_irq);
	struct xen_lock_waiting *w = this_cpu_ptr(&lock_waiting);
	int cpu = smp_processor_id();
	u64 start;
	unsigned long flags;

	/* If kicker interrupts not initialized yet, just spin */
	if (irq == -1)
		return;

	start = spin_time_start();

	/*
	 * Make sure an interrupt handler can't upset things in a
	 * partially setup state.
	 */
	local_irq_save(flags);
	/*
	 * We don't really care if we're overwriting some other
	 * (lock,want) pair, as that would mean that we're currently
	 * in an interrupt context, and the outer context had
	 * interrupts enabled.  That has already kicked the VCPU out
	 * of xen_poll_irq(), so it will just return spuriously and
	 * retry with newly setup (lock,want).
	 *
	 * The ordering protocol on this is that the "lock" pointer
	 * may only be set non-NULL if the "want" ticket is correct.
	 * If we're updating "want", we must first clear "lock".
	 */
	w->lock = NULL;
	smp_wmb();
	w->want = want;
	smp_wmb();
	w->lock = lock;

	/* This uses set_bit, which atomic and therefore a barrier */
	cpumask_set_cpu(cpu, &waiting_cpus);
	add_stats(TAKEN_SLOW, 1);

	/* clear pending */
	xen_clear_irq_pending(irq);

	/* Only check lock once pending cleared */
	barrier();

	/*
	 * Mark entry to slowpath before doing the pickup test to make
	 * sure we don't deadlock with an unlocker.
	 */
	__ticket_enter_slowpath(lock);

	/*
	 * check again make sure it didn't become free while
	 * we weren't looking
	 */
	if (ACCESS_ONCE(lock->tickets.head) == want) {
		add_stats(TAKEN_SLOW_PICKUP, 1);
		goto out;
	}

	/* Allow interrupts while blocked */
	local_irq_restore(flags);

	/*
	 * If an interrupt happens here, it will leave the wakeup irq
	 * pending, which will cause xen_poll_irq() to return
	 * immediately.
	 */

	/* Block until irq becomes pending (or perhaps a spurious wakeup) */
	xen_poll_irq(irq);
	add_stats(TAKEN_SLOW_SPURIOUS, !xen_test_irq_pending(irq));

	local_irq_save(flags);

	kstat_incr_irq_this_cpu(irq);
out:
	cpumask_clear_cpu(cpu, &waiting_cpus);
	w->lock = NULL;

	local_irq_restore(flags);

	spin_time_accum_blocked(start);
}
PV_CALLEE_SAVE_REGS_THUNK(xen_lock_spinning);

static void xen_unlock_kick(struct arch_spinlock *lock, __ticket_t next)
{
	int cpu;

	add_stats(RELEASED_SLOW, 1);

	for_each_cpu(cpu, &waiting_cpus) {
		const struct xen_lock_waiting *w = &per_cpu(lock_waiting, cpu);

		/* Make sure we read lock before want */
		if (ACCESS_ONCE(w->lock) == lock &&
		    ACCESS_ONCE(w->want) == next) {
			add_stats(RELEASED_SLOW_KICKED, 1);
			xen_send_IPI_one(cpu, XEN_SPIN_UNLOCK_VECTOR);
			break;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		}
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static void xen_spin_unlock(struct arch_spinlock *lock)
{
	struct xen_spinlock *xl = (struct xen_spinlock *)lock;

	ADD_STATS(released, 1);

	smp_wmb();		/* make sure no writes get moved after unlock */
	xl->lock = 0;		/* release lock */

	/*
	 * Make sure unlock happens before checking for waiting
	 * spinners.  We need a strong barrier to enforce the
	 * write-read ordering to different memory locations, as the
	 * CPU makes no implied guarantees about their ordering.
	 */
	mb();

	if (unlikely(xl->spinners))
		xen_spin_unlock_slow(xl);
}

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
static irqreturn_t dummy_handler(int irq, void *dev_id)
{
	BUG();
	return IRQ_HANDLED;
}

<<<<<<< HEAD
<<<<<<< HEAD
void __cpuinit xen_init_lock_cpu(int cpu)
{
	int irq;
	const char *name;
=======
=======
>>>>>>> v3.18
void xen_init_lock_cpu(int cpu)
{
	int irq;
	char *name;

	if (!xen_pvspin)
		return;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	WARN(per_cpu(lock_kicker_irq, cpu) >= 0, "spinlock on CPU%d exists on IRQ%d!\n",
	     cpu, per_cpu(lock_kicker_irq, cpu));

<<<<<<< HEAD
<<<<<<< HEAD
	/*
	 * See git commit f10cd522c5fbfec9ae3cc01967868c9c2401ed23
	 * (xen: disable PV spinlocks on HVM)
	 */
	if (xen_hvm_domain())
		return;

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	name = kasprintf(GFP_KERNEL, "spinlock%d", cpu);
	irq = bind_ipi_to_irqhandler(XEN_SPIN_UNLOCK_VECTOR,
				     cpu,
				     dummy_handler,
<<<<<<< HEAD
<<<<<<< HEAD
				     IRQF_DISABLED|IRQF_PERCPU|IRQF_NOBALANCING,
=======
				     IRQF_PERCPU|IRQF_NOBALANCING,
>>>>>>> v3.18
=======
				     IRQF_PERCPU|IRQF_NOBALANCING,
>>>>>>> v3.18
				     name,
				     NULL);

	if (irq >= 0) {
		disable_irq(irq); /* make sure it's never delivered */
		per_cpu(lock_kicker_irq, cpu) = irq;
<<<<<<< HEAD
<<<<<<< HEAD
=======
		per_cpu(irq_name, cpu) = name;
>>>>>>> v3.18
=======
		per_cpu(irq_name, cpu) = name;
>>>>>>> v3.18
	}

	printk("cpu %d spinlock event irq %d\n", cpu, irq);
}

void xen_uninit_lock_cpu(int cpu)
{
<<<<<<< HEAD
<<<<<<< HEAD
	/*
	 * See git commit f10cd522c5fbfec9ae3cc01967868c9c2401ed23
	 * (xen: disable PV spinlocks on HVM)
	 */
	if (xen_hvm_domain())
=======
	if (!xen_pvspin)
>>>>>>> v3.18
=======
	if (!xen_pvspin)
>>>>>>> v3.18
		return;

	unbind_from_irqhandler(per_cpu(lock_kicker_irq, cpu), NULL);
	per_cpu(lock_kicker_irq, cpu) = -1;
<<<<<<< HEAD
<<<<<<< HEAD
}

void __init xen_init_spinlocks(void)
{
	/*
	 * See git commit f10cd522c5fbfec9ae3cc01967868c9c2401ed23
	 * (xen: disable PV spinlocks on HVM)
	 */
	if (xen_hvm_domain())
		return;

	BUILD_BUG_ON(sizeof(struct xen_spinlock) > sizeof(arch_spinlock_t));

	pv_lock_ops.spin_is_locked = xen_spin_is_locked;
	pv_lock_ops.spin_is_contended = xen_spin_is_contended;
	pv_lock_ops.spin_lock = xen_spin_lock;
	pv_lock_ops.spin_lock_flags = xen_spin_lock_flags;
	pv_lock_ops.spin_trylock = xen_spin_trylock;
	pv_lock_ops.spin_unlock = xen_spin_unlock;
}
=======
=======
>>>>>>> v3.18
	kfree(per_cpu(irq_name, cpu));
	per_cpu(irq_name, cpu) = NULL;
}


/*
 * Our init of PV spinlocks is split in two init functions due to us
 * using paravirt patching and jump labels patching and having to do
 * all of this before SMP code is invoked.
 *
 * The paravirt patching needs to be done _before_ the alternative asm code
 * is started, otherwise we would not patch the core kernel code.
 */
void __init xen_init_spinlocks(void)
{

	if (!xen_pvspin) {
		printk(KERN_DEBUG "xen: PV spinlocks disabled\n");
		return;
	}
	printk(KERN_DEBUG "xen: PV spinlocks enabled\n");
	pv_lock_ops.lock_spinning = PV_CALLEE_SAVE(xen_lock_spinning);
	pv_lock_ops.unlock_kick = xen_unlock_kick;
}

/*
 * While the jump_label init code needs to happend _after_ the jump labels are
 * enabled and before SMP is started. Hence we use pre-SMP initcall level
 * init. We cannot do it in xen_init_spinlocks as that is done before
 * jump labels are activated.
 */
static __init int xen_init_spinlocks_jump(void)
{
	if (!xen_pvspin)
		return 0;

	if (!xen_domain())
		return 0;

	static_key_slow_inc(&paravirt_ticketlocks_enabled);
	return 0;
}
early_initcall(xen_init_spinlocks_jump);

static __init int xen_parse_nopvspin(char *arg)
{
	xen_pvspin = false;
	return 0;
}
early_param("xen_nopvspin", xen_parse_nopvspin);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

#ifdef CONFIG_XEN_DEBUG_FS

static struct dentry *d_spin_debug;

static int __init xen_spinlock_debugfs(void)
{
	struct dentry *d_xen = xen_init_debugfs();

	if (d_xen == NULL)
		return -ENOMEM;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (!xen_pvspin)
		return 0;

>>>>>>> v3.18
=======
	if (!xen_pvspin)
		return 0;

>>>>>>> v3.18
	d_spin_debug = debugfs_create_dir("spinlocks", d_xen);

	debugfs_create_u8("zero_stats", 0644, d_spin_debug, &zero_stats);

<<<<<<< HEAD
<<<<<<< HEAD
	debugfs_create_u32("timeout", 0644, d_spin_debug, &lock_timeout);

	debugfs_create_u64("taken", 0444, d_spin_debug, &spinlock_stats.taken);
	debugfs_create_u32("taken_slow", 0444, d_spin_debug,
			   &spinlock_stats.taken_slow);
	debugfs_create_u32("taken_slow_nested", 0444, d_spin_debug,
			   &spinlock_stats.taken_slow_nested);
	debugfs_create_u32("taken_slow_pickup", 0444, d_spin_debug,
			   &spinlock_stats.taken_slow_pickup);
	debugfs_create_u32("taken_slow_spurious", 0444, d_spin_debug,
			   &spinlock_stats.taken_slow_spurious);
	debugfs_create_u32("taken_slow_irqenable", 0444, d_spin_debug,
			   &spinlock_stats.taken_slow_irqenable);

	debugfs_create_u64("released", 0444, d_spin_debug, &spinlock_stats.released);
	debugfs_create_u32("released_slow", 0444, d_spin_debug,
			   &spinlock_stats.released_slow);
	debugfs_create_u32("released_slow_kicked", 0444, d_spin_debug,
			   &spinlock_stats.released_slow_kicked);

	debugfs_create_u64("time_spinning", 0444, d_spin_debug,
			   &spinlock_stats.time_spinning);
	debugfs_create_u64("time_blocked", 0444, d_spin_debug,
			   &spinlock_stats.time_blocked);
	debugfs_create_u64("time_total", 0444, d_spin_debug,
			   &spinlock_stats.time_total);

	debugfs_create_u32_array("histo_total", 0444, d_spin_debug,
				spinlock_stats.histo_spin_total, HISTO_BUCKETS + 1);
	debugfs_create_u32_array("histo_spinning", 0444, d_spin_debug,
				spinlock_stats.histo_spin_spinning, HISTO_BUCKETS + 1);
=======
=======
>>>>>>> v3.18
	debugfs_create_u32("taken_slow", 0444, d_spin_debug,
			   &spinlock_stats.contention_stats[TAKEN_SLOW]);
	debugfs_create_u32("taken_slow_pickup", 0444, d_spin_debug,
			   &spinlock_stats.contention_stats[TAKEN_SLOW_PICKUP]);
	debugfs_create_u32("taken_slow_spurious", 0444, d_spin_debug,
			   &spinlock_stats.contention_stats[TAKEN_SLOW_SPURIOUS]);

	debugfs_create_u32("released_slow", 0444, d_spin_debug,
			   &spinlock_stats.contention_stats[RELEASED_SLOW]);
	debugfs_create_u32("released_slow_kicked", 0444, d_spin_debug,
			   &spinlock_stats.contention_stats[RELEASED_SLOW_KICKED]);

	debugfs_create_u64("time_blocked", 0444, d_spin_debug,
			   &spinlock_stats.time_blocked);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	debugfs_create_u32_array("histo_blocked", 0444, d_spin_debug,
				spinlock_stats.histo_spin_blocked, HISTO_BUCKETS + 1);

	return 0;
}
fs_initcall(xen_spinlock_debugfs);

#endif	/* CONFIG_XEN_DEBUG_FS */
