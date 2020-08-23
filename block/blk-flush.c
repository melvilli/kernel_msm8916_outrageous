/*
 * Functions to sequence FLUSH and FUA writes.
 *
 * Copyright (C) 2011		Max Planck Institute for Gravitational Physics
 * Copyright (C) 2011		Tejun Heo <tj@kernel.org>
 *
 * This file is released under the GPLv2.
 *
 * REQ_{FLUSH|FUA} requests are decomposed to sequences consisted of three
 * optional steps - PREFLUSH, DATA and POSTFLUSH - according to the request
 * properties and hardware capability.
 *
 * If a request doesn't have data, only REQ_FLUSH makes sense, which
 * indicates a simple flush request.  If there is data, REQ_FLUSH indicates
 * that the device cache should be flushed before the data is executed, and
 * REQ_FUA means that the data must be on non-volatile media on request
 * completion.
 *
 * If the device doesn't have writeback cache, FLUSH and FUA don't make any
 * difference.  The requests are either completed immediately if there's no
 * data or executed as normal requests otherwise.
 *
 * If the device has writeback cache and supports FUA, REQ_FLUSH is
 * translated to PREFLUSH but REQ_FUA is passed down directly with DATA.
 *
 * If the device has writeback cache and doesn't support FUA, REQ_FLUSH is
 * translated to PREFLUSH and REQ_FUA to POSTFLUSH.
 *
 * The actual execution of flush is double buffered.  Whenever a request
 * needs to execute PRE or POSTFLUSH, it queues at
<<<<<<< HEAD
<<<<<<< HEAD
 * q->flush_queue[q->flush_pending_idx].  Once certain criteria are met, a
=======
 * fq->flush_queue[fq->flush_pending_idx].  Once certain criteria are met, a
>>>>>>> v3.18
=======
 * fq->flush_queue[fq->flush_pending_idx].  Once certain criteria are met, a
>>>>>>> v3.18
 * flush is issued and the pending_idx is toggled.  When the flush
 * completes, all the requests which were pending are proceeded to the next
 * step.  This allows arbitrary merging of different types of FLUSH/FUA
 * requests.
 *
 * Currently, the following conditions are used to determine when to issue
 * flush.
 *
 * C1. At any given time, only one flush shall be in progress.  This makes
 *     double buffering sufficient.
 *
 * C2. Flush is deferred if any request is executing DATA of its sequence.
 *     This avoids issuing separate POSTFLUSHes for requests which shared
 *     PREFLUSH.
 *
 * C3. The second condition is ignored if there is a request which has
 *     waited longer than FLUSH_PENDING_TIMEOUT.  This is to avoid
 *     starvation in the unlikely case where there are continuous stream of
 *     FUA (without FLUSH) requests.
 *
 * For devices which support FUA, it isn't clear whether C2 (and thus C3)
 * is beneficial.
 *
 * Note that a sequenced FLUSH/FUA request with DATA is completed twice.
 * Once while executing DATA and again after the whole sequence is
 * complete.  The first completion updates the contained bio but doesn't
 * finish it so that the bio submitter is notified only after the whole
 * sequence is complete.  This is implemented by testing REQ_FLUSH_SEQ in
 * req_bio_endio().
 *
 * The above peculiarity requires that each FLUSH/FUA request has only one
 * bio attached to it, which is guaranteed as they aren't allowed to be
 * merged in the usual way.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/bio.h>
#include <linux/blkdev.h>
#include <linux/gfp.h>
<<<<<<< HEAD
<<<<<<< HEAD

#include "blk.h"
=======
=======
>>>>>>> v3.18
#include <linux/blk-mq.h>

#include "blk.h"
#include "blk-mq.h"
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

/* FLUSH/FUA sequences */
enum {
	REQ_FSEQ_PREFLUSH	= (1 << 0), /* pre-flushing in progress */
	REQ_FSEQ_DATA		= (1 << 1), /* data write in progress */
	REQ_FSEQ_POSTFLUSH	= (1 << 2), /* post-flushing in progress */
	REQ_FSEQ_DONE		= (1 << 3),

