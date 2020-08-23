#ifndef _ASM_GENERIC_PERCPU_H_
#define _ASM_GENERIC_PERCPU_H_

#include <linux/compiler.h>
#include <linux/threads.h>
#include <linux/percpu-defs.h>

#ifdef CONFIG_SMP

/*
 * per_cpu_offset() is the offset that has to be added to a
 * percpu variable to get to the instance for a certain processor.
 *
 * Most arches use the __per_cpu_offset array for those offsets but
 * some arches have their own ways of determining the offset (x86_64, s390).
 */
#ifndef __per_cpu_offset
extern unsigned long __per_cpu_offset[NR_CPUS];

#define per_cpu_offset(x) (__per_cpu_offset[x])
#endif

/*
 * Determine the offset for the currently active processor.
 * An arch may define __my_cpu_offset to provide a more effective
 * means of obtaining the offset to the per cpu variables of the
 * current processor.
 */
#ifndef __my_cpu_offset
#define __my_cpu_offset per_cpu_offset(raw_smp_processor_id())
#endif
#ifdef CONFIG_DEBUG_PREEMPT
#define my_cpu_offset per_cpu_offset(smp_processor_id())
#else
#define my_cpu_offset __my_cpu_offset
#endif

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Add a offset to a pointer but keep the pointer as is.
 *
 * Only S390 provides its own means of moving the pointer.
 */
#ifndef SHIFT_PERCPU_PTR
/* Weird cast keeps both GCC and sparse happy. */
#define SHIFT_PERCPU_PTR(__p, __offset)	({				\
	__verify_pcpu_ptr((__p));					\
	RELOC_HIDE((typeof(*(__p)) __kernel __force *)(__p), (__offset)); \
})
#endif

/*
 * A percpu variable may point to a discarded regions. The following are
 * established ways to produce a usable pointer from the percpu variable
 * offset.
 */
#define per_cpu(var, cpu) \
	(*SHIFT_PERCPU_PTR(&(var), per_cpu_offset(cpu)))

#ifndef __this_cpu_ptr
#define __this_cpu_ptr(ptr) SHIFT_PERCPU_PTR(ptr, __my_cpu_offset)
#endif
#ifdef CONFIG_DEBUG_PREEMPT
#define this_cpu_ptr(ptr) SHIFT_PERCPU_PTR(ptr, my_cpu_offset)
#else
#define this_cpu_ptr(ptr) __this_cpu_ptr(ptr)
#endif

#define __get_cpu_var(var) (*this_cpu_ptr(&(var)))
#define __raw_get_cpu_var(var) (*__this_cpu_ptr(&(var)))

#ifdef CONFIG_HAVE_SETUP_PER_CPU_AREA
extern void setup_per_cpu_areas(void);
#endif

#else /* ! SMP */

#define VERIFY_PERCPU_PTR(__p) ({			\
	__verify_pcpu_ptr((__p));			\
	(typeof(*(__p)) __kernel __force *)(__p);	\
})

#define per_cpu(var, cpu)	(*((void)(cpu), VERIFY_PERCPU_PTR(&(var))))
#define __get_cpu_var(var)	(*VERIFY_PERCPU_PTR(&(var)))
#define __raw_get_cpu_var(var)	(*VERIFY_PERCPU_PTR(&(var)))
#define this_cpu_ptr(ptr)	per_cpu_ptr(ptr, 0)
#define __this_cpu_ptr(ptr)	this_cpu_ptr(ptr)

#endif	/* SMP */

#ifndef PER_CPU_BASE_SECTION
#ifdef CONFIG_SMP
#define PER_CPU_BASE_SECTION ".data..percpu"
#else
#define PER_CPU_BASE_SECTION ".data"
#endif
#endif

#ifdef CONFIG_SMP

#ifdef MODULE
#define PER_CPU_SHARED_ALIGNED_SECTION ""
#define PER_CPU_ALIGNED_SECTION ""
#else
#define PER_CPU_SHARED_ALIGNED_SECTION "..shared_aligned"
#define PER_CPU_ALIGNED_SECTION "..shared_aligned"
#endif
#define PER_CPU_FIRST_SECTION "..first"

#else

