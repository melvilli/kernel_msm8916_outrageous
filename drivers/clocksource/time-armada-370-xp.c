/*
 * Marvell Armada 370/XP SoC timer handling.
 *
 * Copyright (C) 2012 Marvell
 *
 * Lior Amsalem <alior@marvell.com>
 * Gregory CLEMENT <gregory.clement@free-electrons.com>
 * Thomas Petazzoni <thomas.petazzoni@free-electrons.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 *
 * Timer 0 is used as free-running clocksource, while timer 1 is
 * used as clock_event_device.
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
 *
 * ---
 * Clocksource driver for Armada 370 and Armada XP SoC.
 * This driver implements one compatible string for each SoC, given
 * each has its own characteristics:
 *
 *   * Armada 370 has no 25 MHz fixed timer.
 *
 *   * Armada XP cannot work properly without such 25 MHz fixed timer as
 *     doing otherwise leads to using a clocksource whose frequency varies
 *     when doing cpufreq frequency changes.
 *
 * See Documentation/devicetree/bindings/timer/marvell,armada-370-xp-timer.txt
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 */

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/clk.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/cpu.h>
>>>>>>> v3.18
=======
#include <linux/cpu.h>
>>>>>>> v3.18
#include <linux/timer.h>
#include <linux/clockchips.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/irq.h>
#include <linux/module.h>
#include <linux/sched_clock.h>
<<<<<<< HEAD
<<<<<<< HEAD

#include <asm/localtimer.h>
#include <linux/percpu.h>
=======
#include <linux/percpu.h>

>>>>>>> v3.18
=======
#include <linux/percpu.h>

>>>>>>> v3.18
/*
 * Timer block registers.
 */
#define TIMER_CTRL_OFF		0x0000
<<<<<<< HEAD
<<<<<<< HEAD
#define  TIMER0_EN		 0x0001
#define  TIMER0_RELOAD_EN	 0x0002
#define  TIMER0_25MHZ            0x0800
#define  TIMER0_DIV(div)         ((div) << 19)
#define  TIMER1_EN		 0x0004
#define  TIMER1_RELOAD_EN	 0x0008
#define  TIMER1_25MHZ            0x1000
=======
=======
>>>>>>> v3.18
#define  TIMER0_EN		 BIT(0)
#define  TIMER0_RELOAD_EN	 BIT(1)
#define  TIMER0_25MHZ            BIT(11)
#define  TIMER0_DIV(div)         ((div) << 19)
#define  TIMER1_EN		 BIT(2)
#define  TIMER1_RELOAD_EN	 BIT(3)
#define  TIMER1_25MHZ            BIT(12)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#define  TIMER1_DIV(div)         ((div) << 22)
#define TIMER_EVENTS_STATUS	0x0004
#define  TIMER0_CLR_MASK         (~0x1)
#define  TIMER1_CLR_MASK         (~0x100)
#define TIMER0_RELOAD_OFF	0x0010
#define TIMER0_VAL_OFF		0x0014
#define TIMER1_RELOAD_OFF	0x0018
#define TIMER1_VAL_OFF		0x001c

#define LCL_TIMER_EVENTS_STATUS	0x0028
/* Global timers are connected to the coherency fabric clock, and the
   below divider reduces their incrementing frequency. */
#define TIMER_DIVIDER_SHIFT     5
#define TIMER_DIVIDER           (1 << TIMER_DIVIDER_SHIFT)

/*
 * SoC-specific data.
 */
static void __iomem *timer_base, *local_base;
static unsigned int timer_clk;
static bool timer25Mhz = true;
<<<<<<< HEAD
<<<<<<< HEAD
=======
static u32 enable_mask;
>>>>>>> v3.18
=======
static u32 enable_mask;
>>>>>>> v3.18

/*
 * Number of timer ticks per jiffy.
 */
static u32 ticks_per_jiffy;

<<<<<<< HEAD
<<<<<<< HEAD
static struct clock_event_device __percpu **percpu_armada_370_xp_evt;

static u32 notrace armada_370_xp_read_sched_clock(void)
=======
=======
>>>>>>> v3.18
static struct clock_event_device __percpu *armada_370_xp_evt;

static void local_timer_ctrl_clrset(u32 clr, u32 set)
{
	writel((readl(local_base + TIMER_CTRL_OFF) & ~clr) | set,
		local_base + TIMER_CTRL_OFF);
}

static u64 notrace armada_370_xp_read_sched_clock(void)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	return ~readl(timer_base + TIMER0_VAL_OFF);
}

/*
 * Clockevent handling.
 */
