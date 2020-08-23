/*
<<<<<<< HEAD
<<<<<<< HEAD
 * arch/arm/plat-orion/include/plat/orion_nand.h
 *
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

<<<<<<< HEAD
<<<<<<< HEAD
#ifndef __PLAT_ORION_NAND_H
#define __PLAT_ORION_NAND_H
=======
#ifndef __MTD_ORION_NAND_H
#define __MTD_ORION_NAND_H
>>>>>>> v3.18
=======
#ifndef __MTD_ORION_NAND_H
#define __MTD_ORION_NAND_H
>>>>>>> v3.18

/*
 * Device bus NAND private data
 */
struct orion_nand_data {
	struct mtd_partition *parts;
	int (*dev_ready)(struct mtd_info *mtd);
	u32 nr_parts;
	u8 ale;		/* address line number connected to ALE */
	u8 cle;		/* address line number connected to CLE */
	u8 width;	/* buswidth */
	u8 chip_delay;
};


#endif