#define PER_CPU_SHARED_ALIGNED_SECTION ""
#define PER_CPU_ALIGNED_SECTION "..shared_aligned"
#define PER_CPU_FIRST_SECTION ""

#endif

#ifndef PER_CPU_ATTRIBUTES
#define PER_CPU_ATTRIBUTES
#endif

#ifndef PER_CPU_DEF_ATTRIBUTES
#define PER_CPU_DEF_ATTRIBUTES
=======
=======
>>>>>>> v3.18
 * Arch may define arch_raw_cpu_ptr() to provide more efficient address
 * translations for raw_cpu_ptr().
 */
#ifndef arch_raw_cpu_ptr
#define arch_raw_cpu_ptr(ptr) SHIFT_PERCPU_PTR(ptr, __my_cpu_offset)
#endif

#ifdef CONFIG_HAVE_SETUP_PER_CPU_AREA
extern void setup_per_cpu_areas(void);
#endif

#endif	/* SMP */

#ifndef PER_CPU_BASE_SECTION
#ifdef CONFIG_SMP
#define PER_CPU_BASE_SECTION ".data..percpu"
#else
#define PER_CPU_BASE_SECTION ".data"
#endif
#endif

#ifndef PER_CPU_ATTRIBUTES
#define PER_CPU_ATTRIBUTES
#endif

#ifndef PER_CPU_DEF_ATTRIBUTES
#define PER_CPU_DEF_ATTRIBUTES
#endif

#define raw_cpu_generic_to_op(pcp, val, op)				\
do {									\
	*raw_cpu_ptr(&(pcp)) op val;					\
} while (0)

#define raw_cpu_generic_add_return(pcp, val)				\
({									\
	raw_cpu_add(pcp, val);						\
	raw_cpu_read(pcp);						\
})

#define raw_cpu_generic_xchg(pcp, nval)					\
({									\
	typeof(pcp) __ret;						\
	__ret = raw_cpu_read(pcp);					\
	raw_cpu_write(pcp, nval);					\
	__ret;								\
})

#define raw_cpu_generic_cmpxchg(pcp, oval, nval)			\
({									\
	typeof(pcp) __ret;						\
	__ret = raw_cpu_read(pcp);					\
	if (__ret == (oval))						\
		raw_cpu_write(pcp, nval);				\
	__ret;								\
})

#define raw_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2) \
({									\
	int __ret = 0;							\
	if (raw_cpu_read(pcp1) == (oval1) &&				\
			 raw_cpu_read(pcp2)  == (oval2)) {		\
		raw_cpu_write(pcp1, nval1);				\
		raw_cpu_write(pcp2, nval2);				\
		__ret = 1;						\
	}								\
	(__ret);							\
})

#define this_cpu_generic_read(pcp)					\
({									\
	typeof(pcp) __ret;						\
	preempt_disable();						\
	__ret = *this_cpu_ptr(&(pcp));					\
	preempt_enable();						\
	__ret;								\
})

#define this_cpu_generic_to_op(pcp, val, op)				\
do {									\
	unsigned long __flags;						\
	raw_local_irq_save(__flags);					\
	*raw_cpu_ptr(&(pcp)) op val;					\
	raw_local_irq_restore(__flags);					\
} while (0)

#define this_cpu_generic_add_return(pcp, val)				\
({									\
	typeof(pcp) __ret;						\
	unsigned long __flags;						\
	raw_local_irq_save(__flags);					\
	raw_cpu_add(pcp, val);						\
	__ret = raw_cpu_read(pcp);					\
	raw_local_irq_restore(__flags);					\
	__ret;								\
})

#define this_cpu_generic_xchg(pcp, nval)				\
({									\
	typeof(pcp) __ret;						\
	unsigned long __flags;						\
	raw_local_irq_save(__flags);					\
	__ret = raw_cpu_read(pcp);					\
	raw_cpu_write(pcp, nval);					\
	raw_local_irq_restore(__flags);					\
	__ret;								\
})

#define this_cpu_generic_cmpxchg(pcp, oval, nval)			\
({									\
	typeof(pcp) __ret;						\
	unsigned long __flags;						\
	raw_local_irq_save(__flags);					\
	__ret = raw_cpu_read(pcp);					\
	if (__ret == (oval))						\
		raw_cpu_write(pcp, nval);				\
	raw_local_irq_restore(__flags);					\
	__ret;								\
})

