/*
 * fs/f2fs/data.c
 *
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *             http://www.samsung.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/fs.h>
#include <linux/f2fs_fs.h>
#include <linux/buffer_head.h>
#include <linux/mpage.h>
#include <linux/aio.h>
#include <linux/writeback.h>
#include <linux/backing-dev.h>
<<<<<<< HEAD
#include <linux/pagevec.h>
#include <linux/blkdev.h>
#include <linux/bio.h>
#include <linux/prefetch.h>
#include <linux/uio.h>
#include <linux/mm.h>
#include <linux/memcontrol.h>
#include <linux/cleancache.h>
=======
#include <linux/blkdev.h>
#include <linux/bio.h>
#include <linux/prefetch.h>
>>>>>>> v3.18

#include "f2fs.h"
#include "node.h"
#include "segment.h"
<<<<<<< HEAD
#include "trace.h"
#include <trace/events/f2fs.h>

static bool __is_cp_guaranteed(struct page *page)
{
	struct address_space *mapping = page->mapping;
	struct inode *inode;
	struct f2fs_sb_info *sbi;

	if (!mapping)
		return false;

	inode = mapping->host;
	sbi = F2FS_I_SB(inode);

	if (inode->i_ino == F2FS_META_INO(sbi) ||
			inode->i_ino ==  F2FS_NODE_INO(sbi) ||
			S_ISDIR(inode->i_mode) ||
			is_cold_data(page))
		return true;
	return false;
}

=======
#include <trace/events/f2fs.h>

>>>>>>> v3.18
static void f2fs_read_end_io(struct bio *bio, int err)
{
	struct bio_vec *bvec;
	int i;

<<<<<<< HEAD
#ifdef CONFIG_F2FS_FAULT_INJECTION
	if (time_to_inject(F2FS_P_SB(bio->bi_io_vec->bv_page), FAULT_IO)) {
		f2fs_show_injection_info(FAULT_IO);
		err = -EIO;
	}
#endif

	if (f2fs_bio_encrypted(bio)) {
		if (err) {
			fscrypt_release_ctx(bio->bi_private);
		} else {
			fscrypt_decrypt_bio_pages(bio->bi_private, bio);
			return;
		}
	}

=======
>>>>>>> v3.18
	bio_for_each_segment_all(bvec, bio, i) {
		struct page *page = bvec->bv_page;

		if (!err) {
<<<<<<< HEAD
			if (!PageUptodate(page))
				SetPageUptodate(page);
=======
			SetPageUptodate(page);
>>>>>>> v3.18
		} else {
			ClearPageUptodate(page);
			SetPageError(page);
		}
		unlock_page(page);
	}
	bio_put(bio);
}

static void f2fs_write_end_io(struct bio *bio, int err)
{
	struct f2fs_sb_info *sbi = bio->bi_private;
	struct bio_vec *bvec;
	int i;

	bio_for_each_segment_all(bvec, bio, i) {
		struct page *page = bvec->bv_page;
<<<<<<< HEAD
		enum count_type type = WB_DATA_TYPE(page);

		if (IS_DUMMY_WRITTEN_PAGE(page)) {
			set_page_private(page, (unsigned long)NULL);
			ClearPagePrivate(page);
			unlock_page(page);
			mempool_free(page, sbi->write_io_dummy);

			if (unlikely(err))
				f2fs_stop_checkpoint(sbi, true);
			continue;
		}

		fscrypt_pullback_bio_page(&page, true);

		if (unlikely(err)) {
			set_bit(AS_EIO, &page->mapping->flags);
			f2fs_stop_checkpoint(sbi, true);
		}
		dec_page_count(sbi, type);
		clear_cold_data(page);
		end_page_writeback(page);
	}
	if (!get_pages(sbi, F2FS_WB_CP_DATA) &&
				wq_has_sleeper(&sbi->cp_wait))
		wake_up(&sbi->cp_wait);

	bio_put(bio);
}

/*
 * Return true, if pre_bio's bdev is same as its target device.
 */
struct block_device *f2fs_target_device(struct f2fs_sb_info *sbi,
				block_t blk_addr, struct bio *bio)
{
	struct block_device *bdev = sbi->sb->s_bdev;
	int i;

	for (i = 0; i < sbi->s_ndevs; i++) {
		if (FDEV(i).start_blk <= blk_addr &&
					FDEV(i).end_blk >= blk_addr) {
			blk_addr -= FDEV(i).start_blk;
			bdev = FDEV(i).bdev;
			break;
		}
	}
	if (bio) {
		bio->bi_bdev = bdev;
		bio->bi_sector = SECTOR_FROM_BLOCK(blk_addr);
	}
	return bdev;
}

int f2fs_target_device_index(struct f2fs_sb_info *sbi, block_t blkaddr)
{
	int i;

	for (i = 0; i < sbi->s_ndevs; i++)
		if (FDEV(i).start_blk <= blkaddr && FDEV(i).end_blk >= blkaddr)
			return i;
	return 0;
}

static bool __same_bdev(struct f2fs_sb_info *sbi,
				block_t blk_addr, struct bio *bio)
{
	return f2fs_target_device(sbi, blk_addr, NULL) == bio->bi_bdev;
=======

		if (unlikely(err)) {
			set_page_dirty(page);
			set_bit(AS_EIO, &page->mapping->flags);
			f2fs_stop_checkpoint(sbi);
		}
		end_page_writeback(page);
		dec_page_count(sbi, F2FS_WRITEBACK);
	}

	if (sbi->wait_io) {
		complete(sbi->wait_io);
		sbi->wait_io = NULL;
	}

	if (!get_pages(sbi, F2FS_WRITEBACK) &&
			!list_empty(&sbi->cp_wait.task_list))
		wake_up(&sbi->cp_wait);

	bio_put(bio);
>>>>>>> v3.18
}

/*
 * Low-level block read/write IO operations.
 */
static struct bio *__bio_alloc(struct f2fs_sb_info *sbi, block_t blk_addr,
				int npages, bool is_read)
{
	struct bio *bio;

<<<<<<< HEAD
	bio = f2fs_bio_alloc(npages);

	f2fs_target_device(sbi, blk_addr, bio);
	bio->bi_end_io = is_read ? f2fs_read_end_io : f2fs_write_end_io;
	bio->bi_private = is_read ? NULL : sbi;
=======
	/* No failure on bio allocation */
	bio = bio_alloc(GFP_NOIO, npages);

	bio->bi_bdev = sbi->sb->s_bdev;
	bio->bi_iter.bi_sector = SECTOR_FROM_BLOCK(blk_addr);
	bio->bi_end_io = is_read ? f2fs_read_end_io : f2fs_write_end_io;
	bio->bi_private = sbi;
>>>>>>> v3.18

	return bio;
}

<<<<<<< HEAD
static inline void __submit_bio(struct f2fs_sb_info *sbi,
				struct bio *bio, enum page_type type)
{
	if (!is_read_io(bio_op(bio))) {
		unsigned int start;

		if (f2fs_sb_mounted_blkzoned(sbi->sb) &&
			current->plug && (type == DATA || type == NODE))
			blk_finish_plug(current->plug);

		if (type != DATA && type != NODE)
			goto submit_io;

		start = bio->bi_size >> F2FS_BLKSIZE_BITS;
		start %= F2FS_IO_SIZE(sbi);

		if (start == 0)
			goto submit_io;

		/* fill dummy pages */
		for (; start < F2FS_IO_SIZE(sbi); start++) {
			struct page *page =
				mempool_alloc(sbi->write_io_dummy,
					GFP_NOIO | __GFP_ZERO | __GFP_NOFAIL);
			f2fs_bug_on(sbi, !page);

			SetPagePrivate(page);
			set_page_private(page, (unsigned long)DUMMY_WRITTEN_PAGE);
			lock_page(page);
			if (bio_add_page(bio, page, PAGE_SIZE, 0) < PAGE_SIZE)
				f2fs_bug_on(sbi, 1);
		}
		/*
		 * In the NODE case, we lose next block address chain. So, we
		 * need to do checkpoint in f2fs_sync_file.
		 */
		if (type == NODE)
			set_sbi_flag(sbi, SBI_NEED_CP);
	}
submit_io:
	if (is_read_io(bio_op(bio)))
		trace_f2fs_submit_read_bio(sbi->sb, type, bio);
	else
		trace_f2fs_submit_write_bio(sbi->sb, type, bio);
	submit_bio(bio_op(bio), bio);
}

