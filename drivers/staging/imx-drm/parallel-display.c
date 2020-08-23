/*
 * i.MX drm driver - parallel display implementation
 *
 * Copyright (C) 2012 Sascha Hauer, Pengutronix
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

<<<<<<< HEAD
=======
#include <linux/component.h>
>>>>>>> v3.18
#include <linux/module.h>
#include <drm/drmP.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_crtc_helper.h>
<<<<<<< HEAD
#include <linux/videodev2.h>
#include <linux/pinctrl/consumer.h>
=======
#include <drm/drm_panel.h>
#include <linux/videodev2.h>
#include <video/of_display_timing.h>
>>>>>>> v3.18

#include "imx-drm.h"

#define con_to_imxpd(x) container_of(x, struct imx_parallel_display, connector)
#define enc_to_imxpd(x) container_of(x, struct imx_parallel_display, encoder)

struct imx_parallel_display {
	struct drm_connector connector;
<<<<<<< HEAD
	struct imx_drm_connector *imx_drm_connector;
	struct drm_encoder encoder;
	struct imx_drm_encoder *imx_drm_encoder;
=======
	struct drm_encoder encoder;
>>>>>>> v3.18
	struct device *dev;
	void *edid;
	int edid_len;
	u32 interface_pix_fmt;
	int mode_valid;
	struct drm_display_mode mode;
<<<<<<< HEAD
=======
	struct drm_panel *panel;
>>>>>>> v3.18
};

static enum drm_connector_status imx_pd_connector_detect(
		struct drm_connector *connector, bool force)
{
	return connector_status_connected;
}

<<<<<<< HEAD
static void imx_pd_connector_destroy(struct drm_connector *connector)
{
	/* do not free here */
}

=======
>>>>>>> v3.18
static int imx_pd_connector_get_modes(struct drm_connector *connector)
{
	struct imx_parallel_display *imxpd = con_to_imxpd(connector);
	struct device_node *np = imxpd->dev->of_node;
	int num_modes = 0;

<<<<<<< HEAD
=======
	if (imxpd->panel && imxpd->panel->funcs &&
	    imxpd->panel->funcs->get_modes) {
		num_modes = imxpd->panel->funcs->get_modes(imxpd->panel);
		if (num_modes > 0)
			return num_modes;
	}

>>>>>>> v3.18
	if (imxpd->edid) {
		drm_mode_connector_update_edid_property(connector, imxpd->edid);
		num_modes = drm_add_edid_modes(connector, imxpd->edid);
	}

	if (imxpd->mode_valid) {
		struct drm_display_mode *mode = drm_mode_create(connector->dev);
<<<<<<< HEAD
=======

		if (!mode)
			return -EINVAL;
>>>>>>> v3.18
		drm_mode_copy(mode, &imxpd->mode);
		mode->type |= DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED,
		drm_mode_probed_add(connector, mode);
		num_modes++;
	}

	if (np) {
		struct drm_display_mode *mode = drm_mode_create(connector->dev);
<<<<<<< HEAD
		of_get_drm_display_mode(np, &imxpd->mode, 0);
=======

		if (!mode)
			return -EINVAL;
		of_get_drm_display_mode(np, &imxpd->mode, OF_USE_NATIVE_MODE);
>>>>>>> v3.18
		drm_mode_copy(mode, &imxpd->mode);
		mode->type |= DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED,
		drm_mode_probed_add(connector, mode);
		num_modes++;
	}

	return num_modes;
}

<<<<<<< HEAD
static int imx_pd_connector_mode_valid(struct drm_connector *connector,
			  struct drm_display_mode *mode)
{
	return 0;
}

=======
>>>>>>> v3.18
static struct drm_encoder *imx_pd_connector_best_encoder(
		struct drm_connector *connector)
{
	struct imx_parallel_display *imxpd = con_to_imxpd(connector);

	return &imxpd->encoder;
}

static void imx_pd_encoder_dpms(struct drm_encoder *encoder, int mode)
{
<<<<<<< HEAD
=======
	struct imx_parallel_display *imxpd = enc_to_imxpd(encoder);

	if (mode != DRM_MODE_DPMS_ON)
		drm_panel_disable(imxpd->panel);
	else
		drm_panel_enable(imxpd->panel);
>>>>>>> v3.18
}

static bool imx_pd_encoder_mode_fixup(struct drm_encoder *encoder,
			   const struct drm_display_mode *mode,
			   struct drm_display_mode *adjusted_mode)
{
	return true;
}

static void imx_pd_encoder_prepare(struct drm_encoder *encoder)
{
	struct imx_parallel_display *imxpd = enc_to_imxpd(encoder);

<<<<<<< HEAD
	imx_drm_crtc_panel_format(encoder->crtc, DRM_MODE_ENCODER_NONE,
			imxpd->interface_pix_fmt);
=======
	imx_drm_panel_format(encoder, imxpd->interface_pix_fmt);
>>>>>>> v3.18
}

