/* exynos_drm_vidi.c
 *
 * Copyright (C) 2012 Samsung Electronics Co.Ltd
 * Authors:
 *	Inki Dae <inki.dae@samsung.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */
#include <drm/drmP.h>

#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/module.h>
=======
>>>>>>> v3.18
#include <linux/platform_device.h>

#include <drm/exynos_drm.h>

#include <drm/drm_edid.h>
#include <drm/drm_crtc_helper.h>

#include "exynos_drm_drv.h"
#include "exynos_drm_crtc.h"
#include "exynos_drm_encoder.h"
<<<<<<< HEAD
=======
#include "exynos_drm_vidi.h"
>>>>>>> v3.18

/* vidi has totally three virtual windows. */
#define WINDOWS_NR		3

<<<<<<< HEAD
#define get_vidi_context(dev)	platform_get_drvdata(to_platform_device(dev))
=======
#define get_vidi_mgr(dev)	platform_get_drvdata(to_platform_device(dev))
#define ctx_from_connector(c)	container_of(c, struct vidi_context, \
					connector)
>>>>>>> v3.18

struct vidi_win_data {
	unsigned int		offset_x;
	unsigned int		offset_y;
	unsigned int		ovl_width;
	unsigned int		ovl_height;
	unsigned int		fb_width;
	unsigned int		fb_height;
	unsigned int		bpp;
	dma_addr_t		dma_addr;
	unsigned int		buf_offsize;
	unsigned int		line_size;	/* bytes */
	bool			enabled;
};

struct vidi_context {
<<<<<<< HEAD
	struct exynos_drm_subdrv	subdrv;
	struct drm_crtc			*crtc;
=======
	struct drm_device		*drm_dev;
	struct drm_crtc			*crtc;
	struct drm_encoder		*encoder;
	struct drm_connector		connector;
	struct exynos_drm_subdrv	subdrv;
>>>>>>> v3.18
	struct vidi_win_data		win_data[WINDOWS_NR];
	struct edid			*raw_edid;
	unsigned int			clkdiv;
	unsigned int			default_win;
	unsigned long			irq_flags;
	unsigned int			connected;
	bool				vblank_on;
	bool				suspended;
	bool				direct_vblank;
	struct work_struct		work;
	struct mutex			lock;
<<<<<<< HEAD
=======
	int				pipe;
>>>>>>> v3.18
};

static const char fake_edid_info[] = {
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x4c, 0x2d, 0x05, 0x05,
	0x00, 0x00, 0x00, 0x00, 0x30, 0x12, 0x01, 0x03, 0x80, 0x10, 0x09, 0x78,
	0x0a, 0xee, 0x91, 0xa3, 0x54, 0x4c, 0x99, 0x26, 0x0f, 0x50, 0x54, 0xbd,
	0xee, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x66, 0x21, 0x50, 0xb0, 0x51, 0x00,
	0x1b, 0x30, 0x40, 0x70, 0x36, 0x00, 0xa0, 0x5a, 0x00, 0x00, 0x00, 0x1e,
	0x01, 0x1d, 0x00, 0x72, 0x51, 0xd0, 0x1e, 0x20, 0x6e, 0x28, 0x55, 0x00,
	0xa0, 0x5a, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x18,
	0x4b, 0x1a, 0x44, 0x17, 0x00, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x00, 0x00, 0x00, 0xfc, 0x00, 0x53, 0x41, 0x4d, 0x53, 0x55, 0x4e, 0x47,
	0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0xbc, 0x02, 0x03, 0x1e, 0xf1,
	0x46, 0x84, 0x05, 0x03, 0x10, 0x20, 0x22, 0x23, 0x09, 0x07, 0x07, 0x83,
	0x01, 0x00, 0x00, 0xe2, 0x00, 0x0f, 0x67, 0x03, 0x0c, 0x00, 0x10, 0x00,
	0xb8, 0x2d, 0x01, 0x1d, 0x80, 0x18, 0x71, 0x1c, 0x16, 0x20, 0x58, 0x2c,
	0x25, 0x00, 0xa0, 0x5a, 0x00, 0x00, 0x00, 0x9e, 0x8c, 0x0a, 0xd0, 0x8a,
	0x20, 0xe0, 0x2d, 0x10, 0x10, 0x3e, 0x96, 0x00, 0xa0, 0x5a, 0x00, 0x00,
	0x00, 0x18, 0x02, 0x3a, 0x80, 0x18, 0x71, 0x38, 0x2d, 0x40, 0x58, 0x2c,
	0x45, 0x00, 0xa0, 0x5a, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x06
};