static void __submit_merged_bio(struct f2fs_bio_info *io)
{
	struct f2fs_io_info *fio = &io->fio;
=======
static void __submit_merged_bio(struct f2fs_bio_info *io)
{
	struct f2fs_io_info *fio = &io->fio;
	int rw;
>>>>>>> v3.18

	if (!io->bio)
		return;

<<<<<<< HEAD
	bio_set_op_attrs(io->bio, fio->op, fio->op_flags);

	if (is_read_io(fio->op))
		trace_f2fs_prepare_read_bio(io->sbi->sb, fio->type, io->bio);
	else
		trace_f2fs_prepare_write_bio(io->sbi->sb, fio->type, io->bio);

	__submit_bio(io->sbi, io->bio, fio->type);
	io->bio = NULL;
}

static bool __has_merged_page(struct f2fs_bio_info *io,
				struct inode *inode, nid_t ino, pgoff_t idx)
{
	struct bio_vec *bvec;
	struct page *target;
	int i;

	if (!io->bio)
		return false;

	if (!inode && !ino)
		return true;

	bio_for_each_segment_all(bvec, io->bio, i) {

		if (bvec->bv_page->mapping)
			target = bvec->bv_page;
		else
			target = fscrypt_control_page(bvec->bv_page);

		if (idx != target->index)
			continue;

		if (inode && inode == target->mapping->host)
			return true;
		if (ino && ino == ino_of_node(target))
			return true;
	}

	return false;
}

static bool has_merged_page(struct f2fs_sb_info *sbi, struct inode *inode,
				nid_t ino, pgoff_t idx, enum page_type type)
{
	enum page_type btype = PAGE_TYPE_OF_BIO(type);
	enum temp_type temp;
	struct f2fs_bio_info *io;
	bool ret = false;

	for (temp = HOT; temp < NR_TEMP_TYPE; temp++) {
		io = sbi->write_io[btype] + temp;

		down_read(&io->io_rwsem);
		ret = __has_merged_page(io, inode, ino, idx);
		up_read(&io->io_rwsem);

		/* TODO: use HOT temp only for meta pages now. */
		if (ret || btype == META)
			break;
	}
	return ret;
}

static void __f2fs_submit_merged_write(struct f2fs_sb_info *sbi,
				enum page_type type, enum temp_type temp)
{
	enum page_type btype = PAGE_TYPE_OF_BIO(type);
	struct f2fs_bio_info *io = sbi->write_io[btype] + temp;
=======
	rw = fio->rw;

	if (is_read_io(rw)) {
		trace_f2fs_submit_read_bio(io->sbi->sb, rw,
						fio->type, io->bio);
		submit_bio(rw, io->bio);
	} else {
		trace_f2fs_submit_write_bio(io->sbi->sb, rw,
						fio->type, io->bio);
		/*
		 * META_FLUSH is only from the checkpoint procedure, and we
		 * should wait this metadata bio for FS consistency.
		 */
		if (fio->type == META_FLUSH) {
			DECLARE_COMPLETION_ONSTACK(wait);
			io->sbi->wait_io = &wait;
			submit_bio(rw, io->bio);
			wait_for_completion(&wait);
		} else {
			submit_bio(rw, io->bio);
		}
	}

	io->bio = NULL;
}

void f2fs_submit_merged_bio(struct f2fs_sb_info *sbi,
				enum page_type type, int rw)
{
	enum page_type btype = PAGE_TYPE_OF_BIO(type);
	struct f2fs_bio_info *io;

	io = is_read_io(rw) ? &sbi->read_io : &sbi->write_io[btype];
>>>>>>> v3.18

	down_write(&io->io_rwsem);

	/* change META to META_FLUSH in the checkpoint procedure */
	if (type >= META_FLUSH) {
		io->fio.type = META_FLUSH;
<<<<<<< HEAD
		io->fio.op = REQ_OP_WRITE;
		io->fio.op_flags = REQ_META | REQ_PRIO;
		if (!test_opt(sbi, NOBARRIER))
			io->fio.op_flags |= WRITE_FLUSH | REQ_FUA;
=======
		if (test_opt(sbi, NOBARRIER))
			io->fio.rw = WRITE_FLUSH | REQ_META | REQ_PRIO;
		else
			io->fio.rw = WRITE_FLUSH_FUA | REQ_META | REQ_PRIO;
>>>>>>> v3.18
	}
	__submit_merged_bio(io);
	up_write(&io->io_rwsem);
}

<<<<<<< HEAD
static void __submit_merged_write_cond(struct f2fs_sb_info *sbi,
				struct inode *inode, nid_t ino, pgoff_t idx,
				enum page_type type, bool force)
{
	enum temp_type temp;

	if (!force && !has_merged_page(sbi, inode, ino, idx, type))
		return;

	for (temp = HOT; temp < NR_TEMP_TYPE; temp++) {

		__f2fs_submit_merged_write(sbi, type, temp);

		/* TODO: use HOT temp only for meta pages now. */
		if (type >= META)
			break;
	}
}

void f2fs_submit_merged_write(struct f2fs_sb_info *sbi, enum page_type type)
{
	__submit_merged_write_cond(sbi, NULL, 0, 0, type, true);
}

void f2fs_submit_merged_write_cond(struct f2fs_sb_info *sbi,
				struct inode *inode, nid_t ino, pgoff_t idx,
				enum page_type type)
{
	__submit_merged_write_cond(sbi, inode, ino, idx, type, false);
}

void f2fs_flush_merged_writes(struct f2fs_sb_info *sbi)
{
	f2fs_submit_merged_write(sbi, DATA);
	f2fs_submit_merged_write(sbi, NODE);
	f2fs_submit_merged_write(sbi, META);
}

/*
 * Fill the locked page with data located in the block address.
 * A caller needs to unlock the page on failure.
 */
int f2fs_submit_page_bio(struct f2fs_io_info *fio)
{
	struct bio *bio;
	struct page *page = fio->encrypted_page ?
			fio->encrypted_page : fio->page;

	trace_f2fs_submit_page_bio(page, fio);
	f2fs_trace_ios(fio, 0);

	/* Allocate a new bio */
	bio = __bio_alloc(fio->sbi, fio->new_blkaddr, 1, is_read_io(fio->op));

	if (bio_add_page(bio, page, PAGE_SIZE, 0) < PAGE_SIZE) {
		bio_put(bio);
		return -EFAULT;
	}
	bio_set_op_attrs(bio, fio->op, fio->op_flags);

	__submit_bio(fio->sbi, bio, fio->type);

	if (!is_read_io(fio->op))
		inc_page_count(fio->sbi, WB_DATA_TYPE(fio->page));
	return 0;
}

int f2fs_submit_page_write(struct f2fs_io_info *fio)
{
	struct f2fs_sb_info *sbi = fio->sbi;
	enum page_type btype = PAGE_TYPE_OF_BIO(fio->type);
	struct f2fs_bio_info *io = sbi->write_io[btype] + fio->temp;
	struct page *bio_page;
	int err = 0;

	f2fs_bug_on(sbi, is_read_io(fio->op));

	down_write(&io->io_rwsem);
next:
	if (fio->in_list) {
		spin_lock(&io->io_lock);
		if (list_empty(&io->io_list)) {
			spin_unlock(&io->io_lock);
			goto out_fail;
		}
		fio = list_first_entry(&io->io_list,
						struct f2fs_io_info, list);
		list_del(&fio->list);
		spin_unlock(&io->io_lock);
	}

	if (fio->old_blkaddr != NEW_ADDR)
		verify_block_addr(sbi, fio->old_blkaddr);
	verify_block_addr(sbi, fio->new_blkaddr);

	bio_page = fio->encrypted_page ? fio->encrypted_page : fio->page;

	/* set submitted = 1 as a return value */
	fio->submitted = 1;

	inc_page_count(sbi, WB_DATA_TYPE(bio_page));

	if (io->bio && (io->last_block_in_bio != fio->new_blkaddr - 1 ||
	    (io->fio.op != fio->op || io->fio.op_flags != fio->op_flags) ||
			!__same_bdev(sbi, fio->new_blkaddr, io->bio)))
		__submit_merged_bio(io);
alloc_new:
	if (io->bio == NULL) {
		if ((fio->type == DATA || fio->type == NODE) &&
				fio->new_blkaddr & F2FS_IO_SIZE_MASK(sbi)) {
			err = -EAGAIN;
			dec_page_count(sbi, WB_DATA_TYPE(bio_page));
			goto out_fail;
		}
		io->bio = __bio_alloc(sbi, fio->new_blkaddr,
						BIO_MAX_PAGES, false);
		io->fio = *fio;
	}

	if (bio_add_page(io->bio, bio_page, PAGE_SIZE, 0) < PAGE_SIZE) {
=======
/*
 * Fill the locked page with data located in the block address.
 * Return unlocked page.
 */
int f2fs_submit_page_bio(struct f2fs_sb_info *sbi, struct page *page,
					block_t blk_addr, int rw)
{
	struct bio *bio;

	trace_f2fs_submit_page_bio(page, blk_addr, rw);

	/* Allocate a new bio */
	bio = __bio_alloc(sbi, blk_addr, 1, is_read_io(rw));

	if (bio_add_page(bio, page, PAGE_CACHE_SIZE, 0) < PAGE_CACHE_SIZE) {
		bio_put(bio);
		f2fs_put_page(page, 1);
		return -EFAULT;
	}

	submit_bio(rw, bio);
	return 0;
}

void f2fs_submit_page_mbio(struct f2fs_sb_info *sbi, struct page *page,
			block_t blk_addr, struct f2fs_io_info *fio)
{
	enum page_type btype = PAGE_TYPE_OF_BIO(fio->type);
	struct f2fs_bio_info *io;
	bool is_read = is_read_io(fio->rw);

	io = is_read ? &sbi->read_io : &sbi->write_io[btype];

	verify_block_addr(sbi, blk_addr);

	down_write(&io->io_rwsem);

	if (!is_read)
		inc_page_count(sbi, F2FS_WRITEBACK);

	if (io->bio && (io->last_block_in_bio != blk_addr - 1 ||
						io->fio.rw != fio->rw))
		__submit_merged_bio(io);
alloc_new:
	if (io->bio == NULL) {
		int bio_blocks = MAX_BIO_BLOCKS(sbi);

		io->bio = __bio_alloc(sbi, blk_addr, bio_blocks, is_read);
		io->fio = *fio;
	}

	if (bio_add_page(io->bio, page, PAGE_CACHE_SIZE, 0) <
							PAGE_CACHE_SIZE) {
>>>>>>> v3.18
		__submit_merged_bio(io);
		goto alloc_new;
	}

<<<<<<< HEAD
	io->last_block_in_bio = fio->new_blkaddr;
	f2fs_trace_ios(fio, 0);

	trace_f2fs_submit_page_write(fio->page, fio);

	if (fio->in_list)
		goto next;
out_fail:
	up_write(&io->io_rwsem);
	return err;
}

static void __set_data_blkaddr(struct dnode_of_data *dn)
{
	struct f2fs_node *rn = F2FS_NODE(dn->node_page);
	__le32 *addr_array;

	/* Get physical address of data block */
	addr_array = blkaddr_in_node(rn);
	addr_array[dn->ofs_in_node] = cpu_to_le32(dn->data_blkaddr);
=======
	io->last_block_in_bio = blk_addr;

	up_write(&io->io_rwsem);
	trace_f2fs_submit_page_mbio(page, fio->rw, fio->type, blk_addr);
>>>>>>> v3.18
}

/*
 * Lock ordering for the change of data block address:
 * ->data_page
 *  ->node_page
 *    update block addresses in the node page
 */
<<<<<<< HEAD
void set_data_blkaddr(struct dnode_of_data *dn)
{
	f2fs_wait_on_page_writeback(dn->node_page, NODE, true);
	__set_data_blkaddr(dn);
	if (set_page_dirty(dn->node_page))
		dn->node_changed = true;
}

void f2fs_update_data_blkaddr(struct dnode_of_data *dn, block_t blkaddr)
{
	dn->data_blkaddr = blkaddr;
	set_data_blkaddr(dn);
	f2fs_update_extent_cache(dn);
}

/* dn->ofs_in_node will be returned with up-to-date last block pointer */
int reserve_new_blocks(struct dnode_of_data *dn, blkcnt_t count)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(dn->inode);
	int err;

	if (!count)
		return 0;

	if (unlikely(is_inode_flag_set(dn->inode, FI_NO_ALLOC)))
		return -EPERM;
	if (unlikely((err = inc_valid_block_count(sbi, dn->inode, &count))))
		return err;

	trace_f2fs_reserve_new_blocks(dn->inode, dn->nid,
						dn->ofs_in_node, count);

	f2fs_wait_on_page_writeback(dn->node_page, NODE, true);

	for (; count > 0; dn->ofs_in_node++) {
		block_t blkaddr =
			datablock_addr(dn->node_page, dn->ofs_in_node);
		if (blkaddr == NULL_ADDR) {
			dn->data_blkaddr = NEW_ADDR;
			__set_data_blkaddr(dn);
			count--;
		}
	}

	if (set_page_dirty(dn->node_page))
		dn->node_changed = true;
	return 0;
}

/* Should keep dn->ofs_in_node unchanged */
int reserve_new_block(struct dnode_of_data *dn)
{
	unsigned int ofs_in_node = dn->ofs_in_node;
	int ret;

	ret = reserve_new_blocks(dn, 1);
	dn->ofs_in_node = ofs_in_node;
	return ret;
}

=======
static void __set_data_blkaddr(struct dnode_of_data *dn, block_t new_addr)
{
	struct f2fs_node *rn;
	__le32 *addr_array;
	struct page *node_page = dn->node_page;
	unsigned int ofs_in_node = dn->ofs_in_node;

	f2fs_wait_on_page_writeback(node_page, NODE);

	rn = F2FS_NODE(node_page);

	/* Get physical address of data block */
	addr_array = blkaddr_in_node(rn);
	addr_array[ofs_in_node] = cpu_to_le32(new_addr);
	set_page_dirty(node_page);
}

int reserve_new_block(struct dnode_of_data *dn)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(dn->inode);

	if (unlikely(is_inode_flag_set(F2FS_I(dn->inode), FI_NO_ALLOC)))
		return -EPERM;
	if (unlikely(!inc_valid_block_count(sbi, dn->inode, 1)))
		return -ENOSPC;

	trace_f2fs_reserve_new_block(dn->inode, dn->nid, dn->ofs_in_node);

	__set_data_blkaddr(dn, NEW_ADDR);
	dn->data_blkaddr = NEW_ADDR;
	mark_inode_dirty(dn->inode);
	sync_inode_page(dn);
	return 0;
}

>>>>>>> v3.18
int f2fs_reserve_block(struct dnode_of_data *dn, pgoff_t index)
{
	bool need_put = dn->inode_page ? false : true;
	int err;

<<<<<<< HEAD
=======
	/* if inode_page exists, index should be zero */
	f2fs_bug_on(F2FS_I_SB(dn->inode), !need_put && index);

>>>>>>> v3.18
	err = get_dnode_of_data(dn, index, ALLOC_NODE);
	if (err)
		return err;

	if (dn->data_blkaddr == NULL_ADDR)
		err = reserve_new_block(dn);
	if (err || need_put)
		f2fs_put_dnode(dn);
	return err;
}

<<<<<<< HEAD
int f2fs_get_block(struct dnode_of_data *dn, pgoff_t index)
{
	struct extent_info ei  = {0,0,0};
	struct inode *inode = dn->inode;

	if (f2fs_lookup_extent_cache(inode, index, &ei)) {
		dn->data_blkaddr = ei.blk + index - ei.fofs;
		return 0;
	}

	return f2fs_reserve_block(dn, index);
}

struct page *get_read_data_page(struct inode *inode, pgoff_t index,
						int op_flags, bool for_write)
{
	struct address_space *mapping = inode->i_mapping;
	struct dnode_of_data dn;
	struct page *page;
	struct extent_info ei = {0,0,0};
	int err;
	struct f2fs_io_info fio = {
		.sbi = F2FS_I_SB(inode),
		.type = DATA,
		.op = REQ_OP_READ,
		.op_flags = op_flags,
		.encrypted_page = NULL,
	};

	if (f2fs_encrypted_inode(inode) && S_ISREG(inode->i_mode))
		return read_mapping_page(mapping, index, NULL);

	page = f2fs_grab_cache_page(mapping, index, for_write);
	if (!page)
		return ERR_PTR(-ENOMEM);

	if (f2fs_lookup_extent_cache(inode, index, &ei)) {
		dn.data_blkaddr = ei.blk + index - ei.fofs;
		goto got_it;
	}

	set_new_dnode(&dn, inode, NULL, NULL, 0);
	err = get_dnode_of_data(&dn, index, LOOKUP_NODE);
	if (err)
		goto put_err;
	f2fs_put_dnode(&dn);

	if (unlikely(dn.data_blkaddr == NULL_ADDR)) {
		err = -ENOENT;
		goto put_err;
	}
got_it:
	if (PageUptodate(page)) {
		unlock_page(page);
		return page;
	}

	/*
	 * A new dentry page is allocated but not able to be written, since its
	 * new inode page couldn't be allocated due to -ENOSPC.
	 * In such the case, its blkaddr can be remained as NEW_ADDR.
	 * see, f2fs_add_link -> get_new_data_page -> init_inode_metadata.
	 */
	if (dn.data_blkaddr == NEW_ADDR) {
		zero_user_segment(page, 0, PAGE_SIZE);
		if (!PageUptodate(page))
			SetPageUptodate(page);
		unlock_page(page);
		return page;
	}

	fio.new_blkaddr = fio.old_blkaddr = dn.data_blkaddr;
	fio.page = page;
	err = f2fs_submit_page_bio(&fio);
	if (err)
		goto put_err;
	return page;

put_err:
	f2fs_put_page(page, 1);
	return ERR_PTR(err);
}

struct page *find_data_page(struct inode *inode, pgoff_t index)
{
	struct address_space *mapping = inode->i_mapping;
	struct page *page;
=======
static int check_extent_cache(struct inode *inode, pgoff_t pgofs,
					struct buffer_head *bh_result)
{
	struct f2fs_inode_info *fi = F2FS_I(inode);
	pgoff_t start_fofs, end_fofs;
	block_t start_blkaddr;

	if (is_inode_flag_set(fi, FI_NO_EXTENT))
		return 0;

	read_lock(&fi->ext.ext_lock);
	if (fi->ext.len == 0) {
		read_unlock(&fi->ext.ext_lock);
		return 0;
	}

	stat_inc_total_hit(inode->i_sb);

	start_fofs = fi->ext.fofs;
	end_fofs = fi->ext.fofs + fi->ext.len - 1;
	start_blkaddr = fi->ext.blk_addr;

	if (pgofs >= start_fofs && pgofs <= end_fofs) {
		unsigned int blkbits = inode->i_sb->s_blocksize_bits;
		size_t count;

		clear_buffer_new(bh_result);
		map_bh(bh_result, inode->i_sb,
				start_blkaddr + pgofs - start_fofs);
		count = end_fofs - pgofs + 1;
		if (count < (UINT_MAX >> blkbits))
			bh_result->b_size = (count << blkbits);
		else
			bh_result->b_size = UINT_MAX;

		stat_inc_read_hit(inode->i_sb);
		read_unlock(&fi->ext.ext_lock);
		return 1;
	}
	read_unlock(&fi->ext.ext_lock);
	return 0;
}

void update_extent_cache(block_t blk_addr, struct dnode_of_data *dn)
{
	struct f2fs_inode_info *fi = F2FS_I(dn->inode);
	pgoff_t fofs, start_fofs, end_fofs;
	block_t start_blkaddr, end_blkaddr;
	int need_update = true;

	f2fs_bug_on(F2FS_I_SB(dn->inode), blk_addr == NEW_ADDR);
	fofs = start_bidx_of_node(ofs_of_node(dn->node_page), fi) +
							dn->ofs_in_node;

	/* Update the page address in the parent node */
	__set_data_blkaddr(dn, blk_addr);

	if (is_inode_flag_set(fi, FI_NO_EXTENT))
		return;

	write_lock(&fi->ext.ext_lock);

	start_fofs = fi->ext.fofs;
	end_fofs = fi->ext.fofs + fi->ext.len - 1;
	start_blkaddr = fi->ext.blk_addr;
	end_blkaddr = fi->ext.blk_addr + fi->ext.len - 1;

	/* Drop and initialize the matched extent */
	if (fi->ext.len == 1 && fofs == start_fofs)
		fi->ext.len = 0;

	/* Initial extent */
	if (fi->ext.len == 0) {
		if (blk_addr != NULL_ADDR) {
			fi->ext.fofs = fofs;
			fi->ext.blk_addr = blk_addr;
			fi->ext.len = 1;
		}
		goto end_update;
	}

	/* Front merge */
	if (fofs == start_fofs - 1 && blk_addr == start_blkaddr - 1) {
		fi->ext.fofs--;
		fi->ext.blk_addr--;
		fi->ext.len++;
		goto end_update;
	}

	/* Back merge */
	if (fofs == end_fofs + 1 && blk_addr == end_blkaddr + 1) {
		fi->ext.len++;
		goto end_update;
	}

	/* Split the existing extent */
	if (fi->ext.len > 1 &&
		fofs >= start_fofs && fofs <= end_fofs) {
		if ((end_fofs - fofs) < (fi->ext.len >> 1)) {
			fi->ext.len = fofs - start_fofs;
		} else {
			fi->ext.fofs = fofs + 1;
			fi->ext.blk_addr = start_blkaddr +
					fofs - start_fofs + 1;
			fi->ext.len -= fofs - start_fofs + 1;
		}
	} else {
		need_update = false;
	}

	/* Finally, if the extent is very fragmented, let's drop the cache. */
	if (fi->ext.len < F2FS_MIN_EXTENT_LEN) {
		fi->ext.len = 0;
		set_inode_flag(fi, FI_NO_EXTENT);
		need_update = true;
	}
end_update:
	write_unlock(&fi->ext.ext_lock);
	if (need_update)
		sync_inode_page(dn);
	return;
}

struct page *find_data_page(struct inode *inode, pgoff_t index, bool sync)
{
	struct address_space *mapping = inode->i_mapping;
	struct dnode_of_data dn;
	struct page *page;
	int err;
>>>>>>> v3.18

	page = find_get_page(mapping, index);
	if (page && PageUptodate(page))
		return page;
	f2fs_put_page(page, 0);

<<<<<<< HEAD
	page = get_read_data_page(inode, index, REQ_SYNC, false);
	if (IS_ERR(page))
		return page;

	if (PageUptodate(page))
		return page;

	wait_on_page_locked(page);
	if (unlikely(!PageUptodate(page))) {
		f2fs_put_page(page, 0);
		return ERR_PTR(-EIO);
=======
	set_new_dnode(&dn, inode, NULL, NULL, 0);
	err = get_dnode_of_data(&dn, index, LOOKUP_NODE);
	if (err)
		return ERR_PTR(err);
	f2fs_put_dnode(&dn);

	if (dn.data_blkaddr == NULL_ADDR)
		return ERR_PTR(-ENOENT);

	/* By fallocate(), there is no cached page, but with NEW_ADDR */
	if (unlikely(dn.data_blkaddr == NEW_ADDR))
		return ERR_PTR(-EINVAL);

	page = grab_cache_page(mapping, index);
	if (!page)
		return ERR_PTR(-ENOMEM);

	if (PageUptodate(page)) {
		unlock_page(page);
		return page;
	}

	err = f2fs_submit_page_bio(F2FS_I_SB(inode), page, dn.data_blkaddr,
					sync ? READ_SYNC : READA);
	if (err)
		return ERR_PTR(err);

	if (sync) {
		wait_on_page_locked(page);
		if (unlikely(!PageUptodate(page))) {
			f2fs_put_page(page, 0);
			return ERR_PTR(-EIO);
		}
>>>>>>> v3.18
	}
	return page;
}

/*
 * If it tries to access a hole, return an error.
 * Because, the callers, functions in dir.c and GC, should be able to know
 * whether this page exists or not.
 */
<<<<<<< HEAD
struct page *get_lock_data_page(struct inode *inode, pgoff_t index,
							bool for_write)
{
	struct address_space *mapping = inode->i_mapping;
	struct page *page;
repeat:
	page = get_read_data_page(inode, index, REQ_SYNC, for_write);
	if (IS_ERR(page))
		return page;

	/* wait for read completion */
	lock_page(page);
	if (unlikely(page->mapping != mapping)) {
		f2fs_put_page(page, 1);
		goto repeat;
	}
=======
struct page *get_lock_data_page(struct inode *inode, pgoff_t index)
{
	struct address_space *mapping = inode->i_mapping;
	struct dnode_of_data dn;
	struct page *page;
	int err;

repeat:
	page = grab_cache_page(mapping, index);
	if (!page)
		return ERR_PTR(-ENOMEM);

	set_new_dnode(&dn, inode, NULL, NULL, 0);
	err = get_dnode_of_data(&dn, index, LOOKUP_NODE);
	if (err) {
		f2fs_put_page(page, 1);
		return ERR_PTR(err);
	}
	f2fs_put_dnode(&dn);

	if (unlikely(dn.data_blkaddr == NULL_ADDR)) {
		f2fs_put_page(page, 1);
		return ERR_PTR(-ENOENT);
	}

	if (PageUptodate(page))
		return page;

	/*
	 * A new dentry page is allocated but not able to be written, since its
	 * new inode page couldn't be allocated due to -ENOSPC.
	 * In such the case, its blkaddr can be remained as NEW_ADDR.
	 * see, f2fs_add_link -> get_new_data_page -> init_inode_metadata.
	 */
	if (dn.data_blkaddr == NEW_ADDR) {
		zero_user_segment(page, 0, PAGE_CACHE_SIZE);
		SetPageUptodate(page);
		return page;
	}

	err = f2fs_submit_page_bio(F2FS_I_SB(inode), page,
					dn.data_blkaddr, READ_SYNC);
	if (err)
		return ERR_PTR(err);

	lock_page(page);
>>>>>>> v3.18
	if (unlikely(!PageUptodate(page))) {
		f2fs_put_page(page, 1);
		return ERR_PTR(-EIO);
	}
<<<<<<< HEAD
=======
	if (unlikely(page->mapping != mapping)) {
		f2fs_put_page(page, 1);
		goto repeat;
	}
>>>>>>> v3.18
	return page;
}

/*
 * Caller ensures that this data page is never allocated.
 * A new zero-filled data page is allocated in the page cache.
 *
 * Also, caller should grab and release a rwsem by calling f2fs_lock_op() and
 * f2fs_unlock_op().
<<<<<<< HEAD
 * Note that, ipage is set only by make_empty_dir, and if any error occur,
 * ipage should be released by this function.
=======
 * Note that, ipage is set only by make_empty_dir.
>>>>>>> v3.18
 */
struct page *get_new_data_page(struct inode *inode,
		struct page *ipage, pgoff_t index, bool new_i_size)
{
	struct address_space *mapping = inode->i_mapping;
	struct page *page;
	struct dnode_of_data dn;
	int err;

<<<<<<< HEAD
	page = f2fs_grab_cache_page(mapping, index, true);
	if (!page) {
		/*
		 * before exiting, we should make sure ipage will be released
		 * if any error occur.
		 */
		f2fs_put_page(ipage, 1);
		return ERR_PTR(-ENOMEM);
	}

	set_new_dnode(&dn, inode, ipage, NULL, 0);
	err = f2fs_reserve_block(&dn, index);
	if (err) {
		f2fs_put_page(page, 1);
		return ERR_PTR(err);
	}
	if (!ipage)
		f2fs_put_dnode(&dn);

	if (PageUptodate(page))
		goto got_it;

	if (dn.data_blkaddr == NEW_ADDR) {
		zero_user_segment(page, 0, PAGE_SIZE);
		if (!PageUptodate(page))
			SetPageUptodate(page);
	} else {
		f2fs_put_page(page, 1);

		/* if ipage exists, blkaddr should be NEW_ADDR */
		f2fs_bug_on(F2FS_I_SB(inode), ipage);
		page = get_lock_data_page(inode, index, true);
		if (IS_ERR(page))
			return page;
	}
got_it:
	if (new_i_size && i_size_read(inode) <
				((loff_t)(index + 1) << PAGE_SHIFT))
		f2fs_i_size_write(inode, ((loff_t)(index + 1) << PAGE_SHIFT));
	return page;
=======
	set_new_dnode(&dn, inode, ipage, NULL, 0);
	err = f2fs_reserve_block(&dn, index);
	if (err)
		return ERR_PTR(err);
repeat:
	page = grab_cache_page(mapping, index);
	if (!page) {
		err = -ENOMEM;
		goto put_err;
	}

	if (PageUptodate(page))
		return page;

	if (dn.data_blkaddr == NEW_ADDR) {
		zero_user_segment(page, 0, PAGE_CACHE_SIZE);
		SetPageUptodate(page);
	} else {
		err = f2fs_submit_page_bio(F2FS_I_SB(inode), page,
						dn.data_blkaddr, READ_SYNC);
		if (err)
			goto put_err;

		lock_page(page);
		if (unlikely(!PageUptodate(page))) {
			f2fs_put_page(page, 1);
			err = -EIO;
			goto put_err;
		}
		if (unlikely(page->mapping != mapping)) {
			f2fs_put_page(page, 1);
			goto repeat;
		}
	}

	if (new_i_size &&
		i_size_read(inode) < ((index + 1) << PAGE_CACHE_SHIFT)) {
		i_size_write(inode, ((index + 1) << PAGE_CACHE_SHIFT));
		/* Only the directory inode sets new_i_size */
		set_inode_flag(F2FS_I(inode), FI_UPDATE_DIR);
	}
	return page;

put_err:
	f2fs_put_dnode(&dn);
	return ERR_PTR(err);
>>>>>>> v3.18
}

static int __allocate_data_block(struct dnode_of_data *dn)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(dn->inode);
<<<<<<< HEAD
	struct f2fs_summary sum;
	struct node_info ni;
	pgoff_t fofs;
	blkcnt_t count = 1;
	int err;

	if (unlikely(is_inode_flag_set(dn->inode, FI_NO_ALLOC)))
		return -EPERM;

	dn->data_blkaddr = datablock_addr(dn->node_page, dn->ofs_in_node);
	if (dn->data_blkaddr == NEW_ADDR)
		goto alloc;

	if (unlikely((err = inc_valid_block_count(sbi, dn->inode, &count))))
		return err;

alloc:
	get_node_info(sbi, dn->nid, &ni);
	set_summary(&sum, dn->nid, dn->ofs_in_node, ni.version);

	allocate_data_block(sbi, NULL, dn->data_blkaddr, &dn->data_blkaddr,
					&sum, CURSEG_WARM_DATA, NULL, false);
	set_data_blkaddr(dn);

	/* update i_size */
	fofs = start_bidx_of_node(ofs_of_node(dn->node_page), dn->inode) +
							dn->ofs_in_node;
	if (i_size_read(dn->inode) < ((loff_t)(fofs + 1) << PAGE_SHIFT))
		f2fs_i_size_write(dn->inode,
				((loff_t)(fofs + 1) << PAGE_SHIFT));
	return 0;
}

static inline bool __force_buffered_io(struct inode *inode, int rw)
{
	return ((f2fs_encrypted_inode(inode) && S_ISREG(inode->i_mode)) ||
			(rw == WRITE && test_opt(F2FS_I_SB(inode), LFS)) ||
			F2FS_I_SB(inode)->s_ndevs);
}

int f2fs_preallocate_blocks(struct inode *inode, loff_t pos,
					size_t count, bool dio)
{
	struct f2fs_map_blocks map;
	int err = 0;

	map.m_lblk = F2FS_BLK_ALIGN(pos);
	map.m_len = F2FS_BYTES_TO_BLK(pos + count);
	if (map.m_len > map.m_lblk)
		map.m_len -= map.m_lblk;
	else
		map.m_len = 0;

	map.m_next_pgofs = NULL;

	if (dio) {
		err = f2fs_convert_inline_inode(inode);
		if (err)
			return err;
		return f2fs_map_blocks(inode, &map, 1,
			__force_buffered_io(inode, WRITE) ?
				F2FS_GET_BLOCK_PRE_AIO :
				F2FS_GET_BLOCK_PRE_DIO);
	}
	if (pos + count > MAX_INLINE_DATA) {
		err = f2fs_convert_inline_inode(inode);
		if (err)
			return err;
	}
	if (!f2fs_has_inline_data(inode))
		return f2fs_map_blocks(inode, &map, 1, F2FS_GET_BLOCK_PRE_AIO);
	return err;
}

static inline void __do_map_lock(struct f2fs_sb_info *sbi, int flag, bool lock)
{
	if (flag == F2FS_GET_BLOCK_PRE_AIO) {
		if (lock)
			down_read(&sbi->node_change);
		else
			up_read(&sbi->node_change);
	} else {
		if (lock)
			f2fs_lock_op(sbi);
		else
			f2fs_unlock_op(sbi);
	}
}

/*
 * f2fs_map_blocks() now supported readahead/bmap/rw direct_IO with
 * f2fs_map_blocks structure.
=======
	struct f2fs_inode_info *fi = F2FS_I(dn->inode);
	struct f2fs_summary sum;
	block_t new_blkaddr;
	struct node_info ni;
	pgoff_t fofs;
	int type;

	if (unlikely(is_inode_flag_set(F2FS_I(dn->inode), FI_NO_ALLOC)))
		return -EPERM;
	if (unlikely(!inc_valid_block_count(sbi, dn->inode, 1)))
		return -ENOSPC;

	__set_data_blkaddr(dn, NEW_ADDR);
	dn->data_blkaddr = NEW_ADDR;

	get_node_info(sbi, dn->nid, &ni);
	set_summary(&sum, dn->nid, dn->ofs_in_node, ni.version);

	type = CURSEG_WARM_DATA;

	allocate_data_block(sbi, NULL, NULL_ADDR, &new_blkaddr, &sum, type);

	/* direct IO doesn't use extent cache to maximize the performance */
	set_inode_flag(F2FS_I(dn->inode), FI_NO_EXTENT);
	update_extent_cache(new_blkaddr, dn);
	clear_inode_flag(F2FS_I(dn->inode), FI_NO_EXTENT);

	/* update i_size */
	fofs = start_bidx_of_node(ofs_of_node(dn->node_page), fi) +
							dn->ofs_in_node;
	if (i_size_read(dn->inode) < ((fofs + 1) << PAGE_CACHE_SHIFT))
		i_size_write(dn->inode, ((fofs + 1) << PAGE_CACHE_SHIFT));

	dn->data_blkaddr = new_blkaddr;
	return 0;
}

/*
 * get_data_block() now supported readahead/bmap/rw direct_IO with mapped bh.
>>>>>>> v3.18
 * If original data blocks are allocated, then give them to blockdev.
 * Otherwise,
 *     a. preallocate requested block addresses
 *     b. do not use extent cache for better performance
 *     c. give the block addresses to blockdev
 */
<<<<<<< HEAD
int f2fs_map_blocks(struct inode *inode, struct f2fs_map_blocks *map,
						int create, int flag)
{
	unsigned int maxblocks = map->m_len;
	struct dnode_of_data dn;
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
	int mode = create ? ALLOC_NODE : LOOKUP_NODE;
	pgoff_t pgofs, end_offset, end;
	int err = 0, ofs = 1;
	unsigned int ofs_in_node, last_ofs_in_node;
	blkcnt_t prealloc;
	struct extent_info ei = {0,0,0};
	block_t blkaddr;

	if (!maxblocks)
		return 0;

	map->m_len = 0;
	map->m_flags = 0;

	/* it only supports block size == page size */
	pgofs =	(pgoff_t)map->m_lblk;
	end = pgofs + maxblocks;

	if (!create && f2fs_lookup_extent_cache(inode, pgofs, &ei)) {
		map->m_pblk = ei.blk + pgofs - ei.fofs;
		map->m_len = min((pgoff_t)maxblocks, ei.fofs + ei.len - pgofs);
		map->m_flags = F2FS_MAP_MAPPED;
		goto out;
	}

next_dnode:
	if (create)
		__do_map_lock(sbi, flag, true);
=======
static int __get_data_block(struct inode *inode, sector_t iblock,
			struct buffer_head *bh_result, int create, bool fiemap)
{
	unsigned int blkbits = inode->i_sb->s_blocksize_bits;
	unsigned maxblocks = bh_result->b_size >> blkbits;
	struct dnode_of_data dn;
	int mode = create ? ALLOC_NODE : LOOKUP_NODE_RA;
	pgoff_t pgofs, end_offset;
	int err = 0, ofs = 1;
	bool allocated = false;

	/* Get the page offset from the block offset(iblock) */
	pgofs =	(pgoff_t)(iblock >> (PAGE_CACHE_SHIFT - blkbits));

	if (check_extent_cache(inode, pgofs, bh_result))
		goto out;

	if (create) {
		f2fs_balance_fs(F2FS_I_SB(inode));
		f2fs_lock_op(F2FS_I_SB(inode));
	}
>>>>>>> v3.18

	/* When reading holes, we need its node page */
	set_new_dnode(&dn, inode, NULL, NULL, 0);
	err = get_dnode_of_data(&dn, pgofs, mode);
	if (err) {
<<<<<<< HEAD
		if (flag == F2FS_GET_BLOCK_BMAP)
			map->m_pblk = 0;
		if (err == -ENOENT) {
			err = 0;
			if (map->m_next_pgofs)
				*map->m_next_pgofs =
					get_next_page_offset(&dn, pgofs);
		}
		goto unlock_out;
	}

	prealloc = 0;
	last_ofs_in_node = ofs_in_node = dn.ofs_in_node;
	end_offset = ADDRS_PER_PAGE(dn.node_page, inode);

next_block:
	blkaddr = datablock_addr(dn.node_page, dn.ofs_in_node);

	if (blkaddr == NEW_ADDR || blkaddr == NULL_ADDR) {
		if (create) {
			if (unlikely(f2fs_cp_error(sbi))) {
				err = -EIO;
				goto sync_out;
			}
			if (flag == F2FS_GET_BLOCK_PRE_AIO) {
				if (blkaddr == NULL_ADDR) {
					prealloc++;
					last_ofs_in_node = dn.ofs_in_node;
				}
			} else {
				err = __allocate_data_block(&dn);
				if (!err)
					set_inode_flag(inode, FI_APPEND_WRITE);
			}
			if (err)
				goto sync_out;
			map->m_flags |= F2FS_MAP_NEW;
			blkaddr = dn.data_blkaddr;
		} else {
			if (flag == F2FS_GET_BLOCK_BMAP) {
				map->m_pblk = 0;
				goto sync_out;
			}
			if (flag == F2FS_GET_BLOCK_FIEMAP &&
						blkaddr == NULL_ADDR) {
				if (map->m_next_pgofs)
					*map->m_next_pgofs = pgofs + 1;
			}
			if (flag != F2FS_GET_BLOCK_FIEMAP ||
						blkaddr != NEW_ADDR)
				goto sync_out;
		}
	}

	if (flag == F2FS_GET_BLOCK_PRE_AIO)
		goto skip;

	if (map->m_len == 0) {
		/* preallocated unwritten block should be mapped for fiemap. */
		if (blkaddr == NEW_ADDR)
			map->m_flags |= F2FS_MAP_UNWRITTEN;
		map->m_flags |= F2FS_MAP_MAPPED;

		map->m_pblk = blkaddr;
		map->m_len = 1;
	} else if ((map->m_pblk != NEW_ADDR &&
			blkaddr == (map->m_pblk + ofs)) ||
			(map->m_pblk == NEW_ADDR && blkaddr == NEW_ADDR) ||
			flag == F2FS_GET_BLOCK_PRE_DIO) {
		ofs++;
		map->m_len++;
	} else {
		goto sync_out;
	}

skip:
	dn.ofs_in_node++;
	pgofs++;

	/* preallocate blocks in batch for one dnode page */
	if (flag == F2FS_GET_BLOCK_PRE_AIO &&
			(pgofs == end || dn.ofs_in_node == end_offset)) {

		dn.ofs_in_node = ofs_in_node;
		err = reserve_new_blocks(&dn, prealloc);
		if (err)
			goto sync_out;

		map->m_len += dn.ofs_in_node - ofs_in_node;
		if (prealloc && dn.ofs_in_node != last_ofs_in_node + 1) {
			err = -ENOSPC;
			goto sync_out;
		}
		dn.ofs_in_node = end_offset;
	}

	if (pgofs >= end)
		goto sync_out;
	else if (dn.ofs_in_node < end_offset)
		goto next_block;

	f2fs_put_dnode(&dn);

	if (create) {
		__do_map_lock(sbi, flag, false);
		f2fs_balance_fs(sbi, dn.node_changed);
	}
	goto next_dnode;

sync_out:
	f2fs_put_dnode(&dn);
unlock_out:
	if (create) {
		__do_map_lock(sbi, flag, false);
		f2fs_balance_fs(sbi, dn.node_changed);
	}
out:
	trace_f2fs_map_blocks(inode, map, err);
	return err;
}

static int __get_data_block(struct inode *inode, sector_t iblock,
			struct buffer_head *bh, int create, int flag,
			pgoff_t *next_pgofs)
{
	struct f2fs_map_blocks map;
	int err;

	map.m_lblk = iblock;
	map.m_len = bh->b_size >> inode->i_blkbits;
	map.m_next_pgofs = next_pgofs;

	err = f2fs_map_blocks(inode, &map, create, flag);
	if (!err) {
		map_bh(bh, inode->i_sb, map.m_pblk);
		bh->b_state = (bh->b_state & ~F2FS_MAP_FLAGS) | map.m_flags;
		bh->b_size = (u64)map.m_len << inode->i_blkbits;
	}
=======
		if (err == -ENOENT)
			err = 0;
		goto unlock_out;
	}
	if (dn.data_blkaddr == NEW_ADDR && !fiemap)
		goto put_out;

	if (dn.data_blkaddr != NULL_ADDR) {
		map_bh(bh_result, inode->i_sb, dn.data_blkaddr);
	} else if (create) {
		err = __allocate_data_block(&dn);
		if (err)
			goto put_out;
		allocated = true;
		map_bh(bh_result, inode->i_sb, dn.data_blkaddr);
	} else {
		goto put_out;
	}

	end_offset = ADDRS_PER_PAGE(dn.node_page, F2FS_I(inode));
	bh_result->b_size = (((size_t)1) << blkbits);
	dn.ofs_in_node++;
	pgofs++;

get_next:
	if (dn.ofs_in_node >= end_offset) {
		if (allocated)
			sync_inode_page(&dn);
		allocated = false;
		f2fs_put_dnode(&dn);

		set_new_dnode(&dn, inode, NULL, NULL, 0);
		err = get_dnode_of_data(&dn, pgofs, mode);
		if (err) {
			if (err == -ENOENT)
				err = 0;
			goto unlock_out;
		}
		if (dn.data_blkaddr == NEW_ADDR && !fiemap)
			goto put_out;

		end_offset = ADDRS_PER_PAGE(dn.node_page, F2FS_I(inode));
	}

	if (maxblocks > (bh_result->b_size >> blkbits)) {
		block_t blkaddr = datablock_addr(dn.node_page, dn.ofs_in_node);
		if (blkaddr == NULL_ADDR && create) {
			err = __allocate_data_block(&dn);
			if (err)
				goto sync_out;
			allocated = true;
			blkaddr = dn.data_blkaddr;
		}
		/* Give more consecutive addresses for the readahead */
		if (blkaddr == (bh_result->b_blocknr + ofs)) {
			ofs++;
			dn.ofs_in_node++;
			pgofs++;
			bh_result->b_size += (((size_t)1) << blkbits);
			goto get_next;
		}
	}
sync_out:
	if (allocated)
		sync_inode_page(&dn);
put_out:
	f2fs_put_dnode(&dn);
unlock_out:
	if (create)
		f2fs_unlock_op(F2FS_I_SB(inode));
out:
	trace_f2fs_get_data_block(inode, iblock, bh_result, err);
>>>>>>> v3.18
	return err;
}

static int get_data_block(struct inode *inode, sector_t iblock,
<<<<<<< HEAD
			struct buffer_head *bh_result, int create, int flag,
			pgoff_t *next_pgofs)
{
	return __get_data_block(inode, iblock, bh_result, create,
							flag, next_pgofs);
}

static int get_data_block_dio(struct inode *inode, sector_t iblock,
			struct buffer_head *bh_result, int create)
{
	return __get_data_block(inode, iblock, bh_result, create,
						F2FS_GET_BLOCK_DIO, NULL);
}

static int get_data_block_bmap(struct inode *inode, sector_t iblock,
			struct buffer_head *bh_result, int create)
{
	/* Block number less than F2FS MAX BLOCKS */
	if (unlikely(iblock >= F2FS_I_SB(inode)->max_file_blocks))
		return -EFBIG;

	return __get_data_block(inode, iblock, bh_result, create,
						F2FS_GET_BLOCK_BMAP, NULL);
}

static inline sector_t logical_to_blk(struct inode *inode, loff_t offset)
{
	return (offset >> inode->i_blkbits);
}

static inline loff_t blk_to_logical(struct inode *inode, sector_t blk)
{
	return (blk << inode->i_blkbits);
=======
			struct buffer_head *bh_result, int create)
{
	return __get_data_block(inode, iblock, bh_result, create, false);
}

static int get_data_block_fiemap(struct inode *inode, sector_t iblock,
			struct buffer_head *bh_result, int create)
{
	return __get_data_block(inode, iblock, bh_result, create, true);
>>>>>>> v3.18
}

int f2fs_fiemap(struct inode *inode, struct fiemap_extent_info *fieinfo,
		u64 start, u64 len)
{
<<<<<<< HEAD
	struct buffer_head map_bh;
	sector_t start_blk, last_blk;
	pgoff_t next_pgofs;
	u64 logical = 0, phys = 0, size = 0;
	u32 flags = 0;
	int ret = 0;

	ret = fiemap_check_flags(fieinfo, FIEMAP_FLAG_SYNC);
	if (ret)
		return ret;

	if (f2fs_has_inline_data(inode)) {
		ret = f2fs_inline_data_fiemap(inode, fieinfo, start, len);
		if (ret != -EAGAIN)
			return ret;
	}

	inode_lock(inode);

	if (logical_to_blk(inode, len) == 0)
		len = blk_to_logical(inode, 1);

	start_blk = logical_to_blk(inode, start);
	last_blk = logical_to_blk(inode, start + len - 1);

next:
	memset(&map_bh, 0, sizeof(struct buffer_head));
	map_bh.b_size = len;

	ret = get_data_block(inode, start_blk, &map_bh, 0,
					F2FS_GET_BLOCK_FIEMAP, &next_pgofs);
	if (ret)
		goto out;

	/* HOLE */
	if (!buffer_mapped(&map_bh)) {
		start_blk = next_pgofs;

		if (blk_to_logical(inode, start_blk) < blk_to_logical(inode,
					F2FS_I_SB(inode)->max_file_blocks))
			goto prep_next;

		flags |= FIEMAP_EXTENT_LAST;
	}

	if (size) {
		if (f2fs_encrypted_inode(inode))
			flags |= FIEMAP_EXTENT_DATA_ENCRYPTED;

		ret = fiemap_fill_next_extent(fieinfo, logical,
				phys, size, flags);
	}

	if (start_blk > last_blk || ret)
		goto out;

	logical = blk_to_logical(inode, start_blk);
	phys = blk_to_logical(inode, map_bh.b_blocknr);
	size = map_bh.b_size;
	flags = 0;
	if (buffer_unwritten(&map_bh))
		flags = FIEMAP_EXTENT_UNWRITTEN;

	start_blk += logical_to_blk(inode, size);

prep_next:
	cond_resched();
	if (fatal_signal_pending(current))
		ret = -EINTR;
	else
		goto next;
out:
	if (ret == 1)
		ret = 0;

	inode_unlock(inode);
	return ret;
}

static struct bio *f2fs_grab_bio(struct inode *inode, block_t blkaddr,
				 unsigned nr_pages)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
	struct fscrypt_ctx *ctx = NULL;
	struct bio *bio;

	if (f2fs_encrypted_inode(inode) && S_ISREG(inode->i_mode)) {
		ctx = fscrypt_get_ctx(inode, GFP_NOFS);
		if (IS_ERR(ctx))
			return ERR_CAST(ctx);

		/* wait the page to be moved by cleaning */
		f2fs_wait_on_encrypted_page_writeback(sbi, blkaddr);
	}

	bio = bio_alloc(GFP_KERNEL, min_t(int, nr_pages, BIO_MAX_PAGES));
	if (!bio) {
		if (ctx)
			fscrypt_release_ctx(ctx);
		return ERR_PTR(-ENOMEM);
	}
	f2fs_target_device(sbi, blkaddr, bio);
	bio->bi_end_io = f2fs_read_end_io;
	bio->bi_private = ctx;

	return bio;
}

/*
 * This function was originally taken from fs/mpage.c, and customized for f2fs.
 * Major change was from block_size == page_size in f2fs by default.
 */
static int f2fs_mpage_readpages(struct address_space *mapping,
			struct list_head *pages, struct page *page,
			unsigned nr_pages)
{
	struct bio *bio = NULL;
	unsigned page_idx;
	sector_t last_block_in_bio = 0;
	struct inode *inode = mapping->host;
	const unsigned blkbits = inode->i_blkbits;
	const unsigned blocksize = 1 << blkbits;
	sector_t block_in_file;
	sector_t last_block;
	sector_t last_block_in_file;
	sector_t block_nr;
	struct f2fs_map_blocks map;

	map.m_pblk = 0;
	map.m_lblk = 0;
	map.m_len = 0;
	map.m_flags = 0;
	map.m_next_pgofs = NULL;

	for (page_idx = 0; nr_pages; page_idx++, nr_pages--) {

		if (pages) {
			page = list_last_entry(pages, struct page, lru);

			prefetchw(&page->flags);
			list_del(&page->lru);
			if (add_to_page_cache_lru(page, mapping,
						  page->index, GFP_KERNEL))
				goto next_page;
		}

		block_in_file = (sector_t)page->index;
		last_block = block_in_file + nr_pages;
		last_block_in_file = (i_size_read(inode) + blocksize - 1) >>
								blkbits;
		if (last_block > last_block_in_file)
			last_block = last_block_in_file;

		/*
		 * Map blocks using the previous result first.
		 */
		if ((map.m_flags & F2FS_MAP_MAPPED) &&
				block_in_file > map.m_lblk &&
				block_in_file < (map.m_lblk + map.m_len))
			goto got_it;

		/*
		 * Then do more f2fs_map_blocks() calls until we are
		 * done with this page.
		 */
		map.m_flags = 0;

		if (block_in_file < last_block) {
			map.m_lblk = block_in_file;
			map.m_len = last_block - block_in_file;

			if (f2fs_map_blocks(inode, &map, 0,
						F2FS_GET_BLOCK_READ))
				goto set_error_page;
		}
got_it:
		if ((map.m_flags & F2FS_MAP_MAPPED)) {
			block_nr = map.m_pblk + block_in_file - map.m_lblk;
			SetPageMappedToDisk(page);

			if (!PageUptodate(page) && !cleancache_get_page(page)) {
				SetPageUptodate(page);
				goto confused;
			}
		} else {
			zero_user_segment(page, 0, PAGE_SIZE);
			if (!PageUptodate(page))
				SetPageUptodate(page);
			unlock_page(page);
			goto next_page;
		}

		/*
		 * This page will go to BIO.  Do we need to send this
		 * BIO off first?
		 */
		if (bio && (last_block_in_bio != block_nr - 1 ||
			!__same_bdev(F2FS_I_SB(inode), block_nr, bio))) {
submit_and_realloc:
			__submit_bio(F2FS_I_SB(inode), bio, DATA);
			bio = NULL;
		}
		if (bio == NULL) {
			bio = f2fs_grab_bio(inode, block_nr, nr_pages);
			if (IS_ERR(bio)) {
				bio = NULL;
				goto set_error_page;
			}
			bio_set_op_attrs(bio, REQ_OP_READ, 0);
		}

		if (bio_add_page(bio, page, blocksize, 0) < blocksize)
			goto submit_and_realloc;

		last_block_in_bio = block_nr;
		goto next_page;
set_error_page:
		SetPageError(page);
		zero_user_segment(page, 0, PAGE_SIZE);
		unlock_page(page);
		goto next_page;
confused:
		if (bio) {
			__submit_bio(F2FS_I_SB(inode), bio, DATA);
			bio = NULL;
		}
		unlock_page(page);
next_page:
		if (pages)
			put_page(page);
	}
	BUG_ON(pages && !list_empty(pages));
	if (bio)
		__submit_bio(F2FS_I_SB(inode), bio, DATA);
	return 0;
=======
	return generic_block_fiemap(inode, fieinfo,
				start, len, get_data_block_fiemap);
>>>>>>> v3.18
}

static int f2fs_read_data_page(struct file *file, struct page *page)
{
	struct inode *inode = page->mapping->host;
<<<<<<< HEAD
	int ret = -EAGAIN;
=======
	int ret;
>>>>>>> v3.18

	trace_f2fs_readpage(page, DATA);

	/* If the file has inline data, try to read it directly */
	if (f2fs_has_inline_data(inode))
		ret = f2fs_read_inline_data(inode, page);
<<<<<<< HEAD
	if (ret == -EAGAIN)
		ret = f2fs_mpage_readpages(page->mapping, NULL, page, 1);
=======
	else
		ret = mpage_readpage(page, get_data_block);

>>>>>>> v3.18
	return ret;
}

static int f2fs_read_data_pages(struct file *file,
			struct address_space *mapping,
			struct list_head *pages, unsigned nr_pages)
{
	struct inode *inode = file->f_mapping->host;
<<<<<<< HEAD
	struct page *page = list_last_entry(pages, struct page, lru);

	trace_f2fs_readpages(inode, page, nr_pages);
=======
>>>>>>> v3.18

	/* If the file has inline data, skip readpages */
	if (f2fs_has_inline_data(inode))
		return 0;

<<<<<<< HEAD
	return f2fs_mpage_readpages(mapping, pages, NULL, nr_pages);
}

static int encrypt_one_page(struct f2fs_io_info *fio)
{
	struct inode *inode = fio->page->mapping->host;
	gfp_t gfp_flags = GFP_NOFS;

	if (!f2fs_encrypted_inode(inode) || !S_ISREG(inode->i_mode))
		return 0;

	/* wait for GCed encrypted page writeback */
	f2fs_wait_on_encrypted_page_writeback(fio->sbi, fio->old_blkaddr);

retry_encrypt:
	fio->encrypted_page = fscrypt_encrypt_page(inode, fio->page,
			PAGE_SIZE, 0, fio->page->index, gfp_flags);
	if (!IS_ERR(fio->encrypted_page))
		return 0;

	/* flush pending IOs and wait for a while in the ENOMEM case */
	if (PTR_ERR(fio->encrypted_page) == -ENOMEM) {
		f2fs_flush_merged_writes(fio->sbi);
		congestion_wait(BLK_RW_ASYNC, HZ/50);
		gfp_flags |= __GFP_NOFAIL;
		goto retry_encrypt;
	}
	return PTR_ERR(fio->encrypted_page);
}

static inline bool need_inplace_update(struct f2fs_io_info *fio)
{
	struct inode *inode = fio->page->mapping->host;

	if (S_ISDIR(inode->i_mode) || f2fs_is_atomic_file(inode))
		return false;
	if (is_cold_data(fio->page))
		return false;
	if (IS_ATOMIC_WRITTEN_PAGE(fio->page))
		return false;

	return need_inplace_update_policy(inode, fio);
}

static inline bool valid_ipu_blkaddr(struct f2fs_io_info *fio)
{
	if (fio->old_blkaddr == NEW_ADDR)
		return false;
	if (fio->old_blkaddr == NULL_ADDR)
		return false;
	return true;
}

int do_write_data_page(struct f2fs_io_info *fio)
{
	struct page *page = fio->page;
	struct inode *inode = page->mapping->host;
	struct dnode_of_data dn;
	struct extent_info ei = {0,0,0};
	bool ipu_force = false;
	int err = 0;

	set_new_dnode(&dn, inode, NULL, NULL, 0);
	if (need_inplace_update(fio) &&
			f2fs_lookup_extent_cache(inode, page->index, &ei)) {
		fio->old_blkaddr = ei.blk + page->index - ei.fofs;

		if (valid_ipu_blkaddr(fio)) {
			ipu_force = true;
			fio->need_lock = LOCK_DONE;
			goto got_it;
		}
	}

	/* Deadlock due to between page->lock and f2fs_lock_op */
	if (fio->need_lock == LOCK_REQ && !f2fs_trylock_op(fio->sbi))
		return -EAGAIN;

	err = get_dnode_of_data(&dn, page->index, LOOKUP_NODE);
	if (err)
		goto out;

	fio->old_blkaddr = dn.data_blkaddr;

	/* This page is already truncated */
	if (fio->old_blkaddr == NULL_ADDR) {
		ClearPageUptodate(page);
		goto out_writepage;
	}
got_it:
	/*
	 * If current allocation needs SSR,
	 * it had better in-place writes for updated data.
	 */
	if (ipu_force || (valid_ipu_blkaddr(fio) && need_inplace_update(fio))) {
		err = encrypt_one_page(fio);
		if (err)
			goto out_writepage;

		set_page_writeback(page);
		f2fs_put_dnode(&dn);
		if (fio->need_lock == LOCK_REQ)
			f2fs_unlock_op(fio->sbi);
		err = rewrite_data_page(fio);
		trace_f2fs_do_write_data_page(fio->page, IPU);
		set_inode_flag(inode, FI_UPDATE_WRITE);
		return err;
	}

	if (fio->need_lock == LOCK_RETRY) {
		if (!f2fs_trylock_op(fio->sbi)) {
			err = -EAGAIN;
			goto out_writepage;
		}
		fio->need_lock = LOCK_REQ;
	}

	err = encrypt_one_page(fio);
	if (err)
=======
	return mpage_readpages(mapping, pages, nr_pages, get_data_block);
}

int do_write_data_page(struct page *page, struct f2fs_io_info *fio)
{
	struct inode *inode = page->mapping->host;
	block_t old_blkaddr, new_blkaddr;
	struct dnode_of_data dn;
	int err = 0;

	set_new_dnode(&dn, inode, NULL, NULL, 0);
	err = get_dnode_of_data(&dn, page->index, LOOKUP_NODE);
	if (err)
		return err;

	old_blkaddr = dn.data_blkaddr;

	/* This page is already truncated */
	if (old_blkaddr == NULL_ADDR)
>>>>>>> v3.18
		goto out_writepage;

	set_page_writeback(page);

<<<<<<< HEAD
	/* LFS mode write path */
	write_data_page(&dn, fio);
	trace_f2fs_do_write_data_page(page, OPU);
	set_inode_flag(inode, FI_APPEND_WRITE);
	if (page->index == 0)
		set_inode_flag(inode, FI_FIRST_BLOCK_WRITTEN);
out_writepage:
	f2fs_put_dnode(&dn);
out:
	if (fio->need_lock == LOCK_REQ)
		f2fs_unlock_op(fio->sbi);
	return err;
}

static int __write_data_page(struct page *page, bool *submitted,
				struct writeback_control *wbc)
=======
	/*
	 * If current allocation needs SSR,
	 * it had better in-place writes for updated data.
	 */
	if (unlikely(old_blkaddr != NEW_ADDR &&
			!is_cold_data(page) &&
			need_inplace_update(inode))) {
		rewrite_data_page(page, old_blkaddr, fio);
		set_inode_flag(F2FS_I(inode), FI_UPDATE_WRITE);
	} else {
		write_data_page(page, &dn, &new_blkaddr, fio);
		update_extent_cache(new_blkaddr, &dn);
		set_inode_flag(F2FS_I(inode), FI_APPEND_WRITE);
	}
out_writepage:
	f2fs_put_dnode(&dn);
	return err;
}