	REQ_FSEQ_ACTIONS	= REQ_FSEQ_PREFLUSH | REQ_FSEQ_DATA |
				  REQ_FSEQ_POSTFLUSH,

	/*
	 * If flush has been pending longer than the following timeout,
	 * it's issued even if flush_data requests are still in flight.
	 */
	FLUSH_PENDING_TIMEOUT	= 5 * HZ,
};

<<<<<<< HEAD
<<<<<<< HEAD
static bool blk_kick_flush(struct request_queue *q);
=======
static bool blk_kick_flush(struct request_queue *q,
			   struct blk_flush_queue *fq);
>>>>>>> v3.18
=======
static bool blk_kick_flush(struct request_queue *q,
			   struct blk_flush_queue *fq);
>>>>>>> v3.18

static unsigned int blk_flush_policy(unsigned int fflags, struct request *rq)
{
	unsigned int policy = 0;

	if (blk_rq_sectors(rq))
		policy |= REQ_FSEQ_DATA;

	if (fflags & REQ_FLUSH) {
		if (rq->cmd_flags & REQ_FLUSH)
			policy |= REQ_FSEQ_PREFLUSH;
		if (!(fflags & REQ_FUA) && (rq->cmd_flags & REQ_FUA))
			policy |= REQ_FSEQ_POSTFLUSH;
	}
	return policy;
}

static unsigned int blk_flush_cur_seq(struct request *rq)
{
	return 1 << ffz(rq->flush.seq);
}

static void blk_flush_restore_request(struct request *rq)
{
	/*
	 * After flush data completion, @rq->bio is %NULL but we need to
	 * complete the bio again.  @rq->biotail is guaranteed to equal the
	 * original @rq->bio.  Restore it.
	 */
	rq->bio = rq->biotail;

	/* make @rq a normal request */
	rq->cmd_flags &= ~REQ_FLUSH_SEQ;
	rq->end_io = rq->flush.saved_end_io;
}

<<<<<<< HEAD
<<<<<<< HEAD
/**
 * blk_flush_complete_seq - complete flush sequence
 * @rq: FLUSH/FUA request being sequenced
=======
=======
>>>>>>> v3.18
static bool blk_flush_queue_rq(struct request *rq, bool add_front)
{
	if (rq->q->mq_ops) {
		struct request_queue *q = rq->q;

		blk_mq_add_to_requeue_list(rq, add_front);
		blk_mq_kick_requeue_list(q);
		return false;
	} else {
		if (add_front)
			list_add(&rq->queuelist, &rq->q->queue_head);
		else
			list_add_tail(&rq->queuelist, &rq->q->queue_head);
		return true;
	}
}

/**
 * blk_flush_complete_seq - complete flush sequence
 * @rq: FLUSH/FUA request being sequenced
 * @fq: flush queue
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * @seq: sequences to complete (mask of %REQ_FSEQ_*, can be zero)
 * @error: whether an error occurred
 *
 * @rq just completed @seq part of its flush sequence, record the
 * completion and trigger the next step.
 *
 * CONTEXT:
<<<<<<< HEAD
<<<<<<< HEAD
 * spin_lock_irq(q->queue_lock)
=======
 * spin_lock_irq(q->queue_lock or fq->mq_flush_lock)
>>>>>>> v3.18
=======
 * spin_lock_irq(q->queue_lock or fq->mq_flush_lock)
>>>>>>> v3.18
 *
 * RETURNS:
 * %true if requests were added to the dispatch queue, %false otherwise.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static bool blk_flush_complete_seq(struct request *rq, unsigned int seq,
				   int error)
{
	struct request_queue *q = rq->q;
	struct list_head *pending = &q->flush_queue[q->flush_pending_idx];
	bool queued = false;
=======
=======
>>>>>>> v3.18
static bool blk_flush_complete_seq(struct request *rq,
				   struct blk_flush_queue *fq,
				   unsigned int seq, int error)
{
	struct request_queue *q = rq->q;
	struct list_head *pending = &fq->flush_queue[fq->flush_pending_idx];
	bool queued = false, kicked;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	BUG_ON(rq->flush.seq & seq);
	rq->flush.seq |= seq;

	if (likely(!error))
		seq = blk_flush_cur_seq(rq);
	else
		seq = REQ_FSEQ_DONE;

	switch (seq) {
	case REQ_FSEQ_PREFLUSH:
	case REQ_FSEQ_POSTFLUSH:
		/* queue for flush */
		if (list_empty(pending))