<<<<<<< HEAD
static bool vidi_display_is_connected(struct device *dev)
{
	struct vidi_context *ctx = get_vidi_context(dev);

	DRM_DEBUG_KMS("%s\n", __FILE__);

	/*
	 * connection request would come from user side
	 * to do hotplug through specific ioctl.
	 */
	return ctx->connected ? true : false;
}

static struct edid *vidi_get_edid(struct device *dev,
			struct drm_connector *connector)
{
	struct vidi_context *ctx = get_vidi_context(dev);
	struct edid *edid;
	int edid_len;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	/*
	 * the edid data comes from user side and it would be set
	 * to ctx->raw_edid through specific ioctl.
	 */
	if (!ctx->raw_edid) {
		DRM_DEBUG_KMS("raw_edid is null.\n");
		return ERR_PTR(-EFAULT);
	}

	edid_len = (1 + ctx->raw_edid->extensions) * EDID_LENGTH;
	edid = kmemdup(ctx->raw_edid, edid_len, GFP_KERNEL);
	if (!edid) {
		DRM_DEBUG_KMS("failed to allocate edid\n");
		return ERR_PTR(-ENOMEM);
	}

	return edid;
}

static void *vidi_get_panel(struct device *dev)
{
	DRM_DEBUG_KMS("%s\n", __FILE__);

	/* TODO. */

	return NULL;
}

static int vidi_check_timing(struct device *dev, void *timing)
{
	DRM_DEBUG_KMS("%s\n", __FILE__);

	/* TODO. */

	return 0;
}

static int vidi_display_power_on(struct device *dev, int mode)
{
	DRM_DEBUG_KMS("%s\n", __FILE__);

	/* TODO */

	return 0;
}

static struct exynos_drm_display_ops vidi_display_ops = {
	.type = EXYNOS_DISPLAY_TYPE_VIDI,
	.is_connected = vidi_display_is_connected,
	.get_edid = vidi_get_edid,
	.get_panel = vidi_get_panel,
	.check_timing = vidi_check_timing,
	.power_on = vidi_display_power_on,
};

static void vidi_dpms(struct device *subdrv_dev, int mode)
{
	struct vidi_context *ctx = get_vidi_context(subdrv_dev);

	DRM_DEBUG_KMS("%s, %d\n", __FILE__, mode);

	mutex_lock(&ctx->lock);

	switch (mode) {
	case DRM_MODE_DPMS_ON:
		/* TODO. */
		break;
	case DRM_MODE_DPMS_STANDBY:
	case DRM_MODE_DPMS_SUSPEND:
	case DRM_MODE_DPMS_OFF:
		/* TODO. */
		break;
	default:
		DRM_DEBUG_KMS("unspecified mode %d\n", mode);
		break;
	}

	mutex_unlock(&ctx->lock);
}

static void vidi_apply(struct device *subdrv_dev)
{
	struct vidi_context *ctx = get_vidi_context(subdrv_dev);
	struct exynos_drm_manager *mgr = ctx->subdrv.manager;
	struct exynos_drm_manager_ops *mgr_ops = mgr->ops;
	struct exynos_drm_overlay_ops *ovl_ops = mgr->overlay_ops;
	struct vidi_win_data *win_data;
	int i;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	for (i = 0; i < WINDOWS_NR; i++) {
		win_data = &ctx->win_data[i];
		if (win_data->enabled && (ovl_ops && ovl_ops->commit))
			ovl_ops->commit(subdrv_dev, i);
	}

	if (mgr_ops && mgr_ops->commit)
		mgr_ops->commit(subdrv_dev);
}

