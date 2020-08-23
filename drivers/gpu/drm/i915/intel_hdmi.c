/*
 * Copyright 2006 Dave Airlie <airlied@linux.ie>
 * Copyright © 2006-2009 Intel Corporation
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
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *	Eric Anholt <eric@anholt.net>
 *	Jesse Barnes <jesse.barnes@intel.com>
 */

#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/delay.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/hdmi.h>
>>>>>>> v3.18
=======
#include <linux/hdmi.h>
>>>>>>> v3.18
#include <drm/drmP.h>
#include <drm/drm_crtc.h>
#include <drm/drm_edid.h>
#include "intel_drv.h"
#include <drm/i915_drm.h>
#include "i915_drv.h"

static struct drm_device *intel_hdmi_to_dev(struct intel_hdmi *intel_hdmi)
{
	return hdmi_to_dig_port(intel_hdmi)->base.base.dev;
}

static void
assert_hdmi_port_disabled(struct intel_hdmi *intel_hdmi)
{
	struct drm_device *dev = intel_hdmi_to_dev(intel_hdmi);
	struct drm_i915_private *dev_priv = dev->dev_private;
	uint32_t enabled_bits;

	enabled_bits = HAS_DDI(dev) ? DDI_BUF_CTL_ENABLE : SDVO_ENABLE;

	WARN(I915_READ(intel_hdmi->hdmi_reg) & enabled_bits,
	     "HDMI port enabled, expecting disabled\n");
}

struct intel_hdmi *enc_to_intel_hdmi(struct drm_encoder *encoder)
{
	struct intel_digital_port *intel_dig_port =
		container_of(encoder, struct intel_digital_port, base.base);
	return &intel_dig_port->hdmi;
}

static struct intel_hdmi *intel_attached_hdmi(struct drm_connector *connector)
{
	return enc_to_intel_hdmi(&intel_attached_encoder(connector)->base);
}

<<<<<<< HEAD
<<<<<<< HEAD
void intel_dip_infoframe_csum(struct dip_infoframe *frame)
{
	uint8_t *data = (uint8_t *)frame;
	uint8_t sum = 0;
	unsigned i;

	frame->checksum = 0;
	frame->ecc = 0;

	for (i = 0; i < frame->len + DIP_HEADER_SIZE; i++)
		sum += data[i];

	frame->checksum = 0x100 - sum;
}

