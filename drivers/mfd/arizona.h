/*
 * wm5102.h  --  WM5102 MFD internals
 *
 * Copyright 2012 Wolfson Microelectronics plc
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _WM5102_H
#define _WM5102_H

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/of.h>
>>>>>>> v3.18
=======
#include <linux/of.h>
>>>>>>> v3.18
#include <linux/regmap.h>
#include <linux/pm.h>

struct wm_arizona;

extern const struct regmap_config wm5102_i2c_regmap;
extern const struct regmap_config wm5102_spi_regmap;

extern const struct regmap_config wm5110_i2c_regmap;
extern const struct regmap_config wm5110_spi_regmap;

<<<<<<< HEAD
<<<<<<< HEAD
extern const struct dev_pm_ops arizona_pm_ops;

=======
=======
>>>>>>> v3.18
extern const struct regmap_config wm8997_i2c_regmap;

extern const struct dev_pm_ops arizona_pm_ops;

extern const struct of_device_id arizona_of_match[];

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
extern const struct regmap_irq_chip wm5102_aod;
extern const struct regmap_irq_chip wm5102_irq;

extern const struct regmap_irq_chip wm5110_aod;
extern const struct regmap_irq_chip wm5110_irq;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
extern const struct regmap_irq_chip wm5110_revd_irq;

extern const struct regmap_irq_chip wm8997_aod;
extern const struct regmap_irq_chip wm8997_irq;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

int arizona_dev_init(struct arizona *arizona);
int arizona_dev_exit(struct arizona *arizona);
int arizona_irq_init(struct arizona *arizona);
int arizona_irq_exit(struct arizona *arizona);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
#ifdef CONFIG_OF
unsigned long arizona_of_get_type(struct device *dev);
#else
static inline unsigned long arizona_of_get_type(struct device *dev)
{
	return 0;
}
#endif

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif
