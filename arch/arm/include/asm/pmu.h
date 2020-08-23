/*
 *  linux/arch/arm/include/asm/pmu.h
 *
 *  Copyright (C) 2009 picoChip Designs Ltd, Jamie Iles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef __ARM_PMU_H__
#define __ARM_PMU_H__

#include <linux/interrupt.h>
#include <linux/perf_event.h>

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Types of PMUs that can be accessed directly and require mutual
 * exclusion between profiling tools.
 */
enum arm_pmu_type {
	ARM_PMU_DEVICE_CPU	= 0,
	ARM_PMU_DEVICE_L2CC	= 1,
	ARM_NUM_PMU_DEVICES,
};

enum arm_pmu_state {
	ARM_PMU_STATE_OFF       = 0,
	ARM_PMU_STATE_GOING_DOWN,
	ARM_PMU_STATE_RUNNING,
};

/*
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * struct arm_pmu_platdata - ARM PMU platform data
 *
 * @handle_irq: an optional handler which will be called from the
 *	interrupt and passed the address of the low level handler,
 *	and can be used to implement any platform specific handling
 *	before or after calling it.
<<<<<<< HEAD
<<<<<<< HEAD
 * @request_pmu_irq: an optional handler in case the platform wants
 *	to use a percpu IRQ API call. e.g. request_percpu_irq
 * @free_pmu_irq: an optional handler in case the platform wants
 *	to use a percpu IRQ API call. e.g. free_percpu_irq
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * @runtime_resume: an optional handler which will be called by the
 *	runtime PM framework following a call to pm_runtime_get().
 *	Note that if pm_runtime_get() is called more than once in
 *	succession this handler will only be called once.
 * @runtime_suspend: an optional handler which will be called by the
 *	runtime PM framework following a call to pm_runtime_put().
 *	Note that if pm_runtime_get() is called more than once in
 *	succession this handler will only be called following the
 *	final call to pm_runtime_put() that actually disables the
 *	hardware.
 */
struct arm_pmu_platdata {
	irqreturn_t (*handle_irq)(int irq, void *dev,
				  irq_handler_t pmu_handler);
<<<<<<< HEAD
<<<<<<< HEAD
	int	(*request_pmu_irq)(int irq, irq_handler_t *irq_h, void *dev_id);
	void	(*free_pmu_irq)(int irq, void *dev_id);
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	int (*runtime_resume)(struct device *dev);
	int (*runtime_suspend)(struct device *dev);
};

#ifdef CONFIG_HW_PERF_EVENTS

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
/*
 * The ARMv7 CPU PMU supports up to 32 event counters.
 */
#define ARMPMU_MAX_HWEVENTS		32

#define HW_OP_UNSUPPORTED		0xFFFF
#define C(_x)				PERF_COUNT_HW_CACHE_##_x
#define CACHE_OP_UNSUPPORTED		0xFFFF

#define PERF_MAP_ALL_UNSUPPORTED					\
	[0 ... PERF_COUNT_HW_MAX - 1] = HW_OP_UNSUPPORTED

