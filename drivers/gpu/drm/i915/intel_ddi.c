/*
 * Copyright © 2012 Intel Corporation
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
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    Eugeni Dodonov <eugeni.dodonov@intel.com>
 *
 */

#include "i915_drv.h"
#include "intel_drv.h"

<<<<<<< HEAD
=======
struct ddi_buf_trans {
	u32 trans1;	/* balance leg enable, de-emph level */
	u32 trans2;	/* vref sel, vswing */
};

>>>>>>> v3.18
/* HDMI/DVI modes ignore everything but the last 2 items. So we share
 * them for both DP and FDI transports, allowing those ports to
 * automatically adapt to HDMI connections as well
 */
<<<<<<< HEAD
static const u32 hsw_ddi_translations_dp[] = {
	0x00FFFFFF, 0x0006000E,		/* DP parameters */
	0x00D75FFF, 0x0005000A,
	0x00C30FFF, 0x00040006,
	0x80AAAFFF, 0x000B0000,
	0x00FFFFFF, 0x0005000A,
	0x00D75FFF, 0x000C0004,
	0x80C30FFF, 0x000B0000,
	0x00FFFFFF, 0x00040006,
	0x80D75FFF, 0x000B0000,
	0x00FFFFFF, 0x00040006		/* HDMI parameters */
};

static const u32 hsw_ddi_translations_fdi[] = {
	0x00FFFFFF, 0x0007000E,		/* FDI parameters */
	0x00D75FFF, 0x000F000A,
	0x00C30FFF, 0x00060006,
	0x00AAAFFF, 0x001E0000,
	0x00FFFFFF, 0x000F000A,
	0x00D75FFF, 0x00160004,
	0x00C30FFF, 0x001E0000,
	0x00FFFFFF, 0x00060006,
	0x00D75FFF, 0x001E0000,
	0x00FFFFFF, 0x00040006		/* HDMI parameters */
};

static enum port intel_ddi_get_encoder_port(struct intel_encoder *intel_encoder)
=======
static const struct ddi_buf_trans hsw_ddi_translations_dp[] = {
	{ 0x00FFFFFF, 0x0006000E },
	{ 0x00D75FFF, 0x0005000A },
	{ 0x00C30FFF, 0x00040006 },
	{ 0x80AAAFFF, 0x000B0000 },
	{ 0x00FFFFFF, 0x0005000A },
	{ 0x00D75FFF, 0x000C0004 },
	{ 0x80C30FFF, 0x000B0000 },
	{ 0x00FFFFFF, 0x00040006 },
	{ 0x80D75FFF, 0x000B0000 },
};

static const struct ddi_buf_trans hsw_ddi_translations_fdi[] = {
	{ 0x00FFFFFF, 0x0007000E },
	{ 0x00D75FFF, 0x000F000A },
	{ 0x00C30FFF, 0x00060006 },
	{ 0x00AAAFFF, 0x001E0000 },
	{ 0x00FFFFFF, 0x000F000A },
	{ 0x00D75FFF, 0x00160004 },
	{ 0x00C30FFF, 0x001E0000 },
	{ 0x00FFFFFF, 0x00060006 },
	{ 0x00D75FFF, 0x001E0000 },
};

static const struct ddi_buf_trans hsw_ddi_translations_hdmi[] = {
					/* Idx	NT mV d	T mV d	db	*/
	{ 0x00FFFFFF, 0x0006000E },	/* 0:	400	400	0	*/
	{ 0x00E79FFF, 0x000E000C },	/* 1:	400	500	2	*/
	{ 0x00D75FFF, 0x0005000A },	/* 2:	400	600	3.5	*/
	{ 0x00FFFFFF, 0x0005000A },	/* 3:	600	600	0	*/
	{ 0x00E79FFF, 0x001D0007 },	/* 4:	600	750	2	*/
	{ 0x00D75FFF, 0x000C0004 },	/* 5:	600	900	3.5	*/
	{ 0x00FFFFFF, 0x00040006 },	/* 6:	800	800	0	*/
	{ 0x80E79FFF, 0x00030002 },	/* 7:	800	1000	2	*/
	{ 0x00FFFFFF, 0x00140005 },	/* 8:	850	850	0	*/
	{ 0x00FFFFFF, 0x000C0004 },	/* 9:	900	900	0	*/
	{ 0x00FFFFFF, 0x001C0003 },	/* 10:	950	950	0	*/
	{ 0x80FFFFFF, 0x00030002 },	/* 11:	1000	1000	0	*/
};

static const struct ddi_buf_trans bdw_ddi_translations_edp[] = {
	{ 0x00FFFFFF, 0x00000012 },
	{ 0x00EBAFFF, 0x00020011 },
	{ 0x00C71FFF, 0x0006000F },
	{ 0x00AAAFFF, 0x000E000A },
	{ 0x00FFFFFF, 0x00020011 },
	{ 0x00DB6FFF, 0x0005000F },
	{ 0x00BEEFFF, 0x000A000C },
	{ 0x00FFFFFF, 0x0005000F },
	{ 0x00DB6FFF, 0x000A000C },
};

static const struct ddi_buf_trans bdw_ddi_translations_dp[] = {
	{ 0x00FFFFFF, 0x0007000E },
	{ 0x00D75FFF, 0x000E000A },
	{ 0x00BEFFFF, 0x00140006 },
	{ 0x80B2CFFF, 0x001B0002 },
	{ 0x00FFFFFF, 0x000E000A },
	{ 0x00D75FFF, 0x00180004 },
	{ 0x80CB2FFF, 0x001B0002 },
	{ 0x00F7DFFF, 0x00180004 },
	{ 0x80D75FFF, 0x001B0002 },
};

static const struct ddi_buf_trans bdw_ddi_translations_fdi[] = {
	{ 0x00FFFFFF, 0x0001000E },
	{ 0x00D75FFF, 0x0004000A },
	{ 0x00C30FFF, 0x00070006 },
	{ 0x00AAAFFF, 0x000C0000 },
	{ 0x00FFFFFF, 0x0004000A },
	{ 0x00D75FFF, 0x00090004 },
	{ 0x00C30FFF, 0x000C0000 },
	{ 0x00FFFFFF, 0x00070006 },
	{ 0x00D75FFF, 0x000C0000 },
};

static const struct ddi_buf_trans bdw_ddi_translations_hdmi[] = {
					/* Idx	NT mV d	T mV df	db	*/
	{ 0x00FFFFFF, 0x0007000E },	/* 0:	400	400	0	*/
	{ 0x00D75FFF, 0x000E000A },	/* 1:	400	600	3.5	*/
	{ 0x00BEFFFF, 0x00140006 },	/* 2:	400	800	6	*/
	{ 0x00FFFFFF, 0x0009000D },	/* 3:	450	450	0	*/
	{ 0x00FFFFFF, 0x000E000A },	/* 4:	600	600	0	*/
	{ 0x00D7FFFF, 0x00140006 },	/* 5:	600	800	2.5	*/
	{ 0x80CB2FFF, 0x001B0002 },	/* 6:	600	1000	4.5	*/
	{ 0x00FFFFFF, 0x00140006 },	/* 7:	800	800	0	*/
	{ 0x80E79FFF, 0x001B0002 },	/* 8:	800	1000	2	*/
	{ 0x80FFFFFF, 0x001B0002 },	/* 9:	1000	1000	0	*/
};

enum port intel_ddi_get_encoder_port(struct intel_encoder *intel_encoder)
>>>>>>> v3.18
{
	struct drm_encoder *encoder = &intel_encoder->base;
	int type = intel_encoder->type;

<<<<<<< HEAD
	if (type == INTEL_OUTPUT_DISPLAYPORT || type == INTEL_OUTPUT_EDP ||
=======
	if (type == INTEL_OUTPUT_DP_MST) {
		struct intel_digital_port *intel_dig_port = enc_to_mst(encoder)->primary;
		return intel_dig_port->port;
	} else if (type == INTEL_OUTPUT_DISPLAYPORT || type == INTEL_OUTPUT_EDP ||
>>>>>>> v3.18
	    type == INTEL_OUTPUT_HDMI || type == INTEL_OUTPUT_UNKNOWN) {
		struct intel_digital_port *intel_dig_port =
			enc_to_dig_port(encoder);
		return intel_dig_port->port;

	} else if (type == INTEL_OUTPUT_ANALOG) {
		return PORT_E;

	} else {
		DRM_ERROR("Invalid DDI encoder type %d\n", type);
		BUG();
	}
}

<<<<<<< HEAD
/* On Haswell, DDI port buffers must be programmed with correct values
 * in advance. The buffer values are different for FDI and DP modes,
=======
/*
 * Starting with Haswell, DDI port buffers must be programmed with correct
 * values in advance. The buffer values are different for FDI and DP modes,
>>>>>>> v3.18
 * but the HDMI/DVI fields are shared among those. So we program the DDI
 * in either FDI or DP modes only, as HDMI connections will work with both
 * of those
 */
<<<<<<< HEAD
static void intel_prepare_ddi_buffers(struct drm_device *dev, enum port port,
				      bool use_fdi_mode)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 reg;
	int i;
	const u32 *ddi_translations = ((use_fdi_mode) ?
		hsw_ddi_translations_fdi :
		hsw_ddi_translations_dp);

	DRM_DEBUG_DRIVER("Initializing DDI buffers for port %c in %s mode\n",
			port_name(port),
			use_fdi_mode ? "FDI" : "DP");

	WARN((use_fdi_mode && (port != PORT_E)),
		"Programming port %c in FDI mode, this probably will not work.\n",
		port_name(port));

	for (i=0, reg=DDI_BUF_TRANS(port); i < ARRAY_SIZE(hsw_ddi_translations_fdi); i++) {
		I915_WRITE(reg, ddi_translations[i]);
		reg += 4;
	}
=======
static void intel_prepare_ddi_buffers(struct drm_device *dev, enum port port)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 reg;
	int i, n_hdmi_entries, hdmi_800mV_0dB;
	int hdmi_level = dev_priv->vbt.ddi_port_info[port].hdmi_level_shift;
	const struct ddi_buf_trans *ddi_translations_fdi;
	const struct ddi_buf_trans *ddi_translations_dp;
	const struct ddi_buf_trans *ddi_translations_edp;
	const struct ddi_buf_trans *ddi_translations_hdmi;
	const struct ddi_buf_trans *ddi_translations;

	if (IS_BROADWELL(dev)) {
		ddi_translations_fdi = bdw_ddi_translations_fdi;
		ddi_translations_dp = bdw_ddi_translations_dp;
		ddi_translations_edp = bdw_ddi_translations_edp;
		ddi_translations_hdmi = bdw_ddi_translations_hdmi;
		n_hdmi_entries = ARRAY_SIZE(bdw_ddi_translations_hdmi);
		hdmi_800mV_0dB = 7;
	} else if (IS_HASWELL(dev)) {
		ddi_translations_fdi = hsw_ddi_translations_fdi;
		ddi_translations_dp = hsw_ddi_translations_dp;
		ddi_translations_edp = hsw_ddi_translations_dp;
		ddi_translations_hdmi = hsw_ddi_translations_hdmi;
		n_hdmi_entries = ARRAY_SIZE(hsw_ddi_translations_hdmi);
		hdmi_800mV_0dB = 6;
	} else {
		WARN(1, "ddi translation table missing\n");
		ddi_translations_edp = bdw_ddi_translations_dp;
		ddi_translations_fdi = bdw_ddi_translations_fdi;
		ddi_translations_dp = bdw_ddi_translations_dp;
		ddi_translations_hdmi = bdw_ddi_translations_hdmi;
		n_hdmi_entries = ARRAY_SIZE(bdw_ddi_translations_hdmi);
		hdmi_800mV_0dB = 7;
	}

	switch (port) {
	case PORT_A:
		ddi_translations = ddi_translations_edp;
		break;
	case PORT_B:
	case PORT_C:
		ddi_translations = ddi_translations_dp;
		break;
	case PORT_D:
		if (intel_dp_is_edp(dev, PORT_D))
			ddi_translations = ddi_translations_edp;
		else
			ddi_translations = ddi_translations_dp;
		break;
	case PORT_E:
		ddi_translations = ddi_translations_fdi;
		break;
	default:
		BUG();
	}

	for (i = 0, reg = DDI_BUF_TRANS(port);
	     i < ARRAY_SIZE(hsw_ddi_translations_fdi); i++) {
		I915_WRITE(reg, ddi_translations[i].trans1);
		reg += 4;
		I915_WRITE(reg, ddi_translations[i].trans2);
		reg += 4;
	}

	/* Choose a good default if VBT is badly populated */
	if (hdmi_level == HDMI_LEVEL_SHIFT_UNKNOWN ||
	    hdmi_level >= n_hdmi_entries)
		hdmi_level = hdmi_800mV_0dB;

	/* Entry 9 is for HDMI: */
	I915_WRITE(reg, ddi_translations_hdmi[hdmi_level].trans1);
	reg += 4;
	I915_WRITE(reg, ddi_translations_hdmi[hdmi_level].trans2);
	reg += 4;
>>>>>>> v3.18
}

