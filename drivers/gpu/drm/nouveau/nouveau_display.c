/*
 * Copyright (C) 2008 Maarten Maathuis.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <drm/drmP.h>
#include <drm/drm_crtc_helper.h>

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <nvif/class.h>

>>>>>>> v3.18
=======
#include <nvif/class.h>

>>>>>>> v3.18
#include "nouveau_fbcon.h"
#include "dispnv04/hw.h"
#include "nouveau_crtc.h"
#include "nouveau_dma.h"
#include "nouveau_gem.h"
#include "nouveau_connector.h"
#include "nv50_display.h"

#include "nouveau_fence.h"

<<<<<<< HEAD
<<<<<<< HEAD
#include <subdev/bios/gpio.h>
#include <subdev/gpio.h>
#include <engine/disp.h>

#include <core/class.h>
=======
=======
>>>>>>> v3.18
#include <nvif/event.h>

static int
nouveau_display_vblank_handler(struct nvif_notify *notify)
{
	struct nouveau_crtc *nv_crtc =
		container_of(notify, typeof(*nv_crtc), vblank);
	drm_handle_vblank(nv_crtc->base.dev, nv_crtc->index);
	return NVIF_NOTIFY_KEEP;
}

int
nouveau_display_vblank_enable(struct drm_device *dev, int head)
{
	struct drm_crtc *crtc;
	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct nouveau_crtc *nv_crtc = nouveau_crtc(crtc);
		if (nv_crtc->index == head) {
			nvif_notify_get(&nv_crtc->vblank);
			return 0;
		}
	}
	return -EINVAL;
}

void
nouveau_display_vblank_disable(struct drm_device *dev, int head)
{
	struct drm_crtc *crtc;
	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct nouveau_crtc *nv_crtc = nouveau_crtc(crtc);
		if (nv_crtc->index == head) {
			nvif_notify_put(&nv_crtc->vblank);
			return;
		}
	}
}

static inline int
calc(int blanks, int blanke, int total, int line)
{
	if (blanke >= blanks) {
		if (line >= blanks)
			line -= total;
	} else {
		if (line >= blanks)
			line -= total;
		line -= blanke + 1;
	}
	return line;
}

int
nouveau_display_scanoutpos_head(struct drm_crtc *crtc, int *vpos, int *hpos,
				ktime_t *stime, ktime_t *etime)
{
	struct {
		struct nv04_disp_mthd_v0 base;
		struct nv04_disp_scanoutpos_v0 scan;
	} args = {
		.base.method = NV04_DISP_SCANOUTPOS,
		.base.head = nouveau_crtc(crtc)->index,
	};
	struct nouveau_display *disp = nouveau_display(crtc->dev);
	int ret, retry = 1;

	do {
		ret = nvif_mthd(&disp->disp, 0, &args, sizeof(args));
		if (ret != 0)
			return 0;

		if (args.scan.vline) {
			ret |= DRM_SCANOUTPOS_ACCURATE;
			ret |= DRM_SCANOUTPOS_VALID;
			break;
		}

		if (retry) ndelay(crtc->linedur_ns);
	} while (retry--);

	*hpos = args.scan.hline;
	*vpos = calc(args.scan.vblanks, args.scan.vblanke,
		     args.scan.vtotal, args.scan.vline);
	if (stime) *stime = ns_to_ktime(args.scan.time[0]);
	if (etime) *etime = ns_to_ktime(args.scan.time[1]);

	if (*vpos < 0)
		ret |= DRM_SCANOUTPOS_IN_VBLANK;
	return ret;
}

int
nouveau_display_scanoutpos(struct drm_device *dev, int head, unsigned int flags,
			   int *vpos, int *hpos, ktime_t *stime, ktime_t *etime)
{
	struct drm_crtc *crtc;

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		if (nouveau_crtc(crtc)->index == head) {
			return nouveau_display_scanoutpos_head(crtc, vpos, hpos,
							       stime, etime);
		}
	}

	return 0;
}

int
nouveau_display_vblstamp(struct drm_device *dev, int head, int *max_error,
			 struct timeval *time, unsigned flags)
{
	struct drm_crtc *crtc;

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		if (nouveau_crtc(crtc)->index == head) {
			return drm_calc_vbltimestamp_from_scanoutpos(dev,
					head, max_error, time, flags, crtc,
					&crtc->hwmode);
		}
	}

	return -EINVAL;
}

static void
nouveau_display_vblank_fini(struct drm_device *dev)
{
	struct drm_crtc *crtc;

	drm_vblank_cleanup(dev);

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct nouveau_crtc *nv_crtc = nouveau_crtc(crtc);
		nvif_notify_fini(&nv_crtc->vblank);
	}
}

static int
nouveau_display_vblank_init(struct drm_device *dev)
{
	struct nouveau_display *disp = nouveau_display(dev);
	struct drm_crtc *crtc;
	int ret;

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct nouveau_crtc *nv_crtc = nouveau_crtc(crtc);
		ret = nvif_notify_init(&disp->disp, NULL,
				       nouveau_display_vblank_handler, false,
				       NV04_DISP_NTFY_VBLANK,
				       &(struct nvif_notify_head_req_v0) {
					.head = nv_crtc->index,
				       },
				       sizeof(struct nvif_notify_head_req_v0),
				       sizeof(struct nvif_notify_head_rep_v0),
				       &nv_crtc->vblank);
		if (ret) {
			nouveau_display_vblank_fini(dev);
			return ret;
		}
	}

	ret = drm_vblank_init(dev, dev->mode_config.num_crtc);
	if (ret) {
		nouveau_display_vblank_fini(dev);
		return ret;
	}

	return 0;
}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

static void
nouveau_user_framebuffer_destroy(struct drm_framebuffer *drm_fb)
{
	struct nouveau_framebuffer *fb = nouveau_framebuffer(drm_fb);
<<<<<<< HEAD
<<<<<<< HEAD

	if (fb->nvbo)
		drm_gem_object_unreference_unlocked(fb->nvbo->gem);
=======
=======
>>>>>>> v3.18
	struct nouveau_display *disp = nouveau_display(drm_fb->dev);

	if (disp->fb_dtor)
		disp->fb_dtor(drm_fb);

	if (fb->nvbo)
		drm_gem_object_unreference_unlocked(&fb->nvbo->gem);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	drm_framebuffer_cleanup(drm_fb);
	kfree(fb);
}

static int
nouveau_user_framebuffer_create_handle(struct drm_framebuffer *drm_fb,
				       struct drm_file *file_priv,
				       unsigned int *handle)
{
	struct nouveau_framebuffer *fb = nouveau_framebuffer(drm_fb);

<<<<<<< HEAD
<<<<<<< HEAD
	return drm_gem_handle_create(file_priv, fb->nvbo->gem, handle);
=======
	return drm_gem_handle_create(file_priv, &fb->nvbo->gem, handle);
>>>>>>> v3.18
=======
	return drm_gem_handle_create(file_priv, &fb->nvbo->gem, handle);
>>>>>>> v3.18
}

static const struct drm_framebuffer_funcs nouveau_framebuffer_funcs = {
	.destroy = nouveau_user_framebuffer_destroy,
	.create_handle = nouveau_user_framebuffer_create_handle,
};

int
nouveau_framebuffer_init(struct drm_device *dev,
			 struct nouveau_framebuffer *nv_fb,
			 struct drm_mode_fb_cmd2 *mode_cmd,
			 struct nouveau_bo *nvbo)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct nouveau_drm *drm = nouveau_drm(dev);
=======
	struct nouveau_display *disp = nouveau_display(dev);
>>>>>>> v3.18
=======
	struct nouveau_display *disp = nouveau_display(dev);
>>>>>>> v3.18
	struct drm_framebuffer *fb = &nv_fb->base;
	int ret;

	drm_helper_mode_fill_fb_struct(fb, mode_cmd);
	nv_fb->nvbo = nvbo;

<<<<<<< HEAD
<<<<<<< HEAD
	if (nv_device(drm->device)->card_type >= NV_50) {
		u32 tile_flags = nouveau_bo_tile_layout(nvbo);
		if (tile_flags == 0x7a00 ||
		    tile_flags == 0xfe00)
			nv_fb->r_dma = NvEvoFB32;
		else
		if (tile_flags == 0x7000)
			nv_fb->r_dma = NvEvoFB16;
		else
			nv_fb->r_dma = NvEvoVRAM_LP;

		switch (fb->depth) {
		case  8: nv_fb->r_format = 0x1e00; break;
		case 15: nv_fb->r_format = 0xe900; break;
		case 16: nv_fb->r_format = 0xe800; break;
		case 24:
		case 32: nv_fb->r_format = 0xcf00; break;
		case 30: nv_fb->r_format = 0xd100; break;
		default:
			 NV_ERROR(drm, "unknown depth %d\n", fb->depth);
			 return -EINVAL;
		}

		if (nv_device(drm->device)->chipset == 0x50)
			nv_fb->r_format |= (tile_flags << 8);

		if (!tile_flags) {
			if (nv_device(drm->device)->card_type < NV_D0)
				nv_fb->r_pitch = 0x00100000 | fb->pitches[0];
			else
				nv_fb->r_pitch = 0x01000000 | fb->pitches[0];
		} else {
			u32 mode = nvbo->tile_mode;
			if (nv_device(drm->device)->card_type >= NV_C0)
				mode >>= 4;
			nv_fb->r_pitch = ((fb->pitches[0] / 4) << 4) | mode;
		}
	}

	ret = drm_framebuffer_init(dev, fb, &nouveau_framebuffer_funcs);
	if (ret) {
		return ret;
	}

	return 0;
=======
=======
>>>>>>> v3.18
	ret = drm_framebuffer_init(dev, fb, &nouveau_framebuffer_funcs);
	if (ret)
		return ret;

	if (disp->fb_ctor) {
		ret = disp->fb_ctor(fb);
		if (ret)
			disp->fb_dtor(fb);
	}

	return ret;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static struct drm_framebuffer *
nouveau_user_framebuffer_create(struct drm_device *dev,
				struct drm_file *file_priv,
				struct drm_mode_fb_cmd2 *mode_cmd)
{
	struct nouveau_framebuffer *nouveau_fb;
	struct drm_gem_object *gem;
<<<<<<< HEAD
<<<<<<< HEAD
	int ret;
=======
	int ret = -ENOMEM;
>>>>>>> v3.18
=======
	int ret = -ENOMEM;
>>>>>>> v3.18

	gem = drm_gem_object_lookup(dev, file_priv, mode_cmd->handles[0]);
	if (!gem)
		return ERR_PTR(-ENOENT);

	nouveau_fb = kzalloc(sizeof(struct nouveau_framebuffer), GFP_KERNEL);
	if (!nouveau_fb)
<<<<<<< HEAD
<<<<<<< HEAD
		return ERR_PTR(-ENOMEM);

	ret = nouveau_framebuffer_init(dev, nouveau_fb, mode_cmd, nouveau_gem_object(gem));
	if (ret) {
		drm_gem_object_unreference(gem);
		return ERR_PTR(ret);
	}

	return &nouveau_fb->base;
=======
=======
>>>>>>> v3.18
		goto err_unref;

	ret = nouveau_framebuffer_init(dev, nouveau_fb, mode_cmd, nouveau_gem_object(gem));
	if (ret)
		goto err;

	return &nouveau_fb->base;

err:
	kfree(nouveau_fb);
err_unref:
	drm_gem_object_unreference(gem);
	return ERR_PTR(ret);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static const struct drm_mode_config_funcs nouveau_mode_config_funcs = {
	.fb_create = nouveau_user_framebuffer_create,
	.output_poll_changed = nouveau_fbcon_output_poll_changed,
};


struct nouveau_drm_prop_enum_list {
	u8 gen_mask;
	int type;
	char *name;
};

static struct nouveau_drm_prop_enum_list underscan[] = {
	{ 6, UNDERSCAN_AUTO, "auto" },
	{ 6, UNDERSCAN_OFF, "off" },
	{ 6, UNDERSCAN_ON, "on" },
	{}
};

static struct nouveau_drm_prop_enum_list dither_mode[] = {
	{ 7, DITHERING_MODE_AUTO, "auto" },
	{ 7, DITHERING_MODE_OFF, "off" },
	{ 1, DITHERING_MODE_ON, "on" },
	{ 6, DITHERING_MODE_STATIC2X2, "static 2x2" },
	{ 6, DITHERING_MODE_DYNAMIC2X2, "dynamic 2x2" },
	{ 4, DITHERING_MODE_TEMPORAL, "temporal" },
	{}
};

static struct nouveau_drm_prop_enum_list dither_depth[] = {
	{ 6, DITHERING_DEPTH_AUTO, "auto" },
	{ 6, DITHERING_DEPTH_6BPC, "6 bpc" },
	{ 6, DITHERING_DEPTH_8BPC, "8 bpc" },
	{}
};

#define PROP_ENUM(p,gen,n,list) do {                                           \
	struct nouveau_drm_prop_enum_list *l = (list);                         \
	int c = 0;                                                             \
	while (l->gen_mask) {                                                  \
		if (l->gen_mask & (1 << (gen)))                                \
			c++;                                                   \
		l++;                                                           \
	}                                                                      \
	if (c) {                                                               \
		p = drm_property_create(dev, DRM_MODE_PROP_ENUM, n, c);        \
		l = (list);                                                    \
		c = 0;                                                         \
		while (p && l->gen_mask) {                                     \
			if (l->gen_mask & (1 << (gen))) {                      \
				drm_property_add_enum(p, c, l->type, l->name); \
				c++;                                           \
			}                                                      \
			l++;                                                   \
		}                                                              \
	}                                                                      \
} while(0)

int
nouveau_display_init(struct drm_device *dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nouveau_display *disp = nouveau_display(dev);
	struct nouveau_gpio *gpio = nouveau_gpio(drm->device);
=======
	struct nouveau_display *disp = nouveau_display(dev);
>>>>>>> v3.18
=======
	struct nouveau_display *disp = nouveau_display(dev);
>>>>>>> v3.18
	struct drm_connector *connector;
	int ret;

	ret = disp->init(dev);
	if (ret)
		return ret;

	/* enable polling for external displays */
	drm_kms_helper_poll_enable(dev);

	/* enable hotplug interrupts */
	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		struct nouveau_connector *conn = nouveau_connector(connector);
