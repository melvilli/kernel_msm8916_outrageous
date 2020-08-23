/*
 * arch/xtensa/kernel/time.c
 *
 * Timer and clock support.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2005 Tensilica Inc.
 *
 * Chris Zankel <chris@zankel.net>
 */

#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/clocksource.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/clockchips.h>
>>>>>>> v3.18
=======
#include <linux/clockchips.h>
>>>>>>> v3.18
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/profile.h>
#include <linux/delay.h>
#include <linux/irqdomain.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/sched_clock.h>
>>>>>>> v3.18
=======
#include <linux/sched_clock.h>
>>>>>>> v3.18

#include <asm/timex.h>
#include <asm/platform.h>

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_XTENSA_CALIBRATE_CCOUNT
unsigned long ccount_per_jiffy;		/* per 1/HZ */
unsigned long nsec_per_ccount;		/* nsec per ccount increment */
#endif
=======
unsigned long ccount_freq;		/* ccount Hz */
EXPORT_SYMBOL(ccount_freq);
>>>>>>> v3.18
=======
unsigned long ccount_freq;		/* ccount Hz */
EXPORT_SYMBOL(ccount_freq);
>>>>>>> v3.18

static cycle_t ccount_read(struct clocksource *cs)
{
	return (cycle_t)get_ccount();
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
static u64 notrace ccount_sched_clock_read(void)
{
	return get_ccount();
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static struct clocksource ccount_clocksource = {
	.name = "ccount",
	.rating = 200,
	.read = ccount_read,
	.mask = CLOCKSOURCE_MASK(32),
<<<<<<< HEAD
<<<<<<< HEAD
};
=======
=======
>>>>>>> v3.18
	.flags = CLOCK_SOURCE_IS_CONTINUOUS,
};

static int ccount_timer_set_next_event(unsigned long delta,
		struct clock_event_device *dev);
static void ccount_timer_set_mode(enum clock_event_mode mode,
		struct clock_event_device *evt);
struct ccount_timer {
	struct clock_event_device evt;
	int irq_enabled;
	char name[24];
};
static DEFINE_PER_CPU(struct ccount_timer, ccount_timer);

static int ccount_timer_set_next_event(unsigned long delta,
		struct clock_event_device *dev)
{
	unsigned long flags, next;
	int ret = 0;

	local_irq_save(flags);
	next = get_ccount() + delta;
	set_linux_timer(next);
	if (next - get_ccount() > delta)
		ret = -ETIME;
	local_irq_restore(flags);

	return ret;
}

static void ccount_timer_set_mode(enum clock_event_mode mode,
		struct clock_event_device *evt)
{
	struct ccount_timer *timer =
		container_of(evt, struct ccount_timer, evt);

	/*
	 * There is no way to disable the timer interrupt at the device level,
	 * only at the intenable register itself. Since enable_irq/disable_irq
	 * calls are nested, we need to make sure that these calls are
	 * balanced.
	 */
	switch (mode) {
	case CLOCK_EVT_MODE_SHUTDOWN:
	case CLOCK_EVT_MODE_UNUSED:
		if (timer->irq_enabled) {
			disable_irq(evt->irq);
			timer->irq_enabled = 0;
		}
		break;
	case CLOCK_EVT_MODE_RESUME:
	case CLOCK_EVT_MODE_ONESHOT:
		if (!timer->irq_enabled) {
			enable_irq(evt->irq);
			timer->irq_enabled = 1;
		}
	default:
		break;
	}
}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

static irqreturn_t timer_interrupt(int irq, void *dev_id);
static struct irqaction timer_irqaction = {
	.handler =	timer_interrupt,
<<<<<<< HEAD
<<<<<<< HEAD
	.flags =	IRQF_DISABLED,
	.name =		"timer",
};

void __init time_init(void)
{
	unsigned int irq;
#ifdef CONFIG_XTENSA_CALIBRATE_CCOUNT
	printk("Calibrating CPU frequency ");
	platform_calibrate_ccount();
	printk("%d.%02d MHz\n", (int)ccount_per_jiffy/(1000000/HZ),
			(int)(ccount_per_jiffy/(10000/HZ))%100);
#endif
	clocksource_register_hz(&ccount_clocksource, CCOUNT_PER_JIFFY * HZ);

	/* Initialize the linux timer interrupt. */

	irq = irq_create_mapping(NULL, LINUX_TIMER_INT);
	setup_irq(irq, &timer_irqaction);
	set_linux_timer(get_ccount() + CCOUNT_PER_JIFFY);
=======
=======
>>>>>>> v3.18
	.flags =	IRQF_TIMER,
	.name =		"timer",
};

void local_timer_setup(unsigned cpu)
{
	struct ccount_timer *timer = &per_cpu(ccount_timer, cpu);
	struct clock_event_device *clockevent = &timer->evt;

	timer->irq_enabled = 1;
	clockevent->name = timer->name;
	snprintf(timer->name, sizeof(timer->name), "ccount_clockevent_%u", cpu);
	clockevent->features = CLOCK_EVT_FEAT_ONESHOT;
	clockevent->rating = 300;
	clockevent->set_next_event = ccount_timer_set_next_event;
	clockevent->set_mode = ccount_timer_set_mode;
	clockevent->cpumask = cpumask_of(cpu);
	clockevent->irq = irq_create_mapping(NULL, LINUX_TIMER_INT);
	if (WARN(!clockevent->irq, "error: can't map timer irq"))
		return;
	clockevents_config_and_register(clockevent, ccount_freq,
					0xf, 0xffffffff);
}

void __init time_init(void)
{
#ifdef CONFIG_XTENSA_CALIBRATE_CCOUNT
	printk("Calibrating CPU frequency ");
	platform_calibrate_ccount();
	printk("%d.%02d MHz\n", (int)ccount_freq/1000000,
			(int)(ccount_freq/10000)%100);
#else
	ccount_freq = CONFIG_XTENSA_CPU_CLOCK*1000000UL;
#endif
	clocksource_register_hz(&ccount_clocksource, ccount_freq);
	local_timer_setup(0);
	setup_irq(this_cpu_ptr(&ccount_timer)->evt.irq, &timer_irqaction);
	sched_clock_register(ccount_sched_clock_read, 32, ccount_freq);
	clocksource_of_init();
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/*
 * The timer interrupt is called HZ times per second.
 */

<<<<<<< HEAD
<<<<<<< HEAD
irqreturn_t timer_interrupt (int irq, void *dev_id)
{

	unsigned long next;

	next = get_linux_timer();

again:
	while ((signed long)(get_ccount() - next) > 0) {

		profile_tick(CPU_PROFILING);
#ifndef CONFIG_SMP
		update_process_times(user_mode(get_irq_regs()));
#endif

		xtime_update(1); /* Linux handler in kernel/time/timekeeping */

		/* Note that writing CCOMPARE clears the interrupt. */

		next += CCOUNT_PER_JIFFY;
		set_linux_timer(next);
	}

	/* Allow platform to do something useful (Wdog). */

	platform_heartbeat();

	/* Make sure we didn't miss any tick... */

	if ((signed long)(get_ccount() - next) > 0)
		goto again;

=======
=======
>>>>>>> v3.18
irqreturn_t timer_interrupt(int irq, void *dev_id)
{
	struct clock_event_device *evt = &this_cpu_ptr(&ccount_timer)->evt;

	set_linux_timer(get_linux_timer());
	evt->event_handler(evt);

	/* Allow platform to do something useful (Wdog). */
	platform_heartbeat();

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return IRQ_HANDLED;
}

#ifndef CONFIG_GENERIC_CALIBRATE_DELAY
<<<<<<< HEAD
<<<<<<< HEAD
void __cpuinit calibrate_delay(void)
{
	loops_per_jiffy = CCOUNT_PER_JIFFY;
=======
void calibrate_delay(void)
{
	loops_per_jiffy = ccount_freq / HZ;
>>>>>>> v3.18
=======
void calibrate_delay(void)
{
	loops_per_jiffy = ccount_freq / HZ;
>>>>>>> v3.18
	printk("Calibrating delay loop (skipped)... "
	       "%lu.%02lu BogoMIPS preset\n",
	       loops_per_jiffy/(1000000/HZ),
	       (loops_per_jiffy/(10000/HZ)) % 100);
}
#endif
