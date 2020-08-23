/*
 * Copyright (C) 2005-2013 Imagination Technologies Ltd.
 *
 * This file contains the Meta-specific time handling details.
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/init.h>

#include <clocksource/metag_generic.h>

void __init time_init(void)
{
=======
=======
>>>>>>> v3.18
#include <clocksource/metag_generic.h>
#include <linux/clk-provider.h>
#include <linux/init.h>
#include <asm/clock.h>

void __init time_init(void)
{
#ifdef CONFIG_COMMON_CLK
	/* Init clocks from device tree */
	of_clk_init(NULL);
#endif

	/* Init meta clocks, particularly the core clock */
	init_metag_clocks();

	/* Set up the timer clock sources */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	metag_generic_timer_init();
}