<<<<<<< HEAD
<<<<<<< HEAD
		if (gpio && conn->hpd.func != DCB_GPIO_UNUSED) {
			nouveau_event_get(gpio->events, conn->hpd.line,
					 &conn->hpd_func);
		}
=======
		nvif_notify_get(&conn->hpd);
>>>>>>> v3.18
=======
		nvif_notify_get(&conn->hpd);
>>>>>>> v3.18
	}

	return ret;
}

void
nouveau_display_fini(struct drm_device *dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nouveau_display *disp = nouveau_display(dev);
	struct nouveau_gpio *gpio = nouveau_gpio(drm->device);
	struct drm_connector *connector;
=======
=======
>>>>>>> v3.18
	struct nouveau_display *disp = nouveau_display(dev);
	struct drm_connector *connector;
	int head;

	/* Make sure that drm and hw vblank irqs get properly disabled. */
	for (head = 0; head < dev->mode_config.num_crtc; head++)
		drm_vblank_off(dev, head);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* disable hotplug interrupts */
	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		struct nouveau_connector *conn = nouveau_connector(connector);
<<<<<<< HEAD
<<<<<<< HEAD
		if (gpio && conn->hpd.func != DCB_GPIO_UNUSED) {
			nouveau_event_put(gpio->events, conn->hpd.line,
					 &conn->hpd_func);
		}
=======
		nvif_notify_put(&conn->hpd);
>>>>>>> v3.18
=======
		nvif_notify_put(&conn->hpd);
>>>>>>> v3.18
	}

	drm_kms_helper_poll_disable(dev);
	disp->fini(dev);
}

