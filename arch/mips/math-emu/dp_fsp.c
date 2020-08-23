/* IEEE754 floating point arithmetic
 * double precision: common utilities
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


#include "ieee754dp.h"

ieee754dp ieee754dp_fsp(ieee754sp x)
=======
=======
>>>>>>> v3.18
 *  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
 */

#include "ieee754sp.h"
#include "ieee754dp.h"

union ieee754dp ieee754dp_fsp(union ieee754sp x)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	COMPXSP;

	EXPLODEXSP;

<<<<<<< HEAD
<<<<<<< HEAD
	CLEARCX;
=======
	ieee754_clearcx();
>>>>>>> v3.18
=======
	ieee754_clearcx();
>>>>>>> v3.18

	FLUSHXSP;

	switch (xc) {
	case IEEE754_CLASS_SNAN:
<<<<<<< HEAD
<<<<<<< HEAD
		SETCX(IEEE754_INVALID_OPERATION);
		return ieee754dp_nanxcpt(ieee754dp_indef(), "fsp");
=======
		ieee754_setcx(IEEE754_INVALID_OPERATION);
		return ieee754dp_nanxcpt(ieee754dp_indef());

>>>>>>> v3.18
=======
		ieee754_setcx(IEEE754_INVALID_OPERATION);
		return ieee754dp_nanxcpt(ieee754dp_indef());

>>>>>>> v3.18
	case IEEE754_CLASS_QNAN:
		return ieee754dp_nanxcpt(builddp(xs,
						 DP_EMAX + 1 + DP_EBIAS,
						 ((u64) xm
<<<<<<< HEAD
<<<<<<< HEAD
						  << (DP_MBITS -
						      SP_MBITS))), "fsp",
					 x);
	case IEEE754_CLASS_INF:
		return ieee754dp_inf(xs);
	case IEEE754_CLASS_ZERO:
		return ieee754dp_zero(xs);
	case IEEE754_CLASS_DNORM:
		/* normalize */
		while ((xm >> SP_MBITS) == 0) {
=======
=======
>>>>>>> v3.18
						  << (DP_FBITS -
						      SP_FBITS))));
	case IEEE754_CLASS_INF:
		return ieee754dp_inf(xs);

	case IEEE754_CLASS_ZERO:
		return ieee754dp_zero(xs);

	case IEEE754_CLASS_DNORM:
		/* normalize */
		while ((xm >> SP_FBITS) == 0) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			xm <<= 1;
			xe--;
		}
		break;
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
	case IEEE754_CLASS_NORM:
		break;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	/* CAN'T possibly overflow,underflow, or need rounding
=======
	/*
	 * Can't possibly overflow,underflow, or need rounding
>>>>>>> v3.18
=======
	/*
	 * Can't possibly overflow,underflow, or need rounding
>>>>>>> v3.18
	 */

	/* drop the hidden bit */
	xm &= ~SP_HIDDEN_BIT;

	return builddp(xs, xe + DP_EBIAS,
<<<<<<< HEAD
<<<<<<< HEAD
		       (u64) xm << (DP_MBITS - SP_MBITS));
=======
		       (u64) xm << (DP_FBITS - SP_FBITS));
>>>>>>> v3.18
=======
		       (u64) xm << (DP_FBITS - SP_FBITS));
>>>>>>> v3.18
}
