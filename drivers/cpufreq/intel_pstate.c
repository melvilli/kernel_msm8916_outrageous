/*
 * intel_pstate.c: Native P state management for Intel processors
 *
 * (C) Copyright 2012 Intel Corporation
 * Author: Dirk Brandewie <dirk.j.brandewie@intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

#include <linux/kernel.h>
#include <linux/kernel_stat.h>
#include <linux/module.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/tick.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/cpu.h>
#include <linux/cpufreq.h>
#include <linux/sysfs.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/acpi.h>
>>>>>>> v3.18
=======
#include <linux/acpi.h>
>>>>>>> v3.18
#include <trace/events/power.h>

#include <asm/div64.h>
#include <asm/msr.h>
#include <asm/cpu_device_id.h>

<<<<<<< HEAD
<<<<<<< HEAD
#define SAMPLE_COUNT		3
=======
=======
>>>>>>> v3.18
#define BYT_RATIOS		0x66a
#define BYT_VIDS		0x66b
#define BYT_TURBO_RATIOS	0x66c
#define BYT_TURBO_VIDS		0x66d
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

#define FRAC_BITS 8
#define int_tofp(X) ((int64_t)(X) << FRAC_BITS)
#define fp_toint(X) ((X) >> FRAC_BITS)

<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
static inline int32_t mul_fp(int32_t x, int32_t y)
{
	return ((int64_t)x * (int64_t)y) >> FRAC_BITS;
}

static inline int32_t div_fp(int32_t x, int32_t y)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return div_s64((int64_t)x << FRAC_BITS, (int64_t)y);
=======
=======
>>>>>>> v3.18
	return div_s64((int64_t)x << FRAC_BITS, y);
}

static inline int ceiling_fp(int32_t x)
{
	int mask, ret;

	ret = fp_toint(x);
	mask = (1 << FRAC_BITS) - 1;
	if (x & mask)
		ret += 1;
	return ret;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

struct sample {
	int32_t core_pct_busy;
	u64 aperf;
	u64 mperf;
	int freq;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	ktime_t time;
>>>>>>> v3.18
=======
	ktime_t time;
>>>>>>> v3.18
};

struct pstate_data {
	int	current_pstate;
	int	min_pstate;
	int	max_pstate;
<<<<<<< HEAD
<<<<<<< HEAD
	int	turbo_pstate;
};

=======
=======
>>>>>>> v3.18
	int	scaling;
	int	turbo_pstate;
};

struct vid_data {
	int min;
	int max;
	int turbo;
	int32_t ratio;
};

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
struct _pid {
	int setpoint;
	int32_t integral;
	int32_t p_gain;
	int32_t i_gain;
	int32_t d_gain;
	int deadband;
	int32_t last_err;
};

struct cpudata {
	int cpu;

<<<<<<< HEAD
<<<<<<< HEAD
	char name[64];

	struct timer_list timer;

	struct pstate_adjust_policy *pstate_policy;
	struct pstate_data pstate;
	struct _pid pid;

	int min_pstate_count;

	u64	prev_aperf;
	u64	prev_mperf;
	int	sample_ptr;
	struct sample samples[SAMPLE_COUNT];
=======
=======
>>>>>>> v3.18
	struct timer_list timer;

	struct pstate_data pstate;
	struct vid_data vid;
	struct _pid pid;

	ktime_t last_sample_time;
	u64	prev_aperf;
	u64	prev_mperf;
	struct sample sample;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
};

static struct cpudata **all_cpu_data;
struct pstate_adjust_policy {
	int sample_rate_ms;
	int deadband;
	int setpoint;
	int p_gain_pct;
	int d_gain_pct;
	int i_gain_pct;
};

<<<<<<< HEAD
<<<<<<< HEAD
static struct pstate_adjust_policy default_policy = {
	.sample_rate_ms = 10,
	.deadband = 0,
	.setpoint = 97,
	.p_gain_pct = 20,
	.d_gain_pct = 0,
	.i_gain_pct = 0,
};

struct perf_limits {
	int no_turbo;
=======
=======
>>>>>>> v3.18
struct pstate_funcs {
	int (*get_max)(void);
	int (*get_min)(void);
	int (*get_turbo)(void);
	int (*get_scaling)(void);
	void (*set)(struct cpudata*, int pstate);
	void (*get_vid)(struct cpudata *);
};

struct cpu_defaults {
	struct pstate_adjust_policy pid_policy;
	struct pstate_funcs funcs;
};

static struct pstate_adjust_policy pid_params;
static struct pstate_funcs pstate_funcs;

struct perf_limits {
	int no_turbo;
	int turbo_disabled;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	int max_perf_pct;
	int min_perf_pct;
	int32_t max_perf;
	int32_t min_perf;
	int max_policy_pct;
	int max_sysfs_pct;
};

static struct perf_limits limits = {
	.no_turbo = 0,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	.turbo_disabled = 0,
>>>>>>> v3.18
=======
	.turbo_disabled = 0,
>>>>>>> v3.18
	.max_perf_pct = 100,
	.max_perf = int_tofp(1),
	.min_perf_pct = 0,
	.min_perf = 0,
	.max_policy_pct = 100,
	.max_sysfs_pct = 100,
};

static inline void pid_reset(struct _pid *pid, int setpoint, int busy,
<<<<<<< HEAD
<<<<<<< HEAD
			int deadband, int integral) {
	pid->setpoint = setpoint;
	pid->deadband  = deadband;
	pid->integral  = int_tofp(integral);
	pid->last_err  = setpoint - busy;
=======
=======
>>>>>>> v3.18
			     int deadband, int integral) {
	pid->setpoint = setpoint;
	pid->deadband  = deadband;
	pid->integral  = int_tofp(integral);
	pid->last_err  = int_tofp(setpoint) - int_tofp(busy);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static inline void pid_p_gain_set(struct _pid *pid, int percent)
{
	pid->p_gain = div_fp(int_tofp(percent), int_tofp(100));
}

static inline void pid_i_gain_set(struct _pid *pid, int percent)
{
	pid->i_gain = div_fp(int_tofp(percent), int_tofp(100));
}

static inline void pid_d_gain_set(struct _pid *pid, int percent)
{
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	pid->d_gain = div_fp(int_tofp(percent), int_tofp(100));
}

static signed int pid_calc(struct _pid *pid, int32_t busy)
{
	signed int result;
	int32_t pterm, dterm, fp_error;
	int32_t integral_limit;

	fp_error = int_tofp(pid->setpoint) - busy;

	if (abs(fp_error) <= int_tofp(pid->deadband))
		return 0;

	pterm = mul_fp(pid->p_gain, fp_error);

	pid->integral += fp_error;

	/* limit the integral term */
	integral_limit = int_tofp(30);
	if (pid->integral > integral_limit)
		pid->integral = integral_limit;
	if (pid->integral < -integral_limit)
		pid->integral = -integral_limit;

	dterm = mul_fp(pid->d_gain, fp_error - pid->last_err);
	pid->last_err = fp_error;

	result = pterm + mul_fp(pid->integral, pid->i_gain) + dterm;
