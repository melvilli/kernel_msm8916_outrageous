/* i915_drv.h -- Private header for the I915 driver -*- linux-c -*-
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

#ifndef _I915_DRV_H_
#define _I915_DRV_H_

#include <uapi/drm/i915_drm.h>

#include "i915_reg.h"
#include "intel_bios.h"
#include "intel_ringbuffer.h"
<<<<<<< HEAD
=======
#include "intel_lrc.h"
#include "i915_gem_gtt.h"
#include "i915_gem_render_state.h"
>>>>>>> v3.18
#include <linux/io-mapping.h>
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>
#include <drm/intel-gtt.h>
<<<<<<< HEAD
#include <linux/backlight.h>
=======
#include <drm/drm_legacy.h> /* for struct drm_dma_handle */
#include <drm/drm_gem.h>
#include <linux/backlight.h>
#include <linux/hashtable.h>
>>>>>>> v3.18
#include <linux/intel-iommu.h>
#include <linux/kref.h>
#include <linux/pm_qos.h>

/* General customization:
 */

<<<<<<< HEAD
#define DRIVER_AUTHOR		"Tungsten Graphics, Inc."

#define DRIVER_NAME		"i915"
#define DRIVER_DESC		"Intel Graphics"
#define DRIVER_DATE		"20080730"

enum pipe {
	PIPE_A = 0,
	PIPE_B,
	PIPE_C,
	I915_MAX_PIPES
=======
#define DRIVER_NAME		"i915"
#define DRIVER_DESC		"Intel Graphics"
#define DRIVER_DATE		"20140905"

enum pipe {
	INVALID_PIPE = -1,
	PIPE_A = 0,
	PIPE_B,
	PIPE_C,
	_PIPE_EDP,
	I915_MAX_PIPES = _PIPE_EDP
>>>>>>> v3.18
};
#define pipe_name(p) ((p) + 'A')

enum transcoder {
	TRANSCODER_A = 0,
	TRANSCODER_B,
	TRANSCODER_C,
<<<<<<< HEAD
	TRANSCODER_EDP = 0xF,
=======
	TRANSCODER_EDP,
	I915_MAX_TRANSCODERS
>>>>>>> v3.18
};
#define transcoder_name(t) ((t) + 'A')

enum plane {
	PLANE_A = 0,
	PLANE_B,
	PLANE_C,
};
#define plane_name(p) ((p) + 'A')

<<<<<<< HEAD
=======
#define sprite_name(p, s) ((p) * INTEL_INFO(dev)->num_sprites[(p)] + (s) + 'A')

>>>>>>> v3.18
enum port {
	PORT_A = 0,
	PORT_B,
	PORT_C,
	PORT_D,
	PORT_E,
	I915_MAX_PORTS
};
#define port_name(p) ((p) + 'A')

<<<<<<< HEAD
=======
#define I915_NUM_PHYS_VLV 2

enum dpio_channel {
	DPIO_CH0,
	DPIO_CH1
};

enum dpio_phy {
	DPIO_PHY0,
	DPIO_PHY1
};

enum intel_display_power_domain {
	POWER_DOMAIN_PIPE_A,
	POWER_DOMAIN_PIPE_B,
	POWER_DOMAIN_PIPE_C,
	POWER_DOMAIN_PIPE_A_PANEL_FITTER,
	POWER_DOMAIN_PIPE_B_PANEL_FITTER,
	POWER_DOMAIN_PIPE_C_PANEL_FITTER,
	POWER_DOMAIN_TRANSCODER_A,
	POWER_DOMAIN_TRANSCODER_B,
	POWER_DOMAIN_TRANSCODER_C,
	POWER_DOMAIN_TRANSCODER_EDP,
	POWER_DOMAIN_PORT_DDI_A_2_LANES,
	POWER_DOMAIN_PORT_DDI_A_4_LANES,
	POWER_DOMAIN_PORT_DDI_B_2_LANES,
	POWER_DOMAIN_PORT_DDI_B_4_LANES,
	POWER_DOMAIN_PORT_DDI_C_2_LANES,
	POWER_DOMAIN_PORT_DDI_C_4_LANES,
	POWER_DOMAIN_PORT_DDI_D_2_LANES,
	POWER_DOMAIN_PORT_DDI_D_4_LANES,
	POWER_DOMAIN_PORT_DSI,
	POWER_DOMAIN_PORT_CRT,
	POWER_DOMAIN_PORT_OTHER,
	POWER_DOMAIN_VGA,
	POWER_DOMAIN_AUDIO,
	POWER_DOMAIN_PLLS,
	POWER_DOMAIN_INIT,

	POWER_DOMAIN_NUM,
};

#define POWER_DOMAIN_PIPE(pipe) ((pipe) + POWER_DOMAIN_PIPE_A)
#define POWER_DOMAIN_PIPE_PANEL_FITTER(pipe) \
		((pipe) + POWER_DOMAIN_PIPE_A_PANEL_FITTER)
#define POWER_DOMAIN_TRANSCODER(tran) \
	((tran) == TRANSCODER_EDP ? POWER_DOMAIN_TRANSCODER_EDP : \
	 (tran) + POWER_DOMAIN_TRANSCODER_A)

>>>>>>> v3.18
enum hpd_pin {
	HPD_NONE = 0,
	HPD_PORT_A = HPD_NONE, /* PORT_A is internal */
	HPD_TV = HPD_NONE,     /* TV is known to be unreliable */
	HPD_CRT,
	HPD_SDVO_B,
	HPD_SDVO_C,
	HPD_PORT_B,
	HPD_PORT_C,
	HPD_PORT_D,
	HPD_NUM_PINS
};

#define I915_GEM_GPU_DOMAINS \
	(I915_GEM_DOMAIN_RENDER | \
	 I915_GEM_DOMAIN_SAMPLER | \
	 I915_GEM_DOMAIN_COMMAND | \
	 I915_GEM_DOMAIN_INSTRUCTION | \
	 I915_GEM_DOMAIN_VERTEX)

<<<<<<< HEAD
#define for_each_pipe(p) for ((p) = 0; (p) < INTEL_INFO(dev)->num_pipes; (p)++)
=======
#define for_each_pipe(__dev_priv, __p) \
	for ((__p) = 0; (__p) < INTEL_INFO(__dev_priv)->num_pipes; (__p)++)
#define for_each_plane(pipe, p) \
	for ((p) = 0; (p) < INTEL_INFO(dev)->num_sprites[(pipe)] + 1; (p)++)
#define for_each_sprite(p, s) for ((s) = 0; (s) < INTEL_INFO(dev)->num_sprites[(p)]; (s)++)

#define for_each_crtc(dev, crtc) \
	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head)

#define for_each_intel_crtc(dev, intel_crtc) \
	list_for_each_entry(intel_crtc, &dev->mode_config.crtc_list, base.head)

#define for_each_intel_encoder(dev, intel_encoder)		\
	list_for_each_entry(intel_encoder,			\
			    &(dev)->mode_config.encoder_list,	\
			    base.head)
>>>>>>> v3.18

#define for_each_encoder_on_crtc(dev, __crtc, intel_encoder) \
	list_for_each_entry((intel_encoder), &(dev)->mode_config.encoder_list, base.head) \
		if ((intel_encoder)->base.crtc == (__crtc))

<<<<<<< HEAD
struct intel_pch_pll {
	int refcount; /* count of number of CRTCs sharing this PLL */
	int active; /* count of number of active CRTCs (i.e. DPMS on) */
	bool on; /* is the PLL actually active? Disabled during modeset */
	int pll_reg;
	int fp0_reg;
	int fp1_reg;
};
#define I915_NUM_PLLS 2
=======
#define for_each_connector_on_encoder(dev, __encoder, intel_connector) \
	list_for_each_entry((intel_connector), &(dev)->mode_config.connector_list, base.head) \
		if ((intel_connector)->base.encoder == (__encoder))

#define for_each_power_domain(domain, mask)				\
	for ((domain) = 0; (domain) < POWER_DOMAIN_NUM; (domain)++)	\
		if ((1 << (domain)) & (mask))

struct drm_i915_private;
struct i915_mm_struct;
struct i915_mmu_object;

enum intel_dpll_id {
	DPLL_ID_PRIVATE = -1, /* non-shared dpll in use */
	/* real shared dpll ids must be >= 0 */
	DPLL_ID_PCH_PLL_A = 0,
	DPLL_ID_PCH_PLL_B = 1,
	DPLL_ID_WRPLL1 = 0,
	DPLL_ID_WRPLL2 = 1,
};
#define I915_NUM_PLLS 2

struct intel_dpll_hw_state {
	/* i9xx, pch plls */
	uint32_t dpll;
	uint32_t dpll_md;
	uint32_t fp0;
	uint32_t fp1;

	/* hsw, bdw */
	uint32_t wrpll;
};

struct intel_shared_dpll {
	int refcount; /* count of number of CRTCs sharing this PLL */
	int active; /* count of number of active CRTCs (i.e. DPMS on) */
	bool on; /* is the PLL actually active? Disabled during modeset */
	const char *name;
	/* should match the index in the dev_priv->shared_dplls array */
	enum intel_dpll_id id;
	struct intel_dpll_hw_state hw_state;
	/* The mode_set hook is optional and should be used together with the
	 * intel_prepare_shared_dpll function. */
	void (*mode_set)(struct drm_i915_private *dev_priv,
			 struct intel_shared_dpll *pll);
	void (*enable)(struct drm_i915_private *dev_priv,
		       struct intel_shared_dpll *pll);
	void (*disable)(struct drm_i915_private *dev_priv,
			struct intel_shared_dpll *pll);
	bool (*get_hw_state)(struct drm_i915_private *dev_priv,
			     struct intel_shared_dpll *pll,
			     struct intel_dpll_hw_state *hw_state);
};
>>>>>>> v3.18

/* Used by dp and fdi links */
struct intel_link_m_n {
	uint32_t	tu;
	uint32_t	gmch_m;
	uint32_t	gmch_n;
	uint32_t	link_m;
	uint32_t	link_n;
};

void intel_link_compute_m_n(int bpp, int nlanes,
			    int pixel_clock, int link_clock,
			    struct intel_link_m_n *m_n);

<<<<<<< HEAD
struct intel_ddi_plls {
	int spll_refcount;
	int wrpll1_refcount;
	int wrpll2_refcount;
};

=======
>>>>>>> v3.18
/* Interface history:
 *
 * 1.1: Original.
 * 1.2: Add Power Management
 * 1.3: Add vblank support
 * 1.4: Fix cmdbuffer path, add heap destroy
 * 1.5: Add vblank pipe configuration
 * 1.6: - New ioctl for scheduling buffer swaps on vertical blank
 *      - Support vertical blank on secondary display pipe
 */
#define DRIVER_MAJOR		1
#define DRIVER_MINOR		6
#define DRIVER_PATCHLEVEL	0

<<<<<<< HEAD
#define WATCH_COHERENCY	0
#define WATCH_LISTS	0
#define WATCH_GTT	0

#define I915_GEM_PHYS_CURSOR_0 1
#define I915_GEM_PHYS_CURSOR_1 2
#define I915_GEM_PHYS_OVERLAY_REGS 3
#define I915_MAX_PHYS_OBJECT (I915_GEM_PHYS_OVERLAY_REGS)

struct drm_i915_gem_phys_object {
	int id;
	struct page **page_list;
	drm_dma_handle_t *handle;
	struct drm_i915_gem_object *cur_obj;
};

=======
#define WATCH_LISTS	0
#define WATCH_GTT	0

>>>>>>> v3.18
struct opregion_header;
struct opregion_acpi;
struct opregion_swsci;
struct opregion_asle;
<<<<<<< HEAD
struct drm_i915_private;
=======
>>>>>>> v3.18

struct intel_opregion {
	struct opregion_header __iomem *header;
	struct opregion_acpi __iomem *acpi;
	struct opregion_swsci __iomem *swsci;
<<<<<<< HEAD
	struct opregion_asle __iomem *asle;
	void __iomem *vbt;
	u32 __iomem *lid_state;
=======
	u32 swsci_gbda_sub_functions;
	u32 swsci_sbcb_sub_functions;
	struct opregion_asle __iomem *asle;
	void __iomem *vbt;
	u32 __iomem *lid_state;
	struct work_struct asle_work;
>>>>>>> v3.18
};
#define OPREGION_SIZE            (8*1024)

struct intel_overlay;
struct intel_overlay_error_state;

<<<<<<< HEAD
struct drm_i915_master_private {
	drm_local_map_t *sarea;
=======
struct drm_local_map;

struct drm_i915_master_private {
	struct drm_local_map *sarea;
>>>>>>> v3.18
	struct _drm_i915_sarea *sarea_priv;
};
#define I915_FENCE_REG_NONE -1
#define I915_MAX_NUM_FENCES 32
/* 32 fences + sign bit for FENCE_REG_NONE */
#define I915_MAX_NUM_FENCE_BITS 6

struct drm_i915_fence_reg {
	struct list_head lru_list;
	struct drm_i915_gem_object *obj;
	int pin_count;
};

struct sdvo_device_mapping {
	u8 initialized;
	u8 dvo_port;
	u8 slave_addr;
	u8 dvo_wiring;
	u8 i2c_pin;
	u8 ddc_pin;
};

struct intel_display_error_state;

struct drm_i915_error_state {
	struct kref ref;
<<<<<<< HEAD
	u32 eir;
	u32 pgtbl_er;
	u32 ier;
	u32 ccid;
	u32 derrmr;
	u32 forcewake;
	bool waiting[I915_NUM_RINGS];
	u32 pipestat[I915_MAX_PIPES];
	u32 tail[I915_NUM_RINGS];
	u32 head[I915_NUM_RINGS];
	u32 ctl[I915_NUM_RINGS];
	u32 ipeir[I915_NUM_RINGS];
	u32 ipehr[I915_NUM_RINGS];
	u32 instdone[I915_NUM_RINGS];
	u32 acthd[I915_NUM_RINGS];
	u32 semaphore_mboxes[I915_NUM_RINGS][I915_NUM_RINGS - 1];
	u32 semaphore_seqno[I915_NUM_RINGS][I915_NUM_RINGS - 1];
	u32 rc_psmi[I915_NUM_RINGS]; /* sleep state */
	/* our own tracking of ring head and tail */
	u32 cpu_ring_head[I915_NUM_RINGS];
	u32 cpu_ring_tail[I915_NUM_RINGS];
	u32 error; /* gen6+ */
	u32 err_int; /* gen7 */
	u32 instpm[I915_NUM_RINGS];
	u32 instps[I915_NUM_RINGS];
	u32 extra_instdone[I915_NUM_INSTDONE_REG];
	u32 seqno[I915_NUM_RINGS];
	u64 bbaddr;
	u32 fault_reg[I915_NUM_RINGS];
	u32 done_reg;
	u32 faddr[I915_NUM_RINGS];
	u64 fence[I915_MAX_NUM_FENCES];
	struct timeval time;
	struct drm_i915_error_ring {
=======
	struct timeval time;

	char error_msg[128];
	u32 reset_count;
	u32 suspend_count;

	/* Generic register state */
	u32 eir;
	u32 pgtbl_er;
	u32 ier;
	u32 gtier[4];
	u32 ccid;
	u32 derrmr;
	u32 forcewake;
	u32 error; /* gen6+ */
	u32 err_int; /* gen7 */
	u32 done_reg;
	u32 gac_eco;
	u32 gam_ecochk;
	u32 gab_ctl;
	u32 gfx_mode;
	u32 extra_instdone[I915_NUM_INSTDONE_REG];
	u64 fence[I915_MAX_NUM_FENCES];
	struct intel_overlay_error_state *overlay;
	struct intel_display_error_state *display;
	struct drm_i915_error_object *semaphore_obj;

	struct drm_i915_error_ring {
		bool valid;
		/* Software tracked state */
		bool waiting;
		int hangcheck_score;
		enum intel_ring_hangcheck_action hangcheck_action;
		int num_requests;

		/* our own tracking of ring head and tail */
		u32 cpu_ring_head;
		u32 cpu_ring_tail;

		u32 semaphore_seqno[I915_NUM_RINGS - 1];

		/* Register state */
		u32 tail;
		u32 head;
		u32 ctl;
		u32 hws;
		u32 ipeir;
		u32 ipehr;
		u32 instdone;
		u32 bbstate;
		u32 instpm;
		u32 instps;
		u32 seqno;
		u64 bbaddr;
		u64 acthd;
		u32 fault_reg;
		u64 faddr;
		u32 rc_psmi; /* sleep state */
		u32 semaphore_mboxes[I915_NUM_RINGS - 1];

>>>>>>> v3.18
		struct drm_i915_error_object {
			int page_count;
			u32 gtt_offset;
			u32 *pages[0];
<<<<<<< HEAD
		} *ringbuffer, *batchbuffer, *ctx;
=======
		} *ringbuffer, *batchbuffer, *wa_batchbuffer, *ctx, *hws_page;

>>>>>>> v3.18
		struct drm_i915_error_request {
			long jiffies;
			u32 seqno;
			u32 tail;
		} *requests;
<<<<<<< HEAD
		int num_requests;
	} ring[I915_NUM_RINGS];
=======

		struct {
			u32 gfx_mode;
			union {
				u64 pdp[4];
				u32 pp_dir_base;
			};
		} vm_info;

		pid_t pid;
		char comm[TASK_COMM_LEN];
	} ring[I915_NUM_RINGS];

>>>>>>> v3.18
	struct drm_i915_error_buffer {
		u32 size;
		u32 name;
		u32 rseqno, wseqno;
		u32 gtt_offset;
		u32 read_domains;
		u32 write_domain;
		s32 fence_reg:I915_MAX_NUM_FENCE_BITS;
		s32 pinned:2;
		u32 tiling:2;
		u32 dirty:1;
		u32 purgeable:1;
<<<<<<< HEAD
		s32 ring:4;
		u32 cache_level:2;
	} *active_bo, *pinned_bo;
	u32 active_bo_count, pinned_bo_count;
	struct intel_overlay_error_state *overlay;
	struct intel_display_error_state *display;
};

struct intel_crtc_config;
struct intel_crtc;

struct drm_i915_display_funcs {
	bool (*fbc_enabled)(struct drm_device *dev);
	void (*enable_fbc)(struct drm_crtc *crtc, unsigned long interval);
	void (*disable_fbc)(struct drm_device *dev);
	int (*get_display_clock_speed)(struct drm_device *dev);
	int (*get_fifo_size)(struct drm_device *dev, int plane);
	void (*update_wm)(struct drm_device *dev);
	void (*update_sprite_wm)(struct drm_device *dev, int pipe,
				 uint32_t sprite_width, int pixel_size);
	void (*update_linetime_wm)(struct drm_device *dev, int pipe,
				 struct drm_display_mode *mode);
=======
		u32 userptr:1;
		s32 ring:4;
		u32 cache_level:3;
	} **active_bo, **pinned_bo;

	u32 *active_bo_count, *pinned_bo_count;
	u32 vm_count;
};

struct intel_connector;
struct intel_crtc_config;
struct intel_plane_config;
struct intel_crtc;
struct intel_limit;
struct dpll;

