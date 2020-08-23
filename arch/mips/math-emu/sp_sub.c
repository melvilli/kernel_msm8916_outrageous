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

ieee754sp ieee754sp_sub(ieee754sp x, ieee754sp y)
{
=======
 *  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
 */

#include "ieee754sp.h"

union ieee754sp ieee754sp_sub(union ieee754sp x, union ieee754sp y)
{
	int s;

>>>>>>> v3.18
	COMPXSP;
	COMPYSP;

	EXPLODEXSP;
	EXPLODEYSP;

<<<<<<< HEAD
	CLEARCX;
=======
	ieee754_clearcx();
>>>>>>> v3.18

	FLUSHXSP;
	FLUSHYSP;

	switch (CLPAIR(xc, yc)) {
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_QNAN):
	case CLPAIR(IEEE754_CLASS_QNAN, IEEE754_CLASS_SNAN):
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_SNAN):
	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_SNAN):
	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_SNAN):
	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_SNAN):
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_SNAN):
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_NORM):
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_DNORM):
	case CLPAIR(IEEE754_CLASS_SNAN, IEEE754_CLASS_INF):
<<<<<<< HEAD
		SETCX(IEEE754_INVALID_OPERATION);
		return ieee754sp_nanxcpt(ieee754sp_indef(), "sub", x, y);
=======
		ieee754_setcx(IEEE754_INVALID_OPERATION);
		return ieee754sp_nanxcpt(ieee754sp_indef());
>>>>>>> v3.18

	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_QNAN):
	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_QNAN):
	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_QNAN):
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_QNAN):
		return y;

	case CLPAIR(IEEE754_CLASS_QNAN, IEEE754_CLASS_QNAN):
	case CLPAIR(IEEE754_CLASS_QNAN, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_QNAN, IEEE754_CLASS_NORM):
	case CLPAIR(IEEE754_CLASS_QNAN, IEEE754_CLASS_DNORM):
	case CLPAIR(IEEE754_CLASS_QNAN, IEEE754_CLASS_INF):
		return x;


<<<<<<< HEAD
		/* Infinity handling
		 */

	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_INF):
		if (xs != ys)
			return x;
		SETCX(IEEE754_INVALID_OPERATION);
		return ieee754sp_xcpt(ieee754sp_indef(), "sub", x, y);
=======
	/*
	 * Infinity handling
	 */
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_INF):
		if (xs != ys)
			return x;
		ieee754_setcx(IEEE754_INVALID_OPERATION);
		return ieee754sp_indef();
>>>>>>> v3.18

	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_INF):
	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_INF):
	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_INF):
		return ieee754sp_inf(ys ^ 1);

	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_NORM):
	case CLPAIR(IEEE754_CLASS_INF, IEEE754_CLASS_DNORM):
		return x;

<<<<<<< HEAD
		/* Zero handling
		 */

=======
	/*
	 * Zero handling
	 */
>>>>>>> v3.18
	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_ZERO):
		if (xs != ys)
			return x;
		else
<<<<<<< HEAD
			return ieee754sp_zero(ieee754_csr.rm ==
					      IEEE754_RD);
=======
			return ieee754sp_zero(ieee754_csr.rm == FPU_CSR_RD);
>>>>>>> v3.18

	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_ZERO):
	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_ZERO):
		return x;

	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_NORM):
	case CLPAIR(IEEE754_CLASS_ZERO, IEEE754_CLASS_DNORM):
		/* quick fix up */
<<<<<<< HEAD
		DPSIGN(y) ^= 1;
=======
		SPSIGN(y) ^= 1;
>>>>>>> v3.18
		return y;

	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_DNORM):
		SPDNORMX;

	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_DNORM):
		SPDNORMY;
		break;

	case CLPAIR(IEEE754_CLASS_DNORM, IEEE754_CLASS_NORM):
		SPDNORMX;
		break;

	case CLPAIR(IEEE754_CLASS_NORM, IEEE754_CLASS_NORM):
		break;
	}
	/* flip sign of y and handle as add */
	ys ^= 1;

	assert(xm & SP_HIDDEN_BIT);
	assert(ym & SP_HIDDEN_BIT);


	/* provide guard,round and stick bit space */
	xm <<= 3;
	ym <<= 3;

	if (xe > ye) {
<<<<<<< HEAD
		/* have to shift y fraction right to align
		 */
		int s = xe - ye;
		SPXSRSYn(s);
	} else if (ye > xe) {
		/* have to shift x fraction right to align
		 */
		int s = ye - xe;
=======
		/*
		 * have to shift y fraction right to align
		 */
		s = xe - ye;
		SPXSRSYn(s);
	} else if (ye > xe) {
		/*
		 * have to shift x fraction right to align
		 */
		s = ye - xe;
>>>>>>> v3.18
		SPXSRSXn(s);
	}
	assert(xe == ye);
	assert(xe <= SP_EMAX);

	if (xs == ys) {
		/* generate 28 bit result of adding two 27 bit numbers
		 */
		xm = xm + ym;
		xe = xe;
		xs = xs;

<<<<<<< HEAD
		if (xm >> (SP_MBITS + 1 + 3)) { /* carry out */
=======
		if (xm >> (SP_FBITS + 1 + 3)) { /* carry out */
>>>>>>> v3.18
			SPXSRSX1();	/* shift preserving sticky */
		}
	} else {
		if (xm >= ym) {
			xm = xm - ym;
			xe = xe;
			xs = xs;
		} else {
			xm = ym - xm;
			xe = xe;
			xs = ys;
		}
		if (xm == 0) {
<<<<<<< HEAD
			if (ieee754_csr.rm == IEEE754_RD)
=======
			if (ieee754_csr.rm == FPU_CSR_RD)
>>>>>>> v3.18
				return ieee754sp_zero(1);	/* round negative inf. => sign = -1 */
			else
				return ieee754sp_zero(0);	/* other round modes   => sign = 1 */
		}
		/* normalize to rounding precision
		 */
<<<<<<< HEAD
		while ((xm >> (SP_MBITS + 3)) == 0) {
=======
		while ((xm >> (SP_FBITS + 3)) == 0) {
>>>>>>> v3.18
			xm <<= 1;
			xe--;
		}
	}
<<<<<<< HEAD
	SPNORMRET2(xs, xe, xm, "sub", x, y);
=======

	return ieee754sp_format(xs, xe, xm);
>>>>>>> v3.18
}