<<<<<<< HEAD
<<<<<<< HEAD
int
nouveau_display_create(struct drm_device *dev)
{
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nouveau_display *disp;
	u32 pclass = dev->pdev->class >> 8;
	int ret, gen;

	disp = drm->display = kzalloc(sizeof(*disp), GFP_KERNEL);
	if (!disp)
		return -ENOMEM;

	drm_mode_config_init(dev);
	drm_mode_create_scaling_mode_property(dev);
	drm_mode_create_dvi_i_properties(dev);

	if (nv_device(drm->device)->card_type < NV_50)
		gen = 0;
	else
	if (nv_device(drm->device)->card_type < NV_D0)
=======
=======
>>>>>>> v3.18
static void
nouveau_display_create_properties(struct drm_device *dev)
{
	struct nouveau_display *disp = nouveau_display(dev);
	int gen;

	if (disp->disp.oclass < NV50_DISP)
		gen = 0;
	else
	if (disp->disp.oclass < GF110_DISP)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		gen = 1;
	else
		gen = 2;

	PROP_ENUM(disp->dithering_mode, gen, "dithering mode", dither_mode);
	PROP_ENUM(disp->dithering_depth, gen, "dithering depth", dither_depth);
	PROP_ENUM(disp->underscan_property, gen, "underscan", underscan);

	disp->underscan_hborder_property =
		drm_property_create_range(dev, 0, "underscan hborder", 0, 128);

	disp->underscan_vborder_property =
		drm_property_create_range(dev, 0, "underscan vborder", 0, 128);

<<<<<<< HEAD
<<<<<<< HEAD
	if (gen >= 1) {
		/* -90..+90 */
		disp->vibrant_hue_property =
			drm_property_create_range(dev, 0, "vibrant hue", 0, 180);

		/* -100..+100 */
		disp->color_vibrance_property =
			drm_property_create_range(dev, 0, "color vibrance", 0, 200);
	}

	dev->mode_config.funcs = &nouveau_mode_config_funcs;
	dev->mode_config.fb_base = pci_resource_start(dev->pdev, 1);

	dev->mode_config.min_width = 0;
	dev->mode_config.min_height = 0;
	if (nv_device(drm->device)->card_type < NV_10) {
		dev->mode_config.max_width = 2048;
		dev->mode_config.max_height = 2048;
	} else
	if (nv_device(drm->device)->card_type < NV_50) {
=======
=======
>>>>>>> v3.18
	if (gen < 1)
		return;

	/* -90..+90 */
	disp->vibrant_hue_property =
		drm_property_create_range(dev, 0, "vibrant hue", 0, 180);

	/* -100..+100 */
	disp->color_vibrance_property =
		drm_property_create_range(dev, 0, "color vibrance", 0, 200);
}

int
nouveau_display_create(struct drm_device *dev)
{
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nouveau_display *disp;
	int ret;

	disp = drm->display = kzalloc(sizeof(*disp), GFP_KERNEL);
	if (!disp)
		return -ENOMEM;

	drm_mode_config_init(dev);
	drm_mode_create_scaling_mode_property(dev);
	drm_mode_create_dvi_i_properties(dev);

	dev->mode_config.funcs = &nouveau_mode_config_funcs;
	dev->mode_config.fb_base = nv_device_resource_start(nvkm_device(&drm->device), 1);

	dev->mode_config.min_width = 0;
	dev->mode_config.min_height = 0;
	if (drm->device.info.family < NV_DEVICE_INFO_V0_CELSIUS) {
		dev->mode_config.max_width = 2048;
		dev->mode_config.max_height = 2048;
	} else
	if (drm->device.info.family < NV_DEVICE_INFO_V0_TESLA) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		dev->mode_config.max_width = 4096;
		dev->mode_config.max_height = 4096;
	} else {
		dev->mode_config.max_width = 8192;
		dev->mode_config.max_height = 8192;
	}

	dev->mode_config.preferred_depth = 24;
	dev->mode_config.prefer_shadow = 1;

<<<<<<< HEAD
<<<<<<< HEAD
	drm_kms_helper_poll_init(dev);
	drm_kms_helper_poll_disable(dev);

	if (nouveau_modeset == 1 ||
	    (nouveau_modeset < 0 && pclass == PCI_CLASS_DISPLAY_VGA)) {
		if (nv_device(drm->device)->card_type < NV_50)
			ret = nv04_display_create(dev);
		else
			ret = nv50_display_create(dev);
		if (ret)
			goto disp_create_err;

		if (dev->mode_config.num_crtc) {
			ret = drm_vblank_init(dev, dev->mode_config.num_crtc);
			if (ret)
				goto vblank_err;
		}

		nouveau_backlight_init(dev);
	}

=======
=======
>>>>>>> v3.18
	if (drm->device.info.chipset < 0x11)
		dev->mode_config.async_page_flip = false;
	else
		dev->mode_config.async_page_flip = true;

	drm_kms_helper_poll_init(dev);
	drm_kms_helper_poll_disable(dev);

	if (nouveau_modeset != 2 && drm->vbios.dcb.entries) {
		static const u16 oclass[] = {
			GM107_DISP,
			GK110_DISP,
			GK104_DISP,
			GF110_DISP,
			GT214_DISP,
			GT206_DISP,
			GT200_DISP,
			G82_DISP,
			NV50_DISP,
			NV04_DISP,
		};
		int i;

		for (i = 0, ret = -ENODEV; ret && i < ARRAY_SIZE(oclass); i++) {
			ret = nvif_object_init(nvif_object(&drm->device), NULL,
					       NVDRM_DISPLAY, oclass[i],
					       NULL, 0, &disp->disp);
		}

		if (ret == 0) {
			nouveau_display_create_properties(dev);
			if (disp->disp.oclass < NV50_DISP)
				ret = nv04_display_create(dev);
			else
				ret = nv50_display_create(dev);
		}
	} else {
		ret = 0;
	}

	if (ret)
		goto disp_create_err;

	if (dev->mode_config.num_crtc) {
		ret = nouveau_display_vblank_init(dev);
		if (ret)
			goto vblank_err;
	}

	nouveau_backlight_init(dev);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;

vblank_err:
	disp->dtor(dev);
disp_create_err:
	drm_kms_helper_poll_fini(dev);
	drm_mode_config_cleanup(dev);
	return ret;
}

void
nouveau_display_destroy(struct drm_device *dev)
{
	struct nouveau_display *disp = nouveau_display(dev);

	nouveau_backlight_exit(dev);
<<<<<<< HEAD
<<<<<<< HEAD
	drm_vblank_cleanup(dev);
=======
	nouveau_display_vblank_fini(dev);
>>>>>>> v3.18
=======
	nouveau_display_vblank_fini(dev);
>>>>>>> v3.18

	drm_kms_helper_poll_fini(dev);
	drm_mode_config_cleanup(dev);

	if (disp->dtor)
		disp->dtor(dev);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	nvif_object_fini(&disp->disp);

>>>>>>> v3.18
=======
	nvif_object_fini(&disp->disp);

>>>>>>> v3.18
	nouveau_drm(dev)->display = NULL;
	kfree(disp);
}

int
<<<<<<< HEAD
<<<<<<< HEAD
nouveau_display_suspend(struct drm_device *dev)
{
	struct nouveau_drm *drm = nouveau_drm(dev);
=======
nouveau_display_suspend(struct drm_device *dev, bool runtime)
{
>>>>>>> v3.18
=======
nouveau_display_suspend(struct drm_device *dev, bool runtime)
{
>>>>>>> v3.18
	struct drm_crtc *crtc;

	nouveau_display_fini(dev);

<<<<<<< HEAD
<<<<<<< HEAD
	NV_INFO(drm, "unpinning framebuffer(s)...\n");
	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct nouveau_framebuffer *nouveau_fb;

		nouveau_fb = nouveau_framebuffer(crtc->fb);
=======
=======
>>>>>>> v3.18
	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct nouveau_framebuffer *nouveau_fb;

		nouveau_fb = nouveau_framebuffer(crtc->primary->fb);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		if (!nouveau_fb || !nouveau_fb->nvbo)
			continue;

		nouveau_bo_unpin(nouveau_fb->nvbo);
	}

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct nouveau_crtc *nv_crtc = nouveau_crtc(crtc);

		nouveau_bo_unmap(nv_crtc->cursor.nvbo);
		nouveau_bo_unpin(nv_crtc->cursor.nvbo);
	}

	return 0;
}

void
<<<<<<< HEAD
<<<<<<< HEAD
nouveau_display_resume(struct drm_device *dev)
{
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct drm_crtc *crtc;
	int ret;

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct nouveau_framebuffer *nouveau_fb;

		nouveau_fb = nouveau_framebuffer(crtc->fb);
		if (!nouveau_fb || !nouveau_fb->nvbo)
			continue;

		nouveau_bo_pin(nouveau_fb->nvbo, TTM_PL_FLAG_VRAM);
=======
=======
>>>>>>> v3.18
nouveau_display_resume(struct drm_device *dev, bool runtime)
{
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct drm_crtc *crtc;
	int ret, head;

	/* re-pin fb/cursors */
	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct nouveau_framebuffer *nouveau_fb;

		nouveau_fb = nouveau_framebuffer(crtc->primary->fb);
		if (!nouveau_fb || !nouveau_fb->nvbo)
			continue;

		ret = nouveau_bo_pin(nouveau_fb->nvbo, TTM_PL_FLAG_VRAM);
		if (ret)
			NV_ERROR(drm, "Could not pin framebuffer\n");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct nouveau_crtc *nv_crtc = nouveau_crtc(crtc);

		ret = nouveau_bo_pin(nv_crtc->cursor.nvbo, TTM_PL_FLAG_VRAM);
		if (!ret)
			ret = nouveau_bo_map(nv_crtc->cursor.nvbo);
		if (ret)
			NV_ERROR(drm, "Could not pin/map cursor.\n");
	}

<<<<<<< HEAD
<<<<<<< HEAD
	nouveau_fbcon_set_suspend(dev, 0);
	nouveau_fbcon_zfill_all(dev);

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	nouveau_display_init(dev);

	/* Force CLUT to get re-loaded during modeset */
	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct nouveau_crtc *nv_crtc = nouveau_crtc(crtc);

		nv_crtc->lut.depth = 0;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	/* Make sure that drm and hw vblank irqs get resumed if needed. */
	for (head = 0; head < dev->mode_config.num_crtc; head++)
		drm_vblank_on(dev, head);

	/* This should ensure we don't hit a locking problem when someone
	 * wakes us up via a connector.  We should never go into suspend
	 * while the display is on anyways.
	 */
	if (runtime)
		return;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	drm_helper_resume_force_mode(dev);

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct nouveau_crtc *nv_crtc = nouveau_crtc(crtc);
		u32 offset = nv_crtc->cursor.nvbo->bo.offset;

		nv_crtc->cursor.set_offset(nv_crtc, offset);
		nv_crtc->cursor.set_pos(nv_crtc, nv_crtc->cursor_saved_x,
						 nv_crtc->cursor_saved_y);
	}
}

