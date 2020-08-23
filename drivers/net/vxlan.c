/*
 * VXLAN: Virtual eXtensible Local Area Network
 *
 * Copyright (c) 2012-2013 Vyatta Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
<<<<<<< HEAD
<<<<<<< HEAD
 *
 * TODO
 *  - IPv6 (not in RFC)
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/skbuff.h>
#include <linux/rculist.h>
#include <linux/netdevice.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/igmp.h>
#include <linux/etherdevice.h>
#include <linux/if_ether.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/if_vlan.h>
>>>>>>> v3.18
=======
#include <linux/if_vlan.h>
>>>>>>> v3.18
#include <linux/hash.h>
#include <linux/ethtool.h>
#include <net/arp.h>
#include <net/ndisc.h>
#include <net/ip.h>
#include <net/ip_tunnels.h>
#include <net/icmp.h>
#include <net/udp.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <net/udp_tunnel.h>
>>>>>>> v3.18
=======
#include <net/udp_tunnel.h>
>>>>>>> v3.18
#include <net/rtnetlink.h>
#include <net/route.h>
#include <net/dsfield.h>
#include <net/inet_ecn.h>
#include <net/net_namespace.h>
#include <net/netns/generic.h>
<<<<<<< HEAD
<<<<<<< HEAD

#define VXLAN_VERSION	"0.1"

=======
=======
>>>>>>> v3.18
#include <net/vxlan.h>
#include <net/protocol.h>
#include <net/udp_tunnel.h>
#if IS_ENABLED(CONFIG_IPV6)
#include <net/ipv6.h>
#include <net/addrconf.h>
#include <net/ip6_tunnel.h>
#include <net/ip6_checksum.h>
#endif

#define VXLAN_VERSION	"0.1"

#define PORT_HASH_BITS	8
#define PORT_HASH_SIZE  (1<<PORT_HASH_BITS)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#define VNI_HASH_BITS	10
#define VNI_HASH_SIZE	(1<<VNI_HASH_BITS)
#define FDB_HASH_BITS	8
#define FDB_HASH_SIZE	(1<<FDB_HASH_BITS)
#define FDB_AGE_DEFAULT 300 /* 5 min */
#define FDB_AGE_INTERVAL (10 * HZ)	/* rescan interval */

#define VXLAN_N_VID	(1u << 24)
#define VXLAN_VID_MASK	(VXLAN_N_VID - 1)
<<<<<<< HEAD
<<<<<<< HEAD
/* IP header + UDP + VXLAN + Ethernet header */
#define VXLAN_HEADROOM (20 + 8 + 8 + 14)

#define VXLAN_FLAGS 0x08000000	/* struct vxlanhdr.vx_flags required value. */

/* VXLAN protocol header */
struct vxlanhdr {
	__be32 vx_flags;
	__be32 vx_vni;
};

/* UDP port for VXLAN traffic.
 * The IANA assigned port is 4789, but the Linux default is 8472
 * for compatability with early adopters.
 */
static unsigned int vxlan_port __read_mostly = 8472;
module_param_named(udp_port, vxlan_port, uint, 0444);
=======
=======
>>>>>>> v3.18
#define VXLAN_HLEN (sizeof(struct udphdr) + sizeof(struct vxlanhdr))

#define VXLAN_FLAGS 0x08000000	/* struct vxlanhdr.vx_flags required value. */

/* UDP port for VXLAN traffic.
 * The IANA assigned port is 4789, but the Linux default is 8472
 * for compatibility with early adopters.
 */
static unsigned short vxlan_port __read_mostly = 8472;
module_param_named(udp_port, vxlan_port, ushort, 0444);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
MODULE_PARM_DESC(udp_port, "Destination UDP port");

static bool log_ecn_error = true;
module_param(log_ecn_error, bool, 0644);
MODULE_PARM_DESC(log_ecn_error, "Log packets received with corrupted ECN");

<<<<<<< HEAD
<<<<<<< HEAD
/* per-net private data for this module */
static unsigned int vxlan_net_id;
struct vxlan_net {
	struct socket	  *sock;	/* UDP encap socket */
	struct hlist_head vni_list[VNI_HASH_SIZE];
};

struct vxlan_rdst {
	struct rcu_head		 rcu;
	__be32			 remote_ip;
	__be16			 remote_port;
	u32			 remote_vni;
	u32			 remote_ifindex;
	struct vxlan_rdst	*remote_next;
=======
=======
>>>>>>> v3.18
static int vxlan_net_id;

static const u8 all_zeros_mac[ETH_ALEN];

/* per-network namespace private data for this module */
struct vxlan_net {
	struct list_head  vxlan_list;
	struct hlist_head sock_list[PORT_HASH_SIZE];
	spinlock_t	  sock_lock;
};

union vxlan_addr {
	struct sockaddr_in sin;
	struct sockaddr_in6 sin6;
	struct sockaddr sa;
};

struct vxlan_rdst {
	union vxlan_addr	 remote_ip;
	__be16			 remote_port;
	u32			 remote_vni;
	u32			 remote_ifindex;
	struct list_head	 list;
	struct rcu_head		 rcu;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
};

/* Forwarding table entry */
struct vxlan_fdb {
	struct hlist_node hlist;	/* linked list of entries */
	struct rcu_head	  rcu;
	unsigned long	  updated;	/* jiffies */
	unsigned long	  used;
<<<<<<< HEAD
<<<<<<< HEAD
	struct vxlan_rdst remote;
=======
	struct list_head  remotes;
>>>>>>> v3.18
=======
	struct list_head  remotes;
>>>>>>> v3.18
	u16		  state;	/* see ndm_state */
	u8		  flags;	/* see ndm_flags */
	u8		  eth_addr[ETH_ALEN];
};

/* Pseudo network device */
struct vxlan_dev {
<<<<<<< HEAD
<<<<<<< HEAD
	struct hlist_node hlist;
	struct net_device *dev;
	struct vxlan_rdst default_dst;	/* default destination */
	__be32		  saddr;	/* source address */
=======
=======
>>>>>>> v3.18
	struct hlist_node hlist;	/* vni hash table */
	struct list_head  next;		/* vxlan's per namespace list */
	struct vxlan_sock *vn_sock;	/* listening socket */
	struct net_device *dev;
	struct net	  *net;		/* netns for packet i/o */
	struct vxlan_rdst default_dst;	/* default destination */
	union vxlan_addr  saddr;	/* source address */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	__be16		  dst_port;
	__u16		  port_min;	/* source port range */
	__u16		  port_max;
	__u8		  tos;		/* TOS override */
	__u8		  ttl;
<<<<<<< HEAD
<<<<<<< HEAD
	u32		  flags;	/* VXLAN_F_* below */
=======
=======
>>>>>>> v3.18
	u32		  flags;	/* VXLAN_F_* in vxlan.h */

	struct work_struct sock_work;
	struct work_struct igmp_join;
	struct work_struct igmp_leave;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	unsigned long	  age_interval;
	struct timer_list age_timer;
	spinlock_t	  hash_lock;
	unsigned int	  addrcnt;
	unsigned int	  addrmax;

	struct hlist_head fdb_head[FDB_HASH_SIZE];
};

<<<<<<< HEAD
<<<<<<< HEAD
#define VXLAN_F_LEARN	0x01
#define VXLAN_F_PROXY	0x02
#define VXLAN_F_RSC	0x04
#define VXLAN_F_L2MISS	0x08
#define VXLAN_F_L3MISS	0x10

/* salt for hash table */
static u32 vxlan_salt __read_mostly;

static inline struct hlist_head *vni_head(struct net *net, u32 id)
{
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);

	return &vn->vni_list[hash_32(id, VNI_HASH_BITS)];
}

/* Look up VNI in a per net namespace table */
static struct vxlan_dev *vxlan_find_vni(struct net *net, u32 id)
{
	struct vxlan_dev *vxlan;

	hlist_for_each_entry_rcu(vxlan, vni_head(net, id), hlist) {
=======
=======
>>>>>>> v3.18
/* salt for hash table */
static u32 vxlan_salt __read_mostly;
static struct workqueue_struct *vxlan_wq;

static void vxlan_sock_work(struct work_struct *work);

#if IS_ENABLED(CONFIG_IPV6)
static inline
bool vxlan_addr_equal(const union vxlan_addr *a, const union vxlan_addr *b)
{
       if (a->sa.sa_family != b->sa.sa_family)
               return false;
       if (a->sa.sa_family == AF_INET6)
               return ipv6_addr_equal(&a->sin6.sin6_addr, &b->sin6.sin6_addr);
       else
               return a->sin.sin_addr.s_addr == b->sin.sin_addr.s_addr;
}

static inline bool vxlan_addr_any(const union vxlan_addr *ipa)
{
       if (ipa->sa.sa_family == AF_INET6)
               return ipv6_addr_any(&ipa->sin6.sin6_addr);
       else
               return ipa->sin.sin_addr.s_addr == htonl(INADDR_ANY);
}

static inline bool vxlan_addr_multicast(const union vxlan_addr *ipa)
{
       if (ipa->sa.sa_family == AF_INET6)
               return ipv6_addr_is_multicast(&ipa->sin6.sin6_addr);
       else
               return IN_MULTICAST(ntohl(ipa->sin.sin_addr.s_addr));
}

static int vxlan_nla_get_addr(union vxlan_addr *ip, struct nlattr *nla)
{
       if (nla_len(nla) >= sizeof(struct in6_addr)) {
               nla_memcpy(&ip->sin6.sin6_addr, nla, sizeof(struct in6_addr));
               ip->sa.sa_family = AF_INET6;
               return 0;
       } else if (nla_len(nla) >= sizeof(__be32)) {
               ip->sin.sin_addr.s_addr = nla_get_be32(nla);
               ip->sa.sa_family = AF_INET;
               return 0;
       } else {
               return -EAFNOSUPPORT;
       }
}

static int vxlan_nla_put_addr(struct sk_buff *skb, int attr,
                             const union vxlan_addr *ip)
{
       if (ip->sa.sa_family == AF_INET6)
               return nla_put(skb, attr, sizeof(struct in6_addr), &ip->sin6.sin6_addr);
       else
               return nla_put_be32(skb, attr, ip->sin.sin_addr.s_addr);
}

#else /* !CONFIG_IPV6 */

static inline
bool vxlan_addr_equal(const union vxlan_addr *a, const union vxlan_addr *b)
{
       return a->sin.sin_addr.s_addr == b->sin.sin_addr.s_addr;
}

static inline bool vxlan_addr_any(const union vxlan_addr *ipa)
{
       return ipa->sin.sin_addr.s_addr == htonl(INADDR_ANY);
}

static inline bool vxlan_addr_multicast(const union vxlan_addr *ipa)
{
       return IN_MULTICAST(ntohl(ipa->sin.sin_addr.s_addr));
}

static int vxlan_nla_get_addr(union vxlan_addr *ip, struct nlattr *nla)
{
       if (nla_len(nla) >= sizeof(struct in6_addr)) {
               return -EAFNOSUPPORT;
       } else if (nla_len(nla) >= sizeof(__be32)) {
               ip->sin.sin_addr.s_addr = nla_get_be32(nla);
               ip->sa.sa_family = AF_INET;
               return 0;
       } else {
               return -EAFNOSUPPORT;
       }
}

static int vxlan_nla_put_addr(struct sk_buff *skb, int attr,
                             const union vxlan_addr *ip)
{
       return nla_put_be32(skb, attr, ip->sin.sin_addr.s_addr);
}
#endif

/* Virtual Network hash table head */
static inline struct hlist_head *vni_head(struct vxlan_sock *vs, u32 id)
{
	return &vs->vni_list[hash_32(id, VNI_HASH_BITS)];
}

/* Socket hash table head */
static inline struct hlist_head *vs_head(struct net *net, __be16 port)
{
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);

	return &vn->sock_list[hash_32(ntohs(port), PORT_HASH_BITS)];
}

/* First remote destination for a forwarding entry.
 * Guaranteed to be non-NULL because remotes are never deleted.
 */
static inline struct vxlan_rdst *first_remote_rcu(struct vxlan_fdb *fdb)
{
	return list_entry_rcu(fdb->remotes.next, struct vxlan_rdst, list);
}

static inline struct vxlan_rdst *first_remote_rtnl(struct vxlan_fdb *fdb)
{
	return list_first_entry(&fdb->remotes, struct vxlan_rdst, list);
}

/* Find VXLAN socket based on network namespace, address family and UDP port */
static struct vxlan_sock *vxlan_find_sock(struct net *net,
					  sa_family_t family, __be16 port)
{
	struct vxlan_sock *vs;

	hlist_for_each_entry_rcu(vs, vs_head(net, port), hlist) {
		if (inet_sk(vs->sock->sk)->inet_sport == port &&
		    inet_sk(vs->sock->sk)->sk.sk_family == family)
			return vs;
	}
	return NULL;
}

static struct vxlan_dev *vxlan_vs_find_vni(struct vxlan_sock *vs, u32 id)
{
	struct vxlan_dev *vxlan;

	hlist_for_each_entry_rcu(vxlan, vni_head(vs, id), hlist) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		if (vxlan->default_dst.remote_vni == id)
			return vxlan;
	}

	return NULL;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* Fill in neighbour message in skbuff. */
static int vxlan_fdb_info(struct sk_buff *skb, struct vxlan_dev *vxlan,
			   const struct vxlan_fdb *fdb,
			   u32 portid, u32 seq, int type, unsigned int flags,
			   const struct vxlan_rdst *rdst)
=======
=======
>>>>>>> v3.18
/* Look up VNI in a per net namespace table */
static struct vxlan_dev *vxlan_find_vni(struct net *net, u32 id,
					sa_family_t family, __be16 port)
{
	struct vxlan_sock *vs;

	vs = vxlan_find_sock(net, family, port);
	if (!vs)
		return NULL;

	return vxlan_vs_find_vni(vs, id);
}

/* Fill in neighbour message in skbuff. */
static int vxlan_fdb_info(struct sk_buff *skb, struct vxlan_dev *vxlan,
			  const struct vxlan_fdb *fdb,
			  u32 portid, u32 seq, int type, unsigned int flags,
			  const struct vxlan_rdst *rdst)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	unsigned long now = jiffies;
	struct nda_cacheinfo ci;
	struct nlmsghdr *nlh;
	struct ndmsg *ndm;
	bool send_ip, send_eth;

	nlh = nlmsg_put(skb, portid, seq, type, sizeof(*ndm), flags);
	if (nlh == NULL)
		return -EMSGSIZE;

	ndm = nlmsg_data(nlh);
	memset(ndm, 0, sizeof(*ndm));

	send_eth = send_ip = true;

	if (type == RTM_GETNEIGH) {
		ndm->ndm_family	= AF_INET;
<<<<<<< HEAD
<<<<<<< HEAD
		send_ip = rdst->remote_ip != htonl(INADDR_ANY);
=======
		send_ip = !vxlan_addr_any(&rdst->remote_ip);
>>>>>>> v3.18
=======
		send_ip = !vxlan_addr_any(&rdst->remote_ip);
>>>>>>> v3.18
		send_eth = !is_zero_ether_addr(fdb->eth_addr);
	} else
		ndm->ndm_family	= AF_BRIDGE;
	ndm->ndm_state = fdb->state;
	ndm->ndm_ifindex = vxlan->dev->ifindex;
	ndm->ndm_flags = fdb->flags;
<<<<<<< HEAD
<<<<<<< HEAD
	ndm->ndm_type = NDA_DST;
=======
	ndm->ndm_type = RTN_UNICAST;
>>>>>>> v3.18
=======
	ndm->ndm_type = RTN_UNICAST;
>>>>>>> v3.18

	if (send_eth && nla_put(skb, NDA_LLADDR, ETH_ALEN, &fdb->eth_addr))
		goto nla_put_failure;

<<<<<<< HEAD
<<<<<<< HEAD
	if (send_ip && nla_put_be32(skb, NDA_DST, rdst->remote_ip))
=======
	if (send_ip && vxlan_nla_put_addr(skb, NDA_DST, &rdst->remote_ip))
>>>>>>> v3.18
=======
	if (send_ip && vxlan_nla_put_addr(skb, NDA_DST, &rdst->remote_ip))
>>>>>>> v3.18
		goto nla_put_failure;

	if (rdst->remote_port && rdst->remote_port != vxlan->dst_port &&
	    nla_put_be16(skb, NDA_PORT, rdst->remote_port))
		goto nla_put_failure;
	if (rdst->remote_vni != vxlan->default_dst.remote_vni &&
<<<<<<< HEAD
<<<<<<< HEAD
	    nla_put_be32(skb, NDA_VNI, rdst->remote_vni))
=======
	    nla_put_u32(skb, NDA_VNI, rdst->remote_vni))
>>>>>>> v3.18
=======
	    nla_put_u32(skb, NDA_VNI, rdst->remote_vni))
>>>>>>> v3.18
		goto nla_put_failure;
	if (rdst->remote_ifindex &&
	    nla_put_u32(skb, NDA_IFINDEX, rdst->remote_ifindex))
		goto nla_put_failure;

	ci.ndm_used	 = jiffies_to_clock_t(now - fdb->used);
	ci.ndm_confirmed = 0;
	ci.ndm_updated	 = jiffies_to_clock_t(now - fdb->updated);
	ci.ndm_refcnt	 = 0;

	if (nla_put(skb, NDA_CACHEINFO, sizeof(ci), &ci))
		goto nla_put_failure;

	return nlmsg_end(skb, nlh);

nla_put_failure:
	nlmsg_cancel(skb, nlh);
	return -EMSGSIZE;
}

static inline size_t vxlan_nlmsg_size(void)
{
	return NLMSG_ALIGN(sizeof(struct ndmsg))
		+ nla_total_size(ETH_ALEN) /* NDA_LLADDR */
<<<<<<< HEAD
<<<<<<< HEAD
		+ nla_total_size(sizeof(__be32)) /* NDA_DST */
=======
		+ nla_total_size(sizeof(struct in6_addr)) /* NDA_DST */
>>>>>>> v3.18
=======
		+ nla_total_size(sizeof(struct in6_addr)) /* NDA_DST */
>>>>>>> v3.18
		+ nla_total_size(sizeof(__be16)) /* NDA_PORT */
		+ nla_total_size(sizeof(__be32)) /* NDA_VNI */
		+ nla_total_size(sizeof(__u32)) /* NDA_IFINDEX */
		+ nla_total_size(sizeof(struct nda_cacheinfo));
}

<<<<<<< HEAD
<<<<<<< HEAD
static void vxlan_fdb_notify(struct vxlan_dev *vxlan,
			     const struct vxlan_fdb *fdb, int type)
=======
static void vxlan_fdb_notify(struct vxlan_dev *vxlan, struct vxlan_fdb *fdb,
			     struct vxlan_rdst *rd, int type)
>>>>>>> v3.18
=======
static void vxlan_fdb_notify(struct vxlan_dev *vxlan, struct vxlan_fdb *fdb,
			     struct vxlan_rdst *rd, int type)
>>>>>>> v3.18
{
	struct net *net = dev_net(vxlan->dev);
	struct sk_buff *skb;
	int err = -ENOBUFS;

	skb = nlmsg_new(vxlan_nlmsg_size(), GFP_ATOMIC);
	if (skb == NULL)
		goto errout;

<<<<<<< HEAD
<<<<<<< HEAD
	err = vxlan_fdb_info(skb, vxlan, fdb, 0, 0, type, 0, &fdb->remote);
=======
	err = vxlan_fdb_info(skb, vxlan, fdb, 0, 0, type, 0, rd);
>>>>>>> v3.18
=======
	err = vxlan_fdb_info(skb, vxlan, fdb, 0, 0, type, 0, rd);
>>>>>>> v3.18
	if (err < 0) {
		/* -EMSGSIZE implies BUG in vxlan_nlmsg_size() */
		WARN_ON(err == -EMSGSIZE);
		kfree_skb(skb);
		goto errout;
	}

	rtnl_notify(skb, net, 0, RTNLGRP_NEIGH, NULL, GFP_ATOMIC);
	return;
errout:
	if (err < 0)
		rtnl_set_sk_err(net, RTNLGRP_NEIGH, err);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void vxlan_ip_miss(struct net_device *dev, __be32 ipa)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct vxlan_fdb f;

	memset(&f, 0, sizeof f);
	f.state = NUD_STALE;
	f.remote.remote_ip = ipa; /* goes to NDA_DST */
	f.remote.remote_vni = VXLAN_N_VID;

	vxlan_fdb_notify(vxlan, &f, RTM_GETNEIGH);
=======
=======
>>>>>>> v3.18
static void vxlan_ip_miss(struct net_device *dev, union vxlan_addr *ipa)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct vxlan_fdb f = {
		.state = NUD_STALE,
	};
	struct vxlan_rdst remote = {
		.remote_ip = *ipa, /* goes to NDA_DST */
		.remote_vni = VXLAN_N_VID,
	};

	vxlan_fdb_notify(vxlan, &f, &remote, RTM_GETNEIGH);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static void vxlan_fdb_miss(struct vxlan_dev *vxlan, const u8 eth_addr[ETH_ALEN])
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct vxlan_fdb	f;

	memset(&f, 0, sizeof f);
	f.state = NUD_STALE;
	memcpy(f.eth_addr, eth_addr, ETH_ALEN);

	vxlan_fdb_notify(vxlan, &f, RTM_GETNEIGH);
=======
=======
>>>>>>> v3.18
	struct vxlan_fdb f = {
		.state = NUD_STALE,
	};
	struct vxlan_rdst remote = { };

	memcpy(f.eth_addr, eth_addr, ETH_ALEN);

	vxlan_fdb_notify(vxlan, &f, &remote, RTM_GETNEIGH);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/* Hash Ethernet address */
static u32 eth_hash(const unsigned char *addr)
{
	u64 value = get_unaligned((u64 *)addr);

	/* only want 6 bytes */
#ifdef __BIG_ENDIAN
	value >>= 16;
#else
	value <<= 16;
#endif
	return hash_64(value, FDB_HASH_BITS);
}

/* Hash chain to use given mac address */
static inline struct hlist_head *vxlan_fdb_head(struct vxlan_dev *vxlan,
						const u8 *mac)
{
	return &vxlan->fdb_head[eth_hash(mac)];
}

/* Look up Ethernet address in forwarding table */
static struct vxlan_fdb *__vxlan_find_mac(struct vxlan_dev *vxlan,
					const u8 *mac)
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	struct hlist_head *head = vxlan_fdb_head(vxlan, mac);
	struct vxlan_fdb *f;

	hlist_for_each_entry_rcu(f, head, hlist) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (compare_ether_addr(mac, f->eth_addr) == 0)
=======
		if (ether_addr_equal(mac, f->eth_addr))
>>>>>>> v3.18
=======
		if (ether_addr_equal(mac, f->eth_addr))
>>>>>>> v3.18
			return f;
	}

	return NULL;
}

static struct vxlan_fdb *vxlan_find_mac(struct vxlan_dev *vxlan,
					const u8 *mac)
{
	struct vxlan_fdb *f;

	f = __vxlan_find_mac(vxlan, mac);
	if (f)
		f->used = jiffies;

	return f;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* Add/update destinations for multicast */
static int vxlan_fdb_append(struct vxlan_fdb *f,
			    __be32 ip, __be16 port, __u32 vni, __u32 ifindex)
{
	struct vxlan_rdst *rd_prev, *rd;

	rd_prev = NULL;
	for (rd = &f->remote; rd; rd = rd->remote_next) {
		if (rd->remote_ip == ip &&
		    rd->remote_port == port &&
		    rd->remote_vni == vni &&
		    rd->remote_ifindex == ifindex)
			return 0;
		rd_prev = rd;
	}
	rd = kmalloc(sizeof(*rd), GFP_ATOMIC);
	if (rd == NULL)
		return -ENOBUFS;
	rd->remote_ip = ip;
	rd->remote_port = port;
	rd->remote_vni = vni;
	rd->remote_ifindex = ifindex;
	rd->remote_next = NULL;
	rd_prev->remote_next = rd;
	return 1;
}

/* Add new entry to forwarding table -- assumes lock held */
static int vxlan_fdb_create(struct vxlan_dev *vxlan,
			    const u8 *mac, __be32 ip,
=======
=======
>>>>>>> v3.18
/* caller should hold vxlan->hash_lock */
static struct vxlan_rdst *vxlan_fdb_find_rdst(struct vxlan_fdb *f,
					      union vxlan_addr *ip, __be16 port,
					      __u32 vni, __u32 ifindex)
{
	struct vxlan_rdst *rd;

	list_for_each_entry(rd, &f->remotes, list) {
		if (vxlan_addr_equal(&rd->remote_ip, ip) &&
		    rd->remote_port == port &&
		    rd->remote_vni == vni &&
		    rd->remote_ifindex == ifindex)
			return rd;
	}

	return NULL;
}

/* Replace destination of unicast mac */
static int vxlan_fdb_replace(struct vxlan_fdb *f,
			     union vxlan_addr *ip, __be16 port, __u32 vni, __u32 ifindex)
{
	struct vxlan_rdst *rd;

	rd = vxlan_fdb_find_rdst(f, ip, port, vni, ifindex);
	if (rd)
		return 0;

	rd = list_first_entry_or_null(&f->remotes, struct vxlan_rdst, list);
	if (!rd)
		return 0;
	rd->remote_ip = *ip;
	rd->remote_port = port;
	rd->remote_vni = vni;
	rd->remote_ifindex = ifindex;
	return 1;
}

/* Add/update destinations for multicast */
static int vxlan_fdb_append(struct vxlan_fdb *f,
			    union vxlan_addr *ip, __be16 port, __u32 vni,
			    __u32 ifindex, struct vxlan_rdst **rdp)
{
	struct vxlan_rdst *rd;

	rd = vxlan_fdb_find_rdst(f, ip, port, vni, ifindex);
	if (rd)
		return 0;

	rd = kmalloc(sizeof(*rd), GFP_ATOMIC);
	if (rd == NULL)
		return -ENOBUFS;
	rd->remote_ip = *ip;
	rd->remote_port = port;
	rd->remote_vni = vni;
	rd->remote_ifindex = ifindex;

	list_add_tail_rcu(&rd->list, &f->remotes);

	*rdp = rd;
	return 1;
}

static struct sk_buff **vxlan_gro_receive(struct sk_buff **head, struct sk_buff *skb)
{
	struct sk_buff *p, **pp = NULL;
	struct vxlanhdr *vh, *vh2;
	struct ethhdr *eh, *eh2;
	unsigned int hlen, off_vx, off_eth;
	const struct packet_offload *ptype;
	__be16 type;
	int flush = 1;

	off_vx = skb_gro_offset(skb);
	hlen = off_vx + sizeof(*vh);
	vh   = skb_gro_header_fast(skb, off_vx);
	if (skb_gro_header_hard(skb, hlen)) {
		vh = skb_gro_header_slow(skb, hlen, off_vx);
		if (unlikely(!vh))
			goto out;
	}
	skb_gro_pull(skb, sizeof(struct vxlanhdr)); /* pull vxlan header */
	skb_gro_postpull_rcsum(skb, vh, sizeof(struct vxlanhdr));

	off_eth = skb_gro_offset(skb);
	hlen = off_eth + sizeof(*eh);
	eh   = skb_gro_header_fast(skb, off_eth);
	if (skb_gro_header_hard(skb, hlen)) {
		eh = skb_gro_header_slow(skb, hlen, off_eth);
		if (unlikely(!eh))
			goto out;
	}

	flush = 0;

	for (p = *head; p; p = p->next) {
		if (!NAPI_GRO_CB(p)->same_flow)
			continue;

		vh2 = (struct vxlanhdr *)(p->data + off_vx);
		eh2 = (struct ethhdr   *)(p->data + off_eth);
		if (vh->vx_vni != vh2->vx_vni || compare_ether_header(eh, eh2)) {
			NAPI_GRO_CB(p)->same_flow = 0;
			continue;
		}
	}

	type = eh->h_proto;

	rcu_read_lock();
	ptype = gro_find_receive_by_type(type);
	if (ptype == NULL) {
		flush = 1;
		goto out_unlock;
	}

	skb_gro_pull(skb, sizeof(*eh)); /* pull inner eth header */
	skb_gro_postpull_rcsum(skb, eh, sizeof(*eh));
	pp = ptype->callbacks.gro_receive(head, skb);

out_unlock:
	rcu_read_unlock();
out:
	NAPI_GRO_CB(skb)->flush |= flush;

	return pp;
}

static int vxlan_gro_complete(struct sk_buff *skb, int nhoff)
{
	struct ethhdr *eh;
	struct packet_offload *ptype;
	__be16 type;
	int vxlan_len  = sizeof(struct vxlanhdr) + sizeof(struct ethhdr);
	int err = -ENOSYS;

	udp_tunnel_gro_complete(skb, nhoff);

	eh = (struct ethhdr *)(skb->data + nhoff + sizeof(struct vxlanhdr));
	type = eh->h_proto;

	rcu_read_lock();
	ptype = gro_find_complete_by_type(type);
	if (ptype != NULL)
		err = ptype->callbacks.gro_complete(skb, nhoff + vxlan_len);

	rcu_read_unlock();
	return err;
}

/* Notify netdevs that UDP port started listening */
static void vxlan_notify_add_rx_port(struct vxlan_sock *vs)
{
	struct net_device *dev;
	struct sock *sk = vs->sock->sk;
	struct net *net = sock_net(sk);
	sa_family_t sa_family = sk->sk_family;
	__be16 port = inet_sk(sk)->inet_sport;
	int err;

	if (sa_family == AF_INET) {
		err = udp_add_offload(&vs->udp_offloads);
		if (err)
			pr_warn("vxlan: udp_add_offload failed with status %d\n", err);
	}

	rcu_read_lock();
	for_each_netdev_rcu(net, dev) {
		if (dev->netdev_ops->ndo_add_vxlan_port)
			dev->netdev_ops->ndo_add_vxlan_port(dev, sa_family,
							    port);
	}
	rcu_read_unlock();
}

/* Notify netdevs that UDP port is no more listening */
static void vxlan_notify_del_rx_port(struct vxlan_sock *vs)
{
	struct net_device *dev;
	struct sock *sk = vs->sock->sk;
	struct net *net = sock_net(sk);
	sa_family_t sa_family = sk->sk_family;
	__be16 port = inet_sk(sk)->inet_sport;

	rcu_read_lock();
	for_each_netdev_rcu(net, dev) {
		if (dev->netdev_ops->ndo_del_vxlan_port)
			dev->netdev_ops->ndo_del_vxlan_port(dev, sa_family,
							    port);
	}
	rcu_read_unlock();

	if (sa_family == AF_INET)
		udp_del_offload(&vs->udp_offloads);
}

/* Add new entry to forwarding table -- assumes lock held */
static int vxlan_fdb_create(struct vxlan_dev *vxlan,
			    const u8 *mac, union vxlan_addr *ip,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			    __u16 state, __u16 flags,
			    __be16 port, __u32 vni, __u32 ifindex,
			    __u8 ndm_flags)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct vxlan_rdst *rd = NULL;
>>>>>>> v3.18
=======
	struct vxlan_rdst *rd = NULL;
>>>>>>> v3.18
	struct vxlan_fdb *f;
	int notify = 0;

	f = __vxlan_find_mac(vxlan, mac);
	if (f) {
		if (flags & NLM_F_EXCL) {
			netdev_dbg(vxlan->dev,
				   "lost race to create %pM\n", mac);
			return -EEXIST;
		}
		if (f->state != state) {
			f->state = state;
			f->updated = jiffies;
			notify = 1;
		}
		if (f->flags != ndm_flags) {
			f->flags = ndm_flags;
			f->updated = jiffies;
			notify = 1;
		}
<<<<<<< HEAD
<<<<<<< HEAD
		if ((flags & NLM_F_APPEND) &&
		    is_multicast_ether_addr(f->eth_addr)) {
			int rc = vxlan_fdb_append(f, ip, port, vni, ifindex);
=======
=======
>>>>>>> v3.18
		if ((flags & NLM_F_REPLACE)) {
			/* Only change unicasts */
			if (!(is_multicast_ether_addr(f->eth_addr) ||
			     is_zero_ether_addr(f->eth_addr))) {
				int rc = vxlan_fdb_replace(f, ip, port, vni,
							   ifindex);

				if (rc < 0)
					return rc;
				notify |= rc;
			} else
				return -EOPNOTSUPP;
		}
		if ((flags & NLM_F_APPEND) &&
		    (is_multicast_ether_addr(f->eth_addr) ||
		     is_zero_ether_addr(f->eth_addr))) {
			int rc = vxlan_fdb_append(f, ip, port, vni, ifindex,
						  &rd);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

			if (rc < 0)
				return rc;
			notify |= rc;
		}
	} else {
		if (!(flags & NLM_F_CREATE))
			return -ENOENT;

		if (vxlan->addrmax && vxlan->addrcnt >= vxlan->addrmax)
			return -ENOSPC;

<<<<<<< HEAD
<<<<<<< HEAD
		netdev_dbg(vxlan->dev, "add %pM -> %pI4\n", mac, &ip);
=======
=======
>>>>>>> v3.18
		/* Disallow replace to add a multicast entry */
		if ((flags & NLM_F_REPLACE) &&
		    (is_multicast_ether_addr(mac) || is_zero_ether_addr(mac)))
			return -EOPNOTSUPP;

		netdev_dbg(vxlan->dev, "add %pM -> %pIS\n", mac, ip);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		f = kmalloc(sizeof(*f), GFP_ATOMIC);
		if (!f)
			return -ENOMEM;

		notify = 1;
<<<<<<< HEAD
<<<<<<< HEAD
		f->remote.remote_ip = ip;
		f->remote.remote_port = port;
		f->remote.remote_vni = vni;
		f->remote.remote_ifindex = ifindex;
		f->remote.remote_next = NULL;
		f->state = state;
		f->flags = ndm_flags;
		f->updated = f->used = jiffies;
		memcpy(f->eth_addr, mac, ETH_ALEN);

=======
=======
>>>>>>> v3.18
		f->state = state;
		f->flags = ndm_flags;
		f->updated = f->used = jiffies;
		INIT_LIST_HEAD(&f->remotes);
		memcpy(f->eth_addr, mac, ETH_ALEN);

		vxlan_fdb_append(f, ip, port, vni, ifindex, &rd);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		++vxlan->addrcnt;
		hlist_add_head_rcu(&f->hlist,
				   vxlan_fdb_head(vxlan, mac));
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (notify)
		vxlan_fdb_notify(vxlan, f, RTM_NEWNEIGH);
=======
=======
>>>>>>> v3.18
	if (notify) {
		if (rd == NULL)
			rd = first_remote_rtnl(f);
		vxlan_fdb_notify(vxlan, f, rd, RTM_NEWNEIGH);
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return 0;
}

static void vxlan_fdb_free(struct rcu_head *head)
{
	struct vxlan_fdb *f = container_of(head, struct vxlan_fdb, rcu);
<<<<<<< HEAD
<<<<<<< HEAD

	while (f->remote.remote_next) {
		struct vxlan_rdst *rd = f->remote.remote_next;

		f->remote.remote_next = rd->remote_next;
		kfree(rd);
	}
=======
=======
>>>>>>> v3.18
	struct vxlan_rdst *rd, *nd;

	list_for_each_entry_safe(rd, nd, &f->remotes, list)
		kfree(rd);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	kfree(f);
}

static void vxlan_fdb_destroy(struct vxlan_dev *vxlan, struct vxlan_fdb *f)
{
	netdev_dbg(vxlan->dev,
		    "delete %pM\n", f->eth_addr);

	--vxlan->addrcnt;
<<<<<<< HEAD
<<<<<<< HEAD
	vxlan_fdb_notify(vxlan, f, RTM_DELNEIGH);
=======
	vxlan_fdb_notify(vxlan, f, first_remote_rtnl(f), RTM_DELNEIGH);
>>>>>>> v3.18
=======
	vxlan_fdb_notify(vxlan, f, first_remote_rtnl(f), RTM_DELNEIGH);
>>>>>>> v3.18

	hlist_del_rcu(&f->hlist);
	call_rcu(&f->rcu, vxlan_fdb_free);
}

<<<<<<< HEAD
<<<<<<< HEAD
/* Add static entry (via netlink) */
static int vxlan_fdb_add(struct ndmsg *ndm, struct nlattr *tb[],
			 struct net_device *dev,
			 const unsigned char *addr, u16 flags)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct net *net = dev_net(vxlan->dev);
	__be32 ip;
	__be16 port;
	u32 vni, ifindex;
	int err;

	if (!(ndm->ndm_state & (NUD_PERMANENT|NUD_REACHABLE))) {
		pr_info("RTM_NEWNEIGH with invalid state %#x\n",
			ndm->ndm_state);
		return -EINVAL;
	}

	if (tb[NDA_DST] == NULL)
		return -EINVAL;

	if (nla_len(tb[NDA_DST]) != sizeof(__be32))
		return -EAFNOSUPPORT;

	ip = nla_get_be32(tb[NDA_DST]);

	if (tb[NDA_PORT]) {
		if (nla_len(tb[NDA_PORT]) != sizeof(__be16))
			return -EINVAL;
		port = nla_get_be16(tb[NDA_PORT]);
	} else
		port = vxlan->dst_port;
=======
=======
>>>>>>> v3.18
static int vxlan_fdb_parse(struct nlattr *tb[], struct vxlan_dev *vxlan,
			   union vxlan_addr *ip, __be16 *port, u32 *vni, u32 *ifindex)
{
	struct net *net = dev_net(vxlan->dev);
	int err;

	if (tb[NDA_DST]) {
		err = vxlan_nla_get_addr(ip, tb[NDA_DST]);
		if (err)
			return err;
	} else {
		union vxlan_addr *remote = &vxlan->default_dst.remote_ip;
		if (remote->sa.sa_family == AF_INET) {
			ip->sin.sin_addr.s_addr = htonl(INADDR_ANY);
			ip->sa.sa_family = AF_INET;
#if IS_ENABLED(CONFIG_IPV6)
		} else {
			ip->sin6.sin6_addr = in6addr_any;
			ip->sa.sa_family = AF_INET6;
#endif
		}
	}

	if (tb[NDA_PORT]) {
		if (nla_len(tb[NDA_PORT]) != sizeof(__be16))
			return -EINVAL;
		*port = nla_get_be16(tb[NDA_PORT]);
	} else {
		*port = vxlan->dst_port;
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (tb[NDA_VNI]) {
		if (nla_len(tb[NDA_VNI]) != sizeof(u32))
			return -EINVAL;
<<<<<<< HEAD
<<<<<<< HEAD
		vni = nla_get_u32(tb[NDA_VNI]);
	} else
		vni = vxlan->default_dst.remote_vni;
=======
=======
>>>>>>> v3.18
		*vni = nla_get_u32(tb[NDA_VNI]);
	} else {
		*vni = vxlan->default_dst.remote_vni;
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (tb[NDA_IFINDEX]) {
		struct net_device *tdev;

		if (nla_len(tb[NDA_IFINDEX]) != sizeof(u32))
			return -EINVAL;
<<<<<<< HEAD
<<<<<<< HEAD
		ifindex = nla_get_u32(tb[NDA_IFINDEX]);
		tdev = dev_get_by_index(net, ifindex);
		if (!tdev)
			return -EADDRNOTAVAIL;
		dev_put(tdev);
	} else
		ifindex = 0;

	spin_lock_bh(&vxlan->hash_lock);
	err = vxlan_fdb_create(vxlan, addr, ip, ndm->ndm_state, flags,
=======
=======
>>>>>>> v3.18
		*ifindex = nla_get_u32(tb[NDA_IFINDEX]);
		tdev = __dev_get_by_index(net, *ifindex);
		if (!tdev)
			return -EADDRNOTAVAIL;
	} else {
		*ifindex = 0;
	}

	return 0;
}

/* Add static entry (via netlink) */
static int vxlan_fdb_add(struct ndmsg *ndm, struct nlattr *tb[],
			 struct net_device *dev,
			 const unsigned char *addr, u16 flags)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	/* struct net *net = dev_net(vxlan->dev); */
	union vxlan_addr ip;
	__be16 port;
	u32 vni, ifindex;
	int err;

	if (!(ndm->ndm_state & (NUD_PERMANENT|NUD_REACHABLE))) {
		pr_info("RTM_NEWNEIGH with invalid state %#x\n",
			ndm->ndm_state);
		return -EINVAL;
	}

	if (tb[NDA_DST] == NULL)
		return -EINVAL;

	err = vxlan_fdb_parse(tb, vxlan, &ip, &port, &vni, &ifindex);
	if (err)
		return err;

	if (vxlan->default_dst.remote_ip.sa.sa_family != ip.sa.sa_family)
		return -EAFNOSUPPORT;

	spin_lock_bh(&vxlan->hash_lock);
	err = vxlan_fdb_create(vxlan, addr, &ip, ndm->ndm_state, flags,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			       port, vni, ifindex, ndm->ndm_flags);
	spin_unlock_bh(&vxlan->hash_lock);

	return err;
}

/* Delete entry (via netlink) */
static int vxlan_fdb_delete(struct ndmsg *ndm, struct nlattr *tb[],
			    struct net_device *dev,
			    const unsigned char *addr)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct vxlan_fdb *f;
<<<<<<< HEAD
<<<<<<< HEAD
	int err = -ENOENT;

	spin_lock_bh(&vxlan->hash_lock);
	f = vxlan_find_mac(vxlan, addr);
	if (f) {
		vxlan_fdb_destroy(vxlan, f);
		err = 0;
	}
=======
=======
>>>>>>> v3.18
	struct vxlan_rdst *rd = NULL;
	union vxlan_addr ip;
	__be16 port;
	u32 vni, ifindex;
	int err;

	err = vxlan_fdb_parse(tb, vxlan, &ip, &port, &vni, &ifindex);
	if (err)
		return err;

	err = -ENOENT;

	spin_lock_bh(&vxlan->hash_lock);
	f = vxlan_find_mac(vxlan, addr);
	if (!f)
		goto out;

	if (!vxlan_addr_any(&ip)) {
		rd = vxlan_fdb_find_rdst(f, &ip, port, vni, ifindex);
		if (!rd)
			goto out;
	}

	err = 0;

	/* remove a destination if it's not the only one on the list,
	 * otherwise destroy the fdb entry
	 */
	if (rd && !list_is_singular(&f->remotes)) {
		list_del_rcu(&rd->list);
		vxlan_fdb_notify(vxlan, f, rd, RTM_DELNEIGH);
		kfree_rcu(rd, rcu);
		goto out;
	}

	vxlan_fdb_destroy(vxlan, f);

out:
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	spin_unlock_bh(&vxlan->hash_lock);

	return err;
}

/* Dump forwarding table */
static int vxlan_fdb_dump(struct sk_buff *skb, struct netlink_callback *cb,
<<<<<<< HEAD
<<<<<<< HEAD
			  struct net_device *dev, int idx)
=======
			  struct net_device *dev,
			  struct net_device *filter_dev, int idx)
>>>>>>> v3.18
=======
			  struct net_device *dev,
			  struct net_device *filter_dev, int idx)
>>>>>>> v3.18
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	unsigned int h;

	for (h = 0; h < FDB_HASH_SIZE; ++h) {
		struct vxlan_fdb *f;
		int err;

		hlist_for_each_entry_rcu(f, &vxlan->fdb_head[h], hlist) {
			struct vxlan_rdst *rd;
<<<<<<< HEAD
<<<<<<< HEAD
			for (rd = &f->remote; rd; rd = rd->remote_next) {
				if (idx < cb->args[0])
					goto skip;

=======
=======
>>>>>>> v3.18

			if (idx < cb->args[0])
				goto skip;

			list_for_each_entry_rcu(rd, &f->remotes, list) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
				err = vxlan_fdb_info(skb, vxlan, f,
						     NETLINK_CB(cb->skb).portid,
						     cb->nlh->nlmsg_seq,
						     RTM_NEWNEIGH,
						     NLM_F_MULTI, rd);
				if (err < 0)
<<<<<<< HEAD
<<<<<<< HEAD
					break;
skip:
				++idx;
			}
		}
	}

=======
=======
>>>>>>> v3.18
					goto out;
			}
