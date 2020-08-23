/*
 * ChromeOS EC multi-function device
 *
 * Copyright (C) 2012 Google, Inc
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
 * The ChromeOS EC multi function device is used to mux all the requests
 * to the EC device for its multiple features: keyboard controller,
 * battery charging and regulator control, firmware update.
 */

#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/mfd/core.h>
#include <linux/mfd/cros_ec.h>
#include <linux/mfd/cros_ec_commands.h>
<<<<<<< HEAD
<<<<<<< HEAD

int cros_ec_prepare_tx(struct cros_ec_device *ec_dev,
		       struct cros_ec_msg *msg)
=======
=======
>>>>>>> v3.18
#include <linux/delay.h>

#define EC_COMMAND_RETRIES	50

int cros_ec_prepare_tx(struct cros_ec_device *ec_dev,
		       struct cros_ec_command *msg)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	uint8_t *out;
	int csum, i;

<<<<<<< HEAD
<<<<<<< HEAD
	BUG_ON(msg->out_len > EC_HOST_PARAM_SIZE);
	out = ec_dev->dout;
	out[0] = EC_CMD_VERSION0 + msg->version;
	out[1] = msg->cmd;
	out[2] = msg->out_len;
	csum = out[0] + out[1] + out[2];
	for (i = 0; i < msg->out_len; i++)
		csum += out[EC_MSG_TX_HEADER_BYTES + i] = msg->out_buf[i];
	out[EC_MSG_TX_HEADER_BYTES + msg->out_len] = (uint8_t)(csum & 0xff);

	return EC_MSG_TX_PROTO_BYTES + msg->out_len;
}
EXPORT_SYMBOL(cros_ec_prepare_tx);

static int cros_ec_command_sendrecv(struct cros_ec_device *ec_dev,
		uint16_t cmd, void *out_buf, int out_len,
		void *in_buf, int in_len)
{
	struct cros_ec_msg msg;

	msg.version = cmd >> 8;
	msg.cmd = cmd & 0xff;
	msg.out_buf = out_buf;
	msg.out_len = out_len;
	msg.in_buf = in_buf;
	msg.in_len = in_len;

	return ec_dev->command_xfer(ec_dev, &msg);
}

static int cros_ec_command_recv(struct cros_ec_device *ec_dev,
		uint16_t cmd, void *buf, int buf_len)
{
	return cros_ec_command_sendrecv(ec_dev, cmd, NULL, 0, buf, buf_len);
}

static int cros_ec_command_send(struct cros_ec_device *ec_dev,
		uint16_t cmd, void *buf, int buf_len)
{
	return cros_ec_command_sendrecv(ec_dev, cmd, buf, buf_len, NULL, 0);
}

static irqreturn_t ec_irq_thread(int irq, void *data)
{
	struct cros_ec_device *ec_dev = data;

	if (device_may_wakeup(ec_dev->dev))
		pm_wakeup_event(ec_dev->dev, 0);

	blocking_notifier_call_chain(&ec_dev->event_notifier, 1, ec_dev);

	return IRQ_HANDLED;
}

static struct mfd_cell cros_devs[] = {
=======
=======
>>>>>>> v3.18
	BUG_ON(msg->outsize > EC_PROTO2_MAX_PARAM_SIZE);
	out = ec_dev->dout;
	out[0] = EC_CMD_VERSION0 + msg->version;
	out[1] = msg->command;
	out[2] = msg->outsize;
	csum = out[0] + out[1] + out[2];
	for (i = 0; i < msg->outsize; i++)
		csum += out[EC_MSG_TX_HEADER_BYTES + i] = msg->outdata[i];
	out[EC_MSG_TX_HEADER_BYTES + msg->outsize] = (uint8_t)(csum & 0xff);

	return EC_MSG_TX_PROTO_BYTES + msg->outsize;
}
EXPORT_SYMBOL(cros_ec_prepare_tx);

int cros_ec_check_result(struct cros_ec_device *ec_dev,
			 struct cros_ec_command *msg)
{
	switch (msg->result) {
	case EC_RES_SUCCESS:
		return 0;
	case EC_RES_IN_PROGRESS:
		dev_dbg(ec_dev->dev, "command 0x%02x in progress\n",
			msg->command);
		return -EAGAIN;
	default:
		dev_dbg(ec_dev->dev, "command 0x%02x returned %d\n",
			msg->command, msg->result);
		return 0;
	}
}
EXPORT_SYMBOL(cros_ec_check_result);

int cros_ec_cmd_xfer(struct cros_ec_device *ec_dev,
		     struct cros_ec_command *msg)
{
	int ret;

	mutex_lock(&ec_dev->lock);
	ret = ec_dev->cmd_xfer(ec_dev, msg);
	if (msg->result == EC_RES_IN_PROGRESS) {
		int i;
		struct cros_ec_command status_msg;
		struct ec_response_get_comms_status status;

		status_msg.version = 0;
		status_msg.command = EC_CMD_GET_COMMS_STATUS;
		status_msg.outdata = NULL;
		status_msg.outsize = 0;
		status_msg.indata = (uint8_t *)&status;
		status_msg.insize = sizeof(status);

		/*
		 * Query the EC's status until it's no longer busy or
		 * we encounter an error.
		 */
		for (i = 0; i < EC_COMMAND_RETRIES; i++) {
			usleep_range(10000, 11000);

			ret = ec_dev->cmd_xfer(ec_dev, &status_msg);
			if (ret < 0)
				break;

			msg->result = status_msg.result;
			if (status_msg.result != EC_RES_SUCCESS)
				break;
			if (!(status.flags & EC_COMMS_STATUS_PROCESSING))
				break;
		}
	}
	mutex_unlock(&ec_dev->lock);

