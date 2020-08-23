/* atomic.h: Thankfully the V9 is at least reasonable for this
 *           stuff.
 *
 * Copyright (C) 1996, 1997, 2000, 2012 David S. Miller (davem@redhat.com)
 */

#ifndef __ARCH_SPARC64_ATOMIC__
#define __ARCH_SPARC64_ATOMIC__

#include <linux/types.h>
#include <asm/cmpxchg.h>
<<<<<<< HEAD
=======
#include <asm/barrier.h>
>>>>>>> v3.18

#define ATOMIC_INIT(i)		{ (i) }
#define ATOMIC64_INIT(i)	{ (i) }

<<<<<<< HEAD
#define atomic_read(v)		(*(volatile int *)&(v)->counter)
#define atomic64_read(v)	(*(volatile long *)&(v)->counter)
=======
#define atomic_read(v)		ACCESS_ONCE((v)->counter)
#define atomic64_read(v)	ACCESS_ONCE((v)->counter)
>>>>>>> v3.18

#define atomic_set(v, i)	(((v)->counter) = i)
#define atomic64_set(v, i)	(((v)->counter) = i)

<<<<<<< HEAD
extern void atomic_add(int, atomic_t *);
extern void atomic64_add(long, atomic64_t *);
extern void atomic_sub(int, atomic_t *);
extern void atomic64_sub(long, atomic64_t *);

extern int atomic_add_ret(int, atomic_t *);
extern long atomic64_add_ret(long, atomic64_t *);
extern int atomic_sub_ret(int, atomic_t *);
extern long atomic64_sub_ret(long, atomic64_t *);

#define atomic_dec_return(v) atomic_sub_ret(1, v)
#define atomic64_dec_return(v) atomic64_sub_ret(1, v)

#define atomic_inc_return(v) atomic_add_ret(1, v)
#define atomic64_inc_return(v) atomic64_add_ret(1, v)

#define atomic_sub_return(i, v) atomic_sub_ret(i, v)
#define atomic64_sub_return(i, v) atomic64_sub_ret(i, v)

#define atomic_add_return(i, v) atomic_add_ret(i, v)
#define atomic64_add_return(i, v) atomic64_add_ret(i, v)
=======
#define ATOMIC_OP(op)							\
void atomic_##op(int, atomic_t *);					\
void atomic64_##op(long, atomic64_t *);

#define ATOMIC_OP_RETURN(op)						\
int atomic_##op##_return(int, atomic_t *);				\
long atomic64_##op##_return(long, atomic64_t *);

#define ATOMIC_OPS(op) ATOMIC_OP(op) ATOMIC_OP_RETURN(op)

ATOMIC_OPS(add)
ATOMIC_OPS(sub)

#undef ATOMIC_OPS
#undef ATOMIC_OP_RETURN
#undef ATOMIC_OP

#define atomic_dec_return(v)   atomic_sub_return(1, v)
#define atomic64_dec_return(v) atomic64_sub_return(1, v)

#define atomic_inc_return(v)   atomic_add_return(1, v)
#define atomic64_inc_return(v) atomic64_add_return(1, v)
>>>>>>> v3.18

/*
 * atomic_inc_and_test - increment and test
 * @v: pointer of type atomic_t
 *
 * Atomically increments @v by 1
 * and returns true if the result is zero, or false for all
 * other cases.
 */
#define atomic_inc_and_test(v) (atomic_inc_return(v) == 0)
#define atomic64_inc_and_test(v) (atomic64_inc_return(v) == 0)

<<<<<<< HEAD
#define atomic_sub_and_test(i, v) (atomic_sub_ret(i, v) == 0)
#define atomic64_sub_and_test(i, v) (atomic64_sub_ret(i, v) == 0)

#define atomic_dec_and_test(v) (atomic_sub_ret(1, v) == 0)
#define atomic64_dec_and_test(v) (atomic64_sub_ret(1, v) == 0)
=======
#define atomic_sub_and_test(i, v) (atomic_sub_return(i, v) == 0)
#define atomic64_sub_and_test(i, v) (atomic64_sub_return(i, v) == 0)

#define atomic_dec_and_test(v) (atomic_sub_return(1, v) == 0)
#define atomic64_dec_and_test(v) (atomic64_sub_return(1, v) == 0)
>>>>>>> v3.18

#define atomic_inc(v) atomic_add(1, v)
#define atomic64_inc(v) atomic64_add(1, v)

#define atomic_dec(v) atomic_sub(1, v)
#define atomic64_dec(v) atomic64_sub(1, v)

<<<<<<< HEAD
#define atomic_add_negative(i, v) (atomic_add_ret(i, v) < 0)
#define atomic64_add_negative(i, v) (atomic64_add_ret(i, v) < 0)
=======
#define atomic_add_negative(i, v) (atomic_add_return(i, v) < 0)
#define atomic64_add_negative(i, v) (atomic64_add_return(i, v) < 0)
>>>>>>> v3.18

#define atomic_cmpxchg(v, o, n) (cmpxchg(&((v)->counter), (o), (n)))
#define atomic_xchg(v, new) (xchg(&((v)->counter), new))

static inline int __atomic_add_unless(atomic_t *v, int a, int u)
{
	int c, old;
	c = atomic_read(v);
	for (;;) {
		if (unlikely(c == (u)))
			break;
		old = atomic_cmpxchg((v), c, c + (a));
		if (likely(old == c))
			break;
		c = old;
	}
	return c;
}

#define atomic64_cmpxchg(v, o, n) \
	((__typeof__((v)->counter))cmpxchg(&((v)->counter), (o), (n)))
#define atomic64_xchg(v, new) (xchg(&((v)->counter), new))

static inline long atomic64_add_unless(atomic64_t *v, long a, long u)
{
	long c, old;
	c = atomic64_read(v);
	for (;;) {
		if (unlikely(c == (u)))
			break;
		old = atomic64_cmpxchg((v), c, c + (a));
		if (likely(old == c))
			break;
		c = old;
	}
	return c != (u);
}

#define atomic64_inc_not_zero(v) atomic64_add_unless((v), 1, 0)

<<<<<<< HEAD
extern long atomic64_dec_if_positive(atomic64_t *v);

/* Atomic operations are already serializing */
#define smp_mb__before_atomic_dec()	barrier()
#define smp_mb__after_atomic_dec()	barrier()
#define smp_mb__before_atomic_inc()	barrier()
#define smp_mb__after_atomic_inc()	barrier()
=======
long atomic64_dec_if_positive(atomic64_t *v);
>>>>>>> v3.18

#endif /* !(__ARCH_SPARC64_ATOMIC__) */
