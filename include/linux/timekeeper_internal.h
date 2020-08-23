/*
 * You SHOULD NOT be including this unless you're vsyscall
 * handling code or timekeeping internal code!
 */

#ifndef _LINUX_TIMEKEEPER_INTERNAL_H
#define _LINUX_TIMEKEEPER_INTERNAL_H

#include <linux/clocksource.h>
#include <linux/jiffies.h>
#include <linux/time.h>

<<<<<<< HEAD
/* Structure holding internal timekeeping values. */
struct timekeeper {
	/* Current clocksource used for timekeeping. */
	struct clocksource	*clock;
	/* NTP adjusted clock multiplier */
	u32			mult;
	/* The shift value of the current clocksource. */
	u32			shift;
	/* Number of clock cycles in one NTP interval. */
	cycle_t			cycle_interval;
	/* Last cycle value (also stored in clock->cycle_last) */
	cycle_t			cycle_last;
	/* Number of clock shifted nano seconds in one NTP interval. */
	u64			xtime_interval;
	/* shifted nano seconds left over when rounding cycle_interval */
	s64			xtime_remainder;
	/* Raw nano seconds accumulated per NTP interval. */
	u32			raw_interval;

	/* Current CLOCK_REALTIME time in seconds */
	u64			xtime_sec;
	/* Clock shifted nano seconds */
	u64			xtime_nsec;

	/* Difference between accumulated time and NTP time in ntp
	 * shifted nano seconds. */
	s64			ntp_error;
	/* Shift conversion between clock shifted nano seconds and
	 * ntp shifted nano seconds. */
	u32			ntp_error_shift;

	/*
	 * wall_to_monotonic is what we need to add to xtime (or xtime corrected
	 * for sub jiffie times) to get to monotonic time.  Monotonic is pegged
	 * at zero at system boot time, so wall_to_monotonic will be negative,
	 * however, we will ALWAYS keep the tv_nsec part positive so we can use
	 * the usual normalization.
	 *
	 * wall_to_monotonic is moved after resume from suspend for the
	 * monotonic time not to jump. We need to add total_sleep_time to
	 * wall_to_monotonic to get the real boot based time offset.
	 *
	 * - wall_to_monotonic is no longer the boot time, getboottime must be
	 * used instead.
	 */
	struct timespec		wall_to_monotonic;
	/* Offset clock monotonic -> clock realtime */
	ktime_t			offs_real;
	/* time spent in suspend */
	struct timespec		total_sleep_time;
	/* Offset clock monotonic -> clock boottime */
	ktime_t			offs_boot;
	/* The raw monotonic time for the CLOCK_MONOTONIC_RAW posix clock. */
	struct timespec		raw_time;
	/* The current UTC to TAI offset in seconds */
	s32			tai_offset;
	/* Offset clock monotonic -> clock tai */
	ktime_t			offs_tai;

};

static inline struct timespec tk_xtime(struct timekeeper *tk)
{
	struct timespec ts;

	ts.tv_sec = tk->xtime_sec;
	ts.tv_nsec = (long)(tk->xtime_nsec >> tk->shift);
	return ts;
}


=======
/**
 * struct tk_read_base - base structure for timekeeping readout
 * @clock:	Current clocksource used for timekeeping.
 * @read:	Read function of @clock
 * @mask:	Bitmask for two's complement subtraction of non 64bit clocks
 * @cycle_last: @clock cycle value at last update
 * @mult:	NTP adjusted multiplier for scaled math conversion
 * @shift:	Shift value for scaled math conversion
 * @xtime_nsec: Shifted (fractional) nano seconds offset for readout
 * @base_mono:  ktime_t (nanoseconds) base time for readout
 *
 * This struct has size 56 byte on 64 bit. Together with a seqcount it
 * occupies a single 64byte cache line.
 *
 * The struct is separate from struct timekeeper as it is also used
 * for a fast NMI safe accessor to clock monotonic.
 */
struct tk_read_base {
	struct clocksource	*clock;
	cycle_t			(*read)(struct clocksource *cs);
	cycle_t			mask;
	cycle_t			cycle_last;
	u32			mult;
	u32			shift;
	u64			xtime_nsec;
	ktime_t			base_mono;
};

