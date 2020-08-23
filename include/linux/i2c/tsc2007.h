#ifndef __LINUX_I2C_TSC2007_H
#define __LINUX_I2C_TSC2007_H

/* linux/i2c/tsc2007.h */

struct tsc2007_platform_data {
	u16	model;				/* 2007. */
	u16	x_plate_ohms;	/* must be non-zero value */
	u16	max_rt; /* max. resistance above which samples are ignored */
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long poll_delay; /* delay (in ms) after pen-down event
				     before polling starts */
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	unsigned long poll_period; /* time (in ms) between samples */
	int	fuzzx; /* fuzz factor for X, Y and pressure axes */
	int	fuzzy;
	int	fuzzz;
<<<<<<< HEAD
<<<<<<< HEAD
	u16	min_x;
	u16	min_y;
	u16	max_x;
	u16	max_y;
	unsigned long irq_flags;
	bool	invert_x;
	bool	invert_y;
	bool	invert_z1;
	bool	invert_z2;

	int	(*get_pendown_state)(void);
	void	(*clear_penirq)(void);		/* If needed, clear 2nd level
						   interrupt source */
	int	(*init_platform_hw)(void);
	void	(*exit_platform_hw)(void);
	int	(*power_shutdown)(bool);
=======
=======
>>>>>>> v3.18

	int	(*get_pendown_state)(struct device *);
	/* If needed, clear 2nd level interrupt source */
	void	(*clear_penirq)(void);
	int	(*init_platform_hw)(void);
	void	(*exit_platform_hw)(void);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
};

#endif
