/* IEEE754 floating point arithmetic
 * single precision
 */
/*
 * MIPS floating point support
 * Copyright (C) 1994-2000 Algorithmics Ltd.
 *
<<<<<<< HEAD
 * ########################################################################
 *
=======
>>>>>>> v3.18
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
<<<<<<< HEAD
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * ########################################################################
 */


#include "ieee754sp.h"

int ieee754sp_class(ieee754sp x)
=======
 *  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
 */

#include <linux/compiler.h>

#include "ieee754sp.h"

int ieee754sp_class(union ieee754sp x)
>>>>>>> v3.18
{
	COMPXSP;
	EXPLODEXSP;
	return xc;
}

<<<<<<< HEAD
int ieee754sp_isnan(ieee754sp x)
=======
int ieee754sp_isnan(union ieee754sp x)
>>>>>>> v3.18
{
	return ieee754sp_class(x) >= IEEE754_CLASS_SNAN;
}

<<<<<<< HEAD
int ieee754sp_issnan(ieee754sp x)
{
	assert(ieee754sp_isnan(x));
	return (SPMANT(x) & SP_MBIT(SP_MBITS-1));
}


ieee754sp ieee754sp_xcpt(ieee754sp r, const char *op, ...)
{
	struct ieee754xctx ax;

	if (!TSTX())
		return r;

	ax.op = op;
	ax.rt = IEEE754_RT_SP;
	ax.rv.sp = r;
	va_start(ax.ap, op);
	ieee754_xcpt(&ax);
	va_end(ax.ap);
	return ax.rv.sp;
}

