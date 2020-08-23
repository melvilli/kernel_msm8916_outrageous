#include <linux/mount.h>
#include <linux/seq_file.h>
#include <linux/poll.h>

struct mnt_namespace {
	atomic_t		count;
	unsigned int		proc_inum;
	struct mount *	root;
	struct list_head	list;
	struct user_namespace	*user_ns;
	u64			seq;	/* Sequence number to prevent loops */
	wait_queue_head_t poll;
<<<<<<< HEAD
<<<<<<< HEAD
	int event;
=======
	u64 event;
>>>>>>> v3.18
=======
	u64 event;
>>>>>>> v3.18
};

struct mnt_pcp {
	int mnt_count;
	int mnt_writers;
};

struct mountpoint {
<<<<<<< HEAD
<<<<<<< HEAD
	struct list_head m_hash;
	struct dentry *m_dentry;
=======
	struct hlist_node m_hash;
	struct dentry *m_dentry;
	struct hlist_head m_list;
>>>>>>> v3.18
=======
	struct hlist_node m_hash;
	struct dentry *m_dentry;
	struct hlist_head m_list;
>>>>>>> v3.18
	int m_count;
};

struct mount {
<<<<<<< HEAD
<<<<<<< HEAD
	struct list_head mnt_hash;
	struct mount *mnt_parent;
	struct dentry *mnt_mountpoint;
	struct vfsmount mnt;
=======
=======
>>>>>>> v3.18
	struct hlist_node mnt_hash;
	struct mount *mnt_parent;
	struct dentry *mnt_mountpoint;
	struct vfsmount mnt;
	union {
		struct rcu_head mnt_rcu;
		struct llist_node mnt_llist;
	};
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#ifdef CONFIG_SMP
	struct mnt_pcp __percpu *mnt_pcp;
#else
	int mnt_count;
	int mnt_writers;
#endif
	struct list_head mnt_mounts;	/* list of children, anchored here */
	struct list_head mnt_child;	/* and going through their mnt_child */
	struct list_head mnt_instance;	/* mount instance on sb->s_mounts */
	const char *mnt_devname;	/* Name of device e.g. /dev/dsk/hda1 */
	struct list_head mnt_list;
	struct list_head mnt_expire;	/* link in fs-specific expiry list */
	struct list_head mnt_share;	/* circular list of shared mounts */
	struct list_head mnt_slave_list;/* list of slave mounts */
	struct list_head mnt_slave;	/* slave list entry */
	struct mount *mnt_master;	/* slave is on master->mnt_slave_list */
	struct mnt_namespace *mnt_ns;	/* containing namespace */
	struct mountpoint *mnt_mp;	/* where is it mounted */
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct hlist_node mnt_mp_list;	/* list mounts with the same mountpoint */
>>>>>>> v3.18
=======
	struct hlist_node mnt_mp_list;	/* list mounts with the same mountpoint */
>>>>>>> v3.18
#ifdef CONFIG_FSNOTIFY
	struct hlist_head mnt_fsnotify_marks;
	__u32 mnt_fsnotify_mask;
#endif
	int mnt_id;			/* mount identifier */
	int mnt_group_id;		/* peer group identifier */
	int mnt_expiry_mark;		/* true if marked for expiry */
<<<<<<< HEAD
<<<<<<< HEAD
	int mnt_pinned;
	int mnt_ghosts;
=======
	struct hlist_head mnt_pins;
	struct path mnt_ex_mountpoint;
>>>>>>> v3.18
=======
	struct hlist_head mnt_pins;
	struct path mnt_ex_mountpoint;
>>>>>>> v3.18
};

#define MNT_NS_INTERNAL ERR_PTR(-EINVAL) /* distinct from any mnt_namespace */

static inline struct mount *real_mount(struct vfsmount *mnt)
{
	return container_of(mnt, struct mount, mnt);
}

static inline int mnt_has_parent(struct mount *mnt)
{
	return mnt != mnt->mnt_parent;
}

static inline int is_mounted(struct vfsmount *mnt)
{
	/* neither detached nor internal? */
	return !IS_ERR_OR_NULL(real_mount(mnt)->mnt_ns);
}

<<<<<<< HEAD
<<<<<<< HEAD
extern struct mount *__lookup_mnt(struct vfsmount *, struct dentry *, int);
=======
=======
>>>>>>> v3.18
extern struct mount *__lookup_mnt(struct vfsmount *, struct dentry *);
extern struct mount *__lookup_mnt_last(struct vfsmount *, struct dentry *);

extern bool legitimize_mnt(struct vfsmount *, unsigned);

extern void __detach_mounts(struct dentry *dentry);

static inline void detach_mounts(struct dentry *dentry)
{
	if (!d_mountpoint(dentry))
		return;
	__detach_mounts(dentry);
}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

static inline void get_mnt_ns(struct mnt_namespace *ns)
{
	atomic_inc(&ns->count);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
extern seqlock_t mount_lock;

static inline void lock_mount_hash(void)
{
	write_seqlock(&mount_lock);
}

static inline void unlock_mount_hash(void)
{
	write_sequnlock(&mount_lock);
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
struct proc_mounts {
	struct seq_file m;
	struct mnt_namespace *ns;
	struct path root;
	int (*show)(struct seq_file *, struct vfsmount *);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	void *cached_mount;
	u64 cached_event;
	loff_t cached_index;
>>>>>>> v3.18
=======
	void *cached_mount;
	u64 cached_event;
	loff_t cached_index;
>>>>>>> v3.18
};

#define proc_mounts(p) (container_of((p), struct proc_mounts, m))

extern const struct seq_operations mounts_op;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18

extern bool __is_local_mountpoint(struct dentry *dentry);
static inline bool is_local_mountpoint(struct dentry *dentry)
{
	if (!d_mountpoint(dentry))
		return false;

	return __is_local_mountpoint(dentry);
}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
