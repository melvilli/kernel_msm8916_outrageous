<<<<<<< HEAD
<<<<<<< HEAD
/**
 * \file drm_irq.c
 * IRQ support
=======
/*
 * drm_irq.c IRQ and vblank support
>>>>>>> v3.18
=======
/*
 * drm_irq.c IRQ and vblank support
>>>>>>> v3.18
 *
 * \author Rickard E. (Rik) Faith <faith@valinux.com>
 * \author Gareth Hughes <gareth@valinux.com>
 */

/*
 * Created: Fri Mar 19 14:30:16 1999 by faith@valinux.com
 *
 * Copyright 1999, 2000 Precision Insight, Inc., Cedar Park, Texas.
 * Copyright 2000 VA Linux Systems, Inc., Sunnyvale, California.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * VA LINUX SYSTEMS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <drm/drmP.h>
#include "drm_trace.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "drm_internal.h"
>>>>>>> v3.18
=======
#include "drm_internal.h"
>>>>>>> v3.18

#include <linux/interrupt.h>	/* For task queue support */
#include <linux/slab.h>

#include <linux/vgaarb.h>
#include <linux/export.h>

/* Access macro for slots in vblank timestamp ringbuffer. */
<<<<<<< HEAD
<<<<<<< HEAD
#define vblanktimestamp(dev, crtc, count) ( \
	(dev)->_vblank_time[(crtc) * DRM_VBLANKTIME_RBSIZE + \
	((count) % DRM_VBLANKTIME_RBSIZE)])
=======
#define vblanktimestamp(dev, crtc, count) \
	((dev)->vblank[crtc].time[(count) % DRM_VBLANKTIME_RBSIZE])
>>>>>>> v3.18
=======
#define vblanktimestamp(dev, crtc, count) \
	((dev)->vblank[crtc].time[(count) % DRM_VBLANKTIME_RBSIZE])
>>>>>>> v3.18

/* Retry timestamp calculation up to 3 times to satisfy
 * drm_timestamp_precision before giving up.
 */
#define DRM_TIMESTAMP_MAXRETRIES 3

/* Threshold in nanoseconds for detection of redundant
 * vblank irq in drm_handle_vblank(). 1 msec should be ok.
 */
#define DRM_REDUNDANT_VBLIRQ_THRESH_NS 1000000

<<<<<<< HEAD
<<<<<<< HEAD
/**
 * Get interrupt from bus id.
 *
 * \param inode device inode.
 * \param file_priv DRM file private.
 * \param cmd command.
 * \param arg user argument, pointing to a drm_irq_busid structure.
 * \return zero on success or a negative number on failure.
 *
 * Finds the PCI device with the specified bus id and gets its IRQ number.
 * This IOCTL is deprecated, and will now return EINVAL for any busid not equal
 * to that of the device that this DRM instance attached to.
 */
int drm_irq_by_busid(struct drm_device *dev, void *data,
		     struct drm_file *file_priv)
{
	struct drm_irq_busid *p = data;

	if (!dev->driver->bus->irq_by_busid)
		return -EINVAL;

	if (!drm_core_check_feature(dev, DRIVER_HAVE_IRQ))
		return -EINVAL;

	return dev->driver->bus->irq_by_busid(dev, p);
}

/*
 * Clear vblank timestamp buffer for a crtc.
 */
static void clear_vblank_timestamps(struct drm_device *dev, int crtc)
{
	memset(&dev->_vblank_time[crtc * DRM_VBLANKTIME_RBSIZE], 0,
		DRM_VBLANKTIME_RBSIZE * sizeof(struct timeval));
=======
=======
>>>>>>> v3.18
static bool
drm_get_last_vbltimestamp(struct drm_device *dev, int crtc,
			  struct timeval *tvblank, unsigned flags);

static unsigned int drm_timestamp_precision = 20;  /* Default to 20 usecs. */

/*
 * Default to use monotonic timestamps for wait-for-vblank and page-flip
 * complete events.
 */
unsigned int drm_timestamp_monotonic = 1;

static int drm_vblank_offdelay = 5000;    /* Default to 5000 msecs. */

module_param_named(vblankoffdelay, drm_vblank_offdelay, int, 0600);
module_param_named(timestamp_precision_usec, drm_timestamp_precision, int, 0600);
module_param_named(timestamp_monotonic, drm_timestamp_monotonic, int, 0600);

/**
 * drm_update_vblank_count - update the master vblank counter
 * @dev: DRM device
 * @crtc: counter to update
 *
 * Call back into the driver to update the appropriate vblank counter
 * (specified by @crtc).  Deal with wraparound, if it occurred, and
 * update the last read value so we can deal with wraparound on the next
 * call if necessary.
 *
 * Only necessary when going from off->on, to account for frames we
 * didn't get an interrupt for.
 *
 * Note: caller must hold dev->vbl_lock since this reads & writes
 * device vblank fields.
 */
static void drm_update_vblank_count(struct drm_device *dev, int crtc)
{
	struct drm_vblank_crtc *vblank = &dev->vblank[crtc];
	u32 cur_vblank, diff, tslot;
	bool rc;
	struct timeval t_vblank;

	/*
	 * Interrupts were disabled prior to this call, so deal with counter
	 * wrap if needed.
	 * NOTE!  It's possible we lost a full dev->max_vblank_count events
	 * here if the register is small or we had vblank interrupts off for
	 * a long time.
	 *
	 * We repeat the hardware vblank counter & timestamp query until
	 * we get consistent results. This to prevent races between gpu
	 * updating its hardware counter while we are retrieving the
	 * corresponding vblank timestamp.
	 */
	do {
		cur_vblank = dev->driver->get_vblank_counter(dev, crtc);
		rc = drm_get_last_vbltimestamp(dev, crtc, &t_vblank, 0);
	} while (cur_vblank != dev->driver->get_vblank_counter(dev, crtc));

	/* Deal with counter wrap */
	diff = cur_vblank - vblank->last;
	if (cur_vblank < vblank->last) {
		diff += dev->max_vblank_count;

		DRM_DEBUG("last_vblank[%d]=0x%x, cur_vblank=0x%x => diff=0x%x\n",
			  crtc, vblank->last, cur_vblank, diff);
	}

	DRM_DEBUG("updating vblank count on crtc %d, missed %d\n",
		  crtc, diff);

	if (diff == 0)
		return;

	/* Reinitialize corresponding vblank timestamp if high-precision query
	 * available. Skip this step if query unsupported or failed. Will
	 * reinitialize delayed at next vblank interrupt in that case.
	 */
	if (rc) {
		tslot = atomic_read(&vblank->count) + diff;
		vblanktimestamp(dev, crtc, tslot) = t_vblank;
	}

	smp_mb__before_atomic();
	atomic_add(diff, &vblank->count);
	smp_mb__after_atomic();
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/*
 * Disable vblank irq's on crtc, make sure that last vblank count
 * of hardware and corresponding consistent software vblank counter
 * are preserved, even if there are any spurious vblank irq's after
 * disable.
 */
static void vblank_disable_and_save(struct drm_device *dev, int crtc)
{
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long irqflags;
	u32 vblcount;
	s64 diff_ns;
	int vblrc;
=======
=======
>>>>>>> v3.18
	struct drm_vblank_crtc *vblank = &dev->vblank[crtc];
	unsigned long irqflags;
	u32 vblcount;
	s64 diff_ns;
	bool vblrc;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	struct timeval tvblank;
	int count = DRM_TIMESTAMP_MAXRETRIES;

	/* Prevent vblank irq processing while disabling vblank irqs,
	 * so no updates of timestamps or count can happen after we've
	 * disabled. Needed to prevent races in case of delayed irq's.
	 */
	spin_lock_irqsave(&dev->vblank_time_lock, irqflags);

<<<<<<< HEAD
<<<<<<< HEAD
	dev->driver->disable_vblank(dev, crtc);
	dev->vblank_enabled[crtc] = 0;
=======
=======
>>>>>>> v3.18
	/*
	 * If the vblank interrupt was already disbled update the count
	 * and timestamp to maintain the appearance that the counter
	 * has been ticking all along until this time. This makes the
	 * count account for the entire time between drm_vblank_on() and
	 * drm_vblank_off().
	 *
	 * But only do this if precise vblank timestamps are available.
	 * Otherwise we might read a totally bogus timestamp since drivers
	 * lacking precise timestamp support rely upon sampling the system clock
	 * at vblank interrupt time. Which obviously won't work out well if the
	 * vblank interrupt is disabled.
	 */
	if (!vblank->enabled &&
	    drm_get_last_vbltimestamp(dev, crtc, &tvblank, 0)) {
		drm_update_vblank_count(dev, crtc);
		spin_unlock_irqrestore(&dev->vblank_time_lock, irqflags);
		return;
	}

	dev->driver->disable_vblank(dev, crtc);
	vblank->enabled = false;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* No further vblank irq's will be processed after
	 * this point. Get current hardware vblank count and
	 * vblank timestamp, repeat until they are consistent.
	 *
	 * FIXME: There is still a race condition here and in
	 * drm_update_vblank_count() which can cause off-by-one
	 * reinitialization of software vblank counter. If gpu
	 * vblank counter doesn't increment exactly at the leading
	 * edge of a vblank interval, then we can lose 1 count if
	 * we happen to execute between start of vblank and the
	 * delayed gpu counter increment.
	 */
	do {
<<<<<<< HEAD
<<<<<<< HEAD
		dev->last_vblank[crtc] = dev->driver->get_vblank_counter(dev, crtc);
		vblrc = drm_get_last_vbltimestamp(dev, crtc, &tvblank, 0);
	} while (dev->last_vblank[crtc] != dev->driver->get_vblank_counter(dev, crtc) && (--count) && vblrc);
=======
		vblank->last = dev->driver->get_vblank_counter(dev, crtc);
		vblrc = drm_get_last_vbltimestamp(dev, crtc, &tvblank, 0);
	} while (vblank->last != dev->driver->get_vblank_counter(dev, crtc) && (--count) && vblrc);
>>>>>>> v3.18
=======
		vblank->last = dev->driver->get_vblank_counter(dev, crtc);
		vblrc = drm_get_last_vbltimestamp(dev, crtc, &tvblank, 0);
	} while (vblank->last != dev->driver->get_vblank_counter(dev, crtc) && (--count) && vblrc);
>>>>>>> v3.18

	if (!count)
		vblrc = 0;

	/* Compute time difference to stored timestamp of last vblank
	 * as updated by last invocation of drm_handle_vblank() in vblank irq.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	vblcount = atomic_read(&dev->_vblank_count[crtc]);
=======
	vblcount = atomic_read(&vblank->count);
>>>>>>> v3.18
=======
	vblcount = atomic_read(&vblank->count);
>>>>>>> v3.18
	diff_ns = timeval_to_ns(&tvblank) -
		  timeval_to_ns(&vblanktimestamp(dev, crtc, vblcount));

	/* If there is at least 1 msec difference between the last stored
	 * timestamp and tvblank, then we are currently executing our
	 * disable inside a new vblank interval, the tvblank timestamp
	 * corresponds to this new vblank interval and the irq handler
	 * for this vblank didn't run yet and won't run due to our disable.
	 * Therefore we need to do the job of drm_handle_vblank() and
	 * increment the vblank counter by one to account for this vblank.
	 *
	 * Skip this step if there isn't any high precision timestamp
	 * available. In that case we can't account for this and just
	 * hope for the best.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if ((vblrc > 0) && (abs64(diff_ns) > 1000000)) {
		atomic_inc(&dev->_vblank_count[crtc]);
		smp_mb__after_atomic();
	}

	/* Invalidate all timestamps while vblank irq's are off. */
	clear_vblank_timestamps(dev, crtc);

=======
=======
>>>>>>> v3.18
	if (vblrc && (abs64(diff_ns) > 1000000)) {
		/* Store new timestamp in ringbuffer. */
		vblanktimestamp(dev, crtc, vblcount + 1) = tvblank;

		/* Increment cooked vblank count. This also atomically commits
		 * the timestamp computed above.
		 */
		smp_mb__before_atomic();
		atomic_inc(&vblank->count);
		smp_mb__after_atomic();
	}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	spin_unlock_irqrestore(&dev->vblank_time_lock, irqflags);
}