	return ret;
}
EXPORT_SYMBOL(cros_ec_cmd_xfer);

static const struct mfd_cell cros_devs[] = {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	{
		.name = "cros-ec-keyb",
		.id = 1,
		.of_compatible = "google,cros-ec-keyb",
	},
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	{
		.name = "cros-ec-i2c-tunnel",
		.id = 2,
		.of_compatible = "google,cros-ec-i2c-tunnel",
	},
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
};

int cros_ec_register(struct cros_ec_device *ec_dev)
{
	struct device *dev = ec_dev->dev;
	int err = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	BLOCKING_INIT_NOTIFIER_HEAD(&ec_dev->event_notifier);

	ec_dev->command_send = cros_ec_command_send;
	ec_dev->command_recv = cros_ec_command_recv;
	ec_dev->command_sendrecv = cros_ec_command_sendrecv;

	if (ec_dev->din_size) {
		ec_dev->din = kmalloc(ec_dev->din_size, GFP_KERNEL);
		if (!ec_dev->din) {
			err = -ENOMEM;
			goto fail_din;
		}
	}
	if (ec_dev->dout_size) {
		ec_dev->dout = kmalloc(ec_dev->dout_size, GFP_KERNEL);
		if (!ec_dev->dout) {
			err = -ENOMEM;
			goto fail_dout;
		}
	}

	if (!ec_dev->irq) {
		dev_dbg(dev, "no valid IRQ: %d\n", ec_dev->irq);
		goto fail_irq;
	}

	err = request_threaded_irq(ec_dev->irq, NULL, ec_irq_thread,
				   IRQF_TRIGGER_LOW | IRQF_ONESHOT,
				   "chromeos-ec", ec_dev);
	if (err) {
		dev_err(dev, "request irq %d: error %d\n", ec_dev->irq, err);
		goto fail_irq;
	}
=======
=======
>>>>>>> v3.18
	if (ec_dev->din_size) {
		ec_dev->din = devm_kzalloc(dev, ec_dev->din_size, GFP_KERNEL);
		if (!ec_dev->din)
			return -ENOMEM;
	}
	if (ec_dev->dout_size) {
		ec_dev->dout = devm_kzalloc(dev, ec_dev->dout_size, GFP_KERNEL);
		if (!ec_dev->dout)
			return -ENOMEM;
	}

	mutex_init(&ec_dev->lock);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	err = mfd_add_devices(dev, 0, cros_devs,
			      ARRAY_SIZE(cros_devs),
			      NULL, ec_dev->irq, NULL);
	if (err) {
		dev_err(dev, "failed to add mfd devices\n");
<<<<<<< HEAD
<<<<<<< HEAD
		goto fail_mfd;
	}

	dev_info(dev, "Chrome EC (%s)\n", ec_dev->name);

	return 0;

fail_mfd:
	free_irq(ec_dev->irq, ec_dev);
fail_irq:
	kfree(ec_dev->dout);
fail_dout:
	kfree(ec_dev->din);
fail_din:
	return err;
=======
=======
>>>>>>> v3.18
		return err;
	}

	dev_info(dev, "Chrome EC device registered\n");

	return 0;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}
EXPORT_SYMBOL(cros_ec_register);

int cros_ec_remove(struct cros_ec_device *ec_dev)
{
	mfd_remove_devices(ec_dev->dev);
<<<<<<< HEAD
<<<<<<< HEAD
	free_irq(ec_dev->irq, ec_dev);
	kfree(ec_dev->dout);
	kfree(ec_dev->din);
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return 0;
}
EXPORT_SYMBOL(cros_ec_remove);

#ifdef CONFIG_PM_SLEEP
int cros_ec_suspend(struct cros_ec_device *ec_dev)
{
	struct device *dev = ec_dev->dev;

	if (device_may_wakeup(dev))
		ec_dev->wake_enabled = !enable_irq_wake(ec_dev->irq);

	disable_irq(ec_dev->irq);
	ec_dev->was_wake_device = ec_dev->wake_enabled;

	return 0;
}
EXPORT_SYMBOL(cros_ec_suspend);

int cros_ec_resume(struct cros_ec_device *ec_dev)
{
	enable_irq(ec_dev->irq);

	if (ec_dev->wake_enabled) {
		disable_irq_wake(ec_dev->irq);
		ec_dev->wake_enabled = 0;
	}

	return 0;
}
EXPORT_SYMBOL(cros_ec_resume);

#endif
<<<<<<< HEAD
<<<<<<< HEAD
=======

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ChromeOS EC core driver");
>>>>>>> v3.18
=======

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ChromeOS EC core driver");
>>>>>>> v3.18
