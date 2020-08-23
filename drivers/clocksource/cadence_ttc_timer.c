/*
 * This file contains driver for the Cadence Triple Timer Counter Rev 06
 *
 *  Copyright (C) 2011-2013 Xilinx
 *
 * based on arch/mips/kernel/time.c timer driver
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/clk.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/clk-provider.h>
>>>>>>> v3.18
=======
#include <linux/clk-provider.h>
>>>>>>> v3.18
#include <linux/interrupt.h>
#include <linux/clockchips.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/clk-provider.h>

/*
 * This driver configures the 2 16-bit count-up timers as follows:
=======
=======
>>>>>>> v3.18
#include <linux/sched_clock.h>

/*
 * This driver configures the 2 16/32-bit count-up timers as follows:
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 *
 * T1: Timer 1, clocksource for generic timekeeping
 * T2: Timer 2, clockevent source for hrtimers
 * T3: Timer 3, <unused>
 *
 * The input frequency to the timer module for emulation is 2.5MHz which is
 * common to all the timer channels (T1, T2, and T3). With a pre-scaler of 32,
 * the timers are clocked at 78.125KHz (12.8 us resolution).

 * The input frequency to the timer module in silicon is configurable and
 * obtained from device tree. The pre-scaler of 32 is used.
 */

/*
 * Timer Register Offset Definitions of Timer 1, Increment base address by 4
 * and use same offsets for Timer 2
 */
#define TTC_CLK_CNTRL_OFFSET		0x00 /* Clock Control Reg, RW */
#define TTC_CNT_CNTRL_OFFSET		0x0C /* Counter Control Reg, RW */
#define TTC_COUNT_VAL_OFFSET		0x18 /* Counter Value Reg, RO */
#define TTC_INTR_VAL_OFFSET		0x24 /* Interval Count Reg, RW */
#define TTC_ISR_OFFSET		0x54 /* Interrupt Status Reg, RO */
#define TTC_IER_OFFSET		0x60 /* Interrupt Enable Reg, RW */

#define TTC_CNT_CNTRL_DISABLE_MASK	0x1

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
#define TTC_CLK_CNTRL_CSRC_MASK		(1 << 5)	/* clock source */
#define TTC_CLK_CNTRL_PSV_MASK		0x1e
#define TTC_CLK_CNTRL_PSV_SHIFT		1

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
/*
 * Setup the timers to use pre-scaling, using a fixed value for now that will
 * work across most input frequency, but it may need to be more dynamic
 */
#define PRESCALE_EXPONENT	11	/* 2 ^ PRESCALE_EXPONENT = PRESCALE */
#define PRESCALE		2048	/* The exponent must match this */
#define CLK_CNTRL_PRESCALE	((PRESCALE_EXPONENT - 1) << 1)
#define CLK_CNTRL_PRESCALE_EN	1
#define CNT_CNTRL_RESET		(1 << 4)

<<<<<<< HEAD
<<<<<<< HEAD
=======
#define MAX_F_ERR 50

>>>>>>> v3.18
=======
#define MAX_F_ERR 50

>>>>>>> v3.18
/**
 * struct ttc_timer - This definition defines local timer structure
 *
 * @base_addr:	Base address of timer
<<<<<<< HEAD
<<<<<<< HEAD
=======
 * @freq:	Timer input clock frequency
>>>>>>> v3.18
=======
 * @freq:	Timer input clock frequency
>>>>>>> v3.18
 * @clk:	Associated clock source
 * @clk_rate_change_nb	Notifier block for clock rate changes
 */
struct ttc_timer {
	void __iomem *base_addr;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	unsigned long freq;
>>>>>>> v3.18
=======
	unsigned long freq;
>>>>>>> v3.18
	struct clk *clk;
	struct notifier_block clk_rate_change_nb;
};

#define to_ttc_timer(x) \
		container_of(x, struct ttc_timer, clk_rate_change_nb)

