/*
 *  Setup code for AT91RM9200 Evaluation Kits with Device Tree support
 *
 *  Copyright (C) 2011 Atmel,
 *                2011 Nicolas Ferre <nicolas.ferre@atmel.com>
 *                2012 Joachim Eastwood <manabian@gmail.com>
 *
 * Licensed under GPLv2 or later.
 */

#include <linux/types.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/of_irq.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/of_platform.h>
=======
#include <linux/clk-provider.h>
>>>>>>> v3.18
=======
#include <linux/clk-provider.h>
>>>>>>> v3.18

#include <asm/setup.h>
#include <asm/irq.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include "at91_aic.h"
#include "generic.h"

<<<<<<< HEAD
<<<<<<< HEAD

static const struct of_device_id irq_of_match[] __initconst = {
	{ .compatible = "atmel,at91rm9200-aic", .data = at91_aic_of_init },
	{ /*sentinel*/ }
};

static void __init at91rm9200_dt_init_irq(void)
{
	of_irq_init(irq_of_match);
}

static void __init at91rm9200_dt_device_init(void)
{
	of_platform_populate(NULL, of_default_bus_match_table, NULL, NULL);
=======
=======
>>>>>>> v3.18
static void __init at91rm9200_dt_timer_init(void)
{
#if defined(CONFIG_COMMON_CLK)
	of_clk_init(NULL);
#endif
	at91rm9200_timer_init();
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static const char *at91rm9200_dt_board_compat[] __initdata = {
	"atmel,at91rm9200",
	NULL
};

DT_MACHINE_START(at91rm9200_dt, "Atmel AT91RM9200 (Device Tree)")
<<<<<<< HEAD
<<<<<<< HEAD
	.init_time      = at91rm9200_timer_init,
	.map_io		= at91_map_io,
	.handle_irq	= at91_aic_handle_irq,
	.init_early	= at91rm9200_dt_initialize,
	.init_irq	= at91rm9200_dt_init_irq,
	.init_machine	= at91rm9200_dt_device_init,
=======
	.init_time      = at91rm9200_dt_timer_init,
	.map_io		= at91_map_io,
	.init_early	= at91rm9200_dt_initialize,
>>>>>>> v3.18
=======
	.init_time      = at91rm9200_dt_timer_init,
	.map_io		= at91_map_io,
	.init_early	= at91rm9200_dt_initialize,
>>>>>>> v3.18
	.dt_compat	= at91rm9200_dt_board_compat,
MACHINE_END