static int
<<<<<<< HEAD
<<<<<<< HEAD
nouveau_page_flip_reserve(struct nouveau_bo *old_bo,
			  struct nouveau_bo *new_bo)
{
	int ret;

	ret = nouveau_bo_pin(new_bo, TTM_PL_FLAG_VRAM);
	if (ret)
		return ret;

	ret = ttm_bo_reserve(&new_bo->bo, false, false, false, 0);
	if (ret)
		goto fail;

	if (likely(old_bo != new_bo)) {
		ret = ttm_bo_reserve(&old_bo->bo, false, false, false, 0);
		if (ret)
			goto fail_unreserve;
	}

	return 0;

fail_unreserve:
	ttm_bo_unreserve(&new_bo->bo);
fail:
	nouveau_bo_unpin(new_bo);
	return ret;
}

static void
nouveau_page_flip_unreserve(struct nouveau_bo *old_bo,
			    struct nouveau_bo *new_bo,
			    struct nouveau_fence *fence)
{
	nouveau_bo_fence(new_bo, fence);
	ttm_bo_unreserve(&new_bo->bo);

	if (likely(old_bo != new_bo)) {
		nouveau_bo_fence(old_bo, fence);
		ttm_bo_unreserve(&old_bo->bo);
	}

	nouveau_bo_unpin(old_bo);
}