/* Program DDI buffers translations for DP. By default, program ports A-D in DP
 * mode and port E for FDI.
 */
void intel_prepare_ddi(struct drm_device *dev)
{
	int port;

	if (!HAS_DDI(dev))
		return;

<<<<<<< HEAD
	for (port = PORT_A; port < PORT_E; port++)
		intel_prepare_ddi_buffers(dev, port, false);

	/* DDI E is the suggested one to work in FDI mode, so program is as such
	 * by default. It will have to be re-programmed in case a digital DP
	 * output will be detected on it
	 */
	intel_prepare_ddi_buffers(dev, PORT_E, true);
}

static const long hsw_ddi_buf_ctl_values[] = {
	DDI_BUF_EMP_400MV_0DB_HSW,
	DDI_BUF_EMP_400MV_3_5DB_HSW,
	DDI_BUF_EMP_400MV_6DB_HSW,
	DDI_BUF_EMP_400MV_9_5DB_HSW,
	DDI_BUF_EMP_600MV_0DB_HSW,
	DDI_BUF_EMP_600MV_3_5DB_HSW,
	DDI_BUF_EMP_600MV_6DB_HSW,
	DDI_BUF_EMP_800MV_0DB_HSW,
	DDI_BUF_EMP_800MV_3_5DB_HSW
};

=======
	for (port = PORT_A; port <= PORT_E; port++)
		intel_prepare_ddi_buffers(dev, port);
}

>>>>>>> v3.18
static void intel_wait_ddi_buf_idle(struct drm_i915_private *dev_priv,
				    enum port port)
{
	uint32_t reg = DDI_BUF_CTL(port);
	int i;

	for (i = 0; i < 8; i++) {
		udelay(1);
		if (I915_READ(reg) & DDI_BUF_IS_IDLE)
			return;
	}
	DRM_ERROR("Timeout waiting for DDI BUF %c idle bit\n", port_name(port));
}

/* Starting with Haswell, different DDI ports can work in FDI mode for
 * connection to the PCH-located connectors. For this, it is necessary to train
 * both the DDI port and PCH receiver for the desired DDI buffer settings.
 *
 * The recommended port to work in FDI mode is DDI E, which we use here. Also,
 * please note that when FDI mode is active on DDI E, it shares 2 lines with
 * DDI A (which is used for eDP)
 */

void hsw_fdi_link_train(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	u32 temp, i, rx_ctl_val;

	/* Set the FDI_RX_MISC pwrdn lanes and the 2 workarounds listed at the
	 * mode set "sequence for CRT port" document:
	 * - TP1 to TP2 time with the default value
	 * - FDI delay to 90h
<<<<<<< HEAD
=======
	 *
	 * WaFDIAutoLinkSetTimingOverrride:hsw
>>>>>>> v3.18
	 */
	I915_WRITE(_FDI_RXA_MISC, FDI_RX_PWRDN_LANE1_VAL(2) |
				  FDI_RX_PWRDN_LANE0_VAL(2) |
				  FDI_RX_TP1_TO_TP2_48 | FDI_RX_FDI_DELAY_90);

	/* Enable the PCH Receiver FDI PLL */
	rx_ctl_val = dev_priv->fdi_rx_config | FDI_RX_ENHANCE_FRAME_ENABLE |
<<<<<<< HEAD
		     FDI_RX_PLL_ENABLE | ((intel_crtc->fdi_lanes - 1) << 19);
=======
		     FDI_RX_PLL_ENABLE |
		     FDI_DP_PORT_WIDTH(intel_crtc->config.fdi_lanes);
>>>>>>> v3.18
	I915_WRITE(_FDI_RXA_CTL, rx_ctl_val);
	POSTING_READ(_FDI_RXA_CTL);
	udelay(220);

	/* Switch from Rawclk to PCDclk */
	rx_ctl_val |= FDI_PCDCLK;
	I915_WRITE(_FDI_RXA_CTL, rx_ctl_val);

	/* Configure Port Clock Select */
<<<<<<< HEAD
	I915_WRITE(PORT_CLK_SEL(PORT_E), intel_crtc->ddi_pll_sel);

	/* Start the training iterating through available voltages and emphasis,
	 * testing each value twice. */
	for (i = 0; i < ARRAY_SIZE(hsw_ddi_buf_ctl_values) * 2; i++) {
=======
	I915_WRITE(PORT_CLK_SEL(PORT_E), intel_crtc->config.ddi_pll_sel);
	WARN_ON(intel_crtc->config.ddi_pll_sel != PORT_CLK_SEL_SPLL);

	/* Start the training iterating through available voltages and emphasis,
	 * testing each value twice. */
	for (i = 0; i < ARRAY_SIZE(hsw_ddi_translations_fdi) * 2; i++) {
>>>>>>> v3.18
		/* Configure DP_TP_CTL with auto-training */
		I915_WRITE(DP_TP_CTL(PORT_E),
					DP_TP_CTL_FDI_AUTOTRAIN |
					DP_TP_CTL_ENHANCED_FRAME_ENABLE |
					DP_TP_CTL_LINK_TRAIN_PAT1 |
					DP_TP_CTL_ENABLE);

		/* Configure and enable DDI_BUF_CTL for DDI E with next voltage.
		 * DDI E does not support port reversal, the functionality is
		 * achieved on the PCH side in FDI_RX_CTL, so no need to set the
		 * port reversal bit */
		I915_WRITE(DDI_BUF_CTL(PORT_E),
			   DDI_BUF_CTL_ENABLE |
<<<<<<< HEAD
			   ((intel_crtc->fdi_lanes - 1) << 1) |
			   hsw_ddi_buf_ctl_values[i / 2]);
=======
			   ((intel_crtc->config.fdi_lanes - 1) << 1) |
			   DDI_BUF_TRANS_SELECT(i / 2));
>>>>>>> v3.18
		POSTING_READ(DDI_BUF_CTL(PORT_E));

		udelay(600);

		/* Program PCH FDI Receiver TU */
		I915_WRITE(_FDI_RXA_TUSIZE1, TU_SIZE(64));

		/* Enable PCH FDI Receiver with auto-training */
		rx_ctl_val |= FDI_RX_ENABLE | FDI_LINK_TRAIN_AUTO;
		I915_WRITE(_FDI_RXA_CTL, rx_ctl_val);
		POSTING_READ(_FDI_RXA_CTL);

		/* Wait for FDI receiver lane calibration */
		udelay(30);

		/* Unset FDI_RX_MISC pwrdn lanes */
		temp = I915_READ(_FDI_RXA_MISC);
		temp &= ~(FDI_RX_PWRDN_LANE1_MASK | FDI_RX_PWRDN_LANE0_MASK);
		I915_WRITE(_FDI_RXA_MISC, temp);
		POSTING_READ(_FDI_RXA_MISC);

		/* Wait for FDI auto training time */
		udelay(5);

		temp = I915_READ(DP_TP_STATUS(PORT_E));
		if (temp & DP_TP_STATUS_AUTOTRAIN_DONE) {
			DRM_DEBUG_KMS("FDI link training done on step %d\n", i);

			/* Enable normal pixel sending for FDI */
			I915_WRITE(DP_TP_CTL(PORT_E),
				   DP_TP_CTL_FDI_AUTOTRAIN |
				   DP_TP_CTL_LINK_TRAIN_NORMAL |
				   DP_TP_CTL_ENHANCED_FRAME_ENABLE |
				   DP_TP_CTL_ENABLE);

			return;
		}

		temp = I915_READ(DDI_BUF_CTL(PORT_E));
		temp &= ~DDI_BUF_CTL_ENABLE;
		I915_WRITE(DDI_BUF_CTL(PORT_E), temp);
		POSTING_READ(DDI_BUF_CTL(PORT_E));

		/* Disable DP_TP_CTL and FDI_RX_CTL and retry */
		temp = I915_READ(DP_TP_CTL(PORT_E));
		temp &= ~(DP_TP_CTL_ENABLE | DP_TP_CTL_LINK_TRAIN_MASK);
		temp |= DP_TP_CTL_LINK_TRAIN_PAT1;
		I915_WRITE(DP_TP_CTL(PORT_E), temp);
		POSTING_READ(DP_TP_CTL(PORT_E));

		intel_wait_ddi_buf_idle(dev_priv, PORT_E);

		rx_ctl_val &= ~FDI_RX_ENABLE;
		I915_WRITE(_FDI_RXA_CTL, rx_ctl_val);
		POSTING_READ(_FDI_RXA_CTL);

		/* Reset FDI_RX_MISC pwrdn lanes */
		temp = I915_READ(_FDI_RXA_MISC);
		temp &= ~(FDI_RX_PWRDN_LANE1_MASK | FDI_RX_PWRDN_LANE0_MASK);
		temp |= FDI_RX_PWRDN_LANE1_VAL(2) | FDI_RX_PWRDN_LANE0_VAL(2);
		I915_WRITE(_FDI_RXA_MISC, temp);
		POSTING_READ(_FDI_RXA_MISC);
	}

	DRM_ERROR("FDI link training failed!\n");
}

<<<<<<< HEAD
/* WRPLL clock dividers */
struct wrpll_tmds_clock {
	u32 clock;
	u16 p;		/* Post divider */
	u16 n2;		/* Feedback divider */
	u16 r2;		/* Reference divider */
};

/* Table of matching values for WRPLL clocks programming for each frequency.
 * The code assumes this table is sorted. */