struct drm_i915_display_funcs {
	bool (*fbc_enabled)(struct drm_device *dev);
	void (*enable_fbc)(struct drm_crtc *crtc);
	void (*disable_fbc)(struct drm_device *dev);
	int (*get_display_clock_speed)(struct drm_device *dev);
	int (*get_fifo_size)(struct drm_device *dev, int plane);
	/**
	 * find_dpll() - Find the best values for the PLL
	 * @limit: limits for the PLL
	 * @crtc: current CRTC
	 * @target: target frequency in kHz
	 * @refclk: reference clock frequency in kHz
	 * @match_clock: if provided, @best_clock P divider must
	 *               match the P divider from @match_clock
	 *               used for LVDS downclocking
	 * @best_clock: best PLL values found
	 *
	 * Returns true on success, false on failure.
	 */
	bool (*find_dpll)(const struct intel_limit *limit,
			  struct drm_crtc *crtc,
			  int target, int refclk,
			  struct dpll *match_clock,
			  struct dpll *best_clock);
	void (*update_wm)(struct drm_crtc *crtc);
	void (*update_sprite_wm)(struct drm_plane *plane,
				 struct drm_crtc *crtc,
				 uint32_t sprite_width, uint32_t sprite_height,
				 int pixel_size, bool enable, bool scaled);
>>>>>>> v3.18
	void (*modeset_global_resources)(struct drm_device *dev);
	/* Returns the active state of the crtc, and if the crtc is active,
	 * fills out the pipe-config with the hw state. */
	bool (*get_pipe_config)(struct intel_crtc *,
				struct intel_crtc_config *);
<<<<<<< HEAD
=======
	void (*get_plane_config)(struct intel_crtc *,
				 struct intel_plane_config *);
>>>>>>> v3.18
	int (*crtc_mode_set)(struct drm_crtc *crtc,
			     int x, int y,
			     struct drm_framebuffer *old_fb);
	void (*crtc_enable)(struct drm_crtc *crtc);
	void (*crtc_disable)(struct drm_crtc *crtc);
	void (*off)(struct drm_crtc *crtc);
	void (*write_eld)(struct drm_connector *connector,
<<<<<<< HEAD
			  struct drm_crtc *crtc);
=======
			  struct drm_crtc *crtc,
			  struct drm_display_mode *mode);
>>>>>>> v3.18
	void (*fdi_link_train)(struct drm_crtc *crtc);
	void (*init_clock_gating)(struct drm_device *dev);
	int (*queue_flip)(struct drm_device *dev, struct drm_crtc *crtc,
			  struct drm_framebuffer *fb,
<<<<<<< HEAD
			  struct drm_i915_gem_object *obj);
	int (*update_plane)(struct drm_crtc *crtc, struct drm_framebuffer *fb,
			    int x, int y);
=======
			  struct drm_i915_gem_object *obj,
			  struct intel_engine_cs *ring,
			  uint32_t flags);
	void (*update_primary_plane)(struct drm_crtc *crtc,
				     struct drm_framebuffer *fb,
				     int x, int y);
>>>>>>> v3.18
	void (*hpd_irq_setup)(struct drm_device *dev);
	/* clock updates for mode set */
	/* cursor updates */
	/* render clock increase/decrease */
	/* display clock increase/decrease */
	/* pll clock increase/decrease */
<<<<<<< HEAD
};

struct drm_i915_gt_funcs {
	void (*force_wake_get)(struct drm_i915_private *dev_priv);
	void (*force_wake_put)(struct drm_i915_private *dev_priv);
};

#define DEV_INFO_FLAGS \
	DEV_INFO_FLAG(is_mobile) DEV_INFO_SEP \
	DEV_INFO_FLAG(is_i85x) DEV_INFO_SEP \
	DEV_INFO_FLAG(is_i915g) DEV_INFO_SEP \
	DEV_INFO_FLAG(is_i945gm) DEV_INFO_SEP \
	DEV_INFO_FLAG(is_g33) DEV_INFO_SEP \
	DEV_INFO_FLAG(need_gfx_hws) DEV_INFO_SEP \
	DEV_INFO_FLAG(is_g4x) DEV_INFO_SEP \
	DEV_INFO_FLAG(is_pineview) DEV_INFO_SEP \
	DEV_INFO_FLAG(is_broadwater) DEV_INFO_SEP \
	DEV_INFO_FLAG(is_crestline) DEV_INFO_SEP \
	DEV_INFO_FLAG(is_ivybridge) DEV_INFO_SEP \
	DEV_INFO_FLAG(is_valleyview) DEV_INFO_SEP \
	DEV_INFO_FLAG(is_haswell) DEV_INFO_SEP \
	DEV_INFO_FLAG(has_force_wake) DEV_INFO_SEP \
	DEV_INFO_FLAG(has_fbc) DEV_INFO_SEP \
	DEV_INFO_FLAG(has_pipe_cxsr) DEV_INFO_SEP \
	DEV_INFO_FLAG(has_hotplug) DEV_INFO_SEP \
	DEV_INFO_FLAG(cursor_needs_physical) DEV_INFO_SEP \
	DEV_INFO_FLAG(has_overlay) DEV_INFO_SEP \
	DEV_INFO_FLAG(overlay_needs_physical) DEV_INFO_SEP \
	DEV_INFO_FLAG(supports_tv) DEV_INFO_SEP \
	DEV_INFO_FLAG(has_bsd_ring) DEV_INFO_SEP \
	DEV_INFO_FLAG(has_blt_ring) DEV_INFO_SEP \
	DEV_INFO_FLAG(has_llc)

struct intel_device_info {
	u32 display_mmio_offset;
	u8 num_pipes:3;
	u8 gen;
	u8 is_mobile:1;
	u8 is_i85x:1;
	u8 is_i915g:1;
	u8 is_i945gm:1;
	u8 is_g33:1;
	u8 need_gfx_hws:1;
	u8 is_g4x:1;
	u8 is_pineview:1;
	u8 is_broadwater:1;
	u8 is_crestline:1;
	u8 is_ivybridge:1;
	u8 is_valleyview:1;
	u8 has_force_wake:1;
	u8 is_haswell:1;
	u8 has_fbc:1;
	u8 has_pipe_cxsr:1;
	u8 has_hotplug:1;
	u8 cursor_needs_physical:1;
	u8 has_overlay:1;
	u8 overlay_needs_physical:1;
	u8 supports_tv:1;
	u8 has_bsd_ring:1;
	u8 has_blt_ring:1;
	u8 has_llc:1;
};

enum i915_cache_level {
	I915_CACHE_NONE = 0,
	I915_CACHE_LLC,
	I915_CACHE_LLC_MLC, /* gen6+, in docs at least! */
};

/* The Graphics Translation Table is the way in which GEN hardware translates a
 * Graphics Virtual Address into a Physical Address. In addition to the normal
 * collateral associated with any va->pa translations GEN hardware also has a
 * portion of the GTT which can be mapped by the CPU and remain both coherent
 * and correct (in cases like swizzling). That region is referred to as GMADR in
 * the spec.
 */
struct i915_gtt {
	unsigned long start;		/* Start offset of used GTT */
	size_t total;			/* Total size GTT can map */
	size_t stolen_size;		/* Total size of stolen memory */

	unsigned long mappable_end;	/* End offset that we can CPU map */
	struct io_mapping *mappable;	/* Mapping to our CPU mappable region */
	phys_addr_t mappable_base;	/* PA of our GMADR */

	/** "Graphics Stolen Memory" holds the global PTEs */
	void __iomem *gsm;

	bool do_idle_maps;
	dma_addr_t scratch_page_dma;
	struct page *scratch_page;

	/* global gtt ops */
	int (*gtt_probe)(struct drm_device *dev, size_t *gtt_total,
			  size_t *stolen, phys_addr_t *mappable_base,
			  unsigned long *mappable_end);
	void (*gtt_remove)(struct drm_device *dev);
	void (*gtt_clear_range)(struct drm_device *dev,
				unsigned int first_entry,
				unsigned int num_entries);
	void (*gtt_insert_entries)(struct drm_device *dev,
				   struct sg_table *st,
				   unsigned int pg_start,
				   enum i915_cache_level cache_level);
};
#define gtt_total_entries(gtt) ((gtt).total >> PAGE_SHIFT)

#define I915_PPGTT_PD_ENTRIES 512
#define I915_PPGTT_PT_ENTRIES 1024
struct i915_hw_ppgtt {
	struct drm_device *dev;
	unsigned num_pd_entries;
	struct page **pt_pages;
	uint32_t pd_offset;
	dma_addr_t *pt_dma_addr;
	dma_addr_t scratch_page_dma_addr;

	/* pte functions, mirroring the interface of the global gtt. */
	void (*clear_range)(struct i915_hw_ppgtt *ppgtt,
			    unsigned int first_entry,
			    unsigned int num_entries);
	void (*insert_entries)(struct i915_hw_ppgtt *ppgtt,
			       struct sg_table *st,
			       unsigned int pg_start,
			       enum i915_cache_level cache_level);
	int (*enable)(struct drm_device *dev);
	void (*cleanup)(struct i915_hw_ppgtt *ppgtt);
};


/* This must match up with the value previously used for execbuf2.rsvd1. */
#define DEFAULT_CONTEXT_ID 0
struct i915_hw_context {
	int id;
	bool is_initialized;
	struct drm_i915_file_private *file_priv;
	struct intel_ring_buffer *ring;
	struct drm_i915_gem_object *obj;
};

enum no_fbc_reason {
	FBC_NO_OUTPUT, /* no outputs enabled to compress */
	FBC_STOLEN_TOO_SMALL, /* not enough space to hold compressed buffers */
	FBC_UNSUPPORTED_MODE, /* interlace or doublescanned mode */
	FBC_MODE_TOO_LARGE, /* mode too large for compression */
	FBC_BAD_PLANE, /* fbc not supported on plane */
	FBC_NOT_TILED, /* buffer not tiled */
	FBC_MULTIPLE_PIPES, /* more than one pipe active */
	FBC_MODULE_PARAM,
=======

	int (*setup_backlight)(struct intel_connector *connector);
	uint32_t (*get_backlight)(struct intel_connector *connector);
	void (*set_backlight)(struct intel_connector *connector,
			      uint32_t level);
	void (*disable_backlight)(struct intel_connector *connector);
	void (*enable_backlight)(struct intel_connector *connector);
};

struct intel_uncore_funcs {
	void (*force_wake_get)(struct drm_i915_private *dev_priv,
							int fw_engine);
	void (*force_wake_put)(struct drm_i915_private *dev_priv,
							int fw_engine);

	uint8_t  (*mmio_readb)(struct drm_i915_private *dev_priv, off_t offset, bool trace);
	uint16_t (*mmio_readw)(struct drm_i915_private *dev_priv, off_t offset, bool trace);
	uint32_t (*mmio_readl)(struct drm_i915_private *dev_priv, off_t offset, bool trace);
	uint64_t (*mmio_readq)(struct drm_i915_private *dev_priv, off_t offset, bool trace);

	void (*mmio_writeb)(struct drm_i915_private *dev_priv, off_t offset,
				uint8_t val, bool trace);
	void (*mmio_writew)(struct drm_i915_private *dev_priv, off_t offset,
				uint16_t val, bool trace);
	void (*mmio_writel)(struct drm_i915_private *dev_priv, off_t offset,
				uint32_t val, bool trace);
	void (*mmio_writeq)(struct drm_i915_private *dev_priv, off_t offset,
				uint64_t val, bool trace);
};

struct intel_uncore {
	spinlock_t lock; /** lock is also taken in irq contexts. */

	struct intel_uncore_funcs funcs;

	unsigned fifo_count;
	unsigned forcewake_count;

	unsigned fw_rendercount;
	unsigned fw_mediacount;

	struct timer_list force_wake_timer;
};

#define DEV_INFO_FOR_EACH_FLAG(func, sep) \
	func(is_mobile) sep \
	func(is_i85x) sep \
	func(is_i915g) sep \
	func(is_i945gm) sep \
	func(is_g33) sep \
	func(need_gfx_hws) sep \
	func(is_g4x) sep \
	func(is_pineview) sep \
	func(is_broadwater) sep \
	func(is_crestline) sep \
	func(is_ivybridge) sep \
	func(is_valleyview) sep \
	func(is_haswell) sep \
	func(is_preliminary) sep \
	func(has_fbc) sep \
	func(has_pipe_cxsr) sep \
	func(has_hotplug) sep \
	func(cursor_needs_physical) sep \
	func(has_overlay) sep \
	func(overlay_needs_physical) sep \
	func(supports_tv) sep \
	func(has_llc) sep \
	func(has_ddi) sep \
	func(has_fpga_dbg)

#define DEFINE_FLAG(name) u8 name:1
#define SEP_SEMICOLON ;

struct intel_device_info {
	u32 display_mmio_offset;
	u16 device_id;
	u8 num_pipes:3;
	u8 num_sprites[I915_MAX_PIPES];
	u8 gen;
	u8 ring_mask; /* Rings supported by the HW */
	DEV_INFO_FOR_EACH_FLAG(DEFINE_FLAG, SEP_SEMICOLON);
	/* Register offsets for the various display pipes and transcoders */
	int pipe_offsets[I915_MAX_TRANSCODERS];
	int trans_offsets[I915_MAX_TRANSCODERS];
	int palette_offsets[I915_MAX_PIPES];
	int cursor_offsets[I915_MAX_PIPES];
};

#undef DEFINE_FLAG
#undef SEP_SEMICOLON

enum i915_cache_level {
	I915_CACHE_NONE = 0,
	I915_CACHE_LLC, /* also used for snoopable memory on non-LLC */
	I915_CACHE_L3_LLC, /* gen7+, L3 sits between the domain specifc
			      caches, eg sampler/render caches, and the
			      large Last-Level-Cache. LLC is coherent with
			      the CPU, but L3 is only visible to the GPU. */
	I915_CACHE_WT, /* hsw:gt3e WriteThrough for scanouts */
};

struct i915_ctx_hang_stats {
	/* This context had batch pending when hang was declared */
	unsigned batch_pending;

	/* This context had batch active when hang was declared */
	unsigned batch_active;

	/* Time when this context was last blamed for a GPU reset */
	unsigned long guilty_ts;

	/* This context is banned to submit more work */
	bool banned;
};

/* This must match up with the value previously used for execbuf2.rsvd1. */
#define DEFAULT_CONTEXT_HANDLE 0
/**
 * struct intel_context - as the name implies, represents a context.
 * @ref: reference count.
 * @user_handle: userspace tracking identity for this context.
 * @remap_slice: l3 row remapping information.
 * @file_priv: filp associated with this context (NULL for global default
 *	       context).
 * @hang_stats: information about the role of this context in possible GPU
 *		hangs.
 * @vm: virtual memory space used by this context.
 * @legacy_hw_ctx: render context backing object and whether it is correctly
 *                initialized (legacy ring submission mechanism only).
 * @link: link in the global list of contexts.
 *
 * Contexts are memory images used by the hardware to store copies of their
 * internal state.
 */
struct intel_context {
	struct kref ref;
	int user_handle;
	uint8_t remap_slice;
	struct drm_i915_file_private *file_priv;
	struct i915_ctx_hang_stats hang_stats;
	struct i915_hw_ppgtt *ppgtt;

	/* Legacy ring buffer submission */
	struct {
		struct drm_i915_gem_object *rcs_state;
		bool initialized;
	} legacy_hw_ctx;

	/* Execlists */
	bool rcs_initialized;
	struct {
		struct drm_i915_gem_object *state;
		struct intel_ringbuffer *ringbuf;
	} engine[I915_NUM_RINGS];

	struct list_head link;
};

struct i915_fbc {
	unsigned long size;
	unsigned threshold;
	unsigned int fb_id;
	enum plane plane;
	int y;

	struct drm_mm_node compressed_fb;
	struct drm_mm_node *compressed_llb;

	bool false_color;

	struct intel_fbc_work {
		struct delayed_work work;
		struct drm_crtc *crtc;
		struct drm_framebuffer *fb;
	} *fbc_work;

	enum no_fbc_reason {
		FBC_OK, /* FBC is enabled */
		FBC_UNSUPPORTED, /* FBC is not supported by this chipset */
		FBC_NO_OUTPUT, /* no outputs enabled to compress */
		FBC_STOLEN_TOO_SMALL, /* not enough space for buffers */
		FBC_UNSUPPORTED_MODE, /* interlace or doublescanned mode */
		FBC_MODE_TOO_LARGE, /* mode too large for compression */
		FBC_BAD_PLANE, /* fbc not supported on plane */
		FBC_NOT_TILED, /* buffer not tiled */
		FBC_MULTIPLE_PIPES, /* more than one pipe active */
		FBC_MODULE_PARAM,
		FBC_CHIP_DEFAULT, /* disabled by default on this chip */
	} no_fbc_reason;
};

struct i915_drrs {
	struct intel_connector *connector;
};

struct intel_dp;
struct i915_psr {
	struct mutex lock;
	bool sink_support;
	bool source_ok;
	struct intel_dp *enabled;
	bool active;
	struct delayed_work work;
	unsigned busy_frontbuffer_bits;
>>>>>>> v3.18
};

enum intel_pch {
	PCH_NONE = 0,	/* No PCH present */
	PCH_IBX,	/* Ibexpeak PCH */
	PCH_CPT,	/* Cougarpoint PCH */
	PCH_LPT,	/* Lynxpoint PCH */
	PCH_NOP,
};

enum intel_sbi_destination {
	SBI_ICLK,
	SBI_MPHY,
};

#define QUIRK_PIPEA_FORCE (1<<0)
#define QUIRK_LVDS_SSC_DISABLE (1<<1)
#define QUIRK_INVERT_BRIGHTNESS (1<<2)
<<<<<<< HEAD
#define QUIRK_NO_PCH_PWM_ENABLE (1<<3)
=======
#define QUIRK_BACKLIGHT_PRESENT (1<<3)
#define QUIRK_PIPEB_FORCE (1<<4)
>>>>>>> v3.18

struct intel_fbdev;
struct intel_fbc_work;

struct intel_gmbus {
	struct i2c_adapter adapter;
	u32 force_bit;
	u32 reg0;
	u32 gpio_reg;
	struct i2c_algo_bit_data bit_algo;
	struct drm_i915_private *dev_priv;
};

