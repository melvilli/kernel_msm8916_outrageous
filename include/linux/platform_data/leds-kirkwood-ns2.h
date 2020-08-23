/*
<<<<<<< HEAD
 * arch/arm/mach-kirkwood/include/mach/leds-ns2.h
 *
=======
>>>>>>> v3.18
 * Platform data structure for Network Space v2 LED driver
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

<<<<<<< HEAD
#ifndef __MACH_LEDS_NS2_H
#define __MACH_LEDS_NS2_H
=======
#ifndef __LEDS_KIRKWOOD_NS2_H
#define __LEDS_KIRKWOOD_NS2_H
>>>>>>> v3.18

struct ns2_led {
	const char	*name;
	const char	*default_trigger;
	unsigned	cmd;
	unsigned	slow;
};

struct ns2_led_platform_data {
	int		num_leds;
	struct ns2_led	*leds;
};

<<<<<<< HEAD
#endif /* __MACH_LEDS_NS2_H */
=======
#endif /* __LEDS_KIRKWOOD_NS2_H */
>>>>>>> v3.18