static const struct wrpll_tmds_clock wrpll_tmds_clock_table[] = {
	{19750,	38,	25,	18},
	{20000,	48,	32,	18},
	{21000,	36,	21,	15},
	{21912,	42,	29,	17},
	{22000,	36,	22,	15},
	{23000,	36,	23,	15},
	{23500,	40,	40,	23},
	{23750,	26,	16,	14},
	{24000,	36,	24,	15},
	{25000,	36,	25,	15},
	{25175,	26,	40,	33},
	{25200,	30,	21,	15},
	{26000,	36,	26,	15},
	{27000,	30,	21,	14},
	{27027,	18,	100,	111},
	{27500,	30,	29,	19},
	{28000,	34,	30,	17},
	{28320,	26,	30,	22},
	{28322,	32,	42,	25},
	{28750,	24,	23,	18},
	{29000,	30,	29,	18},
	{29750,	32,	30,	17},
	{30000,	30,	25,	15},
	{30750,	30,	41,	24},
	{31000,	30,	31,	18},
	{31500,	30,	28,	16},
	{32000,	30,	32,	18},
	{32500,	28,	32,	19},
	{33000,	24,	22,	15},
	{34000,	28,	30,	17},
	{35000,	26,	32,	19},
	{35500,	24,	30,	19},
	{36000,	26,	26,	15},
	{36750,	26,	46,	26},
	{37000,	24,	23,	14},
	{37762,	22,	40,	26},
	{37800,	20,	21,	15},
	{38000,	24,	27,	16},
	{38250,	24,	34,	20},
	{39000,	24,	26,	15},
	{40000,	24,	32,	18},
	{40500,	20,	21,	14},
	{40541,	22,	147,	89},
	{40750,	18,	19,	14},
	{41000,	16,	17,	14},
	{41500,	22,	44,	26},
	{41540,	22,	44,	26},
	{42000,	18,	21,	15},
	{42500,	22,	45,	26},
	{43000,	20,	43,	27},
	{43163,	20,	24,	15},
	{44000,	18,	22,	15},
	{44900,	20,	108,	65},
	{45000,	20,	25,	15},
	{45250,	20,	52,	31},
	{46000,	18,	23,	15},
	{46750,	20,	45,	26},
	{47000,	20,	40,	23},
	{48000,	18,	24,	15},
	{49000,	18,	49,	30},
	{49500,	16,	22,	15},
	{50000,	18,	25,	15},
	{50500,	18,	32,	19},
	{51000,	18,	34,	20},
	{52000,	18,	26,	15},
	{52406,	14,	34,	25},
	{53000,	16,	22,	14},
	{54000,	16,	24,	15},
	{54054,	16,	173,	108},
	{54500,	14,	24,	17},
	{55000,	12,	22,	18},
	{56000,	14,	45,	31},
	{56250,	16,	25,	15},
	{56750,	14,	25,	17},
	{57000,	16,	27,	16},
	{58000,	16,	43,	25},
	{58250,	16,	38,	22},
	{58750,	16,	40,	23},
	{59000,	14,	26,	17},
	{59341,	14,	40,	26},
	{59400,	16,	44,	25},
	{60000,	16,	32,	18},
	{60500,	12,	39,	29},
	{61000,	14,	49,	31},
	{62000,	14,	37,	23},
	{62250,	14,	42,	26},
	{63000,	12,	21,	15},
	{63500,	14,	28,	17},
	{64000,	12,	27,	19},
	{65000,	14,	32,	19},
	{65250,	12,	29,	20},
	{65500,	12,	32,	22},
	{66000,	12,	22,	15},
	{66667,	14,	38,	22},
	{66750,	10,	21,	17},
	{67000,	14,	33,	19},
	{67750,	14,	58,	33},
	{68000,	14,	30,	17},
	{68179,	14,	46,	26},
	{68250,	14,	46,	26},
	{69000,	12,	23,	15},
	{70000,	12,	28,	18},
	{71000,	12,	30,	19},
	{72000,	12,	24,	15},
	{73000,	10,	23,	17},
	{74000,	12,	23,	14},
	{74176,	8,	100,	91},
	{74250,	10,	22,	16},
	{74481,	12,	43,	26},
	{74500,	10,	29,	21},
	{75000,	12,	25,	15},
	{75250,	10,	39,	28},
	{76000,	12,	27,	16},
	{77000,	12,	53,	31},
	{78000,	12,	26,	15},
	{78750,	12,	28,	16},
	{79000,	10,	38,	26},
	{79500,	10,	28,	19},
	{80000,	12,	32,	18},
	{81000,	10,	21,	14},
	{81081,	6,	100,	111},
	{81624,	8,	29,	24},
	{82000,	8,	17,	14},
	{83000,	10,	40,	26},
	{83950,	10,	28,	18},
	{84000,	10,	28,	18},
	{84750,	6,	16,	17},
	{85000,	6,	17,	18},
	{85250,	10,	30,	19},
	{85750,	10,	27,	17},
	{86000,	10,	43,	27},
	{87000,	10,	29,	18},
	{88000,	10,	44,	27},
	{88500,	10,	41,	25},
	{89000,	10,	28,	17},
	{89012,	6,	90,	91},
	{89100,	10,	33,	20},
	{90000,	10,	25,	15},
	{91000,	10,	32,	19},
	{92000,	10,	46,	27},
	{93000,	10,	31,	18},
	{94000,	10,	40,	23},
	{94500,	10,	28,	16},
	{95000,	10,	44,	25},
	{95654,	10,	39,	22},
	{95750,	10,	39,	22},
	{96000,	10,	32,	18},
	{97000,	8,	23,	16},
	{97750,	8,	42,	29},
	{98000,	8,	45,	31},
	{99000,	8,	22,	15},
	{99750,	8,	34,	23},
	{100000,	6,	20,	18},
	{100500,	6,	19,	17},
	{101000,	6,	37,	33},
	{101250,	8,	21,	14},
	{102000,	6,	17,	15},
	{102250,	6,	25,	22},
	{103000,	8,	29,	19},
	{104000,	8,	37,	24},
	{105000,	8,	28,	18},
	{106000,	8,	22,	14},
	{107000,	8,	46,	29},
	{107214,	8,	27,	17},
	{108000,	8,	24,	15},
	{108108,	8,	173,	108},
	{109000,	6,	23,	19},
	{110000,	6,	22,	18},
	{110013,	6,	22,	18},
	{110250,	8,	49,	30},
	{110500,	8,	36,	22},
	{111000,	8,	23,	14},
	{111264,	8,	150,	91},
	{111375,	8,	33,	20},
	{112000,	8,	63,	38},
	{112500,	8,	25,	15},
	{113100,	8,	57,	34},
	{113309,	8,	42,	25},
	{114000,	8,	27,	16},
	{115000,	6,	23,	18},
	{116000,	8,	43,	25},
	{117000,	8,	26,	15},
	{117500,	8,	40,	23},
	{118000,	6,	38,	29},
	{119000,	8,	30,	17},
	{119500,	8,	46,	26},
	{119651,	8,	39,	22},
	{120000,	8,	32,	18},
	{121000,	6,	39,	29},
	{121250,	6,	31,	23},
	{121750,	6,	23,	17},
	{122000,	6,	42,	31},
	{122614,	6,	30,	22},
	{123000,	6,	41,	30},
	{123379,	6,	37,	27},
	{124000,	6,	51,	37},
	{125000,	6,	25,	18},
	{125250,	4,	13,	14},
	{125750,	4,	27,	29},
	{126000,	6,	21,	15},
	{127000,	6,	24,	17},
	{127250,	6,	41,	29},
	{128000,	6,	27,	19},
	{129000,	6,	43,	30},
	{129859,	4,	25,	26},
	{130000,	6,	26,	18},
	{130250,	6,	42,	29},
	{131000,	6,	32,	22},
	{131500,	6,	38,	26},
	{131850,	6,	41,	28},
	{132000,	6,	22,	15},
	{132750,	6,	28,	19},
	{133000,	6,	34,	23},
	{133330,	6,	37,	25},
	{134000,	6,	61,	41},
	{135000,	6,	21,	14},
	{135250,	6,	167,	111},
	{136000,	6,	62,	41},
	{137000,	6,	35,	23},
	{138000,	6,	23,	15},
	{138500,	6,	40,	26},
	{138750,	6,	37,	24},
	{139000,	6,	34,	22},
	{139050,	6,	34,	22},
	{139054,	6,	34,	22},
	{140000,	6,	28,	18},
	{141000,	6,	36,	23},
	{141500,	6,	22,	14},
	{142000,	6,	30,	19},
	{143000,	6,	27,	17},
	{143472,	4,	17,	16},
	{144000,	6,	24,	15},
	{145000,	6,	29,	18},
	{146000,	6,	47,	29},
	{146250,	6,	26,	16},
	{147000,	6,	49,	30},
	{147891,	6,	23,	14},
	{148000,	6,	23,	14},
	{148250,	6,	28,	17},
	{148352,	4,	100,	91},
	{148500,	6,	33,	20},
	{149000,	6,	48,	29},
	{150000,	6,	25,	15},
	{151000,	4,	19,	17},
	{152000,	6,	27,	16},
	{152280,	6,	44,	26},
	{153000,	6,	34,	20},
	{154000,	6,	53,	31},
	{155000,	6,	31,	18},
	{155250,	6,	50,	29},
	{155750,	6,	45,	26},
	{156000,	6,	26,	15},
	{157000,	6,	61,	35},
	{157500,	6,	28,	16},
	{158000,	6,	65,	37},
	{158250,	6,	44,	25},
	{159000,	6,	53,	30},
	{159500,	6,	39,	22},
	{160000,	6,	32,	18},
	{161000,	4,	31,	26},
	{162000,	4,	18,	15},
	{162162,	4,	131,	109},
	{162500,	4,	53,	44},
	{163000,	4,	29,	24},
	{164000,	4,	17,	14},
	{165000,	4,	22,	18},
	{166000,	4,	32,	26},
	{167000,	4,	26,	21},
	{168000,	4,	46,	37},
	{169000,	4,	104,	83},
	{169128,	4,	64,	51},
	{169500,	4,	39,	31},
	{170000,	4,	34,	27},
	{171000,	4,	19,	15},
	{172000,	4,	51,	40},
	{172750,	4,	32,	25},
	{172800,	4,	32,	25},
	{173000,	4,	41,	32},
	{174000,	4,	49,	38},
	{174787,	4,	22,	17},
	{175000,	4,	35,	27},
	{176000,	4,	30,	23},
	{177000,	4,	38,	29},
	{178000,	4,	29,	22},
	{178500,	4,	37,	28},
	{179000,	4,	53,	40},
	{179500,	4,	73,	55},
	{180000,	4,	20,	15},
	{181000,	4,	55,	41},
	{182000,	4,	31,	23},
	{183000,	4,	42,	31},
	{184000,	4,	30,	22},
	{184750,	4,	26,	19},
	{185000,	4,	37,	27},
	{186000,	4,	51,	37},
	{187000,	4,	36,	26},
	{188000,	4,	32,	23},
	{189000,	4,	21,	15},
	{190000,	4,	38,	27},
	{190960,	4,	41,	29},
	{191000,	4,	41,	29},
	{192000,	4,	27,	19},
	{192250,	4,	37,	26},
	{193000,	4,	20,	14},
	{193250,	4,	53,	37},
	{194000,	4,	23,	16},
	{194208,	4,	23,	16},
	{195000,	4,	26,	18},
	{196000,	4,	45,	31},
	{197000,	4,	35,	24},
	{197750,	4,	41,	28},
	{198000,	4,	22,	15},
	{198500,	4,	25,	17},
	{199000,	4,	28,	19},
	{200000,	4,	37,	25},
	{201000,	4,	61,	41},
	{202000,	4,	112,	75},
	{202500,	4,	21,	14},
	{203000,	4,	146,	97},
	{204000,	4,	62,	41},
	{204750,	4,	44,	29},
	{205000,	4,	38,	25},
	{206000,	4,	29,	19},
	{207000,	4,	23,	15},
	{207500,	4,	40,	26},
	{208000,	4,	37,	24},
	{208900,	4,	48,	31},
	{209000,	4,	48,	31},
	{209250,	4,	31,	20},
	{210000,	4,	28,	18},
	{211000,	4,	25,	16},
	{212000,	4,	22,	14},
	{213000,	4,	30,	19},
	{213750,	4,	38,	24},
	{214000,	4,	46,	29},
	{214750,	4,	35,	22},
	{215000,	4,	43,	27},
	{216000,	4,	24,	15},
	{217000,	4,	37,	23},
	{218000,	4,	42,	26},
	{218250,	4,	42,	26},
	{218750,	4,	34,	21},
	{219000,	4,	47,	29},
	{220000,	4,	44,	27},
	{220640,	4,	49,	30},
	{220750,	4,	36,	22},
	{221000,	4,	36,	22},
	{222000,	4,	23,	14},
	{222525,	4,	28,	17},
	{222750,	4,	33,	20},
	{227000,	4,	37,	22},
	{230250,	4,	29,	17},
	{233500,	4,	38,	22},
	{235000,	4,	40,	23},
	{238000,	4,	30,	17},
	{241500,	2,	17,	19},
	{245250,	2,	20,	22},
	{247750,	2,	22,	24},
	{253250,	2,	15,	16},
	{256250,	2,	18,	19},
	{262500,	2,	31,	32},
	{267250,	2,	66,	67},
	{268500,	2,	94,	95},
	{270000,	2,	14,	14},
	{272500,	2,	77,	76},
	{273750,	2,	57,	56},
	{280750,	2,	24,	23},
	{281250,	2,	23,	22},
	{286000,	2,	17,	16},
	{291750,	2,	26,	24},
	{296703,	2,	56,	51},
	{297000,	2,	22,	20},
	{298000,	2,	21,	19},
};

static void intel_ddi_mode_set(struct drm_encoder *encoder,
			       struct drm_display_mode *mode,
			       struct drm_display_mode *adjusted_mode)
{
	struct drm_crtc *crtc = encoder->crtc;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct intel_encoder *intel_encoder = to_intel_encoder(encoder);
	int port = intel_ddi_get_encoder_port(intel_encoder);
	int pipe = intel_crtc->pipe;
	int type = intel_encoder->type;

	DRM_DEBUG_KMS("Preparing DDI mode for Haswell on port %c, pipe %c\n",
		      port_name(port), pipe_name(pipe));

