/*
 * Simple stack backtrace regression test module
 *
 * (C) Copyright 2008 Intel Corporation
 * Author: Arjan van de Ven <arjan@linux.intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

#include <linux/completion.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/stacktrace.h>

static void backtrace_test_normal(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	printk("Testing a backtrace from process context.\n");
	printk("The following trace is a kernel self test and not a bug!\n");
=======
	pr_info("Testing a backtrace from process context.\n");
	pr_info("The following trace is a kernel self test and not a bug!\n");
>>>>>>> v3.18
=======
	pr_info("Testing a backtrace from process context.\n");
	pr_info("The following trace is a kernel self test and not a bug!\n");
>>>>>>> v3.18

	dump_stack();
}

static DECLARE_COMPLETION(backtrace_work);

static void backtrace_test_irq_callback(unsigned long data)
{
	dump_stack();
	complete(&backtrace_work);
}

static DECLARE_TASKLET(backtrace_tasklet, &backtrace_test_irq_callback, 0);

static void backtrace_test_irq(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	printk("Testing a backtrace from irq context.\n");
	printk("The following trace is a kernel self test and not a bug!\n");
=======
	pr_info("Testing a backtrace from irq context.\n");
	pr_info("The following trace is a kernel self test and not a bug!\n");
>>>>>>> v3.18
=======
	pr_info("Testing a backtrace from irq context.\n");
	pr_info("The following trace is a kernel self test and not a bug!\n");
>>>>>>> v3.18

	init_completion(&backtrace_work);
	tasklet_schedule(&backtrace_tasklet);
	wait_for_completion(&backtrace_work);
}

#ifdef CONFIG_STACKTRACE
static void backtrace_test_saved(void)
{
	struct stack_trace trace;
	unsigned long entries[8];

<<<<<<< HEAD
<<<<<<< HEAD
	printk("Testing a saved backtrace.\n");
	printk("The following trace is a kernel self test and not a bug!\n");
=======
	pr_info("Testing a saved backtrace.\n");
	pr_info("The following trace is a kernel self test and not a bug!\n");
>>>>>>> v3.18
=======
	pr_info("Testing a saved backtrace.\n");
	pr_info("The following trace is a kernel self test and not a bug!\n");
>>>>>>> v3.18

	trace.nr_entries = 0;
	trace.max_entries = ARRAY_SIZE(entries);
	trace.entries = entries;
	trace.skip = 0;

	save_stack_trace(&trace);
	print_stack_trace(&trace, 0);
}
#else
static void backtrace_test_saved(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	printk("Saved backtrace test skipped.\n");
=======
	pr_info("Saved backtrace test skipped.\n");
>>>>>>> v3.18
=======
	pr_info("Saved backtrace test skipped.\n");
>>>>>>> v3.18
}
#endif

static int backtrace_regression_test(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	printk("====[ backtrace testing ]===========\n");
=======
	pr_info("====[ backtrace testing ]===========\n");
>>>>>>> v3.18
=======
	pr_info("====[ backtrace testing ]===========\n");
>>>>>>> v3.18

	backtrace_test_normal();
	backtrace_test_irq();
	backtrace_test_saved();

<<<<<<< HEAD
<<<<<<< HEAD
	printk("====[ end of backtrace testing ]====\n");
=======
	pr_info("====[ end of backtrace testing ]====\n");
>>>>>>> v3.18
=======
	pr_info("====[ end of backtrace testing ]====\n");
>>>>>>> v3.18
	return 0;
}

static void exitf(void)
{
}

module_init(backtrace_regression_test);
module_exit(exitf);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arjan van de Ven <arjan@linux.intel.com>");
