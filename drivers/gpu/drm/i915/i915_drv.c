/* i915_drv.c -- i830,i845,i855,i865,i915 driver -*- linux-c -*-
 */
/*
 *
 * Copyright 2003 Tungsten Graphics, Inc., Cedar Park, Texas.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <linux/device.h>
<<<<<<< HEAD
=======
#include <linux/acpi.h>
>>>>>>> v3.18
#include <drm/drmP.h>
#include <drm/i915_drm.h>
#include "i915_drv.h"
#include "i915_trace.h"
#include "intel_drv.h"

#include <linux/console.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <drm/drm_crtc_helper.h>

static int i915_modeset __read_mostly = -1;
module_param_named(modeset, i915_modeset, int, 0400);
MODULE_PARM_DESC(modeset,
		"Use kernel modesetting [KMS] (0=DRM_I915_KMS from .config, "
		"1=on, -1=force vga console preference [default])");

unsigned int i915_fbpercrtc __always_unused = 0;
module_param_named(fbpercrtc, i915_fbpercrtc, int, 0400);

int i915_panel_ignore_lid __read_mostly = 1;
module_param_named(panel_ignore_lid, i915_panel_ignore_lid, int, 0600);
MODULE_PARM_DESC(panel_ignore_lid,
		"Override lid status (0=autodetect, 1=autodetect disabled [default], "
		"-1=force lid closed, -2=force lid open)");

unsigned int i915_powersave __read_mostly = 1;
module_param_named(powersave, i915_powersave, int, 0600);
MODULE_PARM_DESC(powersave,
		"Enable powersavings, fbc, downclocking, etc. (default: true)");

int i915_semaphores __read_mostly = -1;
module_param_named(semaphores, i915_semaphores, int, 0600);
MODULE_PARM_DESC(semaphores,
		"Use semaphores for inter-ring sync (default: -1 (use per-chip defaults))");

int i915_enable_rc6 __read_mostly = -1;
module_param_named(i915_enable_rc6, i915_enable_rc6, int, 0400);
MODULE_PARM_DESC(i915_enable_rc6,
		"Enable power-saving render C-state 6. "
		"Different stages can be selected via bitmask values "
		"(0 = disable; 1 = enable rc6; 2 = enable deep rc6; 4 = enable deepest rc6). "
		"For example, 3 would enable rc6 and deep rc6, and 7 would enable everything. "
		"default: -1 (use per-chip default)");

int i915_enable_fbc __read_mostly = -1;
module_param_named(i915_enable_fbc, i915_enable_fbc, int, 0600);
MODULE_PARM_DESC(i915_enable_fbc,
		"Enable frame buffer compression for power savings "
		"(default: -1 (use per-chip default))");

unsigned int i915_lvds_downclock __read_mostly = 0;
module_param_named(lvds_downclock, i915_lvds_downclock, int, 0400);
MODULE_PARM_DESC(lvds_downclock,
		"Use panel (LVDS/eDP) downclocking for power savings "
		"(default: false)");

int i915_lvds_channel_mode __read_mostly;
module_param_named(lvds_channel_mode, i915_lvds_channel_mode, int, 0600);
MODULE_PARM_DESC(lvds_channel_mode,
		 "Specify LVDS channel mode "
		 "(0=probe BIOS [default], 1=single-channel, 2=dual-channel)");

int i915_panel_use_ssc __read_mostly = -1;
module_param_named(lvds_use_ssc, i915_panel_use_ssc, int, 0600);
MODULE_PARM_DESC(lvds_use_ssc,
		"Use Spread Spectrum Clock with panels [LVDS/eDP] "
		"(default: auto from VBT)");

int i915_vbt_sdvo_panel_type __read_mostly = -1;
module_param_named(vbt_sdvo_panel_type, i915_vbt_sdvo_panel_type, int, 0600);
MODULE_PARM_DESC(vbt_sdvo_panel_type,
		"Override/Ignore selection of SDVO panel mode in the VBT "
		"(-2=ignore, -1=auto [default], index in VBT BIOS table)");

static bool i915_try_reset __read_mostly = true;
module_param_named(reset, i915_try_reset, bool, 0600);
MODULE_PARM_DESC(reset, "Attempt GPU resets (default: true)");

bool i915_enable_hangcheck __read_mostly = true;
module_param_named(enable_hangcheck, i915_enable_hangcheck, bool, 0644);
MODULE_PARM_DESC(enable_hangcheck,
		"Periodically check GPU activity for detecting hangs. "
		"WARNING: Disabling this can cause system wide hangs. "
		"(default: true)");

int i915_enable_ppgtt __read_mostly = -1;
module_param_named(i915_enable_ppgtt, i915_enable_ppgtt, int, 0600);
MODULE_PARM_DESC(i915_enable_ppgtt,
		"Enable PPGTT (default: true)");

unsigned int i915_preliminary_hw_support __read_mostly = 0;
module_param_named(preliminary_hw_support, i915_preliminary_hw_support, int, 0600);
MODULE_PARM_DESC(preliminary_hw_support,
		"Enable preliminary hardware support. (default: false)");

int i915_disable_power_well __read_mostly = 0;
module_param_named(disable_power_well, i915_disable_power_well, int, 0600);
MODULE_PARM_DESC(disable_power_well,
		 "Disable the power well when possible (default: false)");

static struct drm_driver driver;
extern int intel_agp_enabled;

#define INTEL_VGA_DEVICE(id, info) {		\
	.class = PCI_BASE_CLASS_DISPLAY << 16,	\
	.class_mask = 0xff0000,			\
	.vendor = 0x8086,			\
	.device = id,				\
	.subvendor = PCI_ANY_ID,		\
	.subdevice = PCI_ANY_ID,		\
	.driver_data = (unsigned long) info }

#define INTEL_QUANTA_VGA_DEVICE(info) {		\
	.class = PCI_BASE_CLASS_DISPLAY << 16,	\
	.class_mask = 0xff0000,			\
	.vendor = 0x8086,			\
	.device = 0x16a,			\
	.subvendor = 0x152d,			\
	.subdevice = 0x8990,			\
	.driver_data = (unsigned long) info }

=======
#include <linux/pm_runtime.h>
#include <drm/drm_crtc_helper.h>

static struct drm_driver driver;

#define GEN_DEFAULT_PIPEOFFSETS \
	.pipe_offsets = { PIPE_A_OFFSET, PIPE_B_OFFSET, \
			  PIPE_C_OFFSET, PIPE_EDP_OFFSET }, \
	.trans_offsets = { TRANSCODER_A_OFFSET, TRANSCODER_B_OFFSET, \
			   TRANSCODER_C_OFFSET, TRANSCODER_EDP_OFFSET }, \
	.palette_offsets = { PALETTE_A_OFFSET, PALETTE_B_OFFSET }

#define GEN_CHV_PIPEOFFSETS \
	.pipe_offsets = { PIPE_A_OFFSET, PIPE_B_OFFSET, \
			  CHV_PIPE_C_OFFSET }, \
	.trans_offsets = { TRANSCODER_A_OFFSET, TRANSCODER_B_OFFSET, \
			   CHV_TRANSCODER_C_OFFSET, }, \
	.palette_offsets = { PALETTE_A_OFFSET, PALETTE_B_OFFSET, \
			     CHV_PALETTE_C_OFFSET }

#define CURSOR_OFFSETS \
	.cursor_offsets = { CURSOR_A_OFFSET, CURSOR_B_OFFSET, CHV_CURSOR_C_OFFSET }

#define IVB_CURSOR_OFFSETS \
	.cursor_offsets = { CURSOR_A_OFFSET, IVB_CURSOR_B_OFFSET, IVB_CURSOR_C_OFFSET }
>>>>>>> v3.18

static const struct intel_device_info intel_i830_info = {
	.gen = 2, .is_mobile = 1, .cursor_needs_physical = 1, .num_pipes = 2,
	.has_overlay = 1, .overlay_needs_physical = 1,
<<<<<<< HEAD
=======
	.ring_mask = RENDER_RING,
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_845g_info = {
	.gen = 2, .num_pipes = 1,
	.has_overlay = 1, .overlay_needs_physical = 1,
<<<<<<< HEAD
=======
	.ring_mask = RENDER_RING,
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_i85x_info = {
	.gen = 2, .is_i85x = 1, .is_mobile = 1, .num_pipes = 2,
	.cursor_needs_physical = 1,
	.has_overlay = 1, .overlay_needs_physical = 1,
<<<<<<< HEAD
=======
	.has_fbc = 1,
	.ring_mask = RENDER_RING,
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_i865g_info = {
	.gen = 2, .num_pipes = 1,
	.has_overlay = 1, .overlay_needs_physical = 1,
<<<<<<< HEAD
=======
	.ring_mask = RENDER_RING,
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_i915g_info = {
	.gen = 3, .is_i915g = 1, .cursor_needs_physical = 1, .num_pipes = 2,
	.has_overlay = 1, .overlay_needs_physical = 1,
<<<<<<< HEAD
=======
	.ring_mask = RENDER_RING,
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};
static const struct intel_device_info intel_i915gm_info = {
	.gen = 3, .is_mobile = 1, .num_pipes = 2,
	.cursor_needs_physical = 1,
	.has_overlay = 1, .overlay_needs_physical = 1,
	.supports_tv = 1,
<<<<<<< HEAD
=======
	.has_fbc = 1,
	.ring_mask = RENDER_RING,
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};
static const struct intel_device_info intel_i945g_info = {
	.gen = 3, .has_hotplug = 1, .cursor_needs_physical = 1, .num_pipes = 2,
	.has_overlay = 1, .overlay_needs_physical = 1,
<<<<<<< HEAD
=======
	.ring_mask = RENDER_RING,
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};
static const struct intel_device_info intel_i945gm_info = {
	.gen = 3, .is_i945gm = 1, .is_mobile = 1, .num_pipes = 2,
	.has_hotplug = 1, .cursor_needs_physical = 1,
	.has_overlay = 1, .overlay_needs_physical = 1,
	.supports_tv = 1,
<<<<<<< HEAD
=======
	.has_fbc = 1,
	.ring_mask = RENDER_RING,
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_i965g_info = {
	.gen = 4, .is_broadwater = 1, .num_pipes = 2,
	.has_hotplug = 1,
	.has_overlay = 1,
<<<<<<< HEAD
=======
	.ring_mask = RENDER_RING,
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_i965gm_info = {
	.gen = 4, .is_crestline = 1, .num_pipes = 2,
	.is_mobile = 1, .has_fbc = 1, .has_hotplug = 1,
	.has_overlay = 1,
	.supports_tv = 1,
<<<<<<< HEAD
=======
	.ring_mask = RENDER_RING,
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_g33_info = {
	.gen = 3, .is_g33 = 1, .num_pipes = 2,
	.need_gfx_hws = 1, .has_hotplug = 1,
	.has_overlay = 1,
<<<<<<< HEAD
=======
	.ring_mask = RENDER_RING,
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_g45_info = {
	.gen = 4, .is_g4x = 1, .need_gfx_hws = 1, .num_pipes = 2,
	.has_pipe_cxsr = 1, .has_hotplug = 1,
<<<<<<< HEAD
	.has_bsd_ring = 1,
=======
	.ring_mask = RENDER_RING | BSD_RING,
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_gm45_info = {
	.gen = 4, .is_g4x = 1, .num_pipes = 2,
	.is_mobile = 1, .need_gfx_hws = 1, .has_fbc = 1,
	.has_pipe_cxsr = 1, .has_hotplug = 1,
	.supports_tv = 1,
<<<<<<< HEAD
	.has_bsd_ring = 1,
=======
	.ring_mask = RENDER_RING | BSD_RING,
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_pineview_info = {
	.gen = 3, .is_g33 = 1, .is_pineview = 1, .is_mobile = 1, .num_pipes = 2,
	.need_gfx_hws = 1, .has_hotplug = 1,
	.has_overlay = 1,
<<<<<<< HEAD
=======
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_ironlake_d_info = {
	.gen = 5, .num_pipes = 2,
	.need_gfx_hws = 1, .has_hotplug = 1,
<<<<<<< HEAD
	.has_bsd_ring = 1,
=======
	.ring_mask = RENDER_RING | BSD_RING,
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_ironlake_m_info = {
	.gen = 5, .is_mobile = 1, .num_pipes = 2,
	.need_gfx_hws = 1, .has_hotplug = 1,
	.has_fbc = 1,
<<<<<<< HEAD
	.has_bsd_ring = 1,
=======
	.ring_mask = RENDER_RING | BSD_RING,
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_sandybridge_d_info = {
	.gen = 6, .num_pipes = 2,
	.need_gfx_hws = 1, .has_hotplug = 1,
<<<<<<< HEAD
	.has_bsd_ring = 1,
	.has_blt_ring = 1,
	.has_llc = 1,
	.has_force_wake = 1,
=======
	.has_fbc = 1,
	.ring_mask = RENDER_RING | BSD_RING | BLT_RING,
	.has_llc = 1,
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_sandybridge_m_info = {
	.gen = 6, .is_mobile = 1, .num_pipes = 2,
	.need_gfx_hws = 1, .has_hotplug = 1,
	.has_fbc = 1,
<<<<<<< HEAD
	.has_bsd_ring = 1,
	.has_blt_ring = 1,
	.has_llc = 1,
	.has_force_wake = 1,
=======
	.ring_mask = RENDER_RING | BSD_RING | BLT_RING,
	.has_llc = 1,
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};

#define GEN7_FEATURES  \
	.gen = 7, .num_pipes = 3, \
	.need_gfx_hws = 1, .has_hotplug = 1, \
<<<<<<< HEAD
	.has_bsd_ring = 1, \
	.has_blt_ring = 1, \
	.has_llc = 1, \
	.has_force_wake = 1
=======
	.has_fbc = 1, \
	.ring_mask = RENDER_RING | BSD_RING | BLT_RING, \
	.has_llc = 1
>>>>>>> v3.18

static const struct intel_device_info intel_ivybridge_d_info = {
	GEN7_FEATURES,
	.is_ivybridge = 1,
<<<<<<< HEAD
=======
	GEN_DEFAULT_PIPEOFFSETS,
	IVB_CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_ivybridge_m_info = {
	GEN7_FEATURES,
	.is_ivybridge = 1,
	.is_mobile = 1,
<<<<<<< HEAD
=======
	GEN_DEFAULT_PIPEOFFSETS,
	IVB_CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_ivybridge_q_info = {
	GEN7_FEATURES,
	.is_ivybridge = 1,
	.num_pipes = 0, /* legal, last one wins */