	intel_crtc->eld_vld = false;
	if (type == INTEL_OUTPUT_DISPLAYPORT || type == INTEL_OUTPUT_EDP) {
		struct intel_dp *intel_dp = enc_to_intel_dp(encoder);
		struct intel_digital_port *intel_dig_port =
			enc_to_dig_port(encoder);

		intel_dp->DP = intel_dig_port->saved_port_bits |
			       DDI_BUF_CTL_ENABLE | DDI_BUF_EMP_400MV_0DB_HSW;
		switch (intel_dp->lane_count) {
		case 1:
			intel_dp->DP |= DDI_PORT_WIDTH_X1;
			break;
		case 2:
			intel_dp->DP |= DDI_PORT_WIDTH_X2;
			break;
		case 4:
			intel_dp->DP |= DDI_PORT_WIDTH_X4;
			break;
		default:
			intel_dp->DP |= DDI_PORT_WIDTH_X4;
			WARN(1, "Unexpected DP lane count %d\n",
			     intel_dp->lane_count);
			break;
		}

		if (intel_dp->has_audio) {
			DRM_DEBUG_DRIVER("DP audio on pipe %c on DDI\n",
					 pipe_name(intel_crtc->pipe));

			/* write eld */
			DRM_DEBUG_DRIVER("DP audio: write eld information\n");
			intel_write_eld(encoder, adjusted_mode);
		}

		intel_dp_init_link_config(intel_dp);

	} else if (type == INTEL_OUTPUT_HDMI) {
		struct intel_hdmi *intel_hdmi = enc_to_intel_hdmi(encoder);

		if (intel_hdmi->has_audio) {
			/* Proper support for digital audio needs a new logic
			 * and a new set of registers, so we leave it for future
			 * patch bombing.
			 */
			DRM_DEBUG_DRIVER("HDMI audio on pipe %c on DDI\n",
					 pipe_name(intel_crtc->pipe));

			/* write eld */
			DRM_DEBUG_DRIVER("HDMI audio: write eld information\n");
			intel_write_eld(encoder, adjusted_mode);
		}

		intel_hdmi->set_infoframes(encoder, adjusted_mode);
	}
=======
void intel_ddi_init_dp_buf_reg(struct intel_encoder *encoder)
{
	struct intel_dp *intel_dp = enc_to_intel_dp(&encoder->base);
	struct intel_digital_port *intel_dig_port =
		enc_to_dig_port(&encoder->base);

	intel_dp->DP = intel_dig_port->saved_port_bits |
		DDI_BUF_CTL_ENABLE | DDI_BUF_TRANS_SELECT(0);
	intel_dp->DP |= DDI_PORT_WIDTH(intel_dp->lane_count);

>>>>>>> v3.18
}

static struct intel_encoder *
intel_ddi_get_crtc_encoder(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct intel_encoder *intel_encoder, *ret = NULL;
	int num_encoders = 0;

	for_each_encoder_on_crtc(dev, crtc, intel_encoder) {
		ret = intel_encoder;
		num_encoders++;
	}

	if (num_encoders != 1)
<<<<<<< HEAD
		WARN(1, "%d encoders on crtc for pipe %d\n", num_encoders,
		     intel_crtc->pipe);
=======
		WARN(1, "%d encoders on crtc for pipe %c\n", num_encoders,
		     pipe_name(intel_crtc->pipe));
>>>>>>> v3.18

	BUG_ON(ret == NULL);
	return ret;
}

<<<<<<< HEAD
void intel_ddi_put_crtc_pll(struct drm_crtc *crtc)
{
	struct drm_i915_private *dev_priv = crtc->dev->dev_private;
	struct intel_ddi_plls *plls = &dev_priv->ddi_plls;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	uint32_t val;

	switch (intel_crtc->ddi_pll_sel) {
	case PORT_CLK_SEL_SPLL:
		plls->spll_refcount--;
		if (plls->spll_refcount == 0) {
			DRM_DEBUG_KMS("Disabling SPLL\n");
			val = I915_READ(SPLL_CTL);
			WARN_ON(!(val & SPLL_PLL_ENABLE));
			I915_WRITE(SPLL_CTL, val & ~SPLL_PLL_ENABLE);
			POSTING_READ(SPLL_CTL);
		}
		break;
	case PORT_CLK_SEL_WRPLL1:
		plls->wrpll1_refcount--;
		if (plls->wrpll1_refcount == 0) {
			DRM_DEBUG_KMS("Disabling WRPLL 1\n");
			val = I915_READ(WRPLL_CTL1);
			WARN_ON(!(val & WRPLL_PLL_ENABLE));
			I915_WRITE(WRPLL_CTL1, val & ~WRPLL_PLL_ENABLE);
			POSTING_READ(WRPLL_CTL1);
		}
		break;
	case PORT_CLK_SEL_WRPLL2:
		plls->wrpll2_refcount--;
		if (plls->wrpll2_refcount == 0) {
			DRM_DEBUG_KMS("Disabling WRPLL 2\n");
			val = I915_READ(WRPLL_CTL2);
			WARN_ON(!(val & WRPLL_PLL_ENABLE));
			I915_WRITE(WRPLL_CTL2, val & ~WRPLL_PLL_ENABLE);
			POSTING_READ(WRPLL_CTL2);
		}
		break;
	}

	WARN(plls->spll_refcount < 0, "Invalid SPLL refcount\n");
	WARN(plls->wrpll1_refcount < 0, "Invalid WRPLL1 refcount\n");
	WARN(plls->wrpll2_refcount < 0, "Invalid WRPLL2 refcount\n");

	intel_crtc->ddi_pll_sel = PORT_CLK_SEL_NONE;
}

static void intel_ddi_calculate_wrpll(int clock, int *p, int *n2, int *r2)
{
	u32 i;

	for (i = 0; i < ARRAY_SIZE(wrpll_tmds_clock_table); i++)
		if (clock <= wrpll_tmds_clock_table[i].clock)
			break;

	if (i == ARRAY_SIZE(wrpll_tmds_clock_table))
		i--;

	*p = wrpll_tmds_clock_table[i].p;
	*n2 = wrpll_tmds_clock_table[i].n2;
	*r2 = wrpll_tmds_clock_table[i].r2;

	if (wrpll_tmds_clock_table[i].clock != clock)
		DRM_INFO("WRPLL: using settings for %dKHz on %dKHz mode\n",
			 wrpll_tmds_clock_table[i].clock, clock);

	DRM_DEBUG_KMS("WRPLL: %dKHz refresh rate with p=%d, n2=%d r2=%d\n",
		      clock, *p, *n2, *r2);
}

bool intel_ddi_pll_mode_set(struct drm_crtc *crtc, int clock)
{
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct intel_encoder *intel_encoder = intel_ddi_get_crtc_encoder(crtc);
	struct drm_encoder *encoder = &intel_encoder->base;
	struct drm_i915_private *dev_priv = crtc->dev->dev_private;
	struct intel_ddi_plls *plls = &dev_priv->ddi_plls;
	int type = intel_encoder->type;
	enum pipe pipe = intel_crtc->pipe;
	uint32_t reg, val;

	/* TODO: reuse PLLs when possible (compare values) */

	intel_ddi_put_crtc_pll(crtc);

	if (type == INTEL_OUTPUT_DISPLAYPORT || type == INTEL_OUTPUT_EDP) {
		struct intel_dp *intel_dp = enc_to_intel_dp(encoder);

		switch (intel_dp->link_bw) {
		case DP_LINK_BW_1_62:
			intel_crtc->ddi_pll_sel = PORT_CLK_SEL_LCPLL_810;
			break;
		case DP_LINK_BW_2_7:
			intel_crtc->ddi_pll_sel = PORT_CLK_SEL_LCPLL_1350;
			break;
		case DP_LINK_BW_5_4:
			intel_crtc->ddi_pll_sel = PORT_CLK_SEL_LCPLL_2700;
			break;
		default:
			DRM_ERROR("Link bandwidth %d unsupported\n",
				  intel_dp->link_bw);
			return false;
		}

		/* We don't need to turn any PLL on because we'll use LCPLL. */
		return true;

	} else if (type == INTEL_OUTPUT_HDMI) {
		int p, n2, r2;

		if (plls->wrpll1_refcount == 0) {
			DRM_DEBUG_KMS("Using WRPLL 1 on pipe %c\n",
				      pipe_name(pipe));
			plls->wrpll1_refcount++;
			reg = WRPLL_CTL1;
			intel_crtc->ddi_pll_sel = PORT_CLK_SEL_WRPLL1;
		} else if (plls->wrpll2_refcount == 0) {
			DRM_DEBUG_KMS("Using WRPLL 2 on pipe %c\n",
				      pipe_name(pipe));
			plls->wrpll2_refcount++;
			reg = WRPLL_CTL2;
			intel_crtc->ddi_pll_sel = PORT_CLK_SEL_WRPLL2;
		} else {
			DRM_ERROR("No WRPLLs available!\n");
			return false;
		}

		WARN(I915_READ(reg) & WRPLL_PLL_ENABLE,
		     "WRPLL already enabled\n");

		intel_ddi_calculate_wrpll(clock, &p, &n2, &r2);

		val = WRPLL_PLL_ENABLE | WRPLL_PLL_SELECT_LCPLL_2700 |
		      WRPLL_DIVIDER_REFERENCE(r2) | WRPLL_DIVIDER_FEEDBACK(n2) |
		      WRPLL_DIVIDER_POST(p);

	} else if (type == INTEL_OUTPUT_ANALOG) {
		if (plls->spll_refcount == 0) {
			DRM_DEBUG_KMS("Using SPLL on pipe %c\n",
				      pipe_name(pipe));
			plls->spll_refcount++;
			reg = SPLL_CTL;
			intel_crtc->ddi_pll_sel = PORT_CLK_SEL_SPLL;
		} else {
			DRM_ERROR("SPLL already in use\n");
			return false;
		}

		WARN(I915_READ(reg) & SPLL_PLL_ENABLE,
		     "SPLL already enabled\n");

		val = SPLL_PLL_ENABLE | SPLL_PLL_FREQ_1350MHz | SPLL_PLL_SSC;

	} else {
		WARN(1, "Invalid DDI encoder type %d\n", type);
		return false;
	}

	I915_WRITE(reg, val);
	udelay(20);

	return true;
=======
#define LC_FREQ 2700
#define LC_FREQ_2K U64_C(LC_FREQ * 2000)

#define P_MIN 2
#define P_MAX 64
#define P_INC 2

/* Constraints for PLL good behavior */
#define REF_MIN 48
#define REF_MAX 400
#define VCO_MIN 2400
#define VCO_MAX 4800

#define abs_diff(a, b) ({			\
	typeof(a) __a = (a);			\
	typeof(b) __b = (b);			\
	(void) (&__a == &__b);			\
	__a > __b ? (__a - __b) : (__b - __a); })

struct wrpll_rnp {
	unsigned p, n2, r2;
};

static unsigned wrpll_get_budget_for_freq(int clock)
{
	unsigned budget;

	switch (clock) {
	case 25175000:
	case 25200000:
	case 27000000:
	case 27027000:
	case 37762500:
	case 37800000:
	case 40500000:
	case 40541000:
	case 54000000:
	case 54054000:
	case 59341000:
	case 59400000:
	case 72000000:
	case 74176000:
	case 74250000:
	case 81000000:
	case 81081000:
	case 89012000:
	case 89100000:
	case 108000000:
	case 108108000:
	case 111264000:
	case 111375000:
	case 148352000:
	case 148500000:
	case 162000000:
	case 162162000:
	case 222525000:
	case 222750000:
	case 296703000:
	case 297000000:
		budget = 0;
		break;
	case 233500000:
	case 245250000:
	case 247750000:
	case 253250000:
	case 298000000:
		budget = 1500;
		break;
	case 169128000:
	case 169500000:
	case 179500000:
	case 202000000:
		budget = 2000;
		break;
	case 256250000:
	case 262500000:
	case 270000000:
	case 272500000:
	case 273750000:
	case 280750000:
	case 281250000:
	case 286000000:
	case 291750000:
		budget = 4000;
		break;
	case 267250000:
	case 268500000:
		budget = 5000;
		break;
	default:
		budget = 1000;
		break;
	}

	return budget;
}

static void wrpll_update_rnp(uint64_t freq2k, unsigned budget,
			     unsigned r2, unsigned n2, unsigned p,
			     struct wrpll_rnp *best)
{
	uint64_t a, b, c, d, diff, diff_best;

	/* No best (r,n,p) yet */
	if (best->p == 0) {
		best->p = p;
		best->n2 = n2;
		best->r2 = r2;
		return;
	}

	/*
	 * Output clock is (LC_FREQ_2K / 2000) * N / (P * R), which compares to
	 * freq2k.
	 *
	 * delta = 1e6 *
	 *	   abs(freq2k - (LC_FREQ_2K * n2/(p * r2))) /
	 *	   freq2k;
	 *
	 * and we would like delta <= budget.
	 *
	 * If the discrepancy is above the PPM-based budget, always prefer to
	 * improve upon the previous solution.  However, if you're within the
	 * budget, try to maximize Ref * VCO, that is N / (P * R^2).
	 */
	a = freq2k * budget * p * r2;
	b = freq2k * budget * best->p * best->r2;
	diff = abs_diff(freq2k * p * r2, LC_FREQ_2K * n2);
	diff_best = abs_diff(freq2k * best->p * best->r2,
			     LC_FREQ_2K * best->n2);
	c = 1000000 * diff;
	d = 1000000 * diff_best;

	if (a < c && b < d) {
		/* If both are above the budget, pick the closer */
		if (best->p * best->r2 * diff < p * r2 * diff_best) {
			best->p = p;
			best->n2 = n2;
			best->r2 = r2;
		}
	} else if (a >= c && b < d) {
		/* If A is below the threshold but B is above it?  Update. */
		best->p = p;
		best->n2 = n2;
		best->r2 = r2;
	} else if (a >= c && b >= d) {
		/* Both are below the limit, so pick the higher n2/(r2*r2) */
		if (n2 * best->r2 * best->r2 > best->n2 * r2 * r2) {
			best->p = p;
			best->n2 = n2;
			best->r2 = r2;
		}
	}
	/* Otherwise a < c && b >= d, do nothing */
}

