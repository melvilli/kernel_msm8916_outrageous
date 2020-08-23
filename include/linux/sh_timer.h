#ifndef __SH_TIMER_H__
#define __SH_TIMER_H__

struct sh_timer_config {
<<<<<<< HEAD
	char *name;
	long channel_offset;
	int timer_bit;
	unsigned long clockevent_rating;
	unsigned long clocksource_rating;
=======
	unsigned int channels_mask;
>>>>>>> v3.18
};

#endif /* __SH_TIMER_H__ */