skip:
			++idx;
		}
	}
out:
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return idx;
}

/* Watch incoming packets to learn mapping between Ethernet address
 * and Tunnel endpoint.
 * Return true if packet is bogus and should be droppped.
 */
static bool vxlan_snoop(struct net_device *dev,
<<<<<<< HEAD
<<<<<<< HEAD
			__be32 src_ip, const u8 *src_mac)
=======
			union vxlan_addr *src_ip, const u8 *src_mac)
>>>>>>> v3.18
=======
			union vxlan_addr *src_ip, const u8 *src_mac)
>>>>>>> v3.18
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct vxlan_fdb *f;

	f = vxlan_find_mac(vxlan, src_mac);
	if (likely(f)) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (likely(f->remote.remote_ip == src_ip))
=======
		struct vxlan_rdst *rdst = first_remote_rcu(f);

		if (likely(vxlan_addr_equal(&rdst->remote_ip, src_ip)))
>>>>>>> v3.18
=======
		struct vxlan_rdst *rdst = first_remote_rcu(f);

		if (likely(vxlan_addr_equal(&rdst->remote_ip, src_ip)))
>>>>>>> v3.18
			return false;

		/* Don't migrate static entries, drop packets */
		if (f->state & NUD_NOARP)
			return true;

		if (net_ratelimit())
			netdev_info(dev,
<<<<<<< HEAD
<<<<<<< HEAD
				    "%pM migrated from %pI4 to %pI4\n",
				    src_mac, &f->remote.remote_ip, &src_ip);

		f->remote.remote_ip = src_ip;
		f->updated = jiffies;
=======
=======
>>>>>>> v3.18
				    "%pM migrated from %pIS to %pIS\n",
				    src_mac, &rdst->remote_ip, &src_ip);

		rdst->remote_ip = *src_ip;
		f->updated = jiffies;
		vxlan_fdb_notify(vxlan, f, rdst, RTM_NEWNEIGH);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	} else {
		/* learned new entry */
		spin_lock(&vxlan->hash_lock);

		/* close off race between vxlan_flush and incoming packets */
		if (netif_running(dev))
			vxlan_fdb_create(vxlan, src_mac, src_ip,
					 NUD_REACHABLE,
					 NLM_F_EXCL|NLM_F_CREATE,
					 vxlan->dst_port,
					 vxlan->default_dst.remote_vni,
					 0, NTF_SELF);
		spin_unlock(&vxlan->hash_lock);
	}

	return false;
}

<<<<<<< HEAD
<<<<<<< HEAD