struct ttc_timer_clocksource {
<<<<<<< HEAD
<<<<<<< HEAD
=======
	u32			scale_clk_ctrl_reg_old;
	u32			scale_clk_ctrl_reg_new;
>>>>>>> v3.18
=======
	u32			scale_clk_ctrl_reg_old;
	u32			scale_clk_ctrl_reg_new;
>>>>>>> v3.18
	struct ttc_timer	ttc;
	struct clocksource	cs;
};

#define to_ttc_timer_clksrc(x) \
		container_of(x, struct ttc_timer_clocksource, cs)

struct ttc_timer_clockevent {
	struct ttc_timer		ttc;
	struct clock_event_device	ce;
};

#define to_ttc_timer_clkevent(x) \
		container_of(x, struct ttc_timer_clockevent, ce)

<<<<<<< HEAD
<<<<<<< HEAD
=======
static void __iomem *ttc_sched_clock_val_reg;

>>>>>>> v3.18
=======
static void __iomem *ttc_sched_clock_val_reg;

>>>>>>> v3.18
/**
 * ttc_set_interval - Set the timer interval value
 *
 * @timer:	Pointer to the timer instance
 * @cycles:	Timer interval ticks
 **/
static void ttc_set_interval(struct ttc_timer *timer,
					unsigned long cycles)
{
	u32 ctrl_reg;

	/* Disable the counter, set the counter value  and re-enable counter */
<<<<<<< HEAD
<<<<<<< HEAD
	ctrl_reg = __raw_readl(timer->base_addr + TTC_CNT_CNTRL_OFFSET);
	ctrl_reg |= TTC_CNT_CNTRL_DISABLE_MASK;
	__raw_writel(ctrl_reg, timer->base_addr + TTC_CNT_CNTRL_OFFSET);

	__raw_writel(cycles, timer->base_addr + TTC_INTR_VAL_OFFSET);
=======
=======
>>>>>>> v3.18
	ctrl_reg = readl_relaxed(timer->base_addr + TTC_CNT_CNTRL_OFFSET);
	ctrl_reg |= TTC_CNT_CNTRL_DISABLE_MASK;
	writel_relaxed(ctrl_reg, timer->base_addr + TTC_CNT_CNTRL_OFFSET);

	writel_relaxed(cycles, timer->base_addr + TTC_INTR_VAL_OFFSET);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/*
	 * Reset the counter (0x10) so that it starts from 0, one-shot
	 * mode makes this needed for timing to be right.
	 */
	ctrl_reg |= CNT_CNTRL_RESET;
	ctrl_reg &= ~TTC_CNT_CNTRL_DISABLE_MASK;
<<<<<<< HEAD
<<<<<<< HEAD
	__raw_writel(ctrl_reg, timer->base_addr + TTC_CNT_CNTRL_OFFSET);
=======
	writel_relaxed(ctrl_reg, timer->base_addr + TTC_CNT_CNTRL_OFFSET);
>>>>>>> v3.18
=======
	writel_relaxed(ctrl_reg, timer->base_addr + TTC_CNT_CNTRL_OFFSET);
>>>>>>> v3.18
}

/**
 * ttc_clock_event_interrupt - Clock event timer interrupt handler
 *
 * @irq:	IRQ number of the Timer
 * @dev_id:	void pointer to the ttc_timer instance
 *
 * returns: Always IRQ_HANDLED - success
 **/
static irqreturn_t ttc_clock_event_interrupt(int irq, void *dev_id)
{
	struct ttc_timer_clockevent *ttce = dev_id;
	struct ttc_timer *timer = &ttce->ttc;

	/* Acknowledge the interrupt and call event handler */
<<<<<<< HEAD
<<<<<<< HEAD
	__raw_readl(timer->base_addr + TTC_ISR_OFFSET);
=======
	readl_relaxed(timer->base_addr + TTC_ISR_OFFSET);
>>>>>>> v3.18
=======
	readl_relaxed(timer->base_addr + TTC_ISR_OFFSET);
>>>>>>> v3.18

	ttce->ce.event_handler(&ttce->ce);

	return IRQ_HANDLED;
}

