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

#include <subdev/bios.h>
#include <subdev/bios/dcb.h>
#include <subdev/bios/dp.h>
#include <subdev/bios/init.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <subdev/timer.h>
>>>>>>> v3.18
=======
#include <subdev/timer.h>
>>>>>>> v3.18

#include "nv50.h"

static inline u32
<<<<<<< HEAD
<<<<<<< HEAD
nvd0_sor_soff(struct dcb_output *outp)
{
	return (ffs(outp->or) - 1) * 0x800;
}

static inline u32
nvd0_sor_loff(struct dcb_output *outp)
{
	return nvd0_sor_soff(outp) + !(outp->sorconf.link & 1) * 0x80;
=======
=======
>>>>>>> v3.18
nvd0_sor_soff(struct nvkm_output_dp *outp)
{
	return (ffs(outp->base.info.or) - 1) * 0x800;
}

static inline u32
nvd0_sor_loff(struct nvkm_output_dp *outp)
{
	return nvd0_sor_soff(outp) + !(outp->base.info.sorconf.link & 1) * 0x80;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static inline u32
nvd0_sor_dp_lane_map(struct nv50_disp_priv *priv, u8 lane)
{
	static const u8 nvd0[] = { 16, 8, 0, 24 };
	return nvd0[lane];
}

static int
<<<<<<< HEAD
<<<<<<< HEAD
nvd0_sor_dp_pattern(struct nouveau_disp *disp, struct dcb_output *outp,
		    int head, int pattern)
{
	struct nv50_disp_priv *priv = (void *)disp;
=======
nvd0_sor_dp_pattern(struct nvkm_output_dp *outp, int pattern)
{
	struct nv50_disp_priv *priv = (void *)nouveau_disp(outp);
>>>>>>> v3.18
=======
nvd0_sor_dp_pattern(struct nvkm_output_dp *outp, int pattern)
{
	struct nv50_disp_priv *priv = (void *)nouveau_disp(outp);
>>>>>>> v3.18
	const u32 loff = nvd0_sor_loff(outp);
	nv_mask(priv, 0x61c110 + loff, 0x0f0f0f0f, 0x01010101 * pattern);
	return 0;
}

static int
<<<<<<< HEAD
<<<<<<< HEAD
nvd0_sor_dp_lnk_ctl(struct nouveau_disp *disp, struct dcb_output *outp,
		    int head, int link_nr, int link_bw, bool enh_frame)
{
	struct nv50_disp_priv *priv = (void *)disp;
=======
nvd0_sor_dp_lnk_ctl(struct nvkm_output_dp *outp, int nr, int bw, bool ef)
{
	struct nv50_disp_priv *priv = (void *)nouveau_disp(outp);
>>>>>>> v3.18
=======
nvd0_sor_dp_lnk_ctl(struct nvkm_output_dp *outp, int nr, int bw, bool ef)
{
	struct nv50_disp_priv *priv = (void *)nouveau_disp(outp);
>>>>>>> v3.18
	const u32 soff = nvd0_sor_soff(outp);
	const u32 loff = nvd0_sor_loff(outp);
	u32 dpctrl = 0x00000000;
	u32 clksor = 0x00000000;
<<<<<<< HEAD
<<<<<<< HEAD
	u32 lane = 0;
	int i;

	clksor |= link_bw << 18;
	dpctrl |= ((1 << link_nr) - 1) << 16;
	if (enh_frame)
		dpctrl |= 0x00004000;

	for (i = 0; i < link_nr; i++)
		lane |= 1 << (nvd0_sor_dp_lane_map(priv, i) >> 3);

	nv_mask(priv, 0x612300 + soff, 0x007c0000, clksor);
	nv_mask(priv, 0x61c10c + loff, 0x001f4000, dpctrl);
	nv_mask(priv, 0x61c130 + loff, 0x0000000f, lane);
=======
=======
>>>>>>> v3.18

	clksor |= bw << 18;
	dpctrl |= ((1 << nr) - 1) << 16;
	if (ef)
		dpctrl |= 0x00004000;

	nv_mask(priv, 0x612300 + soff, 0x007c0000, clksor);
	nv_mask(priv, 0x61c10c + loff, 0x001f4000, dpctrl);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
}

static int
<<<<<<< HEAD
<<<<<<< HEAD
nvd0_sor_dp_drv_ctl(struct nouveau_disp *disp, struct dcb_output *outp,
		    int head, int lane, int swing, int preem)
{
	struct nouveau_bios *bios = nouveau_bios(disp);
	struct nv50_disp_priv *priv = (void *)disp;
	const u32 loff = nvd0_sor_loff(outp);
	u32 addr, shift = nvd0_sor_dp_lane_map(priv, lane);
=======
=======
>>>>>>> v3.18
nvd0_sor_dp_drv_ctl(struct nvkm_output_dp *outp, int ln, int vs, int pe, int pc)
{
	struct nv50_disp_priv *priv = (void *)nouveau_disp(outp);
	struct nouveau_bios *bios = nouveau_bios(priv);
	const u32 shift = nvd0_sor_dp_lane_map(priv, ln);
	const u32 loff = nvd0_sor_loff(outp);
	u32 addr, data[4];
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	u8  ver, hdr, cnt, len;
	struct nvbios_dpout info;
	struct nvbios_dpcfg ocfg;

<<<<<<< HEAD
<<<<<<< HEAD
	addr = nvbios_dpout_match(bios, outp->hasht, outp->hashm,
=======
	addr = nvbios_dpout_match(bios, outp->base.info.hasht,
					outp->base.info.hashm,
>>>>>>> v3.18
=======
	addr = nvbios_dpout_match(bios, outp->base.info.hasht,
					outp->base.info.hashm,
>>>>>>> v3.18
				 &ver, &hdr, &cnt, &len, &info);
	if (!addr)
		return -ENODEV;

<<<<<<< HEAD
<<<<<<< HEAD
	addr = nvbios_dpcfg_match(bios, addr, 0, swing, preem,
=======
	addr = nvbios_dpcfg_match(bios, addr, pc, vs, pe,
>>>>>>> v3.18
=======
	addr = nvbios_dpcfg_match(bios, addr, pc, vs, pe,
>>>>>>> v3.18
				 &ver, &hdr, &cnt, &len, &ocfg);
	if (!addr)
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	nv_mask(priv, 0x61c118 + loff, 0x000000ff << shift, ocfg.drv << shift);
	nv_mask(priv, 0x61c120 + loff, 0x000000ff << shift, ocfg.pre << shift);
	nv_mask(priv, 0x61c130 + loff, 0x0000ff00, ocfg.unk << 8);
	nv_mask(priv, 0x61c13c + loff, 0x00000000, 0x00000000);
	return 0;
}

const struct nouveau_dp_func
nvd0_sor_dp_func = {
	.pattern = nvd0_sor_dp_pattern,
=======
=======
>>>>>>> v3.18
	data[0] = nv_rd32(priv, 0x61c118 + loff) & ~(0x000000ff << shift);
	data[1] = nv_rd32(priv, 0x61c120 + loff) & ~(0x000000ff << shift);
	data[2] = nv_rd32(priv, 0x61c130 + loff);
	if ((data[2] & 0x0000ff00) < (ocfg.tx_pu << 8) || ln == 0)
		data[2] = (data[2] & ~0x0000ff00) | (ocfg.tx_pu << 8);
	nv_wr32(priv, 0x61c118 + loff, data[0] | (ocfg.dc << shift));
	nv_wr32(priv, 0x61c120 + loff, data[1] | (ocfg.pe << shift));
	nv_wr32(priv, 0x61c130 + loff, data[2] | (ocfg.tx_pu << 8));
	data[3] = nv_rd32(priv, 0x61c13c + loff) & ~(0x000000ff << shift);
	nv_wr32(priv, 0x61c13c + loff, data[3] | (ocfg.pc << shift));
	return 0;
}

struct nvkm_output_dp_impl
nvd0_sor_dp_impl = {
	.base.base.handle = DCB_OUTPUT_DP,
	.base.base.ofuncs = &(struct nouveau_ofuncs) {
		.ctor = _nvkm_output_dp_ctor,
		.dtor = _nvkm_output_dp_dtor,
		.init = _nvkm_output_dp_init,
		.fini = _nvkm_output_dp_fini,
	},
	.pattern = nvd0_sor_dp_pattern,
	.lnk_pwr = nv94_sor_dp_lnk_pwr,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	.lnk_ctl = nvd0_sor_dp_lnk_ctl,
	.drv_ctl = nvd0_sor_dp_drv_ctl,
};
