/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2007-2011 Nicira, Inc.
=======
 * Copyright (c) 2007-2014 Nicira, Inc.
>>>>>>> v3.18
=======
 * Copyright (c) 2007-2014 Nicira, Inc.
>>>>>>> v3.18
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef FLOW_H
#define FLOW_H 1

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/cache.h>
>>>>>>> v3.18
=======
#include <linux/cache.h>
>>>>>>> v3.18
#include <linux/kernel.h>
#include <linux/netlink.h>
#include <linux/openvswitch.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/rcupdate.h>
#include <linux/if_ether.h>
#include <linux/in6.h>
#include <linux/jiffies.h>
#include <linux/time.h>
#include <linux/flex_array.h>
#include <net/inet_ecn.h>

struct sk_buff;

<<<<<<< HEAD
<<<<<<< HEAD
struct sw_flow_actions {
	struct rcu_head rcu;
	u32 actions_len;
	struct nlattr actions[];
};

struct sw_flow_key {
=======
=======
>>>>>>> v3.18
/* Used to memset ovs_key_ipv4_tunnel padding. */
#define OVS_TUNNEL_KEY_SIZE					\
	(offsetof(struct ovs_key_ipv4_tunnel, ipv4_ttl) +	\
	FIELD_SIZEOF(struct ovs_key_ipv4_tunnel, ipv4_ttl))

struct ovs_key_ipv4_tunnel {
	__be64 tun_id;
	__be32 ipv4_src;
	__be32 ipv4_dst;
	__be16 tun_flags;
	u8   ipv4_tos;
	u8   ipv4_ttl;
} __packed __aligned(4); /* Minimize padding. */

struct ovs_tunnel_info {
	struct ovs_key_ipv4_tunnel tunnel;
	struct geneve_opt *options;
	u8 options_len;
};

/* Store options at the end of the array if they are less than the
 * maximum size. This allows us to get the benefits of variable length
 * matching for small options.
 */
#define GENEVE_OPTS(flow_key, opt_len)	\
	((struct geneve_opt *)((flow_key)->tun_opts + \
			       FIELD_SIZEOF(struct sw_flow_key, tun_opts) - \
			       opt_len))

static inline void ovs_flow_tun_info_init(struct ovs_tunnel_info *tun_info,
					  const struct iphdr *iph,
					  __be64 tun_id, __be16 tun_flags,
					  struct geneve_opt *opts,
					  u8 opts_len)
{
	tun_info->tunnel.tun_id = tun_id;
	tun_info->tunnel.ipv4_src = iph->saddr;
	tun_info->tunnel.ipv4_dst = iph->daddr;
	tun_info->tunnel.ipv4_tos = iph->tos;
	tun_info->tunnel.ipv4_ttl = iph->ttl;
	tun_info->tunnel.tun_flags = tun_flags;

	/* clear struct padding. */
	memset((unsigned char *)&tun_info->tunnel + OVS_TUNNEL_KEY_SIZE, 0,
	       sizeof(tun_info->tunnel) - OVS_TUNNEL_KEY_SIZE);

	tun_info->options = opts;
	tun_info->options_len = opts_len;
}

struct sw_flow_key {
	u8 tun_opts[255];
	u8 tun_opts_len;
	struct ovs_key_ipv4_tunnel tun_key;  /* Encapsulating tunnel key. */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	struct {
		u32	priority;	/* Packet QoS priority. */
		u32	skb_mark;	/* SKB mark. */
		u16	in_port;	/* Input switch port (or DP_MAX_PORTS). */
<<<<<<< HEAD
<<<<<<< HEAD
	} phy;
=======
	} __packed phy; /* Safe when right after 'tun_key'. */
	u32 ovs_flow_hash;		/* Datapath computed hash value.  */
	u32 recirc_id;			/* Recirculation ID.  */
>>>>>>> v3.18
=======
	} __packed phy; /* Safe when right after 'tun_key'. */
	u32 ovs_flow_hash;		/* Datapath computed hash value.  */
	u32 recirc_id;			/* Recirculation ID.  */