/* See if multicast group is already in use by other ID */
static bool vxlan_group_used(struct vxlan_net *vn,
			     const struct vxlan_dev *this)
{
	const struct vxlan_dev *vxlan;
	unsigned h;

	for (h = 0; h < VNI_HASH_SIZE; ++h)
		hlist_for_each_entry(vxlan, &vn->vni_list[h], hlist) {
			if (vxlan == this)
				continue;

			if (!netif_running(vxlan->dev))
				continue;

			if (vxlan->default_dst.remote_ip == this->default_dst.remote_ip)
				return true;
		}
=======
=======
>>>>>>> v3.18
/* See if multicast group is already in use by other ID */
static bool vxlan_group_used(struct vxlan_net *vn, struct vxlan_dev *dev)
{
	struct vxlan_dev *vxlan;

	/* The vxlan_sock is only used by dev, leaving group has
	 * no effect on other vxlan devices.
	 */
	if (atomic_read(&dev->vn_sock->refcnt) == 1)
		return false;

	list_for_each_entry(vxlan, &vn->vxlan_list, next) {
		if (!netif_running(vxlan->dev) || vxlan == dev)
			continue;

		if (vxlan->vn_sock != dev->vn_sock)
			continue;

		if (!vxlan_addr_equal(&vxlan->default_dst.remote_ip,
				      &dev->default_dst.remote_ip))
			continue;

		if (vxlan->default_dst.remote_ifindex !=
		    dev->default_dst.remote_ifindex)
			continue;

		return true;
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return false;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* kernel equivalent to IP_ADD_MEMBERSHIP */
static int vxlan_join_group(struct net_device *dev)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct vxlan_net *vn = net_generic(dev_net(dev), vxlan_net_id);
	struct sock *sk = vn->sock->sk;
	struct ip_mreqn mreq = {
		.imr_multiaddr.s_addr	= vxlan->default_dst.remote_ip,
		.imr_ifindex		= vxlan->default_dst.remote_ifindex,
	};
	int err;

	/* Already a member of group */
	if (vxlan_group_used(vn, vxlan))
		return 0;

	/* Need to drop RTNL to call multicast join */
	rtnl_unlock();
	lock_sock(sk);
	err = ip_mc_join_group(sk, &mreq);
	release_sock(sk);
	rtnl_lock();

	return err;
}


/* kernel equivalent to IP_DROP_MEMBERSHIP */
static int vxlan_leave_group(struct net_device *dev)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct vxlan_net *vn = net_generic(dev_net(dev), vxlan_net_id);
	int err = 0;
	struct sock *sk = vn->sock->sk;
	struct ip_mreqn mreq = {
		.imr_multiaddr.s_addr	= vxlan->default_dst.remote_ip,
		.imr_ifindex		= vxlan->default_dst.remote_ifindex,
	};

	/* Only leave group when last vxlan is done. */
	if (vxlan_group_used(vn, vxlan))
		return 0;

	/* Need to drop RTNL to call multicast leave */
	rtnl_unlock();
	lock_sock(sk);
	err = ip_mc_leave_group(sk, &mreq);
	release_sock(sk);
	rtnl_lock();

	return err;
=======
=======
>>>>>>> v3.18
static void vxlan_sock_hold(struct vxlan_sock *vs)
{
	atomic_inc(&vs->refcnt);
}

void vxlan_sock_release(struct vxlan_sock *vs)
{
	struct sock *sk = vs->sock->sk;
	struct net *net = sock_net(sk);
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);

	if (!atomic_dec_and_test(&vs->refcnt))
		return;

	spin_lock(&vn->sock_lock);
	hlist_del_rcu(&vs->hlist);
	vxlan_notify_del_rx_port(vs);
	spin_unlock(&vn->sock_lock);

	queue_work(vxlan_wq, &vs->del_work);
}
EXPORT_SYMBOL_GPL(vxlan_sock_release);

/* Callback to update multicast group membership when first VNI on
 * multicast asddress is brought up
 * Done as workqueue because ip_mc_join_group acquires RTNL.
 */
static void vxlan_igmp_join(struct work_struct *work)
{
	struct vxlan_dev *vxlan = container_of(work, struct vxlan_dev, igmp_join);
	struct vxlan_sock *vs = vxlan->vn_sock;
	struct sock *sk = vs->sock->sk;
	union vxlan_addr *ip = &vxlan->default_dst.remote_ip;
	int ifindex = vxlan->default_dst.remote_ifindex;

	lock_sock(sk);
	if (ip->sa.sa_family == AF_INET) {
		struct ip_mreqn mreq = {
			.imr_multiaddr.s_addr	= ip->sin.sin_addr.s_addr,
			.imr_ifindex		= ifindex,
		};

		ip_mc_join_group(sk, &mreq);
#if IS_ENABLED(CONFIG_IPV6)
	} else {
		ipv6_stub->ipv6_sock_mc_join(sk, ifindex,
					     &ip->sin6.sin6_addr);
#endif
	}
	release_sock(sk);

	vxlan_sock_release(vs);
	dev_put(vxlan->dev);
}

/* Inverse of vxlan_igmp_join when last VNI is brought down */
static void vxlan_igmp_leave(struct work_struct *work)
{
	struct vxlan_dev *vxlan = container_of(work, struct vxlan_dev, igmp_leave);
	struct vxlan_sock *vs = vxlan->vn_sock;
	struct sock *sk = vs->sock->sk;
	union vxlan_addr *ip = &vxlan->default_dst.remote_ip;
	int ifindex = vxlan->default_dst.remote_ifindex;

	lock_sock(sk);
	if (ip->sa.sa_family == AF_INET) {
		struct ip_mreqn mreq = {
			.imr_multiaddr.s_addr	= ip->sin.sin_addr.s_addr,
			.imr_ifindex		= ifindex,
		};

		ip_mc_leave_group(sk, &mreq);
#if IS_ENABLED(CONFIG_IPV6)
	} else {
		ipv6_stub->ipv6_sock_mc_drop(sk, ifindex,
					     &ip->sin6.sin6_addr);
#endif
	}

	release_sock(sk);

	vxlan_sock_release(vs);
	dev_put(vxlan->dev);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/* Callback from net/ipv4/udp.c to receive packets */
static int vxlan_udp_encap_recv(struct sock *sk, struct sk_buff *skb)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct iphdr *oip;
	struct vxlanhdr *vxh;
	struct vxlan_dev *vxlan;
	struct pcpu_tstats *stats;
	__u32 vni;
	int err;

	/* pop off outer UDP header */
	__skb_pull(skb, sizeof(struct udphdr));

	/* Need Vxlan and inner Ethernet header to be present */
	if (!pskb_may_pull(skb, sizeof(struct vxlanhdr)))
		goto error;

	/* Drop packets with reserved bits set */
	vxh = (struct vxlanhdr *) skb->data;
=======
=======
>>>>>>> v3.18
	struct vxlan_sock *vs;
	struct vxlanhdr *vxh;

	/* Need Vxlan and inner Ethernet header to be present */
	if (!pskb_may_pull(skb, VXLAN_HLEN))
		goto error;

	/* Return packets with reserved bits set */
	vxh = (struct vxlanhdr *)(udp_hdr(skb) + 1);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (vxh->vx_flags != htonl(VXLAN_FLAGS) ||
	    (vxh->vx_vni & htonl(0xff))) {
		netdev_dbg(skb->dev, "invalid vxlan flags=%#x vni=%#x\n",
			   ntohl(vxh->vx_flags), ntohl(vxh->vx_vni));
		goto error;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	__skb_pull(skb, sizeof(struct vxlanhdr));

	/* Is this VNI defined? */
	vni = ntohl(vxh->vx_vni) >> 8;
	vxlan = vxlan_find_vni(sock_net(sk), vni);
	if (!vxlan) {
		netdev_dbg(skb->dev, "unknown vni %d\n", vni);
		goto drop;
	}

	if (!pskb_may_pull(skb, ETH_HLEN)) {
		vxlan->dev->stats.rx_length_errors++;
		vxlan->dev->stats.rx_errors++;
		goto drop;
	}

	skb_reset_mac_header(skb);

	/* Re-examine inner Ethernet packet */
	oip = ip_hdr(skb);
	skb->protocol = eth_type_trans(skb, vxlan->dev);

	/* Ignore packet loops (and multicast echo) */
	if (compare_ether_addr(eth_hdr(skb)->h_source,
			       vxlan->dev->dev_addr) == 0)
		goto drop;

	if ((vxlan->flags & VXLAN_F_LEARN) &&
	    vxlan_snoop(skb->dev, oip->saddr, eth_hdr(skb)->h_source))
		goto drop;

	__skb_tunnel_rx(skb, vxlan->dev);
	skb_reset_network_header(skb);

	/* If the NIC driver gave us an encapsulated packet with
	 * CHECKSUM_UNNECESSARY and Rx checksum feature is enabled,
	 * leave the CHECKSUM_UNNECESSARY, the device checksummed it
	 * for us. Otherwise force the upper layers to verify it.
	 */
	if (skb->ip_summed != CHECKSUM_UNNECESSARY || !skb->encapsulation ||
	    !(vxlan->dev->features & NETIF_F_RXCSUM))
		skb->ip_summed = CHECKSUM_NONE;

	skb->encapsulation = 0;

	err = IP_ECN_decapsulate(oip, skb);
	if (unlikely(err)) {
		if (log_ecn_error)
			net_info_ratelimited("non-ECT from %pI4 with TOS=%#x\n",
					     &oip->saddr, oip->tos);
=======
=======
>>>>>>> v3.18
	if (iptunnel_pull_header(skb, VXLAN_HLEN, htons(ETH_P_TEB)))
		goto drop;

	vs = rcu_dereference_sk_user_data(sk);
	if (!vs)
		goto drop;

	vs->rcv(vs, skb, vxh->vx_vni);
	return 0;

drop:
	/* Consume bad packet */
	kfree_skb(skb);
	return 0;

error:
	/* Return non vxlan pkt */
	return 1;
}

static void vxlan_rcv(struct vxlan_sock *vs,
		      struct sk_buff *skb, __be32 vx_vni)
{
	struct iphdr *oip = NULL;
	struct ipv6hdr *oip6 = NULL;
	struct vxlan_dev *vxlan;
	struct pcpu_sw_netstats *stats;
	union vxlan_addr saddr;
	__u32 vni;
	int err = 0;
	union vxlan_addr *remote_ip;

	vni = ntohl(vx_vni) >> 8;
	/* Is this VNI defined? */
	vxlan = vxlan_vs_find_vni(vs, vni);
	if (!vxlan)
		goto drop;

	remote_ip = &vxlan->default_dst.remote_ip;
	skb_reset_mac_header(skb);
	skb_scrub_packet(skb, !net_eq(vxlan->net, dev_net(vxlan->dev)));
	skb->protocol = eth_type_trans(skb, vxlan->dev);
	skb_postpull_rcsum(skb, eth_hdr(skb), ETH_HLEN);

	/* Ignore packet loops (and multicast echo) */
	if (ether_addr_equal(eth_hdr(skb)->h_source, vxlan->dev->dev_addr))
		goto drop;

	/* Re-examine inner Ethernet packet */
	if (remote_ip->sa.sa_family == AF_INET) {
		oip = ip_hdr(skb);
		saddr.sin.sin_addr.s_addr = oip->saddr;
		saddr.sa.sa_family = AF_INET;
#if IS_ENABLED(CONFIG_IPV6)
	} else {
		oip6 = ipv6_hdr(skb);
		saddr.sin6.sin6_addr = oip6->saddr;
		saddr.sa.sa_family = AF_INET6;
#endif
	}

	if ((vxlan->flags & VXLAN_F_LEARN) &&
	    vxlan_snoop(skb->dev, &saddr, eth_hdr(skb)->h_source))
		goto drop;

	skb_reset_network_header(skb);

	if (oip6)
		err = IP6_ECN_decapsulate(oip6, skb);
	if (oip)
		err = IP_ECN_decapsulate(oip, skb);

	if (unlikely(err)) {
		if (log_ecn_error) {
			if (oip6)
				net_info_ratelimited("non-ECT from %pI6\n",
						     &oip6->saddr);
			if (oip)
				net_info_ratelimited("non-ECT from %pI4 with TOS=%#x\n",
						     &oip->saddr, oip->tos);
		}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		if (err > 1) {
			++vxlan->dev->stats.rx_frame_errors;
			++vxlan->dev->stats.rx_errors;
			goto drop;
		}
	}

	stats = this_cpu_ptr(vxlan->dev->tstats);
	u64_stats_update_begin(&stats->syncp);
	stats->rx_packets++;
	stats->rx_bytes += skb->len;
	u64_stats_update_end(&stats->syncp);

	netif_rx(skb);

<<<<<<< HEAD
<<<<<<< HEAD
	return 0;
error:
	/* Put UDP header back */
	__skb_push(skb, sizeof(struct udphdr));

	return 1;
drop:
	/* Consume bad packet */
	kfree_skb(skb);
	return 0;
=======
=======
>>>>>>> v3.18
	return;
drop:
	/* Consume bad packet */
	kfree_skb(skb);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static int arp_reduce(struct net_device *dev, struct sk_buff *skb)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct arphdr *parp;
	u8 *arpptr, *sha;
	__be32 sip, tip;
	struct neighbour *n;

	if (dev->flags & IFF_NOARP)
		goto out;

	if (!pskb_may_pull(skb, arp_hdr_len(dev))) {
		dev->stats.tx_dropped++;
		goto out;
	}
	parp = arp_hdr(skb);

	if ((parp->ar_hrd != htons(ARPHRD_ETHER) &&
	     parp->ar_hrd != htons(ARPHRD_IEEE802)) ||
	    parp->ar_pro != htons(ETH_P_IP) ||
	    parp->ar_op != htons(ARPOP_REQUEST) ||
	    parp->ar_hln != dev->addr_len ||
	    parp->ar_pln != 4)
		goto out;
	arpptr = (u8 *)parp + sizeof(struct arphdr);
	sha = arpptr;
	arpptr += dev->addr_len;	/* sha */
	memcpy(&sip, arpptr, sizeof(sip));
	arpptr += sizeof(sip);
	arpptr += dev->addr_len;	/* tha */
	memcpy(&tip, arpptr, sizeof(tip));

	if (ipv4_is_loopback(tip) ||
	    ipv4_is_multicast(tip))
		goto out;

	n = neigh_lookup(&arp_tbl, &tip, dev);

	if (n) {
		struct vxlan_fdb *f;
		struct sk_buff	*reply;

		if (!(n->nud_state & NUD_CONNECTED)) {
			neigh_release(n);
			goto out;
		}

		f = vxlan_find_mac(vxlan, n->ha);
<<<<<<< HEAD
<<<<<<< HEAD
		if (f && f->remote.remote_ip == htonl(INADDR_ANY)) {
=======
		if (f && vxlan_addr_any(&(first_remote_rcu(f)->remote_ip))) {
>>>>>>> v3.18
=======
		if (f && vxlan_addr_any(&(first_remote_rcu(f)->remote_ip))) {
>>>>>>> v3.18
			/* bridge-local neighbor */
			neigh_release(n);
			goto out;
		}

		reply = arp_create(ARPOP_REPLY, ETH_P_ARP, sip, dev, tip, sha,
				n->ha, sha);

		neigh_release(n);

		if (reply == NULL)
			goto out;

		skb_reset_mac_header(reply);
		__skb_pull(reply, skb_network_offset(reply));
		reply->ip_summed = CHECKSUM_UNNECESSARY;
		reply->pkt_type = PACKET_HOST;

		if (netif_rx_ni(reply) == NET_RX_DROP)
			dev->stats.rx_dropped++;
<<<<<<< HEAD
<<<<<<< HEAD
	} else if (vxlan->flags & VXLAN_F_L3MISS)
		vxlan_ip_miss(dev, tip);
=======
=======
>>>>>>> v3.18
	} else if (vxlan->flags & VXLAN_F_L3MISS) {
		union vxlan_addr ipa = {
			.sin.sin_addr.s_addr = tip,
			.sin.sin_family = AF_INET,
		};

		vxlan_ip_miss(dev, &ipa);
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
out:
	consume_skb(skb);
	return NETDEV_TX_OK;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
#if IS_ENABLED(CONFIG_IPV6)
static struct sk_buff *vxlan_na_create(struct sk_buff *request,
	struct neighbour *n, bool isrouter)
{
	struct net_device *dev = request->dev;
	struct sk_buff *reply;
	struct nd_msg *ns, *na;
	struct ipv6hdr *pip6;
	u8 *daddr;
	int na_olen = 8; /* opt hdr + ETH_ALEN for target */
	int ns_olen;
	int i, len;

	if (dev == NULL)
		return NULL;

	len = LL_RESERVED_SPACE(dev) + sizeof(struct ipv6hdr) +
		sizeof(*na) + na_olen + dev->needed_tailroom;
	reply = alloc_skb(len, GFP_ATOMIC);
	if (reply == NULL)
		return NULL;

	reply->protocol = htons(ETH_P_IPV6);
	reply->dev = dev;
	skb_reserve(reply, LL_RESERVED_SPACE(request->dev));
	skb_push(reply, sizeof(struct ethhdr));
	skb_set_mac_header(reply, 0);

	ns = (struct nd_msg *)skb_transport_header(request);

	daddr = eth_hdr(request)->h_source;
	ns_olen = request->len - skb_transport_offset(request) - sizeof(*ns);
	for (i = 0; i < ns_olen-1; i += (ns->opt[i+1]<<3)) {
		if (ns->opt[i] == ND_OPT_SOURCE_LL_ADDR) {
			daddr = ns->opt + i + sizeof(struct nd_opt_hdr);
			break;
		}
	}

	/* Ethernet header */
	ether_addr_copy(eth_hdr(reply)->h_dest, daddr);
	ether_addr_copy(eth_hdr(reply)->h_source, n->ha);
	eth_hdr(reply)->h_proto = htons(ETH_P_IPV6);
	reply->protocol = htons(ETH_P_IPV6);

	skb_pull(reply, sizeof(struct ethhdr));
	skb_set_network_header(reply, 0);
	skb_put(reply, sizeof(struct ipv6hdr));

	/* IPv6 header */

	pip6 = ipv6_hdr(reply);
	memset(pip6, 0, sizeof(struct ipv6hdr));
	pip6->version = 6;
	pip6->priority = ipv6_hdr(request)->priority;
	pip6->nexthdr = IPPROTO_ICMPV6;
	pip6->hop_limit = 255;
	pip6->daddr = ipv6_hdr(request)->saddr;
	pip6->saddr = *(struct in6_addr *)n->primary_key;

	skb_pull(reply, sizeof(struct ipv6hdr));
	skb_set_transport_header(reply, 0);

	na = (struct nd_msg *)skb_put(reply, sizeof(*na) + na_olen);

	/* Neighbor Advertisement */
	memset(na, 0, sizeof(*na)+na_olen);
	na->icmph.icmp6_type = NDISC_NEIGHBOUR_ADVERTISEMENT;
	na->icmph.icmp6_router = isrouter;
	na->icmph.icmp6_override = 1;
	na->icmph.icmp6_solicited = 1;
	na->target = ns->target;
	ether_addr_copy(&na->opt[2], n->ha);
	na->opt[0] = ND_OPT_TARGET_LL_ADDR;
	na->opt[1] = na_olen >> 3;

	na->icmph.icmp6_cksum = csum_ipv6_magic(&pip6->saddr,
		&pip6->daddr, sizeof(*na)+na_olen, IPPROTO_ICMPV6,
		csum_partial(na, sizeof(*na)+na_olen, 0));

	pip6->payload_len = htons(sizeof(*na)+na_olen);

	skb_push(reply, sizeof(struct ipv6hdr));

	reply->ip_summed = CHECKSUM_UNNECESSARY;

	return reply;
}

static int neigh_reduce(struct net_device *dev, struct sk_buff *skb)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct nd_msg *msg;
	const struct ipv6hdr *iphdr;
	const struct in6_addr *saddr, *daddr;
	struct neighbour *n;
	struct inet6_dev *in6_dev;

	in6_dev = __in6_dev_get(dev);
	if (!in6_dev)
		goto out;

	iphdr = ipv6_hdr(skb);
	saddr = &iphdr->saddr;
	daddr = &iphdr->daddr;

	msg = (struct nd_msg *)skb_transport_header(skb);
	if (msg->icmph.icmp6_code != 0 ||
	    msg->icmph.icmp6_type != NDISC_NEIGHBOUR_SOLICITATION)
		goto out;

	if (ipv6_addr_loopback(daddr) ||
	    ipv6_addr_is_multicast(&msg->target))
		goto out;

	n = neigh_lookup(ipv6_stub->nd_tbl, &msg->target, dev);

	if (n) {
		struct vxlan_fdb *f;
		struct sk_buff *reply;

		if (!(n->nud_state & NUD_CONNECTED)) {
			neigh_release(n);
			goto out;
		}

		f = vxlan_find_mac(vxlan, n->ha);
		if (f && vxlan_addr_any(&(first_remote_rcu(f)->remote_ip))) {
			/* bridge-local neighbor */
			neigh_release(n);
			goto out;
		}

		reply = vxlan_na_create(skb, n,
					!!(f ? f->flags & NTF_ROUTER : 0));

		neigh_release(n);

		if (reply == NULL)
			goto out;

		if (netif_rx_ni(reply) == NET_RX_DROP)
			dev->stats.rx_dropped++;

	} else if (vxlan->flags & VXLAN_F_L3MISS) {
		union vxlan_addr ipa = {
			.sin6.sin6_addr = msg->target,
			.sin6.sin6_family = AF_INET6,
		};

		vxlan_ip_miss(dev, &ipa);
	}

out:
	consume_skb(skb);
	return NETDEV_TX_OK;
}
#endif

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static bool route_shortcircuit(struct net_device *dev, struct sk_buff *skb)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct neighbour *n;
<<<<<<< HEAD
<<<<<<< HEAD
	struct iphdr *pip;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (is_multicast_ether_addr(eth_hdr(skb)->h_dest))
		return false;

	n = NULL;
	switch (ntohs(eth_hdr(skb)->h_proto)) {
	case ETH_P_IP:
<<<<<<< HEAD
<<<<<<< HEAD
=======
	{
		struct iphdr *pip;

>>>>>>> v3.18
=======
	{
		struct iphdr *pip;

>>>>>>> v3.18
		if (!pskb_may_pull(skb, sizeof(struct iphdr)))
			return false;
		pip = ip_hdr(skb);
		n = neigh_lookup(&arp_tbl, &pip->daddr, dev);
<<<<<<< HEAD
<<<<<<< HEAD
		break;
=======
=======
>>>>>>> v3.18
		if (!n && (vxlan->flags & VXLAN_F_L3MISS)) {
			union vxlan_addr ipa = {
				.sin.sin_addr.s_addr = pip->daddr,
				.sin.sin_family = AF_INET,
			};

			vxlan_ip_miss(dev, &ipa);
			return false;
		}

		break;
	}
#if IS_ENABLED(CONFIG_IPV6)
	case ETH_P_IPV6:
	{
		struct ipv6hdr *pip6;

		if (!pskb_may_pull(skb, sizeof(struct ipv6hdr)))
			return false;
		pip6 = ipv6_hdr(skb);
		n = neigh_lookup(ipv6_stub->nd_tbl, &pip6->daddr, dev);
		if (!n && (vxlan->flags & VXLAN_F_L3MISS)) {
			union vxlan_addr ipa = {
				.sin6.sin6_addr = pip6->daddr,
				.sin6.sin6_family = AF_INET6,
			};

			vxlan_ip_miss(dev, &ipa);
			return false;
		}

		break;
	}
#endif
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	default:
		return false;
	}

	if (n) {
		bool diff;

<<<<<<< HEAD
<<<<<<< HEAD
		diff = compare_ether_addr(eth_hdr(skb)->h_dest, n->ha) != 0;
=======
		diff = !ether_addr_equal(eth_hdr(skb)->h_dest, n->ha);
>>>>>>> v3.18
=======
		diff = !ether_addr_equal(eth_hdr(skb)->h_dest, n->ha);
>>>>>>> v3.18
		if (diff) {
			memcpy(eth_hdr(skb)->h_source, eth_hdr(skb)->h_dest,
				dev->addr_len);
			memcpy(eth_hdr(skb)->h_dest, n->ha, dev->addr_len);
		}
		neigh_release(n);
		return diff;
<<<<<<< HEAD
<<<<<<< HEAD
	} else if (vxlan->flags & VXLAN_F_L3MISS)
		vxlan_ip_miss(dev, pip->daddr);
	return false;
}

static void vxlan_sock_free(struct sk_buff *skb)
{
	sock_put(skb->sk);
}

/* On transmit, associate with the tunnel socket */
static void vxlan_set_owner(struct net_device *dev, struct sk_buff *skb)
{
	struct vxlan_net *vn = net_generic(dev_net(dev), vxlan_net_id);
	struct sock *sk = vn->sock->sk;

	skb_orphan(skb);
	sock_hold(sk);
	skb->sk = sk;
	skb->destructor = vxlan_sock_free;
}

/* Compute source port for outgoing packet
 *   first choice to use L4 flow hash since it will spread
 *     better and maybe available from hardware
 *   secondary choice is to use jhash on the Ethernet header
 */
static __be16 vxlan_src_port(const struct vxlan_dev *vxlan, struct sk_buff *skb)
{
	unsigned int range = (vxlan->port_max - vxlan->port_min) + 1;
	u32 hash;

	hash = skb_get_rxhash(skb);
	if (!hash)
		hash = jhash(skb->data, 2 * ETH_ALEN,
			     (__force u32) skb->protocol);

	return htons((((u64) hash * range) >> 32) + vxlan->port_min);
}

static int handle_offloads(struct sk_buff *skb)
{
	if (skb_is_gso(skb)) {
		int err = skb_unclone(skb, GFP_ATOMIC);
		if (unlikely(err))
			return err;

		skb_shinfo(skb)->gso_type |= SKB_GSO_UDP_TUNNEL;
	} else if (skb->ip_summed != CHECKSUM_PARTIAL)
		skb->ip_summed = CHECKSUM_NONE;

	return 0;
}
=======
=======
>>>>>>> v3.18
	}

	return false;
}

#if IS_ENABLED(CONFIG_IPV6)
static int vxlan6_xmit_skb(struct vxlan_sock *vs,
			   struct dst_entry *dst, struct sk_buff *skb,
			   struct net_device *dev, struct in6_addr *saddr,
			   struct in6_addr *daddr, __u8 prio, __u8 ttl,
			   __be16 src_port, __be16 dst_port, __be32 vni,
			   bool xnet)
{
	struct vxlanhdr *vxh;
	int min_headroom;
	int err;
	bool udp_sum = !udp_get_no_check6_tx(vs->sock->sk);

	skb = udp_tunnel_handle_offloads(skb, udp_sum);
	if (IS_ERR(skb))
		return -EINVAL;

	skb_scrub_packet(skb, xnet);

	min_headroom = LL_RESERVED_SPACE(dst->dev) + dst->header_len
			+ VXLAN_HLEN + sizeof(struct ipv6hdr)
			+ (vlan_tx_tag_present(skb) ? VLAN_HLEN : 0);

	/* Need space for new headers (invalidates iph ptr) */
	err = skb_cow_head(skb, min_headroom);
	if (unlikely(err))
		return err;

	if (vlan_tx_tag_present(skb)) {
		if (WARN_ON(!__vlan_put_tag(skb,
					    skb->vlan_proto,
					    vlan_tx_tag_get(skb))))
			return -ENOMEM;

		skb->vlan_tci = 0;
	}

	vxh = (struct vxlanhdr *) __skb_push(skb, sizeof(*vxh));
	vxh->vx_flags = htonl(VXLAN_FLAGS);
	vxh->vx_vni = vni;

	skb_set_inner_protocol(skb, htons(ETH_P_TEB));

	udp_tunnel6_xmit_skb(vs->sock, dst, skb, dev, saddr, daddr, prio,
			     ttl, src_port, dst_port);
	return 0;
}
#endif

int vxlan_xmit_skb(struct vxlan_sock *vs,
		   struct rtable *rt, struct sk_buff *skb,
		   __be32 src, __be32 dst, __u8 tos, __u8 ttl, __be16 df,
		   __be16 src_port, __be16 dst_port, __be32 vni, bool xnet)
{
	struct vxlanhdr *vxh;
	int min_headroom;
	int err;
	bool udp_sum = !vs->sock->sk->sk_no_check_tx;

	skb = udp_tunnel_handle_offloads(skb, udp_sum);
	if (IS_ERR(skb))
		return -EINVAL;

	min_headroom = LL_RESERVED_SPACE(rt->dst.dev) + rt->dst.header_len
			+ VXLAN_HLEN + sizeof(struct iphdr)
			+ (vlan_tx_tag_present(skb) ? VLAN_HLEN : 0);

	/* Need space for new headers (invalidates iph ptr) */
	err = skb_cow_head(skb, min_headroom);
	if (unlikely(err))
		return err;

	if (vlan_tx_tag_present(skb)) {
		if (WARN_ON(!__vlan_put_tag(skb,
					    skb->vlan_proto,
					    vlan_tx_tag_get(skb))))
			return -ENOMEM;

		skb->vlan_tci = 0;
	}

	vxh = (struct vxlanhdr *) __skb_push(skb, sizeof(*vxh));
	vxh->vx_flags = htonl(VXLAN_FLAGS);
	vxh->vx_vni = vni;

	skb_set_inner_protocol(skb, htons(ETH_P_TEB));

	return udp_tunnel_xmit_skb(vs->sock, rt, skb, src, dst, tos,
				   ttl, df, src_port, dst_port, xnet);
}
EXPORT_SYMBOL_GPL(vxlan_xmit_skb);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

/* Bypass encapsulation if the destination is local */
static void vxlan_encap_bypass(struct sk_buff *skb, struct vxlan_dev *src_vxlan,
			       struct vxlan_dev *dst_vxlan)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct pcpu_tstats *tx_stats = this_cpu_ptr(src_vxlan->dev->tstats);
	struct pcpu_tstats *rx_stats = this_cpu_ptr(dst_vxlan->dev->tstats);

=======
=======
>>>>>>> v3.18
	struct pcpu_sw_netstats *tx_stats, *rx_stats;
	union vxlan_addr loopback;
	union vxlan_addr *remote_ip = &dst_vxlan->default_dst.remote_ip;
	struct net_device *dev = skb->dev;
	int len = skb->len;

	tx_stats = this_cpu_ptr(src_vxlan->dev->tstats);
	rx_stats = this_cpu_ptr(dst_vxlan->dev->tstats);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	skb->pkt_type = PACKET_HOST;
	skb->encapsulation = 0;
	skb->dev = dst_vxlan->dev;
	__skb_pull(skb, skb_network_offset(skb));

<<<<<<< HEAD
<<<<<<< HEAD
	if (dst_vxlan->flags & VXLAN_F_LEARN)
		vxlan_snoop(skb->dev, htonl(INADDR_LOOPBACK),
			    eth_hdr(skb)->h_source);

	u64_stats_update_begin(&tx_stats->syncp);
	tx_stats->tx_packets++;
	tx_stats->tx_bytes += skb->len;
=======
=======
>>>>>>> v3.18
	if (remote_ip->sa.sa_family == AF_INET) {
		loopback.sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		loopback.sa.sa_family =  AF_INET;
#if IS_ENABLED(CONFIG_IPV6)
	} else {
		loopback.sin6.sin6_addr = in6addr_loopback;
		loopback.sa.sa_family =  AF_INET6;
#endif
	}

	if (dst_vxlan->flags & VXLAN_F_LEARN)
		vxlan_snoop(skb->dev, &loopback, eth_hdr(skb)->h_source);

	u64_stats_update_begin(&tx_stats->syncp);
	tx_stats->tx_packets++;
	tx_stats->tx_bytes += len;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	u64_stats_update_end(&tx_stats->syncp);

	if (netif_rx(skb) == NET_RX_SUCCESS) {
		u64_stats_update_begin(&rx_stats->syncp);
		rx_stats->rx_packets++;
<<<<<<< HEAD
<<<<<<< HEAD
		rx_stats->rx_bytes += skb->len;
		u64_stats_update_end(&rx_stats->syncp);
	} else {
		skb->dev->stats.rx_dropped++;
	}
}

static netdev_tx_t vxlan_xmit_one(struct sk_buff *skb, struct net_device *dev,
				  struct vxlan_rdst *rdst, bool did_rsc)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct rtable *rt;
	const struct iphdr *old_iph;
	struct iphdr *iph;
	struct vxlanhdr *vxh;
	struct udphdr *uh;
	struct flowi4 fl4;
	__be32 dst;
	__be16 src_port, dst_port;
        u32 vni;
	__be16 df = 0;
	__u8 tos, ttl;

	dst_port = rdst->remote_port ? rdst->remote_port : vxlan->dst_port;
	vni = rdst->remote_vni;
	dst = rdst->remote_ip;

	if (!dst) {
		if (did_rsc) {
			/* short-circuited back to local bridge */
			vxlan_encap_bypass(skb, vxlan, vxlan);
			return NETDEV_TX_OK;
=======
=======
>>>>>>> v3.18
		rx_stats->rx_bytes += len;
		u64_stats_update_end(&rx_stats->syncp);
	} else {
		dev->stats.rx_dropped++;
	}
}

static void vxlan_xmit_one(struct sk_buff *skb, struct net_device *dev,
			   struct vxlan_rdst *rdst, bool did_rsc)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct rtable *rt = NULL;
	const struct iphdr *old_iph;
	struct flowi4 fl4;
	union vxlan_addr *dst;
	__be16 src_port = 0, dst_port;
	u32 vni;
	__be16 df = 0;
	__u8 tos, ttl;
	int err;

	dst_port = rdst->remote_port ? rdst->remote_port : vxlan->dst_port;
	vni = rdst->remote_vni;
	dst = &rdst->remote_ip;

	if (vxlan_addr_any(dst)) {
		if (did_rsc) {
			/* short-circuited back to local bridge */
			vxlan_encap_bypass(skb, vxlan, vxlan);
			return;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		}
		goto drop;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (!skb->encapsulation) {
		skb_reset_inner_headers(skb);
		skb->encapsulation = 1;
	}

	/* Need space for new headers (invalidates iph ptr) */
	if (skb_cow_head(skb, VXLAN_HEADROOM))
		goto drop;

	old_iph = ip_hdr(skb);

	ttl = vxlan->ttl;
	if (!ttl && IN_MULTICAST(ntohl(dst)))
=======
=======
>>>>>>> v3.18
	old_iph = ip_hdr(skb);

	ttl = vxlan->ttl;
	if (!ttl && vxlan_addr_multicast(dst))
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		ttl = 1;

	tos = vxlan->tos;
	if (tos == 1)
		tos = ip_tunnel_get_dsfield(old_iph, skb);

<<<<<<< HEAD
<<<<<<< HEAD
	src_port = vxlan_src_port(vxlan, skb);

	memset(&fl4, 0, sizeof(fl4));
	fl4.flowi4_oif = rdst->remote_ifindex;
	fl4.flowi4_tos = RT_TOS(tos);
	fl4.daddr = dst;
	fl4.saddr = vxlan->saddr;

	rt = ip_route_output_key(dev_net(dev), &fl4);
	if (IS_ERR(rt)) {
		netdev_dbg(dev, "no route to %pI4\n", &dst);
		dev->stats.tx_carrier_errors++;
		goto tx_error;
	}

	if (rt->dst.dev == dev) {
		netdev_dbg(dev, "circular route to %pI4\n", &dst);
		ip_rt_put(rt);
		dev->stats.collisions++;
		goto tx_error;
	}

	/* Bypass encapsulation if the destination is local */
	if (rt->rt_flags & RTCF_LOCAL &&
	    !(rt->rt_flags & (RTCF_BROADCAST | RTCF_MULTICAST))) {
		struct vxlan_dev *dst_vxlan;

		ip_rt_put(rt);
		dst_vxlan = vxlan_find_vni(dev_net(dev), vni);
		if (!dst_vxlan)
			goto tx_error;
		vxlan_encap_bypass(skb, vxlan, dst_vxlan);
		return NETDEV_TX_OK;
	}

	memset(&(IPCB(skb)->opt), 0, sizeof(IPCB(skb)->opt));
	IPCB(skb)->flags &= ~(IPSKB_XFRM_TUNNEL_SIZE | IPSKB_XFRM_TRANSFORMED |
			      IPSKB_REROUTED);
	skb_dst_drop(skb);
	skb_dst_set(skb, &rt->dst);

	vxh = (struct vxlanhdr *) __skb_push(skb, sizeof(*vxh));
	vxh->vx_flags = htonl(VXLAN_FLAGS);
	vxh->vx_vni = htonl(vni << 8);

	__skb_push(skb, sizeof(*uh));
	skb_reset_transport_header(skb);
	uh = udp_hdr(skb);

	uh->dest = dst_port;
	uh->source = src_port;

	uh->len = htons(skb->len);
	uh->check = 0;

	__skb_push(skb, sizeof(*iph));
	skb_reset_network_header(skb);
	iph		= ip_hdr(skb);
	iph->version	= 4;
	iph->ihl	= sizeof(struct iphdr) >> 2;
	iph->frag_off	= df;
	iph->protocol	= IPPROTO_UDP;
	iph->tos	= ip_tunnel_ecn_encap(tos, old_iph, skb);
	iph->daddr	= dst;
	iph->saddr	= fl4.saddr;
	iph->ttl	= ttl ? : ip4_dst_hoplimit(&rt->dst);
	__ip_select_ident(iph, skb_shinfo(skb)->gso_segs ?: 1);

	nf_reset(skb);

	vxlan_set_owner(dev, skb);

	if (handle_offloads(skb))
		goto drop;

	iptunnel_xmit(skb, dev);
	return NETDEV_TX_OK;
=======
=======
>>>>>>> v3.18
	src_port = udp_flow_src_port(dev_net(dev), skb, vxlan->port_min,
				     vxlan->port_max, true);

	if (dst->sa.sa_family == AF_INET) {
		memset(&fl4, 0, sizeof(fl4));
		fl4.flowi4_oif = rdst->remote_ifindex;
		fl4.flowi4_tos = RT_TOS(tos);
		fl4.daddr = dst->sin.sin_addr.s_addr;
		fl4.saddr = vxlan->saddr.sin.sin_addr.s_addr;

		rt = ip_route_output_key(vxlan->net, &fl4);
		if (IS_ERR(rt)) {
			netdev_dbg(dev, "no route to %pI4\n",
				   &dst->sin.sin_addr.s_addr);
			dev->stats.tx_carrier_errors++;
			goto tx_error;
		}

		if (rt->dst.dev == dev) {
			netdev_dbg(dev, "circular route to %pI4\n",
				   &dst->sin.sin_addr.s_addr);
			dev->stats.collisions++;
			goto rt_tx_error;
		}

		/* Bypass encapsulation if the destination is local */
		if (rt->rt_flags & RTCF_LOCAL &&
		    !(rt->rt_flags & (RTCF_BROADCAST | RTCF_MULTICAST))) {
			struct vxlan_dev *dst_vxlan;

			ip_rt_put(rt);
			dst_vxlan = vxlan_find_vni(vxlan->net, vni,
						   dst->sa.sa_family, dst_port);
			if (!dst_vxlan)
				goto tx_error;
			vxlan_encap_bypass(skb, vxlan, dst_vxlan);
			return;
		}

		tos = ip_tunnel_ecn_encap(tos, old_iph, skb);
		ttl = ttl ? : ip4_dst_hoplimit(&rt->dst);

		err = vxlan_xmit_skb(vxlan->vn_sock, rt, skb,
				     fl4.saddr, dst->sin.sin_addr.s_addr,
				     tos, ttl, df, src_port, dst_port,
				     htonl(vni << 8),
				     !net_eq(vxlan->net, dev_net(vxlan->dev)));

		if (err < 0)
			goto rt_tx_error;
		iptunnel_xmit_stats(err, &dev->stats, dev->tstats);
#if IS_ENABLED(CONFIG_IPV6)
	} else {
		struct sock *sk = vxlan->vn_sock->sock->sk;
		struct dst_entry *ndst;
		struct flowi6 fl6;
		u32 flags;

		memset(&fl6, 0, sizeof(fl6));
		fl6.flowi6_oif = rdst->remote_ifindex;
		fl6.daddr = dst->sin6.sin6_addr;
		fl6.saddr = vxlan->saddr.sin6.sin6_addr;
		fl6.flowi6_proto = IPPROTO_UDP;

		if (ipv6_stub->ipv6_dst_lookup(sk, &ndst, &fl6)) {
			netdev_dbg(dev, "no route to %pI6\n",
				   &dst->sin6.sin6_addr);
			dev->stats.tx_carrier_errors++;
			goto tx_error;
		}

		if (ndst->dev == dev) {
			netdev_dbg(dev, "circular route to %pI6\n",
				   &dst->sin6.sin6_addr);
			dst_release(ndst);
			dev->stats.collisions++;
			goto tx_error;
		}

		/* Bypass encapsulation if the destination is local */
		flags = ((struct rt6_info *)ndst)->rt6i_flags;
		if (flags & RTF_LOCAL &&
		    !(flags & (RTCF_BROADCAST | RTCF_MULTICAST))) {
			struct vxlan_dev *dst_vxlan;

			dst_release(ndst);
			dst_vxlan = vxlan_find_vni(vxlan->net, vni,
						   dst->sa.sa_family, dst_port);
			if (!dst_vxlan)
				goto tx_error;
			vxlan_encap_bypass(skb, vxlan, dst_vxlan);
			return;
		}

		ttl = ttl ? : ip6_dst_hoplimit(ndst);

		err = vxlan6_xmit_skb(vxlan->vn_sock, ndst, skb,
				      dev, &fl6.saddr, &fl6.daddr, 0, ttl,
				      src_port, dst_port, htonl(vni << 8),
				      !net_eq(vxlan->net, dev_net(vxlan->dev)));
#endif
	}

	return;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

drop:
	dev->stats.tx_dropped++;
	goto tx_free;

<<<<<<< HEAD
<<<<<<< HEAD
=======
rt_tx_error:
	ip_rt_put(rt);
>>>>>>> v3.18
=======
rt_tx_error:
	ip_rt_put(rt);
>>>>>>> v3.18
tx_error:
	dev->stats.tx_errors++;
tx_free:
	dev_kfree_skb(skb);
<<<<<<< HEAD
<<<<<<< HEAD
	return NETDEV_TX_OK;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/* Transmit local packets over Vxlan
 *
 * Outer IP header inherits ECN and DF from inner header.
 * Outer UDP destination is the VXLAN assigned port.
 *           source port is based on hash of flow
 */
static netdev_tx_t vxlan_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct ethhdr *eth;
	bool did_rsc = false;
<<<<<<< HEAD
<<<<<<< HEAD
	struct vxlan_rdst *rdst0, *rdst;
	struct vxlan_fdb *f;
	int rc1, rc;
=======
	struct vxlan_rdst *rdst, *fdst = NULL;
	struct vxlan_fdb *f;
>>>>>>> v3.18
=======
	struct vxlan_rdst *rdst, *fdst = NULL;
	struct vxlan_fdb *f;
>>>>>>> v3.18

	skb_reset_mac_header(skb);
	eth = eth_hdr(skb);

<<<<<<< HEAD
<<<<<<< HEAD
	if ((vxlan->flags & VXLAN_F_PROXY) && ntohs(eth->h_proto) == ETH_P_ARP)
		return arp_reduce(dev, skb);
=======
=======
>>>>>>> v3.18
	if ((vxlan->flags & VXLAN_F_PROXY)) {
		if (ntohs(eth->h_proto) == ETH_P_ARP)
			return arp_reduce(dev, skb);
#if IS_ENABLED(CONFIG_IPV6)
		else if (ntohs(eth->h_proto) == ETH_P_IPV6 &&
			 pskb_may_pull(skb, sizeof(struct ipv6hdr)
				       + sizeof(struct nd_msg)) &&
			 ipv6_hdr(skb)->nexthdr == IPPROTO_ICMPV6) {
				struct nd_msg *msg;

				msg = (struct nd_msg *)skb_transport_header(skb);
				if (msg->icmph.icmp6_code == 0 &&
				    msg->icmph.icmp6_type == NDISC_NEIGHBOUR_SOLICITATION)
					return neigh_reduce(dev, skb);
		}
		eth = eth_hdr(skb);
#endif
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	f = vxlan_find_mac(vxlan, eth->h_dest);
	did_rsc = false;

	if (f && (f->flags & NTF_ROUTER) && (vxlan->flags & VXLAN_F_RSC) &&
<<<<<<< HEAD
<<<<<<< HEAD
	    ntohs(eth->h_proto) == ETH_P_IP) {
=======
	    (ntohs(eth->h_proto) == ETH_P_IP ||
	     ntohs(eth->h_proto) == ETH_P_IPV6)) {
>>>>>>> v3.18
=======
	    (ntohs(eth->h_proto) == ETH_P_IP ||
	     ntohs(eth->h_proto) == ETH_P_IPV6)) {
>>>>>>> v3.18
		did_rsc = route_shortcircuit(dev, skb);
		if (did_rsc)
			f = vxlan_find_mac(vxlan, eth->h_dest);
	}

	if (f == NULL) {
<<<<<<< HEAD
<<<<<<< HEAD
		rdst0 = &vxlan->default_dst;

		if (rdst0->remote_ip == htonl(INADDR_ANY) &&
		    (vxlan->flags & VXLAN_F_L2MISS) &&
		    !is_multicast_ether_addr(eth->h_dest))
			vxlan_fdb_miss(vxlan, eth->h_dest);
	} else
		rdst0 = &f->remote;

	rc = NETDEV_TX_OK;

	/* if there are multiple destinations, send copies */
	for (rdst = rdst0->remote_next; rdst; rdst = rdst->remote_next) {
		struct sk_buff *skb1;

		skb1 = skb_clone(skb, GFP_ATOMIC);
		if (skb1) {
			rc1 = vxlan_xmit_one(skb1, dev, rdst, did_rsc);
			if (rc == NETDEV_TX_OK)
				rc = rc1;
		}
	}

	rc1 = vxlan_xmit_one(skb, dev, rdst0, did_rsc);
	if (rc == NETDEV_TX_OK)
		rc = rc1;
	return rc;
=======
=======
>>>>>>> v3.18
		f = vxlan_find_mac(vxlan, all_zeros_mac);
		if (f == NULL) {
			if ((vxlan->flags & VXLAN_F_L2MISS) &&
			    !is_multicast_ether_addr(eth->h_dest))
				vxlan_fdb_miss(vxlan, eth->h_dest);

			dev->stats.tx_dropped++;
			kfree_skb(skb);
			return NETDEV_TX_OK;
		}
	}

	list_for_each_entry_rcu(rdst, &f->remotes, list) {
		struct sk_buff *skb1;

		if (!fdst) {
			fdst = rdst;
			continue;
		}
		skb1 = skb_clone(skb, GFP_ATOMIC);
		if (skb1)
			vxlan_xmit_one(skb1, dev, rdst, did_rsc);
	}

	if (fdst)
		vxlan_xmit_one(skb, dev, fdst, did_rsc);
	else
		kfree_skb(skb);
	return NETDEV_TX_OK;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/* Walk the forwarding table and purge stale entries */
static void vxlan_cleanup(unsigned long arg)
{
	struct vxlan_dev *vxlan = (struct vxlan_dev *) arg;
	unsigned long next_timer = jiffies + FDB_AGE_INTERVAL;
	unsigned int h;

	if (!netif_running(vxlan->dev))
		return;

	spin_lock_bh(&vxlan->hash_lock);
	for (h = 0; h < FDB_HASH_SIZE; ++h) {
		struct hlist_node *p, *n;
		hlist_for_each_safe(p, n, &vxlan->fdb_head[h]) {
			struct vxlan_fdb *f
				= container_of(p, struct vxlan_fdb, hlist);
			unsigned long timeout;

			if (f->state & NUD_PERMANENT)
				continue;

			timeout = f->used + vxlan->age_interval * HZ;
			if (time_before_eq(timeout, jiffies)) {
				netdev_dbg(vxlan->dev,
					   "garbage collect %pM\n",
					   f->eth_addr);
				f->state = NUD_STALE;
				vxlan_fdb_destroy(vxlan, f);
			} else if (time_before(timeout, next_timer))
				next_timer = timeout;
		}
	}
	spin_unlock_bh(&vxlan->hash_lock);

	mod_timer(&vxlan->age_timer, next_timer);
}

<<<<<<< HEAD
<<<<<<< HEAD
/* Setup stats when device is created */
static int vxlan_init(struct net_device *dev)
{
	dev->tstats = alloc_percpu(struct pcpu_tstats);
	if (!dev->tstats)
		return -ENOMEM;

	return 0;
}

=======
=======
>>>>>>> v3.18
static void vxlan_vs_add_dev(struct vxlan_sock *vs, struct vxlan_dev *vxlan)
{
	__u32 vni = vxlan->default_dst.remote_vni;

	vxlan->vn_sock = vs;
	hlist_add_head_rcu(&vxlan->hlist, vni_head(vs, vni));
}

/* Setup stats when device is created */
static int vxlan_init(struct net_device *dev)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct vxlan_net *vn = net_generic(vxlan->net, vxlan_net_id);
	struct vxlan_sock *vs;
	bool ipv6 = vxlan->flags & VXLAN_F_IPV6;

	dev->tstats = netdev_alloc_pcpu_stats(struct pcpu_sw_netstats);
	if (!dev->tstats)
		return -ENOMEM;

	spin_lock(&vn->sock_lock);
	vs = vxlan_find_sock(vxlan->net, ipv6 ? AF_INET6 : AF_INET,
			     vxlan->dst_port);
	if (vs) {
		/* If we have a socket with same port already, reuse it */
		atomic_inc(&vs->refcnt);
		vxlan_vs_add_dev(vs, vxlan);
	} else {
		/* otherwise make new socket outside of RTNL */
		dev_hold(dev);
		queue_work(vxlan_wq, &vxlan->sock_work);
	}
	spin_unlock(&vn->sock_lock);

	return 0;
}

static void vxlan_fdb_delete_default(struct vxlan_dev *vxlan)
{
	struct vxlan_fdb *f;

	spin_lock_bh(&vxlan->hash_lock);
	f = __vxlan_find_mac(vxlan, all_zeros_mac);
	if (f)
		vxlan_fdb_destroy(vxlan, f);
	spin_unlock_bh(&vxlan->hash_lock);
}

static void vxlan_uninit(struct net_device *dev)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct vxlan_sock *vs = vxlan->vn_sock;

	vxlan_fdb_delete_default(vxlan);

	if (vs)
		vxlan_sock_release(vs);
	free_percpu(dev->tstats);
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
/* Start ageing timer and join group when device is brought up */
static int vxlan_open(struct net_device *dev)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
<<<<<<< HEAD
<<<<<<< HEAD
	int err;

	if (IN_MULTICAST(ntohl(vxlan->default_dst.remote_ip))) {
		err = vxlan_join_group(dev);
		if (err)
			return err;
=======
=======
>>>>>>> v3.18
	struct vxlan_sock *vs = vxlan->vn_sock;

	/* socket hasn't been created */
	if (!vs)
		return -ENOTCONN;

	if (vxlan_addr_multicast(&vxlan->default_dst.remote_ip)) {
		vxlan_sock_hold(vs);
		dev_hold(dev);
		queue_work(vxlan_wq, &vxlan->igmp_join);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}

	if (vxlan->age_interval)
		mod_timer(&vxlan->age_timer, jiffies + FDB_AGE_INTERVAL);

	return 0;
}

/* Purge the forwarding table */
static void vxlan_flush(struct vxlan_dev *vxlan)
{
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned h;
=======
	unsigned int h;
>>>>>>> v3.18
=======
	unsigned int h;
>>>>>>> v3.18

	spin_lock_bh(&vxlan->hash_lock);
	for (h = 0; h < FDB_HASH_SIZE; ++h) {
		struct hlist_node *p, *n;
		hlist_for_each_safe(p, n, &vxlan->fdb_head[h]) {
			struct vxlan_fdb *f
				= container_of(p, struct vxlan_fdb, hlist);
<<<<<<< HEAD
<<<<<<< HEAD
			vxlan_fdb_destroy(vxlan, f);
=======
			/* the all_zeros_mac entry is deleted at vxlan_uninit */
			if (!is_zero_ether_addr(f->eth_addr))
				vxlan_fdb_destroy(vxlan, f);
>>>>>>> v3.18
=======
			/* the all_zeros_mac entry is deleted at vxlan_uninit */
			if (!is_zero_ether_addr(f->eth_addr))
				vxlan_fdb_destroy(vxlan, f);
>>>>>>> v3.18
		}
	}
	spin_unlock_bh(&vxlan->hash_lock);
}

/* Cleanup timer and forwarding table on shutdown */
static int vxlan_stop(struct net_device *dev)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
<<<<<<< HEAD
<<<<<<< HEAD

	if (IN_MULTICAST(ntohl(vxlan->default_dst.remote_ip)))
		vxlan_leave_group(dev);
=======
=======
>>>>>>> v3.18
	struct vxlan_net *vn = net_generic(vxlan->net, vxlan_net_id);
	struct vxlan_sock *vs = vxlan->vn_sock;

	if (vs && vxlan_addr_multicast(&vxlan->default_dst.remote_ip) &&
	    !vxlan_group_used(vn, vxlan)) {
		vxlan_sock_hold(vs);
		dev_hold(dev);
		queue_work(vxlan_wq, &vxlan->igmp_leave);
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	del_timer_sync(&vxlan->age_timer);

	vxlan_flush(vxlan);

	return 0;
}

/* Stub, nothing needs to be done. */
static void vxlan_set_multicast_list(struct net_device *dev)
{
}

<<<<<<< HEAD
<<<<<<< HEAD
static const struct net_device_ops vxlan_netdev_ops = {
	.ndo_init		= vxlan_init,
=======
=======
>>>>>>> v3.18
static int vxlan_change_mtu(struct net_device *dev, int new_mtu)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct vxlan_rdst *dst = &vxlan->default_dst;
	struct net_device *lowerdev;
	int max_mtu;

	lowerdev = __dev_get_by_index(vxlan->net, dst->remote_ifindex);
	if (lowerdev == NULL)
		return eth_change_mtu(dev, new_mtu);

	if (dst->remote_ip.sa.sa_family == AF_INET6)
		max_mtu = lowerdev->mtu - VXLAN6_HEADROOM;
	else
		max_mtu = lowerdev->mtu - VXLAN_HEADROOM;

	if (new_mtu < 68 || new_mtu > max_mtu)
		return -EINVAL;

	dev->mtu = new_mtu;
	return 0;
}

static const struct net_device_ops vxlan_netdev_ops = {
	.ndo_init		= vxlan_init,
	.ndo_uninit		= vxlan_uninit,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	.ndo_open		= vxlan_open,
	.ndo_stop		= vxlan_stop,
	.ndo_start_xmit		= vxlan_xmit,
	.ndo_get_stats64	= ip_tunnel_get_stats64,
	.ndo_set_rx_mode	= vxlan_set_multicast_list,
<<<<<<< HEAD
<<<<<<< HEAD
	.ndo_change_mtu		= eth_change_mtu,
=======
	.ndo_change_mtu		= vxlan_change_mtu,
>>>>>>> v3.18
=======
	.ndo_change_mtu		= vxlan_change_mtu,
>>>>>>> v3.18
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_set_mac_address	= eth_mac_addr,
	.ndo_fdb_add		= vxlan_fdb_add,
	.ndo_fdb_del		= vxlan_fdb_delete,
	.ndo_fdb_dump		= vxlan_fdb_dump,
};

/* Info for udev, that this is a virtual tunnel endpoint */
static struct device_type vxlan_type = {
	.name = "vxlan",
};

<<<<<<< HEAD
<<<<<<< HEAD
static void vxlan_free(struct net_device *dev)
{
	free_percpu(dev->tstats);
	free_netdev(dev);
}
=======
=======
>>>>>>> v3.18
/* Calls the ndo_add_vxlan_port of the caller in order to
 * supply the listening VXLAN udp ports. Callers are expected
 * to implement the ndo_add_vxlan_port.
 */
void vxlan_get_rx_port(struct net_device *dev)
{
	struct vxlan_sock *vs;
	struct net *net = dev_net(dev);
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);
	sa_family_t sa_family;
	__be16 port;
	unsigned int i;

	spin_lock(&vn->sock_lock);
	for (i = 0; i < PORT_HASH_SIZE; ++i) {
		hlist_for_each_entry_rcu(vs, &vn->sock_list[i], hlist) {
			port = inet_sk(vs->sock->sk)->inet_sport;
			sa_family = vs->sock->sk->sk_family;
			dev->netdev_ops->ndo_add_vxlan_port(dev, sa_family,
							    port);
		}
	}
	spin_unlock(&vn->sock_lock);
}
EXPORT_SYMBOL_GPL(vxlan_get_rx_port);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

/* Initialize the device structure. */
static void vxlan_setup(struct net_device *dev)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned h;
	int low, high;

	eth_hw_addr_random(dev);
	ether_setup(dev);
	dev->needed_headroom = ETH_HLEN + VXLAN_HEADROOM;

	dev->netdev_ops = &vxlan_netdev_ops;
	dev->destructor = vxlan_free;
=======
=======
>>>>>>> v3.18
	unsigned int h;

	eth_hw_addr_random(dev);
	ether_setup(dev);
	if (vxlan->default_dst.remote_ip.sa.sa_family == AF_INET6)
		dev->needed_headroom = ETH_HLEN + VXLAN6_HEADROOM;
	else
		dev->needed_headroom = ETH_HLEN + VXLAN_HEADROOM;

	dev->netdev_ops = &vxlan_netdev_ops;
	dev->destructor = free_netdev;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	SET_NETDEV_DEVTYPE(dev, &vxlan_type);

	dev->tx_queue_len = 0;
	dev->features	|= NETIF_F_LLTX;
<<<<<<< HEAD
<<<<<<< HEAD
	dev->features	|= NETIF_F_NETNS_LOCAL;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	dev->features	|= NETIF_F_SG | NETIF_F_HW_CSUM;
	dev->features   |= NETIF_F_RXCSUM;
	dev->features   |= NETIF_F_GSO_SOFTWARE;

<<<<<<< HEAD
<<<<<<< HEAD
	dev->hw_features |= NETIF_F_SG | NETIF_F_HW_CSUM | NETIF_F_RXCSUM;
	dev->hw_features |= NETIF_F_GSO_SOFTWARE;
	dev->priv_flags	&= ~IFF_XMIT_DST_RELEASE;
	dev->priv_flags |= IFF_LIVE_ADDR_CHANGE;

	spin_lock_init(&vxlan->hash_lock);
=======
=======
>>>>>>> v3.18
	dev->vlan_features = dev->features;
	dev->features |= NETIF_F_HW_VLAN_CTAG_TX | NETIF_F_HW_VLAN_STAG_TX;
	dev->hw_features |= NETIF_F_SG | NETIF_F_HW_CSUM | NETIF_F_RXCSUM;
	dev->hw_features |= NETIF_F_GSO_SOFTWARE;
	dev->hw_features |= NETIF_F_HW_VLAN_CTAG_TX | NETIF_F_HW_VLAN_STAG_TX;
	netif_keep_dst(dev);
	dev->priv_flags |= IFF_LIVE_ADDR_CHANGE;

	INIT_LIST_HEAD(&vxlan->next);
	spin_lock_init(&vxlan->hash_lock);
	INIT_WORK(&vxlan->igmp_join, vxlan_igmp_join);
	INIT_WORK(&vxlan->igmp_leave, vxlan_igmp_leave);
	INIT_WORK(&vxlan->sock_work, vxlan_sock_work);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	init_timer_deferrable(&vxlan->age_timer);
	vxlan->age_timer.function = vxlan_cleanup;
	vxlan->age_timer.data = (unsigned long) vxlan;

<<<<<<< HEAD
<<<<<<< HEAD
	inet_get_local_port_range(&low, &high);
	vxlan->port_min = low;
	vxlan->port_max = high;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	vxlan->dst_port = htons(vxlan_port);

	vxlan->dev = dev;

	for (h = 0; h < FDB_HASH_SIZE; ++h)
		INIT_HLIST_HEAD(&vxlan->fdb_head[h]);
}