static int
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
nouveau_page_flip_emit(struct nouveau_channel *chan,
		       struct nouveau_bo *old_bo,
		       struct nouveau_bo *new_bo,
		       struct nouveau_page_flip_state *s,
		       struct nouveau_fence **pfence)
{
	struct nouveau_fence_chan *fctx = chan->fence;
	struct nouveau_drm *drm = chan->drm;
	struct drm_device *dev = drm->dev;
	unsigned long flags;
	int ret;

	/* Queue it to the pending list */
	spin_lock_irqsave(&dev->event_lock, flags);
	list_add_tail(&s->head, &fctx->flip);
	spin_unlock_irqrestore(&dev->event_lock, flags);

	/* Synchronize with the old framebuffer */
<<<<<<< HEAD
<<<<<<< HEAD
	ret = nouveau_fence_sync(old_bo->bo.sync_obj, chan);
=======
	ret = nouveau_fence_sync(old_bo, chan, false, false);
>>>>>>> v3.18
=======
	ret = nouveau_fence_sync(old_bo, chan, false, false);
>>>>>>> v3.18
	if (ret)
		goto fail;

	/* Emit the pageflip */
<<<<<<< HEAD
<<<<<<< HEAD
	ret = RING_SPACE(chan, 3);
	if (ret)
		goto fail;