struct i915_suspend_saved_registers {
	u8 saveLBB;
	u32 saveDSPACNTR;
	u32 saveDSPBCNTR;
	u32 saveDSPARB;
	u32 savePIPEACONF;
	u32 savePIPEBCONF;
	u32 savePIPEASRC;
	u32 savePIPEBSRC;
	u32 saveFPA0;
	u32 saveFPA1;
	u32 saveDPLL_A;
	u32 saveDPLL_A_MD;
	u32 saveHTOTAL_A;
	u32 saveHBLANK_A;
	u32 saveHSYNC_A;
	u32 saveVTOTAL_A;
	u32 saveVBLANK_A;
	u32 saveVSYNC_A;
	u32 saveBCLRPAT_A;
	u32 saveTRANSACONF;
	u32 saveTRANS_HTOTAL_A;
	u32 saveTRANS_HBLANK_A;
	u32 saveTRANS_HSYNC_A;
	u32 saveTRANS_VTOTAL_A;
	u32 saveTRANS_VBLANK_A;
	u32 saveTRANS_VSYNC_A;
	u32 savePIPEASTAT;
	u32 saveDSPASTRIDE;
	u32 saveDSPASIZE;
	u32 saveDSPAPOS;
	u32 saveDSPAADDR;
	u32 saveDSPASURF;
	u32 saveDSPATILEOFF;
	u32 savePFIT_PGM_RATIOS;
	u32 saveBLC_HIST_CTL;
	u32 saveBLC_PWM_CTL;
	u32 saveBLC_PWM_CTL2;
<<<<<<< HEAD
=======
	u32 saveBLC_HIST_CTL_B;
>>>>>>> v3.18
	u32 saveBLC_CPU_PWM_CTL;
	u32 saveBLC_CPU_PWM_CTL2;
	u32 saveFPB0;
	u32 saveFPB1;
	u32 saveDPLL_B;
	u32 saveDPLL_B_MD;
	u32 saveHTOTAL_B;
	u32 saveHBLANK_B;
	u32 saveHSYNC_B;
	u32 saveVTOTAL_B;
	u32 saveVBLANK_B;
	u32 saveVSYNC_B;
	u32 saveBCLRPAT_B;
	u32 saveTRANSBCONF;
	u32 saveTRANS_HTOTAL_B;
	u32 saveTRANS_HBLANK_B;
	u32 saveTRANS_HSYNC_B;
	u32 saveTRANS_VTOTAL_B;
	u32 saveTRANS_VBLANK_B;
	u32 saveTRANS_VSYNC_B;
	u32 savePIPEBSTAT;
	u32 saveDSPBSTRIDE;
	u32 saveDSPBSIZE;
	u32 saveDSPBPOS;
	u32 saveDSPBADDR;
	u32 saveDSPBSURF;
	u32 saveDSPBTILEOFF;
	u32 saveVGA0;
	u32 saveVGA1;
	u32 saveVGA_PD;
	u32 saveVGACNTRL;
	u32 saveADPA;
	u32 saveLVDS;
	u32 savePP_ON_DELAYS;
	u32 savePP_OFF_DELAYS;
	u32 saveDVOA;
	u32 saveDVOB;
	u32 saveDVOC;
	u32 savePP_ON;
	u32 savePP_OFF;
	u32 savePP_CONTROL;
	u32 savePP_DIVISOR;
	u32 savePFIT_CONTROL;
	u32 save_palette_a[256];
	u32 save_palette_b[256];
<<<<<<< HEAD
	u32 saveDPFC_CB_BASE;
	u32 saveFBC_CFB_BASE;
	u32 saveFBC_LL_BASE;
	u32 saveFBC_CONTROL;
	u32 saveFBC_CONTROL2;
=======
	u32 saveFBC_CONTROL;
>>>>>>> v3.18
	u32 saveIER;
	u32 saveIIR;
	u32 saveIMR;
	u32 saveDEIER;
	u32 saveDEIMR;
	u32 saveGTIER;
	u32 saveGTIMR;
	u32 saveFDI_RXA_IMR;
	u32 saveFDI_RXB_IMR;
	u32 saveCACHE_MODE_0;
	u32 saveMI_ARB_STATE;
	u32 saveSWF0[16];
	u32 saveSWF1[16];
	u32 saveSWF2[3];
	u8 saveMSR;
	u8 saveSR[8];
	u8 saveGR[25];
	u8 saveAR_INDEX;
	u8 saveAR[21];
	u8 saveDACMASK;
	u8 saveCR[37];
	uint64_t saveFENCE[I915_MAX_NUM_FENCES];
	u32 saveCURACNTR;
	u32 saveCURAPOS;
	u32 saveCURABASE;
	u32 saveCURBCNTR;
	u32 saveCURBPOS;
	u32 saveCURBBASE;
	u32 saveCURSIZE;
	u32 saveDP_B;
	u32 saveDP_C;
	u32 saveDP_D;
	u32 savePIPEA_GMCH_DATA_M;
	u32 savePIPEB_GMCH_DATA_M;
	u32 savePIPEA_GMCH_DATA_N;
	u32 savePIPEB_GMCH_DATA_N;
	u32 savePIPEA_DP_LINK_M;
	u32 savePIPEB_DP_LINK_M;
	u32 savePIPEA_DP_LINK_N;
	u32 savePIPEB_DP_LINK_N;
	u32 saveFDI_RXA_CTL;
	u32 saveFDI_TXA_CTL;
	u32 saveFDI_RXB_CTL;
	u32 saveFDI_TXB_CTL;
	u32 savePFA_CTL_1;
	u32 savePFB_CTL_1;
	u32 savePFA_WIN_SZ;
	u32 savePFB_WIN_SZ;
	u32 savePFA_WIN_POS;
	u32 savePFB_WIN_POS;
	u32 savePCH_DREF_CONTROL;
	u32 saveDISP_ARB_CTL;
	u32 savePIPEA_DATA_M1;
	u32 savePIPEA_DATA_N1;
	u32 savePIPEA_LINK_M1;
	u32 savePIPEA_LINK_N1;
	u32 savePIPEB_DATA_M1;
	u32 savePIPEB_DATA_N1;
	u32 savePIPEB_LINK_M1;
	u32 savePIPEB_LINK_N1;
	u32 saveMCHBAR_RENDER_STANDBY;
	u32 savePCH_PORT_HOTPLUG;
};

<<<<<<< HEAD
struct intel_gen6_power_mgmt {
	struct work_struct work;
	u32 pm_iir;
	/* lock - irqsave spinlock that protectects the work_struct and
	 * pm_iir. */
	spinlock_t lock;

	/* The below variables an all the rps hw state are protected by
	 * dev->struct mutext. */
	u8 cur_delay;
	u8 min_delay;
	u8 max_delay;
	u8 hw_max;

	struct delayed_work delayed_resume_work;

=======
struct vlv_s0ix_state {
	/* GAM */
	u32 wr_watermark;
	u32 gfx_prio_ctrl;
	u32 arb_mode;
	u32 gfx_pend_tlb0;
	u32 gfx_pend_tlb1;
	u32 lra_limits[GEN7_LRA_LIMITS_REG_NUM];
	u32 media_max_req_count;
	u32 gfx_max_req_count;
	u32 render_hwsp;
	u32 ecochk;
	u32 bsd_hwsp;
	u32 blt_hwsp;
	u32 tlb_rd_addr;

	/* MBC */
	u32 g3dctl;
	u32 gsckgctl;
	u32 mbctl;

	/* GCP */
	u32 ucgctl1;
	u32 ucgctl3;
	u32 rcgctl1;
	u32 rcgctl2;
	u32 rstctl;
	u32 misccpctl;

	/* GPM */
	u32 gfxpause;
	u32 rpdeuhwtc;
	u32 rpdeuc;
	u32 ecobus;
	u32 pwrdwnupctl;
	u32 rp_down_timeout;
	u32 rp_deucsw;
	u32 rcubmabdtmr;
	u32 rcedata;
	u32 spare2gh;

	/* Display 1 CZ domain */
	u32 gt_imr;
	u32 gt_ier;
	u32 pm_imr;
	u32 pm_ier;
	u32 gt_scratch[GEN7_GT_SCRATCH_REG_NUM];

	/* GT SA CZ domain */
	u32 tilectl;
	u32 gt_fifoctl;
	u32 gtlc_wake_ctrl;
	u32 gtlc_survive;
	u32 pmwgicz;

	/* Display 2 CZ domain */
	u32 gu_ctl0;
	u32 gu_ctl1;
	u32 clock_gate_dis2;
};

struct intel_rps_ei {
	u32 cz_clock;
	u32 render_c0;
	u32 media_c0;
};

struct intel_gen6_power_mgmt {
	/* work and pm_iir are protected by dev_priv->irq_lock */
	struct work_struct work;
	u32 pm_iir;

	/* Frequencies are stored in potentially platform dependent multiples.
	 * In other words, *_freq needs to be multiplied by X to be interesting.
	 * Soft limits are those which are used for the dynamic reclocking done
	 * by the driver (raise frequencies under heavy loads, and lower for
	 * lighter loads). Hard limits are those imposed by the hardware.
	 *
	 * A distinction is made for overclocking, which is never enabled by
	 * default, and is considered to be above the hard limit if it's
	 * possible at all.
	 */
	u8 cur_freq;		/* Current frequency (cached, may not == HW) */
	u8 min_freq_softlimit;	/* Minimum frequency permitted by the driver */
	u8 max_freq_softlimit;	/* Max frequency permitted by the driver */
	u8 max_freq;		/* Maximum frequency, RP0 if not overclocking */
	u8 min_freq;		/* AKA RPn. Minimum frequency */
	u8 efficient_freq;	/* AKA RPe. Pre-determined balanced frequency */
	u8 rp1_freq;		/* "less than" RP0 power/freqency */
	u8 rp0_freq;		/* Non-overclocked max frequency. */
	u32 cz_freq;

	u32 ei_interrupt_count;

	int last_adj;
	enum { LOW_POWER, BETWEEN, HIGH_POWER } power;

	bool enabled;
	struct delayed_work delayed_resume_work;

	/* manual wa residency calculations */
	struct intel_rps_ei up_ei, down_ei;

>>>>>>> v3.18
	/*
	 * Protects RPS/RC6 register access and PCU communication.
	 * Must be taken after struct_mutex if nested.
	 */
	struct mutex hw_lock;
};

/* defined intel_pm.c */
extern spinlock_t mchdev_lock;

struct intel_ilk_power_mgmt {
	u8 cur_delay;
	u8 min_delay;
	u8 max_delay;
	u8 fmax;
	u8 fstart;

	u64 last_count1;
	unsigned long last_time1;
	unsigned long chipset_power;
	u64 last_count2;
<<<<<<< HEAD
	struct timespec last_time2;
=======
	u64 last_time2;
>>>>>>> v3.18
	unsigned long gfx_power;
	u8 corr;

	int c_m;
	int r_t;

	struct drm_i915_gem_object *pwrctx;
	struct drm_i915_gem_object *renderctx;
};

<<<<<<< HEAD
=======
struct drm_i915_private;
struct i915_power_well;

struct i915_power_well_ops {
	/*
	 * Synchronize the well's hw state to match the current sw state, for
	 * example enable/disable it based on the current refcount. Called
	 * during driver init and resume time, possibly after first calling
	 * the enable/disable handlers.
	 */
	void (*sync_hw)(struct drm_i915_private *dev_priv,
			struct i915_power_well *power_well);
	/*
	 * Enable the well and resources that depend on it (for example
	 * interrupts located on the well). Called after the 0->1 refcount
	 * transition.
	 */
	void (*enable)(struct drm_i915_private *dev_priv,
		       struct i915_power_well *power_well);
	/*
	 * Disable the well and resources that depend on it. Called after
	 * the 1->0 refcount transition.
	 */
	void (*disable)(struct drm_i915_private *dev_priv,
			struct i915_power_well *power_well);
	/* Returns the hw enabled state. */
	bool (*is_enabled)(struct drm_i915_private *dev_priv,
			   struct i915_power_well *power_well);
};

/* Power well structure for haswell */
struct i915_power_well {
	const char *name;
	bool always_on;
	/* power well enable/disable usage count */
	int count;
	/* cached hw enabled state */
	bool hw_enabled;
	unsigned long domains;
	unsigned long data;
	const struct i915_power_well_ops *ops;
};

struct i915_power_domains {
	/*
	 * Power wells needed for initialization at driver init and suspend
	 * time are on. They are kept on until after the first modeset.
	 */
	bool init_power_on;
	bool initializing;
	int power_well_count;

	struct mutex lock;
	int domain_use_count[POWER_DOMAIN_NUM];
	struct i915_power_well *power_wells;
};

>>>>>>> v3.18
struct i915_dri1_state {
	unsigned allow_batchbuffer : 1;
	u32 __iomem *gfx_hws_cpu_addr;

	unsigned int cpp;
	int back_offset;
	int front_offset;
	int current_page;
	int page_flipping;

	uint32_t counter;
};

<<<<<<< HEAD
struct intel_l3_parity {
	u32 *remap_info;
	struct work_struct error_work;
=======
struct i915_ums_state {
	/**
	 * Flag if the X Server, and thus DRM, is not currently in
	 * control of the device.
	 *
	 * This is set between LeaveVT and EnterVT.  It needs to be
	 * replaced with a semaphore.  It also needs to be
	 * transitioned away from for kernel modesetting.
	 */
	int mm_suspended;
};

#define MAX_L3_SLICES 2
struct intel_l3_parity {
	u32 *remap_info[MAX_L3_SLICES];
	struct work_struct error_work;
	int which_slice;
>>>>>>> v3.18
};

struct i915_gem_mm {
	/** Memory allocator for GTT stolen memory */
	struct drm_mm stolen;
<<<<<<< HEAD
	/** Memory allocator for GTT */
	struct drm_mm gtt_space;
=======
>>>>>>> v3.18
	/** List of all objects in gtt_space. Used to restore gtt
	 * mappings on resume */
	struct list_head bound_list;
	/**
	 * List of objects which are not bound to the GTT (thus
	 * are idle and not used by the GPU) but still have
	 * (presumably uncached) pages still attached.
	 */
	struct list_head unbound_list;

	/** Usable portion of the GTT for GEM */
	unsigned long stolen_base; /* limited to low memory (32-bit) */

<<<<<<< HEAD
	int gtt_mtrr;

	/** PPGTT used for aliasing the PPGTT with the GTT */
	struct i915_hw_ppgtt *aliasing_ppgtt;

	struct shrinker inactive_shrinker;
	bool shrinker_no_lock_stealing;

	/**
	 * List of objects currently involved in rendering.
	 *
	 * Includes buffers having the contents of their GPU caches
	 * flushed, not necessarily primitives.  last_rendering_seqno
	 * represents when the rendering involved will be completed.
	 *
	 * A reference is held on the buffer while on this list.
	 */
	struct list_head active_list;

	/**
	 * LRU list of objects which are not in the ringbuffer and
	 * are ready to unbind, but are still in the GTT.
	 *
	 * last_rendering_seqno is 0 while an object is in this list.
	 *
	 * A reference is not held on the buffer while on this list,
	 * as merely being GTT-bound shouldn't prevent its being
	 * freed, and we'll pull it off the list in the free path.
	 */
	struct list_head inactive_list;

=======
	/** PPGTT used for aliasing the PPGTT with the GTT */
	struct i915_hw_ppgtt *aliasing_ppgtt;

	struct notifier_block oom_notifier;
	struct shrinker shrinker;
	bool shrinker_no_lock_stealing;

>>>>>>> v3.18
	/** LRU list of objects with fence regs on them. */
	struct list_head fence_list;

	/**
	 * We leave the user IRQ off as much as possible,
	 * but this means that requests will finish and never
	 * be retired once the system goes idle. Set a timer to
	 * fire periodically while the ring is running. When it
	 * fires, go retire requests.
	 */
	struct delayed_work retire_work;

	/**
<<<<<<< HEAD
=======
	 * When we detect an idle GPU, we want to turn on
	 * powersaving features. So once we see that there
	 * are no more requests outstanding and no more
	 * arrive within a small period of time, we fire
	 * off the idle_work.
	 */
	struct delayed_work idle_work;

	/**
>>>>>>> v3.18
	 * Are we in a non-interruptible section of code like
	 * modesetting?
	 */
	bool interruptible;

	/**
<<<<<<< HEAD
	 * Flag if the X Server, and thus DRM, is not currently in
	 * control of the device.
	 *
	 * This is set between LeaveVT and EnterVT.  It needs to be
	 * replaced with a semaphore.  It also needs to be
	 * transitioned away from for kernel modesetting.
	 */
	int suspended;
=======
	 * Is the GPU currently considered idle, or busy executing userspace
	 * requests?  Whilst idle, we attempt to power down the hardware and
	 * display clocks. In order to reduce the effect on performance, there
	 * is a slight delay before we do so.
	 */
	bool busy;

	/* the indicator for dispatch video commands on two BSD rings */
	int bsd_ring_dispatch_index;
>>>>>>> v3.18

	/** Bit 6 swizzling required for X tiling */
	uint32_t bit_6_swizzle_x;
	/** Bit 6 swizzling required for Y tiling */
	uint32_t bit_6_swizzle_y;

<<<<<<< HEAD
	/* storage for physical objects */
	struct drm_i915_gem_phys_object *phys_objs[I915_MAX_PHYS_OBJECT];

	/* accounting, useful for userland debugging */
=======
	/* accounting, useful for userland debugging */
	spinlock_t object_stat_lock;
>>>>>>> v3.18
	size_t object_memory;
	u32 object_count;
};

<<<<<<< HEAD
=======
struct drm_i915_error_state_buf {
	struct drm_i915_private *i915;
	unsigned bytes;
	unsigned size;
	int err;
	u8 *buf;
	loff_t start;
	loff_t pos;
};

struct i915_error_state_file_priv {
	struct drm_device *dev;
	struct drm_i915_error_state *error;
};

>>>>>>> v3.18
struct i915_gpu_error {
	/* For hangcheck timer */
#define DRM_I915_HANGCHECK_PERIOD 1500 /* in ms */
#define DRM_I915_HANGCHECK_JIFFIES msecs_to_jiffies(DRM_I915_HANGCHECK_PERIOD)
<<<<<<< HEAD
	struct timer_list hangcheck_timer;
	int hangcheck_count;
	uint32_t last_acthd[I915_NUM_RINGS];
	uint32_t prev_instdone[I915_NUM_INSTDONE_REG];
=======
	/* Hang gpu twice in this window and your context gets banned */
#define DRM_I915_CTX_BAN_PERIOD DIV_ROUND_UP(8*DRM_I915_HANGCHECK_PERIOD, 1000)

	struct timer_list hangcheck_timer;
>>>>>>> v3.18

	/* For reset and error_state handling. */
	spinlock_t lock;
	/* Protected by the above dev->gpu_error.lock. */
	struct drm_i915_error_state *first_error;
	struct work_struct work;

<<<<<<< HEAD
	unsigned long last_reset;

	/**
	 * State variable and reset counter controlling the reset flow
	 *
	 * Upper bits are for the reset counter.  This counter is used by the
	 * wait_seqno code to race-free noticed that a reset event happened and
	 * that it needs to restart the entire ioctl (since most likely the
	 * seqno it waited for won't ever signal anytime soon).
=======

	unsigned long missed_irq_rings;

	/**
	 * State variable controlling the reset flow and count
	 *
	 * This is a counter which gets incremented when reset is triggered,
	 * and again when reset has been handled. So odd values (lowest bit set)
	 * means that reset is in progress and even values that
	 * (reset_counter >> 1):th reset was successfully completed.
	 *
	 * If reset is not completed succesfully, the I915_WEDGE bit is
	 * set meaning that hardware is terminally sour and there is no
	 * recovery. All waiters on the reset_queue will be woken when
	 * that happens.
	 *
	 * This counter is used by the wait_seqno code to notice that reset
	 * event happened and it needs to restart the entire ioctl (since most
	 * likely the seqno it waited for won't ever signal anytime soon).
>>>>>>> v3.18
	 *
	 * This is important for lock-free wait paths, where no contended lock
	 * naturally enforces the correct ordering between the bail-out of the
	 * waiter and the gpu reset work code.
<<<<<<< HEAD
	 *
	 * Lowest bit controls the reset state machine: Set means a reset is in
	 * progress. This state will (presuming we don't have any bugs) decay
	 * into either unset (successful reset) or the special WEDGED value (hw
	 * terminally sour). All waiters on the reset_queue will be woken when
	 * that happens.
	 */
	atomic_t reset_counter;

	/**
	 * Special values/flags for reset_counter
	 *
	 * Note that the code relies on
	 * 	I915_WEDGED & I915_RESET_IN_PROGRESS_FLAG
	 * being true.
	 */
#define I915_RESET_IN_PROGRESS_FLAG	1
#define I915_WEDGED			0xffffffff
=======
	 */
	atomic_t reset_counter;

#define I915_RESET_IN_PROGRESS_FLAG	1
#define I915_WEDGED			(1 << 31)
>>>>>>> v3.18

	/**
	 * Waitqueue to signal when the reset has completed. Used by clients
	 * that wait for dev_priv->mm.wedged to settle.
	 */
	wait_queue_head_t reset_queue;

<<<<<<< HEAD
	/* For gpu hang simulation. */
	unsigned int stop_rings;
=======
	/* Userspace knobs for gpu hang simulation;
	 * combines both a ring mask, and extra flags
	 */
	u32 stop_rings;
#define I915_STOP_RING_ALLOW_BAN       (1 << 31)
#define I915_STOP_RING_ALLOW_WARN      (1 << 30)

	/* For missed irq/seqno simulation. */
	unsigned int test_irq_rings;

