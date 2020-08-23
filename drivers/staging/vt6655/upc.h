/*
 * Copyright (c) 1996, 2003 VIA Networking Technologies, Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * File: upc.h
 *
 * Purpose: Macros to access device
 *
 * Author: Tevin Chen
 *
 * Date: Mar 17, 1997
 *
 */

#ifndef __UPC_H__
#define __UPC_H__

#include "device.h"
#include "ttype.h"

/*---------------------  Export Definitions -------------------------*/

//
<<<<<<< HEAD
<<<<<<< HEAD
//  For IO mapped
//

#ifdef IO_MAP

#define VNSvInPortB(dwIOAddress, pbyData)	\
do {						\
	*(pbyData) = inb(dwIOAddress);		\
} while (0)

#define VNSvInPortW(dwIOAddress, pwData)	\
do {						\
	*(pwData) = inw(dwIOAddress);		\
} while (0)

#define VNSvInPortD(dwIOAddress, pdwData)	\
do {						\
	*(pdwData) = inl(dwIOAddress);		\
} while (0)

#define VNSvOutPortB(dwIOAddress, byData)	\
	outb(byData, dwIOAddress)

#define VNSvOutPortW(dwIOAddress, wData)	\
	outw(wData, dwIOAddress)

#define VNSvOutPortD(dwIOAddress, dwData)	\
	outl(dwData, dwIOAddress)

#else

//
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
//  For memory mapped IO
//

#define VNSvInPortB(dwIOAddress, pbyData)				\
do {									\
<<<<<<< HEAD
<<<<<<< HEAD
	volatile unsigned char *pbyAddr = (unsigned char *)(dwIOAddress); \
	*(pbyData) = readb(pbyAddr);					\
=======
	*(pbyData) = ioread8(dwIOAddress);				\
>>>>>>> v3.18
=======
	*(pbyData) = ioread8(dwIOAddress);				\
>>>>>>> v3.18
} while (0)

#define VNSvInPortW(dwIOAddress, pwData)				\
do {									\
<<<<<<< HEAD
<<<<<<< HEAD
	volatile unsigned short *pwAddr = (unsigned short *)(dwIOAddress); \
	*(pwData) = readw(pwAddr);					\
=======
	*(pwData) = ioread16(dwIOAddress);				\
>>>>>>> v3.18
=======
	*(pwData) = ioread16(dwIOAddress);				\
>>>>>>> v3.18
} while (0)

#define VNSvInPortD(dwIOAddress, pdwData)				\
do {									\
<<<<<<< HEAD
<<<<<<< HEAD
	volatile unsigned long *pdwAddr = (unsigned long *)(dwIOAddress); \
	*(pdwData) = readl(pdwAddr);					\
=======
	*(pdwData) = ioread32(dwIOAddress);				\
>>>>>>> v3.18
=======
	*(pdwData) = ioread32(dwIOAddress);				\
>>>>>>> v3.18
} while (0)

#define VNSvOutPortB(dwIOAddress, byData)				\
do {									\
<<<<<<< HEAD
<<<<<<< HEAD
	volatile unsigned char *pbyAddr = (unsigned char *)(dwIOAddress); \
	writeb((unsigned char)byData, pbyAddr);				\
=======
	iowrite8((u8)byData, dwIOAddress);				\
>>>>>>> v3.18
=======
	iowrite8((u8)byData, dwIOAddress);				\
>>>>>>> v3.18
} while (0)

#define VNSvOutPortW(dwIOAddress, wData)				\
do {									\
<<<<<<< HEAD
<<<<<<< HEAD
	volatile unsigned short *pwAddr = ((unsigned short *)(dwIOAddress)); \
	writew((unsigned short)wData, pwAddr);				\
=======
	iowrite16((u16)wData, dwIOAddress);				\
>>>>>>> v3.18
=======
	iowrite16((u16)wData, dwIOAddress);				\
>>>>>>> v3.18
} while (0)

#define VNSvOutPortD(dwIOAddress, dwData)				\
do {									\
<<<<<<< HEAD
<<<<<<< HEAD
	volatile unsigned long *pdwAddr = (unsigned long *)(dwIOAddress); \
	writel((unsigned long)dwData, pdwAddr);				\
} while (0)

#endif

//
// ALWAYS IO-Mapped IO when in 16-bit/32-bit environment
//
#define PCBvInPortB(dwIOAddress, pbyData)	\
do {						\
	*(pbyData) = inb(dwIOAddress);		\
} while (0)

#define PCBvInPortW(dwIOAddress, pwData)	\
do {						\
	*(pwData) = inw(dwIOAddress);		\
} while (0)

#define PCBvInPortD(dwIOAddress, pdwData)	\
do {						\
	*(pdwData) = inl(dwIOAddress);		\
} while (0)

#define PCBvOutPortB(dwIOAddress, byData)	\
	outb(byData, dwIOAddress)

#define PCBvOutPortW(dwIOAddress, wData)	\
	outw(wData, dwIOAddress)

#define PCBvOutPortD(dwIOAddress, dwData)	\
	outl(dwData, dwIOAddress)

=======
	iowrite32((u32)dwData, dwIOAddress);				\
} while (0)

>>>>>>> v3.18
=======
	iowrite32((u32)dwData, dwIOAddress);				\
} while (0)

>>>>>>> v3.18
#define PCAvDelayByIO(uDelayUnit)				\
do {								\
	unsigned char byData;					\
	unsigned long ii;					\
								\
	if (uDelayUnit <= 50) {					\
		udelay(uDelayUnit);				\
	} else {						\
		for (ii = 0; ii < (uDelayUnit); ii++)		\
			byData = inb(0x61);			\
	}							\
} while (0)

/*---------------------  Export Classes  ----------------------------*/

/*---------------------  Export Variables  --------------------------*/

/*---------------------  Export Functions  --------------------------*/

#endif // __UPC_H__