	if (nv_device(drm->device)->card_type < NV_C0) {
		BEGIN_NV04(chan, NvSubSw, NV_SW_PAGE_FLIP, 1);
		OUT_RING  (chan, 0x00000000);
		OUT_RING  (chan, 0x00000000);
	} else {
		BEGIN_NVC0(chan, 0, NV10_SUBCHAN_REF_CNT, 1);
		OUT_RING  (chan, 0);
		BEGIN_IMC0(chan, 0, NVSW_SUBCHAN_PAGE_FLIP, 0x0000);
	}
=======
=======
>>>>>>> v3.18
	ret = RING_SPACE(chan, 2);
	if (ret)
		goto fail;

	if (drm->device.info.family < NV_DEVICE_INFO_V0_FERMI)
		BEGIN_NV04(chan, NvSubSw, NV_SW_PAGE_FLIP, 1);
	else
		BEGIN_NVC0(chan, FermiSw, NV_SW_PAGE_FLIP, 1);
	OUT_RING  (chan, 0x00000000);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	FIRE_RING (chan);

	ret = nouveau_fence_new(chan, false, pfence);
	if (ret)
		goto fail;

	return 0;
fail:
	spin_lock_irqsave(&dev->event_lock, flags);
	list_del(&s->head);
	spin_unlock_irqrestore(&dev->event_lock, flags);
	return ret;
}