static int
armada_370_xp_clkevt_next_event(unsigned long delta,
				struct clock_event_device *dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	u32 u;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	/*
	 * Clear clockevent timer interrupt.
	 */
	writel(TIMER0_CLR_MASK, local_base + LCL_TIMER_EVENTS_STATUS);

	/*
	 * Setup new clockevent timer value.
	 */
	writel(delta, local_base + TIMER0_VAL_OFF);

	/*
	 * Enable the timer.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	u = readl(local_base + TIMER_CTRL_OFF);
	u = ((u & ~TIMER0_RELOAD_EN) | TIMER0_EN |
	     TIMER0_DIV(TIMER_DIVIDER_SHIFT));
	writel(u, local_base + TIMER_CTRL_OFF);

=======
	local_timer_ctrl_clrset(TIMER0_RELOAD_EN, enable_mask);
>>>>>>> v3.18
=======
	local_timer_ctrl_clrset(TIMER0_RELOAD_EN, enable_mask);
>>>>>>> v3.18
	return 0;
}

static void
armada_370_xp_clkevt_mode(enum clock_event_mode mode,
			  struct clock_event_device *dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	u32 u;

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (mode == CLOCK_EVT_MODE_PERIODIC) {

		/*
		 * Setup timer to fire at 1/HZ intervals.
		 */
		writel(ticks_per_jiffy - 1, local_base + TIMER0_RELOAD_OFF);
		writel(ticks_per_jiffy - 1, local_base + TIMER0_VAL_OFF);

		/*
		 * Enable timer.
		 */
<<<<<<< HEAD
<<<<<<< HEAD

		u = readl(local_base + TIMER_CTRL_OFF);

		writel((u | TIMER0_EN | TIMER0_RELOAD_EN |
			TIMER0_DIV(TIMER_DIVIDER_SHIFT)),
			local_base + TIMER_CTRL_OFF);
=======
		local_timer_ctrl_clrset(0, TIMER0_RELOAD_EN | enable_mask);
>>>>>>> v3.18
=======
		local_timer_ctrl_clrset(0, TIMER0_RELOAD_EN | enable_mask);
>>>>>>> v3.18
	} else {
		/*
		 * Disable timer.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		u = readl(local_base + TIMER_CTRL_OFF);
		writel(u & ~TIMER0_EN, local_base + TIMER_CTRL_OFF);
=======
		local_timer_ctrl_clrset(TIMER0_EN, 0);
>>>>>>> v3.18
=======
		local_timer_ctrl_clrset(TIMER0_EN, 0);
>>>>>>> v3.18

		/*
		 * ACK pending timer interrupt.
		 */
		writel(TIMER0_CLR_MASK, local_base + LCL_TIMER_EVENTS_STATUS);
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct clock_event_device armada_370_xp_clkevt = {
	.name		= "armada_370_xp_per_cpu_tick",
	.features	= CLOCK_EVT_FEAT_ONESHOT | CLOCK_EVT_FEAT_PERIODIC,
	.shift		= 32,
	.rating		= 300,
	.set_next_event	= armada_370_xp_clkevt_next_event,
	.set_mode	= armada_370_xp_clkevt_mode,
};
=======
static int armada_370_xp_clkevt_irq;
>>>>>>> v3.18
=======
static int armada_370_xp_clkevt_irq;
>>>>>>> v3.18

static irqreturn_t armada_370_xp_timer_interrupt(int irq, void *dev_id)
{
	/*
	 * ACK timer interrupt and call event handler.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	struct clock_event_device *evt = *(struct clock_event_device **)dev_id;
=======
	struct clock_event_device *evt = dev_id;
>>>>>>> v3.18
=======
	struct clock_event_device *evt = dev_id;
>>>>>>> v3.18

	writel(TIMER0_CLR_MASK, local_base + LCL_TIMER_EVENTS_STATUS);
	evt->event_handler(evt);

	return IRQ_HANDLED;
}

/*
 * Setup the local clock events for a CPU.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int __cpuinit armada_370_xp_timer_setup(struct clock_event_device *evt)
{
	u32 u;
	int cpu = smp_processor_id();

	/* Use existing clock_event for cpu 0 */
	if (!smp_processor_id())
		return 0;

	u = readl(local_base + TIMER_CTRL_OFF);
	if (timer25Mhz)
		writel(u | TIMER0_25MHZ, local_base + TIMER_CTRL_OFF);
	else
		writel(u & ~TIMER0_25MHZ, local_base + TIMER_CTRL_OFF);

	evt->name		= armada_370_xp_clkevt.name;
	evt->irq		= armada_370_xp_clkevt.irq;
	evt->features		= armada_370_xp_clkevt.features;
	evt->shift		= armada_370_xp_clkevt.shift;
	evt->rating		= armada_370_xp_clkevt.rating,
	evt->set_next_event	= armada_370_xp_clkevt_next_event,
	evt->set_mode		= armada_370_xp_clkevt_mode,
	evt->cpumask		= cpumask_of(cpu);

	*__this_cpu_ptr(percpu_armada_370_xp_evt) = evt;

=======
=======
>>>>>>> v3.18
static int armada_370_xp_timer_setup(struct clock_event_device *evt)
{
	u32 clr = 0, set = 0;
	int cpu = smp_processor_id();

	if (timer25Mhz)
		set = TIMER0_25MHZ;
	else
		clr = TIMER0_25MHZ;
	local_timer_ctrl_clrset(clr, set);

	evt->name		= "armada_370_xp_per_cpu_tick",
	evt->features		= CLOCK_EVT_FEAT_ONESHOT |
				  CLOCK_EVT_FEAT_PERIODIC;
	evt->shift		= 32,
	evt->rating		= 300,
	evt->set_next_event	= armada_370_xp_clkevt_next_event,
	evt->set_mode		= armada_370_xp_clkevt_mode,
	evt->irq		= armada_370_xp_clkevt_irq;
	evt->cpumask		= cpumask_of(cpu);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	clockevents_config_and_register(evt, timer_clk, 1, 0xfffffffe);
	enable_percpu_irq(evt->irq, 0);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void  armada_370_xp_timer_stop(struct clock_event_device *evt)
=======
static void armada_370_xp_timer_stop(struct clock_event_device *evt)
>>>>>>> v3.18
=======
static void armada_370_xp_timer_stop(struct clock_event_device *evt)
>>>>>>> v3.18
{
	evt->set_mode(CLOCK_EVT_MODE_UNUSED, evt);
	disable_percpu_irq(evt->irq);
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct local_timer_ops armada_370_xp_local_timer_ops __cpuinitdata = {
	.setup	= armada_370_xp_timer_setup,
	.stop	=  armada_370_xp_timer_stop,
};

void __init armada_370_xp_timer_init(void)
{
	u32 u;
	struct device_node *np;
	int res;

	np = of_find_compatible_node(NULL, NULL, "marvell,armada-370-xp-timer");
=======
=======
>>>>>>> v3.18
static int armada_370_xp_timer_cpu_notify(struct notifier_block *self,
					   unsigned long action, void *hcpu)
{
	/*
	 * Grab cpu pointer in each case to avoid spurious
	 * preemptible warnings
	 */
	switch (action & ~CPU_TASKS_FROZEN) {
	case CPU_STARTING:
		armada_370_xp_timer_setup(this_cpu_ptr(armada_370_xp_evt));
		break;
	case CPU_DYING:
		armada_370_xp_timer_stop(this_cpu_ptr(armada_370_xp_evt));
		break;
	}

	return NOTIFY_OK;
}

static struct notifier_block armada_370_xp_timer_cpu_nb = {
	.notifier_call = armada_370_xp_timer_cpu_notify,
};

static void __init armada_370_xp_timer_common_init(struct device_node *np)
{
	u32 clr = 0, set = 0;
	int res;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	timer_base = of_iomap(np, 0);
	WARN_ON(!timer_base);
	local_base = of_iomap(np, 1);

<<<<<<< HEAD
<<<<<<< HEAD
	if (of_find_property(np, "marvell,timer-25Mhz", NULL)) {
		/* The fixed 25MHz timer is available so let's use it */
		u = readl(local_base + TIMER_CTRL_OFF);
		writel(u | TIMER0_25MHZ,
		       local_base + TIMER_CTRL_OFF);
		u = readl(timer_base + TIMER_CTRL_OFF);
		writel(u | TIMER0_25MHZ,
		       timer_base + TIMER_CTRL_OFF);
		timer_clk = 25000000;
	} else {
		unsigned long rate = 0;
		struct clk *clk = of_clk_get(np, 0);
		WARN_ON(IS_ERR(clk));
		rate =  clk_get_rate(clk);
		u = readl(local_base + TIMER_CTRL_OFF);
		writel(u & ~(TIMER0_25MHZ),
		       local_base + TIMER_CTRL_OFF);

		u = readl(timer_base + TIMER_CTRL_OFF);
		writel(u & ~(TIMER0_25MHZ),
		       timer_base + TIMER_CTRL_OFF);

		timer_clk = rate / TIMER_DIVIDER;
		timer25Mhz = false;
	}
=======
=======
>>>>>>> v3.18
	if (timer25Mhz) {
		set = TIMER0_25MHZ;		
		enable_mask = TIMER0_EN;
	} else {
		clr = TIMER0_25MHZ;
		enable_mask = TIMER0_EN | TIMER0_DIV(TIMER_DIVIDER_SHIFT);
	}
	atomic_io_modify(timer_base + TIMER_CTRL_OFF, clr | set, set);
	local_timer_ctrl_clrset(clr, set);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/*
	 * We use timer 0 as clocksource, and private(local) timer 0
	 * for clockevents
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	armada_370_xp_clkevt.irq = irq_of_parse_and_map(np, 4);
=======
	armada_370_xp_clkevt_irq = irq_of_parse_and_map(np, 4);
>>>>>>> v3.18
=======
	armada_370_xp_clkevt_irq = irq_of_parse_and_map(np, 4);
>>>>>>> v3.18

	ticks_per_jiffy = (timer_clk + HZ / 2) / HZ;

	/*
<<<<<<< HEAD
<<<<<<< HEAD
	 * Set scale and timer for sched_clock.
	 */
	setup_sched_clock(armada_370_xp_read_sched_clock, 32, timer_clk);

	/*
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	 * Setup free-running clocksource timer (interrupts
	 * disabled).
	 */
	writel(0xffffffff, timer_base + TIMER0_VAL_OFF);
	writel(0xffffffff, timer_base + TIMER0_RELOAD_OFF);

<<<<<<< HEAD
<<<<<<< HEAD
	u = readl(timer_base + TIMER_CTRL_OFF);

	writel((u | TIMER0_EN | TIMER0_RELOAD_EN |
		TIMER0_DIV(TIMER_DIVIDER_SHIFT)), timer_base + TIMER_CTRL_OFF);
=======
=======
>>>>>>> v3.18
	atomic_io_modify(timer_base + TIMER_CTRL_OFF,
		TIMER0_RELOAD_EN | enable_mask,
		TIMER0_RELOAD_EN | enable_mask);

	/*
	 * Set scale and timer for sched_clock.
	 */
	sched_clock_register(armada_370_xp_read_sched_clock, 32, timer_clk);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	clocksource_mmio_init(timer_base + TIMER0_VAL_OFF,
			      "armada_370_xp_clocksource",
			      timer_clk, 300, 32, clocksource_mmio_readl_down);

<<<<<<< HEAD
<<<<<<< HEAD
	/* Register the clockevent on the private timer of CPU 0 */
	armada_370_xp_clkevt.cpumask = cpumask_of(0);
	clockevents_config_and_register(&armada_370_xp_clkevt,
					timer_clk, 1, 0xfffffffe);

	percpu_armada_370_xp_evt = alloc_percpu(struct clock_event_device *);
=======
	register_cpu_notifier(&armada_370_xp_timer_cpu_nb);

	armada_370_xp_evt = alloc_percpu(struct clock_event_device);
>>>>>>> v3.18
=======
	register_cpu_notifier(&armada_370_xp_timer_cpu_nb);

	armada_370_xp_evt = alloc_percpu(struct clock_event_device);
>>>>>>> v3.18


	/*
	 * Setup clockevent timer (interrupt-driven).
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	*__this_cpu_ptr(percpu_armada_370_xp_evt) = &armada_370_xp_clkevt;
	res = request_percpu_irq(armada_370_xp_clkevt.irq,
				armada_370_xp_timer_interrupt,
				armada_370_xp_clkevt.name,
				percpu_armada_370_xp_evt);
	if (!res) {
		enable_percpu_irq(armada_370_xp_clkevt.irq, 0);
#ifdef CONFIG_LOCAL_TIMERS
		local_timer_register(&armada_370_xp_local_timer_ops);
#endif
	}
}
=======
=======
>>>>>>> v3.18
	res = request_percpu_irq(armada_370_xp_clkevt_irq,
				armada_370_xp_timer_interrupt,
				"armada_370_xp_per_cpu_tick",
				armada_370_xp_evt);
	/* Immediately configure the timer on the boot CPU */
	if (!res)
		armada_370_xp_timer_setup(this_cpu_ptr(armada_370_xp_evt));
}

static void __init armada_xp_timer_init(struct device_node *np)
{
	struct clk *clk = of_clk_get_by_name(np, "fixed");

	/* The 25Mhz fixed clock is mandatory, and must always be available */
	BUG_ON(IS_ERR(clk));
	timer_clk = clk_get_rate(clk);

	armada_370_xp_timer_common_init(np);
}
CLOCKSOURCE_OF_DECLARE(armada_xp, "marvell,armada-xp-timer",
		       armada_xp_timer_init);

static void __init armada_370_timer_init(struct device_node *np)
{
	struct clk *clk = of_clk_get(np, 0);

	BUG_ON(IS_ERR(clk));
	timer_clk = clk_get_rate(clk) / TIMER_DIVIDER;
	timer25Mhz = false;

	armada_370_xp_timer_common_init(np);
}
CLOCKSOURCE_OF_DECLARE(armada_370, "marvell,armada-370-timer",
		       armada_370_timer_init);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
