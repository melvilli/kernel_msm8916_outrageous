/*
 * Copyright 2012 Red Hat Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: Ben Skeggs
 */

#include <core/os.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <core/class.h>
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
#include <core/engctx.h>

#include <subdev/fb.h>
#include <subdev/timer.h>
#include <subdev/instmem.h>

#include <engine/mpeg.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <engine/graph/nv40.h>

struct nv40_mpeg_priv {
	struct nouveau_mpeg base;
};

struct nv40_mpeg_chan {
	struct nouveau_mpeg_chan base;
};

/*******************************************************************************
 * PMPEG context
 ******************************************************************************/

static int
nv40_mpeg_context_ctor(struct nouveau_object *parent,
		       struct nouveau_object *engine,
		       struct nouveau_oclass *oclass, void *data, u32 size,
		       struct nouveau_object **pobject)
{
	struct nv40_mpeg_chan *chan;
	int ret;

	ret = nouveau_mpeg_context_create(parent, engine, oclass, NULL,
					  264 * 4, 16,
					  NVOBJ_FLAG_ZERO_ALLOC, &chan);
	*pobject = nv_object(chan);
	if (ret)
		return ret;
=======
=======
>>>>>>> v3.18
#include <engine/mpeg/nv31.h>

/*******************************************************************************
 * MPEG object classes
 ******************************************************************************/

static int
nv40_mpeg_mthd_dma(struct nouveau_object *object, u32 mthd, void *arg, u32 len)
{
	struct nouveau_instmem *imem = nouveau_instmem(object);
	struct nv31_mpeg_priv *priv = (void *)object->engine;
	u32 inst = *(u32 *)arg << 4;
	u32 dma0 = nv_ro32(imem, inst + 0);
	u32 dma1 = nv_ro32(imem, inst + 4);
	u32 dma2 = nv_ro32(imem, inst + 8);
	u32 base = (dma2 & 0xfffff000) | (dma0 >> 20);
	u32 size = dma1 + 1;

	/* only allow linear DMA objects */
	if (!(dma0 & 0x00002000))
		return -EINVAL;

	if (mthd == 0x0190) {
		/* DMA_CMD */
		nv_mask(priv, 0x00b300, 0x00030000, (dma0 & 0x00030000));
		nv_wr32(priv, 0x00b334, base);
		nv_wr32(priv, 0x00b324, size);
	} else
	if (mthd == 0x01a0) {
		/* DMA_DATA */
		nv_mask(priv, 0x00b300, 0x000c0000, (dma0 & 0x00030000) << 2);
		nv_wr32(priv, 0x00b360, base);
		nv_wr32(priv, 0x00b364, size);
	} else {
		/* DMA_IMAGE, VRAM only */
		if (dma0 & 0x00030000)
			return -EINVAL;

		nv_wr32(priv, 0x00b370, base);
		nv_wr32(priv, 0x00b374, size);
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int
nv40_mpeg_context_fini(struct nouveau_object *object, bool suspend)
{

	struct nv40_mpeg_priv *priv = (void *)object->engine;
	struct nv40_mpeg_chan *chan = (void *)object;
	u32 inst = 0x80000000 | nv_gpuobj(chan)->addr >> 4;

	nv_mask(priv, 0x00b32c, 0x00000001, 0x00000000);
	if (nv_rd32(priv, 0x00b318) == inst)
		nv_mask(priv, 0x00b318, 0x80000000, 0x00000000);
	nv_mask(priv, 0x00b32c, 0x00000001, 0x00000001);
	return 0;
}

static struct nouveau_oclass
nv40_mpeg_cclass = {
	.handle = NV_ENGCTX(MPEG, 0x40),
	.ofuncs = &(struct nouveau_ofuncs) {
		.ctor = nv40_mpeg_context_ctor,
		.dtor = _nouveau_mpeg_context_dtor,
		.init = _nouveau_mpeg_context_init,
		.fini = nv40_mpeg_context_fini,
		.rd32 = _nouveau_mpeg_context_rd32,
		.wr32 = _nouveau_mpeg_context_wr32,
	},
=======
=======
>>>>>>> v3.18
static struct nouveau_omthds
nv40_mpeg_omthds[] = {
	{ 0x0190, 0x0190, nv40_mpeg_mthd_dma },
	{ 0x01a0, 0x01a0, nv40_mpeg_mthd_dma },
	{ 0x01b0, 0x01b0, nv40_mpeg_mthd_dma },
	{}
};

struct nouveau_oclass
nv40_mpeg_sclass[] = {
	{ 0x3174, &nv31_mpeg_ofuncs, nv40_mpeg_omthds },
	{}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
};

/*******************************************************************************
 * PMPEG engine/subdev functions
 ******************************************************************************/

static void
nv40_mpeg_intr(struct nouveau_subdev *subdev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct nv40_mpeg_priv *priv = (void *)subdev;
=======
	struct nv31_mpeg_priv *priv = (void *)subdev;
>>>>>>> v3.18
=======
	struct nv31_mpeg_priv *priv = (void *)subdev;
>>>>>>> v3.18
	u32 stat;

	if ((stat = nv_rd32(priv, 0x00b100)))
		nv31_mpeg_intr(subdev);

	if ((stat = nv_rd32(priv, 0x00b800))) {
		nv_error(priv, "PMSRCH 0x%08x\n", stat);
		nv_wr32(priv, 0x00b800, stat);
	}
}

static int
nv40_mpeg_ctor(struct nouveau_object *parent, struct nouveau_object *engine,
	       struct nouveau_oclass *oclass, void *data, u32 size,
	       struct nouveau_object **pobject)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct nv40_mpeg_priv *priv;
=======
	struct nv31_mpeg_priv *priv;
>>>>>>> v3.18
=======
	struct nv31_mpeg_priv *priv;
>>>>>>> v3.18
	int ret;

	ret = nouveau_mpeg_create(parent, engine, oclass, &priv);
	*pobject = nv_object(priv);
	if (ret)
		return ret;

	nv_subdev(priv)->unit = 0x00000002;
	nv_subdev(priv)->intr = nv40_mpeg_intr;
<<<<<<< HEAD
<<<<<<< HEAD
	nv_engine(priv)->cclass = &nv40_mpeg_cclass;
	nv_engine(priv)->sclass = nv31_mpeg_sclass;
=======
	nv_engine(priv)->cclass = &nv31_mpeg_cclass;
	nv_engine(priv)->sclass = nv40_mpeg_sclass;
>>>>>>> v3.18
=======
	nv_engine(priv)->cclass = &nv31_mpeg_cclass;
	nv_engine(priv)->sclass = nv40_mpeg_sclass;
>>>>>>> v3.18
	nv_engine(priv)->tile_prog = nv31_mpeg_tile_prog;
	return 0;
}

struct nouveau_oclass
nv40_mpeg_oclass = {
	.handle = NV_ENGINE(MPEG, 0x40),
	.ofuncs = &(struct nouveau_ofuncs) {
		.ctor = nv40_mpeg_ctor,
		.dtor = _nouveau_mpeg_dtor,
		.init = nv31_mpeg_init,
		.fini = _nouveau_mpeg_fini,
	},
};
