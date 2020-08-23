/*
 * Copyright © 2008-2010 Intel Corporation
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
 *    Eric Anholt <eric@anholt.net>
 *    Zou Nan hai <nanhai.zou@intel.com>
 *    Xiang Hai hao<haihao.xiang@intel.com>
 *
 */

#include <drm/drmP.h>
#include "i915_drv.h"
#include <drm/i915_drm.h>
#include "i915_trace.h"
#include "intel_drv.h"

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * 965+ support PIPE_CONTROL commands, which provide finer grained control
 * over cache flushing.
 */
struct pipe_control {
	struct drm_i915_gem_object *obj;
	volatile u32 *cpu_page;
	u32 gtt_offset;
};

static inline int ring_space(struct intel_ring_buffer *ring)
{
	int space = (ring->head & HEAD_ADDR) - (ring->tail + I915_RING_FREE_SPACE);
	if (space < 0)
		space += ring->size;
	return space;
}

static int
gen2_render_ring_flush(struct intel_ring_buffer *ring,
=======
=======
>>>>>>> v3.18
bool
intel_ring_initialized(struct intel_engine_cs *ring)
{
	struct drm_device *dev = ring->dev;

	if (!dev)
		return false;

	if (i915.enable_execlists) {
		struct intel_context *dctx = ring->default_context;
		struct intel_ringbuffer *ringbuf = dctx->engine[ring->id].ringbuf;

		return ringbuf->obj;
	} else
		return ring->buffer && ring->buffer->obj;
}

int __intel_ring_space(int head, int tail, int size)
{
	int space = head - (tail + I915_RING_FREE_SPACE);
	if (space < 0)
		space += size;
	return space;
}

int intel_ring_space(struct intel_ringbuffer *ringbuf)
{
	return __intel_ring_space(ringbuf->head & HEAD_ADDR,
				  ringbuf->tail, ringbuf->size);
}

bool intel_ring_stopped(struct intel_engine_cs *ring)
{
	struct drm_i915_private *dev_priv = ring->dev->dev_private;
	return dev_priv->gpu_error.stop_rings & intel_ring_flag(ring);
}

void __intel_ring_advance(struct intel_engine_cs *ring)
{
	struct intel_ringbuffer *ringbuf = ring->buffer;
	ringbuf->tail &= ringbuf->size - 1;
	if (intel_ring_stopped(ring))
		return;
	ring->write_tail(ring, ringbuf->tail);
}

static int
gen2_render_ring_flush(struct intel_engine_cs *ring,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		       u32	invalidate_domains,
		       u32	flush_domains)
{
	u32 cmd;
	int ret;

	cmd = MI_FLUSH;
	if (((invalidate_domains|flush_domains) & I915_GEM_DOMAIN_RENDER) == 0)
		cmd |= MI_NO_WRITE_FLUSH;

	if (invalidate_domains & I915_GEM_DOMAIN_SAMPLER)
		cmd |= MI_READ_FLUSH;

	ret = intel_ring_begin(ring, 2);
	if (ret)
		return ret;

	intel_ring_emit(ring, cmd);
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_advance(ring);

	return 0;
}

static int
<<<<<<< HEAD
<<<<<<< HEAD
gen4_render_ring_flush(struct intel_ring_buffer *ring,
=======
gen4_render_ring_flush(struct intel_engine_cs *ring,
>>>>>>> v3.18
=======
gen4_render_ring_flush(struct intel_engine_cs *ring,
>>>>>>> v3.18
		       u32	invalidate_domains,
		       u32	flush_domains)
{
	struct drm_device *dev = ring->dev;
	u32 cmd;
	int ret;

	/*
	 * read/write caches:
	 *
	 * I915_GEM_DOMAIN_RENDER is always invalidated, but is
	 * only flushed if MI_NO_WRITE_FLUSH is unset.  On 965, it is
	 * also flushed at 2d versus 3d pipeline switches.
	 *
	 * read-only caches:
	 *
	 * I915_GEM_DOMAIN_SAMPLER is flushed on pre-965 if
	 * MI_READ_FLUSH is set, and is always flushed on 965.
	 *
	 * I915_GEM_DOMAIN_COMMAND may not exist?
	 *
	 * I915_GEM_DOMAIN_INSTRUCTION, which exists on 965, is
	 * invalidated when MI_EXE_FLUSH is set.
	 *
	 * I915_GEM_DOMAIN_VERTEX, which exists on 965, is
	 * invalidated with every MI_FLUSH.
	 *
	 * TLBs:
	 *
	 * On 965, TLBs associated with I915_GEM_DOMAIN_COMMAND
	 * and I915_GEM_DOMAIN_CPU in are invalidated at PTE write and
	 * I915_GEM_DOMAIN_RENDER and I915_GEM_DOMAIN_SAMPLER
	 * are flushed at any MI_FLUSH.
	 */

	cmd = MI_FLUSH | MI_NO_WRITE_FLUSH;
	if ((invalidate_domains|flush_domains) & I915_GEM_DOMAIN_RENDER)
		cmd &= ~MI_NO_WRITE_FLUSH;
	if (invalidate_domains & I915_GEM_DOMAIN_INSTRUCTION)
		cmd |= MI_EXE_FLUSH;

	if (invalidate_domains & I915_GEM_DOMAIN_COMMAND &&
	    (IS_G4X(dev) || IS_GEN5(dev)))
		cmd |= MI_INVALIDATE_ISP;

	ret = intel_ring_begin(ring, 2);
	if (ret)
		return ret;

	intel_ring_emit(ring, cmd);
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_advance(ring);

	return 0;
}

/**
 * Emits a PIPE_CONTROL with a non-zero post-sync operation, for
 * implementing two workarounds on gen6.  From section 1.4.7.1
 * "PIPE_CONTROL" of the Sandy Bridge PRM volume 2 part 1:
 *
 * [DevSNB-C+{W/A}] Before any depth stall flush (including those
 * produced by non-pipelined state commands), software needs to first
 * send a PIPE_CONTROL with no bits set except Post-Sync Operation !=
 * 0.
 *
 * [Dev-SNB{W/A}]: Before a PIPE_CONTROL with Write Cache Flush Enable
 * =1, a PIPE_CONTROL with any non-zero post-sync-op is required.
 *
 * And the workaround for these two requires this workaround first:
 *
 * [Dev-SNB{W/A}]: Pipe-control with CS-stall bit set must be sent
 * BEFORE the pipe-control with a post-sync op and no write-cache
 * flushes.
 *
 * And this last workaround is tricky because of the requirements on
 * that bit.  From section 1.4.7.2.3 "Stall" of the Sandy Bridge PRM
 * volume 2 part 1:
 *
 *     "1 of the following must also be set:
 *      - Render Target Cache Flush Enable ([12] of DW1)
 *      - Depth Cache Flush Enable ([0] of DW1)
 *      - Stall at Pixel Scoreboard ([1] of DW1)
 *      - Depth Stall ([13] of DW1)
 *      - Post-Sync Operation ([13] of DW1)
 *      - Notify Enable ([8] of DW1)"
 *
 * The cache flushes require the workaround flush that triggered this
 * one, so we can't use it.  Depth stall would trigger the same.
 * Post-sync nonzero is what triggered this second workaround, so we
 * can't use that one either.  Notify enable is IRQs, which aren't
 * really our business.  That leaves only stall at scoreboard.
 */
static int
<<<<<<< HEAD
<<<<<<< HEAD
intel_emit_post_sync_nonzero_flush(struct intel_ring_buffer *ring)
{
	struct pipe_control *pc = ring->private;
	u32 scratch_addr = pc->gtt_offset + 128;
=======
intel_emit_post_sync_nonzero_flush(struct intel_engine_cs *ring)
{
	u32 scratch_addr = ring->scratch.gtt_offset + 2 * CACHELINE_BYTES;
>>>>>>> v3.18
=======
intel_emit_post_sync_nonzero_flush(struct intel_engine_cs *ring)
{
	u32 scratch_addr = ring->scratch.gtt_offset + 2 * CACHELINE_BYTES;
>>>>>>> v3.18
	int ret;


	ret = intel_ring_begin(ring, 6);
	if (ret)
		return ret;

	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL(5));
	intel_ring_emit(ring, PIPE_CONTROL_CS_STALL |
			PIPE_CONTROL_STALL_AT_SCOREBOARD);
	intel_ring_emit(ring, scratch_addr | PIPE_CONTROL_GLOBAL_GTT); /* address */
	intel_ring_emit(ring, 0); /* low dword */
	intel_ring_emit(ring, 0); /* high dword */
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_advance(ring);

	ret = intel_ring_begin(ring, 6);
	if (ret)
		return ret;

	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL(5));
	intel_ring_emit(ring, PIPE_CONTROL_QW_WRITE);
	intel_ring_emit(ring, scratch_addr | PIPE_CONTROL_GLOBAL_GTT); /* address */
	intel_ring_emit(ring, 0);
	intel_ring_emit(ring, 0);
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_advance(ring);

	return 0;
}

static int
<<<<<<< HEAD
<<<<<<< HEAD
gen6_render_ring_flush(struct intel_ring_buffer *ring,
                         u32 invalidate_domains, u32 flush_domains)
{
	u32 flags = 0;
	struct pipe_control *pc = ring->private;
	u32 scratch_addr = pc->gtt_offset + 128;
=======
=======
>>>>>>> v3.18
gen6_render_ring_flush(struct intel_engine_cs *ring,
                         u32 invalidate_domains, u32 flush_domains)
{
	u32 flags = 0;
	u32 scratch_addr = ring->scratch.gtt_offset + 2 * CACHELINE_BYTES;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	int ret;

	/* Force SNB workarounds for PIPE_CONTROL flushes */
	ret = intel_emit_post_sync_nonzero_flush(ring);
	if (ret)
		return ret;

	/* Just flush everything.  Experiments have shown that reducing the
	 * number of bits based on the write domains has little performance
	 * impact.
	 */
	if (flush_domains) {
		flags |= PIPE_CONTROL_RENDER_TARGET_CACHE_FLUSH;
		flags |= PIPE_CONTROL_DEPTH_CACHE_FLUSH;
		/*
		 * Ensure that any following seqno writes only happen
		 * when the render cache is indeed flushed.
		 */
		flags |= PIPE_CONTROL_CS_STALL;
	}
	if (invalidate_domains) {
		flags |= PIPE_CONTROL_TLB_INVALIDATE;
		flags |= PIPE_CONTROL_INSTRUCTION_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_TEXTURE_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_VF_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_CONST_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_STATE_CACHE_INVALIDATE;
		/*
		 * TLB invalidate requires a post-sync write.
		 */
		flags |= PIPE_CONTROL_QW_WRITE | PIPE_CONTROL_CS_STALL;
	}

	ret = intel_ring_begin(ring, 4);
	if (ret)
		return ret;

	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL(4));
	intel_ring_emit(ring, flags);
	intel_ring_emit(ring, scratch_addr | PIPE_CONTROL_GLOBAL_GTT);
	intel_ring_emit(ring, 0);
	intel_ring_advance(ring);

	return 0;
}

static int
<<<<<<< HEAD
<<<<<<< HEAD
gen7_render_ring_cs_stall_wa(struct intel_ring_buffer *ring)
=======
gen7_render_ring_cs_stall_wa(struct intel_engine_cs *ring)
>>>>>>> v3.18
=======
gen7_render_ring_cs_stall_wa(struct intel_engine_cs *ring)
>>>>>>> v3.18
{
	int ret;

	ret = intel_ring_begin(ring, 4);
	if (ret)
		return ret;

	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL(4));
	intel_ring_emit(ring, PIPE_CONTROL_CS_STALL |
			      PIPE_CONTROL_STALL_AT_SCOREBOARD);
	intel_ring_emit(ring, 0);
	intel_ring_emit(ring, 0);
	intel_ring_advance(ring);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int
gen7_render_ring_flush(struct intel_ring_buffer *ring,
		       u32 invalidate_domains, u32 flush_domains)
{
	u32 flags = 0;
	struct pipe_control *pc = ring->private;
	u32 scratch_addr = pc->gtt_offset + 128;
=======
=======
>>>>>>> v3.18
static int gen7_ring_fbc_flush(struct intel_engine_cs *ring, u32 value)
{
	int ret;

	if (!ring->fbc_dirty)
		return 0;

	ret = intel_ring_begin(ring, 6);
	if (ret)
		return ret;
	/* WaFbcNukeOn3DBlt:ivb/hsw */
	intel_ring_emit(ring, MI_LOAD_REGISTER_IMM(1));
	intel_ring_emit(ring, MSG_FBC_REND_STATE);
	intel_ring_emit(ring, value);
	intel_ring_emit(ring, MI_STORE_REGISTER_MEM(1) | MI_SRM_LRM_GLOBAL_GTT);
	intel_ring_emit(ring, MSG_FBC_REND_STATE);
	intel_ring_emit(ring, ring->scratch.gtt_offset + 256);
	intel_ring_advance(ring);

	ring->fbc_dirty = false;
	return 0;
}

static int
gen7_render_ring_flush(struct intel_engine_cs *ring,
		       u32 invalidate_domains, u32 flush_domains)
{
	u32 flags = 0;
	u32 scratch_addr = ring->scratch.gtt_offset + 2 * CACHELINE_BYTES;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	int ret;

	/*
	 * Ensure that any following seqno writes only happen when the render
	 * cache is indeed flushed.
	 *
	 * Workaround: 4th PIPE_CONTROL command (except the ones with only
	 * read-cache invalidate bits set) must have the CS_STALL bit set. We
	 * don't try to be clever and just set it unconditionally.
	 */
	flags |= PIPE_CONTROL_CS_STALL;

	/* Just flush everything.  Experiments have shown that reducing the
	 * number of bits based on the write domains has little performance
	 * impact.
	 */
	if (flush_domains) {
		flags |= PIPE_CONTROL_RENDER_TARGET_CACHE_FLUSH;
		flags |= PIPE_CONTROL_DEPTH_CACHE_FLUSH;
	}
	if (invalidate_domains) {
		flags |= PIPE_CONTROL_TLB_INVALIDATE;
		flags |= PIPE_CONTROL_INSTRUCTION_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_TEXTURE_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_VF_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_CONST_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_STATE_CACHE_INVALIDATE;
<<<<<<< HEAD
<<<<<<< HEAD
		flags |= PIPE_CONTROL_MEDIA_STATE_CLEAR;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
		/*
		 * TLB invalidate requires a post-sync write.
		 */
		flags |= PIPE_CONTROL_QW_WRITE;
		flags |= PIPE_CONTROL_GLOBAL_GTT_IVB;

<<<<<<< HEAD
<<<<<<< HEAD
		flags |= PIPE_CONTROL_STALL_AT_SCOREBOARD;

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
		/* Workaround: we must issue a pipe_control with CS-stall bit
		 * set before a pipe_control command that has the state cache
		 * invalidate bit set. */
		gen7_render_ring_cs_stall_wa(ring);
	}

	ret = intel_ring_begin(ring, 4);
	if (ret)
		return ret;

	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL(4));
	intel_ring_emit(ring, flags);
	intel_ring_emit(ring, scratch_addr);
	intel_ring_emit(ring, 0);
	intel_ring_advance(ring);

<<<<<<< HEAD
<<<<<<< HEAD
	return 0;
}

static void ring_write_tail(struct intel_ring_buffer *ring,
			    u32 value)
{
	drm_i915_private_t *dev_priv = ring->dev->dev_private;
	I915_WRITE_TAIL(ring, value);
}

u32 intel_ring_get_active_head(struct intel_ring_buffer *ring)
{
	drm_i915_private_t *dev_priv = ring->dev->dev_private;
	u32 acthd_reg = INTEL_INFO(ring->dev)->gen >= 4 ?
			RING_ACTHD(ring->mmio_base) : ACTHD;

	return I915_READ(acthd_reg);
}

static int init_ring_common(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_i915_gem_object *obj = ring->obj;
	int ret = 0;
	u32 head;

	if (HAS_FORCE_WAKE(dev))
		gen6_gt_force_wake_get(dev_priv);

	/* Stop the ring if it's running. */
=======
=======
>>>>>>> v3.18
	if (!invalidate_domains && flush_domains)
		return gen7_ring_fbc_flush(ring, FBC_REND_NUKE);

	return 0;
}

static int
gen8_emit_pipe_control(struct intel_engine_cs *ring,
		       u32 flags, u32 scratch_addr)
{
	int ret;

	ret = intel_ring_begin(ring, 6);
	if (ret)
		return ret;

	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL(6));
	intel_ring_emit(ring, flags);
	intel_ring_emit(ring, scratch_addr);
	intel_ring_emit(ring, 0);
	intel_ring_emit(ring, 0);
	intel_ring_emit(ring, 0);
	intel_ring_advance(ring);

	return 0;
}

static int
gen8_render_ring_flush(struct intel_engine_cs *ring,
		       u32 invalidate_domains, u32 flush_domains)
{
	u32 flags = 0;
	u32 scratch_addr = ring->scratch.gtt_offset + 2 * CACHELINE_BYTES;
	int ret;

	flags |= PIPE_CONTROL_CS_STALL;

	if (flush_domains) {
		flags |= PIPE_CONTROL_RENDER_TARGET_CACHE_FLUSH;
		flags |= PIPE_CONTROL_DEPTH_CACHE_FLUSH;
	}
	if (invalidate_domains) {
		flags |= PIPE_CONTROL_TLB_INVALIDATE;
		flags |= PIPE_CONTROL_INSTRUCTION_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_TEXTURE_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_VF_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_CONST_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_STATE_CACHE_INVALIDATE;
		flags |= PIPE_CONTROL_QW_WRITE;
		flags |= PIPE_CONTROL_GLOBAL_GTT_IVB;

		/* WaCsStallBeforeStateCacheInvalidate:bdw,chv */
		ret = gen8_emit_pipe_control(ring,
					     PIPE_CONTROL_CS_STALL |
					     PIPE_CONTROL_STALL_AT_SCOREBOARD,
					     0);
		if (ret)
			return ret;
	}

	ret = gen8_emit_pipe_control(ring, flags, scratch_addr);
	if (ret)
		return ret;

	if (!invalidate_domains && flush_domains)
		return gen7_ring_fbc_flush(ring, FBC_REND_NUKE);

	return 0;
}

static void ring_write_tail(struct intel_engine_cs *ring,
			    u32 value)
{
	struct drm_i915_private *dev_priv = ring->dev->dev_private;
	I915_WRITE_TAIL(ring, value);
}

u64 intel_ring_get_active_head(struct intel_engine_cs *ring)
{
	struct drm_i915_private *dev_priv = ring->dev->dev_private;
	u64 acthd;

	if (INTEL_INFO(ring->dev)->gen >= 8)
		acthd = I915_READ64_2x32(RING_ACTHD(ring->mmio_base),
					 RING_ACTHD_UDW(ring->mmio_base));
	else if (INTEL_INFO(ring->dev)->gen >= 4)
		acthd = I915_READ(RING_ACTHD(ring->mmio_base));
	else
		acthd = I915_READ(ACTHD);

	return acthd;
}

static void ring_setup_phys_status_page(struct intel_engine_cs *ring)
{
	struct drm_i915_private *dev_priv = ring->dev->dev_private;
	u32 addr;

	addr = dev_priv->status_page_dmah->busaddr;
	if (INTEL_INFO(ring->dev)->gen >= 4)
		addr |= (dev_priv->status_page_dmah->busaddr >> 28) & 0xf0;
	I915_WRITE(HWS_PGA, addr);
}

static bool stop_ring(struct intel_engine_cs *ring)
{
	struct drm_i915_private *dev_priv = to_i915(ring->dev);

	if (!IS_GEN2(ring->dev)) {
		I915_WRITE_MODE(ring, _MASKED_BIT_ENABLE(STOP_RING));
		if (wait_for((I915_READ_MODE(ring) & MODE_IDLE) != 0, 1000)) {
			DRM_ERROR("%s : timed out trying to stop ring\n", ring->name);
			/* Sometimes we observe that the idle flag is not
			 * set even though the ring is empty. So double
			 * check before giving up.
			 */
			if (I915_READ_HEAD(ring) != I915_READ_TAIL(ring))
				return false;
		}
	}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	I915_WRITE_CTL(ring, 0);
	I915_WRITE_HEAD(ring, 0);
	ring->write_tail(ring, 0);

<<<<<<< HEAD
<<<<<<< HEAD
	head = I915_READ_HEAD(ring) & HEAD_ADDR;

	/* G45 ring initialization fails to reset head to zero */
	if (head != 0) {
=======
=======
>>>>>>> v3.18
	if (!IS_GEN2(ring->dev)) {
		(void)I915_READ_CTL(ring);
		I915_WRITE_MODE(ring, _MASKED_BIT_DISABLE(STOP_RING));
	}

	return (I915_READ_HEAD(ring) & HEAD_ADDR) == 0;
}

static int init_ring_common(struct intel_engine_cs *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_ringbuffer *ringbuf = ring->buffer;
	struct drm_i915_gem_object *obj = ringbuf->obj;
	int ret = 0;

	gen6_gt_force_wake_get(dev_priv, FORCEWAKE_ALL);

	if (!stop_ring(ring)) {
		/* G45 ring initialization often fails to reset head to zero */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		DRM_DEBUG_KMS("%s head not reset to zero "
			      "ctl %08x head %08x tail %08x start %08x\n",
			      ring->name,
			      I915_READ_CTL(ring),
			      I915_READ_HEAD(ring),
			      I915_READ_TAIL(ring),
			      I915_READ_START(ring));

<<<<<<< HEAD
<<<<<<< HEAD
		I915_WRITE_HEAD(ring, 0);

		if (I915_READ_HEAD(ring) & HEAD_ADDR) {
=======
		if (!stop_ring(ring)) {
>>>>>>> v3.18
=======
		if (!stop_ring(ring)) {
>>>>>>> v3.18
			DRM_ERROR("failed to set %s head to zero "
				  "ctl %08x head %08x tail %08x start %08x\n",
				  ring->name,
				  I915_READ_CTL(ring),
				  I915_READ_HEAD(ring),
				  I915_READ_TAIL(ring),
				  I915_READ_START(ring));
<<<<<<< HEAD
<<<<<<< HEAD
		}
	}

=======
=======
>>>>>>> v3.18
			ret = -EIO;
			goto out;
		}
	}

	if (I915_NEED_GFX_HWS(dev))
		intel_ring_setup_status_page(ring);
	else
		ring_setup_phys_status_page(ring);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	/* Enforce ordering by reading HEAD register back */
	I915_READ_HEAD(ring);

	/* Initialize the ring. This must happen _after_ we've cleared the ring
	 * registers with the above sequence (the readback of the HEAD registers
	 * also enforces ordering), otherwise the hw might lose the new ring
	 * register values. */
<<<<<<< HEAD
<<<<<<< HEAD
	I915_WRITE_START(ring, obj->gtt_offset);
	I915_WRITE_CTL(ring,
			((ring->size - PAGE_SIZE) & RING_NR_PAGES)
=======
=======
>>>>>>> v3.18
	I915_WRITE_START(ring, i915_gem_obj_ggtt_offset(obj));

	/* WaClearRingBufHeadRegAtInit:ctg,elk */
	if (I915_READ_HEAD(ring))
		DRM_DEBUG("%s initialization failed [head=%08x], fudging\n",
			  ring->name, I915_READ_HEAD(ring));
	I915_WRITE_HEAD(ring, 0);
	(void)I915_READ_HEAD(ring);

	I915_WRITE_CTL(ring,
			((ringbuf->size - PAGE_SIZE) & RING_NR_PAGES)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			| RING_VALID);

	/* If the head is still not zero, the ring is dead */
	if (wait_for((I915_READ_CTL(ring) & RING_VALID) != 0 &&
<<<<<<< HEAD
<<<<<<< HEAD
		     I915_READ_START(ring) == obj->gtt_offset &&
		     (I915_READ_HEAD(ring) & HEAD_ADDR) == 0, 50)) {
		DRM_ERROR("%s initialization failed "
				"ctl %08x head %08x tail %08x start %08x\n",
				ring->name,
				I915_READ_CTL(ring),
				I915_READ_HEAD(ring),
				I915_READ_TAIL(ring),
				I915_READ_START(ring));
=======
=======
>>>>>>> v3.18
		     I915_READ_START(ring) == i915_gem_obj_ggtt_offset(obj) &&
		     (I915_READ_HEAD(ring) & HEAD_ADDR) == 0, 50)) {
		DRM_ERROR("%s initialization failed "
			  "ctl %08x (valid? %d) head %08x tail %08x start %08x [expected %08lx]\n",
			  ring->name,
			  I915_READ_CTL(ring), I915_READ_CTL(ring) & RING_VALID,
			  I915_READ_HEAD(ring), I915_READ_TAIL(ring),
			  I915_READ_START(ring), (unsigned long)i915_gem_obj_ggtt_offset(obj));
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		ret = -EIO;
		goto out;
	}

	if (!drm_core_check_feature(ring->dev, DRIVER_MODESET))
		i915_kernel_lost_context(ring->dev);
	else {
<<<<<<< HEAD
<<<<<<< HEAD
		ring->head = I915_READ_HEAD(ring);
		ring->tail = I915_READ_TAIL(ring) & TAIL_ADDR;
		ring->space = ring_space(ring);
		ring->last_retired_head = -1;
	}

out:
	if (HAS_FORCE_WAKE(dev))
		gen6_gt_force_wake_put(dev_priv);
=======
=======
>>>>>>> v3.18
		ringbuf->head = I915_READ_HEAD(ring);
		ringbuf->tail = I915_READ_TAIL(ring) & TAIL_ADDR;
		ringbuf->space = intel_ring_space(ringbuf);
		ringbuf->last_retired_head = -1;
	}

	memset(&ring->hangcheck, 0, sizeof(ring->hangcheck));

out:
	gen6_gt_force_wake_put(dev_priv, FORCEWAKE_ALL);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int
init_pipe_control(struct intel_ring_buffer *ring)
{
	struct pipe_control *pc;
	struct drm_i915_gem_object *obj;
	int ret;

	if (ring->private)
		return 0;

	pc = kmalloc(sizeof(*pc), GFP_KERNEL);
	if (!pc)
		return -ENOMEM;

	obj = i915_gem_alloc_object(ring->dev, 4096);
	if (obj == NULL) {
=======
=======
>>>>>>> v3.18
void
intel_fini_pipe_control(struct intel_engine_cs *ring)
{
	struct drm_device *dev = ring->dev;

	if (ring->scratch.obj == NULL)
		return;

	if (INTEL_INFO(dev)->gen >= 5) {
		kunmap(sg_page(ring->scratch.obj->pages->sgl));
		i915_gem_object_ggtt_unpin(ring->scratch.obj);
	}

	drm_gem_object_unreference(&ring->scratch.obj->base);
	ring->scratch.obj = NULL;
}

int
intel_init_pipe_control(struct intel_engine_cs *ring)
{
	int ret;

	if (ring->scratch.obj)
		return 0;

	ring->scratch.obj = i915_gem_alloc_object(ring->dev, 4096);
	if (ring->scratch.obj == NULL) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		DRM_ERROR("Failed to allocate seqno page\n");
		ret = -ENOMEM;
		goto err;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	i915_gem_object_set_cache_level(obj, I915_CACHE_LLC);

	ret = i915_gem_object_pin(obj, 4096, true, false);
	if (ret)
		goto err_unref;

	pc->gtt_offset = obj->gtt_offset;
	pc->cpu_page =  kmap(sg_page(obj->pages->sgl));
	if (pc->cpu_page == NULL)
		goto err_unpin;

	DRM_DEBUG_DRIVER("%s pipe control offset: 0x%08x\n",
			 ring->name, pc->gtt_offset);

	pc->obj = obj;
	ring->private = pc;
	return 0;

err_unpin:
	i915_gem_object_unpin(obj);
err_unref:
	drm_gem_object_unreference(&obj->base);
err:
	kfree(pc);
	return ret;
}

static void
cleanup_pipe_control(struct intel_ring_buffer *ring)
{
	struct pipe_control *pc = ring->private;
	struct drm_i915_gem_object *obj;

	obj = pc->obj;

	kunmap(sg_page(obj->pages->sgl));
	i915_gem_object_unpin(obj);
	drm_gem_object_unreference(&obj->base);

	kfree(pc);
}

static int init_render_ring(struct intel_ring_buffer *ring)
=======
=======
>>>>>>> v3.18
	ret = i915_gem_object_set_cache_level(ring->scratch.obj, I915_CACHE_LLC);
	if (ret)
		goto err_unref;

	ret = i915_gem_obj_ggtt_pin(ring->scratch.obj, 4096, 0);
	if (ret)
		goto err_unref;

	ring->scratch.gtt_offset = i915_gem_obj_ggtt_offset(ring->scratch.obj);
	ring->scratch.cpu_page = kmap(sg_page(ring->scratch.obj->pages->sgl));
	if (ring->scratch.cpu_page == NULL) {
		ret = -ENOMEM;
		goto err_unpin;
	}

	DRM_DEBUG_DRIVER("%s pipe control offset: 0x%08x\n",
			 ring->name, ring->scratch.gtt_offset);
	return 0;

err_unpin:
	i915_gem_object_ggtt_unpin(ring->scratch.obj);
err_unref:
	drm_gem_object_unreference(&ring->scratch.obj->base);
err:
	return ret;
}

static inline void intel_ring_emit_wa(struct intel_engine_cs *ring,
				       u32 addr, u32 value)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (WARN_ON(dev_priv->num_wa_regs >= I915_MAX_WA_REGS))
		return;

	intel_ring_emit(ring, MI_LOAD_REGISTER_IMM(1));
	intel_ring_emit(ring, addr);
	intel_ring_emit(ring, value);

	dev_priv->intel_wa_regs[dev_priv->num_wa_regs].addr = addr;
	dev_priv->intel_wa_regs[dev_priv->num_wa_regs].mask = value & 0xFFFF;
	/* value is updated with the status of remaining bits of this
	 * register when it is read from debugfs file
	 */
	dev_priv->intel_wa_regs[dev_priv->num_wa_regs].value = value;
	dev_priv->num_wa_regs++;

	return;
}

static int bdw_init_workarounds(struct intel_engine_cs *ring)
{
	int ret;
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;

	/*
	 * workarounds applied in this fn are part of register state context,
	 * they need to be re-initialized followed by gpu reset, suspend/resume,
	 * module reload.
	 */
	dev_priv->num_wa_regs = 0;
	memset(dev_priv->intel_wa_regs, 0, sizeof(dev_priv->intel_wa_regs));

	/*
	 * update the number of dwords required based on the
	 * actual number of workarounds applied
	 */
	ret = intel_ring_begin(ring, 18);
	if (ret)
		return ret;

	/* WaDisablePartialInstShootdown:bdw */
	/* WaDisableThreadStallDopClockGating:bdw */
	/* FIXME: Unclear whether we really need this on production bdw. */
	intel_ring_emit_wa(ring, GEN8_ROW_CHICKEN,
			   _MASKED_BIT_ENABLE(PARTIAL_INSTRUCTION_SHOOTDOWN_DISABLE
					     | STALL_DOP_GATING_DISABLE));

	/* WaDisableDopClockGating:bdw May not be needed for production */
	intel_ring_emit_wa(ring, GEN7_ROW_CHICKEN2,
			   _MASKED_BIT_ENABLE(DOP_CLOCK_GATING_DISABLE));

	intel_ring_emit_wa(ring, HALF_SLICE_CHICKEN3,
			   _MASKED_BIT_ENABLE(GEN8_SAMPLER_POWER_BYPASS_DIS));

	/* Use Force Non-Coherent whenever executing a 3D context. This is a
	 * workaround for for a possible hang in the unlikely event a TLB
	 * invalidation occurs during a PSD flush.
	 */
	intel_ring_emit_wa(ring, HDC_CHICKEN0,
			   _MASKED_BIT_ENABLE(HDC_FORCE_NON_COHERENT));

	/* Wa4x4STCOptimizationDisable:bdw */
	intel_ring_emit_wa(ring, CACHE_MODE_1,
			   _MASKED_BIT_ENABLE(GEN8_4x4_STC_OPTIMIZATION_DISABLE));

	/*
	 * BSpec recommends 8x4 when MSAA is used,
	 * however in practice 16x4 seems fastest.
	 *
	 * Note that PS/WM thread counts depend on the WIZ hashing
	 * disable bit, which we don't touch here, but it's good
	 * to keep in mind (see 3DSTATE_PS and 3DSTATE_WM).
	 */
	intel_ring_emit_wa(ring, GEN7_GT_MODE,
			   GEN6_WIZ_HASHING_MASK | GEN6_WIZ_HASHING_16x4);

	intel_ring_advance(ring);

	DRM_DEBUG_DRIVER("Number of Workarounds applied: %d\n",
			 dev_priv->num_wa_regs);

	return 0;
}

static int chv_init_workarounds(struct intel_engine_cs *ring)
{
	int ret;
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;

	/*
	 * workarounds applied in this fn are part of register state context,
	 * they need to be re-initialized followed by gpu reset, suspend/resume,
	 * module reload.
	 */
	dev_priv->num_wa_regs = 0;
	memset(dev_priv->intel_wa_regs, 0, sizeof(dev_priv->intel_wa_regs));

	ret = intel_ring_begin(ring, 12);
	if (ret)
		return ret;

	/* WaDisablePartialInstShootdown:chv */
	intel_ring_emit_wa(ring, GEN8_ROW_CHICKEN,
			   _MASKED_BIT_ENABLE(PARTIAL_INSTRUCTION_SHOOTDOWN_DISABLE));

	/* WaDisableThreadStallDopClockGating:chv */
	intel_ring_emit_wa(ring, GEN8_ROW_CHICKEN,
			   _MASKED_BIT_ENABLE(STALL_DOP_GATING_DISABLE));

	/* WaDisableDopClockGating:chv (pre-production hw) */
	intel_ring_emit_wa(ring, GEN7_ROW_CHICKEN2,
			   _MASKED_BIT_ENABLE(DOP_CLOCK_GATING_DISABLE));

	/* WaDisableSamplerPowerBypass:chv (pre-production hw) */
	intel_ring_emit_wa(ring, HALF_SLICE_CHICKEN3,
			   _MASKED_BIT_ENABLE(GEN8_SAMPLER_POWER_BYPASS_DIS));

	intel_ring_advance(ring);

	return 0;
}

static int init_render_ring(struct intel_engine_cs *ring)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int ret = init_ring_common(ring);
<<<<<<< HEAD
<<<<<<< HEAD

	if (INTEL_INFO(dev)->gen > 3)
=======
=======
>>>>>>> v3.18
	if (ret)
		return ret;

	/* WaTimedSingleVertexDispatch:cl,bw,ctg,elk,ilk,snb */
	if (INTEL_INFO(dev)->gen >= 4 && INTEL_INFO(dev)->gen < 7)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		I915_WRITE(MI_MODE, _MASKED_BIT_ENABLE(VS_TIMER_DISPATCH));

	/* We need to disable the AsyncFlip performance optimisations in order
	 * to use MI_WAIT_FOR_EVENT within the CS. It should already be
	 * programmed to '1' on all products.
<<<<<<< HEAD
<<<<<<< HEAD
=======
	 *
	 * WaDisableAsyncFlipPerfMode:snb,ivb,hsw,vlv,bdw,chv
>>>>>>> v3.18
=======
	 *
	 * WaDisableAsyncFlipPerfMode:snb,ivb,hsw,vlv,bdw,chv
>>>>>>> v3.18
	 */
	if (INTEL_INFO(dev)->gen >= 6)
		I915_WRITE(MI_MODE, _MASKED_BIT_ENABLE(ASYNC_FLIP_PERF_DISABLE));

	/* Required for the hardware to program scanline values for waiting */
<<<<<<< HEAD
<<<<<<< HEAD
	if (INTEL_INFO(dev)->gen == 6)
		I915_WRITE(GFX_MODE,
			   _MASKED_BIT_ENABLE(GFX_TLB_INVALIDATE_ALWAYS));

	if (IS_GEN7(dev))
		I915_WRITE(GFX_MODE_GEN7,
			   _MASKED_BIT_DISABLE(GFX_TLB_INVALIDATE_ALWAYS) |
			   _MASKED_BIT_ENABLE(GFX_REPLAY_MODE));

	if (INTEL_INFO(dev)->gen >= 5) {
		ret = init_pipe_control(ring);
=======
=======
>>>>>>> v3.18
	/* WaEnableFlushTlbInvalidationMode:snb */
	if (INTEL_INFO(dev)->gen == 6)
		I915_WRITE(GFX_MODE,
			   _MASKED_BIT_ENABLE(GFX_TLB_INVALIDATE_EXPLICIT));

	/* WaBCSVCSTlbInvalidationMode:ivb,vlv,hsw */
	if (IS_GEN7(dev))
		I915_WRITE(GFX_MODE_GEN7,
			   _MASKED_BIT_ENABLE(GFX_TLB_INVALIDATE_EXPLICIT) |
			   _MASKED_BIT_ENABLE(GFX_REPLAY_MODE));

	if (INTEL_INFO(dev)->gen >= 5) {
		ret = intel_init_pipe_control(ring);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		if (ret)
			return ret;
	}

	if (IS_GEN6(dev)) {
		/* From the Sandybridge PRM, volume 1 part 3, page 24:
		 * "If this bit is set, STCunit will have LRA as replacement
		 *  policy. [...] This bit must be reset.  LRA replacement
		 *  policy is not supported."
		 */
		I915_WRITE(CACHE_MODE_0,
			   _MASKED_BIT_DISABLE(CM0_STC_EVICT_DISABLE_LRA_SNB));
<<<<<<< HEAD
<<<<<<< HEAD

		/* This is not explicitly set for GEN6, so read the register.
		 * see intel_ring_mi_set_context() for why we care.
		 * TODO: consider explicitly setting the bit for GEN5
		 */
		ring->itlb_before_ctx_switch =
			!!(I915_READ(GFX_MODE) & GFX_TLB_INVALIDATE_ALWAYS);
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}

	if (INTEL_INFO(dev)->gen >= 6)
		I915_WRITE(INSTPM, _MASKED_BIT_ENABLE(INSTPM_FORCE_ORDERING));

<<<<<<< HEAD
<<<<<<< HEAD
	if (HAS_L3_GPU_CACHE(dev))
		I915_WRITE_IMR(ring, ~GEN6_RENDER_L3_PARITY_ERROR);
=======
	if (HAS_L3_DPF(dev))
		I915_WRITE_IMR(ring, ~GT_PARITY_ERROR(dev));
>>>>>>> v3.18
=======
	if (HAS_L3_DPF(dev))
		I915_WRITE_IMR(ring, ~GT_PARITY_ERROR(dev));
>>>>>>> v3.18

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void render_ring_cleanup(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;

	if (!ring->private)
		return;

	if (HAS_BROKEN_CS_TLB(dev))
		drm_gem_object_unreference(to_gem_object(ring->private));

	if (INTEL_INFO(dev)->gen >= 5)
		cleanup_pipe_control(ring);

	ring->private = NULL;
}

static void
update_mboxes(struct intel_ring_buffer *ring,
	      u32 mmio_offset)
{
	intel_ring_emit(ring, MI_LOAD_REGISTER_IMM(1));
	intel_ring_emit(ring, mmio_offset);
	intel_ring_emit(ring, ring->outstanding_lazy_request);
=======
=======
>>>>>>> v3.18
static void render_ring_cleanup(struct intel_engine_cs *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (dev_priv->semaphore_obj) {
		i915_gem_object_ggtt_unpin(dev_priv->semaphore_obj);
		drm_gem_object_unreference(&dev_priv->semaphore_obj->base);
		dev_priv->semaphore_obj = NULL;
	}

	intel_fini_pipe_control(ring);
}

static int gen8_rcs_signal(struct intel_engine_cs *signaller,
			   unsigned int num_dwords)
{
#define MBOX_UPDATE_DWORDS 8
	struct drm_device *dev = signaller->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_engine_cs *waiter;
	int i, ret, num_rings;

	num_rings = hweight32(INTEL_INFO(dev)->ring_mask);
	num_dwords += (num_rings-1) * MBOX_UPDATE_DWORDS;
#undef MBOX_UPDATE_DWORDS

	ret = intel_ring_begin(signaller, num_dwords);
	if (ret)
		return ret;

	for_each_ring(waiter, dev_priv, i) {
		u64 gtt_offset = signaller->semaphore.signal_ggtt[i];
		if (gtt_offset == MI_SEMAPHORE_SYNC_INVALID)
			continue;

		intel_ring_emit(signaller, GFX_OP_PIPE_CONTROL(6));
		intel_ring_emit(signaller, PIPE_CONTROL_GLOBAL_GTT_IVB |
					   PIPE_CONTROL_QW_WRITE |
					   PIPE_CONTROL_FLUSH_ENABLE);
		intel_ring_emit(signaller, lower_32_bits(gtt_offset));
		intel_ring_emit(signaller, upper_32_bits(gtt_offset));
		intel_ring_emit(signaller, signaller->outstanding_lazy_seqno);
		intel_ring_emit(signaller, 0);
		intel_ring_emit(signaller, MI_SEMAPHORE_SIGNAL |
					   MI_SEMAPHORE_TARGET(waiter->id));
		intel_ring_emit(signaller, 0);
	}

	return 0;
}

static int gen8_xcs_signal(struct intel_engine_cs *signaller,
			   unsigned int num_dwords)
{
#define MBOX_UPDATE_DWORDS 6
	struct drm_device *dev = signaller->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_engine_cs *waiter;
	int i, ret, num_rings;

	num_rings = hweight32(INTEL_INFO(dev)->ring_mask);
	num_dwords += (num_rings-1) * MBOX_UPDATE_DWORDS;
#undef MBOX_UPDATE_DWORDS

	ret = intel_ring_begin(signaller, num_dwords);
	if (ret)
		return ret;

	for_each_ring(waiter, dev_priv, i) {
		u64 gtt_offset = signaller->semaphore.signal_ggtt[i];
		if (gtt_offset == MI_SEMAPHORE_SYNC_INVALID)
			continue;

		intel_ring_emit(signaller, (MI_FLUSH_DW + 1) |
					   MI_FLUSH_DW_OP_STOREDW);
		intel_ring_emit(signaller, lower_32_bits(gtt_offset) |
					   MI_FLUSH_DW_USE_GTT);
		intel_ring_emit(signaller, upper_32_bits(gtt_offset));
		intel_ring_emit(signaller, signaller->outstanding_lazy_seqno);
		intel_ring_emit(signaller, MI_SEMAPHORE_SIGNAL |
					   MI_SEMAPHORE_TARGET(waiter->id));
		intel_ring_emit(signaller, 0);
	}

	return 0;
}

static int gen6_signal(struct intel_engine_cs *signaller,
		       unsigned int num_dwords)
{
	struct drm_device *dev = signaller->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_engine_cs *useless;
	int i, ret, num_rings;

#define MBOX_UPDATE_DWORDS 3
	num_rings = hweight32(INTEL_INFO(dev)->ring_mask);
	num_dwords += round_up((num_rings-1) * MBOX_UPDATE_DWORDS, 2);
#undef MBOX_UPDATE_DWORDS

	ret = intel_ring_begin(signaller, num_dwords);
	if (ret)
		return ret;

	for_each_ring(useless, dev_priv, i) {
		u32 mbox_reg = signaller->semaphore.mbox.signal[i];
		if (mbox_reg != GEN6_NOSYNC) {
			intel_ring_emit(signaller, MI_LOAD_REGISTER_IMM(1));
			intel_ring_emit(signaller, mbox_reg);
			intel_ring_emit(signaller, signaller->outstanding_lazy_seqno);
		}
	}

	/* If num_dwords was rounded, make sure the tail pointer is correct */
	if (num_rings % 2 == 0)
		intel_ring_emit(signaller, MI_NOOP);

	return 0;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/**
 * gen6_add_request - Update the semaphore mailbox registers
 * 
 * @ring - ring that is adding a request
 * @seqno - return seqno stuck into the ring
 *
 * Update the mailbox registers in the *other* rings with the current seqno.
 * This acts like a signal in the canonical semaphore.
 */
static int
<<<<<<< HEAD
<<<<<<< HEAD
gen6_add_request(struct intel_ring_buffer *ring)
{
	u32 mbox1_reg;
	u32 mbox2_reg;
	int ret;

	ret = intel_ring_begin(ring, 10);
	if (ret)
		return ret;

	mbox1_reg = ring->signal_mbox[0];
	mbox2_reg = ring->signal_mbox[1];

	update_mboxes(ring, mbox1_reg);
	update_mboxes(ring, mbox2_reg);
	intel_ring_emit(ring, MI_STORE_DWORD_INDEX);
	intel_ring_emit(ring, I915_GEM_HWS_INDEX << MI_STORE_DWORD_INDEX_SHIFT);
	intel_ring_emit(ring, ring->outstanding_lazy_request);
	intel_ring_emit(ring, MI_USER_INTERRUPT);
	intel_ring_advance(ring);
=======
=======
>>>>>>> v3.18
gen6_add_request(struct intel_engine_cs *ring)
{
	int ret;

	if (ring->semaphore.signal)
		ret = ring->semaphore.signal(ring, 4);
	else
		ret = intel_ring_begin(ring, 4);

	if (ret)
		return ret;

	intel_ring_emit(ring, MI_STORE_DWORD_INDEX);
	intel_ring_emit(ring, I915_GEM_HWS_INDEX << MI_STORE_DWORD_INDEX_SHIFT);
	intel_ring_emit(ring, ring->outstanding_lazy_seqno);
	intel_ring_emit(ring, MI_USER_INTERRUPT);
	__intel_ring_advance(ring);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return 0;
}

static inline bool i915_gem_has_seqno_wrapped(struct drm_device *dev,
					      u32 seqno)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	return dev_priv->last_seqno < seqno;
}

/**
 * intel_ring_sync - sync the waiter to the signaller on seqno
 *
 * @waiter - ring that is waiting
 * @signaller - ring which has, or will signal
 * @seqno - seqno which the waiter will block on
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int
gen6_ring_sync(struct intel_ring_buffer *waiter,
	       struct intel_ring_buffer *signaller,
	       u32 seqno)
{
	int ret;
	u32 dw1 = MI_SEMAPHORE_MBOX |
		  MI_SEMAPHORE_COMPARE |
		  MI_SEMAPHORE_REGISTER;
=======
=======
>>>>>>> v3.18

static int
gen8_ring_sync(struct intel_engine_cs *waiter,
	       struct intel_engine_cs *signaller,
	       u32 seqno)
{
	struct drm_i915_private *dev_priv = waiter->dev->dev_private;
	int ret;

	ret = intel_ring_begin(waiter, 4);
	if (ret)
		return ret;

	intel_ring_emit(waiter, MI_SEMAPHORE_WAIT |
				MI_SEMAPHORE_GLOBAL_GTT |
				MI_SEMAPHORE_POLL |
				MI_SEMAPHORE_SAD_GTE_SDD);
	intel_ring_emit(waiter, seqno);
	intel_ring_emit(waiter,
			lower_32_bits(GEN8_WAIT_OFFSET(waiter, signaller->id)));
	intel_ring_emit(waiter,
			upper_32_bits(GEN8_WAIT_OFFSET(waiter, signaller->id)));
	intel_ring_advance(waiter);
	return 0;
}

static int
gen6_ring_sync(struct intel_engine_cs *waiter,
	       struct intel_engine_cs *signaller,
	       u32 seqno)
{
	u32 dw1 = MI_SEMAPHORE_MBOX |
		  MI_SEMAPHORE_COMPARE |
		  MI_SEMAPHORE_REGISTER;
	u32 wait_mbox = signaller->semaphore.mbox.wait[waiter->id];
	int ret;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* Throughout all of the GEM code, seqno passed implies our current
	 * seqno is >= the last seqno executed. However for hardware the
	 * comparison is strictly greater than.
	 */
	seqno -= 1;

<<<<<<< HEAD
<<<<<<< HEAD
	WARN_ON(signaller->semaphore_register[waiter->id] ==
		MI_SEMAPHORE_SYNC_INVALID);
=======
	WARN_ON(wait_mbox == MI_SEMAPHORE_SYNC_INVALID);
>>>>>>> v3.18
=======
	WARN_ON(wait_mbox == MI_SEMAPHORE_SYNC_INVALID);
>>>>>>> v3.18

	ret = intel_ring_begin(waiter, 4);
	if (ret)
		return ret;

	/* If seqno wrap happened, omit the wait with no-ops */
	if (likely(!i915_gem_has_seqno_wrapped(waiter->dev, seqno))) {
<<<<<<< HEAD
<<<<<<< HEAD
		intel_ring_emit(waiter,
				dw1 |
				signaller->semaphore_register[waiter->id]);
=======
		intel_ring_emit(waiter, dw1 | wait_mbox);
>>>>>>> v3.18
=======
		intel_ring_emit(waiter, dw1 | wait_mbox);
>>>>>>> v3.18
		intel_ring_emit(waiter, seqno);
		intel_ring_emit(waiter, 0);
		intel_ring_emit(waiter, MI_NOOP);
	} else {
		intel_ring_emit(waiter, MI_NOOP);
		intel_ring_emit(waiter, MI_NOOP);
		intel_ring_emit(waiter, MI_NOOP);
		intel_ring_emit(waiter, MI_NOOP);
	}
	intel_ring_advance(waiter);

	return 0;
}

#define PIPE_CONTROL_FLUSH(ring__, addr__)					\
do {									\
	intel_ring_emit(ring__, GFX_OP_PIPE_CONTROL(4) | PIPE_CONTROL_QW_WRITE |		\
		 PIPE_CONTROL_DEPTH_STALL);				\
	intel_ring_emit(ring__, (addr__) | PIPE_CONTROL_GLOBAL_GTT);			\
	intel_ring_emit(ring__, 0);							\
	intel_ring_emit(ring__, 0);							\
} while (0)

static int
<<<<<<< HEAD
<<<<<<< HEAD
pc_render_add_request(struct intel_ring_buffer *ring)
{
	struct pipe_control *pc = ring->private;
	u32 scratch_addr = pc->gtt_offset + 128;
=======
pc_render_add_request(struct intel_engine_cs *ring)
{
	u32 scratch_addr = ring->scratch.gtt_offset + 2 * CACHELINE_BYTES;
>>>>>>> v3.18
=======
pc_render_add_request(struct intel_engine_cs *ring)
{
	u32 scratch_addr = ring->scratch.gtt_offset + 2 * CACHELINE_BYTES;
>>>>>>> v3.18
	int ret;

	/* For Ironlake, MI_USER_INTERRUPT was deprecated and apparently
	 * incoherent with writes to memory, i.e. completely fubar,
	 * so we need to use PIPE_NOTIFY instead.
	 *
	 * However, we also need to workaround the qword write
	 * incoherence by flushing the 6 PIPE_NOTIFY buffers out to
	 * memory before requesting an interrupt.
	 */
	ret = intel_ring_begin(ring, 32);
	if (ret)
		return ret;

	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL(4) | PIPE_CONTROL_QW_WRITE |
			PIPE_CONTROL_WRITE_FLUSH |
			PIPE_CONTROL_TEXTURE_CACHE_INVALIDATE);
<<<<<<< HEAD
<<<<<<< HEAD
	intel_ring_emit(ring, pc->gtt_offset | PIPE_CONTROL_GLOBAL_GTT);
	intel_ring_emit(ring, ring->outstanding_lazy_request);
	intel_ring_emit(ring, 0);
	PIPE_CONTROL_FLUSH(ring, scratch_addr);
	scratch_addr += 128; /* write to separate cachelines */
	PIPE_CONTROL_FLUSH(ring, scratch_addr);
	scratch_addr += 128;
	PIPE_CONTROL_FLUSH(ring, scratch_addr);
	scratch_addr += 128;
	PIPE_CONTROL_FLUSH(ring, scratch_addr);
	scratch_addr += 128;
	PIPE_CONTROL_FLUSH(ring, scratch_addr);
	scratch_addr += 128;
=======
=======
>>>>>>> v3.18
	intel_ring_emit(ring, ring->scratch.gtt_offset | PIPE_CONTROL_GLOBAL_GTT);
	intel_ring_emit(ring, ring->outstanding_lazy_seqno);
	intel_ring_emit(ring, 0);
	PIPE_CONTROL_FLUSH(ring, scratch_addr);
	scratch_addr += 2 * CACHELINE_BYTES; /* write to separate cachelines */
	PIPE_CONTROL_FLUSH(ring, scratch_addr);
	scratch_addr += 2 * CACHELINE_BYTES;
	PIPE_CONTROL_FLUSH(ring, scratch_addr);
	scratch_addr += 2 * CACHELINE_BYTES;
	PIPE_CONTROL_FLUSH(ring, scratch_addr);
	scratch_addr += 2 * CACHELINE_BYTES;
	PIPE_CONTROL_FLUSH(ring, scratch_addr);
	scratch_addr += 2 * CACHELINE_BYTES;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	PIPE_CONTROL_FLUSH(ring, scratch_addr);

	intel_ring_emit(ring, GFX_OP_PIPE_CONTROL(4) | PIPE_CONTROL_QW_WRITE |
			PIPE_CONTROL_WRITE_FLUSH |
			PIPE_CONTROL_TEXTURE_CACHE_INVALIDATE |
			PIPE_CONTROL_NOTIFY);
<<<<<<< HEAD
<<<<<<< HEAD
	intel_ring_emit(ring, pc->gtt_offset | PIPE_CONTROL_GLOBAL_GTT);
	intel_ring_emit(ring, ring->outstanding_lazy_request);
	intel_ring_emit(ring, 0);
	intel_ring_advance(ring);
=======
=======
>>>>>>> v3.18
	intel_ring_emit(ring, ring->scratch.gtt_offset | PIPE_CONTROL_GLOBAL_GTT);
	intel_ring_emit(ring, ring->outstanding_lazy_seqno);
	intel_ring_emit(ring, 0);
	__intel_ring_advance(ring);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return 0;
}

static u32
<<<<<<< HEAD
<<<<<<< HEAD
gen6_ring_get_seqno(struct intel_ring_buffer *ring, bool lazy_coherency)
=======
gen6_ring_get_seqno(struct intel_engine_cs *ring, bool lazy_coherency)
>>>>>>> v3.18
=======
gen6_ring_get_seqno(struct intel_engine_cs *ring, bool lazy_coherency)
>>>>>>> v3.18
{
	/* Workaround to force correct ordering between irq and seqno writes on
	 * ivb (and maybe also on snb) by reading from a CS register (like
	 * ACTHD) before reading the status page. */
<<<<<<< HEAD
<<<<<<< HEAD
	if (!lazy_coherency)
		intel_ring_get_active_head(ring);
=======
=======
>>>>>>> v3.18
	if (!lazy_coherency) {
		struct drm_i915_private *dev_priv = ring->dev->dev_private;
		POSTING_READ(RING_ACTHD(ring->mmio_base));
	}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return intel_read_status_page(ring, I915_GEM_HWS_INDEX);
}

static u32
<<<<<<< HEAD
<<<<<<< HEAD
ring_get_seqno(struct intel_ring_buffer *ring, bool lazy_coherency)
=======
ring_get_seqno(struct intel_engine_cs *ring, bool lazy_coherency)
>>>>>>> v3.18
=======
ring_get_seqno(struct intel_engine_cs *ring, bool lazy_coherency)
>>>>>>> v3.18
{
	return intel_read_status_page(ring, I915_GEM_HWS_INDEX);
}

static void
<<<<<<< HEAD
<<<<<<< HEAD
ring_set_seqno(struct intel_ring_buffer *ring, u32 seqno)
=======
ring_set_seqno(struct intel_engine_cs *ring, u32 seqno)
>>>>>>> v3.18
=======
ring_set_seqno(struct intel_engine_cs *ring, u32 seqno)
>>>>>>> v3.18
{
	intel_write_status_page(ring, I915_GEM_HWS_INDEX, seqno);
}

static u32
<<<<<<< HEAD
<<<<<<< HEAD
pc_render_get_seqno(struct intel_ring_buffer *ring, bool lazy_coherency)
{
	struct pipe_control *pc = ring->private;
	return pc->cpu_page[0];
}

static void
pc_render_set_seqno(struct intel_ring_buffer *ring, u32 seqno)
{
	struct pipe_control *pc = ring->private;
	pc->cpu_page[0] = seqno;
}

static bool
gen5_ring_get_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
=======
=======
>>>>>>> v3.18
pc_render_get_seqno(struct intel_engine_cs *ring, bool lazy_coherency)
{
	return ring->scratch.cpu_page[0];
}

static void
pc_render_set_seqno(struct intel_engine_cs *ring, u32 seqno)
{
	ring->scratch.cpu_page[0] = seqno;
}

static bool
gen5_ring_get_irq(struct intel_engine_cs *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	unsigned long flags;

	if (!dev->irq_enabled)
		return false;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
<<<<<<< HEAD
<<<<<<< HEAD
	if (ring->irq_refcount++ == 0) {
		dev_priv->gt_irq_mask &= ~ring->irq_enable_mask;
		I915_WRITE(GTIMR, dev_priv->gt_irq_mask);
		POSTING_READ(GTIMR);
	}
=======
	if (ring->irq_refcount++ == 0)
		gen5_enable_gt_irq(dev_priv, ring->irq_enable_mask);
>>>>>>> v3.18
=======
	if (ring->irq_refcount++ == 0)
		gen5_enable_gt_irq(dev_priv, ring->irq_enable_mask);
>>>>>>> v3.18
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);

	return true;
}

static void
<<<<<<< HEAD
<<<<<<< HEAD
gen5_ring_put_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	unsigned long flags;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (--ring->irq_refcount == 0) {
		dev_priv->gt_irq_mask |= ring->irq_enable_mask;
		I915_WRITE(GTIMR, dev_priv->gt_irq_mask);
		POSTING_READ(GTIMR);
	}
=======
=======
>>>>>>> v3.18
gen5_ring_put_irq(struct intel_engine_cs *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	unsigned long flags;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (--ring->irq_refcount == 0)
		gen5_disable_gt_irq(dev_priv, ring->irq_enable_mask);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);
}

static bool
<<<<<<< HEAD
<<<<<<< HEAD
i9xx_ring_get_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
=======
=======
>>>>>>> v3.18
i9xx_ring_get_irq(struct intel_engine_cs *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	unsigned long flags;

	if (!dev->irq_enabled)
		return false;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (ring->irq_refcount++ == 0) {
		dev_priv->irq_mask &= ~ring->irq_enable_mask;
		I915_WRITE(IMR, dev_priv->irq_mask);
		POSTING_READ(IMR);
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);

	return true;
}

static void
<<<<<<< HEAD
<<<<<<< HEAD
i9xx_ring_put_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
=======
=======
>>>>>>> v3.18
i9xx_ring_put_irq(struct intel_engine_cs *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	unsigned long flags;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (--ring->irq_refcount == 0) {
		dev_priv->irq_mask |= ring->irq_enable_mask;
		I915_WRITE(IMR, dev_priv->irq_mask);
		POSTING_READ(IMR);
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);
}

static bool
<<<<<<< HEAD
<<<<<<< HEAD
i8xx_ring_get_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
=======
=======
>>>>>>> v3.18
i8xx_ring_get_irq(struct intel_engine_cs *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	unsigned long flags;

	if (!dev->irq_enabled)
		return false;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (ring->irq_refcount++ == 0) {
		dev_priv->irq_mask &= ~ring->irq_enable_mask;
		I915_WRITE16(IMR, dev_priv->irq_mask);
		POSTING_READ16(IMR);
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);

	return true;
}

static void
<<<<<<< HEAD
<<<<<<< HEAD
i8xx_ring_put_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
=======
=======
>>>>>>> v3.18
i8xx_ring_put_irq(struct intel_engine_cs *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	unsigned long flags;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (--ring->irq_refcount == 0) {
		dev_priv->irq_mask |= ring->irq_enable_mask;
		I915_WRITE16(IMR, dev_priv->irq_mask);
		POSTING_READ16(IMR);
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);
}

<<<<<<< HEAD
<<<<<<< HEAD
void intel_ring_setup_status_page(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = ring->dev->dev_private;
=======
=======
>>>>>>> v3.18
void intel_ring_setup_status_page(struct intel_engine_cs *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = ring->dev->dev_private;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	u32 mmio = 0;

	/* The ring status page addresses are no longer next to the rest of
	 * the ring registers as of gen7.
	 */
	if (IS_GEN7(dev)) {
		switch (ring->id) {
		case RCS:
			mmio = RENDER_HWS_PGA_GEN7;
			break;
		case BCS:
			mmio = BLT_HWS_PGA_GEN7;
			break;
<<<<<<< HEAD
<<<<<<< HEAD
		case VCS:
			mmio = BSD_HWS_PGA_GEN7;
			break;
=======
=======
>>>>>>> v3.18
		/*
		 * VCS2 actually doesn't exist on Gen7. Only shut up
		 * gcc switch check warning
		 */
		case VCS2:
		case VCS:
			mmio = BSD_HWS_PGA_GEN7;
			break;
		case VECS:
			mmio = VEBOX_HWS_PGA_GEN7;
			break;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		}
	} else if (IS_GEN6(ring->dev)) {
		mmio = RING_HWS_PGA_GEN6(ring->mmio_base);
	} else {
<<<<<<< HEAD
<<<<<<< HEAD
=======
		/* XXX: gen8 returns to sanity */
>>>>>>> v3.18
=======
		/* XXX: gen8 returns to sanity */
>>>>>>> v3.18
		mmio = RING_HWS_PGA(ring->mmio_base);
	}

	I915_WRITE(mmio, (u32)ring->status_page.gfx_addr);
	POSTING_READ(mmio);

<<<<<<< HEAD
<<<<<<< HEAD
	/* Flush the TLB for this page */
	if (INTEL_INFO(dev)->gen >= 6) {
		u32 reg = RING_INSTPM(ring->mmio_base);
=======
=======
>>>>>>> v3.18
	/*
	 * Flush the TLB for this page
	 *
	 * FIXME: These two bits have disappeared on gen8, so a question
	 * arises: do we still need this and if so how should we go about
	 * invalidating the TLB?
	 */
	if (INTEL_INFO(dev)->gen >= 6 && INTEL_INFO(dev)->gen < 8) {
		u32 reg = RING_INSTPM(ring->mmio_base);

		/* ring should be idle before issuing a sync flush*/
		WARN_ON((I915_READ_MODE(ring) & MODE_IDLE) == 0);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		I915_WRITE(reg,
			   _MASKED_BIT_ENABLE(INSTPM_TLB_INVALIDATE |
					      INSTPM_SYNC_FLUSH));
		if (wait_for((I915_READ(reg) & INSTPM_SYNC_FLUSH) == 0,
			     1000))
			DRM_ERROR("%s: wait for SyncFlush to complete for TLB invalidation timed out\n",
				  ring->name);
	}
}

static int
<<<<<<< HEAD
<<<<<<< HEAD
bsd_ring_flush(struct intel_ring_buffer *ring,
=======
bsd_ring_flush(struct intel_engine_cs *ring,
>>>>>>> v3.18
=======
bsd_ring_flush(struct intel_engine_cs *ring,
>>>>>>> v3.18
	       u32     invalidate_domains,
	       u32     flush_domains)
{
	int ret;

	ret = intel_ring_begin(ring, 2);
	if (ret)
		return ret;

	intel_ring_emit(ring, MI_FLUSH);
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_advance(ring);
	return 0;
}

static int
<<<<<<< HEAD
<<<<<<< HEAD
i9xx_add_request(struct intel_ring_buffer *ring)
=======
i9xx_add_request(struct intel_engine_cs *ring)
>>>>>>> v3.18
=======
i9xx_add_request(struct intel_engine_cs *ring)
>>>>>>> v3.18
{
	int ret;

	ret = intel_ring_begin(ring, 4);
	if (ret)
		return ret;

	intel_ring_emit(ring, MI_STORE_DWORD_INDEX);
	intel_ring_emit(ring, I915_GEM_HWS_INDEX << MI_STORE_DWORD_INDEX_SHIFT);
<<<<<<< HEAD
<<<<<<< HEAD
	intel_ring_emit(ring, ring->outstanding_lazy_request);
	intel_ring_emit(ring, MI_USER_INTERRUPT);
	intel_ring_advance(ring);
=======
	intel_ring_emit(ring, ring->outstanding_lazy_seqno);
	intel_ring_emit(ring, MI_USER_INTERRUPT);
	__intel_ring_advance(ring);
>>>>>>> v3.18
=======
	intel_ring_emit(ring, ring->outstanding_lazy_seqno);
	intel_ring_emit(ring, MI_USER_INTERRUPT);
	__intel_ring_advance(ring);
>>>>>>> v3.18

	return 0;
}

static bool
<<<<<<< HEAD
<<<<<<< HEAD
gen6_ring_get_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
=======
=======
>>>>>>> v3.18
gen6_ring_get_irq(struct intel_engine_cs *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	unsigned long flags;

	if (!dev->irq_enabled)
	       return false;

<<<<<<< HEAD
<<<<<<< HEAD
	/* It looks like we need to prevent the gt from suspending while waiting
	 * for an notifiy irq, otherwise irqs seem to get lost on at least the
	 * blt/bsd rings on ivb. */
	gen6_gt_force_wake_get(dev_priv);

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (ring->irq_refcount++ == 0) {
		if (HAS_L3_GPU_CACHE(dev) && ring->id == RCS)
			I915_WRITE_IMR(ring, ~(ring->irq_enable_mask |
						GEN6_RENDER_L3_PARITY_ERROR));
		else
			I915_WRITE_IMR(ring, ~ring->irq_enable_mask);
		dev_priv->gt_irq_mask &= ~ring->irq_enable_mask;
		I915_WRITE(GTIMR, dev_priv->gt_irq_mask);
		POSTING_READ(GTIMR);
=======
=======
>>>>>>> v3.18
	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (ring->irq_refcount++ == 0) {
		if (HAS_L3_DPF(dev) && ring->id == RCS)
			I915_WRITE_IMR(ring,
				       ~(ring->irq_enable_mask |
					 GT_PARITY_ERROR(dev)));
		else
			I915_WRITE_IMR(ring, ~ring->irq_enable_mask);
		gen5_enable_gt_irq(dev_priv, ring->irq_enable_mask);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);

	return true;
}

static void
<<<<<<< HEAD
<<<<<<< HEAD
gen6_ring_put_irq(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
=======
=======
>>>>>>> v3.18
gen6_ring_put_irq(struct intel_engine_cs *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	unsigned long flags;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (--ring->irq_refcount == 0) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (HAS_L3_GPU_CACHE(dev) && ring->id == RCS)
			I915_WRITE_IMR(ring, ~GEN6_RENDER_L3_PARITY_ERROR);
		else
			I915_WRITE_IMR(ring, ~0);
		dev_priv->gt_irq_mask |= ring->irq_enable_mask;
		I915_WRITE(GTIMR, dev_priv->gt_irq_mask);
		POSTING_READ(GTIMR);
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);

	gen6_gt_force_wake_put(dev_priv);
}

static int
i965_dispatch_execbuffer(struct intel_ring_buffer *ring,
			 u32 offset, u32 length,
=======
=======
>>>>>>> v3.18
		if (HAS_L3_DPF(dev) && ring->id == RCS)
			I915_WRITE_IMR(ring, ~GT_PARITY_ERROR(dev));
		else
			I915_WRITE_IMR(ring, ~0);
		gen5_disable_gt_irq(dev_priv, ring->irq_enable_mask);
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);
}

static bool
hsw_vebox_get_irq(struct intel_engine_cs *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	unsigned long flags;

	if (!dev->irq_enabled)
		return false;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (ring->irq_refcount++ == 0) {
		I915_WRITE_IMR(ring, ~ring->irq_enable_mask);
		gen6_enable_pm_irq(dev_priv, ring->irq_enable_mask);
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);

	return true;
}

static void
hsw_vebox_put_irq(struct intel_engine_cs *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	unsigned long flags;

	if (!dev->irq_enabled)
		return;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (--ring->irq_refcount == 0) {
		I915_WRITE_IMR(ring, ~0);
		gen6_disable_pm_irq(dev_priv, ring->irq_enable_mask);
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);
}

static bool
gen8_ring_get_irq(struct intel_engine_cs *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	unsigned long flags;

	if (!dev->irq_enabled)
		return false;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (ring->irq_refcount++ == 0) {
		if (HAS_L3_DPF(dev) && ring->id == RCS) {
			I915_WRITE_IMR(ring,
				       ~(ring->irq_enable_mask |
					 GT_RENDER_L3_PARITY_ERROR_INTERRUPT));
		} else {
			I915_WRITE_IMR(ring, ~ring->irq_enable_mask);
		}
		POSTING_READ(RING_IMR(ring->mmio_base));
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);

	return true;
}

static void
gen8_ring_put_irq(struct intel_engine_cs *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	unsigned long flags;

	spin_lock_irqsave(&dev_priv->irq_lock, flags);
	if (--ring->irq_refcount == 0) {
		if (HAS_L3_DPF(dev) && ring->id == RCS) {
			I915_WRITE_IMR(ring,
				       ~GT_RENDER_L3_PARITY_ERROR_INTERRUPT);
		} else {
			I915_WRITE_IMR(ring, ~0);
		}
		POSTING_READ(RING_IMR(ring->mmio_base));
	}
	spin_unlock_irqrestore(&dev_priv->irq_lock, flags);
}

static int
i965_dispatch_execbuffer(struct intel_engine_cs *ring,
			 u64 offset, u32 length,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			 unsigned flags)
{
	int ret;

	ret = intel_ring_begin(ring, 2);
	if (ret)
		return ret;

	intel_ring_emit(ring,
			MI_BATCH_BUFFER_START |
			MI_BATCH_GTT |
			(flags & I915_DISPATCH_SECURE ? 0 : MI_BATCH_NON_SECURE_I965));
	intel_ring_emit(ring, offset);
	intel_ring_advance(ring);

	return 0;
}

/* Just userspace ABI convention to limit the wa batch bo to a resonable size */
#define I830_BATCH_LIMIT (256*1024)
<<<<<<< HEAD
<<<<<<< HEAD
static int
i830_dispatch_execbuffer(struct intel_ring_buffer *ring,
				u32 offset, u32 len,
				unsigned flags)
{
	int ret;

	if (flags & I915_DISPATCH_PINNED) {
		ret = intel_ring_begin(ring, 4);
		if (ret)
			return ret;

		intel_ring_emit(ring, MI_BATCH_BUFFER);
		intel_ring_emit(ring, offset | (flags & I915_DISPATCH_SECURE ? 0 : MI_BATCH_NON_SECURE));
		intel_ring_emit(ring, offset + len - 8);
		intel_ring_emit(ring, MI_NOOP);
		intel_ring_advance(ring);
	} else {
		struct drm_i915_gem_object *obj = ring->private;
		u32 cs_offset = obj->gtt_offset;

		if (len > I830_BATCH_LIMIT)
			return -ENOSPC;

		ret = intel_ring_begin(ring, 9+3);
		if (ret)
			return ret;
		/* Blit the batch (which has now all relocs applied) to the stable batch
		 * scratch bo area (so that the CS never stumbles over its tlb
		 * invalidation bug) ... */
		intel_ring_emit(ring, XY_SRC_COPY_BLT_CMD |
				XY_SRC_COPY_BLT_WRITE_ALPHA |
				XY_SRC_COPY_BLT_WRITE_RGB);
		intel_ring_emit(ring, BLT_DEPTH_32 | BLT_ROP_GXCOPY | 4096);
		intel_ring_emit(ring, 0);
		intel_ring_emit(ring, (DIV_ROUND_UP(len, 4096) << 16) | 1024);
		intel_ring_emit(ring, cs_offset);
		intel_ring_emit(ring, 0);
		intel_ring_emit(ring, 4096);
		intel_ring_emit(ring, offset);
		intel_ring_emit(ring, MI_FLUSH);

		/* ... and execute it. */
		intel_ring_emit(ring, MI_BATCH_BUFFER);
		intel_ring_emit(ring, cs_offset | (flags & I915_DISPATCH_SECURE ? 0 : MI_BATCH_NON_SECURE));
		intel_ring_emit(ring, cs_offset + len - 8);
		intel_ring_advance(ring);
	}

=======
=======
>>>>>>> v3.18
#define I830_TLB_ENTRIES (2)
#define I830_WA_SIZE max(I830_TLB_ENTRIES*4096, I830_BATCH_LIMIT)
static int
i830_dispatch_execbuffer(struct intel_engine_cs *ring,
				u64 offset, u32 len,
				unsigned flags)
{
	u32 cs_offset = ring->scratch.gtt_offset;
	int ret;

	ret = intel_ring_begin(ring, 6);
	if (ret)
		return ret;

	/* Evict the invalid PTE TLBs */
	intel_ring_emit(ring, COLOR_BLT_CMD | BLT_WRITE_RGBA);
	intel_ring_emit(ring, BLT_DEPTH_32 | BLT_ROP_COLOR_COPY | 4096);
	intel_ring_emit(ring, I830_TLB_ENTRIES << 16 | 4); /* load each page */
	intel_ring_emit(ring, cs_offset);
	intel_ring_emit(ring, 0xdeadbeef);
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_advance(ring);

	if ((flags & I915_DISPATCH_PINNED) == 0) {
		if (len > I830_BATCH_LIMIT)
			return -ENOSPC;

		ret = intel_ring_begin(ring, 6 + 2);
		if (ret)
			return ret;

		/* Blit the batch (which has now all relocs applied) to the
		 * stable batch scratch bo area (so that the CS never
		 * stumbles over its tlb invalidation bug) ...
		 */
		intel_ring_emit(ring, SRC_COPY_BLT_CMD | BLT_WRITE_RGBA);
		intel_ring_emit(ring, BLT_DEPTH_32 | BLT_ROP_SRC_COPY | 4096);
		intel_ring_emit(ring, DIV_ROUND_UP(len, 4096) << 16 | 4096);
		intel_ring_emit(ring, cs_offset);
		intel_ring_emit(ring, 4096);
		intel_ring_emit(ring, offset);

		intel_ring_emit(ring, MI_FLUSH);
		intel_ring_emit(ring, MI_NOOP);
		intel_ring_advance(ring);

		/* ... and execute it. */
		offset = cs_offset;
	}

	ret = intel_ring_begin(ring, 4);
	if (ret)
		return ret;

	intel_ring_emit(ring, MI_BATCH_BUFFER);
	intel_ring_emit(ring, offset | (flags & I915_DISPATCH_SECURE ? 0 : MI_BATCH_NON_SECURE));
	intel_ring_emit(ring, offset + len - 8);
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_advance(ring);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
}

static int
<<<<<<< HEAD
<<<<<<< HEAD
i915_dispatch_execbuffer(struct intel_ring_buffer *ring,
			 u32 offset, u32 len,
=======
i915_dispatch_execbuffer(struct intel_engine_cs *ring,
			 u64 offset, u32 len,
>>>>>>> v3.18
=======
i915_dispatch_execbuffer(struct intel_engine_cs *ring,
			 u64 offset, u32 len,
>>>>>>> v3.18
			 unsigned flags)
{
	int ret;

	ret = intel_ring_begin(ring, 2);
	if (ret)
		return ret;

	intel_ring_emit(ring, MI_BATCH_BUFFER_START | MI_BATCH_GTT);
	intel_ring_emit(ring, offset | (flags & I915_DISPATCH_SECURE ? 0 : MI_BATCH_NON_SECURE));
	intel_ring_advance(ring);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void cleanup_status_page(struct intel_ring_buffer *ring)
=======
static void cleanup_status_page(struct intel_engine_cs *ring)
>>>>>>> v3.18
=======
static void cleanup_status_page(struct intel_engine_cs *ring)
>>>>>>> v3.18
{
	struct drm_i915_gem_object *obj;

	obj = ring->status_page.obj;
	if (obj == NULL)
		return;

	kunmap(sg_page(obj->pages->sgl));
<<<<<<< HEAD
<<<<<<< HEAD
	i915_gem_object_unpin(obj);
=======
	i915_gem_object_ggtt_unpin(obj);
>>>>>>> v3.18
=======
	i915_gem_object_ggtt_unpin(obj);
>>>>>>> v3.18
	drm_gem_object_unreference(&obj->base);
	ring->status_page.obj = NULL;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int init_status_page(struct intel_ring_buffer *ring)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_gem_object *obj;
	int ret;

	obj = i915_gem_alloc_object(dev, 4096);
	if (obj == NULL) {
		DRM_ERROR("Failed to allocate status page\n");
		ret = -ENOMEM;
		goto err;
	}

	i915_gem_object_set_cache_level(obj, I915_CACHE_LLC);

	ret = i915_gem_object_pin(obj, 4096, true, false);
	if (ret != 0) {
		goto err_unref;
	}

	ring->status_page.gfx_addr = obj->gtt_offset;
	ring->status_page.page_addr = kmap(sg_page(obj->pages->sgl));
	if (ring->status_page.page_addr == NULL) {
		ret = -ENOMEM;
		goto err_unpin;
	}
	ring->status_page.obj = obj;
	memset(ring->status_page.page_addr, 0, PAGE_SIZE);

	intel_ring_setup_status_page(ring);
	DRM_DEBUG_DRIVER("%s hws offset: 0x%08x\n",
			ring->name, ring->status_page.gfx_addr);

	return 0;

err_unpin:
	i915_gem_object_unpin(obj);
err_unref:
	drm_gem_object_unreference(&obj->base);
err:
	return ret;
}

static int init_phys_hws_pga(struct intel_ring_buffer *ring)
{
	struct drm_i915_private *dev_priv = ring->dev->dev_private;
	u32 addr;

	if (!dev_priv->status_page_dmah) {
		dev_priv->status_page_dmah =
			drm_pci_alloc(ring->dev, PAGE_SIZE, PAGE_SIZE);
		if (!dev_priv->status_page_dmah)
			return -ENOMEM;
	}

	addr = dev_priv->status_page_dmah->busaddr;
	if (INTEL_INFO(ring->dev)->gen >= 4)
		addr |= (dev_priv->status_page_dmah->busaddr >> 28) & 0xf0;
	I915_WRITE(HWS_PGA, addr);

	ring->status_page.page_addr = dev_priv->status_page_dmah->vaddr;
	memset(ring->status_page.page_addr, 0, PAGE_SIZE);

	return 0;
}

static int intel_init_ring_buffer(struct drm_device *dev,
				  struct intel_ring_buffer *ring)
{
	struct drm_i915_gem_object *obj;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int ret;

	ring->dev = dev;
	INIT_LIST_HEAD(&ring->active_list);
	INIT_LIST_HEAD(&ring->request_list);
	ring->size = 32 * PAGE_SIZE;
	memset(ring->sync_seqno, 0, sizeof(ring->sync_seqno));
=======
=======
>>>>>>> v3.18
static int init_status_page(struct intel_engine_cs *ring)
{
	struct drm_i915_gem_object *obj;

	if ((obj = ring->status_page.obj) == NULL) {
		unsigned flags;
		int ret;

		obj = i915_gem_alloc_object(ring->dev, 4096);
		if (obj == NULL) {
			DRM_ERROR("Failed to allocate status page\n");
			return -ENOMEM;
		}

		ret = i915_gem_object_set_cache_level(obj, I915_CACHE_LLC);
		if (ret)
			goto err_unref;

		flags = 0;
		if (!HAS_LLC(ring->dev))
			/* On g33, we cannot place HWS above 256MiB, so
			 * restrict its pinning to the low mappable arena.
			 * Though this restriction is not documented for
			 * gen4, gen5, or byt, they also behave similarly
			 * and hang if the HWS is placed at the top of the
			 * GTT. To generalise, it appears that all !llc
			 * platforms have issues with us placing the HWS
			 * above the mappable region (even though we never
			 * actualy map it).
			 */
			flags |= PIN_MAPPABLE;
		ret = i915_gem_obj_ggtt_pin(obj, 4096, flags);
		if (ret) {
err_unref:
			drm_gem_object_unreference(&obj->base);
			return ret;
		}

		ring->status_page.obj = obj;
	}

	ring->status_page.gfx_addr = i915_gem_obj_ggtt_offset(obj);
	ring->status_page.page_addr = kmap(sg_page(obj->pages->sgl));
	memset(ring->status_page.page_addr, 0, PAGE_SIZE);

	DRM_DEBUG_DRIVER("%s hws offset: 0x%08x\n",
			ring->name, ring->status_page.gfx_addr);

	return 0;
}

static int init_phys_status_page(struct intel_engine_cs *ring)
{
	struct drm_i915_private *dev_priv = ring->dev->dev_private;

	if (!dev_priv->status_page_dmah) {
		dev_priv->status_page_dmah =
			drm_pci_alloc(ring->dev, PAGE_SIZE, PAGE_SIZE);
		if (!dev_priv->status_page_dmah)
			return -ENOMEM;
	}

	ring->status_page.page_addr = dev_priv->status_page_dmah->vaddr;
	memset(ring->status_page.page_addr, 0, PAGE_SIZE);

	return 0;
}

void intel_destroy_ringbuffer_obj(struct intel_ringbuffer *ringbuf)
{
	if (!ringbuf->obj)
		return;

	iounmap(ringbuf->virtual_start);
	i915_gem_object_ggtt_unpin(ringbuf->obj);
	drm_gem_object_unreference(&ringbuf->obj->base);
	ringbuf->obj = NULL;
}

int intel_alloc_ringbuffer_obj(struct drm_device *dev,
			       struct intel_ringbuffer *ringbuf)
{
	struct drm_i915_private *dev_priv = to_i915(dev);
	struct drm_i915_gem_object *obj;
	int ret;

	if (ringbuf->obj)
		return 0;

	obj = NULL;
	if (!HAS_LLC(dev))
		obj = i915_gem_object_create_stolen(dev, ringbuf->size);
	if (obj == NULL)
		obj = i915_gem_alloc_object(dev, ringbuf->size);
	if (obj == NULL)
		return -ENOMEM;

	/* mark ring buffers as read-only from GPU side by default */
	obj->gt_ro = 1;

	ret = i915_gem_obj_ggtt_pin(obj, PAGE_SIZE, PIN_MAPPABLE);
	if (ret)
		goto err_unref;

	ret = i915_gem_object_set_to_gtt_domain(obj, true);
	if (ret)
		goto err_unpin;

	ringbuf->virtual_start =
		ioremap_wc(dev_priv->gtt.mappable_base + i915_gem_obj_ggtt_offset(obj),
				ringbuf->size);
	if (ringbuf->virtual_start == NULL) {
		ret = -EINVAL;
		goto err_unpin;
	}

	ringbuf->obj = obj;
	return 0;

err_unpin:
	i915_gem_object_ggtt_unpin(obj);
err_unref:
	drm_gem_object_unreference(&obj->base);
	return ret;
}

static int intel_init_ring_buffer(struct drm_device *dev,
				  struct intel_engine_cs *ring)
{
	struct intel_ringbuffer *ringbuf = ring->buffer;
	int ret;

	if (ringbuf == NULL) {
		ringbuf = kzalloc(sizeof(*ringbuf), GFP_KERNEL);
		if (!ringbuf)
			return -ENOMEM;
		ring->buffer = ringbuf;
	}

	ring->dev = dev;
	INIT_LIST_HEAD(&ring->active_list);
	INIT_LIST_HEAD(&ring->request_list);
	INIT_LIST_HEAD(&ring->execlist_queue);
	ringbuf->size = 32 * PAGE_SIZE;
	ringbuf->ring = ring;
	memset(ring->semaphore.sync_seqno, 0, sizeof(ring->semaphore.sync_seqno));
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	init_waitqueue_head(&ring->irq_queue);

	if (I915_NEED_GFX_HWS(dev)) {
		ret = init_status_page(ring);
		if (ret)
<<<<<<< HEAD
<<<<<<< HEAD
			return ret;
	} else {
		BUG_ON(ring->id != RCS);
		ret = init_phys_hws_pga(ring);
		if (ret)
			return ret;
	}

	obj = NULL;
	if (!HAS_LLC(dev))
		obj = i915_gem_object_create_stolen(dev, ring->size);
	if (obj == NULL)
		obj = i915_gem_alloc_object(dev, ring->size);
	if (obj == NULL) {
		DRM_ERROR("Failed to allocate ringbuffer\n");
		ret = -ENOMEM;
		goto err_hws;
	}

	ring->obj = obj;

	ret = i915_gem_object_pin(obj, PAGE_SIZE, true, false);
	if (ret)
		goto err_unref;

	ret = i915_gem_object_set_to_gtt_domain(obj, true);
	if (ret)
		goto err_unpin;

	ring->virtual_start =
		ioremap_wc(dev_priv->gtt.mappable_base + obj->gtt_offset,
			   ring->size);
	if (ring->virtual_start == NULL) {
		DRM_ERROR("Failed to map ringbuffer.\n");
		ret = -EINVAL;
		goto err_unpin;
	}

	ret = ring->init(ring);
	if (ret)
		goto err_unmap;

	/* Workaround an erratum on the i830 which causes a hang if
	 * the TAIL pointer points to within the last 2 cachelines
	 * of the buffer.
	 */
	ring->effective_size = ring->size;
	if (IS_I830(ring->dev) || IS_845G(ring->dev))
		ring->effective_size -= 128;

	return 0;

err_unmap:
	iounmap(ring->virtual_start);
err_unpin:
	i915_gem_object_unpin(obj);
err_unref:
	drm_gem_object_unreference(&obj->base);
	ring->obj = NULL;
err_hws:
	cleanup_status_page(ring);
	return ret;
}

void intel_cleanup_ring_buffer(struct intel_ring_buffer *ring)
{
	struct drm_i915_private *dev_priv;
	int ret;

	if (ring->obj == NULL)
		return;

	/* Disable the ring buffer. The ring must be idle at this point */
	dev_priv = ring->dev->dev_private;
	ret = intel_ring_idle(ring);
	if (ret)
		DRM_ERROR("failed to quiesce %s whilst cleaning up: %d\n",
			  ring->name, ret);

	I915_WRITE_CTL(ring, 0);

	iounmap(ring->virtual_start);

	i915_gem_object_unpin(ring->obj);
	drm_gem_object_unreference(&ring->obj->base);
	ring->obj = NULL;
=======
=======
>>>>>>> v3.18
			goto error;
	} else {
		BUG_ON(ring->id != RCS);
		ret = init_phys_status_page(ring);
		if (ret)
			goto error;
	}

	ret = intel_alloc_ringbuffer_obj(dev, ringbuf);
	if (ret) {
		DRM_ERROR("Failed to allocate ringbuffer %s: %d\n", ring->name, ret);
		goto error;
	}

	/* Workaround an erratum on the i830 which causes a hang if
	 * the TAIL pointer points to within the last 2 cachelines
	 * of the buffer.
	 */
	ringbuf->effective_size = ringbuf->size;
	if (IS_I830(dev) || IS_845G(dev))
		ringbuf->effective_size -= 2 * CACHELINE_BYTES;

	ret = i915_cmd_parser_init_ring(ring);
	if (ret)
		goto error;

	ret = ring->init(ring);
	if (ret)
		goto error;

	return 0;

error:
	kfree(ringbuf);
	ring->buffer = NULL;
	return ret;
}

void intel_cleanup_ring_buffer(struct intel_engine_cs *ring)
{
	struct drm_i915_private *dev_priv = to_i915(ring->dev);
	struct intel_ringbuffer *ringbuf = ring->buffer;

	if (!intel_ring_initialized(ring))
		return;

	intel_stop_ring_buffer(ring);
	WARN_ON(!IS_GEN2(ring->dev) && (I915_READ_MODE(ring) & MODE_IDLE) == 0);

	intel_destroy_ringbuffer_obj(ringbuf);
	ring->preallocated_lazy_request = NULL;
	ring->outstanding_lazy_seqno = 0;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (ring->cleanup)
		ring->cleanup(ring);

	cleanup_status_page(ring);
<<<<<<< HEAD
<<<<<<< HEAD
}

static int intel_ring_wait_seqno(struct intel_ring_buffer *ring, u32 seqno)
{
	int ret;

	ret = i915_wait_seqno(ring, seqno);
	if (!ret)
		i915_gem_retire_requests_ring(ring);

	return ret;
}

static int intel_ring_wait_request(struct intel_ring_buffer *ring, int n)
{
=======
=======
>>>>>>> v3.18

	i915_cmd_parser_fini_ring(ring);

	kfree(ringbuf);
	ring->buffer = NULL;
}

static int intel_ring_wait_request(struct intel_engine_cs *ring, int n)
{
	struct intel_ringbuffer *ringbuf = ring->buffer;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	struct drm_i915_gem_request *request;
	u32 seqno = 0;
	int ret;

<<<<<<< HEAD
<<<<<<< HEAD
	i915_gem_retire_requests_ring(ring);

	if (ring->last_retired_head != -1) {
		ring->head = ring->last_retired_head;
		ring->last_retired_head = -1;
		ring->space = ring_space(ring);
		if (ring->space >= n)
=======
=======
>>>>>>> v3.18
	if (ringbuf->last_retired_head != -1) {
		ringbuf->head = ringbuf->last_retired_head;
		ringbuf->last_retired_head = -1;

		ringbuf->space = intel_ring_space(ringbuf);
		if (ringbuf->space >= n)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			return 0;
	}

	list_for_each_entry(request, &ring->request_list, list) {
<<<<<<< HEAD
<<<<<<< HEAD
		int space;

		if (request->tail == -1)
			continue;

		space = request->tail - (ring->tail + I915_RING_FREE_SPACE);
		if (space < 0)
			space += ring->size;
		if (space >= n) {
			seqno = request->seqno;
			break;
		}

		/* Consume this request in case we need more space than
		 * is available and so need to prevent a race between
		 * updating last_retired_head and direct reads of
		 * I915_RING_HEAD. It also provides a nice sanity check.
		 */
		request->tail = -1;
=======
=======
>>>>>>> v3.18
		if (__intel_ring_space(request->tail, ringbuf->tail,
				       ringbuf->size) >= n) {
			seqno = request->seqno;
			break;
		}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}

	if (seqno == 0)
		return -ENOSPC;

<<<<<<< HEAD
<<<<<<< HEAD
	ret = intel_ring_wait_seqno(ring, seqno);
	if (ret)
		return ret;

	if (WARN_ON(ring->last_retired_head == -1))
		return -ENOSPC;

	ring->head = ring->last_retired_head;
	ring->last_retired_head = -1;
	ring->space = ring_space(ring);
	if (WARN_ON(ring->space < n))
		return -ENOSPC;

	return 0;
}

static int ring_wait_for_space(struct intel_ring_buffer *ring, int n)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
=======
=======
>>>>>>> v3.18
	ret = i915_wait_seqno(ring, seqno);
	if (ret)
		return ret;

	i915_gem_retire_requests_ring(ring);
	ringbuf->head = ringbuf->last_retired_head;
	ringbuf->last_retired_head = -1;

	ringbuf->space = intel_ring_space(ringbuf);
	return 0;
}

static int ring_wait_for_space(struct intel_engine_cs *ring, int n)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_ringbuffer *ringbuf = ring->buffer;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	unsigned long end;
	int ret;

	ret = intel_ring_wait_request(ring, n);
	if (ret != -ENOSPC)
		return ret;

<<<<<<< HEAD
<<<<<<< HEAD
	trace_i915_ring_wait_begin(ring);
=======
	/* force the tail write in case we have been skipping them */
	__intel_ring_advance(ring);

>>>>>>> v3.18
=======
	/* force the tail write in case we have been skipping them */
	__intel_ring_advance(ring);

>>>>>>> v3.18
	/* With GEM the hangcheck timer should kick us out of the loop,
	 * leaving it early runs the risk of corrupting GEM state (due
	 * to running on almost untested codepaths). But on resume
	 * timers don't work yet, so prevent a complete hang in that
	 * case by choosing an insanely large timeout. */
	end = jiffies + 60 * HZ;

<<<<<<< HEAD
<<<<<<< HEAD
	do {
		ring->head = I915_READ_HEAD(ring);
		ring->space = ring_space(ring);
		if (ring->space >= n) {
			trace_i915_ring_wait_end(ring);
			return 0;
		}

		if (dev->primary->master) {
=======
=======
>>>>>>> v3.18
	trace_i915_ring_wait_begin(ring);
	do {
		ringbuf->head = I915_READ_HEAD(ring);
		ringbuf->space = intel_ring_space(ringbuf);
		if (ringbuf->space >= n) {
			ret = 0;
			break;
		}

		if (!drm_core_check_feature(dev, DRIVER_MODESET) &&
		    dev->primary->master) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			struct drm_i915_master_private *master_priv = dev->primary->master->driver_priv;
			if (master_priv->sarea_priv)
				master_priv->sarea_priv->perf_boxes |= I915_BOX_WAIT;
		}

		msleep(1);

<<<<<<< HEAD
<<<<<<< HEAD
		ret = i915_gem_check_wedge(&dev_priv->gpu_error,
					   dev_priv->mm.interruptible);
		if (ret)
			return ret;
	} while (!time_after(jiffies, end));
	trace_i915_ring_wait_end(ring);
	return -EBUSY;
}

static int intel_wrap_ring_buffer(struct intel_ring_buffer *ring)
{
	uint32_t __iomem *virt;
	int rem = ring->size - ring->tail;

	if (ring->space < rem) {
=======
=======
>>>>>>> v3.18
		if (dev_priv->mm.interruptible && signal_pending(current)) {
			ret = -ERESTARTSYS;
			break;
		}

		ret = i915_gem_check_wedge(&dev_priv->gpu_error,
					   dev_priv->mm.interruptible);
		if (ret)
			break;

		if (time_after(jiffies, end)) {
			ret = -EBUSY;
			break;
		}
	} while (1);
	trace_i915_ring_wait_end(ring);
	return ret;
}

static int intel_wrap_ring_buffer(struct intel_engine_cs *ring)
{
	uint32_t __iomem *virt;
	struct intel_ringbuffer *ringbuf = ring->buffer;
	int rem = ringbuf->size - ringbuf->tail;

	if (ringbuf->space < rem) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		int ret = ring_wait_for_space(ring, rem);
		if (ret)
			return ret;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	virt = ring->virtual_start + ring->tail;
=======
	virt = ringbuf->virtual_start + ringbuf->tail;
>>>>>>> v3.18
=======
	virt = ringbuf->virtual_start + ringbuf->tail;
>>>>>>> v3.18
	rem /= 4;
	while (rem--)
		iowrite32(MI_NOOP, virt++);

<<<<<<< HEAD
<<<<<<< HEAD
	ring->tail = 0;
	ring->space = ring_space(ring);
=======
	ringbuf->tail = 0;
	ringbuf->space = intel_ring_space(ringbuf);
>>>>>>> v3.18
=======
	ringbuf->tail = 0;
	ringbuf->space = intel_ring_space(ringbuf);
>>>>>>> v3.18

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
int intel_ring_idle(struct intel_ring_buffer *ring)
=======
int intel_ring_idle(struct intel_engine_cs *ring)
>>>>>>> v3.18
=======
int intel_ring_idle(struct intel_engine_cs *ring)
>>>>>>> v3.18
{
	u32 seqno;
	int ret;

	/* We need to add any requests required to flush the objects and ring */
<<<<<<< HEAD
<<<<<<< HEAD
	if (ring->outstanding_lazy_request) {
		ret = i915_add_request(ring, NULL, NULL);
=======
	if (ring->outstanding_lazy_seqno) {
		ret = i915_add_request(ring, NULL);
>>>>>>> v3.18
=======
	if (ring->outstanding_lazy_seqno) {
		ret = i915_add_request(ring, NULL);
>>>>>>> v3.18
		if (ret)
			return ret;
	}

	/* Wait upon the last request to be completed */
	if (list_empty(&ring->request_list))
		return 0;

	seqno = list_entry(ring->request_list.prev,
			   struct drm_i915_gem_request,
			   list)->seqno;

	return i915_wait_seqno(ring, seqno);
}

static int
<<<<<<< HEAD
<<<<<<< HEAD
intel_ring_alloc_seqno(struct intel_ring_buffer *ring)
{
	if (ring->outstanding_lazy_request)
		return 0;

	return i915_gem_get_seqno(ring->dev, &ring->outstanding_lazy_request);
}

static int __intel_ring_prepare(struct intel_ring_buffer *ring,
				int bytes)
{
	int ret;

	if (unlikely(ring->tail + bytes > ring->effective_size)) {
=======
=======
>>>>>>> v3.18
intel_ring_alloc_seqno(struct intel_engine_cs *ring)
{
	if (ring->outstanding_lazy_seqno)
		return 0;

	if (ring->preallocated_lazy_request == NULL) {
		struct drm_i915_gem_request *request;

		request = kmalloc(sizeof(*request), GFP_KERNEL);
		if (request == NULL)
			return -ENOMEM;

		ring->preallocated_lazy_request = request;
	}

	return i915_gem_get_seqno(ring->dev, &ring->outstanding_lazy_seqno);
}

static int __intel_ring_prepare(struct intel_engine_cs *ring,
				int bytes)
{
	struct intel_ringbuffer *ringbuf = ring->buffer;
	int ret;

	if (unlikely(ringbuf->tail + bytes > ringbuf->effective_size)) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		ret = intel_wrap_ring_buffer(ring);
		if (unlikely(ret))
			return ret;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (unlikely(ring->space < bytes)) {
=======
	if (unlikely(ringbuf->space < bytes)) {
>>>>>>> v3.18
=======
	if (unlikely(ringbuf->space < bytes)) {
>>>>>>> v3.18
		ret = ring_wait_for_space(ring, bytes);
		if (unlikely(ret))
			return ret;
	}

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
int intel_ring_begin(struct intel_ring_buffer *ring,
		     int num_dwords)
{
	drm_i915_private_t *dev_priv = ring->dev->dev_private;
=======
=======
>>>>>>> v3.18
int intel_ring_begin(struct intel_engine_cs *ring,
		     int num_dwords)
{
	struct drm_i915_private *dev_priv = ring->dev->dev_private;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	int ret;

	ret = i915_gem_check_wedge(&dev_priv->gpu_error,
				   dev_priv->mm.interruptible);
	if (ret)
		return ret;

	ret = __intel_ring_prepare(ring, num_dwords * sizeof(uint32_t));
	if (ret)
		return ret;

	/* Preallocate the olr before touching the ring */
	ret = intel_ring_alloc_seqno(ring);
	if (ret)
		return ret;

<<<<<<< HEAD
<<<<<<< HEAD
	ring->space -= num_dwords * sizeof(uint32_t);
	return 0;
}

void intel_ring_init_seqno(struct intel_ring_buffer *ring, u32 seqno)
{
	struct drm_i915_private *dev_priv = ring->dev->dev_private;

	BUG_ON(ring->outstanding_lazy_request);

	if (INTEL_INFO(ring->dev)->gen >= 6) {
		I915_WRITE(RING_SYNC_0(ring->mmio_base), 0);
		I915_WRITE(RING_SYNC_1(ring->mmio_base), 0);
	}

	ring->set_seqno(ring, seqno);
}

void intel_ring_advance(struct intel_ring_buffer *ring)
{
	struct drm_i915_private *dev_priv = ring->dev->dev_private;

	ring->tail &= ring->size - 1;
	if (dev_priv->gpu_error.stop_rings & intel_ring_flag(ring))
		return;
	ring->write_tail(ring, ring->tail);
}


static void gen6_bsd_ring_write_tail(struct intel_ring_buffer *ring,
				     u32 value)
{
	drm_i915_private_t *dev_priv = ring->dev->dev_private;
=======
=======
>>>>>>> v3.18
	ring->buffer->space -= num_dwords * sizeof(uint32_t);
	return 0;
}

/* Align the ring tail to a cacheline boundary */
int intel_ring_cacheline_align(struct intel_engine_cs *ring)
{
	int num_dwords = (ring->buffer->tail & (CACHELINE_BYTES - 1)) / sizeof(uint32_t);
	int ret;

	if (num_dwords == 0)
		return 0;

	num_dwords = CACHELINE_BYTES / sizeof(uint32_t) - num_dwords;
	ret = intel_ring_begin(ring, num_dwords);
	if (ret)
		return ret;

	while (num_dwords--)
		intel_ring_emit(ring, MI_NOOP);

	intel_ring_advance(ring);

	return 0;
}

void intel_ring_init_seqno(struct intel_engine_cs *ring, u32 seqno)
{
	struct drm_device *dev = ring->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;

	BUG_ON(ring->outstanding_lazy_seqno);

	if (INTEL_INFO(dev)->gen == 6 || INTEL_INFO(dev)->gen == 7) {
		I915_WRITE(RING_SYNC_0(ring->mmio_base), 0);
		I915_WRITE(RING_SYNC_1(ring->mmio_base), 0);
		if (HAS_VEBOX(dev))
			I915_WRITE(RING_SYNC_2(ring->mmio_base), 0);
	}

	ring->set_seqno(ring, seqno);
	ring->hangcheck.seqno = seqno;
}

static void gen6_bsd_ring_write_tail(struct intel_engine_cs *ring,
				     u32 value)
{
	struct drm_i915_private *dev_priv = ring->dev->dev_private;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

       /* Every tail move must follow the sequence below */

	/* Disable notification that the ring is IDLE. The GT
	 * will then assume that it is busy and bring it out of rc6.
	 */
	I915_WRITE(GEN6_BSD_SLEEP_PSMI_CONTROL,
		   _MASKED_BIT_ENABLE(GEN6_BSD_SLEEP_MSG_DISABLE));

	/* Clear the context id. Here be magic! */
	I915_WRITE64(GEN6_BSD_RNCID, 0x0);

	/* Wait for the ring not to be idle, i.e. for it to wake up. */
	if (wait_for((I915_READ(GEN6_BSD_SLEEP_PSMI_CONTROL) &
		      GEN6_BSD_SLEEP_INDICATOR) == 0,
		     50))
		DRM_ERROR("timed out waiting for the BSD ring to wake up\n");

	/* Now that the ring is fully powered up, update the tail */
	I915_WRITE_TAIL(ring, value);
	POSTING_READ(RING_TAIL(ring->mmio_base));

	/* Let the ring send IDLE messages to the GT again,
	 * and so let it sleep to conserve power when idle.
	 */
	I915_WRITE(GEN6_BSD_SLEEP_PSMI_CONTROL,
		   _MASKED_BIT_DISABLE(GEN6_BSD_SLEEP_MSG_DISABLE));
}

<<<<<<< HEAD
<<<<<<< HEAD
static int gen6_ring_flush(struct intel_ring_buffer *ring,
			   u32 invalidate, u32 flush)
=======
static int gen6_bsd_ring_flush(struct intel_engine_cs *ring,
			       u32 invalidate, u32 flush)
>>>>>>> v3.18
=======
static int gen6_bsd_ring_flush(struct intel_engine_cs *ring,
			       u32 invalidate, u32 flush)
>>>>>>> v3.18
{
	uint32_t cmd;
	int ret;

	ret = intel_ring_begin(ring, 4);
	if (ret)
		return ret;

	cmd = MI_FLUSH_DW;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (INTEL_INFO(ring->dev)->gen >= 8)
		cmd += 1;
>>>>>>> v3.18
=======
	if (INTEL_INFO(ring->dev)->gen >= 8)
		cmd += 1;
>>>>>>> v3.18
	/*
	 * Bspec vol 1c.5 - video engine command streamer:
	 * "If ENABLED, all TLBs will be invalidated once the flush
	 * operation is complete. This bit is only valid when the
	 * Post-Sync Operation field is a value of 1h or 3h."
	 */
	if (invalidate & I915_GEM_GPU_DOMAINS)
		cmd |= MI_INVALIDATE_TLB | MI_INVALIDATE_BSD |
			MI_FLUSH_DW_STORE_INDEX | MI_FLUSH_DW_OP_STOREDW;
	intel_ring_emit(ring, cmd);
	intel_ring_emit(ring, I915_GEM_HWS_SCRATCH_ADDR | MI_FLUSH_DW_USE_GTT);
<<<<<<< HEAD
<<<<<<< HEAD
	intel_ring_emit(ring, 0);
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_advance(ring);
=======
=======
>>>>>>> v3.18
	if (INTEL_INFO(ring->dev)->gen >= 8) {
		intel_ring_emit(ring, 0); /* upper addr */
		intel_ring_emit(ring, 0); /* value */
	} else  {
		intel_ring_emit(ring, 0);
		intel_ring_emit(ring, MI_NOOP);
	}
	intel_ring_advance(ring);
	return 0;
}

static int
gen8_ring_dispatch_execbuffer(struct intel_engine_cs *ring,
			      u64 offset, u32 len,
			      unsigned flags)
{
	bool ppgtt = USES_PPGTT(ring->dev) && !(flags & I915_DISPATCH_SECURE);
	int ret;

	ret = intel_ring_begin(ring, 4);
	if (ret)
		return ret;

	/* FIXME(BDW): Address space and security selectors. */
	intel_ring_emit(ring, MI_BATCH_BUFFER_START_GEN8 | (ppgtt<<8));
	intel_ring_emit(ring, lower_32_bits(offset));
	intel_ring_emit(ring, upper_32_bits(offset));
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_advance(ring);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
}

static int
<<<<<<< HEAD
<<<<<<< HEAD
hsw_ring_dispatch_execbuffer(struct intel_ring_buffer *ring,
			      u32 offset, u32 len,
=======
hsw_ring_dispatch_execbuffer(struct intel_engine_cs *ring,
			      u64 offset, u32 len,
>>>>>>> v3.18
=======
hsw_ring_dispatch_execbuffer(struct intel_engine_cs *ring,
			      u64 offset, u32 len,
>>>>>>> v3.18
			      unsigned flags)
{
	int ret;

	ret = intel_ring_begin(ring, 2);
	if (ret)
		return ret;

	intel_ring_emit(ring,
<<<<<<< HEAD
<<<<<<< HEAD
			MI_BATCH_BUFFER_START | MI_BATCH_PPGTT_HSW |
			(flags & I915_DISPATCH_SECURE ? 0 : MI_BATCH_NON_SECURE_HSW));
=======
			MI_BATCH_BUFFER_START |
			(flags & I915_DISPATCH_SECURE ?
			 0 : MI_BATCH_PPGTT_HSW | MI_BATCH_NON_SECURE_HSW));
>>>>>>> v3.18
=======
			MI_BATCH_BUFFER_START |
			(flags & I915_DISPATCH_SECURE ?
			 0 : MI_BATCH_PPGTT_HSW | MI_BATCH_NON_SECURE_HSW));
>>>>>>> v3.18
	/* bit0-7 is the length on GEN6+ */
	intel_ring_emit(ring, offset);
	intel_ring_advance(ring);

	return 0;
}

static int
<<<<<<< HEAD
<<<<<<< HEAD
gen6_ring_dispatch_execbuffer(struct intel_ring_buffer *ring,
			      u32 offset, u32 len,
=======
gen6_ring_dispatch_execbuffer(struct intel_engine_cs *ring,
			      u64 offset, u32 len,
>>>>>>> v3.18
=======
gen6_ring_dispatch_execbuffer(struct intel_engine_cs *ring,
			      u64 offset, u32 len,
>>>>>>> v3.18
			      unsigned flags)
{
	int ret;

	ret = intel_ring_begin(ring, 2);
	if (ret)
		return ret;

	intel_ring_emit(ring,
			MI_BATCH_BUFFER_START |
			(flags & I915_DISPATCH_SECURE ? 0 : MI_BATCH_NON_SECURE_I965));
	/* bit0-7 is the length on GEN6+ */
	intel_ring_emit(ring, offset);
	intel_ring_advance(ring);

	return 0;
}

/* Blitter support (SandyBridge+) */

<<<<<<< HEAD
<<<<<<< HEAD
static int blt_ring_flush(struct intel_ring_buffer *ring,
			  u32 invalidate, u32 flush)
{
=======
=======
>>>>>>> v3.18
static int gen6_ring_flush(struct intel_engine_cs *ring,
			   u32 invalidate, u32 flush)
{
	struct drm_device *dev = ring->dev;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	uint32_t cmd;
	int ret;

	ret = intel_ring_begin(ring, 4);
	if (ret)
		return ret;

	cmd = MI_FLUSH_DW;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (INTEL_INFO(ring->dev)->gen >= 8)
		cmd += 1;
>>>>>>> v3.18
=======
	if (INTEL_INFO(ring->dev)->gen >= 8)
		cmd += 1;
>>>>>>> v3.18
	/*
	 * Bspec vol 1c.3 - blitter engine command streamer:
	 * "If ENABLED, all TLBs will be invalidated once the flush
	 * operation is complete. This bit is only valid when the
	 * Post-Sync Operation field is a value of 1h or 3h."
	 */
	if (invalidate & I915_GEM_DOMAIN_RENDER)
		cmd |= MI_INVALIDATE_TLB | MI_FLUSH_DW_STORE_INDEX |
			MI_FLUSH_DW_OP_STOREDW;
	intel_ring_emit(ring, cmd);
	intel_ring_emit(ring, I915_GEM_HWS_SCRATCH_ADDR | MI_FLUSH_DW_USE_GTT);
<<<<<<< HEAD
<<<<<<< HEAD
	intel_ring_emit(ring, 0);
	intel_ring_emit(ring, MI_NOOP);
	intel_ring_advance(ring);
=======
=======
>>>>>>> v3.18
	if (INTEL_INFO(ring->dev)->gen >= 8) {
		intel_ring_emit(ring, 0); /* upper addr */
		intel_ring_emit(ring, 0); /* value */
	} else  {
		intel_ring_emit(ring, 0);
		intel_ring_emit(ring, MI_NOOP);
	}
	intel_ring_advance(ring);

	if (IS_GEN7(dev) && !invalidate && flush)
		return gen7_ring_fbc_flush(ring, FBC_REND_CACHE_CLEAN);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
}

int intel_init_render_ring_buffer(struct drm_device *dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct intel_ring_buffer *ring = &dev_priv->ring[RCS];
=======
=======
>>>>>>> v3.18
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_engine_cs *ring = &dev_priv->ring[RCS];
	struct drm_i915_gem_object *obj;
	int ret;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	ring->name = "render ring";
	ring->id = RCS;
	ring->mmio_base = RENDER_RING_BASE;

<<<<<<< HEAD
<<<<<<< HEAD
	if (INTEL_INFO(dev)->gen >= 6) {
=======
=======
>>>>>>> v3.18
	if (INTEL_INFO(dev)->gen >= 8) {
		if (i915_semaphore_is_enabled(dev)) {
			obj = i915_gem_alloc_object(dev, 4096);
			if (obj == NULL) {
				DRM_ERROR("Failed to allocate semaphore bo. Disabling semaphores\n");
				i915.semaphores = 0;
			} else {
				i915_gem_object_set_cache_level(obj, I915_CACHE_LLC);
				ret = i915_gem_obj_ggtt_pin(obj, 0, PIN_NONBLOCK);
				if (ret != 0) {
					drm_gem_object_unreference(&obj->base);
					DRM_ERROR("Failed to pin semaphore bo. Disabling semaphores\n");
					i915.semaphores = 0;
				} else
					dev_priv->semaphore_obj = obj;
			}
		}
		if (IS_CHERRYVIEW(dev))
			ring->init_context = chv_init_workarounds;
		else
			ring->init_context = bdw_init_workarounds;
		ring->add_request = gen6_add_request;
		ring->flush = gen8_render_ring_flush;
		ring->irq_get = gen8_ring_get_irq;
		ring->irq_put = gen8_ring_put_irq;
		ring->irq_enable_mask = GT_RENDER_USER_INTERRUPT;
		ring->get_seqno = gen6_ring_get_seqno;
		ring->set_seqno = ring_set_seqno;
		if (i915_semaphore_is_enabled(dev)) {
			WARN_ON(!dev_priv->semaphore_obj);
			ring->semaphore.sync_to = gen8_ring_sync;
			ring->semaphore.signal = gen8_rcs_signal;
			GEN8_RING_SEMAPHORE_INIT;
		}
	} else if (INTEL_INFO(dev)->gen >= 6) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		ring->add_request = gen6_add_request;
		ring->flush = gen7_render_ring_flush;
		if (INTEL_INFO(dev)->gen == 6)
			ring->flush = gen6_render_ring_flush;
		ring->irq_get = gen6_ring_get_irq;
		ring->irq_put = gen6_ring_put_irq;
<<<<<<< HEAD
<<<<<<< HEAD
		ring->irq_enable_mask = GT_USER_INTERRUPT;
		ring->get_seqno = gen6_ring_get_seqno;
		ring->set_seqno = ring_set_seqno;
		ring->sync_to = gen6_ring_sync;
		ring->semaphore_register[0] = MI_SEMAPHORE_SYNC_INVALID;
		ring->semaphore_register[1] = MI_SEMAPHORE_SYNC_RV;
		ring->semaphore_register[2] = MI_SEMAPHORE_SYNC_RB;
		ring->signal_mbox[0] = GEN6_VRSYNC;
		ring->signal_mbox[1] = GEN6_BRSYNC;
=======
=======
>>>>>>> v3.18
		ring->irq_enable_mask = GT_RENDER_USER_INTERRUPT;
		ring->get_seqno = gen6_ring_get_seqno;
		ring->set_seqno = ring_set_seqno;
		if (i915_semaphore_is_enabled(dev)) {
			ring->semaphore.sync_to = gen6_ring_sync;
			ring->semaphore.signal = gen6_signal;
			/*
			 * The current semaphore is only applied on pre-gen8
			 * platform.  And there is no VCS2 ring on the pre-gen8
			 * platform. So the semaphore between RCS and VCS2 is
			 * initialized as INVALID.  Gen8 will initialize the
			 * sema between VCS2 and RCS later.
			 */
			ring->semaphore.mbox.wait[RCS] = MI_SEMAPHORE_SYNC_INVALID;
			ring->semaphore.mbox.wait[VCS] = MI_SEMAPHORE_SYNC_RV;
			ring->semaphore.mbox.wait[BCS] = MI_SEMAPHORE_SYNC_RB;
			ring->semaphore.mbox.wait[VECS] = MI_SEMAPHORE_SYNC_RVE;
			ring->semaphore.mbox.wait[VCS2] = MI_SEMAPHORE_SYNC_INVALID;
			ring->semaphore.mbox.signal[RCS] = GEN6_NOSYNC;
			ring->semaphore.mbox.signal[VCS] = GEN6_VRSYNC;
			ring->semaphore.mbox.signal[BCS] = GEN6_BRSYNC;
			ring->semaphore.mbox.signal[VECS] = GEN6_VERSYNC;
			ring->semaphore.mbox.signal[VCS2] = GEN6_NOSYNC;
		}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	} else if (IS_GEN5(dev)) {
		ring->add_request = pc_render_add_request;
		ring->flush = gen4_render_ring_flush;
		ring->get_seqno = pc_render_get_seqno;
		ring->set_seqno = pc_render_set_seqno;
		ring->irq_get = gen5_ring_get_irq;
		ring->irq_put = gen5_ring_put_irq;
<<<<<<< HEAD
<<<<<<< HEAD
		ring->irq_enable_mask = GT_USER_INTERRUPT | GT_PIPE_NOTIFY;
=======
		ring->irq_enable_mask = GT_RENDER_USER_INTERRUPT |
					GT_RENDER_PIPECTL_NOTIFY_INTERRUPT;
>>>>>>> v3.18
=======
		ring->irq_enable_mask = GT_RENDER_USER_INTERRUPT |
					GT_RENDER_PIPECTL_NOTIFY_INTERRUPT;
>>>>>>> v3.18
	} else {
		ring->add_request = i9xx_add_request;
		if (INTEL_INFO(dev)->gen < 4)
			ring->flush = gen2_render_ring_flush;
		else
			ring->flush = gen4_render_ring_flush;
		ring->get_seqno = ring_get_seqno;
		ring->set_seqno = ring_set_seqno;
		if (IS_GEN2(dev)) {
			ring->irq_get = i8xx_ring_get_irq;
			ring->irq_put = i8xx_ring_put_irq;
		} else {
			ring->irq_get = i9xx_ring_get_irq;
			ring->irq_put = i9xx_ring_put_irq;
		}
		ring->irq_enable_mask = I915_USER_INTERRUPT;
	}
	ring->write_tail = ring_write_tail;
<<<<<<< HEAD
<<<<<<< HEAD
	if (IS_HASWELL(dev))
		ring->dispatch_execbuffer = hsw_ring_dispatch_execbuffer;
=======
=======
>>>>>>> v3.18

	if (IS_HASWELL(dev))
		ring->dispatch_execbuffer = hsw_ring_dispatch_execbuffer;
	else if (IS_GEN8(dev))
		ring->dispatch_execbuffer = gen8_ring_dispatch_execbuffer;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	else if (INTEL_INFO(dev)->gen >= 6)
		ring->dispatch_execbuffer = gen6_ring_dispatch_execbuffer;
	else if (INTEL_INFO(dev)->gen >= 4)
		ring->dispatch_execbuffer = i965_dispatch_execbuffer;
	else if (IS_I830(dev) || IS_845G(dev))
		ring->dispatch_execbuffer = i830_dispatch_execbuffer;
	else
		ring->dispatch_execbuffer = i915_dispatch_execbuffer;
	ring->init = init_render_ring;
	ring->cleanup = render_ring_cleanup;

	/* Workaround batchbuffer to combat CS tlb bug. */
	if (HAS_BROKEN_CS_TLB(dev)) {
<<<<<<< HEAD
<<<<<<< HEAD
		struct drm_i915_gem_object *obj;
		int ret;

		obj = i915_gem_alloc_object(dev, I830_BATCH_LIMIT);
=======
		obj = i915_gem_alloc_object(dev, I830_WA_SIZE);
>>>>>>> v3.18
=======
		obj = i915_gem_alloc_object(dev, I830_WA_SIZE);
>>>>>>> v3.18
		if (obj == NULL) {
			DRM_ERROR("Failed to allocate batch bo\n");
			return -ENOMEM;
		}

<<<<<<< HEAD
<<<<<<< HEAD
		ret = i915_gem_object_pin(obj, 0, true, false);
=======
		ret = i915_gem_obj_ggtt_pin(obj, 0, 0);
>>>>>>> v3.18
=======
		ret = i915_gem_obj_ggtt_pin(obj, 0, 0);
>>>>>>> v3.18
		if (ret != 0) {
			drm_gem_object_unreference(&obj->base);
			DRM_ERROR("Failed to ping batch bo\n");
			return ret;
		}

<<<<<<< HEAD
<<<<<<< HEAD
		ring->private = obj;
=======
		ring->scratch.obj = obj;
		ring->scratch.gtt_offset = i915_gem_obj_ggtt_offset(obj);
>>>>>>> v3.18
=======
		ring->scratch.obj = obj;
		ring->scratch.gtt_offset = i915_gem_obj_ggtt_offset(obj);
>>>>>>> v3.18
	}

	return intel_init_ring_buffer(dev, ring);
}

int intel_render_ring_init_dri(struct drm_device *dev, u64 start, u32 size)
{
<<<<<<< HEAD
<<<<<<< HEAD
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct intel_ring_buffer *ring = &dev_priv->ring[RCS];
	int ret;

=======
=======
>>>>>>> v3.18
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_engine_cs *ring = &dev_priv->ring[RCS];
	struct intel_ringbuffer *ringbuf = ring->buffer;
	int ret;

	if (ringbuf == NULL) {
		ringbuf = kzalloc(sizeof(*ringbuf), GFP_KERNEL);
		if (!ringbuf)
			return -ENOMEM;
		ring->buffer = ringbuf;
	}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	ring->name = "render ring";
	ring->id = RCS;
	ring->mmio_base = RENDER_RING_BASE;

	if (INTEL_INFO(dev)->gen >= 6) {
		/* non-kms not supported on gen6+ */
<<<<<<< HEAD
<<<<<<< HEAD
		return -ENODEV;
=======
		ret = -ENODEV;
		goto err_ringbuf;
>>>>>>> v3.18
=======
		ret = -ENODEV;
		goto err_ringbuf;
>>>>>>> v3.18
	}

	/* Note: gem is not supported on gen5/ilk without kms (the corresponding
	 * gem_init ioctl returns with -ENODEV). Hence we do not need to set up
	 * the special gen5 functions. */
	ring->add_request = i9xx_add_request;
	if (INTEL_INFO(dev)->gen < 4)
		ring->flush = gen2_render_ring_flush;
	else
		ring->flush = gen4_render_ring_flush;
	ring->get_seqno = ring_get_seqno;
	ring->set_seqno = ring_set_seqno;
	if (IS_GEN2(dev)) {
		ring->irq_get = i8xx_ring_get_irq;
		ring->irq_put = i8xx_ring_put_irq;
	} else {
		ring->irq_get = i9xx_ring_get_irq;
		ring->irq_put = i9xx_ring_put_irq;
	}
	ring->irq_enable_mask = I915_USER_INTERRUPT;
	ring->write_tail = ring_write_tail;
	if (INTEL_INFO(dev)->gen >= 4)
		ring->dispatch_execbuffer = i965_dispatch_execbuffer;
	else if (IS_I830(dev) || IS_845G(dev))
		ring->dispatch_execbuffer = i830_dispatch_execbuffer;
	else
		ring->dispatch_execbuffer = i915_dispatch_execbuffer;
	ring->init = init_render_ring;
	ring->cleanup = render_ring_cleanup;

	ring->dev = dev;
	INIT_LIST_HEAD(&ring->active_list);
	INIT_LIST_HEAD(&ring->request_list);

<<<<<<< HEAD
<<<<<<< HEAD
	ring->size = size;
	ring->effective_size = ring->size;
	if (IS_I830(ring->dev) || IS_845G(ring->dev))
		ring->effective_size -= 128;

	ring->virtual_start = ioremap_wc(start, size);
	if (ring->virtual_start == NULL) {
		DRM_ERROR("can not ioremap virtual address for"
			  " ring buffer\n");
		return -ENOMEM;
	}

	if (!I915_NEED_GFX_HWS(dev)) {
		ret = init_phys_hws_pga(ring);
		if (ret)
			return ret;
	}

	return 0;
=======
=======
>>>>>>> v3.18
	ringbuf->size = size;
	ringbuf->effective_size = ringbuf->size;
	if (IS_I830(ring->dev) || IS_845G(ring->dev))
		ringbuf->effective_size -= 2 * CACHELINE_BYTES;

	ringbuf->virtual_start = ioremap_wc(start, size);
	if (ringbuf->virtual_start == NULL) {
		DRM_ERROR("can not ioremap virtual address for"
			  " ring buffer\n");
		ret = -ENOMEM;
		goto err_ringbuf;
	}

	if (!I915_NEED_GFX_HWS(dev)) {
		ret = init_phys_status_page(ring);
		if (ret)
			goto err_vstart;
	}

	return 0;

err_vstart:
	iounmap(ringbuf->virtual_start);
err_ringbuf:
	kfree(ringbuf);
	ring->buffer = NULL;
	return ret;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

int intel_init_bsd_ring_buffer(struct drm_device *dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct intel_ring_buffer *ring = &dev_priv->ring[VCS];
=======
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_engine_cs *ring = &dev_priv->ring[VCS];
>>>>>>> v3.18
=======
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_engine_cs *ring = &dev_priv->ring[VCS];
>>>>>>> v3.18

	ring->name = "bsd ring";
	ring->id = VCS;

	ring->write_tail = ring_write_tail;
<<<<<<< HEAD
<<<<<<< HEAD
	if (IS_GEN6(dev) || IS_GEN7(dev)) {
=======
	if (INTEL_INFO(dev)->gen >= 6) {
>>>>>>> v3.18
=======
	if (INTEL_INFO(dev)->gen >= 6) {
>>>>>>> v3.18
		ring->mmio_base = GEN6_BSD_RING_BASE;
		/* gen6 bsd needs a special wa for tail updates */
		if (IS_GEN6(dev))
			ring->write_tail = gen6_bsd_ring_write_tail;
<<<<<<< HEAD
<<<<<<< HEAD
		ring->flush = gen6_ring_flush;
		ring->add_request = gen6_add_request;
		ring->get_seqno = gen6_ring_get_seqno;
		ring->set_seqno = ring_set_seqno;
		ring->irq_enable_mask = GEN6_BSD_USER_INTERRUPT;
		ring->irq_get = gen6_ring_get_irq;
		ring->irq_put = gen6_ring_put_irq;
		ring->dispatch_execbuffer = gen6_ring_dispatch_execbuffer;
		ring->sync_to = gen6_ring_sync;
		ring->semaphore_register[0] = MI_SEMAPHORE_SYNC_VR;
		ring->semaphore_register[1] = MI_SEMAPHORE_SYNC_INVALID;
		ring->semaphore_register[2] = MI_SEMAPHORE_SYNC_VB;
		ring->signal_mbox[0] = GEN6_RVSYNC;
		ring->signal_mbox[1] = GEN6_BVSYNC;
=======
=======
>>>>>>> v3.18
		ring->flush = gen6_bsd_ring_flush;
		ring->add_request = gen6_add_request;
		ring->get_seqno = gen6_ring_get_seqno;
		ring->set_seqno = ring_set_seqno;
		if (INTEL_INFO(dev)->gen >= 8) {
			ring->irq_enable_mask =
				GT_RENDER_USER_INTERRUPT << GEN8_VCS1_IRQ_SHIFT;
			ring->irq_get = gen8_ring_get_irq;
			ring->irq_put = gen8_ring_put_irq;
			ring->dispatch_execbuffer =
				gen8_ring_dispatch_execbuffer;
			if (i915_semaphore_is_enabled(dev)) {
				ring->semaphore.sync_to = gen8_ring_sync;
				ring->semaphore.signal = gen8_xcs_signal;
				GEN8_RING_SEMAPHORE_INIT;
			}
		} else {
			ring->irq_enable_mask = GT_BSD_USER_INTERRUPT;
			ring->irq_get = gen6_ring_get_irq;
			ring->irq_put = gen6_ring_put_irq;
			ring->dispatch_execbuffer =
				gen6_ring_dispatch_execbuffer;
			if (i915_semaphore_is_enabled(dev)) {
				ring->semaphore.sync_to = gen6_ring_sync;
				ring->semaphore.signal = gen6_signal;
				ring->semaphore.mbox.wait[RCS] = MI_SEMAPHORE_SYNC_VR;
				ring->semaphore.mbox.wait[VCS] = MI_SEMAPHORE_SYNC_INVALID;
				ring->semaphore.mbox.wait[BCS] = MI_SEMAPHORE_SYNC_VB;
				ring->semaphore.mbox.wait[VECS] = MI_SEMAPHORE_SYNC_VVE;
				ring->semaphore.mbox.wait[VCS2] = MI_SEMAPHORE_SYNC_INVALID;
				ring->semaphore.mbox.signal[RCS] = GEN6_RVSYNC;
				ring->semaphore.mbox.signal[VCS] = GEN6_NOSYNC;
				ring->semaphore.mbox.signal[BCS] = GEN6_BVSYNC;
				ring->semaphore.mbox.signal[VECS] = GEN6_VEVSYNC;
				ring->semaphore.mbox.signal[VCS2] = GEN6_NOSYNC;
			}
		}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	} else {
		ring->mmio_base = BSD_RING_BASE;
		ring->flush = bsd_ring_flush;
		ring->add_request = i9xx_add_request;
		ring->get_seqno = ring_get_seqno;
		ring->set_seqno = ring_set_seqno;
		if (IS_GEN5(dev)) {
<<<<<<< HEAD
<<<<<<< HEAD
			ring->irq_enable_mask = GT_BSD_USER_INTERRUPT;
=======
			ring->irq_enable_mask = ILK_BSD_USER_INTERRUPT;
>>>>>>> v3.18
=======
			ring->irq_enable_mask = ILK_BSD_USER_INTERRUPT;
>>>>>>> v3.18
			ring->irq_get = gen5_ring_get_irq;
			ring->irq_put = gen5_ring_put_irq;
		} else {
			ring->irq_enable_mask = I915_BSD_USER_INTERRUPT;
			ring->irq_get = i9xx_ring_get_irq;
			ring->irq_put = i9xx_ring_put_irq;
		}
		ring->dispatch_execbuffer = i965_dispatch_execbuffer;
	}
	ring->init = init_ring_common;

	return intel_init_ring_buffer(dev, ring);
}

<<<<<<< HEAD
<<<<<<< HEAD
int intel_init_blt_ring_buffer(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct intel_ring_buffer *ring = &dev_priv->ring[BCS];
=======
=======
>>>>>>> v3.18
/**
 * Initialize the second BSD ring for Broadwell GT3.
 * It is noted that this only exists on Broadwell GT3.
 */
int intel_init_bsd2_ring_buffer(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_engine_cs *ring = &dev_priv->ring[VCS2];

	if ((INTEL_INFO(dev)->gen != 8)) {
		DRM_ERROR("No dual-BSD ring on non-BDW machine\n");
		return -EINVAL;
	}

	ring->name = "bsd2 ring";
	ring->id = VCS2;

	ring->write_tail = ring_write_tail;
	ring->mmio_base = GEN8_BSD2_RING_BASE;
	ring->flush = gen6_bsd_ring_flush;
	ring->add_request = gen6_add_request;
	ring->get_seqno = gen6_ring_get_seqno;
	ring->set_seqno = ring_set_seqno;
	ring->irq_enable_mask =
			GT_RENDER_USER_INTERRUPT << GEN8_VCS2_IRQ_SHIFT;
	ring->irq_get = gen8_ring_get_irq;
	ring->irq_put = gen8_ring_put_irq;
	ring->dispatch_execbuffer =
			gen8_ring_dispatch_execbuffer;
	if (i915_semaphore_is_enabled(dev)) {
		ring->semaphore.sync_to = gen8_ring_sync;
		ring->semaphore.signal = gen8_xcs_signal;
		GEN8_RING_SEMAPHORE_INIT;
	}
	ring->init = init_ring_common;

	return intel_init_ring_buffer(dev, ring);
}

int intel_init_blt_ring_buffer(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_engine_cs *ring = &dev_priv->ring[BCS];
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	ring->name = "blitter ring";
	ring->id = BCS;

	ring->mmio_base = BLT_RING_BASE;
	ring->write_tail = ring_write_tail;
<<<<<<< HEAD
<<<<<<< HEAD
	ring->flush = blt_ring_flush;
	ring->add_request = gen6_add_request;
	ring->get_seqno = gen6_ring_get_seqno;
	ring->set_seqno = ring_set_seqno;
	ring->irq_enable_mask = GEN6_BLITTER_USER_INTERRUPT;
	ring->irq_get = gen6_ring_get_irq;
	ring->irq_put = gen6_ring_put_irq;
	ring->dispatch_execbuffer = gen6_ring_dispatch_execbuffer;
	ring->sync_to = gen6_ring_sync;
	ring->semaphore_register[0] = MI_SEMAPHORE_SYNC_BR;
	ring->semaphore_register[1] = MI_SEMAPHORE_SYNC_BV;
	ring->semaphore_register[2] = MI_SEMAPHORE_SYNC_INVALID;
	ring->signal_mbox[0] = GEN6_RBSYNC;
	ring->signal_mbox[1] = GEN6_VBSYNC;
=======
=======
>>>>>>> v3.18
	ring->flush = gen6_ring_flush;
	ring->add_request = gen6_add_request;
	ring->get_seqno = gen6_ring_get_seqno;
	ring->set_seqno = ring_set_seqno;
	if (INTEL_INFO(dev)->gen >= 8) {
		ring->irq_enable_mask =
			GT_RENDER_USER_INTERRUPT << GEN8_BCS_IRQ_SHIFT;
		ring->irq_get = gen8_ring_get_irq;
		ring->irq_put = gen8_ring_put_irq;
		ring->dispatch_execbuffer = gen8_ring_dispatch_execbuffer;
		if (i915_semaphore_is_enabled(dev)) {
			ring->semaphore.sync_to = gen8_ring_sync;
			ring->semaphore.signal = gen8_xcs_signal;
			GEN8_RING_SEMAPHORE_INIT;
		}
	} else {
		ring->irq_enable_mask = GT_BLT_USER_INTERRUPT;
		ring->irq_get = gen6_ring_get_irq;
		ring->irq_put = gen6_ring_put_irq;
		ring->dispatch_execbuffer = gen6_ring_dispatch_execbuffer;
		if (i915_semaphore_is_enabled(dev)) {
			ring->semaphore.signal = gen6_signal;
			ring->semaphore.sync_to = gen6_ring_sync;
			/*
			 * The current semaphore is only applied on pre-gen8
			 * platform.  And there is no VCS2 ring on the pre-gen8
			 * platform. So the semaphore between BCS and VCS2 is
			 * initialized as INVALID.  Gen8 will initialize the
			 * sema between BCS and VCS2 later.
			 */
			ring->semaphore.mbox.wait[RCS] = MI_SEMAPHORE_SYNC_BR;
			ring->semaphore.mbox.wait[VCS] = MI_SEMAPHORE_SYNC_BV;
			ring->semaphore.mbox.wait[BCS] = MI_SEMAPHORE_SYNC_INVALID;
			ring->semaphore.mbox.wait[VECS] = MI_SEMAPHORE_SYNC_BVE;
			ring->semaphore.mbox.wait[VCS2] = MI_SEMAPHORE_SYNC_INVALID;
			ring->semaphore.mbox.signal[RCS] = GEN6_RBSYNC;
			ring->semaphore.mbox.signal[VCS] = GEN6_VBSYNC;
			ring->semaphore.mbox.signal[BCS] = GEN6_NOSYNC;
			ring->semaphore.mbox.signal[VECS] = GEN6_VEBSYNC;
			ring->semaphore.mbox.signal[VCS2] = GEN6_NOSYNC;
		}
	}
	ring->init = init_ring_common;

	return intel_init_ring_buffer(dev, ring);
}

int intel_init_vebox_ring_buffer(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_engine_cs *ring = &dev_priv->ring[VECS];

	ring->name = "video enhancement ring";
	ring->id = VECS;

	ring->mmio_base = VEBOX_RING_BASE;
	ring->write_tail = ring_write_tail;
	ring->flush = gen6_ring_flush;
	ring->add_request = gen6_add_request;
	ring->get_seqno = gen6_ring_get_seqno;
	ring->set_seqno = ring_set_seqno;

	if (INTEL_INFO(dev)->gen >= 8) {
		ring->irq_enable_mask =
			GT_RENDER_USER_INTERRUPT << GEN8_VECS_IRQ_SHIFT;
		ring->irq_get = gen8_ring_get_irq;
		ring->irq_put = gen8_ring_put_irq;
		ring->dispatch_execbuffer = gen8_ring_dispatch_execbuffer;
		if (i915_semaphore_is_enabled(dev)) {
			ring->semaphore.sync_to = gen8_ring_sync;
			ring->semaphore.signal = gen8_xcs_signal;
			GEN8_RING_SEMAPHORE_INIT;
		}
	} else {
		ring->irq_enable_mask = PM_VEBOX_USER_INTERRUPT;
		ring->irq_get = hsw_vebox_get_irq;
		ring->irq_put = hsw_vebox_put_irq;
		ring->dispatch_execbuffer = gen6_ring_dispatch_execbuffer;
		if (i915_semaphore_is_enabled(dev)) {
			ring->semaphore.sync_to = gen6_ring_sync;
			ring->semaphore.signal = gen6_signal;
			ring->semaphore.mbox.wait[RCS] = MI_SEMAPHORE_SYNC_VER;
			ring->semaphore.mbox.wait[VCS] = MI_SEMAPHORE_SYNC_VEV;
			ring->semaphore.mbox.wait[BCS] = MI_SEMAPHORE_SYNC_VEB;
			ring->semaphore.mbox.wait[VECS] = MI_SEMAPHORE_SYNC_INVALID;
			ring->semaphore.mbox.wait[VCS2] = MI_SEMAPHORE_SYNC_INVALID;
			ring->semaphore.mbox.signal[RCS] = GEN6_RVESYNC;
			ring->semaphore.mbox.signal[VCS] = GEN6_VVESYNC;
			ring->semaphore.mbox.signal[BCS] = GEN6_BVESYNC;
			ring->semaphore.mbox.signal[VECS] = GEN6_NOSYNC;
			ring->semaphore.mbox.signal[VCS2] = GEN6_NOSYNC;
		}
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	ring->init = init_ring_common;

	return intel_init_ring_buffer(dev, ring);
}

int
<<<<<<< HEAD
<<<<<<< HEAD
intel_ring_flush_all_caches(struct intel_ring_buffer *ring)
=======
intel_ring_flush_all_caches(struct intel_engine_cs *ring)
>>>>>>> v3.18
=======
intel_ring_flush_all_caches(struct intel_engine_cs *ring)
>>>>>>> v3.18
{
	int ret;

	if (!ring->gpu_caches_dirty)
		return 0;

	ret = ring->flush(ring, 0, I915_GEM_GPU_DOMAINS);
	if (ret)
		return ret;

	trace_i915_gem_ring_flush(ring, 0, I915_GEM_GPU_DOMAINS);

	ring->gpu_caches_dirty = false;
	return 0;
}

int
<<<<<<< HEAD
<<<<<<< HEAD
intel_ring_invalidate_all_caches(struct intel_ring_buffer *ring)
=======
intel_ring_invalidate_all_caches(struct intel_engine_cs *ring)
>>>>>>> v3.18
=======
intel_ring_invalidate_all_caches(struct intel_engine_cs *ring)
>>>>>>> v3.18
{
	uint32_t flush_domains;
	int ret;

	flush_domains = 0;
	if (ring->gpu_caches_dirty)
		flush_domains = I915_GEM_GPU_DOMAINS;

	ret = ring->flush(ring, I915_GEM_GPU_DOMAINS, flush_domains);
	if (ret)
		return ret;

	trace_i915_gem_ring_flush(ring, I915_GEM_GPU_DOMAINS, flush_domains);

	ring->gpu_caches_dirty = false;
	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18

void
intel_stop_ring_buffer(struct intel_engine_cs *ring)
{
	int ret;

	if (!intel_ring_initialized(ring))
		return;

	ret = intel_ring_idle(ring);
	if (ret && !i915_reset_in_progress(&to_i915(ring->dev)->gpu_error))
		DRM_ERROR("failed to quiesce %s whilst cleaning up: %d\n",
			  ring->name, ret);

	stop_ring(ring);
}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