/**
 * __ttc_clocksource_read - Reads the timer counter register
 *
 * returns: Current timer counter register value
 **/
static cycle_t __ttc_clocksource_read(struct clocksource *cs)
{
	struct ttc_timer *timer = &to_ttc_timer_clksrc(cs)->ttc;

<<<<<<< HEAD
<<<<<<< HEAD
	return (cycle_t)__raw_readl(timer->base_addr +
				TTC_COUNT_VAL_OFFSET);
}

=======
=======
>>>>>>> v3.18
	return (cycle_t)readl_relaxed(timer->base_addr +
				TTC_COUNT_VAL_OFFSET);
}

static u64 notrace ttc_sched_clock_read(void)
{
	return readl_relaxed(ttc_sched_clock_val_reg);
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
/**
 * ttc_set_next_event - Sets the time interval for next event
 *
 * @cycles:	Timer interval ticks
 * @evt:	Address of clock event instance
 *
 * returns: Always 0 - success
 **/
static int ttc_set_next_event(unsigned long cycles,
					struct clock_event_device *evt)
{
	struct ttc_timer_clockevent *ttce = to_ttc_timer_clkevent(evt);
	struct ttc_timer *timer = &ttce->ttc;

	ttc_set_interval(timer, cycles);
	return 0;
}

/**
 * ttc_set_mode - Sets the mode of timer
 *
 * @mode:	Mode to be set
 * @evt:	Address of clock event instance
 **/
static void ttc_set_mode(enum clock_event_mode mode,
					struct clock_event_device *evt)
{
	struct ttc_timer_clockevent *ttce = to_ttc_timer_clkevent(evt);
	struct ttc_timer *timer = &ttce->ttc;
	u32 ctrl_reg;

	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
<<<<<<< HEAD
<<<<<<< HEAD
		ttc_set_interval(timer,
				DIV_ROUND_CLOSEST(clk_get_rate(ttce->ttc.clk),
					PRESCALE * HZ));
=======
		ttc_set_interval(timer, DIV_ROUND_CLOSEST(ttce->ttc.freq,
						PRESCALE * HZ));
>>>>>>> v3.18
=======
		ttc_set_interval(timer, DIV_ROUND_CLOSEST(ttce->ttc.freq,
						PRESCALE * HZ));
>>>>>>> v3.18
		break;
	case CLOCK_EVT_MODE_ONESHOT:
	case CLOCK_EVT_MODE_UNUSED:
	case CLOCK_EVT_MODE_SHUTDOWN:
<<<<<<< HEAD
<<<<<<< HEAD
		ctrl_reg = __raw_readl(timer->base_addr +
					TTC_CNT_CNTRL_OFFSET);
		ctrl_reg |= TTC_CNT_CNTRL_DISABLE_MASK;
		__raw_writel(ctrl_reg,
				timer->base_addr + TTC_CNT_CNTRL_OFFSET);
		break;
	case CLOCK_EVT_MODE_RESUME:
		ctrl_reg = __raw_readl(timer->base_addr +
					TTC_CNT_CNTRL_OFFSET);
		ctrl_reg &= ~TTC_CNT_CNTRL_DISABLE_MASK;
		__raw_writel(ctrl_reg,
=======
=======
>>>>>>> v3.18
		ctrl_reg = readl_relaxed(timer->base_addr +
					TTC_CNT_CNTRL_OFFSET);
		ctrl_reg |= TTC_CNT_CNTRL_DISABLE_MASK;
		writel_relaxed(ctrl_reg,
				timer->base_addr + TTC_CNT_CNTRL_OFFSET);
		break;
	case CLOCK_EVT_MODE_RESUME:
		ctrl_reg = readl_relaxed(timer->base_addr +
					TTC_CNT_CNTRL_OFFSET);
		ctrl_reg &= ~TTC_CNT_CNTRL_DISABLE_MASK;
		writel_relaxed(ctrl_reg,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
				timer->base_addr + TTC_CNT_CNTRL_OFFSET);
		break;
	}
}

static int ttc_rate_change_clocksource_cb(struct notifier_block *nb,
		unsigned long event, void *data)
{
	struct clk_notifier_data *ndata = data;
	struct ttc_timer *ttc = to_ttc_timer(nb);
	struct ttc_timer_clocksource *ttccs = container_of(ttc,
			struct ttc_timer_clocksource, ttc);

	switch (event) {
<<<<<<< HEAD
<<<<<<< HEAD
	case POST_RATE_CHANGE:
		/*
		 * Do whatever is necessary to maintain a proper time base
		 *
		 * I cannot find a way to adjust the currently used clocksource
		 * to the new frequency. __clocksource_updatefreq_hz() sounds
		 * good, but does not work. Not sure what's that missing.
		 *
		 * This approach works, but triggers two clocksource switches.
		 * The first after unregister to clocksource jiffies. And
		 * another one after the register to the newly registered timer.
		 *
		 * Alternatively we could 'waste' another HW timer to ping pong
		 * between clock sources. That would also use one register and
		 * one unregister call, but only trigger one clocksource switch
		 * for the cost of another HW timer used by the OS.
		 */
		clocksource_unregister(&ttccs->cs);
		clocksource_register_hz(&ttccs->cs,
				ndata->new_rate / PRESCALE);
		/* fall through */
	case PRE_RATE_CHANGE:
	case ABORT_RATE_CHANGE:
	default:
		return NOTIFY_DONE;
	}
}

static void __init ttc_setup_clocksource(struct clk *clk, void __iomem *base)
=======
=======
>>>>>>> v3.18
	case PRE_RATE_CHANGE:
	{
		u32 psv;
		unsigned long factor, rate_low, rate_high;

		if (ndata->new_rate > ndata->old_rate) {
			factor = DIV_ROUND_CLOSEST(ndata->new_rate,
					ndata->old_rate);
			rate_low = ndata->old_rate;
			rate_high = ndata->new_rate;
		} else {
			factor = DIV_ROUND_CLOSEST(ndata->old_rate,
					ndata->new_rate);
			rate_low = ndata->new_rate;
			rate_high = ndata->old_rate;
		}

		if (!is_power_of_2(factor))
				return NOTIFY_BAD;

		if (abs(rate_high - (factor * rate_low)) > MAX_F_ERR)
			return NOTIFY_BAD;

		factor = __ilog2_u32(factor);

		/*
		 * store timer clock ctrl register so we can restore it in case
		 * of an abort.
		 */
		ttccs->scale_clk_ctrl_reg_old =
			readl_relaxed(ttccs->ttc.base_addr +
			TTC_CLK_CNTRL_OFFSET);

		psv = (ttccs->scale_clk_ctrl_reg_old &
				TTC_CLK_CNTRL_PSV_MASK) >>
				TTC_CLK_CNTRL_PSV_SHIFT;
		if (ndata->new_rate < ndata->old_rate)
			psv -= factor;
		else
			psv += factor;

		/* prescaler within legal range? */
		if (psv & ~(TTC_CLK_CNTRL_PSV_MASK >> TTC_CLK_CNTRL_PSV_SHIFT))
			return NOTIFY_BAD;

		ttccs->scale_clk_ctrl_reg_new = ttccs->scale_clk_ctrl_reg_old &
			~TTC_CLK_CNTRL_PSV_MASK;
		ttccs->scale_clk_ctrl_reg_new |= psv << TTC_CLK_CNTRL_PSV_SHIFT;


		/* scale down: adjust divider in post-change notification */
		if (ndata->new_rate < ndata->old_rate)
			return NOTIFY_DONE;

		/* scale up: adjust divider now - before frequency change */
		writel_relaxed(ttccs->scale_clk_ctrl_reg_new,
			       ttccs->ttc.base_addr + TTC_CLK_CNTRL_OFFSET);
		break;
	}
	case POST_RATE_CHANGE:
		/* scale up: pre-change notification did the adjustment */
		if (ndata->new_rate > ndata->old_rate)
			return NOTIFY_OK;

		/* scale down: adjust divider now - after frequency change */
		writel_relaxed(ttccs->scale_clk_ctrl_reg_new,
			       ttccs->ttc.base_addr + TTC_CLK_CNTRL_OFFSET);
		break;

	case ABORT_RATE_CHANGE:
		/* we have to undo the adjustment in case we scale up */
		if (ndata->new_rate < ndata->old_rate)
			return NOTIFY_OK;

		/* restore original register value */
		writel_relaxed(ttccs->scale_clk_ctrl_reg_old,
			       ttccs->ttc.base_addr + TTC_CLK_CNTRL_OFFSET);
		/* fall through */
	default:
		return NOTIFY_DONE;
	}

	return NOTIFY_DONE;
}