static void imx_pd_encoder_commit(struct drm_encoder *encoder)
{
}

static void imx_pd_encoder_mode_set(struct drm_encoder *encoder,
			 struct drm_display_mode *mode,
			 struct drm_display_mode *adjusted_mode)
{
}

static void imx_pd_encoder_disable(struct drm_encoder *encoder)
{
}

<<<<<<< HEAD
static void imx_pd_encoder_destroy(struct drm_encoder *encoder)
{
	/* do not free here */
}

=======
>>>>>>> v3.18
static struct drm_connector_funcs imx_pd_connector_funcs = {
	.dpms = drm_helper_connector_dpms,
	.fill_modes = drm_helper_probe_single_connector_modes,
	.detect = imx_pd_connector_detect,
<<<<<<< HEAD
	.destroy = imx_pd_connector_destroy,
=======
	.destroy = imx_drm_connector_destroy,
>>>>>>> v3.18
};

static struct drm_connector_helper_funcs imx_pd_connector_helper_funcs = {
	.get_modes = imx_pd_connector_get_modes,
	.best_encoder = imx_pd_connector_best_encoder,
<<<<<<< HEAD
	.mode_valid = imx_pd_connector_mode_valid,
};

static struct drm_encoder_funcs imx_pd_encoder_funcs = {
	.destroy = imx_pd_encoder_destroy,
=======
};

static struct drm_encoder_funcs imx_pd_encoder_funcs = {
	.destroy = imx_drm_encoder_destroy,
>>>>>>> v3.18
};

static struct drm_encoder_helper_funcs imx_pd_encoder_helper_funcs = {
	.dpms = imx_pd_encoder_dpms,
	.mode_fixup = imx_pd_encoder_mode_fixup,
	.prepare = imx_pd_encoder_prepare,
	.commit = imx_pd_encoder_commit,
	.mode_set = imx_pd_encoder_mode_set,
	.disable = imx_pd_encoder_disable,
};

<<<<<<< HEAD
static int imx_pd_register(struct imx_parallel_display *imxpd)
{
	int ret;

	drm_mode_connector_attach_encoder(&imxpd->connector, &imxpd->encoder);

	imxpd->connector.funcs = &imx_pd_connector_funcs;
	imxpd->encoder.funcs = &imx_pd_encoder_funcs;

	imxpd->encoder.encoder_type = DRM_MODE_ENCODER_NONE;
	imxpd->connector.connector_type = DRM_MODE_CONNECTOR_VGA;

	drm_encoder_helper_add(&imxpd->encoder, &imx_pd_encoder_helper_funcs);
	ret = imx_drm_add_encoder(&imxpd->encoder, &imxpd->imx_drm_encoder,
			THIS_MODULE);
	if (ret) {
		dev_err(imxpd->dev, "adding encoder failed with %d\n", ret);
		return ret;
	}

	drm_connector_helper_add(&imxpd->connector,
			&imx_pd_connector_helper_funcs);

	ret = imx_drm_add_connector(&imxpd->connector,
			&imxpd->imx_drm_connector, THIS_MODULE);
	if (ret) {
		imx_drm_remove_encoder(imxpd->imx_drm_encoder);
		dev_err(imxpd->dev, "adding connector failed with %d\n", ret);
		return ret;
	}
=======
static int imx_pd_register(struct drm_device *drm,
	struct imx_parallel_display *imxpd)
{
	int ret;

	ret = imx_drm_encoder_parse_of(drm, &imxpd->encoder,
				       imxpd->dev->of_node);
	if (ret)
		return ret;

	/* set the connector's dpms to OFF so that
	 * drm_helper_connector_dpms() won't return
	 * immediately since the current state is ON
	 * at this point.
	 */
	imxpd->connector.dpms = DRM_MODE_DPMS_OFF;

	drm_encoder_helper_add(&imxpd->encoder, &imx_pd_encoder_helper_funcs);
	drm_encoder_init(drm, &imxpd->encoder, &imx_pd_encoder_funcs,
			 DRM_MODE_ENCODER_NONE);

	drm_connector_helper_add(&imxpd->connector,
			&imx_pd_connector_helper_funcs);
	drm_connector_init(drm, &imxpd->connector, &imx_pd_connector_funcs,
			   DRM_MODE_CONNECTOR_VGA);

	if (imxpd->panel)
		drm_panel_attach(imxpd->panel, &imxpd->connector);

	drm_mode_connector_attach_encoder(&imxpd->connector, &imxpd->encoder);
>>>>>>> v3.18

	imxpd->connector.encoder = &imxpd->encoder;

	return 0;
}

<<<<<<< HEAD
static int imx_pd_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
=======
static int imx_pd_bind(struct device *dev, struct device *master, void *data)
{
	struct drm_device *drm = data;
	struct device_node *np = dev->of_node;
	struct device_node *panel_node;
>>>>>>> v3.18
	const u8 *edidp;
	struct imx_parallel_display *imxpd;
	int ret;
	const char *fmt;
<<<<<<< HEAD
	struct pinctrl *pinctrl;

	imxpd = devm_kzalloc(&pdev->dev, sizeof(*imxpd), GFP_KERNEL);
	if (!imxpd)
		return -ENOMEM;

	pinctrl = devm_pinctrl_get_select_default(&pdev->dev);
	if (IS_ERR(pinctrl)) {
		ret = PTR_ERR(pinctrl);
		dev_warn(&pdev->dev, "pinctrl_get_select_default failed with %d",
				ret);
		return ret;
	}

=======

	imxpd = devm_kzalloc(dev, sizeof(*imxpd), GFP_KERNEL);
	if (!imxpd)
		return -ENOMEM;

>>>>>>> v3.18
	edidp = of_get_property(np, "edid", &imxpd->edid_len);
	if (edidp)
		imxpd->edid = kmemdup(edidp, imxpd->edid_len, GFP_KERNEL);

	ret = of_property_read_string(np, "interface-pix-fmt", &fmt);
	if (!ret) {
		if (!strcmp(fmt, "rgb24"))
			imxpd->interface_pix_fmt = V4L2_PIX_FMT_RGB24;
		else if (!strcmp(fmt, "rgb565"))
			imxpd->interface_pix_fmt = V4L2_PIX_FMT_RGB565;
		else if (!strcmp(fmt, "bgr666"))
			imxpd->interface_pix_fmt = V4L2_PIX_FMT_BGR666;
<<<<<<< HEAD
	}

	imxpd->dev = &pdev->dev;

	ret = imx_pd_register(imxpd);
	if (ret)
		return ret;

	ret = imx_drm_encoder_add_possible_crtcs(imxpd->imx_drm_encoder, np);

	platform_set_drvdata(pdev, imxpd);
=======
		else if (!strcmp(fmt, "lvds666"))
			imxpd->interface_pix_fmt =
					v4l2_fourcc('L', 'V', 'D', '6');
	}

	panel_node = of_parse_phandle(np, "fsl,panel", 0);
	if (panel_node)
		imxpd->panel = of_drm_find_panel(panel_node);

	imxpd->dev = dev;

	ret = imx_pd_register(drm, imxpd);
	if (ret)
		return ret;

	dev_set_drvdata(dev, imxpd);
>>>>>>> v3.18

	return 0;
}