static int f2fs_write_data_page(struct page *page,
					struct writeback_control *wbc)
>>>>>>> v3.18
{
	struct inode *inode = page->mapping->host;
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
	loff_t i_size = i_size_read(inode);
	const pgoff_t end_index = ((unsigned long long) i_size)
<<<<<<< HEAD
							>> PAGE_SHIFT;
	loff_t psize = (page->index + 1) << PAGE_SHIFT;
=======
							>> PAGE_CACHE_SHIFT;
>>>>>>> v3.18
	unsigned offset = 0;
	bool need_balance_fs = false;
	int err = 0;
	struct f2fs_io_info fio = {
<<<<<<< HEAD
		.sbi = sbi,
		.type = DATA,
		.op = REQ_OP_WRITE,
		.op_flags = wbc_to_write_flags(wbc),
		.old_blkaddr = NULL_ADDR,
		.page = page,
		.encrypted_page = NULL,
		.submitted = false,
		.need_lock = LOCK_RETRY,
=======
		.type = DATA,
		.rw = (wbc->sync_mode == WB_SYNC_ALL) ? WRITE_SYNC : WRITE,
>>>>>>> v3.18
	};

	trace_f2fs_writepage(page, DATA);

<<<<<<< HEAD
	if (unlikely(is_sbi_flag_set(sbi, SBI_POR_DOING)))
		goto redirty_out;

=======
>>>>>>> v3.18
	if (page->index < end_index)
		goto write;

	/*
	 * If the offset is out-of-range of file size,
	 * this page does not have to be written to disk.
	 */
<<<<<<< HEAD
	offset = i_size & (PAGE_SIZE - 1);
	if ((page->index >= end_index + 1) || !offset)
		goto out;

	zero_user_segment(page, offset, PAGE_SIZE);
write:
	if (f2fs_is_drop_cache(inode))
		goto out;
	/* we should not write 0'th page having journal header */
	if (f2fs_is_volatile_file(inode) && (!page->index ||
			(!wbc->for_reclaim &&
			available_free_memory(sbi, BASE_CHECK))))
		goto redirty_out;

	/* we should bypass data pages to proceed the kworkder jobs */
	if (unlikely(f2fs_cp_error(sbi))) {
		mapping_set_error(page->mapping, -EIO);
		goto out;
	}

	/* Dentry blocks are controlled by checkpoint */
	if (S_ISDIR(inode->i_mode)) {
		fio.need_lock = LOCK_DONE;
		err = do_write_data_page(&fio);
		goto done;
	}

	if (!wbc->for_reclaim)
		need_balance_fs = true;
	else if (has_not_enough_free_secs(sbi, 0, 0))
		goto redirty_out;
	else
		set_inode_flag(inode, FI_HOT_DATA);

	err = -EAGAIN;
	if (f2fs_has_inline_data(inode)) {
		err = f2fs_write_inline_data(inode, page);
		if (!err)
			goto out;
	}

	if (err == -EAGAIN) {
		err = do_write_data_page(&fio);
		if (err == -EAGAIN) {
			fio.need_lock = LOCK_REQ;
			err = do_write_data_page(&fio);
		}
	}
	if (F2FS_I(inode)->last_disk_size < psize)
		F2FS_I(inode)->last_disk_size = psize;

=======
	offset = i_size & (PAGE_CACHE_SIZE - 1);
	if ((page->index >= end_index + 1) || !offset)
		goto out;

	zero_user_segment(page, offset, PAGE_CACHE_SIZE);
write:
	if (unlikely(sbi->por_doing))
		goto redirty_out;

	/* Dentry blocks are controlled by checkpoint */
	if (S_ISDIR(inode->i_mode)) {
		if (unlikely(f2fs_cp_error(sbi)))
			goto redirty_out;
		err = do_write_data_page(page, &fio);
		goto done;
	}

	/* we should bypass data pages to proceed the kworkder jobs */
	if (unlikely(f2fs_cp_error(sbi))) {
		SetPageError(page);
		unlock_page(page);
		goto out;
	}

	if (!wbc->for_reclaim)
		need_balance_fs = true;
	else if (has_not_enough_free_secs(sbi, 0))
		goto redirty_out;

	f2fs_lock_op(sbi);
	if (f2fs_has_inline_data(inode) || f2fs_may_inline(inode))
		err = f2fs_write_inline_data(inode, page, offset);
	else
		err = do_write_data_page(page, &fio);
	f2fs_unlock_op(sbi);
>>>>>>> v3.18
done:
	if (err && err != -ENOENT)
		goto redirty_out;

<<<<<<< HEAD
out:
	inode_dec_dirty_pages(inode);
	if (err)
		ClearPageUptodate(page);

	if (wbc->for_reclaim) {
		f2fs_submit_merged_write_cond(sbi, inode, 0, page->index, DATA);
		clear_inode_flag(inode, FI_HOT_DATA);
		remove_dirty_inode(inode);
		submitted = NULL;
	}

	unlock_page(page);
	if (!S_ISDIR(inode->i_mode))
		f2fs_balance_fs(sbi, need_balance_fs);

	if (unlikely(f2fs_cp_error(sbi))) {
		f2fs_submit_merged_write(sbi, DATA);
		submitted = NULL;
	}

	if (submitted)
		*submitted = fio.submitted;

=======
	clear_cold_data(page);
out:
	inode_dec_dirty_pages(inode);
	unlock_page(page);
	if (need_balance_fs)
		f2fs_balance_fs(sbi);
	if (wbc->for_reclaim)
		f2fs_submit_merged_bio(sbi, DATA, WRITE);
>>>>>>> v3.18
	return 0;

redirty_out:
	redirty_page_for_writepage(wbc, page);
<<<<<<< HEAD
	if (!err)
		return AOP_WRITEPAGE_ACTIVATE;
	unlock_page(page);
	return err;
}