static const struct nla_policy vxlan_policy[IFLA_VXLAN_MAX + 1] = {
	[IFLA_VXLAN_ID]		= { .type = NLA_U32 },
	[IFLA_VXLAN_GROUP]	= { .len = FIELD_SIZEOF(struct iphdr, daddr) },
<<<<<<< HEAD
<<<<<<< HEAD
	[IFLA_VXLAN_LINK]	= { .type = NLA_U32 },
	[IFLA_VXLAN_LOCAL]	= { .len = FIELD_SIZEOF(struct iphdr, saddr) },
=======
=======
>>>>>>> v3.18
	[IFLA_VXLAN_GROUP6]	= { .len = sizeof(struct in6_addr) },
	[IFLA_VXLAN_LINK]	= { .type = NLA_U32 },
	[IFLA_VXLAN_LOCAL]	= { .len = FIELD_SIZEOF(struct iphdr, saddr) },
	[IFLA_VXLAN_LOCAL6]	= { .len = sizeof(struct in6_addr) },
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	[IFLA_VXLAN_TOS]	= { .type = NLA_U8 },
	[IFLA_VXLAN_TTL]	= { .type = NLA_U8 },
	[IFLA_VXLAN_LEARNING]	= { .type = NLA_U8 },
	[IFLA_VXLAN_AGEING]	= { .type = NLA_U32 },
	[IFLA_VXLAN_LIMIT]	= { .type = NLA_U32 },
	[IFLA_VXLAN_PORT_RANGE] = { .len  = sizeof(struct ifla_vxlan_port_range) },
	[IFLA_VXLAN_PROXY]	= { .type = NLA_U8 },
	[IFLA_VXLAN_RSC]	= { .type = NLA_U8 },
	[IFLA_VXLAN_L2MISS]	= { .type = NLA_U8 },
	[IFLA_VXLAN_L3MISS]	= { .type = NLA_U8 },
	[IFLA_VXLAN_PORT]	= { .type = NLA_U16 },
};