<<<<<<< HEAD
<<<<<<< HEAD
			q->flush_pending_since = jiffies;
=======
			fq->flush_pending_since = jiffies;
>>>>>>> v3.18
=======
			fq->flush_pending_since = jiffies;
>>>>>>> v3.18
		list_move_tail(&rq->flush.list, pending);
		break;

	case REQ_FSEQ_DATA:
<<<<<<< HEAD
<<<<<<< HEAD
		list_move_tail(&rq->flush.list, &q->flush_data_in_flight);
		list_add(&rq->queuelist, &q->queue_head);
		queued = true;
=======
		list_move_tail(&rq->flush.list, &fq->flush_data_in_flight);
		queued = blk_flush_queue_rq(rq, true);
>>>>>>> v3.18
=======
		list_move_tail(&rq->flush.list, &fq->flush_data_in_flight);
		queued = blk_flush_queue_rq(rq, true);
>>>>>>> v3.18
		break;

	case REQ_FSEQ_DONE:
		/*
		 * @rq was previously adjusted by blk_flush_issue() for
		 * flush sequencing and may already have gone through the
		 * flush data request completion path.  Restore @rq for
		 * normal completion and end it.
		 */
		BUG_ON(!list_empty(&rq->queuelist));
		list_del_init(&rq->flush.list);
		blk_flush_restore_request(rq);
<<<<<<< HEAD
<<<<<<< HEAD
		__blk_end_request_all(rq, error);
=======
=======
>>>>>>> v3.18
		if (q->mq_ops)
			blk_mq_end_request(rq, error);
		else
			__blk_end_request_all(rq, error);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		break;

	default:
		BUG();
	}

<<<<<<< HEAD
<<<<<<< HEAD
	return blk_kick_flush(q) | queued;
=======
	kicked = blk_kick_flush(q, fq);
	return kicked | queued;
>>>>>>> v3.18
=======
	kicked = blk_kick_flush(q, fq);
	return kicked | queued;
>>>>>>> v3.18
}