static int f2fs_write_data_page(struct page *page,
					struct writeback_control *wbc)
{
	return __write_data_page(page, NULL, wbc);
}

/*
 * This function was copied from write_cche_pages from mm/page-writeback.c.
 * The major change is making write step of cold data page separately from
 * warm/hot data page.
 */
static int f2fs_write_cache_pages(struct address_space *mapping,
					struct writeback_control *wbc)
{
	int ret = 0;
	int done = 0;
	struct pagevec pvec;
	int nr_pages;
	pgoff_t uninitialized_var(writeback_index);
	pgoff_t index;
	pgoff_t end;		/* Inclusive */
	pgoff_t done_index;
	pgoff_t last_idx = ULONG_MAX;
	int cycled;
	int range_whole = 0;
	int tag;

	pagevec_init(&pvec, 0);

	if (get_dirty_pages(mapping->host) <=
				SM_I(F2FS_M_SB(mapping))->min_hot_blocks)
		set_inode_flag(mapping->host, FI_HOT_DATA);
	else
		clear_inode_flag(mapping->host, FI_HOT_DATA);

	if (wbc->range_cyclic) {
		writeback_index = mapping->writeback_index; /* prev offset */
		index = writeback_index;
		if (index == 0)
			cycled = 1;
		else
			cycled = 0;
		end = -1;
	} else {
		index = wbc->range_start >> PAGE_SHIFT;
		end = wbc->range_end >> PAGE_SHIFT;
		if (wbc->range_start == 0 && wbc->range_end == LLONG_MAX)
			range_whole = 1;
		cycled = 1; /* ignore range_cyclic tests */
	}
	if (wbc->sync_mode == WB_SYNC_ALL || wbc->tagged_writepages)
		tag = PAGECACHE_TAG_TOWRITE;
	else
		tag = PAGECACHE_TAG_DIRTY;
retry:
	if (wbc->sync_mode == WB_SYNC_ALL || wbc->tagged_writepages)
		tag_pages_for_writeback(mapping, index, end);
	done_index = index;
	while (!done && (index <= end)) {
		int i;

		nr_pages = pagevec_lookup_tag(&pvec, mapping, &index, tag,
			      min(end - index, (pgoff_t)PAGEVEC_SIZE - 1) + 1);
		if (nr_pages == 0)
			break;

		for (i = 0; i < nr_pages; i++) {
			struct page *page = pvec.pages[i];
			bool submitted = false;

			if (page->index > end) {
				done = 1;
				break;
			}

			done_index = page->index;
retry_write:
			lock_page(page);

			if (unlikely(page->mapping != mapping)) {
continue_unlock:
				unlock_page(page);
				continue;
			}

			if (!PageDirty(page)) {
				/* someone wrote it for us */
				goto continue_unlock;
			}

			if (PageWriteback(page)) {
				if (wbc->sync_mode != WB_SYNC_NONE)
					f2fs_wait_on_page_writeback(page,
								DATA, true);
				else
					goto continue_unlock;
			}

			BUG_ON(PageWriteback(page));
			if (!clear_page_dirty_for_io(page))
				goto continue_unlock;

			ret = __write_data_page(page, &submitted, wbc);
			if (unlikely(ret)) {
				/*
				 * keep nr_to_write, since vfs uses this to
				 * get # of written pages.
				 */
				if (ret == AOP_WRITEPAGE_ACTIVATE) {
					unlock_page(page);
					ret = 0;
					continue;
				} else if (ret == -EAGAIN) {
					ret = 0;
					if (wbc->sync_mode == WB_SYNC_ALL) {
						cond_resched();
						congestion_wait(BLK_RW_ASYNC,
									HZ/50);
						goto retry_write;
					}
					continue;
				}
				done_index = page->index + 1;
				done = 1;
				break;
			} else if (submitted) {
				last_idx = page->index;
			}

			/* give a priority to WB_SYNC threads */
			if ((atomic_read(&F2FS_M_SB(mapping)->wb_sync_req) ||
					--wbc->nr_to_write <= 0) &&
					wbc->sync_mode == WB_SYNC_NONE) {
				done = 1;
				break;
			}
		}
		pagevec_release(&pvec);
		cond_resched();
	}

