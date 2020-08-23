/*
 *  drivers/extcon/extcon_gpio.c
 *
 *  Single-state GPIO extcon driver based on extcon class
 *
 * Copyright (C) 2008 Google, Inc.
 * Author: Mike Lockwood <lockwood@android.com>
 *
 * Modified by MyungJoo Ham <myungjoo.ham@samsung.com> to support extcon
 * (originally switch class is supported)
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
*/

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/gpio.h>
#include <linux/extcon.h>
#include <linux/extcon/extcon-gpio.h>

struct gpio_extcon_data {
	struct extcon_dev edev;
	unsigned gpio;
=======
=======
>>>>>>> v3.18
#include <linux/extcon.h>
#include <linux/extcon/extcon-gpio.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/workqueue.h>

struct gpio_extcon_data {
	struct extcon_dev *edev;
	unsigned gpio;
	bool gpio_active_low;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	const char *state_on;
	const char *state_off;
	int irq;
	struct delayed_work work;
	unsigned long debounce_jiffies;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	bool check_on_resume;
>>>>>>> v3.18
=======
	bool check_on_resume;
>>>>>>> v3.18
};

static void gpio_extcon_work(struct work_struct *work)
{
	int state;
	struct gpio_extcon_data	*data =
		container_of(to_delayed_work(work), struct gpio_extcon_data,
			     work);

	state = gpio_get_value(data->gpio);
<<<<<<< HEAD
<<<<<<< HEAD
	extcon_set_state(&data->edev, state);
=======
	if (data->gpio_active_low)
		state = !state;
	extcon_set_state(data->edev, state);
>>>>>>> v3.18
=======
	if (data->gpio_active_low)
		state = !state;
	extcon_set_state(data->edev, state);
>>>>>>> v3.18
}

static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
	struct gpio_extcon_data *extcon_data = dev_id;

<<<<<<< HEAD
<<<<<<< HEAD
	schedule_delayed_work(&extcon_data->work,
=======
	queue_delayed_work(system_power_efficient_wq, &extcon_data->work,
>>>>>>> v3.18
=======
	queue_delayed_work(system_power_efficient_wq, &extcon_data->work,
>>>>>>> v3.18
			      extcon_data->debounce_jiffies);
	return IRQ_HANDLED;
}

static ssize_t extcon_gpio_print_state(struct extcon_dev *edev, char *buf)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct gpio_extcon_data	*extcon_data =
		container_of(edev, struct gpio_extcon_data, edev);
	const char *state;
=======
=======
>>>>>>> v3.18
	struct device *dev = edev->dev.parent;
	struct gpio_extcon_data *extcon_data = dev_get_drvdata(dev);
	const char *state;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (extcon_get_state(edev))
		state = extcon_data->state_on;
	else
		state = extcon_data->state_off;

	if (state)
		return sprintf(buf, "%s\n", state);
	return -EINVAL;
}

static int gpio_extcon_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct gpio_extcon_platform_data *pdata = pdev->dev.platform_data;
	struct gpio_extcon_data *extcon_data;
	int ret = 0;
=======
	struct gpio_extcon_platform_data *pdata = dev_get_platdata(&pdev->dev);
	struct gpio_extcon_data *extcon_data;
	int ret;
>>>>>>> v3.18
=======
	struct gpio_extcon_platform_data *pdata = dev_get_platdata(&pdev->dev);
	struct gpio_extcon_data *extcon_data;
	int ret;
>>>>>>> v3.18

	if (!pdata)
		return -EBUSY;
	if (!pdata->irq_flags) {
		dev_err(&pdev->dev, "IRQ flag is not specified.\n");
		return -EINVAL;
	}

	extcon_data = devm_kzalloc(&pdev->dev, sizeof(struct gpio_extcon_data),
				   GFP_KERNEL);
	if (!extcon_data)
		return -ENOMEM;

<<<<<<< HEAD
<<<<<<< HEAD
	extcon_data->edev.name = pdata->name;
	extcon_data->gpio = pdata->gpio;
	extcon_data->state_on = pdata->state_on;
	extcon_data->state_off = pdata->state_off;
	if (pdata->state_on && pdata->state_off)
		extcon_data->edev.print_state = extcon_gpio_print_state;
	extcon_data->debounce_jiffies = msecs_to_jiffies(pdata->debounce);

