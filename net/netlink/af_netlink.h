#ifndef _AF_NETLINK_H
#define _AF_NETLINK_H

<<<<<<< HEAD
=======
#include <linux/rhashtable.h>
>>>>>>> v3.18
#include <net/sock.h>

#define NLGRPSZ(x)	(ALIGN(x, sizeof(unsigned long) * 8) / 8)
#define NLGRPLONGS(x)	(NLGRPSZ(x)/sizeof(unsigned long))

struct netlink_ring {
	void			**pg_vec;
	unsigned int		head;
	unsigned int		frames_per_block;
	unsigned int		frame_size;
	unsigned int		frame_max;

	unsigned int		pg_vec_order;
	unsigned int		pg_vec_pages;
	unsigned int		pg_vec_len;

	atomic_t		pending;
};

struct netlink_sock {
	/* struct sock has to be the first member of netlink_sock */
	struct sock		sk;
	u32			portid;
	u32			dst_portid;
	u32			dst_group;
	u32			flags;
	u32			subscriptions;
	u32			ngroups;
	unsigned long		*groups;
	unsigned long		state;
<<<<<<< HEAD
=======
	size_t			max_recvmsg_len;
>>>>>>> v3.18
	wait_queue_head_t	wait;
	bool			cb_running;
	struct netlink_callback	cb;
	struct mutex		*cb_mutex;
	struct mutex		cb_def_mutex;
	void			(*netlink_rcv)(struct sk_buff *skb);
<<<<<<< HEAD
	void			(*netlink_bind)(int group);
=======
	int			(*netlink_bind)(int group);
	void			(*netlink_unbind)(int group);
>>>>>>> v3.18
	struct module		*module;
#ifdef CONFIG_NETLINK_MMAP
	struct mutex		pg_vec_lock;
	struct netlink_ring	rx_ring;
	struct netlink_ring	tx_ring;
	atomic_t		mapped;
#endif /* CONFIG_NETLINK_MMAP */
<<<<<<< HEAD
=======

	struct rhash_head	node;
>>>>>>> v3.18
};

static inline struct netlink_sock *nlk_sk(struct sock *sk)
{
	return container_of(sk, struct netlink_sock, sk);
}

<<<<<<< HEAD
struct nl_portid_hash {
	struct hlist_head	*table;
	unsigned long		rehash_time;

	unsigned int		mask;
	unsigned int		shift;

	unsigned int		entries;
	unsigned int		max_shift;

	u32			rnd;
};

struct netlink_table {
	struct nl_portid_hash	hash;
=======
struct netlink_table {
	struct rhashtable	hash;
>>>>>>> v3.18
	struct hlist_head	mc_list;
	struct listeners __rcu	*listeners;
	unsigned int		flags;
	unsigned int		groups;
	struct mutex		*cb_mutex;
	struct module		*module;
<<<<<<< HEAD
	void			(*bind)(int group);
=======
	int			(*bind)(int group);
	void			(*unbind)(int group);
	bool			(*compare)(struct net *net, struct sock *sock);
>>>>>>> v3.18
	int			registered;
};

extern struct netlink_table *nl_table;
extern rwlock_t nl_table_lock;
<<<<<<< HEAD
=======
extern struct mutex nl_sk_hash_lock;
>>>>>>> v3.18

#endif