	/* Used to prevent gem_check_wedged returning -EAGAIN during gpu reset   */
	bool reload_in_reset;
>>>>>>> v3.18
};

enum modeset_restore {
	MODESET_ON_LID_OPEN,
	MODESET_DONE,
	MODESET_SUSPENDED,
};

<<<<<<< HEAD
typedef struct drm_i915_private {
	struct drm_device *dev;
	struct kmem_cache *slab;

	const struct intel_device_info *info;
=======
struct ddi_vbt_port_info {
	/*
	 * This is an index in the HDMI/DVI DDI buffer translation table.
	 * The special value HDMI_LEVEL_SHIFT_UNKNOWN means the VBT didn't
	 * populate this field.
	 */
#define HDMI_LEVEL_SHIFT_UNKNOWN	0xff
	uint8_t hdmi_level_shift;

	uint8_t supports_dvi:1;
	uint8_t supports_hdmi:1;
	uint8_t supports_dp:1;
};

enum drrs_support_type {
	DRRS_NOT_SUPPORTED = 0,
	STATIC_DRRS_SUPPORT = 1,
	SEAMLESS_DRRS_SUPPORT = 2
};

struct intel_vbt_data {
	struct drm_display_mode *lfp_lvds_vbt_mode; /* if any */
	struct drm_display_mode *sdvo_lvds_vbt_mode; /* if any */

	/* Feature bits */
	unsigned int int_tv_support:1;
	unsigned int lvds_dither:1;
	unsigned int lvds_vbt:1;
	unsigned int int_crt_support:1;
	unsigned int lvds_use_ssc:1;
	unsigned int display_clock_mode:1;
	unsigned int fdi_rx_polarity_inverted:1;
	unsigned int has_mipi:1;
	int lvds_ssc_freq;
	unsigned int bios_lvds_val; /* initial [PCH_]LVDS reg val in VBIOS */

	enum drrs_support_type drrs_type;

	/* eDP */
	int edp_rate;
	int edp_lanes;
	int edp_preemphasis;
	int edp_vswing;
	bool edp_initialized;
	bool edp_support;
	int edp_bpp;
	struct edp_power_seq edp_pps;

	struct {
		u16 pwm_freq_hz;
		bool present;
		bool active_low_pwm;
		u8 min_brightness;	/* min_brightness/255 of max */
	} backlight;

	/* MIPI DSI */
	struct {
		u16 port;
		u16 panel_id;
		struct mipi_config *config;
		struct mipi_pps_data *pps;
		u8 seq_version;
		u32 size;
		u8 *data;
		u8 *sequence[MIPI_SEQ_MAX];
	} dsi;

	int crt_ddc_pin;

	int child_dev_num;
	union child_device_config *child_dev;

	struct ddi_vbt_port_info ddi_port_info[I915_MAX_PORTS];
};

enum intel_ddb_partitioning {
	INTEL_DDB_PART_1_2,
	INTEL_DDB_PART_5_6, /* IVB+ */
};

struct intel_wm_level {
	bool enable;
	uint32_t pri_val;
	uint32_t spr_val;
	uint32_t cur_val;
	uint32_t fbc_val;
};

struct ilk_wm_values {
	uint32_t wm_pipe[3];
	uint32_t wm_lp[3];
	uint32_t wm_lp_spr[3];
	uint32_t wm_linetime[3];
	bool enable_fbc_wm;
	enum intel_ddb_partitioning partitioning;
};

/*
 * This struct helps tracking the state needed for runtime PM, which puts the
 * device in PCI D3 state. Notice that when this happens, nothing on the
 * graphics device works, even register access, so we don't get interrupts nor
 * anything else.
 *
 * Every piece of our code that needs to actually touch the hardware needs to
 * either call intel_runtime_pm_get or call intel_display_power_get with the
 * appropriate power domain.
 *
 * Our driver uses the autosuspend delay feature, which means we'll only really
 * suspend if we stay with zero refcount for a certain amount of time. The
 * default value is currently very conservative (see intel_init_runtime_pm), but
 * it can be changed with the standard runtime PM files from sysfs.
 *
 * The irqs_disabled variable becomes true exactly after we disable the IRQs and
 * goes back to false exactly before we reenable the IRQs. We use this variable
 * to check if someone is trying to enable/disable IRQs while they're supposed
 * to be disabled. This shouldn't happen and we'll print some error messages in
 * case it happens.
 *
 * For more, read the Documentation/power/runtime_pm.txt.
 */
struct i915_runtime_pm {
	bool suspended;
	bool _irqs_disabled;
};

enum intel_pipe_crc_source {
	INTEL_PIPE_CRC_SOURCE_NONE,
	INTEL_PIPE_CRC_SOURCE_PLANE1,
	INTEL_PIPE_CRC_SOURCE_PLANE2,
	INTEL_PIPE_CRC_SOURCE_PF,
	INTEL_PIPE_CRC_SOURCE_PIPE,
	/* TV/DP on pre-gen5/vlv can't use the pipe source. */
	INTEL_PIPE_CRC_SOURCE_TV,
	INTEL_PIPE_CRC_SOURCE_DP_B,
	INTEL_PIPE_CRC_SOURCE_DP_C,
	INTEL_PIPE_CRC_SOURCE_DP_D,
	INTEL_PIPE_CRC_SOURCE_AUTO,
	INTEL_PIPE_CRC_SOURCE_MAX,
};

struct intel_pipe_crc_entry {
	uint32_t frame;
	uint32_t crc[5];
};

#define INTEL_PIPE_CRC_ENTRIES_NR	128
struct intel_pipe_crc {
	spinlock_t lock;
	bool opened;		/* exclusive access to the result file */
	struct intel_pipe_crc_entry *entries;
	enum intel_pipe_crc_source source;
	int head, tail;
	wait_queue_head_t wq;
};

struct i915_frontbuffer_tracking {
	struct mutex lock;

	/*
	 * Tracking bits for delayed frontbuffer flushing du to gpu activity or
	 * scheduled flips.
	 */
	unsigned busy_bits;
	unsigned flip_bits;
};

struct drm_i915_private {
	struct drm_device *dev;
	struct kmem_cache *slab;

	const struct intel_device_info info;
>>>>>>> v3.18

	int relative_constants_mode;

	void __iomem *regs;

<<<<<<< HEAD
	struct drm_i915_gt_funcs gt;
	/** gt_fifo_count and the subsequent register write are synchronized
	 * with dev->struct_mutex. */
	unsigned gt_fifo_count;
	/** forcewake_count is protected by gt_lock */
	unsigned forcewake_count;
	/** gt_lock is also taken in irq contexts. */
	spinlock_t gt_lock;
=======
	struct intel_uncore uncore;
>>>>>>> v3.18

	struct intel_gmbus gmbus[GMBUS_NUM_PORTS];


	/** gmbus_mutex protects against concurrent usage of the single hw gmbus
	 * controller on different i2c buses. */
	struct mutex gmbus_mutex;

	/**
	 * Base address of the gmbus and gpio block.
	 */
	uint32_t gpio_mmio_base;

<<<<<<< HEAD
	wait_queue_head_t gmbus_wait_queue;

	struct pci_dev *bridge_dev;
	struct intel_ring_buffer ring[I915_NUM_RINGS];
	uint32_t last_seqno, next_seqno;

	drm_dma_handle_t *status_page_dmah;
	struct resource mch_res;

	atomic_t irq_received;

	/* protects the irq masks */
	spinlock_t irq_lock;

=======
	/* MMIO base address for MIPI regs */
	uint32_t mipi_mmio_base;

	wait_queue_head_t gmbus_wait_queue;

	struct pci_dev *bridge_dev;
	struct intel_engine_cs ring[I915_NUM_RINGS];
	struct drm_i915_gem_object *semaphore_obj;
	uint32_t last_seqno, next_seqno;

	struct drm_dma_handle *status_page_dmah;
	struct resource mch_res;

	/* protects the irq masks */
	spinlock_t irq_lock;

	/* protects the mmio flip data */
	spinlock_t mmio_flip_lock;

	bool display_irqs_enabled;

>>>>>>> v3.18
	/* To control wakeup latency, e.g. for irq-driven dp aux transfers. */
	struct pm_qos_request pm_qos;

	/* DPIO indirect register protection */
	struct mutex dpio_lock;

	/** Cached value of IMR to avoid reads in updating the bitfield */
<<<<<<< HEAD
	u32 irq_mask;
	u32 gt_irq_mask;

	struct work_struct hotplug_work;
	bool enable_hotplug_processing;
=======
	union {
		u32 irq_mask;
		u32 de_irq_mask[I915_MAX_PIPES];
	};
	u32 gt_irq_mask;
	u32 pm_irq_mask;
	u32 pm_rps_events;
	u32 pipestat_irq_mask[I915_MAX_PIPES];

	struct work_struct hotplug_work;
>>>>>>> v3.18
	struct {
		unsigned long hpd_last_jiffies;
		int hpd_cnt;
		enum {
			HPD_ENABLED = 0,
			HPD_DISABLED = 1,
			HPD_MARK_DISABLED = 2
		} hpd_mark;
	} hpd_stats[HPD_NUM_PINS];
<<<<<<< HEAD
	struct timer_list hotplug_reenable_timer;

	int num_pch_pll;
	int num_plane;

	unsigned long cfb_size;
	unsigned int cfb_fb;
	enum plane cfb_plane;
	int cfb_y;
	struct intel_fbc_work *fbc_work;

	struct intel_opregion opregion;

	/* overlay */
	struct intel_overlay *overlay;
	unsigned int sprite_scaling_enabled;

	/* backlight */
	struct {
		int level;
		bool enabled;
		struct backlight_device *device;
	} backlight;

	/* LVDS info */
	struct drm_display_mode *lfp_lvds_vbt_mode; /* if any */
	struct drm_display_mode *sdvo_lvds_vbt_mode; /* if any */

	/* Feature bits from the VBIOS */
	unsigned int int_tv_support:1;
	unsigned int lvds_dither:1;
	unsigned int lvds_vbt:1;
	unsigned int int_crt_support:1;
	unsigned int lvds_use_ssc:1;
	unsigned int display_clock_mode:1;
	unsigned int fdi_rx_polarity_inverted:1;
	int lvds_ssc_freq;
	unsigned int bios_lvds_val; /* initial [PCH_]LVDS reg val in VBIOS */
	struct {
		int rate;
		int lanes;
		int preemphasis;
		int vswing;

		bool initialized;
		bool support;
		int bpp;
		struct edp_power_seq pps;
	} edp;
	bool no_aux_handshake;

	int crt_ddc_pin;
=======
	u32 hpd_event_bits;
	struct delayed_work hotplug_reenable_work;

	struct i915_fbc fbc;
	struct i915_drrs drrs;
	struct intel_opregion opregion;
	struct intel_vbt_data vbt;

	/* overlay */
	struct intel_overlay *overlay;

	/* backlight registers and fields in struct intel_panel */
	spinlock_t backlight_lock;

	/* LVDS info */
	bool no_aux_handshake;

	/* protects panel power sequencer state */
	struct mutex pps_mutex;

>>>>>>> v3.18
	struct drm_i915_fence_reg fence_regs[I915_MAX_NUM_FENCES]; /* assume 965 */
	int fence_reg_start; /* 4 if userland hasn't ioctl'd us yet */
	int num_fence_regs; /* 8 on pre-965, 16 otherwise */

	unsigned int fsb_freq, mem_freq, is_ddr3;
<<<<<<< HEAD

=======
	unsigned int vlv_cdclk_freq;

	/**
	 * wq - Driver workqueue for GEM.
	 *
	 * NOTE: Work items scheduled here are not allowed to grab any modeset
	 * locks, for otherwise the flushing done in the pageflip code will
	 * result in deadlocks.
	 */
>>>>>>> v3.18
	struct workqueue_struct *wq;

	/* Display functions */
	struct drm_i915_display_funcs display;

	/* PCH chipset type */
	enum intel_pch pch_type;
	unsigned short pch_id;

	unsigned long quirks;

	enum modeset_restore modeset_restore;
	struct mutex modeset_restore_lock;

<<<<<<< HEAD
	struct i915_gtt gtt;

	struct i915_gem_mm mm;
=======
	struct list_head vm_list; /* Global list of all address spaces */
	struct i915_gtt gtt; /* VM representing the global address space */

	struct i915_gem_mm mm;
	DECLARE_HASHTABLE(mm_structs, 7);
	struct mutex mm_lock;
>>>>>>> v3.18

	/* Kernel Modesetting */

	struct sdvo_device_mapping sdvo_mappings[2];
<<<<<<< HEAD
	/* indicate whether the LVDS_BORDER should be enabled or not */
	unsigned int lvds_border_bits;
	/* Panel fitter placement and size for Ironlake+ */
	u32 pch_pf_pos, pch_pf_size;

	struct drm_crtc *plane_to_crtc_mapping[3];
	struct drm_crtc *pipe_to_crtc_mapping[3];
	wait_queue_head_t pending_flip_queue;

	struct intel_pch_pll pch_plls[I915_NUM_PLLS];
	struct intel_ddi_plls ddi_plls;
=======

	struct drm_crtc *plane_to_crtc_mapping[I915_MAX_PIPES];
	struct drm_crtc *pipe_to_crtc_mapping[I915_MAX_PIPES];
	wait_queue_head_t pending_flip_queue;

#ifdef CONFIG_DEBUG_FS
	struct intel_pipe_crc pipe_crc[I915_MAX_PIPES];
#endif

	int num_shared_dpll;
	struct intel_shared_dpll shared_dplls[I915_NUM_PLLS];
	int dpio_phy_iosf_port[I915_NUM_PHYS_VLV];

	/*
	 * workarounds are currently applied at different places and
	 * changes are being done to consolidate them so exact count is
	 * not clear at this point, use a max value for now.
	 */
#define I915_MAX_WA_REGS  16
	struct {
		u32 addr;
		u32 value;
		/* bitmask representing WA bits */
		u32 mask;
	} intel_wa_regs[I915_MAX_WA_REGS];
	u32 num_wa_regs;
>>>>>>> v3.18

	/* Reclocking support */
	bool render_reclock_avail;
	bool lvds_downclock_avail;
	/* indicates the reduced downclock for LVDS*/
	int lvds_downclock;
<<<<<<< HEAD
	u16 orig_clock;
	int child_dev_num;
	struct child_device_config *child_dev;
=======

	struct i915_frontbuffer_tracking fb_tracking;

	u16 orig_clock;
>>>>>>> v3.18

	bool mchbar_need_disable;

	struct intel_l3_parity l3_parity;

<<<<<<< HEAD
=======
	/* Cannot be determined by PCIID. You must always read a register. */
	size_t ellc_size;

>>>>>>> v3.18
	/* gen6+ rps state */
	struct intel_gen6_power_mgmt rps;

	/* ilk-only ips/rps state. Everything in here is protected by the global
	 * mchdev_lock in intel_pm.c */
	struct intel_ilk_power_mgmt ips;

<<<<<<< HEAD
	enum no_fbc_reason no_fbc_reason;

	struct drm_mm_node *compressed_fb;
	struct drm_mm_node *compressed_llb;

	struct i915_gpu_error gpu_error;

	/* list of fbdev register on this device */
	struct intel_fbdev *fbdev;

	/*
	 * The console may be contended at resume, but we don't
	 * want it to block on it.
	 */
	struct work_struct console_resume_work;
=======
	struct i915_power_domains power_domains;

	struct i915_psr psr;

	struct i915_gpu_error gpu_error;

	struct drm_i915_gem_object *vlv_pctx;

#ifdef CONFIG_DRM_I915_FBDEV
	/* list of fbdev register on this device */
	struct intel_fbdev *fbdev;
	struct work_struct fbdev_suspend_work;
#endif
>>>>>>> v3.18

	struct drm_property *broadcast_rgb_property;
	struct drm_property *force_audio_property;

<<<<<<< HEAD
	bool hw_contexts_disabled;
	uint32_t hw_context_size;

	u32 fdi_rx_config;

	struct i915_suspend_saved_registers regfile;
=======
	uint32_t hw_context_size;
	struct list_head context_list;

	u32 fdi_rx_config;

	u32 suspend_count;
	struct i915_suspend_saved_registers regfile;
	struct vlv_s0ix_state vlv_s0ix_state;

	struct {
		/*
		 * Raw watermark latency values:
		 * in 0.1us units for WM0,
		 * in 0.5us units for WM1+.
		 */
		/* primary */
		uint16_t pri_latency[5];
		/* sprite */
		uint16_t spr_latency[5];
		/* cursor */
		uint16_t cur_latency[5];

		/* current hardware state */
		struct ilk_wm_values hw;
	} wm;

	struct i915_runtime_pm pm;

	struct intel_digital_port *hpd_irq_port[I915_MAX_PORTS];
	u32 long_hpd_port_mask;
	u32 short_hpd_port_mask;
	struct work_struct dig_port_work;

	/*
	 * if we get a HPD irq from DP and a HPD irq from non-DP
	 * the non-DP HPD could block the workqueue on a mode config
	 * mutex getting, that userspace may have taken. However
	 * userspace is waiting on the DP workqueue to run which is
	 * blocked behind the non-DP one.
	 */
	struct workqueue_struct *dp_wq;

	uint32_t bios_vgacntr;
>>>>>>> v3.18

	/* Old dri1 support infrastructure, beware the dragons ya fools entering
	 * here! */
	struct i915_dri1_state dri1;
<<<<<<< HEAD
} drm_i915_private_t;
=======
	/* Old ums support infrastructure, same warning applies. */
	struct i915_ums_state ums;

	/* Abstract the submission mechanism (legacy ringbuffer or execlists) away */
	struct {
		int (*do_execbuf)(struct drm_device *dev, struct drm_file *file,
				  struct intel_engine_cs *ring,
				  struct intel_context *ctx,
				  struct drm_i915_gem_execbuffer2 *args,
				  struct list_head *vmas,
				  struct drm_i915_gem_object *batch_obj,
				  u64 exec_start, u32 flags);
		int (*init_rings)(struct drm_device *dev);
		void (*cleanup_ring)(struct intel_engine_cs *ring);
		void (*stop_ring)(struct intel_engine_cs *ring);
	} gt;

	/*
	 * NOTE: This is the dri1/ums dungeon, don't add stuff here. Your patch
	 * will be rejected. Instead look for a better place.
	 */
};

static inline struct drm_i915_private *to_i915(const struct drm_device *dev)
{
	return dev->dev_private;
}
>>>>>>> v3.18

/* Iterate over initialised rings */
#define for_each_ring(ring__, dev_priv__, i__) \
	for ((i__) = 0; (i__) < I915_NUM_RINGS; (i__)++) \
		if (((ring__) = &(dev_priv__)->ring[(i__)]), intel_ring_initialized((ring__)))

enum hdmi_force_audio {
	HDMI_AUDIO_OFF_DVI = -2,	/* no aux data for HDMI-DVI converter */
	HDMI_AUDIO_OFF,			/* force turn off HDMI audio */
	HDMI_AUDIO_AUTO,		/* trust EDID */
	HDMI_AUDIO_ON,			/* force turn on HDMI audio */
};

<<<<<<< HEAD
#define I915_GTT_RESERVED ((struct drm_mm_node *)0x1)
=======
#define I915_GTT_OFFSET_NONE ((u32)-1)
>>>>>>> v3.18

struct drm_i915_gem_object_ops {
	/* Interface between the GEM object and its backing storage.
	 * get_pages() is called once prior to the use of the associated set
	 * of pages before to binding them into the GTT, and put_pages() is
	 * called after we no longer need them. As we expect there to be
	 * associated cost with migrating pages between the backing storage
	 * and making them available for the GPU (e.g. clflush), we may hold
	 * onto the pages after they are no longer referenced by the GPU
	 * in case they may be used again shortly (for example migrating the
	 * pages to a different memory domain within the GTT). put_pages()
	 * will therefore most likely be called when the object itself is
	 * being released or under memory pressure (where we attempt to
	 * reap pages for the shrinker).
	 */
	int (*get_pages)(struct drm_i915_gem_object *);
	void (*put_pages)(struct drm_i915_gem_object *);
<<<<<<< HEAD
};

=======
	int (*dmabuf_export)(struct drm_i915_gem_object *);
	void (*release)(struct drm_i915_gem_object *);
};

/*
 * Frontbuffer tracking bits. Set in obj->frontbuffer_bits while a gem bo is
 * considered to be the frontbuffer for the given plane interface-vise. This
 * doesn't mean that the hw necessarily already scans it out, but that any
 * rendering (by the cpu or gpu) will land in the frontbuffer eventually.
 *
 * We have one bit per pipe and per scanout plane type.
 */
#define INTEL_FRONTBUFFER_BITS_PER_PIPE 4
#define INTEL_FRONTBUFFER_BITS \
	(INTEL_FRONTBUFFER_BITS_PER_PIPE * I915_MAX_PIPES)
#define INTEL_FRONTBUFFER_PRIMARY(pipe) \
	(1 << (INTEL_FRONTBUFFER_BITS_PER_PIPE * (pipe)))
#define INTEL_FRONTBUFFER_CURSOR(pipe) \
	(1 << (1 +(INTEL_FRONTBUFFER_BITS_PER_PIPE * (pipe))))
#define INTEL_FRONTBUFFER_SPRITE(pipe) \
	(1 << (2 +(INTEL_FRONTBUFFER_BITS_PER_PIPE * (pipe))))
#define INTEL_FRONTBUFFER_OVERLAY(pipe) \
	(1 << (3 +(INTEL_FRONTBUFFER_BITS_PER_PIPE * (pipe))))
#define INTEL_FRONTBUFFER_ALL_MASK(pipe) \
	(0xf << (INTEL_FRONTBUFFER_BITS_PER_PIPE * (pipe)))

>>>>>>> v3.18
struct drm_i915_gem_object {
	struct drm_gem_object base;