#define this_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2)	\
({									\
	int __ret;							\
	unsigned long __flags;						\
	raw_local_irq_save(__flags);					\
	__ret = raw_cpu_generic_cmpxchg_double(pcp1, pcp2,		\
			oval1, oval2, nval1, nval2);			\
	raw_local_irq_restore(__flags);					\
	__ret;								\
})

#ifndef raw_cpu_read_1
#define raw_cpu_read_1(pcp)		(*raw_cpu_ptr(&(pcp)))
#endif
#ifndef raw_cpu_read_2
#define raw_cpu_read_2(pcp)		(*raw_cpu_ptr(&(pcp)))
#endif
#ifndef raw_cpu_read_4
#define raw_cpu_read_4(pcp)		(*raw_cpu_ptr(&(pcp)))
#endif
#ifndef raw_cpu_read_8
#define raw_cpu_read_8(pcp)		(*raw_cpu_ptr(&(pcp)))
#endif

#ifndef raw_cpu_write_1
#define raw_cpu_write_1(pcp, val)	raw_cpu_generic_to_op(pcp, val, =)
#endif
#ifndef raw_cpu_write_2
#define raw_cpu_write_2(pcp, val)	raw_cpu_generic_to_op(pcp, val, =)
#endif
#ifndef raw_cpu_write_4
#define raw_cpu_write_4(pcp, val)	raw_cpu_generic_to_op(pcp, val, =)
#endif
#ifndef raw_cpu_write_8
#define raw_cpu_write_8(pcp, val)	raw_cpu_generic_to_op(pcp, val, =)
#endif

#ifndef raw_cpu_add_1
#define raw_cpu_add_1(pcp, val)		raw_cpu_generic_to_op(pcp, val, +=)
#endif
#ifndef raw_cpu_add_2
#define raw_cpu_add_2(pcp, val)		raw_cpu_generic_to_op(pcp, val, +=)
#endif
#ifndef raw_cpu_add_4
#define raw_cpu_add_4(pcp, val)		raw_cpu_generic_to_op(pcp, val, +=)
#endif
#ifndef raw_cpu_add_8
#define raw_cpu_add_8(pcp, val)		raw_cpu_generic_to_op(pcp, val, +=)
#endif

#ifndef raw_cpu_and_1
#define raw_cpu_and_1(pcp, val)		raw_cpu_generic_to_op(pcp, val, &=)
#endif
#ifndef raw_cpu_and_2
#define raw_cpu_and_2(pcp, val)		raw_cpu_generic_to_op(pcp, val, &=)
#endif
#ifndef raw_cpu_and_4
#define raw_cpu_and_4(pcp, val)		raw_cpu_generic_to_op(pcp, val, &=)
#endif
#ifndef raw_cpu_and_8
#define raw_cpu_and_8(pcp, val)		raw_cpu_generic_to_op(pcp, val, &=)
#endif

#ifndef raw_cpu_or_1
#define raw_cpu_or_1(pcp, val)		raw_cpu_generic_to_op(pcp, val, |=)
#endif
#ifndef raw_cpu_or_2
#define raw_cpu_or_2(pcp, val)		raw_cpu_generic_to_op(pcp, val, |=)
#endif
#ifndef raw_cpu_or_4
#define raw_cpu_or_4(pcp, val)		raw_cpu_generic_to_op(pcp, val, |=)
#endif
#ifndef raw_cpu_or_8
#define raw_cpu_or_8(pcp, val)		raw_cpu_generic_to_op(pcp, val, |=)
#endif

#ifndef raw_cpu_add_return_1
#define raw_cpu_add_return_1(pcp, val)	raw_cpu_generic_add_return(pcp, val)
#endif
#ifndef raw_cpu_add_return_2
#define raw_cpu_add_return_2(pcp, val)	raw_cpu_generic_add_return(pcp, val)
#endif
#ifndef raw_cpu_add_return_4
#define raw_cpu_add_return_4(pcp, val)	raw_cpu_generic_add_return(pcp, val)
#endif
#ifndef raw_cpu_add_return_8
#define raw_cpu_add_return_8(pcp, val)	raw_cpu_generic_add_return(pcp, val)
#endif