	if (!cycled && !done) {
		cycled = 1;
		index = 0;
		end = writeback_index - 1;
		goto retry;
	}
	if (wbc->range_cyclic || (range_whole && wbc->nr_to_write > 0))
		mapping->writeback_index = done_index;

	if (last_idx != ULONG_MAX)
		f2fs_submit_merged_write_cond(F2FS_M_SB(mapping), mapping->host,
						0, last_idx, DATA);

=======
	return AOP_WRITEPAGE_ACTIVATE;
}

static int __f2fs_writepage(struct page *page, struct writeback_control *wbc,
			void *data)
{
	struct address_space *mapping = data;
	int ret = mapping->a_ops->writepage(page, wbc);
	mapping_set_error(mapping, ret);
>>>>>>> v3.18
	return ret;
}

static int f2fs_write_data_pages(struct address_space *mapping,
			    struct writeback_control *wbc)
{
	struct inode *inode = mapping->host;
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
<<<<<<< HEAD
	struct blk_plug plug;
	int ret;
=======
	bool locked = false;
	int ret;
	long diff;

	trace_f2fs_writepages(mapping->host, wbc, DATA);
>>>>>>> v3.18

	/* deal with chardevs and other special file */
	if (!mapping->a_ops->writepage)
		return 0;

<<<<<<< HEAD
	/* skip writing if there is no dirty page in this inode */
	if (!get_dirty_pages(inode) && wbc->sync_mode == WB_SYNC_NONE)
		return 0;

