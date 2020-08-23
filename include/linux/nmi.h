/*
 *  linux/include/linux/nmi.h
 */
#ifndef LINUX_NMI_H
#define LINUX_NMI_H

#include <linux/sched.h>
#include <asm/irq.h>

/**
 * touch_nmi_watchdog - restart NMI watchdog timeout.
 * 
 * If the architecture supports the NMI watchdog, touch_nmi_watchdog()
 * may be used to reset the timeout - for code which intentionally
 * disables interrupts for a long time. This call is stateless.
 */
<<<<<<< HEAD
<<<<<<< HEAD
#if defined(CONFIG_HAVE_NMI_WATCHDOG) || defined(CONFIG_HARDLOCKUP_DETECTOR_NMI)
#include <asm/nmi.h>
#endif

#if defined(CONFIG_HAVE_NMI_WATCHDOG) || defined(CONFIG_HARDLOCKUP_DETECTOR)
=======
#if defined(CONFIG_HAVE_NMI_WATCHDOG) || defined(CONFIG_HARDLOCKUP_DETECTOR)
#include <asm/nmi.h>
>>>>>>> v3.18
=======
#if defined(CONFIG_HAVE_NMI_WATCHDOG) || defined(CONFIG_HARDLOCKUP_DETECTOR)
#include <asm/nmi.h>
>>>>>>> v3.18
extern void touch_nmi_watchdog(void);
#else
static inline void touch_nmi_watchdog(void)
{
	touch_softlockup_watchdog();
}
#endif

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
#if defined(CONFIG_HARDLOCKUP_DETECTOR)
extern void watchdog_enable_hardlockup_detector(bool val);
extern bool watchdog_hardlockup_detector_is_enabled(void);
#else
static inline void watchdog_enable_hardlockup_detector(bool val)
{
}
static inline bool watchdog_hardlockup_detector_is_enabled(void)
{
	return true;
}
#endif

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
/*
 * Create trigger_all_cpu_backtrace() out of the arch-provided
 * base function. Return whether such support was available,
 * to allow calling code to fall back to some other mechanism:
 */
#ifdef arch_trigger_all_cpu_backtrace
static inline bool trigger_all_cpu_backtrace(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	arch_trigger_all_cpu_backtrace();

	return true;
}
=======
=======
>>>>>>> v3.18
	arch_trigger_all_cpu_backtrace(true);

	return true;
}
static inline bool trigger_allbutself_cpu_backtrace(void)
{
	arch_trigger_all_cpu_backtrace(false);
	return true;
}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#else
static inline bool trigger_all_cpu_backtrace(void)
{
	return false;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
static inline bool trigger_allbutself_cpu_backtrace(void)
{
	return false;
}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif

#ifdef CONFIG_LOCKUP_DETECTOR
int hw_nmi_is_cpu_stuck(struct pt_regs *);
u64 hw_nmi_get_sample_period(int watchdog_thresh);
<<<<<<< HEAD
<<<<<<< HEAD
extern int watchdog_enabled;
extern int watchdog_thresh;
=======
extern int watchdog_user_enabled;
extern int watchdog_thresh;
extern int sysctl_softlockup_all_cpu_backtrace;
>>>>>>> v3.18
=======
extern int watchdog_user_enabled;
extern int watchdog_thresh;
extern int sysctl_softlockup_all_cpu_backtrace;
>>>>>>> v3.18
struct ctl_table;
extern int proc_dowatchdog(struct ctl_table *, int ,
			   void __user *, size_t *, loff_t *);
#endif

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
#ifdef CONFIG_HAVE_ACPI_APEI_NMI
#include <asm/nmi.h>
#endif

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif
