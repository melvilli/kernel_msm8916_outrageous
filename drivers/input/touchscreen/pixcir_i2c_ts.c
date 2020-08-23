/*
 * Driver for Pixcir I2C touchscreen controllers.
 *
 * Copyright (C) 2010-2011 Pixcir, Inc.
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
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <linux/delay.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/input.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/input/pixcir_ts.h>
=======
=======
>>>>>>> v3.18
#include <linux/input/mt.h>
#include <linux/input/pixcir_ts.h>
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/of_device.h>

#define PIXCIR_MAX_SLOTS       5 /* Max fingers supported by driver */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

struct pixcir_i2c_ts_data {
	struct i2c_client *client;
	struct input_dev *input;
<<<<<<< HEAD
<<<<<<< HEAD
	const struct pixcir_ts_platform_data *chip;
	bool exiting;
};

static void pixcir_ts_poscheck(struct pixcir_i2c_ts_data *data)
{
	struct pixcir_i2c_ts_data *tsdata = data;
	u8 rdbuf[10], wrbuf[1] = { 0 };
	u8 touch;
	int ret;
=======
=======
>>>>>>> v3.18
	const struct pixcir_ts_platform_data *pdata;
	bool running;
	int max_fingers;	/* Max fingers supported in this instance */
};

struct pixcir_touch {
	int x;
	int y;
	int id;
};

struct pixcir_report_data {
	int num_touches;
	struct pixcir_touch touches[PIXCIR_MAX_SLOTS];
};

static void pixcir_ts_parse(struct pixcir_i2c_ts_data *tsdata,
			    struct pixcir_report_data *report)
{
	u8 rdbuf[2 + PIXCIR_MAX_SLOTS * 5];
	u8 wrbuf[1] = { 0 };
	u8 *bufptr;
	u8 touch;
	int ret, i;
	int readsize;
	const struct pixcir_i2c_chip_data *chip = &tsdata->pdata->chip;

	memset(report, 0, sizeof(struct pixcir_report_data));