int
nouveau_crtc_page_flip(struct drm_crtc *crtc, struct drm_framebuffer *fb,
<<<<<<< HEAD
<<<<<<< HEAD
		       struct drm_pending_vblank_event *event)
{
	struct drm_device *dev = crtc->dev;
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nouveau_bo *old_bo = nouveau_framebuffer(crtc->fb)->nvbo;
	struct nouveau_bo *new_bo = nouveau_framebuffer(fb)->nvbo;
	struct nouveau_page_flip_state *s;
	struct nouveau_channel *chan = NULL;
	struct nouveau_fence *fence;
	int ret;

	if (!drm->channel)
		return -ENODEV;
=======
=======
>>>>>>> v3.18
		       struct drm_pending_vblank_event *event, u32 flags)
{
	const int swap_interval = (flags & DRM_MODE_PAGE_FLIP_ASYNC) ? 0 : 1;
	struct drm_device *dev = crtc->dev;
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nouveau_bo *old_bo = nouveau_framebuffer(crtc->primary->fb)->nvbo;
	struct nouveau_bo *new_bo = nouveau_framebuffer(fb)->nvbo;
	struct nouveau_page_flip_state *s;
	struct nouveau_channel *chan;
	struct nouveau_cli *cli;
	struct nouveau_fence *fence;
	int ret;

	chan = drm->channel;
	if (!chan)
		return -ENODEV;
	cli = (void *)nvif_client(&chan->device->base);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	s = kzalloc(sizeof(*s), GFP_KERNEL);
	if (!s)
		return -ENOMEM;

<<<<<<< HEAD
<<<<<<< HEAD
	/* Don't let the buffers go away while we flip */
	ret = nouveau_page_flip_reserve(old_bo, new_bo);
	if (ret)
		goto fail_free;
=======
=======
>>>>>>> v3.18
	if (new_bo != old_bo) {
		ret = nouveau_bo_pin(new_bo, TTM_PL_FLAG_VRAM);
		if (ret)
			goto fail_free;
	}

	mutex_lock(&cli->mutex);
	ret = ttm_bo_reserve(&new_bo->bo, true, false, false, NULL);
	if (ret)
		goto fail_unpin;

	/* synchronise rendering channel with the kernel's channel */
	ret = nouveau_fence_sync(new_bo, chan, false, true);
	if (ret) {
		ttm_bo_unreserve(&new_bo->bo);
		goto fail_unpin;
	}

	if (new_bo != old_bo) {
		ttm_bo_unreserve(&new_bo->bo);

		ret = ttm_bo_reserve(&old_bo->bo, true, false, false, NULL);
		if (ret)
			goto fail_unpin;
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* Initialize a page flip struct */
	*s = (struct nouveau_page_flip_state)
		{ { }, event, nouveau_crtc(crtc)->index,
		  fb->bits_per_pixel, fb->pitches[0], crtc->x, crtc->y,
		  new_bo->bo.offset };

<<<<<<< HEAD
<<<<<<< HEAD
	/* Choose the channel the flip will be handled in */
	fence = new_bo->bo.sync_obj;
	if (fence)
		chan = fence->channel;
	if (!chan)
		chan = drm->channel;
	mutex_lock(&chan->cli->mutex);

	/* Emit a page flip */
	if (nv_device(drm->device)->card_type >= NV_50) {
		ret = nv50_display_flip_next(crtc, fb, chan, 0);
		if (ret) {
			mutex_unlock(&chan->cli->mutex);
			goto fail_unreserve;
		}
	}

	ret = nouveau_page_flip_emit(chan, old_bo, new_bo, s, &fence);
	mutex_unlock(&chan->cli->mutex);
	if (ret)
		goto fail_unreserve;

	/* Update the crtc struct and cleanup */
	crtc->fb = fb;

	nouveau_page_flip_unreserve(old_bo, new_bo, fence);
=======
=======
>>>>>>> v3.18
	/* Keep vblanks on during flip, for the target crtc of this flip */
	drm_vblank_get(dev, nouveau_crtc(crtc)->index);

	/* Emit a page flip */
	if (drm->device.info.family >= NV_DEVICE_INFO_V0_TESLA) {
		ret = nv50_display_flip_next(crtc, fb, chan, swap_interval);
		if (ret)
			goto fail_unreserve;
	} else {
		struct nv04_display *dispnv04 = nv04_display(dev);
		int head = nouveau_crtc(crtc)->index;

		if (swap_interval) {
			ret = RING_SPACE(chan, 8);
			if (ret)
				goto fail_unreserve;

			BEGIN_NV04(chan, NvSubImageBlit, 0x012c, 1);
			OUT_RING  (chan, 0);
			BEGIN_NV04(chan, NvSubImageBlit, 0x0134, 1);
			OUT_RING  (chan, head);
			BEGIN_NV04(chan, NvSubImageBlit, 0x0100, 1);
			OUT_RING  (chan, 0);
			BEGIN_NV04(chan, NvSubImageBlit, 0x0130, 1);
			OUT_RING  (chan, 0);
		}

		nouveau_bo_ref(new_bo, &dispnv04->image[head]);
	}

	ret = nouveau_page_flip_emit(chan, old_bo, new_bo, s, &fence);
	if (ret)
		goto fail_unreserve;
	mutex_unlock(&cli->mutex);

	/* Update the crtc struct and cleanup */
	crtc->primary->fb = fb;

	nouveau_bo_fence(old_bo, fence, false);
	ttm_bo_unreserve(&old_bo->bo);
	if (old_bo != new_bo)
		nouveau_bo_unpin(old_bo);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	nouveau_fence_unref(&fence);
	return 0;

fail_unreserve:
<<<<<<< HEAD
<<<<<<< HEAD
	nouveau_page_flip_unreserve(old_bo, new_bo, NULL);
=======
=======
>>>>>>> v3.18
	drm_vblank_put(dev, nouveau_crtc(crtc)->index);
	ttm_bo_unreserve(&old_bo->bo);
fail_unpin:
	mutex_unlock(&cli->mutex);
	if (old_bo != new_bo)
		nouveau_bo_unpin(new_bo);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
fail_free:
	kfree(s);
	return ret;
}

int
nouveau_finish_page_flip(struct nouveau_channel *chan,
			 struct nouveau_page_flip_state *ps)
{
	struct nouveau_fence_chan *fctx = chan->fence;
	struct nouveau_drm *drm = chan->drm;
	struct drm_device *dev = drm->dev;
	struct nouveau_page_flip_state *s;
	unsigned long flags;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	int crtcid = -1;
>>>>>>> v3.18
=======
	int crtcid = -1;
>>>>>>> v3.18

