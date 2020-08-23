/* drivers/rtc/rtc-s3c.c
 *
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * Copyright (c) 2004,2006 Simtec Electronics
 *	Ben Dooks, <ben@simtec.co.uk>
 *	http://armlinux.simtec.co.uk/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * S3C2410/S3C2440/S3C24XX Internal RTC Driver
*/

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/rtc.h>
#include <linux/bcd.h>
#include <linux/clk.h>
#include <linux/log2.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/uaccess.h>
#include <linux/io.h>

#include <asm/irq.h>
#include "rtc-s3c.h"

<<<<<<< HEAD
enum s3c_cpu_type {
	TYPE_S3C2410,
	TYPE_S3C2416,
	TYPE_S3C2443,
	TYPE_S3C64XX,
};

struct s3c_rtc_drv_data {
	int cpu_type;
};

/* I have yet to find an S3C implementation with more than one
 * of these rtc blocks in */

static struct clk *rtc_clk;
static void __iomem *s3c_rtc_base;
static int s3c_rtc_alarmno = NO_IRQ;
static int s3c_rtc_tickno  = NO_IRQ;
static enum s3c_cpu_type s3c_rtc_cpu_type;

static DEFINE_SPINLOCK(s3c_rtc_pie_lock);

static void s3c_rtc_alarm_clk_enable(bool enable)
{
	static DEFINE_SPINLOCK(s3c_rtc_alarm_clk_lock);
	static bool alarm_clk_enabled;
	unsigned long irq_flags;

	spin_lock_irqsave(&s3c_rtc_alarm_clk_lock, irq_flags);
	if (enable) {
		if (!alarm_clk_enabled) {
			clk_enable(rtc_clk);
			alarm_clk_enabled = true;
		}
	} else {
		if (alarm_clk_enabled) {
			clk_disable(rtc_clk);
			alarm_clk_enabled = false;
		}
	}
	spin_unlock_irqrestore(&s3c_rtc_alarm_clk_lock, irq_flags);
}

/* IRQ Handlers */

