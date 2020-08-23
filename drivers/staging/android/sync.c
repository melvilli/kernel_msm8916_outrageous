/*
 * drivers/base/sync.c
 *
 * Copyright (C) 2012 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/debugfs.h>
#include <linux/export.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/anon_inodes.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/sync.h>
=======

#include "sync.h"
>>>>>>> v3.18
=======

#include "sync.h"
>>>>>>> v3.18

#define CREATE_TRACE_POINTS
#include "trace/sync.h"

<<<<<<< HEAD
<<<<<<< HEAD
static void sync_fence_signal_pt(struct sync_pt *pt);
static int _sync_pt_has_signaled(struct sync_pt *pt);
static void sync_fence_free(struct kref *kref);

static LIST_HEAD(sync_timeline_list_head);
static DEFINE_SPINLOCK(sync_timeline_list_lock);

static LIST_HEAD(sync_fence_list_head);
static DEFINE_SPINLOCK(sync_fence_list_lock);
=======
static const struct fence_ops android_fence_ops;
static const struct file_operations sync_fence_fops;
>>>>>>> v3.18
=======
static const struct fence_ops android_fence_ops;
static const struct file_operations sync_fence_fops;
>>>>>>> v3.18

struct sync_timeline *sync_timeline_create(const struct sync_timeline_ops *ops,
					   int size, const char *name)
{
	struct sync_timeline *obj;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long flags;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (size < sizeof(struct sync_timeline))
		return NULL;

	obj = kzalloc(size, GFP_KERNEL);
	if (obj == NULL)
		return NULL;

	kref_init(&obj->kref);
	obj->ops = ops;
<<<<<<< HEAD
<<<<<<< HEAD
	strlcpy(obj->name, name, sizeof(obj->name));

	INIT_LIST_HEAD(&obj->child_list_head);
	spin_lock_init(&obj->child_list_lock);

	INIT_LIST_HEAD(&obj->active_list_head);
	spin_lock_init(&obj->active_list_lock);

	spin_lock_irqsave(&sync_timeline_list_lock, flags);
	list_add_tail(&obj->sync_timeline_list, &sync_timeline_list_head);
	spin_unlock_irqrestore(&sync_timeline_list_lock, flags);
=======
=======
>>>>>>> v3.18
	obj->context = fence_context_alloc(1);
	strlcpy(obj->name, name, sizeof(obj->name));

	INIT_LIST_HEAD(&obj->child_list_head);
	INIT_LIST_HEAD(&obj->active_list_head);
	spin_lock_init(&obj->child_list_lock);

	sync_timeline_debug_add(obj);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return obj;
}
EXPORT_SYMBOL(sync_timeline_create);

static void sync_timeline_free(struct kref *kref)
{
	struct sync_timeline *obj =
		container_of(kref, struct sync_timeline, kref);
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long flags;

	spin_lock_irqsave(&sync_timeline_list_lock, flags);
	list_del(&obj->sync_timeline_list);
	spin_unlock_irqrestore(&sync_timeline_list_lock, flags);
=======

	sync_timeline_debug_remove(obj);
>>>>>>> v3.18
=======

	sync_timeline_debug_remove(obj);
>>>>>>> v3.18

	if (obj->ops->release_obj)
		obj->ops->release_obj(obj);

	kfree(obj);
}

<<<<<<< HEAD
<<<<<<< HEAD
void sync_timeline_destroy(struct sync_timeline *obj)
{
	obj->destroyed = true;
=======
=======
>>>>>>> v3.18
static void sync_timeline_get(struct sync_timeline *obj)
{
	kref_get(&obj->kref);
}

static void sync_timeline_put(struct sync_timeline *obj)
{
	kref_put(&obj->kref, sync_timeline_free);
}

void sync_timeline_destroy(struct sync_timeline *obj)
{
	obj->destroyed = true;
	/*
	 * Ensure timeline is marked as destroyed before
	 * changing timeline's fences status.
	 */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	smp_wmb();

	/*
	 * signal any children that their parent is going away.
	 */
	sync_timeline_signal(obj);
<<<<<<< HEAD
<<<<<<< HEAD

	kref_put(&obj->kref, sync_timeline_free);
}
EXPORT_SYMBOL(sync_timeline_destroy);

static void sync_timeline_add_pt(struct sync_timeline *obj, struct sync_pt *pt)
{
	unsigned long flags;

	pt->parent = obj;

	spin_lock_irqsave(&obj->child_list_lock, flags);
	list_add_tail(&pt->child_list, &obj->child_list_head);
	spin_unlock_irqrestore(&obj->child_list_lock, flags);
}

static void sync_timeline_remove_pt(struct sync_pt *pt)
{
	struct sync_timeline *obj = pt->parent;
	unsigned long flags;

	spin_lock_irqsave(&obj->active_list_lock, flags);
	if (!list_empty(&pt->active_list))
		list_del_init(&pt->active_list);
	spin_unlock_irqrestore(&obj->active_list_lock, flags);

	spin_lock_irqsave(&obj->child_list_lock, flags);
	if (!list_empty(&pt->child_list)) {
		list_del_init(&pt->child_list);
	}
	spin_unlock_irqrestore(&obj->child_list_lock, flags);
}

