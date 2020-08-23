/* IEEE754 floating point arithmetic
 * single precision
 */
/*
 * MIPS floating point support
 * Copyright (C) 1994-2000 Algorithmics Ltd.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * ########################################################################
 *
=======
>>>>>>> v3.18
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
<<<<<<< HEAD
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * ########################################################################
 */


#include "ieee754sp.h"

ieee754sp ieee754sp_fdp(ieee754dp x)
{
	COMPXDP;
	ieee754sp nan;

	EXPLODEXDP;

	CLEARCX;
=======
=======
>>>>>>> v3.18
 *  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
 */

#include "ieee754sp.h"
#include "ieee754dp.h"

union ieee754sp ieee754sp_fdp(union ieee754dp x)
{
	u32 rm;

	COMPXDP;
	union ieee754sp nan;

	EXPLODEXDP;

	ieee754_clearcx();
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	FLUSHXDP;

	switch (xc) {
	case IEEE754_CLASS_SNAN:
<<<<<<< HEAD
<<<<<<< HEAD
		SETCX(IEEE754_INVALID_OPERATION);
		return ieee754sp_nanxcpt(ieee754sp_indef(), "fdp");
	case IEEE754_CLASS_QNAN:
		nan = buildsp(xs, SP_EMAX + 1 + SP_EBIAS, (u32)
				(xm >> (DP_MBITS - SP_MBITS)));
		if (!ieee754sp_isnan(nan))
			nan = ieee754sp_indef();
		return ieee754sp_nanxcpt(nan, "fdp", x);
	case IEEE754_CLASS_INF:
		return ieee754sp_inf(xs);
	case IEEE754_CLASS_ZERO:
		return ieee754sp_zero(xs);
	case IEEE754_CLASS_DNORM:
		/* can't possibly be sp representable */
		SETCX(IEEE754_UNDERFLOW);
		SETCX(IEEE754_INEXACT);
		if ((ieee754_csr.rm == IEEE754_RU && !xs) ||
				(ieee754_csr.rm == IEEE754_RD && xs))
			return ieee754sp_xcpt(ieee754sp_mind(xs), "fdp", x);
		return ieee754sp_xcpt(ieee754sp_zero(xs), "fdp", x);
=======
=======
>>>>>>> v3.18
		ieee754_setcx(IEEE754_INVALID_OPERATION);
		return ieee754sp_nanxcpt(ieee754sp_indef());

	case IEEE754_CLASS_QNAN:
		nan = buildsp(xs, SP_EMAX + 1 + SP_EBIAS, (u32)
				(xm >> (DP_FBITS - SP_FBITS)));
		if (!ieee754sp_isnan(nan))
			nan = ieee754sp_indef();
		return ieee754sp_nanxcpt(nan);

	case IEEE754_CLASS_INF:
		return ieee754sp_inf(xs);

	case IEEE754_CLASS_ZERO:
		return ieee754sp_zero(xs);

	case IEEE754_CLASS_DNORM:
		/* can't possibly be sp representable */
		ieee754_setcx(IEEE754_UNDERFLOW);
		ieee754_setcx(IEEE754_INEXACT);
		if ((ieee754_csr.rm == FPU_CSR_RU && !xs) ||
				(ieee754_csr.rm == FPU_CSR_RD && xs))
			return ieee754sp_mind(xs);
		return ieee754sp_zero(xs);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	case IEEE754_CLASS_NORM:
		break;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	{
		u32 rm;

		/* convert from DP_MBITS to SP_MBITS+3 with sticky right shift
		 */
		rm = (xm >> (DP_MBITS - (SP_MBITS + 3))) |
		    ((xm << (64 - (DP_MBITS - (SP_MBITS + 3)))) != 0);

		SPNORMRET1(xs, xe, rm, "fdp", x);
	}
=======
=======
>>>>>>> v3.18
	/*
	 * Convert from DP_FBITS to SP_FBITS+3 with sticky right shift.
	 */
	rm = (xm >> (DP_FBITS - (SP_FBITS + 3))) |
	     ((xm << (64 - (DP_FBITS - (SP_FBITS + 3)))) != 0);

	return ieee754sp_format(xs, xe, rm);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}
