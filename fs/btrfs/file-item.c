/*
 * Copyright (C) 2007 Oracle.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
 */

#include <linux/bio.h>
#include <linux/slab.h>
#include <linux/pagemap.h>
#include <linux/highmem.h>
#include "ctree.h"
#include "disk-io.h"
#include "transaction.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "volumes.h"
>>>>>>> v3.18
=======
#include "volumes.h"
>>>>>>> v3.18
#include "print-tree.h"

#define __MAX_CSUM_ITEMS(r, size) ((unsigned long)(((BTRFS_LEAF_DATA_SIZE(r) - \
				   sizeof(struct btrfs_item) * 2) / \
				  size) - 1))

#define MAX_CSUM_ITEMS(r, size) (min_t(u32, __MAX_CSUM_ITEMS(r, size), \
				       PAGE_CACHE_SIZE))

#define MAX_ORDERED_SUM_BYTES(r) ((PAGE_SIZE - \
				   sizeof(struct btrfs_ordered_sum)) / \
<<<<<<< HEAD
<<<<<<< HEAD
				   sizeof(struct btrfs_sector_sum) * \
				   (r)->sectorsize - (r)->sectorsize)
=======
				   sizeof(u32) * (r)->sectorsize)
>>>>>>> v3.18
=======
				   sizeof(u32) * (r)->sectorsize)
>>>>>>> v3.18

int btrfs_insert_file_extent(struct btrfs_trans_handle *trans,
			     struct btrfs_root *root,
			     u64 objectid, u64 pos,
			     u64 disk_offset, u64 disk_num_bytes,
			     u64 num_bytes, u64 offset, u64 ram_bytes,
			     u8 compression, u8 encryption, u16 other_encoding)
{
	int ret = 0;
	struct btrfs_file_extent_item *item;
	struct btrfs_key file_key;
	struct btrfs_path *path;
	struct extent_buffer *leaf;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;
	file_key.objectid = objectid;
	file_key.offset = pos;
<<<<<<< HEAD
<<<<<<< HEAD
	btrfs_set_key_type(&file_key, BTRFS_EXTENT_DATA_KEY);
=======
	file_key.type = BTRFS_EXTENT_DATA_KEY;
>>>>>>> v3.18
=======
	file_key.type = BTRFS_EXTENT_DATA_KEY;
>>>>>>> v3.18

	path->leave_spinning = 1;
	ret = btrfs_insert_empty_item(trans, root, path, &file_key,
				      sizeof(*item));
	if (ret < 0)
		goto out;
	BUG_ON(ret); /* Can't happen */
	leaf = path->nodes[0];
	item = btrfs_item_ptr(leaf, path->slots[0],
			      struct btrfs_file_extent_item);
	btrfs_set_file_extent_disk_bytenr(leaf, item, disk_offset);
	btrfs_set_file_extent_disk_num_bytes(leaf, item, disk_num_bytes);
	btrfs_set_file_extent_offset(leaf, item, offset);
	btrfs_set_file_extent_num_bytes(leaf, item, num_bytes);
	btrfs_set_file_extent_ram_bytes(leaf, item, ram_bytes);
	btrfs_set_file_extent_generation(leaf, item, trans->transid);
	btrfs_set_file_extent_type(leaf, item, BTRFS_FILE_EXTENT_REG);
	btrfs_set_file_extent_compression(leaf, item, compression);
	btrfs_set_file_extent_encryption(leaf, item, encryption);
	btrfs_set_file_extent_other_encoding(leaf, item, other_encoding);

	btrfs_mark_buffer_dirty(leaf);
out:
	btrfs_free_path(path);
	return ret;
}

static struct btrfs_csum_item *
btrfs_lookup_csum(struct btrfs_trans_handle *trans,
		  struct btrfs_root *root,
		  struct btrfs_path *path,
		  u64 bytenr, int cow)
{
	int ret;
	struct btrfs_key file_key;
	struct btrfs_key found_key;
	struct btrfs_csum_item *item;
	struct extent_buffer *leaf;
	u64 csum_offset = 0;
	u16 csum_size = btrfs_super_csum_size(root->fs_info->super_copy);
	int csums_in_item;

	file_key.objectid = BTRFS_EXTENT_CSUM_OBJECTID;
	file_key.offset = bytenr;
<<<<<<< HEAD
<<<<<<< HEAD
	btrfs_set_key_type(&file_key, BTRFS_EXTENT_CSUM_KEY);
=======
	file_key.type = BTRFS_EXTENT_CSUM_KEY;
>>>>>>> v3.18
=======
	file_key.type = BTRFS_EXTENT_CSUM_KEY;
>>>>>>> v3.18
	ret = btrfs_search_slot(trans, root, &file_key, path, 0, cow);
	if (ret < 0)
		goto fail;
	leaf = path->nodes[0];
	if (ret > 0) {
		ret = 1;
		if (path->slots[0] == 0)
			goto fail;
		path->slots[0]--;
		btrfs_item_key_to_cpu(leaf, &found_key, path->slots[0]);
<<<<<<< HEAD
<<<<<<< HEAD
		if (btrfs_key_type(&found_key) != BTRFS_EXTENT_CSUM_KEY)
=======
		if (found_key.type != BTRFS_EXTENT_CSUM_KEY)
>>>>>>> v3.18
=======
		if (found_key.type != BTRFS_EXTENT_CSUM_KEY)
>>>>>>> v3.18
			goto fail;

		csum_offset = (bytenr - found_key.offset) >>
				root->fs_info->sb->s_blocksize_bits;
		csums_in_item = btrfs_item_size_nr(leaf, path->slots[0]);
		csums_in_item /= csum_size;

		if (csum_offset == csums_in_item) {
			ret = -EFBIG;
			goto fail;
		} else if (csum_offset > csums_in_item) {
			goto fail;
		}
	}
	item = btrfs_item_ptr(leaf, path->slots[0], struct btrfs_csum_item);
	item = (struct btrfs_csum_item *)((unsigned char *)item +
					  csum_offset * csum_size);
	return item;
fail:
	if (ret > 0)
		ret = -ENOENT;
	return ERR_PTR(ret);
}