<<<<<<< HEAD
<<<<<<< HEAD

=======
	result = result + (1 << (FRAC_BITS-1));
>>>>>>> v3.18
=======
	result = result + (1 << (FRAC_BITS-1));
>>>>>>> v3.18
	return (signed int)fp_toint(result);
}

static inline void intel_pstate_busy_pid_reset(struct cpudata *cpu)
{
<<<<<<< HEAD
<<<<<<< HEAD
	pid_p_gain_set(&cpu->pid, cpu->pstate_policy->p_gain_pct);
	pid_d_gain_set(&cpu->pid, cpu->pstate_policy->d_gain_pct);
	pid_i_gain_set(&cpu->pid, cpu->pstate_policy->i_gain_pct);

	pid_reset(&cpu->pid,
		cpu->pstate_policy->setpoint,
		100,
		cpu->pstate_policy->deadband,
		0);
=======
=======
>>>>>>> v3.18
	pid_p_gain_set(&cpu->pid, pid_params.p_gain_pct);
	pid_d_gain_set(&cpu->pid, pid_params.d_gain_pct);
	pid_i_gain_set(&cpu->pid, pid_params.i_gain_pct);

	pid_reset(&cpu->pid, pid_params.setpoint, 100, pid_params.deadband, 0);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static inline void intel_pstate_reset_all_pid(void)
{
	unsigned int cpu;
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
	for_each_online_cpu(cpu) {
		if (all_cpu_data[cpu])
			intel_pstate_busy_pid_reset(all_cpu_data[cpu]);
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
static inline void update_turbo_state(void)
{
	u64 misc_en;
	struct cpudata *cpu;

	cpu = all_cpu_data[0];
	rdmsrl(MSR_IA32_MISC_ENABLE, misc_en);
	limits.turbo_disabled =
		(misc_en & MSR_IA32_MISC_ENABLE_TURBO_DISABLE ||
		 cpu->pstate.max_pstate == cpu->pstate.turbo_pstate);
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
/************************** debugfs begin ************************/
static int pid_param_set(void *data, u64 val)
{
	*(u32 *)data = val;
	intel_pstate_reset_all_pid();
	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
static int pid_param_get(void *data, u64 *val)
{
	*val = *(u32 *)data;
	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
DEFINE_SIMPLE_ATTRIBUTE(fops_pid_param, pid_param_get,
			pid_param_set, "%llu\n");
=======
DEFINE_SIMPLE_ATTRIBUTE(fops_pid_param, pid_param_get, pid_param_set, "%llu\n");
>>>>>>> v3.18
=======
DEFINE_SIMPLE_ATTRIBUTE(fops_pid_param, pid_param_get, pid_param_set, "%llu\n");
>>>>>>> v3.18

struct pid_param {
	char *name;
	void *value;
};

static struct pid_param pid_files[] = {
<<<<<<< HEAD
<<<<<<< HEAD
	{"sample_rate_ms", &default_policy.sample_rate_ms},
	{"d_gain_pct", &default_policy.d_gain_pct},
	{"i_gain_pct", &default_policy.i_gain_pct},
	{"deadband", &default_policy.deadband},
	{"setpoint", &default_policy.setpoint},
	{"p_gain_pct", &default_policy.p_gain_pct},
	{NULL, NULL}
};

static struct dentry *debugfs_parent;
static void intel_pstate_debug_expose_params(void)
{
=======
=======
>>>>>>> v3.18
	{"sample_rate_ms", &pid_params.sample_rate_ms},
	{"d_gain_pct", &pid_params.d_gain_pct},
	{"i_gain_pct", &pid_params.i_gain_pct},
	{"deadband", &pid_params.deadband},
	{"setpoint", &pid_params.setpoint},
	{"p_gain_pct", &pid_params.p_gain_pct},
	{NULL, NULL}
};

static void __init intel_pstate_debug_expose_params(void)
{
	struct dentry *debugfs_parent;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	int i = 0;

	debugfs_parent = debugfs_create_dir("pstate_snb", NULL);
	if (IS_ERR_OR_NULL(debugfs_parent))
		return;
	while (pid_files[i].name) {
		debugfs_create_file(pid_files[i].name, 0660,
<<<<<<< HEAD
<<<<<<< HEAD
				debugfs_parent, pid_files[i].value,
				&fops_pid_param);
=======
				    debugfs_parent, pid_files[i].value,
				    &fops_pid_param);
>>>>>>> v3.18
=======
				    debugfs_parent, pid_files[i].value,
				    &fops_pid_param);
>>>>>>> v3.18
		i++;
	}
}

/************************** debugfs end ************************/

/************************** sysfs begin ************************/
#define show_one(file_name, object)					\
	static ssize_t show_##file_name					\
	(struct kobject *kobj, struct attribute *attr, char *buf)	\
	{								\
		return sprintf(buf, "%u\n", limits.object);		\
	}

<<<<<<< HEAD
<<<<<<< HEAD
static ssize_t store_no_turbo(struct kobject *a, struct attribute *b,
				const char *buf, size_t count)
{
	unsigned int input;
	int ret;
	ret = sscanf(buf, "%u", &input);
	if (ret != 1)
		return -EINVAL;
	limits.no_turbo = clamp_t(int, input, 0 , 1);
=======
=======
>>>>>>> v3.18
static ssize_t show_no_turbo(struct kobject *kobj,
			     struct attribute *attr, char *buf)
{
	ssize_t ret;

	update_turbo_state();
	if (limits.turbo_disabled)
		ret = sprintf(buf, "%u\n", limits.turbo_disabled);
	else
		ret = sprintf(buf, "%u\n", limits.no_turbo);

	return ret;
}

static ssize_t store_no_turbo(struct kobject *a, struct attribute *b,
			      const char *buf, size_t count)
{
	unsigned int input;
	int ret;

	ret = sscanf(buf, "%u", &input);
	if (ret != 1)
		return -EINVAL;

	update_turbo_state();
	if (limits.turbo_disabled) {
		pr_warn("Turbo disabled by BIOS or unavailable on processor\n");
		return -EPERM;
	}
	limits.no_turbo = clamp_t(int, input, 0, 1);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return count;
}

static ssize_t store_max_perf_pct(struct kobject *a, struct attribute *b,
<<<<<<< HEAD
<<<<<<< HEAD
				const char *buf, size_t count)
{
	unsigned int input;
	int ret;
=======
=======
>>>>>>> v3.18
				  const char *buf, size_t count)
{
	unsigned int input;
	int ret;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	ret = sscanf(buf, "%u", &input);
	if (ret != 1)
		return -EINVAL;

	limits.max_sysfs_pct = clamp_t(int, input, 0 , 100);
	limits.max_perf_pct = min(limits.max_policy_pct, limits.max_sysfs_pct);
	limits.max_perf = div_fp(int_tofp(limits.max_perf_pct), int_tofp(100));
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
	return count;
}

static ssize_t store_min_perf_pct(struct kobject *a, struct attribute *b,
<<<<<<< HEAD
<<<<<<< HEAD
				const char *buf, size_t count)
{
	unsigned int input;
	int ret;
=======
=======
>>>>>>> v3.18
				  const char *buf, size_t count)
{
	unsigned int input;
	int ret;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	ret = sscanf(buf, "%u", &input);
	if (ret != 1)
		return -EINVAL;
	limits.min_perf_pct = clamp_t(int, input, 0 , 100);
	limits.min_perf = div_fp(int_tofp(limits.min_perf_pct), int_tofp(100));

	return count;
}

<<<<<<< HEAD
<<<<<<< HEAD
show_one(no_turbo, no_turbo);
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
show_one(max_perf_pct, max_perf_pct);
show_one(min_perf_pct, min_perf_pct);

define_one_global_rw(no_turbo);
define_one_global_rw(max_perf_pct);
define_one_global_rw(min_perf_pct);

static struct attribute *intel_pstate_attributes[] = {
	&no_turbo.attr,
	&max_perf_pct.attr,
	&min_perf_pct.attr,
	NULL
};

static struct attribute_group intel_pstate_attr_group = {
	.attrs = intel_pstate_attributes,
};
<<<<<<< HEAD
<<<<<<< HEAD
static struct kobject *intel_pstate_kobject;

static void intel_pstate_sysfs_expose_params(void)
{
=======
=======
>>>>>>> v3.18

static void __init intel_pstate_sysfs_expose_params(void)
{
	struct kobject *intel_pstate_kobject;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	int rc;

	intel_pstate_kobject = kobject_create_and_add("intel_pstate",
						&cpu_subsys.dev_root->kobj);
	BUG_ON(!intel_pstate_kobject);
<<<<<<< HEAD
<<<<<<< HEAD
	rc = sysfs_create_group(intel_pstate_kobject,
				&intel_pstate_attr_group);
=======
	rc = sysfs_create_group(intel_pstate_kobject, &intel_pstate_attr_group);
>>>>>>> v3.18
=======
	rc = sysfs_create_group(intel_pstate_kobject, &intel_pstate_attr_group);
>>>>>>> v3.18
	BUG_ON(rc);
}

/************************** sysfs end ************************/
<<<<<<< HEAD
<<<<<<< HEAD

static int intel_pstate_min_pstate(void)
{
	u64 value;
=======
=======
>>>>>>> v3.18
static int byt_get_min_pstate(void)
{
	u64 value;

	rdmsrl(BYT_RATIOS, value);
	return (value >> 8) & 0x7F;
}

static int byt_get_max_pstate(void)
{
	u64 value;

	rdmsrl(BYT_RATIOS, value);
	return (value >> 16) & 0x7F;
}

static int byt_get_turbo_pstate(void)
{
	u64 value;

	rdmsrl(BYT_TURBO_RATIOS, value);
	return value & 0x7F;
}

static void byt_set_pstate(struct cpudata *cpudata, int pstate)
{
	u64 val;
	int32_t vid_fp;
	u32 vid;

	val = pstate << 8;
	if (limits.no_turbo && !limits.turbo_disabled)
		val |= (u64)1 << 32;

	vid_fp = cpudata->vid.min + mul_fp(
		int_tofp(pstate - cpudata->pstate.min_pstate),
		cpudata->vid.ratio);

	vid_fp = clamp_t(int32_t, vid_fp, cpudata->vid.min, cpudata->vid.max);
	vid = ceiling_fp(vid_fp);

	if (pstate > cpudata->pstate.max_pstate)
		vid = cpudata->vid.turbo;

	val |= vid;

	wrmsrl(MSR_IA32_PERF_CTL, val);
}

#define BYT_BCLK_FREQS 5
static int byt_freq_table[BYT_BCLK_FREQS] = { 833, 1000, 1333, 1167, 800};

static int byt_get_scaling(void)
{
	u64 value;
	int i;

	rdmsrl(MSR_FSB_FREQ, value);
	i = value & 0x3;

	BUG_ON(i > BYT_BCLK_FREQS);

	return byt_freq_table[i] * 100;
}

static void byt_get_vid(struct cpudata *cpudata)
{
	u64 value;

	rdmsrl(BYT_VIDS, value);
	cpudata->vid.min = int_tofp((value >> 8) & 0x7f);
	cpudata->vid.max = int_tofp((value >> 16) & 0x7f);
	cpudata->vid.ratio = div_fp(
		cpudata->vid.max - cpudata->vid.min,
		int_tofp(cpudata->pstate.max_pstate -
			cpudata->pstate.min_pstate));

	rdmsrl(BYT_TURBO_VIDS, value);
	cpudata->vid.turbo = value & 0x7f;
}

static int core_get_min_pstate(void)
{
	u64 value;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	rdmsrl(MSR_PLATFORM_INFO, value);
	return (value >> 40) & 0xFF;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int intel_pstate_max_pstate(void)
{
	u64 value;
=======
=======
>>>>>>> v3.18
static int core_get_max_pstate(void)
{
	u64 value;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	rdmsrl(MSR_PLATFORM_INFO, value);
	return (value >> 8) & 0xFF;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int intel_pstate_turbo_pstate(void)
{
	u64 value;
	int nont, ret;
	rdmsrl(MSR_NHM_TURBO_RATIO_LIMIT, value);
	nont = intel_pstate_max_pstate();
	ret = ((value) & 255);
=======
=======
>>>>>>> v3.18
static int core_get_turbo_pstate(void)
{
	u64 value;
	int nont, ret;

	rdmsrl(MSR_NHM_TURBO_RATIO_LIMIT, value);
	nont = core_get_max_pstate();
	ret = (value) & 255;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (ret <= nont)
		ret = nont;
	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
static inline int core_get_scaling(void)
{
	return 100000;
}

static void core_set_pstate(struct cpudata *cpudata, int pstate)
{
	u64 val;

	val = pstate << 8;
	if (limits.no_turbo && !limits.turbo_disabled)
		val |= (u64)1 << 32;

	wrmsrl_on_cpu(cpudata->cpu, MSR_IA32_PERF_CTL, val);
}

static struct cpu_defaults core_params = {
	.pid_policy = {
		.sample_rate_ms = 10,
		.deadband = 0,
		.setpoint = 97,
		.p_gain_pct = 20,
		.d_gain_pct = 0,
		.i_gain_pct = 0,
	},
	.funcs = {
		.get_max = core_get_max_pstate,
		.get_min = core_get_min_pstate,
		.get_turbo = core_get_turbo_pstate,
		.get_scaling = core_get_scaling,
		.set = core_set_pstate,
	},
};

static struct cpu_defaults byt_params = {
	.pid_policy = {
		.sample_rate_ms = 10,
		.deadband = 0,
		.setpoint = 97,
		.p_gain_pct = 14,
		.d_gain_pct = 0,
		.i_gain_pct = 4,
	},
	.funcs = {
		.get_max = byt_get_max_pstate,
		.get_min = byt_get_min_pstate,
		.get_turbo = byt_get_turbo_pstate,
		.set = byt_set_pstate,
		.get_scaling = byt_get_scaling,
		.get_vid = byt_get_vid,
	},
};

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static void intel_pstate_get_min_max(struct cpudata *cpu, int *min, int *max)
{
	int max_perf = cpu->pstate.turbo_pstate;
	int max_perf_adj;
	int min_perf;
<<<<<<< HEAD
<<<<<<< HEAD
	if (limits.no_turbo)
=======

	if (limits.no_turbo || limits.turbo_disabled)
>>>>>>> v3.18
=======

	if (limits.no_turbo || limits.turbo_disabled)
>>>>>>> v3.18
		max_perf = cpu->pstate.max_pstate;

	max_perf_adj = fp_toint(mul_fp(int_tofp(max_perf), limits.max_perf));
	*max = clamp_t(int, max_perf_adj,
			cpu->pstate.min_pstate, cpu->pstate.turbo_pstate);

	min_perf = fp_toint(mul_fp(int_tofp(max_perf), limits.min_perf));
<<<<<<< HEAD
<<<<<<< HEAD
	*min = clamp_t(int, min_perf,
			cpu->pstate.min_pstate, max_perf);
=======
	*min = clamp_t(int, min_perf, cpu->pstate.min_pstate, max_perf);
>>>>>>> v3.18
=======
	*min = clamp_t(int, min_perf, cpu->pstate.min_pstate, max_perf);
>>>>>>> v3.18
}

static void intel_pstate_set_pstate(struct cpudata *cpu, int pstate)
{
	int max_perf, min_perf;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	update_turbo_state();

>>>>>>> v3.18
=======
	update_turbo_state();

>>>>>>> v3.18
	intel_pstate_get_min_max(cpu, &min_perf, &max_perf);

	pstate = clamp_t(int, pstate, min_perf, max_perf);

	if (pstate == cpu->pstate.current_pstate)
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	trace_cpu_frequency(pstate * 100000, cpu->cpu);

	cpu->pstate.current_pstate = pstate;
	if (limits.no_turbo)
		wrmsrl(MSR_IA32_PERF_CTL, BIT(32) | (pstate << 8));
	else
		wrmsrl(MSR_IA32_PERF_CTL, pstate << 8);

}

static inline void intel_pstate_pstate_increase(struct cpudata *cpu, int steps)
{
	int target;
	target = cpu->pstate.current_pstate + steps;

	intel_pstate_set_pstate(cpu, target);
}

static inline void intel_pstate_pstate_decrease(struct cpudata *cpu, int steps)
{
	int target;
	target = cpu->pstate.current_pstate - steps;
	intel_pstate_set_pstate(cpu, target);
}

static void intel_pstate_get_cpu_pstates(struct cpudata *cpu)
{
	sprintf(cpu->name, "Intel 2nd generation core");

	cpu->pstate.min_pstate = intel_pstate_min_pstate();
	cpu->pstate.max_pstate = intel_pstate_max_pstate();
	cpu->pstate.turbo_pstate = intel_pstate_turbo_pstate();

	/*
	 * goto max pstate so we don't slow up boot if we are built-in if we are
	 * a module we will take care of it during normal operation
	 */
	intel_pstate_set_pstate(cpu, cpu->pstate.max_pstate);
}

static inline void intel_pstate_calc_busy(struct cpudata *cpu,
					struct sample *sample)
{
	u64 core_pct;
	core_pct = div64_u64(int_tofp(sample->aperf * 100),
			     sample->mperf);
	sample->freq = fp_toint(cpu->pstate.max_pstate * core_pct * 1000);

	sample->core_pct_busy = core_pct;
=======
=======
>>>>>>> v3.18
	trace_cpu_frequency(pstate * cpu->pstate.scaling, cpu->cpu);

	cpu->pstate.current_pstate = pstate;

	pstate_funcs.set(cpu, pstate);
}

static void intel_pstate_get_cpu_pstates(struct cpudata *cpu)
{
	cpu->pstate.min_pstate = pstate_funcs.get_min();
	cpu->pstate.max_pstate = pstate_funcs.get_max();
	cpu->pstate.turbo_pstate = pstate_funcs.get_turbo();
	cpu->pstate.scaling = pstate_funcs.get_scaling();

	if (pstate_funcs.get_vid)
		pstate_funcs.get_vid(cpu);
	intel_pstate_set_pstate(cpu, cpu->pstate.min_pstate);
}

static inline void intel_pstate_calc_busy(struct cpudata *cpu)
{
	struct sample *sample = &cpu->sample;
	int64_t core_pct;

	core_pct = int_tofp(sample->aperf) * int_tofp(100);
	core_pct = div64_u64(core_pct, int_tofp(sample->mperf));

	sample->freq = fp_toint(
		mul_fp(int_tofp(
			cpu->pstate.max_pstate * cpu->pstate.scaling / 100),
			core_pct));

	sample->core_pct_busy = (int32_t)core_pct;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static inline void intel_pstate_sample(struct cpudata *cpu)
{
	u64 aperf, mperf;
<<<<<<< HEAD
<<<<<<< HEAD

	rdmsrl(MSR_IA32_APERF, aperf);
	rdmsrl(MSR_IA32_MPERF, mperf);
	cpu->sample_ptr = (cpu->sample_ptr + 1) % SAMPLE_COUNT;
	cpu->samples[cpu->sample_ptr].aperf = aperf;
	cpu->samples[cpu->sample_ptr].mperf = mperf;
	cpu->samples[cpu->sample_ptr].aperf -= cpu->prev_aperf;
	cpu->samples[cpu->sample_ptr].mperf -= cpu->prev_mperf;

	intel_pstate_calc_busy(cpu, &cpu->samples[cpu->sample_ptr]);
=======
=======
>>>>>>> v3.18
	unsigned long flags;

	local_irq_save(flags);
	rdmsrl(MSR_IA32_APERF, aperf);
	rdmsrl(MSR_IA32_MPERF, mperf);
	local_irq_restore(flags);

	cpu->last_sample_time = cpu->sample.time;
	cpu->sample.time = ktime_get();
	cpu->sample.aperf = aperf;
	cpu->sample.mperf = mperf;
	cpu->sample.aperf -= cpu->prev_aperf;
	cpu->sample.mperf -= cpu->prev_mperf;

	intel_pstate_calc_busy(cpu);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	cpu->prev_aperf = aperf;
	cpu->prev_mperf = mperf;
}

static inline void intel_pstate_set_sample_time(struct cpudata *cpu)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int sample_time, delay;

	sample_time = cpu->pstate_policy->sample_rate_ms;
	delay = msecs_to_jiffies(sample_time);
=======
	int delay;

	delay = msecs_to_jiffies(pid_params.sample_rate_ms);
>>>>>>> v3.18
=======
	int delay;

	delay = msecs_to_jiffies(pid_params.sample_rate_ms);
>>>>>>> v3.18
	mod_timer_pinned(&cpu->timer, jiffies + delay);
}

static inline int32_t intel_pstate_get_scaled_busy(struct cpudata *cpu)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int32_t core_busy, max_pstate, current_pstate;

	core_busy = cpu->samples[cpu->sample_ptr].core_pct_busy;
	max_pstate = int_tofp(cpu->pstate.max_pstate);
	current_pstate = int_tofp(cpu->pstate.current_pstate);
	return mul_fp(core_busy, div_fp(max_pstate, current_pstate));
=======
=======
>>>>>>> v3.18
	int32_t core_busy, max_pstate, current_pstate, sample_ratio;
	u32 duration_us;
	u32 sample_time;

	core_busy = cpu->sample.core_pct_busy;
	max_pstate = int_tofp(cpu->pstate.max_pstate);
	current_pstate = int_tofp(cpu->pstate.current_pstate);
	core_busy = mul_fp(core_busy, div_fp(max_pstate, current_pstate));

	sample_time = pid_params.sample_rate_ms  * USEC_PER_MSEC;
	duration_us = (u32) ktime_us_delta(cpu->sample.time,
					   cpu->last_sample_time);
	if (duration_us > sample_time * 3) {
		sample_ratio = div_fp(int_tofp(sample_time),
				      int_tofp(duration_us));
		core_busy = mul_fp(core_busy, sample_ratio);
	}

	return core_busy;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static inline void intel_pstate_adjust_busy_pstate(struct cpudata *cpu)
{
	int32_t busy_scaled;
	struct _pid *pid;
<<<<<<< HEAD
<<<<<<< HEAD
	signed int ctl = 0;
	int steps;
=======
	signed int ctl;
>>>>>>> v3.18
=======
	signed int ctl;
>>>>>>> v3.18

	pid = &cpu->pid;
	busy_scaled = intel_pstate_get_scaled_busy(cpu);

	ctl = pid_calc(pid, busy_scaled);

<<<<<<< HEAD
<<<<<<< HEAD
	steps = abs(ctl);
	if (ctl < 0)
		intel_pstate_pstate_increase(cpu, steps);
	else
		intel_pstate_pstate_decrease(cpu, steps);
=======
	/* Negative values of ctl increase the pstate and vice versa */
	intel_pstate_set_pstate(cpu, cpu->pstate.current_pstate - ctl);
>>>>>>> v3.18
=======
	/* Negative values of ctl increase the pstate and vice versa */
	intel_pstate_set_pstate(cpu, cpu->pstate.current_pstate - ctl);
>>>>>>> v3.18
}

static void intel_pstate_timer_func(unsigned long __data)
{
	struct cpudata *cpu = (struct cpudata *) __data;
<<<<<<< HEAD
<<<<<<< HEAD

	intel_pstate_sample(cpu);
	intel_pstate_adjust_busy_pstate(cpu);

	if (cpu->pstate.current_pstate == cpu->pstate.min_pstate) {
		cpu->min_pstate_count++;
		if (!(cpu->min_pstate_count % 5)) {
			intel_pstate_set_pstate(cpu, cpu->pstate.max_pstate);
		}
	} else
		cpu->min_pstate_count = 0;
=======
=======
>>>>>>> v3.18
	struct sample *sample;

	intel_pstate_sample(cpu);

	sample = &cpu->sample;

	intel_pstate_adjust_busy_pstate(cpu);

	trace_pstate_sample(fp_toint(sample->core_pct_busy),
			fp_toint(intel_pstate_get_scaled_busy(cpu)),
			cpu->pstate.current_pstate,
			sample->mperf,
			sample->aperf,
			sample->freq);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	intel_pstate_set_sample_time(cpu);
}

#define ICPU(model, policy) \
	{ X86_VENDOR_INTEL, 6, model, X86_FEATURE_APERFMPERF,\
			(unsigned long)&policy }

static const struct x86_cpu_id intel_pstate_cpu_ids[] = {
<<<<<<< HEAD
<<<<<<< HEAD
	ICPU(0x2a, default_policy),
	ICPU(0x2d, default_policy),
	ICPU(0x3a, default_policy),
	ICPU(0x3c, default_policy),
	ICPU(0x3e, default_policy),
	ICPU(0x3f, default_policy),
	ICPU(0x45, default_policy),
	ICPU(0x46, default_policy),
=======
=======
>>>>>>> v3.18
	ICPU(0x2a, core_params),
	ICPU(0x2d, core_params),
	ICPU(0x37, byt_params),
	ICPU(0x3a, core_params),
	ICPU(0x3c, core_params),
	ICPU(0x3d, core_params),
	ICPU(0x3e, core_params),
	ICPU(0x3f, core_params),
	ICPU(0x45, core_params),
	ICPU(0x46, core_params),
	ICPU(0x4c, byt_params),
	ICPU(0x4f, core_params),
	ICPU(0x56, core_params),
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	{}
};
MODULE_DEVICE_TABLE(x86cpu, intel_pstate_cpu_ids);

static int intel_pstate_init_cpu(unsigned int cpunum)
{
<<<<<<< HEAD
<<<<<<< HEAD

	const struct x86_cpu_id *id;
	struct cpudata *cpu;

	id = x86_match_cpu(intel_pstate_cpu_ids);
	if (!id)
		return -ENODEV;

	all_cpu_data[cpunum] = kzalloc(sizeof(struct cpudata), GFP_KERNEL);
=======
=======
>>>>>>> v3.18
	struct cpudata *cpu;

	if (!all_cpu_data[cpunum])
		all_cpu_data[cpunum] = kzalloc(sizeof(struct cpudata),
					       GFP_KERNEL);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (!all_cpu_data[cpunum])
		return -ENOMEM;

	cpu = all_cpu_data[cpunum];

<<<<<<< HEAD
<<<<<<< HEAD
	intel_pstate_get_cpu_pstates(cpu);
	if (!cpu->pstate.current_pstate) {
		all_cpu_data[cpunum] = NULL;
		kfree(cpu);
		return -ENODATA;
	}

	cpu->cpu = cpunum;
	cpu->pstate_policy =
		(struct pstate_adjust_policy *)id->driver_data;
	init_timer_deferrable(&cpu->timer);
	cpu->timer.function = intel_pstate_timer_func;
	cpu->timer.data =
		(unsigned long)cpu;
	cpu->timer.expires = jiffies + HZ/100;
	intel_pstate_busy_pid_reset(cpu);
	intel_pstate_sample(cpu);
	intel_pstate_set_pstate(cpu, cpu->pstate.max_pstate);

	add_timer_on(&cpu->timer, cpunum);

	pr_info("Intel pstate controlling: cpu %d\n", cpunum);
=======
=======
>>>>>>> v3.18
	cpu->cpu = cpunum;
	intel_pstate_get_cpu_pstates(cpu);

	init_timer_deferrable(&cpu->timer);
	cpu->timer.function = intel_pstate_timer_func;
	cpu->timer.data = (unsigned long)cpu;
	cpu->timer.expires = jiffies + HZ/100;
	intel_pstate_busy_pid_reset(cpu);
	intel_pstate_sample(cpu);

	add_timer_on(&cpu->timer, cpunum);

	pr_debug("Intel pstate controlling: cpu %d\n", cpunum);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return 0;
}

static unsigned int intel_pstate_get(unsigned int cpu_num)
{
	struct sample *sample;
	struct cpudata *cpu;

	cpu = all_cpu_data[cpu_num];
	if (!cpu)
		return 0;
<<<<<<< HEAD
<<<<<<< HEAD
	sample = &cpu->samples[cpu->sample_ptr];
=======
	sample = &cpu->sample;
>>>>>>> v3.18
=======
	sample = &cpu->sample;
>>>>>>> v3.18
	return sample->freq;
}

static int intel_pstate_set_policy(struct cpufreq_policy *policy)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct cpudata *cpu;

	cpu = all_cpu_data[policy->cpu];

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (!policy->cpuinfo.max_freq)
		return -ENODEV;

	if (policy->policy == CPUFREQ_POLICY_PERFORMANCE) {
		limits.min_perf_pct = 100;
		limits.min_perf = int_tofp(1);
		limits.max_policy_pct = 100;
		limits.max_perf_pct = 100;
		limits.max_perf = int_tofp(1);
		limits.no_turbo = 0;
		return 0;
	}
	limits.min_perf_pct = (policy->min * 100) / policy->cpuinfo.max_freq;
	limits.min_perf_pct = clamp_t(int, limits.min_perf_pct, 0 , 100);
	limits.min_perf = div_fp(int_tofp(limits.min_perf_pct), int_tofp(100));

<<<<<<< HEAD
<<<<<<< HEAD
	limits.max_policy_pct = policy->max * 100 / policy->cpuinfo.max_freq;
=======
	limits.max_policy_pct = (policy->max * 100) / policy->cpuinfo.max_freq;
>>>>>>> v3.18
=======
	limits.max_policy_pct = (policy->max * 100) / policy->cpuinfo.max_freq;
>>>>>>> v3.18
	limits.max_policy_pct = clamp_t(int, limits.max_policy_pct, 0 , 100);
	limits.max_perf_pct = min(limits.max_policy_pct, limits.max_sysfs_pct);
	limits.max_perf = div_fp(int_tofp(limits.max_perf_pct), int_tofp(100));

	return 0;
}

static int intel_pstate_verify_policy(struct cpufreq_policy *policy)
{
	cpufreq_verify_within_cpu_limits(policy);

<<<<<<< HEAD
<<<<<<< HEAD
	if ((policy->policy != CPUFREQ_POLICY_POWERSAVE) &&
		(policy->policy != CPUFREQ_POLICY_PERFORMANCE))
=======
	if (policy->policy != CPUFREQ_POLICY_POWERSAVE &&
	    policy->policy != CPUFREQ_POLICY_PERFORMANCE)
>>>>>>> v3.18
=======
	if (policy->policy != CPUFREQ_POLICY_POWERSAVE &&
	    policy->policy != CPUFREQ_POLICY_PERFORMANCE)
>>>>>>> v3.18
		return -EINVAL;

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int intel_pstate_cpu_exit(struct cpufreq_policy *policy)
{
	int cpu = policy->cpu;

	del_timer(&all_cpu_data[cpu]->timer);
	kfree(all_cpu_data[cpu]);
	all_cpu_data[cpu] = NULL;
	return 0;
=======
=======
>>>>>>> v3.18
static void intel_pstate_stop_cpu(struct cpufreq_policy *policy)
{
	int cpu_num = policy->cpu;
	struct cpudata *cpu = all_cpu_data[cpu_num];

	pr_info("intel_pstate CPU %d exiting\n", cpu_num);

	del_timer_sync(&all_cpu_data[cpu_num]->timer);
	intel_pstate_set_pstate(cpu, cpu->pstate.min_pstate);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static int intel_pstate_cpu_init(struct cpufreq_policy *policy)
{
	struct cpudata *cpu;
	int rc;

	rc = intel_pstate_init_cpu(policy->cpu);
	if (rc)
		return rc;

	cpu = all_cpu_data[policy->cpu];

<<<<<<< HEAD
<<<<<<< HEAD
	if (!limits.no_turbo &&
		limits.min_perf_pct == 100 && limits.max_perf_pct == 100)
=======
	if (limits.min_perf_pct == 100 && limits.max_perf_pct == 100)
>>>>>>> v3.18
=======
	if (limits.min_perf_pct == 100 && limits.max_perf_pct == 100)
>>>>>>> v3.18
		policy->policy = CPUFREQ_POLICY_PERFORMANCE;
	else
		policy->policy = CPUFREQ_POLICY_POWERSAVE;

<<<<<<< HEAD
<<<<<<< HEAD
	policy->min = cpu->pstate.min_pstate * 100000;
	policy->max = cpu->pstate.turbo_pstate * 100000;

	/* cpuinfo and default policy values */
	policy->cpuinfo.min_freq = cpu->pstate.min_pstate * 100000;
	policy->cpuinfo.max_freq = cpu->pstate.turbo_pstate * 100000;
=======
=======
>>>>>>> v3.18
	policy->min = cpu->pstate.min_pstate * cpu->pstate.scaling;
	policy->max = cpu->pstate.turbo_pstate * cpu->pstate.scaling;

	/* cpuinfo and default policy values */
	policy->cpuinfo.min_freq = cpu->pstate.min_pstate * cpu->pstate.scaling;
	policy->cpuinfo.max_freq =
		cpu->pstate.turbo_pstate * cpu->pstate.scaling;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	policy->cpuinfo.transition_latency = CPUFREQ_ETERNAL;
	cpumask_set_cpu(policy->cpu, policy->cpus);

	return 0;
}

static struct cpufreq_driver intel_pstate_driver = {
	.flags		= CPUFREQ_CONST_LOOPS,
	.verify		= intel_pstate_verify_policy,
	.setpolicy	= intel_pstate_set_policy,
	.get		= intel_pstate_get,
	.init		= intel_pstate_cpu_init,
<<<<<<< HEAD
<<<<<<< HEAD
	.exit		= intel_pstate_cpu_exit,
=======
	.stop_cpu	= intel_pstate_stop_cpu,
>>>>>>> v3.18
=======
	.stop_cpu	= intel_pstate_stop_cpu,
>>>>>>> v3.18
	.name		= "intel_pstate",
};

static int __initdata no_load;

static int intel_pstate_msrs_not_valid(void)
{
	/* Check that all the msr's we are using are valid. */
	u64 aperf, mperf, tmp;

	rdmsrl(MSR_IA32_APERF, aperf);
	rdmsrl(MSR_IA32_MPERF, mperf);

<<<<<<< HEAD
<<<<<<< HEAD
	if (!intel_pstate_min_pstate() ||
		!intel_pstate_max_pstate() ||
		!intel_pstate_turbo_pstate())
=======
	if (!pstate_funcs.get_max() ||
	    !pstate_funcs.get_min() ||
	    !pstate_funcs.get_turbo())
>>>>>>> v3.18
=======
	if (!pstate_funcs.get_max() ||
	    !pstate_funcs.get_min() ||
	    !pstate_funcs.get_turbo())
>>>>>>> v3.18
		return -ENODEV;

	rdmsrl(MSR_IA32_APERF, tmp);
	if (!(tmp - aperf))
		return -ENODEV;

	rdmsrl(MSR_IA32_MPERF, tmp);
	if (!(tmp - mperf))
		return -ENODEV;

	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18

static void copy_pid_params(struct pstate_adjust_policy *policy)
{
	pid_params.sample_rate_ms = policy->sample_rate_ms;
	pid_params.p_gain_pct = policy->p_gain_pct;
	pid_params.i_gain_pct = policy->i_gain_pct;
	pid_params.d_gain_pct = policy->d_gain_pct;
	pid_params.deadband = policy->deadband;
	pid_params.setpoint = policy->setpoint;
}

static void copy_cpu_funcs(struct pstate_funcs *funcs)
{
	pstate_funcs.get_max   = funcs->get_max;
	pstate_funcs.get_min   = funcs->get_min;
	pstate_funcs.get_turbo = funcs->get_turbo;
	pstate_funcs.get_scaling = funcs->get_scaling;
	pstate_funcs.set       = funcs->set;
	pstate_funcs.get_vid   = funcs->get_vid;
}

#if IS_ENABLED(CONFIG_ACPI)
#include <acpi/processor.h>

static bool intel_pstate_no_acpi_pss(void)
{
	int i;

	for_each_possible_cpu(i) {
		acpi_status status;
		union acpi_object *pss;
		struct acpi_buffer buffer = { ACPI_ALLOCATE_BUFFER, NULL };
		struct acpi_processor *pr = per_cpu(processors, i);

		if (!pr)
			continue;

		status = acpi_evaluate_object(pr->handle, "_PSS", NULL, &buffer);
		if (ACPI_FAILURE(status))
			continue;

		pss = buffer.pointer;
		if (pss && pss->type == ACPI_TYPE_PACKAGE) {
			kfree(pss);
			return false;
		}

		kfree(pss);
	}

	return true;
}

struct hw_vendor_info {
	u16  valid;
	char oem_id[ACPI_OEM_ID_SIZE];
	char oem_table_id[ACPI_OEM_TABLE_ID_SIZE];
};

/* Hardware vendor-specific info that has its own power management modes */
static struct hw_vendor_info vendor_info[] = {
	{1, "HP    ", "ProLiant"},
	{0, "", ""},
};

static bool intel_pstate_platform_pwr_mgmt_exists(void)
{
	struct acpi_table_header hdr;
	struct hw_vendor_info *v_info;

	if (acpi_disabled ||
	    ACPI_FAILURE(acpi_get_table_header(ACPI_SIG_FADT, 0, &hdr)))
		return false;

	for (v_info = vendor_info; v_info->valid; v_info++) {
		if (!strncmp(hdr.oem_id, v_info->oem_id, ACPI_OEM_ID_SIZE) &&
		    !strncmp(hdr.oem_table_id, v_info->oem_table_id, ACPI_OEM_TABLE_ID_SIZE) &&
		    intel_pstate_no_acpi_pss())
			return true;
	}

	return false;
}
#else /* CONFIG_ACPI not enabled */
static inline bool intel_pstate_platform_pwr_mgmt_exists(void) { return false; }
#endif /* CONFIG_ACPI */

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static int __init intel_pstate_init(void)
{
	int cpu, rc = 0;
	const struct x86_cpu_id *id;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct cpu_defaults *cpu_info;
>>>>>>> v3.18
=======
	struct cpu_defaults *cpu_info;
>>>>>>> v3.18

	if (no_load)
		return -ENODEV;

	id = x86_match_cpu(intel_pstate_cpu_ids);
	if (!id)
		return -ENODEV;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	/*
	 * The Intel pstate driver will be ignored if the platform
	 * firmware has its own power management modes.
	 */
	if (intel_pstate_platform_pwr_mgmt_exists())
		return -ENODEV;

	cpu_info = (struct cpu_defaults *)id->driver_data;

	copy_pid_params(&cpu_info->pid_policy);
	copy_cpu_funcs(&cpu_info->funcs);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (intel_pstate_msrs_not_valid())
		return -ENODEV;

	pr_info("Intel P-state driver initializing.\n");

	all_cpu_data = vzalloc(sizeof(void *) * num_possible_cpus());
	if (!all_cpu_data)
		return -ENOMEM;

	rc = cpufreq_register_driver(&intel_pstate_driver);
	if (rc)
		goto out;

	intel_pstate_debug_expose_params();
	intel_pstate_sysfs_expose_params();
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
	return rc;
out:
	get_online_cpus();
	for_each_online_cpu(cpu) {
		if (all_cpu_data[cpu]) {
			del_timer_sync(&all_cpu_data[cpu]->timer);
			kfree(all_cpu_data[cpu]);
		}
	}

	put_online_cpus();
	vfree(all_cpu_data);
	return -ENODEV;
}
device_initcall(intel_pstate_init);

static int __init intel_pstate_setup(char *str)
{
	if (!str)
		return -EINVAL;

	if (!strcmp(str, "disable"))
		no_load = 1;
	return 0;
}
early_param("intel_pstate", intel_pstate_setup);

MODULE_AUTHOR("Dirk Brandewie <dirk.j.brandewie@intel.com>");
MODULE_DESCRIPTION("'intel_pstate' - P state driver Intel Core processors");
MODULE_LICENSE("GPL");