	const struct drm_i915_gem_object_ops *ops;

<<<<<<< HEAD
	/** Current space allocated to this object in the GTT, if any. */
	struct drm_mm_node *gtt_space;
	/** Stolen memory for this object, instead of being backed by shmem. */
	struct drm_mm_node *stolen;
	struct list_head gtt_list;

	/** This object's place on the active/inactive lists */
	struct list_head ring_list;
	struct list_head mm_list;
	/** This object's place in the batchbuffer or on the eviction list */
	struct list_head exec_list;
=======
	/** List of VMAs backed by this object */
	struct list_head vma_list;

	/** Stolen memory for this object, instead of being backed by shmem. */
	struct drm_mm_node *stolen;
	struct list_head global_list;

	struct list_head ring_list;
	/** Used in execbuf to temporarily hold a ref */
	struct list_head obj_exec_link;
>>>>>>> v3.18

	/**
	 * This is set if the object is on the active lists (has pending
	 * rendering and so a non-zero seqno), and is not set if it i s on
	 * inactive (ready to be unbound) list.
	 */
	unsigned int active:1;

	/**
	 * This is set if the object has been written to since last bound
	 * to the GTT
	 */
	unsigned int dirty:1;

	/**
	 * Fence register bits (if any) for this object.  Will be set
	 * as needed when mapped into the GTT.
	 * Protected by dev->struct_mutex.
	 */
	signed int fence_reg:I915_MAX_NUM_FENCE_BITS;

	/**
	 * Advice: are the backing pages purgeable?
	 */
	unsigned int madv:2;

	/**
	 * Current tiling mode for the object.
	 */
	unsigned int tiling_mode:2;
	/**
	 * Whether the tiling parameters for the currently associated fence
	 * register have changed. Note that for the purposes of tracking
	 * tiling changes we also treat the unfenced register, the register
	 * slot that the object occupies whilst it executes a fenced
	 * command (such as BLT on gen2/3), as a "fence".
	 */
	unsigned int fence_dirty:1;

<<<<<<< HEAD
	/** How many users have pinned this object in GTT space. The following
	 * users can each hold at most one reference: pwrite/pread, pin_ioctl
	 * (via user_pin_count), execbuffer (objects are not allowed multiple
	 * times for the same batchbuffer), and the framebuffer code. When
	 * switching/pageflipping, the framebuffer code has at most two buffers
	 * pinned per crtc.
	 *
	 * In the worst case this is 1 + 1 + 1 + 2*2 = 7. That would fit into 3
	 * bits with absolutely no headroom. So use 4 bits. */
	unsigned int pin_count:4;
#define DRM_I915_GEM_OBJECT_MAX_PIN_COUNT 0xf

=======
>>>>>>> v3.18
	/**
	 * Is the object at the current location in the gtt mappable and
	 * fenceable? Used to avoid costly recalculations.
	 */
	unsigned int map_and_fenceable:1;

	/**
	 * Whether the current gtt mapping needs to be mappable (and isn't just
	 * mappable by accident). Track pin and fault separate for a more
	 * accurate mappable working set.
	 */
	unsigned int fault_mappable:1;
	unsigned int pin_mappable:1;
<<<<<<< HEAD

	/*
	 * Is the GPU currently using a fence to access this buffer,
	 */
	unsigned int pending_fenced_gpu_access:1;
	unsigned int fenced_gpu_access:1;

	unsigned int cache_level:2;
=======
	unsigned int pin_display:1;

	/*
	 * Is the object to be mapped as read-only to the GPU
	 * Only honoured if hardware has relevant pte bit
	 */
	unsigned long gt_ro:1;
	unsigned int cache_level:3;
>>>>>>> v3.18

	unsigned int has_aliasing_ppgtt_mapping:1;
	unsigned int has_global_gtt_mapping:1;
	unsigned int has_dma_mapping:1;

<<<<<<< HEAD
=======
	unsigned int frontbuffer_bits:INTEL_FRONTBUFFER_BITS;

>>>>>>> v3.18
	struct sg_table *pages;
	int pages_pin_count;

	/* prime dma-buf support */
	void *dma_buf_vmapping;
	int vmapping_count;

<<<<<<< HEAD
	/**
	 * Used for performing relocations during execbuffer insertion.
	 */
	struct hlist_node exec_node;
	unsigned long exec_handle;
	struct drm_i915_gem_exec_object2 *exec_entry;

	/**
	 * Current offset of the object in GTT space.
	 *
	 * This is the same as gtt_space->start
	 */
	uint32_t gtt_offset;

	struct intel_ring_buffer *ring;
=======
	struct intel_engine_cs *ring;
>>>>>>> v3.18

	/** Breadcrumb of last rendering to the buffer. */
	uint32_t last_read_seqno;
	uint32_t last_write_seqno;
	/** Breadcrumb of last fenced GPU access to the buffer. */
	uint32_t last_fenced_seqno;

	/** Current tiling stride for the object, if it's tiled. */
	uint32_t stride;

<<<<<<< HEAD
=======
	/** References from framebuffers, locks out tiling changes. */
	unsigned long framebuffer_references;

>>>>>>> v3.18
	/** Record of address bit 17 of each page at last unbind. */
	unsigned long *bit_17;

	/** User space pin count and filp owning the pin */
<<<<<<< HEAD
	uint32_t user_pin_count;
	struct drm_file *pin_filp;

	/** for phy allocated objects */
	struct drm_i915_gem_phys_object *phys_obj;
};
#define to_gem_object(obj) (&((struct drm_i915_gem_object *)(obj))->base)

#define to_intel_bo(x) container_of(x, struct drm_i915_gem_object, base)

=======
	unsigned long user_pin_count;
	struct drm_file *pin_filp;

	/** for phy allocated objects */
	struct drm_dma_handle *phys_handle;

	union {
		struct i915_gem_userptr {
			uintptr_t ptr;
			unsigned read_only :1;
			unsigned workers :4;
#define I915_GEM_USERPTR_MAX_WORKERS 15

			struct i915_mm_struct *mm;
			struct i915_mmu_object *mmu_object;
			struct work_struct *work;
		} userptr;
	};
};
#define to_intel_bo(x) container_of(x, struct drm_i915_gem_object, base)

void i915_gem_track_fb(struct drm_i915_gem_object *old,
		       struct drm_i915_gem_object *new,
		       unsigned frontbuffer_bits);

>>>>>>> v3.18
/**
 * Request queue structure.
 *
 * The request queue allows us to note sequence numbers that have been emitted
 * and may be associated with active buffers to be retired.
 *
 * By keeping this list, we can avoid having to do questionable
 * sequence-number comparisons on buffer last_rendering_seqnos, and associate
 * an emission time with seqnos for tracking how far ahead of the GPU we are.
 */
struct drm_i915_gem_request {
	/** On Which ring this request was generated */
<<<<<<< HEAD
	struct intel_ring_buffer *ring;
=======
	struct intel_engine_cs *ring;
>>>>>>> v3.18

	/** GEM sequence number associated with this request. */
	uint32_t seqno;

<<<<<<< HEAD
	/** Postion in the ringbuffer of the end of the request */
	u32 tail;

=======
	/** Position in the ringbuffer of the start of the request */
	u32 head;

	/** Position in the ringbuffer of the end of the request */
	u32 tail;

	/** Context related to this request */
	struct intel_context *ctx;

	/** Batch buffer related to this request if any */
	struct drm_i915_gem_object *batch_obj;

>>>>>>> v3.18
	/** Time at which this request was emitted, in jiffies. */
	unsigned long emitted_jiffies;

	/** global list entry for this request */
	struct list_head list;

	struct drm_i915_file_private *file_priv;
	/** file_priv list entry for this request */
	struct list_head client_list;
};

struct drm_i915_file_private {
<<<<<<< HEAD
	struct {
		spinlock_t lock;
		struct list_head request_list;
	} mm;
	struct idr context_idr;
};

#define INTEL_INFO(dev)	(((struct drm_i915_private *) (dev)->dev_private)->info)

#define IS_I830(dev)		((dev)->pci_device == 0x3577)
#define IS_845G(dev)		((dev)->pci_device == 0x2562)
#define IS_I85X(dev)		(INTEL_INFO(dev)->is_i85x)
#define IS_I865G(dev)		((dev)->pci_device == 0x2572)
#define IS_I915G(dev)		(INTEL_INFO(dev)->is_i915g)
#define IS_I915GM(dev)		((dev)->pci_device == 0x2592)
#define IS_I945G(dev)		((dev)->pci_device == 0x2772)
#define IS_I945GM(dev)		(INTEL_INFO(dev)->is_i945gm)
#define IS_BROADWATER(dev)	(INTEL_INFO(dev)->is_broadwater)
#define IS_CRESTLINE(dev)	(INTEL_INFO(dev)->is_crestline)
#define IS_GM45(dev)		((dev)->pci_device == 0x2A42)
#define IS_G4X(dev)		(INTEL_INFO(dev)->is_g4x)
#define IS_PINEVIEW_G(dev)	((dev)->pci_device == 0xa001)
#define IS_PINEVIEW_M(dev)	((dev)->pci_device == 0xa011)
#define IS_PINEVIEW(dev)	(INTEL_INFO(dev)->is_pineview)
#define IS_G33(dev)		(INTEL_INFO(dev)->is_g33)
#define IS_IRONLAKE_D(dev)	((dev)->pci_device == 0x0042)
#define IS_IRONLAKE_M(dev)	((dev)->pci_device == 0x0046)
#define IS_IVYBRIDGE(dev)	(INTEL_INFO(dev)->is_ivybridge)
#define IS_IVB_GT1(dev)		((dev)->pci_device == 0x0156 || \
				 (dev)->pci_device == 0x0152 ||	\
				 (dev)->pci_device == 0x015a)
#define IS_SNB_GT1(dev)		((dev)->pci_device == 0x0102 || \
				 (dev)->pci_device == 0x0106 ||	\
				 (dev)->pci_device == 0x010A)
#define IS_VALLEYVIEW(dev)	(INTEL_INFO(dev)->is_valleyview)
#define IS_HASWELL(dev)	(INTEL_INFO(dev)->is_haswell)
#define IS_MOBILE(dev)		(INTEL_INFO(dev)->is_mobile)
#define IS_ULT(dev)		(IS_HASWELL(dev) && \
				 ((dev)->pci_device & 0xFF00) == 0x0A00)
=======
	struct drm_i915_private *dev_priv;
	struct drm_file *file;

	struct {
		spinlock_t lock;
		struct list_head request_list;
		struct delayed_work idle_work;
	} mm;
	struct idr context_idr;

	atomic_t rps_wait_boost;
	struct  intel_engine_cs *bsd_ring;
};

/*
 * A command that requires special handling by the command parser.
 */
struct drm_i915_cmd_descriptor {
	/*
	 * Flags describing how the command parser processes the command.
	 *
	 * CMD_DESC_FIXED: The command has a fixed length if this is set,
	 *                 a length mask if not set
	 * CMD_DESC_SKIP: The command is allowed but does not follow the
	 *                standard length encoding for the opcode range in
	 *                which it falls
	 * CMD_DESC_REJECT: The command is never allowed
	 * CMD_DESC_REGISTER: The command should be checked against the
	 *                    register whitelist for the appropriate ring
	 * CMD_DESC_MASTER: The command is allowed if the submitting process
	 *                  is the DRM master
	 */
	u32 flags;
#define CMD_DESC_FIXED    (1<<0)
#define CMD_DESC_SKIP     (1<<1)
#define CMD_DESC_REJECT   (1<<2)
#define CMD_DESC_REGISTER (1<<3)
#define CMD_DESC_BITMASK  (1<<4)
#define CMD_DESC_MASTER   (1<<5)

	/*
	 * The command's unique identification bits and the bitmask to get them.
	 * This isn't strictly the opcode field as defined in the spec and may
	 * also include type, subtype, and/or subop fields.
	 */
	struct {
		u32 value;
		u32 mask;
	} cmd;

	/*
	 * The command's length. The command is either fixed length (i.e. does
	 * not include a length field) or has a length field mask. The flag
	 * CMD_DESC_FIXED indicates a fixed length. Otherwise, the command has
	 * a length mask. All command entries in a command table must include
	 * length information.
	 */
	union {
		u32 fixed;
		u32 mask;
	} length;

	/*
	 * Describes where to find a register address in the command to check
	 * against the ring's register whitelist. Only valid if flags has the
	 * CMD_DESC_REGISTER bit set.
	 */
	struct {
		u32 offset;
		u32 mask;
	} reg;

#define MAX_CMD_DESC_BITMASKS 3
	/*
	 * Describes command checks where a particular dword is masked and
	 * compared against an expected value. If the command does not match
	 * the expected value, the parser rejects it. Only valid if flags has
	 * the CMD_DESC_BITMASK bit set. Only entries where mask is non-zero
	 * are valid.
	 *
	 * If the check specifies a non-zero condition_mask then the parser
	 * only performs the check when the bits specified by condition_mask
	 * are non-zero.
	 */
	struct {
		u32 offset;
		u32 mask;
		u32 expected;
		u32 condition_offset;
		u32 condition_mask;
	} bits[MAX_CMD_DESC_BITMASKS];
};

/*
 * A table of commands requiring special handling by the command parser.
 *
 * Each ring has an array of tables. Each table consists of an array of command
 * descriptors, which must be sorted with command opcodes in ascending order.
 */
struct drm_i915_cmd_table {
	const struct drm_i915_cmd_descriptor *table;
	int count;
};

/* Note that the (struct drm_i915_private *) cast is just to shut up gcc. */
#define __I915__(p) ({ \
	struct drm_i915_private *__p; \
	if (__builtin_types_compatible_p(typeof(*p), struct drm_i915_private)) \
		__p = (struct drm_i915_private *)p; \
	else if (__builtin_types_compatible_p(typeof(*p), struct drm_device)) \
		__p = to_i915((struct drm_device *)p); \
	else \
		BUILD_BUG(); \
	__p; \
})
#define INTEL_INFO(p) 	(&__I915__(p)->info)
#define INTEL_DEVID(p)	(INTEL_INFO(p)->device_id)

#define IS_I830(dev)		(INTEL_DEVID(dev) == 0x3577)
#define IS_845G(dev)		(INTEL_DEVID(dev) == 0x2562)
#define IS_I85X(dev)		(INTEL_INFO(dev)->is_i85x)
#define IS_I865G(dev)		(INTEL_DEVID(dev) == 0x2572)
#define IS_I915G(dev)		(INTEL_INFO(dev)->is_i915g)
#define IS_I915GM(dev)		(INTEL_DEVID(dev) == 0x2592)
#define IS_I945G(dev)		(INTEL_DEVID(dev) == 0x2772)
#define IS_I945GM(dev)		(INTEL_INFO(dev)->is_i945gm)
#define IS_BROADWATER(dev)	(INTEL_INFO(dev)->is_broadwater)
#define IS_CRESTLINE(dev)	(INTEL_INFO(dev)->is_crestline)
#define IS_GM45(dev)		(INTEL_DEVID(dev) == 0x2A42)
#define IS_G4X(dev)		(INTEL_INFO(dev)->is_g4x)
#define IS_PINEVIEW_G(dev)	(INTEL_DEVID(dev) == 0xa001)
#define IS_PINEVIEW_M(dev)	(INTEL_DEVID(dev) == 0xa011)
#define IS_PINEVIEW(dev)	(INTEL_INFO(dev)->is_pineview)
#define IS_G33(dev)		(INTEL_INFO(dev)->is_g33)
#define IS_IRONLAKE_M(dev)	(INTEL_DEVID(dev) == 0x0046)
#define IS_IVYBRIDGE(dev)	(INTEL_INFO(dev)->is_ivybridge)
#define IS_IVB_GT1(dev)		(INTEL_DEVID(dev) == 0x0156 || \
				 INTEL_DEVID(dev) == 0x0152 || \
				 INTEL_DEVID(dev) == 0x015a)
#define IS_SNB_GT1(dev)		(INTEL_DEVID(dev) == 0x0102 || \
				 INTEL_DEVID(dev) == 0x0106 || \
				 INTEL_DEVID(dev) == 0x010A)
#define IS_VALLEYVIEW(dev)	(INTEL_INFO(dev)->is_valleyview)
#define IS_CHERRYVIEW(dev)	(INTEL_INFO(dev)->is_valleyview && IS_GEN8(dev))
#define IS_HASWELL(dev)	(INTEL_INFO(dev)->is_haswell)
#define IS_BROADWELL(dev)	(!INTEL_INFO(dev)->is_valleyview && IS_GEN8(dev))
#define IS_MOBILE(dev)		(INTEL_INFO(dev)->is_mobile)
#define IS_HSW_EARLY_SDV(dev)	(IS_HASWELL(dev) && \
				 (INTEL_DEVID(dev) & 0xFF00) == 0x0C00)
#define IS_BDW_ULT(dev)		(IS_BROADWELL(dev) && \
				 ((INTEL_DEVID(dev) & 0xf) == 0x2  || \
				 (INTEL_DEVID(dev) & 0xf) == 0x6 || \
				 (INTEL_DEVID(dev) & 0xf) == 0xe))
#define IS_HSW_ULT(dev)		(IS_HASWELL(dev) && \
				 (INTEL_DEVID(dev) & 0xFF00) == 0x0A00)
#define IS_ULT(dev)		(IS_HSW_ULT(dev) || IS_BDW_ULT(dev))
#define IS_HSW_GT3(dev)		(IS_HASWELL(dev) && \
				 (INTEL_DEVID(dev) & 0x00F0) == 0x0020)
/* ULX machines are also considered ULT. */
#define IS_HSW_ULX(dev)		(INTEL_DEVID(dev) == 0x0A0E || \
				 INTEL_DEVID(dev) == 0x0A1E)
#define IS_PRELIMINARY_HW(intel_info) ((intel_info)->is_preliminary)
>>>>>>> v3.18

/*
 * The genX designation typically refers to the render engine, so render
 * capability related checks should use IS_GEN, while display and other checks
 * have their own (e.g. HAS_PCH_SPLIT for ILK+ display, IS_foo for particular
 * chips, etc.).
 */
