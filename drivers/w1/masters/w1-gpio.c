/*
 * w1-gpio - GPIO w1 bus master driver
 *
 * Copyright (C) 2007 Ville Syrjala <syrjala@sci.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/w1-gpio.h>
#include <linux/gpio.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/pinctrl/consumer.h>
#include <linux/err.h>
#include <linux/of.h>
=======
#include <linux/err.h>
#include <linux/of.h>
#include <linux/delay.h>
>>>>>>> v3.18
=======
#include <linux/err.h>
#include <linux/of.h>
#include <linux/delay.h>
>>>>>>> v3.18

#include "../w1.h"
#include "../w1_int.h"

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
static u8 w1_gpio_set_pullup(void *data, int delay)
{
	struct w1_gpio_platform_data *pdata = data;

	if (delay) {
		pdata->pullup_duration = delay;
	} else {
		if (pdata->pullup_duration) {
			gpio_direction_output(pdata->pin, 1);

			msleep(pdata->pullup_duration);

			gpio_direction_input(pdata->pin);
		}
		pdata->pullup_duration = 0;
	}

	return 0;
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static void w1_gpio_write_bit_dir(void *data, u8 bit)
{
	struct w1_gpio_platform_data *pdata = data;

	if (bit)
		gpio_direction_input(pdata->pin);
	else
		gpio_direction_output(pdata->pin, 0);
}

static void w1_gpio_write_bit_val(void *data, u8 bit)
{
	struct w1_gpio_platform_data *pdata = data;

	gpio_set_value(pdata->pin, bit);
}

static u8 w1_gpio_read_bit(void *data)
{
	struct w1_gpio_platform_data *pdata = data;

	return gpio_get_value(pdata->pin) ? 1 : 0;
}

#if defined(CONFIG_OF)
static struct of_device_id w1_gpio_dt_ids[] = {
	{ .compatible = "w1-gpio" },
	{}
};
MODULE_DEVICE_TABLE(of, w1_gpio_dt_ids);
#endif

static int w1_gpio_probe_dt(struct platform_device *pdev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct w1_gpio_platform_data *pdata = pdev->dev.platform_data;
	struct device_node *np = pdev->dev.of_node;
=======
	struct w1_gpio_platform_data *pdata = dev_get_platdata(&pdev->dev);
	struct device_node *np = pdev->dev.of_node;
	int gpio;
>>>>>>> v3.18
=======
	struct w1_gpio_platform_data *pdata = dev_get_platdata(&pdev->dev);
	struct device_node *np = pdev->dev.of_node;
	int gpio;
>>>>>>> v3.18

	pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return -ENOMEM;

	if (of_get_property(np, "linux,open-drain", NULL))
		pdata->is_open_drain = 1;

<<<<<<< HEAD
<<<<<<< HEAD
	pdata->pin = of_get_gpio(np, 0);
	pdata->ext_pullup_enable_pin = of_get_gpio(np, 1);
=======
=======
>>>>>>> v3.18
	gpio = of_get_gpio(np, 0);
	if (gpio < 0) {
		if (gpio != -EPROBE_DEFER)
			dev_err(&pdev->dev,
					"Failed to parse gpio property for data pin (%d)\n",
					gpio);

		return gpio;
	}
	pdata->pin = gpio;

	gpio = of_get_gpio(np, 1);
	if (gpio == -EPROBE_DEFER)
		return gpio;
	/* ignore other errors as the pullup gpio is optional */
	pdata->ext_pullup_enable_pin = gpio;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	pdev->dev.platform_data = pdata;

	return 0;
}