<<<<<<< HEAD
=======
	GEN_DEFAULT_PIPEOFFSETS,
	IVB_CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_valleyview_m_info = {
	GEN7_FEATURES,
	.is_mobile = 1,
	.num_pipes = 2,
	.is_valleyview = 1,
	.display_mmio_offset = VLV_DISPLAY_BASE,
<<<<<<< HEAD
	.has_llc = 0, /* legal, last one wins */
=======
	.has_fbc = 0, /* legal, last one wins */
	.has_llc = 0, /* legal, last one wins */
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_valleyview_d_info = {
	GEN7_FEATURES,
	.num_pipes = 2,
	.is_valleyview = 1,
	.display_mmio_offset = VLV_DISPLAY_BASE,
<<<<<<< HEAD
	.has_llc = 0, /* legal, last one wins */
=======
	.has_fbc = 0, /* legal, last one wins */
	.has_llc = 0, /* legal, last one wins */
	GEN_DEFAULT_PIPEOFFSETS,
	CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_haswell_d_info = {
	GEN7_FEATURES,
	.is_haswell = 1,
<<<<<<< HEAD
=======
	.has_ddi = 1,
	.has_fpga_dbg = 1,
	.ring_mask = RENDER_RING | BSD_RING | BLT_RING | VEBOX_RING,
	GEN_DEFAULT_PIPEOFFSETS,
	IVB_CURSOR_OFFSETS,
>>>>>>> v3.18
};

static const struct intel_device_info intel_haswell_m_info = {
	GEN7_FEATURES,
	.is_haswell = 1,
	.is_mobile = 1,
<<<<<<< HEAD
};

static const struct pci_device_id pciidlist[] = {		/* aka */
	INTEL_VGA_DEVICE(0x3577, &intel_i830_info),		/* I830_M */
	INTEL_VGA_DEVICE(0x2562, &intel_845g_info),		/* 845_G */
	INTEL_VGA_DEVICE(0x3582, &intel_i85x_info),		/* I855_GM */
	INTEL_VGA_DEVICE(0x358e, &intel_i85x_info),
	INTEL_VGA_DEVICE(0x2572, &intel_i865g_info),		/* I865_G */
	INTEL_VGA_DEVICE(0x2582, &intel_i915g_info),		/* I915_G */
	INTEL_VGA_DEVICE(0x258a, &intel_i915g_info),		/* E7221_G */
	INTEL_VGA_DEVICE(0x2592, &intel_i915gm_info),		/* I915_GM */
	INTEL_VGA_DEVICE(0x2772, &intel_i945g_info),		/* I945_G */
	INTEL_VGA_DEVICE(0x27a2, &intel_i945gm_info),		/* I945_GM */
	INTEL_VGA_DEVICE(0x27ae, &intel_i945gm_info),		/* I945_GME */
	INTEL_VGA_DEVICE(0x2972, &intel_i965g_info),		/* I946_GZ */
	INTEL_VGA_DEVICE(0x2982, &intel_i965g_info),		/* G35_G */
	INTEL_VGA_DEVICE(0x2992, &intel_i965g_info),		/* I965_Q */
	INTEL_VGA_DEVICE(0x29a2, &intel_i965g_info),		/* I965_G */
	INTEL_VGA_DEVICE(0x29b2, &intel_g33_info),		/* Q35_G */
	INTEL_VGA_DEVICE(0x29c2, &intel_g33_info),		/* G33_G */
	INTEL_VGA_DEVICE(0x29d2, &intel_g33_info),		/* Q33_G */
	INTEL_VGA_DEVICE(0x2a02, &intel_i965gm_info),		/* I965_GM */
	INTEL_VGA_DEVICE(0x2a12, &intel_i965gm_info),		/* I965_GME */
	INTEL_VGA_DEVICE(0x2a42, &intel_gm45_info),		/* GM45_G */
	INTEL_VGA_DEVICE(0x2e02, &intel_g45_info),		/* IGD_E_G */
	INTEL_VGA_DEVICE(0x2e12, &intel_g45_info),		/* Q45_G */
	INTEL_VGA_DEVICE(0x2e22, &intel_g45_info),		/* G45_G */
	INTEL_VGA_DEVICE(0x2e32, &intel_g45_info),		/* G41_G */
	INTEL_VGA_DEVICE(0x2e42, &intel_g45_info),		/* B43_G */
	INTEL_VGA_DEVICE(0x2e92, &intel_g45_info),		/* B43_G.1 */
	INTEL_VGA_DEVICE(0xa001, &intel_pineview_info),
	INTEL_VGA_DEVICE(0xa011, &intel_pineview_info),
	INTEL_VGA_DEVICE(0x0042, &intel_ironlake_d_info),
	INTEL_VGA_DEVICE(0x0046, &intel_ironlake_m_info),
	INTEL_VGA_DEVICE(0x0102, &intel_sandybridge_d_info),
	INTEL_VGA_DEVICE(0x0112, &intel_sandybridge_d_info),
	INTEL_VGA_DEVICE(0x0122, &intel_sandybridge_d_info),
	INTEL_VGA_DEVICE(0x0106, &intel_sandybridge_m_info),
	INTEL_VGA_DEVICE(0x0116, &intel_sandybridge_m_info),
	INTEL_VGA_DEVICE(0x0126, &intel_sandybridge_m_info),
	INTEL_VGA_DEVICE(0x010A, &intel_sandybridge_d_info),
	INTEL_VGA_DEVICE(0x0156, &intel_ivybridge_m_info), /* GT1 mobile */
	INTEL_VGA_DEVICE(0x0166, &intel_ivybridge_m_info), /* GT2 mobile */
	INTEL_VGA_DEVICE(0x0152, &intel_ivybridge_d_info), /* GT1 desktop */
	INTEL_VGA_DEVICE(0x0162, &intel_ivybridge_d_info), /* GT2 desktop */
	INTEL_VGA_DEVICE(0x015a, &intel_ivybridge_d_info), /* GT1 server */
	INTEL_QUANTA_VGA_DEVICE(&intel_ivybridge_q_info), /* Quanta transcode */
	INTEL_VGA_DEVICE(0x016a, &intel_ivybridge_d_info), /* GT2 server */
	INTEL_VGA_DEVICE(0x0402, &intel_haswell_d_info), /* GT1 desktop */
	INTEL_VGA_DEVICE(0x0412, &intel_haswell_d_info), /* GT2 desktop */
	INTEL_VGA_DEVICE(0x0422, &intel_haswell_d_info), /* GT3 desktop */
	INTEL_VGA_DEVICE(0x040a, &intel_haswell_d_info), /* GT1 server */
	INTEL_VGA_DEVICE(0x041a, &intel_haswell_d_info), /* GT2 server */
	INTEL_VGA_DEVICE(0x042a, &intel_haswell_d_info), /* GT3 server */
	INTEL_VGA_DEVICE(0x0406, &intel_haswell_m_info), /* GT1 mobile */
	INTEL_VGA_DEVICE(0x0416, &intel_haswell_m_info), /* GT2 mobile */
	INTEL_VGA_DEVICE(0x0426, &intel_haswell_m_info), /* GT2 mobile */
	INTEL_VGA_DEVICE(0x040B, &intel_haswell_d_info), /* GT1 reserved */
	INTEL_VGA_DEVICE(0x041B, &intel_haswell_d_info), /* GT2 reserved */
	INTEL_VGA_DEVICE(0x042B, &intel_haswell_d_info), /* GT3 reserved */
	INTEL_VGA_DEVICE(0x040E, &intel_haswell_d_info), /* GT1 reserved */
	INTEL_VGA_DEVICE(0x041E, &intel_haswell_d_info), /* GT2 reserved */
	INTEL_VGA_DEVICE(0x042E, &intel_haswell_d_info), /* GT3 reserved */
	INTEL_VGA_DEVICE(0x0C02, &intel_haswell_d_info), /* SDV GT1 desktop */
	INTEL_VGA_DEVICE(0x0C12, &intel_haswell_d_info), /* SDV GT2 desktop */
	INTEL_VGA_DEVICE(0x0C22, &intel_haswell_d_info), /* SDV GT3 desktop */
	INTEL_VGA_DEVICE(0x0C0A, &intel_haswell_d_info), /* SDV GT1 server */
	INTEL_VGA_DEVICE(0x0C1A, &intel_haswell_d_info), /* SDV GT2 server */
	INTEL_VGA_DEVICE(0x0C2A, &intel_haswell_d_info), /* SDV GT3 server */
	INTEL_VGA_DEVICE(0x0C06, &intel_haswell_m_info), /* SDV GT1 mobile */
	INTEL_VGA_DEVICE(0x0C16, &intel_haswell_m_info), /* SDV GT2 mobile */
	INTEL_VGA_DEVICE(0x0C26, &intel_haswell_m_info), /* SDV GT3 mobile */
	INTEL_VGA_DEVICE(0x0C0B, &intel_haswell_d_info), /* SDV GT1 reserved */
	INTEL_VGA_DEVICE(0x0C1B, &intel_haswell_d_info), /* SDV GT2 reserved */
	INTEL_VGA_DEVICE(0x0C2B, &intel_haswell_d_info), /* SDV GT3 reserved */
	INTEL_VGA_DEVICE(0x0C0E, &intel_haswell_d_info), /* SDV GT1 reserved */
	INTEL_VGA_DEVICE(0x0C1E, &intel_haswell_d_info), /* SDV GT2 reserved */
	INTEL_VGA_DEVICE(0x0C2E, &intel_haswell_d_info), /* SDV GT3 reserved */
	INTEL_VGA_DEVICE(0x0A02, &intel_haswell_d_info), /* ULT GT1 desktop */
	INTEL_VGA_DEVICE(0x0A12, &intel_haswell_d_info), /* ULT GT2 desktop */
	INTEL_VGA_DEVICE(0x0A22, &intel_haswell_d_info), /* ULT GT3 desktop */
	INTEL_VGA_DEVICE(0x0A0A, &intel_haswell_d_info), /* ULT GT1 server */
	INTEL_VGA_DEVICE(0x0A1A, &intel_haswell_d_info), /* ULT GT2 server */
	INTEL_VGA_DEVICE(0x0A2A, &intel_haswell_d_info), /* ULT GT3 server */
	INTEL_VGA_DEVICE(0x0A06, &intel_haswell_m_info), /* ULT GT1 mobile */
	INTEL_VGA_DEVICE(0x0A16, &intel_haswell_m_info), /* ULT GT2 mobile */
	INTEL_VGA_DEVICE(0x0A26, &intel_haswell_m_info), /* ULT GT3 mobile */
	INTEL_VGA_DEVICE(0x0A0B, &intel_haswell_d_info), /* ULT GT1 reserved */
	INTEL_VGA_DEVICE(0x0A1B, &intel_haswell_d_info), /* ULT GT2 reserved */
	INTEL_VGA_DEVICE(0x0A2B, &intel_haswell_d_info), /* ULT GT3 reserved */
	INTEL_VGA_DEVICE(0x0A0E, &intel_haswell_m_info), /* ULT GT1 reserved */
	INTEL_VGA_DEVICE(0x0A1E, &intel_haswell_m_info), /* ULT GT2 reserved */
	INTEL_VGA_DEVICE(0x0A2E, &intel_haswell_m_info), /* ULT GT3 reserved */
	INTEL_VGA_DEVICE(0x0D02, &intel_haswell_d_info), /* CRW GT1 desktop */
	INTEL_VGA_DEVICE(0x0D12, &intel_haswell_d_info), /* CRW GT2 desktop */
	INTEL_VGA_DEVICE(0x0D22, &intel_haswell_d_info), /* CRW GT3 desktop */
	INTEL_VGA_DEVICE(0x0D0A, &intel_haswell_d_info), /* CRW GT1 server */
	INTEL_VGA_DEVICE(0x0D1A, &intel_haswell_d_info), /* CRW GT2 server */
	INTEL_VGA_DEVICE(0x0D2A, &intel_haswell_d_info), /* CRW GT3 server */
	INTEL_VGA_DEVICE(0x0D06, &intel_haswell_m_info), /* CRW GT1 mobile */
	INTEL_VGA_DEVICE(0x0D16, &intel_haswell_m_info), /* CRW GT2 mobile */
	INTEL_VGA_DEVICE(0x0D26, &intel_haswell_m_info), /* CRW GT3 mobile */
	INTEL_VGA_DEVICE(0x0D0B, &intel_haswell_d_info), /* CRW GT1 reserved */
	INTEL_VGA_DEVICE(0x0D1B, &intel_haswell_d_info), /* CRW GT2 reserved */
	INTEL_VGA_DEVICE(0x0D2B, &intel_haswell_d_info), /* CRW GT3 reserved */
	INTEL_VGA_DEVICE(0x0D0E, &intel_haswell_d_info), /* CRW GT1 reserved */
	INTEL_VGA_DEVICE(0x0D1E, &intel_haswell_d_info), /* CRW GT2 reserved */
	INTEL_VGA_DEVICE(0x0D2E, &intel_haswell_d_info), /* CRW GT3 reserved */
	INTEL_VGA_DEVICE(0x0f30, &intel_valleyview_m_info),
	INTEL_VGA_DEVICE(0x0f31, &intel_valleyview_m_info),
	INTEL_VGA_DEVICE(0x0f32, &intel_valleyview_m_info),
	INTEL_VGA_DEVICE(0x0f33, &intel_valleyview_m_info),
	INTEL_VGA_DEVICE(0x0157, &intel_valleyview_m_info),
	INTEL_VGA_DEVICE(0x0155, &intel_valleyview_d_info),
=======
	.has_ddi = 1,
	.has_fpga_dbg = 1,
	.ring_mask = RENDER_RING | BSD_RING | BLT_RING | VEBOX_RING,
	GEN_DEFAULT_PIPEOFFSETS,
	IVB_CURSOR_OFFSETS,
};

static const struct intel_device_info intel_broadwell_d_info = {
	.gen = 8, .num_pipes = 3,
	.need_gfx_hws = 1, .has_hotplug = 1,
	.ring_mask = RENDER_RING | BSD_RING | BLT_RING | VEBOX_RING,
	.has_llc = 1,
	.has_ddi = 1,
	.has_fpga_dbg = 1,
	.has_fbc = 1,
	GEN_DEFAULT_PIPEOFFSETS,
	IVB_CURSOR_OFFSETS,
};

static const struct intel_device_info intel_broadwell_m_info = {
	.gen = 8, .is_mobile = 1, .num_pipes = 3,
	.need_gfx_hws = 1, .has_hotplug = 1,
	.ring_mask = RENDER_RING | BSD_RING | BLT_RING | VEBOX_RING,
	.has_llc = 1,
	.has_ddi = 1,
	.has_fpga_dbg = 1,
	.has_fbc = 1,
	GEN_DEFAULT_PIPEOFFSETS,
	IVB_CURSOR_OFFSETS,
};

static const struct intel_device_info intel_broadwell_gt3d_info = {
	.gen = 8, .num_pipes = 3,
	.need_gfx_hws = 1, .has_hotplug = 1,
	.ring_mask = RENDER_RING | BSD_RING | BLT_RING | VEBOX_RING | BSD2_RING,
	.has_llc = 1,
	.has_ddi = 1,
	.has_fpga_dbg = 1,
	.has_fbc = 1,
	GEN_DEFAULT_PIPEOFFSETS,
	IVB_CURSOR_OFFSETS,
};

static const struct intel_device_info intel_broadwell_gt3m_info = {
	.gen = 8, .is_mobile = 1, .num_pipes = 3,
	.need_gfx_hws = 1, .has_hotplug = 1,
	.ring_mask = RENDER_RING | BSD_RING | BLT_RING | VEBOX_RING | BSD2_RING,
	.has_llc = 1,
	.has_ddi = 1,
	.has_fpga_dbg = 1,
	.has_fbc = 1,
	GEN_DEFAULT_PIPEOFFSETS,
	IVB_CURSOR_OFFSETS,
};

static const struct intel_device_info intel_cherryview_info = {
	.is_preliminary = 1,
	.gen = 8, .num_pipes = 3,
	.need_gfx_hws = 1, .has_hotplug = 1,
	.ring_mask = RENDER_RING | BSD_RING | BLT_RING | VEBOX_RING,
	.is_valleyview = 1,
	.display_mmio_offset = VLV_DISPLAY_BASE,
	GEN_CHV_PIPEOFFSETS,
	CURSOR_OFFSETS,
};

/*
 * Make sure any device matches here are from most specific to most
 * general.  For example, since the Quanta match is based on the subsystem
 * and subvendor IDs, we need it to come before the more general IVB
 * PCI ID matches, otherwise we'll use the wrong info struct above.
 */
#define INTEL_PCI_IDS \
	INTEL_I830_IDS(&intel_i830_info),	\
	INTEL_I845G_IDS(&intel_845g_info),	\
	INTEL_I85X_IDS(&intel_i85x_info),	\
	INTEL_I865G_IDS(&intel_i865g_info),	\
	INTEL_I915G_IDS(&intel_i915g_info),	\
	INTEL_I915GM_IDS(&intel_i915gm_info),	\
	INTEL_I945G_IDS(&intel_i945g_info),	\
	INTEL_I945GM_IDS(&intel_i945gm_info),	\
	INTEL_I965G_IDS(&intel_i965g_info),	\
	INTEL_G33_IDS(&intel_g33_info),		\
	INTEL_I965GM_IDS(&intel_i965gm_info),	\
	INTEL_GM45_IDS(&intel_gm45_info), 	\
	INTEL_G45_IDS(&intel_g45_info), 	\
	INTEL_PINEVIEW_IDS(&intel_pineview_info),	\
	INTEL_IRONLAKE_D_IDS(&intel_ironlake_d_info),	\
	INTEL_IRONLAKE_M_IDS(&intel_ironlake_m_info),	\
	INTEL_SNB_D_IDS(&intel_sandybridge_d_info),	\
	INTEL_SNB_M_IDS(&intel_sandybridge_m_info),	\
	INTEL_IVB_Q_IDS(&intel_ivybridge_q_info), /* must be first IVB */ \
	INTEL_IVB_M_IDS(&intel_ivybridge_m_info),	\
	INTEL_IVB_D_IDS(&intel_ivybridge_d_info),	\
	INTEL_HSW_D_IDS(&intel_haswell_d_info), \
	INTEL_HSW_M_IDS(&intel_haswell_m_info), \
	INTEL_VLV_M_IDS(&intel_valleyview_m_info),	\
	INTEL_VLV_D_IDS(&intel_valleyview_d_info),	\
	INTEL_BDW_GT12M_IDS(&intel_broadwell_m_info),	\
	INTEL_BDW_GT12D_IDS(&intel_broadwell_d_info),	\
	INTEL_BDW_GT3M_IDS(&intel_broadwell_gt3m_info),	\
	INTEL_BDW_GT3D_IDS(&intel_broadwell_gt3d_info), \
	INTEL_CHV_IDS(&intel_cherryview_info)

static const struct pci_device_id pciidlist[] = {		/* aka */
	INTEL_PCI_IDS,
>>>>>>> v3.18
	{0, 0, 0}
};

#if defined(CONFIG_DRM_I915_KMS)
MODULE_DEVICE_TABLE(pci, pciidlist);
#endif

void intel_detect_pch(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
<<<<<<< HEAD
	struct pci_dev *pch;
=======
	struct pci_dev *pch = NULL;
>>>>>>> v3.18

	/* In all current cases, num_pipes is equivalent to the PCH_NOP setting
	 * (which really amounts to a PCH but no South Display).
	 */
	if (INTEL_INFO(dev)->num_pipes == 0) {
		dev_priv->pch_type = PCH_NOP;
<<<<<<< HEAD
		dev_priv->num_pch_pll = 0;
=======
>>>>>>> v3.18
		return;
	}

	/*
	 * The reason to probe ISA bridge instead of Dev31:Fun0 is to
	 * make graphics device passthrough work easy for VMM, that only
	 * need to expose ISA bridge to let driver know the real hardware
	 * underneath. This is a requirement from virtualization team.
<<<<<<< HEAD
	 */
	pch = pci_get_class(PCI_CLASS_BRIDGE_ISA << 8, NULL);
	if (pch) {
		if (pch->vendor == PCI_VENDOR_ID_INTEL) {
			unsigned short id;
			id = pch->device & INTEL_PCH_DEVICE_ID_MASK;
=======
	 *
	 * In some virtualized environments (e.g. XEN), there is irrelevant
	 * ISA bridge in the system. To work reliably, we should scan trhough
	 * all the ISA bridge devices and check for the first match, instead
	 * of only checking the first one.
	 */
	while ((pch = pci_get_class(PCI_CLASS_BRIDGE_ISA << 8, pch))) {
		if (pch->vendor == PCI_VENDOR_ID_INTEL) {
			unsigned short id = pch->device & INTEL_PCH_DEVICE_ID_MASK;
>>>>>>> v3.18
			dev_priv->pch_id = id;

			if (id == INTEL_PCH_IBX_DEVICE_ID_TYPE) {
				dev_priv->pch_type = PCH_IBX;
<<<<<<< HEAD
				dev_priv->num_pch_pll = 2;
=======
>>>>>>> v3.18
				DRM_DEBUG_KMS("Found Ibex Peak PCH\n");
				WARN_ON(!IS_GEN5(dev));
			} else if (id == INTEL_PCH_CPT_DEVICE_ID_TYPE) {
				dev_priv->pch_type = PCH_CPT;
<<<<<<< HEAD
				dev_priv->num_pch_pll = 2;
=======
>>>>>>> v3.18
				DRM_DEBUG_KMS("Found CougarPoint PCH\n");
				WARN_ON(!(IS_GEN6(dev) || IS_IVYBRIDGE(dev)));
			} else if (id == INTEL_PCH_PPT_DEVICE_ID_TYPE) {
				/* PantherPoint is CPT compatible */
				dev_priv->pch_type = PCH_CPT;
<<<<<<< HEAD
				dev_priv->num_pch_pll = 2;
				DRM_DEBUG_KMS("Found PatherPoint PCH\n");
				WARN_ON(!(IS_GEN6(dev) || IS_IVYBRIDGE(dev)));
			} else if (id == INTEL_PCH_LPT_DEVICE_ID_TYPE) {
				dev_priv->pch_type = PCH_LPT;
				dev_priv->num_pch_pll = 0;
				DRM_DEBUG_KMS("Found LynxPoint PCH\n");
				WARN_ON(!IS_HASWELL(dev));
				WARN_ON(IS_ULT(dev));
			} else if (id == INTEL_PCH_LPT_LP_DEVICE_ID_TYPE) {
				dev_priv->pch_type = PCH_LPT;
				dev_priv->num_pch_pll = 0;
				DRM_DEBUG_KMS("Found LynxPoint LP PCH\n");
				WARN_ON(!IS_HASWELL(dev));
				WARN_ON(!IS_ULT(dev));
			}
			BUG_ON(dev_priv->num_pch_pll > I915_NUM_PLLS);
		}
		pci_dev_put(pch);
	}
=======
				DRM_DEBUG_KMS("Found PantherPoint PCH\n");
				WARN_ON(!(IS_GEN6(dev) || IS_IVYBRIDGE(dev)));
			} else if (id == INTEL_PCH_LPT_DEVICE_ID_TYPE) {
				dev_priv->pch_type = PCH_LPT;
				DRM_DEBUG_KMS("Found LynxPoint PCH\n");
				WARN_ON(!IS_HASWELL(dev));
				WARN_ON(IS_ULT(dev));
			} else if (IS_BROADWELL(dev)) {
				dev_priv->pch_type = PCH_LPT;
				dev_priv->pch_id =
					INTEL_PCH_LPT_LP_DEVICE_ID_TYPE;
				DRM_DEBUG_KMS("This is Broadwell, assuming "
					      "LynxPoint LP PCH\n");
			} else if (id == INTEL_PCH_LPT_LP_DEVICE_ID_TYPE) {
				dev_priv->pch_type = PCH_LPT;
				DRM_DEBUG_KMS("Found LynxPoint LP PCH\n");
				WARN_ON(!IS_HASWELL(dev));
				WARN_ON(!IS_ULT(dev));
			} else
				continue;

			break;
		}
	}
	if (!pch)
		DRM_DEBUG_KMS("No PCH found.\n");

	pci_dev_put(pch);
>>>>>>> v3.18
}

bool i915_semaphore_is_enabled(struct drm_device *dev)
{
	if (INTEL_INFO(dev)->gen < 6)
<<<<<<< HEAD
		return 0;

	if (i915_semaphores >= 0)
		return i915_semaphores;
=======
		return false;

	if (i915.semaphores >= 0)
		return i915.semaphores;

	/* TODO: make semaphores and Execlists play nicely together */
	if (i915.enable_execlists)
		return false;

	/* Until we get further testing... */
	if (IS_GEN8(dev))
		return false;
>>>>>>> v3.18

#ifdef CONFIG_INTEL_IOMMU
	/* Enable semaphores on SNB when IO remapping is off */
	if (INTEL_INFO(dev)->gen == 6 && intel_iommu_gfx_mapped)
		return false;
#endif

<<<<<<< HEAD
	return 1;
}

=======
	return true;
}