/**
 * struct timekeeper - Structure holding internal timekeeping values.
 * @tkr:		The readout base structure
 * @xtime_sec:		Current CLOCK_REALTIME time in seconds
 * @wall_to_monotonic:	CLOCK_REALTIME to CLOCK_MONOTONIC offset
 * @offs_real:		Offset clock monotonic -> clock realtime
 * @offs_boot:		Offset clock monotonic -> clock boottime
 * @offs_tai:		Offset clock monotonic -> clock tai
 * @tai_offset:		The current UTC to TAI offset in seconds
 * @base_raw:		Monotonic raw base time in ktime_t format
 * @raw_time:		Monotonic raw base time in timespec64 format
 * @cycle_interval:	Number of clock cycles in one NTP interval
 * @xtime_interval:	Number of clock shifted nano seconds in one NTP
 *			interval.
 * @xtime_remainder:	Shifted nano seconds left over when rounding
 *			@cycle_interval
 * @raw_interval:	Raw nano seconds accumulated per NTP interval.
 * @ntp_error:		Difference between accumulated time and NTP time in ntp
 *			shifted nano seconds.
 * @ntp_error_shift:	Shift conversion between clock shifted nano seconds and
 *			ntp shifted nano seconds.
 *
 * Note: For timespec(64) based interfaces wall_to_monotonic is what
 * we need to add to xtime (or xtime corrected for sub jiffie times)
 * to get to monotonic time.  Monotonic is pegged at zero at system
 * boot time, so wall_to_monotonic will be negative, however, we will
 * ALWAYS keep the tv_nsec part positive so we can use the usual
 * normalization.
 *
 * wall_to_monotonic is moved after resume from suspend for the
 * monotonic time not to jump. We need to add total_sleep_time to
 * wall_to_monotonic to get the real boot based time offset.
 *
 * wall_to_monotonic is no longer the boot time, getboottime must be
 * used instead.
 */
struct timekeeper {
	struct tk_read_base	tkr;
	u64			xtime_sec;
	struct timespec64	wall_to_monotonic;
	ktime_t			offs_real;
	ktime_t			offs_boot;
	ktime_t			offs_tai;
	s32			tai_offset;
	ktime_t			base_raw;
	struct timespec64	raw_time;

	/* The following members are for timekeeping internal use */
	cycle_t			cycle_interval;
	u64			xtime_interval;
	s64			xtime_remainder;
	u32			raw_interval;
	/* The ntp_tick_length() value currently being used.
	 * This cached copy ensures we consistently apply the tick
	 * length for an entire tick, as ntp_tick_length may change
	 * mid-tick, and we don't want to apply that new value to
	 * the tick in progress.
	 */
	u64			ntp_tick;
	/* Difference between accumulated time and NTP time in ntp
	 * shifted nano seconds. */
	s64			ntp_error;
	u32			ntp_error_shift;
	u32			ntp_err_mult;
};

>>>>>>> v3.18
#ifdef CONFIG_GENERIC_TIME_VSYSCALL

extern void update_vsyscall(struct timekeeper *tk);
extern void update_vsyscall_tz(void);

#elif defined(CONFIG_GENERIC_TIME_VSYSCALL_OLD)

extern void update_vsyscall_old(struct timespec *ts, struct timespec *wtm,
<<<<<<< HEAD
				struct clocksource *c, u32 mult);
extern void update_vsyscall_tz(void);

static inline void update_vsyscall(struct timekeeper *tk)
{
	struct timespec xt;

	xt = tk_xtime(tk);
	update_vsyscall_old(&xt, &tk->wall_to_monotonic, tk->clock, tk->mult);
}

=======
				struct clocksource *c, u32 mult,
				cycle_t cycle_last);
extern void update_vsyscall_tz(void);

>>>>>>> v3.18
#else

static inline void update_vsyscall(struct timekeeper *tk)
{
}
static inline void update_vsyscall_tz(void)
{
}
#endif

#endif /* _LINUX_TIMEKEEPER_INTERNAL_H */