static irqreturn_t s3c_rtc_alarmirq(int irq, void *id)
{
	struct rtc_device *rdev = id;

	clk_enable(rtc_clk);
	rtc_update_irq(rdev, 1, RTC_AF | RTC_IRQF);

	if (s3c_rtc_cpu_type == TYPE_S3C64XX)
		writeb(S3C2410_INTP_ALM, s3c_rtc_base + S3C2410_INTP);

	clk_disable(rtc_clk);

	s3c_rtc_alarm_clk_enable(false);
=======
struct s3c_rtc {
	struct device *dev;
	struct rtc_device *rtc;

	void __iomem *base;
	struct clk *rtc_clk;
	struct clk *rtc_src_clk;
	bool enabled;

	struct s3c_rtc_data *data;

	int irq_alarm;
	int irq_tick;

	spinlock_t pie_lock;
	spinlock_t alarm_clk_lock;

	int ticnt_save, ticnt_en_save;
	bool wake_en;
};

struct s3c_rtc_data {
	int max_user_freq;
	bool needs_src_clk;

	void (*irq_handler) (struct s3c_rtc *info, int mask);
	void (*set_freq) (struct s3c_rtc *info, int freq);
	void (*enable_tick) (struct s3c_rtc *info, struct seq_file *seq);
	void (*select_tick_clk) (struct s3c_rtc *info);
	void (*save_tick_cnt) (struct s3c_rtc *info);
	void (*restore_tick_cnt) (struct s3c_rtc *info);
	void (*enable) (struct s3c_rtc *info);
	void (*disable) (struct s3c_rtc *info);
};

static void s3c_rtc_alarm_clk_enable(struct s3c_rtc *info, bool enable)
{
	unsigned long irq_flags;

	spin_lock_irqsave(&info->alarm_clk_lock, irq_flags);
	if (enable) {
		if (!info->enabled) {
			clk_enable(info->rtc_clk);
			if (info->data->needs_src_clk)
				clk_enable(info->rtc_src_clk);
			info->enabled = true;
		}
	} else {
		if (info->enabled) {
			if (info->data->needs_src_clk)
				clk_disable(info->rtc_src_clk);
			clk_disable(info->rtc_clk);
			info->enabled = false;
		}
	}
	spin_unlock_irqrestore(&info->alarm_clk_lock, irq_flags);
}

/* IRQ Handlers */
static irqreturn_t s3c_rtc_tickirq(int irq, void *id)
{
	struct s3c_rtc *info = (struct s3c_rtc *)id;

	if (info->data->irq_handler)
		info->data->irq_handler(info, S3C2410_INTP_TIC);
>>>>>>> v3.18

	return IRQ_HANDLED;
}

<<<<<<< HEAD
static irqreturn_t s3c_rtc_tickirq(int irq, void *id)
{
	struct rtc_device *rdev = id;

	clk_enable(rtc_clk);
	rtc_update_irq(rdev, 1, RTC_PF | RTC_IRQF);

	if (s3c_rtc_cpu_type == TYPE_S3C64XX)
		writeb(S3C2410_INTP_TIC, s3c_rtc_base + S3C2410_INTP);

	clk_disable(rtc_clk);
=======
static irqreturn_t s3c_rtc_alarmirq(int irq, void *id)
{
	struct s3c_rtc *info = (struct s3c_rtc *)id;

	if (info->data->irq_handler)
		info->data->irq_handler(info, S3C2410_INTP_ALM);

>>>>>>> v3.18
	return IRQ_HANDLED;
}

/* Update control registers */
static int s3c_rtc_setaie(struct device *dev, unsigned int enabled)
{
<<<<<<< HEAD
	unsigned int tmp;

	dev_dbg(dev, "%s: aie=%d\n", __func__, enabled);

	clk_enable(rtc_clk);
	tmp = readb(s3c_rtc_base + S3C2410_RTCALM) & ~S3C2410_RTCALM_ALMEN;
=======
	struct s3c_rtc *info = dev_get_drvdata(dev);
	unsigned int tmp;

	dev_dbg(info->dev, "%s: aie=%d\n", __func__, enabled);

	clk_enable(info->rtc_clk);
	if (info->data->needs_src_clk)
		clk_enable(info->rtc_src_clk);
	tmp = readb(info->base + S3C2410_RTCALM) & ~S3C2410_RTCALM_ALMEN;
>>>>>>> v3.18

	if (enabled)
		tmp |= S3C2410_RTCALM_ALMEN;

<<<<<<< HEAD
	writeb(tmp, s3c_rtc_base + S3C2410_RTCALM);
	clk_disable(rtc_clk);

	s3c_rtc_alarm_clk_enable(enabled);
=======
	writeb(tmp, info->base + S3C2410_RTCALM);
	if (info->data->needs_src_clk)
		clk_disable(info->rtc_src_clk);
	clk_disable(info->rtc_clk);

	s3c_rtc_alarm_clk_enable(info, enabled);
>>>>>>> v3.18

	return 0;
}

<<<<<<< HEAD
static int s3c_rtc_setfreq(struct device *dev, int freq)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct rtc_device *rtc_dev = platform_get_drvdata(pdev);
	unsigned int tmp = 0;
	int val;

	if (!is_power_of_2(freq))
		return -EINVAL;

	clk_enable(rtc_clk);
	spin_lock_irq(&s3c_rtc_pie_lock);

	if (s3c_rtc_cpu_type != TYPE_S3C64XX) {
		tmp = readb(s3c_rtc_base + S3C2410_TICNT);
		tmp &= S3C2410_TICNT_ENABLE;
	}

	val = (rtc_dev->max_user_freq / freq) - 1;

	if (s3c_rtc_cpu_type == TYPE_S3C2416 || s3c_rtc_cpu_type == TYPE_S3C2443) {
		tmp |= S3C2443_TICNT_PART(val);
		writel(S3C2443_TICNT1_PART(val), s3c_rtc_base + S3C2443_TICNT1);

		if (s3c_rtc_cpu_type == TYPE_S3C2416)
			writel(S3C2416_TICNT2_PART(val), s3c_rtc_base + S3C2416_TICNT2);
	} else {
		tmp |= val;
	}

	writel(tmp, s3c_rtc_base + S3C2410_TICNT);
	spin_unlock_irq(&s3c_rtc_pie_lock);
	clk_disable(rtc_clk);
=======
/* Set RTC frequency */
static int s3c_rtc_setfreq(struct s3c_rtc *info, int freq)
{
	if (!is_power_of_2(freq))
		return -EINVAL;

	clk_enable(info->rtc_clk);
	if (info->data->needs_src_clk)
		clk_enable(info->rtc_src_clk);
	spin_lock_irq(&info->pie_lock);

	if (info->data->set_freq)
		info->data->set_freq(info, freq);

	spin_unlock_irq(&info->pie_lock);
	if (info->data->needs_src_clk)
		clk_disable(info->rtc_src_clk);
	clk_disable(info->rtc_clk);
>>>>>>> v3.18

	return 0;
}

/* Time read/write */
<<<<<<< HEAD

static int s3c_rtc_gettime(struct device *dev, struct rtc_time *rtc_tm)
{
	unsigned int have_retried = 0;
	void __iomem *base = s3c_rtc_base;

	clk_enable(rtc_clk);
 retry_get_time:
	rtc_tm->tm_min  = readb(base + S3C2410_RTCMIN);
	rtc_tm->tm_hour = readb(base + S3C2410_RTCHOUR);
	rtc_tm->tm_mday = readb(base + S3C2410_RTCDATE);
	rtc_tm->tm_mon  = readb(base + S3C2410_RTCMON);
	rtc_tm->tm_year = readb(base + S3C2410_RTCYEAR);
	rtc_tm->tm_sec  = readb(base + S3C2410_RTCSEC);
=======
static int s3c_rtc_gettime(struct device *dev, struct rtc_time *rtc_tm)
{
	struct s3c_rtc *info = dev_get_drvdata(dev);
	unsigned int have_retried = 0;

	clk_enable(info->rtc_clk);
	if (info->data->needs_src_clk)
		clk_enable(info->rtc_src_clk);

 retry_get_time:
	rtc_tm->tm_min  = readb(info->base + S3C2410_RTCMIN);
	rtc_tm->tm_hour = readb(info->base + S3C2410_RTCHOUR);
	rtc_tm->tm_mday = readb(info->base + S3C2410_RTCDATE);
	rtc_tm->tm_mon  = readb(info->base + S3C2410_RTCMON);
	rtc_tm->tm_year = readb(info->base + S3C2410_RTCYEAR);
	rtc_tm->tm_sec  = readb(info->base + S3C2410_RTCSEC);
>>>>>>> v3.18

	/* the only way to work out whether the system was mid-update
	 * when we read it is to check the second counter, and if it
	 * is zero, then we re-try the entire read
	 */

	if (rtc_tm->tm_sec == 0 && !have_retried) {
		have_retried = 1;
		goto retry_get_time;
	}

	rtc_tm->tm_sec = bcd2bin(rtc_tm->tm_sec);
	rtc_tm->tm_min = bcd2bin(rtc_tm->tm_min);
	rtc_tm->tm_hour = bcd2bin(rtc_tm->tm_hour);
	rtc_tm->tm_mday = bcd2bin(rtc_tm->tm_mday);
	rtc_tm->tm_mon = bcd2bin(rtc_tm->tm_mon);
	rtc_tm->tm_year = bcd2bin(rtc_tm->tm_year);

	rtc_tm->tm_year += 100;

	dev_dbg(dev, "read time %04d.%02d.%02d %02d:%02d:%02d\n",
		 1900 + rtc_tm->tm_year, rtc_tm->tm_mon, rtc_tm->tm_mday,
		 rtc_tm->tm_hour, rtc_tm->tm_min, rtc_tm->tm_sec);

	rtc_tm->tm_mon -= 1;

<<<<<<< HEAD
	clk_disable(rtc_clk);
=======
	if (info->data->needs_src_clk)
		clk_disable(info->rtc_src_clk);
	clk_disable(info->rtc_clk);

>>>>>>> v3.18
	return rtc_valid_tm(rtc_tm);
}

static int s3c_rtc_settime(struct device *dev, struct rtc_time *tm)
{
<<<<<<< HEAD
	void __iomem *base = s3c_rtc_base;
=======
	struct s3c_rtc *info = dev_get_drvdata(dev);
>>>>>>> v3.18
	int year = tm->tm_year - 100;

	dev_dbg(dev, "set time %04d.%02d.%02d %02d:%02d:%02d\n",
		 1900 + tm->tm_year, tm->tm_mon, tm->tm_mday,
		 tm->tm_hour, tm->tm_min, tm->tm_sec);

	/* we get around y2k by simply not supporting it */

	if (year < 0 || year >= 100) {
		dev_err(dev, "rtc only supports 100 years\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	clk_enable(rtc_clk);
	writeb(bin2bcd(tm->tm_sec),  base + S3C2410_RTCSEC);
	writeb(bin2bcd(tm->tm_min),  base + S3C2410_RTCMIN);
	writeb(bin2bcd(tm->tm_hour), base + S3C2410_RTCHOUR);
	writeb(bin2bcd(tm->tm_mday), base + S3C2410_RTCDATE);
	writeb(bin2bcd(tm->tm_mon + 1), base + S3C2410_RTCMON);
	writeb(bin2bcd(year), base + S3C2410_RTCYEAR);
	clk_disable(rtc_clk);
=======
	clk_enable(info->rtc_clk);
	if (info->data->needs_src_clk)
		clk_enable(info->rtc_src_clk);

	writeb(bin2bcd(tm->tm_sec),  info->base + S3C2410_RTCSEC);
	writeb(bin2bcd(tm->tm_min),  info->base + S3C2410_RTCMIN);
	writeb(bin2bcd(tm->tm_hour), info->base + S3C2410_RTCHOUR);
	writeb(bin2bcd(tm->tm_mday), info->base + S3C2410_RTCDATE);
	writeb(bin2bcd(tm->tm_mon + 1), info->base + S3C2410_RTCMON);
	writeb(bin2bcd(year), info->base + S3C2410_RTCYEAR);

	if (info->data->needs_src_clk)
		clk_disable(info->rtc_src_clk);
	clk_disable(info->rtc_clk);
>>>>>>> v3.18

	return 0;
}

static int s3c_rtc_getalarm(struct device *dev, struct rtc_wkalrm *alrm)
{
<<<<<<< HEAD
	struct rtc_time *alm_tm = &alrm->time;
	void __iomem *base = s3c_rtc_base;
	unsigned int alm_en;

	clk_enable(rtc_clk);
	alm_tm->tm_sec  = readb(base + S3C2410_ALMSEC);
	alm_tm->tm_min  = readb(base + S3C2410_ALMMIN);
	alm_tm->tm_hour = readb(base + S3C2410_ALMHOUR);
	alm_tm->tm_mon  = readb(base + S3C2410_ALMMON);
	alm_tm->tm_mday = readb(base + S3C2410_ALMDATE);
	alm_tm->tm_year = readb(base + S3C2410_ALMYEAR);

	alm_en = readb(base + S3C2410_RTCALM);
=======
	struct s3c_rtc *info = dev_get_drvdata(dev);
	struct rtc_time *alm_tm = &alrm->time;
	unsigned int alm_en;

	clk_enable(info->rtc_clk);
	if (info->data->needs_src_clk)
		clk_enable(info->rtc_src_clk);

	alm_tm->tm_sec  = readb(info->base + S3C2410_ALMSEC);
	alm_tm->tm_min  = readb(info->base + S3C2410_ALMMIN);
	alm_tm->tm_hour = readb(info->base + S3C2410_ALMHOUR);
	alm_tm->tm_mon  = readb(info->base + S3C2410_ALMMON);
	alm_tm->tm_mday = readb(info->base + S3C2410_ALMDATE);
	alm_tm->tm_year = readb(info->base + S3C2410_ALMYEAR);

	alm_en = readb(info->base + S3C2410_RTCALM);
>>>>>>> v3.18

	alrm->enabled = (alm_en & S3C2410_RTCALM_ALMEN) ? 1 : 0;

	dev_dbg(dev, "read alarm %d, %04d.%02d.%02d %02d:%02d:%02d\n",
		 alm_en,
		 1900 + alm_tm->tm_year, alm_tm->tm_mon, alm_tm->tm_mday,
		 alm_tm->tm_hour, alm_tm->tm_min, alm_tm->tm_sec);


	/* decode the alarm enable field */

	if (alm_en & S3C2410_RTCALM_SECEN)
		alm_tm->tm_sec = bcd2bin(alm_tm->tm_sec);
	else
		alm_tm->tm_sec = -1;

	if (alm_en & S3C2410_RTCALM_MINEN)
		alm_tm->tm_min = bcd2bin(alm_tm->tm_min);
	else
		alm_tm->tm_min = -1;

	if (alm_en & S3C2410_RTCALM_HOUREN)
		alm_tm->tm_hour = bcd2bin(alm_tm->tm_hour);
	else
		alm_tm->tm_hour = -1;

	if (alm_en & S3C2410_RTCALM_DAYEN)
		alm_tm->tm_mday = bcd2bin(alm_tm->tm_mday);
	else
		alm_tm->tm_mday = -1;

	if (alm_en & S3C2410_RTCALM_MONEN) {
		alm_tm->tm_mon = bcd2bin(alm_tm->tm_mon);
		alm_tm->tm_mon -= 1;
	} else {
		alm_tm->tm_mon = -1;
	}

	if (alm_en & S3C2410_RTCALM_YEAREN)
		alm_tm->tm_year = bcd2bin(alm_tm->tm_year);
	else
		alm_tm->tm_year = -1;

<<<<<<< HEAD
	clk_disable(rtc_clk);
=======
	if (info->data->needs_src_clk)
		clk_disable(info->rtc_src_clk);
	clk_disable(info->rtc_clk);

>>>>>>> v3.18
	return 0;
}

static int s3c_rtc_setalarm(struct device *dev, struct rtc_wkalrm *alrm)
{
<<<<<<< HEAD
	struct rtc_time *tm = &alrm->time;
	void __iomem *base = s3c_rtc_base;
	unsigned int alrm_en;

	clk_enable(rtc_clk);
=======
	struct s3c_rtc *info = dev_get_drvdata(dev);
	struct rtc_time *tm = &alrm->time;
	unsigned int alrm_en;

	clk_enable(info->rtc_clk);
	if (info->data->needs_src_clk)
		clk_enable(info->rtc_src_clk);

>>>>>>> v3.18
	dev_dbg(dev, "s3c_rtc_setalarm: %d, %04d.%02d.%02d %02d:%02d:%02d\n",
		 alrm->enabled,
		 1900 + tm->tm_year, tm->tm_mon + 1, tm->tm_mday,
		 tm->tm_hour, tm->tm_min, tm->tm_sec);

<<<<<<< HEAD
	alrm_en = readb(base + S3C2410_RTCALM) & S3C2410_RTCALM_ALMEN;
	writeb(0x00, base + S3C2410_RTCALM);

	if (tm->tm_sec < 60 && tm->tm_sec >= 0) {
		alrm_en |= S3C2410_RTCALM_SECEN;
		writeb(bin2bcd(tm->tm_sec), base + S3C2410_ALMSEC);
=======
	alrm_en = readb(info->base + S3C2410_RTCALM) & S3C2410_RTCALM_ALMEN;
	writeb(0x00, info->base + S3C2410_RTCALM);

	if (tm->tm_sec < 60 && tm->tm_sec >= 0) {
		alrm_en |= S3C2410_RTCALM_SECEN;
		writeb(bin2bcd(tm->tm_sec), info->base + S3C2410_ALMSEC);
>>>>>>> v3.18
	}

	if (tm->tm_min < 60 && tm->tm_min >= 0) {
		alrm_en |= S3C2410_RTCALM_MINEN;
<<<<<<< HEAD
		writeb(bin2bcd(tm->tm_min), base + S3C2410_ALMMIN);
=======
		writeb(bin2bcd(tm->tm_min), info->base + S3C2410_ALMMIN);
>>>>>>> v3.18
	}

	if (tm->tm_hour < 24 && tm->tm_hour >= 0) {
		alrm_en |= S3C2410_RTCALM_HOUREN;
<<<<<<< HEAD
		writeb(bin2bcd(tm->tm_hour), base + S3C2410_ALMHOUR);
=======
		writeb(bin2bcd(tm->tm_hour), info->base + S3C2410_ALMHOUR);
>>>>>>> v3.18
	}

	dev_dbg(dev, "setting S3C2410_RTCALM to %08x\n", alrm_en);

<<<<<<< HEAD
	writeb(alrm_en, base + S3C2410_RTCALM);

	s3c_rtc_setaie(dev, alrm->enabled);

	clk_disable(rtc_clk);
=======
	writeb(alrm_en, info->base + S3C2410_RTCALM);

	s3c_rtc_setaie(dev, alrm->enabled);

	if (info->data->needs_src_clk)
		clk_disable(info->rtc_src_clk);
	clk_disable(info->rtc_clk);

>>>>>>> v3.18
	return 0;
}

static int s3c_rtc_proc(struct device *dev, struct seq_file *seq)
{
<<<<<<< HEAD
	unsigned int ticnt;

	clk_enable(rtc_clk);
	if (s3c_rtc_cpu_type == TYPE_S3C64XX) {
		ticnt = readw(s3c_rtc_base + S3C2410_RTCCON);
		ticnt &= S3C64XX_RTCCON_TICEN;
	} else {
		ticnt = readb(s3c_rtc_base + S3C2410_TICNT);
		ticnt &= S3C2410_TICNT_ENABLE;
	}

	seq_printf(seq, "periodic_IRQ\t: %s\n", ticnt  ? "yes" : "no");
	clk_disable(rtc_clk);
=======
	struct s3c_rtc *info = dev_get_drvdata(dev);

	clk_enable(info->rtc_clk);
	if (info->data->needs_src_clk)
		clk_enable(info->rtc_src_clk);

	if (info->data->enable_tick)
		info->data->enable_tick(info, seq);

	if (info->data->needs_src_clk)
		clk_disable(info->rtc_src_clk);
	clk_disable(info->rtc_clk);

>>>>>>> v3.18
	return 0;
}

static const struct rtc_class_ops s3c_rtcops = {
	.read_time	= s3c_rtc_gettime,
	.set_time	= s3c_rtc_settime,
	.read_alarm	= s3c_rtc_getalarm,
	.set_alarm	= s3c_rtc_setalarm,
	.proc		= s3c_rtc_proc,
	.alarm_irq_enable = s3c_rtc_setaie,
};

<<<<<<< HEAD
static void s3c_rtc_enable(struct platform_device *pdev, int en)
{
	void __iomem *base = s3c_rtc_base;
	unsigned int tmp;

	if (s3c_rtc_base == NULL)
		return;

	clk_enable(rtc_clk);
	if (!en) {
		tmp = readw(base + S3C2410_RTCCON);
		if (s3c_rtc_cpu_type == TYPE_S3C64XX)
			tmp &= ~S3C64XX_RTCCON_TICEN;
		tmp &= ~S3C2410_RTCCON_RTCEN;
		writew(tmp, base + S3C2410_RTCCON);

		if (s3c_rtc_cpu_type != TYPE_S3C64XX) {
			tmp = readb(base + S3C2410_TICNT);
			tmp &= ~S3C2410_TICNT_ENABLE;
			writeb(tmp, base + S3C2410_TICNT);
		}
	} else {
		/* re-enable the device, and check it is ok */

		if ((readw(base+S3C2410_RTCCON) & S3C2410_RTCCON_RTCEN) == 0) {
			dev_info(&pdev->dev, "rtc disabled, re-enabling\n");

			tmp = readw(base + S3C2410_RTCCON);
			writew(tmp | S3C2410_RTCCON_RTCEN,
				base + S3C2410_RTCCON);
		}

		if ((readw(base + S3C2410_RTCCON) & S3C2410_RTCCON_CNTSEL)) {
			dev_info(&pdev->dev, "removing RTCCON_CNTSEL\n");

			tmp = readw(base + S3C2410_RTCCON);
			writew(tmp & ~S3C2410_RTCCON_CNTSEL,
				base + S3C2410_RTCCON);
		}

		if ((readw(base + S3C2410_RTCCON) & S3C2410_RTCCON_CLKRST)) {
			dev_info(&pdev->dev, "removing RTCCON_CLKRST\n");

			tmp = readw(base + S3C2410_RTCCON);
			writew(tmp & ~S3C2410_RTCCON_CLKRST,
				base + S3C2410_RTCCON);
		}
	}
	clk_disable(rtc_clk);
}

static int s3c_rtc_remove(struct platform_device *dev)
{
	platform_set_drvdata(dev, NULL);

	s3c_rtc_setaie(&dev->dev, 0);

	clk_unprepare(rtc_clk);
	rtc_clk = NULL;
=======
static void s3c24xx_rtc_enable(struct s3c_rtc *info)
{
	unsigned int con, tmp;

	clk_enable(info->rtc_clk);
	if (info->data->needs_src_clk)
		clk_enable(info->rtc_src_clk);

	con = readw(info->base + S3C2410_RTCCON);
	/* re-enable the device, and check it is ok */
	if ((con & S3C2410_RTCCON_RTCEN) == 0) {
		dev_info(info->dev, "rtc disabled, re-enabling\n");

		tmp = readw(info->base + S3C2410_RTCCON);
		writew(tmp | S3C2410_RTCCON_RTCEN,
			info->base + S3C2410_RTCCON);
	}

	if (con & S3C2410_RTCCON_CNTSEL) {
		dev_info(info->dev, "removing RTCCON_CNTSEL\n");

		tmp = readw(info->base + S3C2410_RTCCON);
		writew(tmp & ~S3C2410_RTCCON_CNTSEL,
			info->base + S3C2410_RTCCON);
	}

	if (con & S3C2410_RTCCON_CLKRST) {
		dev_info(info->dev, "removing RTCCON_CLKRST\n");

		tmp = readw(info->base + S3C2410_RTCCON);
		writew(tmp & ~S3C2410_RTCCON_CLKRST,
			info->base + S3C2410_RTCCON);
	}

	if (info->data->needs_src_clk)
		clk_disable(info->rtc_src_clk);
	clk_disable(info->rtc_clk);
}

static void s3c24xx_rtc_disable(struct s3c_rtc *info)
{
	unsigned int con;

	clk_enable(info->rtc_clk);
	if (info->data->needs_src_clk)
		clk_enable(info->rtc_src_clk);

	con = readw(info->base + S3C2410_RTCCON);
	con &= ~S3C2410_RTCCON_RTCEN;
	writew(con, info->base + S3C2410_RTCCON);

	con = readb(info->base + S3C2410_TICNT);
	con &= ~S3C2410_TICNT_ENABLE;
	writeb(con, info->base + S3C2410_TICNT);

	if (info->data->needs_src_clk)
		clk_disable(info->rtc_src_clk);
	clk_disable(info->rtc_clk);
}

static void s3c6410_rtc_disable(struct s3c_rtc *info)
{
	unsigned int con;

	clk_enable(info->rtc_clk);
	if (info->data->needs_src_clk)
		clk_enable(info->rtc_src_clk);

	con = readw(info->base + S3C2410_RTCCON);
	con &= ~S3C64XX_RTCCON_TICEN;
	con &= ~S3C2410_RTCCON_RTCEN;
	writew(con, info->base + S3C2410_RTCCON);

	if (info->data->needs_src_clk)
		clk_disable(info->rtc_src_clk);
	clk_disable(info->rtc_clk);
}

static int s3c_rtc_remove(struct platform_device *pdev)
{
	struct s3c_rtc *info = platform_get_drvdata(pdev);

	s3c_rtc_setaie(info->dev, 0);

	clk_unprepare(info->rtc_clk);
	info->rtc_clk = NULL;
>>>>>>> v3.18

	return 0;
}

static const struct of_device_id s3c_rtc_dt_match[];

<<<<<<< HEAD
static inline int s3c_rtc_get_driver_data(struct platform_device *pdev)
{
#ifdef CONFIG_OF
	struct s3c_rtc_drv_data *data;
	if (pdev->dev.of_node) {
		const struct of_device_id *match;
		match = of_match_node(s3c_rtc_dt_match, pdev->dev.of_node);
		data = (struct s3c_rtc_drv_data *) match->data;
		return data->cpu_type;
	}
#endif
	return platform_get_device_id(pdev)->driver_data;
=======
static struct s3c_rtc_data *s3c_rtc_get_data(struct platform_device *pdev)
{
	const struct of_device_id *match;

	match = of_match_node(s3c_rtc_dt_match, pdev->dev.of_node);
	return (struct s3c_rtc_data *)match->data;
>>>>>>> v3.18
}

static int s3c_rtc_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct rtc_device *rtc;
	struct rtc_time rtc_tm;
	struct resource *res;
	int ret;
	int tmp;

	dev_dbg(&pdev->dev, "%s: probe=%p\n", __func__, pdev);

	/* find the IRQs */

	s3c_rtc_tickno = platform_get_irq(pdev, 1);
	if (s3c_rtc_tickno < 0) {
		dev_err(&pdev->dev, "no irq for rtc tick\n");
		return s3c_rtc_tickno;
	}

	s3c_rtc_alarmno = platform_get_irq(pdev, 0);
	if (s3c_rtc_alarmno < 0) {
		dev_err(&pdev->dev, "no irq for alarm\n");
		return s3c_rtc_alarmno;
	}

	dev_dbg(&pdev->dev, "s3c2410_rtc: tick irq %d, alarm irq %d\n",
		 s3c_rtc_tickno, s3c_rtc_alarmno);

	/* get the memory region */

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	s3c_rtc_base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(s3c_rtc_base))
		return PTR_ERR(s3c_rtc_base);

	rtc_clk = devm_clk_get(&pdev->dev, "rtc");
	if (IS_ERR(rtc_clk)) {
		dev_err(&pdev->dev, "failed to find rtc clock source\n");
		ret = PTR_ERR(rtc_clk);
		rtc_clk = NULL;
		return ret;
	}

	clk_prepare_enable(rtc_clk);

	/* check to see if everything is setup correctly */

	s3c_rtc_enable(pdev, 1);

	dev_dbg(&pdev->dev, "s3c2410_rtc: RTCCON=%02x\n",
		 readw(s3c_rtc_base + S3C2410_RTCCON));
=======
	struct s3c_rtc *info = NULL;
	struct rtc_time rtc_tm;
	struct resource *res;
	int ret;

	info = devm_kzalloc(&pdev->dev, sizeof(*info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	/* find the IRQs */
	info->irq_tick = platform_get_irq(pdev, 1);
	if (info->irq_tick < 0) {
		dev_err(&pdev->dev, "no irq for rtc tick\n");
		return info->irq_tick;
	}

	info->dev = &pdev->dev;
	info->data = s3c_rtc_get_data(pdev);
	if (!info->data) {
		dev_err(&pdev->dev, "failed getting s3c_rtc_data\n");
		return -EINVAL;
	}
	spin_lock_init(&info->pie_lock);
	spin_lock_init(&info->alarm_clk_lock);

	platform_set_drvdata(pdev, info);

	info->irq_alarm = platform_get_irq(pdev, 0);
	if (info->irq_alarm < 0) {
		dev_err(&pdev->dev, "no irq for alarm\n");
		return info->irq_alarm;
	}

	dev_dbg(&pdev->dev, "s3c2410_rtc: tick irq %d, alarm irq %d\n",
		 info->irq_tick, info->irq_alarm);

	/* get the memory region */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	info->base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(info->base))
		return PTR_ERR(info->base);

	info->rtc_clk = devm_clk_get(&pdev->dev, "rtc");
	if (IS_ERR(info->rtc_clk)) {
		dev_err(&pdev->dev, "failed to find rtc clock\n");
		return PTR_ERR(info->rtc_clk);
	}
	clk_prepare_enable(info->rtc_clk);

	if (info->data->needs_src_clk) {
		info->rtc_src_clk = devm_clk_get(&pdev->dev, "rtc_src");
		if (IS_ERR(info->rtc_src_clk)) {
			dev_err(&pdev->dev,
				"failed to find rtc source clock\n");
			return PTR_ERR(info->rtc_src_clk);
		}
		clk_prepare_enable(info->rtc_src_clk);
	}

	/* check to see if everything is setup correctly */
	if (info->data->enable)
		info->data->enable(info);

	dev_dbg(&pdev->dev, "s3c2410_rtc: RTCCON=%02x\n",
		 readw(info->base + S3C2410_RTCCON));
>>>>>>> v3.18

	device_init_wakeup(&pdev->dev, 1);

	/* register RTC and exit */
<<<<<<< HEAD

	rtc = devm_rtc_device_register(&pdev->dev, "s3c", &s3c_rtcops,
				  THIS_MODULE);

	if (IS_ERR(rtc)) {
		dev_err(&pdev->dev, "cannot attach rtc\n");
		ret = PTR_ERR(rtc);
		goto err_nortc;
	}

	s3c_rtc_cpu_type = s3c_rtc_get_driver_data(pdev);

	/* Check RTC Time */

	s3c_rtc_gettime(NULL, &rtc_tm);
=======
	info->rtc = devm_rtc_device_register(&pdev->dev, "s3c", &s3c_rtcops,
				  THIS_MODULE);
	if (IS_ERR(info->rtc)) {
		dev_err(&pdev->dev, "cannot attach rtc\n");
		ret = PTR_ERR(info->rtc);
		goto err_nortc;
	}

	ret = devm_request_irq(&pdev->dev, info->irq_alarm, s3c_rtc_alarmirq,
			  0,  "s3c2410-rtc alarm", info);
	if (ret) {
		dev_err(&pdev->dev, "IRQ%d error %d\n", info->irq_alarm, ret);
		goto err_nortc;
	}

	ret = devm_request_irq(&pdev->dev, info->irq_tick, s3c_rtc_tickirq,
			  0,  "s3c2410-rtc tick", info);
	if (ret) {
		dev_err(&pdev->dev, "IRQ%d error %d\n", info->irq_tick, ret);
		goto err_nortc;
	}

	/* Check RTC Time */
	s3c_rtc_gettime(&pdev->dev, &rtc_tm);
>>>>>>> v3.18

	if (rtc_valid_tm(&rtc_tm)) {
		rtc_tm.tm_year	= 100;
		rtc_tm.tm_mon	= 0;
		rtc_tm.tm_mday	= 1;
		rtc_tm.tm_hour	= 0;
		rtc_tm.tm_min	= 0;
		rtc_tm.tm_sec	= 0;

<<<<<<< HEAD
		s3c_rtc_settime(NULL, &rtc_tm);
=======
		s3c_rtc_settime(&pdev->dev, &rtc_tm);
>>>>>>> v3.18

		dev_warn(&pdev->dev, "warning: invalid RTC value so initializing it\n");
	}

<<<<<<< HEAD
	if (s3c_rtc_cpu_type != TYPE_S3C2410)
		rtc->max_user_freq = 32768;
	else
		rtc->max_user_freq = 128;

	if (s3c_rtc_cpu_type == TYPE_S3C2416 || s3c_rtc_cpu_type == TYPE_S3C2443) {
		tmp = readw(s3c_rtc_base + S3C2410_RTCCON);
		tmp |= S3C2443_RTCCON_TICSEL;
		writew(tmp, s3c_rtc_base + S3C2410_RTCCON);
	}

	platform_set_drvdata(pdev, rtc);

	s3c_rtc_setfreq(&pdev->dev, 1);

	ret = devm_request_irq(&pdev->dev, s3c_rtc_alarmno, s3c_rtc_alarmirq,
			  0,  "s3c2410-rtc alarm", rtc);
	if (ret) {
		dev_err(&pdev->dev, "IRQ%d error %d\n", s3c_rtc_alarmno, ret);
		goto err_alarm_irq;
	}

	ret = devm_request_irq(&pdev->dev, s3c_rtc_tickno, s3c_rtc_tickirq,
			  0,  "s3c2410-rtc tick", rtc);
	if (ret) {
		dev_err(&pdev->dev, "IRQ%d error %d\n", s3c_rtc_tickno, ret);
		goto err_alarm_irq;
	}

	clk_disable(rtc_clk);

	return 0;

 err_alarm_irq:
	platform_set_drvdata(pdev, NULL);

 err_nortc:
	s3c_rtc_enable(pdev, 0);
	clk_disable_unprepare(rtc_clk);
=======
	if (info->data->select_tick_clk)
		info->data->select_tick_clk(info);

	s3c_rtc_setfreq(info, 1);

	if (info->data->needs_src_clk)
		clk_disable(info->rtc_src_clk);
	clk_disable(info->rtc_clk);

	return 0;

 err_nortc:
	if (info->data->disable)
		info->data->disable(info);
	clk_disable_unprepare(info->rtc_clk);
>>>>>>> v3.18

	return ret;
}

#ifdef CONFIG_PM_SLEEP
<<<<<<< HEAD
/* RTC Power management control */

static int ticnt_save, ticnt_en_save;
static bool wake_en;

static int s3c_rtc_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);

	clk_enable(rtc_clk);
	/* save TICNT for anyone using periodic interrupts */
	ticnt_save = readb(s3c_rtc_base + S3C2410_TICNT);
	if (s3c_rtc_cpu_type == TYPE_S3C64XX) {
		ticnt_en_save = readw(s3c_rtc_base + S3C2410_RTCCON);
		ticnt_en_save &= S3C64XX_RTCCON_TICEN;
	}
	s3c_rtc_enable(pdev, 0);

	if (device_may_wakeup(dev) && !wake_en) {
		if (enable_irq_wake(s3c_rtc_alarmno) == 0)
			wake_en = true;
		else
			dev_err(dev, "enable_irq_wake failed\n");
	}
	clk_disable(rtc_clk);
=======

static int s3c_rtc_suspend(struct device *dev)
{
	struct s3c_rtc *info = dev_get_drvdata(dev);

	clk_enable(info->rtc_clk);
	if (info->data->needs_src_clk)
		clk_enable(info->rtc_src_clk);

	/* save TICNT for anyone using periodic interrupts */
	if (info->data->save_tick_cnt)
		info->data->save_tick_cnt(info);

	if (info->data->disable)
		info->data->disable(info);

	if (device_may_wakeup(dev) && !info->wake_en) {
		if (enable_irq_wake(info->irq_alarm) == 0)
			info->wake_en = true;
		else
			dev_err(dev, "enable_irq_wake failed\n");
	}

	if (info->data->needs_src_clk)
		clk_disable(info->rtc_src_clk);
	clk_disable(info->rtc_clk);
>>>>>>> v3.18

	return 0;
}

static int s3c_rtc_resume(struct device *dev)
{
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	unsigned int tmp;

	clk_enable(rtc_clk);
	s3c_rtc_enable(pdev, 1);
	writeb(ticnt_save, s3c_rtc_base + S3C2410_TICNT);
	if (s3c_rtc_cpu_type == TYPE_S3C64XX && ticnt_en_save) {
		tmp = readw(s3c_rtc_base + S3C2410_RTCCON);
		writew(tmp | ticnt_en_save, s3c_rtc_base + S3C2410_RTCCON);
	}

	if (device_may_wakeup(dev) && wake_en) {
		disable_irq_wake(s3c_rtc_alarmno);
		wake_en = false;
	}
	clk_disable(rtc_clk);
=======
	struct s3c_rtc *info = dev_get_drvdata(dev);

	clk_enable(info->rtc_clk);
	if (info->data->needs_src_clk)
		clk_enable(info->rtc_src_clk);

	if (info->data->enable)
		info->data->enable(info);

	if (info->data->restore_tick_cnt)
		info->data->restore_tick_cnt(info);

	if (device_may_wakeup(dev) && info->wake_en) {
		disable_irq_wake(info->irq_alarm);
		info->wake_en = false;
	}

	if (info->data->needs_src_clk)
		clk_disable(info->rtc_src_clk);
	clk_disable(info->rtc_clk);
>>>>>>> v3.18

	return 0;
}
#endif
<<<<<<< HEAD

static SIMPLE_DEV_PM_OPS(s3c_rtc_pm_ops, s3c_rtc_suspend, s3c_rtc_resume);

#ifdef CONFIG_OF
static struct s3c_rtc_drv_data s3c_rtc_drv_data_array[] = {
	[TYPE_S3C2410] = { TYPE_S3C2410 },
	[TYPE_S3C2416] = { TYPE_S3C2416 },
	[TYPE_S3C2443] = { TYPE_S3C2443 },
	[TYPE_S3C64XX] = { TYPE_S3C64XX },
=======
static SIMPLE_DEV_PM_OPS(s3c_rtc_pm_ops, s3c_rtc_suspend, s3c_rtc_resume);

static void s3c24xx_rtc_irq(struct s3c_rtc *info, int mask)
{
	clk_enable(info->rtc_clk);
	if (info->data->needs_src_clk)
		clk_enable(info->rtc_src_clk);
	rtc_update_irq(info->rtc, 1, RTC_AF | RTC_IRQF);
	if (info->data->needs_src_clk)
		clk_disable(info->rtc_src_clk);
	clk_disable(info->rtc_clk);

	s3c_rtc_alarm_clk_enable(info, false);
}

static void s3c6410_rtc_irq(struct s3c_rtc *info, int mask)
{
	clk_enable(info->rtc_clk);
	if (info->data->needs_src_clk)
		clk_enable(info->rtc_src_clk);
	rtc_update_irq(info->rtc, 1, RTC_AF | RTC_IRQF);
	writeb(mask, info->base + S3C2410_INTP);
	if (info->data->needs_src_clk)
		clk_disable(info->rtc_src_clk);
	clk_disable(info->rtc_clk);

	s3c_rtc_alarm_clk_enable(info, false);
}

static void s3c2410_rtc_setfreq(struct s3c_rtc *info, int freq)
{
	unsigned int tmp = 0;
	int val;

	tmp = readb(info->base + S3C2410_TICNT);
	tmp &= S3C2410_TICNT_ENABLE;

	val = (info->rtc->max_user_freq / freq) - 1;
	tmp |= val;

	writel(tmp, info->base + S3C2410_TICNT);
}

static void s3c2416_rtc_setfreq(struct s3c_rtc *info, int freq)
{
	unsigned int tmp = 0;
	int val;

	tmp = readb(info->base + S3C2410_TICNT);
	tmp &= S3C2410_TICNT_ENABLE;

	val = (info->rtc->max_user_freq / freq) - 1;

	tmp |= S3C2443_TICNT_PART(val);
	writel(S3C2443_TICNT1_PART(val), info->base + S3C2443_TICNT1);

	writel(S3C2416_TICNT2_PART(val), info->base + S3C2416_TICNT2);

	writel(tmp, info->base + S3C2410_TICNT);
}

static void s3c2443_rtc_setfreq(struct s3c_rtc *info, int freq)
{
	unsigned int tmp = 0;
	int val;

	tmp = readb(info->base + S3C2410_TICNT);
	tmp &= S3C2410_TICNT_ENABLE;

	val = (info->rtc->max_user_freq / freq) - 1;

	tmp |= S3C2443_TICNT_PART(val);
	writel(S3C2443_TICNT1_PART(val), info->base + S3C2443_TICNT1);

	writel(tmp, info->base + S3C2410_TICNT);
}

static void s3c6410_rtc_setfreq(struct s3c_rtc *info, int freq)
{
	int val;

	val = (info->rtc->max_user_freq / freq) - 1;
	writel(val, info->base + S3C2410_TICNT);
}

static void s3c24xx_rtc_enable_tick(struct s3c_rtc *info, struct seq_file *seq)
{
	unsigned int ticnt;

	ticnt = readb(info->base + S3C2410_TICNT);
	ticnt &= S3C2410_TICNT_ENABLE;

	seq_printf(seq, "periodic_IRQ\t: %s\n", ticnt  ? "yes" : "no");
}

static void s3c2416_rtc_select_tick_clk(struct s3c_rtc *info)
{
	unsigned int con;

	con = readw(info->base + S3C2410_RTCCON);
	con |= S3C2443_RTCCON_TICSEL;
	writew(con, info->base + S3C2410_RTCCON);
}

static void s3c6410_rtc_enable_tick(struct s3c_rtc *info, struct seq_file *seq)
{
	unsigned int ticnt;

	ticnt = readw(info->base + S3C2410_RTCCON);
	ticnt &= S3C64XX_RTCCON_TICEN;

	seq_printf(seq, "periodic_IRQ\t: %s\n", ticnt  ? "yes" : "no");
}

static void s3c24xx_rtc_save_tick_cnt(struct s3c_rtc *info)
{
	info->ticnt_save = readb(info->base + S3C2410_TICNT);
}

static void s3c24xx_rtc_restore_tick_cnt(struct s3c_rtc *info)
{
	writeb(info->ticnt_save, info->base + S3C2410_TICNT);
}

static void s3c6410_rtc_save_tick_cnt(struct s3c_rtc *info)
{
	info->ticnt_en_save = readw(info->base + S3C2410_RTCCON);
	info->ticnt_en_save &= S3C64XX_RTCCON_TICEN;
	info->ticnt_save = readl(info->base + S3C2410_TICNT);
}

static void s3c6410_rtc_restore_tick_cnt(struct s3c_rtc *info)
{
	unsigned int con;

	writel(info->ticnt_save, info->base + S3C2410_TICNT);
	if (info->ticnt_en_save) {
		con = readw(info->base + S3C2410_RTCCON);
		writew(con | info->ticnt_en_save,
				info->base + S3C2410_RTCCON);
	}
}

static struct s3c_rtc_data const s3c2410_rtc_data = {
	.max_user_freq		= 128,
	.irq_handler		= s3c24xx_rtc_irq,
	.set_freq		= s3c2410_rtc_setfreq,
	.enable_tick		= s3c24xx_rtc_enable_tick,
	.save_tick_cnt		= s3c24xx_rtc_save_tick_cnt,
	.restore_tick_cnt	= s3c24xx_rtc_restore_tick_cnt,
	.enable			= s3c24xx_rtc_enable,
	.disable		= s3c24xx_rtc_disable,
};

static struct s3c_rtc_data const s3c2416_rtc_data = {
	.max_user_freq		= 32768,
	.irq_handler		= s3c24xx_rtc_irq,
	.set_freq		= s3c2416_rtc_setfreq,
	.enable_tick		= s3c24xx_rtc_enable_tick,
	.select_tick_clk	= s3c2416_rtc_select_tick_clk,
	.save_tick_cnt		= s3c24xx_rtc_save_tick_cnt,
	.restore_tick_cnt	= s3c24xx_rtc_restore_tick_cnt,
	.enable			= s3c24xx_rtc_enable,
	.disable		= s3c24xx_rtc_disable,
};

static struct s3c_rtc_data const s3c2443_rtc_data = {
	.max_user_freq		= 32768,
	.irq_handler		= s3c24xx_rtc_irq,
	.set_freq		= s3c2443_rtc_setfreq,
	.enable_tick		= s3c24xx_rtc_enable_tick,
	.select_tick_clk	= s3c2416_rtc_select_tick_clk,
	.save_tick_cnt		= s3c24xx_rtc_save_tick_cnt,
	.restore_tick_cnt	= s3c24xx_rtc_restore_tick_cnt,
	.enable			= s3c24xx_rtc_enable,
	.disable		= s3c24xx_rtc_disable,
};

static struct s3c_rtc_data const s3c6410_rtc_data = {
	.max_user_freq		= 32768,
	.irq_handler		= s3c6410_rtc_irq,
	.set_freq		= s3c6410_rtc_setfreq,
	.enable_tick		= s3c6410_rtc_enable_tick,
	.save_tick_cnt		= s3c6410_rtc_save_tick_cnt,
	.restore_tick_cnt	= s3c6410_rtc_restore_tick_cnt,
	.enable			= s3c24xx_rtc_enable,
	.disable		= s3c6410_rtc_disable,
};

static struct s3c_rtc_data const exynos3250_rtc_data = {
	.max_user_freq		= 32768,
	.needs_src_clk		= true,
	.irq_handler		= s3c6410_rtc_irq,
	.set_freq		= s3c6410_rtc_setfreq,
	.enable_tick		= s3c6410_rtc_enable_tick,
	.save_tick_cnt		= s3c6410_rtc_save_tick_cnt,
	.restore_tick_cnt	= s3c6410_rtc_restore_tick_cnt,
	.enable			= s3c24xx_rtc_enable,
	.disable		= s3c6410_rtc_disable,
>>>>>>> v3.18
};

static const struct of_device_id s3c_rtc_dt_match[] = {
	{
		.compatible = "samsung,s3c2410-rtc",
<<<<<<< HEAD
		.data = &s3c_rtc_drv_data_array[TYPE_S3C2410],
	}, {
		.compatible = "samsung,s3c2416-rtc",
		.data = &s3c_rtc_drv_data_array[TYPE_S3C2416],
	}, {
		.compatible = "samsung,s3c2443-rtc",
		.data = &s3c_rtc_drv_data_array[TYPE_S3C2443],
	}, {
		.compatible = "samsung,s3c6410-rtc",
		.data = &s3c_rtc_drv_data_array[TYPE_S3C64XX],
	},
	{},
};
MODULE_DEVICE_TABLE(of, s3c_rtc_dt_match);
#endif

static struct platform_device_id s3c_rtc_driver_ids[] = {
	{
		.name		= "s3c2410-rtc",
		.driver_data	= TYPE_S3C2410,
	}, {
		.name		= "s3c2416-rtc",
		.driver_data	= TYPE_S3C2416,
	}, {
		.name		= "s3c2443-rtc",
		.driver_data	= TYPE_S3C2443,
	}, {
		.name		= "s3c64xx-rtc",
		.driver_data	= TYPE_S3C64XX,
	},
	{ }
};

MODULE_DEVICE_TABLE(platform, s3c_rtc_driver_ids);
=======
		.data = (void *)&s3c2410_rtc_data,
	}, {
		.compatible = "samsung,s3c2416-rtc",
		.data = (void *)&s3c2416_rtc_data,
	}, {
		.compatible = "samsung,s3c2443-rtc",
		.data = (void *)&s3c2443_rtc_data,
	}, {
		.compatible = "samsung,s3c6410-rtc",
		.data = (void *)&s3c6410_rtc_data,
	}, {
		.compatible = "samsung,exynos3250-rtc",
		.data = (void *)&exynos3250_rtc_data,
	},
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, s3c_rtc_dt_match);
>>>>>>> v3.18

static struct platform_driver s3c_rtc_driver = {
	.probe		= s3c_rtc_probe,
	.remove		= s3c_rtc_remove,
<<<<<<< HEAD
	.id_table	= s3c_rtc_driver_ids,
=======
>>>>>>> v3.18
	.driver		= {
		.name	= "s3c-rtc",
		.owner	= THIS_MODULE,
		.pm	= &s3c_rtc_pm_ops,
		.of_match_table	= of_match_ptr(s3c_rtc_dt_match),
	},
};
<<<<<<< HEAD

=======
>>>>>>> v3.18
module_platform_driver(s3c_rtc_driver);

MODULE_DESCRIPTION("Samsung S3C RTC Driver");
MODULE_AUTHOR("Ben Dooks <ben@simtec.co.uk>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:s3c2410-rtc");