static void __init ttc_setup_clocksource(struct clk *clk, void __iomem *base,
					 u32 timer_width)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	struct ttc_timer_clocksource *ttccs;
	int err;

	ttccs = kzalloc(sizeof(*ttccs), GFP_KERNEL);
	if (WARN_ON(!ttccs))
		return;

	ttccs->ttc.clk = clk;

	err = clk_prepare_enable(ttccs->ttc.clk);
	if (WARN_ON(err)) {
		kfree(ttccs);
		return;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
	ttccs->ttc.freq = clk_get_rate(ttccs->ttc.clk);

>>>>>>> v3.18
=======
	ttccs->ttc.freq = clk_get_rate(ttccs->ttc.clk);

>>>>>>> v3.18
	ttccs->ttc.clk_rate_change_nb.notifier_call =
		ttc_rate_change_clocksource_cb;
	ttccs->ttc.clk_rate_change_nb.next = NULL;
	if (clk_notifier_register(ttccs->ttc.clk,
				&ttccs->ttc.clk_rate_change_nb))
		pr_warn("Unable to register clock notifier.\n");

	ttccs->ttc.base_addr = base;
	ttccs->cs.name = "ttc_clocksource";
	ttccs->cs.rating = 200;
	ttccs->cs.read = __ttc_clocksource_read;
<<<<<<< HEAD
<<<<<<< HEAD
	ttccs->cs.mask = CLOCKSOURCE_MASK(16);
=======
	ttccs->cs.mask = CLOCKSOURCE_MASK(timer_width);
>>>>>>> v3.18
=======
	ttccs->cs.mask = CLOCKSOURCE_MASK(timer_width);
>>>>>>> v3.18
	ttccs->cs.flags = CLOCK_SOURCE_IS_CONTINUOUS;

	/*
	 * Setup the clock source counter to be an incrementing counter
	 * with no interrupt and it rolls over at 0xFFFF. Pre-scale
	 * it by 32 also. Let it start running now.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	__raw_writel(0x0,  ttccs->ttc.base_addr + TTC_IER_OFFSET);
	__raw_writel(CLK_CNTRL_PRESCALE | CLK_CNTRL_PRESCALE_EN,
		     ttccs->ttc.base_addr + TTC_CLK_CNTRL_OFFSET);
	__raw_writel(CNT_CNTRL_RESET,
		     ttccs->ttc.base_addr + TTC_CNT_CNTRL_OFFSET);

	err = clocksource_register_hz(&ttccs->cs,
			clk_get_rate(ttccs->ttc.clk) / PRESCALE);
=======
=======
>>>>>>> v3.18
	writel_relaxed(0x0,  ttccs->ttc.base_addr + TTC_IER_OFFSET);
	writel_relaxed(CLK_CNTRL_PRESCALE | CLK_CNTRL_PRESCALE_EN,
		     ttccs->ttc.base_addr + TTC_CLK_CNTRL_OFFSET);
	writel_relaxed(CNT_CNTRL_RESET,
		     ttccs->ttc.base_addr + TTC_CNT_CNTRL_OFFSET);

	err = clocksource_register_hz(&ttccs->cs, ttccs->ttc.freq / PRESCALE);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (WARN_ON(err)) {
		kfree(ttccs);
		return;
	}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18

	ttc_sched_clock_val_reg = base + TTC_COUNT_VAL_OFFSET;
	sched_clock_register(ttc_sched_clock_read, timer_width,
			     ttccs->ttc.freq / PRESCALE);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static int ttc_rate_change_clockevent_cb(struct notifier_block *nb,
		unsigned long event, void *data)
{
	struct clk_notifier_data *ndata = data;
	struct ttc_timer *ttc = to_ttc_timer(nb);
	struct ttc_timer_clockevent *ttcce = container_of(ttc,
			struct ttc_timer_clockevent, ttc);

	switch (event) {
	case POST_RATE_CHANGE:
<<<<<<< HEAD
<<<<<<< HEAD
	{
		unsigned long flags;

		/*
		 * clockevents_update_freq should be called with IRQ disabled on
		 * the CPU the timer provides events for. The timer we use is
		 * common to both CPUs, not sure if we need to run on both
		 * cores.
		 */
		local_irq_save(flags);
		clockevents_update_freq(&ttcce->ce,
				ndata->new_rate / PRESCALE);
		local_irq_restore(flags);

		/* fall through */
	}
