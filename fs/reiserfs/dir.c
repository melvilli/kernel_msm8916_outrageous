/*
 * Copyright 2000 by Hans Reiser, licensing governed by reiserfs/README
 */

#include <linux/string.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include "reiserfs.h"
#include <linux/stat.h>
#include <linux/buffer_head.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/uaccess.h>

extern const struct reiserfs_key MIN_KEY;

static int reiserfs_readdir(struct file *, void *, filldir_t);
=======
=======
>>>>>>> v3.18
#include <linux/uaccess.h>

extern const struct reiserfs_key MIN_KEY;

static int reiserfs_readdir(struct file *, struct dir_context *);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static int reiserfs_dir_fsync(struct file *filp, loff_t start, loff_t end,
			      int datasync);

const struct file_operations reiserfs_dir_operations = {
	.llseek = generic_file_llseek,
	.read = generic_read_dir,
<<<<<<< HEAD
<<<<<<< HEAD
	.readdir = reiserfs_readdir,
=======
	.iterate = reiserfs_readdir,
>>>>>>> v3.18
=======
	.iterate = reiserfs_readdir,
>>>>>>> v3.18
	.fsync = reiserfs_dir_fsync,
	.unlocked_ioctl = reiserfs_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = reiserfs_compat_ioctl,
#endif
};

static int reiserfs_dir_fsync(struct file *filp, loff_t start, loff_t end,
			      int datasync)
{
	struct inode *inode = filp->f_mapping->host;
	int err;

	err = filemap_write_and_wait_range(inode->i_mapping, start, end);
	if (err)
		return err;

	mutex_lock(&inode->i_mutex);
	reiserfs_write_lock(inode->i_sb);
	err = reiserfs_commit_for_inode(inode);
	reiserfs_write_unlock(inode->i_sb);
	mutex_unlock(&inode->i_mutex);
	if (err < 0)
		return err;
	return 0;
}

#define store_ih(where,what) copy_item_head (where, what)

<<<<<<< HEAD
<<<<<<< HEAD
static inline bool is_privroot_deh(struct dentry *dir,
				   struct reiserfs_de_head *deh)
{
	struct dentry *privroot = REISERFS_SB(dir->d_sb)->priv_root;
	return (dir == dir->d_parent && privroot->d_inode &&
	        deh->deh_objectid == INODE_PKEY(privroot->d_inode)->k_objectid);
}

