/*
 * Copyright 2012 Nouveau Community
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
 * Authors: Martin Peres <martin.peres@labri.fr>
 *          Ben Skeggs
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include <subdev/bus.h>

struct nv04_bus_priv {
	struct nouveau_bus base;
};
=======
#include "nv04.h"
>>>>>>> v3.18
=======
#include "nv04.h"
>>>>>>> v3.18

static void
nv04_bus_intr(struct nouveau_subdev *subdev)
{
	struct nouveau_bus *pbus = nouveau_bus(subdev);
	u32 stat = nv_rd32(pbus, 0x001100) & nv_rd32(pbus, 0x001140);

	if (stat & 0x00000001) {
		nv_error(pbus, "BUS ERROR\n");
		stat &= ~0x00000001;
		nv_wr32(pbus, 0x001100, 0x00000001);
	}

	if (stat & 0x00000110) {
		subdev = nouveau_subdev(subdev, NVDEV_SUBDEV_GPIO);
		if (subdev && subdev->intr)
			subdev->intr(subdev);
		stat &= ~0x00000110;
		nv_wr32(pbus, 0x001100, 0x00000110);
	}

	if (stat) {
		nv_error(pbus, "unknown intr 0x%08x\n", stat);
		nv_mask(pbus, 0x001140, stat, 0x00000000);
	}
}

static int
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
nv04_bus_init(struct nouveau_object *object)
{
	struct nv04_bus_priv *priv = (void *)object;

	nv_wr32(priv, 0x001100, 0xffffffff);
	nv_wr32(priv, 0x001140, 0x00000111);

	return nouveau_bus_init(&priv->base);
}

int
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
nv04_bus_ctor(struct nouveau_object *parent, struct nouveau_object *engine,
	      struct nouveau_oclass *oclass, void *data, u32 size,
	      struct nouveau_object **pobject)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct nv04_bus_impl *impl = (void *)oclass;
>>>>>>> v3.18
=======
	struct nv04_bus_impl *impl = (void *)oclass;
>>>>>>> v3.18
	struct nv04_bus_priv *priv;
	int ret;

	ret = nouveau_bus_create(parent, engine, oclass, &priv);
	*pobject = nv_object(priv);
	if (ret)
		return ret;

<<<<<<< HEAD
<<<<<<< HEAD
	nv_subdev(priv)->intr = nv04_bus_intr;
	return 0;
}

static int
nv04_bus_init(struct nouveau_object *object)
{
	struct nv04_bus_priv *priv = (void *)object;

	nv_wr32(priv, 0x001100, 0xffffffff);
	nv_wr32(priv, 0x001140, 0x00000111);

	return nouveau_bus_init(&priv->base);
}

struct nouveau_oclass
nv04_bus_oclass = {
	.handle = NV_SUBDEV(BUS, 0x04),
	.ofuncs = &(struct nouveau_ofuncs) {
=======
=======
>>>>>>> v3.18
	nv_subdev(priv)->intr = impl->intr;
	priv->base.hwsq_exec = impl->hwsq_exec;
	priv->base.hwsq_size = impl->hwsq_size;
	return 0;
}

struct nouveau_oclass *
nv04_bus_oclass = &(struct nv04_bus_impl) {
	.base.handle = NV_SUBDEV(BUS, 0x04),
	.base.ofuncs = &(struct nouveau_ofuncs) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		.ctor = nv04_bus_ctor,
		.dtor = _nouveau_bus_dtor,
		.init = nv04_bus_init,
		.fini = _nouveau_bus_fini,
	},
<<<<<<< HEAD
<<<<<<< HEAD
};
=======
	.intr = nv04_bus_intr,
}.base;
>>>>>>> v3.18
=======
	.intr = nv04_bus_intr,
}.base;
>>>>>>> v3.18