static void vidi_commit(struct device *dev)
{
	struct vidi_context *ctx = get_vidi_context(dev);

	DRM_DEBUG_KMS("%s\n", __FILE__);
=======
static void vidi_apply(struct exynos_drm_manager *mgr)
{
	struct vidi_context *ctx = mgr->ctx;
	struct exynos_drm_manager_ops *mgr_ops = mgr->ops;
	struct vidi_win_data *win_data;
	int i;

	for (i = 0; i < WINDOWS_NR; i++) {
		win_data = &ctx->win_data[i];
		if (win_data->enabled && (mgr_ops && mgr_ops->win_commit))
			mgr_ops->win_commit(mgr, i);
	}

	if (mgr_ops && mgr_ops->commit)
		mgr_ops->commit(mgr);
}

static void vidi_commit(struct exynos_drm_manager *mgr)
{
	struct vidi_context *ctx = mgr->ctx;
>>>>>>> v3.18

	if (ctx->suspended)
		return;
}

<<<<<<< HEAD
static int vidi_enable_vblank(struct device *dev)
{
	struct vidi_context *ctx = get_vidi_context(dev);

	DRM_DEBUG_KMS("%s\n", __FILE__);
=======
static int vidi_enable_vblank(struct exynos_drm_manager *mgr)
{
	struct vidi_context *ctx = mgr->ctx;
>>>>>>> v3.18

	if (ctx->suspended)
		return -EPERM;

	if (!test_and_set_bit(0, &ctx->irq_flags))
		ctx->vblank_on = true;

	ctx->direct_vblank = true;

	/*
	 * in case of page flip request, vidi_finish_pageflip function
	 * will not be called because direct_vblank is true and then
<<<<<<< HEAD
	 * that function will be called by overlay_ops->commit callback
=======
	 * that function will be called by manager_ops->win_commit callback
>>>>>>> v3.18
	 */
	schedule_work(&ctx->work);

	return 0;
}

<<<<<<< HEAD
static void vidi_disable_vblank(struct device *dev)
{
	struct vidi_context *ctx = get_vidi_context(dev);

	DRM_DEBUG_KMS("%s\n", __FILE__);
=======
static void vidi_disable_vblank(struct exynos_drm_manager *mgr)
{
	struct vidi_context *ctx = mgr->ctx;
>>>>>>> v3.18

	if (ctx->suspended)
		return;

	if (test_and_clear_bit(0, &ctx->irq_flags))
		ctx->vblank_on = false;
}

<<<<<<< HEAD
static struct exynos_drm_manager_ops vidi_manager_ops = {
	.dpms = vidi_dpms,
	.apply = vidi_apply,
	.commit = vidi_commit,
	.enable_vblank = vidi_enable_vblank,
	.disable_vblank = vidi_disable_vblank,
};

static void vidi_win_mode_set(struct device *dev,
			      struct exynos_drm_overlay *overlay)
{
	struct vidi_context *ctx = get_vidi_context(dev);
=======
static void vidi_win_mode_set(struct exynos_drm_manager *mgr,
			struct exynos_drm_overlay *overlay)
{
	struct vidi_context *ctx = mgr->ctx;
>>>>>>> v3.18
	struct vidi_win_data *win_data;
	int win;
	unsigned long offset;

<<<<<<< HEAD
	DRM_DEBUG_KMS("%s\n", __FILE__);

	if (!overlay) {
		dev_err(dev, "overlay is NULL\n");
=======
	if (!overlay) {
		DRM_ERROR("overlay is NULL\n");
>>>>>>> v3.18
		return;
	}

	win = overlay->zpos;
	if (win == DEFAULT_ZPOS)
		win = ctx->default_win;

<<<<<<< HEAD
	if (win < 0 || win > WINDOWS_NR)
=======
	if (win < 0 || win >= WINDOWS_NR)
>>>>>>> v3.18
		return;

	offset = overlay->fb_x * (overlay->bpp >> 3);
	offset += overlay->fb_y * overlay->pitch;

	DRM_DEBUG_KMS("offset = 0x%lx, pitch = %x\n", offset, overlay->pitch);

	win_data = &ctx->win_data[win];

	win_data->offset_x = overlay->crtc_x;
	win_data->offset_y = overlay->crtc_y;
	win_data->ovl_width = overlay->crtc_width;
	win_data->ovl_height = overlay->crtc_height;
	win_data->fb_width = overlay->fb_width;
	win_data->fb_height = overlay->fb_height;
	win_data->dma_addr = overlay->dma_addr[0] + offset;
	win_data->bpp = overlay->bpp;
	win_data->buf_offsize = (overlay->fb_width - overlay->crtc_width) *
				(overlay->bpp >> 3);
	win_data->line_size = overlay->crtc_width * (overlay->bpp >> 3);

	/*
	 * some parts of win_data should be transferred to user side
	 * through specific ioctl.
	 */

	DRM_DEBUG_KMS("offset_x = %d, offset_y = %d\n",
			win_data->offset_x, win_data->offset_y);
	DRM_DEBUG_KMS("ovl_width = %d, ovl_height = %d\n",
			win_data->ovl_width, win_data->ovl_height);
	DRM_DEBUG_KMS("paddr = 0x%lx\n", (unsigned long)win_data->dma_addr);
	DRM_DEBUG_KMS("fb_width = %d, crtc_width = %d\n",
			overlay->fb_width, overlay->crtc_width);
}

<<<<<<< HEAD
static void vidi_win_commit(struct device *dev, int zpos)
{
	struct vidi_context *ctx = get_vidi_context(dev);
	struct vidi_win_data *win_data;
	int win = zpos;

	DRM_DEBUG_KMS("%s\n", __FILE__);

=======
static void vidi_win_commit(struct exynos_drm_manager *mgr, int zpos)
{
	struct vidi_context *ctx = mgr->ctx;
	struct vidi_win_data *win_data;
	int win = zpos;

>>>>>>> v3.18
	if (ctx->suspended)
		return;

	if (win == DEFAULT_ZPOS)
		win = ctx->default_win;

<<<<<<< HEAD
	if (win < 0 || win > WINDOWS_NR)
=======
	if (win < 0 || win >= WINDOWS_NR)
>>>>>>> v3.18
		return;

	win_data = &ctx->win_data[win];

	win_data->enabled = true;

<<<<<<< HEAD
	DRM_DEBUG_KMS("dma_addr = 0x%x\n", win_data->dma_addr);
=======
	DRM_DEBUG_KMS("dma_addr = %pad\n", &win_data->dma_addr);
>>>>>>> v3.18

	if (ctx->vblank_on)
		schedule_work(&ctx->work);
}

<<<<<<< HEAD
static void vidi_win_disable(struct device *dev, int zpos)
{
	struct vidi_context *ctx = get_vidi_context(dev);
	struct vidi_win_data *win_data;
	int win = zpos;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	if (win == DEFAULT_ZPOS)
		win = ctx->default_win;

	if (win < 0 || win > WINDOWS_NR)
=======
static void vidi_win_disable(struct exynos_drm_manager *mgr, int zpos)
{
	struct vidi_context *ctx = mgr->ctx;
	struct vidi_win_data *win_data;
	int win = zpos;

	if (win == DEFAULT_ZPOS)
		win = ctx->default_win;

	if (win < 0 || win >= WINDOWS_NR)
>>>>>>> v3.18
		return;

	win_data = &ctx->win_data[win];
	win_data->enabled = false;

	/* TODO. */
}

<<<<<<< HEAD
static struct exynos_drm_overlay_ops vidi_overlay_ops = {
	.mode_set = vidi_win_mode_set,
	.commit = vidi_win_commit,
	.disable = vidi_win_disable,
};

static struct exynos_drm_manager vidi_manager = {
	.pipe		= -1,
	.ops		= &vidi_manager_ops,
	.overlay_ops	= &vidi_overlay_ops,
	.display_ops	= &vidi_display_ops,
};

static void vidi_fake_vblank_handler(struct work_struct *work)
{
	struct vidi_context *ctx = container_of(work, struct vidi_context,
					work);
	struct exynos_drm_subdrv *subdrv = &ctx->subdrv;
	struct exynos_drm_manager *manager = subdrv->manager;

	if (manager->pipe < 0)
		return;

	/* refresh rate is about 50Hz. */
	usleep_range(16000, 20000);

	mutex_lock(&ctx->lock);

	if (ctx->direct_vblank) {
		drm_handle_vblank(subdrv->drm_dev, manager->pipe);
		ctx->direct_vblank = false;
		mutex_unlock(&ctx->lock);
		return;
	}

	mutex_unlock(&ctx->lock);

	exynos_drm_crtc_finish_pageflip(subdrv->drm_dev, manager->pipe);
}

static int vidi_subdrv_probe(struct drm_device *drm_dev, struct device *dev)
{
	DRM_DEBUG_KMS("%s\n", __FILE__);

	/*
	 * enable drm irq mode.
	 * - with irq_enabled = 1, we can use the vblank feature.
	 *
	 * P.S. note that we wouldn't use drm irq handler but
	 *	just specific driver own one instead because
	 *	drm framework supports only one irq handler.
	 */
	drm_dev->irq_enabled = 1;

	/*
	 * with vblank_disable_allowed = 1, vblank interrupt will be disabled
	 * by drm timer once a current process gives up ownership of
	 * vblank event.(after drm_vblank_put function is called)
	 */
	drm_dev->vblank_disable_allowed = 1;
=======
static int vidi_power_on(struct exynos_drm_manager *mgr, bool enable)
{
	struct vidi_context *ctx = mgr->ctx;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	if (enable != false && enable != true)
		return -EINVAL;

	if (enable) {
		ctx->suspended = false;

		/* if vblank was enabled status, enable it again. */
		if (test_and_clear_bit(0, &ctx->irq_flags))
			vidi_enable_vblank(mgr);

		vidi_apply(mgr);
	} else {
		ctx->suspended = true;
	}

	return 0;
}

static void vidi_dpms(struct exynos_drm_manager *mgr, int mode)
{
	struct vidi_context *ctx = mgr->ctx;

	DRM_DEBUG_KMS("%d\n", mode);

	mutex_lock(&ctx->lock);

	switch (mode) {
	case DRM_MODE_DPMS_ON:
		vidi_power_on(mgr, true);
		break;
	case DRM_MODE_DPMS_STANDBY:
	case DRM_MODE_DPMS_SUSPEND:
	case DRM_MODE_DPMS_OFF:
		vidi_power_on(mgr, false);
		break;
	default:
		DRM_DEBUG_KMS("unspecified mode %d\n", mode);
		break;
	}

	mutex_unlock(&ctx->lock);
}

static int vidi_mgr_initialize(struct exynos_drm_manager *mgr,
			struct drm_device *drm_dev)
{
	struct vidi_context *ctx = mgr->ctx;
	struct exynos_drm_private *priv = drm_dev->dev_private;

	mgr->drm_dev = ctx->drm_dev = drm_dev;
	mgr->pipe = ctx->pipe = priv->pipe++;
>>>>>>> v3.18

	return 0;
}

<<<<<<< HEAD
static void vidi_subdrv_remove(struct drm_device *drm_dev, struct device *dev)
{
	DRM_DEBUG_KMS("%s\n", __FILE__);

	/* TODO. */
}

static int vidi_power_on(struct vidi_context *ctx, bool enable)
{
	struct exynos_drm_subdrv *subdrv = &ctx->subdrv;
	struct device *dev = subdrv->dev;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	if (enable != false && enable != true)
		return -EINVAL;

	if (enable) {
		ctx->suspended = false;

		/* if vblank was enabled status, enable it again. */
		if (test_and_clear_bit(0, &ctx->irq_flags))
			vidi_enable_vblank(dev);

		vidi_apply(dev);
	} else {
		ctx->suspended = true;
	}

	return 0;
=======
static struct exynos_drm_manager_ops vidi_manager_ops = {
	.dpms = vidi_dpms,
	.commit = vidi_commit,
	.enable_vblank = vidi_enable_vblank,
	.disable_vblank = vidi_disable_vblank,
	.win_mode_set = vidi_win_mode_set,
	.win_commit = vidi_win_commit,
	.win_disable = vidi_win_disable,
};

static struct exynos_drm_manager vidi_manager = {
	.type = EXYNOS_DISPLAY_TYPE_VIDI,
	.ops = &vidi_manager_ops,
};

static void vidi_fake_vblank_handler(struct work_struct *work)
{
	struct vidi_context *ctx = container_of(work, struct vidi_context,
					work);

	if (ctx->pipe < 0)
		return;

	/* refresh rate is about 50Hz. */
	usleep_range(16000, 20000);

	mutex_lock(&ctx->lock);

	if (ctx->direct_vblank) {
		drm_handle_vblank(ctx->drm_dev, ctx->pipe);
		ctx->direct_vblank = false;
		mutex_unlock(&ctx->lock);
		return;
	}

	mutex_unlock(&ctx->lock);

	exynos_drm_crtc_finish_pageflip(ctx->drm_dev, ctx->pipe);
>>>>>>> v3.18
}

static int vidi_show_connection(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	int rc;
<<<<<<< HEAD
	struct vidi_context *ctx = get_vidi_context(dev);
=======
	struct exynos_drm_manager *mgr = get_vidi_mgr(dev);
	struct vidi_context *ctx = mgr->ctx;
>>>>>>> v3.18

	mutex_lock(&ctx->lock);

	rc = sprintf(buf, "%d\n", ctx->connected);

	mutex_unlock(&ctx->lock);

	return rc;
}

static int vidi_store_connection(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t len)
{
<<<<<<< HEAD
	struct vidi_context *ctx = get_vidi_context(dev);
	int ret;

	DRM_DEBUG_KMS("%s\n", __FILE__);

=======
	struct exynos_drm_manager *mgr = get_vidi_mgr(dev);
	struct vidi_context *ctx = mgr->ctx;
	int ret;

>>>>>>> v3.18
	ret = kstrtoint(buf, 0, &ctx->connected);
	if (ret)
		return ret;

	if (ctx->connected > 1)
		return -EINVAL;

	/* use fake edid data for test. */
	if (!ctx->raw_edid)
		ctx->raw_edid = (struct edid *)fake_edid_info;

	/* if raw_edid isn't same as fake data then it can't be tested. */
	if (ctx->raw_edid != (struct edid *)fake_edid_info) {
		DRM_DEBUG_KMS("edid data is not fake data.\n");
		return -EINVAL;
	}

	DRM_DEBUG_KMS("requested connection.\n");

<<<<<<< HEAD
	drm_helper_hpd_irq_event(ctx->subdrv.drm_dev);
=======
	drm_helper_hpd_irq_event(ctx->drm_dev);
>>>>>>> v3.18

	return len;
}

static DEVICE_ATTR(connection, 0644, vidi_show_connection,
			vidi_store_connection);

int vidi_connection_ioctl(struct drm_device *drm_dev, void *data,
				struct drm_file *file_priv)
{
	struct vidi_context *ctx = NULL;
	struct drm_encoder *encoder;
<<<<<<< HEAD
	struct exynos_drm_manager *manager;
	struct exynos_drm_display_ops *display_ops;
	struct drm_exynos_vidi_connection *vidi = data;
	int edid_len;

	DRM_DEBUG_KMS("%s\n", __FILE__);
=======
	struct exynos_drm_display *display;
	struct drm_exynos_vidi_connection *vidi = data;
>>>>>>> v3.18

	if (!vidi) {
		DRM_DEBUG_KMS("user data for vidi is null.\n");
		return -EINVAL;
	}

	if (vidi->connection > 1) {
		DRM_DEBUG_KMS("connection should be 0 or 1.\n");
		return -EINVAL;
	}

	list_for_each_entry(encoder, &drm_dev->mode_config.encoder_list,
								head) {
<<<<<<< HEAD
		manager = exynos_drm_get_manager(encoder);
		display_ops = manager->display_ops;

		if (display_ops->type == EXYNOS_DISPLAY_TYPE_VIDI) {
			ctx = get_vidi_context(manager->dev);
=======
		display = exynos_drm_get_display(encoder);

		if (display->type == EXYNOS_DISPLAY_TYPE_VIDI) {
			ctx = display->ctx;
>>>>>>> v3.18
			break;
		}
	}

	if (!ctx) {
		DRM_DEBUG_KMS("not found virtual device type encoder.\n");
		return -EINVAL;
	}

	if (ctx->connected == vidi->connection) {
		DRM_DEBUG_KMS("same connection request.\n");
		return -EINVAL;
	}

	if (vidi->connection) {
		struct edid *raw_edid  = (struct edid *)(uint32_t)vidi->edid;
		if (!drm_edid_is_valid(raw_edid)) {
			DRM_DEBUG_KMS("edid data is invalid.\n");
			return -EINVAL;
		}
<<<<<<< HEAD
		edid_len = (1 + raw_edid->extensions) * EDID_LENGTH;
		ctx->raw_edid = kmemdup(raw_edid, edid_len, GFP_KERNEL);
=======
		ctx->raw_edid = drm_edid_duplicate(raw_edid);
>>>>>>> v3.18
		if (!ctx->raw_edid) {
			DRM_DEBUG_KMS("failed to allocate raw_edid.\n");
			return -ENOMEM;
		}
	} else {
		/*
		 * with connection = 0, free raw_edid
		 * only if raw edid data isn't same as fake data.
		 */
		if (ctx->raw_edid && ctx->raw_edid !=
				(struct edid *)fake_edid_info) {
			kfree(ctx->raw_edid);
			ctx->raw_edid = NULL;
		}
	}

	ctx->connected = vidi->connection;
<<<<<<< HEAD
	drm_helper_hpd_irq_event(ctx->subdrv.drm_dev);
=======
	drm_helper_hpd_irq_event(ctx->drm_dev);
>>>>>>> v3.18

	return 0;
}

<<<<<<< HEAD
static int vidi_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct vidi_context *ctx;
	struct exynos_drm_subdrv *subdrv;
	int ret;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	ctx = devm_kzalloc(dev, sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	ctx->default_win = 0;

	INIT_WORK(&ctx->work, vidi_fake_vblank_handler);

	subdrv = &ctx->subdrv;
	subdrv->dev = dev;
	subdrv->manager = &vidi_manager;
	subdrv->probe = vidi_subdrv_probe;
	subdrv->remove = vidi_subdrv_remove;

	mutex_init(&ctx->lock);

	platform_set_drvdata(pdev, ctx);

	ret = device_create_file(dev, &dev_attr_connection);
	if (ret < 0)
		DRM_INFO("failed to create connection sysfs.\n");

	exynos_drm_subdrv_register(subdrv);
=======
static enum drm_connector_status vidi_detect(struct drm_connector *connector,
			bool force)
{
	struct vidi_context *ctx = ctx_from_connector(connector);

	/*
	 * connection request would come from user side
	 * to do hotplug through specific ioctl.
	 */
	return ctx->connected ? connector_status_connected :
			connector_status_disconnected;
}

static void vidi_connector_destroy(struct drm_connector *connector)
{
}

static struct drm_connector_funcs vidi_connector_funcs = {
	.dpms = drm_helper_connector_dpms,
	.fill_modes = drm_helper_probe_single_connector_modes,
	.detect = vidi_detect,
	.destroy = vidi_connector_destroy,
};

static int vidi_get_modes(struct drm_connector *connector)
{
	struct vidi_context *ctx = ctx_from_connector(connector);
	struct edid *edid;
	int edid_len;

	/*
	 * the edid data comes from user side and it would be set
	 * to ctx->raw_edid through specific ioctl.
	 */
	if (!ctx->raw_edid) {
		DRM_DEBUG_KMS("raw_edid is null.\n");
		return -EFAULT;
	}

	edid_len = (1 + ctx->raw_edid->extensions) * EDID_LENGTH;
	edid = kmemdup(ctx->raw_edid, edid_len, GFP_KERNEL);
	if (!edid) {
		DRM_DEBUG_KMS("failed to allocate edid\n");
		return -ENOMEM;
	}

	drm_mode_connector_update_edid_property(connector, edid);

	return drm_add_edid_modes(connector, edid);
}

static struct drm_encoder *vidi_best_encoder(struct drm_connector *connector)
{
	struct vidi_context *ctx = ctx_from_connector(connector);

	return ctx->encoder;
}

static struct drm_connector_helper_funcs vidi_connector_helper_funcs = {
	.get_modes = vidi_get_modes,
	.best_encoder = vidi_best_encoder,
};

static int vidi_create_connector(struct exynos_drm_display *display,
				struct drm_encoder *encoder)
{
	struct vidi_context *ctx = display->ctx;
	struct drm_connector *connector = &ctx->connector;
	int ret;

	ctx->encoder = encoder;
	connector->polled = DRM_CONNECTOR_POLL_HPD;

	ret = drm_connector_init(ctx->drm_dev, connector,
			&vidi_connector_funcs, DRM_MODE_CONNECTOR_VIRTUAL);
	if (ret) {
		DRM_ERROR("Failed to initialize connector with drm\n");
		return ret;
	}

	drm_connector_helper_add(connector, &vidi_connector_helper_funcs);
	drm_connector_register(connector);
	drm_mode_connector_attach_encoder(connector, encoder);
>>>>>>> v3.18

	return 0;
}

<<<<<<< HEAD
static int vidi_remove(struct platform_device *pdev)
{
	struct vidi_context *ctx = platform_get_drvdata(pdev);

	DRM_DEBUG_KMS("%s\n", __FILE__);

	exynos_drm_subdrv_unregister(&ctx->subdrv);

	if (ctx->raw_edid != (struct edid *)fake_edid_info) {
		kfree(ctx->raw_edid);
		ctx->raw_edid = NULL;
=======

static struct exynos_drm_display_ops vidi_display_ops = {
	.create_connector = vidi_create_connector,
};

static struct exynos_drm_display vidi_display = {
	.type = EXYNOS_DISPLAY_TYPE_VIDI,
	.ops = &vidi_display_ops,
};

static int vidi_subdrv_probe(struct drm_device *drm_dev, struct device *dev)
{
	struct exynos_drm_manager *mgr = get_vidi_mgr(dev);
	struct vidi_context *ctx = mgr->ctx;
	struct drm_crtc *crtc = ctx->crtc;
	int ret;

	vidi_mgr_initialize(mgr, drm_dev);

	ret = exynos_drm_crtc_create(&vidi_manager);
	if (ret) {
		DRM_ERROR("failed to create crtc.\n");
		return ret;
	}

	ret = exynos_drm_create_enc_conn(drm_dev, &vidi_display);
	if (ret) {
		crtc->funcs->destroy(crtc);
		DRM_ERROR("failed to create encoder and connector.\n");
		return ret;
>>>>>>> v3.18
	}

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_PM_SLEEP
static int vidi_suspend(struct device *dev)
{
	struct vidi_context *ctx = get_vidi_context(dev);

	return vidi_power_on(ctx, false);
}

static int vidi_resume(struct device *dev)
{
	struct vidi_context *ctx = get_vidi_context(dev);

	return vidi_power_on(ctx, true);
}
#endif

static const struct dev_pm_ops vidi_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(vidi_suspend, vidi_resume)
};
=======
static int vidi_probe(struct platform_device *pdev)
{
	struct exynos_drm_subdrv *subdrv;
	struct vidi_context *ctx;
	int ret;

	ctx = devm_kzalloc(&pdev->dev, sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	ctx->default_win = 0;

	INIT_WORK(&ctx->work, vidi_fake_vblank_handler);

	vidi_manager.ctx = ctx;
	vidi_display.ctx = ctx;

	mutex_init(&ctx->lock);

	platform_set_drvdata(pdev, &vidi_manager);

	subdrv = &ctx->subdrv;
	subdrv->dev = &pdev->dev;
	subdrv->probe = vidi_subdrv_probe;

	ret = exynos_drm_subdrv_register(subdrv);
	if (ret < 0) {
		dev_err(&pdev->dev, "failed to register drm vidi device\n");
		return ret;
	}

	ret = device_create_file(&pdev->dev, &dev_attr_connection);
	if (ret < 0) {
		exynos_drm_subdrv_unregister(subdrv);
		DRM_INFO("failed to create connection sysfs.\n");
	}

	return 0;
}

static int vidi_remove(struct platform_device *pdev)
{
	struct exynos_drm_manager *mgr = platform_get_drvdata(pdev);
	struct vidi_context *ctx = mgr->ctx;

	if (ctx->raw_edid != (struct edid *)fake_edid_info) {
		kfree(ctx->raw_edid);
		ctx->raw_edid = NULL;

		return -EINVAL;
	}

	return 0;
}
>>>>>>> v3.18

struct platform_driver vidi_driver = {
	.probe		= vidi_probe,
	.remove		= vidi_remove,
	.driver		= {
		.name	= "exynos-drm-vidi",
		.owner	= THIS_MODULE,
<<<<<<< HEAD
		.pm	= &vidi_pm_ops,
	},
};
=======
	},
};

int exynos_drm_probe_vidi(void)
{
	struct platform_device *pdev;
	int ret;

	pdev = platform_device_register_simple("exynos-drm-vidi", -1, NULL, 0);
	if (IS_ERR(pdev))
		return PTR_ERR(pdev);

	ret = platform_driver_register(&vidi_driver);
	if (ret) {
		platform_device_unregister(pdev);
		return ret;
	}

	return ret;
}

void exynos_drm_remove_vidi(void)
{
	struct vidi_context *ctx = vidi_manager.ctx;
	struct exynos_drm_subdrv *subdrv = &ctx->subdrv;
	struct platform_device *pdev = to_platform_device(subdrv->dev);

	platform_driver_unregister(&vidi_driver);
	platform_device_unregister(pdev);
}
>>>>>>> v3.18