#ifndef raw_cpu_xchg_1
#define raw_cpu_xchg_1(pcp, nval)	raw_cpu_generic_xchg(pcp, nval)
#endif
#ifndef raw_cpu_xchg_2
#define raw_cpu_xchg_2(pcp, nval)	raw_cpu_generic_xchg(pcp, nval)
#endif
#ifndef raw_cpu_xchg_4
#define raw_cpu_xchg_4(pcp, nval)	raw_cpu_generic_xchg(pcp, nval)
#endif
#ifndef raw_cpu_xchg_8
#define raw_cpu_xchg_8(pcp, nval)	raw_cpu_generic_xchg(pcp, nval)
#endif

#ifndef raw_cpu_cmpxchg_1
#define raw_cpu_cmpxchg_1(pcp, oval, nval) \
	raw_cpu_generic_cmpxchg(pcp, oval, nval)
#endif
#ifndef raw_cpu_cmpxchg_2
#define raw_cpu_cmpxchg_2(pcp, oval, nval) \
	raw_cpu_generic_cmpxchg(pcp, oval, nval)
#endif
#ifndef raw_cpu_cmpxchg_4
#define raw_cpu_cmpxchg_4(pcp, oval, nval) \
	raw_cpu_generic_cmpxchg(pcp, oval, nval)
#endif
#ifndef raw_cpu_cmpxchg_8
#define raw_cpu_cmpxchg_8(pcp, oval, nval) \
	raw_cpu_generic_cmpxchg(pcp, oval, nval)
#endif

#ifndef raw_cpu_cmpxchg_double_1
#define raw_cpu_cmpxchg_double_1(pcp1, pcp2, oval1, oval2, nval1, nval2) \
	raw_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2)
#endif
#ifndef raw_cpu_cmpxchg_double_2
#define raw_cpu_cmpxchg_double_2(pcp1, pcp2, oval1, oval2, nval1, nval2) \
	raw_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2)
#endif
#ifndef raw_cpu_cmpxchg_double_4
#define raw_cpu_cmpxchg_double_4(pcp1, pcp2, oval1, oval2, nval1, nval2) \
	raw_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2)
#endif
#ifndef raw_cpu_cmpxchg_double_8
#define raw_cpu_cmpxchg_double_8(pcp1, pcp2, oval1, oval2, nval1, nval2) \
	raw_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2)
#endif

#ifndef this_cpu_read_1
#define this_cpu_read_1(pcp)		this_cpu_generic_read(pcp)
#endif
#ifndef this_cpu_read_2
#define this_cpu_read_2(pcp)		this_cpu_generic_read(pcp)
#endif
#ifndef this_cpu_read_4
#define this_cpu_read_4(pcp)		this_cpu_generic_read(pcp)
#endif
#ifndef this_cpu_read_8
#define this_cpu_read_8(pcp)		this_cpu_generic_read(pcp)
#endif

#ifndef this_cpu_write_1
#define this_cpu_write_1(pcp, val)	this_cpu_generic_to_op(pcp, val, =)
#endif
#ifndef this_cpu_write_2
#define this_cpu_write_2(pcp, val)	this_cpu_generic_to_op(pcp, val, =)
#endif
#ifndef this_cpu_write_4
#define this_cpu_write_4(pcp, val)	this_cpu_generic_to_op(pcp, val, =)
#endif
#ifndef this_cpu_write_8
#define this_cpu_write_8(pcp, val)	this_cpu_generic_to_op(pcp, val, =)
#endif

#ifndef this_cpu_add_1
#define this_cpu_add_1(pcp, val)	this_cpu_generic_to_op(pcp, val, +=)
#endif
#ifndef this_cpu_add_2
#define this_cpu_add_2(pcp, val)	this_cpu_generic_to_op(pcp, val, +=)
#endif
#ifndef this_cpu_add_4
#define this_cpu_add_4(pcp, val)	this_cpu_generic_to_op(pcp, val, +=)
#endif
#ifndef this_cpu_add_8
#define this_cpu_add_8(pcp, val)	this_cpu_generic_to_op(pcp, val, +=)
#endif