<<<<<<< HEAD
static int imx_pd_remove(struct platform_device *pdev)
{
	struct imx_parallel_display *imxpd = platform_get_drvdata(pdev);
	struct drm_connector *connector = &imxpd->connector;
	struct drm_encoder *encoder = &imxpd->encoder;

	drm_mode_connector_detach_encoder(connector, encoder);

	imx_drm_remove_connector(imxpd->imx_drm_connector);
	imx_drm_remove_encoder(imxpd->imx_drm_encoder);

=======
static void imx_pd_unbind(struct device *dev, struct device *master,
	void *data)
{
	struct imx_parallel_display *imxpd = dev_get_drvdata(dev);

	imxpd->encoder.funcs->destroy(&imxpd->encoder);
	imxpd->connector.funcs->destroy(&imxpd->connector);
}

static const struct component_ops imx_pd_ops = {
	.bind	= imx_pd_bind,
	.unbind	= imx_pd_unbind,
};

static int imx_pd_probe(struct platform_device *pdev)
{
	return component_add(&pdev->dev, &imx_pd_ops);
}

static int imx_pd_remove(struct platform_device *pdev)
{
	component_del(&pdev->dev, &imx_pd_ops);
>>>>>>> v3.18
	return 0;
}

static const struct of_device_id imx_pd_dt_ids[] = {
	{ .compatible = "fsl,imx-parallel-display", },
	{ /* sentinel */ }
};
<<<<<<< HEAD
=======
MODULE_DEVICE_TABLE(of, imx_pd_dt_ids);
>>>>>>> v3.18

static struct platform_driver imx_pd_driver = {
	.probe		= imx_pd_probe,
	.remove		= imx_pd_remove,
	.driver		= {
		.of_match_table = imx_pd_dt_ids,
		.name	= "imx-parallel-display",
		.owner	= THIS_MODULE,
	},
};

module_platform_driver(imx_pd_driver);

MODULE_DESCRIPTION("i.MX parallel display driver");
MODULE_AUTHOR("Sascha Hauer, Pengutronix");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_ALIAS("platform:imx-parallel-display");
>>>>>>> v3.18
