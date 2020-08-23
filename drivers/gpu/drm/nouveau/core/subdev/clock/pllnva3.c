/*
 * Copyright 2010 Red Hat Inc.
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

#include <subdev/clock.h>
#include <subdev/bios.h>
#include <subdev/bios/pll.h>

#include "pll.h"

int
<<<<<<< HEAD
<<<<<<< HEAD
nva3_pll_calc(struct nouveau_clock *clock, struct nvbios_pll *info,
=======
nva3_pll_calc(struct nouveau_subdev *subdev, struct nvbios_pll *info,
>>>>>>> v3.18
=======
nva3_pll_calc(struct nouveau_subdev *subdev, struct nvbios_pll *info,
>>>>>>> v3.18
	      u32 freq, int *pN, int *pfN, int *pM, int *P)
{
	u32 best_err = ~0, err;
	int M, lM, hM, N, fN;

	*P = info->vco1.max_freq / freq;
	if (*P > info->max_p)
		*P = info->max_p;
	if (*P < info->min_p)
		*P = info->min_p;

	lM = (info->refclk + info->vco1.max_inputfreq) / info->vco1.max_inputfreq;
	lM = max(lM, (int)info->vco1.min_m);
	hM = (info->refclk + info->vco1.min_inputfreq) / info->vco1.min_inputfreq;
	hM = min(hM, (int)info->vco1.max_m);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	lM = min(lM, hM);
>>>>>>> v3.18
=======
	lM = min(lM, hM);
>>>>>>> v3.18

	for (M = lM; M <= hM; M++) {
		u32 tmp = freq * *P * M;
		N  = tmp / info->refclk;
		fN = tmp % info->refclk;
<<<<<<< HEAD
<<<<<<< HEAD
		if (!pfN && fN >= info->refclk / 2)
			N++;
=======
=======
>>>>>>> v3.18

		if (!pfN) {
			if (fN >= info->refclk / 2)
				N++;
		} else {
			if (fN <  info->refclk / 2)
				N--;
			fN = tmp - (N * info->refclk);
		}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

		if (N < info->vco1.min_n)
			continue;
		if (N > info->vco1.max_n)
			break;

		err = abs(freq - (info->refclk * N / M / *P));
		if (err < best_err) {
			best_err = err;
			*pN = N;
			*pM = M;
		}

		if (pfN) {
<<<<<<< HEAD
<<<<<<< HEAD
			*pfN = (((fN << 13) / info->refclk) - 4096) & 0xffff;
=======
			*pfN = ((fN << 13) + info->refclk / 2) / info->refclk;
			*pfN = (*pfN - 4096) & 0xffff;
>>>>>>> v3.18
=======
			*pfN = ((fN << 13) + info->refclk / 2) / info->refclk;
			*pfN = (*pfN - 4096) & 0xffff;
>>>>>>> v3.18
			return freq;
		}
	}

	if (unlikely(best_err == ~0)) {
<<<<<<< HEAD
<<<<<<< HEAD
		nv_error(clock, "unable to find matching pll values\n");
=======
		nv_error(subdev, "unable to find matching pll values\n");
>>>>>>> v3.18
=======
		nv_error(subdev, "unable to find matching pll values\n");
>>>>>>> v3.18
		return -EINVAL;
	}

	return info->refclk * *pN / *pM / *P;
}
