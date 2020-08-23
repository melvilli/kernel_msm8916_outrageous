/*
 * ThingM blink(1) USB RGB LED driver
 *
<<<<<<< HEAD
 * Copyright 2013 Savoir-faire Linux Inc.
=======
 * Copyright 2013-2014 Savoir-faire Linux Inc.
>>>>>>> v3.18
 *	Vivien Didelot <vivien.didelot@savoirfairelinux.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2.
 */

#include <linux/hid.h>
<<<<<<< HEAD
#include <linux/leds.h>
#include <linux/module.h>

#include "hid-ids.h"

#define BLINK1_CMD_SIZE		9

#define blink1_rgb_to_r(rgb)	((rgb & 0xFF0000) >> 16)
#define blink1_rgb_to_g(rgb)	((rgb & 0x00FF00) >> 8)
#define blink1_rgb_to_b(rgb)	((rgb & 0x0000FF) >> 0)

/**
 * struct blink1_data - blink(1) device specific data
 * @hdev:		HID device.
 * @led_cdev:		LED class instance.
 * @rgb:		8-bit per channel RGB notation.
 * @fade:		fade time in hundredths of a second.
 * @brightness:		brightness coefficient.
 * @play:		play/pause in-memory patterns.
 */
struct blink1_data {
	struct hid_device *hdev;
	struct led_classdev led_cdev;
	u32 rgb;
	u16 fade;
	u8 brightness;
	bool play;
};