int btrfs_lookup_file_extent(struct btrfs_trans_handle *trans,
			     struct btrfs_root *root,
			     struct btrfs_path *path, u64 objectid,
			     u64 offset, int mod)
{
	int ret;
	struct btrfs_key file_key;
	int ins_len = mod < 0 ? -1 : 0;
	int cow = mod != 0;

	file_key.objectid = objectid;
	file_key.offset = offset;
<<<<<<< HEAD
<<<<<<< HEAD
	btrfs_set_key_type(&file_key, BTRFS_EXTENT_DATA_KEY);
=======
	file_key.type = BTRFS_EXTENT_DATA_KEY;
>>>>>>> v3.18
=======
	file_key.type = BTRFS_EXTENT_DATA_KEY;
>>>>>>> v3.18
	ret = btrfs_search_slot(trans, root, &file_key, path, ins_len, cow);
	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
static void btrfs_io_bio_endio_readpage(struct btrfs_io_bio *bio, int err)
{
	kfree(bio->csum_allocated);
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static int __btrfs_lookup_bio_sums(struct btrfs_root *root,
				   struct inode *inode, struct bio *bio,
				   u64 logical_offset, u32 *dst, int dio)
{
<<<<<<< HEAD
<<<<<<< HEAD
	u32 sum[16];
	int len;
	struct bio_vec *bvec = bio->bi_io_vec;
	int bio_index = 0;
=======
=======
>>>>>>> v3.18
	struct bio_vec *bvec = bio->bi_io_vec;
	struct btrfs_io_bio *btrfs_bio = btrfs_io_bio(bio);
	struct btrfs_csum_item *item = NULL;
	struct extent_io_tree *io_tree = &BTRFS_I(inode)->io_tree;
	struct btrfs_path *path;
	u8 *csum;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	u64 offset = 0;
	u64 item_start_offset = 0;
	u64 item_last_offset = 0;
	u64 disk_bytenr;
	u32 diff;
<<<<<<< HEAD
<<<<<<< HEAD
	u16 csum_size = btrfs_super_csum_size(root->fs_info->super_copy);
	int count;
	struct btrfs_path *path;
	struct btrfs_csum_item *item = NULL;
	struct extent_io_tree *io_tree = &BTRFS_I(inode)->io_tree;
=======
=======
>>>>>>> v3.18
	int nblocks;
	int bio_index = 0;
	int count;
	u16 csum_size = btrfs_super_csum_size(root->fs_info->super_copy);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;
<<<<<<< HEAD
<<<<<<< HEAD
	if (bio->bi_size > PAGE_CACHE_SIZE * 8)
=======
=======
>>>>>>> v3.18

	nblocks = bio->bi_iter.bi_size >> inode->i_sb->s_blocksize_bits;
	if (!dst) {
		if (nblocks * csum_size > BTRFS_BIO_INLINE_CSUM_SIZE) {
			btrfs_bio->csum_allocated = kmalloc(nblocks * csum_size,
							    GFP_NOFS);
			if (!btrfs_bio->csum_allocated) {
				btrfs_free_path(path);
				return -ENOMEM;
			}
			btrfs_bio->csum = btrfs_bio->csum_allocated;
			btrfs_bio->end_io = btrfs_io_bio_endio_readpage;
		} else {
			btrfs_bio->csum = btrfs_bio->csum_inline;
		}
		csum = btrfs_bio->csum;
	} else {
		csum = (u8 *)dst;
	}

	if (bio->bi_iter.bi_size > PAGE_CACHE_SIZE * 8)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		path->reada = 2;

	WARN_ON(bio->bi_vcnt <= 0);

	/*
	 * the free space stuff is only read when it hasn't been
	 * updated in the current transaction.  So, we can safely
	 * read from the commit root and sidestep a nasty deadlock
	 * between reading the free space cache and updating the csum tree.
	 */
	if (btrfs_is_free_space_inode(inode)) {
		path->search_commit_root = 1;
		path->skip_locking = 1;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	disk_bytenr = (u64)bio->bi_sector << 9;
	if (dio)
		offset = logical_offset;
	while (bio_index < bio->bi_vcnt) {
		len = min_t(int, ARRAY_SIZE(sum), bio->bi_vcnt - bio_index);
		if (!dio)
			offset = page_offset(bvec->bv_page) + bvec->bv_offset;
		count = btrfs_find_ordered_sum(inode, offset, disk_bytenr, sum,
					       len);
=======
=======
>>>>>>> v3.18
	disk_bytenr = (u64)bio->bi_iter.bi_sector << 9;
	if (dio)
		offset = logical_offset;
	while (bio_index < bio->bi_vcnt) {
		if (!dio)
			offset = page_offset(bvec->bv_page) + bvec->bv_offset;
		count = btrfs_find_ordered_sum(inode, offset, disk_bytenr,
					       (u32 *)csum, nblocks);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		if (count)
			goto found;

		if (!item || disk_bytenr < item_start_offset ||
		    disk_bytenr >= item_last_offset) {
			struct btrfs_key found_key;
			u32 item_size;

			if (item)
				btrfs_release_path(path);
			item = btrfs_lookup_csum(NULL, root->fs_info->csum_root,
						 path, disk_bytenr, 0);
			if (IS_ERR(item)) {
				count = 1;
<<<<<<< HEAD
<<<<<<< HEAD
				sum[0] = 0;
=======
				memset(csum, 0, csum_size);
>>>>>>> v3.18
=======
				memset(csum, 0, csum_size);
>>>>>>> v3.18
				if (BTRFS_I(inode)->root->root_key.objectid ==
				    BTRFS_DATA_RELOC_TREE_OBJECTID) {
					set_extent_bits(io_tree, offset,
						offset + bvec->bv_len - 1,
						EXTENT_NODATASUM, GFP_NOFS);
				} else {
<<<<<<< HEAD
<<<<<<< HEAD
					printk(KERN_INFO "btrfs no csum found "
					       "for inode %llu start %llu\n",
					       (unsigned long long)
					       btrfs_ino(inode),
					       (unsigned long long)offset);
=======
					btrfs_info(BTRFS_I(inode)->root->fs_info,
						   "no csum found for inode %llu start %llu",
					       btrfs_ino(inode), offset);
>>>>>>> v3.18
=======
					btrfs_info(BTRFS_I(inode)->root->fs_info,
						   "no csum found for inode %llu start %llu",
					       btrfs_ino(inode), offset);
>>>>>>> v3.18
				}
				item = NULL;
				btrfs_release_path(path);
				goto found;
			}
			btrfs_item_key_to_cpu(path->nodes[0], &found_key,
					      path->slots[0]);

			item_start_offset = found_key.offset;
			item_size = btrfs_item_size_nr(path->nodes[0],
						       path->slots[0]);
			item_last_offset = item_start_offset +
				(item_size / csum_size) *
				root->sectorsize;
			item = btrfs_item_ptr(path->nodes[0], path->slots[0],
					      struct btrfs_csum_item);
		}
		/*
		 * this byte range must be able to fit inside
		 * a single leaf so it will also fit inside a u32
		 */
		diff = disk_bytenr - item_start_offset;
		diff = diff / root->sectorsize;
		diff = diff * csum_size;
<<<<<<< HEAD
<<<<<<< HEAD
		count = min_t(int, len, (item_last_offset - disk_bytenr) >>
					inode->i_sb->s_blocksize_bits);
		read_extent_buffer(path->nodes[0], sum,
				   ((unsigned long)item) + diff,
				   csum_size * count);
found:
		if (dst) {
			memcpy(dst, sum, count * csum_size);
			dst += count;
		} else {
			if (dio)
				extent_cache_csums_dio(io_tree, offset, sum,
						       count);
			else
				extent_cache_csums(io_tree, bio, bio_index, sum,
					    count);
		}
		while (count--) {
			disk_bytenr += bvec->bv_len;
			offset += bvec->bv_len;
			bio_index++;
=======
=======
>>>>>>> v3.18
		count = min_t(int, nblocks, (item_last_offset - disk_bytenr) >>
					    inode->i_sb->s_blocksize_bits);
		read_extent_buffer(path->nodes[0], csum,
				   ((unsigned long)item) + diff,
				   csum_size * count);
found:
		csum += count * csum_size;
		nblocks -= count;
		bio_index += count;
		while (count--) {
			disk_bytenr += bvec->bv_len;
			offset += bvec->bv_len;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			bvec++;
		}
	}
	btrfs_free_path(path);
	return 0;
}

int btrfs_lookup_bio_sums(struct btrfs_root *root, struct inode *inode,
			  struct bio *bio, u32 *dst)
{
	return __btrfs_lookup_bio_sums(root, inode, bio, 0, dst, 0);
}

int btrfs_lookup_bio_sums_dio(struct btrfs_root *root, struct inode *inode,
			      struct bio *bio, u64 offset)
{
	return __btrfs_lookup_bio_sums(root, inode, bio, offset, NULL, 1);
}

int btrfs_lookup_csums_range(struct btrfs_root *root, u64 start, u64 end,
			     struct list_head *list, int search_commit)
{
	struct btrfs_key key;
	struct btrfs_path *path;
	struct extent_buffer *leaf;
	struct btrfs_ordered_sum *sums;
<<<<<<< HEAD
<<<<<<< HEAD
	struct btrfs_sector_sum *sector_sum;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	struct btrfs_csum_item *item;
	LIST_HEAD(tmplist);
	unsigned long offset;
	int ret;
	size_t size;
	u64 csum_end;
	u16 csum_size = btrfs_super_csum_size(root->fs_info->super_copy);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	ASSERT(IS_ALIGNED(start, root->sectorsize) &&
	       IS_ALIGNED(end + 1, root->sectorsize));

>>>>>>> v3.18
=======
	ASSERT(IS_ALIGNED(start, root->sectorsize) &&
	       IS_ALIGNED(end + 1, root->sectorsize));

>>>>>>> v3.18
	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	if (search_commit) {
		path->skip_locking = 1;
		path->reada = 2;
		path->search_commit_root = 1;
	}

	key.objectid = BTRFS_EXTENT_CSUM_OBJECTID;
	key.offset = start;
	key.type = BTRFS_EXTENT_CSUM_KEY;

	ret = btrfs_search_slot(NULL, root, &key, path, 0, 0);
	if (ret < 0)
		goto fail;
	if (ret > 0 && path->slots[0] > 0) {
		leaf = path->nodes[0];
		btrfs_item_key_to_cpu(leaf, &key, path->slots[0] - 1);
		if (key.objectid == BTRFS_EXTENT_CSUM_OBJECTID &&
		    key.type == BTRFS_EXTENT_CSUM_KEY) {
			offset = (start - key.offset) >>
				 root->fs_info->sb->s_blocksize_bits;
			if (offset * csum_size <
			    btrfs_item_size_nr(leaf, path->slots[0] - 1))
				path->slots[0]--;
		}
	}

	while (start <= end) {
		leaf = path->nodes[0];
		if (path->slots[0] >= btrfs_header_nritems(leaf)) {
			ret = btrfs_next_leaf(root, path);
			if (ret < 0)
				goto fail;
			if (ret > 0)
				break;
			leaf = path->nodes[0];
		}

		btrfs_item_key_to_cpu(leaf, &key, path->slots[0]);
		if (key.objectid != BTRFS_EXTENT_CSUM_OBJECTID ||
		    key.type != BTRFS_EXTENT_CSUM_KEY ||
		    key.offset > end)
			break;

		if (key.offset > start)
			start = key.offset;

		size = btrfs_item_size_nr(leaf, path->slots[0]);
		csum_end = key.offset + (size / csum_size) * root->sectorsize;
		if (csum_end <= start) {
			path->slots[0]++;
			continue;
		}

		csum_end = min(csum_end, end + 1);
		item = btrfs_item_ptr(path->nodes[0], path->slots[0],
				      struct btrfs_csum_item);
		while (start < csum_end) {
			size = min_t(size_t, csum_end - start,
<<<<<<< HEAD
<<<<<<< HEAD
					MAX_ORDERED_SUM_BYTES(root));
			sums = kzalloc(btrfs_ordered_sum_size(root, size),
					GFP_NOFS);
=======
				     MAX_ORDERED_SUM_BYTES(root));
			sums = kzalloc(btrfs_ordered_sum_size(root, size),
				       GFP_NOFS);
>>>>>>> v3.18
=======
				     MAX_ORDERED_SUM_BYTES(root));
			sums = kzalloc(btrfs_ordered_sum_size(root, size),
				       GFP_NOFS);
>>>>>>> v3.18
			if (!sums) {
				ret = -ENOMEM;
				goto fail;
			}

<<<<<<< HEAD
<<<<<<< HEAD
			sector_sum = sums->sums;
			sums->bytenr = start;
			sums->len = size;
=======
			sums->bytenr = start;
			sums->len = (int)size;
>>>>>>> v3.18
=======
			sums->bytenr = start;
			sums->len = (int)size;
>>>>>>> v3.18

			offset = (start - key.offset) >>
				root->fs_info->sb->s_blocksize_bits;
			offset *= csum_size;
<<<<<<< HEAD
<<<<<<< HEAD

			while (size > 0) {
				read_extent_buffer(path->nodes[0],
						&sector_sum->sum,
						((unsigned long)item) +
						offset, csum_size);
				sector_sum->bytenr = start;

				size -= root->sectorsize;
				start += root->sectorsize;
				offset += csum_size;
				sector_sum++;
			}
=======
=======
>>>>>>> v3.18
			size >>= root->fs_info->sb->s_blocksize_bits;

			read_extent_buffer(path->nodes[0],
					   sums->sums,
					   ((unsigned long)item) + offset,
					   csum_size * size);

			start += root->sectorsize * size;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			list_add_tail(&sums->list, &tmplist);
		}
		path->slots[0]++;
	}
	ret = 0;
fail:
	while (ret < 0 && !list_empty(&tmplist)) {
		sums = list_entry(tmplist.next, struct btrfs_ordered_sum, list);
		list_del(&sums->list);
		kfree(sums);
	}
	list_splice_tail(&tmplist, list);

	btrfs_free_path(path);
	return ret;
}

int btrfs_csum_one_bio(struct btrfs_root *root, struct inode *inode,
		       struct bio *bio, u64 file_start, int contig)
{
	struct btrfs_ordered_sum *sums;
<<<<<<< HEAD
<<<<<<< HEAD
	struct btrfs_sector_sum *sector_sum;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	struct btrfs_ordered_extent *ordered;
	char *data;
	struct bio_vec *bvec = bio->bi_io_vec;
	int bio_index = 0;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long total_bytes = 0;
	unsigned long this_sum_bytes = 0;
	u64 offset;
	u64 disk_bytenr;

	WARN_ON(bio->bi_vcnt <= 0);
	sums = kzalloc(btrfs_ordered_sum_size(root, bio->bi_size), GFP_NOFS);
	if (!sums)
		return -ENOMEM;

	sector_sum = sums->sums;
	disk_bytenr = (u64)bio->bi_sector << 9;
	sums->len = bio->bi_size;
=======
=======
>>>>>>> v3.18
	int index;
	unsigned long total_bytes = 0;
	unsigned long this_sum_bytes = 0;
	u64 offset;

	WARN_ON(bio->bi_vcnt <= 0);
	sums = kzalloc(btrfs_ordered_sum_size(root, bio->bi_iter.bi_size),
		       GFP_NOFS);
	if (!sums)
		return -ENOMEM;

	sums->len = bio->bi_iter.bi_size;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	INIT_LIST_HEAD(&sums->list);

	if (contig)
		offset = file_start;
	else
		offset = page_offset(bvec->bv_page) + bvec->bv_offset;

	ordered = btrfs_lookup_ordered_extent(inode, offset);
	BUG_ON(!ordered); /* Logic error */
<<<<<<< HEAD
<<<<<<< HEAD
	sums->bytenr = ordered->start;
=======
	sums->bytenr = (u64)bio->bi_iter.bi_sector << 9;
	index = 0;
>>>>>>> v3.18
=======
	sums->bytenr = (u64)bio->bi_iter.bi_sector << 9;
	index = 0;
>>>>>>> v3.18

	while (bio_index < bio->bi_vcnt) {
		if (!contig)
			offset = page_offset(bvec->bv_page) + bvec->bv_offset;

		if (offset >= ordered->file_offset + ordered->len ||
		    offset < ordered->file_offset) {
			unsigned long bytes_left;
			sums->len = this_sum_bytes;
			this_sum_bytes = 0;
			btrfs_add_ordered_sum(inode, ordered, sums);
			btrfs_put_ordered_extent(ordered);

<<<<<<< HEAD
<<<<<<< HEAD
			bytes_left = bio->bi_size - total_bytes;
=======
			bytes_left = bio->bi_iter.bi_size - total_bytes;
>>>>>>> v3.18
=======
			bytes_left = bio->bi_iter.bi_size - total_bytes;
>>>>>>> v3.18

			sums = kzalloc(btrfs_ordered_sum_size(root, bytes_left),
				       GFP_NOFS);
			BUG_ON(!sums); /* -ENOMEM */
<<<<<<< HEAD
<<<<<<< HEAD
			sector_sum = sums->sums;
			sums->len = bytes_left;
			ordered = btrfs_lookup_ordered_extent(inode, offset);
			BUG_ON(!ordered); /* Logic error */
			sums->bytenr = ordered->start;
		}

		data = kmap_atomic(bvec->bv_page);
		sector_sum->sum = ~(u32)0;
		sector_sum->sum = btrfs_csum_data(data + bvec->bv_offset,
						  sector_sum->sum,
						  bvec->bv_len);
		kunmap_atomic(data);
		btrfs_csum_final(sector_sum->sum,
				 (char *)&sector_sum->sum);
		sector_sum->bytenr = disk_bytenr;

		sector_sum++;
		bio_index++;
		total_bytes += bvec->bv_len;
		this_sum_bytes += bvec->bv_len;
		disk_bytenr += bvec->bv_len;
=======
=======
>>>>>>> v3.18
			sums->len = bytes_left;
			ordered = btrfs_lookup_ordered_extent(inode, offset);
			BUG_ON(!ordered); /* Logic error */
			sums->bytenr = ((u64)bio->bi_iter.bi_sector << 9) +
				       total_bytes;
			index = 0;
		}

		data = kmap_atomic(bvec->bv_page);
		sums->sums[index] = ~(u32)0;
		sums->sums[index] = btrfs_csum_data(data + bvec->bv_offset,
						    sums->sums[index],
						    bvec->bv_len);
		kunmap_atomic(data);
		btrfs_csum_final(sums->sums[index],
				 (char *)(sums->sums + index));

		bio_index++;
		index++;
		total_bytes += bvec->bv_len;
		this_sum_bytes += bvec->bv_len;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		offset += bvec->bv_len;
		bvec++;
	}
	this_sum_bytes = 0;
	btrfs_add_ordered_sum(inode, ordered, sums);
	btrfs_put_ordered_extent(ordered);
	return 0;
}

/*
 * helper function for csum removal, this expects the
 * key to describe the csum pointed to by the path, and it expects
 * the csum to overlap the range [bytenr, len]
 *
 * The csum should not be entirely contained in the range and the
 * range should not be entirely contained in the csum.
 *
 * This calls btrfs_truncate_item with the correct args based on the
 * overlap, and fixes up the key as required.
 */
static noinline void truncate_one_csum(struct btrfs_root *root,
				       struct btrfs_path *path,
				       struct btrfs_key *key,
				       u64 bytenr, u64 len)
{
	struct extent_buffer *leaf;
	u16 csum_size = btrfs_super_csum_size(root->fs_info->super_copy);
	u64 csum_end;
	u64 end_byte = bytenr + len;
	u32 blocksize_bits = root->fs_info->sb->s_blocksize_bits;

	leaf = path->nodes[0];
	csum_end = btrfs_item_size_nr(leaf, path->slots[0]) / csum_size;
	csum_end <<= root->fs_info->sb->s_blocksize_bits;
	csum_end += key->offset;

	if (key->offset < bytenr && csum_end <= end_byte) {
		/*
		 *         [ bytenr - len ]
		 *         [   ]
		 *   [csum     ]
		 *   A simple truncate off the end of the item
		 */
		u32 new_size = (bytenr - key->offset) >> blocksize_bits;
		new_size *= csum_size;
		btrfs_truncate_item(root, path, new_size, 1);
	} else if (key->offset >= bytenr && csum_end > end_byte &&
		   end_byte > key->offset) {
		/*
		 *         [ bytenr - len ]
		 *                 [ ]
		 *                 [csum     ]
		 * we need to truncate from the beginning of the csum
		 */
		u32 new_size = (csum_end - end_byte) >> blocksize_bits;
		new_size *= csum_size;

		btrfs_truncate_item(root, path, new_size, 0);

		key->offset = end_byte;
		btrfs_set_item_key_safe(root, path, key);
	} else {
		BUG();
	}
}

/*
 * deletes the csum items from the csum tree for a given
 * range of bytes.
 */
int btrfs_del_csums(struct btrfs_trans_handle *trans,
		    struct btrfs_root *root, u64 bytenr, u64 len)
{
	struct btrfs_path *path;
	struct btrfs_key key;
	u64 end_byte = bytenr + len;
	u64 csum_end;
	struct extent_buffer *leaf;
	int ret;
	u16 csum_size = btrfs_super_csum_size(root->fs_info->super_copy);
	int blocksize_bits = root->fs_info->sb->s_blocksize_bits;

	root = root->fs_info->csum_root;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	while (1) {
		key.objectid = BTRFS_EXTENT_CSUM_OBJECTID;
		key.offset = end_byte - 1;
		key.type = BTRFS_EXTENT_CSUM_KEY;

		path->leave_spinning = 1;
		ret = btrfs_search_slot(trans, root, &key, path, -1, 1);
		if (ret > 0) {
			if (path->slots[0] == 0)
				break;
			path->slots[0]--;
		} else if (ret < 0) {
			break;
		}

		leaf = path->nodes[0];
		btrfs_item_key_to_cpu(leaf, &key, path->slots[0]);

		if (key.objectid != BTRFS_EXTENT_CSUM_OBJECTID ||
		    key.type != BTRFS_EXTENT_CSUM_KEY) {
			break;
		}

		if (key.offset >= end_byte)
			break;

		csum_end = btrfs_item_size_nr(leaf, path->slots[0]) / csum_size;
		csum_end <<= blocksize_bits;
		csum_end += key.offset;

		/* this csum ends before we start, we're done */
		if (csum_end <= bytenr)
			break;

		/* delete the entire item, it is inside our range */
		if (key.offset >= bytenr && csum_end <= end_byte) {
			ret = btrfs_del_item(trans, root, path);
			if (ret)
				goto out;
			if (key.offset == bytenr)
				break;
		} else if (key.offset < bytenr && csum_end > end_byte) {
			unsigned long offset;
			unsigned long shift_len;
			unsigned long item_offset;
			/*
			 *        [ bytenr - len ]
			 *     [csum                ]
			 *
			 * Our bytes are in the middle of the csum,
			 * we need to split this item and insert a new one.
			 *
			 * But we can't drop the path because the
			 * csum could change, get removed, extended etc.
			 *
			 * The trick here is the max size of a csum item leaves
			 * enough room in the tree block for a single
			 * item header.  So, we split the item in place,
			 * adding a new header pointing to the existing
			 * bytes.  Then we loop around again and we have
			 * a nicely formed csum item that we can neatly
			 * truncate.
			 */
			offset = (bytenr - key.offset) >> blocksize_bits;
			offset *= csum_size;

			shift_len = (len >> blocksize_bits) * csum_size;

			item_offset = btrfs_item_ptr_offset(leaf,
							    path->slots[0]);

			memset_extent_buffer(leaf, 0, item_offset + offset,
					     shift_len);
			key.offset = bytenr;

			/*
			 * btrfs_split_item returns -EAGAIN when the
			 * item changed size or key
			 */
			ret = btrfs_split_item(trans, root, path, &key, offset);
			if (ret && ret != -EAGAIN) {
				btrfs_abort_transaction(trans, root, ret);
				goto out;
			}

			key.offset = end_byte - 1;
		} else {
			truncate_one_csum(root, path, &key, bytenr, len);
			if (key.offset < bytenr)
				break;
		}
		btrfs_release_path(path);
	}
	ret = 0;
out:
	btrfs_free_path(path);
	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static u64 btrfs_sector_sum_left(struct btrfs_ordered_sum *sums,
				 struct btrfs_sector_sum *sector_sum,
				 u64 total_bytes, u64 sectorsize)
{
	u64 tmp = sectorsize;
	u64 next_sector = sector_sum->bytenr;
	struct btrfs_sector_sum *next = sector_sum + 1;

	while ((tmp + total_bytes) < sums->len) {
		if (next_sector + sectorsize != next->bytenr)
			break;
		tmp += sectorsize;
		next_sector = next->bytenr;
		next++;
	}
	return tmp;
}

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
int btrfs_csum_file_blocks(struct btrfs_trans_handle *trans,
			   struct btrfs_root *root,
			   struct btrfs_ordered_sum *sums)
{
<<<<<<< HEAD
<<<<<<< HEAD
	u64 bytenr;
	int ret;
	struct btrfs_key file_key;
	struct btrfs_key found_key;
	u64 next_offset;
	u64 total_bytes = 0;
	int found_next;
=======
	struct btrfs_key file_key;
	struct btrfs_key found_key;
>>>>>>> v3.18
=======
	struct btrfs_key file_key;
	struct btrfs_key found_key;
>>>>>>> v3.18
	struct btrfs_path *path;
	struct btrfs_csum_item *item;
	struct btrfs_csum_item *item_end;
	struct extent_buffer *leaf = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
	u64 csum_offset;
	struct btrfs_sector_sum *sector_sum;
	u32 nritems;
	u32 ins_size;
=======
=======
>>>>>>> v3.18
	u64 next_offset;
	u64 total_bytes = 0;
	u64 csum_offset;
	u64 bytenr;
	u32 nritems;
	u32 ins_size;
	int index = 0;
	int found_next;
	int ret;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	u16 csum_size = btrfs_super_csum_size(root->fs_info->super_copy);

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;
<<<<<<< HEAD
<<<<<<< HEAD

	sector_sum = sums->sums;
again:
	next_offset = (u64)-1;
	found_next = 0;
	file_key.objectid = BTRFS_EXTENT_CSUM_OBJECTID;
	file_key.offset = sector_sum->bytenr;
	bytenr = sector_sum->bytenr;
	btrfs_set_key_type(&file_key, BTRFS_EXTENT_CSUM_KEY);

	item = btrfs_lookup_csum(trans, root, path, sector_sum->bytenr, 1);
	if (!IS_ERR(item)) {
		leaf = path->nodes[0];
		ret = 0;
=======
=======
>>>>>>> v3.18
again:
	next_offset = (u64)-1;
	found_next = 0;
	bytenr = sums->bytenr + total_bytes;
	file_key.objectid = BTRFS_EXTENT_CSUM_OBJECTID;
	file_key.offset = bytenr;
	file_key.type = BTRFS_EXTENT_CSUM_KEY;

	item = btrfs_lookup_csum(trans, root, path, bytenr, 1);
	if (!IS_ERR(item)) {
		ret = 0;
		leaf = path->nodes[0];
		item_end = btrfs_item_ptr(leaf, path->slots[0],
					  struct btrfs_csum_item);
		item_end = (struct btrfs_csum_item *)((char *)item_end +
			   btrfs_item_size_nr(leaf, path->slots[0]));
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		goto found;
	}
	ret = PTR_ERR(item);
	if (ret != -EFBIG && ret != -ENOENT)
		goto fail_unlock;

	if (ret == -EFBIG) {
		u32 item_size;
		/* we found one, but it isn't big enough yet */
		leaf = path->nodes[0];
		item_size = btrfs_item_size_nr(leaf, path->slots[0]);
		if ((item_size / csum_size) >=
		    MAX_CSUM_ITEMS(root, csum_size)) {
			/* already at max size, make a new one */
			goto insert;
		}
	} else {
		int slot = path->slots[0] + 1;
		/* we didn't find a csum item, insert one */
		nritems = btrfs_header_nritems(path->nodes[0]);
<<<<<<< HEAD
<<<<<<< HEAD
		if (path->slots[0] >= nritems - 1) {
=======
		if (!nritems || (path->slots[0] >= nritems - 1)) {
>>>>>>> v3.18
=======
		if (!nritems || (path->slots[0] >= nritems - 1)) {
>>>>>>> v3.18
			ret = btrfs_next_leaf(root, path);
			if (ret == 1)
				found_next = 1;
			if (ret != 0)
				goto insert;
			slot = path->slots[0];
		}
		btrfs_item_key_to_cpu(path->nodes[0], &found_key, slot);
		if (found_key.objectid != BTRFS_EXTENT_CSUM_OBJECTID ||
		    found_key.type != BTRFS_EXTENT_CSUM_KEY) {
			found_next = 1;
			goto insert;
		}
		next_offset = found_key.offset;
		found_next = 1;
		goto insert;
	}

	/*
	 * at this point, we know the tree has an item, but it isn't big
	 * enough yet to put our csum in.  Grow it
	 */
	btrfs_release_path(path);
	ret = btrfs_search_slot(trans, root, &file_key, path,
				csum_size, 1);
	if (ret < 0)
		goto fail_unlock;

	if (ret > 0) {
		if (path->slots[0] == 0)
			goto insert;
		path->slots[0]--;
	}

	leaf = path->nodes[0];
	btrfs_item_key_to_cpu(leaf, &found_key, path->slots[0]);
	csum_offset = (bytenr - found_key.offset) >>
			root->fs_info->sb->s_blocksize_bits;

<<<<<<< HEAD
<<<<<<< HEAD
	if (btrfs_key_type(&found_key) != BTRFS_EXTENT_CSUM_KEY ||
=======
	if (found_key.type != BTRFS_EXTENT_CSUM_KEY ||
>>>>>>> v3.18
=======
	if (found_key.type != BTRFS_EXTENT_CSUM_KEY ||
>>>>>>> v3.18
	    found_key.objectid != BTRFS_EXTENT_CSUM_OBJECTID ||
	    csum_offset >= MAX_CSUM_ITEMS(root, csum_size)) {
		goto insert;
	}

	if (csum_offset == btrfs_item_size_nr(leaf, path->slots[0]) /
	    csum_size) {
		int extend_nr;
		u64 tmp;
		u32 diff;
		u32 free_space;

		if (btrfs_leaf_free_space(root, leaf) <
				 sizeof(struct btrfs_item) + csum_size * 2)
			goto insert;

		free_space = btrfs_leaf_free_space(root, leaf) -
					 sizeof(struct btrfs_item) - csum_size;
<<<<<<< HEAD
<<<<<<< HEAD
		tmp = btrfs_sector_sum_left(sums, sector_sum, total_bytes,
					    root->sectorsize);
=======
		tmp = sums->len - total_bytes;
>>>>>>> v3.18
=======
		tmp = sums->len - total_bytes;
>>>>>>> v3.18
		tmp >>= root->fs_info->sb->s_blocksize_bits;
		WARN_ON(tmp < 1);

		extend_nr = max_t(int, 1, (int)tmp);
		diff = (csum_offset + extend_nr) * csum_size;
		diff = min(diff, MAX_CSUM_ITEMS(root, csum_size) * csum_size);

		diff = diff - btrfs_item_size_nr(leaf, path->slots[0]);
		diff = min(free_space, diff);
		diff /= csum_size;
		diff *= csum_size;

		btrfs_extend_item(root, path, diff);
<<<<<<< HEAD
<<<<<<< HEAD
=======
		ret = 0;
>>>>>>> v3.18
=======
		ret = 0;
>>>>>>> v3.18
		goto csum;
	}

insert:
	btrfs_release_path(path);
	csum_offset = 0;
	if (found_next) {
		u64 tmp;

<<<<<<< HEAD
<<<<<<< HEAD
		tmp = btrfs_sector_sum_left(sums, sector_sum, total_bytes,
					    root->sectorsize);
=======
		tmp = sums->len - total_bytes;
>>>>>>> v3.18
=======
		tmp = sums->len - total_bytes;
>>>>>>> v3.18
		tmp >>= root->fs_info->sb->s_blocksize_bits;
		tmp = min(tmp, (next_offset - file_key.offset) >>
					 root->fs_info->sb->s_blocksize_bits);

		tmp = max((u64)1, tmp);
		tmp = min(tmp, (u64)MAX_CSUM_ITEMS(root, csum_size));
		ins_size = csum_size * tmp;
	} else {
		ins_size = csum_size;
	}
	path->leave_spinning = 1;
	ret = btrfs_insert_empty_item(trans, root, path, &file_key,
				      ins_size);
	path->leave_spinning = 0;
	if (ret < 0)
		goto fail_unlock;
<<<<<<< HEAD
<<<<<<< HEAD
	if (ret != 0) {
		WARN_ON(1);
		goto fail_unlock;
	}
csum:
	leaf = path->nodes[0];
	item = btrfs_item_ptr(leaf, path->slots[0], struct btrfs_csum_item);
	ret = 0;
	item = (struct btrfs_csum_item *)((unsigned char *)item +
					  csum_offset * csum_size);
found:
	item_end = btrfs_item_ptr(leaf, path->slots[0], struct btrfs_csum_item);
	item_end = (struct btrfs_csum_item *)((unsigned char *)item_end +
				      btrfs_item_size_nr(leaf, path->slots[0]));
next_sector:

	write_extent_buffer(leaf, &sector_sum->sum, (unsigned long)item, csum_size);

	total_bytes += root->sectorsize;
	sector_sum++;
	if (total_bytes < sums->len) {
		item = (struct btrfs_csum_item *)((char *)item +
						  csum_size);
		if (item < item_end && bytenr + PAGE_CACHE_SIZE ==
		    sector_sum->bytenr) {
			bytenr = sector_sum->bytenr;
			goto next_sector;
		}
	}
=======
=======
>>>>>>> v3.18
	if (WARN_ON(ret != 0))
		goto fail_unlock;
	leaf = path->nodes[0];
csum:
	item = btrfs_item_ptr(leaf, path->slots[0], struct btrfs_csum_item);
	item_end = (struct btrfs_csum_item *)((unsigned char *)item +
				      btrfs_item_size_nr(leaf, path->slots[0]));
	item = (struct btrfs_csum_item *)((unsigned char *)item +
					  csum_offset * csum_size);
found:
	ins_size = (u32)(sums->len - total_bytes) >>
		   root->fs_info->sb->s_blocksize_bits;
	ins_size *= csum_size;
	ins_size = min_t(u32, (unsigned long)item_end - (unsigned long)item,
			      ins_size);
	write_extent_buffer(leaf, sums->sums + index, (unsigned long)item,
			    ins_size);

	ins_size /= csum_size;
	total_bytes += ins_size * root->sectorsize;
	index += ins_size;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	btrfs_mark_buffer_dirty(path->nodes[0]);
	if (total_bytes < sums->len) {
		btrfs_release_path(path);
		cond_resched();
		goto again;
	}
out:
	btrfs_free_path(path);
	return ret;

fail_unlock:
	goto out;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18

void btrfs_extent_item_to_extent_map(struct inode *inode,
				     const struct btrfs_path *path,
				     struct btrfs_file_extent_item *fi,
				     const bool new_inline,
				     struct extent_map *em)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct extent_buffer *leaf = path->nodes[0];
	const int slot = path->slots[0];
	struct btrfs_key key;
	u64 extent_start, extent_end;
	u64 bytenr;
	u8 type = btrfs_file_extent_type(leaf, fi);
	int compress_type = btrfs_file_extent_compression(leaf, fi);

	em->bdev = root->fs_info->fs_devices->latest_bdev;
	btrfs_item_key_to_cpu(leaf, &key, slot);
	extent_start = key.offset;

	if (type == BTRFS_FILE_EXTENT_REG ||
	    type == BTRFS_FILE_EXTENT_PREALLOC) {
		extent_end = extent_start +
			btrfs_file_extent_num_bytes(leaf, fi);
	} else if (type == BTRFS_FILE_EXTENT_INLINE) {
		size_t size;
		size = btrfs_file_extent_inline_len(leaf, slot, fi);
		extent_end = ALIGN(extent_start + size, root->sectorsize);
	}

	em->ram_bytes = btrfs_file_extent_ram_bytes(leaf, fi);
	if (type == BTRFS_FILE_EXTENT_REG ||
	    type == BTRFS_FILE_EXTENT_PREALLOC) {
		em->start = extent_start;
		em->len = extent_end - extent_start;
		em->orig_start = extent_start -
			btrfs_file_extent_offset(leaf, fi);
		em->orig_block_len = btrfs_file_extent_disk_num_bytes(leaf, fi);
		bytenr = btrfs_file_extent_disk_bytenr(leaf, fi);
		if (bytenr == 0) {
			em->block_start = EXTENT_MAP_HOLE;
			return;
		}
		if (compress_type != BTRFS_COMPRESS_NONE) {
			set_bit(EXTENT_FLAG_COMPRESSED, &em->flags);
			em->compress_type = compress_type;
			em->block_start = bytenr;
			em->block_len = em->orig_block_len;
		} else {
			bytenr += btrfs_file_extent_offset(leaf, fi);
			em->block_start = bytenr;
			em->block_len = em->len;
			if (type == BTRFS_FILE_EXTENT_PREALLOC)
				set_bit(EXTENT_FLAG_PREALLOC, &em->flags);
		}
	} else if (type == BTRFS_FILE_EXTENT_INLINE) {
		em->block_start = EXTENT_MAP_INLINE;
		em->start = extent_start;
		em->len = extent_end - extent_start;
		/*
		 * Initialize orig_start and block_len with the same values
		 * as in inode.c:btrfs_get_extent().
		 */
		em->orig_start = EXTENT_MAP_HOLE;
		em->block_len = (u64)-1;
		if (!new_inline && compress_type != BTRFS_COMPRESS_NONE) {
			set_bit(EXTENT_FLAG_COMPRESSED, &em->flags);
			em->compress_type = compress_type;
		}
	} else {
		btrfs_err(root->fs_info,
			  "unknown file extent item type %d, inode %llu, offset %llu, root %llu",
			  type, btrfs_ino(inode), extent_start,
			  root->root_key.objectid);
	}
}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