=======
=======
>>>>>>> v3.18
	sync_timeline_put(obj);
}
EXPORT_SYMBOL(sync_timeline_destroy);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
void sync_timeline_signal(struct sync_timeline *obj)
{
	unsigned long flags;
	LIST_HEAD(signaled_pts);
<<<<<<< HEAD
<<<<<<< HEAD
	struct list_head *pos, *n;

	trace_sync_timeline(obj);

	spin_lock_irqsave(&obj->active_list_lock, flags);

	list_for_each_safe(pos, n, &obj->active_list_head) {
		struct sync_pt *pt =
			container_of(pos, struct sync_pt, active_list);

		if (_sync_pt_has_signaled(pt)) {
			list_del_init(pos);
			list_add(&pt->signaled_list, &signaled_pts);
			kref_get(&pt->fence->kref);
		}
	}

	spin_unlock_irqrestore(&obj->active_list_lock, flags);

	list_for_each_safe(pos, n, &signaled_pts) {
		struct sync_pt *pt =
			container_of(pos, struct sync_pt, signaled_list);

		list_del_init(pos);
		sync_fence_signal_pt(pt);
		kref_put(&pt->fence->kref, sync_fence_free);
	}
}
EXPORT_SYMBOL(sync_timeline_signal);

struct sync_pt *sync_pt_create(struct sync_timeline *parent, int size)
{
=======
=======
>>>>>>> v3.18
	struct sync_pt *pt, *next;

	trace_sync_timeline(obj);

	spin_lock_irqsave(&obj->child_list_lock, flags);

	list_for_each_entry_safe(pt, next, &obj->active_list_head,
				 active_list) {
		if (fence_is_signaled_locked(&pt->base))
			list_del(&pt->active_list);
	}

	spin_unlock_irqrestore(&obj->child_list_lock, flags);
}
EXPORT_SYMBOL(sync_timeline_signal);

struct sync_pt *sync_pt_create(struct sync_timeline *obj, int size)
{
	unsigned long flags;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	struct sync_pt *pt;

	if (size < sizeof(struct sync_pt))
		return NULL;

	pt = kzalloc(size, GFP_KERNEL);
	if (pt == NULL)
		return NULL;

<<<<<<< HEAD
<<<<<<< HEAD
	INIT_LIST_HEAD(&pt->active_list);
	kref_get(&parent->kref);
	sync_timeline_add_pt(parent, pt);

=======
=======
>>>>>>> v3.18
	spin_lock_irqsave(&obj->child_list_lock, flags);
	sync_timeline_get(obj);
	fence_init(&pt->base, &android_fence_ops, &obj->child_list_lock,
		   obj->context, ++obj->value);
	list_add_tail(&pt->child_list, &obj->child_list_head);
	INIT_LIST_HEAD(&pt->active_list);
	spin_unlock_irqrestore(&obj->child_list_lock, flags);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return pt;
}
EXPORT_SYMBOL(sync_pt_create);

void sync_pt_free(struct sync_pt *pt)
{
<<<<<<< HEAD
<<<<<<< HEAD
	if (pt->parent->ops->free_pt)
		pt->parent->ops->free_pt(pt);

	sync_timeline_remove_pt(pt);

	kref_put(&pt->parent->kref, sync_timeline_free);

	kfree(pt);
}
EXPORT_SYMBOL(sync_pt_free);

/* call with pt->parent->active_list_lock held */
static int _sync_pt_has_signaled(struct sync_pt *pt)
{
	int old_status = pt->status;

	if (!pt->status)
		pt->status = pt->parent->ops->has_signaled(pt);

	if (!pt->status && pt->parent->destroyed)
		pt->status = -ENOENT;

	if (pt->status != old_status)
		pt->timestamp = ktime_get();

	return pt->status;
}

static struct sync_pt *sync_pt_dup(struct sync_pt *pt)
{
	return pt->parent->ops->dup(pt);
}

/* Adds a sync pt to the active queue.  Called when added to a fence */
static void sync_pt_activate(struct sync_pt *pt)
{
	struct sync_timeline *obj = pt->parent;
	unsigned long flags;
	int err;

	spin_lock_irqsave(&obj->active_list_lock, flags);

	err = _sync_pt_has_signaled(pt);
	if (err != 0)
		goto out;

	list_add_tail(&pt->active_list, &obj->active_list_head);

out:
	spin_unlock_irqrestore(&obj->active_list_lock, flags);
}

static int sync_fence_release(struct inode *inode, struct file *file);
static unsigned int sync_fence_poll(struct file *file, poll_table *wait);
static long sync_fence_ioctl(struct file *file, unsigned int cmd,
			     unsigned long arg);


static const struct file_operations sync_fence_fops = {
	.release = sync_fence_release,
	.poll = sync_fence_poll,
	.unlocked_ioctl = sync_fence_ioctl,
	.compat_ioctl = sync_fence_ioctl,
};

static struct sync_fence *sync_fence_alloc(const char *name)
{
	struct sync_fence *fence;
	unsigned long flags;

	fence = kzalloc(sizeof(struct sync_fence), GFP_KERNEL);
=======
=======
>>>>>>> v3.18
	fence_put(&pt->base);
}
EXPORT_SYMBOL(sync_pt_free);

static struct sync_fence *sync_fence_alloc(int size, const char *name)
{
	struct sync_fence *fence;

	fence = kzalloc(size, GFP_KERNEL);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (fence == NULL)
		return NULL;

	fence->file = anon_inode_getfile("sync_fence", &sync_fence_fops,
					 fence, 0);
	if (IS_ERR(fence->file))
		goto err;

	kref_init(&fence->kref);
	strlcpy(fence->name, name, sizeof(fence->name));

<<<<<<< HEAD
<<<<<<< HEAD
	INIT_LIST_HEAD(&fence->pt_list_head);
	INIT_LIST_HEAD(&fence->waiter_list_head);
	spin_lock_init(&fence->waiter_list_lock);

	init_waitqueue_head(&fence->wq);