#define PERF_CACHE_MAP_ALL_UNSUPPORTED					\
[0 ... C(MAX) - 1] = {							\
	[0 ... C(OP_MAX) - 1] = {					\
		[0 ... C(RESULT_MAX) - 1] = CACHE_OP_UNSUPPORTED,	\
	},								\
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
/* The events for a given PMU register set. */
struct pmu_hw_events {
	/*
	 * The events that are active on the PMU for the given index.
	 */
	struct perf_event	**events;

	/*
	 * A 1 bit for an index indicates that the counter is being used for
	 * an event. A 0 means that the counter can be used.
	 */
	unsigned long           *used_mask;

<<<<<<< HEAD
<<<<<<< HEAD
	u32			*from_idle;

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	/*
	 * Hardware lock to serialize accesses to PMU registers. Needed for the
	 * read/modify/write sequences.
	 */
	raw_spinlock_t		pmu_lock;
};

struct arm_pmu {
	struct pmu	pmu;
	cpumask_t	active_irqs;
	char		*name;
<<<<<<< HEAD
<<<<<<< HEAD
	int		num_events;
	int             pmu_state;
	atomic_t	active_events;
	struct mutex	reserve_mutex;
	u64		max_period;
	struct platform_device	*plat_device;
	irqreturn_t	(*handle_irq)(int irq_num, void *dev);
	int		(*request_pmu_irq)(int irq, irq_handler_t *irq_h, void *dev_id);
	void		(*free_pmu_irq)(int irq, void *dev_id);
=======
	irqreturn_t	(*handle_irq)(int irq_num, void *dev);
>>>>>>> v3.18
=======
	irqreturn_t	(*handle_irq)(int irq_num, void *dev);
>>>>>>> v3.18
	void		(*enable)(struct perf_event *event);
	void		(*disable)(struct perf_event *event);
	int		(*get_event_idx)(struct pmu_hw_events *hw_events,
					 struct perf_event *event);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	void		(*clear_event_idx)(struct pmu_hw_events *hw_events,
					 struct perf_event *event);
>>>>>>> v3.18
=======
	void		(*clear_event_idx)(struct pmu_hw_events *hw_events,
					 struct perf_event *event);
>>>>>>> v3.18
	int		(*set_event_filter)(struct hw_perf_event *evt,
					    struct perf_event_attr *attr);
	u32		(*read_counter)(struct perf_event *event);
	void		(*write_counter)(struct perf_event *event, u32 val);
	void		(*start)(struct arm_pmu *);
	void		(*stop)(struct arm_pmu *);
	void		(*reset)(void *);
<<<<<<< HEAD
<<<<<<< HEAD
	void		(*force_reset)(void *);
	int		(*request_irq)(struct arm_pmu *, irq_handler_t handler);
	void		(*free_irq)(struct arm_pmu *);
	int		(*map_event)(struct perf_event *event);
	struct pmu_hw_events	*(*get_hw_events)(void);
	int	(*test_set_event_constraints)(struct perf_event *event);
	int	(*clear_event_constraints)(struct perf_event *event);
	void		(*save_pm_registers)(void *hcpu);
	void		(*restore_pm_registers)(void *hcpu);
=======
=======
>>>>>>> v3.18
	int		(*request_irq)(struct arm_pmu *, irq_handler_t handler);
	void		(*free_irq)(struct arm_pmu *);
	int		(*map_event)(struct perf_event *event);
	int		num_events;
	atomic_t	active_events;
	struct mutex	reserve_mutex;
	u64		max_period;
	struct platform_device	*plat_device;
	struct pmu_hw_events	*(*get_hw_events)(void);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
};

#define to_arm_pmu(p) (container_of(p, struct arm_pmu, pmu))

extern const struct dev_pm_ops armpmu_dev_pm_ops;

int armpmu_register(struct arm_pmu *armpmu, int type);

u64 armpmu_event_update(struct perf_event *event);

int armpmu_event_set_period(struct perf_event *event);

int armpmu_map_event(struct perf_event *event,
		     const unsigned (*event_map)[PERF_COUNT_HW_MAX],
<<<<<<< HEAD
<<<<<<< HEAD
		     unsigned (*cache_map)[PERF_COUNT_HW_CACHE_MAX]
=======
		     const unsigned (*cache_map)[PERF_COUNT_HW_CACHE_MAX]
>>>>>>> v3.18
=======
		     const unsigned (*cache_map)[PERF_COUNT_HW_CACHE_MAX]
>>>>>>> v3.18
						[PERF_COUNT_HW_CACHE_OP_MAX]
						[PERF_COUNT_HW_CACHE_RESULT_MAX],
		     u32 raw_event_mask);

<<<<<<< HEAD
<<<<<<< HEAD
int cpu_pmu_request_irq(struct arm_pmu *cpu_pmu, irq_handler_t handler);

void cpu_pmu_free_irq(struct arm_pmu *cpu_pmu);

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif /* CONFIG_HW_PERF_EVENTS */

#endif /* __ARM_PMU_H__ */