	ret = extcon_dev_register(&extcon_data->edev, &pdev->dev);
	if (ret < 0)
		return ret;

	ret = devm_gpio_request_one(&pdev->dev, extcon_data->gpio, GPIOF_DIR_IN,
				    pdev->name);
	if (ret < 0)
		goto err;
=======
=======
>>>>>>> v3.18
	extcon_data->edev = devm_extcon_dev_allocate(&pdev->dev, NULL);
	if (IS_ERR(extcon_data->edev)) {
		dev_err(&pdev->dev, "failed to allocate extcon device\n");
		return -ENOMEM;
	}
	extcon_data->edev->name = pdata->name;

	extcon_data->gpio = pdata->gpio;
	extcon_data->gpio_active_low = pdata->gpio_active_low;
	extcon_data->state_on = pdata->state_on;
	extcon_data->state_off = pdata->state_off;
	extcon_data->check_on_resume = pdata->check_on_resume;
	if (pdata->state_on && pdata->state_off)
		extcon_data->edev->print_state = extcon_gpio_print_state;

	ret = devm_gpio_request_one(&pdev->dev, extcon_data->gpio, GPIOF_DIR_IN,
				    pdev->name);
	if (ret < 0)
		return ret;

	if (pdata->debounce) {
		ret = gpio_set_debounce(extcon_data->gpio,
					pdata->debounce * 1000);
		if (ret < 0)
			extcon_data->debounce_jiffies =
				msecs_to_jiffies(pdata->debounce);
	}

	ret = devm_extcon_dev_register(&pdev->dev, extcon_data->edev);
	if (ret < 0)
		return ret;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	INIT_DELAYED_WORK(&extcon_data->work, gpio_extcon_work);

	extcon_data->irq = gpio_to_irq(extcon_data->gpio);
<<<<<<< HEAD
<<<<<<< HEAD
	if (extcon_data->irq < 0) {
		ret = extcon_data->irq;
		goto err;
	}
=======
	if (extcon_data->irq < 0)
		return extcon_data->irq;
>>>>>>> v3.18
=======
	if (extcon_data->irq < 0)
		return extcon_data->irq;
>>>>>>> v3.18

	ret = request_any_context_irq(extcon_data->irq, gpio_irq_handler,
				      pdata->irq_flags, pdev->name,
				      extcon_data);
	if (ret < 0)
<<<<<<< HEAD
<<<<<<< HEAD
		goto err;
=======
		return ret;
>>>>>>> v3.18
=======
		return ret;
>>>>>>> v3.18

	platform_set_drvdata(pdev, extcon_data);
	/* Perform initial detection */
	gpio_extcon_work(&extcon_data->work.work);

	return 0;
<<<<<<< HEAD
<<<<<<< HEAD

err:
	extcon_dev_unregister(&extcon_data->edev);

	return ret;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static int gpio_extcon_remove(struct platform_device *pdev)
{
	struct gpio_extcon_data *extcon_data = platform_get_drvdata(pdev);

	cancel_delayed_work_sync(&extcon_data->work);
	free_irq(extcon_data->irq, extcon_data);
<<<<<<< HEAD
<<<<<<< HEAD
	extcon_dev_unregister(&extcon_data->edev);
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
#ifdef CONFIG_PM_SLEEP
static int gpio_extcon_resume(struct device *dev)
{
	struct gpio_extcon_data *extcon_data;

	extcon_data = dev_get_drvdata(dev);
	if (extcon_data->check_on_resume)
		queue_delayed_work(system_power_efficient_wq,
			&extcon_data->work, extcon_data->debounce_jiffies);

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(gpio_extcon_pm_ops, NULL, gpio_extcon_resume);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static struct platform_driver gpio_extcon_driver = {
	.probe		= gpio_extcon_probe,
	.remove		= gpio_extcon_remove,
	.driver		= {
		.name	= "extcon-gpio",
		.owner	= THIS_MODULE,
<<<<<<< HEAD
<<<<<<< HEAD
=======
		.pm	= &gpio_extcon_pm_ops,
>>>>>>> v3.18
=======
		.pm	= &gpio_extcon_pm_ops,
>>>>>>> v3.18
	},
};

module_platform_driver(gpio_extcon_driver);

MODULE_AUTHOR("Mike Lockwood <lockwood@android.com>");
MODULE_DESCRIPTION("GPIO extcon driver");
MODULE_LICENSE("GPL");