	spin_lock_irqsave(&sync_fence_list_lock, flags);
	list_add_tail(&fence->sync_fence_list, &sync_fence_list_head);
	spin_unlock_irqrestore(&sync_fence_list_lock, flags);

=======
	init_waitqueue_head(&fence->wq);

>>>>>>> v3.18
=======
	init_waitqueue_head(&fence->wq);

>>>>>>> v3.18
	return fence;

err:
	kfree(fence);
	return NULL;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* TODO: implement a create which takes more that one sync_pt */
struct sync_fence *sync_fence_create(const char *name, struct sync_pt *pt)
{
	struct sync_fence *fence;

	if (pt->fence)
		return NULL;

	fence = sync_fence_alloc(name);
	if (fence == NULL)
		return NULL;

	pt->fence = fence;
	list_add(&pt->pt_list, &fence->pt_list_head);
	sync_pt_activate(pt);

	/*
	 * signal the fence in case pt was activated before
	 * sync_pt_activate(pt) was called
	 */
	sync_fence_signal_pt(pt);

	return fence;
}
EXPORT_SYMBOL(sync_fence_create);

static int sync_fence_copy_pts(struct sync_fence *dst, struct sync_fence *src)
{
	struct list_head *pos;

	list_for_each(pos, &src->pt_list_head) {
		struct sync_pt *orig_pt =
			container_of(pos, struct sync_pt, pt_list);
		struct sync_pt *new_pt = sync_pt_dup(orig_pt);

		if (new_pt == NULL)
			return -ENOMEM;

		new_pt->fence = dst;
		list_add(&new_pt->pt_list, &dst->pt_list_head);
	}

	return 0;
}

static int sync_fence_merge_pts(struct sync_fence *dst, struct sync_fence *src)
{
	struct list_head *src_pos, *dst_pos, *n;

	list_for_each(src_pos, &src->pt_list_head) {
		struct sync_pt *src_pt =
			container_of(src_pos, struct sync_pt, pt_list);
		bool collapsed = false;

		list_for_each_safe(dst_pos, n, &dst->pt_list_head) {
			struct sync_pt *dst_pt =
				container_of(dst_pos, struct sync_pt, pt_list);
			/* collapse two sync_pts on the same timeline
			 * to a single sync_pt that will signal at
			 * the later of the two
			 */
			if (dst_pt->parent == src_pt->parent) {
				int cmp_val;
				int (*cmp_fn)
					(struct sync_pt *, struct sync_pt *);

				cmp_fn = dst_pt->parent->ops->compare;
				cmp_val = cmp_fn(dst_pt, src_pt);

				/*
				 * Out-of-order users like oneshot don't follow
				 * a timeline ordering.
				 */
				if (cmp_val != -cmp_fn(src_pt, dst_pt))
					break;

				if (cmp_val == -1) {
					struct sync_pt *new_pt =
						sync_pt_dup(src_pt);
					if (new_pt == NULL)
						return -ENOMEM;

					new_pt->fence = dst;
					list_replace(&dst_pt->pt_list,
						     &new_pt->pt_list);
					sync_pt_free(dst_pt);
				}
				collapsed = true;
				break;
			}
		}

		if (!collapsed) {
			struct sync_pt *new_pt = sync_pt_dup(src_pt);

			if (new_pt == NULL)
				return -ENOMEM;

			new_pt->fence = dst;
			list_add(&new_pt->pt_list, &dst->pt_list_head);
		}
	}

	return 0;
}

static void sync_fence_detach_pts(struct sync_fence *fence)
{
	struct list_head *pos, *n;

	list_for_each_safe(pos, n, &fence->pt_list_head) {
		struct sync_pt *pt = container_of(pos, struct sync_pt, pt_list);
		sync_timeline_remove_pt(pt);
	}
}

static void sync_fence_free_pts(struct sync_fence *fence)
{
	struct list_head *pos, *n;

	list_for_each_safe(pos, n, &fence->pt_list_head) {
		struct sync_pt *pt = container_of(pos, struct sync_pt, pt_list);
		sync_pt_free(pt);
	}
}
=======
=======
>>>>>>> v3.18
static void fence_check_cb_func(struct fence *f, struct fence_cb *cb)
{
	struct sync_fence_cb *check;
	struct sync_fence *fence;

	check = container_of(cb, struct sync_fence_cb, cb);
	fence = check->fence;

	if (atomic_dec_and_test(&fence->status))
		wake_up_all(&fence->wq);
}

/* TODO: implement a create which takes more that one sync_pt */
struct sync_fence *sync_fence_create(const char *name, struct sync_pt *pt)
{
	struct sync_fence *fence;

	fence = sync_fence_alloc(offsetof(struct sync_fence, cbs[1]), name);
	if (fence == NULL)
		return NULL;

	fence->num_fences = 1;
	atomic_set(&fence->status, 1);

	fence->cbs[0].sync_pt = &pt->base;
	fence->cbs[0].fence = fence;
	if (fence_add_callback(&pt->base, &fence->cbs[0].cb,
			       fence_check_cb_func))
		atomic_dec(&fence->status);

	sync_fence_debug_add(fence);

	return fence;
}
EXPORT_SYMBOL(sync_fence_create);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

struct sync_fence *sync_fence_fdget(int fd)
{
	struct file *file = fget(fd);

	if (file == NULL)
		return NULL;

	if (file->f_op != &sync_fence_fops)
		goto err;