static int intel_ddi_calc_wrpll_link(struct drm_i915_private *dev_priv,
				     int reg)
{
	int refclk = LC_FREQ;
	int n, p, r;
	u32 wrpll;

	wrpll = I915_READ(reg);
	switch (wrpll & WRPLL_PLL_REF_MASK) {
	case WRPLL_PLL_SSC:
	case WRPLL_PLL_NON_SSC:
		/*
		 * We could calculate spread here, but our checking
		 * code only cares about 5% accuracy, and spread is a max of
		 * 0.5% downspread.
		 */
		refclk = 135;
		break;
	case WRPLL_PLL_LCPLL:
		refclk = LC_FREQ;
		break;
	default:
		WARN(1, "bad wrpll refclk\n");
		return 0;
	}

	r = wrpll & WRPLL_DIVIDER_REF_MASK;
	p = (wrpll & WRPLL_DIVIDER_POST_MASK) >> WRPLL_DIVIDER_POST_SHIFT;
	n = (wrpll & WRPLL_DIVIDER_FB_MASK) >> WRPLL_DIVIDER_FB_SHIFT;

	/* Convert to KHz, p & r have a fixed point portion */
	return (refclk * n * 100) / (p * r);
}

static void hsw_ddi_clock_get(struct intel_encoder *encoder,
			      struct intel_crtc_config *pipe_config)
{
	struct drm_i915_private *dev_priv = encoder->base.dev->dev_private;
	int link_clock = 0;
	u32 val, pll;

	val = pipe_config->ddi_pll_sel;
	switch (val & PORT_CLK_SEL_MASK) {
	case PORT_CLK_SEL_LCPLL_810:
		link_clock = 81000;
		break;
	case PORT_CLK_SEL_LCPLL_1350:
		link_clock = 135000;
		break;
	case PORT_CLK_SEL_LCPLL_2700:
		link_clock = 270000;
		break;
	case PORT_CLK_SEL_WRPLL1:
		link_clock = intel_ddi_calc_wrpll_link(dev_priv, WRPLL_CTL1);
		break;
	case PORT_CLK_SEL_WRPLL2:
		link_clock = intel_ddi_calc_wrpll_link(dev_priv, WRPLL_CTL2);
		break;
	case PORT_CLK_SEL_SPLL:
		pll = I915_READ(SPLL_CTL) & SPLL_PLL_FREQ_MASK;
		if (pll == SPLL_PLL_FREQ_810MHz)
			link_clock = 81000;
		else if (pll == SPLL_PLL_FREQ_1350MHz)
			link_clock = 135000;
		else if (pll == SPLL_PLL_FREQ_2700MHz)
			link_clock = 270000;
		else {
			WARN(1, "bad spll freq\n");
			return;
		}
		break;
	default:
		WARN(1, "bad port clock sel\n");
		return;
	}

	pipe_config->port_clock = link_clock * 2;

	if (pipe_config->has_pch_encoder)
		pipe_config->adjusted_mode.crtc_clock =
			intel_dotclock_calculate(pipe_config->port_clock,
						 &pipe_config->fdi_m_n);
	else if (pipe_config->has_dp_encoder)
		pipe_config->adjusted_mode.crtc_clock =
			intel_dotclock_calculate(pipe_config->port_clock,
						 &pipe_config->dp_m_n);
	else
		pipe_config->adjusted_mode.crtc_clock = pipe_config->port_clock;
}

void intel_ddi_clock_get(struct intel_encoder *encoder,
			 struct intel_crtc_config *pipe_config)
{
	hsw_ddi_clock_get(encoder, pipe_config);
}

static void
hsw_ddi_calculate_wrpll(int clock /* in Hz */,
			unsigned *r2_out, unsigned *n2_out, unsigned *p_out)
{
	uint64_t freq2k;
	unsigned p, n2, r2;
	struct wrpll_rnp best = { 0, 0, 0 };
	unsigned budget;

	freq2k = clock / 100;

	budget = wrpll_get_budget_for_freq(clock);

	/* Special case handling for 540 pixel clock: bypass WR PLL entirely
	 * and directly pass the LC PLL to it. */
	if (freq2k == 5400000) {
		*n2_out = 2;
		*p_out = 1;
		*r2_out = 2;
		return;
	}

	/*
	 * Ref = LC_FREQ / R, where Ref is the actual reference input seen by
	 * the WR PLL.
	 *
	 * We want R so that REF_MIN <= Ref <= REF_MAX.
	 * Injecting R2 = 2 * R gives:
	 *   REF_MAX * r2 > LC_FREQ * 2 and
	 *   REF_MIN * r2 < LC_FREQ * 2
	 *
	 * Which means the desired boundaries for r2 are:
	 *  LC_FREQ * 2 / REF_MAX < r2 < LC_FREQ * 2 / REF_MIN
	 *
	 */
	for (r2 = LC_FREQ * 2 / REF_MAX + 1;
	     r2 <= LC_FREQ * 2 / REF_MIN;
	     r2++) {

		/*
		 * VCO = N * Ref, that is: VCO = N * LC_FREQ / R
		 *
		 * Once again we want VCO_MIN <= VCO <= VCO_MAX.
		 * Injecting R2 = 2 * R and N2 = 2 * N, we get:
		 *   VCO_MAX * r2 > n2 * LC_FREQ and
		 *   VCO_MIN * r2 < n2 * LC_FREQ)
		 *
		 * Which means the desired boundaries for n2 are:
		 * VCO_MIN * r2 / LC_FREQ < n2 < VCO_MAX * r2 / LC_FREQ
		 */
		for (n2 = VCO_MIN * r2 / LC_FREQ + 1;
		     n2 <= VCO_MAX * r2 / LC_FREQ;
		     n2++) {

			for (p = P_MIN; p <= P_MAX; p += P_INC)
				wrpll_update_rnp(freq2k, budget,
						 r2, n2, p, &best);
		}
	}

	*n2_out = best.n2;
	*p_out = best.p;
	*r2_out = best.r2;
}

static bool
hsw_ddi_pll_select(struct intel_crtc *intel_crtc,
		   struct intel_encoder *intel_encoder,
		   int clock)
{
	if (intel_encoder->type == INTEL_OUTPUT_HDMI) {
		struct intel_shared_dpll *pll;
		uint32_t val;
		unsigned p, n2, r2;

		hsw_ddi_calculate_wrpll(clock * 1000, &r2, &n2, &p);

		val = WRPLL_PLL_ENABLE | WRPLL_PLL_LCPLL |
		      WRPLL_DIVIDER_REFERENCE(r2) | WRPLL_DIVIDER_FEEDBACK(n2) |
		      WRPLL_DIVIDER_POST(p);

		intel_crtc->config.dpll_hw_state.wrpll = val;

		pll = intel_get_shared_dpll(intel_crtc);
		if (pll == NULL) {
			DRM_DEBUG_DRIVER("failed to find PLL for pipe %c\n",
					 pipe_name(intel_crtc->pipe));
			return false;
		}

		intel_crtc->config.ddi_pll_sel = PORT_CLK_SEL_WRPLL(pll->id);
	}

	return true;
}


/*
 * Tries to find a *shared* PLL for the CRTC and store it in
 * intel_crtc->ddi_pll_sel.
 *
 * For private DPLLs, compute_config() should do the selection for us. This
 * function should be folded into compute_config() eventually.
 */
bool intel_ddi_pll_select(struct intel_crtc *intel_crtc)
{
	struct drm_crtc *crtc = &intel_crtc->base;
	struct intel_encoder *intel_encoder = intel_ddi_get_crtc_encoder(crtc);
	int clock = intel_crtc->config.port_clock;

	intel_put_shared_dpll(intel_crtc);

	return hsw_ddi_pll_select(intel_crtc, intel_encoder, clock);
>>>>>>> v3.18
}