void intel_hpd_cancel_work(struct drm_i915_private *dev_priv)
{
	spin_lock_irq(&dev_priv->irq_lock);

	dev_priv->long_hpd_port_mask = 0;
	dev_priv->short_hpd_port_mask = 0;
	dev_priv->hpd_event_bits = 0;

	spin_unlock_irq(&dev_priv->irq_lock);

	cancel_work_sync(&dev_priv->dig_port_work);
	cancel_work_sync(&dev_priv->hotplug_work);
	cancel_delayed_work_sync(&dev_priv->hotplug_reenable_work);
}

static void intel_suspend_encoders(struct drm_i915_private *dev_priv)
{
	struct drm_device *dev = dev_priv->dev;
	struct drm_encoder *encoder;

	drm_modeset_lock_all(dev);
	list_for_each_entry(encoder, &dev->mode_config.encoder_list, head) {
		struct intel_encoder *intel_encoder = to_intel_encoder(encoder);

		if (intel_encoder->suspend)
			intel_encoder->suspend(intel_encoder);
	}
	drm_modeset_unlock_all(dev);
}

static int intel_suspend_complete(struct drm_i915_private *dev_priv);
static int intel_resume_prepare(struct drm_i915_private *dev_priv,
				bool rpm_resume);

>>>>>>> v3.18
static int i915_drm_freeze(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_crtc *crtc;
<<<<<<< HEAD
=======
	pci_power_t opregion_target_state;
>>>>>>> v3.18

	/* ignore lid events during suspend */
	mutex_lock(&dev_priv->modeset_restore_lock);
	dev_priv->modeset_restore = MODESET_SUSPENDED;
	mutex_unlock(&dev_priv->modeset_restore_lock);

<<<<<<< HEAD
	intel_set_power_well(dev, true);
=======
	/* We do a lot of poking in a lot of registers, make sure they work
	 * properly. */
	intel_display_set_init_power(dev_priv, true);
>>>>>>> v3.18

	drm_kms_helper_poll_disable(dev);

	pci_save_state(dev->pdev);

	/* If KMS is active, we do the leavevt stuff here */
	if (drm_core_check_feature(dev, DRIVER_MODESET)) {
<<<<<<< HEAD
		int error = i915_gem_idle(dev);
=======
		int error;

		error = i915_gem_suspend(dev);
>>>>>>> v3.18
		if (error) {
			dev_err(&dev->pdev->dev,
				"GEM idle failed, resume might fail\n");
			return error;
		}

<<<<<<< HEAD
		cancel_delayed_work_sync(&dev_priv->rps.delayed_resume_work);

		drm_irq_uninstall(dev);
		dev_priv->enable_hotplug_processing = false;
		/*
		 * Disable CRTCs directly since we want to preserve sw state
		 * for _thaw.
		 */
		list_for_each_entry(crtc, &dev->mode_config.crtc_list, head)
			dev_priv->display.crtc_disable(crtc);
	}

	i915_save_state(dev);

	intel_opregion_fini(dev);

	console_lock();
	intel_fbdev_set_suspend(dev, 1);
	console_unlock();
=======
		/*
		 * Disable CRTCs directly since we want to preserve sw state
		 * for _thaw. Also, power gate the CRTC power wells.
		 */
		drm_modeset_lock_all(dev);
		for_each_crtc(dev, crtc)
			intel_crtc_control(crtc, false);
		drm_modeset_unlock_all(dev);

		intel_dp_mst_suspend(dev);

		flush_delayed_work(&dev_priv->rps.delayed_resume_work);

		intel_runtime_pm_disable_interrupts(dev);
		intel_hpd_cancel_work(dev_priv);

		intel_suspend_encoders(dev_priv);

		intel_suspend_gt_powersave(dev);

		intel_modeset_suspend_hw(dev);
	}

	i915_gem_suspend_gtt_mappings(dev);

	i915_save_state(dev);

	opregion_target_state = PCI_D3cold;