static int vxlan_validate(struct nlattr *tb[], struct nlattr *data[])
{
	if (tb[IFLA_ADDRESS]) {
		if (nla_len(tb[IFLA_ADDRESS]) != ETH_ALEN) {
			pr_debug("invalid link address (not ethernet)\n");
			return -EINVAL;
		}

		if (!is_valid_ether_addr(nla_data(tb[IFLA_ADDRESS]))) {
			pr_debug("invalid all zero ethernet address\n");
			return -EADDRNOTAVAIL;
		}
	}

	if (!data)
		return -EINVAL;

	if (data[IFLA_VXLAN_ID]) {
		__u32 id = nla_get_u32(data[IFLA_VXLAN_ID]);
<<<<<<< HEAD
<<<<<<< HEAD
		if (id >= VXLAN_N_VID)
=======
		if (id >= VXLAN_VID_MASK)
>>>>>>> v3.18
=======
		if (id >= VXLAN_VID_MASK)
>>>>>>> v3.18
			return -ERANGE;
	}

	if (data[IFLA_VXLAN_PORT_RANGE]) {
		const struct ifla_vxlan_port_range *p
			= nla_data(data[IFLA_VXLAN_PORT_RANGE]);

		if (ntohs(p->high) < ntohs(p->low)) {
			pr_debug("port range %u .. %u not valid\n",
				 ntohs(p->low), ntohs(p->high));
			return -EINVAL;
		}
	}

	return 0;
}