#define IS_GEN2(dev)	(INTEL_INFO(dev)->gen == 2)
#define IS_GEN3(dev)	(INTEL_INFO(dev)->gen == 3)
#define IS_GEN4(dev)	(INTEL_INFO(dev)->gen == 4)
#define IS_GEN5(dev)	(INTEL_INFO(dev)->gen == 5)
#define IS_GEN6(dev)	(INTEL_INFO(dev)->gen == 6)
#define IS_GEN7(dev)	(INTEL_INFO(dev)->gen == 7)
<<<<<<< HEAD

#define HAS_BSD(dev)            (INTEL_INFO(dev)->has_bsd_ring)
#define HAS_BLT(dev)            (INTEL_INFO(dev)->has_blt_ring)
#define HAS_LLC(dev)            (INTEL_INFO(dev)->has_llc)
#define I915_NEED_GFX_HWS(dev)	(INTEL_INFO(dev)->need_gfx_hws)

#define HAS_HW_CONTEXTS(dev)	(INTEL_INFO(dev)->gen >= 6)
#define HAS_ALIASING_PPGTT(dev)	(INTEL_INFO(dev)->gen >=6 && !IS_VALLEYVIEW(dev))
=======
#define IS_GEN8(dev)	(INTEL_INFO(dev)->gen == 8)

#define RENDER_RING		(1<<RCS)
#define BSD_RING		(1<<VCS)
#define BLT_RING		(1<<BCS)
#define VEBOX_RING		(1<<VECS)
#define BSD2_RING		(1<<VCS2)
#define HAS_BSD(dev)		(INTEL_INFO(dev)->ring_mask & BSD_RING)
#define HAS_BSD2(dev)		(INTEL_INFO(dev)->ring_mask & BSD2_RING)
#define HAS_BLT(dev)		(INTEL_INFO(dev)->ring_mask & BLT_RING)
#define HAS_VEBOX(dev)		(INTEL_INFO(dev)->ring_mask & VEBOX_RING)
#define HAS_LLC(dev)		(INTEL_INFO(dev)->has_llc)
#define HAS_WT(dev)		((IS_HASWELL(dev) || IS_BROADWELL(dev)) && \
				 to_i915(dev)->ellc_size)
#define I915_NEED_GFX_HWS(dev)	(INTEL_INFO(dev)->need_gfx_hws)

#define HAS_HW_CONTEXTS(dev)	(INTEL_INFO(dev)->gen >= 6)
#define HAS_LOGICAL_RING_CONTEXTS(dev)	(INTEL_INFO(dev)->gen >= 8)
#define HAS_ALIASING_PPGTT(dev)	(INTEL_INFO(dev)->gen >= 6)
#define HAS_PPGTT(dev)		(INTEL_INFO(dev)->gen >= 7 && !IS_GEN8(dev))
#define USES_PPGTT(dev)		(i915.enable_ppgtt)
#define USES_FULL_PPGTT(dev)	(i915.enable_ppgtt == 2)
>>>>>>> v3.18

#define HAS_OVERLAY(dev)		(INTEL_INFO(dev)->has_overlay)
#define OVERLAY_NEEDS_PHYSICAL(dev)	(INTEL_INFO(dev)->overlay_needs_physical)

/* Early gen2 have a totally busted CS tlb and require pinned batches. */
#define HAS_BROKEN_CS_TLB(dev)		(IS_I830(dev) || IS_845G(dev))
<<<<<<< HEAD
=======
/*
 * dp aux and gmbus irq on gen4 seems to be able to generate legacy interrupts
 * even when in MSI mode. This results in spurious interrupt warnings if the
 * legacy irq no. is shared with another device. The kernel then disables that
 * interrupt source and so prevents the other device from working properly.
 */
#define HAS_AUX_IRQ(dev) (INTEL_INFO(dev)->gen >= 5)
#define HAS_GMBUS_IRQ(dev) (INTEL_INFO(dev)->gen >= 5)
>>>>>>> v3.18

/* With the 945 and later, Y tiling got adjusted so that it was 32 128-byte
 * rows, which changed the alignment requirements and fence programming.
 */
#define HAS_128_BYTE_Y_TILING(dev) (!IS_GEN2(dev) && !(IS_I915G(dev) || \
						      IS_I915GM(dev)))
#define SUPPORTS_DIGITAL_OUTPUTS(dev)	(!IS_GEN2(dev) && !IS_PINEVIEW(dev))
#define SUPPORTS_INTEGRATED_HDMI(dev)	(IS_G4X(dev) || IS_GEN5(dev))
#define SUPPORTS_INTEGRATED_DP(dev)	(IS_G4X(dev) || IS_GEN5(dev))
<<<<<<< HEAD
#define SUPPORTS_EDP(dev)		(IS_IRONLAKE_M(dev))
#define SUPPORTS_TV(dev)		(INTEL_INFO(dev)->supports_tv)
#define I915_HAS_HOTPLUG(dev)		 (INTEL_INFO(dev)->has_hotplug)
/* dsparb controlled by hw only */
#define DSPARB_HWCONTROL(dev) (IS_G4X(dev) || IS_IRONLAKE(dev))

#define HAS_FW_BLC(dev) (INTEL_INFO(dev)->gen > 2)
#define HAS_PIPE_CXSR(dev) (INTEL_INFO(dev)->has_pipe_cxsr)
#define I915_HAS_FBC(dev) (INTEL_INFO(dev)->has_fbc)

#define HAS_PIPE_CONTROL(dev) (INTEL_INFO(dev)->gen >= 5)

#define HAS_DDI(dev)		(IS_HASWELL(dev))
#define HAS_POWER_WELL(dev)	(IS_HASWELL(dev))
=======
#define SUPPORTS_TV(dev)		(INTEL_INFO(dev)->supports_tv)
#define I915_HAS_HOTPLUG(dev)		 (INTEL_INFO(dev)->has_hotplug)

#define HAS_FW_BLC(dev) (INTEL_INFO(dev)->gen > 2)
#define HAS_PIPE_CXSR(dev) (INTEL_INFO(dev)->has_pipe_cxsr)
#define HAS_FBC(dev) (INTEL_INFO(dev)->has_fbc)

#define HAS_IPS(dev)		(IS_ULT(dev) || IS_BROADWELL(dev))

#define HAS_DDI(dev)		(INTEL_INFO(dev)->has_ddi)
#define HAS_FPGA_DBG_UNCLAIMED(dev)	(INTEL_INFO(dev)->has_fpga_dbg)
#define HAS_PSR(dev)		(IS_HASWELL(dev) || IS_BROADWELL(dev))
#define HAS_RUNTIME_PM(dev)	(IS_GEN6(dev) || IS_HASWELL(dev) || \
				 IS_BROADWELL(dev) || IS_VALLEYVIEW(dev))
>>>>>>> v3.18

#define INTEL_PCH_DEVICE_ID_MASK		0xff00
#define INTEL_PCH_IBX_DEVICE_ID_TYPE		0x3b00
#define INTEL_PCH_CPT_DEVICE_ID_TYPE		0x1c00
#define INTEL_PCH_PPT_DEVICE_ID_TYPE		0x1e00
#define INTEL_PCH_LPT_DEVICE_ID_TYPE		0x8c00
#define INTEL_PCH_LPT_LP_DEVICE_ID_TYPE		0x9c00

<<<<<<< HEAD
#define INTEL_PCH_TYPE(dev) (((struct drm_i915_private *)(dev)->dev_private)->pch_type)
=======
#define INTEL_PCH_TYPE(dev) (to_i915(dev)->pch_type)
>>>>>>> v3.18
#define HAS_PCH_LPT(dev) (INTEL_PCH_TYPE(dev) == PCH_LPT)
#define HAS_PCH_CPT(dev) (INTEL_PCH_TYPE(dev) == PCH_CPT)
#define HAS_PCH_IBX(dev) (INTEL_PCH_TYPE(dev) == PCH_IBX)
#define HAS_PCH_NOP(dev) (INTEL_PCH_TYPE(dev) == PCH_NOP)
#define HAS_PCH_SPLIT(dev) (INTEL_PCH_TYPE(dev) != PCH_NONE)

<<<<<<< HEAD
#define HAS_FORCE_WAKE(dev) (INTEL_INFO(dev)->has_force_wake)

#define HAS_L3_GPU_CACHE(dev) (IS_IVYBRIDGE(dev) || IS_HASWELL(dev))
=======
#define HAS_GMCH_DISPLAY(dev) (INTEL_INFO(dev)->gen < 5 || IS_VALLEYVIEW(dev))

/* DPF == dynamic parity feature */
#define HAS_L3_DPF(dev) (IS_IVYBRIDGE(dev) || IS_HASWELL(dev))
#define NUM_L3_SLICES(dev) (IS_HSW_GT3(dev) ? 2 : HAS_L3_DPF(dev))
>>>>>>> v3.18

#define GT_FREQUENCY_MULTIPLIER 50

#include "i915_trace.h"

<<<<<<< HEAD
/**
 * RC6 is a special power stage which allows the GPU to enter an very
 * low-voltage mode when idle, using down to 0V while at this stage.  This
 * stage is entered automatically when the GPU is idle when RC6 support is
 * enabled, and as soon as new workload arises GPU wakes up automatically as well.
 *
 * There are different RC6 modes available in Intel GPU, which differentiate
 * among each other with the latency required to enter and leave RC6 and
 * voltage consumed by the GPU in different states.
 *
 * The combination of the following flags define which states GPU is allowed
 * to enter, while RC6 is the normal RC6 state, RC6p is the deep RC6, and
 * RC6pp is deepest RC6. Their support by hardware varies according to the
 * GPU, BIOS, chipset and platform. RC6 is usually the safest one and the one
 * which brings the most power savings; deeper states save more power, but
 * require higher latency to switch to and wake up.
 */
#define INTEL_RC6_ENABLE			(1<<0)
#define INTEL_RC6p_ENABLE			(1<<1)
#define INTEL_RC6pp_ENABLE			(1<<2)

extern struct drm_ioctl_desc i915_ioctls[];
extern int i915_max_ioctl;
extern unsigned int i915_fbpercrtc __always_unused;
extern int i915_panel_ignore_lid __read_mostly;
extern unsigned int i915_powersave __read_mostly;
extern int i915_semaphores __read_mostly;
extern unsigned int i915_lvds_downclock __read_mostly;
extern int i915_lvds_channel_mode __read_mostly;
extern int i915_panel_use_ssc __read_mostly;
extern int i915_vbt_sdvo_panel_type __read_mostly;
extern int i915_enable_rc6 __read_mostly;
extern int i915_enable_fbc __read_mostly;
extern bool i915_enable_hangcheck __read_mostly;
extern int i915_enable_ppgtt __read_mostly;
extern unsigned int i915_preliminary_hw_support __read_mostly;
extern int i915_disable_power_well __read_mostly;
=======
extern const struct drm_ioctl_desc i915_ioctls[];
extern int i915_max_ioctl;
>>>>>>> v3.18

extern int i915_suspend(struct drm_device *dev, pm_message_t state);
extern int i915_resume(struct drm_device *dev);
extern int i915_master_create(struct drm_device *dev, struct drm_master *master);
extern void i915_master_destroy(struct drm_device *dev, struct drm_master *master);

<<<<<<< HEAD
=======
/* i915_params.c */
struct i915_params {
	int modeset;
	int panel_ignore_lid;
	unsigned int powersave;
	int semaphores;
	unsigned int lvds_downclock;
	int lvds_channel_mode;
	int panel_use_ssc;
	int vbt_sdvo_panel_type;
	int enable_rc6;
	int enable_fbc;
	int enable_ppgtt;
	int enable_execlists;
	int enable_psr;
	unsigned int preliminary_hw_support;
	int disable_power_well;
	int enable_ips;
	int invert_brightness;
	int enable_cmd_parser;
	/* leave bools at the end to not create holes */
	bool enable_hangcheck;
	bool fastboot;
	bool prefault_disable;
	bool reset;
	bool disable_display;
	bool disable_vtd_wa;
	int use_mmio_flip;
	bool mmio_debug;
};
extern struct i915_params i915 __read_mostly;

>>>>>>> v3.18
				/* i915_dma.c */
void i915_update_dri1_breadcrumb(struct drm_device *dev);
extern void i915_kernel_lost_context(struct drm_device * dev);
extern int i915_driver_load(struct drm_device *, unsigned long flags);
extern int i915_driver_unload(struct drm_device *);
<<<<<<< HEAD
extern int i915_driver_open(struct drm_device *dev, struct drm_file *file_priv);
extern void i915_driver_lastclose(struct drm_device * dev);
extern void i915_driver_preclose(struct drm_device *dev,
				 struct drm_file *file_priv);
extern void i915_driver_postclose(struct drm_device *dev,
				  struct drm_file *file_priv);
=======
extern int i915_driver_open(struct drm_device *dev, struct drm_file *file);
extern void i915_driver_lastclose(struct drm_device * dev);
extern void i915_driver_preclose(struct drm_device *dev,
				 struct drm_file *file);
extern void i915_driver_postclose(struct drm_device *dev,
				  struct drm_file *file);
>>>>>>> v3.18
extern int i915_driver_device_is_agp(struct drm_device * dev);
#ifdef CONFIG_COMPAT
extern long i915_compat_ioctl(struct file *filp, unsigned int cmd,
			      unsigned long arg);
#endif
extern int i915_emit_box(struct drm_device *dev,
			 struct drm_clip_rect *box,
			 int DR1, int DR4);
extern int intel_gpu_reset(struct drm_device *dev);
extern int i915_reset(struct drm_device *dev);
extern unsigned long i915_chipset_val(struct drm_i915_private *dev_priv);
extern unsigned long i915_mch_val(struct drm_i915_private *dev_priv);
extern unsigned long i915_gfx_val(struct drm_i915_private *dev_priv);
extern void i915_update_gfx_val(struct drm_i915_private *dev_priv);
<<<<<<< HEAD

extern void intel_console_resume(struct work_struct *work);

/* i915_irq.c */
void i915_hangcheck_elapsed(unsigned long data);
void i915_handle_error(struct drm_device *dev, bool wedged);

extern void intel_irq_init(struct drm_device *dev);
extern void intel_pm_init(struct drm_device *dev);
extern void intel_hpd_init(struct drm_device *dev);
extern void intel_gt_init(struct drm_device *dev);
extern void intel_gt_sanitize(struct drm_device *dev);

void i915_error_state_free(struct kref *error_ref);

void
i915_enable_pipestat(drm_i915_private_t *dev_priv, int pipe, u32 mask);

void
i915_disable_pipestat(drm_i915_private_t *dev_priv, int pipe, u32 mask);

void intel_enable_asle(struct drm_device *dev);

#ifdef CONFIG_DEBUG_FS
extern void i915_destroy_error_state(struct drm_device *dev);
#else
#define i915_destroy_error_state(x)
#endif

=======
int vlv_force_gfx_clock(struct drm_i915_private *dev_priv, bool on);
void intel_hpd_cancel_work(struct drm_i915_private *dev_priv);

/* i915_irq.c */
void i915_queue_hangcheck(struct drm_device *dev);
__printf(3, 4)
void i915_handle_error(struct drm_device *dev, bool wedged,
		       const char *fmt, ...);

void gen6_set_pm_mask(struct drm_i915_private *dev_priv, u32 pm_iir,
							int new_delay);
extern void intel_irq_init(struct drm_device *dev);
extern void intel_hpd_init(struct drm_device *dev);

extern void intel_uncore_sanitize(struct drm_device *dev);
extern void intel_uncore_early_sanitize(struct drm_device *dev,
					bool restore_forcewake);
extern void intel_uncore_init(struct drm_device *dev);
extern void intel_uncore_check_errors(struct drm_device *dev);
extern void intel_uncore_fini(struct drm_device *dev);
extern void intel_uncore_forcewake_reset(struct drm_device *dev, bool restore);

void
i915_enable_pipestat(struct drm_i915_private *dev_priv, enum pipe pipe,
		     u32 status_mask);

void
i915_disable_pipestat(struct drm_i915_private *dev_priv, enum pipe pipe,
		      u32 status_mask);

void valleyview_enable_display_irqs(struct drm_i915_private *dev_priv);
void valleyview_disable_display_irqs(struct drm_i915_private *dev_priv);
>>>>>>> v3.18

/* i915_gem.c */
int i915_gem_init_ioctl(struct drm_device *dev, void *data,
			struct drm_file *file_priv);
int i915_gem_create_ioctl(struct drm_device *dev, void *data,
			  struct drm_file *file_priv);
int i915_gem_pread_ioctl(struct drm_device *dev, void *data,
			 struct drm_file *file_priv);
int i915_gem_pwrite_ioctl(struct drm_device *dev, void *data,
			  struct drm_file *file_priv);
int i915_gem_mmap_ioctl(struct drm_device *dev, void *data,
			struct drm_file *file_priv);
int i915_gem_mmap_gtt_ioctl(struct drm_device *dev, void *data,
			struct drm_file *file_priv);
int i915_gem_set_domain_ioctl(struct drm_device *dev, void *data,
			      struct drm_file *file_priv);
int i915_gem_sw_finish_ioctl(struct drm_device *dev, void *data,
			     struct drm_file *file_priv);
<<<<<<< HEAD
=======
void i915_gem_execbuffer_move_to_active(struct list_head *vmas,
					struct intel_engine_cs *ring);
void i915_gem_execbuffer_retire_commands(struct drm_device *dev,
					 struct drm_file *file,
					 struct intel_engine_cs *ring,
					 struct drm_i915_gem_object *obj);
int i915_gem_ringbuffer_submission(struct drm_device *dev,
				   struct drm_file *file,
				   struct intel_engine_cs *ring,
				   struct intel_context *ctx,
				   struct drm_i915_gem_execbuffer2 *args,
				   struct list_head *vmas,
				   struct drm_i915_gem_object *batch_obj,
				   u64 exec_start, u32 flags);
>>>>>>> v3.18
int i915_gem_execbuffer(struct drm_device *dev, void *data,
			struct drm_file *file_priv);
int i915_gem_execbuffer2(struct drm_device *dev, void *data,
			 struct drm_file *file_priv);
int i915_gem_pin_ioctl(struct drm_device *dev, void *data,
		       struct drm_file *file_priv);
int i915_gem_unpin_ioctl(struct drm_device *dev, void *data,
			 struct drm_file *file_priv);
int i915_gem_busy_ioctl(struct drm_device *dev, void *data,
			struct drm_file *file_priv);
int i915_gem_get_caching_ioctl(struct drm_device *dev, void *data,
			       struct drm_file *file);
int i915_gem_set_caching_ioctl(struct drm_device *dev, void *data,
			       struct drm_file *file);
int i915_gem_throttle_ioctl(struct drm_device *dev, void *data,
			    struct drm_file *file_priv);
int i915_gem_madvise_ioctl(struct drm_device *dev, void *data,
			   struct drm_file *file_priv);
int i915_gem_entervt_ioctl(struct drm_device *dev, void *data,
			   struct drm_file *file_priv);
int i915_gem_leavevt_ioctl(struct drm_device *dev, void *data,
			   struct drm_file *file_priv);
int i915_gem_set_tiling(struct drm_device *dev, void *data,
			struct drm_file *file_priv);
int i915_gem_get_tiling(struct drm_device *dev, void *data,
			struct drm_file *file_priv);
<<<<<<< HEAD
=======
int i915_gem_init_userptr(struct drm_device *dev);
int i915_gem_userptr_ioctl(struct drm_device *dev, void *data,
			   struct drm_file *file);
>>>>>>> v3.18
int i915_gem_get_aperture_ioctl(struct drm_device *dev, void *data,
				struct drm_file *file_priv);
int i915_gem_wait_ioctl(struct drm_device *dev, void *data,
			struct drm_file *file_priv);
void i915_gem_load(struct drm_device *dev);
<<<<<<< HEAD
void *i915_gem_object_alloc(struct drm_device *dev);
void i915_gem_object_free(struct drm_i915_gem_object *obj);
int i915_gem_init_object(struct drm_gem_object *obj);
=======
unsigned long i915_gem_shrink(struct drm_i915_private *dev_priv,
			      long target,
			      unsigned flags);