	/* during POR, we don't need to trigger writepage at all. */
	if (unlikely(is_sbi_flag_set(sbi, SBI_POR_DOING)))
		goto skip_write;

=======
>>>>>>> v3.18
	if (S_ISDIR(inode->i_mode) && wbc->sync_mode == WB_SYNC_NONE &&
			get_dirty_pages(inode) < nr_pages_to_skip(sbi, DATA) &&
			available_free_memory(sbi, DIRTY_DENTS))
		goto skip_write;

<<<<<<< HEAD
	/* skip writing during file defragment */
	if (is_inode_flag_set(inode, FI_DO_DEFRAG))
		goto skip_write;

	trace_f2fs_writepages(mapping->host, wbc, DATA);

	/* to avoid spliting IOs due to mixed WB_SYNC_ALL and WB_SYNC_NONE */
	if (wbc->sync_mode == WB_SYNC_ALL)
		atomic_inc(&sbi->wb_sync_req);
	else if (atomic_read(&sbi->wb_sync_req))
		goto skip_write;

	blk_start_plug(&plug);
	ret = f2fs_write_cache_pages(mapping, wbc);
	blk_finish_plug(&plug);

	if (wbc->sync_mode == WB_SYNC_ALL)
		atomic_dec(&sbi->wb_sync_req);
	/*
	 * if some pages were truncated, we cannot guarantee its mapping->host
	 * to detect pending bios.
	 */

