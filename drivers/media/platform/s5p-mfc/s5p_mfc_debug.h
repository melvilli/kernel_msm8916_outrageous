/*
<<<<<<< HEAD
<<<<<<< HEAD
 * drivers/media/platform/samsung/mfc5/s5p_mfc_debug.h
=======
 * drivers/media/platform/s5p-mfc/s5p_mfc_debug.h
>>>>>>> v3.18
=======
 * drivers/media/platform/s5p-mfc/s5p_mfc_debug.h
>>>>>>> v3.18
 *
 * Header file for Samsung MFC (Multi Function Codec - FIMV) driver
 * This file contains debug macros
 *
 * Kamil Debski, Copyright (c) 2011 Samsung Electronics
 * http://www.samsung.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef S5P_MFC_DEBUG_H_
#define S5P_MFC_DEBUG_H_

#define DEBUG

#ifdef DEBUG
<<<<<<< HEAD
<<<<<<< HEAD
extern int debug;

#define mfc_debug(level, fmt, args...)				\
	do {							\
		if (debug >= level)				\
=======
=======
>>>>>>> v3.18
extern int mfc_debug_level;

#define mfc_debug(level, fmt, args...)				\
	do {							\
		if (mfc_debug_level >= level)			\
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			printk(KERN_DEBUG "%s:%d: " fmt,	\
				__func__, __LINE__, ##args);	\
	} while (0)
#else
#define mfc_debug(level, fmt, args...)
#endif

#define mfc_debug_enter() mfc_debug(5, "enter\n")
#define mfc_debug_leave() mfc_debug(5, "leave\n")

#define mfc_err(fmt, args...)				\
	do {						\
		printk(KERN_ERR "%s:%d: " fmt,		\
		       __func__, __LINE__, ##args);	\
	} while (0)

#define mfc_info(fmt, args...)				\
	do {						\
		printk(KERN_INFO "%s:%d: " fmt,		\
		       __func__, __LINE__, ##args);	\
	} while (0)

#endif /* S5P_MFC_DEBUG_H_ */