static void vxlan_get_drvinfo(struct net_device *netdev,
			      struct ethtool_drvinfo *drvinfo)
{
	strlcpy(drvinfo->version, VXLAN_VERSION, sizeof(drvinfo->version));
	strlcpy(drvinfo->driver, "vxlan", sizeof(drvinfo->driver));
}

static const struct ethtool_ops vxlan_ethtool_ops = {
	.get_drvinfo	= vxlan_get_drvinfo,
	.get_link	= ethtool_op_get_link,
};

<<<<<<< HEAD
<<<<<<< HEAD
static int vxlan_newlink(struct net *net, struct net_device *dev,
			 struct nlattr *tb[], struct nlattr *data[])
{
=======
=======
>>>>>>> v3.18
static void vxlan_del_work(struct work_struct *work)
{
	struct vxlan_sock *vs = container_of(work, struct vxlan_sock, del_work);
	udp_tunnel_sock_release(vs->sock);
	kfree_rcu(vs, rcu);
}

static struct socket *vxlan_create_sock(struct net *net, bool ipv6,
					__be16 port, u32 flags)
{
	struct socket *sock;
	struct udp_port_cfg udp_conf;
	int err;

	memset(&udp_conf, 0, sizeof(udp_conf));

	if (ipv6) {
		udp_conf.family = AF_INET6;
		udp_conf.use_udp6_tx_checksums =
		    !(flags & VXLAN_F_UDP_ZERO_CSUM6_TX);
		udp_conf.use_udp6_rx_checksums =
		    !(flags & VXLAN_F_UDP_ZERO_CSUM6_RX);
	} else {
		udp_conf.family = AF_INET;
		udp_conf.local_ip.s_addr = INADDR_ANY;
		udp_conf.use_udp_checksums =
		    !!(flags & VXLAN_F_UDP_CSUM);
	}

	udp_conf.local_udp_port = port;

	/* Open UDP socket */
	err = udp_sock_create(net, &udp_conf, &sock);
	if (err < 0)
		return ERR_PTR(err);

	return sock;
}

/* Create new listen socket if needed */
static struct vxlan_sock *vxlan_socket_create(struct net *net, __be16 port,
					      vxlan_rcv_t *rcv, void *data,
					      u32 flags)
{
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);
	struct vxlan_sock *vs;
	struct socket *sock;
	unsigned int h;
	bool ipv6 = !!(flags & VXLAN_F_IPV6);
	struct udp_tunnel_sock_cfg tunnel_cfg;

	vs = kzalloc(sizeof(*vs), GFP_KERNEL);
	if (!vs)
		return ERR_PTR(-ENOMEM);

	for (h = 0; h < VNI_HASH_SIZE; ++h)
		INIT_HLIST_HEAD(&vs->vni_list[h]);

	INIT_WORK(&vs->del_work, vxlan_del_work);

	sock = vxlan_create_sock(net, ipv6, port, flags);
	if (IS_ERR(sock)) {
		kfree(vs);
		return ERR_CAST(sock);
	}

	vs->sock = sock;
	atomic_set(&vs->refcnt, 1);
	vs->rcv = rcv;
	vs->data = data;

	/* Initialize the vxlan udp offloads structure */
	vs->udp_offloads.port = port;
	vs->udp_offloads.callbacks.gro_receive  = vxlan_gro_receive;
	vs->udp_offloads.callbacks.gro_complete = vxlan_gro_complete;

	spin_lock(&vn->sock_lock);
	hlist_add_head_rcu(&vs->hlist, vs_head(net, port));
	vxlan_notify_add_rx_port(vs);
	spin_unlock(&vn->sock_lock);

	/* Mark socket as an encapsulation socket. */
	tunnel_cfg.sk_user_data = vs;
	tunnel_cfg.encap_type = 1;
	tunnel_cfg.encap_rcv = vxlan_udp_encap_recv;
	tunnel_cfg.encap_destroy = NULL;

	setup_udp_tunnel_sock(net, sock, &tunnel_cfg);

	return vs;
}

struct vxlan_sock *vxlan_sock_add(struct net *net, __be16 port,
				  vxlan_rcv_t *rcv, void *data,
				  bool no_share, u32 flags)
{
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);
	struct vxlan_sock *vs;
	bool ipv6 = flags & VXLAN_F_IPV6;

	vs = vxlan_socket_create(net, port, rcv, data, flags);
	if (!IS_ERR(vs))
		return vs;

	if (no_share)	/* Return error if sharing is not allowed. */
		return vs;

	spin_lock(&vn->sock_lock);
	vs = vxlan_find_sock(net, ipv6 ? AF_INET6 : AF_INET, port);
	if (vs) {
		if (vs->rcv == rcv)
			atomic_inc(&vs->refcnt);
		else
			vs = ERR_PTR(-EBUSY);
	}
	spin_unlock(&vn->sock_lock);

	if (!vs)
		vs = ERR_PTR(-EINVAL);

	return vs;
}
EXPORT_SYMBOL_GPL(vxlan_sock_add);

/* Scheduled at device creation to bind to a socket */
static void vxlan_sock_work(struct work_struct *work)
{
	struct vxlan_dev *vxlan = container_of(work, struct vxlan_dev, sock_work);
	struct net *net = vxlan->net;
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);
	__be16 port = vxlan->dst_port;
	struct vxlan_sock *nvs;

	nvs = vxlan_sock_add(net, port, vxlan_rcv, NULL, false, vxlan->flags);
	spin_lock(&vn->sock_lock);
	if (!IS_ERR(nvs))
		vxlan_vs_add_dev(nvs, vxlan);
	spin_unlock(&vn->sock_lock);

	dev_put(vxlan->dev);
}

static int vxlan_newlink(struct net *net, struct net_device *dev,
			 struct nlattr *tb[], struct nlattr *data[])
{
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	struct vxlan_dev *vxlan = netdev_priv(dev);
	struct vxlan_rdst *dst = &vxlan->default_dst;
	__u32 vni;
	int err;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	bool use_ipv6 = false;
>>>>>>> v3.18
=======
	bool use_ipv6 = false;
>>>>>>> v3.18

	if (!data[IFLA_VXLAN_ID])
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	vni = nla_get_u32(data[IFLA_VXLAN_ID]);
	if (vxlan_find_vni(net, vni)) {
		pr_info("duplicate VNI %u\n", vni);
		return -EEXIST;
	}
	dst->remote_vni = vni;

	if (data[IFLA_VXLAN_GROUP])
		dst->remote_ip = nla_get_be32(data[IFLA_VXLAN_GROUP]);

	if (data[IFLA_VXLAN_LOCAL])
		vxlan->saddr = nla_get_be32(data[IFLA_VXLAN_LOCAL]);
=======
=======
>>>>>>> v3.18
	vxlan->net = dev_net(dev);

	vni = nla_get_u32(data[IFLA_VXLAN_ID]);
	dst->remote_vni = vni;

	/* Unless IPv6 is explicitly requested, assume IPv4 */
	dst->remote_ip.sa.sa_family = AF_INET;
	if (data[IFLA_VXLAN_GROUP]) {
		dst->remote_ip.sin.sin_addr.s_addr = nla_get_be32(data[IFLA_VXLAN_GROUP]);
	} else if (data[IFLA_VXLAN_GROUP6]) {
		if (!IS_ENABLED(CONFIG_IPV6))
			return -EPFNOSUPPORT;

		nla_memcpy(&dst->remote_ip.sin6.sin6_addr, data[IFLA_VXLAN_GROUP6],
			   sizeof(struct in6_addr));
		dst->remote_ip.sa.sa_family = AF_INET6;
		use_ipv6 = true;
	}

	if (data[IFLA_VXLAN_LOCAL]) {
		vxlan->saddr.sin.sin_addr.s_addr = nla_get_be32(data[IFLA_VXLAN_LOCAL]);
		vxlan->saddr.sa.sa_family = AF_INET;
	} else if (data[IFLA_VXLAN_LOCAL6]) {
		if (!IS_ENABLED(CONFIG_IPV6))
			return -EPFNOSUPPORT;

		/* TODO: respect scope id */
		nla_memcpy(&vxlan->saddr.sin6.sin6_addr, data[IFLA_VXLAN_LOCAL6],
			   sizeof(struct in6_addr));
		vxlan->saddr.sa.sa_family = AF_INET6;
		use_ipv6 = true;
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (data[IFLA_VXLAN_LINK] &&
	    (dst->remote_ifindex = nla_get_u32(data[IFLA_VXLAN_LINK]))) {
		struct net_device *lowerdev
			 = __dev_get_by_index(net, dst->remote_ifindex);

		if (!lowerdev) {
			pr_info("ifindex %d does not exist\n", dst->remote_ifindex);
			return -ENODEV;
		}

<<<<<<< HEAD
<<<<<<< HEAD
		if (!tb[IFLA_MTU])
			dev->mtu = lowerdev->mtu - VXLAN_HEADROOM;

		/* update header length based on lower device */
		dev->needed_headroom = lowerdev->hard_header_len +
				       VXLAN_HEADROOM;
	}
=======
=======
>>>>>>> v3.18
#if IS_ENABLED(CONFIG_IPV6)
		if (use_ipv6) {
			struct inet6_dev *idev = __in6_dev_get(lowerdev);
			if (idev && idev->cnf.disable_ipv6) {
				pr_info("IPv6 is disabled via sysctl\n");
				return -EPERM;
			}
			vxlan->flags |= VXLAN_F_IPV6;
		}
#endif

		if (!tb[IFLA_MTU])
			dev->mtu = lowerdev->mtu - (use_ipv6 ? VXLAN6_HEADROOM : VXLAN_HEADROOM);

		dev->needed_headroom = lowerdev->hard_header_len +
				       (use_ipv6 ? VXLAN6_HEADROOM : VXLAN_HEADROOM);
	} else if (use_ipv6)
		vxlan->flags |= VXLAN_F_IPV6;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (data[IFLA_VXLAN_TOS])
		vxlan->tos  = nla_get_u8(data[IFLA_VXLAN_TOS]);

	if (data[IFLA_VXLAN_TTL])
		vxlan->ttl = nla_get_u8(data[IFLA_VXLAN_TTL]);

	if (!data[IFLA_VXLAN_LEARNING] || nla_get_u8(data[IFLA_VXLAN_LEARNING]))
		vxlan->flags |= VXLAN_F_LEARN;

	if (data[IFLA_VXLAN_AGEING])
		vxlan->age_interval = nla_get_u32(data[IFLA_VXLAN_AGEING]);
	else
		vxlan->age_interval = FDB_AGE_DEFAULT;

	if (data[IFLA_VXLAN_PROXY] && nla_get_u8(data[IFLA_VXLAN_PROXY]))
		vxlan->flags |= VXLAN_F_PROXY;

	if (data[IFLA_VXLAN_RSC] && nla_get_u8(data[IFLA_VXLAN_RSC]))
		vxlan->flags |= VXLAN_F_RSC;

	if (data[IFLA_VXLAN_L2MISS] && nla_get_u8(data[IFLA_VXLAN_L2MISS]))
		vxlan->flags |= VXLAN_F_L2MISS;

	if (data[IFLA_VXLAN_L3MISS] && nla_get_u8(data[IFLA_VXLAN_L3MISS]))
		vxlan->flags |= VXLAN_F_L3MISS;

	if (data[IFLA_VXLAN_LIMIT])
		vxlan->addrmax = nla_get_u32(data[IFLA_VXLAN_LIMIT]);

	if (data[IFLA_VXLAN_PORT_RANGE]) {
		const struct ifla_vxlan_port_range *p
			= nla_data(data[IFLA_VXLAN_PORT_RANGE]);
		vxlan->port_min = ntohs(p->low);
		vxlan->port_max = ntohs(p->high);
	}

	if (data[IFLA_VXLAN_PORT])
		vxlan->dst_port = nla_get_be16(data[IFLA_VXLAN_PORT]);

<<<<<<< HEAD
<<<<<<< HEAD
	SET_ETHTOOL_OPS(dev, &vxlan_ethtool_ops);