	remove_dirty_inode(inode);
=======
	diff = nr_pages_to_write(sbi, DATA, wbc);

	if (!S_ISDIR(inode->i_mode)) {
		mutex_lock(&sbi->writepages);
		locked = true;
	}
	ret = write_cache_pages(mapping, wbc, __f2fs_writepage, mapping);
	if (locked)
		mutex_unlock(&sbi->writepages);

	f2fs_submit_merged_bio(sbi, DATA, WRITE);

	remove_dirty_dir_inode(inode);

	wbc->nr_to_write = max((long)0, wbc->nr_to_write - diff);
>>>>>>> v3.18
	return ret;

skip_write:
	wbc->pages_skipped += get_dirty_pages(inode);
<<<<<<< HEAD
	trace_f2fs_writepages(mapping->host, wbc, DATA);
=======
>>>>>>> v3.18
	return 0;
}

static void f2fs_write_failed(struct address_space *mapping, loff_t to)
{
	struct inode *inode = mapping->host;
<<<<<<< HEAD
	loff_t i_size = i_size_read(inode);

	if (to > i_size) {
		down_write(&F2FS_I(inode)->i_mmap_sem);
		truncate_pagecache(inode, 0, i_size);
		truncate_blocks(inode, i_size, true);
		up_write(&F2FS_I(inode)->i_mmap_sem);
	}
}

static int prepare_write_begin(struct f2fs_sb_info *sbi,
			struct page *page, loff_t pos, unsigned len,
			block_t *blk_addr, bool *node_changed)
{
	struct inode *inode = page->mapping->host;
	pgoff_t index = page->index;
	struct dnode_of_data dn;
	struct page *ipage;
	bool locked = false;
	struct extent_info ei = {0,0,0};
	int err = 0;

	/*
	 * we already allocated all the blocks, so we don't need to get
	 * the block addresses when there is no need to fill the page.
	 */
	if (!f2fs_has_inline_data(inode) && len == PAGE_SIZE)
		return 0;

	if (f2fs_has_inline_data(inode) ||
			(pos & PAGE_MASK) >= i_size_read(inode)) {
		__do_map_lock(sbi, F2FS_GET_BLOCK_PRE_AIO, true);
		locked = true;
	}
restart:
	/* check inline_data */
	ipage = get_node_page(sbi, inode->i_ino);
	if (IS_ERR(ipage)) {
		err = PTR_ERR(ipage);
		goto unlock_out;
	}

	set_new_dnode(&dn, inode, ipage, ipage, 0);

	if (f2fs_has_inline_data(inode)) {
		if (pos + len <= MAX_INLINE_DATA) {
			read_inline_data(page, ipage);
			set_inode_flag(inode, FI_DATA_EXIST);
			if (inode->i_nlink)
				set_inline_node(ipage);
		} else {
			err = f2fs_convert_inline_page(&dn, page);
			if (err)
				goto out;
			if (dn.data_blkaddr == NULL_ADDR)
				err = f2fs_get_block(&dn, index);
		}
	} else if (locked) {
		err = f2fs_get_block(&dn, index);
	} else {
		if (f2fs_lookup_extent_cache(inode, index, &ei)) {
			dn.data_blkaddr = ei.blk + index - ei.fofs;
		} else {
			/* hole case */
			err = get_dnode_of_data(&dn, index, LOOKUP_NODE);
			if (err || dn.data_blkaddr == NULL_ADDR) {
				f2fs_put_dnode(&dn);
				__do_map_lock(sbi, F2FS_GET_BLOCK_PRE_AIO,
								true);
				locked = true;
				goto restart;
			}
		}
	}

	/* convert_inline_page can make node_changed */
	*blk_addr = dn.data_blkaddr;
	*node_changed = dn.node_changed;
out:
	f2fs_put_dnode(&dn);
unlock_out:
	if (locked)
		__do_map_lock(sbi, F2FS_GET_BLOCK_PRE_AIO, false);
	return err;
=======

	if (to > inode->i_size) {
		truncate_pagecache(inode, inode->i_size);
		truncate_blocks(inode, inode->i_size, true);
	}
>>>>>>> v3.18
}

static int f2fs_write_begin(struct file *file, struct address_space *mapping,
		loff_t pos, unsigned len, unsigned flags,
		struct page **pagep, void **fsdata)
{
	struct inode *inode = mapping->host;
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
<<<<<<< HEAD
	struct page *page = NULL;
	pgoff_t index = ((unsigned long long) pos) >> PAGE_SHIFT;
	bool need_balance = false;
	block_t blkaddr = NULL_ADDR;
=======
	struct page *page;
	pgoff_t index = ((unsigned long long) pos) >> PAGE_CACHE_SHIFT;
	struct dnode_of_data dn;
>>>>>>> v3.18
	int err = 0;

	trace_f2fs_write_begin(inode, pos, len, flags);

<<<<<<< HEAD
	/*
	 * We should check this at this moment to avoid deadlock on inode page
	 * and #0 page. The locking rule for inline_data conversion should be:
	 * lock_page(page #0) -> lock_page(inode_page)
	 */
	if (index != 0) {
		err = f2fs_convert_inline_inode(inode);
		if (err)
			goto fail;
	}
repeat:
	/*
	 * Do not use grab_cache_page_write_begin() to avoid deadlock due to
	 * wait_for_stable_page. Will wait that below with our IO control.
	 */
	page = grab_cache_page(mapping, index);
=======
	f2fs_balance_fs(sbi);
repeat:
	err = f2fs_convert_inline_data(inode, pos + len, NULL);
	if (err)
		goto fail;

	page = grab_cache_page_write_begin(mapping, index, flags);
>>>>>>> v3.18
	if (!page) {
		err = -ENOMEM;
		goto fail;
	}

<<<<<<< HEAD
	*pagep = page;

	err = prepare_write_begin(sbi, page, pos, len,
					&blkaddr, &need_balance);
	if (err)
		goto fail;

	if (need_balance && has_not_enough_free_secs(sbi, 0, 0)) {
		unlock_page(page);
		f2fs_balance_fs(sbi, true);
		lock_page(page);
		if (page->mapping != mapping) {
			/* The page got truncated from under us */
			f2fs_put_page(page, 1);
			goto repeat;
		}
	}

	f2fs_wait_on_page_writeback(page, DATA, false);

	/* wait for GCed encrypted page writeback */
	if (f2fs_encrypted_inode(inode) && S_ISREG(inode->i_mode))
		f2fs_wait_on_encrypted_page_writeback(sbi, blkaddr);

	if (len == PAGE_SIZE || PageUptodate(page))
		return 0;

	if (!(pos & (PAGE_SIZE - 1)) && (pos + len) >= i_size_read(inode)) {
		zero_user_segment(page, len, PAGE_SIZE);
		return 0;
	}

	if (blkaddr == NEW_ADDR) {
		zero_user_segment(page, 0, PAGE_SIZE);
		SetPageUptodate(page);
	} else {
		struct bio *bio;

		bio = f2fs_grab_bio(inode, blkaddr, 1);
		if (IS_ERR(bio)) {
			err = PTR_ERR(bio);
			goto fail;
		}
		bio->bi_rw = READ_SYNC;
		if (bio_add_page(bio, page, PAGE_SIZE, 0) < PAGE_SIZE) {
			bio_put(bio);
			err = -EFAULT;
			goto fail;
		}

		__submit_bio(sbi, bio, DATA);

		lock_page(page);
		if (unlikely(page->mapping != mapping)) {
			f2fs_put_page(page, 1);
			goto repeat;
		}
		if (unlikely(!PageUptodate(page))) {
			err = -EIO;
			goto fail;
		}
	}
	return 0;

fail:
	f2fs_put_page(page, 1);
=======
	/* to avoid latency during memory pressure */
	unlock_page(page);

	*pagep = page;

	if (f2fs_has_inline_data(inode) && (pos + len) <= MAX_INLINE_DATA)
		goto inline_data;

	f2fs_lock_op(sbi);
	set_new_dnode(&dn, inode, NULL, NULL, 0);
	err = f2fs_reserve_block(&dn, index);
	f2fs_unlock_op(sbi);
	if (err) {
		f2fs_put_page(page, 0);
		goto fail;
	}
inline_data:
	lock_page(page);
	if (unlikely(page->mapping != mapping)) {
		f2fs_put_page(page, 1);
		goto repeat;
	}

	f2fs_wait_on_page_writeback(page, DATA);

	if ((len == PAGE_CACHE_SIZE) || PageUptodate(page))
		return 0;