int reiserfs_readdir_dentry(struct dentry *dentry, void *dirent,
			   filldir_t filldir, loff_t *pos)
{
	struct inode *inode = dentry->d_inode;
	struct cpu_key pos_key;	/* key of current position in the directory (key of directory entry) */
=======
=======
>>>>>>> v3.18
static inline bool is_privroot_deh(struct inode *dir, struct reiserfs_de_head *deh)
{
	struct dentry *privroot = REISERFS_SB(dir->i_sb)->priv_root;
	return (privroot->d_inode &&
	        deh->deh_objectid == INODE_PKEY(privroot->d_inode)->k_objectid);
}

int reiserfs_readdir_inode(struct inode *inode, struct dir_context *ctx)
{

	/* key of current position in the directory (key of directory entry) */
	struct cpu_key pos_key;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	INITIALIZE_PATH(path_to_entry);
	struct buffer_head *bh;
	int item_num, entry_num;
	const struct reiserfs_key *rkey;
	struct item_head *ih, tmp_ih;
	int search_res;
	char *local_buf;
	loff_t next_pos;
	char small_buf[32];	/* avoid kmalloc if we can */
	struct reiserfs_dir_entry de;
	int ret = 0;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	int depth;
>>>>>>> v3.18
=======
	int depth;
>>>>>>> v3.18

	reiserfs_write_lock(inode->i_sb);

	reiserfs_check_lock_depth(inode->i_sb, "readdir");

<<<<<<< HEAD
<<<<<<< HEAD
	/* form key for search the next directory entry using f_pos field of
	   file structure */
	make_cpu_key(&pos_key, inode, *pos ?: DOT_OFFSET, TYPE_DIRENTRY, 3);
=======
=======
>>>>>>> v3.18
	/*
	 * form key for search the next directory entry using
	 * f_pos field of file structure
	 */
	make_cpu_key(&pos_key, inode, ctx->pos ?: DOT_OFFSET, TYPE_DIRENTRY, 3);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	next_pos = cpu_key_k_offset(&pos_key);

	path_to_entry.reada = PATH_READA;
	while (1) {
<<<<<<< HEAD
<<<<<<< HEAD
	      research:
		/* search the directory item, containing entry with specified key */
=======
=======
>>>>>>> v3.18
research:
		/*
		 * search the directory item, containing entry with
		 * specified key
		 */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		search_res =
		    search_by_entry_key(inode->i_sb, &pos_key, &path_to_entry,
					&de);
		if (search_res == IO_ERROR) {
<<<<<<< HEAD
<<<<<<< HEAD
			// FIXME: we could just skip part of directory which could
			// not be read
=======
=======
>>>>>>> v3.18
			/*
			 * FIXME: we could just skip part of directory
			 * which could not be read
			 */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			ret = -EIO;
			goto out;
		}
		entry_num = de.de_entry_num;
		bh = de.de_bh;
		item_num = de.de_item_num;
		ih = de.de_ih;
		store_ih(&tmp_ih, ih);

		/* we must have found item, that is item of this directory, */
<<<<<<< HEAD
<<<<<<< HEAD
		RFALSE(COMP_SHORT_KEYS(&(ih->ih_key), &pos_key),
=======
		RFALSE(COMP_SHORT_KEYS(&ih->ih_key, &pos_key),
>>>>>>> v3.18
=======
		RFALSE(COMP_SHORT_KEYS(&ih->ih_key, &pos_key),
>>>>>>> v3.18
		       "vs-9000: found item %h does not match to dir we readdir %K",
		       ih, &pos_key);
		RFALSE(item_num > B_NR_ITEMS(bh) - 1,
		       "vs-9005 item_num == %d, item amount == %d",
		       item_num, B_NR_ITEMS(bh));

<<<<<<< HEAD
<<<<<<< HEAD
		/* and entry must be not more than number of entries in the item */
		RFALSE(I_ENTRY_COUNT(ih) < entry_num,
		       "vs-9010: entry number is too big %d (%d)",
		       entry_num, I_ENTRY_COUNT(ih));

		if (search_res == POSITION_FOUND
		    || entry_num < I_ENTRY_COUNT(ih)) {
			/* go through all entries in the directory item beginning from the entry, that has been found */
			struct reiserfs_de_head *deh =
			    B_I_DEH(bh, ih) + entry_num;

			for (; entry_num < I_ENTRY_COUNT(ih);
			     entry_num++, deh++) {
				int d_reclen;
				char *d_name;
				off_t d_off;
				ino_t d_ino;
				loff_t cur_pos = deh_offset(deh);

				if (!de_visible(deh))
					/* it is hidden entry */
=======
=======
>>>>>>> v3.18
		/*
		 * and entry must be not more than number of entries
		 * in the item
		 */
		RFALSE(ih_entry_count(ih) < entry_num,
		       "vs-9010: entry number is too big %d (%d)",
		       entry_num, ih_entry_count(ih));

		/*
		 * go through all entries in the directory item beginning
		 * from the entry, that has been found
		 */
		if (search_res == POSITION_FOUND
		    || entry_num < ih_entry_count(ih)) {
			struct reiserfs_de_head *deh =
			    B_I_DEH(bh, ih) + entry_num;

			for (; entry_num < ih_entry_count(ih);
			     entry_num++, deh++) {
				int d_reclen;
				char *d_name;
				ino_t d_ino;
				loff_t cur_pos = deh_offset(deh);

				/* it is hidden entry */
				if (!de_visible(deh))
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
					continue;
				d_reclen = entry_length(bh, ih, entry_num);
				d_name = B_I_DEH_ENTRY_FILE_NAME(bh, ih, deh);

				if (d_reclen <= 0 ||
				    d_name + d_reclen > bh->b_data + bh->b_size) {
<<<<<<< HEAD
<<<<<<< HEAD
					/* There is corrupted data in entry,
					 * We'd better stop here */
=======
=======
>>>>>>> v3.18
					/*
					 * There is corrupted data in entry,
					 * We'd better stop here
					 */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
					pathrelse(&path_to_entry);
					ret = -EIO;
					goto out;
				}

				if (!d_name[d_reclen - 1])
					d_reclen = strlen(d_name);

<<<<<<< HEAD
<<<<<<< HEAD
				if (d_reclen >
				    REISERFS_MAX_NAME(inode->i_sb->
						      s_blocksize)) {
					/* too big to send back to VFS */
=======
=======
>>>>>>> v3.18
				/* too big to send back to VFS */
				if (d_reclen >
				    REISERFS_MAX_NAME(inode->i_sb->
						      s_blocksize)) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
					continue;
				}

				/* Ignore the .reiserfs_priv entry */
<<<<<<< HEAD
<<<<<<< HEAD
				if (is_privroot_deh(dentry, deh))
					continue;

				d_off = deh_offset(deh);
				*pos = d_off;
=======
=======
>>>>>>> v3.18
				if (is_privroot_deh(inode, deh))
					continue;

				ctx->pos = deh_offset(deh);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
				d_ino = deh_objectid(deh);
				if (d_reclen <= 32) {
					local_buf = small_buf;
				} else {
					local_buf = kmalloc(d_reclen,
							    GFP_NOFS);
					if (!local_buf) {
						pathrelse(&path_to_entry);
						ret = -ENOMEM;
						goto out;
					}
					if (item_moved(&tmp_ih, &path_to_entry)) {
						kfree(local_buf);
						goto research;
					}
				}
<<<<<<< HEAD
<<<<<<< HEAD
				// Note, that we copy name to user space via temporary
				// buffer (local_buf) because filldir will block if
				// user space buffer is swapped out. At that time
				// entry can move to somewhere else
=======
=======
>>>>>>> v3.18

				/*
				 * Note, that we copy name to user space via
				 * temporary buffer (local_buf) because
				 * filldir will block if user space buffer is
				 * swapped out. At that time entry can move to
				 * somewhere else
				 */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
				memcpy(local_buf, d_name, d_reclen);

				/*
				 * Since filldir might sleep, we can release
				 * the write lock here for other waiters
				 */
<<<<<<< HEAD
<<<<<<< HEAD
				reiserfs_write_unlock(inode->i_sb);
				if (filldir
				    (dirent, local_buf, d_reclen, d_off, d_ino,
				     DT_UNKNOWN) < 0) {
					reiserfs_write_lock(inode->i_sb);
=======
=======
>>>>>>> v3.18
				depth = reiserfs_write_unlock_nested(inode->i_sb);
				if (!dir_emit
				    (ctx, local_buf, d_reclen, d_ino,
				     DT_UNKNOWN)) {
					reiserfs_write_lock_nested(inode->i_sb, depth);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
					if (local_buf != small_buf) {
						kfree(local_buf);
					}
					goto end;
				}
<<<<<<< HEAD
<<<<<<< HEAD
				reiserfs_write_lock(inode->i_sb);
=======
				reiserfs_write_lock_nested(inode->i_sb, depth);
>>>>>>> v3.18
=======
				reiserfs_write_lock_nested(inode->i_sb, depth);
>>>>>>> v3.18
				if (local_buf != small_buf) {
					kfree(local_buf);
				}

				/* deh_offset(deh) may be invalid now. */
				next_pos = cur_pos + 1;

				if (item_moved(&tmp_ih, &path_to_entry)) {
					set_cpu_key_k_offset(&pos_key,
							     next_pos);
					goto research;
				}
			}	/* for */
		}

<<<<<<< HEAD
<<<<<<< HEAD
		if (item_num != B_NR_ITEMS(bh) - 1)
			// end of directory has been reached
			goto end;

		/* item we went through is last item of node. Using right
		   delimiting key check is it directory end */
		rkey = get_rkey(&path_to_entry, inode->i_sb);
		if (!comp_le_keys(rkey, &MIN_KEY)) {
			/* set pos_key to key, that is the smallest and greater
			   that key of the last entry in the item */
=======
=======
>>>>>>> v3.18
		/* end of directory has been reached */
		if (item_num != B_NR_ITEMS(bh) - 1)
			goto end;

		/*
		 * item we went through is last item of node. Using right
		 * delimiting key check is it directory end
		 */
		rkey = get_rkey(&path_to_entry, inode->i_sb);
		if (!comp_le_keys(rkey, &MIN_KEY)) {
			/*
			 * set pos_key to key, that is the smallest and greater
			 * that key of the last entry in the item
			 */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			set_cpu_key_k_offset(&pos_key, next_pos);
			continue;
		}

<<<<<<< HEAD
<<<<<<< HEAD
		if (COMP_SHORT_KEYS(rkey, &pos_key)) {
			// end of directory has been reached
=======
		/* end of directory has been reached */
		if (COMP_SHORT_KEYS(rkey, &pos_key)) {
>>>>>>> v3.18
=======
		/* end of directory has been reached */
		if (COMP_SHORT_KEYS(rkey, &pos_key)) {
>>>>>>> v3.18
			goto end;
		}

		/* directory continues in the right neighboring block */
		set_cpu_key_k_offset(&pos_key,
				     le_key_k_offset(KEY_FORMAT_3_5, rkey));

	}			/* while */

end:
<<<<<<< HEAD
<<<<<<< HEAD
	*pos = next_pos;
=======
	ctx->pos = next_pos;
>>>>>>> v3.18
=======
	ctx->pos = next_pos;
>>>>>>> v3.18
	pathrelse(&path_to_entry);
	reiserfs_check_path(&path_to_entry);
out:
	reiserfs_write_unlock(inode->i_sb);
	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int reiserfs_readdir(struct file *file, void *dirent, filldir_t filldir)
{
	struct dentry *dentry = file->f_path.dentry;
	return reiserfs_readdir_dentry(dentry, dirent, filldir, &file->f_pos);
}

/* compose directory item containing "." and ".." entries (entries are
   not aligned to 4 byte boundary) */
/* the last four params are LE */
void make_empty_dir_item_v1(char *body, __le32 dirid, __le32 objid,
			    __le32 par_dirid, __le32 par_objid)
{
	struct reiserfs_de_head *deh;

	memset(body, 0, EMPTY_DIR_SIZE_V1);
	deh = (struct reiserfs_de_head *)body;

	/* direntry header of "." */
	put_deh_offset(&(deh[0]), DOT_OFFSET);
	/* these two are from make_le_item_head, and are are LE */
	deh[0].deh_dir_id = dirid;
	deh[0].deh_objectid = objid;
	deh[0].deh_state = 0;	/* Endian safe if 0 */
	put_deh_location(&(deh[0]), EMPTY_DIR_SIZE_V1 - strlen("."));
	mark_de_visible(&(deh[0]));

	/* direntry header of ".." */
	put_deh_offset(&(deh[1]), DOT_DOT_OFFSET);
	/* key of ".." for the root directory */
	/* these two are from the inode, and are are LE */
	deh[1].deh_dir_id = par_dirid;
	deh[1].deh_objectid = par_objid;
	deh[1].deh_state = 0;	/* Endian safe if 0 */
	put_deh_location(&(deh[1]), deh_location(&(deh[0])) - strlen(".."));
	mark_de_visible(&(deh[1]));

	/* copy ".." and "." */
	memcpy(body + deh_location(&(deh[0])), ".", 1);
	memcpy(body + deh_location(&(deh[1])), "..", 2);
=======
=======
>>>>>>> v3.18
static int reiserfs_readdir(struct file *file, struct dir_context *ctx)
{
	return reiserfs_readdir_inode(file_inode(file), ctx);
}

/*
 * compose directory item containing "." and ".." entries (entries are
 * not aligned to 4 byte boundary)
 */
void make_empty_dir_item_v1(char *body, __le32 dirid, __le32 objid,
			    __le32 par_dirid, __le32 par_objid)
{
	struct reiserfs_de_head *dot, *dotdot;

	memset(body, 0, EMPTY_DIR_SIZE_V1);
	dot = (struct reiserfs_de_head *)body;
	dotdot = dot + 1;

	/* direntry header of "." */
	put_deh_offset(dot, DOT_OFFSET);
	/* these two are from make_le_item_head, and are are LE */
	dot->deh_dir_id = dirid;
	dot->deh_objectid = objid;
	dot->deh_state = 0;	/* Endian safe if 0 */
	put_deh_location(dot, EMPTY_DIR_SIZE_V1 - strlen("."));
	mark_de_visible(dot);

	/* direntry header of ".." */
	put_deh_offset(dotdot, DOT_DOT_OFFSET);
	/* key of ".." for the root directory */
	/* these two are from the inode, and are are LE */
	dotdot->deh_dir_id = par_dirid;
	dotdot->deh_objectid = par_objid;
	dotdot->deh_state = 0;	/* Endian safe if 0 */
	put_deh_location(dotdot, deh_location(dot) - strlen(".."));
	mark_de_visible(dotdot);

	/* copy ".." and "." */
	memcpy(body + deh_location(dot), ".", 1);
	memcpy(body + deh_location(dotdot), "..", 2);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/* compose directory item containing "." and ".." entries */
void make_empty_dir_item(char *body, __le32 dirid, __le32 objid,
			 __le32 par_dirid, __le32 par_objid)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct reiserfs_de_head *deh;

	memset(body, 0, EMPTY_DIR_SIZE);
	deh = (struct reiserfs_de_head *)body;

	/* direntry header of "." */
	put_deh_offset(&(deh[0]), DOT_OFFSET);
	/* these two are from make_le_item_head, and are are LE */
	deh[0].deh_dir_id = dirid;
	deh[0].deh_objectid = objid;
	deh[0].deh_state = 0;	/* Endian safe if 0 */
	put_deh_location(&(deh[0]), EMPTY_DIR_SIZE - ROUND_UP(strlen(".")));
	mark_de_visible(&(deh[0]));

	/* direntry header of ".." */
	put_deh_offset(&(deh[1]), DOT_DOT_OFFSET);
	/* key of ".." for the root directory */
	/* these two are from the inode, and are are LE */
	deh[1].deh_dir_id = par_dirid;
	deh[1].deh_objectid = par_objid;
	deh[1].deh_state = 0;	/* Endian safe if 0 */
	put_deh_location(&(deh[1]),
			 deh_location(&(deh[0])) - ROUND_UP(strlen("..")));
	mark_de_visible(&(deh[1]));

	/* copy ".." and "." */
	memcpy(body + deh_location(&(deh[0])), ".", 1);
	memcpy(body + deh_location(&(deh[1])), "..", 2);
=======
=======
>>>>>>> v3.18
	struct reiserfs_de_head *dot, *dotdot;

	memset(body, 0, EMPTY_DIR_SIZE);
	dot = (struct reiserfs_de_head *)body;
	dotdot = dot + 1;

	/* direntry header of "." */
	put_deh_offset(dot, DOT_OFFSET);
	/* these two are from make_le_item_head, and are are LE */
	dot->deh_dir_id = dirid;
	dot->deh_objectid = objid;
	dot->deh_state = 0;	/* Endian safe if 0 */
	put_deh_location(dot, EMPTY_DIR_SIZE - ROUND_UP(strlen(".")));
	mark_de_visible(dot);

	/* direntry header of ".." */
	put_deh_offset(dotdot, DOT_DOT_OFFSET);
	/* key of ".." for the root directory */
	/* these two are from the inode, and are are LE */
	dotdot->deh_dir_id = par_dirid;
	dotdot->deh_objectid = par_objid;
	dotdot->deh_state = 0;	/* Endian safe if 0 */
	put_deh_location(dotdot, deh_location(dot) - ROUND_UP(strlen("..")));
	mark_de_visible(dotdot);

	/* copy ".." and "." */
	memcpy(body + deh_location(dot), ".", 1);
	memcpy(body + deh_location(dotdot), "..", 2);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}