ieee754sp ieee754sp_nanxcpt(ieee754sp r, const char *op, ...)
{
	struct ieee754xctx ax;

=======
static inline int ieee754sp_issnan(union ieee754sp x)
{
	assert(ieee754sp_isnan(x));
	return (SPMANT(x) & SP_MBIT(SP_FBITS-1));
}


union ieee754sp __cold ieee754sp_nanxcpt(union ieee754sp r)
{
>>>>>>> v3.18
	assert(ieee754sp_isnan(r));

	if (!ieee754sp_issnan(r))	/* QNAN does not cause invalid op !! */
		return r;

<<<<<<< HEAD
	if (!SETANDTESTCX(IEEE754_INVALID_OPERATION)) {
		/* not enabled convert to a quiet NaN */
		SPMANT(r) &= (~SP_MBIT(SP_MBITS-1));
=======
	if (!ieee754_setandtestcx(IEEE754_INVALID_OPERATION)) {
		/* not enabled convert to a quiet NaN */
		SPMANT(r) &= (~SP_MBIT(SP_FBITS-1));
>>>>>>> v3.18
		if (ieee754sp_isnan(r))
			return r;
		else
			return ieee754sp_indef();
	}

<<<<<<< HEAD
	ax.op = op;
	ax.rt = 0;
	ax.rv.sp = r;
	va_start(ax.ap, op);
	ieee754_xcpt(&ax);
	va_end(ax.ap);
	return ax.rv.sp;
}

ieee754sp ieee754sp_bestnan(ieee754sp x, ieee754sp y)
{
	assert(ieee754sp_isnan(x));
	assert(ieee754sp_isnan(y));

	if (SPMANT(x) > SPMANT(y))
		return x;
	else
		return y;
}


static unsigned get_rounding(int sn, unsigned xm)
=======
	return r;
}

static unsigned ieee754sp_get_rounding(int sn, unsigned xm)
>>>>>>> v3.18
{
	/* inexact must round of 3 bits
	 */
	if (xm & (SP_MBIT(3) - 1)) {
		switch (ieee754_csr.rm) {
<<<<<<< HEAD
		case IEEE754_RZ:
			break;
		case IEEE754_RN:
			xm += 0x3 + ((xm >> 3) & 1);
			/* xm += (xm&0x8)?0x4:0x3 */
			break;
		case IEEE754_RU:	/* toward +Infinity */
			if (!sn)	/* ?? */
				xm += 0x8;
			break;
		case IEEE754_RD:	/* toward -Infinity */
=======
		case FPU_CSR_RZ:
			break;
		case FPU_CSR_RN:
			xm += 0x3 + ((xm >> 3) & 1);
			/* xm += (xm&0x8)?0x4:0x3 */
			break;
		case FPU_CSR_RU:	/* toward +Infinity */
			if (!sn)	/* ?? */
				xm += 0x8;
			break;
		case FPU_CSR_RD:	/* toward -Infinity */
>>>>>>> v3.18
			if (sn) /* ?? */
				xm += 0x8;
			break;
		}
	}
	return xm;
}


/* generate a normal/denormal number with over,under handling
 * sn is sign
 * xe is an unbiased exponent
 * xm is 3bit extended precision value.
 */
<<<<<<< HEAD
ieee754sp ieee754sp_format(int sn, int xe, unsigned xm)
{
	assert(xm);		/* we don't gen exact zeros (probably should) */

	assert((xm >> (SP_MBITS + 1 + 3)) == 0);	/* no execess */
=======
union ieee754sp ieee754sp_format(int sn, int xe, unsigned xm)
{
	assert(xm);		/* we don't gen exact zeros (probably should) */

	assert((xm >> (SP_FBITS + 1 + 3)) == 0);	/* no execess */
>>>>>>> v3.18
	assert(xm & (SP_HIDDEN_BIT << 3));

	if (xe < SP_EMIN) {
		/* strip lower bits */
		int es = SP_EMIN - xe;

		if (ieee754_csr.nod) {
<<<<<<< HEAD
			SETCX(IEEE754_UNDERFLOW);
			SETCX(IEEE754_INEXACT);

			switch(ieee754_csr.rm) {
			case IEEE754_RN:
			case IEEE754_RZ:
				return ieee754sp_zero(sn);
			case IEEE754_RU:      /* toward +Infinity */
				if(sn == 0)
					return ieee754sp_min(0);
				else
					return ieee754sp_zero(1);
			case IEEE754_RD:      /* toward -Infinity */
				if(sn == 0)
=======
			ieee754_setcx(IEEE754_UNDERFLOW);
			ieee754_setcx(IEEE754_INEXACT);

			switch(ieee754_csr.rm) {
			case FPU_CSR_RN:
			case FPU_CSR_RZ:
				return ieee754sp_zero(sn);
			case FPU_CSR_RU:      /* toward +Infinity */
				if (sn == 0)
					return ieee754sp_min(0);
				else
					return ieee754sp_zero(1);
			case FPU_CSR_RD:      /* toward -Infinity */
				if (sn == 0)
>>>>>>> v3.18
					return ieee754sp_zero(0);
				else
					return ieee754sp_min(1);
			}
		}

<<<<<<< HEAD
		if (xe == SP_EMIN - 1
				&& get_rounding(sn, xm) >> (SP_MBITS + 1 + 3))
		{
			/* Not tiny after rounding */
			SETCX(IEEE754_INEXACT);
			xm = get_rounding(sn, xm);
=======
		if (xe == SP_EMIN - 1 &&
		    ieee754sp_get_rounding(sn, xm) >> (SP_FBITS + 1 + 3))
		{
			/* Not tiny after rounding */
			ieee754_setcx(IEEE754_INEXACT);
			xm = ieee754sp_get_rounding(sn, xm);
>>>>>>> v3.18
			xm >>= 1;
			/* Clear grs bits */
			xm &= ~(SP_MBIT(3) - 1);
			xe++;
<<<<<<< HEAD
		}
		else {
=======
		} else {
>>>>>>> v3.18
			/* sticky right shift es bits
			 */
			SPXSRSXn(es);
			assert((xm & (SP_HIDDEN_BIT << 3)) == 0);
			assert(xe == SP_EMIN);
		}
	}
	if (xm & (SP_MBIT(3) - 1)) {
<<<<<<< HEAD
		SETCX(IEEE754_INEXACT);
		if ((xm & (SP_HIDDEN_BIT << 3)) == 0) {
			SETCX(IEEE754_UNDERFLOW);
=======
		ieee754_setcx(IEEE754_INEXACT);
		if ((xm & (SP_HIDDEN_BIT << 3)) == 0) {
			ieee754_setcx(IEEE754_UNDERFLOW);
>>>>>>> v3.18
		}

		/* inexact must round of 3 bits
		 */
<<<<<<< HEAD
		xm = get_rounding(sn, xm);
		/* adjust exponent for rounding add overflowing
		 */
		if (xm >> (SP_MBITS + 1 + 3)) {
=======
		xm = ieee754sp_get_rounding(sn, xm);
		/* adjust exponent for rounding add overflowing
		 */
		if (xm >> (SP_FBITS + 1 + 3)) {
>>>>>>> v3.18
			/* add causes mantissa overflow */
			xm >>= 1;
			xe++;
		}
	}
	/* strip grs bits */
	xm >>= 3;

<<<<<<< HEAD
	assert((xm >> (SP_MBITS + 1)) == 0);	/* no execess */
	assert(xe >= SP_EMIN);

	if (xe > SP_EMAX) {
		SETCX(IEEE754_OVERFLOW);
		SETCX(IEEE754_INEXACT);
		/* -O can be table indexed by (rm,sn) */
		switch (ieee754_csr.rm) {
		case IEEE754_RN:
			return ieee754sp_inf(sn);
		case IEEE754_RZ:
			return ieee754sp_max(sn);
		case IEEE754_RU:	/* toward +Infinity */
=======
	assert((xm >> (SP_FBITS + 1)) == 0);	/* no execess */
	assert(xe >= SP_EMIN);

	if (xe > SP_EMAX) {
		ieee754_setcx(IEEE754_OVERFLOW);
		ieee754_setcx(IEEE754_INEXACT);
		/* -O can be table indexed by (rm,sn) */
		switch (ieee754_csr.rm) {
		case FPU_CSR_RN:
			return ieee754sp_inf(sn);
		case FPU_CSR_RZ:
			return ieee754sp_max(sn);
		case FPU_CSR_RU:	/* toward +Infinity */
>>>>>>> v3.18
			if (sn == 0)
				return ieee754sp_inf(0);
			else
				return ieee754sp_max(1);
<<<<<<< HEAD
		case IEEE754_RD:	/* toward -Infinity */
=======
		case FPU_CSR_RD:	/* toward -Infinity */
>>>>>>> v3.18
			if (sn == 0)
				return ieee754sp_max(0);
			else
				return ieee754sp_inf(1);
		}
	}
	/* gen norm/denorm/zero */

	if ((xm & SP_HIDDEN_BIT) == 0) {
		/* we underflow (tiny/zero) */
		assert(xe == SP_EMIN);
		if (ieee754_csr.mx & IEEE754_UNDERFLOW)
<<<<<<< HEAD
			SETCX(IEEE754_UNDERFLOW);
		return buildsp(sn, SP_EMIN - 1 + SP_EBIAS, xm);
	} else {
		assert((xm >> (SP_MBITS + 1)) == 0);	/* no execess */
=======
			ieee754_setcx(IEEE754_UNDERFLOW);
		return buildsp(sn, SP_EMIN - 1 + SP_EBIAS, xm);
	} else {
		assert((xm >> (SP_FBITS + 1)) == 0);	/* no execess */
>>>>>>> v3.18
		assert(xm & SP_HIDDEN_BIT);

		return buildsp(sn, xe + SP_EBIAS, xm & ~SP_HIDDEN_BIT);
	}
}