static u32 g4x_infoframe_index(struct dip_infoframe *frame)
{
	switch (frame->type) {
	case DIP_TYPE_AVI:
		return VIDEO_DIP_SELECT_AVI;
	case DIP_TYPE_SPD:
		return VIDEO_DIP_SELECT_SPD;
	default:
		DRM_DEBUG_DRIVER("unknown info frame type %d\n", frame->type);
=======
=======
>>>>>>> v3.18
static u32 g4x_infoframe_index(enum hdmi_infoframe_type type)
{
	switch (type) {
	case HDMI_INFOFRAME_TYPE_AVI:
		return VIDEO_DIP_SELECT_AVI;
	case HDMI_INFOFRAME_TYPE_SPD:
		return VIDEO_DIP_SELECT_SPD;
	case HDMI_INFOFRAME_TYPE_VENDOR:
		return VIDEO_DIP_SELECT_VENDOR;
	default:
		DRM_DEBUG_DRIVER("unknown info frame type %d\n", type);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return 0;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static u32 g4x_infoframe_enable(struct dip_infoframe *frame)
{
	switch (frame->type) {
	case DIP_TYPE_AVI:
		return VIDEO_DIP_ENABLE_AVI;
	case DIP_TYPE_SPD:
		return VIDEO_DIP_ENABLE_SPD;
	default:
		DRM_DEBUG_DRIVER("unknown info frame type %d\n", frame->type);
=======
=======
>>>>>>> v3.18
static u32 g4x_infoframe_enable(enum hdmi_infoframe_type type)
{
	switch (type) {
	case HDMI_INFOFRAME_TYPE_AVI:
		return VIDEO_DIP_ENABLE_AVI;
	case HDMI_INFOFRAME_TYPE_SPD:
		return VIDEO_DIP_ENABLE_SPD;
	case HDMI_INFOFRAME_TYPE_VENDOR:
		return VIDEO_DIP_ENABLE_VENDOR;
	default:
		DRM_DEBUG_DRIVER("unknown info frame type %d\n", type);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return 0;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static u32 hsw_infoframe_enable(struct dip_infoframe *frame)
{
	switch (frame->type) {
	case DIP_TYPE_AVI:
		return VIDEO_DIP_ENABLE_AVI_HSW;
	case DIP_TYPE_SPD:
		return VIDEO_DIP_ENABLE_SPD_HSW;
	default:
		DRM_DEBUG_DRIVER("unknown info frame type %d\n", frame->type);
=======
=======
>>>>>>> v3.18
static u32 hsw_infoframe_enable(enum hdmi_infoframe_type type)
{
	switch (type) {
	case HDMI_INFOFRAME_TYPE_AVI:
		return VIDEO_DIP_ENABLE_AVI_HSW;
	case HDMI_INFOFRAME_TYPE_SPD:
		return VIDEO_DIP_ENABLE_SPD_HSW;
	case HDMI_INFOFRAME_TYPE_VENDOR:
		return VIDEO_DIP_ENABLE_VS_HSW;
	default:
		DRM_DEBUG_DRIVER("unknown info frame type %d\n", type);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return 0;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static u32 hsw_infoframe_data_reg(struct dip_infoframe *frame,
				  enum transcoder cpu_transcoder)
{
	switch (frame->type) {
	case DIP_TYPE_AVI:
		return HSW_TVIDEO_DIP_AVI_DATA(cpu_transcoder);
	case DIP_TYPE_SPD:
		return HSW_TVIDEO_DIP_SPD_DATA(cpu_transcoder);
	default:
		DRM_DEBUG_DRIVER("unknown info frame type %d\n", frame->type);
=======
=======
>>>>>>> v3.18
static u32 hsw_infoframe_data_reg(enum hdmi_infoframe_type type,
				  enum transcoder cpu_transcoder,
				  struct drm_i915_private *dev_priv)
{
	switch (type) {
	case HDMI_INFOFRAME_TYPE_AVI:
		return HSW_TVIDEO_DIP_AVI_DATA(cpu_transcoder);
	case HDMI_INFOFRAME_TYPE_SPD:
		return HSW_TVIDEO_DIP_SPD_DATA(cpu_transcoder);
	case HDMI_INFOFRAME_TYPE_VENDOR:
		return HSW_TVIDEO_DIP_VS_DATA(cpu_transcoder);
	default:
		DRM_DEBUG_DRIVER("unknown info frame type %d\n", type);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return 0;
	}
}

static void g4x_write_infoframe(struct drm_encoder *encoder,
<<<<<<< HEAD
<<<<<<< HEAD
				struct dip_infoframe *frame)
{
	uint32_t *data = (uint32_t *)frame;
	struct drm_device *dev = encoder->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 val = I915_READ(VIDEO_DIP_CTL);
	unsigned i, len = DIP_HEADER_SIZE + frame->len;
=======
=======
>>>>>>> v3.18
				enum hdmi_infoframe_type type,
				const void *frame, ssize_t len)
{
	const uint32_t *data = frame;
	struct drm_device *dev = encoder->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 val = I915_READ(VIDEO_DIP_CTL);
	int i;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	WARN(!(val & VIDEO_DIP_ENABLE), "Writing DIP with CTL reg disabled\n");

	val &= ~(VIDEO_DIP_SELECT_MASK | 0xf); /* clear DIP data offset */
<<<<<<< HEAD
<<<<<<< HEAD
	val |= g4x_infoframe_index(frame);

	val &= ~g4x_infoframe_enable(frame);
=======
	val |= g4x_infoframe_index(type);

	val &= ~g4x_infoframe_enable(type);
>>>>>>> v3.18
=======
	val |= g4x_infoframe_index(type);

	val &= ~g4x_infoframe_enable(type);
>>>>>>> v3.18

	I915_WRITE(VIDEO_DIP_CTL, val);

	mmiowb();
	for (i = 0; i < len; i += 4) {
		I915_WRITE(VIDEO_DIP_DATA, *data);
		data++;
	}
	/* Write every possible data byte to force correct ECC calculation. */
	for (; i < VIDEO_DIP_DATA_SIZE; i += 4)
		I915_WRITE(VIDEO_DIP_DATA, 0);
	mmiowb();

<<<<<<< HEAD
<<<<<<< HEAD
	val |= g4x_infoframe_enable(frame);
=======
	val |= g4x_infoframe_enable(type);
>>>>>>> v3.18
=======
	val |= g4x_infoframe_enable(type);
>>>>>>> v3.18
	val &= ~VIDEO_DIP_FREQ_MASK;
	val |= VIDEO_DIP_FREQ_VSYNC;

	I915_WRITE(VIDEO_DIP_CTL, val);
	POSTING_READ(VIDEO_DIP_CTL);
}

static void ibx_write_infoframe(struct drm_encoder *encoder,
<<<<<<< HEAD
<<<<<<< HEAD
				struct dip_infoframe *frame)
{
	uint32_t *data = (uint32_t *)frame;
	struct drm_device *dev = encoder->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(encoder->crtc);
	int reg = TVIDEO_DIP_CTL(intel_crtc->pipe);
	unsigned i, len = DIP_HEADER_SIZE + frame->len;
=======
=======
>>>>>>> v3.18
				enum hdmi_infoframe_type type,
				const void *frame, ssize_t len)
{
	const uint32_t *data = frame;
	struct drm_device *dev = encoder->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(encoder->crtc);
	int i, reg = TVIDEO_DIP_CTL(intel_crtc->pipe);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	u32 val = I915_READ(reg);

	WARN(!(val & VIDEO_DIP_ENABLE), "Writing DIP with CTL reg disabled\n");

	val &= ~(VIDEO_DIP_SELECT_MASK | 0xf); /* clear DIP data offset */
<<<<<<< HEAD
<<<<<<< HEAD
	val |= g4x_infoframe_index(frame);

	val &= ~g4x_infoframe_enable(frame);
=======
	val |= g4x_infoframe_index(type);

	val &= ~g4x_infoframe_enable(type);
>>>>>>> v3.18
=======
	val |= g4x_infoframe_index(type);

	val &= ~g4x_infoframe_enable(type);
>>>>>>> v3.18

	I915_WRITE(reg, val);

	mmiowb();
	for (i = 0; i < len; i += 4) {
		I915_WRITE(TVIDEO_DIP_DATA(intel_crtc->pipe), *data);
		data++;
	}
	/* Write every possible data byte to force correct ECC calculation. */
	for (; i < VIDEO_DIP_DATA_SIZE; i += 4)
		I915_WRITE(TVIDEO_DIP_DATA(intel_crtc->pipe), 0);
	mmiowb();

<<<<<<< HEAD
<<<<<<< HEAD
	val |= g4x_infoframe_enable(frame);
=======
	val |= g4x_infoframe_enable(type);
>>>>>>> v3.18
=======
	val |= g4x_infoframe_enable(type);
>>>>>>> v3.18
	val &= ~VIDEO_DIP_FREQ_MASK;
	val |= VIDEO_DIP_FREQ_VSYNC;

	I915_WRITE(reg, val);
	POSTING_READ(reg);
}

static void cpt_write_infoframe(struct drm_encoder *encoder,
<<<<<<< HEAD
<<<<<<< HEAD
				struct dip_infoframe *frame)
{
	uint32_t *data = (uint32_t *)frame;
	struct drm_device *dev = encoder->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(encoder->crtc);
	int reg = TVIDEO_DIP_CTL(intel_crtc->pipe);
	unsigned i, len = DIP_HEADER_SIZE + frame->len;
=======
=======
>>>>>>> v3.18
				enum hdmi_infoframe_type type,
				const void *frame, ssize_t len)
{
	const uint32_t *data = frame;
	struct drm_device *dev = encoder->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(encoder->crtc);
	int i, reg = TVIDEO_DIP_CTL(intel_crtc->pipe);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	u32 val = I915_READ(reg);

	WARN(!(val & VIDEO_DIP_ENABLE), "Writing DIP with CTL reg disabled\n");

	val &= ~(VIDEO_DIP_SELECT_MASK | 0xf); /* clear DIP data offset */
<<<<<<< HEAD
<<<<<<< HEAD
	val |= g4x_infoframe_index(frame);

	/* The DIP control register spec says that we need to update the AVI
	 * infoframe without clearing its enable bit */
	if (frame->type != DIP_TYPE_AVI)
		val &= ~g4x_infoframe_enable(frame);
=======
=======
>>>>>>> v3.18
	val |= g4x_infoframe_index(type);

	/* The DIP control register spec says that we need to update the AVI
	 * infoframe without clearing its enable bit */
	if (type != HDMI_INFOFRAME_TYPE_AVI)
		val &= ~g4x_infoframe_enable(type);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	I915_WRITE(reg, val);

	mmiowb();
	for (i = 0; i < len; i += 4) {
		I915_WRITE(TVIDEO_DIP_DATA(intel_crtc->pipe), *data);
		data++;
	}
	/* Write every possible data byte to force correct ECC calculation. */
	for (; i < VIDEO_DIP_DATA_SIZE; i += 4)
		I915_WRITE(TVIDEO_DIP_DATA(intel_crtc->pipe), 0);
	mmiowb();

<<<<<<< HEAD
<<<<<<< HEAD
	val |= g4x_infoframe_enable(frame);
=======
	val |= g4x_infoframe_enable(type);
>>>>>>> v3.18
=======
	val |= g4x_infoframe_enable(type);
>>>>>>> v3.18
	val &= ~VIDEO_DIP_FREQ_MASK;
	val |= VIDEO_DIP_FREQ_VSYNC;

	I915_WRITE(reg, val);
	POSTING_READ(reg);
}

static void vlv_write_infoframe(struct drm_encoder *encoder,
<<<<<<< HEAD
<<<<<<< HEAD
				     struct dip_infoframe *frame)
{
	uint32_t *data = (uint32_t *)frame;
	struct drm_device *dev = encoder->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(encoder->crtc);
	int reg = VLV_TVIDEO_DIP_CTL(intel_crtc->pipe);
	unsigned i, len = DIP_HEADER_SIZE + frame->len;
=======
=======
>>>>>>> v3.18
				enum hdmi_infoframe_type type,
				const void *frame, ssize_t len)
{
	const uint32_t *data = frame;
	struct drm_device *dev = encoder->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(encoder->crtc);
	int i, reg = VLV_TVIDEO_DIP_CTL(intel_crtc->pipe);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	u32 val = I915_READ(reg);

	WARN(!(val & VIDEO_DIP_ENABLE), "Writing DIP with CTL reg disabled\n");

	val &= ~(VIDEO_DIP_SELECT_MASK | 0xf); /* clear DIP data offset */
<<<<<<< HEAD
<<<<<<< HEAD
	val |= g4x_infoframe_index(frame);

	val &= ~g4x_infoframe_enable(frame);
=======
	val |= g4x_infoframe_index(type);

	val &= ~g4x_infoframe_enable(type);
>>>>>>> v3.18
=======
	val |= g4x_infoframe_index(type);

	val &= ~g4x_infoframe_enable(type);
>>>>>>> v3.18

	I915_WRITE(reg, val);

	mmiowb();
	for (i = 0; i < len; i += 4) {
		I915_WRITE(VLV_TVIDEO_DIP_DATA(intel_crtc->pipe), *data);
		data++;
	}
	/* Write every possible data byte to force correct ECC calculation. */
	for (; i < VIDEO_DIP_DATA_SIZE; i += 4)
		I915_WRITE(VLV_TVIDEO_DIP_DATA(intel_crtc->pipe), 0);
	mmiowb();

<<<<<<< HEAD
<<<<<<< HEAD
	val |= g4x_infoframe_enable(frame);
=======
	val |= g4x_infoframe_enable(type);
>>>>>>> v3.18
=======
	val |= g4x_infoframe_enable(type);
>>>>>>> v3.18
	val &= ~VIDEO_DIP_FREQ_MASK;
	val |= VIDEO_DIP_FREQ_VSYNC;

	I915_WRITE(reg, val);
	POSTING_READ(reg);
}

static void hsw_write_infoframe(struct drm_encoder *encoder,
<<<<<<< HEAD
<<<<<<< HEAD
				struct dip_infoframe *frame)
{
	uint32_t *data = (uint32_t *)frame;
=======
=======
>>>>>>> v3.18
				enum hdmi_infoframe_type type,
				const void *frame, ssize_t len)
{
	const uint32_t *data = frame;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	struct drm_device *dev = encoder->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(encoder->crtc);
	u32 ctl_reg = HSW_TVIDEO_DIP_CTL(intel_crtc->config.cpu_transcoder);
<<<<<<< HEAD
<<<<<<< HEAD
	u32 data_reg = hsw_infoframe_data_reg(frame, intel_crtc->config.cpu_transcoder);
	unsigned int i, len = DIP_HEADER_SIZE + frame->len;
	u32 val = I915_READ(ctl_reg);

	if (data_reg == 0)
		return;

	val &= ~hsw_infoframe_enable(frame);
=======
=======
>>>>>>> v3.18
	u32 data_reg;
	int i;
	u32 val = I915_READ(ctl_reg);

	data_reg = hsw_infoframe_data_reg(type,
					  intel_crtc->config.cpu_transcoder,
					  dev_priv);
	if (data_reg == 0)
		return;

	val &= ~hsw_infoframe_enable(type);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	I915_WRITE(ctl_reg, val);

	mmiowb();
	for (i = 0; i < len; i += 4) {
		I915_WRITE(data_reg + i, *data);
		data++;
	}
	/* Write every possible data byte to force correct ECC calculation. */
	for (; i < VIDEO_DIP_DATA_SIZE; i += 4)
		I915_WRITE(data_reg + i, 0);
	mmiowb();

<<<<<<< HEAD
<<<<<<< HEAD
	val |= hsw_infoframe_enable(frame);
=======
	val |= hsw_infoframe_enable(type);
>>>>>>> v3.18
=======
	val |= hsw_infoframe_enable(type);
>>>>>>> v3.18
	I915_WRITE(ctl_reg, val);
	POSTING_READ(ctl_reg);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void intel_set_infoframe(struct drm_encoder *encoder,
				struct dip_infoframe *frame)
{
	struct intel_hdmi *intel_hdmi = enc_to_intel_hdmi(encoder);

	intel_dip_infoframe_csum(frame);
	intel_hdmi->write_infoframe(encoder, frame);
=======
=======
>>>>>>> v3.18
/*
 * The data we write to the DIP data buffer registers is 1 byte bigger than the
 * HDMI infoframe size because of an ECC/reserved byte at position 3 (starting
 * at 0). It's also a byte used by DisplayPort so the same DIP registers can be
 * used for both technologies.
 *
 * DW0: Reserved/ECC/DP | HB2 | HB1 | HB0
 * DW1:       DB3       | DB2 | DB1 | DB0
 * DW2:       DB7       | DB6 | DB5 | DB4
 * DW3: ...
 *
 * (HB is Header Byte, DB is Data Byte)
 *
 * The hdmi pack() functions don't know about that hardware specific hole so we
 * trick them by giving an offset into the buffer and moving back the header
 * bytes by one.
 */
static void intel_write_infoframe(struct drm_encoder *encoder,
				  union hdmi_infoframe *frame)
{
	struct intel_hdmi *intel_hdmi = enc_to_intel_hdmi(encoder);
	uint8_t buffer[VIDEO_DIP_DATA_SIZE];
	ssize_t len;

	/* see comment above for the reason for this offset */
	len = hdmi_infoframe_pack(frame, buffer + 1, sizeof(buffer) - 1);
	if (len < 0)
		return;

	/* Insert the 'hole' (see big comment above) at position 3 */
	buffer[0] = buffer[1];
	buffer[1] = buffer[2];
	buffer[2] = buffer[3];
	buffer[3] = 0;
	len++;

	intel_hdmi->write_infoframe(encoder, frame->any.type, buffer, len);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static void intel_hdmi_set_avi_infoframe(struct drm_encoder *encoder,
					 struct drm_display_mode *adjusted_mode)
{
	struct intel_hdmi *intel_hdmi = enc_to_intel_hdmi(encoder);
	struct intel_crtc *intel_crtc = to_intel_crtc(encoder->crtc);
<<<<<<< HEAD
<<<<<<< HEAD
	struct dip_infoframe avi_if = {
		.type = DIP_TYPE_AVI,
		.ver = DIP_VERSION_AVI,
		.len = DIP_LEN_AVI,
	};

	if (adjusted_mode->flags & DRM_MODE_FLAG_DBLCLK)
		avi_if.body.avi.YQ_CN_PR |= DIP_AVI_PR_2;

	if (intel_hdmi->rgb_quant_range_selectable) {
		if (intel_crtc->config.limited_color_range)
			avi_if.body.avi.ITC_EC_Q_SC |= DIP_AVI_RGB_QUANT_RANGE_LIMITED;
		else
			avi_if.body.avi.ITC_EC_Q_SC |= DIP_AVI_RGB_QUANT_RANGE_FULL;
	}

	avi_if.body.avi.VIC = drm_match_cea_mode(adjusted_mode);

	intel_set_infoframe(encoder, &avi_if);
=======
=======
>>>>>>> v3.18
	union hdmi_infoframe frame;
	int ret;

	/* Set user selected PAR to incoming mode's member */
	adjusted_mode->picture_aspect_ratio = intel_hdmi->aspect_ratio;

	ret = drm_hdmi_avi_infoframe_from_display_mode(&frame.avi,
						       adjusted_mode);
	if (ret < 0) {
		DRM_ERROR("couldn't fill AVI infoframe\n");
		return;
	}

	if (intel_hdmi->rgb_quant_range_selectable) {
		if (intel_crtc->config.limited_color_range)
			frame.avi.quantization_range =
				HDMI_QUANTIZATION_RANGE_LIMITED;
		else
			frame.avi.quantization_range =
				HDMI_QUANTIZATION_RANGE_FULL;
	}

	intel_write_infoframe(encoder, &frame);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static void intel_hdmi_set_spd_infoframe(struct drm_encoder *encoder)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct dip_infoframe spd_if;

	memset(&spd_if, 0, sizeof(spd_if));
	spd_if.type = DIP_TYPE_SPD;
	spd_if.ver = DIP_VERSION_SPD;
	spd_if.len = DIP_LEN_SPD;
	strcpy(spd_if.body.spd.vn, "Intel");
	strcpy(spd_if.body.spd.pd, "Integrated gfx");
	spd_if.body.spd.sdi = DIP_SPD_PC;

	intel_set_infoframe(encoder, &spd_if);
}

static void g4x_set_infoframes(struct drm_encoder *encoder,
=======
=======
>>>>>>> v3.18
	union hdmi_infoframe frame;
	int ret;

	ret = hdmi_spd_infoframe_init(&frame.spd, "Intel", "Integrated gfx");
	if (ret < 0) {
		DRM_ERROR("couldn't fill SPD infoframe\n");
		return;
	}

	frame.spd.sdi = HDMI_SPD_SDI_PC;

	intel_write_infoframe(encoder, &frame);
}

static void
intel_hdmi_set_hdmi_infoframe(struct drm_encoder *encoder,
			      struct drm_display_mode *adjusted_mode)
{
	union hdmi_infoframe frame;
	int ret;

	ret = drm_hdmi_vendor_infoframe_from_display_mode(&frame.vendor.hdmi,
							  adjusted_mode);
	if (ret < 0)
		return;

	intel_write_infoframe(encoder, &frame);
}

static void g4x_set_infoframes(struct drm_encoder *encoder,
			       bool enable,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			       struct drm_display_mode *adjusted_mode)
{
	struct drm_i915_private *dev_priv = encoder->dev->dev_private;
	struct intel_digital_port *intel_dig_port = enc_to_dig_port(encoder);
	struct intel_hdmi *intel_hdmi = &intel_dig_port->hdmi;
	u32 reg = VIDEO_DIP_CTL;
	u32 val = I915_READ(reg);
<<<<<<< HEAD
<<<<<<< HEAD
	u32 port;
=======
	u32 port = VIDEO_DIP_PORT(intel_dig_port->port);
>>>>>>> v3.18
=======
	u32 port = VIDEO_DIP_PORT(intel_dig_port->port);
>>>>>>> v3.18

	assert_hdmi_port_disabled(intel_hdmi);

	/* If the registers were not initialized yet, they might be zeroes,
	 * which means we're selecting the AVI DIP and we're setting its
	 * frequency to once. This seems to really confuse the HW and make
	 * things stop working (the register spec says the AVI always needs to
	 * be sent every VSync). So here we avoid writing to the register more
	 * than we need and also explicitly select the AVI DIP and explicitly
	 * set its frequency to every VSync. Avoiding to write it twice seems to
	 * be enough to solve the problem, but being defensive shouldn't hurt us
	 * either. */
	val |= VIDEO_DIP_SELECT_AVI | VIDEO_DIP_FREQ_VSYNC;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!intel_hdmi->has_hdmi_sink) {
=======
	if (!enable) {
>>>>>>> v3.18
=======
	if (!enable) {
>>>>>>> v3.18
		if (!(val & VIDEO_DIP_ENABLE))
			return;
		val &= ~VIDEO_DIP_ENABLE;
		I915_WRITE(reg, val);
		POSTING_READ(reg);
		return;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	switch (intel_dig_port->port) {
	case PORT_B:
		port = VIDEO_DIP_PORT_B;
		break;
	case PORT_C:
		port = VIDEO_DIP_PORT_C;
		break;
	default:
		BUG();
		return;
	}

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (port != (val & VIDEO_DIP_PORT_MASK)) {
		if (val & VIDEO_DIP_ENABLE) {
			val &= ~VIDEO_DIP_ENABLE;
			I915_WRITE(reg, val);
			POSTING_READ(reg);
		}
		val &= ~VIDEO_DIP_PORT_MASK;
		val |= port;
	}

	val |= VIDEO_DIP_ENABLE;
	val &= ~VIDEO_DIP_ENABLE_VENDOR;

	I915_WRITE(reg, val);
	POSTING_READ(reg);

	intel_hdmi_set_avi_infoframe(encoder, adjusted_mode);
	intel_hdmi_set_spd_infoframe(encoder);
<<<<<<< HEAD
<<<<<<< HEAD
}

static void ibx_set_infoframes(struct drm_encoder *encoder,
=======
=======
>>>>>>> v3.18
	intel_hdmi_set_hdmi_infoframe(encoder, adjusted_mode);
}

static void ibx_set_infoframes(struct drm_encoder *encoder,
			       bool enable,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			       struct drm_display_mode *adjusted_mode)
{
	struct drm_i915_private *dev_priv = encoder->dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(encoder->crtc);
	struct intel_digital_port *intel_dig_port = enc_to_dig_port(encoder);
	struct intel_hdmi *intel_hdmi = &intel_dig_port->hdmi;
	u32 reg = TVIDEO_DIP_CTL(intel_crtc->pipe);
	u32 val = I915_READ(reg);
<<<<<<< HEAD
<<<<<<< HEAD
	u32 port;
=======
	u32 port = VIDEO_DIP_PORT(intel_dig_port->port);
>>>>>>> v3.18
=======
	u32 port = VIDEO_DIP_PORT(intel_dig_port->port);
>>>>>>> v3.18

	assert_hdmi_port_disabled(intel_hdmi);

	/* See the big comment in g4x_set_infoframes() */
	val |= VIDEO_DIP_SELECT_AVI | VIDEO_DIP_FREQ_VSYNC;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!intel_hdmi->has_hdmi_sink) {
=======
	if (!enable) {
>>>>>>> v3.18
=======
	if (!enable) {
>>>>>>> v3.18
		if (!(val & VIDEO_DIP_ENABLE))
			return;
		val &= ~VIDEO_DIP_ENABLE;
		I915_WRITE(reg, val);
		POSTING_READ(reg);
		return;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	switch (intel_dig_port->port) {
	case PORT_B:
		port = VIDEO_DIP_PORT_B;
		break;
	case PORT_C:
		port = VIDEO_DIP_PORT_C;
		break;
	case PORT_D:
		port = VIDEO_DIP_PORT_D;
		break;
	default:
		BUG();
		return;
	}

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (port != (val & VIDEO_DIP_PORT_MASK)) {
		if (val & VIDEO_DIP_ENABLE) {
			val &= ~VIDEO_DIP_ENABLE;
			I915_WRITE(reg, val);
			POSTING_READ(reg);
		}
		val &= ~VIDEO_DIP_PORT_MASK;
		val |= port;
	}

	val |= VIDEO_DIP_ENABLE;
	val &= ~(VIDEO_DIP_ENABLE_VENDOR | VIDEO_DIP_ENABLE_GAMUT |
		 VIDEO_DIP_ENABLE_GCP);

	I915_WRITE(reg, val);
	POSTING_READ(reg);

	intel_hdmi_set_avi_infoframe(encoder, adjusted_mode);
	intel_hdmi_set_spd_infoframe(encoder);
<<<<<<< HEAD
<<<<<<< HEAD
}

static void cpt_set_infoframes(struct drm_encoder *encoder,
=======
=======
>>>>>>> v3.18
	intel_hdmi_set_hdmi_infoframe(encoder, adjusted_mode);
}

static void cpt_set_infoframes(struct drm_encoder *encoder,
			       bool enable,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			       struct drm_display_mode *adjusted_mode)
{
	struct drm_i915_private *dev_priv = encoder->dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(encoder->crtc);
	struct intel_hdmi *intel_hdmi = enc_to_intel_hdmi(encoder);
	u32 reg = TVIDEO_DIP_CTL(intel_crtc->pipe);
	u32 val = I915_READ(reg);

	assert_hdmi_port_disabled(intel_hdmi);

	/* See the big comment in g4x_set_infoframes() */
	val |= VIDEO_DIP_SELECT_AVI | VIDEO_DIP_FREQ_VSYNC;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!intel_hdmi->has_hdmi_sink) {
=======
	if (!enable) {
>>>>>>> v3.18
=======
	if (!enable) {
>>>>>>> v3.18
		if (!(val & VIDEO_DIP_ENABLE))
			return;
		val &= ~(VIDEO_DIP_ENABLE | VIDEO_DIP_ENABLE_AVI);
		I915_WRITE(reg, val);
		POSTING_READ(reg);
		return;
	}

	/* Set both together, unset both together: see the spec. */
	val |= VIDEO_DIP_ENABLE | VIDEO_DIP_ENABLE_AVI;
	val &= ~(VIDEO_DIP_ENABLE_VENDOR | VIDEO_DIP_ENABLE_GAMUT |
		 VIDEO_DIP_ENABLE_GCP);

	I915_WRITE(reg, val);
	POSTING_READ(reg);

	intel_hdmi_set_avi_infoframe(encoder, adjusted_mode);
	intel_hdmi_set_spd_infoframe(encoder);
<<<<<<< HEAD
<<<<<<< HEAD
}

static void vlv_set_infoframes(struct drm_encoder *encoder,
			       struct drm_display_mode *adjusted_mode)
{
	struct drm_i915_private *dev_priv = encoder->dev->dev_private;
=======
=======
>>>>>>> v3.18
	intel_hdmi_set_hdmi_infoframe(encoder, adjusted_mode);
}

static void vlv_set_infoframes(struct drm_encoder *encoder,
			       bool enable,
			       struct drm_display_mode *adjusted_mode)
{
	struct drm_i915_private *dev_priv = encoder->dev->dev_private;
	struct intel_digital_port *intel_dig_port = enc_to_dig_port(encoder);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	struct intel_crtc *intel_crtc = to_intel_crtc(encoder->crtc);
	struct intel_hdmi *intel_hdmi = enc_to_intel_hdmi(encoder);
	u32 reg = VLV_TVIDEO_DIP_CTL(intel_crtc->pipe);
	u32 val = I915_READ(reg);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	u32 port = VIDEO_DIP_PORT(intel_dig_port->port);
>>>>>>> v3.18
=======
	u32 port = VIDEO_DIP_PORT(intel_dig_port->port);
>>>>>>> v3.18

	assert_hdmi_port_disabled(intel_hdmi);

	/* See the big comment in g4x_set_infoframes() */
	val |= VIDEO_DIP_SELECT_AVI | VIDEO_DIP_FREQ_VSYNC;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!intel_hdmi->has_hdmi_sink) {
=======
	if (!enable) {
>>>>>>> v3.18
=======
	if (!enable) {
>>>>>>> v3.18
		if (!(val & VIDEO_DIP_ENABLE))
			return;
		val &= ~VIDEO_DIP_ENABLE;
		I915_WRITE(reg, val);
		POSTING_READ(reg);
		return;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	val |= VIDEO_DIP_ENABLE;
	val &= ~(VIDEO_DIP_ENABLE_VENDOR | VIDEO_DIP_ENABLE_GAMUT |
		 VIDEO_DIP_ENABLE_GCP);
=======
=======
>>>>>>> v3.18
	if (port != (val & VIDEO_DIP_PORT_MASK)) {
		if (val & VIDEO_DIP_ENABLE) {
			val &= ~VIDEO_DIP_ENABLE;
			I915_WRITE(reg, val);
			POSTING_READ(reg);
		}
		val &= ~VIDEO_DIP_PORT_MASK;
		val |= port;
	}

	val |= VIDEO_DIP_ENABLE;
	val &= ~(VIDEO_DIP_ENABLE_AVI | VIDEO_DIP_ENABLE_VENDOR |
		 VIDEO_DIP_ENABLE_GAMUT | VIDEO_DIP_ENABLE_GCP);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	I915_WRITE(reg, val);
	POSTING_READ(reg);

	intel_hdmi_set_avi_infoframe(encoder, adjusted_mode);
	intel_hdmi_set_spd_infoframe(encoder);
<<<<<<< HEAD
<<<<<<< HEAD
}

static void hsw_set_infoframes(struct drm_encoder *encoder,
=======
=======
>>>>>>> v3.18
	intel_hdmi_set_hdmi_infoframe(encoder, adjusted_mode);
}

static void hsw_set_infoframes(struct drm_encoder *encoder,
			       bool enable,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			       struct drm_display_mode *adjusted_mode)
{
	struct drm_i915_private *dev_priv = encoder->dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(encoder->crtc);
	struct intel_hdmi *intel_hdmi = enc_to_intel_hdmi(encoder);
	u32 reg = HSW_TVIDEO_DIP_CTL(intel_crtc->config.cpu_transcoder);
	u32 val = I915_READ(reg);

	assert_hdmi_port_disabled(intel_hdmi);

<<<<<<< HEAD
<<<<<<< HEAD
	if (!intel_hdmi->has_hdmi_sink) {
=======
	if (!enable) {
>>>>>>> v3.18
=======
	if (!enable) {
>>>>>>> v3.18
		I915_WRITE(reg, 0);
		POSTING_READ(reg);
		return;
	}

	val &= ~(VIDEO_DIP_ENABLE_VSC_HSW | VIDEO_DIP_ENABLE_GCP_HSW |
		 VIDEO_DIP_ENABLE_VS_HSW | VIDEO_DIP_ENABLE_GMP_HSW);

	I915_WRITE(reg, val);
	POSTING_READ(reg);

	intel_hdmi_set_avi_infoframe(encoder, adjusted_mode);
	intel_hdmi_set_spd_infoframe(encoder);
<<<<<<< HEAD
<<<<<<< HEAD
}

static void intel_hdmi_mode_set(struct drm_encoder *encoder,
				struct drm_display_mode *mode,
				struct drm_display_mode *adjusted_mode)
{
	struct drm_device *dev = encoder->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(encoder->crtc);
	struct intel_hdmi *intel_hdmi = enc_to_intel_hdmi(encoder);
	u32 hdmi_val;

	hdmi_val = SDVO_ENCODING_HDMI;
	if (!HAS_PCH_SPLIT(dev) && !IS_VALLEYVIEW(dev))
=======
=======
>>>>>>> v3.18
	intel_hdmi_set_hdmi_infoframe(encoder, adjusted_mode);
}

static void intel_hdmi_prepare(struct intel_encoder *encoder)
{
	struct drm_device *dev = encoder->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *crtc = to_intel_crtc(encoder->base.crtc);
	struct intel_hdmi *intel_hdmi = enc_to_intel_hdmi(&encoder->base);
	struct drm_display_mode *adjusted_mode = &crtc->config.adjusted_mode;
	u32 hdmi_val;

	hdmi_val = SDVO_ENCODING_HDMI;
	if (!HAS_PCH_SPLIT(dev))
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		hdmi_val |= intel_hdmi->color_range;
	if (adjusted_mode->flags & DRM_MODE_FLAG_PVSYNC)
		hdmi_val |= SDVO_VSYNC_ACTIVE_HIGH;
	if (adjusted_mode->flags & DRM_MODE_FLAG_PHSYNC)
		hdmi_val |= SDVO_HSYNC_ACTIVE_HIGH;

<<<<<<< HEAD
<<<<<<< HEAD
	if (intel_crtc->config.pipe_bpp > 24)
=======
	if (crtc->config.pipe_bpp > 24)
>>>>>>> v3.18
=======
	if (crtc->config.pipe_bpp > 24)
>>>>>>> v3.18
		hdmi_val |= HDMI_COLOR_FORMAT_12bpc;
	else
		hdmi_val |= SDVO_COLOR_FORMAT_8bpc;

<<<<<<< HEAD
<<<<<<< HEAD
	/* Required on CPT */
	if (intel_hdmi->has_hdmi_sink && HAS_PCH_CPT(dev))
		hdmi_val |= HDMI_MODE_SELECT_HDMI;

	if (intel_hdmi->has_audio) {
		DRM_DEBUG_DRIVER("Enabling HDMI audio on pipe %c\n",
				 pipe_name(intel_crtc->pipe));
		hdmi_val |= SDVO_AUDIO_ENABLE;
		hdmi_val |= HDMI_MODE_SELECT_HDMI;
		intel_write_eld(encoder, adjusted_mode);
	}

	if (HAS_PCH_CPT(dev))
		hdmi_val |= SDVO_PIPE_SEL_CPT(intel_crtc->pipe);
	else
		hdmi_val |= SDVO_PIPE_SEL(intel_crtc->pipe);

	I915_WRITE(intel_hdmi->hdmi_reg, hdmi_val);
	POSTING_READ(intel_hdmi->hdmi_reg);

	intel_hdmi->set_infoframes(encoder, adjusted_mode);
=======
=======
>>>>>>> v3.18
	if (crtc->config.has_hdmi_sink)
		hdmi_val |= HDMI_MODE_SELECT_HDMI;

	if (crtc->config.has_audio) {
		WARN_ON(!crtc->config.has_hdmi_sink);
		DRM_DEBUG_DRIVER("Enabling HDMI audio on pipe %c\n",
				 pipe_name(crtc->pipe));
		hdmi_val |= SDVO_AUDIO_ENABLE;
		intel_write_eld(&encoder->base, adjusted_mode);
	}

	if (HAS_PCH_CPT(dev))
		hdmi_val |= SDVO_PIPE_SEL_CPT(crtc->pipe);
	else if (IS_CHERRYVIEW(dev))
		hdmi_val |= SDVO_PIPE_SEL_CHV(crtc->pipe);
	else
		hdmi_val |= SDVO_PIPE_SEL(crtc->pipe);

	I915_WRITE(intel_hdmi->hdmi_reg, hdmi_val);
	POSTING_READ(intel_hdmi->hdmi_reg);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static bool intel_hdmi_get_hw_state(struct intel_encoder *encoder,
				    enum pipe *pipe)
{
	struct drm_device *dev = encoder->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_hdmi *intel_hdmi = enc_to_intel_hdmi(&encoder->base);
<<<<<<< HEAD
<<<<<<< HEAD
	u32 tmp;

=======
=======
>>>>>>> v3.18
	enum intel_display_power_domain power_domain;
	u32 tmp;

	power_domain = intel_display_port_power_domain(encoder);
	if (!intel_display_power_enabled(dev_priv, power_domain))
		return false;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	tmp = I915_READ(intel_hdmi->hdmi_reg);

	if (!(tmp & SDVO_ENABLE))
		return false;

	if (HAS_PCH_CPT(dev))
		*pipe = PORT_TO_PIPE_CPT(tmp);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	else if (IS_CHERRYVIEW(dev))
		*pipe = SDVO_PORT_TO_PIPE_CHV(tmp);
>>>>>>> v3.18
=======
	else if (IS_CHERRYVIEW(dev))
		*pipe = SDVO_PORT_TO_PIPE_CHV(tmp);
>>>>>>> v3.18
	else
		*pipe = PORT_TO_PIPE(tmp);

	return true;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
static void intel_hdmi_get_config(struct intel_encoder *encoder,
				  struct intel_crtc_config *pipe_config)
{
	struct intel_hdmi *intel_hdmi = enc_to_intel_hdmi(&encoder->base);
	struct drm_device *dev = encoder->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 tmp, flags = 0;
	int dotclock;

	tmp = I915_READ(intel_hdmi->hdmi_reg);

	if (tmp & SDVO_HSYNC_ACTIVE_HIGH)
		flags |= DRM_MODE_FLAG_PHSYNC;
	else
		flags |= DRM_MODE_FLAG_NHSYNC;

	if (tmp & SDVO_VSYNC_ACTIVE_HIGH)
		flags |= DRM_MODE_FLAG_PVSYNC;
	else
		flags |= DRM_MODE_FLAG_NVSYNC;

	if (tmp & HDMI_MODE_SELECT_HDMI)
		pipe_config->has_hdmi_sink = true;

	if (tmp & SDVO_AUDIO_ENABLE)
		pipe_config->has_audio = true;

	if (!HAS_PCH_SPLIT(dev) &&
	    tmp & HDMI_COLOR_RANGE_16_235)
		pipe_config->limited_color_range = true;

	pipe_config->adjusted_mode.flags |= flags;

	if ((tmp & SDVO_COLOR_FORMAT_MASK) == HDMI_COLOR_FORMAT_12bpc)
		dotclock = pipe_config->port_clock * 2 / 3;
	else
		dotclock = pipe_config->port_clock;

	if (HAS_PCH_SPLIT(dev_priv->dev))
		ironlake_check_encoder_dotclock(pipe_config, dotclock);

	pipe_config->adjusted_mode.crtc_clock = dotclock;
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static void intel_enable_hdmi(struct intel_encoder *encoder)
{
	struct drm_device *dev = encoder->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(encoder->base.crtc);
	struct intel_hdmi *intel_hdmi = enc_to_intel_hdmi(&encoder->base);
	u32 temp;
	u32 enable_bits = SDVO_ENABLE;

<<<<<<< HEAD
<<<<<<< HEAD
	if (intel_hdmi->has_audio)
=======
	if (intel_crtc->config.has_audio)
>>>>>>> v3.18
=======
	if (intel_crtc->config.has_audio)
>>>>>>> v3.18
		enable_bits |= SDVO_AUDIO_ENABLE;

	temp = I915_READ(intel_hdmi->hdmi_reg);

	/* HW workaround for IBX, we need to move the port to transcoder A
	 * before disabling it, so restore the transcoder select bit here. */
	if (HAS_PCH_IBX(dev))
		enable_bits |= SDVO_PIPE_SEL(intel_crtc->pipe);

	/* HW workaround, need to toggle enable bit off and on for 12bpc, but
	 * we do this anyway which shows more stable in testing.
	 */
	if (HAS_PCH_SPLIT(dev)) {
		I915_WRITE(intel_hdmi->hdmi_reg, temp & ~SDVO_ENABLE);
		POSTING_READ(intel_hdmi->hdmi_reg);
	}

	temp |= enable_bits;

	I915_WRITE(intel_hdmi->hdmi_reg, temp);
	POSTING_READ(intel_hdmi->hdmi_reg);

	/* HW workaround, need to write this twice for issue that may result
	 * in first write getting masked.
	 */
	if (HAS_PCH_SPLIT(dev)) {
		I915_WRITE(intel_hdmi->hdmi_reg, temp);
		POSTING_READ(intel_hdmi->hdmi_reg);
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
static void vlv_enable_hdmi(struct intel_encoder *encoder)
{
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static void intel_disable_hdmi(struct intel_encoder *encoder)
{
	struct drm_device *dev = encoder->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_hdmi *intel_hdmi = enc_to_intel_hdmi(&encoder->base);
	u32 temp;
	u32 enable_bits = SDVO_ENABLE | SDVO_AUDIO_ENABLE;

	temp = I915_READ(intel_hdmi->hdmi_reg);

	/* HW workaround for IBX, we need to move the port to transcoder A
	 * before disabling it. */
	if (HAS_PCH_IBX(dev)) {
		struct drm_crtc *crtc = encoder->base.crtc;
		int pipe = crtc ? to_intel_crtc(crtc)->pipe : -1;

		if (temp & SDVO_PIPE_B_SELECT) {
			temp &= ~SDVO_PIPE_B_SELECT;
			I915_WRITE(intel_hdmi->hdmi_reg, temp);
			POSTING_READ(intel_hdmi->hdmi_reg);

			/* Again we need to write this twice. */
			I915_WRITE(intel_hdmi->hdmi_reg, temp);
			POSTING_READ(intel_hdmi->hdmi_reg);

			/* Transcoder selection bits only update
			 * effectively on vblank. */
			if (crtc)
				intel_wait_for_vblank(dev, pipe);
			else
				msleep(50);
		}
	}

	/* HW workaround, need to toggle enable bit off and on for 12bpc, but
	 * we do this anyway which shows more stable in testing.
	 */
	if (HAS_PCH_SPLIT(dev)) {
		I915_WRITE(intel_hdmi->hdmi_reg, temp & ~SDVO_ENABLE);
		POSTING_READ(intel_hdmi->hdmi_reg);
	}

	temp &= ~enable_bits;

	I915_WRITE(intel_hdmi->hdmi_reg, temp);
	POSTING_READ(intel_hdmi->hdmi_reg);

	/* HW workaround, need to write this twice for issue that may result
	 * in first write getting masked.
	 */
	if (HAS_PCH_SPLIT(dev)) {
		I915_WRITE(intel_hdmi->hdmi_reg, temp);
		POSTING_READ(intel_hdmi->hdmi_reg);
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static int intel_hdmi_mode_valid(struct drm_connector *connector,
				 struct drm_display_mode *mode)
{
	if (mode->clock > 165000)
		return MODE_CLOCK_HIGH;
	if (mode->clock < 20000)
=======
=======
>>>>>>> v3.18
static int hdmi_portclock_limit(struct intel_hdmi *hdmi, bool respect_dvi_limit)
{
	struct drm_device *dev = intel_hdmi_to_dev(hdmi);

	if ((respect_dvi_limit && !hdmi->has_hdmi_sink) || IS_G4X(dev))
		return 165000;
	else if (IS_HASWELL(dev) || INTEL_INFO(dev)->gen >= 8)
		return 300000;
	else
		return 225000;
}

static enum drm_mode_status
intel_hdmi_mode_valid(struct drm_connector *connector,
		      struct drm_display_mode *mode)
{
	int clock = mode->clock;

	if (mode->flags & DRM_MODE_FLAG_DBLCLK)
		clock *= 2;

	if (clock > hdmi_portclock_limit(intel_attached_hdmi(connector),
					 true))
		return MODE_CLOCK_HIGH;
	if (clock < 20000)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return MODE_CLOCK_LOW;

	if (mode->flags & DRM_MODE_FLAG_DBLSCAN)
		return MODE_NO_DBLESCAN;

	return MODE_OK;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
static bool hdmi_12bpc_possible(struct intel_crtc *crtc)
{
	struct drm_device *dev = crtc->base.dev;
	struct intel_encoder *encoder;
	int count = 0, count_hdmi = 0;

	if (HAS_GMCH_DISPLAY(dev))
		return false;

	for_each_intel_encoder(dev, encoder) {
		if (encoder->new_crtc != crtc)
			continue;

		count_hdmi += encoder->type == INTEL_OUTPUT_HDMI;
		count++;
	}

	/*
	 * HDMI 12bpc affects the clocks, so it's only possible
	 * when not cloning with other encoder types.
	 */
	return count_hdmi > 0 && count_hdmi == count;
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
bool intel_hdmi_compute_config(struct intel_encoder *encoder,
			       struct intel_crtc_config *pipe_config)
{
	struct intel_hdmi *intel_hdmi = enc_to_intel_hdmi(&encoder->base);
	struct drm_device *dev = encoder->base.dev;
	struct drm_display_mode *adjusted_mode = &pipe_config->adjusted_mode;
<<<<<<< HEAD
<<<<<<< HEAD

	if (intel_hdmi->color_range_auto) {
		/* See CEA-861-E - 5.1 Default Encoding Parameters */
		if (intel_hdmi->has_hdmi_sink &&
=======
=======
>>>>>>> v3.18
	int clock_12bpc = pipe_config->adjusted_mode.crtc_clock * 3 / 2;
	int portclock_limit = hdmi_portclock_limit(intel_hdmi, false);
	int desired_bpp;

	pipe_config->has_hdmi_sink = intel_hdmi->has_hdmi_sink;

	if (intel_hdmi->color_range_auto) {
		/* See CEA-861-E - 5.1 Default Encoding Parameters */
		if (pipe_config->has_hdmi_sink &&
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		    drm_match_cea_mode(adjusted_mode) > 1)
			intel_hdmi->color_range = HDMI_COLOR_RANGE_16_235;
		else
			intel_hdmi->color_range = 0;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	if (adjusted_mode->flags & DRM_MODE_FLAG_DBLCLK) {
		pipe_config->pixel_multiplier = 2;
	}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (intel_hdmi->color_range)
		pipe_config->limited_color_range = true;

	if (HAS_PCH_SPLIT(dev) && !HAS_DDI(dev))
		pipe_config->has_pch_encoder = true;

<<<<<<< HEAD
<<<<<<< HEAD
	/*
	 * HDMI is either 12 or 8, so if the display lets 10bpc sneak
	 * through, clamp it down. Note that g4x/vlv don't support 12bpc hdmi
	 * outputs.
	 */
	if (pipe_config->pipe_bpp > 8*3 && HAS_PCH_SPLIT(dev)) {
		DRM_DEBUG_KMS("forcing bpc to 12 for HDMI\n");
		pipe_config->pipe_bpp = 12*3;
	} else {
		DRM_DEBUG_KMS("forcing bpc to 8 for HDMI\n");
		pipe_config->pipe_bpp = 8*3;
=======
=======
>>>>>>> v3.18
	if (pipe_config->has_hdmi_sink && intel_hdmi->has_audio)
		pipe_config->has_audio = true;

	/*
	 * HDMI is either 12 or 8, so if the display lets 10bpc sneak
	 * through, clamp it down. Note that g4x/vlv don't support 12bpc hdmi
	 * outputs. We also need to check that the higher clock still fits
	 * within limits.
	 */
	if (pipe_config->pipe_bpp > 8*3 && pipe_config->has_hdmi_sink &&
	    clock_12bpc <= portclock_limit &&
	    hdmi_12bpc_possible(encoder->new_crtc)) {
		DRM_DEBUG_KMS("picking bpc to 12 for HDMI output\n");
		desired_bpp = 12*3;

		/* Need to adjust the port link by 1.5x for 12bpc. */
		pipe_config->port_clock = clock_12bpc;
	} else {
		DRM_DEBUG_KMS("picking bpc to 8 for HDMI output\n");
		desired_bpp = 8*3;
	}

	if (!pipe_config->bw_constrained) {
		DRM_DEBUG_KMS("forcing pipe bpc to %i for HDMI\n", desired_bpp);
		pipe_config->pipe_bpp = desired_bpp;
	}

	if (adjusted_mode->crtc_clock > portclock_limit) {
		DRM_DEBUG_KMS("too high HDMI clock, rejecting mode\n");
		return false;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}

	return true;
}

<<<<<<< HEAD
<<<<<<< HEAD
static enum drm_connector_status
intel_hdmi_detect(struct drm_connector *connector, bool force)
{
	struct drm_device *dev = connector->dev;
	struct intel_hdmi *intel_hdmi = intel_attached_hdmi(connector);
	struct intel_digital_port *intel_dig_port =
		hdmi_to_dig_port(intel_hdmi);
	struct intel_encoder *intel_encoder = &intel_dig_port->base;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct edid *edid;
	enum drm_connector_status status = connector_status_disconnected;
=======
=======
>>>>>>> v3.18
static void
intel_hdmi_unset_edid(struct drm_connector *connector)
{
	struct intel_hdmi *intel_hdmi = intel_attached_hdmi(connector);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	intel_hdmi->has_hdmi_sink = false;
	intel_hdmi->has_audio = false;
	intel_hdmi->rgb_quant_range_selectable = false;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18

	kfree(to_intel_connector(connector)->detect_edid);
	to_intel_connector(connector)->detect_edid = NULL;
}

static bool
intel_hdmi_set_edid(struct drm_connector *connector)
{
	struct drm_i915_private *dev_priv = to_i915(connector->dev);
	struct intel_hdmi *intel_hdmi = intel_attached_hdmi(connector);
	struct intel_encoder *intel_encoder =
		&hdmi_to_dig_port(intel_hdmi)->base;
	enum intel_display_power_domain power_domain;
	struct edid *edid;
	bool connected = false;

	power_domain = intel_display_port_power_domain(intel_encoder);
	intel_display_power_get(dev_priv, power_domain);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	edid = drm_get_edid(connector,
			    intel_gmbus_get_adapter(dev_priv,
						    intel_hdmi->ddc_bus));

<<<<<<< HEAD
<<<<<<< HEAD
	if (edid) {
		if (edid->input & DRM_EDID_INPUT_DIGITAL) {
			status = connector_status_connected;
			if (intel_hdmi->force_audio != HDMI_AUDIO_OFF_DVI)
				intel_hdmi->has_hdmi_sink =
						drm_detect_hdmi_monitor(edid);
			intel_hdmi->has_audio = drm_detect_monitor_audio(edid);
			intel_hdmi->rgb_quant_range_selectable =
				drm_rgb_quant_range_selectable(edid);
		}
		kfree(edid);
	}

	if (status == connector_status_connected) {
		if (intel_hdmi->force_audio != HDMI_AUDIO_AUTO)
			intel_hdmi->has_audio =
				(intel_hdmi->force_audio == HDMI_AUDIO_ON);
		intel_encoder->type = INTEL_OUTPUT_HDMI;
	}

	return status;
}

static int intel_hdmi_get_modes(struct drm_connector *connector)
{
	struct intel_hdmi *intel_hdmi = intel_attached_hdmi(connector);
	struct drm_i915_private *dev_priv = connector->dev->dev_private;

	/* We should parse the EDID data and find out if it's an HDMI sink so
	 * we can send audio to it.
	 */

	return intel_ddc_get_modes(connector,
				   intel_gmbus_get_adapter(dev_priv,
							   intel_hdmi->ddc_bus));
=======
=======
>>>>>>> v3.18
	intel_display_power_put(dev_priv, power_domain);

	to_intel_connector(connector)->detect_edid = edid;
	if (edid && edid->input & DRM_EDID_INPUT_DIGITAL) {
		intel_hdmi->rgb_quant_range_selectable =
			drm_rgb_quant_range_selectable(edid);

		intel_hdmi->has_audio = drm_detect_monitor_audio(edid);
		if (intel_hdmi->force_audio != HDMI_AUDIO_AUTO)
			intel_hdmi->has_audio =
				intel_hdmi->force_audio == HDMI_AUDIO_ON;

		if (intel_hdmi->force_audio != HDMI_AUDIO_OFF_DVI)
			intel_hdmi->has_hdmi_sink =
				drm_detect_hdmi_monitor(edid);

		connected = true;
	}

	return connected;
}

static enum drm_connector_status
intel_hdmi_detect(struct drm_connector *connector, bool force)
{
	enum drm_connector_status status;

	DRM_DEBUG_KMS("[CONNECTOR:%d:%s]\n",
		      connector->base.id, connector->name);

	intel_hdmi_unset_edid(connector);

	if (intel_hdmi_set_edid(connector)) {
		struct intel_hdmi *intel_hdmi = intel_attached_hdmi(connector);

		hdmi_to_dig_port(intel_hdmi)->base.type = INTEL_OUTPUT_HDMI;
		status = connector_status_connected;
	} else
		status = connector_status_disconnected;

	return status;
}

static void
intel_hdmi_force(struct drm_connector *connector)
{
	struct intel_hdmi *intel_hdmi = intel_attached_hdmi(connector);

	DRM_DEBUG_KMS("[CONNECTOR:%d:%s]\n",
		      connector->base.id, connector->name);

	intel_hdmi_unset_edid(connector);

	if (connector->status != connector_status_connected)
		return;

	intel_hdmi_set_edid(connector);
	hdmi_to_dig_port(intel_hdmi)->base.type = INTEL_OUTPUT_HDMI;
}

static int intel_hdmi_get_modes(struct drm_connector *connector)
{
	struct edid *edid;

	edid = to_intel_connector(connector)->detect_edid;
	if (edid == NULL)
		return 0;

	return intel_connector_update_modes(connector, edid);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static bool
intel_hdmi_detect_audio(struct drm_connector *connector)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct intel_hdmi *intel_hdmi = intel_attached_hdmi(connector);
	struct drm_i915_private *dev_priv = connector->dev->dev_private;
	struct edid *edid;
	bool has_audio = false;

	edid = drm_get_edid(connector,
			    intel_gmbus_get_adapter(dev_priv,
						    intel_hdmi->ddc_bus));
	if (edid) {
		if (edid->input & DRM_EDID_INPUT_DIGITAL)
			has_audio = drm_detect_monitor_audio(edid);
		kfree(edid);
	}
=======
=======
>>>>>>> v3.18
	bool has_audio = false;
	struct edid *edid;

	edid = to_intel_connector(connector)->detect_edid;
	if (edid && edid->input & DRM_EDID_INPUT_DIGITAL)
		has_audio = drm_detect_monitor_audio(edid);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return has_audio;
}

static int
intel_hdmi_set_property(struct drm_connector *connector,
			struct drm_property *property,
			uint64_t val)
{
	struct intel_hdmi *intel_hdmi = intel_attached_hdmi(connector);
	struct intel_digital_port *intel_dig_port =
		hdmi_to_dig_port(intel_hdmi);
	struct drm_i915_private *dev_priv = connector->dev->dev_private;
	int ret;

	ret = drm_object_property_set_value(&connector->base, property, val);
	if (ret)
		return ret;

	if (property == dev_priv->force_audio_property) {
		enum hdmi_force_audio i = val;
		bool has_audio;

		if (i == intel_hdmi->force_audio)
			return 0;

		intel_hdmi->force_audio = i;

		if (i == HDMI_AUDIO_AUTO)
			has_audio = intel_hdmi_detect_audio(connector);
		else
			has_audio = (i == HDMI_AUDIO_ON);

		if (i == HDMI_AUDIO_OFF_DVI)
			intel_hdmi->has_hdmi_sink = 0;

		intel_hdmi->has_audio = has_audio;
		goto done;
	}

	if (property == dev_priv->broadcast_rgb_property) {
		bool old_auto = intel_hdmi->color_range_auto;
		uint32_t old_range = intel_hdmi->color_range;

		switch (val) {
		case INTEL_BROADCAST_RGB_AUTO:
			intel_hdmi->color_range_auto = true;
			break;
		case INTEL_BROADCAST_RGB_FULL:
			intel_hdmi->color_range_auto = false;
			intel_hdmi->color_range = 0;
			break;
		case INTEL_BROADCAST_RGB_LIMITED:
			intel_hdmi->color_range_auto = false;
			intel_hdmi->color_range = HDMI_COLOR_RANGE_16_235;
			break;
		default:
			return -EINVAL;
		}

		if (old_auto == intel_hdmi->color_range_auto &&
		    old_range == intel_hdmi->color_range)
			return 0;

		goto done;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	if (property == connector->dev->mode_config.aspect_ratio_property) {
		switch (val) {
		case DRM_MODE_PICTURE_ASPECT_NONE:
			intel_hdmi->aspect_ratio = HDMI_PICTURE_ASPECT_NONE;
			break;
		case DRM_MODE_PICTURE_ASPECT_4_3:
			intel_hdmi->aspect_ratio = HDMI_PICTURE_ASPECT_4_3;
			break;
		case DRM_MODE_PICTURE_ASPECT_16_9:
			intel_hdmi->aspect_ratio = HDMI_PICTURE_ASPECT_16_9;
			break;
		default:
			return -EINVAL;
		}
		goto done;
	}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return -EINVAL;

done:
	if (intel_dig_port->base.base.crtc)
		intel_crtc_restore_mode(intel_dig_port->base.base.crtc);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void intel_hdmi_destroy(struct drm_connector *connector)
{
	drm_sysfs_connector_remove(connector);
=======
=======
>>>>>>> v3.18
static void intel_hdmi_pre_enable(struct intel_encoder *encoder)
{
	struct intel_hdmi *intel_hdmi = enc_to_intel_hdmi(&encoder->base);
	struct intel_crtc *intel_crtc = to_intel_crtc(encoder->base.crtc);
	struct drm_display_mode *adjusted_mode =
		&intel_crtc->config.adjusted_mode;

	intel_hdmi_prepare(encoder);

	intel_hdmi->set_infoframes(&encoder->base,
				   intel_crtc->config.has_hdmi_sink,
				   adjusted_mode);
}

static void vlv_hdmi_pre_enable(struct intel_encoder *encoder)
{
	struct intel_digital_port *dport = enc_to_dig_port(&encoder->base);
	struct intel_hdmi *intel_hdmi = &dport->hdmi;
	struct drm_device *dev = encoder->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc =
		to_intel_crtc(encoder->base.crtc);
	struct drm_display_mode *adjusted_mode =
		&intel_crtc->config.adjusted_mode;
	enum dpio_channel port = vlv_dport_to_channel(dport);
	int pipe = intel_crtc->pipe;
	u32 val;

	/* Enable clock channels for this port */
	mutex_lock(&dev_priv->dpio_lock);
	val = vlv_dpio_read(dev_priv, pipe, VLV_PCS01_DW8(port));
	val = 0;
	if (pipe)
		val |= (1<<21);
	else
		val &= ~(1<<21);
	val |= 0x001000c4;
	vlv_dpio_write(dev_priv, pipe, VLV_PCS_DW8(port), val);

	/* HDMI 1.0V-2dB */
	vlv_dpio_write(dev_priv, pipe, VLV_TX_DW5(port), 0);
	vlv_dpio_write(dev_priv, pipe, VLV_TX_DW4(port), 0x2b245f5f);
	vlv_dpio_write(dev_priv, pipe, VLV_TX_DW2(port), 0x5578b83a);
	vlv_dpio_write(dev_priv, pipe, VLV_TX_DW3(port), 0x0c782040);
	vlv_dpio_write(dev_priv, pipe, VLV_TX3_DW4(port), 0x2b247878);
	vlv_dpio_write(dev_priv, pipe, VLV_PCS_DW11(port), 0x00030000);
	vlv_dpio_write(dev_priv, pipe, VLV_PCS_DW9(port), 0x00002000);
	vlv_dpio_write(dev_priv, pipe, VLV_TX_DW5(port), DPIO_TX_OCALINIT_EN);

	/* Program lane clock */
	vlv_dpio_write(dev_priv, pipe, VLV_PCS_DW14(port), 0x00760018);
	vlv_dpio_write(dev_priv, pipe, VLV_PCS_DW23(port), 0x00400888);
	mutex_unlock(&dev_priv->dpio_lock);

	intel_hdmi->set_infoframes(&encoder->base,
				   intel_crtc->config.has_hdmi_sink,
				   adjusted_mode);

	intel_enable_hdmi(encoder);

	vlv_wait_port_ready(dev_priv, dport);
}

static void vlv_hdmi_pre_pll_enable(struct intel_encoder *encoder)
{
	struct intel_digital_port *dport = enc_to_dig_port(&encoder->base);
	struct drm_device *dev = encoder->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc =
		to_intel_crtc(encoder->base.crtc);
	enum dpio_channel port = vlv_dport_to_channel(dport);
	int pipe = intel_crtc->pipe;

	intel_hdmi_prepare(encoder);

	/* Program Tx lane resets to default */
	mutex_lock(&dev_priv->dpio_lock);
	vlv_dpio_write(dev_priv, pipe, VLV_PCS_DW0(port),
			 DPIO_PCS_TX_LANE2_RESET |
			 DPIO_PCS_TX_LANE1_RESET);
	vlv_dpio_write(dev_priv, pipe, VLV_PCS_DW1(port),
			 DPIO_PCS_CLK_CRI_RXEB_EIOS_EN |
			 DPIO_PCS_CLK_CRI_RXDIGFILTSG_EN |
			 (1<<DPIO_PCS_CLK_DATAWIDTH_SHIFT) |
			 DPIO_PCS_CLK_SOFT_RESET);

	/* Fix up inter-pair skew failure */
	vlv_dpio_write(dev_priv, pipe, VLV_PCS_DW12(port), 0x00750f00);
	vlv_dpio_write(dev_priv, pipe, VLV_TX_DW11(port), 0x00001500);
	vlv_dpio_write(dev_priv, pipe, VLV_TX_DW14(port), 0x40400000);

	vlv_dpio_write(dev_priv, pipe, VLV_PCS_DW9(port), 0x00002000);
	vlv_dpio_write(dev_priv, pipe, VLV_TX_DW5(port), DPIO_TX_OCALINIT_EN);
	mutex_unlock(&dev_priv->dpio_lock);
}

static void chv_hdmi_pre_pll_enable(struct intel_encoder *encoder)
{
	struct intel_digital_port *dport = enc_to_dig_port(&encoder->base);
	struct drm_device *dev = encoder->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc =
		to_intel_crtc(encoder->base.crtc);
	enum dpio_channel ch = vlv_dport_to_channel(dport);
	enum pipe pipe = intel_crtc->pipe;
	u32 val;

	intel_hdmi_prepare(encoder);

	mutex_lock(&dev_priv->dpio_lock);

	/* program left/right clock distribution */
	if (pipe != PIPE_B) {
		val = vlv_dpio_read(dev_priv, pipe, _CHV_CMN_DW5_CH0);
		val &= ~(CHV_BUFLEFTENA1_MASK | CHV_BUFRIGHTENA1_MASK);
		if (ch == DPIO_CH0)
			val |= CHV_BUFLEFTENA1_FORCE;
		if (ch == DPIO_CH1)
			val |= CHV_BUFRIGHTENA1_FORCE;
		vlv_dpio_write(dev_priv, pipe, _CHV_CMN_DW5_CH0, val);
	} else {
		val = vlv_dpio_read(dev_priv, pipe, _CHV_CMN_DW1_CH1);
		val &= ~(CHV_BUFLEFTENA2_MASK | CHV_BUFRIGHTENA2_MASK);
		if (ch == DPIO_CH0)
			val |= CHV_BUFLEFTENA2_FORCE;
		if (ch == DPIO_CH1)
			val |= CHV_BUFRIGHTENA2_FORCE;
		vlv_dpio_write(dev_priv, pipe, _CHV_CMN_DW1_CH1, val);
	}

	/* program clock channel usage */
	val = vlv_dpio_read(dev_priv, pipe, VLV_PCS01_DW8(ch));
	val |= CHV_PCS_USEDCLKCHANNEL_OVRRIDE;
	if (pipe != PIPE_B)
		val &= ~CHV_PCS_USEDCLKCHANNEL;
	else
		val |= CHV_PCS_USEDCLKCHANNEL;
	vlv_dpio_write(dev_priv, pipe, VLV_PCS01_DW8(ch), val);

	val = vlv_dpio_read(dev_priv, pipe, VLV_PCS23_DW8(ch));
	val |= CHV_PCS_USEDCLKCHANNEL_OVRRIDE;
	if (pipe != PIPE_B)
		val &= ~CHV_PCS_USEDCLKCHANNEL;
	else
		val |= CHV_PCS_USEDCLKCHANNEL;
	vlv_dpio_write(dev_priv, pipe, VLV_PCS23_DW8(ch), val);

	/*
	 * This a a bit weird since generally CL
	 * matches the pipe, but here we need to
	 * pick the CL based on the port.
	 */
	val = vlv_dpio_read(dev_priv, pipe, CHV_CMN_DW19(ch));
	if (pipe != PIPE_B)
		val &= ~CHV_CMN_USEDCLKCHANNEL;
	else
		val |= CHV_CMN_USEDCLKCHANNEL;
	vlv_dpio_write(dev_priv, pipe, CHV_CMN_DW19(ch), val);

	mutex_unlock(&dev_priv->dpio_lock);
}

static void vlv_hdmi_post_disable(struct intel_encoder *encoder)
{
	struct intel_digital_port *dport = enc_to_dig_port(&encoder->base);
	struct drm_i915_private *dev_priv = encoder->base.dev->dev_private;
	struct intel_crtc *intel_crtc =
		to_intel_crtc(encoder->base.crtc);
	enum dpio_channel port = vlv_dport_to_channel(dport);
	int pipe = intel_crtc->pipe;

	/* Reset lanes to avoid HDMI flicker (VLV w/a) */
	mutex_lock(&dev_priv->dpio_lock);
	vlv_dpio_write(dev_priv, pipe, VLV_PCS_DW0(port), 0x00000000);
	vlv_dpio_write(dev_priv, pipe, VLV_PCS_DW1(port), 0x00e00060);
	mutex_unlock(&dev_priv->dpio_lock);
}

static void chv_hdmi_post_disable(struct intel_encoder *encoder)
{
	struct intel_digital_port *dport = enc_to_dig_port(&encoder->base);
	struct drm_device *dev = encoder->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc =
		to_intel_crtc(encoder->base.crtc);
	enum dpio_channel ch = vlv_dport_to_channel(dport);
	enum pipe pipe = intel_crtc->pipe;
	u32 val;

	mutex_lock(&dev_priv->dpio_lock);

	/* Propagate soft reset to data lane reset */
	val = vlv_dpio_read(dev_priv, pipe, VLV_PCS01_DW1(ch));
	val |= CHV_PCS_REQ_SOFTRESET_EN;
	vlv_dpio_write(dev_priv, pipe, VLV_PCS01_DW1(ch), val);

	val = vlv_dpio_read(dev_priv, pipe, VLV_PCS23_DW1(ch));
	val |= CHV_PCS_REQ_SOFTRESET_EN;
	vlv_dpio_write(dev_priv, pipe, VLV_PCS23_DW1(ch), val);

	val = vlv_dpio_read(dev_priv, pipe, VLV_PCS01_DW0(ch));
	val &= ~(DPIO_PCS_TX_LANE2_RESET | DPIO_PCS_TX_LANE1_RESET);
	vlv_dpio_write(dev_priv, pipe, VLV_PCS01_DW0(ch), val);

	val = vlv_dpio_read(dev_priv, pipe, VLV_PCS23_DW0(ch));
	val &= ~(DPIO_PCS_TX_LANE2_RESET | DPIO_PCS_TX_LANE1_RESET);
	vlv_dpio_write(dev_priv, pipe, VLV_PCS23_DW0(ch), val);

	mutex_unlock(&dev_priv->dpio_lock);
}

static void chv_hdmi_pre_enable(struct intel_encoder *encoder)
{
	struct intel_digital_port *dport = enc_to_dig_port(&encoder->base);
	struct drm_device *dev = encoder->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc =
		to_intel_crtc(encoder->base.crtc);
	enum dpio_channel ch = vlv_dport_to_channel(dport);
	int pipe = intel_crtc->pipe;
	int data, i;
	u32 val;

	mutex_lock(&dev_priv->dpio_lock);

	/* Deassert soft data lane reset*/
	val = vlv_dpio_read(dev_priv, pipe, VLV_PCS01_DW1(ch));
	val |= CHV_PCS_REQ_SOFTRESET_EN;
	vlv_dpio_write(dev_priv, pipe, VLV_PCS01_DW1(ch), val);

	val = vlv_dpio_read(dev_priv, pipe, VLV_PCS23_DW1(ch));
	val |= CHV_PCS_REQ_SOFTRESET_EN;
	vlv_dpio_write(dev_priv, pipe, VLV_PCS23_DW1(ch), val);

	val = vlv_dpio_read(dev_priv, pipe, VLV_PCS01_DW0(ch));
	val |= (DPIO_PCS_TX_LANE2_RESET | DPIO_PCS_TX_LANE1_RESET);
	vlv_dpio_write(dev_priv, pipe, VLV_PCS01_DW0(ch), val);

	val = vlv_dpio_read(dev_priv, pipe, VLV_PCS23_DW0(ch));
	val |= (DPIO_PCS_TX_LANE2_RESET | DPIO_PCS_TX_LANE1_RESET);
	vlv_dpio_write(dev_priv, pipe, VLV_PCS23_DW0(ch), val);

	/* Program Tx latency optimal setting */
	for (i = 0; i < 4; i++) {
		/* Set the latency optimal bit */
		data = (i == 1) ? 0x0 : 0x6;
		vlv_dpio_write(dev_priv, pipe, CHV_TX_DW11(ch, i),
				data << DPIO_FRC_LATENCY_SHFIT);

		/* Set the upar bit */
		data = (i == 1) ? 0x0 : 0x1;
		vlv_dpio_write(dev_priv, pipe, CHV_TX_DW14(ch, i),
				data << DPIO_UPAR_SHIFT);
	}

	/* Data lane stagger programming */
	/* FIXME: Fix up value only after power analysis */

	/* Clear calc init */
	val = vlv_dpio_read(dev_priv, pipe, VLV_PCS01_DW10(ch));
	val &= ~(DPIO_PCS_SWING_CALC_TX0_TX2 | DPIO_PCS_SWING_CALC_TX1_TX3);
	vlv_dpio_write(dev_priv, pipe, VLV_PCS01_DW10(ch), val);

	val = vlv_dpio_read(dev_priv, pipe, VLV_PCS23_DW10(ch));
	val &= ~(DPIO_PCS_SWING_CALC_TX0_TX2 | DPIO_PCS_SWING_CALC_TX1_TX3);
	vlv_dpio_write(dev_priv, pipe, VLV_PCS23_DW10(ch), val);

	/* FIXME: Program the support xxx V-dB */
	/* Use 800mV-0dB */
	for (i = 0; i < 4; i++) {
		val = vlv_dpio_read(dev_priv, pipe, CHV_TX_DW4(ch, i));
		val &= ~DPIO_SWING_DEEMPH9P5_MASK;
		val |= 128 << DPIO_SWING_DEEMPH9P5_SHIFT;
		vlv_dpio_write(dev_priv, pipe, CHV_TX_DW4(ch, i), val);
	}

	for (i = 0; i < 4; i++) {
		val = vlv_dpio_read(dev_priv, pipe, CHV_TX_DW2(ch, i));
		val &= ~DPIO_SWING_MARGIN000_MASK;
		val |= 102 << DPIO_SWING_MARGIN000_SHIFT;
		vlv_dpio_write(dev_priv, pipe, CHV_TX_DW2(ch, i), val);
	}

	/* Disable unique transition scale */
	for (i = 0; i < 4; i++) {
		val = vlv_dpio_read(dev_priv, pipe, CHV_TX_DW3(ch, i));
		val &= ~DPIO_TX_UNIQ_TRANS_SCALE_EN;
		vlv_dpio_write(dev_priv, pipe, CHV_TX_DW3(ch, i), val);
	}

	/* Additional steps for 1200mV-0dB */
#if 0
	val = vlv_dpio_read(dev_priv, pipe, VLV_TX_DW3(ch));
	if (ch)
		val |= DPIO_TX_UNIQ_TRANS_SCALE_CH1;
	else
		val |= DPIO_TX_UNIQ_TRANS_SCALE_CH0;
	vlv_dpio_write(dev_priv, pipe, VLV_TX_DW3(ch), val);

	vlv_dpio_write(dev_priv, pipe, VLV_TX_DW2(ch),
			vlv_dpio_read(dev_priv, pipe, VLV_TX_DW2(ch)) |
				(0x9a << DPIO_UNIQ_TRANS_SCALE_SHIFT));
#endif
	/* Start swing calculation */
	val = vlv_dpio_read(dev_priv, pipe, VLV_PCS01_DW10(ch));
	val |= DPIO_PCS_SWING_CALC_TX0_TX2 | DPIO_PCS_SWING_CALC_TX1_TX3;
	vlv_dpio_write(dev_priv, pipe, VLV_PCS01_DW10(ch), val);

	val = vlv_dpio_read(dev_priv, pipe, VLV_PCS23_DW10(ch));
	val |= DPIO_PCS_SWING_CALC_TX0_TX2 | DPIO_PCS_SWING_CALC_TX1_TX3;
	vlv_dpio_write(dev_priv, pipe, VLV_PCS23_DW10(ch), val);

	/* LRC Bypass */
	val = vlv_dpio_read(dev_priv, pipe, CHV_CMN_DW30);
	val |= DPIO_LRC_BYPASS;
	vlv_dpio_write(dev_priv, pipe, CHV_CMN_DW30, val);

	mutex_unlock(&dev_priv->dpio_lock);

	intel_enable_hdmi(encoder);

	vlv_wait_port_ready(dev_priv, dport);
}

static void intel_hdmi_destroy(struct drm_connector *connector)
{
	kfree(to_intel_connector(connector)->detect_edid);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	drm_connector_cleanup(connector);
	kfree(connector);
}

<<<<<<< HEAD
<<<<<<< HEAD
static const struct drm_encoder_helper_funcs intel_hdmi_helper_funcs = {
	.mode_set = intel_hdmi_mode_set,
};

static const struct drm_connector_funcs intel_hdmi_connector_funcs = {
	.dpms = intel_connector_dpms,
	.detect = intel_hdmi_detect,
=======
=======
>>>>>>> v3.18
static const struct drm_connector_funcs intel_hdmi_connector_funcs = {
	.dpms = intel_connector_dpms,
	.detect = intel_hdmi_detect,
	.force = intel_hdmi_force,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	.fill_modes = drm_helper_probe_single_connector_modes,
	.set_property = intel_hdmi_set_property,
	.destroy = intel_hdmi_destroy,
};

static const struct drm_connector_helper_funcs intel_hdmi_connector_helper_funcs = {
	.get_modes = intel_hdmi_get_modes,
	.mode_valid = intel_hdmi_mode_valid,
	.best_encoder = intel_best_encoder,
};

static const struct drm_encoder_funcs intel_hdmi_enc_funcs = {
	.destroy = intel_encoder_destroy,
};

static void
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
intel_attach_aspect_ratio_property(struct drm_connector *connector)
{
	if (!drm_mode_create_aspect_ratio_property(connector->dev))
		drm_object_attach_property(&connector->base,
			connector->dev->mode_config.aspect_ratio_property,
			DRM_MODE_PICTURE_ASPECT_NONE);
}

static void
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
intel_hdmi_add_properties(struct intel_hdmi *intel_hdmi, struct drm_connector *connector)
{
	intel_attach_force_audio_property(connector);
	intel_attach_broadcast_rgb_property(connector);
	intel_hdmi->color_range_auto = true;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	intel_attach_aspect_ratio_property(connector);
	intel_hdmi->aspect_ratio = HDMI_PICTURE_ASPECT_NONE;
>>>>>>> v3.18
=======
	intel_attach_aspect_ratio_property(connector);
	intel_hdmi->aspect_ratio = HDMI_PICTURE_ASPECT_NONE;
>>>>>>> v3.18
}

void intel_hdmi_init_connector(struct intel_digital_port *intel_dig_port,
			       struct intel_connector *intel_connector)
{
	struct drm_connector *connector = &intel_connector->base;
	struct intel_hdmi *intel_hdmi = &intel_dig_port->hdmi;
	struct intel_encoder *intel_encoder = &intel_dig_port->base;
	struct drm_device *dev = intel_encoder->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	enum port port = intel_dig_port->port;

	drm_connector_init(dev, connector, &intel_hdmi_connector_funcs,
			   DRM_MODE_CONNECTOR_HDMIA);
	drm_connector_helper_add(connector, &intel_hdmi_connector_helper_funcs);

	connector->interlace_allowed = 1;
	connector->doublescan_allowed = 0;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	connector->stereo_allowed = 1;
>>>>>>> v3.18
=======
	connector->stereo_allowed = 1;
>>>>>>> v3.18

	switch (port) {
	case PORT_B:
		intel_hdmi->ddc_bus = GMBUS_PORT_DPB;
		intel_encoder->hpd_pin = HPD_PORT_B;
		break;
	case PORT_C:
		intel_hdmi->ddc_bus = GMBUS_PORT_DPC;
		intel_encoder->hpd_pin = HPD_PORT_C;
		break;
	case PORT_D:
<<<<<<< HEAD
<<<<<<< HEAD
		intel_hdmi->ddc_bus = GMBUS_PORT_DPD;
=======
=======
>>>>>>> v3.18
		if (IS_CHERRYVIEW(dev))
			intel_hdmi->ddc_bus = GMBUS_PORT_DPD_CHV;
		else
			intel_hdmi->ddc_bus = GMBUS_PORT_DPD;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		intel_encoder->hpd_pin = HPD_PORT_D;
		break;
	case PORT_A:
		intel_encoder->hpd_pin = HPD_PORT_A;
		/* Internal port only for eDP. */
	default:
		BUG();
	}

	if (IS_VALLEYVIEW(dev)) {
		intel_hdmi->write_infoframe = vlv_write_infoframe;
		intel_hdmi->set_infoframes = vlv_set_infoframes;
<<<<<<< HEAD
<<<<<<< HEAD
	} else if (!HAS_PCH_SPLIT(dev)) {
=======
	} else if (IS_G4X(dev)) {
>>>>>>> v3.18
=======
	} else if (IS_G4X(dev)) {
>>>>>>> v3.18
		intel_hdmi->write_infoframe = g4x_write_infoframe;
		intel_hdmi->set_infoframes = g4x_set_infoframes;
	} else if (HAS_DDI(dev)) {
		intel_hdmi->write_infoframe = hsw_write_infoframe;
		intel_hdmi->set_infoframes = hsw_set_infoframes;
	} else if (HAS_PCH_IBX(dev)) {
		intel_hdmi->write_infoframe = ibx_write_infoframe;
		intel_hdmi->set_infoframes = ibx_set_infoframes;
	} else {
		intel_hdmi->write_infoframe = cpt_write_infoframe;
		intel_hdmi->set_infoframes = cpt_set_infoframes;
	}

	if (HAS_DDI(dev))
		intel_connector->get_hw_state = intel_ddi_connector_get_hw_state;
	else
		intel_connector->get_hw_state = intel_connector_get_hw_state;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	intel_connector->unregister = intel_connector_unregister;
>>>>>>> v3.18
=======
	intel_connector->unregister = intel_connector_unregister;
>>>>>>> v3.18

	intel_hdmi_add_properties(intel_hdmi, connector);

	intel_connector_attach_encoder(intel_connector, intel_encoder);
<<<<<<< HEAD
<<<<<<< HEAD
	drm_sysfs_connector_add(connector);
=======
	drm_connector_register(connector);
>>>>>>> v3.18
=======
	drm_connector_register(connector);
>>>>>>> v3.18

	/* For G4X desktop chip, PEG_BAND_GAP_DATA 3:0 must first be written
	 * 0xd.  Failure to do so will result in spurious interrupts being
	 * generated on the port when a cable is not attached.
	 */
	if (IS_G4X(dev) && !IS_GM45(dev)) {
		u32 temp = I915_READ(PEG_BAND_GAP_DATA);
		I915_WRITE(PEG_BAND_GAP_DATA, (temp & ~0xf) | 0xd);
	}
}

void intel_hdmi_init(struct drm_device *dev, int hdmi_reg, enum port port)
{
	struct intel_digital_port *intel_dig_port;
	struct intel_encoder *intel_encoder;
<<<<<<< HEAD
<<<<<<< HEAD
	struct drm_encoder *encoder;
	struct intel_connector *intel_connector;

	intel_dig_port = kzalloc(sizeof(struct intel_digital_port), GFP_KERNEL);
	if (!intel_dig_port)
		return;

	intel_connector = kzalloc(sizeof(struct intel_connector), GFP_KERNEL);
=======
=======
>>>>>>> v3.18
	struct intel_connector *intel_connector;

	intel_dig_port = kzalloc(sizeof(*intel_dig_port), GFP_KERNEL);
	if (!intel_dig_port)
		return;

	intel_connector = kzalloc(sizeof(*intel_connector), GFP_KERNEL);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (!intel_connector) {
		kfree(intel_dig_port);
		return;
	}

	intel_encoder = &intel_dig_port->base;
<<<<<<< HEAD
<<<<<<< HEAD
	encoder = &intel_encoder->base;

	drm_encoder_init(dev, &intel_encoder->base, &intel_hdmi_enc_funcs,
			 DRM_MODE_ENCODER_TMDS);
	drm_encoder_helper_add(&intel_encoder->base, &intel_hdmi_helper_funcs);

	intel_encoder->compute_config = intel_hdmi_compute_config;
	intel_encoder->enable = intel_enable_hdmi;
	intel_encoder->disable = intel_disable_hdmi;
	intel_encoder->get_hw_state = intel_hdmi_get_hw_state;

	intel_encoder->type = INTEL_OUTPUT_HDMI;
	intel_encoder->crtc_mask = (1 << 0) | (1 << 1) | (1 << 2);
	intel_encoder->cloneable = false;
=======
=======
>>>>>>> v3.18

	drm_encoder_init(dev, &intel_encoder->base, &intel_hdmi_enc_funcs,
			 DRM_MODE_ENCODER_TMDS);

	intel_encoder->compute_config = intel_hdmi_compute_config;
	intel_encoder->disable = intel_disable_hdmi;
	intel_encoder->get_hw_state = intel_hdmi_get_hw_state;
	intel_encoder->get_config = intel_hdmi_get_config;
	if (IS_CHERRYVIEW(dev)) {
		intel_encoder->pre_pll_enable = chv_hdmi_pre_pll_enable;
		intel_encoder->pre_enable = chv_hdmi_pre_enable;
		intel_encoder->enable = vlv_enable_hdmi;
		intel_encoder->post_disable = chv_hdmi_post_disable;
	} else if (IS_VALLEYVIEW(dev)) {
		intel_encoder->pre_pll_enable = vlv_hdmi_pre_pll_enable;
		intel_encoder->pre_enable = vlv_hdmi_pre_enable;
		intel_encoder->enable = vlv_enable_hdmi;
		intel_encoder->post_disable = vlv_hdmi_post_disable;
	} else {
		intel_encoder->pre_enable = intel_hdmi_pre_enable;
		intel_encoder->enable = intel_enable_hdmi;
	}

	intel_encoder->type = INTEL_OUTPUT_HDMI;
	if (IS_CHERRYVIEW(dev)) {
		if (port == PORT_D)
			intel_encoder->crtc_mask = 1 << 2;
		else
			intel_encoder->crtc_mask = (1 << 0) | (1 << 1);
	} else {
		intel_encoder->crtc_mask = (1 << 0) | (1 << 1) | (1 << 2);
	}
	intel_encoder->cloneable = 1 << INTEL_OUTPUT_ANALOG;
	/*
	 * BSpec is unclear about HDMI+HDMI cloning on g4x, but it seems
	 * to work on real hardware. And since g4x can send infoframes to
	 * only one port anyway, nothing is lost by allowing it.
	 */
	if (IS_G4X(dev))
		intel_encoder->cloneable |= 1 << INTEL_OUTPUT_HDMI;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	intel_dig_port->port = port;
	intel_dig_port->hdmi.hdmi_reg = hdmi_reg;
	intel_dig_port->dp.output_reg = 0;

	intel_hdmi_init_connector(intel_dig_port, intel_connector);
}
