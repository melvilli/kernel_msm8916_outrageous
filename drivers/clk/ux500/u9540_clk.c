/*
 * Clock definitions for u9540 platform.
 *
 * Copyright (C) 2012 ST-Ericsson SA
 * Author: Ulf Hansson <ulf.hansson@linaro.org>
 *
 * License terms: GNU General Public License (GPL) version 2
 */

#include <linux/clk.h>
#include <linux/clkdev.h>
#include <linux/clk-provider.h>
#include <linux/mfd/dbx500-prcmu.h>
#include <linux/platform_data/clk-ux500.h>
<<<<<<< HEAD
<<<<<<< HEAD

#include "clk.h"

void u9540_clk_init(void)
=======
=======
>>>>>>> v3.18
#include "clk.h"

void u9540_clk_init(u32 clkrst1_base, u32 clkrst2_base, u32 clkrst3_base,
		    u32 clkrst5_base, u32 clkrst6_base)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	/* register clocks here */
}