	return file->private_data;

err:
	fput(file);
	return NULL;
}
EXPORT_SYMBOL(sync_fence_fdget);

void sync_fence_put(struct sync_fence *fence)
{
	fput(fence->file);
}
EXPORT_SYMBOL(sync_fence_put);

void sync_fence_install(struct sync_fence *fence, int fd)
{
	fd_install(fd, fence->file);
}
EXPORT_SYMBOL(sync_fence_install);

<<<<<<< HEAD
<<<<<<< HEAD
static int sync_fence_get_status(struct sync_fence *fence)
{
	struct list_head *pos;
	int status = 1;

	list_for_each(pos, &fence->pt_list_head) {
		struct sync_pt *pt = container_of(pos, struct sync_pt, pt_list);
		int pt_status = pt->status;

		if (pt_status < 0) {
			status = pt_status;
			break;
		} else if (status == 1) {
			status = pt_status;
		}
	}

	return status;
=======
=======
>>>>>>> v3.18
static void sync_fence_add_pt(struct sync_fence *fence,
			      int *i, struct fence *pt)
{
	fence->cbs[*i].sync_pt = pt;
	fence->cbs[*i].fence = fence;

	if (!fence_add_callback(pt, &fence->cbs[*i].cb, fence_check_cb_func)) {
		fence_get(pt);
		(*i)++;
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

struct sync_fence *sync_fence_merge(const char *name,
				    struct sync_fence *a, struct sync_fence *b)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct sync_fence *fence;
	struct list_head *pos;
	int err;

	fence = sync_fence_alloc(name);
	if (fence == NULL)
		return NULL;

	err = sync_fence_copy_pts(fence, a);
	if (err < 0)
		goto err;

	err = sync_fence_merge_pts(fence, b);
	if (err < 0)
		goto err;

	list_for_each(pos, &fence->pt_list_head) {
		struct sync_pt *pt =
			container_of(pos, struct sync_pt, pt_list);
		sync_pt_activate(pt);
	}

	/*
	 * signal the fence in case one of it's pts were activated before
	 * they were activated
	 */
	sync_fence_signal_pt(list_first_entry(&fence->pt_list_head,
					      struct sync_pt,
					      pt_list));

	return fence;
err:
	sync_fence_free_pts(fence);
	kfree(fence);
	return NULL;
}
EXPORT_SYMBOL(sync_fence_merge);

static void sync_fence_signal_pt(struct sync_pt *pt)
{
	LIST_HEAD(signaled_waiters);
	struct sync_fence *fence = pt->fence;
	struct list_head *pos;
	struct list_head *n;
	unsigned long flags;
	int status;

	status = sync_fence_get_status(fence);

	spin_lock_irqsave(&fence->waiter_list_lock, flags);
	/*
	 * this should protect against two threads racing on the signaled
	 * false -> true transition
	 */
	if (status && !fence->status) {
		list_for_each_safe(pos, n, &fence->waiter_list_head)
			list_move(pos, &signaled_waiters);

		fence->status = status;
	} else {
		status = 0;
	}
	spin_unlock_irqrestore(&fence->waiter_list_lock, flags);

	if (status) {
		list_for_each_safe(pos, n, &signaled_waiters) {
			struct sync_fence_waiter *waiter =
				container_of(pos, struct sync_fence_waiter,
					     waiter_list);

			list_del(pos);
			waiter->callback(fence, waiter);
		}
		wake_up(&fence->wq);
	}
=======
=======
>>>>>>> v3.18
	int num_fences = a->num_fences + b->num_fences;
	struct sync_fence *fence;
	int i, i_a, i_b;
	unsigned long size = offsetof(struct sync_fence, cbs[num_fences]);

	fence = sync_fence_alloc(size, name);
	if (fence == NULL)
		return NULL;

	atomic_set(&fence->status, num_fences);

	/*
	 * Assume sync_fence a and b are both ordered and have no
	 * duplicates with the same context.
	 *
	 * If a sync_fence can only be created with sync_fence_merge
	 * and sync_fence_create, this is a reasonable assumption.
	 */
	for (i = i_a = i_b = 0; i_a < a->num_fences && i_b < b->num_fences; ) {
		struct fence *pt_a = a->cbs[i_a].sync_pt;
		struct fence *pt_b = b->cbs[i_b].sync_pt;

		if (pt_a->context < pt_b->context) {
			sync_fence_add_pt(fence, &i, pt_a);

			i_a++;
		} else if (pt_a->context > pt_b->context) {
			sync_fence_add_pt(fence, &i, pt_b);

			i_b++;
		} else {
			if (pt_a->seqno - pt_b->seqno <= INT_MAX)
				sync_fence_add_pt(fence, &i, pt_a);
			else
				sync_fence_add_pt(fence, &i, pt_b);

			i_a++;
			i_b++;
		}
	}

	for (; i_a < a->num_fences; i_a++)
		sync_fence_add_pt(fence, &i, a->cbs[i_a].sync_pt);

	for (; i_b < b->num_fences; i_b++)
		sync_fence_add_pt(fence, &i, b->cbs[i_b].sync_pt);

	if (num_fences > i)
		atomic_sub(num_fences - i, &fence->status);
	fence->num_fences = i;

	sync_fence_debug_add(fence);
	return fence;
}
EXPORT_SYMBOL(sync_fence_merge);

int sync_fence_wake_up_wq(wait_queue_t *curr, unsigned mode,
				 int wake_flags, void *key)
{
	struct sync_fence_waiter *wait;

	wait = container_of(curr, struct sync_fence_waiter, work);
	list_del_init(&wait->work.task_list);

	wait->callback(wait->work.private, wait);
	return 1;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

int sync_fence_wait_async(struct sync_fence *fence,
			  struct sync_fence_waiter *waiter)
{
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long flags;
	int err = 0;

	spin_lock_irqsave(&fence->waiter_list_lock, flags);

	if (fence->status) {
		err = fence->status;
		goto out;
	}

	list_add_tail(&waiter->waiter_list, &fence->waiter_list_head);
out:
	spin_unlock_irqrestore(&fence->waiter_list_lock, flags);

	return err;
=======
=======
>>>>>>> v3.18
	int err = atomic_read(&fence->status);
	unsigned long flags;

	if (err < 0)
		return err;

	if (!err)
		return 1;

	init_waitqueue_func_entry(&waiter->work, sync_fence_wake_up_wq);
	waiter->work.private = fence;

	spin_lock_irqsave(&fence->wq.lock, flags);
	err = atomic_read(&fence->status);
	if (err > 0)
		__add_wait_queue_tail(&fence->wq, &waiter->work);
	spin_unlock_irqrestore(&fence->wq.lock, flags);

	if (err < 0)
		return err;

	return !err;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}
EXPORT_SYMBOL(sync_fence_wait_async);

int sync_fence_cancel_async(struct sync_fence *fence,
			     struct sync_fence_waiter *waiter)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct list_head *pos;
	struct list_head *n;
	unsigned long flags;
	int ret = -ENOENT;

	spin_lock_irqsave(&fence->waiter_list_lock, flags);
	/*
	 * Make sure waiter is still in waiter_list because it is possible for
	 * the waiter to be removed from the list while the callback is still
	 * pending.
	 */
	list_for_each_safe(pos, n, &fence->waiter_list_head) {
		struct sync_fence_waiter *list_waiter =
			container_of(pos, struct sync_fence_waiter,
				     waiter_list);
		if (list_waiter == waiter) {
			list_del(pos);
			ret = 0;
			break;
		}
	}
	spin_unlock_irqrestore(&fence->waiter_list_lock, flags);
=======
=======
>>>>>>> v3.18
	unsigned long flags;
	int ret = 0;

	spin_lock_irqsave(&fence->wq.lock, flags);
	if (!list_empty(&waiter->work.task_list))
		list_del_init(&waiter->work.task_list);
	else
		ret = -ENOENT;
	spin_unlock_irqrestore(&fence->wq.lock, flags);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return ret;
}
EXPORT_SYMBOL(sync_fence_cancel_async);

<<<<<<< HEAD
<<<<<<< HEAD
static bool sync_fence_check(struct sync_fence *fence)
{
	/*
	 * Make sure that reads to fence->status are ordered with the
	 * wait queue event triggering
	 */
	smp_rmb();
	return fence->status != 0;
}

static const char *sync_status_str(int status)
{
	if (status > 0)
		return "signaled";
	else if (status == 0)
		return "active";
	else
		return "error";
}

static void sync_pt_log(struct sync_pt *pt, bool pt_callback)
{
	int status = pt->status;
	pr_cont("  %s_pt %s",
		   pt->parent->name,
		   sync_status_str(status));

	if (pt->status) {
		struct timeval tv = ktime_to_timeval(pt->timestamp);
		pr_cont("@%ld.%06ld", tv.tv_sec, tv.tv_usec);
	}

	if (pt->parent->ops->timeline_value_str &&
	    pt->parent->ops->pt_value_str) {
		char value[64];
		pt->parent->ops->pt_value_str(pt, value, sizeof(value));
		pr_cont(": %s", value);
		pt->parent->ops->timeline_value_str(pt->parent, value,
					    sizeof(value));
		pr_cont(" / %s", value);
	}

	pr_cont("\n");

	/* Show additional details for active fences */
	if (pt->status == 0 && pt->parent->ops->pt_log && pt_callback)
		pt->parent->ops->pt_log(pt);
}

void _sync_fence_log(struct sync_fence *fence, bool pt_callback)
{
	struct list_head *pos;
	unsigned long flags;

	pr_info("[%p] %s: %s\n", fence, fence->name,
		sync_status_str(fence->status));

	pr_info("waiters:\n");

	spin_lock_irqsave(&fence->waiter_list_lock, flags);
	list_for_each(pos, &fence->waiter_list_head) {
		struct sync_fence_waiter *waiter =
			container_of(pos, struct sync_fence_waiter,
				     waiter_list);

		pr_info(" %pF\n", waiter->callback);
	}
	spin_unlock_irqrestore(&fence->waiter_list_lock, flags);

	pr_info("syncpoints:\n");
	list_for_each(pos, &fence->pt_list_head) {
		struct sync_pt *pt =
			container_of(pos, struct sync_pt, pt_list);
		sync_pt_log(pt, pt_callback);
	}
}

void sync_fence_log(struct sync_fence *fence)
{
	_sync_fence_log(fence, false);
}
EXPORT_SYMBOL(sync_fence_log);

int sync_fence_wait(struct sync_fence *fence, long timeout)
{
	int err = 0;
	struct sync_pt *pt;

	trace_sync_wait(fence, 1);
	list_for_each_entry(pt, &fence->pt_list_head, pt_list)
		trace_sync_pt(pt);

	if (timeout > 0) {
		timeout = msecs_to_jiffies(timeout);
		err = wait_event_interruptible_timeout(fence->wq,
						       sync_fence_check(fence),
						       timeout);
	} else if (timeout < 0) {
		err = wait_event_interruptible(fence->wq,
					       sync_fence_check(fence));
	}
	trace_sync_wait(fence, 0);

	if (err < 0)
		return err;

	if (fence->status < 0) {
		pr_info("fence error %d on [%pK]\n", fence->status, fence);
		_sync_fence_log(fence, true);
		return fence->status;
	}

	if (fence->status == 0) {
		if (timeout > 0) {
			pr_info("fence timeout on [%pK] after %dms\n", fence,
				jiffies_to_msecs(timeout));
			_sync_fence_log(fence, true);
		}
		return -ETIME;
	}

	return 0;
}
EXPORT_SYMBOL(sync_fence_wait);
=======
=======
>>>>>>> v3.18
int sync_fence_wait(struct sync_fence *fence, long timeout)
{
	long ret;
	int i;

	if (timeout < 0)
		timeout = MAX_SCHEDULE_TIMEOUT;
	else
		timeout = msecs_to_jiffies(timeout);

	trace_sync_wait(fence, 1);
	for (i = 0; i < fence->num_fences; ++i)
		trace_sync_pt(fence->cbs[i].sync_pt);
	ret = wait_event_interruptible_timeout(fence->wq,
					       atomic_read(&fence->status) <= 0,
					       timeout);
	trace_sync_wait(fence, 0);

	if (ret < 0) {
		return ret;
	} else if (ret == 0) {
		if (timeout) {
			pr_info("fence timeout on [%p] after %dms\n", fence,
				jiffies_to_msecs(timeout));
			sync_dump();
		}
		return -ETIME;
	}

	ret = atomic_read(&fence->status);
	if (ret) {
		pr_info("fence error %ld on [%p]\n", ret, fence);
		sync_dump();
	}
	return ret;
}
EXPORT_SYMBOL(sync_fence_wait);

static const char *android_fence_get_driver_name(struct fence *fence)
{
	struct sync_pt *pt = container_of(fence, struct sync_pt, base);
	struct sync_timeline *parent = sync_pt_parent(pt);

	return parent->ops->driver_name;
}

static const char *android_fence_get_timeline_name(struct fence *fence)
{
	struct sync_pt *pt = container_of(fence, struct sync_pt, base);
	struct sync_timeline *parent = sync_pt_parent(pt);

	return parent->name;
}

static void android_fence_release(struct fence *fence)
{
	struct sync_pt *pt = container_of(fence, struct sync_pt, base);
	struct sync_timeline *parent = sync_pt_parent(pt);
	unsigned long flags;

	spin_lock_irqsave(fence->lock, flags);
	list_del(&pt->child_list);
	if (WARN_ON_ONCE(!list_empty(&pt->active_list)))
		list_del(&pt->active_list);
	spin_unlock_irqrestore(fence->lock, flags);

	if (parent->ops->free_pt)
		parent->ops->free_pt(pt);

	sync_timeline_put(parent);
	fence_free(&pt->base);
}

static bool android_fence_signaled(struct fence *fence)
{
	struct sync_pt *pt = container_of(fence, struct sync_pt, base);
	struct sync_timeline *parent = sync_pt_parent(pt);
	int ret;

	ret = parent->ops->has_signaled(pt);
	if (ret < 0)
		fence->status = ret;
	return ret;
}

static bool android_fence_enable_signaling(struct fence *fence)
{
	struct sync_pt *pt = container_of(fence, struct sync_pt, base);
	struct sync_timeline *parent = sync_pt_parent(pt);

	if (android_fence_signaled(fence))
		return false;

	list_add_tail(&pt->active_list, &parent->active_list_head);
	return true;
}

static int android_fence_fill_driver_data(struct fence *fence,
					  void *data, int size)
{
	struct sync_pt *pt = container_of(fence, struct sync_pt, base);
	struct sync_timeline *parent = sync_pt_parent(pt);

	if (!parent->ops->fill_driver_data)
		return 0;
	return parent->ops->fill_driver_data(pt, data, size);
}

static void android_fence_value_str(struct fence *fence,
				    char *str, int size)
{
	struct sync_pt *pt = container_of(fence, struct sync_pt, base);
	struct sync_timeline *parent = sync_pt_parent(pt);

	if (!parent->ops->pt_value_str) {
		if (size)
			*str = 0;
		return;
	}
	parent->ops->pt_value_str(pt, str, size);
}

static void android_fence_timeline_value_str(struct fence *fence,
					     char *str, int size)
{
	struct sync_pt *pt = container_of(fence, struct sync_pt, base);
	struct sync_timeline *parent = sync_pt_parent(pt);

	if (!parent->ops->timeline_value_str) {
		if (size)
			*str = 0;
		return;
	}
	parent->ops->timeline_value_str(parent, str, size);
}

static const struct fence_ops android_fence_ops = {
	.get_driver_name = android_fence_get_driver_name,
	.get_timeline_name = android_fence_get_timeline_name,
	.enable_signaling = android_fence_enable_signaling,
	.signaled = android_fence_signaled,
	.wait = fence_default_wait,
	.release = android_fence_release,
	.fill_driver_data = android_fence_fill_driver_data,
	.fence_value_str = android_fence_value_str,
	.timeline_value_str = android_fence_timeline_value_str,
};
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

static void sync_fence_free(struct kref *kref)
{
	struct sync_fence *fence = container_of(kref, struct sync_fence, kref);
<<<<<<< HEAD
<<<<<<< HEAD

	sync_fence_free_pts(fence);
=======
=======
>>>>>>> v3.18
	int i, status = atomic_read(&fence->status);

	for (i = 0; i < fence->num_fences; ++i) {
		if (status)
			fence_remove_callback(fence->cbs[i].sync_pt,
					      &fence->cbs[i].cb);
		fence_put(fence->cbs[i].sync_pt);
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	kfree(fence);
}

static int sync_fence_release(struct inode *inode, struct file *file)
{
	struct sync_fence *fence = file->private_data;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long flags;

	/*
	 * We need to remove all ways to access this fence before droping
	 * our ref.
	 *
	 * start with its membership in the global fence list
	 */
	spin_lock_irqsave(&sync_fence_list_lock, flags);
	list_del(&fence->sync_fence_list);
	spin_unlock_irqrestore(&sync_fence_list_lock, flags);

	/*
	 * remove its pts from their parents so that sync_timeline_signal()
	 * can't reference the fence.
	 */
	sync_fence_detach_pts(fence);

	kref_put(&fence->kref, sync_fence_free);

=======
=======
>>>>>>> v3.18

	sync_fence_debug_remove(fence);

	kref_put(&fence->kref, sync_fence_free);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
}

static unsigned int sync_fence_poll(struct file *file, poll_table *wait)
{
	struct sync_fence *fence = file->private_data;
<<<<<<< HEAD
<<<<<<< HEAD

	poll_wait(file, &fence->wq, wait);

	/*
	 * Make sure that reads to fence->status are ordered with the
	 * wait queue event triggering
	 */
	smp_rmb();

	if (fence->status == 1)
		return POLLIN;
	else if (fence->status < 0)
		return POLLERR;
	else
		return 0;
=======
=======
>>>>>>> v3.18
	int status;

	poll_wait(file, &fence->wq, wait);

	status = atomic_read(&fence->status);

	if (!status)
		return POLLIN;
	else if (status < 0)
		return POLLERR;
	return 0;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static long sync_fence_ioctl_wait(struct sync_fence *fence, unsigned long arg)
{
	__s32 value;

	if (copy_from_user(&value, (void __user *)arg, sizeof(value)))
		return -EFAULT;

	return sync_fence_wait(fence, value);
}

static long sync_fence_ioctl_merge(struct sync_fence *fence, unsigned long arg)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int fd = get_unused_fd();
=======
	int fd = get_unused_fd_flags(O_CLOEXEC);
>>>>>>> v3.18
=======
	int fd = get_unused_fd_flags(O_CLOEXEC);
>>>>>>> v3.18
	int err;
	struct sync_fence *fence2, *fence3;
	struct sync_merge_data data;

	if (fd < 0)
		return fd;

	if (copy_from_user(&data, (void __user *)arg, sizeof(data))) {
		err = -EFAULT;
		goto err_put_fd;
	}

	fence2 = sync_fence_fdget(data.fd2);
	if (fence2 == NULL) {
		err = -ENOENT;
		goto err_put_fd;
	}

	data.name[sizeof(data.name) - 1] = '\0';
	fence3 = sync_fence_merge(data.name, fence, fence2);
	if (fence3 == NULL) {
		err = -ENOMEM;
		goto err_put_fence2;
	}

	data.fence = fd;
	if (copy_to_user((void __user *)arg, &data, sizeof(data))) {
		err = -EFAULT;
		goto err_put_fence3;
	}

	sync_fence_install(fence3, fd);
	sync_fence_put(fence2);
	return 0;

err_put_fence3:
	sync_fence_put(fence3);

err_put_fence2:
	sync_fence_put(fence2);

err_put_fd:
	put_unused_fd(fd);
	return err;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int sync_fill_pt_info(struct sync_pt *pt, void *data, int size)
=======
static int sync_fill_pt_info(struct fence *fence, void *data, int size)
>>>>>>> v3.18
=======
static int sync_fill_pt_info(struct fence *fence, void *data, int size)
>>>>>>> v3.18
{
	struct sync_pt_info *info = data;
	int ret;

	if (size < sizeof(struct sync_pt_info))
		return -ENOMEM;

	info->len = sizeof(struct sync_pt_info);

<<<<<<< HEAD
<<<<<<< HEAD
	if (pt->parent->ops->fill_driver_data) {
		ret = pt->parent->ops->fill_driver_data(pt, info->driver_data,
							size - sizeof(*info));
=======
	if (fence->ops->fill_driver_data) {
		ret = fence->ops->fill_driver_data(fence, info->driver_data,
						   size - sizeof(*info));
>>>>>>> v3.18
=======
	if (fence->ops->fill_driver_data) {
		ret = fence->ops->fill_driver_data(fence, info->driver_data,
						   size - sizeof(*info));
>>>>>>> v3.18
		if (ret < 0)
			return ret;

		info->len += ret;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	strlcpy(info->obj_name, pt->parent->name, sizeof(info->obj_name));
	strlcpy(info->driver_name, pt->parent->ops->driver_name,
		sizeof(info->driver_name));
	info->status = pt->status;
	info->timestamp_ns = ktime_to_ns(pt->timestamp);
=======
=======
>>>>>>> v3.18
	strlcpy(info->obj_name, fence->ops->get_timeline_name(fence),
		sizeof(info->obj_name));
	strlcpy(info->driver_name, fence->ops->get_driver_name(fence),
		sizeof(info->driver_name));
	if (fence_is_signaled(fence))
		info->status = fence->status >= 0 ? 1 : fence->status;
	else
		info->status = 0;
	info->timestamp_ns = ktime_to_ns(fence->timestamp);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return info->len;
}

static long sync_fence_ioctl_fence_info(struct sync_fence *fence,
					unsigned long arg)
{
	struct sync_fence_info_data *data;
<<<<<<< HEAD
<<<<<<< HEAD
	struct list_head *pos;
	__u32 size;
	__u32 len = 0;
	int ret;
=======
	__u32 size;
	__u32 len = 0;
	int ret, i;
>>>>>>> v3.18
=======
	__u32 size;
	__u32 len = 0;
	int ret, i;
>>>>>>> v3.18

	if (copy_from_user(&size, (void __user *)arg, sizeof(size)))
		return -EFAULT;

	if (size < sizeof(struct sync_fence_info_data))
		return -EINVAL;

	if (size > 4096)
		size = 4096;

	data = kzalloc(size, GFP_KERNEL);
	if (data == NULL)
		return -ENOMEM;

	strlcpy(data->name, fence->name, sizeof(data->name));
<<<<<<< HEAD
<<<<<<< HEAD
	data->status = fence->status;
	len = sizeof(struct sync_fence_info_data);

	list_for_each(pos, &fence->pt_list_head) {
		struct sync_pt *pt =
			container_of(pos, struct sync_pt, pt_list);
=======
=======
>>>>>>> v3.18
	data->status = atomic_read(&fence->status);
	if (data->status >= 0)
		data->status = !data->status;

	len = sizeof(struct sync_fence_info_data);

	for (i = 0; i < fence->num_fences; ++i) {
		struct fence *pt = fence->cbs[i].sync_pt;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

		ret = sync_fill_pt_info(pt, (u8 *)data + len, size - len);

		if (ret < 0)
			goto out;

		len += ret;
	}

	data->len = len;

	if (copy_to_user((void __user *)arg, data, len))
		ret = -EFAULT;
	else
		ret = 0;

out:
	kfree(data);

	return ret;
}

static long sync_fence_ioctl(struct file *file, unsigned int cmd,
			     unsigned long arg)
{
	struct sync_fence *fence = file->private_data;
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
	switch (cmd) {
	case SYNC_IOC_WAIT:
		return sync_fence_ioctl_wait(fence, arg);

	case SYNC_IOC_MERGE:
		return sync_fence_ioctl_merge(fence, arg);

	case SYNC_IOC_FENCE_INFO:
		return sync_fence_ioctl_fence_info(fence, arg);

	default:
		return -ENOTTY;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_DEBUG_FS
static void sync_print_pt(struct seq_file *s, struct sync_pt *pt, bool fence)
{
	int status = pt->status;
	seq_printf(s, "  %s%spt %s",
		   fence ? pt->parent->name : "",
		   fence ? "_" : "",
		   sync_status_str(status));
	if (pt->status) {
		struct timeval tv = ktime_to_timeval(pt->timestamp);
		seq_printf(s, "@%ld.%06ld", tv.tv_sec, tv.tv_usec);
	}

	if (pt->parent->ops->timeline_value_str &&
	    pt->parent->ops->pt_value_str) {
		char value[64];
		pt->parent->ops->pt_value_str(pt, value, sizeof(value));
		seq_printf(s, ": %s", value);
		if (fence) {
			pt->parent->ops->timeline_value_str(pt->parent, value,
						    sizeof(value));
			seq_printf(s, " / %s", value);
		}
	} else if (pt->parent->ops->print_pt) {
		seq_printf(s, ": ");
		pt->parent->ops->print_pt(s, pt);
	}

	seq_printf(s, "\n");
}

static void sync_print_obj(struct seq_file *s, struct sync_timeline *obj)
{
	struct list_head *pos;
	unsigned long flags;

	seq_printf(s, "%s %s", obj->name, obj->ops->driver_name);

	if (obj->ops->timeline_value_str) {
		char value[64];
		obj->ops->timeline_value_str(obj, value, sizeof(value));
		seq_printf(s, ": %s", value);
	} else if (obj->ops->print_obj) {
		seq_printf(s, ": ");
		obj->ops->print_obj(s, obj);
	}

	seq_printf(s, "\n");

	spin_lock_irqsave(&obj->child_list_lock, flags);
	list_for_each(pos, &obj->child_list_head) {
		struct sync_pt *pt =
			container_of(pos, struct sync_pt, child_list);
		sync_print_pt(s, pt, false);
	}
	spin_unlock_irqrestore(&obj->child_list_lock, flags);
}

static void sync_print_fence(struct seq_file *s, struct sync_fence *fence)
{
	struct list_head *pos;
	unsigned long flags;

	seq_printf(s, "[%pK] %s: %s\n", fence, fence->name,
		   sync_status_str(fence->status));

	list_for_each(pos, &fence->pt_list_head) {
		struct sync_pt *pt =
			container_of(pos, struct sync_pt, pt_list);
		sync_print_pt(s, pt, true);
	}

	spin_lock_irqsave(&fence->waiter_list_lock, flags);
	list_for_each(pos, &fence->waiter_list_head) {
		struct sync_fence_waiter *waiter =
			container_of(pos, struct sync_fence_waiter,
				     waiter_list);

		seq_printf(s, "waiter %pF\n", waiter->callback);
	}
	spin_unlock_irqrestore(&fence->waiter_list_lock, flags);
}

static int sync_debugfs_show(struct seq_file *s, void *unused)
{
	unsigned long flags;
	struct list_head *pos;

	seq_printf(s, "objs:\n--------------\n");

	spin_lock_irqsave(&sync_timeline_list_lock, flags);
	list_for_each(pos, &sync_timeline_list_head) {
		struct sync_timeline *obj =
			container_of(pos, struct sync_timeline,
				     sync_timeline_list);

		sync_print_obj(s, obj);
		seq_printf(s, "\n");
	}
	spin_unlock_irqrestore(&sync_timeline_list_lock, flags);

	seq_printf(s, "fences:\n--------------\n");

	spin_lock_irqsave(&sync_fence_list_lock, flags);
	list_for_each(pos, &sync_fence_list_head) {
		struct sync_fence *fence =
			container_of(pos, struct sync_fence, sync_fence_list);

		sync_print_fence(s, fence);
		seq_printf(s, "\n");
	}
	spin_unlock_irqrestore(&sync_fence_list_lock, flags);
	return 0;
}

static int sync_debugfs_open(struct inode *inode, struct file *file)
{
	return single_open(file, sync_debugfs_show, inode->i_private);
}

static const struct file_operations sync_debugfs_fops = {
	.open           = sync_debugfs_open,
	.read           = seq_read,
	.llseek         = seq_lseek,
	.release        = single_release,
};

static __init int sync_debugfs_init(void)
{
	debugfs_create_file("sync", S_IRUGO, NULL, NULL, &sync_debugfs_fops);
	return 0;
}
late_initcall(sync_debugfs_init);
#endif
=======
=======
>>>>>>> v3.18
static const struct file_operations sync_fence_fops = {
	.release = sync_fence_release,
	.poll = sync_fence_poll,
	.unlocked_ioctl = sync_fence_ioctl,
	.compat_ioctl = sync_fence_ioctl,
};

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