static void vblank_disable_fn(unsigned long arg)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct drm_device *dev = (struct drm_device *)arg;
	unsigned long irqflags;
	int i;
=======
=======
>>>>>>> v3.18
	struct drm_vblank_crtc *vblank = (void *)arg;
	struct drm_device *dev = vblank->dev;
	unsigned long irqflags;
	int crtc = vblank->crtc;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (!dev->vblank_disable_allowed)
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	for (i = 0; i < dev->num_crtcs; i++) {
		spin_lock_irqsave(&dev->vbl_lock, irqflags);
		if (atomic_read(&dev->vblank_refcount[i]) == 0 &&
		    dev->vblank_enabled[i]) {
			DRM_DEBUG("disabling vblank on crtc %d\n", i);
			vblank_disable_and_save(dev, i);
		}
		spin_unlock_irqrestore(&dev->vbl_lock, irqflags);
	}
}

void drm_vblank_cleanup(struct drm_device *dev)
{
=======
=======
>>>>>>> v3.18
	spin_lock_irqsave(&dev->vbl_lock, irqflags);
	if (atomic_read(&vblank->refcount) == 0 && vblank->enabled) {
		DRM_DEBUG("disabling vblank on crtc %d\n", crtc);
		vblank_disable_and_save(dev, crtc);
	}
	spin_unlock_irqrestore(&dev->vbl_lock, irqflags);
}

/**
 * drm_vblank_cleanup - cleanup vblank support
 * @dev: DRM device
 *
 * This function cleans up any resources allocated in drm_vblank_init.
 */
void drm_vblank_cleanup(struct drm_device *dev)
{
	int crtc;
	unsigned long irqflags;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	/* Bail if the driver didn't call drm_vblank_init() */
	if (dev->num_crtcs == 0)
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	del_timer_sync(&dev->vblank_disable_timer);

	vblank_disable_fn((unsigned long)dev);

	kfree(dev->vbl_queue);
	kfree(dev->_vblank_count);
	kfree(dev->vblank_refcount);
	kfree(dev->vblank_enabled);
	kfree(dev->last_vblank);
	kfree(dev->last_vblank_wait);
	kfree(dev->vblank_inmodeset);
	kfree(dev->_vblank_time);
=======
=======
>>>>>>> v3.18
	for (crtc = 0; crtc < dev->num_crtcs; crtc++) {
		struct drm_vblank_crtc *vblank = &dev->vblank[crtc];

		del_timer_sync(&vblank->disable_timer);

		spin_lock_irqsave(&dev->vbl_lock, irqflags);
		vblank_disable_and_save(dev, crtc);
		spin_unlock_irqrestore(&dev->vbl_lock, irqflags);
	}

	kfree(dev->vblank);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	dev->num_crtcs = 0;
}
EXPORT_SYMBOL(drm_vblank_cleanup);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
/**
 * drm_vblank_init - initialize vblank support
 * @dev: drm_device
 * @num_crtcs: number of crtcs supported by @dev
 *
 * This function initializes vblank support for @num_crtcs display pipelines.
 *
 * Returns:
 * Zero on success or a negative error code on failure.
 */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
int drm_vblank_init(struct drm_device *dev, int num_crtcs)
{
	int i, ret = -ENOMEM;

<<<<<<< HEAD
<<<<<<< HEAD
	setup_timer(&dev->vblank_disable_timer, vblank_disable_fn,
		    (unsigned long)dev);
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	spin_lock_init(&dev->vbl_lock);
	spin_lock_init(&dev->vblank_time_lock);

	dev->num_crtcs = num_crtcs;

<<<<<<< HEAD
<<<<<<< HEAD
	dev->vbl_queue = kmalloc(sizeof(wait_queue_head_t) * num_crtcs,
				 GFP_KERNEL);
	if (!dev->vbl_queue)
		goto err;

	dev->_vblank_count = kmalloc(sizeof(atomic_t) * num_crtcs, GFP_KERNEL);
	if (!dev->_vblank_count)
		goto err;

	dev->vblank_refcount = kmalloc(sizeof(atomic_t) * num_crtcs,
				       GFP_KERNEL);
	if (!dev->vblank_refcount)
		goto err;

	dev->vblank_enabled = kcalloc(num_crtcs, sizeof(int), GFP_KERNEL);
	if (!dev->vblank_enabled)
		goto err;

	dev->last_vblank = kcalloc(num_crtcs, sizeof(u32), GFP_KERNEL);
	if (!dev->last_vblank)
		goto err;

	dev->last_vblank_wait = kcalloc(num_crtcs, sizeof(u32), GFP_KERNEL);
	if (!dev->last_vblank_wait)
		goto err;

	dev->vblank_inmodeset = kcalloc(num_crtcs, sizeof(int), GFP_KERNEL);
	if (!dev->vblank_inmodeset)
		goto err;

	dev->_vblank_time = kcalloc(num_crtcs * DRM_VBLANKTIME_RBSIZE,
				    sizeof(struct timeval), GFP_KERNEL);
	if (!dev->_vblank_time)
		goto err;

	DRM_INFO("Supports vblank timestamp caching Rev 1 (10.10.2010).\n");
=======
=======
>>>>>>> v3.18
	dev->vblank = kcalloc(num_crtcs, sizeof(*dev->vblank), GFP_KERNEL);
	if (!dev->vblank)
		goto err;

	for (i = 0; i < num_crtcs; i++) {
		struct drm_vblank_crtc *vblank = &dev->vblank[i];

		vblank->dev = dev;
		vblank->crtc = i;
		init_waitqueue_head(&vblank->queue);
		setup_timer(&vblank->disable_timer, vblank_disable_fn,
			    (unsigned long)vblank);
	}

	DRM_INFO("Supports vblank timestamp caching Rev 2 (21.10.2013).\n");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* Driver specific high-precision vblank timestamping supported? */
	if (dev->driver->get_vblank_timestamp)
		DRM_INFO("Driver supports precise vblank timestamp query.\n");
	else
		DRM_INFO("No driver support for vblank timestamp query.\n");

<<<<<<< HEAD
<<<<<<< HEAD
	/* Zero per-crtc vblank stuff */
	for (i = 0; i < num_crtcs; i++) {
		init_waitqueue_head(&dev->vbl_queue[i]);
		atomic_set(&dev->_vblank_count[i], 0);
		atomic_set(&dev->vblank_refcount[i], 0);
	}

	dev->vblank_disable_allowed = 0;
	return 0;

err:
	drm_vblank_cleanup(dev);
=======
=======
>>>>>>> v3.18
	dev->vblank_disable_allowed = false;

	return 0;

err:
	dev->num_crtcs = 0;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return ret;
}
EXPORT_SYMBOL(drm_vblank_init);