	err = register_netdevice(dev);
	if (!err)
		hlist_add_head_rcu(&vxlan->hlist, vni_head(net, dst->remote_vni));

	return err;
=======
=======
>>>>>>> v3.18
	if (data[IFLA_VXLAN_UDP_CSUM] && nla_get_u8(data[IFLA_VXLAN_UDP_CSUM]))
		vxlan->flags |= VXLAN_F_UDP_CSUM;

	if (data[IFLA_VXLAN_UDP_ZERO_CSUM6_TX] &&
	    nla_get_u8(data[IFLA_VXLAN_UDP_ZERO_CSUM6_TX]))
		vxlan->flags |= VXLAN_F_UDP_ZERO_CSUM6_TX;

	if (data[IFLA_VXLAN_UDP_ZERO_CSUM6_RX] &&
	    nla_get_u8(data[IFLA_VXLAN_UDP_ZERO_CSUM6_RX]))
		vxlan->flags |= VXLAN_F_UDP_ZERO_CSUM6_RX;

	if (vxlan_find_vni(net, vni, use_ipv6 ? AF_INET6 : AF_INET,
			   vxlan->dst_port)) {
		pr_info("duplicate VNI %u\n", vni);
		return -EEXIST;
	}

	dev->ethtool_ops = &vxlan_ethtool_ops;

	/* create an fdb entry for a valid default destination */
	if (!vxlan_addr_any(&vxlan->default_dst.remote_ip)) {
		err = vxlan_fdb_create(vxlan, all_zeros_mac,
				       &vxlan->default_dst.remote_ip,
				       NUD_REACHABLE|NUD_PERMANENT,
				       NLM_F_EXCL|NLM_F_CREATE,
				       vxlan->dst_port,
				       vxlan->default_dst.remote_vni,
				       vxlan->default_dst.remote_ifindex,
				       NTF_SELF);
		if (err)
			return err;
	}

	err = register_netdevice(dev);
	if (err) {
		vxlan_fdb_delete_default(vxlan);
		return err;
	}

	list_add(&vxlan->next, &vn->vxlan_list);

	return 0;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static void vxlan_dellink(struct net_device *dev, struct list_head *head)
{
	struct vxlan_dev *vxlan = netdev_priv(dev);
<<<<<<< HEAD
<<<<<<< HEAD

	hlist_del_rcu(&vxlan->hlist);

=======
=======
>>>>>>> v3.18
	struct vxlan_net *vn = net_generic(vxlan->net, vxlan_net_id);

	spin_lock(&vn->sock_lock);
	if (!hlist_unhashed(&vxlan->hlist))
		hlist_del_rcu(&vxlan->hlist);
	spin_unlock(&vn->sock_lock);

	list_del(&vxlan->next);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	unregister_netdevice_queue(dev, head);
}

static size_t vxlan_get_size(const struct net_device *dev)
{

	return nla_total_size(sizeof(__u32)) +	/* IFLA_VXLAN_ID */
<<<<<<< HEAD
<<<<<<< HEAD
		nla_total_size(sizeof(__be32)) +/* IFLA_VXLAN_GROUP */
		nla_total_size(sizeof(__u32)) +	/* IFLA_VXLAN_LINK */
		nla_total_size(sizeof(__be32))+	/* IFLA_VXLAN_LOCAL */
=======
		nla_total_size(sizeof(struct in6_addr)) + /* IFLA_VXLAN_GROUP{6} */
		nla_total_size(sizeof(__u32)) +	/* IFLA_VXLAN_LINK */
		nla_total_size(sizeof(struct in6_addr)) + /* IFLA_VXLAN_LOCAL{6} */
>>>>>>> v3.18
=======
		nla_total_size(sizeof(struct in6_addr)) + /* IFLA_VXLAN_GROUP{6} */
		nla_total_size(sizeof(__u32)) +	/* IFLA_VXLAN_LINK */
		nla_total_size(sizeof(struct in6_addr)) + /* IFLA_VXLAN_LOCAL{6} */
>>>>>>> v3.18
		nla_total_size(sizeof(__u8)) +	/* IFLA_VXLAN_TTL */
		nla_total_size(sizeof(__u8)) +	/* IFLA_VXLAN_TOS */
		nla_total_size(sizeof(__u8)) +	/* IFLA_VXLAN_LEARNING */
		nla_total_size(sizeof(__u8)) +	/* IFLA_VXLAN_PROXY */
		nla_total_size(sizeof(__u8)) +	/* IFLA_VXLAN_RSC */
		nla_total_size(sizeof(__u8)) +	/* IFLA_VXLAN_L2MISS */
		nla_total_size(sizeof(__u8)) +	/* IFLA_VXLAN_L3MISS */
		nla_total_size(sizeof(__u32)) +	/* IFLA_VXLAN_AGEING */
		nla_total_size(sizeof(__u32)) +	/* IFLA_VXLAN_LIMIT */
		nla_total_size(sizeof(struct ifla_vxlan_port_range)) +
<<<<<<< HEAD
<<<<<<< HEAD
		nla_total_size(sizeof(__be16))+ /* IFLA_VXLAN_PORT */
=======
=======
>>>>>>> v3.18
		nla_total_size(sizeof(__be16)) + /* IFLA_VXLAN_PORT */
		nla_total_size(sizeof(__u8)) + /* IFLA_VXLAN_UDP_CSUM */
		nla_total_size(sizeof(__u8)) + /* IFLA_VXLAN_UDP_ZERO_CSUM6_TX */
		nla_total_size(sizeof(__u8)) + /* IFLA_VXLAN_UDP_ZERO_CSUM6_RX */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		0;
}

static int vxlan_fill_info(struct sk_buff *skb, const struct net_device *dev)
{
	const struct vxlan_dev *vxlan = netdev_priv(dev);
	const struct vxlan_rdst *dst = &vxlan->default_dst;
	struct ifla_vxlan_port_range ports = {
		.low =  htons(vxlan->port_min),
		.high = htons(vxlan->port_max),
	};

	if (nla_put_u32(skb, IFLA_VXLAN_ID, dst->remote_vni))
		goto nla_put_failure;

<<<<<<< HEAD
<<<<<<< HEAD
	if (dst->remote_ip && nla_put_be32(skb, IFLA_VXLAN_GROUP, dst->remote_ip))
		goto nla_put_failure;
=======
=======
>>>>>>> v3.18
	if (!vxlan_addr_any(&dst->remote_ip)) {
		if (dst->remote_ip.sa.sa_family == AF_INET) {
			if (nla_put_be32(skb, IFLA_VXLAN_GROUP,
					 dst->remote_ip.sin.sin_addr.s_addr))
				goto nla_put_failure;
#if IS_ENABLED(CONFIG_IPV6)
		} else {
			if (nla_put(skb, IFLA_VXLAN_GROUP6, sizeof(struct in6_addr),
				    &dst->remote_ip.sin6.sin6_addr))
				goto nla_put_failure;
#endif
		}
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (dst->remote_ifindex && nla_put_u32(skb, IFLA_VXLAN_LINK, dst->remote_ifindex))
		goto nla_put_failure;

<<<<<<< HEAD
<<<<<<< HEAD
	if (vxlan->saddr && nla_put_be32(skb, IFLA_VXLAN_LOCAL, vxlan->saddr))
		goto nla_put_failure;
=======
=======
>>>>>>> v3.18
	if (!vxlan_addr_any(&vxlan->saddr)) {
		if (vxlan->saddr.sa.sa_family == AF_INET) {
			if (nla_put_be32(skb, IFLA_VXLAN_LOCAL,
					 vxlan->saddr.sin.sin_addr.s_addr))
				goto nla_put_failure;
#if IS_ENABLED(CONFIG_IPV6)
		} else {
			if (nla_put(skb, IFLA_VXLAN_LOCAL6, sizeof(struct in6_addr),
				    &vxlan->saddr.sin6.sin6_addr))
				goto nla_put_failure;
#endif
		}
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (nla_put_u8(skb, IFLA_VXLAN_TTL, vxlan->ttl) ||
	    nla_put_u8(skb, IFLA_VXLAN_TOS, vxlan->tos) ||
	    nla_put_u8(skb, IFLA_VXLAN_LEARNING,
			!!(vxlan->flags & VXLAN_F_LEARN)) ||
	    nla_put_u8(skb, IFLA_VXLAN_PROXY,
			!!(vxlan->flags & VXLAN_F_PROXY)) ||
	    nla_put_u8(skb, IFLA_VXLAN_RSC, !!(vxlan->flags & VXLAN_F_RSC)) ||
	    nla_put_u8(skb, IFLA_VXLAN_L2MISS,
			!!(vxlan->flags & VXLAN_F_L2MISS)) ||
	    nla_put_u8(skb, IFLA_VXLAN_L3MISS,
			!!(vxlan->flags & VXLAN_F_L3MISS)) ||
	    nla_put_u32(skb, IFLA_VXLAN_AGEING, vxlan->age_interval) ||
	    nla_put_u32(skb, IFLA_VXLAN_LIMIT, vxlan->addrmax) ||
<<<<<<< HEAD
<<<<<<< HEAD
	    nla_put_be16(skb, IFLA_VXLAN_PORT, vxlan->dst_port))
=======
=======
>>>>>>> v3.18
	    nla_put_be16(skb, IFLA_VXLAN_PORT, vxlan->dst_port) ||
	    nla_put_u8(skb, IFLA_VXLAN_UDP_CSUM,
			!!(vxlan->flags & VXLAN_F_UDP_CSUM)) ||
	    nla_put_u8(skb, IFLA_VXLAN_UDP_ZERO_CSUM6_TX,
			!!(vxlan->flags & VXLAN_F_UDP_ZERO_CSUM6_TX)) ||
	    nla_put_u8(skb, IFLA_VXLAN_UDP_ZERO_CSUM6_RX,
			!!(vxlan->flags & VXLAN_F_UDP_ZERO_CSUM6_RX)))
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		goto nla_put_failure;

	if (nla_put(skb, IFLA_VXLAN_PORT_RANGE, sizeof(ports), &ports))
		goto nla_put_failure;

	return 0;

nla_put_failure:
	return -EMSGSIZE;
}

static struct rtnl_link_ops vxlan_link_ops __read_mostly = {
	.kind		= "vxlan",
	.maxtype	= IFLA_VXLAN_MAX,
	.policy		= vxlan_policy,
	.priv_size	= sizeof(struct vxlan_dev),
	.setup		= vxlan_setup,
	.validate	= vxlan_validate,
	.newlink	= vxlan_newlink,
	.dellink	= vxlan_dellink,
	.get_size	= vxlan_get_size,
	.fill_info	= vxlan_fill_info,
};

<<<<<<< HEAD
<<<<<<< HEAD
static __net_init int vxlan_init_net(struct net *net)
{
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);
	struct sock *sk;
	struct sockaddr_in vxlan_addr = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = htonl(INADDR_ANY),
	};
	int rc;
	unsigned h;

	/* Create UDP socket for encapsulation receive. */
	rc = sock_create_kern(AF_INET, SOCK_DGRAM, IPPROTO_UDP, &vn->sock);
	if (rc < 0) {
		pr_debug("UDP socket create failed\n");
		return rc;
	}
	/* Put in proper namespace */
	sk = vn->sock->sk;
	sk_change_net(sk, net);

	vxlan_addr.sin_port = htons(vxlan_port);

	rc = kernel_bind(vn->sock, (struct sockaddr *) &vxlan_addr,
			 sizeof(vxlan_addr));
	if (rc < 0) {
		pr_debug("bind for UDP socket %pI4:%u (%d)\n",
			 &vxlan_addr.sin_addr, ntohs(vxlan_addr.sin_port), rc);
		sk_release_kernel(sk);
		vn->sock = NULL;
		return rc;
	}

	/* Disable multicast loopback */
	inet_sk(sk)->mc_loop = 0;

	/* Mark socket as an encapsulation socket. */
	udp_sk(sk)->encap_type = 1;
	udp_sk(sk)->encap_rcv = vxlan_udp_encap_recv;
	udp_encap_enable();

	for (h = 0; h < VNI_HASH_SIZE; ++h)
		INIT_HLIST_HEAD(&vn->vni_list[h]);
=======
=======
>>>>>>> v3.18
static void vxlan_handle_lowerdev_unregister(struct vxlan_net *vn,
					     struct net_device *dev)
{
	struct vxlan_dev *vxlan, *next;
	LIST_HEAD(list_kill);

	list_for_each_entry_safe(vxlan, next, &vn->vxlan_list, next) {
		struct vxlan_rdst *dst = &vxlan->default_dst;

		/* In case we created vxlan device with carrier
		 * and we loose the carrier due to module unload
		 * we also need to remove vxlan device. In other
		 * cases, it's not necessary and remote_ifindex
		 * is 0 here, so no matches.
		 */
		if (dst->remote_ifindex == dev->ifindex)
			vxlan_dellink(vxlan->dev, &list_kill);
	}

	unregister_netdevice_many(&list_kill);
}

static int vxlan_lowerdev_event(struct notifier_block *unused,
				unsigned long event, void *ptr)
{
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
	struct vxlan_net *vn = net_generic(dev_net(dev), vxlan_net_id);

	if (event == NETDEV_UNREGISTER)
		vxlan_handle_lowerdev_unregister(vn, dev);

	return NOTIFY_DONE;
}

static struct notifier_block vxlan_notifier_block __read_mostly = {
	.notifier_call = vxlan_lowerdev_event,
};

static __net_init int vxlan_init_net(struct net *net)
{
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);
	unsigned int h;

	INIT_LIST_HEAD(&vn->vxlan_list);
	spin_lock_init(&vn->sock_lock);

	for (h = 0; h < PORT_HASH_SIZE; ++h)
		INIT_HLIST_HEAD(&vn->sock_list[h]);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static __net_exit void vxlan_exit_net(struct net *net)
{
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);
	struct vxlan_dev *vxlan;
	unsigned h;

	rtnl_lock();
	for (h = 0; h < VNI_HASH_SIZE; ++h)
		hlist_for_each_entry(vxlan, &vn->vni_list[h], hlist)
			dev_close(vxlan->dev);
	rtnl_unlock();

	if (vn->sock) {
		sk_release_kernel(vn->sock->sk);
		vn->sock = NULL;
	}
=======
=======
>>>>>>> v3.18
static void __net_exit vxlan_exit_net(struct net *net)
{
	struct vxlan_net *vn = net_generic(net, vxlan_net_id);
	struct vxlan_dev *vxlan, *next;
	struct net_device *dev, *aux;
	LIST_HEAD(list);

	rtnl_lock();
	for_each_netdev_safe(net, dev, aux)
		if (dev->rtnl_link_ops == &vxlan_link_ops)
			unregister_netdevice_queue(dev, &list);

	list_for_each_entry_safe(vxlan, next, &vn->vxlan_list, next) {
		/* If vxlan->dev is in the same netns, it has already been added
		 * to the list by the previous loop.
		 */
		if (!net_eq(dev_net(vxlan->dev), net))
			unregister_netdevice_queue(dev, &list);
	}

	unregister_netdevice_many(&list);
	rtnl_unlock();
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static struct pernet_operations vxlan_net_ops = {
	.init = vxlan_init_net,
	.exit = vxlan_exit_net,
	.id   = &vxlan_net_id,
	.size = sizeof(struct vxlan_net),
};

static int __init vxlan_init_module(void)
{
	int rc;

<<<<<<< HEAD
<<<<<<< HEAD
	get_random_bytes(&vxlan_salt, sizeof(vxlan_salt));

	rc = register_pernet_device(&vxlan_net_ops);
	if (rc)
		goto out1;

	rc = rtnl_link_register(&vxlan_link_ops);
	if (rc)
		goto out2;

	return 0;

out2:
	unregister_pernet_device(&vxlan_net_ops);
out1:
	return rc;
}
module_init(vxlan_init_module);
=======
=======
>>>>>>> v3.18
	vxlan_wq = alloc_workqueue("vxlan", 0, 0);
	if (!vxlan_wq)
		return -ENOMEM;

	get_random_bytes(&vxlan_salt, sizeof(vxlan_salt));

	rc = register_pernet_subsys(&vxlan_net_ops);
	if (rc)
		goto out1;

	rc = register_netdevice_notifier(&vxlan_notifier_block);
	if (rc)
		goto out2;

	rc = rtnl_link_register(&vxlan_link_ops);
	if (rc)
		goto out3;

	return 0;
out3:
	unregister_netdevice_notifier(&vxlan_notifier_block);
out2:
	unregister_pernet_subsys(&vxlan_net_ops);
out1:
	destroy_workqueue(vxlan_wq);
	return rc;
}
late_initcall(vxlan_init_module);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

static void __exit vxlan_cleanup_module(void)
{
	rtnl_link_unregister(&vxlan_link_ops);
<<<<<<< HEAD
<<<<<<< HEAD
	unregister_pernet_device(&vxlan_net_ops);
	rcu_barrier();
=======
=======
>>>>>>> v3.18
	unregister_netdevice_notifier(&vxlan_notifier_block);
	destroy_workqueue(vxlan_wq);
	unregister_pernet_subsys(&vxlan_net_ops);
	/* rcu_barrier() is called by netns */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}
module_exit(vxlan_cleanup_module);

MODULE_LICENSE("GPL");
MODULE_VERSION(VXLAN_VERSION);
MODULE_AUTHOR("Stephen Hemminger <stephen@networkplumber.org>");
<<<<<<< HEAD
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("Driver for VXLAN encapsulated traffic");
>>>>>>> v3.18
=======
MODULE_DESCRIPTION("Driver for VXLAN encapsulated traffic");
>>>>>>> v3.18
MODULE_ALIAS_RTNL_LINK("vxlan");