static void flush_end_io(struct request *flush_rq, int error)
{
	struct request_queue *q = flush_rq->q;
<<<<<<< HEAD
<<<<<<< HEAD
	struct list_head *running = &q->flush_queue[q->flush_running_idx];
	bool queued = false;
	struct request *rq, *n;

	BUG_ON(q->flush_pending_idx == q->flush_running_idx);

	/* account completion of the flush request */
	q->flush_running_idx ^= 1;
	elv_completed_request(q, flush_rq);
=======
=======
>>>>>>> v3.18
	struct list_head *running;
	bool queued = false;
	struct request *rq, *n;
	unsigned long flags = 0;
	struct blk_flush_queue *fq = blk_get_flush_queue(q, flush_rq->mq_ctx);

	if (q->mq_ops) {
		spin_lock_irqsave(&fq->mq_flush_lock, flags);
		flush_rq->tag = -1;
	}

	running = &fq->flush_queue[fq->flush_running_idx];
	BUG_ON(fq->flush_pending_idx == fq->flush_running_idx);

	/* account completion of the flush request */
	fq->flush_running_idx ^= 1;

	if (!q->mq_ops)
		elv_completed_request(q, flush_rq);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* and push the waiting requests to the next stage */
	list_for_each_entry_safe(rq, n, running, flush.list) {
		unsigned int seq = blk_flush_cur_seq(rq);

		BUG_ON(seq != REQ_FSEQ_PREFLUSH && seq != REQ_FSEQ_POSTFLUSH);
<<<<<<< HEAD
<<<<<<< HEAD
		queued |= blk_flush_complete_seq(rq, seq, error);
=======
		queued |= blk_flush_complete_seq(rq, fq, seq, error);
>>>>>>> v3.18
=======
		queued |= blk_flush_complete_seq(rq, fq, seq, error);
>>>>>>> v3.18
	}

	/*
	 * Kick the queue to avoid stall for two cases:
	 * 1. Moving a request silently to empty queue_head may stall the
	 * queue.
	 * 2. When flush request is running in non-queueable queue, the
	 * queue is hold. Restart the queue after flush request is finished
	 * to avoid stall.
	 * This function is called from request completion path and calling
	 * directly into request_fn may confuse the driver.  Always use
	 * kblockd.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (queued || q->flush_queue_delayed)
		blk_run_queue_async(q);
	q->flush_queue_delayed = 0;
=======
=======
>>>>>>> v3.18
	if (queued || fq->flush_queue_delayed) {
		WARN_ON(q->mq_ops);
		blk_run_queue_async(q);
	}
	fq->flush_queue_delayed = 0;
	if (q->mq_ops)
		spin_unlock_irqrestore(&fq->mq_flush_lock, flags);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/**
 * blk_kick_flush - consider issuing flush request
 * @q: request_queue being kicked
<<<<<<< HEAD
<<<<<<< HEAD
=======
 * @fq: flush queue
>>>>>>> v3.18
=======
 * @fq: flush queue
>>>>>>> v3.18
 *
 * Flush related states of @q have changed, consider issuing flush request.
 * Please read the comment at the top of this file for more info.
 *
 * CONTEXT:
<<<<<<< HEAD
<<<<<<< HEAD
 * spin_lock_irq(q->queue_lock)
=======
 * spin_lock_irq(q->queue_lock or fq->mq_flush_lock)
>>>>>>> v3.18
=======
 * spin_lock_irq(q->queue_lock or fq->mq_flush_lock)
>>>>>>> v3.18
 *
 * RETURNS:
 * %true if flush was issued, %false otherwise.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static bool blk_kick_flush(struct request_queue *q)
{
	struct list_head *pending = &q->flush_queue[q->flush_pending_idx];
	struct request *first_rq =
		list_first_entry(pending, struct request, flush.list);

	/* C1 described at the top of this file */
	if (q->flush_pending_idx != q->flush_running_idx || list_empty(pending))
		return false;

	/* C2 and C3 */
	if (!list_empty(&q->flush_data_in_flight) &&
	    time_before(jiffies,
			q->flush_pending_since + FLUSH_PENDING_TIMEOUT))
=======
=======
>>>>>>> v3.18
static bool blk_kick_flush(struct request_queue *q, struct blk_flush_queue *fq)
{
	struct list_head *pending = &fq->flush_queue[fq->flush_pending_idx];
	struct request *first_rq =
		list_first_entry(pending, struct request, flush.list);
	struct request *flush_rq = fq->flush_rq;

	/* C1 described at the top of this file */
	if (fq->flush_pending_idx != fq->flush_running_idx || list_empty(pending))
		return false;

	/* C2 and C3 */
	if (!list_empty(&fq->flush_data_in_flight) &&
	    time_before(jiffies,
			fq->flush_pending_since + FLUSH_PENDING_TIMEOUT))
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return false;

	/*
	 * Issue flush and toggle pending_idx.  This makes pending_idx
	 * different from running_idx, which means flush is in flight.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	blk_rq_init(q, &q->flush_rq);
	q->flush_rq.cmd_type = REQ_TYPE_FS;
	q->flush_rq.cmd_flags = WRITE_FLUSH | REQ_FLUSH_SEQ;
	q->flush_rq.rq_disk = first_rq->rq_disk;
	q->flush_rq.end_io = flush_end_io;

	q->flush_pending_idx ^= 1;
	list_add_tail(&q->flush_rq.queuelist, &q->queue_head);
	return true;
=======
=======
>>>>>>> v3.18
	fq->flush_pending_idx ^= 1;

	blk_rq_init(q, flush_rq);

	/*
	 * Borrow tag from the first request since they can't
	 * be in flight at the same time.
	 */
	if (q->mq_ops) {
		flush_rq->mq_ctx = first_rq->mq_ctx;
		flush_rq->tag = first_rq->tag;
	}

	flush_rq->cmd_type = REQ_TYPE_FS;
	flush_rq->cmd_flags = WRITE_FLUSH | REQ_FLUSH_SEQ;
	flush_rq->rq_disk = first_rq->rq_disk;
	flush_rq->end_io = flush_end_io;

	return blk_flush_queue_rq(flush_rq, false);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static void flush_data_end_io(struct request *rq, int error)
{
	struct request_queue *q = rq->q;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct blk_flush_queue *fq = blk_get_flush_queue(q, NULL);
>>>>>>> v3.18
=======
	struct blk_flush_queue *fq = blk_get_flush_queue(q, NULL);
>>>>>>> v3.18

	/*
	 * After populating an empty queue, kick it to avoid stall.  Read
	 * the comment in flush_end_io().
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (blk_flush_complete_seq(rq, REQ_FSEQ_DATA, error))
		blk_run_queue_async(q);
}

=======
=======
>>>>>>> v3.18
	if (blk_flush_complete_seq(rq, fq, REQ_FSEQ_DATA, error))
		blk_run_queue_async(q);
}

static void mq_flush_data_end_io(struct request *rq, int error)
{
	struct request_queue *q = rq->q;
	struct blk_mq_hw_ctx *hctx;
	struct blk_mq_ctx *ctx = rq->mq_ctx;
	unsigned long flags;
	struct blk_flush_queue *fq = blk_get_flush_queue(q, ctx);

	hctx = q->mq_ops->map_queue(q, ctx->cpu);

	/*
	 * After populating an empty queue, kick it to avoid stall.  Read
	 * the comment in flush_end_io().
	 */
	spin_lock_irqsave(&fq->mq_flush_lock, flags);
	if (blk_flush_complete_seq(rq, fq, REQ_FSEQ_DATA, error))
		blk_mq_run_hw_queue(hctx, true);
	spin_unlock_irqrestore(&fq->mq_flush_lock, flags);
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
/**
 * blk_insert_flush - insert a new FLUSH/FUA request
 * @rq: request to insert
 *
 * To be called from __elv_add_request() for %ELEVATOR_INSERT_FLUSH insertions.
<<<<<<< HEAD
<<<<<<< HEAD
=======
 * or __blk_mq_run_hw_queue() to dispatch request.
>>>>>>> v3.18
=======
 * or __blk_mq_run_hw_queue() to dispatch request.
>>>>>>> v3.18
 * @rq is being submitted.  Analyze what needs to be done and put it on the
 * right queue.
 *
 * CONTEXT:
<<<<<<< HEAD
<<<<<<< HEAD
 * spin_lock_irq(q->queue_lock)
=======
 * spin_lock_irq(q->queue_lock) in !mq case
>>>>>>> v3.18
=======
 * spin_lock_irq(q->queue_lock) in !mq case
>>>>>>> v3.18
 */
void blk_insert_flush(struct request *rq)
{
	struct request_queue *q = rq->q;
	unsigned int fflags = q->flush_flags;	/* may change, cache */
	unsigned int policy = blk_flush_policy(fflags, rq);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct blk_flush_queue *fq = blk_get_flush_queue(q, rq->mq_ctx);
>>>>>>> v3.18
=======
	struct blk_flush_queue *fq = blk_get_flush_queue(q, rq->mq_ctx);
>>>>>>> v3.18

	/*
	 * @policy now records what operations need to be done.  Adjust
	 * REQ_FLUSH and FUA for the driver.
	 */
	rq->cmd_flags &= ~REQ_FLUSH;
	if (!(fflags & REQ_FUA))
		rq->cmd_flags &= ~REQ_FUA;

	/*
	 * An empty flush handed down from a stacking driver may
	 * translate into nothing if the underlying device does not
	 * advertise a write-back cache.  In this case, simply
	 * complete the request.
	 */
	if (!policy) {
<<<<<<< HEAD
<<<<<<< HEAD
		__blk_end_bidi_request(rq, 0, 0, 0);
=======
=======
>>>>>>> v3.18
		if (q->mq_ops)
			blk_mq_end_request(rq, 0);
		else
			__blk_end_bidi_request(rq, 0, 0, 0);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return;
	}

	BUG_ON(rq->bio != rq->biotail); /*assumes zero or single bio rq */

	/*
	 * If there's data but flush is not necessary, the request can be
	 * processed directly without going through flush machinery.  Queue
	 * for normal execution.
	 */
	if ((policy & REQ_FSEQ_DATA) &&
	    !(policy & (REQ_FSEQ_PREFLUSH | REQ_FSEQ_POSTFLUSH))) {
<<<<<<< HEAD
<<<<<<< HEAD
		list_add_tail(&rq->queuelist, &q->queue_head);
=======
=======
>>>>>>> v3.18
		if (q->mq_ops) {
			blk_mq_insert_request(rq, false, false, true);
		} else
			list_add_tail(&rq->queuelist, &q->queue_head);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return;
	}

	/*
	 * @rq should go through flush machinery.  Mark it part of flush
	 * sequence and submit for further processing.
	 */
	memset(&rq->flush, 0, sizeof(rq->flush));
	INIT_LIST_HEAD(&rq->flush.list);
	rq->cmd_flags |= REQ_FLUSH_SEQ;
	rq->flush.saved_end_io = rq->end_io; /* Usually NULL */
<<<<<<< HEAD
<<<<<<< HEAD
	rq->end_io = flush_data_end_io;

	blk_flush_complete_seq(rq, REQ_FSEQ_ACTIONS & ~policy, 0);
}

/**
 * blk_abort_flushes - @q is being aborted, abort flush requests
 * @q: request_queue being aborted
 *
 * To be called from elv_abort_queue().  @q is being aborted.  Prepare all
 * FLUSH/FUA requests for abortion.
 *
 * CONTEXT:
 * spin_lock_irq(q->queue_lock)
 */
void blk_abort_flushes(struct request_queue *q)
{
	struct request *rq, *n;
	int i;

	/*
	 * Requests in flight for data are already owned by the dispatch
	 * queue or the device driver.  Just restore for normal completion.
	 */
	list_for_each_entry_safe(rq, n, &q->flush_data_in_flight, flush.list) {
		list_del_init(&rq->flush.list);
		blk_flush_restore_request(rq);
	}

	/*
	 * We need to give away requests on flush queues.  Restore for
	 * normal completion and put them on the dispatch queue.
	 */
	for (i = 0; i < ARRAY_SIZE(q->flush_queue); i++) {
		list_for_each_entry_safe(rq, n, &q->flush_queue[i],
					 flush.list) {
			list_del_init(&rq->flush.list);
			blk_flush_restore_request(rq);
			list_add_tail(&rq->queuelist, &q->queue_head);
		}
	}
}

static void bio_end_flush(struct bio *bio, int err)
{
	if (err)
		clear_bit(BIO_UPTODATE, &bio->bi_flags);
	if (bio->bi_private)
		complete(bio->bi_private);
	bio_put(bio);
=======
=======
>>>>>>> v3.18
	if (q->mq_ops) {
		rq->end_io = mq_flush_data_end_io;

		spin_lock_irq(&fq->mq_flush_lock);
		blk_flush_complete_seq(rq, fq, REQ_FSEQ_ACTIONS & ~policy, 0);
		spin_unlock_irq(&fq->mq_flush_lock);
		return;
	}
	rq->end_io = flush_data_end_io;

	blk_flush_complete_seq(rq, fq, REQ_FSEQ_ACTIONS & ~policy, 0);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/**
 * blkdev_issue_flush - queue a flush
 * @bdev:	blockdev to issue flush for
 * @gfp_mask:	memory allocation flags (for bio_alloc)
 * @error_sector:	error sector
 *
 * Description:
 *    Issue a flush for the block device in question. Caller can supply
 *    room for storing the error offset in case of a flush error, if they
 *    wish to. If WAIT flag is not passed then caller may check only what
 *    request was pushed in some internal queue for later handling.
 */
int blkdev_issue_flush(struct block_device *bdev, gfp_t gfp_mask,
		sector_t *error_sector)
{
<<<<<<< HEAD
<<<<<<< HEAD
	DECLARE_COMPLETION_ONSTACK(wait);
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	struct request_queue *q;
	struct bio *bio;
	int ret = 0;

	if (bdev->bd_disk == NULL)
		return -ENXIO;

	q = bdev_get_queue(bdev);
	if (!q)
		return -ENXIO;

	/*
	 * some block devices may not have their queue correctly set up here
	 * (e.g. loop device without a backing file) and so issuing a flush
	 * here will panic. Ensure there is a request function before issuing
	 * the flush.
	 */
	if (!q->make_request_fn)
		return -ENXIO;

	bio = bio_alloc(gfp_mask, 0);
<<<<<<< HEAD
<<<<<<< HEAD
	bio->bi_end_io = bio_end_flush;
	bio->bi_bdev = bdev;
	bio->bi_private = &wait;

	bio_get(bio);
	submit_bio(WRITE_FLUSH, bio);
	wait_for_completion_io(&wait);
=======
	bio->bi_bdev = bdev;

	ret = submit_bio_wait(WRITE_FLUSH, bio);
>>>>>>> v3.18
=======
	bio->bi_bdev = bdev;

	ret = submit_bio_wait(WRITE_FLUSH, bio);
>>>>>>> v3.18

	/*
	 * The driver must store the error location in ->bi_sector, if
	 * it supports it. For non-stacked drivers, this should be
	 * copied from blk_rq_pos(rq).
	 */
	if (error_sector)
<<<<<<< HEAD
<<<<<<< HEAD
		*error_sector = bio->bi_sector;

	if (!bio_flagged(bio, BIO_UPTODATE))
		ret = -EIO;
=======
		*error_sector = bio->bi_iter.bi_sector;
>>>>>>> v3.18
=======
		*error_sector = bio->bi_iter.bi_sector;
>>>>>>> v3.18

	bio_put(bio);
	return ret;
}
EXPORT_SYMBOL(blkdev_issue_flush);
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18

struct blk_flush_queue *blk_alloc_flush_queue(struct request_queue *q,
		int node, int cmd_size)
{
	struct blk_flush_queue *fq;
	int rq_sz = sizeof(struct request);

	fq = kzalloc_node(sizeof(*fq), GFP_KERNEL, node);
	if (!fq)
		goto fail;

	if (q->mq_ops) {
		spin_lock_init(&fq->mq_flush_lock);
		rq_sz = round_up(rq_sz + cmd_size, cache_line_size());
	}

	fq->flush_rq = kzalloc_node(rq_sz, GFP_KERNEL, node);
	if (!fq->flush_rq)
		goto fail_rq;

	INIT_LIST_HEAD(&fq->flush_queue[0]);
	INIT_LIST_HEAD(&fq->flush_queue[1]);
	INIT_LIST_HEAD(&fq->flush_data_in_flight);

	return fq;

 fail_rq:
	kfree(fq);
 fail:
	return NULL;
}

void blk_free_flush_queue(struct blk_flush_queue *fq)
{
	/* bio based request queue hasn't flush queue */
	if (!fq)
		return;

	kfree(fq->flush_rq);
	kfree(fq);
}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