>>>>>>> v3.18
	struct {
		u8     src[ETH_ALEN];	/* Ethernet source address. */
		u8     dst[ETH_ALEN];	/* Ethernet destination address. */
		__be16 tci;		/* 0 if no VLAN, VLAN_TAG_PRESENT set otherwise. */
		__be16 type;		/* Ethernet frame type. */
	} eth;
	struct {
		u8     proto;		/* IP protocol or lower 8 bits of ARP opcode. */
		u8     tos;		/* IP ToS. */
		u8     ttl;		/* IP TTL/hop limit. */
		u8     frag;		/* One of OVS_FRAG_TYPE_*. */
	} ip;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	struct {
		__be16 src;		/* TCP/UDP/SCTP source port. */
		__be16 dst;		/* TCP/UDP/SCTP destination port. */
		__be16 flags;		/* TCP flags. */
	} tp;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	union {
		struct {
			struct {
				__be32 src;	/* IP source address. */
				__be32 dst;	/* IP destination address. */
			} addr;
<<<<<<< HEAD
<<<<<<< HEAD
			union {
				struct {
					__be16 src;		/* TCP/UDP source port. */
					__be16 dst;		/* TCP/UDP destination port. */
				} tp;
				struct {
					u8 sha[ETH_ALEN];	/* ARP source hardware address. */
					u8 tha[ETH_ALEN];	/* ARP target hardware address. */
				} arp;
			};
=======
=======
>>>>>>> v3.18
			struct {
				u8 sha[ETH_ALEN];	/* ARP source hardware address. */
				u8 tha[ETH_ALEN];	/* ARP target hardware address. */
			} arp;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		} ipv4;
		struct {
			struct {
				struct in6_addr src;	/* IPv6 source address. */
				struct in6_addr dst;	/* IPv6 destination address. */
			} addr;
			__be32 label;			/* IPv6 flow label. */
			struct {
<<<<<<< HEAD
<<<<<<< HEAD
				__be16 src;		/* TCP/UDP source port. */
				__be16 dst;		/* TCP/UDP destination port. */
			} tp;
			struct {
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
				struct in6_addr target;	/* ND target address. */
				u8 sll[ETH_ALEN];	/* ND source link layer address. */
				u8 tll[ETH_ALEN];	/* ND target link layer address. */
			} nd;
		} ipv6;
	};
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
} __aligned(BITS_PER_LONG/8); /* Ensure that we can do comparisons as longs. */

struct sw_flow_key_range {
	unsigned short int start;
	unsigned short int end;
};

struct sw_flow_mask {
	int ref_count;
	struct rcu_head rcu;
	struct list_head list;
	struct sw_flow_key_range range;
	struct sw_flow_key key;
};

struct sw_flow_match {
	struct sw_flow_key *key;
	struct sw_flow_key_range range;
	struct sw_flow_mask *mask;
};

struct sw_flow_actions {
	struct rcu_head rcu;
	u32 actions_len;
	struct nlattr actions[];
};

struct flow_stats {
	u64 packet_count;		/* Number of packets matched. */
	u64 byte_count;			/* Number of bytes matched. */
	unsigned long used;		/* Last used time (in jiffies). */
	spinlock_t lock;		/* Lock for atomic stats update. */
	__be16 tcp_flags;		/* Union of seen TCP flags. */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
};

struct sw_flow {
	struct rcu_head rcu;
	struct hlist_node hash_node[2];
	u32 hash;
<<<<<<< HEAD
<<<<<<< HEAD

	struct sw_flow_key key;
	struct sw_flow_actions __rcu *sf_acts;

	spinlock_t lock;	/* Lock for values below. */
	unsigned long used;	/* Last used time (in jiffies). */
	u64 packet_count;	/* Number of packets matched. */
	u64 byte_count;		/* Number of bytes matched. */
	u8 tcp_flags;		/* Union of seen TCP flags. */
=======
=======
>>>>>>> v3.18
	int stats_last_writer;		/* NUMA-node id of the last writer on
					 * 'stats[0]'.
					 */
	struct sw_flow_key key;
	struct sw_flow_key unmasked_key;
	struct sw_flow_mask *mask;
	struct sw_flow_actions __rcu *sf_acts;
	struct flow_stats __rcu *stats[]; /* One for each NUMA node.  First one
					   * is allocated at flow creation time,
					   * the rest are allocated on demand
					   * while holding the 'stats[0].lock'.
					   */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
};