static void drm_irq_vgaarb_nokms(void *cookie, bool state)
{
	struct drm_device *dev = cookie;

	if (dev->driver->vgaarb_irq) {
		dev->driver->vgaarb_irq(dev, state);
		return;
	}

	if (!dev->irq_enabled)
		return;

	if (state) {
		if (dev->driver->irq_uninstall)
			dev->driver->irq_uninstall(dev);
	} else {
		if (dev->driver->irq_preinstall)
			dev->driver->irq_preinstall(dev);
		if (dev->driver->irq_postinstall)
			dev->driver->irq_postinstall(dev);
	}
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * Install IRQ handler.
 *
 * \param dev DRM device.
 *
 * Initializes the IRQ related data. Installs the handler, calling the driver
 * \c irq_preinstall() and \c irq_postinstall() functions
 * before and after the installation.
 */
int drm_irq_install(struct drm_device *dev)
{
	int ret;
	unsigned long sh_flags = 0;
	char *irqname;
=======
=======
>>>>>>> v3.18
 * drm_irq_install - install IRQ handler
 * @dev: DRM device
 * @irq: IRQ number to install the handler for
 *
 * Initializes the IRQ related data. Installs the handler, calling the driver
 * irq_preinstall() and irq_postinstall() functions before and after the
 * installation.
 *
 * This is the simplified helper interface provided for drivers with no special
 * needs. Drivers which need to install interrupt handlers for multiple
 * interrupts must instead set drm_device->irq_enabled to signal the DRM core
 * that vblank interrupts are available.
 *
 * Returns:
 * Zero on success or a negative error code on failure.
 */
int drm_irq_install(struct drm_device *dev, int irq)
{
	int ret;
	unsigned long sh_flags = 0;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (!drm_core_check_feature(dev, DRIVER_HAVE_IRQ))
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	if (drm_dev_to_irq(dev) == 0)
		return -EINVAL;

	mutex_lock(&dev->struct_mutex);

	/* Driver must have been initialized */
	if (!dev->dev_private) {
		mutex_unlock(&dev->struct_mutex);
		return -EINVAL;
	}

	if (dev->irq_enabled) {
		mutex_unlock(&dev->struct_mutex);
		return -EBUSY;
	}
	dev->irq_enabled = 1;
	mutex_unlock(&dev->struct_mutex);

	DRM_DEBUG("irq=%d\n", drm_dev_to_irq(dev));
=======
=======
>>>>>>> v3.18
	if (irq == 0)
		return -EINVAL;

	/* Driver must have been initialized */
	if (!dev->dev_private)
		return -EINVAL;

	if (dev->irq_enabled)
		return -EBUSY;
	dev->irq_enabled = true;

	DRM_DEBUG("irq=%d\n", irq);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* Before installing handler */
	if (dev->driver->irq_preinstall)
		dev->driver->irq_preinstall(dev);

	/* Install handler */
	if (drm_core_check_feature(dev, DRIVER_IRQ_SHARED))
		sh_flags = IRQF_SHARED;

<<<<<<< HEAD
<<<<<<< HEAD
	if (dev->devname)
		irqname = dev->devname;
	else
		irqname = dev->driver->name;

	ret = request_irq(drm_dev_to_irq(dev), dev->driver->irq_handler,
			  sh_flags, irqname, dev);

	if (ret < 0) {
		mutex_lock(&dev->struct_mutex);
		dev->irq_enabled = 0;
		mutex_unlock(&dev->struct_mutex);
=======
=======
>>>>>>> v3.18
	ret = request_irq(irq, dev->driver->irq_handler,
			  sh_flags, dev->driver->name, dev);

	if (ret < 0) {
		dev->irq_enabled = false;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return ret;
	}

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		vga_client_register(dev->pdev, (void *)dev, drm_irq_vgaarb_nokms, NULL);

	/* After installing handler */
	if (dev->driver->irq_postinstall)
		ret = dev->driver->irq_postinstall(dev);

	if (ret < 0) {
<<<<<<< HEAD
<<<<<<< HEAD
		mutex_lock(&dev->struct_mutex);
		dev->irq_enabled = 0;
		mutex_unlock(&dev->struct_mutex);
		if (!drm_core_check_feature(dev, DRIVER_MODESET))
			vga_client_register(dev->pdev, NULL, NULL, NULL);
		free_irq(drm_dev_to_irq(dev), dev);
=======
=======
>>>>>>> v3.18
		dev->irq_enabled = false;
		if (!drm_core_check_feature(dev, DRIVER_MODESET))
			vga_client_register(dev->pdev, NULL, NULL, NULL);
		free_irq(irq, dev);
	} else {
		dev->irq = irq;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}

	return ret;
}
EXPORT_SYMBOL(drm_irq_install);

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * Uninstall the IRQ handler.
 *
 * \param dev DRM device.
 *
 * Calls the driver's \c irq_uninstall() function, and stops the irq.
=======
=======
>>>>>>> v3.18
 * drm_irq_uninstall - uninstall the IRQ handler
 * @dev: DRM device
 *
 * Calls the driver's irq_uninstall() function and unregisters the IRQ handler.
 * This should only be called by drivers which used drm_irq_install() to set up
 * their interrupt handler. Other drivers must only reset
 * drm_device->irq_enabled to false.
 *
 * Note that for kernel modesetting drivers it is a bug if this function fails.
 * The sanity checks are only to catch buggy user modesetting drivers which call
 * the same function through an ioctl.
 *
 * Returns:
 * Zero on success or a negative error code on failure.
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 */
int drm_irq_uninstall(struct drm_device *dev)
{
	unsigned long irqflags;
<<<<<<< HEAD
<<<<<<< HEAD
	int irq_enabled, i;
=======
	bool irq_enabled;
	int i;
>>>>>>> v3.18
=======
	bool irq_enabled;
	int i;
>>>>>>> v3.18

	if (!drm_core_check_feature(dev, DRIVER_HAVE_IRQ))
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	mutex_lock(&dev->struct_mutex);
	irq_enabled = dev->irq_enabled;
	dev->irq_enabled = 0;
	mutex_unlock(&dev->struct_mutex);
=======
	irq_enabled = dev->irq_enabled;
	dev->irq_enabled = false;
>>>>>>> v3.18
=======
	irq_enabled = dev->irq_enabled;
	dev->irq_enabled = false;
>>>>>>> v3.18

	/*
	 * Wake up any waiters so they don't hang.
	 */
	if (dev->num_crtcs) {
		spin_lock_irqsave(&dev->vbl_lock, irqflags);
		for (i = 0; i < dev->num_crtcs; i++) {
<<<<<<< HEAD
<<<<<<< HEAD
			DRM_WAKEUP(&dev->vbl_queue[i]);
			dev->vblank_enabled[i] = 0;
			dev->last_vblank[i] =
=======
=======
>>>>>>> v3.18
			struct drm_vblank_crtc *vblank = &dev->vblank[i];

			wake_up(&vblank->queue);
			vblank->enabled = false;
			vblank->last =
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
				dev->driver->get_vblank_counter(dev, i);
		}
		spin_unlock_irqrestore(&dev->vbl_lock, irqflags);
	}

	if (!irq_enabled)
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	DRM_DEBUG("irq=%d\n", drm_dev_to_irq(dev));
=======
	DRM_DEBUG("irq=%d\n", dev->irq);
>>>>>>> v3.18
=======
	DRM_DEBUG("irq=%d\n", dev->irq);
>>>>>>> v3.18

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		vga_client_register(dev->pdev, NULL, NULL, NULL);

	if (dev->driver->irq_uninstall)
		dev->driver->irq_uninstall(dev);

<<<<<<< HEAD
<<<<<<< HEAD
	free_irq(drm_dev_to_irq(dev), dev);
=======
	free_irq(dev->irq, dev);
>>>>>>> v3.18
=======
	free_irq(dev->irq, dev);
>>>>>>> v3.18

	return 0;
}
EXPORT_SYMBOL(drm_irq_uninstall);

<<<<<<< HEAD
<<<<<<< HEAD
/**
=======
/*
>>>>>>> v3.18
=======
/*
>>>>>>> v3.18
 * IRQ control ioctl.
 *
 * \param inode device inode.
 * \param file_priv DRM file private.
 * \param cmd command.
 * \param arg user argument, pointing to a drm_control structure.
 * \return zero on success or a negative number on failure.
 *
 * Calls irq_install() or irq_uninstall() according to \p arg.
 */
int drm_control(struct drm_device *dev, void *data,
		struct drm_file *file_priv)
{
	struct drm_control *ctl = data;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	int ret = 0, irq;
>>>>>>> v3.18
=======
	int ret = 0, irq;
>>>>>>> v3.18

	/* if we haven't irq we fallback for compatibility reasons -
	 * this used to be a separate function in drm_dma.h
	 */

<<<<<<< HEAD
<<<<<<< HEAD

	switch (ctl->func) {
	case DRM_INST_HANDLER:
		if (!drm_core_check_feature(dev, DRIVER_HAVE_IRQ))
			return 0;
		if (drm_core_check_feature(dev, DRIVER_MODESET))
			return 0;
		if (dev->if_version < DRM_IF_VERSION(1, 2) &&
		    ctl->irq != drm_dev_to_irq(dev))
			return -EINVAL;
		return drm_irq_install(dev);
	case DRM_UNINST_HANDLER:
		if (!drm_core_check_feature(dev, DRIVER_HAVE_IRQ))
			return 0;
		if (drm_core_check_feature(dev, DRIVER_MODESET))
			return 0;
		return drm_irq_uninstall(dev);
=======
=======
>>>>>>> v3.18
	if (!drm_core_check_feature(dev, DRIVER_HAVE_IRQ))
		return 0;
	if (drm_core_check_feature(dev, DRIVER_MODESET))
		return 0;
	/* UMS was only ever support on pci devices. */
	if (WARN_ON(!dev->pdev))
		return -EINVAL;

	switch (ctl->func) {
	case DRM_INST_HANDLER:
		irq = dev->pdev->irq;

		if (dev->if_version < DRM_IF_VERSION(1, 2) &&
		    ctl->irq != irq)
			return -EINVAL;
		mutex_lock(&dev->struct_mutex);
		ret = drm_irq_install(dev, irq);
		mutex_unlock(&dev->struct_mutex);

		return ret;
	case DRM_UNINST_HANDLER:
		mutex_lock(&dev->struct_mutex);
		ret = drm_irq_uninstall(dev);
		mutex_unlock(&dev->struct_mutex);

		return ret;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	default:
		return -EINVAL;
	}
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * drm_calc_timestamping_constants - Calculate and
 * store various constants which are later needed by
 * vblank and swap-completion timestamping, e.g, by
 * drm_calc_vbltimestamp_from_scanoutpos().
 * They are derived from crtc's true scanout timing,
 * so they take things like panel scaling or other
 * adjustments into account.
 *
 * @crtc drm_crtc whose timestamp constants should be updated.
 *
 */
void drm_calc_timestamping_constants(struct drm_crtc *crtc)
{
	s64 linedur_ns = 0, pixeldur_ns = 0, framedur_ns = 0;
	u64 dotclock;

	/* Dot clock in Hz: */
	dotclock = (u64) crtc->hwmode.clock * 1000;

	/* Fields of interlaced scanout modes are only halve a frame duration.
	 * Double the dotclock to get halve the frame-/line-/pixelduration.
	 */
	if (crtc->hwmode.flags & DRM_MODE_FLAG_INTERLACE)
		dotclock *= 2;

	/* Valid dotclock? */
	if (dotclock > 0) {
		int frame_size;
		/* Convert scanline length in pixels and video dot clock to
		 * line duration, frame duration and pixel duration in
		 * nanoseconds:
		 */
		pixeldur_ns = (s64) div64_u64(1000000000, dotclock);
		linedur_ns  = (s64) div64_u64(((u64) crtc->hwmode.crtc_htotal *
					      1000000000), dotclock);
		frame_size = crtc->hwmode.crtc_htotal *
				crtc->hwmode.crtc_vtotal;
		framedur_ns = (s64) div64_u64((u64) frame_size * 1000000000,
					      dotclock);
=======
=======
>>>>>>> v3.18
 * drm_calc_timestamping_constants - calculate vblank timestamp constants
 * @crtc: drm_crtc whose timestamp constants should be updated.
 * @mode: display mode containing the scanout timings
 *
 * Calculate and store various constants which are later
 * needed by vblank and swap-completion timestamping, e.g,
 * by drm_calc_vbltimestamp_from_scanoutpos(). They are
 * derived from CRTC's true scanout timing, so they take
 * things like panel scaling or other adjustments into account.
 */
void drm_calc_timestamping_constants(struct drm_crtc *crtc,
				     const struct drm_display_mode *mode)
{
	int linedur_ns = 0, pixeldur_ns = 0, framedur_ns = 0;
	int dotclock = mode->crtc_clock;

	/* Valid dotclock? */
	if (dotclock > 0) {
		int frame_size = mode->crtc_htotal * mode->crtc_vtotal;

		/*
		 * Convert scanline length in pixels and video
		 * dot clock to line duration, frame duration
		 * and pixel duration in nanoseconds:
		 */
		pixeldur_ns = 1000000 / dotclock;
		linedur_ns  = div_u64((u64) mode->crtc_htotal * 1000000, dotclock);
		framedur_ns = div_u64((u64) frame_size * 1000000, dotclock);

		/*
		 * Fields of interlaced scanout modes are only half a frame duration.
		 */
		if (mode->flags & DRM_MODE_FLAG_INTERLACE)
			framedur_ns /= 2;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	} else
		DRM_ERROR("crtc %d: Can't calculate constants, dotclock = 0!\n",
			  crtc->base.id);

	crtc->pixeldur_ns = pixeldur_ns;
	crtc->linedur_ns  = linedur_ns;
	crtc->framedur_ns = framedur_ns;

	DRM_DEBUG("crtc %d: hwmode: htotal %d, vtotal %d, vdisplay %d\n",
<<<<<<< HEAD
<<<<<<< HEAD
		  crtc->base.id, crtc->hwmode.crtc_htotal,
		  crtc->hwmode.crtc_vtotal, crtc->hwmode.crtc_vdisplay);
	DRM_DEBUG("crtc %d: clock %d kHz framedur %d linedur %d, pixeldur %d\n",
		  crtc->base.id, (int) dotclock/1000, (int) framedur_ns,
		  (int) linedur_ns, (int) pixeldur_ns);
=======
=======
>>>>>>> v3.18
		  crtc->base.id, mode->crtc_htotal,
		  mode->crtc_vtotal, mode->crtc_vdisplay);
	DRM_DEBUG("crtc %d: clock %d kHz framedur %d linedur %d, pixeldur %d\n",
		  crtc->base.id, dotclock, framedur_ns,
		  linedur_ns, pixeldur_ns);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}
EXPORT_SYMBOL(drm_calc_timestamping_constants);

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * drm_calc_vbltimestamp_from_scanoutpos - helper routine for kms
 * drivers. Implements calculation of exact vblank timestamps from
 * given drm_display_mode timings and current video scanout position
 * of a crtc. This can be called from within get_vblank_timestamp()
 * implementation of a kms driver to implement the actual timestamping.
=======
=======
>>>>>>> v3.18
 * drm_calc_vbltimestamp_from_scanoutpos - precise vblank timestamp helper
 * @dev: DRM device
 * @crtc: Which CRTC's vblank timestamp to retrieve
 * @max_error: Desired maximum allowable error in timestamps (nanosecs)
 *             On return contains true maximum error of timestamp
 * @vblank_time: Pointer to struct timeval which should receive the timestamp
 * @flags: Flags to pass to driver:
 *         0 = Default,
 *         DRM_CALLED_FROM_VBLIRQ = If function is called from vbl IRQ handler
 * @refcrtc: CRTC which defines scanout timing
 * @mode: mode which defines the scanout timings
 *
 * Implements calculation of exact vblank timestamps from given drm_display_mode
 * timings and current video scanout position of a CRTC. This can be called from
 * within get_vblank_timestamp() implementation of a kms driver to implement the
 * actual timestamping.
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 *
 * Should return timestamps conforming to the OML_sync_control OpenML
 * extension specification. The timestamp corresponds to the end of
 * the vblank interval, aka start of scanout of topmost-leftmost display
 * pixel in the following video frame.
 *
 * Requires support for optional dev->driver->get_scanout_position()
 * in kms driver, plus a bit of setup code to provide a drm_display_mode
 * that corresponds to the true scanout timing.
 *
 * The current implementation only handles standard video modes. It
 * returns as no operation if a doublescan or interlaced video mode is
 * active. Higher level code is expected to handle this.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * @dev: DRM device.
 * @crtc: Which crtc's vblank timestamp to retrieve.
 * @max_error: Desired maximum allowable error in timestamps (nanosecs).
 *             On return contains true maximum error of timestamp.
 * @vblank_time: Pointer to struct timeval which should receive the timestamp.
 * @flags: Flags to pass to driver:
 *         0 = Default.
 *         DRM_CALLED_FROM_VBLIRQ = If function is called from vbl irq handler.
 * @refcrtc: drm_crtc* of crtc which defines scanout timing.
 *
 * Returns negative value on error, failure or if not supported in current
 * video mode:
 *
 * -EINVAL   - Invalid crtc.
=======
=======
>>>>>>> v3.18
 * Returns:
 * Negative value on error, failure or if not supported in current
 * video mode:
 *
 * -EINVAL   - Invalid CRTC.
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * -EAGAIN   - Temporary unavailable, e.g., called before initial modeset.
 * -ENOTSUPP - Function not supported in current display mode.
 * -EIO      - Failed, e.g., due to failed scanout position query.
 *
 * Returns or'ed positive status flags on success:
 *
 * DRM_VBLANKTIME_SCANOUTPOS_METHOD - Signal this method used for timestamping.
 * DRM_VBLANKTIME_INVBL - Timestamp taken while scanout was in vblank interval.
 *
 */
int drm_calc_vbltimestamp_from_scanoutpos(struct drm_device *dev, int crtc,
					  int *max_error,
					  struct timeval *vblank_time,
					  unsigned flags,
<<<<<<< HEAD
<<<<<<< HEAD
					  struct drm_crtc *refcrtc)
{
	ktime_t stime, etime, mono_time_offset;
	struct timeval tv_etime;
	struct drm_display_mode *mode;
	int vbl_status, vtotal, vdisplay;
	int vpos, hpos, i;
	s64 framedur_ns, linedur_ns, pixeldur_ns, delta_ns, duration_ns;
=======
=======
>>>>>>> v3.18
					  const struct drm_crtc *refcrtc,
					  const struct drm_display_mode *mode)
{
	struct timeval tv_etime;
	ktime_t stime, etime;
	int vbl_status;
	int vpos, hpos, i;
	int framedur_ns, linedur_ns, pixeldur_ns, delta_ns, duration_ns;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	bool invbl;

	if (crtc < 0 || crtc >= dev->num_crtcs) {
		DRM_ERROR("Invalid crtc %d\n", crtc);
		return -EINVAL;
	}

	/* Scanout position query not supported? Should not happen. */
	if (!dev->driver->get_scanout_position) {
		DRM_ERROR("Called from driver w/o get_scanout_position()!?\n");
		return -EIO;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	mode = &refcrtc->hwmode;
	vtotal = mode->crtc_vtotal;
	vdisplay = mode->crtc_vdisplay;

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	/* Durations of frames, lines, pixels in nanoseconds. */
	framedur_ns = refcrtc->framedur_ns;
	linedur_ns  = refcrtc->linedur_ns;
	pixeldur_ns = refcrtc->pixeldur_ns;

	/* If mode timing undefined, just return as no-op:
	 * Happens during initial modesetting of a crtc.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (vtotal <= 0 || vdisplay <= 0 || framedur_ns == 0) {
=======
	if (framedur_ns == 0) {
>>>>>>> v3.18
=======
	if (framedur_ns == 0) {
>>>>>>> v3.18
		DRM_DEBUG("crtc %d: Noop due to uninitialized mode.\n", crtc);
		return -EAGAIN;
	}

	/* Get current scanout position with system timestamp.
	 * Repeat query up to DRM_TIMESTAMP_MAXRETRIES times
	 * if single query takes longer than max_error nanoseconds.
	 *
	 * This guarantees a tight bound on maximum error if
	 * code gets preempted or delayed for some reason.
	 */
	for (i = 0; i < DRM_TIMESTAMP_MAXRETRIES; i++) {
<<<<<<< HEAD
<<<<<<< HEAD
		/* Disable preemption to make it very likely to
		 * succeed in the first iteration even on PREEMPT_RT kernel.
		 */
		preempt_disable();

		/* Get system timestamp before query. */
		stime = ktime_get();

		/* Get vertical and horizontal scanout pos. vpos, hpos. */
		vbl_status = dev->driver->get_scanout_position(dev, crtc, &vpos, &hpos);

		/* Get system timestamp after query. */
		etime = ktime_get();
		if (!drm_timestamp_monotonic)
			mono_time_offset = ktime_get_monotonic_offset();

		preempt_enable();
=======
=======
>>>>>>> v3.18
		/*
		 * Get vertical and horizontal scanout position vpos, hpos,
		 * and bounding timestamps stime, etime, pre/post query.
		 */
		vbl_status = dev->driver->get_scanout_position(dev, crtc, flags, &vpos,
							       &hpos, &stime, &etime);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

		/* Return as no-op if scanout query unsupported or failed. */
		if (!(vbl_status & DRM_SCANOUTPOS_VALID)) {
			DRM_DEBUG("crtc %d : scanoutpos query failed [%d].\n",
				  crtc, vbl_status);
			return -EIO;
		}

<<<<<<< HEAD
<<<<<<< HEAD
		duration_ns = ktime_to_ns(etime) - ktime_to_ns(stime);

		/* Accept result with <  max_error nsecs timing uncertainty. */
		if (duration_ns <= (s64) *max_error)
=======
=======
>>>>>>> v3.18
		/* Compute uncertainty in timestamp of scanout position query. */
		duration_ns = ktime_to_ns(etime) - ktime_to_ns(stime);

		/* Accept result with <  max_error nsecs timing uncertainty. */
		if (duration_ns <= *max_error)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			break;
	}

	/* Noisy system timing? */
	if (i == DRM_TIMESTAMP_MAXRETRIES) {
		DRM_DEBUG("crtc %d: Noisy timestamp %d us > %d us [%d reps].\n",
<<<<<<< HEAD
<<<<<<< HEAD
			  crtc, (int) duration_ns/1000, *max_error/1000, i);
	}

	/* Return upper bound of timestamp precision error. */
	*max_error = (int) duration_ns;
=======
=======
>>>>>>> v3.18
			  crtc, duration_ns/1000, *max_error/1000, i);
	}

	/* Return upper bound of timestamp precision error. */
	*max_error = duration_ns;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* Check if in vblank area:
	 * vpos is >=0 in video scanout area, but negative
	 * within vblank area, counting down the number of lines until
	 * start of scanout.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	invbl = vbl_status & DRM_SCANOUTPOS_INVBL;
=======
	invbl = vbl_status & DRM_SCANOUTPOS_IN_VBLANK;
>>>>>>> v3.18
=======
	invbl = vbl_status & DRM_SCANOUTPOS_IN_VBLANK;
>>>>>>> v3.18

	/* Convert scanout position into elapsed time at raw_time query
	 * since start of scanout at first display scanline. delta_ns
	 * can be negative if start of scanout hasn't happened yet.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	delta_ns = (s64) vpos * linedur_ns + (s64) hpos * pixeldur_ns;

	/* Is vpos outside nominal vblank area, but less than
	 * 1/100 of a frame height away from start of vblank?
	 * If so, assume this isn't a massively delayed vblank
	 * interrupt, but a vblank interrupt that fired a few
	 * microseconds before true start of vblank. Compensate
	 * by adding a full frame duration to the final timestamp.
	 * Happens, e.g., on ATI R500, R600.
	 *
	 * We only do this if DRM_CALLED_FROM_VBLIRQ.
	 */
	if ((flags & DRM_CALLED_FROM_VBLIRQ) && !invbl &&
	    ((vdisplay - vpos) < vtotal / 100)) {
		delta_ns = delta_ns - framedur_ns;

		/* Signal this correction as "applied". */
		vbl_status |= 0x8;
	}

	if (!drm_timestamp_monotonic)
		etime = ktime_sub(etime, mono_time_offset);
=======
=======
>>>>>>> v3.18
	delta_ns = vpos * linedur_ns + hpos * pixeldur_ns;

	if (!drm_timestamp_monotonic)
		etime = ktime_mono_to_real(etime);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* save this only for debugging purposes */
	tv_etime = ktime_to_timeval(etime);
	/* Subtract time delta from raw timestamp to get final
	 * vblank_time timestamp for end of vblank.
	 */
	if (delta_ns < 0)
		etime = ktime_add_ns(etime, -delta_ns);
	else
		etime = ktime_sub_ns(etime, delta_ns);
	*vblank_time = ktime_to_timeval(etime);

	DRM_DEBUG("crtc %d : v %d p(%d,%d)@ %ld.%ld -> %ld.%ld [e %d us, %d rep]\n",
		  crtc, (int)vbl_status, hpos, vpos,
		  (long)tv_etime.tv_sec, (long)tv_etime.tv_usec,
		  (long)vblank_time->tv_sec, (long)vblank_time->tv_usec,
<<<<<<< HEAD
<<<<<<< HEAD
		  (int)duration_ns/1000, i);

	vbl_status = DRM_VBLANKTIME_SCANOUTPOS_METHOD;
	if (invbl)
		vbl_status |= DRM_VBLANKTIME_INVBL;
=======
=======
>>>>>>> v3.18
		  duration_ns/1000, i);

	vbl_status = DRM_VBLANKTIME_SCANOUTPOS_METHOD;
	if (invbl)
		vbl_status |= DRM_VBLANKTIME_IN_VBLANK;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return vbl_status;
}
EXPORT_SYMBOL(drm_calc_vbltimestamp_from_scanoutpos);

static struct timeval get_drm_timestamp(void)
{
	ktime_t now;

<<<<<<< HEAD
<<<<<<< HEAD
	now = ktime_get();
	if (!drm_timestamp_monotonic)
		now = ktime_sub(now, ktime_get_monotonic_offset());

=======
	now = drm_timestamp_monotonic ? ktime_get() : ktime_get_real();
>>>>>>> v3.18
=======
	now = drm_timestamp_monotonic ? ktime_get() : ktime_get_real();
>>>>>>> v3.18
	return ktime_to_timeval(now);
}

/**
 * drm_get_last_vbltimestamp - retrieve raw timestamp for the most recent
<<<<<<< HEAD
<<<<<<< HEAD
 * vblank interval.
 *
 * @dev: DRM device
 * @crtc: which crtc's vblank timestamp to retrieve
 * @tvblank: Pointer to target struct timeval which should receive the timestamp
 * @flags: Flags to pass to driver:
 *         0 = Default.
 *         DRM_CALLED_FROM_VBLIRQ = If function is called from vbl irq handler.
 *
 * Fetches the system timestamp corresponding to the time of the most recent
 * vblank interval on specified crtc. May call into kms-driver to
=======
=======
>>>>>>> v3.18
 * 			       vblank interval
 * @dev: DRM device
 * @crtc: which CRTC's vblank timestamp to retrieve
 * @tvblank: Pointer to target struct timeval which should receive the timestamp
 * @flags: Flags to pass to driver:
 *         0 = Default,
 *         DRM_CALLED_FROM_VBLIRQ = If function is called from vbl IRQ handler
 *
 * Fetches the system timestamp corresponding to the time of the most recent
 * vblank interval on specified CRTC. May call into kms-driver to
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * compute the timestamp with a high-precision GPU specific method.
 *
 * Returns zero if timestamp originates from uncorrected do_gettimeofday()
 * call, i.e., it isn't very precisely locked to the true vblank.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Returns non-zero if timestamp is considered to be very precise.
 */
u32 drm_get_last_vbltimestamp(struct drm_device *dev, int crtc,
			      struct timeval *tvblank, unsigned flags)
=======
=======
>>>>>>> v3.18
 * Returns:
 * True if timestamp is considered to be very precise, false otherwise.
 */
static bool
drm_get_last_vbltimestamp(struct drm_device *dev, int crtc,
			  struct timeval *tvblank, unsigned flags)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	int ret;

	/* Define requested maximum error on timestamps (nanoseconds). */
	int max_error = (int) drm_timestamp_precision * 1000;

	/* Query driver if possible and precision timestamping enabled. */
	if (dev->driver->get_vblank_timestamp && (max_error > 0)) {
		ret = dev->driver->get_vblank_timestamp(dev, crtc, &max_error,
							tvblank, flags);
		if (ret > 0)
<<<<<<< HEAD
<<<<<<< HEAD
			return (u32) ret;
=======
			return true;
>>>>>>> v3.18
=======
			return true;
>>>>>>> v3.18
	}

	/* GPU high precision timestamp query unsupported or failed.
	 * Return current monotonic/gettimeofday timestamp as best estimate.
	 */
	*tvblank = get_drm_timestamp();

<<<<<<< HEAD
<<<<<<< HEAD
	return 0;
}
EXPORT_SYMBOL(drm_get_last_vbltimestamp);
=======
	return false;
}
>>>>>>> v3.18
=======
	return false;
}
>>>>>>> v3.18

/**
 * drm_vblank_count - retrieve "cooked" vblank counter value
 * @dev: DRM device
 * @crtc: which counter to retrieve
 *
 * Fetches the "cooked" vblank count value that represents the number of
 * vblank events since the system was booted, including lost events due to
 * modesetting activity.
<<<<<<< HEAD
<<<<<<< HEAD
 */
u32 drm_vblank_count(struct drm_device *dev, int crtc)
{
	return atomic_read(&dev->_vblank_count[crtc]);
=======
=======
>>>>>>> v3.18
 *
 * Returns:
 * The software vblank counter.
 */
u32 drm_vblank_count(struct drm_device *dev, int crtc)
{
	struct drm_vblank_crtc *vblank = &dev->vblank[crtc];

	if (WARN_ON(crtc >= dev->num_crtcs))
		return 0;
	return atomic_read(&vblank->count);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}
EXPORT_SYMBOL(drm_vblank_count);

/**
 * drm_vblank_count_and_time - retrieve "cooked" vblank counter value
 * and the system timestamp corresponding to that vblank counter value.
 *
 * @dev: DRM device
 * @crtc: which counter to retrieve
 * @vblanktime: Pointer to struct timeval to receive the vblank timestamp.
 *
 * Fetches the "cooked" vblank count value that represents the number of
 * vblank events since the system was booted, including lost events due to
 * modesetting activity. Returns corresponding system timestamp of the time
<<<<<<< HEAD
<<<<<<< HEAD
 * of the vblank interval that corresponds to the current value vblank counter
 * value.
=======
 * of the vblank interval that corresponds to the current vblank counter value.
>>>>>>> v3.18
=======
 * of the vblank interval that corresponds to the current vblank counter value.
>>>>>>> v3.18
 */
u32 drm_vblank_count_and_time(struct drm_device *dev, int crtc,
			      struct timeval *vblanktime)
{
<<<<<<< HEAD
<<<<<<< HEAD
	u32 cur_vblank;

=======
=======
>>>>>>> v3.18
	struct drm_vblank_crtc *vblank = &dev->vblank[crtc];
	u32 cur_vblank;

	if (WARN_ON(crtc >= dev->num_crtcs))
		return 0;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	/* Read timestamp from slot of _vblank_time ringbuffer
	 * that corresponds to current vblank count. Retry if
	 * count has incremented during readout. This works like
	 * a seqlock.
	 */
	do {
<<<<<<< HEAD
<<<<<<< HEAD
		cur_vblank = atomic_read(&dev->_vblank_count[crtc]);
		*vblanktime = vblanktimestamp(dev, crtc, cur_vblank);
		smp_rmb();
	} while (cur_vblank != atomic_read(&dev->_vblank_count[crtc]));
=======
=======
>>>>>>> v3.18
		cur_vblank = atomic_read(&vblank->count);
		*vblanktime = vblanktimestamp(dev, crtc, cur_vblank);
		smp_rmb();
	} while (cur_vblank != atomic_read(&vblank->count));
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return cur_vblank;
}
EXPORT_SYMBOL(drm_vblank_count_and_time);

static void send_vblank_event(struct drm_device *dev,
		struct drm_pending_vblank_event *e,
		unsigned long seq, struct timeval *now)
{
	WARN_ON_SMP(!spin_is_locked(&dev->event_lock));
	e->event.sequence = seq;
	e->event.tv_sec = now->tv_sec;
	e->event.tv_usec = now->tv_usec;

	list_add_tail(&e->base.link,
		      &e->base.file_priv->event_list);
	wake_up_interruptible(&e->base.file_priv->event_wait);
	trace_drm_vblank_event_delivered(e->base.pid, e->pipe,
					 e->event.sequence);
}

/**
 * drm_send_vblank_event - helper to send vblank event after pageflip
 * @dev: DRM device
 * @crtc: CRTC in question
 * @e: the event to send
 *
 * Updates sequence # and timestamp on event, and sends it to userspace.
 * Caller must hold event lock.
 */
void drm_send_vblank_event(struct drm_device *dev, int crtc,
		struct drm_pending_vblank_event *e)
{
	struct timeval now;
	unsigned int seq;
	if (crtc >= 0) {
		seq = drm_vblank_count_and_time(dev, crtc, &now);
	} else {
		seq = 0;

		now = get_drm_timestamp();
	}
	e->pipe = crtc;
	send_vblank_event(dev, e, seq, &now);
}
EXPORT_SYMBOL(drm_send_vblank_event);

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * drm_update_vblank_count - update the master vblank counter
 * @dev: DRM device
 * @crtc: counter to update
 *
 * Call back into the driver to update the appropriate vblank counter
 * (specified by @crtc).  Deal with wraparound, if it occurred, and
 * update the last read value so we can deal with wraparound on the next
 * call if necessary.
 *
 * Only necessary when going from off->on, to account for frames we
 * didn't get an interrupt for.
 *
 * Note: caller must hold dev->vbl_lock since this reads & writes
 * device vblank fields.
 */
static void drm_update_vblank_count(struct drm_device *dev, int crtc)
{
	u32 cur_vblank, diff, tslot, rc;
	struct timeval t_vblank;

	/*
	 * Interrupts were disabled prior to this call, so deal with counter
	 * wrap if needed.
	 * NOTE!  It's possible we lost a full dev->max_vblank_count events
	 * here if the register is small or we had vblank interrupts off for
	 * a long time.
	 *
	 * We repeat the hardware vblank counter & timestamp query until
	 * we get consistent results. This to prevent races between gpu
	 * updating its hardware counter while we are retrieving the
	 * corresponding vblank timestamp.
	 */
	do {
		cur_vblank = dev->driver->get_vblank_counter(dev, crtc);
		rc = drm_get_last_vbltimestamp(dev, crtc, &t_vblank, 0);
	} while (cur_vblank != dev->driver->get_vblank_counter(dev, crtc));

	/* Deal with counter wrap */
	diff = cur_vblank - dev->last_vblank[crtc];
	if (cur_vblank < dev->last_vblank[crtc]) {
		diff += dev->max_vblank_count;

		DRM_DEBUG("last_vblank[%d]=0x%x, cur_vblank=0x%x => diff=0x%x\n",
			  crtc, dev->last_vblank[crtc], cur_vblank, diff);
	}

	DRM_DEBUG("enabling vblank interrupts on crtc %d, missed %d\n",
		  crtc, diff);

	/* Reinitialize corresponding vblank timestamp if high-precision query
	 * available. Skip this step if query unsupported or failed. Will
	 * reinitialize delayed at next vblank interrupt in that case.
	 */
	if (rc) {
		tslot = atomic_read(&dev->_vblank_count[crtc]) + diff;
		vblanktimestamp(dev, crtc, tslot) = t_vblank;
	}

	smp_mb__before_atomic();
	atomic_add(diff, &dev->_vblank_count[crtc]);
	smp_mb__after_atomic();
=======
=======
>>>>>>> v3.18
 * drm_vblank_enable - enable the vblank interrupt on a CRTC
 * @dev: DRM device
 * @crtc: CRTC in question
 */
static int drm_vblank_enable(struct drm_device *dev, int crtc)
{
	struct drm_vblank_crtc *vblank = &dev->vblank[crtc];
	int ret = 0;

	assert_spin_locked(&dev->vbl_lock);

	spin_lock(&dev->vblank_time_lock);

	if (!vblank->enabled) {
		/*
		 * Enable vblank irqs under vblank_time_lock protection.
		 * All vblank count & timestamp updates are held off
		 * until we are done reinitializing master counter and
		 * timestamps. Filtercode in drm_handle_vblank() will
		 * prevent double-accounting of same vblank interval.
		 */
		ret = dev->driver->enable_vblank(dev, crtc);
		DRM_DEBUG("enabling vblank on crtc %d, ret: %d\n", crtc, ret);
		if (ret)
			atomic_dec(&vblank->refcount);
		else {
			vblank->enabled = true;
			drm_update_vblank_count(dev, crtc);
		}
	}

	spin_unlock(&dev->vblank_time_lock);

	return ret;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/**
 * drm_vblank_get - get a reference count on vblank events
 * @dev: DRM device
 * @crtc: which CRTC to own
 *
 * Acquire a reference count on vblank events to avoid having them disabled
 * while in use.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * RETURNS
=======
 * This is the legacy version of drm_crtc_vblank_get().
 *
 * Returns:
>>>>>>> v3.18
=======
 * This is the legacy version of drm_crtc_vblank_get().
 *
 * Returns:
>>>>>>> v3.18
 * Zero on success, nonzero on failure.
 */
int drm_vblank_get(struct drm_device *dev, int crtc)
{
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long irqflags, irqflags2;
	int ret = 0;

	spin_lock_irqsave(&dev->vbl_lock, irqflags);
	/* Going from 0->1 means we have to enable interrupts again */
	if (atomic_add_return(1, &dev->vblank_refcount[crtc]) == 1) {
		spin_lock_irqsave(&dev->vblank_time_lock, irqflags2);
		if (!dev->vblank_enabled[crtc]) {
			/* Enable vblank irqs under vblank_time_lock protection.
			 * All vblank count & timestamp updates are held off
			 * until we are done reinitializing master counter and
			 * timestamps. Filtercode in drm_handle_vblank() will
			 * prevent double-accounting of same vblank interval.
			 */
			ret = dev->driver->enable_vblank(dev, crtc);
			DRM_DEBUG("enabling vblank on crtc %d, ret: %d\n",
				  crtc, ret);
			if (ret)
				atomic_dec(&dev->vblank_refcount[crtc]);
			else {
				dev->vblank_enabled[crtc] = 1;
				drm_update_vblank_count(dev, crtc);
			}
		}
		spin_unlock_irqrestore(&dev->vblank_time_lock, irqflags2);
	} else {
		if (!dev->vblank_enabled[crtc]) {
			atomic_dec(&dev->vblank_refcount[crtc]);
=======
=======
>>>>>>> v3.18
	struct drm_vblank_crtc *vblank = &dev->vblank[crtc];
	unsigned long irqflags;
	int ret = 0;

	if (WARN_ON(crtc >= dev->num_crtcs))
		return -EINVAL;

	spin_lock_irqsave(&dev->vbl_lock, irqflags);
	/* Going from 0->1 means we have to enable interrupts again */
	if (atomic_add_return(1, &vblank->refcount) == 1) {
		ret = drm_vblank_enable(dev, crtc);
	} else {
		if (!vblank->enabled) {
			atomic_dec(&vblank->refcount);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			ret = -EINVAL;
		}
	}
	spin_unlock_irqrestore(&dev->vbl_lock, irqflags);

	return ret;
}
EXPORT_SYMBOL(drm_vblank_get);

/**
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
 * drm_crtc_vblank_get - get a reference count on vblank events
 * @crtc: which CRTC to own
 *
 * Acquire a reference count on vblank events to avoid having them disabled
 * while in use.
 *
 * This is the native kms version of drm_vblank_off().
 *
 * Returns:
 * Zero on success, nonzero on failure.
 */
int drm_crtc_vblank_get(struct drm_crtc *crtc)
{
	return drm_vblank_get(crtc->dev, drm_crtc_index(crtc));
}
EXPORT_SYMBOL(drm_crtc_vblank_get);

/**
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * drm_vblank_put - give up ownership of vblank events
 * @dev: DRM device
 * @crtc: which counter to give up
 *
 * Release ownership of a given vblank counter, turning off interrupts
 * if possible. Disable interrupts after drm_vblank_offdelay milliseconds.
<<<<<<< HEAD
<<<<<<< HEAD
 */
void drm_vblank_put(struct drm_device *dev, int crtc)
{
	BUG_ON(atomic_read(&dev->vblank_refcount[crtc]) == 0);

	/* Last user schedules interrupt disable */
	if (atomic_dec_and_test(&dev->vblank_refcount[crtc]) &&
	    (drm_vblank_offdelay > 0))
		mod_timer(&dev->vblank_disable_timer,
			  jiffies + ((drm_vblank_offdelay * DRM_HZ)/1000));
=======
=======
>>>>>>> v3.18
 *
 * This is the legacy version of drm_crtc_vblank_put().
 */
void drm_vblank_put(struct drm_device *dev, int crtc)
{
	struct drm_vblank_crtc *vblank = &dev->vblank[crtc];

	BUG_ON(atomic_read(&vblank->refcount) == 0);

	if (WARN_ON(crtc >= dev->num_crtcs))
		return;

	/* Last user schedules interrupt disable */
	if (atomic_dec_and_test(&vblank->refcount)) {
		if (drm_vblank_offdelay == 0)
			return;
		else if (dev->vblank_disable_immediate || drm_vblank_offdelay < 0)
			vblank_disable_fn((unsigned long)vblank);
		else
			mod_timer(&vblank->disable_timer,
				  jiffies + ((drm_vblank_offdelay * HZ)/1000));
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}
EXPORT_SYMBOL(drm_vblank_put);

/**
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
 * drm_crtc_vblank_put - give up ownership of vblank events
 * @crtc: which counter to give up
 *
 * Release ownership of a given vblank counter, turning off interrupts
 * if possible. Disable interrupts after drm_vblank_offdelay milliseconds.
 *
 * This is the native kms version of drm_vblank_put().
 */
void drm_crtc_vblank_put(struct drm_crtc *crtc)
{
	drm_vblank_put(crtc->dev, drm_crtc_index(crtc));
}
EXPORT_SYMBOL(drm_crtc_vblank_put);

/**
 * drm_wait_one_vblank - wait for one vblank
 * @dev: DRM device
 * @crtc: crtc index
 *
 * This waits for one vblank to pass on @crtc, using the irq driver interfaces.
 * It is a failure to call this when the vblank irq for @crtc is disabled, e.g.
 * due to lack of driver support or because the crtc is off.
 */
void drm_wait_one_vblank(struct drm_device *dev, int crtc)
{
	int ret;
	u32 last;

	ret = drm_vblank_get(dev, crtc);
	if (WARN(ret, "vblank not available on crtc %i, ret=%i\n", crtc, ret))
		return;

	last = drm_vblank_count(dev, crtc);

	ret = wait_event_timeout(dev->vblank[crtc].queue,
				 last != drm_vblank_count(dev, crtc),
				 msecs_to_jiffies(100));

	WARN(ret == 0, "vblank wait timed out on crtc %i\n", crtc);

	drm_vblank_put(dev, crtc);
}
EXPORT_SYMBOL(drm_wait_one_vblank);

/**
 * drm_crtc_wait_one_vblank - wait for one vblank
 * @crtc: DRM crtc
 *
 * This waits for one vblank to pass on @crtc, using the irq driver interfaces.
 * It is a failure to call this when the vblank irq for @crtc is disabled, e.g.
 * due to lack of driver support or because the crtc is off.
 */
void drm_crtc_wait_one_vblank(struct drm_crtc *crtc)
{
	drm_wait_one_vblank(crtc->dev, drm_crtc_index(crtc));
}
EXPORT_SYMBOL(drm_crtc_wait_one_vblank);

/**
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * drm_vblank_off - disable vblank events on a CRTC
 * @dev: DRM device
 * @crtc: CRTC in question
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Caller must hold event lock.
 */
void drm_vblank_off(struct drm_device *dev, int crtc)
{
=======
=======
>>>>>>> v3.18
 * Drivers can use this function to shut down the vblank interrupt handling when
 * disabling a crtc. This function ensures that the latest vblank frame count is
 * stored so that drm_vblank_on() can restore it again.
 *
 * Drivers must use this function when the hardware vblank counter can get
 * reset, e.g. when suspending.
 *
 * This is the legacy version of drm_crtc_vblank_off().
 */
void drm_vblank_off(struct drm_device *dev, int crtc)
{
	struct drm_vblank_crtc *vblank = &dev->vblank[crtc];
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	struct drm_pending_vblank_event *e, *t;
	struct timeval now;
	unsigned long irqflags;
	unsigned int seq;

<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock_irqsave(&dev->vbl_lock, irqflags);
	vblank_disable_and_save(dev, crtc);
	DRM_WAKEUP(&dev->vbl_queue[crtc]);
=======
=======
>>>>>>> v3.18
	if (WARN_ON(crtc >= dev->num_crtcs))
		return;

	spin_lock_irqsave(&dev->event_lock, irqflags);

	spin_lock(&dev->vbl_lock);
	vblank_disable_and_save(dev, crtc);
	wake_up(&vblank->queue);

	/*
	 * Prevent subsequent drm_vblank_get() from re-enabling
	 * the vblank interrupt by bumping the refcount.
	 */
	if (!vblank->inmodeset) {
		atomic_inc(&vblank->refcount);
		vblank->inmodeset = 1;
	}
	spin_unlock(&dev->vbl_lock);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* Send any queued vblank events, lest the natives grow disquiet */
	seq = drm_vblank_count_and_time(dev, crtc, &now);

<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock(&dev->event_lock);
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	list_for_each_entry_safe(e, t, &dev->vblank_event_list, base.link) {
		if (e->pipe != crtc)
			continue;
		DRM_DEBUG("Sending premature vblank event on disable: \
			  wanted %d, current %d\n",
			  e->event.sequence, seq);
		list_del(&e->base.link);
		drm_vblank_put(dev, e->pipe);
		send_vblank_event(dev, e, seq, &now);
	}
<<<<<<< HEAD
<<<<<<< HEAD
	spin_unlock(&dev->event_lock);

	spin_unlock_irqrestore(&dev->vbl_lock, irqflags);
}
EXPORT_SYMBOL(drm_vblank_off);
=======
=======
>>>>>>> v3.18
	spin_unlock_irqrestore(&dev->event_lock, irqflags);
}
EXPORT_SYMBOL(drm_vblank_off);

/**
 * drm_crtc_vblank_off - disable vblank events on a CRTC
 * @crtc: CRTC in question
 *
 * Drivers can use this function to shut down the vblank interrupt handling when
 * disabling a crtc. This function ensures that the latest vblank frame count is
 * stored so that drm_vblank_on can restore it again.
 *
 * Drivers must use this function when the hardware vblank counter can get
 * reset, e.g. when suspending.
 *
 * This is the native kms version of drm_vblank_off().
 */
void drm_crtc_vblank_off(struct drm_crtc *crtc)
{
	drm_vblank_off(crtc->dev, drm_crtc_index(crtc));
}
EXPORT_SYMBOL(drm_crtc_vblank_off);

/**
 * drm_vblank_on - enable vblank events on a CRTC
 * @dev: DRM device
 * @crtc: CRTC in question
 *
 * This functions restores the vblank interrupt state captured with
 * drm_vblank_off() again. Note that calls to drm_vblank_on() and
 * drm_vblank_off() can be unbalanced and so can also be unconditionaly called
 * in driver load code to reflect the current hardware state of the crtc.
 *
 * This is the legacy version of drm_crtc_vblank_on().
 */
void drm_vblank_on(struct drm_device *dev, int crtc)
{
	struct drm_vblank_crtc *vblank = &dev->vblank[crtc];
	unsigned long irqflags;

	if (WARN_ON(crtc >= dev->num_crtcs))
		return;

	spin_lock_irqsave(&dev->vbl_lock, irqflags);
	/* Drop our private "prevent drm_vblank_get" refcount */
	if (vblank->inmodeset) {
		atomic_dec(&vblank->refcount);
		vblank->inmodeset = 0;
	}

	/*
	 * sample the current counter to avoid random jumps
	 * when drm_vblank_enable() applies the diff
	 *
	 * -1 to make sure user will never see the same
	 * vblank counter value before and after a modeset
	 */
	vblank->last =
		(dev->driver->get_vblank_counter(dev, crtc) - 1) &
		dev->max_vblank_count;
	/*
	 * re-enable interrupts if there are users left, or the
	 * user wishes vblank interrupts to be enabled all the time.
	 */
	if (atomic_read(&vblank->refcount) != 0 ||
	    (!dev->vblank_disable_immediate && drm_vblank_offdelay == 0))
		WARN_ON(drm_vblank_enable(dev, crtc));
	spin_unlock_irqrestore(&dev->vbl_lock, irqflags);
}
EXPORT_SYMBOL(drm_vblank_on);

/**
 * drm_crtc_vblank_on - enable vblank events on a CRTC
 * @crtc: CRTC in question
 *
 * This functions restores the vblank interrupt state captured with
 * drm_vblank_off() again. Note that calls to drm_vblank_on() and
 * drm_vblank_off() can be unbalanced and so can also be unconditionaly called
 * in driver load code to reflect the current hardware state of the crtc.
 *
 * This is the native kms version of drm_vblank_on().
 */
void drm_crtc_vblank_on(struct drm_crtc *crtc)
{
	drm_vblank_on(crtc->dev, drm_crtc_index(crtc));
}
EXPORT_SYMBOL(drm_crtc_vblank_on);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

/**
 * drm_vblank_pre_modeset - account for vblanks across mode sets
 * @dev: DRM device
 * @crtc: CRTC in question
 *
 * Account for vblank events across mode setting events, which will likely
 * reset the hardware frame counter.
<<<<<<< HEAD
<<<<<<< HEAD
 */
void drm_vblank_pre_modeset(struct drm_device *dev, int crtc)
{
	/* vblank is not initialized (IRQ not installed ?), or has been freed */
	if (!dev->num_crtcs)
		return;
=======
=======
>>>>>>> v3.18
 *
 * This is done by grabbing a temporary vblank reference to ensure that the
 * vblank interrupt keeps running across the modeset sequence. With this the
 * software-side vblank frame counting will ensure that there are no jumps or
 * discontinuities.
 *
 * Unfortunately this approach is racy and also doesn't work when the vblank
 * interrupt stops running, e.g. across system suspend resume. It is therefore
 * highly recommended that drivers use the newer drm_vblank_off() and
 * drm_vblank_on() instead. drm_vblank_pre_modeset() only works correctly when
 * using "cooked" software vblank frame counters and not relying on any hardware
 * counters.
 *
 * Drivers must call drm_vblank_post_modeset() when re-enabling the same crtc
 * again.
 */
void drm_vblank_pre_modeset(struct drm_device *dev, int crtc)
{
	struct drm_vblank_crtc *vblank = &dev->vblank[crtc];

	/* vblank is not initialized (IRQ not installed ?), or has been freed */
	if (!dev->num_crtcs)
		return;

	if (WARN_ON(crtc >= dev->num_crtcs))
		return;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	/*
	 * To avoid all the problems that might happen if interrupts
	 * were enabled/disabled around or between these calls, we just
	 * have the kernel take a reference on the CRTC (just once though
	 * to avoid corrupting the count if multiple, mismatch calls occur),
	 * so that interrupts remain enabled in the interim.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (!dev->vblank_inmodeset[crtc]) {
		dev->vblank_inmodeset[crtc] = 0x1;
		if (drm_vblank_get(dev, crtc) == 0)
			dev->vblank_inmodeset[crtc] |= 0x2;
=======
=======
>>>>>>> v3.18
	if (!vblank->inmodeset) {
		vblank->inmodeset = 0x1;
		if (drm_vblank_get(dev, crtc) == 0)
			vblank->inmodeset |= 0x2;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}
}
EXPORT_SYMBOL(drm_vblank_pre_modeset);

<<<<<<< HEAD
<<<<<<< HEAD
void drm_vblank_post_modeset(struct drm_device *dev, int crtc)
{
=======
=======
>>>>>>> v3.18
/**
 * drm_vblank_post_modeset - undo drm_vblank_pre_modeset changes
 * @dev: DRM device
 * @crtc: CRTC in question
 *
 * This function again drops the temporary vblank reference acquired in
 * drm_vblank_pre_modeset.
 */
void drm_vblank_post_modeset(struct drm_device *dev, int crtc)
{
	struct drm_vblank_crtc *vblank = &dev->vblank[crtc];
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	unsigned long irqflags;

	/* vblank is not initialized (IRQ not installed ?), or has been freed */
	if (!dev->num_crtcs)
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	if (dev->vblank_inmodeset[crtc]) {
		spin_lock_irqsave(&dev->vbl_lock, irqflags);
		dev->vblank_disable_allowed = 1;
		spin_unlock_irqrestore(&dev->vbl_lock, irqflags);

		if (dev->vblank_inmodeset[crtc] & 0x2)
			drm_vblank_put(dev, crtc);

		dev->vblank_inmodeset[crtc] = 0;
=======
=======
>>>>>>> v3.18
	if (vblank->inmodeset) {
		spin_lock_irqsave(&dev->vbl_lock, irqflags);
		dev->vblank_disable_allowed = true;
		spin_unlock_irqrestore(&dev->vbl_lock, irqflags);

		if (vblank->inmodeset & 0x2)
			drm_vblank_put(dev, crtc);

		vblank->inmodeset = 0;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}
}
EXPORT_SYMBOL(drm_vblank_post_modeset);

<<<<<<< HEAD
<<<<<<< HEAD
/**
=======
/*
>>>>>>> v3.18
=======
/*
>>>>>>> v3.18
 * drm_modeset_ctl - handle vblank event counter changes across mode switch
 * @DRM_IOCTL_ARGS: standard ioctl arguments
 *
 * Applications should call the %_DRM_PRE_MODESET and %_DRM_POST_MODESET
 * ioctls around modesetting so that any lost vblank events are accounted for.
 *
 * Generally the counter will reset across mode sets.  If interrupts are
 * enabled around this call, we don't have to do anything since the counter
 * will have already been incremented.
 */
int drm_modeset_ctl(struct drm_device *dev, void *data,
		    struct drm_file *file_priv)
{
	struct drm_modeset_ctl *modeset = data;
	unsigned int crtc;

	/* If drm_vblank_init() hasn't been called yet, just no-op */
	if (!dev->num_crtcs)
		return 0;

	/* KMS drivers handle this internally */
	if (drm_core_check_feature(dev, DRIVER_MODESET))
		return 0;

	crtc = modeset->crtc;
	if (crtc >= dev->num_crtcs)
		return -EINVAL;

	switch (modeset->cmd) {
	case _DRM_PRE_MODESET:
		drm_vblank_pre_modeset(dev, crtc);
		break;
	case _DRM_POST_MODESET:
		drm_vblank_post_modeset(dev, crtc);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int drm_queue_vblank_event(struct drm_device *dev, int pipe,
				  union drm_wait_vblank *vblwait,
				  struct drm_file *file_priv)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct drm_vblank_crtc *vblank = &dev->vblank[pipe];
>>>>>>> v3.18
=======
	struct drm_vblank_crtc *vblank = &dev->vblank[pipe];
>>>>>>> v3.18
	struct drm_pending_vblank_event *e;
	struct timeval now;
	unsigned long flags;
	unsigned int seq;
	int ret;

	e = kzalloc(sizeof *e, GFP_KERNEL);
	if (e == NULL) {
		ret = -ENOMEM;
		goto err_put;
	}

	e->pipe = pipe;
	e->base.pid = current->pid;
	e->event.base.type = DRM_EVENT_VBLANK;
	e->event.base.length = sizeof e->event;
	e->event.user_data = vblwait->request.signal;
	e->base.event = &e->event.base;
	e->base.file_priv = file_priv;
	e->base.destroy = (void (*) (struct drm_pending_event *)) kfree;

	spin_lock_irqsave(&dev->event_lock, flags);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	/*
	 * drm_vblank_off() might have been called after we called
	 * drm_vblank_get(). drm_vblank_off() holds event_lock
	 * around the vblank disable, so no need for further locking.
	 * The reference from drm_vblank_get() protects against
	 * vblank disable from another source.
	 */
	if (!vblank->enabled) {
		ret = -EINVAL;
		goto err_unlock;
	}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (file_priv->event_space < sizeof e->event) {
		ret = -EBUSY;
		goto err_unlock;
	}

	file_priv->event_space -= sizeof e->event;
	seq = drm_vblank_count_and_time(dev, pipe, &now);

	if ((vblwait->request.type & _DRM_VBLANK_NEXTONMISS) &&
	    (seq - vblwait->request.sequence) <= (1 << 23)) {
		vblwait->request.sequence = seq + 1;
		vblwait->reply.sequence = vblwait->request.sequence;
	}

	DRM_DEBUG("event on vblank count %d, current %d, crtc %d\n",
		  vblwait->request.sequence, seq, pipe);

	trace_drm_vblank_event_queued(current->pid, pipe,
				      vblwait->request.sequence);

	e->event.sequence = vblwait->request.sequence;
	if ((seq - vblwait->request.sequence) <= (1 << 23)) {
		drm_vblank_put(dev, pipe);
		send_vblank_event(dev, e, seq, &now);
		vblwait->reply.sequence = seq;
	} else {
		/* drm_handle_vblank_events will call drm_vblank_put */
		list_add_tail(&e->base.link, &dev->vblank_event_list);
		vblwait->reply.sequence = vblwait->request.sequence;
	}

	spin_unlock_irqrestore(&dev->event_lock, flags);

	return 0;

err_unlock:
	spin_unlock_irqrestore(&dev->event_lock, flags);
	kfree(e);
err_put:
	drm_vblank_put(dev, pipe);
	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
/**
=======
/*
>>>>>>> v3.18
=======
/*
>>>>>>> v3.18
 * Wait for VBLANK.
 *
 * \param inode device inode.
 * \param file_priv DRM file private.
 * \param cmd command.
 * \param data user argument, pointing to a drm_wait_vblank structure.
 * \return zero on success or a negative number on failure.
 *
 * This function enables the vblank interrupt on the pipe requested, then
 * sleeps waiting for the requested sequence number to occur, and drops
<<<<<<< HEAD
<<<<<<< HEAD
 * the vblank interrupt refcount afterwards. (vblank irq disable follows that
=======
 * the vblank interrupt refcount afterwards. (vblank IRQ disable follows that
>>>>>>> v3.18
=======
 * the vblank interrupt refcount afterwards. (vblank IRQ disable follows that
>>>>>>> v3.18
 * after a timeout with no further vblank waits scheduled).
 */
int drm_wait_vblank(struct drm_device *dev, void *data,
		    struct drm_file *file_priv)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct drm_vblank_crtc *vblank;
>>>>>>> v3.18
=======
	struct drm_vblank_crtc *vblank;
>>>>>>> v3.18
	union drm_wait_vblank *vblwait = data;
	int ret;
	unsigned int flags, seq, crtc, high_crtc;

<<<<<<< HEAD
<<<<<<< HEAD
	if (drm_core_check_feature(dev, DRIVER_HAVE_IRQ))
		if ((!drm_dev_to_irq(dev)) || (!dev->irq_enabled))
			return -EINVAL;
=======
	if (!dev->irq_enabled)
		return -EINVAL;
>>>>>>> v3.18
=======
	if (!dev->irq_enabled)
		return -EINVAL;
>>>>>>> v3.18

	if (vblwait->request.type & _DRM_VBLANK_SIGNAL)
		return -EINVAL;

	if (vblwait->request.type &
	    ~(_DRM_VBLANK_TYPES_MASK | _DRM_VBLANK_FLAGS_MASK |
	      _DRM_VBLANK_HIGH_CRTC_MASK)) {
		DRM_ERROR("Unsupported type value 0x%x, supported mask 0x%x\n",
			  vblwait->request.type,
			  (_DRM_VBLANK_TYPES_MASK | _DRM_VBLANK_FLAGS_MASK |
			   _DRM_VBLANK_HIGH_CRTC_MASK));
		return -EINVAL;
	}

	flags = vblwait->request.type & _DRM_VBLANK_FLAGS_MASK;
	high_crtc = (vblwait->request.type & _DRM_VBLANK_HIGH_CRTC_MASK);
	if (high_crtc)
		crtc = high_crtc >> _DRM_VBLANK_HIGH_CRTC_SHIFT;
	else
		crtc = flags & _DRM_VBLANK_SECONDARY ? 1 : 0;
	if (crtc >= dev->num_crtcs)
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	vblank = &dev->vblank[crtc];

>>>>>>> v3.18
=======
	vblank = &dev->vblank[crtc];

>>>>>>> v3.18
	ret = drm_vblank_get(dev, crtc);
	if (ret) {
		DRM_DEBUG("failed to acquire vblank counter, %d\n", ret);
		return ret;
	}
	seq = drm_vblank_count(dev, crtc);

	switch (vblwait->request.type & _DRM_VBLANK_TYPES_MASK) {
	case _DRM_VBLANK_RELATIVE:
		vblwait->request.sequence += seq;
		vblwait->request.type &= ~_DRM_VBLANK_RELATIVE;
	case _DRM_VBLANK_ABSOLUTE:
		break;
	default:
		ret = -EINVAL;
		goto done;
	}

	if (flags & _DRM_VBLANK_EVENT) {
		/* must hold on to the vblank ref until the event fires
		 * drm_vblank_put will be called asynchronously
		 */
		return drm_queue_vblank_event(dev, crtc, vblwait, file_priv);
	}

	if ((flags & _DRM_VBLANK_NEXTONMISS) &&
	    (seq - vblwait->request.sequence) <= (1<<23)) {
		vblwait->request.sequence = seq + 1;
	}

	DRM_DEBUG("waiting on vblank count %d, crtc %d\n",
		  vblwait->request.sequence, crtc);
<<<<<<< HEAD
<<<<<<< HEAD
	dev->last_vblank_wait[crtc] = vblwait->request.sequence;
	DRM_WAIT_ON(ret, dev->vbl_queue[crtc], 3 * DRM_HZ,
		    (((drm_vblank_count(dev, crtc) -
		       vblwait->request.sequence) <= (1 << 23)) ||
=======
=======
>>>>>>> v3.18
	vblank->last_wait = vblwait->request.sequence;
	DRM_WAIT_ON(ret, vblank->queue, 3 * HZ,
		    (((drm_vblank_count(dev, crtc) -
		       vblwait->request.sequence) <= (1 << 23)) ||
		     !vblank->enabled ||
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		     !dev->irq_enabled));

	if (ret != -EINTR) {
		struct timeval now;

		vblwait->reply.sequence = drm_vblank_count_and_time(dev, crtc, &now);
		vblwait->reply.tval_sec = now.tv_sec;
		vblwait->reply.tval_usec = now.tv_usec;

		DRM_DEBUG("returning %d to client\n",
			  vblwait->reply.sequence);
	} else {
		DRM_DEBUG("vblank wait interrupted by signal\n");
	}

done:
	drm_vblank_put(dev, crtc);
	return ret;
}

static void drm_handle_vblank_events(struct drm_device *dev, int crtc)
{
	struct drm_pending_vblank_event *e, *t;
	struct timeval now;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long flags;
	unsigned int seq;

	seq = drm_vblank_count_and_time(dev, crtc, &now);

	spin_lock_irqsave(&dev->event_lock, flags);
=======
=======
>>>>>>> v3.18
	unsigned int seq;

	assert_spin_locked(&dev->event_lock);

	seq = drm_vblank_count_and_time(dev, crtc, &now);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	list_for_each_entry_safe(e, t, &dev->vblank_event_list, base.link) {
		if (e->pipe != crtc)
			continue;
		if ((seq - e->event.sequence) > (1<<23))
			continue;

		DRM_DEBUG("vblank event on %d, current %d\n",
			  e->event.sequence, seq);

		list_del(&e->base.link);
		drm_vblank_put(dev, e->pipe);
		send_vblank_event(dev, e, seq, &now);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	spin_unlock_irqrestore(&dev->event_lock, flags);

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	trace_drm_vblank_event(crtc, seq);
}

/**
 * drm_handle_vblank - handle a vblank event
 * @dev: DRM device
 * @crtc: where this event occurred
 *
 * Drivers should call this routine in their vblank interrupt handlers to
 * update the vblank counter and send any signals that may be pending.
 */
bool drm_handle_vblank(struct drm_device *dev, int crtc)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct drm_vblank_crtc *vblank = &dev->vblank[crtc];
>>>>>>> v3.18
=======
	struct drm_vblank_crtc *vblank = &dev->vblank[crtc];
>>>>>>> v3.18
	u32 vblcount;
	s64 diff_ns;
	struct timeval tvblank;
	unsigned long irqflags;

	if (!dev->num_crtcs)
		return false;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	if (WARN_ON(crtc >= dev->num_crtcs))
		return false;

	spin_lock_irqsave(&dev->event_lock, irqflags);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	/* Need timestamp lock to prevent concurrent execution with
	 * vblank enable/disable, as this would cause inconsistent
	 * or corrupted timestamps and vblank counts.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock_irqsave(&dev->vblank_time_lock, irqflags);

	/* Vblank irq handling disabled. Nothing to do. */
	if (!dev->vblank_enabled[crtc]) {
		spin_unlock_irqrestore(&dev->vblank_time_lock, irqflags);
=======
=======
>>>>>>> v3.18
	spin_lock(&dev->vblank_time_lock);

	/* Vblank irq handling disabled. Nothing to do. */
	if (!vblank->enabled) {
		spin_unlock(&dev->vblank_time_lock);
		spin_unlock_irqrestore(&dev->event_lock, irqflags);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return false;
	}

	/* Fetch corresponding timestamp for this vblank interval from
	 * driver and store it in proper slot of timestamp ringbuffer.
	 */

	/* Get current timestamp and count. */
<<<<<<< HEAD
<<<<<<< HEAD
	vblcount = atomic_read(&dev->_vblank_count[crtc]);
=======
	vblcount = atomic_read(&vblank->count);
>>>>>>> v3.18
=======
	vblcount = atomic_read(&vblank->count);
>>>>>>> v3.18
	drm_get_last_vbltimestamp(dev, crtc, &tvblank, DRM_CALLED_FROM_VBLIRQ);

	/* Compute time difference to timestamp of last vblank */
	diff_ns = timeval_to_ns(&tvblank) -
		  timeval_to_ns(&vblanktimestamp(dev, crtc, vblcount));

	/* Update vblank timestamp and count if at least
	 * DRM_REDUNDANT_VBLIRQ_THRESH_NS nanoseconds
	 * difference between last stored timestamp and current
	 * timestamp. A smaller difference means basically
	 * identical timestamps. Happens if this vblank has
	 * been already processed and this is a redundant call,
	 * e.g., due to spurious vblank interrupts. We need to
	 * ignore those for accounting.
	 */
	if (abs64(diff_ns) > DRM_REDUNDANT_VBLIRQ_THRESH_NS) {
		/* Store new timestamp in ringbuffer. */
		vblanktimestamp(dev, crtc, vblcount + 1) = tvblank;

		/* Increment cooked vblank count. This also atomically commits
		 * the timestamp computed above.
		 */
		smp_mb__before_atomic();
<<<<<<< HEAD
<<<<<<< HEAD
		atomic_inc(&dev->_vblank_count[crtc]);
=======
		atomic_inc(&vblank->count);
>>>>>>> v3.18
=======
		atomic_inc(&vblank->count);
>>>>>>> v3.18
		smp_mb__after_atomic();
	} else {
		DRM_DEBUG("crtc %d: Redundant vblirq ignored. diff_ns = %d\n",
			  crtc, (int) diff_ns);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	DRM_WAKEUP(&dev->vbl_queue[crtc]);
	drm_handle_vblank_events(dev, crtc);

	spin_unlock_irqrestore(&dev->vblank_time_lock, irqflags);
=======
=======
>>>>>>> v3.18
	spin_unlock(&dev->vblank_time_lock);

	wake_up(&vblank->queue);
	drm_handle_vblank_events(dev, crtc);

	spin_unlock_irqrestore(&dev->event_lock, irqflags);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return true;
}
EXPORT_SYMBOL(drm_handle_vblank);