#ifndef this_cpu_and_1
#define this_cpu_and_1(pcp, val)	this_cpu_generic_to_op(pcp, val, &=)
#endif
#ifndef this_cpu_and_2
#define this_cpu_and_2(pcp, val)	this_cpu_generic_to_op(pcp, val, &=)
#endif
#ifndef this_cpu_and_4
#define this_cpu_and_4(pcp, val)	this_cpu_generic_to_op(pcp, val, &=)
#endif
#ifndef this_cpu_and_8
#define this_cpu_and_8(pcp, val)	this_cpu_generic_to_op(pcp, val, &=)
#endif

#ifndef this_cpu_or_1
#define this_cpu_or_1(pcp, val)		this_cpu_generic_to_op(pcp, val, |=)
#endif
#ifndef this_cpu_or_2
#define this_cpu_or_2(pcp, val)		this_cpu_generic_to_op(pcp, val, |=)
#endif
#ifndef this_cpu_or_4
#define this_cpu_or_4(pcp, val)		this_cpu_generic_to_op(pcp, val, |=)
#endif
#ifndef this_cpu_or_8
#define this_cpu_or_8(pcp, val)		this_cpu_generic_to_op(pcp, val, |=)
#endif

#ifndef this_cpu_add_return_1
#define this_cpu_add_return_1(pcp, val)	this_cpu_generic_add_return(pcp, val)
#endif
#ifndef this_cpu_add_return_2
#define this_cpu_add_return_2(pcp, val)	this_cpu_generic_add_return(pcp, val)
#endif
#ifndef this_cpu_add_return_4
#define this_cpu_add_return_4(pcp, val)	this_cpu_generic_add_return(pcp, val)
#endif
#ifndef this_cpu_add_return_8
#define this_cpu_add_return_8(pcp, val)	this_cpu_generic_add_return(pcp, val)
#endif

#ifndef this_cpu_xchg_1
#define this_cpu_xchg_1(pcp, nval)	this_cpu_generic_xchg(pcp, nval)
#endif
#ifndef this_cpu_xchg_2
#define this_cpu_xchg_2(pcp, nval)	this_cpu_generic_xchg(pcp, nval)
#endif
#ifndef this_cpu_xchg_4
#define this_cpu_xchg_4(pcp, nval)	this_cpu_generic_xchg(pcp, nval)
#endif
#ifndef this_cpu_xchg_8
#define this_cpu_xchg_8(pcp, nval)	this_cpu_generic_xchg(pcp, nval)
#endif

#ifndef this_cpu_cmpxchg_1
#define this_cpu_cmpxchg_1(pcp, oval, nval) \
	this_cpu_generic_cmpxchg(pcp, oval, nval)
#endif
#ifndef this_cpu_cmpxchg_2
#define this_cpu_cmpxchg_2(pcp, oval, nval) \
	this_cpu_generic_cmpxchg(pcp, oval, nval)
#endif
#ifndef this_cpu_cmpxchg_4
#define this_cpu_cmpxchg_4(pcp, oval, nval) \
	this_cpu_generic_cmpxchg(pcp, oval, nval)
#endif
#ifndef this_cpu_cmpxchg_8
#define this_cpu_cmpxchg_8(pcp, oval, nval) \
	this_cpu_generic_cmpxchg(pcp, oval, nval)
#endif

#ifndef this_cpu_cmpxchg_double_1
#define this_cpu_cmpxchg_double_1(pcp1, pcp2, oval1, oval2, nval1, nval2) \
	this_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2)
#endif
#ifndef this_cpu_cmpxchg_double_2
#define this_cpu_cmpxchg_double_2(pcp1, pcp2, oval1, oval2, nval1, nval2) \
	this_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2)
#endif
#ifndef this_cpu_cmpxchg_double_4
#define this_cpu_cmpxchg_double_4(pcp1, pcp2, oval1, oval2, nval1, nval2) \
	this_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2)
#endif
#ifndef this_cpu_cmpxchg_double_8
#define this_cpu_cmpxchg_double_8(pcp1, pcp2, oval1, oval2, nval1, nval2) \
	this_cpu_generic_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif

#endif /* _ASM_GENERIC_PERCPU_H_ */