#define I915_SHRINK_PURGEABLE 0x1
#define I915_SHRINK_UNBOUND 0x2
#define I915_SHRINK_BOUND 0x4
void *i915_gem_object_alloc(struct drm_device *dev);
void i915_gem_object_free(struct drm_i915_gem_object *obj);
>>>>>>> v3.18
void i915_gem_object_init(struct drm_i915_gem_object *obj,
			 const struct drm_i915_gem_object_ops *ops);
struct drm_i915_gem_object *i915_gem_alloc_object(struct drm_device *dev,
						  size_t size);
<<<<<<< HEAD
void i915_gem_free_object(struct drm_gem_object *obj);

int __must_check i915_gem_object_pin(struct drm_i915_gem_object *obj,
				     uint32_t alignment,
				     bool map_and_fenceable,
				     bool nonblocking);
void i915_gem_object_unpin(struct drm_i915_gem_object *obj);
int __must_check i915_gem_object_unbind(struct drm_i915_gem_object *obj);
int i915_gem_object_put_pages(struct drm_i915_gem_object *obj);
void i915_gem_release_mmap(struct drm_i915_gem_object *obj);
void i915_gem_lastclose(struct drm_device *dev);

=======
void i915_init_vm(struct drm_i915_private *dev_priv,
		  struct i915_address_space *vm);
void i915_gem_free_object(struct drm_gem_object *obj);
void i915_gem_vma_destroy(struct i915_vma *vma);

#define PIN_MAPPABLE 0x1
#define PIN_NONBLOCK 0x2
#define PIN_GLOBAL 0x4
#define PIN_OFFSET_BIAS 0x8
#define PIN_OFFSET_MASK (~4095)
int __must_check i915_gem_object_pin(struct drm_i915_gem_object *obj,
				     struct i915_address_space *vm,
				     uint32_t alignment,
				     uint64_t flags);
int __must_check i915_vma_unbind(struct i915_vma *vma);
int i915_gem_object_put_pages(struct drm_i915_gem_object *obj);
void i915_gem_release_all_mmaps(struct drm_i915_private *dev_priv);
void i915_gem_release_mmap(struct drm_i915_gem_object *obj);
void i915_gem_lastclose(struct drm_device *dev);

int i915_gem_obj_prepare_shmem_read(struct drm_i915_gem_object *obj,
				    int *needs_clflush);

>>>>>>> v3.18
int __must_check i915_gem_object_get_pages(struct drm_i915_gem_object *obj);
static inline struct page *i915_gem_object_get_page(struct drm_i915_gem_object *obj, int n)
{
	struct sg_page_iter sg_iter;

	for_each_sg_page(obj->pages->sgl, &sg_iter, obj->pages->nents, n)
		return sg_page_iter_page(&sg_iter);

	return NULL;
}
static inline void i915_gem_object_pin_pages(struct drm_i915_gem_object *obj)
{
	BUG_ON(obj->pages == NULL);
	obj->pages_pin_count++;
}
static inline void i915_gem_object_unpin_pages(struct drm_i915_gem_object *obj)
{
	BUG_ON(obj->pages_pin_count == 0);
	obj->pages_pin_count--;
}

int __must_check i915_mutex_lock_interruptible(struct drm_device *dev);
int i915_gem_object_sync(struct drm_i915_gem_object *obj,
<<<<<<< HEAD
			 struct intel_ring_buffer *to);
void i915_gem_object_move_to_active(struct drm_i915_gem_object *obj,
				    struct intel_ring_buffer *ring);

=======
			 struct intel_engine_cs *to);
void i915_vma_move_to_active(struct i915_vma *vma,
			     struct intel_engine_cs *ring);
>>>>>>> v3.18
int i915_gem_dumb_create(struct drm_file *file_priv,
			 struct drm_device *dev,
			 struct drm_mode_create_dumb *args);
int i915_gem_mmap_gtt(struct drm_file *file_priv, struct drm_device *dev,
		      uint32_t handle, uint64_t *offset);
<<<<<<< HEAD
int i915_gem_dumb_destroy(struct drm_file *file_priv, struct drm_device *dev,
			  uint32_t handle);
=======
>>>>>>> v3.18
/**
 * Returns true if seq1 is later than seq2.
 */
static inline bool
i915_seqno_passed(uint32_t seq1, uint32_t seq2)
{
	return (int32_t)(seq1 - seq2) >= 0;
}

int __must_check i915_gem_get_seqno(struct drm_device *dev, u32 *seqno);
int __must_check i915_gem_set_seqno(struct drm_device *dev, u32 seqno);
int __must_check i915_gem_object_get_fence(struct drm_i915_gem_object *obj);
int __must_check i915_gem_object_put_fence(struct drm_i915_gem_object *obj);

<<<<<<< HEAD
static inline bool
i915_gem_object_pin_fence(struct drm_i915_gem_object *obj)
{
	if (obj->fence_reg != I915_FENCE_REG_NONE) {
		struct drm_i915_private *dev_priv = obj->base.dev->dev_private;
		dev_priv->fence_regs[obj->fence_reg].pin_count++;
		return true;
	} else
		return false;
}

static inline void
i915_gem_object_unpin_fence(struct drm_i915_gem_object *obj)
{
	if (obj->fence_reg != I915_FENCE_REG_NONE) {
		struct drm_i915_private *dev_priv = obj->base.dev->dev_private;
		dev_priv->fence_regs[obj->fence_reg].pin_count--;
	}
}

void i915_gem_retire_requests(struct drm_device *dev);
void i915_gem_retire_requests_ring(struct intel_ring_buffer *ring);
int __must_check i915_gem_check_wedge(struct i915_gpu_error *error,
				      bool interruptible);
static inline bool i915_reset_in_progress(struct i915_gpu_error *error)
{
	return unlikely(atomic_read(&error->reset_counter)
			& I915_RESET_IN_PROGRESS_FLAG);
=======
bool i915_gem_object_pin_fence(struct drm_i915_gem_object *obj);
void i915_gem_object_unpin_fence(struct drm_i915_gem_object *obj);

struct drm_i915_gem_request *
i915_gem_find_active_request(struct intel_engine_cs *ring);

bool i915_gem_retire_requests(struct drm_device *dev);
void i915_gem_retire_requests_ring(struct intel_engine_cs *ring);
int __must_check i915_gem_check_wedge(struct i915_gpu_error *error,
				      bool interruptible);
int __must_check i915_gem_check_olr(struct intel_engine_cs *ring, u32 seqno);

static inline bool i915_reset_in_progress(struct i915_gpu_error *error)
{
	return unlikely(atomic_read(&error->reset_counter)
			& (I915_RESET_IN_PROGRESS_FLAG | I915_WEDGED));
>>>>>>> v3.18
}

static inline bool i915_terminally_wedged(struct i915_gpu_error *error)
{
<<<<<<< HEAD
	return atomic_read(&error->reset_counter) == I915_WEDGED;
}

void i915_gem_reset(struct drm_device *dev);
void i915_gem_clflush_object(struct drm_i915_gem_object *obj);
int __must_check i915_gem_object_set_domain(struct drm_i915_gem_object *obj,
					    uint32_t read_domains,
					    uint32_t write_domain);
int __must_check i915_gem_object_finish_gpu(struct drm_i915_gem_object *obj);
int __must_check i915_gem_init(struct drm_device *dev);
int __must_check i915_gem_init_hw(struct drm_device *dev);
void i915_gem_l3_remap(struct drm_device *dev);
void i915_gem_init_swizzling(struct drm_device *dev);
void i915_gem_cleanup_ringbuffer(struct drm_device *dev);
int __must_check i915_gpu_idle(struct drm_device *dev);
int __must_check i915_gem_idle(struct drm_device *dev);
int i915_add_request(struct intel_ring_buffer *ring,
		     struct drm_file *file,
		     u32 *seqno);
int __must_check i915_wait_seqno(struct intel_ring_buffer *ring,
=======
	return atomic_read(&error->reset_counter) & I915_WEDGED;
}

static inline u32 i915_reset_count(struct i915_gpu_error *error)
{
	return ((atomic_read(&error->reset_counter) & ~I915_WEDGED) + 1) / 2;
}

static inline bool i915_stop_ring_allow_ban(struct drm_i915_private *dev_priv)
{
	return dev_priv->gpu_error.stop_rings == 0 ||
		dev_priv->gpu_error.stop_rings & I915_STOP_RING_ALLOW_BAN;
}

static inline bool i915_stop_ring_allow_warn(struct drm_i915_private *dev_priv)
{
	return dev_priv->gpu_error.stop_rings == 0 ||
		dev_priv->gpu_error.stop_rings & I915_STOP_RING_ALLOW_WARN;
}

void i915_gem_reset(struct drm_device *dev);
bool i915_gem_clflush_object(struct drm_i915_gem_object *obj, bool force);
int __must_check i915_gem_object_finish_gpu(struct drm_i915_gem_object *obj);
int __must_check i915_gem_init(struct drm_device *dev);
int i915_gem_init_rings(struct drm_device *dev);
int __must_check i915_gem_init_hw(struct drm_device *dev);
int i915_gem_l3_remap(struct intel_engine_cs *ring, int slice);
void i915_gem_init_swizzling(struct drm_device *dev);
void i915_gem_cleanup_ringbuffer(struct drm_device *dev);
int __must_check i915_gpu_idle(struct drm_device *dev);
int __must_check i915_gem_suspend(struct drm_device *dev);
int __i915_add_request(struct intel_engine_cs *ring,
		       struct drm_file *file,
		       struct drm_i915_gem_object *batch_obj,
		       u32 *seqno);
#define i915_add_request(ring, seqno) \
	__i915_add_request(ring, NULL, NULL, seqno)
int __must_check i915_wait_seqno(struct intel_engine_cs *ring,
>>>>>>> v3.18
				 uint32_t seqno);
int i915_gem_fault(struct vm_area_struct *vma, struct vm_fault *vmf);
int __must_check
i915_gem_object_set_to_gtt_domain(struct drm_i915_gem_object *obj,
				  bool write);
int __must_check
i915_gem_object_set_to_cpu_domain(struct drm_i915_gem_object *obj, bool write);
int __must_check
i915_gem_object_pin_to_display_plane(struct drm_i915_gem_object *obj,
				     u32 alignment,
<<<<<<< HEAD
				     struct intel_ring_buffer *pipelined);
int i915_gem_attach_phys_object(struct drm_device *dev,
				struct drm_i915_gem_object *obj,
				int id,
				int align);
void i915_gem_detach_phys_object(struct drm_device *dev,
				 struct drm_i915_gem_object *obj);
void i915_gem_free_all_phys_object(struct drm_device *dev);
=======
				     struct intel_engine_cs *pipelined);
void i915_gem_object_unpin_from_display_plane(struct drm_i915_gem_object *obj);
int i915_gem_object_attach_phys(struct drm_i915_gem_object *obj,
				int align);
int i915_gem_open(struct drm_device *dev, struct drm_file *file);
>>>>>>> v3.18
void i915_gem_release(struct drm_device *dev, struct drm_file *file);

uint32_t
i915_gem_get_gtt_size(struct drm_device *dev, uint32_t size, int tiling_mode);
uint32_t
i915_gem_get_gtt_alignment(struct drm_device *dev, uint32_t size,
			    int tiling_mode, bool fenced);

int i915_gem_object_set_cache_level(struct drm_i915_gem_object *obj,
				    enum i915_cache_level cache_level);

struct drm_gem_object *i915_gem_prime_import(struct drm_device *dev,
				struct dma_buf *dma_buf);

struct dma_buf *i915_gem_prime_export(struct drm_device *dev,
				struct drm_gem_object *gem_obj, int flags);

void i915_gem_restore_fences(struct drm_device *dev);

<<<<<<< HEAD
/* i915_gem_context.c */
void i915_gem_context_init(struct drm_device *dev);
void i915_gem_context_fini(struct drm_device *dev);
void i915_gem_context_close(struct drm_device *dev, struct drm_file *file);
int i915_switch_context(struct intel_ring_buffer *ring,
			struct drm_file *file, int to_id);
=======
unsigned long i915_gem_obj_offset(struct drm_i915_gem_object *o,
				  struct i915_address_space *vm);
bool i915_gem_obj_bound_any(struct drm_i915_gem_object *o);
bool i915_gem_obj_bound(struct drm_i915_gem_object *o,
			struct i915_address_space *vm);
unsigned long i915_gem_obj_size(struct drm_i915_gem_object *o,
				struct i915_address_space *vm);
struct i915_vma *i915_gem_obj_to_vma(struct drm_i915_gem_object *obj,
				     struct i915_address_space *vm);
struct i915_vma *
i915_gem_obj_lookup_or_create_vma(struct drm_i915_gem_object *obj,
				  struct i915_address_space *vm);

struct i915_vma *i915_gem_obj_to_ggtt(struct drm_i915_gem_object *obj);
static inline bool i915_gem_obj_is_pinned(struct drm_i915_gem_object *obj) {
	struct i915_vma *vma;
	list_for_each_entry(vma, &obj->vma_list, vma_link)
		if (vma->pin_count > 0)
			return true;
	return false;
}

/* Some GGTT VM helpers */
#define i915_obj_to_ggtt(obj) \
	(&((struct drm_i915_private *)(obj)->base.dev->dev_private)->gtt.base)
static inline bool i915_is_ggtt(struct i915_address_space *vm)
{
	struct i915_address_space *ggtt =
		&((struct drm_i915_private *)(vm)->dev->dev_private)->gtt.base;
	return vm == ggtt;
}

static inline struct i915_hw_ppgtt *
i915_vm_to_ppgtt(struct i915_address_space *vm)
{
	WARN_ON(i915_is_ggtt(vm));

	return container_of(vm, struct i915_hw_ppgtt, base);
}


static inline bool i915_gem_obj_ggtt_bound(struct drm_i915_gem_object *obj)
{
	return i915_gem_obj_bound(obj, i915_obj_to_ggtt(obj));
}

static inline unsigned long
i915_gem_obj_ggtt_offset(struct drm_i915_gem_object *obj)
{
	return i915_gem_obj_offset(obj, i915_obj_to_ggtt(obj));
}

static inline unsigned long
i915_gem_obj_ggtt_size(struct drm_i915_gem_object *obj)
{
	return i915_gem_obj_size(obj, i915_obj_to_ggtt(obj));
}

static inline int __must_check
i915_gem_obj_ggtt_pin(struct drm_i915_gem_object *obj,
		      uint32_t alignment,
		      unsigned flags)
{
	return i915_gem_object_pin(obj, i915_obj_to_ggtt(obj),
				   alignment, flags | PIN_GLOBAL);
}

static inline int
i915_gem_object_ggtt_unbind(struct drm_i915_gem_object *obj)
{
	return i915_vma_unbind(i915_gem_obj_to_ggtt(obj));
}

void i915_gem_object_ggtt_unpin(struct drm_i915_gem_object *obj);

/* i915_gem_context.c */
int __must_check i915_gem_context_init(struct drm_device *dev);
void i915_gem_context_fini(struct drm_device *dev);
void i915_gem_context_reset(struct drm_device *dev);
int i915_gem_context_open(struct drm_device *dev, struct drm_file *file);
int i915_gem_context_enable(struct drm_i915_private *dev_priv);
void i915_gem_context_close(struct drm_device *dev, struct drm_file *file);
int i915_switch_context(struct intel_engine_cs *ring,
			struct intel_context *to);
struct intel_context *
i915_gem_context_get(struct drm_i915_file_private *file_priv, u32 id);
void i915_gem_context_free(struct kref *ctx_ref);
struct drm_i915_gem_object *
i915_gem_alloc_context_obj(struct drm_device *dev, size_t size);
static inline void i915_gem_context_reference(struct intel_context *ctx)
{
	kref_get(&ctx->ref);
}

static inline void i915_gem_context_unreference(struct intel_context *ctx)
{
	kref_put(&ctx->ref, i915_gem_context_free);
}

static inline bool i915_gem_context_is_default(const struct intel_context *c)
{
	return c->user_handle == DEFAULT_CONTEXT_HANDLE;
}

>>>>>>> v3.18
int i915_gem_context_create_ioctl(struct drm_device *dev, void *data,
				  struct drm_file *file);
int i915_gem_context_destroy_ioctl(struct drm_device *dev, void *data,
				   struct drm_file *file);

<<<<<<< HEAD
/* i915_gem_gtt.c */
void i915_gem_cleanup_aliasing_ppgtt(struct drm_device *dev);
void i915_ppgtt_bind_object(struct i915_hw_ppgtt *ppgtt,
			    struct drm_i915_gem_object *obj,
			    enum i915_cache_level cache_level);
void i915_ppgtt_unbind_object(struct i915_hw_ppgtt *ppgtt,
			      struct drm_i915_gem_object *obj);

void i915_gem_restore_gtt_mappings(struct drm_device *dev);
int __must_check i915_gem_gtt_prepare_object(struct drm_i915_gem_object *obj);
void i915_gem_gtt_bind_object(struct drm_i915_gem_object *obj,
				enum i915_cache_level cache_level);
void i915_gem_gtt_unbind_object(struct drm_i915_gem_object *obj);
void i915_gem_gtt_finish_object(struct drm_i915_gem_object *obj);
void i915_gem_init_global_gtt(struct drm_device *dev);
void i915_gem_setup_global_gtt(struct drm_device *dev, unsigned long start,
			       unsigned long mappable_end, unsigned long end);
int i915_gem_gtt_init(struct drm_device *dev);
=======
/* i915_gem_evict.c */
int __must_check i915_gem_evict_something(struct drm_device *dev,
					  struct i915_address_space *vm,
					  int min_size,
					  unsigned alignment,
					  unsigned cache_level,
					  unsigned long start,
					  unsigned long end,
					  unsigned flags);
int i915_gem_evict_vm(struct i915_address_space *vm, bool do_idle);
int i915_gem_evict_everything(struct drm_device *dev);

/* belongs in i915_gem_gtt.h */
>>>>>>> v3.18
static inline void i915_gem_chipset_flush(struct drm_device *dev)
{
	if (INTEL_INFO(dev)->gen < 6)
		intel_gtt_chipset_flush();
}

<<<<<<< HEAD

/* i915_gem_evict.c */
int __must_check i915_gem_evict_something(struct drm_device *dev, int min_size,
					  unsigned alignment,
					  unsigned cache_level,
					  bool mappable,
					  bool nonblock);
int i915_gem_evict_everything(struct drm_device *dev);

/* i915_gem_stolen.c */
int i915_gem_init_stolen(struct drm_device *dev);
int i915_gem_stolen_setup_compression(struct drm_device *dev, int size);
=======
/* i915_gem_stolen.c */
int i915_gem_init_stolen(struct drm_device *dev);
int i915_gem_stolen_setup_compression(struct drm_device *dev, int size, int fb_cpp);
>>>>>>> v3.18
void i915_gem_stolen_cleanup_compression(struct drm_device *dev);
void i915_gem_cleanup_stolen(struct drm_device *dev);
struct drm_i915_gem_object *
i915_gem_object_create_stolen(struct drm_device *dev, u32 size);
struct drm_i915_gem_object *
i915_gem_object_create_stolen_for_preallocated(struct drm_device *dev,
					       u32 stolen_offset,
					       u32 gtt_offset,
					       u32 size);
<<<<<<< HEAD
void i915_gem_object_release_stolen(struct drm_i915_gem_object *obj);