struct arp_eth_header {
	__be16      ar_hrd;	/* format of hardware address   */
	__be16      ar_pro;	/* format of protocol address   */
	unsigned char   ar_hln;	/* length of hardware address   */
	unsigned char   ar_pln;	/* length of protocol address   */
	__be16      ar_op;	/* ARP opcode (command)     */

	/* Ethernet+IPv4 specific members. */
	unsigned char       ar_sha[ETH_ALEN];	/* sender hardware address  */
	unsigned char       ar_sip[4];		/* sender IP address        */
	unsigned char       ar_tha[ETH_ALEN];	/* target hardware address  */
	unsigned char       ar_tip[4];		/* target IP address        */
} __packed;

<<<<<<< HEAD
<<<<<<< HEAD
int ovs_flow_init(void);
void ovs_flow_exit(void);

struct sw_flow *ovs_flow_alloc(void);
void ovs_flow_deferred_free(struct sw_flow *);
void ovs_flow_free(struct sw_flow *flow);

struct sw_flow_actions *ovs_flow_actions_alloc(const struct nlattr *);
void ovs_flow_deferred_free_acts(struct sw_flow_actions *);

int ovs_flow_extract(struct sk_buff *, u16 in_port, struct sw_flow_key *,
		     int *key_lenp);
void ovs_flow_used(struct sw_flow *, struct sk_buff *);
u64 ovs_flow_used_time(unsigned long flow_jiffies);

int ovs_flow_to_nlattrs(const struct sw_flow_key *, struct sk_buff *);
int ovs_flow_from_nlattrs(struct sw_flow_key *swkey, int *key_lenp,
		      const struct nlattr *);
int ovs_flow_metadata_from_nlattrs(u32 *priority, u32 *mark, u16 *in_port,
			       const struct nlattr *);

#define MAX_ACTIONS_BUFSIZE    (16 * 1024)
#define TBL_MIN_BUCKETS		1024

struct flow_table {
	struct flex_array *buckets;
	unsigned int count, n_buckets;
	struct rcu_head rcu;
	int node_ver;
	u32 hash_seed;
	bool keep_flows;
};

static inline int ovs_flow_tbl_count(struct flow_table *table)
{
	return table->count;
}

static inline int ovs_flow_tbl_need_to_expand(struct flow_table *table)
{
	return (table->count > table->n_buckets);
}

struct sw_flow *ovs_flow_tbl_lookup(struct flow_table *table,
				    struct sw_flow_key *key, int len);
void ovs_flow_tbl_destroy(struct flow_table *table);
void ovs_flow_tbl_deferred_destroy(struct flow_table *table);
struct flow_table *ovs_flow_tbl_alloc(int new_size);
struct flow_table *ovs_flow_tbl_expand(struct flow_table *table);
struct flow_table *ovs_flow_tbl_rehash(struct flow_table *table);
void ovs_flow_tbl_insert(struct flow_table *table, struct sw_flow *flow);
void ovs_flow_tbl_remove(struct flow_table *table, struct sw_flow *flow);
u32 ovs_flow_hash(const struct sw_flow_key *key, int key_len);

struct sw_flow *ovs_flow_tbl_next(struct flow_table *table, u32 *bucket, u32 *idx);
extern const int ovs_key_lens[OVS_KEY_ATTR_MAX + 1];
=======
=======
>>>>>>> v3.18
void ovs_flow_stats_update(struct sw_flow *, __be16 tcp_flags,
			   struct sk_buff *);
void ovs_flow_stats_get(const struct sw_flow *, struct ovs_flow_stats *,
			unsigned long *used, __be16 *tcp_flags);
void ovs_flow_stats_clear(struct sw_flow *);
u64 ovs_flow_used_time(unsigned long flow_jiffies);

int ovs_flow_key_update(struct sk_buff *skb, struct sw_flow_key *key);
int ovs_flow_key_extract(struct ovs_tunnel_info *tun_info, struct sk_buff *skb,
			 struct sw_flow_key *key);
/* Extract key from packet coming from userspace. */
int ovs_flow_key_extract_userspace(const struct nlattr *attr,
				   struct sk_buff *skb,
				   struct sw_flow_key *key);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

#endif /* flow.h */