	if ((pos & PAGE_CACHE_MASK) >= i_size_read(inode)) {
		unsigned start = pos & (PAGE_CACHE_SIZE - 1);
		unsigned end = start + len;

		/* Reading beyond i_size is simple: memset to zero */
		zero_user_segments(page, 0, start, end, PAGE_CACHE_SIZE);
		goto out;
	}

	if (dn.data_blkaddr == NEW_ADDR) {
		zero_user_segment(page, 0, PAGE_CACHE_SIZE);
	} else {
		if (f2fs_has_inline_data(inode)) {
			err = f2fs_read_inline_data(inode, page);
			if (err) {
				page_cache_release(page);
				goto fail;
			}
		} else {
			err = f2fs_submit_page_bio(sbi, page, dn.data_blkaddr,
							READ_SYNC);
			if (err)
				goto fail;
		}

		lock_page(page);
		if (unlikely(!PageUptodate(page))) {
			f2fs_put_page(page, 1);
			err = -EIO;
			goto fail;
		}
		if (unlikely(page->mapping != mapping)) {
			f2fs_put_page(page, 1);
			goto repeat;
		}
	}
out:
	SetPageUptodate(page);
	clear_cold_data(page);
	return 0;
fail:
>>>>>>> v3.18
	f2fs_write_failed(mapping, pos + len);
	return err;
}

static int f2fs_write_end(struct file *file,
			struct address_space *mapping,
			loff_t pos, unsigned len, unsigned copied,
			struct page *page, void *fsdata)
{
	struct inode *inode = page->mapping->host;

	trace_f2fs_write_end(inode, pos, len, copied);

<<<<<<< HEAD
	/*
	 * This should be come from len == PAGE_SIZE, and we expect copied
	 * should be PAGE_SIZE. Otherwise, we treat it with zero copied and
	 * let generic_perform_write() try to copy data again through copied=0.
	 */
	if (!PageUptodate(page)) {
		if (unlikely(copied != len))
			copied = 0;
		else
			SetPageUptodate(page);
	}
	if (!copied)
		goto unlock_out;

	set_page_dirty(page);

	if (pos + copied > i_size_read(inode))
		f2fs_i_size_write(inode, pos + copied);
unlock_out:
	f2fs_put_page(page, 1);
	f2fs_update_time(F2FS_I_SB(inode), REQ_TIME);
	return copied;
}

static ssize_t check_direct_IO(struct inode *inode, int rw,
		const struct iovec *iov, loff_t offset, unsigned long nr_segs)
{
	unsigned blocksize_mask = inode->i_sb->s_blocksize - 1;
	int seg, i;
	size_t size;
	unsigned long addr;
	ssize_t retval = -EINVAL;
	loff_t end = offset;
=======
	if (f2fs_is_atomic_file(inode) || f2fs_is_volatile_file(inode))
		register_inmem_page(inode, page);
	else
		set_page_dirty(page);

	if (pos + copied > i_size_read(inode)) {
		i_size_write(inode, pos + copied);
		mark_inode_dirty(inode);
		update_inode_page(inode);
	}

	f2fs_put_page(page, 1);
	return copied;
}

static int check_direct_IO(struct inode *inode, int rw,
		struct iov_iter *iter, loff_t offset)
{
	unsigned blocksize_mask = inode->i_sb->s_blocksize - 1;

	if (rw == READ)
		return 0;
>>>>>>> v3.18

	if (offset & blocksize_mask)
		return -EINVAL;

<<<<<<< HEAD
	/* Check the memory alignment.  Blocks cannot straddle pages */
	for (seg = 0; seg < nr_segs; seg++) {
		addr = (unsigned long)iov[seg].iov_base;
		size = iov[seg].iov_len;
		end += size;
		if ((addr & blocksize_mask) || (size & blocksize_mask))
			goto out;

		/* If this is a write we don't need to check anymore */
		if (rw & WRITE)
			continue;

		/*
		 * Check to make sure we don't have duplicate iov_base's in this
		 * iovec, if so return EINVAL, otherwise we'll get csum errors
		 * when reading back.
		 */
		for (i = seg + 1; i < nr_segs; i++) {
			if (iov[seg].iov_base == iov[i].iov_base)
				goto out;
		}
	}
	retval = 0;
out:
	return retval;
}

static ssize_t f2fs_direct_IO(int rw, struct kiocb *iocb,
				const struct iovec *iov, loff_t offset,
				unsigned long nr_segs)
{
	struct address_space *mapping = iocb->ki_filp->f_mapping;
	struct inode *inode = mapping->host;
	size_t count = iov_length(iov, nr_segs);
	int err;

	err = check_direct_IO(inode, rw, iov, offset, nr_segs);
	if (err)
		return err;

	if (__force_buffered_io(inode, rw))
=======
	if (iov_iter_alignment(iter) & blocksize_mask)
		return -EINVAL;

	return 0;
}

static ssize_t f2fs_direct_IO(int rw, struct kiocb *iocb,
		struct iov_iter *iter, loff_t offset)
{
	struct file *file = iocb->ki_filp;
	struct address_space *mapping = file->f_mapping;
	struct inode *inode = mapping->host;
	size_t count = iov_iter_count(iter);
	int err;

	/* Let buffer I/O handle the inline data case. */
	if (f2fs_has_inline_data(inode))
		return 0;

	if (check_direct_IO(inode, rw, iter, offset))
>>>>>>> v3.18
		return 0;

	trace_f2fs_direct_IO_enter(inode, offset, count, rw);

<<<<<<< HEAD
	down_read(&F2FS_I(inode)->dio_rwsem[rw]);
	err = blockdev_direct_IO(rw, iocb, inode, iov, offset, nr_segs,
							get_data_block_dio);
	up_read(&F2FS_I(inode)->dio_rwsem[rw]);
	if (err < 0 && (rw & WRITE)) {
		if (err > 0)
			set_inode_flag(inode, FI_UPDATE_WRITE);
		else if (err < 0)
			f2fs_write_failed(mapping, offset + count);
	}
=======
	err = blockdev_direct_IO(rw, iocb, inode, iter, offset, get_data_block);
	if (err < 0 && (rw & WRITE))
		f2fs_write_failed(mapping, offset + count);
>>>>>>> v3.18

	trace_f2fs_direct_IO_exit(inode, offset, count, rw, err);

	return err;
}

<<<<<<< HEAD
void f2fs_invalidate_page(struct page *page, unsigned long offset)
{
	struct inode *inode = page->mapping->host;
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);

	if (inode->i_ino >= F2FS_ROOT_INO(sbi) && (offset % PAGE_SIZE))
		return;

	if (PageDirty(page)) {
		if (inode->i_ino == F2FS_META_INO(sbi)) {
			dec_page_count(sbi, F2FS_DIRTY_META);
		} else if (inode->i_ino == F2FS_NODE_INO(sbi)) {
			dec_page_count(sbi, F2FS_DIRTY_NODES);
		} else {
			inode_dec_dirty_pages(inode);
			remove_dirty_inode(inode);
		}
	}

	/* This is atomic written page, keep Private */
	if (IS_ATOMIC_WRITTEN_PAGE(page))
		return drop_inmem_page(inode, page);

	set_page_private(page, 0);
	ClearPagePrivate(page);
}

int f2fs_release_page(struct page *page, gfp_t wait)
{
	/* If this is dirty page, keep PagePrivate */
	if (PageDirty(page))
		return 0;

	/* This is atomic written page, keep Private */
	if (IS_ATOMIC_WRITTEN_PAGE(page))
		return 0;

	set_page_private(page, 0);
=======
static void f2fs_invalidate_data_page(struct page *page, unsigned int offset,
				      unsigned int length)
{
	struct inode *inode = page->mapping->host;

	if (offset % PAGE_CACHE_SIZE || length != PAGE_CACHE_SIZE)
		return;

	if (PageDirty(page))
		inode_dec_dirty_pages(inode);
	ClearPagePrivate(page);
}

static int f2fs_release_data_page(struct page *page, gfp_t wait)
{
>>>>>>> v3.18
	ClearPagePrivate(page);
	return 1;
}

<<<<<<< HEAD
/*
 * This was copied from __set_page_dirty_buffers which gives higher performance
 * in very high speed storages. (e.g., pmem)
 */
void f2fs_set_page_dirty_nobuffers(struct page *page)
{
	struct address_space *mapping = page->mapping;
	unsigned long flags;

	if (unlikely(!mapping))
		return;

	spin_lock(&mapping->private_lock);
	SetPageDirty(page);
	spin_unlock(&mapping->private_lock);

	spin_lock_irqsave(&mapping->tree_lock, flags);
	WARN_ON_ONCE(!PageUptodate(page));
	account_page_dirtied(page, mapping);
	radix_tree_tag_set(&mapping->page_tree,
			page_index(page), PAGECACHE_TAG_DIRTY);
	spin_unlock_irqrestore(&mapping->tree_lock, flags);

	__mark_inode_dirty(mapping->host, I_DIRTY_PAGES);
	return;
}

=======
>>>>>>> v3.18
static int f2fs_set_data_page_dirty(struct page *page)
{
	struct address_space *mapping = page->mapping;
	struct inode *inode = mapping->host;

	trace_f2fs_set_page_dirty(page, DATA);

<<<<<<< HEAD
	if (!PageUptodate(page))
		SetPageUptodate(page);

	if (f2fs_is_atomic_file(inode) && !f2fs_is_commit_atomic_write(inode)) {
		if (!IS_ATOMIC_WRITTEN_PAGE(page)) {
			register_inmem_page(inode, page);
			return 1;
		}
		/*
		 * Previously, this page has been registered, we just
		 * return here.
		 */
		return 0;
	}

	if (!PageDirty(page)) {
		f2fs_set_page_dirty_nobuffers(page);
=======
	SetPageUptodate(page);
	mark_inode_dirty(inode);

	if (!PageDirty(page)) {
		__set_page_dirty_nobuffers(page);
>>>>>>> v3.18
		update_dirty_page(inode, page);
		return 1;
	}
	return 0;
}

static sector_t f2fs_bmap(struct address_space *mapping, sector_t block)
{
	struct inode *inode = mapping->host;

	if (f2fs_has_inline_data(inode))
		return 0;

<<<<<<< HEAD
	/* make sure allocating whole blocks */
	if (mapping_tagged(mapping, PAGECACHE_TAG_DIRTY))
		filemap_write_and_wait(mapping);

	return generic_block_bmap(mapping, block, get_data_block_bmap);
}

#ifdef CONFIG_MIGRATION
#include <linux/migrate.h>

int f2fs_migrate_page(struct address_space *mapping,
		struct page *newpage, struct page *page, enum migrate_mode mode)
{
	int rc;
	struct f2fs_inode_info *fi = F2FS_I(mapping->host);
	bool atomic_written = IS_ATOMIC_WRITTEN_PAGE(page);

	BUG_ON(PageWriteback(page));

	/* migrating an atomic written page is safe with the inmem_lock hold */
	if (atomic_written) {
		if (mode != MIGRATE_SYNC)
			return -EBUSY;
		if (!mutex_trylock(&fi->inmem_lock))
			return -EAGAIN;
	}

	rc = migrate_page_move_mapping(mapping, newpage,
				page, NULL, mode);
	if (rc != MIGRATEPAGE_SUCCESS) {
		if (atomic_written)
			mutex_unlock(&fi->inmem_lock);
		return rc;
	}

	if (atomic_written) {
		struct inmem_pages *cur;
		list_for_each_entry(cur, &fi->inmem_pages, list)
			if (cur->page == page) {
				cur->page = newpage;
				break;
			}
		mutex_unlock(&fi->inmem_lock);
		put_page(page);
		get_page(newpage);
	}

	if (PagePrivate(page))
		SetPagePrivate(newpage);
	set_page_private(newpage, page_private(page));

	migrate_page_copy(newpage, page);

	return MIGRATEPAGE_SUCCESS;
}
#endif
=======
	return generic_block_bmap(mapping, block, get_data_block);
}
>>>>>>> v3.18

const struct address_space_operations f2fs_dblock_aops = {
	.readpage	= f2fs_read_data_page,
	.readpages	= f2fs_read_data_pages,
	.writepage	= f2fs_write_data_page,
	.writepages	= f2fs_write_data_pages,
	.write_begin	= f2fs_write_begin,
	.write_end	= f2fs_write_end,
	.set_page_dirty	= f2fs_set_data_page_dirty,
<<<<<<< HEAD
	.invalidatepage	= f2fs_invalidate_page,
	.releasepage	= f2fs_release_page,
	.direct_IO	= f2fs_direct_IO,
	.bmap		= f2fs_bmap,
#ifdef CONFIG_MIGRATION
	.migratepage    = f2fs_migrate_page,
#endif
=======
	.invalidatepage	= f2fs_invalidate_data_page,
	.releasepage	= f2fs_release_data_page,
	.direct_IO	= f2fs_direct_IO,
	.bmap		= f2fs_bmap,
>>>>>>> v3.18
};