	i = chip->has_hw_ids ? 1 : 0;
	readsize = 2 + tsdata->max_fingers * (4 + i);
	if (readsize > sizeof(rdbuf))
		readsize = sizeof(rdbuf);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	ret = i2c_master_send(tsdata->client, wrbuf, sizeof(wrbuf));
	if (ret != sizeof(wrbuf)) {
		dev_err(&tsdata->client->dev,
			"%s: i2c_master_send failed(), ret=%d\n",
			__func__, ret);
		return;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	ret = i2c_master_recv(tsdata->client, rdbuf, sizeof(rdbuf));
=======
	ret = i2c_master_recv(tsdata->client, rdbuf, readsize);
>>>>>>> v3.18
=======
	ret = i2c_master_recv(tsdata->client, rdbuf, readsize);
>>>>>>> v3.18
	if (ret != sizeof(rdbuf)) {
		dev_err(&tsdata->client->dev,
			"%s: i2c_master_recv failed(), ret=%d\n",
			__func__, ret);
		return;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	touch = rdbuf[0];
	if (touch) {
		u16 posx1 = (rdbuf[3] << 8) | rdbuf[2];
		u16 posy1 = (rdbuf[5] << 8) | rdbuf[4];
		u16 posx2 = (rdbuf[7] << 8) | rdbuf[6];
		u16 posy2 = (rdbuf[9] << 8) | rdbuf[8];

		input_report_key(tsdata->input, BTN_TOUCH, 1);
		input_report_abs(tsdata->input, ABS_X, posx1);
		input_report_abs(tsdata->input, ABS_Y, posy1);

		input_report_abs(tsdata->input, ABS_MT_POSITION_X, posx1);
		input_report_abs(tsdata->input, ABS_MT_POSITION_Y, posy1);
		input_mt_sync(tsdata->input);

		if (touch == 2) {
			input_report_abs(tsdata->input,
					 ABS_MT_POSITION_X, posx2);
			input_report_abs(tsdata->input,
					 ABS_MT_POSITION_Y, posy2);
			input_mt_sync(tsdata->input);
		}
	} else {
		input_report_key(tsdata->input, BTN_TOUCH, 0);
	}

	input_sync(tsdata->input);
=======
=======
>>>>>>> v3.18
	touch = rdbuf[0] & 0x7;
	if (touch > tsdata->max_fingers)
		touch = tsdata->max_fingers;

	report->num_touches = touch;
	bufptr = &rdbuf[2];

	for (i = 0; i < touch; i++) {
		report->touches[i].x = (bufptr[1] << 8) | bufptr[0];
		report->touches[i].y = (bufptr[3] << 8) | bufptr[2];

		if (chip->has_hw_ids) {
			report->touches[i].id = bufptr[4];
			bufptr = bufptr + 5;
		} else {
			bufptr = bufptr + 4;
		}
	}
}

static void pixcir_ts_report(struct pixcir_i2c_ts_data *ts,
			     struct pixcir_report_data *report)
{
	struct input_mt_pos pos[PIXCIR_MAX_SLOTS];
	int slots[PIXCIR_MAX_SLOTS];
	struct pixcir_touch *touch;
	int n, i, slot;
	struct device *dev = &ts->client->dev;
	const struct pixcir_i2c_chip_data *chip = &ts->pdata->chip;

	n = report->num_touches;
	if (n > PIXCIR_MAX_SLOTS)
		n = PIXCIR_MAX_SLOTS;

	if (!chip->has_hw_ids) {
		for (i = 0; i < n; i++) {
			touch = &report->touches[i];
			pos[i].x = touch->x;
			pos[i].y = touch->y;
		}

		input_mt_assign_slots(ts->input, slots, pos, n);
	}

	for (i = 0; i < n; i++) {
		touch = &report->touches[i];

		if (chip->has_hw_ids) {
			slot = input_mt_get_slot_by_key(ts->input, touch->id);
			if (slot < 0) {
				dev_dbg(dev, "no free slot for id 0x%x\n",
					touch->id);
				continue;
			}
		} else {
			slot = slots[i];
		}

		input_mt_slot(ts->input, slot);
		input_mt_report_slot_state(ts->input,
					   MT_TOOL_FINGER, true);

		input_event(ts->input, EV_ABS, ABS_MT_POSITION_X, touch->x);
		input_event(ts->input, EV_ABS, ABS_MT_POSITION_Y, touch->y);

		dev_dbg(dev, "%d: slot %d, x %d, y %d\n",
			i, slot, touch->x, touch->y);
	}

	input_mt_sync_frame(ts->input);
	input_sync(ts->input);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static irqreturn_t pixcir_ts_isr(int irq, void *dev_id)
{
	struct pixcir_i2c_ts_data *tsdata = dev_id;
<<<<<<< HEAD
<<<<<<< HEAD

	while (!tsdata->exiting) {
		pixcir_ts_poscheck(tsdata);

		if (tsdata->chip->attb_read_val())
			break;
=======
=======
>>>>>>> v3.18
	const struct pixcir_ts_platform_data *pdata = tsdata->pdata;
	struct pixcir_report_data report;

	while (tsdata->running) {
		/* parse packet */
		pixcir_ts_parse(tsdata, &report);

		/* report it */
		pixcir_ts_report(tsdata, &report);

		if (gpio_get_value(pdata->gpio_attb)) {
			if (report.num_touches) {
				/*
				 * Last report with no finger up?
				 * Do it now then.
				 */
				input_mt_sync_frame(tsdata->input);
				input_sync(tsdata->input);
			}
			break;
		}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

		msleep(20);
	}

	return IRQ_HANDLED;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
static int pixcir_set_power_mode(struct pixcir_i2c_ts_data *ts,
				 enum pixcir_power_mode mode)
{
	struct device *dev = &ts->client->dev;
	int ret;

	ret = i2c_smbus_read_byte_data(ts->client, PIXCIR_REG_POWER_MODE);
	if (ret < 0) {
		dev_err(dev, "%s: can't read reg 0x%x : %d\n",
			__func__, PIXCIR_REG_POWER_MODE, ret);
		return ret;
	}

	ret &= ~PIXCIR_POWER_MODE_MASK;
	ret |= mode;

	/* Always AUTO_IDLE */
	ret |= PIXCIR_POWER_ALLOW_IDLE;

	ret = i2c_smbus_write_byte_data(ts->client, PIXCIR_REG_POWER_MODE, ret);
	if (ret < 0) {
		dev_err(dev, "%s: can't write reg 0x%x : %d\n",
			__func__, PIXCIR_REG_POWER_MODE, ret);
		return ret;
	}

	return 0;
}

/*
 * Set the interrupt mode for the device i.e. ATTB line behaviour
 *
 * @polarity : 1 for active high, 0 for active low.
 */
static int pixcir_set_int_mode(struct pixcir_i2c_ts_data *ts,
			       enum pixcir_int_mode mode, bool polarity)
{
	struct device *dev = &ts->client->dev;
	int ret;

	ret = i2c_smbus_read_byte_data(ts->client, PIXCIR_REG_INT_MODE);
	if (ret < 0) {
		dev_err(dev, "%s: can't read reg 0x%x : %d\n",
			__func__, PIXCIR_REG_INT_MODE, ret);
		return ret;
	}

	ret &= ~PIXCIR_INT_MODE_MASK;
	ret |= mode;

	if (polarity)
		ret |= PIXCIR_INT_POL_HIGH;
	else
		ret &= ~PIXCIR_INT_POL_HIGH;

	ret = i2c_smbus_write_byte_data(ts->client, PIXCIR_REG_INT_MODE, ret);
	if (ret < 0) {
		dev_err(dev, "%s: can't write reg 0x%x : %d\n",
			__func__, PIXCIR_REG_INT_MODE, ret);
		return ret;
	}

	return 0;
}

/*
 * Enable/disable interrupt generation
 */
static int pixcir_int_enable(struct pixcir_i2c_ts_data *ts, bool enable)
{
	struct device *dev = &ts->client->dev;
	int ret;

	ret = i2c_smbus_read_byte_data(ts->client, PIXCIR_REG_INT_MODE);
	if (ret < 0) {
		dev_err(dev, "%s: can't read reg 0x%x : %d\n",
			__func__, PIXCIR_REG_INT_MODE, ret);
		return ret;
	}

	if (enable)
		ret |= PIXCIR_INT_ENABLE;
	else
		ret &= ~PIXCIR_INT_ENABLE;

	ret = i2c_smbus_write_byte_data(ts->client, PIXCIR_REG_INT_MODE, ret);
	if (ret < 0) {
		dev_err(dev, "%s: can't write reg 0x%x : %d\n",
			__func__, PIXCIR_REG_INT_MODE, ret);
		return ret;
	}

	return 0;
}

static int pixcir_start(struct pixcir_i2c_ts_data *ts)
{
	struct device *dev = &ts->client->dev;
	int error;

	/* LEVEL_TOUCH interrupt with active low polarity */
	error = pixcir_set_int_mode(ts, PIXCIR_INT_LEVEL_TOUCH, 0);
	if (error) {
		dev_err(dev, "Failed to set interrupt mode: %d\n", error);
		return error;
	}

	ts->running = true;
	mb();	/* Update status before IRQ can fire */

	/* enable interrupt generation */
	error = pixcir_int_enable(ts, true);
	if (error) {
		dev_err(dev, "Failed to enable interrupt generation: %d\n",
			error);
		return error;
	}

	return 0;
}

static int pixcir_stop(struct pixcir_i2c_ts_data *ts)
{
	int error;

	/* Disable interrupt generation */
	error = pixcir_int_enable(ts, false);
	if (error) {
		dev_err(&ts->client->dev,
			"Failed to disable interrupt generation: %d\n",
			error);
		return error;
	}

	/* Exit ISR if running, no more report parsing */
	ts->running = false;
	mb();	/* update status before we synchronize irq */

	/* Wait till running ISR is complete */
	synchronize_irq(ts->client->irq);

	return 0;
}

static int pixcir_input_open(struct input_dev *dev)
{
	struct pixcir_i2c_ts_data *ts = input_get_drvdata(dev);

	return pixcir_start(ts);
}

static void pixcir_input_close(struct input_dev *dev)
{
	struct pixcir_i2c_ts_data *ts = input_get_drvdata(dev);

	pixcir_stop(ts);
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#ifdef CONFIG_PM_SLEEP
static int pixcir_i2c_ts_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
<<<<<<< HEAD
<<<<<<< HEAD

	if (device_may_wakeup(&client->dev))
		enable_irq_wake(client->irq);

	return 0;
=======
=======
>>>>>>> v3.18
	struct pixcir_i2c_ts_data *ts = i2c_get_clientdata(client);
	struct input_dev *input = ts->input;
	int ret = 0;

	mutex_lock(&input->mutex);

	if (device_may_wakeup(&client->dev)) {
		if (!input->users) {
			ret = pixcir_start(ts);
			if (ret) {
				dev_err(dev, "Failed to start\n");
				goto unlock;
			}
		}

		enable_irq_wake(client->irq);
	} else if (input->users) {
		ret = pixcir_stop(ts);
	}

unlock:
	mutex_unlock(&input->mutex);

	return ret;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static int pixcir_i2c_ts_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
<<<<<<< HEAD
<<<<<<< HEAD

	if (device_may_wakeup(&client->dev))
		disable_irq_wake(client->irq);

	return 0;
=======
=======
>>>>>>> v3.18
	struct pixcir_i2c_ts_data *ts = i2c_get_clientdata(client);
	struct input_dev *input = ts->input;
	int ret = 0;

	mutex_lock(&input->mutex);

	if (device_may_wakeup(&client->dev)) {
		disable_irq_wake(client->irq);

		if (!input->users) {
			ret = pixcir_stop(ts);
			if (ret) {
				dev_err(dev, "Failed to stop\n");
				goto unlock;
			}
		}
	} else if (input->users) {
		ret = pixcir_start(ts);
	}

unlock:
	mutex_unlock(&input->mutex);

	return ret;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}
#endif

static SIMPLE_DEV_PM_OPS(pixcir_dev_pm_ops,
			 pixcir_i2c_ts_suspend, pixcir_i2c_ts_resume);

<<<<<<< HEAD
<<<<<<< HEAD
static int pixcir_i2c_ts_probe(struct i2c_client *client,
					 const struct i2c_device_id *id)
{
	const struct pixcir_ts_platform_data *pdata = client->dev.platform_data;
=======
=======
>>>>>>> v3.18
#ifdef CONFIG_OF
static const struct of_device_id pixcir_of_match[];

static struct pixcir_ts_platform_data *pixcir_parse_dt(struct device *dev)
{
	struct pixcir_ts_platform_data *pdata;
	struct device_node *np = dev->of_node;
	const struct of_device_id *match;

	match = of_match_device(of_match_ptr(pixcir_of_match), dev);
	if (!match)
		return ERR_PTR(-EINVAL);

	pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return ERR_PTR(-ENOMEM);

	pdata->chip = *(const struct pixcir_i2c_chip_data *)match->data;

	pdata->gpio_attb = of_get_named_gpio(np, "attb-gpio", 0);
	/* gpio_attb validity is checked in probe */

	if (of_property_read_u32(np, "touchscreen-size-x", &pdata->x_max)) {
		dev_err(dev, "Failed to get touchscreen-size-x property\n");
		return ERR_PTR(-EINVAL);
	}
	pdata->x_max -= 1;

	if (of_property_read_u32(np, "touchscreen-size-y", &pdata->y_max)) {
		dev_err(dev, "Failed to get touchscreen-size-y property\n");
		return ERR_PTR(-EINVAL);
	}
	pdata->y_max -= 1;

	dev_dbg(dev, "%s: x %d, y %d, gpio %d\n", __func__,
		pdata->x_max + 1, pdata->y_max + 1, pdata->gpio_attb);

	return pdata;
}
#else
static struct pixcir_ts_platform_data *pixcir_parse_dt(struct device *dev)
{
	return ERR_PTR(-EINVAL);
}
#endif

static int pixcir_i2c_ts_probe(struct i2c_client *client,
					 const struct i2c_device_id *id)
{
	const struct pixcir_ts_platform_data *pdata =
			dev_get_platdata(&client->dev);
	struct device *dev = &client->dev;
	struct device_node *np = dev->of_node;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	struct pixcir_i2c_ts_data *tsdata;
	struct input_dev *input;
	int error;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	if (np && !pdata) {
		pdata = pixcir_parse_dt(dev);
		if (IS_ERR(pdata))
			return PTR_ERR(pdata);
	}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (!pdata) {
		dev_err(&client->dev, "platform data not defined\n");
		return -EINVAL;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	tsdata = kzalloc(sizeof(*tsdata), GFP_KERNEL);
	input = input_allocate_device();
	if (!tsdata || !input) {
		dev_err(&client->dev, "Failed to allocate driver data!\n");
		error = -ENOMEM;
		goto err_free_mem;
=======
=======
>>>>>>> v3.18
	if (!gpio_is_valid(pdata->gpio_attb)) {
		dev_err(dev, "Invalid gpio_attb in pdata\n");
		return -EINVAL;
	}

	if (!pdata->chip.max_fingers) {
		dev_err(dev, "Invalid max_fingers in pdata\n");
		return -EINVAL;
	}

	tsdata = devm_kzalloc(dev, sizeof(*tsdata), GFP_KERNEL);
	if (!tsdata)
		return -ENOMEM;

	input = devm_input_allocate_device(dev);
	if (!input) {
		dev_err(dev, "Failed to allocate input device\n");
		return -ENOMEM;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}

	tsdata->client = client;
	tsdata->input = input;
<<<<<<< HEAD
<<<<<<< HEAD
	tsdata->chip = pdata;

	input->name = client->name;
	input->id.bustype = BUS_I2C;
=======
=======
>>>>>>> v3.18
	tsdata->pdata = pdata;

	input->name = client->name;
	input->id.bustype = BUS_I2C;
	input->open = pixcir_input_open;
	input->close = pixcir_input_close;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	input->dev.parent = &client->dev;

	__set_bit(EV_KEY, input->evbit);
	__set_bit(EV_ABS, input->evbit);
	__set_bit(BTN_TOUCH, input->keybit);
	input_set_abs_params(input, ABS_X, 0, pdata->x_max, 0, 0);
	input_set_abs_params(input, ABS_Y, 0, pdata->y_max, 0, 0);
	input_set_abs_params(input, ABS_MT_POSITION_X, 0, pdata->x_max, 0, 0);
	input_set_abs_params(input, ABS_MT_POSITION_Y, 0, pdata->y_max, 0, 0);

<<<<<<< HEAD
<<<<<<< HEAD
	input_set_drvdata(input, tsdata);

	error = request_threaded_irq(client->irq, NULL, pixcir_ts_isr,
				     IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
				     client->name, tsdata);
	if (error) {
		dev_err(&client->dev, "Unable to request touchscreen IRQ.\n");
		goto err_free_mem;
	}

	error = input_register_device(input);
	if (error)
		goto err_free_irq;
=======
=======
>>>>>>> v3.18
	tsdata->max_fingers = tsdata->pdata->chip.max_fingers;
	if (tsdata->max_fingers > PIXCIR_MAX_SLOTS) {
		tsdata->max_fingers = PIXCIR_MAX_SLOTS;
		dev_info(dev, "Limiting maximum fingers to %d\n",
			 tsdata->max_fingers);
	}

	error = input_mt_init_slots(input, tsdata->max_fingers,
				    INPUT_MT_DIRECT | INPUT_MT_DROP_UNUSED);
	if (error) {
		dev_err(dev, "Error initializing Multi-Touch slots\n");
		return error;
	}

	input_set_drvdata(input, tsdata);

	error = devm_gpio_request_one(dev, pdata->gpio_attb,
				      GPIOF_DIR_IN, "pixcir_i2c_attb");
	if (error) {
		dev_err(dev, "Failed to request ATTB gpio\n");
		return error;
	}

	error = devm_request_threaded_irq(dev, client->irq, NULL, pixcir_ts_isr,
					  IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
					  client->name, tsdata);
	if (error) {
		dev_err(dev, "failed to request irq %d\n", client->irq);
		return error;
	}

	/* Always be in IDLE mode to save power, device supports auto wake */
	error = pixcir_set_power_mode(tsdata, PIXCIR_POWER_IDLE);
	if (error) {
		dev_err(dev, "Failed to set IDLE mode\n");
		return error;
	}

	/* Stop device till opened */
	error = pixcir_stop(tsdata);
	if (error)
		return error;

	error = input_register_device(input);
	if (error)
		return error;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	i2c_set_clientdata(client, tsdata);
	device_init_wakeup(&client->dev, 1);

	return 0;
<<<<<<< HEAD
<<<<<<< HEAD

err_free_irq:
	free_irq(client->irq, tsdata);
err_free_mem:
	input_free_device(input);
	kfree(tsdata);
	return error;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static int pixcir_i2c_ts_remove(struct i2c_client *client)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct pixcir_i2c_ts_data *tsdata = i2c_get_clientdata(client);

	device_init_wakeup(&client->dev, 0);

	tsdata->exiting = true;
	mb();
	free_irq(client->irq, tsdata);

	input_unregister_device(tsdata->input);
	kfree(tsdata);

=======
	device_init_wakeup(&client->dev, 0);

>>>>>>> v3.18
=======
	device_init_wakeup(&client->dev, 0);

>>>>>>> v3.18
	return 0;
}

static const struct i2c_device_id pixcir_i2c_ts_id[] = {
	{ "pixcir_ts", 0 },
<<<<<<< HEAD
<<<<<<< HEAD
=======
	{ "pixcir_tangoc", 0 },
>>>>>>> v3.18
=======
	{ "pixcir_tangoc", 0 },
>>>>>>> v3.18
	{ }
};
MODULE_DEVICE_TABLE(i2c, pixcir_i2c_ts_id);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
#ifdef CONFIG_OF
static const struct pixcir_i2c_chip_data pixcir_ts_data = {
	.max_fingers = 2,
	/* no hw id support */
};

static const struct pixcir_i2c_chip_data pixcir_tangoc_data = {
	.max_fingers = 5,
	.has_hw_ids = true,
};

static const struct of_device_id pixcir_of_match[] = {
	{ .compatible = "pixcir,pixcir_ts", .data = &pixcir_ts_data },
	{ .compatible = "pixcir,pixcir_tangoc", .data = &pixcir_tangoc_data },
	{ }
};
MODULE_DEVICE_TABLE(of, pixcir_of_match);
#endif

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static struct i2c_driver pixcir_i2c_ts_driver = {
	.driver = {
		.owner	= THIS_MODULE,
		.name	= "pixcir_ts",
		.pm	= &pixcir_dev_pm_ops,
<<<<<<< HEAD
<<<<<<< HEAD
=======
		.of_match_table = of_match_ptr(pixcir_of_match),
>>>>>>> v3.18
=======
		.of_match_table = of_match_ptr(pixcir_of_match),
>>>>>>> v3.18
	},
	.probe		= pixcir_i2c_ts_probe,
	.remove		= pixcir_i2c_ts_remove,
	.id_table	= pixcir_i2c_ts_id,
};

module_i2c_driver(pixcir_i2c_ts_driver);

MODULE_AUTHOR("Jianchun Bian <jcbian@pixcir.com.cn>, Dequan Meng <dqmeng@pixcir.com.cn>");
MODULE_DESCRIPTION("Pixcir I2C Touchscreen Driver");
MODULE_LICENSE("GPL");