void intel_ddi_set_pipe_settings(struct drm_crtc *crtc)
{
	struct drm_i915_private *dev_priv = crtc->dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct intel_encoder *intel_encoder = intel_ddi_get_crtc_encoder(crtc);
	enum transcoder cpu_transcoder = intel_crtc->config.cpu_transcoder;
	int type = intel_encoder->type;
	uint32_t temp;

<<<<<<< HEAD
	if (type == INTEL_OUTPUT_DISPLAYPORT || type == INTEL_OUTPUT_EDP) {

=======
	if (type == INTEL_OUTPUT_DISPLAYPORT || type == INTEL_OUTPUT_EDP || type == INTEL_OUTPUT_DP_MST) {
>>>>>>> v3.18
		temp = TRANS_MSA_SYNC_CLK;
		switch (intel_crtc->config.pipe_bpp) {
		case 18:
			temp |= TRANS_MSA_6_BPC;
			break;
		case 24:
			temp |= TRANS_MSA_8_BPC;
			break;
		case 30:
			temp |= TRANS_MSA_10_BPC;
			break;
		case 36:
			temp |= TRANS_MSA_12_BPC;
			break;
		default:
			BUG();
		}
		I915_WRITE(TRANS_MSA_MISC(cpu_transcoder), temp);
	}
}

<<<<<<< HEAD
=======
void intel_ddi_set_vc_payload_alloc(struct drm_crtc *crtc, bool state)
{
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	enum transcoder cpu_transcoder = intel_crtc->config.cpu_transcoder;
	uint32_t temp;
	temp = I915_READ(TRANS_DDI_FUNC_CTL(cpu_transcoder));
	if (state == true)
		temp |= TRANS_DDI_DP_VC_PAYLOAD_ALLOC;
	else
		temp &= ~TRANS_DDI_DP_VC_PAYLOAD_ALLOC;
	I915_WRITE(TRANS_DDI_FUNC_CTL(cpu_transcoder), temp);
}

>>>>>>> v3.18
void intel_ddi_enable_transcoder_func(struct drm_crtc *crtc)
{
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	struct intel_encoder *intel_encoder = intel_ddi_get_crtc_encoder(crtc);
	struct drm_encoder *encoder = &intel_encoder->base;
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = crtc->dev->dev_private;
=======
	struct drm_device *dev = crtc->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
>>>>>>> v3.18
	enum pipe pipe = intel_crtc->pipe;
	enum transcoder cpu_transcoder = intel_crtc->config.cpu_transcoder;
	enum port port = intel_ddi_get_encoder_port(intel_encoder);
	int type = intel_encoder->type;
	uint32_t temp;

	/* Enable TRANS_DDI_FUNC_CTL for the pipe to work in HDMI mode */
	temp = TRANS_DDI_FUNC_ENABLE;
	temp |= TRANS_DDI_SELECT_PORT(port);

	switch (intel_crtc->config.pipe_bpp) {
	case 18:
		temp |= TRANS_DDI_BPC_6;
		break;
	case 24:
		temp |= TRANS_DDI_BPC_8;
		break;
	case 30:
		temp |= TRANS_DDI_BPC_10;
		break;
	case 36:
		temp |= TRANS_DDI_BPC_12;
		break;
	default:
		BUG();
	}

<<<<<<< HEAD
	if (crtc->mode.flags & DRM_MODE_FLAG_PVSYNC)
		temp |= TRANS_DDI_PVSYNC;
	if (crtc->mode.flags & DRM_MODE_FLAG_PHSYNC)
=======
	if (intel_crtc->config.adjusted_mode.flags & DRM_MODE_FLAG_PVSYNC)
		temp |= TRANS_DDI_PVSYNC;
	if (intel_crtc->config.adjusted_mode.flags & DRM_MODE_FLAG_PHSYNC)
>>>>>>> v3.18
		temp |= TRANS_DDI_PHSYNC;

	if (cpu_transcoder == TRANSCODER_EDP) {
		switch (pipe) {
		case PIPE_A:
<<<<<<< HEAD
			/* Can only use the always-on power well for eDP when
			 * not using the panel fitter, and when not using motion
			  * blur mitigation (which we don't support). */
			if (dev_priv->pch_pf_size)
=======
			/* On Haswell, can only use the always-on power well for
			 * eDP when not using the panel fitter, and when not
			 * using motion blur mitigation (which we don't
			 * support). */
			if (IS_HASWELL(dev) &&
			    (intel_crtc->config.pch_pfit.enabled ||
			     intel_crtc->config.pch_pfit.force_thru))
>>>>>>> v3.18
				temp |= TRANS_DDI_EDP_INPUT_A_ONOFF;
			else
				temp |= TRANS_DDI_EDP_INPUT_A_ON;
			break;
		case PIPE_B:
			temp |= TRANS_DDI_EDP_INPUT_B_ONOFF;
			break;
		case PIPE_C:
			temp |= TRANS_DDI_EDP_INPUT_C_ONOFF;
			break;
		default:
			BUG();
			break;
		}
	}

	if (type == INTEL_OUTPUT_HDMI) {
<<<<<<< HEAD
		struct intel_hdmi *intel_hdmi = enc_to_intel_hdmi(encoder);

		if (intel_hdmi->has_hdmi_sink)
=======
		if (intel_crtc->config.has_hdmi_sink)
>>>>>>> v3.18
			temp |= TRANS_DDI_MODE_SELECT_HDMI;
		else
			temp |= TRANS_DDI_MODE_SELECT_DVI;

	} else if (type == INTEL_OUTPUT_ANALOG) {
		temp |= TRANS_DDI_MODE_SELECT_FDI;
<<<<<<< HEAD
		temp |= (intel_crtc->fdi_lanes - 1) << 1;
=======
		temp |= (intel_crtc->config.fdi_lanes - 1) << 1;
>>>>>>> v3.18

	} else if (type == INTEL_OUTPUT_DISPLAYPORT ||
		   type == INTEL_OUTPUT_EDP) {
		struct intel_dp *intel_dp = enc_to_intel_dp(encoder);

<<<<<<< HEAD
		temp |= TRANS_DDI_MODE_SELECT_DP_SST;

		switch (intel_dp->lane_count) {
		case 1:
			temp |= TRANS_DDI_PORT_WIDTH_X1;
			break;
		case 2:
			temp |= TRANS_DDI_PORT_WIDTH_X2;
			break;
		case 4:
			temp |= TRANS_DDI_PORT_WIDTH_X4;
			break;
		default:
			temp |= TRANS_DDI_PORT_WIDTH_X4;
			WARN(1, "Unsupported lane count %d\n",
			     intel_dp->lane_count);
		}

	} else {
		WARN(1, "Invalid encoder type %d for pipe %d\n",
		     intel_encoder->type, pipe);
=======
		if (intel_dp->is_mst) {
			temp |= TRANS_DDI_MODE_SELECT_DP_MST;
		} else
			temp |= TRANS_DDI_MODE_SELECT_DP_SST;

		temp |= DDI_PORT_WIDTH(intel_dp->lane_count);
	} else if (type == INTEL_OUTPUT_DP_MST) {
		struct intel_dp *intel_dp = &enc_to_mst(encoder)->primary->dp;

		if (intel_dp->is_mst) {
			temp |= TRANS_DDI_MODE_SELECT_DP_MST;
		} else
			temp |= TRANS_DDI_MODE_SELECT_DP_SST;

		temp |= DDI_PORT_WIDTH(intel_dp->lane_count);
	} else {
		WARN(1, "Invalid encoder type %d for pipe %c\n",
		     intel_encoder->type, pipe_name(pipe));
>>>>>>> v3.18
	}

	I915_WRITE(TRANS_DDI_FUNC_CTL(cpu_transcoder), temp);
}

void intel_ddi_disable_transcoder_func(struct drm_i915_private *dev_priv,
				       enum transcoder cpu_transcoder)
{
	uint32_t reg = TRANS_DDI_FUNC_CTL(cpu_transcoder);
	uint32_t val = I915_READ(reg);

<<<<<<< HEAD
	val &= ~(TRANS_DDI_FUNC_ENABLE | TRANS_DDI_PORT_MASK);
=======
	val &= ~(TRANS_DDI_FUNC_ENABLE | TRANS_DDI_PORT_MASK | TRANS_DDI_DP_VC_PAYLOAD_ALLOC);
>>>>>>> v3.18
	val |= TRANS_DDI_PORT_NONE;
	I915_WRITE(reg, val);
}

bool intel_ddi_connector_get_hw_state(struct intel_connector *intel_connector)
{
	struct drm_device *dev = intel_connector->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_encoder *intel_encoder = intel_connector->encoder;
	int type = intel_connector->base.connector_type;
	enum port port = intel_ddi_get_encoder_port(intel_encoder);
	enum pipe pipe = 0;
	enum transcoder cpu_transcoder;
<<<<<<< HEAD
	uint32_t tmp;

=======
	enum intel_display_power_domain power_domain;
	uint32_t tmp;

	power_domain = intel_display_port_power_domain(intel_encoder);
	if (!intel_display_power_enabled(dev_priv, power_domain))
		return false;

>>>>>>> v3.18
	if (!intel_encoder->get_hw_state(intel_encoder, &pipe))
		return false;

	if (port == PORT_A)
		cpu_transcoder = TRANSCODER_EDP;
	else
		cpu_transcoder = (enum transcoder) pipe;

	tmp = I915_READ(TRANS_DDI_FUNC_CTL(cpu_transcoder));

	switch (tmp & TRANS_DDI_MODE_SELECT_MASK) {
	case TRANS_DDI_MODE_SELECT_HDMI:
	case TRANS_DDI_MODE_SELECT_DVI:
		return (type == DRM_MODE_CONNECTOR_HDMIA);

	case TRANS_DDI_MODE_SELECT_DP_SST:
		if (type == DRM_MODE_CONNECTOR_eDP)
			return true;
<<<<<<< HEAD
	case TRANS_DDI_MODE_SELECT_DP_MST:
		return (type == DRM_MODE_CONNECTOR_DisplayPort);
=======
		return (type == DRM_MODE_CONNECTOR_DisplayPort);
	case TRANS_DDI_MODE_SELECT_DP_MST:
		/* if the transcoder is in MST state then
		 * connector isn't connected */
		return false;
>>>>>>> v3.18

	case TRANS_DDI_MODE_SELECT_FDI:
		return (type == DRM_MODE_CONNECTOR_VGA);

	default:
		return false;
	}
}

bool intel_ddi_get_hw_state(struct intel_encoder *encoder,
			    enum pipe *pipe)
{
	struct drm_device *dev = encoder->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	enum port port = intel_ddi_get_encoder_port(encoder);
<<<<<<< HEAD
	u32 tmp;
	int i;

=======
	enum intel_display_power_domain power_domain;
	u32 tmp;
	int i;

	power_domain = intel_display_port_power_domain(encoder);
	if (!intel_display_power_enabled(dev_priv, power_domain))
		return false;

>>>>>>> v3.18
	tmp = I915_READ(DDI_BUF_CTL(port));

	if (!(tmp & DDI_BUF_CTL_ENABLE))
		return false;

	if (port == PORT_A) {
		tmp = I915_READ(TRANS_DDI_FUNC_CTL(TRANSCODER_EDP));

		switch (tmp & TRANS_DDI_EDP_INPUT_MASK) {
		case TRANS_DDI_EDP_INPUT_A_ON:
		case TRANS_DDI_EDP_INPUT_A_ONOFF:
			*pipe = PIPE_A;
			break;
		case TRANS_DDI_EDP_INPUT_B_ONOFF:
			*pipe = PIPE_B;
			break;
		case TRANS_DDI_EDP_INPUT_C_ONOFF:
			*pipe = PIPE_C;
			break;
		}

		return true;
	} else {
		for (i = TRANSCODER_A; i <= TRANSCODER_C; i++) {
			tmp = I915_READ(TRANS_DDI_FUNC_CTL(i));

			if ((tmp & TRANS_DDI_PORT_MASK)
			    == TRANS_DDI_SELECT_PORT(port)) {
<<<<<<< HEAD
=======
				if ((tmp & TRANS_DDI_MODE_SELECT_MASK) == TRANS_DDI_MODE_SELECT_DP_MST)
					return false;

>>>>>>> v3.18
				*pipe = i;
				return true;
			}
		}
	}

<<<<<<< HEAD
	DRM_DEBUG_KMS("No pipe for ddi port %i found\n", port);
=======
	DRM_DEBUG_KMS("No pipe for ddi port %c found\n", port_name(port));
>>>>>>> v3.18

	return false;
}

<<<<<<< HEAD
static uint32_t intel_ddi_get_crtc_pll(struct drm_i915_private *dev_priv,
				       enum pipe pipe)
{
	uint32_t temp, ret;
	enum port port = I915_MAX_PORTS;
	enum transcoder cpu_transcoder = intel_pipe_to_cpu_transcoder(dev_priv,
								      pipe);
	int i;

	if (cpu_transcoder == TRANSCODER_EDP) {
		port = PORT_A;
	} else {
		temp = I915_READ(TRANS_DDI_FUNC_CTL(cpu_transcoder));
		temp &= TRANS_DDI_PORT_MASK;

		for (i = PORT_B; i <= PORT_E; i++)
			if (temp == TRANS_DDI_SELECT_PORT(i))
				port = i;
	}

	if (port == I915_MAX_PORTS) {
		WARN(1, "Pipe %c enabled on an unknown port\n",
		     pipe_name(pipe));
		ret = PORT_CLK_SEL_NONE;
	} else {
		ret = I915_READ(PORT_CLK_SEL(port));
		DRM_DEBUG_KMS("Pipe %c connected to port %c using clock "
			      "0x%08x\n", pipe_name(pipe), port_name(port),
			      ret);
	}

	return ret;
}

void intel_ddi_setup_hw_pll_state(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	enum pipe pipe;
	struct intel_crtc *intel_crtc;

	dev_priv->ddi_plls.spll_refcount = 0;
	dev_priv->ddi_plls.wrpll1_refcount = 0;
	dev_priv->ddi_plls.wrpll2_refcount = 0;

	for_each_pipe(pipe) {
		intel_crtc =
			to_intel_crtc(dev_priv->pipe_to_crtc_mapping[pipe]);

		if (!intel_crtc->active) {
			intel_crtc->ddi_pll_sel = PORT_CLK_SEL_NONE;
			continue;
		}

		intel_crtc->ddi_pll_sel = intel_ddi_get_crtc_pll(dev_priv,
								 pipe);

		switch (intel_crtc->ddi_pll_sel) {
		case PORT_CLK_SEL_SPLL:
			dev_priv->ddi_plls.spll_refcount++;
			break;
		case PORT_CLK_SEL_WRPLL1:
			dev_priv->ddi_plls.wrpll1_refcount++;
			break;
		case PORT_CLK_SEL_WRPLL2:
			dev_priv->ddi_plls.wrpll2_refcount++;
			break;
		}
	}
}

=======
>>>>>>> v3.18
void intel_ddi_enable_pipe_clock(struct intel_crtc *intel_crtc)
{
	struct drm_crtc *crtc = &intel_crtc->base;
	struct drm_i915_private *dev_priv = crtc->dev->dev_private;
	struct intel_encoder *intel_encoder = intel_ddi_get_crtc_encoder(crtc);
	enum port port = intel_ddi_get_encoder_port(intel_encoder);
	enum transcoder cpu_transcoder = intel_crtc->config.cpu_transcoder;

	if (cpu_transcoder != TRANSCODER_EDP)
		I915_WRITE(TRANS_CLK_SEL(cpu_transcoder),
			   TRANS_CLK_SEL_PORT(port));
}

void intel_ddi_disable_pipe_clock(struct intel_crtc *intel_crtc)
{
	struct drm_i915_private *dev_priv = intel_crtc->base.dev->dev_private;
	enum transcoder cpu_transcoder = intel_crtc->config.cpu_transcoder;

	if (cpu_transcoder != TRANSCODER_EDP)
		I915_WRITE(TRANS_CLK_SEL(cpu_transcoder),
			   TRANS_CLK_SEL_DISABLED);
}

static void intel_ddi_pre_enable(struct intel_encoder *intel_encoder)
{
	struct drm_encoder *encoder = &intel_encoder->base;
<<<<<<< HEAD
	struct drm_crtc *crtc = encoder->crtc;
	struct drm_i915_private *dev_priv = encoder->dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	enum port port = intel_ddi_get_encoder_port(intel_encoder);
	int type = intel_encoder->type;

	if (type == INTEL_OUTPUT_EDP) {
		struct intel_dp *intel_dp = enc_to_intel_dp(encoder);
		ironlake_edp_panel_vdd_on(intel_dp);
		ironlake_edp_panel_on(intel_dp);
		ironlake_edp_panel_vdd_off(intel_dp, true);
	}

	WARN_ON(intel_crtc->ddi_pll_sel == PORT_CLK_SEL_NONE);
	I915_WRITE(PORT_CLK_SEL(port), intel_crtc->ddi_pll_sel);
=======
	struct drm_i915_private *dev_priv = encoder->dev->dev_private;
	struct intel_crtc *crtc = to_intel_crtc(encoder->crtc);
	enum port port = intel_ddi_get_encoder_port(intel_encoder);
	int type = intel_encoder->type;

	if (crtc->config.has_audio) {
		DRM_DEBUG_DRIVER("Audio on pipe %c on DDI\n",
				 pipe_name(crtc->pipe));

		/* write eld */
		DRM_DEBUG_DRIVER("DDI audio: write eld information\n");
		intel_write_eld(encoder, &crtc->config.adjusted_mode);
	}

	if (type == INTEL_OUTPUT_EDP) {
		struct intel_dp *intel_dp = enc_to_intel_dp(encoder);
		intel_edp_panel_on(intel_dp);
	}

	WARN_ON(crtc->config.ddi_pll_sel == PORT_CLK_SEL_NONE);
	I915_WRITE(PORT_CLK_SEL(port), crtc->config.ddi_pll_sel);
>>>>>>> v3.18

	if (type == INTEL_OUTPUT_DISPLAYPORT || type == INTEL_OUTPUT_EDP) {
		struct intel_dp *intel_dp = enc_to_intel_dp(encoder);

<<<<<<< HEAD
=======
		intel_ddi_init_dp_buf_reg(intel_encoder);

>>>>>>> v3.18
		intel_dp_sink_dpms(intel_dp, DRM_MODE_DPMS_ON);
		intel_dp_start_link_train(intel_dp);
		intel_dp_complete_link_train(intel_dp);
		if (port != PORT_A)
			intel_dp_stop_link_train(intel_dp);
<<<<<<< HEAD
=======
	} else if (type == INTEL_OUTPUT_HDMI) {
		struct intel_hdmi *intel_hdmi = enc_to_intel_hdmi(encoder);

		intel_hdmi->set_infoframes(encoder,
					   crtc->config.has_hdmi_sink,
					   &crtc->config.adjusted_mode);
>>>>>>> v3.18
	}
}

static void intel_ddi_post_disable(struct intel_encoder *intel_encoder)
{
	struct drm_encoder *encoder = &intel_encoder->base;
	struct drm_i915_private *dev_priv = encoder->dev->dev_private;
	enum port port = intel_ddi_get_encoder_port(intel_encoder);
	int type = intel_encoder->type;
	uint32_t val;
	bool wait = false;

	val = I915_READ(DDI_BUF_CTL(port));
	if (val & DDI_BUF_CTL_ENABLE) {
		val &= ~DDI_BUF_CTL_ENABLE;
		I915_WRITE(DDI_BUF_CTL(port), val);
		wait = true;
	}

	val = I915_READ(DP_TP_CTL(port));
	val &= ~(DP_TP_CTL_ENABLE | DP_TP_CTL_LINK_TRAIN_MASK);
	val |= DP_TP_CTL_LINK_TRAIN_PAT1;
	I915_WRITE(DP_TP_CTL(port), val);

	if (wait)
		intel_wait_ddi_buf_idle(dev_priv, port);

<<<<<<< HEAD
	if (type == INTEL_OUTPUT_EDP) {
		struct intel_dp *intel_dp = enc_to_intel_dp(encoder);
		ironlake_edp_panel_vdd_on(intel_dp);
		ironlake_edp_panel_off(intel_dp);
=======
	if (type == INTEL_OUTPUT_DISPLAYPORT || type == INTEL_OUTPUT_EDP) {
		struct intel_dp *intel_dp = enc_to_intel_dp(encoder);
		intel_dp_sink_dpms(intel_dp, DRM_MODE_DPMS_OFF);
		intel_edp_panel_vdd_on(intel_dp);
		intel_edp_panel_off(intel_dp);
>>>>>>> v3.18
	}

	I915_WRITE(PORT_CLK_SEL(port), PORT_CLK_SEL_NONE);
}

static void intel_enable_ddi(struct intel_encoder *intel_encoder)
{
	struct drm_encoder *encoder = &intel_encoder->base;
	struct drm_crtc *crtc = encoder->crtc;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	struct drm_device *dev = encoder->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	enum port port = intel_ddi_get_encoder_port(intel_encoder);
	int type = intel_encoder->type;
	uint32_t tmp;

	if (type == INTEL_OUTPUT_HDMI) {
		struct intel_digital_port *intel_dig_port =
			enc_to_dig_port(encoder);

		/* In HDMI/DVI mode, the port width, and swing/emphasis values
		 * are ignored so nothing special needs to be done besides
		 * enabling the port.
		 */
		I915_WRITE(DDI_BUF_CTL(port),
			   intel_dig_port->saved_port_bits |
			   DDI_BUF_CTL_ENABLE);
	} else if (type == INTEL_OUTPUT_EDP) {
		struct intel_dp *intel_dp = enc_to_intel_dp(encoder);

		if (port == PORT_A)
			intel_dp_stop_link_train(intel_dp);

<<<<<<< HEAD
		ironlake_edp_backlight_on(intel_dp);
	}

	if (intel_crtc->eld_vld) {
=======
		intel_edp_backlight_on(intel_dp);
		intel_edp_psr_enable(intel_dp);
	}

	if (intel_crtc->config.has_audio) {
		intel_display_power_get(dev_priv, POWER_DOMAIN_AUDIO);
>>>>>>> v3.18
		tmp = I915_READ(HSW_AUD_PIN_ELD_CP_VLD);
		tmp |= ((AUDIO_OUTPUT_ENABLE_A | AUDIO_ELD_VALID_A) << (pipe * 4));
		I915_WRITE(HSW_AUD_PIN_ELD_CP_VLD, tmp);
	}
}

static void intel_disable_ddi(struct intel_encoder *intel_encoder)
{
	struct drm_encoder *encoder = &intel_encoder->base;
	struct drm_crtc *crtc = encoder->crtc;
	struct intel_crtc *intel_crtc = to_intel_crtc(crtc);
	int pipe = intel_crtc->pipe;
	int type = intel_encoder->type;
	struct drm_device *dev = encoder->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	uint32_t tmp;

<<<<<<< HEAD
	tmp = I915_READ(HSW_AUD_PIN_ELD_CP_VLD);
	tmp &= ~((AUDIO_OUTPUT_ENABLE_A | AUDIO_ELD_VALID_A) << (pipe * 4));
	I915_WRITE(HSW_AUD_PIN_ELD_CP_VLD, tmp);
=======
	/* We can't touch HSW_AUD_PIN_ELD_CP_VLD uncionditionally because this
	 * register is part of the power well on Haswell. */
	if (intel_crtc->config.has_audio) {
		tmp = I915_READ(HSW_AUD_PIN_ELD_CP_VLD);
		tmp &= ~((AUDIO_OUTPUT_ENABLE_A | AUDIO_ELD_VALID_A) <<
			 (pipe * 4));
		I915_WRITE(HSW_AUD_PIN_ELD_CP_VLD, tmp);
		intel_display_power_put(dev_priv, POWER_DOMAIN_AUDIO);
	}
>>>>>>> v3.18

	if (type == INTEL_OUTPUT_EDP) {
		struct intel_dp *intel_dp = enc_to_intel_dp(encoder);

<<<<<<< HEAD
		ironlake_edp_backlight_off(intel_dp);
	}
}

int intel_ddi_get_cdclk_freq(struct drm_i915_private *dev_priv)
{
	if (I915_READ(HSW_FUSE_STRAP) & HSW_CDCLK_LIMIT)
		return 450;
	else if ((I915_READ(LCPLL_CTL) & LCPLL_CLK_FREQ_MASK) ==
		 LCPLL_CLK_FREQ_450)
		return 450;
	else if (IS_ULT(dev_priv->dev))
		return 338;
	else
		return 540;
=======
		intel_edp_psr_disable(intel_dp);
		intel_edp_backlight_off(intel_dp);
	}
}

static int bdw_get_cdclk_freq(struct drm_i915_private *dev_priv)
{
	uint32_t lcpll = I915_READ(LCPLL_CTL);
	uint32_t freq = lcpll & LCPLL_CLK_FREQ_MASK;

	if (lcpll & LCPLL_CD_SOURCE_FCLK)
		return 800000;
	else if (I915_READ(FUSE_STRAP) & HSW_CDCLK_LIMIT)
		return 450000;
	else if (freq == LCPLL_CLK_FREQ_450)
		return 450000;
	else if (freq == LCPLL_CLK_FREQ_54O_BDW)
		return 540000;
	else if (freq == LCPLL_CLK_FREQ_337_5_BDW)
		return 337500;
	else
		return 675000;
}

static int hsw_get_cdclk_freq(struct drm_i915_private *dev_priv)
{
	struct drm_device *dev = dev_priv->dev;
	uint32_t lcpll = I915_READ(LCPLL_CTL);
	uint32_t freq = lcpll & LCPLL_CLK_FREQ_MASK;

	if (lcpll & LCPLL_CD_SOURCE_FCLK)
		return 800000;
	else if (I915_READ(FUSE_STRAP) & HSW_CDCLK_LIMIT)
		return 450000;
	else if (freq == LCPLL_CLK_FREQ_450)
		return 450000;
	else if (IS_ULT(dev))
		return 337500;
	else
		return 540000;
}

int intel_ddi_get_cdclk_freq(struct drm_i915_private *dev_priv)
{
	struct drm_device *dev = dev_priv->dev;

	if (IS_BROADWELL(dev))
		return bdw_get_cdclk_freq(dev_priv);

	/* Haswell */
	return hsw_get_cdclk_freq(dev_priv);
}

static void hsw_ddi_pll_enable(struct drm_i915_private *dev_priv,
			       struct intel_shared_dpll *pll)
{
	I915_WRITE(WRPLL_CTL(pll->id), pll->hw_state.wrpll);
	POSTING_READ(WRPLL_CTL(pll->id));
	udelay(20);
}

static void hsw_ddi_pll_disable(struct drm_i915_private *dev_priv,
				struct intel_shared_dpll *pll)
{
	uint32_t val;

	val = I915_READ(WRPLL_CTL(pll->id));
	I915_WRITE(WRPLL_CTL(pll->id), val & ~WRPLL_PLL_ENABLE);
	POSTING_READ(WRPLL_CTL(pll->id));
}

static bool hsw_ddi_pll_get_hw_state(struct drm_i915_private *dev_priv,
				     struct intel_shared_dpll *pll,
				     struct intel_dpll_hw_state *hw_state)
{
	uint32_t val;

	if (!intel_display_power_enabled(dev_priv, POWER_DOMAIN_PLLS))
		return false;

	val = I915_READ(WRPLL_CTL(pll->id));
	hw_state->wrpll = val;

	return val & WRPLL_PLL_ENABLE;
}

static const char * const hsw_ddi_pll_names[] = {
	"WRPLL 1",
	"WRPLL 2",
};

static void hsw_shared_dplls_init(struct drm_i915_private *dev_priv)
{
	int i;

	dev_priv->num_shared_dpll = 2;

	for (i = 0; i < dev_priv->num_shared_dpll; i++) {
		dev_priv->shared_dplls[i].id = i;
		dev_priv->shared_dplls[i].name = hsw_ddi_pll_names[i];
		dev_priv->shared_dplls[i].disable = hsw_ddi_pll_disable;
		dev_priv->shared_dplls[i].enable = hsw_ddi_pll_enable;
		dev_priv->shared_dplls[i].get_hw_state =
			hsw_ddi_pll_get_hw_state;
	}
>>>>>>> v3.18
}

void intel_ddi_pll_init(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	uint32_t val = I915_READ(LCPLL_CTL);

<<<<<<< HEAD
=======
	hsw_shared_dplls_init(dev_priv);

>>>>>>> v3.18
	/* The LCPLL register should be turned on by the BIOS. For now let's
	 * just check its state and print errors in case something is wrong.
	 * Don't even try to turn it on.
	 */

<<<<<<< HEAD
	DRM_DEBUG_KMS("CDCLK running at %dMHz\n",
=======
	DRM_DEBUG_KMS("CDCLK running at %dKHz\n",
>>>>>>> v3.18
		      intel_ddi_get_cdclk_freq(dev_priv));

	if (val & LCPLL_CD_SOURCE_FCLK)
		DRM_ERROR("CDCLK source is not LCPLL\n");

	if (val & LCPLL_PLL_DISABLE)
		DRM_ERROR("LCPLL is disabled\n");
}

void intel_ddi_prepare_link_retrain(struct drm_encoder *encoder)
{
	struct intel_digital_port *intel_dig_port = enc_to_dig_port(encoder);
	struct intel_dp *intel_dp = &intel_dig_port->dp;
	struct drm_i915_private *dev_priv = encoder->dev->dev_private;
	enum port port = intel_dig_port->port;
	uint32_t val;
	bool wait = false;

	if (I915_READ(DP_TP_CTL(port)) & DP_TP_CTL_ENABLE) {
		val = I915_READ(DDI_BUF_CTL(port));
		if (val & DDI_BUF_CTL_ENABLE) {
			val &= ~DDI_BUF_CTL_ENABLE;
			I915_WRITE(DDI_BUF_CTL(port), val);
			wait = true;
		}

		val = I915_READ(DP_TP_CTL(port));
		val &= ~(DP_TP_CTL_ENABLE | DP_TP_CTL_LINK_TRAIN_MASK);
		val |= DP_TP_CTL_LINK_TRAIN_PAT1;
		I915_WRITE(DP_TP_CTL(port), val);
		POSTING_READ(DP_TP_CTL(port));

		if (wait)
			intel_wait_ddi_buf_idle(dev_priv, port);
	}

<<<<<<< HEAD
	val = DP_TP_CTL_ENABLE | DP_TP_CTL_MODE_SST |
	      DP_TP_CTL_LINK_TRAIN_PAT1 | DP_TP_CTL_SCRAMBLE_DISABLE;
	if (intel_dp->link_configuration[1] & DP_LANE_COUNT_ENHANCED_FRAME_EN)
		val |= DP_TP_CTL_ENHANCED_FRAME_ENABLE;
=======
	val = DP_TP_CTL_ENABLE |
	      DP_TP_CTL_LINK_TRAIN_PAT1 | DP_TP_CTL_SCRAMBLE_DISABLE;
	if (intel_dp->is_mst)
		val |= DP_TP_CTL_MODE_MST;
	else {
		val |= DP_TP_CTL_MODE_SST;
		if (drm_dp_enhanced_frame_cap(intel_dp->dpcd))
			val |= DP_TP_CTL_ENHANCED_FRAME_ENABLE;
	}
>>>>>>> v3.18
	I915_WRITE(DP_TP_CTL(port), val);
	POSTING_READ(DP_TP_CTL(port));

	intel_dp->DP |= DDI_BUF_CTL_ENABLE;
	I915_WRITE(DDI_BUF_CTL(port), intel_dp->DP);
	POSTING_READ(DDI_BUF_CTL(port));

	udelay(600);
}

void intel_ddi_fdi_disable(struct drm_crtc *crtc)
{
	struct drm_i915_private *dev_priv = crtc->dev->dev_private;
	struct intel_encoder *intel_encoder = intel_ddi_get_crtc_encoder(crtc);
	uint32_t val;

	intel_ddi_post_disable(intel_encoder);

	val = I915_READ(_FDI_RXA_CTL);
	val &= ~FDI_RX_ENABLE;
	I915_WRITE(_FDI_RXA_CTL, val);

	val = I915_READ(_FDI_RXA_MISC);
	val &= ~(FDI_RX_PWRDN_LANE1_MASK | FDI_RX_PWRDN_LANE0_MASK);
	val |= FDI_RX_PWRDN_LANE1_VAL(2) | FDI_RX_PWRDN_LANE0_VAL(2);
	I915_WRITE(_FDI_RXA_MISC, val);

	val = I915_READ(_FDI_RXA_CTL);
	val &= ~FDI_PCDCLK;
	I915_WRITE(_FDI_RXA_CTL, val);

	val = I915_READ(_FDI_RXA_CTL);
	val &= ~FDI_RX_PLL_ENABLE;
	I915_WRITE(_FDI_RXA_CTL, val);
}

static void intel_ddi_hot_plug(struct intel_encoder *intel_encoder)
{
<<<<<<< HEAD
	struct intel_dp *intel_dp = enc_to_intel_dp(&intel_encoder->base);
	int type = intel_encoder->type;

	if (type == INTEL_OUTPUT_DISPLAYPORT || type == INTEL_OUTPUT_EDP)
		intel_dp_check_link_status(intel_dp);
=======
	struct intel_digital_port *intel_dig_port = enc_to_dig_port(&intel_encoder->base);
	int type = intel_dig_port->base.type;

	if (type != INTEL_OUTPUT_DISPLAYPORT &&
	    type != INTEL_OUTPUT_EDP &&
	    type != INTEL_OUTPUT_UNKNOWN) {
		return;
	}

	intel_dp_hot_plug(intel_encoder);
}

void intel_ddi_get_config(struct intel_encoder *encoder,
			  struct intel_crtc_config *pipe_config)
{
	struct drm_i915_private *dev_priv = encoder->base.dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(encoder->base.crtc);
	enum transcoder cpu_transcoder = intel_crtc->config.cpu_transcoder;
	u32 temp, flags = 0;

	temp = I915_READ(TRANS_DDI_FUNC_CTL(cpu_transcoder));
	if (temp & TRANS_DDI_PHSYNC)
		flags |= DRM_MODE_FLAG_PHSYNC;
	else
		flags |= DRM_MODE_FLAG_NHSYNC;
	if (temp & TRANS_DDI_PVSYNC)
		flags |= DRM_MODE_FLAG_PVSYNC;
	else
		flags |= DRM_MODE_FLAG_NVSYNC;

	pipe_config->adjusted_mode.flags |= flags;

	switch (temp & TRANS_DDI_BPC_MASK) {
	case TRANS_DDI_BPC_6:
		pipe_config->pipe_bpp = 18;
		break;
	case TRANS_DDI_BPC_8:
		pipe_config->pipe_bpp = 24;
		break;
	case TRANS_DDI_BPC_10:
		pipe_config->pipe_bpp = 30;
		break;
	case TRANS_DDI_BPC_12:
		pipe_config->pipe_bpp = 36;
		break;
	default:
		break;
	}

	switch (temp & TRANS_DDI_MODE_SELECT_MASK) {
	case TRANS_DDI_MODE_SELECT_HDMI:
		pipe_config->has_hdmi_sink = true;
	case TRANS_DDI_MODE_SELECT_DVI:
	case TRANS_DDI_MODE_SELECT_FDI:
		break;
	case TRANS_DDI_MODE_SELECT_DP_SST:
	case TRANS_DDI_MODE_SELECT_DP_MST:
		pipe_config->has_dp_encoder = true;
		intel_dp_get_m_n(intel_crtc, pipe_config);
		break;
	default:
		break;
	}

	if (intel_display_power_enabled(dev_priv, POWER_DOMAIN_AUDIO)) {
		temp = I915_READ(HSW_AUD_PIN_ELD_CP_VLD);
		if (temp & (AUDIO_OUTPUT_ENABLE_A << (intel_crtc->pipe * 4)))
			pipe_config->has_audio = true;
	}

	if (encoder->type == INTEL_OUTPUT_EDP && dev_priv->vbt.edp_bpp &&
	    pipe_config->pipe_bpp > dev_priv->vbt.edp_bpp) {
		/*
		 * This is a big fat ugly hack.
		 *
		 * Some machines in UEFI boot mode provide us a VBT that has 18
		 * bpp and 1.62 GHz link bandwidth for eDP, which for reasons
		 * unknown we fail to light up. Yet the same BIOS boots up with
		 * 24 bpp and 2.7 GHz link. Use the same bpp as the BIOS uses as
		 * max, not what it tells us to use.
		 *
		 * Note: This will still be broken if the eDP panel is not lit
		 * up by the BIOS, and thus we can't get the mode at module
		 * load.
		 */
		DRM_DEBUG_KMS("pipe has %d bpp for eDP panel, overriding BIOS-provided max %d bpp\n",
			      pipe_config->pipe_bpp, dev_priv->vbt.edp_bpp);
		dev_priv->vbt.edp_bpp = pipe_config->pipe_bpp;
	}

	hsw_ddi_clock_get(encoder, pipe_config);
>>>>>>> v3.18
}

static void intel_ddi_destroy(struct drm_encoder *encoder)
{
	/* HDMI has nothing special to destroy, so we can go with this. */
	intel_dp_encoder_destroy(encoder);
}

static bool intel_ddi_compute_config(struct intel_encoder *encoder,
				     struct intel_crtc_config *pipe_config)
{
	int type = encoder->type;
<<<<<<< HEAD

	WARN(type == INTEL_OUTPUT_UNKNOWN, "compute_config() on unknown output!\n");

=======
	int port = intel_ddi_get_encoder_port(encoder);

	WARN(type == INTEL_OUTPUT_UNKNOWN, "compute_config() on unknown output!\n");

	if (port == PORT_A)
		pipe_config->cpu_transcoder = TRANSCODER_EDP;

>>>>>>> v3.18
	if (type == INTEL_OUTPUT_HDMI)
		return intel_hdmi_compute_config(encoder, pipe_config);
	else
		return intel_dp_compute_config(encoder, pipe_config);
}

static const struct drm_encoder_funcs intel_ddi_funcs = {
	.destroy = intel_ddi_destroy,
};

<<<<<<< HEAD
static const struct drm_encoder_helper_funcs intel_ddi_helper_funcs = {
	.mode_set = intel_ddi_mode_set,
};
=======
static struct intel_connector *
intel_ddi_init_dp_connector(struct intel_digital_port *intel_dig_port)
{
	struct intel_connector *connector;
	enum port port = intel_dig_port->port;

	connector = kzalloc(sizeof(*connector), GFP_KERNEL);
	if (!connector)
		return NULL;

	intel_dig_port->dp.output_reg = DDI_BUF_CTL(port);
	if (!intel_dp_init_connector(intel_dig_port, connector)) {
		kfree(connector);
		return NULL;
	}

	return connector;
}

static struct intel_connector *
intel_ddi_init_hdmi_connector(struct intel_digital_port *intel_dig_port)
{
	struct intel_connector *connector;
	enum port port = intel_dig_port->port;

	connector = kzalloc(sizeof(*connector), GFP_KERNEL);
	if (!connector)
		return NULL;

	intel_dig_port->hdmi.hdmi_reg = DDI_BUF_CTL(port);
	intel_hdmi_init_connector(intel_dig_port, connector);

	return connector;
}
>>>>>>> v3.18

void intel_ddi_init(struct drm_device *dev, enum port port)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_digital_port *intel_dig_port;
	struct intel_encoder *intel_encoder;
	struct drm_encoder *encoder;
<<<<<<< HEAD
	struct intel_connector *hdmi_connector = NULL;
	struct intel_connector *dp_connector = NULL;

	intel_dig_port = kzalloc(sizeof(struct intel_digital_port), GFP_KERNEL);
	if (!intel_dig_port)
		return;

	dp_connector = kzalloc(sizeof(struct intel_connector), GFP_KERNEL);
	if (!dp_connector) {
		kfree(intel_dig_port);
		return;
	}

	if (port != PORT_A) {
		hdmi_connector = kzalloc(sizeof(struct intel_connector),
					 GFP_KERNEL);
		if (!hdmi_connector) {
			kfree(dp_connector);
			kfree(intel_dig_port);
			return;
		}
	}

=======
	bool init_hdmi, init_dp;

	init_hdmi = (dev_priv->vbt.ddi_port_info[port].supports_dvi ||
		     dev_priv->vbt.ddi_port_info[port].supports_hdmi);
	init_dp = dev_priv->vbt.ddi_port_info[port].supports_dp;
	if (!init_dp && !init_hdmi) {
		DRM_DEBUG_KMS("VBT says port %c is not DVI/HDMI/DP compatible, assuming it is\n",
			      port_name(port));
		init_hdmi = true;
		init_dp = true;
	}

	intel_dig_port = kzalloc(sizeof(*intel_dig_port), GFP_KERNEL);
	if (!intel_dig_port)
		return;

>>>>>>> v3.18
	intel_encoder = &intel_dig_port->base;
	encoder = &intel_encoder->base;

	drm_encoder_init(dev, encoder, &intel_ddi_funcs,
			 DRM_MODE_ENCODER_TMDS);
<<<<<<< HEAD
	drm_encoder_helper_add(encoder, &intel_ddi_helper_funcs);
=======
>>>>>>> v3.18

	intel_encoder->compute_config = intel_ddi_compute_config;
	intel_encoder->enable = intel_enable_ddi;
	intel_encoder->pre_enable = intel_ddi_pre_enable;
	intel_encoder->disable = intel_disable_ddi;
	intel_encoder->post_disable = intel_ddi_post_disable;
	intel_encoder->get_hw_state = intel_ddi_get_hw_state;
<<<<<<< HEAD
=======
	intel_encoder->get_config = intel_ddi_get_config;
>>>>>>> v3.18

	intel_dig_port->port = port;
	intel_dig_port->saved_port_bits = I915_READ(DDI_BUF_CTL(port)) &
					  (DDI_BUF_PORT_REVERSAL |
					   DDI_A_4_LANES);
<<<<<<< HEAD
	if (hdmi_connector)
		intel_dig_port->hdmi.hdmi_reg = DDI_BUF_CTL(port);
	intel_dig_port->dp.output_reg = DDI_BUF_CTL(port);

	intel_encoder->type = INTEL_OUTPUT_UNKNOWN;
	intel_encoder->crtc_mask =  (1 << 0) | (1 << 1) | (1 << 2);
	intel_encoder->cloneable = false;
	intel_encoder->hot_plug = intel_ddi_hot_plug;

	if (hdmi_connector)
		intel_hdmi_init_connector(intel_dig_port, hdmi_connector);
	intel_dp_init_connector(intel_dig_port, dp_connector);
=======

	intel_encoder->type = INTEL_OUTPUT_UNKNOWN;
	intel_encoder->crtc_mask = (1 << 0) | (1 << 1) | (1 << 2);
	intel_encoder->cloneable = 0;
	intel_encoder->hot_plug = intel_ddi_hot_plug;

	if (init_dp) {
		if (!intel_ddi_init_dp_connector(intel_dig_port))
			goto err;

		intel_dig_port->hpd_pulse = intel_dp_hpd_pulse;
		dev_priv->hpd_irq_port[port] = intel_dig_port;
	}

	/* In theory we don't need the encoder->type check, but leave it just in
	 * case we have some really bad VBTs... */
	if (intel_encoder->type != INTEL_OUTPUT_EDP && init_hdmi) {
		if (!intel_ddi_init_hdmi_connector(intel_dig_port))
			goto err;
	}

	return;

err:
	drm_encoder_cleanup(encoder);
	kfree(intel_dig_port);
>>>>>>> v3.18
}