/* i915_gem_tiling.c */
inline static bool i915_gem_object_needs_bit17_swizzle(struct drm_i915_gem_object *obj)
{
	drm_i915_private_t *dev_priv = obj->base.dev->dev_private;
=======

/* i915_gem_tiling.c */
static inline bool i915_gem_object_needs_bit17_swizzle(struct drm_i915_gem_object *obj)
{
	struct drm_i915_private *dev_priv = obj->base.dev->dev_private;
>>>>>>> v3.18

	return dev_priv->mm.bit_6_swizzle_x == I915_BIT_6_SWIZZLE_9_10_17 &&
		obj->tiling_mode != I915_TILING_NONE;
}

void i915_gem_detect_bit_6_swizzle(struct drm_device *dev);
void i915_gem_object_do_bit_17_swizzle(struct drm_i915_gem_object *obj);
void i915_gem_object_save_bit_17_swizzle(struct drm_i915_gem_object *obj);

/* i915_gem_debug.c */
<<<<<<< HEAD
void i915_gem_dump_object(struct drm_i915_gem_object *obj, int len,
			  const char *where, uint32_t mark);
=======
>>>>>>> v3.18
#if WATCH_LISTS
int i915_verify_lists(struct drm_device *dev);
#else
#define i915_verify_lists(dev) 0
#endif
<<<<<<< HEAD
void i915_gem_object_check_coherency(struct drm_i915_gem_object *obj,
				     int handle);
void i915_gem_dump_object(struct drm_i915_gem_object *obj, int len,
			  const char *where, uint32_t mark);
=======
>>>>>>> v3.18

/* i915_debugfs.c */
int i915_debugfs_init(struct drm_minor *minor);
void i915_debugfs_cleanup(struct drm_minor *minor);
<<<<<<< HEAD
=======
#ifdef CONFIG_DEBUG_FS
void intel_display_crc_init(struct drm_device *dev);
#else
static inline void intel_display_crc_init(struct drm_device *dev) {}
#endif

/* i915_gpu_error.c */
__printf(2, 3)
void i915_error_printf(struct drm_i915_error_state_buf *e, const char *f, ...);
int i915_error_state_to_str(struct drm_i915_error_state_buf *estr,
			    const struct i915_error_state_file_priv *error);
int i915_error_state_buf_init(struct drm_i915_error_state_buf *eb,
			      struct drm_i915_private *i915,
			      size_t count, loff_t pos);
static inline void i915_error_state_buf_release(
	struct drm_i915_error_state_buf *eb)
{
	kfree(eb->buf);
}
void i915_capture_error_state(struct drm_device *dev, bool wedge,
			      const char *error_msg);
void i915_error_state_get(struct drm_device *dev,
			  struct i915_error_state_file_priv *error_priv);
void i915_error_state_put(struct i915_error_state_file_priv *error_priv);
void i915_destroy_error_state(struct drm_device *dev);

void i915_get_extra_instdone(struct drm_device *dev, uint32_t *instdone);
const char *i915_cache_level_str(struct drm_i915_private *i915, int type);

/* i915_cmd_parser.c */
int i915_cmd_parser_get_version(void);
int i915_cmd_parser_init_ring(struct intel_engine_cs *ring);
void i915_cmd_parser_fini_ring(struct intel_engine_cs *ring);
bool i915_needs_cmd_parser(struct intel_engine_cs *ring);
int i915_parse_cmds(struct intel_engine_cs *ring,
		    struct drm_i915_gem_object *batch_obj,
		    u32 batch_start_offset,
		    bool is_master);
>>>>>>> v3.18

/* i915_suspend.c */
extern int i915_save_state(struct drm_device *dev);
extern int i915_restore_state(struct drm_device *dev);

/* i915_ums.c */
void i915_save_display_reg(struct drm_device *dev);
void i915_restore_display_reg(struct drm_device *dev);

/* i915_sysfs.c */
void i915_setup_sysfs(struct drm_device *dev_priv);
void i915_teardown_sysfs(struct drm_device *dev_priv);

/* intel_i2c.c */
extern int intel_setup_gmbus(struct drm_device *dev);
extern void intel_teardown_gmbus(struct drm_device *dev);
static inline bool intel_gmbus_is_port_valid(unsigned port)
{
	return (port >= GMBUS_PORT_SSC && port <= GMBUS_PORT_DPD);
}

extern struct i2c_adapter *intel_gmbus_get_adapter(
		struct drm_i915_private *dev_priv, unsigned port);
extern void intel_gmbus_set_speed(struct i2c_adapter *adapter, int speed);
extern void intel_gmbus_force_bit(struct i2c_adapter *adapter, bool force_bit);
static inline bool intel_gmbus_is_forced_bit(struct i2c_adapter *adapter)
{
	return container_of(adapter, struct intel_gmbus, adapter)->force_bit;
}
extern void intel_i2c_reset(struct drm_device *dev);

/* intel_opregion.c */
<<<<<<< HEAD
extern int intel_opregion_setup(struct drm_device *dev);
#ifdef CONFIG_ACPI
extern void intel_opregion_init(struct drm_device *dev);
extern void intel_opregion_fini(struct drm_device *dev);
extern void intel_opregion_asle_intr(struct drm_device *dev);
extern void intel_opregion_gse_intr(struct drm_device *dev);
extern void intel_opregion_enable_asle(struct drm_device *dev);
#else
static inline void intel_opregion_init(struct drm_device *dev) { return; }
static inline void intel_opregion_fini(struct drm_device *dev) { return; }
static inline void intel_opregion_asle_intr(struct drm_device *dev) { return; }
static inline void intel_opregion_gse_intr(struct drm_device *dev) { return; }
static inline void intel_opregion_enable_asle(struct drm_device *dev) { return; }
=======
struct intel_encoder;
#ifdef CONFIG_ACPI
extern int intel_opregion_setup(struct drm_device *dev);
extern void intel_opregion_init(struct drm_device *dev);
extern void intel_opregion_fini(struct drm_device *dev);
extern void intel_opregion_asle_intr(struct drm_device *dev);
extern int intel_opregion_notify_encoder(struct intel_encoder *intel_encoder,
					 bool enable);
extern int intel_opregion_notify_adapter(struct drm_device *dev,
					 pci_power_t state);
#else
static inline int intel_opregion_setup(struct drm_device *dev) { return 0; }
static inline void intel_opregion_init(struct drm_device *dev) { return; }
static inline void intel_opregion_fini(struct drm_device *dev) { return; }
static inline void intel_opregion_asle_intr(struct drm_device *dev) { return; }
static inline int
intel_opregion_notify_encoder(struct intel_encoder *intel_encoder, bool enable)
{
	return 0;
}
static inline int
intel_opregion_notify_adapter(struct drm_device *dev, pci_power_t state)
{
	return 0;
}
>>>>>>> v3.18
#endif

/* intel_acpi.c */
#ifdef CONFIG_ACPI
extern void intel_register_dsm_handler(void);
extern void intel_unregister_dsm_handler(void);
#else
static inline void intel_register_dsm_handler(void) { return; }
static inline void intel_unregister_dsm_handler(void) { return; }
#endif /* CONFIG_ACPI */

/* modesetting */
extern void intel_modeset_init_hw(struct drm_device *dev);
<<<<<<< HEAD
extern void intel_modeset_init(struct drm_device *dev);
extern void intel_modeset_gem_init(struct drm_device *dev);
extern void intel_modeset_cleanup(struct drm_device *dev);
=======
extern void intel_modeset_suspend_hw(struct drm_device *dev);
extern void intel_modeset_init(struct drm_device *dev);
extern void intel_modeset_gem_init(struct drm_device *dev);
extern void intel_modeset_cleanup(struct drm_device *dev);
extern void intel_connector_unregister(struct intel_connector *);
>>>>>>> v3.18
extern int intel_modeset_vga_set_state(struct drm_device *dev, bool state);
extern void intel_modeset_setup_hw_state(struct drm_device *dev,
					 bool force_restore);
extern void i915_redisable_vga(struct drm_device *dev);
<<<<<<< HEAD
extern bool intel_fbc_enabled(struct drm_device *dev);
=======
extern void i915_redisable_vga_power_on(struct drm_device *dev);
extern bool intel_fbc_enabled(struct drm_device *dev);
extern void gen8_fbc_sw_flush(struct drm_device *dev, u32 value);
>>>>>>> v3.18
extern void intel_disable_fbc(struct drm_device *dev);
extern bool ironlake_set_drps(struct drm_device *dev, u8 val);
extern void intel_init_pch_refclk(struct drm_device *dev);
extern void gen6_set_rps(struct drm_device *dev, u8 val);
<<<<<<< HEAD
=======
extern void valleyview_set_rps(struct drm_device *dev, u8 val);
extern void intel_set_memory_cxsr(struct drm_i915_private *dev_priv,
				  bool enable);
>>>>>>> v3.18
extern void intel_detect_pch(struct drm_device *dev);
extern int intel_trans_dp_port_sel(struct drm_crtc *crtc);
extern int intel_enable_rc6(const struct drm_device *dev);

extern bool i915_semaphore_is_enabled(struct drm_device *dev);
int i915_reg_read_ioctl(struct drm_device *dev, void *data,
			struct drm_file *file);
<<<<<<< HEAD

/* overlay */
#ifdef CONFIG_DEBUG_FS
extern struct intel_overlay_error_state *intel_overlay_capture_error_state(struct drm_device *dev);
extern void intel_overlay_print_error_state(struct seq_file *m, struct intel_overlay_error_state *error);

extern struct intel_display_error_state *intel_display_capture_error_state(struct drm_device *dev);
extern void intel_display_print_error_state(struct seq_file *m,
					    struct drm_device *dev,
					    struct intel_display_error_state *error);
#endif
=======
int i915_get_reset_stats_ioctl(struct drm_device *dev, void *data,
			       struct drm_file *file);

void intel_notify_mmio_flip(struct intel_engine_cs *ring);

/* overlay */
extern struct intel_overlay_error_state *intel_overlay_capture_error_state(struct drm_device *dev);
extern void intel_overlay_print_error_state(struct drm_i915_error_state_buf *e,
					    struct intel_overlay_error_state *error);

extern struct intel_display_error_state *intel_display_capture_error_state(struct drm_device *dev);
extern void intel_display_print_error_state(struct drm_i915_error_state_buf *e,
					    struct drm_device *dev,
					    struct intel_display_error_state *error);
>>>>>>> v3.18

/* On SNB platform, before reading ring registers forcewake bit
 * must be set to prevent GT core from power down and stale values being
 * returned.
 */
<<<<<<< HEAD
void gen6_gt_force_wake_get(struct drm_i915_private *dev_priv);
void gen6_gt_force_wake_put(struct drm_i915_private *dev_priv);
int __gen6_gt_wait_for_fifo(struct drm_i915_private *dev_priv);

int sandybridge_pcode_read(struct drm_i915_private *dev_priv, u8 mbox, u32 *val);
int sandybridge_pcode_write(struct drm_i915_private *dev_priv, u8 mbox, u32 val);
int valleyview_punit_read(struct drm_i915_private *dev_priv, u8 addr, u32 *val);
int valleyview_punit_write(struct drm_i915_private *dev_priv, u8 addr, u32 val);

#define __i915_read(x, y) \
	u##x i915_read##x(struct drm_i915_private *dev_priv, u32 reg);

__i915_read(8, b)
__i915_read(16, w)
__i915_read(32, l)
__i915_read(64, q)
#undef __i915_read

#define __i915_write(x, y) \
	void i915_write##x(struct drm_i915_private *dev_priv, u32 reg, u##x val);

__i915_write(8, b)
__i915_write(16, w)
__i915_write(32, l)
__i915_write(64, q)
#undef __i915_write

#define I915_READ8(reg)		i915_read8(dev_priv, (reg))
#define I915_WRITE8(reg, val)	i915_write8(dev_priv, (reg), (val))

#define I915_READ16(reg)	i915_read16(dev_priv, (reg))
#define I915_WRITE16(reg, val)	i915_write16(dev_priv, (reg), (val))
#define I915_READ16_NOTRACE(reg)	readw(dev_priv->regs + (reg))
#define I915_WRITE16_NOTRACE(reg, val)	writew(val, dev_priv->regs + (reg))

#define I915_READ(reg)		i915_read32(dev_priv, (reg))
#define I915_WRITE(reg, val)	i915_write32(dev_priv, (reg), (val))
#define I915_READ_NOTRACE(reg)		readl(dev_priv->regs + (reg))
#define I915_WRITE_NOTRACE(reg, val)	writel(val, dev_priv->regs + (reg))

#define I915_WRITE64(reg, val)	i915_write64(dev_priv, (reg), (val))
#define I915_READ64(reg)	i915_read64(dev_priv, (reg))
=======
void gen6_gt_force_wake_get(struct drm_i915_private *dev_priv, int fw_engine);
void gen6_gt_force_wake_put(struct drm_i915_private *dev_priv, int fw_engine);
void assert_force_wake_inactive(struct drm_i915_private *dev_priv);

int sandybridge_pcode_read(struct drm_i915_private *dev_priv, u8 mbox, u32 *val);
int sandybridge_pcode_write(struct drm_i915_private *dev_priv, u8 mbox, u32 val);

/* intel_sideband.c */
u32 vlv_punit_read(struct drm_i915_private *dev_priv, u8 addr);
void vlv_punit_write(struct drm_i915_private *dev_priv, u8 addr, u32 val);
u32 vlv_nc_read(struct drm_i915_private *dev_priv, u8 addr);
u32 vlv_gpio_nc_read(struct drm_i915_private *dev_priv, u32 reg);
void vlv_gpio_nc_write(struct drm_i915_private *dev_priv, u32 reg, u32 val);
u32 vlv_cck_read(struct drm_i915_private *dev_priv, u32 reg);
void vlv_cck_write(struct drm_i915_private *dev_priv, u32 reg, u32 val);
u32 vlv_ccu_read(struct drm_i915_private *dev_priv, u32 reg);
void vlv_ccu_write(struct drm_i915_private *dev_priv, u32 reg, u32 val);
u32 vlv_bunit_read(struct drm_i915_private *dev_priv, u32 reg);
void vlv_bunit_write(struct drm_i915_private *dev_priv, u32 reg, u32 val);
u32 vlv_gps_core_read(struct drm_i915_private *dev_priv, u32 reg);
void vlv_gps_core_write(struct drm_i915_private *dev_priv, u32 reg, u32 val);
u32 vlv_dpio_read(struct drm_i915_private *dev_priv, enum pipe pipe, int reg);
void vlv_dpio_write(struct drm_i915_private *dev_priv, enum pipe pipe, int reg, u32 val);
u32 intel_sbi_read(struct drm_i915_private *dev_priv, u16 reg,
		   enum intel_sbi_destination destination);
void intel_sbi_write(struct drm_i915_private *dev_priv, u16 reg, u32 value,
		     enum intel_sbi_destination destination);
u32 vlv_flisdsi_read(struct drm_i915_private *dev_priv, u32 reg);
void vlv_flisdsi_write(struct drm_i915_private *dev_priv, u32 reg, u32 val);

int vlv_gpu_freq(struct drm_i915_private *dev_priv, int val);
int vlv_freq_opcode(struct drm_i915_private *dev_priv, int val);

#define FORCEWAKE_RENDER	(1 << 0)
#define FORCEWAKE_MEDIA		(1 << 1)
#define FORCEWAKE_ALL		(FORCEWAKE_RENDER | FORCEWAKE_MEDIA)


#define I915_READ8(reg)		dev_priv->uncore.funcs.mmio_readb(dev_priv, (reg), true)
#define I915_WRITE8(reg, val)	dev_priv->uncore.funcs.mmio_writeb(dev_priv, (reg), (val), true)

#define I915_READ16(reg)	dev_priv->uncore.funcs.mmio_readw(dev_priv, (reg), true)
#define I915_WRITE16(reg, val)	dev_priv->uncore.funcs.mmio_writew(dev_priv, (reg), (val), true)
#define I915_READ16_NOTRACE(reg)	dev_priv->uncore.funcs.mmio_readw(dev_priv, (reg), false)
#define I915_WRITE16_NOTRACE(reg, val)	dev_priv->uncore.funcs.mmio_writew(dev_priv, (reg), (val), false)

#define I915_READ(reg)		dev_priv->uncore.funcs.mmio_readl(dev_priv, (reg), true)
#define I915_WRITE(reg, val)	dev_priv->uncore.funcs.mmio_writel(dev_priv, (reg), (val), true)
#define I915_READ_NOTRACE(reg)		dev_priv->uncore.funcs.mmio_readl(dev_priv, (reg), false)
#define I915_WRITE_NOTRACE(reg, val)	dev_priv->uncore.funcs.mmio_writel(dev_priv, (reg), (val), false)

/* Be very careful with read/write 64-bit values. On 32-bit machines, they
 * will be implemented using 2 32-bit writes in an arbitrary order with
 * an arbitrary delay between them. This can cause the hardware to
 * act upon the intermediate value, possibly leading to corruption and
 * machine death. You have been warned.
 */
#define I915_WRITE64(reg, val)	dev_priv->uncore.funcs.mmio_writeq(dev_priv, (reg), (val), true)
#define I915_READ64(reg)	dev_priv->uncore.funcs.mmio_readq(dev_priv, (reg), true)

#define I915_READ64_2x32(lower_reg, upper_reg) ({			\
		u32 upper = I915_READ(upper_reg);			\
		u32 lower = I915_READ(lower_reg);			\
		u32 tmp = I915_READ(upper_reg);				\
		if (upper != tmp) {					\
			upper = tmp;					\
			lower = I915_READ(lower_reg);			\
			WARN_ON(I915_READ(upper_reg) != upper);		\
		}							\
		(u64)upper << 32 | lower; })
>>>>>>> v3.18

#define POSTING_READ(reg)	(void)I915_READ_NOTRACE(reg)
#define POSTING_READ16(reg)	(void)I915_READ16_NOTRACE(reg)

/* "Broadcast RGB" property */
#define INTEL_BROADCAST_RGB_AUTO 0
#define INTEL_BROADCAST_RGB_FULL 1
#define INTEL_BROADCAST_RGB_LIMITED 2

static inline uint32_t i915_vgacntrl_reg(struct drm_device *dev)
{
<<<<<<< HEAD
	if (HAS_PCH_SPLIT(dev))
		return CPU_VGACNTRL;
	else if (IS_VALLEYVIEW(dev))
		return VLV_VGACNTRL;
=======
	if (IS_VALLEYVIEW(dev))
		return VLV_VGACNTRL;
	else if (INTEL_INFO(dev)->gen >= 5)
		return CPU_VGACNTRL;
>>>>>>> v3.18
	else
		return VGACNTRL;
}

static inline void __user *to_user_ptr(u64 address)
{
	return (void __user *)(uintptr_t)address;
}

static inline unsigned long msecs_to_jiffies_timeout(const unsigned int m)
{
	unsigned long j = msecs_to_jiffies(m);

	return min_t(unsigned long, MAX_JIFFY_OFFSET, j + 1);
}

static inline unsigned long
timespec_to_jiffies_timeout(const struct timespec *value)
{
	unsigned long j = timespec_to_jiffies(value);

	return min_t(unsigned long, MAX_JIFFY_OFFSET, j + 1);
}

<<<<<<< HEAD
=======
/*
 * If you need to wait X milliseconds between events A and B, but event B
 * doesn't happen exactly after event A, you record the timestamp (jiffies) of
 * when event A happened, then just before event B you call this function and
 * pass the timestamp as the first argument, and X as the second argument.
 */
static inline void
wait_remaining_ms_from_jiffies(unsigned long timestamp_jiffies, int to_wait_ms)
{
	unsigned long target_jiffies, tmp_jiffies, remaining_jiffies;

	/*
	 * Don't re-read the value of "jiffies" every time since it may change
	 * behind our back and break the math.
	 */
	tmp_jiffies = jiffies;
	target_jiffies = timestamp_jiffies +
			 msecs_to_jiffies_timeout(to_wait_ms);

	if (time_after(target_jiffies, tmp_jiffies)) {
		remaining_jiffies = target_jiffies - tmp_jiffies;
		while (remaining_jiffies)
			remaining_jiffies =
			    schedule_timeout_uninterruptible(remaining_jiffies);
	}
}

>>>>>>> v3.18
#endif