=======
=======
>>>>>>> v3.18
		/* update cached frequency */
		ttc->freq = ndata->new_rate;

		clockevents_update_freq(&ttcce->ce, ndata->new_rate / PRESCALE);

		/* fall through */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	case PRE_RATE_CHANGE:
	case ABORT_RATE_CHANGE:
	default:
		return NOTIFY_DONE;
	}
}

static void __init ttc_setup_clockevent(struct clk *clk,
						void __iomem *base, u32 irq)
{
	struct ttc_timer_clockevent *ttcce;
	int err;

	ttcce = kzalloc(sizeof(*ttcce), GFP_KERNEL);
	if (WARN_ON(!ttcce))
		return;

	ttcce->ttc.clk = clk;

	err = clk_prepare_enable(ttcce->ttc.clk);
	if (WARN_ON(err)) {
		kfree(ttcce);
		return;
	}

	ttcce->ttc.clk_rate_change_nb.notifier_call =
		ttc_rate_change_clockevent_cb;
	ttcce->ttc.clk_rate_change_nb.next = NULL;
	if (clk_notifier_register(ttcce->ttc.clk,
				&ttcce->ttc.clk_rate_change_nb))
		pr_warn("Unable to register clock notifier.\n");
<<<<<<< HEAD
<<<<<<< HEAD
=======
	ttcce->ttc.freq = clk_get_rate(ttcce->ttc.clk);
>>>>>>> v3.18
=======
	ttcce->ttc.freq = clk_get_rate(ttcce->ttc.clk);
>>>>>>> v3.18

	ttcce->ttc.base_addr = base;
	ttcce->ce.name = "ttc_clockevent";
	ttcce->ce.features = CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT;
	ttcce->ce.set_next_event = ttc_set_next_event;
	ttcce->ce.set_mode = ttc_set_mode;
	ttcce->ce.rating = 200;
	ttcce->ce.irq = irq;
	ttcce->ce.cpumask = cpu_possible_mask;

	/*
	 * Setup the clock event timer to be an interval timer which
	 * is prescaled by 32 using the interval interrupt. Leave it
	 * disabled for now.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	__raw_writel(0x23, ttcce->ttc.base_addr + TTC_CNT_CNTRL_OFFSET);
	__raw_writel(CLK_CNTRL_PRESCALE | CLK_CNTRL_PRESCALE_EN,
		     ttcce->ttc.base_addr + TTC_CLK_CNTRL_OFFSET);
	__raw_writel(0x1,  ttcce->ttc.base_addr + TTC_IER_OFFSET);

	err = request_irq(irq, ttc_clock_event_interrupt,
			  IRQF_DISABLED | IRQF_TIMER,
			  ttcce->ce.name, ttcce);
=======
=======
>>>>>>> v3.18
	writel_relaxed(0x23, ttcce->ttc.base_addr + TTC_CNT_CNTRL_OFFSET);
	writel_relaxed(CLK_CNTRL_PRESCALE | CLK_CNTRL_PRESCALE_EN,
		     ttcce->ttc.base_addr + TTC_CLK_CNTRL_OFFSET);
	writel_relaxed(0x1,  ttcce->ttc.base_addr + TTC_IER_OFFSET);

	err = request_irq(irq, ttc_clock_event_interrupt,
			  IRQF_TIMER, ttcce->ce.name, ttcce);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (WARN_ON(err)) {
		kfree(ttcce);
		return;
	}

	clockevents_config_and_register(&ttcce->ce,
<<<<<<< HEAD
<<<<<<< HEAD
			clk_get_rate(ttcce->ttc.clk) / PRESCALE, 1, 0xfffe);
=======
			ttcce->ttc.freq / PRESCALE, 1, 0xfffe);
>>>>>>> v3.18
=======
			ttcce->ttc.freq / PRESCALE, 1, 0xfffe);
>>>>>>> v3.18
}

/**
 * ttc_timer_init - Initialize the timer
 *
 * Initializes the timer hardware and register the clock source and clock event
 * timers with Linux kernal timer framework
 */
static void __init ttc_timer_init(struct device_node *timer)
{
	unsigned int irq;
	void __iomem *timer_baseaddr;
<<<<<<< HEAD
<<<<<<< HEAD
	struct clk *clk;
	static int initialized;
=======
=======
>>>>>>> v3.18
	struct clk *clk_cs, *clk_ce;
	static int initialized;
	int clksel;
	u32 timer_width = 16;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (initialized)
		return;

	initialized = 1;

	/*
	 * Get the 1st Triple Timer Counter (TTC) block from the device tree
	 * and use it. Note that the event timer uses the interrupt and it's the
	 * 2nd TTC hence the irq_of_parse_and_map(,1)
	 */
	timer_baseaddr = of_iomap(timer, 0);
	if (!timer_baseaddr) {
		pr_err("ERROR: invalid timer base address\n");
		BUG();
	}

	irq = irq_of_parse_and_map(timer, 1);
	if (irq <= 0) {
		pr_err("ERROR: invalid interrupt number\n");
		BUG();
	}

<<<<<<< HEAD
<<<<<<< HEAD
	clk = of_clk_get_by_name(timer, "cpu_1x");
	if (IS_ERR(clk)) {
=======
=======
>>>>>>> v3.18
	of_property_read_u32(timer, "timer-width", &timer_width);

	clksel = readl_relaxed(timer_baseaddr + TTC_CLK_CNTRL_OFFSET);
	clksel = !!(clksel & TTC_CLK_CNTRL_CSRC_MASK);
	clk_cs = of_clk_get(timer, clksel);
	if (IS_ERR(clk_cs)) {
		pr_err("ERROR: timer input clock not found\n");
		BUG();
	}

	clksel = readl_relaxed(timer_baseaddr + 4 + TTC_CLK_CNTRL_OFFSET);
	clksel = !!(clksel & TTC_CLK_CNTRL_CSRC_MASK);
	clk_ce = of_clk_get(timer, clksel);
	if (IS_ERR(clk_ce)) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		pr_err("ERROR: timer input clock not found\n");
		BUG();
	}

<<<<<<< HEAD
<<<<<<< HEAD
	ttc_setup_clocksource(clk, timer_baseaddr);
	ttc_setup_clockevent(clk, timer_baseaddr + 4, irq);
=======
	ttc_setup_clocksource(clk_cs, timer_baseaddr, timer_width);
	ttc_setup_clockevent(clk_ce, timer_baseaddr + 4, irq);
>>>>>>> v3.18
=======
	ttc_setup_clocksource(clk_cs, timer_baseaddr, timer_width);
	ttc_setup_clockevent(clk_ce, timer_baseaddr + 4, irq);
>>>>>>> v3.18

	pr_info("%s #0 at %p, irq=%d\n", timer->name, timer_baseaddr, irq);
}

CLOCKSOURCE_OF_DECLARE(ttc, "cdns,ttc", ttc_timer_init);