static int blink1_send_command(struct blink1_data *data,
		u8 buf[BLINK1_CMD_SIZE])
{
	int ret;

	hid_dbg(data->hdev, "command: %d%c%.2x%.2x%.2x%.2x%.2x%.2x%.2x\n",
			buf[0], buf[1], buf[2], buf[3], buf[4],
			buf[5], buf[6], buf[7], buf[8]);

	ret = data->hdev->hid_output_raw_report(data->hdev, buf,
			BLINK1_CMD_SIZE, HID_FEATURE_REPORT);
=======
#include <linux/hidraw.h>
#include <linux/leds.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>

#include "hid-ids.h"

#define REPORT_ID	1
#define REPORT_SIZE	9

/* Firmware major number of supported devices */
#define THINGM_MAJOR_MK1	'1'
#define THINGM_MAJOR_MK2	'2'

struct thingm_fwinfo {
	char major;
	unsigned numrgb;
	unsigned first;
};

static const struct thingm_fwinfo thingm_fwinfo[] = {
	{
		.major = THINGM_MAJOR_MK1,
		.numrgb = 1,
		.first = 0,
	}, {
		.major = THINGM_MAJOR_MK2,
		.numrgb = 2,
		.first = 1,
	}
};

/* A red, green or blue channel, part of an RGB chip */
struct thingm_led {
	struct thingm_rgb *rgb;
	struct led_classdev ldev;
	char name[32];
};

/* Basically a WS2812 5050 RGB LED chip */
struct thingm_rgb {
	struct thingm_device *tdev;
	struct thingm_led red;
	struct thingm_led green;
	struct thingm_led blue;
	struct work_struct work;
	u8 num;
};

struct thingm_device {
	struct hid_device *hdev;
	struct {
		char major;
		char minor;
	} version;
	const struct thingm_fwinfo *fwinfo;
	struct mutex lock;
	struct thingm_rgb *rgb;
};

static int thingm_send(struct thingm_device *tdev, u8 buf[REPORT_SIZE])
{
	int ret;

	hid_dbg(tdev->hdev, "-> %d %c %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx\n",
			buf[0], buf[1], buf[2], buf[3], buf[4],
			buf[5], buf[6], buf[7], buf[8]);

	ret = hid_hw_raw_request(tdev->hdev, buf[0], buf, REPORT_SIZE,
			HID_FEATURE_REPORT, HID_REQ_SET_REPORT);
>>>>>>> v3.18

	return ret < 0 ? ret : 0;
}

<<<<<<< HEAD
static int blink1_update_color(struct blink1_data *data)
{
	u8 buf[BLINK1_CMD_SIZE] = { 1, 'n', 0, 0, 0, 0, 0, 0, 0 };

	if (data->brightness) {
		unsigned int coef = DIV_ROUND_CLOSEST(255, data->brightness);

		buf[2] = DIV_ROUND_CLOSEST(blink1_rgb_to_r(data->rgb), coef);
		buf[3] = DIV_ROUND_CLOSEST(blink1_rgb_to_g(data->rgb), coef);
		buf[4] = DIV_ROUND_CLOSEST(blink1_rgb_to_b(data->rgb), coef);
	}

	if (data->fade) {
		buf[1] = 'c';
		buf[5] = (data->fade & 0xFF00) >> 8;
		buf[6] = (data->fade & 0x00FF);
	}

	return blink1_send_command(data, buf);
}

static void blink1_led_set(struct led_classdev *led_cdev,
		enum led_brightness brightness)
{
	struct blink1_data *data = dev_get_drvdata(led_cdev->dev->parent);

	data->brightness = brightness;
	if (blink1_update_color(data))
		hid_err(data->hdev, "failed to update color\n");
}

static enum led_brightness blink1_led_get(struct led_classdev *led_cdev)
{
	struct blink1_data *data = dev_get_drvdata(led_cdev->dev->parent);

	return data->brightness;
}

static ssize_t blink1_show_rgb(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct blink1_data *data = dev_get_drvdata(dev->parent);

	return sprintf(buf, "%.6X\n", data->rgb);
}

static ssize_t blink1_store_rgb(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct blink1_data *data = dev_get_drvdata(dev->parent);
	long unsigned int rgb;
	int ret;

	ret = kstrtoul(buf, 16, &rgb);
	if (ret)
		return ret;

	/* RGB triplet notation is 24-bit hexadecimal */
	if (rgb > 0xFFFFFF)
		return -EINVAL;

	data->rgb = rgb;
	ret = blink1_update_color(data);

	return ret ? ret : count;
}

static DEVICE_ATTR(rgb, S_IRUGO | S_IWUSR, blink1_show_rgb, blink1_store_rgb);

static ssize_t blink1_show_fade(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct blink1_data *data = dev_get_drvdata(dev->parent);

	return sprintf(buf, "%d\n", data->fade * 10);
}

static ssize_t blink1_store_fade(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct blink1_data *data = dev_get_drvdata(dev->parent);
	long unsigned int fade;
	int ret;

	ret = kstrtoul(buf, 10, &fade);
	if (ret)
		return ret;

	/* blink(1) accepts 16-bit fade time, number of 10ms ticks */
	fade = DIV_ROUND_CLOSEST(fade, 10);
	if (fade > 65535)
		return -EINVAL;

	data->fade = fade;

	return count;
}

static DEVICE_ATTR(fade, S_IRUGO | S_IWUSR,
		blink1_show_fade, blink1_store_fade);

static ssize_t blink1_show_play(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct blink1_data *data = dev_get_drvdata(dev->parent);

	return sprintf(buf, "%d\n", data->play);
}

static ssize_t blink1_store_play(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct blink1_data *data = dev_get_drvdata(dev->parent);
	u8 cmd[BLINK1_CMD_SIZE] = { 1, 'p', 0, 0, 0, 0, 0, 0, 0 };
	long unsigned int play;
	int ret;

	ret = kstrtoul(buf, 10, &play);
	if (ret)
		return ret;

	data->play = !!play;
	cmd[2] = data->play;
	ret = blink1_send_command(data, cmd);

	return ret ? ret : count;
}

static DEVICE_ATTR(play, S_IRUGO | S_IWUSR,
		blink1_show_play, blink1_store_play);

static const struct attribute_group blink1_sysfs_group = {
	.attrs = (struct attribute *[]) {
		&dev_attr_rgb.attr,
		&dev_attr_fade.attr,
		&dev_attr_play.attr,
		NULL
	},
};

static int thingm_probe(struct hid_device *hdev, const struct hid_device_id *id)
{
	struct blink1_data *data;
	struct led_classdev *led;
	char led_name[13];
	int ret;

	data = devm_kzalloc(&hdev->dev, sizeof(struct blink1_data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	hid_set_drvdata(hdev, data);
	data->hdev = hdev;
	data->rgb = 0xFFFFFF; /* set a default white color */

	ret = hid_parse(hdev);
	if (ret)
		goto error;

	ret = hid_hw_start(hdev, HID_CONNECT_HIDRAW);
	if (ret)
		goto error;

	/* blink(1) serial numbers range is 0x1A001000 to 0x1A002FFF */
	led = &data->led_cdev;
	snprintf(led_name, sizeof(led_name), "blink1::%s", hdev->uniq + 4);
	led->name = led_name;
	led->brightness_set = blink1_led_set;
	led->brightness_get = blink1_led_get;
	ret = led_classdev_register(&hdev->dev, led);
	if (ret)
		goto stop;

	ret = sysfs_create_group(&led->dev->kobj, &blink1_sysfs_group);
	if (ret)
		goto remove_led;

	return 0;

remove_led:
	led_classdev_unregister(led);
stop:
	hid_hw_stop(hdev);
error:
	return ret;
=======
static int thingm_recv(struct thingm_device *tdev, u8 buf[REPORT_SIZE])
{
	int ret;

	ret = hid_hw_raw_request(tdev->hdev, buf[0], buf, REPORT_SIZE,
			HID_FEATURE_REPORT, HID_REQ_GET_REPORT);
	if (ret < 0)
		return ret;

	hid_dbg(tdev->hdev, "<- %d %c %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx\n",
			buf[0], buf[1], buf[2], buf[3], buf[4],
			buf[5], buf[6], buf[7], buf[8]);

	return 0;
}

static int thingm_version(struct thingm_device *tdev)
{
	u8 buf[REPORT_SIZE] = { REPORT_ID, 'v', 0, 0, 0, 0, 0, 0, 0 };
	int err;

	err = thingm_send(tdev, buf);
	if (err)
		return err;

	err = thingm_recv(tdev, buf);
	if (err)
		return err;

	tdev->version.major = buf[3];
	tdev->version.minor = buf[4];

	return 0;
}

static int thingm_write_color(struct thingm_rgb *rgb)
{
	u8 buf[REPORT_SIZE] = { REPORT_ID, 'c', 0, 0, 0, 0, 0, rgb->num, 0 };

	buf[2] = rgb->red.ldev.brightness;
	buf[3] = rgb->green.ldev.brightness;
	buf[4] = rgb->blue.ldev.brightness;

	return thingm_send(rgb->tdev, buf);
}

static void thingm_work(struct work_struct *work)
{
	struct thingm_rgb *rgb = container_of(work, struct thingm_rgb, work);

	mutex_lock(&rgb->tdev->lock);

	if (thingm_write_color(rgb))
		hid_err(rgb->tdev->hdev, "failed to write color\n");

	mutex_unlock(&rgb->tdev->lock);
}

static void thingm_led_set(struct led_classdev *ldev,
		enum led_brightness brightness)
{
	struct thingm_led *led = container_of(ldev, struct thingm_led, ldev);

	/* the ledclass has already stored the brightness value */
	schedule_work(&led->rgb->work);
}

static int thingm_init_rgb(struct thingm_rgb *rgb)
{
	const int minor = ((struct hidraw *) rgb->tdev->hdev->hidraw)->minor;
	int err;

	/* Register the red diode */
	snprintf(rgb->red.name, sizeof(rgb->red.name),
			"thingm%d:red:led%d", minor, rgb->num);
	rgb->red.ldev.name = rgb->red.name;
	rgb->red.ldev.max_brightness = 255;
	rgb->red.ldev.brightness_set = thingm_led_set;
	rgb->red.rgb = rgb;

	err = led_classdev_register(&rgb->tdev->hdev->dev, &rgb->red.ldev);
	if (err)
		return err;

	/* Register the green diode */
	snprintf(rgb->green.name, sizeof(rgb->green.name),
			"thingm%d:green:led%d", minor, rgb->num);
	rgb->green.ldev.name = rgb->green.name;
	rgb->green.ldev.max_brightness = 255;
	rgb->green.ldev.brightness_set = thingm_led_set;
	rgb->green.rgb = rgb;

	err = led_classdev_register(&rgb->tdev->hdev->dev, &rgb->green.ldev);
	if (err)
		goto unregister_red;

	/* Register the blue diode */
	snprintf(rgb->blue.name, sizeof(rgb->blue.name),
			"thingm%d:blue:led%d", minor, rgb->num);
	rgb->blue.ldev.name = rgb->blue.name;
	rgb->blue.ldev.max_brightness = 255;
	rgb->blue.ldev.brightness_set = thingm_led_set;
	rgb->blue.rgb = rgb;

	err = led_classdev_register(&rgb->tdev->hdev->dev, &rgb->blue.ldev);
	if (err)
		goto unregister_green;

	INIT_WORK(&rgb->work, thingm_work);

	return 0;

unregister_green:
	led_classdev_unregister(&rgb->green.ldev);

unregister_red:
	led_classdev_unregister(&rgb->red.ldev);

	return err;
}

static void thingm_remove_rgb(struct thingm_rgb *rgb)
{
	led_classdev_unregister(&rgb->red.ldev);
	led_classdev_unregister(&rgb->green.ldev);
	led_classdev_unregister(&rgb->blue.ldev);
	flush_work(&rgb->work);
}

static int thingm_probe(struct hid_device *hdev, const struct hid_device_id *id)
{
	struct thingm_device *tdev;
	int i, err;

	tdev = devm_kzalloc(&hdev->dev, sizeof(struct thingm_device),
			GFP_KERNEL);
	if (!tdev)
		return -ENOMEM;

	tdev->hdev = hdev;
	hid_set_drvdata(hdev, tdev);

	err = hid_parse(hdev);
	if (err)
		goto error;

	err = hid_hw_start(hdev, HID_CONNECT_HIDRAW);
	if (err)
		goto error;

	mutex_init(&tdev->lock);

	err = thingm_version(tdev);
	if (err)
		goto stop;

	hid_dbg(hdev, "firmware version: %c.%c\n",
			tdev->version.major, tdev->version.minor);

	for (i = 0; i < ARRAY_SIZE(thingm_fwinfo) && !tdev->fwinfo; ++i)
		if (thingm_fwinfo[i].major == tdev->version.major)
			tdev->fwinfo = &thingm_fwinfo[i];

	if (!tdev->fwinfo) {
		hid_err(hdev, "unsupported firmware %c\n", tdev->version.major);
		err = -ENODEV;
		goto stop;
	}

	tdev->rgb = devm_kzalloc(&hdev->dev,
			sizeof(struct thingm_rgb) * tdev->fwinfo->numrgb,
			GFP_KERNEL);
	if (!tdev->rgb) {
		err = -ENOMEM;
		goto stop;
	}

	for (i = 0; i < tdev->fwinfo->numrgb; ++i) {
		struct thingm_rgb *rgb = tdev->rgb + i;

		rgb->tdev = tdev;
		rgb->num = tdev->fwinfo->first + i;
		err = thingm_init_rgb(rgb);
		if (err) {
			while (--i >= 0)
				thingm_remove_rgb(tdev->rgb + i);
			goto stop;
		}
	}

	return 0;
stop:
	hid_hw_stop(hdev);
error:
	return err;
>>>>>>> v3.18
}

static void thingm_remove(struct hid_device *hdev)
{
<<<<<<< HEAD
	struct blink1_data *data = hid_get_drvdata(hdev);
	struct led_classdev *led = &data->led_cdev;

	sysfs_remove_group(&led->dev->kobj, &blink1_sysfs_group);
	led_classdev_unregister(led);
	hid_hw_stop(hdev);
=======
	struct thingm_device *tdev = hid_get_drvdata(hdev);
	int i;

	hid_hw_stop(hdev);

	for (i = 0; i < tdev->fwinfo->numrgb; ++i)
		thingm_remove_rgb(tdev->rgb + i);
>>>>>>> v3.18
}

static const struct hid_device_id thingm_table[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_THINGM, USB_DEVICE_ID_BLINK1) },
	{ }
};
MODULE_DEVICE_TABLE(hid, thingm_table);

static struct hid_driver thingm_driver = {
	.name = "thingm",
	.probe = thingm_probe,
	.remove = thingm_remove,
	.id_table = thingm_table,
};

module_hid_driver(thingm_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vivien Didelot <vivien.didelot@savoirfairelinux.com>");
MODULE_DESCRIPTION("ThingM blink(1) USB RGB LED driver");