	spin_lock_irqsave(&dev->event_lock, flags);

	if (list_empty(&fctx->flip)) {
		NV_ERROR(drm, "unexpected pageflip\n");
		spin_unlock_irqrestore(&dev->event_lock, flags);
		return -EINVAL;
	}

	s = list_first_entry(&fctx->flip, struct nouveau_page_flip_state, head);
<<<<<<< HEAD
<<<<<<< HEAD
	if (s->event)
		drm_send_vblank_event(dev, -1, s->event);
=======
=======
>>>>>>> v3.18
	if (s->event) {
		/* Vblank timestamps/counts are only correct on >= NV-50 */
		if (drm->device.info.family >= NV_DEVICE_INFO_V0_TESLA)
			crtcid = s->crtc;

		drm_send_vblank_event(dev, crtcid, s->event);
	}

	/* Give up ownership of vblank for page-flipped crtc */
	drm_vblank_put(dev, s->crtc);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	list_del(&s->head);
	if (ps)
		*ps = *s;
	kfree(s);

	spin_unlock_irqrestore(&dev->event_lock, flags);
	return 0;
}

int
nouveau_flip_complete(void *data)
{
	struct nouveau_channel *chan = data;
	struct nouveau_drm *drm = chan->drm;
	struct nouveau_page_flip_state state;

	if (!nouveau_finish_page_flip(chan, &state)) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (nv_device(drm->device)->card_type < NV_50) {
=======
		if (drm->device.info.family < NV_DEVICE_INFO_V0_TESLA) {
>>>>>>> v3.18
=======
		if (drm->device.info.family < NV_DEVICE_INFO_V0_TESLA) {
>>>>>>> v3.18
			nv_set_crtc_base(drm->dev, state.crtc, state.offset +
					 state.y * state.pitch +
					 state.x * state.bpp / 8);
		}
	}

	return 0;
}

int
nouveau_display_dumb_create(struct drm_file *file_priv, struct drm_device *dev,
			    struct drm_mode_create_dumb *args)
{
	struct nouveau_bo *bo;
	int ret;

	args->pitch = roundup(args->width * (args->bpp / 8), 256);
	args->size = args->pitch * args->height;
	args->size = roundup(args->size, PAGE_SIZE);

	ret = nouveau_gem_new(dev, args->size, 0, NOUVEAU_GEM_DOMAIN_VRAM, 0, 0, &bo);
	if (ret)
		return ret;

<<<<<<< HEAD
<<<<<<< HEAD
	ret = drm_gem_handle_create(file_priv, bo->gem, &args->handle);
	drm_gem_object_unreference_unlocked(bo->gem);
=======
	ret = drm_gem_handle_create(file_priv, &bo->gem, &args->handle);
	drm_gem_object_unreference_unlocked(&bo->gem);
>>>>>>> v3.18
=======
	ret = drm_gem_handle_create(file_priv, &bo->gem, &args->handle);
	drm_gem_object_unreference_unlocked(&bo->gem);
>>>>>>> v3.18
	return ret;
}

int
<<<<<<< HEAD
<<<<<<< HEAD
nouveau_display_dumb_destroy(struct drm_file *file_priv, struct drm_device *dev,
			     uint32_t handle)
{
	return drm_gem_handle_delete(file_priv, handle);
}

int
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
nouveau_display_dumb_map_offset(struct drm_file *file_priv,
				struct drm_device *dev,
				uint32_t handle, uint64_t *poffset)
{
	struct drm_gem_object *gem;

	gem = drm_gem_object_lookup(dev, file_priv, handle);
	if (gem) {
<<<<<<< HEAD
<<<<<<< HEAD
		struct nouveau_bo *bo = gem->driver_private;
		*poffset = bo->bo.addr_space_offset;
=======
		struct nouveau_bo *bo = nouveau_gem_object(gem);
		*poffset = drm_vma_node_offset_addr(&bo->bo.vma_node);
>>>>>>> v3.18
=======
		struct nouveau_bo *bo = nouveau_gem_object(gem);
		*poffset = drm_vma_node_offset_addr(&bo->bo.vma_node);
>>>>>>> v3.18
		drm_gem_object_unreference_unlocked(gem);
		return 0;
	}

	return -ENOENT;
}