static int w1_gpio_probe(struct platform_device *pdev)
{
	struct w1_bus_master *master;
	struct w1_gpio_platform_data *pdata;
<<<<<<< HEAD
<<<<<<< HEAD
	struct pinctrl *pinctrl;
	int err;

	pinctrl = devm_pinctrl_get_select_default(&pdev->dev);
	if (IS_ERR(pinctrl))
		dev_warn(&pdev->dev, "unable to select pin group\n");

	if (of_have_populated_dt()) {
		err = w1_gpio_probe_dt(pdev);
		if (err < 0) {
			dev_err(&pdev->dev, "Failed to parse DT\n");
			return err;
		}
	}

	pdata = pdev->dev.platform_data;
=======
=======
>>>>>>> v3.18
	int err;

	if (of_have_populated_dt()) {
		err = w1_gpio_probe_dt(pdev);
		if (err < 0)
			return err;
	}

	pdata = dev_get_platdata(&pdev->dev);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (!pdata) {
		dev_err(&pdev->dev, "No configuration data\n");
		return -ENXIO;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	master = kzalloc(sizeof(struct w1_bus_master), GFP_KERNEL);
=======
	master = devm_kzalloc(&pdev->dev, sizeof(struct w1_bus_master),
			GFP_KERNEL);
>>>>>>> v3.18
=======
	master = devm_kzalloc(&pdev->dev, sizeof(struct w1_bus_master),
			GFP_KERNEL);
>>>>>>> v3.18
	if (!master) {
		dev_err(&pdev->dev, "Out of memory\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	err = gpio_request(pdata->pin, "w1");
	if (err) {
		dev_err(&pdev->dev, "gpio_request (pin) failed\n");
		goto free_master;
	}

	if (gpio_is_valid(pdata->ext_pullup_enable_pin)) {
		err = gpio_request_one(pdata->ext_pullup_enable_pin,
				       GPIOF_INIT_LOW, "w1 pullup");
		if (err < 0) {
			dev_err(&pdev->dev, "gpio_request_one "
					"(ext_pullup_enable_pin) failed\n");
			goto free_gpio;
=======
=======
>>>>>>> v3.18
	err = devm_gpio_request(&pdev->dev, pdata->pin, "w1");
	if (err) {
		dev_err(&pdev->dev, "gpio_request (pin) failed\n");
		return err;
	}

	if (gpio_is_valid(pdata->ext_pullup_enable_pin)) {
		err = devm_gpio_request_one(&pdev->dev,
				pdata->ext_pullup_enable_pin, GPIOF_INIT_LOW,
				"w1 pullup");
		if (err < 0) {
			dev_err(&pdev->dev, "gpio_request_one "
					"(ext_pullup_enable_pin) failed\n");
			return err;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		}
	}

	master->data = pdata;
	master->read_bit = w1_gpio_read_bit;

	if (pdata->is_open_drain) {
		gpio_direction_output(pdata->pin, 1);
		master->write_bit = w1_gpio_write_bit_val;
	} else {
		gpio_direction_input(pdata->pin);
		master->write_bit = w1_gpio_write_bit_dir;
<<<<<<< HEAD
<<<<<<< HEAD
=======
		master->set_pullup = w1_gpio_set_pullup;
>>>>>>> v3.18
=======
		master->set_pullup = w1_gpio_set_pullup;
>>>>>>> v3.18
	}

	err = w1_add_master_device(master);
	if (err) {
		dev_err(&pdev->dev, "w1_add_master device failed\n");
<<<<<<< HEAD
<<<<<<< HEAD
		goto free_gpio_ext_pu;
=======
		return err;
>>>>>>> v3.18
=======
		return err;
>>>>>>> v3.18
	}

	if (pdata->enable_external_pullup)
		pdata->enable_external_pullup(1);

	if (gpio_is_valid(pdata->ext_pullup_enable_pin))
		gpio_set_value(pdata->ext_pullup_enable_pin, 1);

	platform_set_drvdata(pdev, master);

	return 0;
<<<<<<< HEAD
<<<<<<< HEAD

 free_gpio_ext_pu:
	if (gpio_is_valid(pdata->ext_pullup_enable_pin))
		gpio_free(pdata->ext_pullup_enable_pin);
 free_gpio:
	gpio_free(pdata->pin);
 free_master:
	kfree(master);

	return err;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static int w1_gpio_remove(struct platform_device *pdev)
{
	struct w1_bus_master *master = platform_get_drvdata(pdev);
<<<<<<< HEAD
<<<<<<< HEAD
	struct w1_gpio_platform_data *pdata = pdev->dev.platform_data;
=======
	struct w1_gpio_platform_data *pdata = dev_get_platdata(&pdev->dev);
>>>>>>> v3.18
=======
	struct w1_gpio_platform_data *pdata = dev_get_platdata(&pdev->dev);
>>>>>>> v3.18

	if (pdata->enable_external_pullup)
		pdata->enable_external_pullup(0);

	if (gpio_is_valid(pdata->ext_pullup_enable_pin))
		gpio_set_value(pdata->ext_pullup_enable_pin, 0);

	w1_remove_master_device(master);
<<<<<<< HEAD
<<<<<<< HEAD
	gpio_free(pdata->pin);
	kfree(master);
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return 0;
}

#ifdef CONFIG_PM

static int w1_gpio_suspend(struct platform_device *pdev, pm_message_t state)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct w1_gpio_platform_data *pdata = pdev->dev.platform_data;
=======
	struct w1_gpio_platform_data *pdata = dev_get_platdata(&pdev->dev);
>>>>>>> v3.18
=======
	struct w1_gpio_platform_data *pdata = dev_get_platdata(&pdev->dev);
>>>>>>> v3.18

	if (pdata->enable_external_pullup)
		pdata->enable_external_pullup(0);

	return 0;
}

static int w1_gpio_resume(struct platform_device *pdev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct w1_gpio_platform_data *pdata = pdev->dev.platform_data;
=======
	struct w1_gpio_platform_data *pdata = dev_get_platdata(&pdev->dev);
>>>>>>> v3.18
=======
	struct w1_gpio_platform_data *pdata = dev_get_platdata(&pdev->dev);
>>>>>>> v3.18

	if (pdata->enable_external_pullup)
		pdata->enable_external_pullup(1);

	return 0;
}

#else
#define w1_gpio_suspend	NULL
#define w1_gpio_resume	NULL
#endif

static struct platform_driver w1_gpio_driver = {
	.driver = {
		.name	= "w1-gpio",
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(w1_gpio_dt_ids),
	},
	.probe = w1_gpio_probe,
	.remove	= w1_gpio_remove,
	.suspend = w1_gpio_suspend,
	.resume = w1_gpio_resume,
};

module_platform_driver(w1_gpio_driver);

MODULE_DESCRIPTION("GPIO w1 bus master driver");
MODULE_AUTHOR("Ville Syrjala <syrjala@sci.fi>");
MODULE_LICENSE("GPL");