#if IS_ENABLED(CONFIG_ACPI_SLEEP)
	if (acpi_target_system_state() < ACPI_STATE_S3)
		opregion_target_state = PCI_D1;
#endif
	intel_opregion_notify_adapter(dev, opregion_target_state);

	intel_uncore_forcewake_reset(dev, false);
	intel_opregion_fini(dev);

	intel_fbdev_set_suspend(dev, FBINFO_STATE_SUSPENDED, true);

	dev_priv->suspend_count++;

	intel_display_set_init_power(dev_priv, false);
>>>>>>> v3.18

	return 0;
}

int i915_suspend(struct drm_device *dev, pm_message_t state)
{
	int error;

	if (!dev || !dev->dev_private) {
		DRM_ERROR("dev: %p\n", dev);
		DRM_ERROR("DRM not initialized, aborting suspend.\n");
		return -ENODEV;
	}

	if (state.event == PM_EVENT_PRETHAW)
		return 0;


	if (dev->switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

	error = i915_drm_freeze(dev);
	if (error)
		return error;

	if (state.event == PM_EVENT_SUSPEND) {
		/* Shut down the device */
		pci_disable_device(dev->pdev);
		pci_set_power_state(dev->pdev, PCI_D3hot);
	}

	return 0;
}

<<<<<<< HEAD
void intel_console_resume(struct work_struct *work)
{
	struct drm_i915_private *dev_priv =
		container_of(work, struct drm_i915_private,
			     console_resume_work);
	struct drm_device *dev = dev_priv->dev;

	console_lock();
	intel_fbdev_set_suspend(dev, 0);
	console_unlock();
}

static void intel_resume_hotplug(struct drm_device *dev)
{
	struct drm_mode_config *mode_config = &dev->mode_config;
	struct intel_encoder *encoder;

	mutex_lock(&mode_config->mutex);
	DRM_DEBUG_KMS("running encoder hotplug functions\n");

	list_for_each_entry(encoder, &mode_config->encoder_list, base.head)
		if (encoder->hot_plug)
			encoder->hot_plug(encoder);

	mutex_unlock(&mode_config->mutex);

	/* Just fire off a uevent and let userspace tell us what to do */
	drm_helper_hpd_irq_event(dev);
}

static int __i915_drm_thaw(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int error = 0;
=======
static int i915_drm_thaw_early(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int ret;

	ret = intel_resume_prepare(dev_priv, false);
	if (ret)
		DRM_ERROR("Resume prepare failed: %d,Continuing resume\n", ret);

	intel_uncore_early_sanitize(dev, true);
	intel_uncore_sanitize(dev);
	intel_power_domains_init_hw(dev_priv);

	return ret;
}

static int __i915_drm_thaw(struct drm_device *dev, bool restore_gtt_mappings)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (drm_core_check_feature(dev, DRIVER_MODESET) &&
	    restore_gtt_mappings) {
		mutex_lock(&dev->struct_mutex);
		i915_gem_restore_gtt_mappings(dev);
		mutex_unlock(&dev->struct_mutex);
	}
>>>>>>> v3.18

	i915_restore_state(dev);
	intel_opregion_setup(dev);

	/* KMS EnterVT equivalent */
	if (drm_core_check_feature(dev, DRIVER_MODESET)) {
		intel_init_pch_refclk(dev);
<<<<<<< HEAD

		mutex_lock(&dev->struct_mutex);
		dev_priv->mm.suspended = 0;

		error = i915_gem_init_hw(dev);
		mutex_unlock(&dev->struct_mutex);

		/* We need working interrupts for modeset enabling ... */
		drm_irq_install(dev);

		intel_modeset_init_hw(dev);

=======
		drm_mode_config_reset(dev);

		mutex_lock(&dev->struct_mutex);
		if (i915_gem_init_hw(dev)) {
			DRM_ERROR("failed to re-initialize GPU, declaring wedged!\n");
			atomic_set_mask(I915_WEDGED, &dev_priv->gpu_error.reset_counter);
		}
		mutex_unlock(&dev->struct_mutex);

		intel_runtime_pm_restore_interrupts(dev);

		intel_modeset_init_hw(dev);

		{
			unsigned long irqflags;
			spin_lock_irqsave(&dev_priv->irq_lock, irqflags);
			if (dev_priv->display.hpd_irq_setup)
				dev_priv->display.hpd_irq_setup(dev);
			spin_unlock_irqrestore(&dev_priv->irq_lock, irqflags);
		}

		intel_dp_mst_resume(dev);
>>>>>>> v3.18
		drm_modeset_lock_all(dev);
		intel_modeset_setup_hw_state(dev, true);
		drm_modeset_unlock_all(dev);

		/*
		 * ... but also need to make sure that hotplug processing
		 * doesn't cause havoc. Like in the driver load code we don't
		 * bother with the tiny race here where we might loose hotplug
		 * notifications.
		 * */
		intel_hpd_init(dev);
<<<<<<< HEAD
		dev_priv->enable_hotplug_processing = true;
		/* Config may have changed between suspend and resume */
		intel_resume_hotplug(dev);
=======
		/* Config may have changed between suspend and resume */
		drm_helper_hpd_irq_event(dev);
>>>>>>> v3.18
	}

	intel_opregion_init(dev);

<<<<<<< HEAD
	/*
	 * The console lock can be pretty contented on resume due
	 * to all the printk activity.  Try to keep it out of the hot
	 * path of resume if possible.
	 */
	if (console_trylock()) {
		intel_fbdev_set_suspend(dev, 0);
		console_unlock();
	} else {
		schedule_work(&dev_priv->console_resume_work);
	}
=======
	intel_fbdev_set_suspend(dev, FBINFO_STATE_RUNNING, false);
>>>>>>> v3.18

	mutex_lock(&dev_priv->modeset_restore_lock);
	dev_priv->modeset_restore = MODESET_DONE;
	mutex_unlock(&dev_priv->modeset_restore_lock);
<<<<<<< HEAD
	return error;
=======

	intel_opregion_notify_adapter(dev, PCI_D0);

	return 0;
>>>>>>> v3.18
}

static int i915_drm_thaw(struct drm_device *dev)
{
<<<<<<< HEAD
	int error = 0;

	intel_gt_sanitize(dev);

	if (drm_core_check_feature(dev, DRIVER_MODESET)) {
		mutex_lock(&dev->struct_mutex);
		i915_gem_restore_gtt_mappings(dev);
		mutex_unlock(&dev->struct_mutex);
	}

	__i915_drm_thaw(dev);

	return error;
}

int i915_resume(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int ret;

	if (dev->switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

=======
	if (drm_core_check_feature(dev, DRIVER_MODESET))
		i915_check_and_clear_faults(dev);

	return __i915_drm_thaw(dev, true);
}

static int i915_resume_early(struct drm_device *dev)
{
	if (dev->switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

	/*
	 * We have a resume ordering issue with the snd-hda driver also
	 * requiring our device to be power up. Due to the lack of a
	 * parent/child relationship we currently solve this with an early
	 * resume hook.
	 *
	 * FIXME: This should be solved with a special hdmi sink device or
	 * similar so that power domains can be employed.
	 */
>>>>>>> v3.18
	if (pci_enable_device(dev->pdev))
		return -EIO;

	pci_set_master(dev->pdev);

<<<<<<< HEAD
	intel_gt_sanitize(dev);

	/*
	 * Platforms with opregion should have sane BIOS, older ones (gen3 and
	 * earlier) need this since the BIOS might clear all our scratch PTEs.
	 */
	if (drm_core_check_feature(dev, DRIVER_MODESET) &&
	    !dev_priv->opregion.header) {
		mutex_lock(&dev->struct_mutex);
		i915_gem_restore_gtt_mappings(dev);
		mutex_unlock(&dev->struct_mutex);
	}

	ret = __i915_drm_thaw(dev);
	if (ret)
		return ret;

	drm_kms_helper_poll_enable(dev);
	return 0;
}

static int i8xx_do_reset(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (IS_I85X(dev))
		return -ENODEV;

	I915_WRITE(D_STATE, I915_READ(D_STATE) | DSTATE_GFX_RESET_I830);
	POSTING_READ(D_STATE);

	if (IS_I830(dev) || IS_845G(dev)) {
		I915_WRITE(DEBUG_RESET_I830,
			   DEBUG_RESET_DISPLAY |
			   DEBUG_RESET_RENDER |
			   DEBUG_RESET_FULL);
		POSTING_READ(DEBUG_RESET_I830);
		msleep(1);

		I915_WRITE(DEBUG_RESET_I830, 0);
		POSTING_READ(DEBUG_RESET_I830);
	}

	msleep(1);

	I915_WRITE(D_STATE, I915_READ(D_STATE) & ~DSTATE_GFX_RESET_I830);
	POSTING_READ(D_STATE);

	return 0;
}

static int i965_reset_complete(struct drm_device *dev)
{
	u8 gdrst;
	pci_read_config_byte(dev->pdev, I965_GDRST, &gdrst);
	return (gdrst & GRDOM_RESET_ENABLE) == 0;
}

static int i965_do_reset(struct drm_device *dev)
{
	int ret;
	u8 gdrst;

	/*
	 * Set the domains we want to reset (GRDOM/bits 2 and 3) as
	 * well as the reset bit (GR/bit 0).  Setting the GR bit
	 * triggers the reset; when done, the hardware will clear it.
	 */
	pci_read_config_byte(dev->pdev, I965_GDRST, &gdrst);
	pci_write_config_byte(dev->pdev, I965_GDRST,
			      gdrst | GRDOM_RENDER |
			      GRDOM_RESET_ENABLE);
	ret =  wait_for(i965_reset_complete(dev), 500);
	if (ret)
		return ret;

	/* We can't reset render&media without also resetting display ... */
	pci_read_config_byte(dev->pdev, I965_GDRST, &gdrst);
	pci_write_config_byte(dev->pdev, I965_GDRST,
			      gdrst | GRDOM_MEDIA |
			      GRDOM_RESET_ENABLE);

	return wait_for(i965_reset_complete(dev), 500);
}

static int ironlake_do_reset(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	u32 gdrst;
	int ret;

	gdrst = I915_READ(MCHBAR_MIRROR_BASE + ILK_GDSR);
	gdrst &= ~GRDOM_MASK;
	I915_WRITE(MCHBAR_MIRROR_BASE + ILK_GDSR,
		   gdrst | GRDOM_RENDER | GRDOM_RESET_ENABLE);
	ret = wait_for(I915_READ(MCHBAR_MIRROR_BASE + ILK_GDSR) & 0x1, 500);
	if (ret)
		return ret;

	/* We can't reset render&media without also resetting display ... */
	gdrst = I915_READ(MCHBAR_MIRROR_BASE + ILK_GDSR);
	gdrst &= ~GRDOM_MASK;
	I915_WRITE(MCHBAR_MIRROR_BASE + ILK_GDSR,
		   gdrst | GRDOM_MEDIA | GRDOM_RESET_ENABLE);
	return wait_for(I915_READ(MCHBAR_MIRROR_BASE + ILK_GDSR) & 0x1, 500);
}

static int gen6_do_reset(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int	ret;
	unsigned long irqflags;

	/* Hold gt_lock across reset to prevent any register access
	 * with forcewake not set correctly
	 */
	spin_lock_irqsave(&dev_priv->gt_lock, irqflags);

	/* Reset the chip */

	/* GEN6_GDRST is not in the gt power well, no need to check
	 * for fifo space for the write or forcewake the chip for
	 * the read
	 */
	I915_WRITE_NOTRACE(GEN6_GDRST, GEN6_GRDOM_FULL);

	/* Spin waiting for the device to ack the reset request */
	ret = wait_for((I915_READ_NOTRACE(GEN6_GDRST) & GEN6_GRDOM_FULL) == 0, 500);

	/* If reset with a user forcewake, try to restore, otherwise turn it off */
	if (dev_priv->forcewake_count)
		dev_priv->gt.force_wake_get(dev_priv);
	else
		dev_priv->gt.force_wake_put(dev_priv);

	/* Restore fifo count */
	dev_priv->gt_fifo_count = I915_READ_NOTRACE(GT_FIFO_FREE_ENTRIES);

	spin_unlock_irqrestore(&dev_priv->gt_lock, irqflags);
	return ret;
}

int intel_gpu_reset(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int ret = -ENODEV;

	switch (INTEL_INFO(dev)->gen) {
	case 7:
	case 6:
		ret = gen6_do_reset(dev);
		break;
	case 5:
		ret = ironlake_do_reset(dev);
		break;
	case 4:
		ret = i965_do_reset(dev);
		break;
	case 2:
		ret = i8xx_do_reset(dev);
		break;
	}

	/* Also reset the gpu hangman. */
	if (dev_priv->gpu_error.stop_rings) {
		DRM_INFO("Simulated gpu hang, resetting stop_rings\n");
		dev_priv->gpu_error.stop_rings = 0;
		if (ret == -ENODEV) {
			DRM_ERROR("Reset not implemented, but ignoring "
				  "error for simulated gpu hangs\n");
			ret = 0;
		}
	}

	return ret;
=======
	return i915_drm_thaw_early(dev);
}

int i915_resume(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int ret;

	/*
	 * Platforms with opregion should have sane BIOS, older ones (gen3 and
	 * earlier) need to restore the GTT mappings since the BIOS might clear
	 * all our scratch PTEs.
	 */
	ret = __i915_drm_thaw(dev, !dev_priv->opregion.header);
	if (ret)
		return ret;

	drm_kms_helper_poll_enable(dev);
	return 0;
}

static int i915_resume_legacy(struct drm_device *dev)
{
	i915_resume_early(dev);
	i915_resume(dev);

	return 0;
>>>>>>> v3.18
}

/**
 * i915_reset - reset chip after a hang
 * @dev: drm device to reset
 *
 * Reset the chip.  Useful if a hang is detected. Returns zero on successful
 * reset or otherwise an error code.
 *
 * Procedure is fairly simple:
 *   - reset the chip using the reset reg
 *   - re-init context state
 *   - re-init hardware status page
 *   - re-init ring buffer
 *   - re-init interrupt state
 *   - re-init display
 */
int i915_reset(struct drm_device *dev)
{
<<<<<<< HEAD
	drm_i915_private_t *dev_priv = dev->dev_private;
	int ret;

	if (!i915_try_reset)
=======
	struct drm_i915_private *dev_priv = dev->dev_private;
	bool simulated;
	int ret;

	if (!i915.reset)
>>>>>>> v3.18
		return 0;

	mutex_lock(&dev->struct_mutex);

	i915_gem_reset(dev);

<<<<<<< HEAD
	ret = -ENODEV;
	if (get_seconds() - dev_priv->gpu_error.last_reset < 5)
		DRM_ERROR("GPU hanging too fast, declaring wedged!\n");
	else
		ret = intel_gpu_reset(dev);

	dev_priv->gpu_error.last_reset = get_seconds();
	if (ret) {
		DRM_ERROR("Failed to reset chip.\n");
=======
	simulated = dev_priv->gpu_error.stop_rings != 0;

	ret = intel_gpu_reset(dev);

	/* Also reset the gpu hangman. */
	if (simulated) {
		DRM_INFO("Simulated gpu hang, resetting stop_rings\n");
		dev_priv->gpu_error.stop_rings = 0;
		if (ret == -ENODEV) {
			DRM_INFO("Reset not implemented, but ignoring "
				 "error for simulated gpu hangs\n");
			ret = 0;
		}
	}

	if (ret) {
		DRM_ERROR("Failed to reset chip: %i\n", ret);
>>>>>>> v3.18
		mutex_unlock(&dev->struct_mutex);
		return ret;
	}

	/* Ok, now get things going again... */

	/*
	 * Everything depends on having the GTT running, so we need to start
	 * there.  Fortunately we don't need to do this unless we reset the
	 * chip at a PCI level.
	 *
	 * Next we need to restore the context, but we don't use those
	 * yet either...
	 *
	 * Ring buffer needs to be re-initialized in the KMS case, or if X
	 * was running at the time of the reset (i.e. we weren't VT
	 * switched away).
	 */
	if (drm_core_check_feature(dev, DRIVER_MODESET) ||
<<<<<<< HEAD
			!dev_priv->mm.suspended) {
		struct intel_ring_buffer *ring;
		int i;

		dev_priv->mm.suspended = 0;

		i915_gem_init_swizzling(dev);

		for_each_ring(ring, dev_priv, i)
			ring->init(ring);

		i915_gem_context_init(dev);
		if (dev_priv->mm.aliasing_ppgtt) {
			ret = dev_priv->mm.aliasing_ppgtt->enable(dev);
			if (ret)
				i915_gem_cleanup_aliasing_ppgtt(dev);
		}

		/*
		 * It would make sense to re-init all the other hw state, at
		 * least the rps/rc6/emon init done within modeset_init_hw. For
		 * some unknown reason, this blows up my ilk, so don't.
		 */

		mutex_unlock(&dev->struct_mutex);

		drm_irq_uninstall(dev);
		drm_irq_install(dev);
		intel_hpd_init(dev);
=======
			!dev_priv->ums.mm_suspended) {
		dev_priv->ums.mm_suspended = 0;

		/* Used to prevent gem_check_wedged returning -EAGAIN during gpu reset */
		dev_priv->gpu_error.reload_in_reset = true;

		ret = i915_gem_init_hw(dev);

		dev_priv->gpu_error.reload_in_reset = false;

		mutex_unlock(&dev->struct_mutex);
		if (ret) {
			DRM_ERROR("Failed hw init on reset %d\n", ret);
			return ret;
		}

		/*
		 * FIXME: This races pretty badly against concurrent holders of
		 * ring interrupts. This is possible since we've started to drop
		 * dev->struct_mutex in select places when waiting for the gpu.
		 */

		/*
		 * rps/rc6 re-init is necessary to restore state lost after the
		 * reset and the re-install of gt irqs. Skip for ironlake per
		 * previous concerns that it doesn't respond well to some forms
		 * of re-init after reset.
		 */
		if (INTEL_INFO(dev)->gen > 5)
			intel_reset_gt_powersave(dev);
>>>>>>> v3.18
	} else {
		mutex_unlock(&dev->struct_mutex);
	}

	return 0;
}

static int i915_pci_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	struct intel_device_info *intel_info =
		(struct intel_device_info *) ent->driver_data;

<<<<<<< HEAD
	if (intel_info->is_valleyview)
		if(!i915_preliminary_hw_support) {
			DRM_ERROR("Preliminary hardware support disabled\n");
			return -ENODEV;
		}
=======
	if (IS_PRELIMINARY_HW(intel_info) && !i915.preliminary_hw_support) {
		DRM_INFO("This hardware requires preliminary hardware support.\n"
			 "See CONFIG_DRM_I915_PRELIMINARY_HW_SUPPORT, and/or modparam preliminary_hw_support\n");
		return -ENODEV;
	}
>>>>>>> v3.18

	/* Only bind to function 0 of the device. Early generations
	 * used function 1 as a placeholder for multi-head. This causes
	 * us confusion instead, especially on the systems where both
	 * functions have the same PCI-ID!
	 */
	if (PCI_FUNC(pdev->devfn))
		return -ENODEV;

<<<<<<< HEAD
	/* We've managed to ship a kms-enabled ddx that shipped with an XvMC
	 * implementation for gen3 (and only gen3) that used legacy drm maps
	 * (gasp!) to share buffers between X and the client. Hence we need to
	 * keep around the fake agp stuff for gen3, even when kms is enabled. */
	if (intel_info->gen != 3) {
		driver.driver_features &=
			~(DRIVER_USE_AGP | DRIVER_REQUIRE_AGP);
	} else if (!intel_agp_enabled) {
		DRM_ERROR("drm/i915 can't work without intel_agp module!\n");
		return -ENODEV;
	}
=======
	driver.driver_features &= ~(DRIVER_USE_AGP);
>>>>>>> v3.18

	return drm_get_pci_dev(pdev, ent, &driver);
}

static void
i915_pci_remove(struct pci_dev *pdev)
{
	struct drm_device *dev = pci_get_drvdata(pdev);

	drm_put_dev(dev);
}

static int i915_pm_suspend(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	struct drm_device *drm_dev = pci_get_drvdata(pdev);
<<<<<<< HEAD
	int error;
=======
>>>>>>> v3.18

	if (!drm_dev || !drm_dev->dev_private) {
		dev_err(dev, "DRM not initialized, aborting suspend.\n");
		return -ENODEV;
	}

	if (drm_dev->switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

<<<<<<< HEAD
	error = i915_drm_freeze(drm_dev);
	if (error)
		return error;

	pci_disable_device(pdev);
	pci_set_power_state(pdev, PCI_D3hot);

	return 0;
=======
	return i915_drm_freeze(drm_dev);
}

static int i915_pm_suspend_late(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	struct drm_device *drm_dev = pci_get_drvdata(pdev);
	struct drm_i915_private *dev_priv = drm_dev->dev_private;
	int ret;

	/*
	 * We have a suspedn ordering issue with the snd-hda driver also
	 * requiring our device to be power up. Due to the lack of a
	 * parent/child relationship we currently solve this with an late
	 * suspend hook.
	 *
	 * FIXME: This should be solved with a special hdmi sink device or
	 * similar so that power domains can be employed.
	 */
	if (drm_dev->switch_power_state == DRM_SWITCH_POWER_OFF)
		return 0;

	ret = intel_suspend_complete(dev_priv);

	if (ret)
		DRM_ERROR("Suspend complete failed: %d\n", ret);
	else {
		pci_disable_device(pdev);
		pci_set_power_state(pdev, PCI_D3hot);
	}

	return ret;
}

static int i915_pm_resume_early(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	struct drm_device *drm_dev = pci_get_drvdata(pdev);

	return i915_resume_early(drm_dev);
>>>>>>> v3.18
}

static int i915_pm_resume(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	struct drm_device *drm_dev = pci_get_drvdata(pdev);

	return i915_resume(drm_dev);
}

static int i915_pm_freeze(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	struct drm_device *drm_dev = pci_get_drvdata(pdev);

	if (!drm_dev || !drm_dev->dev_private) {
		dev_err(dev, "DRM not initialized, aborting suspend.\n");
		return -ENODEV;
	}

	return i915_drm_freeze(drm_dev);
}

<<<<<<< HEAD
=======
static int i915_pm_freeze_late(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	struct drm_device *drm_dev = pci_get_drvdata(pdev);
	struct drm_i915_private *dev_priv = drm_dev->dev_private;

	return intel_suspend_complete(dev_priv);
}

static int i915_pm_thaw_early(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	struct drm_device *drm_dev = pci_get_drvdata(pdev);

	return i915_drm_thaw_early(drm_dev);
}

>>>>>>> v3.18
static int i915_pm_thaw(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	struct drm_device *drm_dev = pci_get_drvdata(pdev);

	return i915_drm_thaw(drm_dev);
}

static int i915_pm_poweroff(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	struct drm_device *drm_dev = pci_get_drvdata(pdev);

	return i915_drm_freeze(drm_dev);
}

<<<<<<< HEAD
static const struct dev_pm_ops i915_pm_ops = {
	.suspend = i915_pm_suspend,
	.resume = i915_pm_resume,
	.freeze = i915_pm_freeze,
	.thaw = i915_pm_thaw,
	.poweroff = i915_pm_poweroff,
	.restore = i915_pm_resume,
=======
static int hsw_suspend_complete(struct drm_i915_private *dev_priv)
{
	hsw_enable_pc8(dev_priv);

	return 0;
}

static int snb_resume_prepare(struct drm_i915_private *dev_priv,
				bool rpm_resume)
{
	struct drm_device *dev = dev_priv->dev;

	if (rpm_resume)
		intel_init_pch_refclk(dev);

	return 0;
}

static int hsw_resume_prepare(struct drm_i915_private *dev_priv,
				bool rpm_resume)
{
	hsw_disable_pc8(dev_priv);

	return 0;
}

/*
 * Save all Gunit registers that may be lost after a D3 and a subsequent
 * S0i[R123] transition. The list of registers needing a save/restore is
 * defined in the VLV2_S0IXRegs document. This documents marks all Gunit
 * registers in the following way:
 * - Driver: saved/restored by the driver
 * - Punit : saved/restored by the Punit firmware
 * - No, w/o marking: no need to save/restore, since the register is R/O or
 *                    used internally by the HW in a way that doesn't depend
 *                    keeping the content across a suspend/resume.
 * - Debug : used for debugging
 *
 * We save/restore all registers marked with 'Driver', with the following
 * exceptions:
 * - Registers out of use, including also registers marked with 'Debug'.
 *   These have no effect on the driver's operation, so we don't save/restore
 *   them to reduce the overhead.
 * - Registers that are fully setup by an initialization function called from
 *   the resume path. For example many clock gating and RPS/RC6 registers.
 * - Registers that provide the right functionality with their reset defaults.
 *
 * TODO: Except for registers that based on the above 3 criteria can be safely
 * ignored, we save/restore all others, practically treating the HW context as
 * a black-box for the driver. Further investigation is needed to reduce the
 * saved/restored registers even further, by following the same 3 criteria.
 */
static void vlv_save_gunit_s0ix_state(struct drm_i915_private *dev_priv)
{
	struct vlv_s0ix_state *s = &dev_priv->vlv_s0ix_state;
	int i;

	/* GAM 0x4000-0x4770 */
	s->wr_watermark		= I915_READ(GEN7_WR_WATERMARK);
	s->gfx_prio_ctrl	= I915_READ(GEN7_GFX_PRIO_CTRL);
	s->arb_mode		= I915_READ(ARB_MODE);
	s->gfx_pend_tlb0	= I915_READ(GEN7_GFX_PEND_TLB0);
	s->gfx_pend_tlb1	= I915_READ(GEN7_GFX_PEND_TLB1);

	for (i = 0; i < ARRAY_SIZE(s->lra_limits); i++)
		s->lra_limits[i] = I915_READ(GEN7_LRA_LIMITS_BASE + i * 4);

	s->media_max_req_count	= I915_READ(GEN7_MEDIA_MAX_REQ_COUNT);
	s->gfx_max_req_count	= I915_READ(GEN7_MEDIA_MAX_REQ_COUNT);

	s->render_hwsp		= I915_READ(RENDER_HWS_PGA_GEN7);
	s->ecochk		= I915_READ(GAM_ECOCHK);
	s->bsd_hwsp		= I915_READ(BSD_HWS_PGA_GEN7);
	s->blt_hwsp		= I915_READ(BLT_HWS_PGA_GEN7);

	s->tlb_rd_addr		= I915_READ(GEN7_TLB_RD_ADDR);

	/* MBC 0x9024-0x91D0, 0x8500 */
	s->g3dctl		= I915_READ(VLV_G3DCTL);
	s->gsckgctl		= I915_READ(VLV_GSCKGCTL);
	s->mbctl		= I915_READ(GEN6_MBCTL);

	/* GCP 0x9400-0x9424, 0x8100-0x810C */
	s->ucgctl1		= I915_READ(GEN6_UCGCTL1);
	s->ucgctl3		= I915_READ(GEN6_UCGCTL3);
	s->rcgctl1		= I915_READ(GEN6_RCGCTL1);
	s->rcgctl2		= I915_READ(GEN6_RCGCTL2);
	s->rstctl		= I915_READ(GEN6_RSTCTL);
	s->misccpctl		= I915_READ(GEN7_MISCCPCTL);

	/* GPM 0xA000-0xAA84, 0x8000-0x80FC */
	s->gfxpause		= I915_READ(GEN6_GFXPAUSE);
	s->rpdeuhwtc		= I915_READ(GEN6_RPDEUHWTC);
	s->rpdeuc		= I915_READ(GEN6_RPDEUC);
	s->ecobus		= I915_READ(ECOBUS);
	s->pwrdwnupctl		= I915_READ(VLV_PWRDWNUPCTL);
	s->rp_down_timeout	= I915_READ(GEN6_RP_DOWN_TIMEOUT);
	s->rp_deucsw		= I915_READ(GEN6_RPDEUCSW);
	s->rcubmabdtmr		= I915_READ(GEN6_RCUBMABDTMR);
	s->rcedata		= I915_READ(VLV_RCEDATA);
	s->spare2gh		= I915_READ(VLV_SPAREG2H);

	/* Display CZ domain, 0x4400C-0x4402C, 0x4F000-0x4F11F */
	s->gt_imr		= I915_READ(GTIMR);
	s->gt_ier		= I915_READ(GTIER);
	s->pm_imr		= I915_READ(GEN6_PMIMR);
	s->pm_ier		= I915_READ(GEN6_PMIER);

	for (i = 0; i < ARRAY_SIZE(s->gt_scratch); i++)
		s->gt_scratch[i] = I915_READ(GEN7_GT_SCRATCH_BASE + i * 4);

	/* GT SA CZ domain, 0x100000-0x138124 */
	s->tilectl		= I915_READ(TILECTL);
	s->gt_fifoctl		= I915_READ(GTFIFOCTL);
	s->gtlc_wake_ctrl	= I915_READ(VLV_GTLC_WAKE_CTRL);
	s->gtlc_survive		= I915_READ(VLV_GTLC_SURVIVABILITY_REG);
	s->pmwgicz		= I915_READ(VLV_PMWGICZ);

	/* Gunit-Display CZ domain, 0x182028-0x1821CF */
	s->gu_ctl0		= I915_READ(VLV_GU_CTL0);
	s->gu_ctl1		= I915_READ(VLV_GU_CTL1);
	s->clock_gate_dis2	= I915_READ(VLV_GUNIT_CLOCK_GATE2);

	/*
	 * Not saving any of:
	 * DFT,		0x9800-0x9EC0
	 * SARB,	0xB000-0xB1FC
	 * GAC,		0x5208-0x524C, 0x14000-0x14C000
	 * PCI CFG
	 */
}

static void vlv_restore_gunit_s0ix_state(struct drm_i915_private *dev_priv)
{
	struct vlv_s0ix_state *s = &dev_priv->vlv_s0ix_state;
	u32 val;
	int i;

	/* GAM 0x4000-0x4770 */
	I915_WRITE(GEN7_WR_WATERMARK,	s->wr_watermark);
	I915_WRITE(GEN7_GFX_PRIO_CTRL,	s->gfx_prio_ctrl);
	I915_WRITE(ARB_MODE,		s->arb_mode | (0xffff << 16));
	I915_WRITE(GEN7_GFX_PEND_TLB0,	s->gfx_pend_tlb0);
	I915_WRITE(GEN7_GFX_PEND_TLB1,	s->gfx_pend_tlb1);

	for (i = 0; i < ARRAY_SIZE(s->lra_limits); i++)
		I915_WRITE(GEN7_LRA_LIMITS_BASE + i * 4, s->lra_limits[i]);

	I915_WRITE(GEN7_MEDIA_MAX_REQ_COUNT, s->media_max_req_count);
	I915_WRITE(GEN7_MEDIA_MAX_REQ_COUNT, s->gfx_max_req_count);

	I915_WRITE(RENDER_HWS_PGA_GEN7,	s->render_hwsp);
	I915_WRITE(GAM_ECOCHK,		s->ecochk);
	I915_WRITE(BSD_HWS_PGA_GEN7,	s->bsd_hwsp);
	I915_WRITE(BLT_HWS_PGA_GEN7,	s->blt_hwsp);

	I915_WRITE(GEN7_TLB_RD_ADDR,	s->tlb_rd_addr);

	/* MBC 0x9024-0x91D0, 0x8500 */
	I915_WRITE(VLV_G3DCTL,		s->g3dctl);
	I915_WRITE(VLV_GSCKGCTL,	s->gsckgctl);
	I915_WRITE(GEN6_MBCTL,		s->mbctl);

	/* GCP 0x9400-0x9424, 0x8100-0x810C */
	I915_WRITE(GEN6_UCGCTL1,	s->ucgctl1);
	I915_WRITE(GEN6_UCGCTL3,	s->ucgctl3);
	I915_WRITE(GEN6_RCGCTL1,	s->rcgctl1);
	I915_WRITE(GEN6_RCGCTL2,	s->rcgctl2);
	I915_WRITE(GEN6_RSTCTL,		s->rstctl);
	I915_WRITE(GEN7_MISCCPCTL,	s->misccpctl);

	/* GPM 0xA000-0xAA84, 0x8000-0x80FC */
	I915_WRITE(GEN6_GFXPAUSE,	s->gfxpause);
	I915_WRITE(GEN6_RPDEUHWTC,	s->rpdeuhwtc);
	I915_WRITE(GEN6_RPDEUC,		s->rpdeuc);
	I915_WRITE(ECOBUS,		s->ecobus);
	I915_WRITE(VLV_PWRDWNUPCTL,	s->pwrdwnupctl);
	I915_WRITE(GEN6_RP_DOWN_TIMEOUT,s->rp_down_timeout);
	I915_WRITE(GEN6_RPDEUCSW,	s->rp_deucsw);
	I915_WRITE(GEN6_RCUBMABDTMR,	s->rcubmabdtmr);
	I915_WRITE(VLV_RCEDATA,		s->rcedata);
	I915_WRITE(VLV_SPAREG2H,	s->spare2gh);

	/* Display CZ domain, 0x4400C-0x4402C, 0x4F000-0x4F11F */
	I915_WRITE(GTIMR,		s->gt_imr);
	I915_WRITE(GTIER,		s->gt_ier);
	I915_WRITE(GEN6_PMIMR,		s->pm_imr);
	I915_WRITE(GEN6_PMIER,		s->pm_ier);

	for (i = 0; i < ARRAY_SIZE(s->gt_scratch); i++)
		I915_WRITE(GEN7_GT_SCRATCH_BASE + i * 4, s->gt_scratch[i]);

	/* GT SA CZ domain, 0x100000-0x138124 */
	I915_WRITE(TILECTL,			s->tilectl);
	I915_WRITE(GTFIFOCTL,			s->gt_fifoctl);
	/*
	 * Preserve the GT allow wake and GFX force clock bit, they are not
	 * be restored, as they are used to control the s0ix suspend/resume
	 * sequence by the caller.
	 */
	val = I915_READ(VLV_GTLC_WAKE_CTRL);
	val &= VLV_GTLC_ALLOWWAKEREQ;
	val |= s->gtlc_wake_ctrl & ~VLV_GTLC_ALLOWWAKEREQ;
	I915_WRITE(VLV_GTLC_WAKE_CTRL, val);

	val = I915_READ(VLV_GTLC_SURVIVABILITY_REG);
	val &= VLV_GFX_CLK_FORCE_ON_BIT;
	val |= s->gtlc_survive & ~VLV_GFX_CLK_FORCE_ON_BIT;
	I915_WRITE(VLV_GTLC_SURVIVABILITY_REG, val);

	I915_WRITE(VLV_PMWGICZ,			s->pmwgicz);

	/* Gunit-Display CZ domain, 0x182028-0x1821CF */
	I915_WRITE(VLV_GU_CTL0,			s->gu_ctl0);
	I915_WRITE(VLV_GU_CTL1,			s->gu_ctl1);
	I915_WRITE(VLV_GUNIT_CLOCK_GATE2,	s->clock_gate_dis2);
}

int vlv_force_gfx_clock(struct drm_i915_private *dev_priv, bool force_on)
{
	u32 val;
	int err;

	val = I915_READ(VLV_GTLC_SURVIVABILITY_REG);
	WARN_ON(!!(val & VLV_GFX_CLK_FORCE_ON_BIT) == force_on);

#define COND (I915_READ(VLV_GTLC_SURVIVABILITY_REG) & VLV_GFX_CLK_STATUS_BIT)
	/* Wait for a previous force-off to settle */
	if (force_on) {
		err = wait_for(!COND, 20);
		if (err) {
			DRM_ERROR("timeout waiting for GFX clock force-off (%08x)\n",
				  I915_READ(VLV_GTLC_SURVIVABILITY_REG));
			return err;
		}
	}

	val = I915_READ(VLV_GTLC_SURVIVABILITY_REG);
	val &= ~VLV_GFX_CLK_FORCE_ON_BIT;
	if (force_on)
		val |= VLV_GFX_CLK_FORCE_ON_BIT;
	I915_WRITE(VLV_GTLC_SURVIVABILITY_REG, val);

	if (!force_on)
		return 0;

	err = wait_for(COND, 20);
	if (err)
		DRM_ERROR("timeout waiting for GFX clock force-on (%08x)\n",
			  I915_READ(VLV_GTLC_SURVIVABILITY_REG));

	return err;
#undef COND
}

static int vlv_allow_gt_wake(struct drm_i915_private *dev_priv, bool allow)
{
	u32 val;
	int err = 0;

	val = I915_READ(VLV_GTLC_WAKE_CTRL);
	val &= ~VLV_GTLC_ALLOWWAKEREQ;
	if (allow)
		val |= VLV_GTLC_ALLOWWAKEREQ;
	I915_WRITE(VLV_GTLC_WAKE_CTRL, val);
	POSTING_READ(VLV_GTLC_WAKE_CTRL);

#define COND (!!(I915_READ(VLV_GTLC_PW_STATUS) & VLV_GTLC_ALLOWWAKEACK) == \
	      allow)
	err = wait_for(COND, 1);
	if (err)
		DRM_ERROR("timeout disabling GT waking\n");
	return err;
#undef COND
}

static int vlv_wait_for_gt_wells(struct drm_i915_private *dev_priv,
				 bool wait_for_on)
{
	u32 mask;
	u32 val;
	int err;

	mask = VLV_GTLC_PW_MEDIA_STATUS_MASK | VLV_GTLC_PW_RENDER_STATUS_MASK;
	val = wait_for_on ? mask : 0;
#define COND ((I915_READ(VLV_GTLC_PW_STATUS) & mask) == val)
	if (COND)
		return 0;

	DRM_DEBUG_KMS("waiting for GT wells to go %s (%08x)\n",
			wait_for_on ? "on" : "off",
			I915_READ(VLV_GTLC_PW_STATUS));

	/*
	 * RC6 transitioning can be delayed up to 2 msec (see
	 * valleyview_enable_rps), use 3 msec for safety.
	 */
	err = wait_for(COND, 3);
	if (err)
		DRM_ERROR("timeout waiting for GT wells to go %s\n",
			  wait_for_on ? "on" : "off");

	return err;
#undef COND
}

static void vlv_check_no_gt_access(struct drm_i915_private *dev_priv)
{
	if (!(I915_READ(VLV_GTLC_PW_STATUS) & VLV_GTLC_ALLOWWAKEERR))
		return;

	DRM_ERROR("GT register access while GT waking disabled\n");
	I915_WRITE(VLV_GTLC_PW_STATUS, VLV_GTLC_ALLOWWAKEERR);
}

static int vlv_suspend_complete(struct drm_i915_private *dev_priv)
{
	u32 mask;
	int err;

	/*
	 * Bspec defines the following GT well on flags as debug only, so
	 * don't treat them as hard failures.
	 */
	(void)vlv_wait_for_gt_wells(dev_priv, false);

	mask = VLV_GTLC_RENDER_CTX_EXISTS | VLV_GTLC_MEDIA_CTX_EXISTS;
	WARN_ON((I915_READ(VLV_GTLC_WAKE_CTRL) & mask) != mask);

	vlv_check_no_gt_access(dev_priv);

	err = vlv_force_gfx_clock(dev_priv, true);
	if (err)
		goto err1;

	err = vlv_allow_gt_wake(dev_priv, false);
	if (err)
		goto err2;
	vlv_save_gunit_s0ix_state(dev_priv);

	err = vlv_force_gfx_clock(dev_priv, false);
	if (err)
		goto err2;

	return 0;

err2:
	/* For safety always re-enable waking and disable gfx clock forcing */
	vlv_allow_gt_wake(dev_priv, true);
err1:
	vlv_force_gfx_clock(dev_priv, false);

	return err;
}

static int vlv_resume_prepare(struct drm_i915_private *dev_priv,
				bool rpm_resume)
{
	struct drm_device *dev = dev_priv->dev;
	int err;
	int ret;

	/*
	 * If any of the steps fail just try to continue, that's the best we
	 * can do at this point. Return the first error code (which will also
	 * leave RPM permanently disabled).
	 */
	ret = vlv_force_gfx_clock(dev_priv, true);

	vlv_restore_gunit_s0ix_state(dev_priv);

	err = vlv_allow_gt_wake(dev_priv, true);
	if (!ret)
		ret = err;

	err = vlv_force_gfx_clock(dev_priv, false);
	if (!ret)
		ret = err;

	vlv_check_no_gt_access(dev_priv);

	if (rpm_resume) {
		intel_init_clock_gating(dev);
		i915_gem_restore_fences(dev);
	}

	return ret;
}

static int intel_runtime_suspend(struct device *device)
{
	struct pci_dev *pdev = to_pci_dev(device);
	struct drm_device *dev = pci_get_drvdata(pdev);
	struct drm_i915_private *dev_priv = dev->dev_private;
	int ret;

	if (WARN_ON_ONCE(!(dev_priv->rps.enabled && intel_enable_rc6(dev))))
		return -ENODEV;

	if (WARN_ON_ONCE(!HAS_RUNTIME_PM(dev)))
		return -ENODEV;

	assert_force_wake_inactive(dev_priv);

	DRM_DEBUG_KMS("Suspending device\n");

	/*
	 * We could deadlock here in case another thread holding struct_mutex
	 * calls RPM suspend concurrently, since the RPM suspend will wait
	 * first for this RPM suspend to finish. In this case the concurrent
	 * RPM resume will be followed by its RPM suspend counterpart. Still
	 * for consistency return -EAGAIN, which will reschedule this suspend.
	 */
	if (!mutex_trylock(&dev->struct_mutex)) {
		DRM_DEBUG_KMS("device lock contention, deffering suspend\n");
		/*
		 * Bump the expiration timestamp, otherwise the suspend won't
		 * be rescheduled.
		 */
		pm_runtime_mark_last_busy(device);

		return -EAGAIN;
	}
	/*
	 * We are safe here against re-faults, since the fault handler takes
	 * an RPM reference.
	 */
	i915_gem_release_all_mmaps(dev_priv);
	mutex_unlock(&dev->struct_mutex);

	/*
	 * rps.work can't be rearmed here, since we get here only after making
	 * sure the GPU is idle and the RPS freq is set to the minimum. See
	 * intel_mark_idle().
	 */
	cancel_work_sync(&dev_priv->rps.work);
	intel_runtime_pm_disable_interrupts(dev);

	ret = intel_suspend_complete(dev_priv);
	if (ret) {
		DRM_ERROR("Runtime suspend failed, disabling it (%d)\n", ret);
		intel_runtime_pm_restore_interrupts(dev);

		return ret;
	}

	del_timer_sync(&dev_priv->gpu_error.hangcheck_timer);
	dev_priv->pm.suspended = true;

	/*
	 * FIXME: We really should find a document that references the arguments
	 * used below!
	 */
	if (IS_HASWELL(dev)) {
		/*
		 * current versions of firmware which depend on this opregion
		 * notification have repurposed the D1 definition to mean
		 * "runtime suspended" vs. what you would normally expect (D3)
		 * to distinguish it from notifications that might be sent via
		 * the suspend path.
		 */
		intel_opregion_notify_adapter(dev, PCI_D1);
	} else {
		/*
		 * On Broadwell, if we use PCI_D1 the PCH DDI ports will stop
		 * being detected, and the call we do at intel_runtime_resume()
		 * won't be able to restore them. Since PCI_D3hot matches the
		 * actual specification and appears to be working, use it. Let's
		 * assume the other non-Haswell platforms will stay the same as
		 * Broadwell.
		 */
		intel_opregion_notify_adapter(dev, PCI_D3hot);
	}

	DRM_DEBUG_KMS("Device suspended\n");
	return 0;
}

static int intel_runtime_resume(struct device *device)
{
	struct pci_dev *pdev = to_pci_dev(device);
	struct drm_device *dev = pci_get_drvdata(pdev);
	struct drm_i915_private *dev_priv = dev->dev_private;
	int ret;

	if (WARN_ON_ONCE(!HAS_RUNTIME_PM(dev)))
		return -ENODEV;

	DRM_DEBUG_KMS("Resuming device\n");

	intel_opregion_notify_adapter(dev, PCI_D0);
	dev_priv->pm.suspended = false;

	ret = intel_resume_prepare(dev_priv, true);
	/*
	 * No point of rolling back things in case of an error, as the best
	 * we can do is to hope that things will still work (and disable RPM).
	 */
	i915_gem_init_swizzling(dev);
	gen6_update_ring_freq(dev);

	intel_runtime_pm_restore_interrupts(dev);
	intel_reset_gt_powersave(dev);

	if (ret)
		DRM_ERROR("Runtime resume failed, disabling it (%d)\n", ret);
	else
		DRM_DEBUG_KMS("Device resumed\n");

	return ret;
}

/*
 * This function implements common functionality of runtime and system
 * suspend sequence.
 */
static int intel_suspend_complete(struct drm_i915_private *dev_priv)
{
	struct drm_device *dev = dev_priv->dev;
	int ret;

	if (IS_HASWELL(dev) || IS_BROADWELL(dev))
		ret = hsw_suspend_complete(dev_priv);
	else if (IS_VALLEYVIEW(dev))
		ret = vlv_suspend_complete(dev_priv);
	else
		ret = 0;

	return ret;
}

/*
 * This function implements common functionality of runtime and system
 * resume sequence. Variable rpm_resume used for implementing different
 * code paths.
 */
static int intel_resume_prepare(struct drm_i915_private *dev_priv,
				bool rpm_resume)
{
	struct drm_device *dev = dev_priv->dev;
	int ret;

	if (IS_GEN6(dev))
		ret = snb_resume_prepare(dev_priv, rpm_resume);
	else if (IS_HASWELL(dev) || IS_BROADWELL(dev))
		ret = hsw_resume_prepare(dev_priv, rpm_resume);
	else if (IS_VALLEYVIEW(dev))
		ret = vlv_resume_prepare(dev_priv, rpm_resume);
	else
		ret = 0;

	return ret;
}

static const struct dev_pm_ops i915_pm_ops = {
	.suspend = i915_pm_suspend,
	.suspend_late = i915_pm_suspend_late,
	.resume_early = i915_pm_resume_early,
	.resume = i915_pm_resume,
	.freeze = i915_pm_freeze,
	.freeze_late = i915_pm_freeze_late,
	.thaw_early = i915_pm_thaw_early,
	.thaw = i915_pm_thaw,
	.poweroff = i915_pm_poweroff,
	.restore_early = i915_pm_resume_early,
	.restore = i915_pm_resume,
	.runtime_suspend = intel_runtime_suspend,
	.runtime_resume = intel_runtime_resume,
>>>>>>> v3.18
};

static const struct vm_operations_struct i915_gem_vm_ops = {
	.fault = i915_gem_fault,
	.open = drm_gem_vm_open,
	.close = drm_gem_vm_close,
};

static const struct file_operations i915_driver_fops = {
	.owner = THIS_MODULE,
	.open = drm_open,
	.release = drm_release,
	.unlocked_ioctl = drm_ioctl,
	.mmap = drm_gem_mmap,
	.poll = drm_poll,
<<<<<<< HEAD
	.fasync = drm_fasync,
=======
>>>>>>> v3.18
	.read = drm_read,
#ifdef CONFIG_COMPAT
	.compat_ioctl = i915_compat_ioctl,
#endif
	.llseek = noop_llseek,
};

static struct drm_driver driver = {
	/* Don't use MTRRs here; the Xserver or userspace app should
	 * deal with them for Intel hardware.
	 */
	.driver_features =
<<<<<<< HEAD
	    DRIVER_USE_AGP | DRIVER_REQUIRE_AGP | /* DRIVER_USE_MTRR |*/
	    DRIVER_HAVE_IRQ | DRIVER_IRQ_SHARED | DRIVER_GEM | DRIVER_PRIME,
=======
	    DRIVER_USE_AGP |
	    DRIVER_HAVE_IRQ | DRIVER_IRQ_SHARED | DRIVER_GEM | DRIVER_PRIME |
	    DRIVER_RENDER,
>>>>>>> v3.18
	.load = i915_driver_load,
	.unload = i915_driver_unload,
	.open = i915_driver_open,
	.lastclose = i915_driver_lastclose,
	.preclose = i915_driver_preclose,
	.postclose = i915_driver_postclose,
<<<<<<< HEAD

	/* Used in place of i915_pm_ops for non-DRIVER_MODESET */
	.suspend = i915_suspend,
	.resume = i915_resume,
=======
	.set_busid = drm_pci_set_busid,

	/* Used in place of i915_pm_ops for non-DRIVER_MODESET */
	.suspend = i915_suspend,
	.resume = i915_resume_legacy,
>>>>>>> v3.18

	.device_is_agp = i915_driver_device_is_agp,
	.master_create = i915_master_create,
	.master_destroy = i915_master_destroy,
#if defined(CONFIG_DEBUG_FS)
	.debugfs_init = i915_debugfs_init,
	.debugfs_cleanup = i915_debugfs_cleanup,
#endif
<<<<<<< HEAD
	.gem_init_object = i915_gem_init_object,
=======
>>>>>>> v3.18
	.gem_free_object = i915_gem_free_object,
	.gem_vm_ops = &i915_gem_vm_ops,

	.prime_handle_to_fd = drm_gem_prime_handle_to_fd,
	.prime_fd_to_handle = drm_gem_prime_fd_to_handle,
	.gem_prime_export = i915_gem_prime_export,
	.gem_prime_import = i915_gem_prime_import,

	.dumb_create = i915_gem_dumb_create,
	.dumb_map_offset = i915_gem_mmap_gtt,
<<<<<<< HEAD
	.dumb_destroy = i915_gem_dumb_destroy,
=======
	.dumb_destroy = drm_gem_dumb_destroy,
>>>>>>> v3.18
	.ioctls = i915_ioctls,
	.fops = &i915_driver_fops,
	.name = DRIVER_NAME,
	.desc = DRIVER_DESC,
	.date = DRIVER_DATE,
	.major = DRIVER_MAJOR,
	.minor = DRIVER_MINOR,
	.patchlevel = DRIVER_PATCHLEVEL,
};

static struct pci_driver i915_pci_driver = {
	.name = DRIVER_NAME,
	.id_table = pciidlist,
	.probe = i915_pci_probe,
	.remove = i915_pci_remove,
	.driver.pm = &i915_pm_ops,
};

static int __init i915_init(void)
{
	driver.num_ioctls = i915_max_ioctl;

	/*
	 * If CONFIG_DRM_I915_KMS is set, default to KMS unless
	 * explicitly disabled with the module pararmeter.
	 *
	 * Otherwise, just follow the parameter (defaulting to off).
	 *
	 * Allow optional vga_text_mode_force boot option to override
	 * the default behavior.
	 */
#if defined(CONFIG_DRM_I915_KMS)
<<<<<<< HEAD
	if (i915_modeset != 0)
		driver.driver_features |= DRIVER_MODESET;
#endif
	if (i915_modeset == 1)
		driver.driver_features |= DRIVER_MODESET;

#ifdef CONFIG_VGA_CONSOLE
	if (vgacon_text_force() && i915_modeset == -1)
		driver.driver_features &= ~DRIVER_MODESET;
#endif

	if (!(driver.driver_features & DRIVER_MODESET))
		driver.get_vblank_timestamp = NULL;
=======
	if (i915.modeset != 0)
		driver.driver_features |= DRIVER_MODESET;
#endif
	if (i915.modeset == 1)
		driver.driver_features |= DRIVER_MODESET;

#ifdef CONFIG_VGA_CONSOLE
	if (vgacon_text_force() && i915.modeset == -1)
		driver.driver_features &= ~DRIVER_MODESET;
#endif

	if (!(driver.driver_features & DRIVER_MODESET)) {
		driver.get_vblank_timestamp = NULL;
#ifndef CONFIG_DRM_I915_UMS
		/* Silently fail loading to not upset userspace. */
		DRM_DEBUG_DRIVER("KMS and UMS disabled.\n");
		return 0;
#endif
	}
>>>>>>> v3.18

	return drm_pci_init(&driver, &i915_pci_driver);
}

static void __exit i915_exit(void)
{
<<<<<<< HEAD
=======
#ifndef CONFIG_DRM_I915_UMS
	if (!(driver.driver_features & DRIVER_MODESET))
		return; /* Never loaded a driver. */
#endif

>>>>>>> v3.18
	drm_pci_exit(&driver, &i915_pci_driver);
}

module_init(i915_init);
module_exit(i915_exit);

<<<<<<< HEAD
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL and additional rights");

/* We give fast paths for the really cool registers */
#define NEEDS_FORCE_WAKE(dev_priv, reg) \
	((HAS_FORCE_WAKE((dev_priv)->dev)) && \
	 ((reg) < 0x40000) &&            \
	 ((reg) != FORCEWAKE))
static void
ilk_dummy_write(struct drm_i915_private *dev_priv)
{
	/* WaIssueDummyWriteToWakeupFromRC6: Issue a dummy write to wake up the
	 * chip from rc6 before touching it for real. MI_MODE is masked, hence
	 * harmless to write 0 into. */
	I915_WRITE_NOTRACE(MI_MODE, 0);
}

static void
hsw_unclaimed_reg_clear(struct drm_i915_private *dev_priv, u32 reg)
{
	if (IS_HASWELL(dev_priv->dev) &&
	    (I915_READ_NOTRACE(FPGA_DBG) & FPGA_DBG_RM_NOCLAIM)) {
		DRM_ERROR("Unknown unclaimed register before writing to %x\n",
			  reg);
		I915_WRITE_NOTRACE(FPGA_DBG, FPGA_DBG_RM_NOCLAIM);
	}
}

static void
hsw_unclaimed_reg_check(struct drm_i915_private *dev_priv, u32 reg)
{
	if (IS_HASWELL(dev_priv->dev) &&
	    (I915_READ_NOTRACE(FPGA_DBG) & FPGA_DBG_RM_NOCLAIM)) {
		DRM_ERROR("Unclaimed write to %x\n", reg);
		I915_WRITE_NOTRACE(FPGA_DBG, FPGA_DBG_RM_NOCLAIM);
	}
}

#define __i915_read(x, y) \
u##x i915_read##x(struct drm_i915_private *dev_priv, u32 reg) { \
	unsigned long irqflags; \
	u##x val = 0; \
	spin_lock_irqsave(&dev_priv->gt_lock, irqflags); \
	if (IS_GEN5(dev_priv->dev)) \
		ilk_dummy_write(dev_priv); \
	if (NEEDS_FORCE_WAKE((dev_priv), (reg))) { \
		if (dev_priv->forcewake_count == 0) \
			dev_priv->gt.force_wake_get(dev_priv); \
		val = read##y(dev_priv->regs + reg); \
		if (dev_priv->forcewake_count == 0) \
			dev_priv->gt.force_wake_put(dev_priv); \
	} else { \
		val = read##y(dev_priv->regs + reg); \
	} \
	spin_unlock_irqrestore(&dev_priv->gt_lock, irqflags); \
	trace_i915_reg_rw(false, reg, val, sizeof(val)); \
	return val; \
}

__i915_read(8, b)
__i915_read(16, w)
__i915_read(32, l)
__i915_read(64, q)
#undef __i915_read

#define __i915_write(x, y) \
void i915_write##x(struct drm_i915_private *dev_priv, u32 reg, u##x val) { \
	unsigned long irqflags; \
	u32 __fifo_ret = 0; \
	trace_i915_reg_rw(true, reg, val, sizeof(val)); \
	spin_lock_irqsave(&dev_priv->gt_lock, irqflags); \
	if (NEEDS_FORCE_WAKE((dev_priv), (reg))) { \
		__fifo_ret = __gen6_gt_wait_for_fifo(dev_priv); \
	} \
	if (IS_GEN5(dev_priv->dev)) \
		ilk_dummy_write(dev_priv); \
	hsw_unclaimed_reg_clear(dev_priv, reg); \
	write##y(val, dev_priv->regs + reg); \
	if (unlikely(__fifo_ret)) { \
		gen6_gt_check_fifodbg(dev_priv); \
	} \
	hsw_unclaimed_reg_check(dev_priv, reg); \
	spin_unlock_irqrestore(&dev_priv->gt_lock, irqflags); \
}
__i915_write(8, b)
__i915_write(16, w)
__i915_write(32, l)
__i915_write(64, q)
#undef __i915_write

static const struct register_whitelist {
	uint64_t offset;
	uint32_t size;
	uint32_t gen_bitmask; /* support gens, 0x10 for 4, 0x30 for 4 and 5, etc. */
} whitelist[] = {
	{ RING_TIMESTAMP(RENDER_RING_BASE), 8, 0xF0 },
};

int i915_reg_read_ioctl(struct drm_device *dev,
			void *data, struct drm_file *file)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_i915_reg_read *reg = data;
	struct register_whitelist const *entry = whitelist;
	int i;

	for (i = 0; i < ARRAY_SIZE(whitelist); i++, entry++) {
		if (entry->offset == reg->offset &&
		    (1 << INTEL_INFO(dev)->gen & entry->gen_bitmask))
			break;
	}

	if (i == ARRAY_SIZE(whitelist))
		return -EINVAL;

	switch (entry->size) {
	case 8:
		reg->val = I915_READ64(reg->offset);
		break;
	case 4:
		reg->val = I915_READ(reg->offset);
		break;
	case 2:
		reg->val = I915_READ16(reg->offset);
		break;
	case 1:
		reg->val = I915_READ8(reg->offset);
		break;
	default:
		WARN_ON(1);
		return -EINVAL;
	}

	return 0;
}
=======
MODULE_AUTHOR("Tungsten Graphics, Inc.");
MODULE_AUTHOR("Intel Corporation");

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL and additional rights");
>>>>>>> v3.18
