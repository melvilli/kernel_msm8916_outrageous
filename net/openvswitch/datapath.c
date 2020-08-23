/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2007-2012 Nicira, Inc.
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

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/if_arp.h>
#include <linux/if_vlan.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/jhash.h>
#include <linux/delay.h>
#include <linux/time.h>
#include <linux/etherdevice.h>
#include <linux/genetlink.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/mutex.h>
#include <linux/percpu.h>
#include <linux/rcupdate.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/ethtool.h>
#include <linux/wait.h>
#include <asm/div64.h>
#include <linux/highmem.h>
#include <linux/netfilter_bridge.h>
#include <linux/netfilter_ipv4.h>
#include <linux/inetdevice.h>
#include <linux/list.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/lockdep.h>
#include <linux/openvswitch.h>
#include <linux/rculist.h>
#include <linux/dmi.h>
#include <linux/workqueue.h>
=======
#include <linux/openvswitch.h>
#include <linux/rculist.h>
#include <linux/dmi.h>
>>>>>>> v3.18
=======
#include <linux/openvswitch.h>
#include <linux/rculist.h>
#include <linux/dmi.h>
>>>>>>> v3.18
#include <net/genetlink.h>
#include <net/net_namespace.h>
#include <net/netns/generic.h>

#include "datapath.h"
#include "flow.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "vport-internal_dev.h"
#include "vport-netdev.h"


#define REHASH_FLOW_INTERVAL (10 * 60 * HZ)
static void rehash_flow_table(struct work_struct *work);
static DECLARE_DELAYED_WORK(rehash_flow_wq, rehash_flow_table);

int ovs_net_id __read_mostly;

static void ovs_notify(struct sk_buff *skb, struct genl_info *info,
		       struct genl_multicast_group *grp)
{
	genl_notify(skb, genl_info_net(info), info->snd_portid,
		    grp->id, info->nlhdr, GFP_KERNEL);
=======
=======
>>>>>>> v3.18
#include "flow_table.h"
#include "flow_netlink.h"
#include "vport-internal_dev.h"
#include "vport-netdev.h"

int ovs_net_id __read_mostly;

static struct genl_family dp_packet_genl_family;
static struct genl_family dp_flow_genl_family;
static struct genl_family dp_datapath_genl_family;

static const struct genl_multicast_group ovs_dp_flow_multicast_group = {
	.name = OVS_FLOW_MCGROUP,
};

static const struct genl_multicast_group ovs_dp_datapath_multicast_group = {
	.name = OVS_DATAPATH_MCGROUP,
};

static const struct genl_multicast_group ovs_dp_vport_multicast_group = {
	.name = OVS_VPORT_MCGROUP,
};

/* Check if need to build a reply message.
 * OVS userspace sets the NLM_F_ECHO flag if it needs the reply. */
static bool ovs_must_notify(struct genl_family *family, struct genl_info *info,
			    unsigned int group)
{
	return info->nlhdr->nlmsg_flags & NLM_F_ECHO ||
	       genl_has_listeners(family, genl_info_net(info)->genl_sock,
				  group);
}

static void ovs_notify(struct genl_family *family,
		       struct sk_buff *skb, struct genl_info *info)
{
	genl_notify(family, skb, genl_info_net(info), info->snd_portid,
		    0, info->nlhdr, GFP_KERNEL);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/**
 * DOC: Locking:
 *
 * All writes e.g. Writes to device state (add/remove datapath, port, set
 * operations on vports, etc.), Writes to other state (flow table
 * modifications, set miscellaneous datapath parameters, etc.) are protected
 * by ovs_lock.
 *
 * Reads are protected by RCU.
 *
 * There are a few special cases (mostly stats) that have their own
 * synchronization but they nest under all of above and don't interact with
 * each other.
 *
 * The RTNL lock nests inside ovs_mutex.
 */

static DEFINE_MUTEX(ovs_mutex);

void ovs_lock(void)
{
	mutex_lock(&ovs_mutex);
}

void ovs_unlock(void)
{
	mutex_unlock(&ovs_mutex);
}

#ifdef CONFIG_LOCKDEP
int lockdep_ovsl_is_held(void)
{
	if (debug_locks)
		return lockdep_is_held(&ovs_mutex);
	else
		return 1;
}
#endif

static struct vport *new_vport(const struct vport_parms *);
<<<<<<< HEAD
<<<<<<< HEAD
static int queue_gso_packets(struct net *, int dp_ifindex, struct sk_buff *,
			     const struct dp_upcall_info *);
static int queue_userspace_packet(struct net *, int dp_ifindex,
				  struct sk_buff *,
=======
static int queue_gso_packets(struct datapath *dp, struct sk_buff *,
			     const struct dp_upcall_info *);
static int queue_userspace_packet(struct datapath *dp, struct sk_buff *,
>>>>>>> v3.18
=======
static int queue_gso_packets(struct datapath *dp, struct sk_buff *,
			     const struct dp_upcall_info *);
static int queue_userspace_packet(struct datapath *dp, struct sk_buff *,
>>>>>>> v3.18
				  const struct dp_upcall_info *);

/* Must be called with rcu_read_lock or ovs_mutex. */
static struct datapath *get_dp(struct net *net, int dp_ifindex)
{
	struct datapath *dp = NULL;
	struct net_device *dev;

	rcu_read_lock();
	dev = dev_get_by_index_rcu(net, dp_ifindex);
	if (dev) {
		struct vport *vport = ovs_internal_dev_get_vport(dev);
		if (vport)
			dp = vport->dp;
	}
	rcu_read_unlock();

	return dp;
}

/* Must be called with rcu_read_lock or ovs_mutex. */
const char *ovs_dp_name(const struct datapath *dp)
{
	struct vport *vport = ovs_vport_ovsl_rcu(dp, OVSP_LOCAL);
	return vport->ops->get_name(vport);
}

static int get_dpifindex(struct datapath *dp)
{
	struct vport *local;
	int ifindex;

	rcu_read_lock();

	local = ovs_vport_rcu(dp, OVSP_LOCAL);
	if (local)
		ifindex = netdev_vport_priv(local)->dev->ifindex;
	else
		ifindex = 0;

	rcu_read_unlock();

	return ifindex;
}

static void destroy_dp_rcu(struct rcu_head *rcu)
{
	struct datapath *dp = container_of(rcu, struct datapath, rcu);

<<<<<<< HEAD
<<<<<<< HEAD
	ovs_flow_tbl_destroy((__force struct flow_table *)dp->table);
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	free_percpu(dp->stats_percpu);
	release_net(ovs_dp_get_net(dp));
	kfree(dp->ports);
	kfree(dp);
}

static struct hlist_head *vport_hash_bucket(const struct datapath *dp,
					    u16 port_no)
{
	return &dp->ports[port_no & (DP_VPORT_HASH_BUCKETS - 1)];
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
/* Called with ovs_mutex or RCU read lock. */
>>>>>>> v3.18
=======
/* Called with ovs_mutex or RCU read lock. */
>>>>>>> v3.18
struct vport *ovs_lookup_vport(const struct datapath *dp, u16 port_no)
{
	struct vport *vport;
	struct hlist_head *head;

	head = vport_hash_bucket(dp, port_no);
	hlist_for_each_entry_rcu(vport, head, dp_hash_node) {
		if (vport->port_no == port_no)
			return vport;
	}
	return NULL;
}

/* Called with ovs_mutex. */
static struct vport *new_vport(const struct vport_parms *parms)
{
	struct vport *vport;

	vport = ovs_vport_add(parms);
	if (!IS_ERR(vport)) {
		struct datapath *dp = parms->dp;
		struct hlist_head *head = vport_hash_bucket(dp, vport->port_no);

		hlist_add_head_rcu(&vport->dp_hash_node, head);
	}
	return vport;
}

void ovs_dp_detach_port(struct vport *p)
{
	ASSERT_OVSL();

	/* First drop references to device. */
	hlist_del_rcu(&p->dp_hash_node);

	/* Then destroy it. */
	ovs_vport_del(p);
}

/* Must be called with rcu_read_lock. */
<<<<<<< HEAD
<<<<<<< HEAD
void ovs_dp_process_received_packet(struct vport *p, struct sk_buff *skb)
{
	struct datapath *dp = p->dp;
	struct sw_flow *flow;
	struct dp_stats_percpu *stats;
	struct sw_flow_key key;
	u64 *stats_counter;
	int error;
	int key_len;

	stats = this_cpu_ptr(dp->stats_percpu);

	/* Extract flow from 'skb' into 'key'. */
	error = ovs_flow_extract(skb, p->port_no, &key, &key_len);
	if (unlikely(error)) {
		kfree_skb(skb);
		return;
	}

	/* Look up flow. */
	flow = ovs_flow_tbl_lookup(rcu_dereference(dp->table), &key, key_len);
	if (unlikely(!flow)) {
		struct dp_upcall_info upcall;

		upcall.cmd = OVS_PACKET_CMD_MISS;
		upcall.key = &key;
		upcall.userdata = NULL;
		upcall.portid = p->upcall_portid;
		ovs_dp_upcall(dp, skb, &upcall);
		consume_skb(skb);
=======
=======
>>>>>>> v3.18
void ovs_dp_process_packet(struct sk_buff *skb, struct sw_flow_key *key)
{
	const struct vport *p = OVS_CB(skb)->input_vport;
	struct datapath *dp = p->dp;
	struct sw_flow *flow;
	struct dp_stats_percpu *stats;
	u64 *stats_counter;
	u32 n_mask_hit;

	stats = this_cpu_ptr(dp->stats_percpu);

	/* Look up flow. */
	flow = ovs_flow_tbl_lookup_stats(&dp->table, key, &n_mask_hit);
	if (unlikely(!flow)) {
		struct dp_upcall_info upcall;
		int error;

		upcall.cmd = OVS_PACKET_CMD_MISS;
		upcall.key = key;
		upcall.userdata = NULL;
		upcall.portid = ovs_vport_find_upcall_portid(p, skb);
		error = ovs_dp_upcall(dp, skb, &upcall);
		if (unlikely(error))
			kfree_skb(skb);
		else
			consume_skb(skb);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		stats_counter = &stats->n_missed;
		goto out;
	}

	OVS_CB(skb)->flow = flow;

<<<<<<< HEAD
<<<<<<< HEAD
	stats_counter = &stats->n_hit;
	ovs_flow_used(OVS_CB(skb)->flow, skb);
	ovs_execute_actions(dp, skb);

out:
	/* Update datapath statistics. */
	u64_stats_update_begin(&stats->sync);
	(*stats_counter)++;
	u64_stats_update_end(&stats->sync);
}

static struct genl_family dp_packet_genl_family = {
	.id = GENL_ID_GENERATE,
	.hdrsize = sizeof(struct ovs_header),
	.name = OVS_PACKET_FAMILY,
	.version = OVS_PACKET_VERSION,
	.maxattr = OVS_PACKET_ATTR_MAX,
	.netnsok = true,
	.parallel_ops = true,
};

=======
=======
>>>>>>> v3.18
	ovs_flow_stats_update(OVS_CB(skb)->flow, key->tp.flags, skb);
	ovs_execute_actions(dp, skb, key);
	stats_counter = &stats->n_hit;

out:
	/* Update datapath statistics. */
	u64_stats_update_begin(&stats->syncp);
	(*stats_counter)++;
	stats->n_mask_hit += n_mask_hit;
	u64_stats_update_end(&stats->syncp);
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
int ovs_dp_upcall(struct datapath *dp, struct sk_buff *skb,
		  const struct dp_upcall_info *upcall_info)
{
	struct dp_stats_percpu *stats;
<<<<<<< HEAD
<<<<<<< HEAD
	int dp_ifindex;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	int err;

	if (upcall_info->portid == 0) {
		err = -ENOTCONN;
		goto err;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	dp_ifindex = get_dpifindex(dp);
	if (!dp_ifindex) {
		err = -ENODEV;
		goto err;
	}

	if (!skb_is_gso(skb))
		err = queue_userspace_packet(ovs_dp_get_net(dp), dp_ifindex, skb, upcall_info);
	else
		err = queue_gso_packets(ovs_dp_get_net(dp), dp_ifindex, skb, upcall_info);
=======
=======
>>>>>>> v3.18
	if (!skb_is_gso(skb))
		err = queue_userspace_packet(dp, skb, upcall_info);
	else
		err = queue_gso_packets(dp, skb, upcall_info);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (err)
		goto err;

	return 0;

err:
	stats = this_cpu_ptr(dp->stats_percpu);

<<<<<<< HEAD
<<<<<<< HEAD
	u64_stats_update_begin(&stats->sync);
	stats->n_lost++;
	u64_stats_update_end(&stats->sync);
=======
	u64_stats_update_begin(&stats->syncp);
	stats->n_lost++;
	u64_stats_update_end(&stats->syncp);
>>>>>>> v3.18
=======
	u64_stats_update_begin(&stats->syncp);
	stats->n_lost++;
	u64_stats_update_end(&stats->syncp);
>>>>>>> v3.18

	return err;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int queue_gso_packets(struct net *net, int dp_ifindex,
			     struct sk_buff *skb,
=======
static int queue_gso_packets(struct datapath *dp, struct sk_buff *skb,
>>>>>>> v3.18
=======
static int queue_gso_packets(struct datapath *dp, struct sk_buff *skb,
>>>>>>> v3.18
			     const struct dp_upcall_info *upcall_info)
{
	unsigned short gso_type = skb_shinfo(skb)->gso_type;
	struct dp_upcall_info later_info;
	struct sw_flow_key later_key;
	struct sk_buff *segs, *nskb;
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
	segs = __skb_gso_segment(skb, NETIF_F_SG | NETIF_F_HW_CSUM, false);
	if (IS_ERR(segs))
		return PTR_ERR(segs);
=======
=======
>>>>>>> v3.18
	segs = __skb_gso_segment(skb, NETIF_F_SG, false);
	if (IS_ERR(segs))
		return PTR_ERR(segs);
	if (segs == NULL)
		return -EINVAL;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* Queue all of the segments. */
	skb = segs;
	do {
<<<<<<< HEAD
<<<<<<< HEAD
		err = queue_userspace_packet(net, dp_ifindex, skb, upcall_info);
=======
		err = queue_userspace_packet(dp, skb, upcall_info);
>>>>>>> v3.18
=======
		err = queue_userspace_packet(dp, skb, upcall_info);
>>>>>>> v3.18
		if (err)
			break;

		if (skb == segs && gso_type & SKB_GSO_UDP) {
			/* The initial flow key extracted by ovs_flow_extract()
			 * in this case is for a first fragment, so we need to
			 * properly mark later fragments.
			 */
			later_key = *upcall_info->key;
			later_key.ip.frag = OVS_FRAG_TYPE_LATER;

			later_info = *upcall_info;
			later_info.key = &later_key;
			upcall_info = &later_info;
		}
	} while ((skb = skb->next));

	/* Free all of the segments. */
	skb = segs;
	do {
		nskb = skb->next;
		if (err)
			kfree_skb(skb);
		else
			consume_skb(skb);
	} while ((skb = nskb));
	return err;
}

static size_t key_attr_size(void)
{
	return    nla_total_size(4)   /* OVS_KEY_ATTR_PRIORITY */
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
		+ nla_total_size(0)   /* OVS_KEY_ATTR_TUNNEL */
		  + nla_total_size(8)   /* OVS_TUNNEL_KEY_ATTR_ID */
		  + nla_total_size(4)   /* OVS_TUNNEL_KEY_ATTR_IPV4_SRC */
		  + nla_total_size(4)   /* OVS_TUNNEL_KEY_ATTR_IPV4_DST */
		  + nla_total_size(1)   /* OVS_TUNNEL_KEY_ATTR_TOS */
		  + nla_total_size(1)   /* OVS_TUNNEL_KEY_ATTR_TTL */
		  + nla_total_size(0)   /* OVS_TUNNEL_KEY_ATTR_DONT_FRAGMENT */
		  + nla_total_size(0)   /* OVS_TUNNEL_KEY_ATTR_CSUM */
		  + nla_total_size(0)   /* OVS_TUNNEL_KEY_ATTR_OAM */
		  + nla_total_size(256)   /* OVS_TUNNEL_KEY_ATTR_GENEVE_OPTS */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		+ nla_total_size(4)   /* OVS_KEY_ATTR_IN_PORT */
		+ nla_total_size(4)   /* OVS_KEY_ATTR_SKB_MARK */
		+ nla_total_size(12)  /* OVS_KEY_ATTR_ETHERNET */
		+ nla_total_size(2)   /* OVS_KEY_ATTR_ETHERTYPE */
		+ nla_total_size(4)   /* OVS_KEY_ATTR_8021Q */
		+ nla_total_size(0)   /* OVS_KEY_ATTR_ENCAP */
		+ nla_total_size(2)   /* OVS_KEY_ATTR_ETHERTYPE */
		+ nla_total_size(40)  /* OVS_KEY_ATTR_IPV6 */
		+ nla_total_size(2)   /* OVS_KEY_ATTR_ICMPV6 */
		+ nla_total_size(28); /* OVS_KEY_ATTR_ND */
}

<<<<<<< HEAD
<<<<<<< HEAD
static size_t upcall_msg_size(const struct sk_buff *skb,
			      const struct nlattr *userdata)
{
	size_t size = NLMSG_ALIGN(sizeof(struct ovs_header))
		+ nla_total_size(skb->len) /* OVS_PACKET_ATTR_PACKET */
=======
=======
>>>>>>> v3.18
static size_t upcall_msg_size(const struct nlattr *userdata,
			      unsigned int hdrlen)
{
	size_t size = NLMSG_ALIGN(sizeof(struct ovs_header))
		+ nla_total_size(hdrlen) /* OVS_PACKET_ATTR_PACKET */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		+ nla_total_size(key_attr_size()); /* OVS_PACKET_ATTR_KEY */

	/* OVS_PACKET_ATTR_USERDATA */
	if (userdata)
		size += NLA_ALIGN(userdata->nla_len);

	return size;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int queue_userspace_packet(struct net *net, int dp_ifindex,
				  struct sk_buff *skb,
=======
static int queue_userspace_packet(struct datapath *dp, struct sk_buff *skb,
>>>>>>> v3.18
=======
static int queue_userspace_packet(struct datapath *dp, struct sk_buff *skb,
>>>>>>> v3.18
				  const struct dp_upcall_info *upcall_info)
{
	struct ovs_header *upcall;
	struct sk_buff *nskb = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
	struct sk_buff *user_skb; /* to be queued to userspace */
	struct nlattr *nla;
	int err;
=======
=======
>>>>>>> v3.18
	struct sk_buff *user_skb = NULL; /* to be queued to userspace */
	struct nlattr *nla;
	struct genl_info info = {
		.dst_sk = ovs_dp_get_net(dp)->genl_sock,
		.snd_portid = upcall_info->portid,
	};
	size_t len;
	unsigned int hlen;
	int err, dp_ifindex;

	dp_ifindex = get_dpifindex(dp);
	if (!dp_ifindex)
		return -ENODEV;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (vlan_tx_tag_present(skb)) {
		nskb = skb_clone(skb, GFP_ATOMIC);
		if (!nskb)
			return -ENOMEM;

		nskb = __vlan_put_tag(nskb, nskb->vlan_proto, vlan_tx_tag_get(nskb));
		if (!nskb)
			return -ENOMEM;

		nskb->vlan_tci = 0;
		skb = nskb;
	}

	if (nla_attr_size(skb->len) > USHRT_MAX) {
		err = -EFBIG;
		goto out;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	user_skb = genlmsg_new(upcall_msg_size(skb, upcall_info->userdata), GFP_ATOMIC);
=======
=======
>>>>>>> v3.18
	/* Complete checksum if needed */
	if (skb->ip_summed == CHECKSUM_PARTIAL &&
	    (err = skb_checksum_help(skb)))
		goto out;

	/* Older versions of OVS user space enforce alignment of the last
	 * Netlink attribute to NLA_ALIGNTO which would require extensive
	 * padding logic. Only perform zerocopy if padding is not required.
	 */
	if (dp->user_features & OVS_DP_F_UNALIGNED)
		hlen = skb_zerocopy_headlen(skb);
	else
		hlen = skb->len;

	len = upcall_msg_size(upcall_info->userdata, hlen);
	user_skb = genlmsg_new_unicast(len, &info, GFP_ATOMIC);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (!user_skb) {
		err = -ENOMEM;
		goto out;
	}

	upcall = genlmsg_put(user_skb, 0, 0, &dp_packet_genl_family,
			     0, upcall_info->cmd);
	upcall->dp_ifindex = dp_ifindex;

	nla = nla_nest_start(user_skb, OVS_PACKET_ATTR_KEY);
<<<<<<< HEAD
<<<<<<< HEAD
	ovs_flow_to_nlattrs(upcall_info->key, user_skb);
=======
	err = ovs_nla_put_flow(upcall_info->key, upcall_info->key, user_skb);
	BUG_ON(err);
>>>>>>> v3.18
=======
	err = ovs_nla_put_flow(upcall_info->key, upcall_info->key, user_skb);
	BUG_ON(err);
>>>>>>> v3.18
	nla_nest_end(user_skb, nla);

	if (upcall_info->userdata)
		__nla_put(user_skb, OVS_PACKET_ATTR_USERDATA,
			  nla_len(upcall_info->userdata),
			  nla_data(upcall_info->userdata));

<<<<<<< HEAD
<<<<<<< HEAD
	nla = __nla_reserve(user_skb, OVS_PACKET_ATTR_PACKET, skb->len);

	skb_copy_and_csum_dev(skb, nla_data(nla));

	genlmsg_end(user_skb, upcall);
	err = genlmsg_unicast(net, user_skb, upcall_info->portid);

out:
	kfree_skb(nskb);
	return err;
}

/* Called with ovs_mutex. */
static int flush_flows(struct datapath *dp)
{
	struct flow_table *old_table;
	struct flow_table *new_table;

	old_table = ovsl_dereference(dp->table);
	new_table = ovs_flow_tbl_alloc(TBL_MIN_BUCKETS);
	if (!new_table)
		return -ENOMEM;

	rcu_assign_pointer(dp->table, new_table);

	ovs_flow_tbl_deferred_destroy(old_table);
	return 0;
}

static int validate_actions(const struct nlattr *attr,
				const struct sw_flow_key *key, int depth);

static int validate_sample(const struct nlattr *attr,
				const struct sw_flow_key *key, int depth)
{
	const struct nlattr *attrs[OVS_SAMPLE_ATTR_MAX + 1];
	const struct nlattr *probability, *actions;
	const struct nlattr *a;
	int rem;

	memset(attrs, 0, sizeof(attrs));
	nla_for_each_nested(a, attr, rem) {
		int type = nla_type(a);
		if (!type || type > OVS_SAMPLE_ATTR_MAX || attrs[type])
			return -EINVAL;
		attrs[type] = a;
	}
	if (rem)
		return -EINVAL;

	probability = attrs[OVS_SAMPLE_ATTR_PROBABILITY];
	if (!probability || nla_len(probability) != sizeof(u32))
		return -EINVAL;

	actions = attrs[OVS_SAMPLE_ATTR_ACTIONS];
	if (!actions || (nla_len(actions) && nla_len(actions) < NLA_HDRLEN))
		return -EINVAL;
	return validate_actions(actions, key, depth + 1);
}

static int validate_tp_port(const struct sw_flow_key *flow_key)
{
	if (flow_key->eth.type == htons(ETH_P_IP)) {
		if (flow_key->ipv4.tp.src || flow_key->ipv4.tp.dst)
			return 0;
	} else if (flow_key->eth.type == htons(ETH_P_IPV6)) {
		if (flow_key->ipv6.tp.src || flow_key->ipv6.tp.dst)
			return 0;
	}

	return -EINVAL;
}

static int validate_set(const struct nlattr *a,
			const struct sw_flow_key *flow_key)
{
	const struct nlattr *ovs_key = nla_data(a);
	int key_type = nla_type(ovs_key);

	/* There can be only one key in a action */
	if (nla_total_size(nla_len(ovs_key)) != nla_len(a))
		return -EINVAL;

	if (key_type > OVS_KEY_ATTR_MAX ||
	    nla_len(ovs_key) != ovs_key_lens[key_type])
		return -EINVAL;

	switch (key_type) {
	const struct ovs_key_ipv4 *ipv4_key;
	const struct ovs_key_ipv6 *ipv6_key;

	case OVS_KEY_ATTR_PRIORITY:
	case OVS_KEY_ATTR_SKB_MARK:
	case OVS_KEY_ATTR_ETHERNET:
		break;

	case OVS_KEY_ATTR_IPV4:
		if (flow_key->eth.type != htons(ETH_P_IP))
			return -EINVAL;

		if (!flow_key->ip.proto)
			return -EINVAL;

		ipv4_key = nla_data(ovs_key);
		if (ipv4_key->ipv4_proto != flow_key->ip.proto)
			return -EINVAL;

		if (ipv4_key->ipv4_frag != flow_key->ip.frag)
			return -EINVAL;

		break;

	case OVS_KEY_ATTR_IPV6:
		if (flow_key->eth.type != htons(ETH_P_IPV6))
			return -EINVAL;

		if (!flow_key->ip.proto)
			return -EINVAL;

		ipv6_key = nla_data(ovs_key);
		if (ipv6_key->ipv6_proto != flow_key->ip.proto)
			return -EINVAL;

		if (ipv6_key->ipv6_frag != flow_key->ip.frag)
			return -EINVAL;

		if (ntohl(ipv6_key->ipv6_label) & 0xFFF00000)
			return -EINVAL;

		break;

	case OVS_KEY_ATTR_TCP:
		if (flow_key->ip.proto != IPPROTO_TCP)
			return -EINVAL;

		return validate_tp_port(flow_key);

	case OVS_KEY_ATTR_UDP:
		if (flow_key->ip.proto != IPPROTO_UDP)
			return -EINVAL;

		return validate_tp_port(flow_key);

	default:
		return -EINVAL;
	}

	return 0;
}

static int validate_userspace(const struct nlattr *attr)
{
	static const struct nla_policy userspace_policy[OVS_USERSPACE_ATTR_MAX + 1] =	{
		[OVS_USERSPACE_ATTR_PID] = {.type = NLA_U32 },
		[OVS_USERSPACE_ATTR_USERDATA] = {.type = NLA_UNSPEC },
	};
	struct nlattr *a[OVS_USERSPACE_ATTR_MAX + 1];
	int error;

	error = nla_parse_nested(a, OVS_USERSPACE_ATTR_MAX,
				 attr, userspace_policy);
	if (error)
		return error;

	if (!a[OVS_USERSPACE_ATTR_PID] ||
	    !nla_get_u32(a[OVS_USERSPACE_ATTR_PID]))
		return -EINVAL;

	return 0;
}

static int validate_actions(const struct nlattr *attr,
				const struct sw_flow_key *key,  int depth)
{
	const struct nlattr *a;
	int rem, err;

	if (depth >= SAMPLE_ACTION_DEPTH)
		return -EOVERFLOW;

	nla_for_each_nested(a, attr, rem) {
		/* Expected argument lengths, (u32)-1 for variable length. */
		static const u32 action_lens[OVS_ACTION_ATTR_MAX + 1] = {
			[OVS_ACTION_ATTR_OUTPUT] = sizeof(u32),
			[OVS_ACTION_ATTR_USERSPACE] = (u32)-1,
			[OVS_ACTION_ATTR_PUSH_VLAN] = sizeof(struct ovs_action_push_vlan),
			[OVS_ACTION_ATTR_POP_VLAN] = 0,
			[OVS_ACTION_ATTR_SET] = (u32)-1,
			[OVS_ACTION_ATTR_SAMPLE] = (u32)-1
		};
		const struct ovs_action_push_vlan *vlan;
		int type = nla_type(a);

		if (type > OVS_ACTION_ATTR_MAX ||
		    (action_lens[type] != nla_len(a) &&
		     action_lens[type] != (u32)-1))
			return -EINVAL;

		switch (type) {
		case OVS_ACTION_ATTR_UNSPEC:
			return -EINVAL;

		case OVS_ACTION_ATTR_USERSPACE:
			err = validate_userspace(a);
			if (err)
				return err;
			break;

		case OVS_ACTION_ATTR_OUTPUT:
			if (nla_get_u32(a) >= DP_MAX_PORTS)
				return -EINVAL;
			break;


		case OVS_ACTION_ATTR_POP_VLAN:
			break;

		case OVS_ACTION_ATTR_PUSH_VLAN:
			vlan = nla_data(a);
			if (vlan->vlan_tpid != htons(ETH_P_8021Q))
				return -EINVAL;
			if (!(vlan->vlan_tci & htons(VLAN_TAG_PRESENT)))
				return -EINVAL;
			break;

		case OVS_ACTION_ATTR_SET:
			err = validate_set(a, key);
			if (err)
				return err;
			break;

		case OVS_ACTION_ATTR_SAMPLE:
			err = validate_sample(a, key, depth);
			if (err)
				return err;
			break;

		default:
			return -EINVAL;
		}
	}

	if (rem > 0)
		return -EINVAL;

	return 0;
}

static void clear_stats(struct sw_flow *flow)
{
	flow->used = 0;
	flow->tcp_flags = 0;
	flow->packet_count = 0;
	flow->byte_count = 0;
=======
=======
>>>>>>> v3.18
	/* Only reserve room for attribute header, packet data is added
	 * in skb_zerocopy() */
	if (!(nla = nla_reserve(user_skb, OVS_PACKET_ATTR_PACKET, 0))) {
		err = -ENOBUFS;
		goto out;
	}
	nla->nla_len = nla_attr_size(skb->len);

	err = skb_zerocopy(user_skb, skb, skb->len, hlen);
	if (err)
		goto out;

	/* Pad OVS_PACKET_ATTR_PACKET if linear copy was performed */
	if (!(dp->user_features & OVS_DP_F_UNALIGNED)) {
		size_t plen = NLA_ALIGN(user_skb->len) - user_skb->len;

		if (plen > 0)
			memset(skb_put(user_skb, plen), 0, plen);
	}

	((struct nlmsghdr *) user_skb->data)->nlmsg_len = user_skb->len;

	err = genlmsg_unicast(ovs_dp_get_net(dp), user_skb, upcall_info->portid);
	user_skb = NULL;
out:
	if (err)
		skb_tx_error(skb);
	kfree_skb(user_skb);
	kfree_skb(nskb);
	return err;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static int ovs_packet_cmd_execute(struct sk_buff *skb, struct genl_info *info)
{
	struct ovs_header *ovs_header = info->userhdr;
	struct nlattr **a = info->attrs;
	struct sw_flow_actions *acts;
	struct sk_buff *packet;
	struct sw_flow *flow;
	struct datapath *dp;
	struct ethhdr *eth;
<<<<<<< HEAD
<<<<<<< HEAD
	int len;
	int err;
	int key_len;
=======
	struct vport *input_vport;
	int len;
	int err;
>>>>>>> v3.18
=======
	struct vport *input_vport;
	int len;
	int err;
>>>>>>> v3.18

	err = -EINVAL;
	if (!a[OVS_PACKET_ATTR_PACKET] || !a[OVS_PACKET_ATTR_KEY] ||
	    !a[OVS_PACKET_ATTR_ACTIONS])
		goto err;

	len = nla_len(a[OVS_PACKET_ATTR_PACKET]);
	packet = __dev_alloc_skb(NET_IP_ALIGN + len, GFP_KERNEL);
	err = -ENOMEM;
	if (!packet)
		goto err;
	skb_reserve(packet, NET_IP_ALIGN);

	nla_memcpy(__skb_put(packet, len), a[OVS_PACKET_ATTR_PACKET], len);

	skb_reset_mac_header(packet);
	eth = eth_hdr(packet);

	/* Normally, setting the skb 'protocol' field would be handled by a
	 * call to eth_type_trans(), but it assumes there's a sending
	 * device, which we may not have. */
	if (ntohs(eth->h_proto) >= ETH_P_802_3_MIN)
		packet->protocol = eth->h_proto;
	else
		packet->protocol = htons(ETH_P_802_2);

	/* Build an sw_flow for sending this packet. */
	flow = ovs_flow_alloc();
	err = PTR_ERR(flow);
	if (IS_ERR(flow))
		goto err_kfree_skb;

<<<<<<< HEAD
<<<<<<< HEAD
	err = ovs_flow_extract(packet, -1, &flow->key, &key_len);
	if (err)
		goto err_flow_free;

	err = ovs_flow_metadata_from_nlattrs(&flow->key.phy.priority,
					     &flow->key.phy.skb_mark,
					     &flow->key.phy.in_port,
					     a[OVS_PACKET_ATTR_KEY]);
	if (err)
		goto err_flow_free;

	err = validate_actions(a[OVS_PACKET_ATTR_ACTIONS], &flow->key, 0);
	if (err)
		goto err_flow_free;

	flow->hash = ovs_flow_hash(&flow->key, key_len);

	acts = ovs_flow_actions_alloc(a[OVS_PACKET_ATTR_ACTIONS]);
	err = PTR_ERR(acts);
	if (IS_ERR(acts))
		goto err_flow_free;
	rcu_assign_pointer(flow->sf_acts, acts);

=======
=======
>>>>>>> v3.18
	err = ovs_flow_key_extract_userspace(a[OVS_PACKET_ATTR_KEY], packet,
					     &flow->key);
	if (err)
		goto err_flow_free;

	acts = ovs_nla_alloc_flow_actions(nla_len(a[OVS_PACKET_ATTR_ACTIONS]));
	err = PTR_ERR(acts);
	if (IS_ERR(acts))
		goto err_flow_free;

	err = ovs_nla_copy_actions(a[OVS_PACKET_ATTR_ACTIONS],
				   &flow->key, 0, &acts);
	if (err)
		goto err_flow_free;

	rcu_assign_pointer(flow->sf_acts, acts);

	OVS_CB(packet)->egress_tun_info = NULL;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	OVS_CB(packet)->flow = flow;
	packet->priority = flow->key.phy.priority;
	packet->mark = flow->key.phy.skb_mark;

	rcu_read_lock();
	dp = get_dp(sock_net(skb->sk), ovs_header->dp_ifindex);
	err = -ENODEV;
	if (!dp)
		goto err_unlock;

<<<<<<< HEAD
<<<<<<< HEAD
	local_bh_disable();
	err = ovs_execute_actions(dp, packet);
	local_bh_enable();
	rcu_read_unlock();

	ovs_flow_free(flow);
=======
=======
>>>>>>> v3.18
	input_vport = ovs_vport_rcu(dp, flow->key.phy.in_port);
	if (!input_vport)
		input_vport = ovs_vport_rcu(dp, OVSP_LOCAL);

	if (!input_vport)
		goto err_unlock;

	OVS_CB(packet)->input_vport = input_vport;

	local_bh_disable();
	err = ovs_execute_actions(dp, packet, &flow->key);
	local_bh_enable();
	rcu_read_unlock();

	ovs_flow_free(flow, false);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return err;

err_unlock:
	rcu_read_unlock();
err_flow_free:
<<<<<<< HEAD
<<<<<<< HEAD
	ovs_flow_free(flow);
=======
	ovs_flow_free(flow, false);
>>>>>>> v3.18
=======
	ovs_flow_free(flow, false);
>>>>>>> v3.18
err_kfree_skb:
	kfree_skb(packet);
err:
	return err;
}

static const struct nla_policy packet_policy[OVS_PACKET_ATTR_MAX + 1] = {
	[OVS_PACKET_ATTR_PACKET] = { .len = ETH_HLEN },
	[OVS_PACKET_ATTR_KEY] = { .type = NLA_NESTED },
	[OVS_PACKET_ATTR_ACTIONS] = { .type = NLA_NESTED },
};

<<<<<<< HEAD
<<<<<<< HEAD
static struct genl_ops dp_packet_genl_ops[] = {
=======
static const struct genl_ops dp_packet_genl_ops[] = {
>>>>>>> v3.18
=======
static const struct genl_ops dp_packet_genl_ops[] = {
>>>>>>> v3.18
	{ .cmd = OVS_PACKET_CMD_EXECUTE,
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = packet_policy,
	  .doit = ovs_packet_cmd_execute
	}
};

<<<<<<< HEAD
<<<<<<< HEAD
static void get_dp_stats(struct datapath *dp, struct ovs_dp_stats *stats)
{
	int i;
	struct flow_table *table = ovsl_dereference(dp->table);

	stats->n_flows = ovs_flow_tbl_count(table);

	stats->n_hit = stats->n_missed = stats->n_lost = 0;
=======
=======
>>>>>>> v3.18
static struct genl_family dp_packet_genl_family = {
	.id = GENL_ID_GENERATE,
	.hdrsize = sizeof(struct ovs_header),
	.name = OVS_PACKET_FAMILY,
	.version = OVS_PACKET_VERSION,
	.maxattr = OVS_PACKET_ATTR_MAX,
	.netnsok = true,
	.parallel_ops = true,
	.ops = dp_packet_genl_ops,
	.n_ops = ARRAY_SIZE(dp_packet_genl_ops),
};

static void get_dp_stats(struct datapath *dp, struct ovs_dp_stats *stats,
			 struct ovs_dp_megaflow_stats *mega_stats)
{
	int i;

	memset(mega_stats, 0, sizeof(*mega_stats));

	stats->n_flows = ovs_flow_tbl_count(&dp->table);
	mega_stats->n_masks = ovs_flow_tbl_num_masks(&dp->table);

	stats->n_hit = stats->n_missed = stats->n_lost = 0;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	for_each_possible_cpu(i) {
		const struct dp_stats_percpu *percpu_stats;
		struct dp_stats_percpu local_stats;
		unsigned int start;

		percpu_stats = per_cpu_ptr(dp->stats_percpu, i);

		do {
<<<<<<< HEAD
<<<<<<< HEAD
			start = u64_stats_fetch_begin_bh(&percpu_stats->sync);
			local_stats = *percpu_stats;
		} while (u64_stats_fetch_retry_bh(&percpu_stats->sync, start));
=======
			start = u64_stats_fetch_begin_irq(&percpu_stats->syncp);
			local_stats = *percpu_stats;
		} while (u64_stats_fetch_retry_irq(&percpu_stats->syncp, start));
>>>>>>> v3.18
=======
			start = u64_stats_fetch_begin_irq(&percpu_stats->syncp);
			local_stats = *percpu_stats;
		} while (u64_stats_fetch_retry_irq(&percpu_stats->syncp, start));
>>>>>>> v3.18

		stats->n_hit += local_stats.n_hit;
		stats->n_missed += local_stats.n_missed;
		stats->n_lost += local_stats.n_lost;
<<<<<<< HEAD
<<<<<<< HEAD
	}
}

static const struct nla_policy flow_policy[OVS_FLOW_ATTR_MAX + 1] = {
	[OVS_FLOW_ATTR_KEY] = { .type = NLA_NESTED },
	[OVS_FLOW_ATTR_ACTIONS] = { .type = NLA_NESTED },
	[OVS_FLOW_ATTR_CLEAR] = { .type = NLA_FLAG },
};

static struct genl_family dp_flow_genl_family = {
	.id = GENL_ID_GENERATE,
	.hdrsize = sizeof(struct ovs_header),
	.name = OVS_FLOW_FAMILY,
	.version = OVS_FLOW_VERSION,
	.maxattr = OVS_FLOW_ATTR_MAX,
	.netnsok = true,
	.parallel_ops = true,
};

static struct genl_multicast_group ovs_dp_flow_multicast_group = {
	.name = OVS_FLOW_MCGROUP
};

=======
=======
>>>>>>> v3.18
		mega_stats->n_mask_hit += local_stats.n_mask_hit;
	}
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static size_t ovs_flow_cmd_msg_size(const struct sw_flow_actions *acts)
{
	return NLMSG_ALIGN(sizeof(struct ovs_header))
		+ nla_total_size(key_attr_size()) /* OVS_FLOW_ATTR_KEY */
<<<<<<< HEAD
<<<<<<< HEAD
=======
		+ nla_total_size(key_attr_size()) /* OVS_FLOW_ATTR_MASK */
>>>>>>> v3.18
=======
		+ nla_total_size(key_attr_size()) /* OVS_FLOW_ATTR_MASK */
>>>>>>> v3.18
		+ nla_total_size(sizeof(struct ovs_flow_stats)) /* OVS_FLOW_ATTR_STATS */
		+ nla_total_size(1) /* OVS_FLOW_ATTR_TCP_FLAGS */
		+ nla_total_size(8) /* OVS_FLOW_ATTR_USED */
		+ nla_total_size(acts->actions_len); /* OVS_FLOW_ATTR_ACTIONS */
}

<<<<<<< HEAD
<<<<<<< HEAD
/* Called with ovs_mutex. */
static int ovs_flow_cmd_fill_info(struct sw_flow *flow, struct datapath *dp,
=======
/* Called with ovs_mutex or RCU read lock. */
static int ovs_flow_cmd_fill_info(const struct sw_flow *flow, int dp_ifindex,
>>>>>>> v3.18
=======
/* Called with ovs_mutex or RCU read lock. */
static int ovs_flow_cmd_fill_info(const struct sw_flow *flow, int dp_ifindex,
>>>>>>> v3.18
				  struct sk_buff *skb, u32 portid,
				  u32 seq, u32 flags, u8 cmd)
{
	const int skb_orig_len = skb->len;
<<<<<<< HEAD
<<<<<<< HEAD
	const struct sw_flow_actions *sf_acts;
	struct ovs_flow_stats stats;
	struct ovs_header *ovs_header;
	struct nlattr *nla;
	unsigned long used;
	u8 tcp_flags;
	int err;

	sf_acts = ovsl_dereference(flow->sf_acts);

=======
=======
>>>>>>> v3.18
	struct nlattr *start;
	struct ovs_flow_stats stats;
	__be16 tcp_flags;
	unsigned long used;
	struct ovs_header *ovs_header;
	struct nlattr *nla;
	int err;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	ovs_header = genlmsg_put(skb, portid, seq, &dp_flow_genl_family, flags, cmd);
	if (!ovs_header)
		return -EMSGSIZE;

<<<<<<< HEAD
<<<<<<< HEAD
	ovs_header->dp_ifindex = get_dpifindex(dp);

	nla = nla_nest_start(skb, OVS_FLOW_ATTR_KEY);
	if (!nla)
		goto nla_put_failure;
	err = ovs_flow_to_nlattrs(&flow->key, skb);
	if (err)
		goto error;
	nla_nest_end(skb, nla);

	spin_lock_bh(&flow->lock);
	used = flow->used;
	stats.n_packets = flow->packet_count;
	stats.n_bytes = flow->byte_count;
	tcp_flags = flow->tcp_flags;
	spin_unlock_bh(&flow->lock);
=======
=======
>>>>>>> v3.18
	ovs_header->dp_ifindex = dp_ifindex;

	/* Fill flow key. */
	nla = nla_nest_start(skb, OVS_FLOW_ATTR_KEY);
	if (!nla)
		goto nla_put_failure;

	err = ovs_nla_put_flow(&flow->unmasked_key, &flow->unmasked_key, skb);
	if (err)
		goto error;
	nla_nest_end(skb, nla);

	nla = nla_nest_start(skb, OVS_FLOW_ATTR_MASK);
	if (!nla)
		goto nla_put_failure;

	err = ovs_nla_put_flow(&flow->key, &flow->mask->key, skb);
	if (err)
		goto error;

	nla_nest_end(skb, nla);

	ovs_flow_stats_get(flow, &stats, &used, &tcp_flags);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (used &&
	    nla_put_u64(skb, OVS_FLOW_ATTR_USED, ovs_flow_used_time(used)))
		goto nla_put_failure;

	if (stats.n_packets &&
<<<<<<< HEAD
<<<<<<< HEAD
	    nla_put(skb, OVS_FLOW_ATTR_STATS,
		    sizeof(struct ovs_flow_stats), &stats))
		goto nla_put_failure;

	if (tcp_flags &&
	    nla_put_u8(skb, OVS_FLOW_ATTR_TCP_FLAGS, tcp_flags))
=======
=======
>>>>>>> v3.18
	    nla_put(skb, OVS_FLOW_ATTR_STATS, sizeof(struct ovs_flow_stats), &stats))
		goto nla_put_failure;

	if ((u8)ntohs(tcp_flags) &&
	     nla_put_u8(skb, OVS_FLOW_ATTR_TCP_FLAGS, (u8)ntohs(tcp_flags)))
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		goto nla_put_failure;

	/* If OVS_FLOW_ATTR_ACTIONS doesn't fit, skip dumping the actions if
	 * this is the first flow to be dumped into 'skb'.  This is unusual for
	 * Netlink but individual action lists can be longer than
	 * NLMSG_GOODSIZE and thus entirely undumpable if we didn't do this.
	 * The userspace caller can always fetch the actions separately if it
	 * really wants them.  (Most userspace callers in fact don't care.)
	 *
	 * This can only fail for dump operations because the skb is always
	 * properly sized for single flows.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	err = nla_put(skb, OVS_FLOW_ATTR_ACTIONS, sf_acts->actions_len,
		      sf_acts->actions);
	if (err < 0 && skb_orig_len)
		goto error;
=======
=======
>>>>>>> v3.18
	start = nla_nest_start(skb, OVS_FLOW_ATTR_ACTIONS);
	if (start) {
		const struct sw_flow_actions *sf_acts;

		sf_acts = rcu_dereference_ovsl(flow->sf_acts);
		err = ovs_nla_put_actions(sf_acts->actions,
					  sf_acts->actions_len, skb);

		if (!err)
			nla_nest_end(skb, start);
		else {
			if (skb_orig_len)
				goto error;

			nla_nest_cancel(skb, start);
		}
	} else if (skb_orig_len)
		goto nla_put_failure;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return genlmsg_end(skb, ovs_header);

nla_put_failure:
	err = -EMSGSIZE;
error:
	genlmsg_cancel(skb, ovs_header);
	return err;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct sk_buff *ovs_flow_cmd_alloc_info(struct sw_flow *flow)
{
	const struct sw_flow_actions *sf_acts;

	sf_acts = ovsl_dereference(flow->sf_acts);

	return genlmsg_new(ovs_flow_cmd_msg_size(sf_acts), GFP_KERNEL);
}

static struct sk_buff *ovs_flow_cmd_build_info(struct sw_flow *flow,
					       struct datapath *dp,
					       u32 portid, u32 seq, u8 cmd)
=======
=======
>>>>>>> v3.18
/* May not be called with RCU read lock. */
static struct sk_buff *ovs_flow_cmd_alloc_info(const struct sw_flow_actions *acts,
					       struct genl_info *info,
					       bool always)
{
	struct sk_buff *skb;

	if (!always && !ovs_must_notify(&dp_flow_genl_family, info, 0))
		return NULL;

	skb = genlmsg_new_unicast(ovs_flow_cmd_msg_size(acts), info, GFP_KERNEL);
	if (!skb)
		return ERR_PTR(-ENOMEM);

	return skb;
}

/* Called with ovs_mutex. */
static struct sk_buff *ovs_flow_cmd_build_info(const struct sw_flow *flow,
					       int dp_ifindex,
					       struct genl_info *info, u8 cmd,
					       bool always)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	struct sk_buff *skb;
	int retval;

<<<<<<< HEAD
<<<<<<< HEAD
	skb = ovs_flow_cmd_alloc_info(flow);
	if (!skb)
		return ERR_PTR(-ENOMEM);

	retval = ovs_flow_cmd_fill_info(flow, dp, skb, portid, seq, 0, cmd);
=======
=======
>>>>>>> v3.18
	skb = ovs_flow_cmd_alloc_info(ovsl_dereference(flow->sf_acts), info,
				      always);
	if (IS_ERR_OR_NULL(skb))
		return skb;

	retval = ovs_flow_cmd_fill_info(flow, dp_ifindex, skb,
					info->snd_portid, info->snd_seq, 0,
					cmd);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	BUG_ON(retval < 0);
	return skb;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int ovs_flow_cmd_new_or_set(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **a = info->attrs;
	struct ovs_header *ovs_header = info->userhdr;
	struct sw_flow_key key;
	struct sw_flow *flow;
	struct sk_buff *reply;
	struct datapath *dp;
	struct flow_table *table;
	int error;
	int key_len;

	/* Extract key. */
	error = -EINVAL;
	if (!a[OVS_FLOW_ATTR_KEY])
		goto error;
	error = ovs_flow_from_nlattrs(&key, &key_len, a[OVS_FLOW_ATTR_KEY]);
	if (error)
		goto error;

	/* Validate actions. */
	if (a[OVS_FLOW_ATTR_ACTIONS]) {
		error = validate_actions(a[OVS_FLOW_ATTR_ACTIONS], &key,  0);
		if (error)
			goto error;
	} else if (info->genlhdr->cmd == OVS_FLOW_CMD_NEW) {
		error = -EINVAL;
		goto error;
	}

	ovs_lock();
	dp = get_dp(sock_net(skb->sk), ovs_header->dp_ifindex);
	error = -ENODEV;
	if (!dp)
		goto err_unlock_ovs;

	table = ovsl_dereference(dp->table);
	flow = ovs_flow_tbl_lookup(table, &key, key_len);
	if (!flow) {
		struct sw_flow_actions *acts;

		/* Bail out if we're not allowed to create a new flow. */
		error = -ENOENT;
		if (info->genlhdr->cmd == OVS_FLOW_CMD_SET)
			goto err_unlock_ovs;

		/* Expand table, if necessary, to make room. */
		if (ovs_flow_tbl_need_to_expand(table)) {
			struct flow_table *new_table;

			new_table = ovs_flow_tbl_expand(table);
			if (!IS_ERR(new_table)) {
				rcu_assign_pointer(dp->table, new_table);
				ovs_flow_tbl_deferred_destroy(table);
				table = ovsl_dereference(dp->table);
			}
		}

		/* Allocate flow. */
		flow = ovs_flow_alloc();
		if (IS_ERR(flow)) {
			error = PTR_ERR(flow);
			goto err_unlock_ovs;
		}
		flow->key = key;
		clear_stats(flow);

		/* Obtain actions. */
		acts = ovs_flow_actions_alloc(a[OVS_FLOW_ATTR_ACTIONS]);
		error = PTR_ERR(acts);
		if (IS_ERR(acts))
			goto error_free_flow;
		rcu_assign_pointer(flow->sf_acts, acts);

		/* Put flow in bucket. */
		flow->hash = ovs_flow_hash(&key, key_len);
		ovs_flow_tbl_insert(table, flow);

		reply = ovs_flow_cmd_build_info(flow, dp, info->snd_portid,
						info->snd_seq,
						OVS_FLOW_CMD_NEW);
	} else {
		/* We found a matching flow. */
		struct sw_flow_actions *old_acts;
		struct nlattr *acts_attrs;
=======
=======
>>>>>>> v3.18
static int ovs_flow_cmd_new(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **a = info->attrs;
	struct ovs_header *ovs_header = info->userhdr;
	struct sw_flow *flow, *new_flow;
	struct sw_flow_mask mask;
	struct sk_buff *reply;
	struct datapath *dp;
	struct sw_flow_actions *acts;
	struct sw_flow_match match;
	int error;

	/* Must have key and actions. */
	error = -EINVAL;
	if (!a[OVS_FLOW_ATTR_KEY])
		goto error;
	if (!a[OVS_FLOW_ATTR_ACTIONS])
		goto error;

	/* Most of the time we need to allocate a new flow, do it before
	 * locking.
	 */
	new_flow = ovs_flow_alloc();
	if (IS_ERR(new_flow)) {
		error = PTR_ERR(new_flow);
		goto error;
	}

	/* Extract key. */
	ovs_match_init(&match, &new_flow->unmasked_key, &mask);
	error = ovs_nla_get_match(&match,
				  a[OVS_FLOW_ATTR_KEY], a[OVS_FLOW_ATTR_MASK]);
	if (error)
		goto err_kfree_flow;

	ovs_flow_mask_key(&new_flow->key, &new_flow->unmasked_key, &mask);

	/* Validate actions. */
	acts = ovs_nla_alloc_flow_actions(nla_len(a[OVS_FLOW_ATTR_ACTIONS]));
	error = PTR_ERR(acts);
	if (IS_ERR(acts))
		goto err_kfree_flow;

	error = ovs_nla_copy_actions(a[OVS_FLOW_ATTR_ACTIONS], &new_flow->key,
				     0, &acts);
	if (error) {
		OVS_NLERR("Flow actions may not be safe on all matching packets.\n");
		goto err_kfree_acts;
	}

	reply = ovs_flow_cmd_alloc_info(acts, info, false);
	if (IS_ERR(reply)) {
		error = PTR_ERR(reply);
		goto err_kfree_acts;
	}

	ovs_lock();
	dp = get_dp(sock_net(skb->sk), ovs_header->dp_ifindex);
	if (unlikely(!dp)) {
		error = -ENODEV;
		goto err_unlock_ovs;
	}
	/* Check if this is a duplicate flow */
	flow = ovs_flow_tbl_lookup(&dp->table, &new_flow->unmasked_key);
	if (likely(!flow)) {
		rcu_assign_pointer(new_flow->sf_acts, acts);

		/* Put flow in bucket. */
		error = ovs_flow_tbl_insert(&dp->table, new_flow, &mask);
		if (unlikely(error)) {
			acts = NULL;
			goto err_unlock_ovs;
		}

		if (unlikely(reply)) {
			error = ovs_flow_cmd_fill_info(new_flow,
						       ovs_header->dp_ifindex,
						       reply, info->snd_portid,
						       info->snd_seq, 0,
						       OVS_FLOW_CMD_NEW);
			BUG_ON(error < 0);
<<<<<<< HEAD
		}
		ovs_unlock();
	} else {
		struct sw_flow_actions *old_acts;
>>>>>>> v3.18

		/* Bail out if we're not allowed to modify an existing flow.
		 * We accept NLM_F_CREATE in place of the intended NLM_F_EXCL
		 * because Generic Netlink treats the latter as a dump
		 * request.  We also accept NLM_F_EXCL in case that bug ever
		 * gets fixed.
		 */
<<<<<<< HEAD
		error = -EEXIST;
		if (info->genlhdr->cmd == OVS_FLOW_CMD_NEW &&
		    info->nlhdr->nlmsg_flags & (NLM_F_CREATE | NLM_F_EXCL))
			goto err_unlock_ovs;

		/* Update actions. */
		old_acts = ovsl_dereference(flow->sf_acts);
		acts_attrs = a[OVS_FLOW_ATTR_ACTIONS];
		if (acts_attrs &&
		   (old_acts->actions_len != nla_len(acts_attrs) ||
		   memcmp(old_acts->actions, nla_data(acts_attrs),
			  old_acts->actions_len))) {
			struct sw_flow_actions *new_acts;

			new_acts = ovs_flow_actions_alloc(acts_attrs);
			error = PTR_ERR(new_acts);
			if (IS_ERR(new_acts))
				goto err_unlock_ovs;

			rcu_assign_pointer(flow->sf_acts, new_acts);
			ovs_flow_deferred_free_acts(old_acts);
		}

		reply = ovs_flow_cmd_build_info(flow, dp, info->snd_portid,
					       info->snd_seq, OVS_FLOW_CMD_NEW);

		/* Clear stats. */
		if (a[OVS_FLOW_ATTR_CLEAR]) {
			spin_lock_bh(&flow->lock);
			clear_stats(flow);
			spin_unlock_bh(&flow->lock);
		}
	}
	ovs_unlock();

	if (!IS_ERR(reply))
		ovs_notify(reply, info, &ovs_dp_flow_multicast_group);
	else
		netlink_set_err(sock_net(skb->sk)->genl_sock, 0,
				ovs_dp_flow_multicast_group.id, PTR_ERR(reply));
	return 0;

error_free_flow:
	ovs_flow_free(flow);
err_unlock_ovs:
	ovs_unlock();
=======
=======
		}
		ovs_unlock();
	} else {
		struct sw_flow_actions *old_acts;

		/* Bail out if we're not allowed to modify an existing flow.
		 * We accept NLM_F_CREATE in place of the intended NLM_F_EXCL
		 * because Generic Netlink treats the latter as a dump
		 * request.  We also accept NLM_F_EXCL in case that bug ever
		 * gets fixed.
		 */
>>>>>>> v3.18
		if (unlikely(info->nlhdr->nlmsg_flags & (NLM_F_CREATE
							 | NLM_F_EXCL))) {
			error = -EEXIST;
			goto err_unlock_ovs;
		}
		/* The unmasked key has to be the same for flow updates. */
		if (unlikely(!ovs_flow_cmp_unmasked_key(flow, &match))) {
			flow = ovs_flow_tbl_lookup_exact(&dp->table, &match);
			if (!flow) {
				error = -ENOENT;
				goto err_unlock_ovs;
			}
		}
		/* Update actions. */
		old_acts = ovsl_dereference(flow->sf_acts);
		rcu_assign_pointer(flow->sf_acts, acts);

		if (unlikely(reply)) {
			error = ovs_flow_cmd_fill_info(flow,
						       ovs_header->dp_ifindex,
						       reply, info->snd_portid,
						       info->snd_seq, 0,
						       OVS_FLOW_CMD_NEW);
			BUG_ON(error < 0);
		}
		ovs_unlock();

		ovs_nla_free_flow_actions(old_acts);
		ovs_flow_free(new_flow, false);
	}

	if (reply)
		ovs_notify(&dp_flow_genl_family, reply, info);
	return 0;

err_unlock_ovs:
	ovs_unlock();
	kfree_skb(reply);
err_kfree_acts:
	kfree(acts);
err_kfree_flow:
	ovs_flow_free(new_flow, false);
error:
	return error;
}

static struct sw_flow_actions *get_flow_actions(const struct nlattr *a,
						const struct sw_flow_key *key,
						const struct sw_flow_mask *mask)
{
	struct sw_flow_actions *acts;
	struct sw_flow_key masked_key;
	int error;

	acts = ovs_nla_alloc_flow_actions(nla_len(a));
	if (IS_ERR(acts))
		return acts;

	ovs_flow_mask_key(&masked_key, key, mask);
	error = ovs_nla_copy_actions(a, &masked_key, 0, &acts);
	if (error) {
		OVS_NLERR("Flow actions may not be safe on all matching packets.\n");
		kfree(acts);
		return ERR_PTR(error);
	}

	return acts;
}

static int ovs_flow_cmd_set(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **a = info->attrs;
	struct ovs_header *ovs_header = info->userhdr;
	struct sw_flow_key key;
	struct sw_flow *flow;
	struct sw_flow_mask mask;
	struct sk_buff *reply = NULL;
	struct datapath *dp;
	struct sw_flow_actions *old_acts = NULL, *acts = NULL;
	struct sw_flow_match match;
	int error;

	/* Extract key. */
	error = -EINVAL;
	if (!a[OVS_FLOW_ATTR_KEY])
		goto error;

	ovs_match_init(&match, &key, &mask);
	error = ovs_nla_get_match(&match,
				  a[OVS_FLOW_ATTR_KEY], a[OVS_FLOW_ATTR_MASK]);
	if (error)
		goto error;

	/* Validate actions. */
	if (a[OVS_FLOW_ATTR_ACTIONS]) {
		acts = get_flow_actions(a[OVS_FLOW_ATTR_ACTIONS], &key, &mask);
		if (IS_ERR(acts)) {
			error = PTR_ERR(acts);
			goto error;
		}
	}

	/* Can allocate before locking if have acts. */
	if (acts) {
		reply = ovs_flow_cmd_alloc_info(acts, info, false);
		if (IS_ERR(reply)) {
			error = PTR_ERR(reply);
			goto err_kfree_acts;
		}
	}

	ovs_lock();
	dp = get_dp(sock_net(skb->sk), ovs_header->dp_ifindex);
	if (unlikely(!dp)) {
		error = -ENODEV;
		goto err_unlock_ovs;
	}
	/* Check that the flow exists. */
	flow = ovs_flow_tbl_lookup_exact(&dp->table, &match);
	if (unlikely(!flow)) {
		error = -ENOENT;
		goto err_unlock_ovs;
	}

	/* Update actions, if present. */
	if (likely(acts)) {
		old_acts = ovsl_dereference(flow->sf_acts);
		rcu_assign_pointer(flow->sf_acts, acts);

		if (unlikely(reply)) {
			error = ovs_flow_cmd_fill_info(flow,
						       ovs_header->dp_ifindex,
						       reply, info->snd_portid,
						       info->snd_seq, 0,
						       OVS_FLOW_CMD_NEW);
			BUG_ON(error < 0);
		}
	} else {
		/* Could not alloc without acts before locking. */
		reply = ovs_flow_cmd_build_info(flow, ovs_header->dp_ifindex,
						info, OVS_FLOW_CMD_NEW, false);
		if (unlikely(IS_ERR(reply))) {
			error = PTR_ERR(reply);
			goto err_unlock_ovs;
		}
	}

	/* Clear stats. */
	if (a[OVS_FLOW_ATTR_CLEAR])
		ovs_flow_stats_clear(flow);
	ovs_unlock();

	if (reply)
		ovs_notify(&dp_flow_genl_family, reply, info);
	if (old_acts)
		ovs_nla_free_flow_actions(old_acts);

	return 0;

err_unlock_ovs:
	ovs_unlock();
	kfree_skb(reply);
err_kfree_acts:
	kfree(acts);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
error:
	return error;
}

static int ovs_flow_cmd_get(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **a = info->attrs;
	struct ovs_header *ovs_header = info->userhdr;
	struct sw_flow_key key;
	struct sk_buff *reply;
	struct sw_flow *flow;
	struct datapath *dp;
<<<<<<< HEAD
<<<<<<< HEAD
	struct flow_table *table;
	int err;
	int key_len;

	if (!a[OVS_FLOW_ATTR_KEY])
		return -EINVAL;
	err = ovs_flow_from_nlattrs(&key, &key_len, a[OVS_FLOW_ATTR_KEY]);
=======
=======
>>>>>>> v3.18
	struct sw_flow_match match;
	int err;

	if (!a[OVS_FLOW_ATTR_KEY]) {
		OVS_NLERR("Flow get message rejected, Key attribute missing.\n");
		return -EINVAL;
	}

	ovs_match_init(&match, &key, NULL);
	err = ovs_nla_get_match(&match, a[OVS_FLOW_ATTR_KEY], NULL);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (err)
		return err;

	ovs_lock();
	dp = get_dp(sock_net(skb->sk), ovs_header->dp_ifindex);
	if (!dp) {
		err = -ENODEV;
		goto unlock;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	table = ovsl_dereference(dp->table);
	flow = ovs_flow_tbl_lookup(table, &key, key_len);
=======
	flow = ovs_flow_tbl_lookup_exact(&dp->table, &match);
>>>>>>> v3.18
=======
	flow = ovs_flow_tbl_lookup_exact(&dp->table, &match);
>>>>>>> v3.18
	if (!flow) {
		err = -ENOENT;
		goto unlock;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	reply = ovs_flow_cmd_build_info(flow, dp, info->snd_portid,
					info->snd_seq, OVS_FLOW_CMD_NEW);
=======
	reply = ovs_flow_cmd_build_info(flow, ovs_header->dp_ifindex, info,
					OVS_FLOW_CMD_NEW, true);
>>>>>>> v3.18
=======
	reply = ovs_flow_cmd_build_info(flow, ovs_header->dp_ifindex, info,
					OVS_FLOW_CMD_NEW, true);
>>>>>>> v3.18
	if (IS_ERR(reply)) {
		err = PTR_ERR(reply);
		goto unlock;
	}

	ovs_unlock();
	return genlmsg_reply(reply, info);
unlock:
	ovs_unlock();
	return err;
}

static int ovs_flow_cmd_del(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **a = info->attrs;
	struct ovs_header *ovs_header = info->userhdr;
	struct sw_flow_key key;
	struct sk_buff *reply;
	struct sw_flow *flow;
	struct datapath *dp;
<<<<<<< HEAD
<<<<<<< HEAD
	struct flow_table *table;
	int err;
	int key_len;

	ovs_lock();
	dp = get_dp(sock_net(skb->sk), ovs_header->dp_ifindex);
	if (!dp) {
=======
=======
>>>>>>> v3.18
	struct sw_flow_match match;
	int err;

	if (likely(a[OVS_FLOW_ATTR_KEY])) {
		ovs_match_init(&match, &key, NULL);
		err = ovs_nla_get_match(&match, a[OVS_FLOW_ATTR_KEY], NULL);
		if (unlikely(err))
			return err;
	}

	ovs_lock();
	dp = get_dp(sock_net(skb->sk), ovs_header->dp_ifindex);
	if (unlikely(!dp)) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		err = -ENODEV;
		goto unlock;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (!a[OVS_FLOW_ATTR_KEY]) {
		err = flush_flows(dp);
		goto unlock;
	}
	err = ovs_flow_from_nlattrs(&key, &key_len, a[OVS_FLOW_ATTR_KEY]);
	if (err)
		goto unlock;

	table = ovsl_dereference(dp->table);
	flow = ovs_flow_tbl_lookup(table, &key, key_len);
	if (!flow) {
=======
=======
>>>>>>> v3.18
	if (unlikely(!a[OVS_FLOW_ATTR_KEY])) {
		err = ovs_flow_tbl_flush(&dp->table);
		goto unlock;
	}

	flow = ovs_flow_tbl_lookup_exact(&dp->table, &match);
	if (unlikely(!flow)) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		err = -ENOENT;
		goto unlock;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	reply = ovs_flow_cmd_alloc_info(flow);
	if (!reply) {
		err = -ENOMEM;
		goto unlock;
	}

	ovs_flow_tbl_remove(table, flow);

	err = ovs_flow_cmd_fill_info(flow, dp, reply, info->snd_portid,
				     info->snd_seq, 0, OVS_FLOW_CMD_DEL);
	BUG_ON(err < 0);

	ovs_flow_deferred_free(flow);
	ovs_unlock();

	ovs_notify(reply, info, &ovs_dp_flow_multicast_group);
=======
=======
>>>>>>> v3.18
	ovs_flow_tbl_remove(&dp->table, flow);
	ovs_unlock();

	reply = ovs_flow_cmd_alloc_info((const struct sw_flow_actions __force *) flow->sf_acts,
					info, false);
	if (likely(reply)) {
		if (likely(!IS_ERR(reply))) {
			rcu_read_lock();	/*To keep RCU checker happy. */
			err = ovs_flow_cmd_fill_info(flow, ovs_header->dp_ifindex,
						     reply, info->snd_portid,
						     info->snd_seq, 0,
						     OVS_FLOW_CMD_DEL);
			rcu_read_unlock();
			BUG_ON(err < 0);

			ovs_notify(&dp_flow_genl_family, reply, info);
		} else {
			netlink_set_err(sock_net(skb->sk)->genl_sock, 0, 0, PTR_ERR(reply));
		}
	}

	ovs_flow_free(flow, true);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
unlock:
	ovs_unlock();
	return err;
}

static int ovs_flow_cmd_dump(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct ovs_header *ovs_header = genlmsg_data(nlmsg_data(cb->nlh));
<<<<<<< HEAD
<<<<<<< HEAD
	struct datapath *dp;
	struct flow_table *table;

	ovs_lock();
	dp = get_dp(sock_net(skb->sk), ovs_header->dp_ifindex);
	if (!dp) {
		ovs_unlock();
		return -ENODEV;
	}

	table = ovsl_dereference(dp->table);

=======
=======
>>>>>>> v3.18
	struct table_instance *ti;
	struct datapath *dp;

	rcu_read_lock();
	dp = get_dp(sock_net(skb->sk), ovs_header->dp_ifindex);
	if (!dp) {
		rcu_read_unlock();
		return -ENODEV;
	}

	ti = rcu_dereference(dp->table.ti);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	for (;;) {
		struct sw_flow *flow;
		u32 bucket, obj;

		bucket = cb->args[0];
		obj = cb->args[1];
<<<<<<< HEAD
<<<<<<< HEAD
		flow = ovs_flow_tbl_next(table, &bucket, &obj);
		if (!flow)
			break;

		if (ovs_flow_cmd_fill_info(flow, dp, skb,
=======
=======
>>>>>>> v3.18
		flow = ovs_flow_tbl_dump_next(ti, &bucket, &obj);
		if (!flow)
			break;

		if (ovs_flow_cmd_fill_info(flow, ovs_header->dp_ifindex, skb,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
					   NETLINK_CB(cb->skb).portid,
					   cb->nlh->nlmsg_seq, NLM_F_MULTI,
					   OVS_FLOW_CMD_NEW) < 0)
			break;

		cb->args[0] = bucket;
		cb->args[1] = obj;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	ovs_unlock();
	return skb->len;
}

static struct genl_ops dp_flow_genl_ops[] = {
	{ .cmd = OVS_FLOW_CMD_NEW,
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = flow_policy,
	  .doit = ovs_flow_cmd_new_or_set
=======
=======
>>>>>>> v3.18
	rcu_read_unlock();
	return skb->len;
}

static const struct nla_policy flow_policy[OVS_FLOW_ATTR_MAX + 1] = {
	[OVS_FLOW_ATTR_KEY] = { .type = NLA_NESTED },
	[OVS_FLOW_ATTR_ACTIONS] = { .type = NLA_NESTED },
	[OVS_FLOW_ATTR_CLEAR] = { .type = NLA_FLAG },
};

static const struct genl_ops dp_flow_genl_ops[] = {
	{ .cmd = OVS_FLOW_CMD_NEW,
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = flow_policy,
	  .doit = ovs_flow_cmd_new
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	},
	{ .cmd = OVS_FLOW_CMD_DEL,
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = flow_policy,
	  .doit = ovs_flow_cmd_del
	},
	{ .cmd = OVS_FLOW_CMD_GET,
	  .flags = 0,		    /* OK for unprivileged users. */
	  .policy = flow_policy,
	  .doit = ovs_flow_cmd_get,
	  .dumpit = ovs_flow_cmd_dump
	},
	{ .cmd = OVS_FLOW_CMD_SET,
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = flow_policy,
<<<<<<< HEAD
<<<<<<< HEAD
	  .doit = ovs_flow_cmd_new_or_set,
	},
};

static const struct nla_policy datapath_policy[OVS_DP_ATTR_MAX + 1] = {
	[OVS_DP_ATTR_NAME] = { .type = NLA_NUL_STRING, .len = IFNAMSIZ - 1 },
	[OVS_DP_ATTR_UPCALL_PID] = { .type = NLA_U32 },
};

static struct genl_family dp_datapath_genl_family = {
	.id = GENL_ID_GENERATE,
	.hdrsize = sizeof(struct ovs_header),
	.name = OVS_DATAPATH_FAMILY,
	.version = OVS_DATAPATH_VERSION,
	.maxattr = OVS_DP_ATTR_MAX,
	.netnsok = true,
	.parallel_ops = true,
};

static struct genl_multicast_group ovs_dp_datapath_multicast_group = {
	.name = OVS_DATAPATH_MCGROUP
=======
=======
>>>>>>> v3.18
	  .doit = ovs_flow_cmd_set,
	},
};

static struct genl_family dp_flow_genl_family = {
	.id = GENL_ID_GENERATE,
	.hdrsize = sizeof(struct ovs_header),
	.name = OVS_FLOW_FAMILY,
	.version = OVS_FLOW_VERSION,
	.maxattr = OVS_FLOW_ATTR_MAX,
	.netnsok = true,
	.parallel_ops = true,
	.ops = dp_flow_genl_ops,
	.n_ops = ARRAY_SIZE(dp_flow_genl_ops),
	.mcgrps = &ovs_dp_flow_multicast_group,
	.n_mcgrps = 1,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
};

static size_t ovs_dp_cmd_msg_size(void)
{
	size_t msgsize = NLMSG_ALIGN(sizeof(struct ovs_header));

	msgsize += nla_total_size(IFNAMSIZ);
	msgsize += nla_total_size(sizeof(struct ovs_dp_stats));
<<<<<<< HEAD
<<<<<<< HEAD
=======
	msgsize += nla_total_size(sizeof(struct ovs_dp_megaflow_stats));
	msgsize += nla_total_size(sizeof(u32)); /* OVS_DP_ATTR_USER_FEATURES */
>>>>>>> v3.18
=======
	msgsize += nla_total_size(sizeof(struct ovs_dp_megaflow_stats));
	msgsize += nla_total_size(sizeof(u32)); /* OVS_DP_ATTR_USER_FEATURES */
>>>>>>> v3.18

	return msgsize;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
/* Called with ovs_mutex. */
>>>>>>> v3.18
=======
/* Called with ovs_mutex. */
>>>>>>> v3.18
static int ovs_dp_cmd_fill_info(struct datapath *dp, struct sk_buff *skb,
				u32 portid, u32 seq, u32 flags, u8 cmd)
{
	struct ovs_header *ovs_header;
	struct ovs_dp_stats dp_stats;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct ovs_dp_megaflow_stats dp_megaflow_stats;
>>>>>>> v3.18
=======
	struct ovs_dp_megaflow_stats dp_megaflow_stats;
>>>>>>> v3.18
	int err;

	ovs_header = genlmsg_put(skb, portid, seq, &dp_datapath_genl_family,
				   flags, cmd);
	if (!ovs_header)
		goto error;

	ovs_header->dp_ifindex = get_dpifindex(dp);

<<<<<<< HEAD
<<<<<<< HEAD
	rcu_read_lock();
	err = nla_put_string(skb, OVS_DP_ATTR_NAME, ovs_dp_name(dp));
	rcu_read_unlock();
	if (err)
		goto nla_put_failure;

	get_dp_stats(dp, &dp_stats);
	if (nla_put(skb, OVS_DP_ATTR_STATS, sizeof(struct ovs_dp_stats), &dp_stats))
=======
=======
>>>>>>> v3.18
	err = nla_put_string(skb, OVS_DP_ATTR_NAME, ovs_dp_name(dp));
	if (err)
		goto nla_put_failure;

	get_dp_stats(dp, &dp_stats, &dp_megaflow_stats);
	if (nla_put(skb, OVS_DP_ATTR_STATS, sizeof(struct ovs_dp_stats),
			&dp_stats))
		goto nla_put_failure;

	if (nla_put(skb, OVS_DP_ATTR_MEGAFLOW_STATS,
			sizeof(struct ovs_dp_megaflow_stats),
			&dp_megaflow_stats))
		goto nla_put_failure;

	if (nla_put_u32(skb, OVS_DP_ATTR_USER_FEATURES, dp->user_features))
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		goto nla_put_failure;

	return genlmsg_end(skb, ovs_header);

nla_put_failure:
	genlmsg_cancel(skb, ovs_header);
error:
	return -EMSGSIZE;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct sk_buff *ovs_dp_cmd_build_info(struct datapath *dp, u32 portid,
					     u32 seq, u8 cmd)
{
	struct sk_buff *skb;
	int retval;

	skb = genlmsg_new(ovs_dp_cmd_msg_size(), GFP_KERNEL);
	if (!skb)
		return ERR_PTR(-ENOMEM);

	retval = ovs_dp_cmd_fill_info(dp, skb, portid, seq, 0, cmd);
	if (retval < 0) {
		kfree_skb(skb);
		return ERR_PTR(retval);
	}
	return skb;
}

/* Called with ovs_mutex. */
=======
=======
>>>>>>> v3.18
static struct sk_buff *ovs_dp_cmd_alloc_info(struct genl_info *info)
{
	return genlmsg_new_unicast(ovs_dp_cmd_msg_size(), info, GFP_KERNEL);
}

/* Called with rcu_read_lock or ovs_mutex. */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static struct datapath *lookup_datapath(struct net *net,
					struct ovs_header *ovs_header,
					struct nlattr *a[OVS_DP_ATTR_MAX + 1])
{
	struct datapath *dp;

	if (!a[OVS_DP_ATTR_NAME])
		dp = get_dp(net, ovs_header->dp_ifindex);
	else {
		struct vport *vport;

<<<<<<< HEAD
<<<<<<< HEAD
		rcu_read_lock();
		vport = ovs_vport_locate(net, nla_data(a[OVS_DP_ATTR_NAME]));
		dp = vport && vport->port_no == OVSP_LOCAL ? vport->dp : NULL;
		rcu_read_unlock();
=======
		vport = ovs_vport_locate(net, nla_data(a[OVS_DP_ATTR_NAME]));
		dp = vport && vport->port_no == OVSP_LOCAL ? vport->dp : NULL;
>>>>>>> v3.18
=======
		vport = ovs_vport_locate(net, nla_data(a[OVS_DP_ATTR_NAME]));
		dp = vport && vport->port_no == OVSP_LOCAL ? vport->dp : NULL;
>>>>>>> v3.18
	}
	return dp ? dp : ERR_PTR(-ENODEV);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
static void ovs_dp_reset_user_features(struct sk_buff *skb, struct genl_info *info)
{
	struct datapath *dp;

	dp = lookup_datapath(sock_net(skb->sk), info->userhdr, info->attrs);
	if (IS_ERR(dp))
		return;

	WARN(dp->user_features, "Dropping previously announced user features\n");
	dp->user_features = 0;
}

static void ovs_dp_change(struct datapath *dp, struct nlattr **a)
{
	if (a[OVS_DP_ATTR_USER_FEATURES])
		dp->user_features = nla_get_u32(a[OVS_DP_ATTR_USER_FEATURES]);
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static int ovs_dp_cmd_new(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **a = info->attrs;
	struct vport_parms parms;
	struct sk_buff *reply;
	struct datapath *dp;
	struct vport *vport;
	struct ovs_net *ovs_net;
	int err, i;

	err = -EINVAL;
	if (!a[OVS_DP_ATTR_NAME] || !a[OVS_DP_ATTR_UPCALL_PID])
		goto err;

<<<<<<< HEAD
<<<<<<< HEAD
	ovs_lock();
=======
	reply = ovs_dp_cmd_alloc_info(info);
	if (!reply)
		return -ENOMEM;
>>>>>>> v3.18
=======
	reply = ovs_dp_cmd_alloc_info(info);
	if (!reply)
		return -ENOMEM;
>>>>>>> v3.18

	err = -ENOMEM;
	dp = kzalloc(sizeof(*dp), GFP_KERNEL);
	if (dp == NULL)
<<<<<<< HEAD
<<<<<<< HEAD
		goto err_unlock_ovs;
=======
		goto err_free_reply;
>>>>>>> v3.18
=======
		goto err_free_reply;
>>>>>>> v3.18

	ovs_dp_set_net(dp, hold_net(sock_net(skb->sk)));

	/* Allocate table. */
<<<<<<< HEAD
<<<<<<< HEAD
	err = -ENOMEM;
	rcu_assign_pointer(dp->table, ovs_flow_tbl_alloc(TBL_MIN_BUCKETS));
	if (!dp->table)
		goto err_free_dp;

	dp->stats_percpu = alloc_percpu(struct dp_stats_percpu);
=======
=======
>>>>>>> v3.18
	err = ovs_flow_tbl_init(&dp->table);
	if (err)
		goto err_free_dp;

	dp->stats_percpu = netdev_alloc_pcpu_stats(struct dp_stats_percpu);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (!dp->stats_percpu) {
		err = -ENOMEM;
		goto err_destroy_table;
	}

	dp->ports = kmalloc(DP_VPORT_HASH_BUCKETS * sizeof(struct hlist_head),
<<<<<<< HEAD
<<<<<<< HEAD
			GFP_KERNEL);
=======
			    GFP_KERNEL);
>>>>>>> v3.18
=======
			    GFP_KERNEL);
>>>>>>> v3.18
	if (!dp->ports) {
		err = -ENOMEM;
		goto err_destroy_percpu;
	}

	for (i = 0; i < DP_VPORT_HASH_BUCKETS; i++)
		INIT_HLIST_HEAD(&dp->ports[i]);

	/* Set up our datapath device. */
	parms.name = nla_data(a[OVS_DP_ATTR_NAME]);
	parms.type = OVS_VPORT_TYPE_INTERNAL;
	parms.options = NULL;
	parms.dp = dp;
	parms.port_no = OVSP_LOCAL;
<<<<<<< HEAD
<<<<<<< HEAD
	parms.upcall_portid = nla_get_u32(a[OVS_DP_ATTR_UPCALL_PID]);
=======
=======
>>>>>>> v3.18
	parms.upcall_portids = a[OVS_DP_ATTR_UPCALL_PID];

	ovs_dp_change(dp, a);

	/* So far only local changes have been made, now need the lock. */
	ovs_lock();
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	vport = new_vport(&parms);
	if (IS_ERR(vport)) {
		err = PTR_ERR(vport);
		if (err == -EBUSY)
			err = -EEXIST;

<<<<<<< HEAD
<<<<<<< HEAD
		goto err_destroy_ports_array;
	}

	reply = ovs_dp_cmd_build_info(dp, info->snd_portid,
				      info->snd_seq, OVS_DP_CMD_NEW);
	err = PTR_ERR(reply);
	if (IS_ERR(reply))
		goto err_destroy_local_port;

	ovs_net = net_generic(ovs_dp_get_net(dp), ovs_net_id);
	list_add_tail(&dp->list_node, &ovs_net->dps);

	ovs_unlock();

	ovs_notify(reply, info, &ovs_dp_datapath_multicast_group);
	return 0;

err_destroy_local_port:
	ovs_dp_detach_port(ovs_vport_ovsl(dp, OVSP_LOCAL));
err_destroy_ports_array:
=======
=======
>>>>>>> v3.18
		if (err == -EEXIST) {
			/* An outdated user space instance that does not understand
			 * the concept of user_features has attempted to create a new
			 * datapath and is likely to reuse it. Drop all user features.
			 */
			if (info->genlhdr->version < OVS_DP_VER_FEATURES)
				ovs_dp_reset_user_features(skb, info);
		}

		goto err_destroy_ports_array;
	}

	err = ovs_dp_cmd_fill_info(dp, reply, info->snd_portid,
				   info->snd_seq, 0, OVS_DP_CMD_NEW);
	BUG_ON(err < 0);

	ovs_net = net_generic(ovs_dp_get_net(dp), ovs_net_id);
	list_add_tail_rcu(&dp->list_node, &ovs_net->dps);

	ovs_unlock();

	ovs_notify(&dp_datapath_genl_family, reply, info);
	return 0;

err_destroy_ports_array:
	ovs_unlock();
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	kfree(dp->ports);
err_destroy_percpu:
	free_percpu(dp->stats_percpu);
err_destroy_table:
<<<<<<< HEAD
<<<<<<< HEAD
	ovs_flow_tbl_destroy(ovsl_dereference(dp->table));
err_free_dp:
	release_net(ovs_dp_get_net(dp));
	kfree(dp);
err_unlock_ovs:
	ovs_unlock();
=======
=======
>>>>>>> v3.18
	ovs_flow_tbl_destroy(&dp->table, false);
err_free_dp:
	release_net(ovs_dp_get_net(dp));
	kfree(dp);
err_free_reply:
	kfree_skb(reply);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
err:
	return err;
}

/* Called with ovs_mutex. */
static void __dp_destroy(struct datapath *dp)
{
	int i;

	for (i = 0; i < DP_VPORT_HASH_BUCKETS; i++) {
		struct vport *vport;
		struct hlist_node *n;

		hlist_for_each_entry_safe(vport, n, &dp->ports[i], dp_hash_node)
			if (vport->port_no != OVSP_LOCAL)
				ovs_dp_detach_port(vport);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	list_del(&dp->list_node);

	/* OVSP_LOCAL is datapath internal port. We need to make sure that
	 * all port in datapath are destroyed first before freeing datapath.
	 */
	ovs_dp_detach_port(ovs_vport_ovsl(dp, OVSP_LOCAL));

=======
=======
>>>>>>> v3.18
	list_del_rcu(&dp->list_node);

	/* OVSP_LOCAL is datapath internal port. We need to make sure that
	 * all ports in datapath are destroyed first before freeing datapath.
	 */
	ovs_dp_detach_port(ovs_vport_ovsl(dp, OVSP_LOCAL));

	/* RCU destroy the flow table */
	ovs_flow_tbl_destroy(&dp->table, true);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	call_rcu(&dp->rcu, destroy_dp_rcu);
}

static int ovs_dp_cmd_del(struct sk_buff *skb, struct genl_info *info)
{
	struct sk_buff *reply;
	struct datapath *dp;
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	reply = ovs_dp_cmd_alloc_info(info);
	if (!reply)
		return -ENOMEM;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	ovs_lock();
	dp = lookup_datapath(sock_net(skb->sk), info->userhdr, info->attrs);
	err = PTR_ERR(dp);
	if (IS_ERR(dp))
<<<<<<< HEAD
<<<<<<< HEAD
		goto unlock;

	reply = ovs_dp_cmd_build_info(dp, info->snd_portid,
				      info->snd_seq, OVS_DP_CMD_DEL);
	err = PTR_ERR(reply);
	if (IS_ERR(reply))
		goto unlock;
=======
=======
>>>>>>> v3.18
		goto err_unlock_free;

	err = ovs_dp_cmd_fill_info(dp, reply, info->snd_portid,
				   info->snd_seq, 0, OVS_DP_CMD_DEL);
	BUG_ON(err < 0);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	__dp_destroy(dp);
	ovs_unlock();

<<<<<<< HEAD
<<<<<<< HEAD
	ovs_notify(reply, info, &ovs_dp_datapath_multicast_group);

	return 0;
unlock:
	ovs_unlock();
=======
=======
>>>>>>> v3.18
	ovs_notify(&dp_datapath_genl_family, reply, info);

	return 0;

err_unlock_free:
	ovs_unlock();
	kfree_skb(reply);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return err;
}

static int ovs_dp_cmd_set(struct sk_buff *skb, struct genl_info *info)
{
	struct sk_buff *reply;
	struct datapath *dp;
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	reply = ovs_dp_cmd_alloc_info(info);
	if (!reply)
		return -ENOMEM;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	ovs_lock();
	dp = lookup_datapath(sock_net(skb->sk), info->userhdr, info->attrs);
	err = PTR_ERR(dp);
	if (IS_ERR(dp))
<<<<<<< HEAD
<<<<<<< HEAD
		goto unlock;

	reply = ovs_dp_cmd_build_info(dp, info->snd_portid,
				      info->snd_seq, OVS_DP_CMD_NEW);
	if (IS_ERR(reply)) {
		err = PTR_ERR(reply);
		netlink_set_err(sock_net(skb->sk)->genl_sock, 0,
				ovs_dp_datapath_multicast_group.id, err);
		err = 0;
		goto unlock;
	}

	ovs_unlock();
	ovs_notify(reply, info, &ovs_dp_datapath_multicast_group);

	return 0;
unlock:
	ovs_unlock();
=======
=======
>>>>>>> v3.18
		goto err_unlock_free;

	ovs_dp_change(dp, info->attrs);

	err = ovs_dp_cmd_fill_info(dp, reply, info->snd_portid,
				   info->snd_seq, 0, OVS_DP_CMD_NEW);
	BUG_ON(err < 0);

	ovs_unlock();
	ovs_notify(&dp_datapath_genl_family, reply, info);

	return 0;

err_unlock_free:
	ovs_unlock();
	kfree_skb(reply);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return err;
}

static int ovs_dp_cmd_get(struct sk_buff *skb, struct genl_info *info)
{
	struct sk_buff *reply;
	struct datapath *dp;
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	reply = ovs_dp_cmd_alloc_info(info);
	if (!reply)
		return -ENOMEM;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	ovs_lock();
	dp = lookup_datapath(sock_net(skb->sk), info->userhdr, info->attrs);
	if (IS_ERR(dp)) {
		err = PTR_ERR(dp);
<<<<<<< HEAD
<<<<<<< HEAD
		goto unlock;
	}

	reply = ovs_dp_cmd_build_info(dp, info->snd_portid,
				      info->snd_seq, OVS_DP_CMD_NEW);
	if (IS_ERR(reply)) {
		err = PTR_ERR(reply);
		goto unlock;
	}

	ovs_unlock();
	return genlmsg_reply(reply, info);

unlock:
	ovs_unlock();
=======
=======
>>>>>>> v3.18
		goto err_unlock_free;
	}
	err = ovs_dp_cmd_fill_info(dp, reply, info->snd_portid,
				   info->snd_seq, 0, OVS_DP_CMD_NEW);
	BUG_ON(err < 0);
	ovs_unlock();

	return genlmsg_reply(reply, info);

err_unlock_free:
	ovs_unlock();
	kfree_skb(reply);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return err;
}

static int ovs_dp_cmd_dump(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct ovs_net *ovs_net = net_generic(sock_net(skb->sk), ovs_net_id);
	struct datapath *dp;
	int skip = cb->args[0];
	int i = 0;

	ovs_lock();
	list_for_each_entry(dp, &ovs_net->dps, list_node) {
		if (i >= skip &&
		    ovs_dp_cmd_fill_info(dp, skb, NETLINK_CB(cb->skb).portid,
					 cb->nlh->nlmsg_seq, NLM_F_MULTI,
					 OVS_DP_CMD_NEW) < 0)
			break;
		i++;
	}
	ovs_unlock();

	cb->args[0] = i;

	return skb->len;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct genl_ops dp_datapath_genl_ops[] = {
=======
=======
>>>>>>> v3.18
static const struct nla_policy datapath_policy[OVS_DP_ATTR_MAX + 1] = {
	[OVS_DP_ATTR_NAME] = { .type = NLA_NUL_STRING, .len = IFNAMSIZ - 1 },
	[OVS_DP_ATTR_UPCALL_PID] = { .type = NLA_U32 },
	[OVS_DP_ATTR_USER_FEATURES] = { .type = NLA_U32 },
};

static const struct genl_ops dp_datapath_genl_ops[] = {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	{ .cmd = OVS_DP_CMD_NEW,
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = datapath_policy,
	  .doit = ovs_dp_cmd_new
	},
	{ .cmd = OVS_DP_CMD_DEL,
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = datapath_policy,
	  .doit = ovs_dp_cmd_del
	},
	{ .cmd = OVS_DP_CMD_GET,
	  .flags = 0,		    /* OK for unprivileged users. */
	  .policy = datapath_policy,
	  .doit = ovs_dp_cmd_get,
	  .dumpit = ovs_dp_cmd_dump
	},
	{ .cmd = OVS_DP_CMD_SET,
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = datapath_policy,
	  .doit = ovs_dp_cmd_set,
	},
};

<<<<<<< HEAD
<<<<<<< HEAD
static const struct nla_policy vport_policy[OVS_VPORT_ATTR_MAX + 1] = {
	[OVS_VPORT_ATTR_NAME] = { .type = NLA_NUL_STRING, .len = IFNAMSIZ - 1 },
	[OVS_VPORT_ATTR_STATS] = { .len = sizeof(struct ovs_vport_stats) },
	[OVS_VPORT_ATTR_PORT_NO] = { .type = NLA_U32 },
	[OVS_VPORT_ATTR_TYPE] = { .type = NLA_U32 },
	[OVS_VPORT_ATTR_UPCALL_PID] = { .type = NLA_U32 },
	[OVS_VPORT_ATTR_OPTIONS] = { .type = NLA_NESTED },
};

static struct genl_family dp_vport_genl_family = {
	.id = GENL_ID_GENERATE,
	.hdrsize = sizeof(struct ovs_header),
	.name = OVS_VPORT_FAMILY,
	.version = OVS_VPORT_VERSION,
	.maxattr = OVS_VPORT_ATTR_MAX,
	.netnsok = true,
	.parallel_ops = true,
};

struct genl_multicast_group ovs_dp_vport_multicast_group = {
	.name = OVS_VPORT_MCGROUP
=======
=======
>>>>>>> v3.18
static struct genl_family dp_datapath_genl_family = {
	.id = GENL_ID_GENERATE,
	.hdrsize = sizeof(struct ovs_header),
	.name = OVS_DATAPATH_FAMILY,
	.version = OVS_DATAPATH_VERSION,
	.maxattr = OVS_DP_ATTR_MAX,
	.netnsok = true,
	.parallel_ops = true,
	.ops = dp_datapath_genl_ops,
	.n_ops = ARRAY_SIZE(dp_datapath_genl_ops),
	.mcgrps = &ovs_dp_datapath_multicast_group,
	.n_mcgrps = 1,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
};

/* Called with ovs_mutex or RCU read lock. */
static int ovs_vport_cmd_fill_info(struct vport *vport, struct sk_buff *skb,
				   u32 portid, u32 seq, u32 flags, u8 cmd)
{
	struct ovs_header *ovs_header;
	struct ovs_vport_stats vport_stats;
	int err;

	ovs_header = genlmsg_put(skb, portid, seq, &dp_vport_genl_family,
				 flags, cmd);
	if (!ovs_header)
		return -EMSGSIZE;

	ovs_header->dp_ifindex = get_dpifindex(vport->dp);

	if (nla_put_u32(skb, OVS_VPORT_ATTR_PORT_NO, vport->port_no) ||
	    nla_put_u32(skb, OVS_VPORT_ATTR_TYPE, vport->ops->type) ||
<<<<<<< HEAD
<<<<<<< HEAD
	    nla_put_string(skb, OVS_VPORT_ATTR_NAME, vport->ops->get_name(vport)) ||
	    nla_put_u32(skb, OVS_VPORT_ATTR_UPCALL_PID, vport->upcall_portid))
=======
	    nla_put_string(skb, OVS_VPORT_ATTR_NAME,
			   vport->ops->get_name(vport)))
>>>>>>> v3.18
=======
	    nla_put_string(skb, OVS_VPORT_ATTR_NAME,
			   vport->ops->get_name(vport)))
>>>>>>> v3.18
		goto nla_put_failure;

	ovs_vport_get_stats(vport, &vport_stats);
	if (nla_put(skb, OVS_VPORT_ATTR_STATS, sizeof(struct ovs_vport_stats),
		    &vport_stats))
		goto nla_put_failure;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (ovs_vport_get_upcall_portids(vport, skb))
		goto nla_put_failure;

>>>>>>> v3.18
=======
	if (ovs_vport_get_upcall_portids(vport, skb))
		goto nla_put_failure;

>>>>>>> v3.18
	err = ovs_vport_get_options(vport, skb);
	if (err == -EMSGSIZE)
		goto error;

	return genlmsg_end(skb, ovs_header);

nla_put_failure:
	err = -EMSGSIZE;
error:
	genlmsg_cancel(skb, ovs_header);
	return err;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* Called with ovs_mutex or RCU read lock. */
=======
=======
>>>>>>> v3.18
static struct sk_buff *ovs_vport_cmd_alloc_info(void)
{
	return nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
}

/* Called with ovs_mutex, only via ovs_dp_notify_wq(). */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
struct sk_buff *ovs_vport_cmd_build_info(struct vport *vport, u32 portid,
					 u32 seq, u8 cmd)
{
	struct sk_buff *skb;
	int retval;

	skb = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_ATOMIC);
	if (!skb)
		return ERR_PTR(-ENOMEM);

	retval = ovs_vport_cmd_fill_info(vport, skb, portid, seq, 0, cmd);
	BUG_ON(retval < 0);

	return skb;
}

/* Called with ovs_mutex or RCU read lock. */
static struct vport *lookup_vport(struct net *net,
				  struct ovs_header *ovs_header,
				  struct nlattr *a[OVS_VPORT_ATTR_MAX + 1])
{
	struct datapath *dp;
	struct vport *vport;

	if (a[OVS_VPORT_ATTR_NAME]) {
		vport = ovs_vport_locate(net, nla_data(a[OVS_VPORT_ATTR_NAME]));
		if (!vport)
			return ERR_PTR(-ENODEV);
		if (ovs_header->dp_ifindex &&
		    ovs_header->dp_ifindex != get_dpifindex(vport->dp))
			return ERR_PTR(-ENODEV);
		return vport;
	} else if (a[OVS_VPORT_ATTR_PORT_NO]) {
		u32 port_no = nla_get_u32(a[OVS_VPORT_ATTR_PORT_NO]);

		if (port_no >= DP_MAX_PORTS)
			return ERR_PTR(-EFBIG);

		dp = get_dp(net, ovs_header->dp_ifindex);
		if (!dp)
			return ERR_PTR(-ENODEV);

		vport = ovs_vport_ovsl_rcu(dp, port_no);
		if (!vport)
			return ERR_PTR(-ENODEV);
		return vport;
	} else
		return ERR_PTR(-EINVAL);
}

static int ovs_vport_cmd_new(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **a = info->attrs;
	struct ovs_header *ovs_header = info->userhdr;
	struct vport_parms parms;
	struct sk_buff *reply;
	struct vport *vport;
	struct datapath *dp;
	u32 port_no;
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
	err = -EINVAL;
	if (!a[OVS_VPORT_ATTR_NAME] || !a[OVS_VPORT_ATTR_TYPE] ||
	    !a[OVS_VPORT_ATTR_UPCALL_PID])
		goto exit;
=======
=======
>>>>>>> v3.18
	if (!a[OVS_VPORT_ATTR_NAME] || !a[OVS_VPORT_ATTR_TYPE] ||
	    !a[OVS_VPORT_ATTR_UPCALL_PID])
		return -EINVAL;

	port_no = a[OVS_VPORT_ATTR_PORT_NO]
		? nla_get_u32(a[OVS_VPORT_ATTR_PORT_NO]) : 0;
	if (port_no >= DP_MAX_PORTS)
		return -EFBIG;

	reply = ovs_vport_cmd_alloc_info();
	if (!reply)
		return -ENOMEM;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	ovs_lock();
	dp = get_dp(sock_net(skb->sk), ovs_header->dp_ifindex);
	err = -ENODEV;
	if (!dp)
<<<<<<< HEAD
<<<<<<< HEAD
		goto exit_unlock;

	if (a[OVS_VPORT_ATTR_PORT_NO]) {
		port_no = nla_get_u32(a[OVS_VPORT_ATTR_PORT_NO]);

		err = -EFBIG;
		if (port_no >= DP_MAX_PORTS)
			goto exit_unlock;

		vport = ovs_vport_ovsl(dp, port_no);
		err = -EBUSY;
		if (vport)
			goto exit_unlock;
=======
=======
>>>>>>> v3.18
		goto exit_unlock_free;

	if (port_no) {
		vport = ovs_vport_ovsl(dp, port_no);
		err = -EBUSY;
		if (vport)
			goto exit_unlock_free;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	} else {
		for (port_no = 1; ; port_no++) {
			if (port_no >= DP_MAX_PORTS) {
				err = -EFBIG;
<<<<<<< HEAD
<<<<<<< HEAD
				goto exit_unlock;
=======
				goto exit_unlock_free;
>>>>>>> v3.18
=======
				goto exit_unlock_free;
>>>>>>> v3.18
			}
			vport = ovs_vport_ovsl(dp, port_no);
			if (!vport)
				break;
		}
	}

	parms.name = nla_data(a[OVS_VPORT_ATTR_NAME]);
	parms.type = nla_get_u32(a[OVS_VPORT_ATTR_TYPE]);
	parms.options = a[OVS_VPORT_ATTR_OPTIONS];
	parms.dp = dp;
	parms.port_no = port_no;
<<<<<<< HEAD
<<<<<<< HEAD
	parms.upcall_portid = nla_get_u32(a[OVS_VPORT_ATTR_UPCALL_PID]);
=======
	parms.upcall_portids = a[OVS_VPORT_ATTR_UPCALL_PID];
>>>>>>> v3.18
=======
	parms.upcall_portids = a[OVS_VPORT_ATTR_UPCALL_PID];
>>>>>>> v3.18

	vport = new_vport(&parms);
	err = PTR_ERR(vport);
	if (IS_ERR(vport))
<<<<<<< HEAD
<<<<<<< HEAD
		goto exit_unlock;

	err = 0;
	reply = ovs_vport_cmd_build_info(vport, info->snd_portid, info->snd_seq,
					 OVS_VPORT_CMD_NEW);
	if (IS_ERR(reply)) {
		err = PTR_ERR(reply);
		ovs_dp_detach_port(vport);
		goto exit_unlock;
	}

	ovs_notify(reply, info, &ovs_dp_vport_multicast_group);

exit_unlock:
	ovs_unlock();
exit:
=======
=======
>>>>>>> v3.18
		goto exit_unlock_free;

	err = ovs_vport_cmd_fill_info(vport, reply, info->snd_portid,
				      info->snd_seq, 0, OVS_VPORT_CMD_NEW);
	BUG_ON(err < 0);
	ovs_unlock();

	ovs_notify(&dp_vport_genl_family, reply, info);
	return 0;

exit_unlock_free:
	ovs_unlock();
	kfree_skb(reply);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return err;
}

static int ovs_vport_cmd_set(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **a = info->attrs;
	struct sk_buff *reply;
	struct vport *vport;
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	reply = ovs_vport_cmd_alloc_info();
	if (!reply)
		return -ENOMEM;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	ovs_lock();
	vport = lookup_vport(sock_net(skb->sk), info->userhdr, a);
	err = PTR_ERR(vport);
	if (IS_ERR(vport))
<<<<<<< HEAD
<<<<<<< HEAD
		goto exit_unlock;

	err = 0;
	if (a[OVS_VPORT_ATTR_TYPE] &&
	    nla_get_u32(a[OVS_VPORT_ATTR_TYPE]) != vport->ops->type)
		err = -EINVAL;

	reply = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!reply) {
		err = -ENOMEM;
		goto exit_unlock;
	}

	if (!err && a[OVS_VPORT_ATTR_OPTIONS])
		err = ovs_vport_set_options(vport, a[OVS_VPORT_ATTR_OPTIONS]);
	if (err)
		goto exit_free;

	if (a[OVS_VPORT_ATTR_UPCALL_PID])
		vport->upcall_portid = nla_get_u32(a[OVS_VPORT_ATTR_UPCALL_PID]);
=======
=======
>>>>>>> v3.18
		goto exit_unlock_free;

	if (a[OVS_VPORT_ATTR_TYPE] &&
	    nla_get_u32(a[OVS_VPORT_ATTR_TYPE]) != vport->ops->type) {
		err = -EINVAL;
		goto exit_unlock_free;
	}

	if (a[OVS_VPORT_ATTR_OPTIONS]) {
		err = ovs_vport_set_options(vport, a[OVS_VPORT_ATTR_OPTIONS]);
		if (err)
			goto exit_unlock_free;
	}


	if (a[OVS_VPORT_ATTR_UPCALL_PID]) {
		struct nlattr *ids = a[OVS_VPORT_ATTR_UPCALL_PID];

		err = ovs_vport_set_upcall_portids(vport, ids);
		if (err)
			goto exit_unlock_free;
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	err = ovs_vport_cmd_fill_info(vport, reply, info->snd_portid,
				      info->snd_seq, 0, OVS_VPORT_CMD_NEW);
	BUG_ON(err < 0);

	ovs_unlock();
<<<<<<< HEAD
<<<<<<< HEAD
	ovs_notify(reply, info, &ovs_dp_vport_multicast_group);
	return 0;

	rtnl_unlock();
	return 0;

exit_free:
	kfree_skb(reply);
exit_unlock:
	ovs_unlock();
=======
=======
>>>>>>> v3.18
	ovs_notify(&dp_vport_genl_family, reply, info);
	return 0;

exit_unlock_free:
	ovs_unlock();
	kfree_skb(reply);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return err;
}

static int ovs_vport_cmd_del(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **a = info->attrs;
	struct sk_buff *reply;
	struct vport *vport;
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	reply = ovs_vport_cmd_alloc_info();
	if (!reply)
		return -ENOMEM;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	ovs_lock();
	vport = lookup_vport(sock_net(skb->sk), info->userhdr, a);
	err = PTR_ERR(vport);
	if (IS_ERR(vport))
<<<<<<< HEAD
<<<<<<< HEAD
		goto exit_unlock;

	if (vport->port_no == OVSP_LOCAL) {
		err = -EINVAL;
		goto exit_unlock;
	}

	reply = ovs_vport_cmd_build_info(vport, info->snd_portid, info->snd_seq,
					 OVS_VPORT_CMD_DEL);
	err = PTR_ERR(reply);
	if (IS_ERR(reply))
		goto exit_unlock;

	err = 0;
	ovs_dp_detach_port(vport);

	ovs_notify(reply, info, &ovs_dp_vport_multicast_group);

exit_unlock:
	ovs_unlock();
=======
=======
>>>>>>> v3.18
		goto exit_unlock_free;

	if (vport->port_no == OVSP_LOCAL) {
		err = -EINVAL;
		goto exit_unlock_free;
	}

	err = ovs_vport_cmd_fill_info(vport, reply, info->snd_portid,
				      info->snd_seq, 0, OVS_VPORT_CMD_DEL);
	BUG_ON(err < 0);
	ovs_dp_detach_port(vport);
	ovs_unlock();

	ovs_notify(&dp_vport_genl_family, reply, info);
	return 0;

exit_unlock_free:
	ovs_unlock();
	kfree_skb(reply);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return err;
}

static int ovs_vport_cmd_get(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **a = info->attrs;
	struct ovs_header *ovs_header = info->userhdr;
	struct sk_buff *reply;
	struct vport *vport;
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	reply = ovs_vport_cmd_alloc_info();
	if (!reply)
		return -ENOMEM;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	rcu_read_lock();
	vport = lookup_vport(sock_net(skb->sk), ovs_header, a);
	err = PTR_ERR(vport);
	if (IS_ERR(vport))
<<<<<<< HEAD
<<<<<<< HEAD
		goto exit_unlock;

	reply = ovs_vport_cmd_build_info(vport, info->snd_portid, info->snd_seq,
					 OVS_VPORT_CMD_NEW);
	err = PTR_ERR(reply);
	if (IS_ERR(reply))
		goto exit_unlock;

=======
=======
>>>>>>> v3.18
		goto exit_unlock_free;
	err = ovs_vport_cmd_fill_info(vport, reply, info->snd_portid,
				      info->snd_seq, 0, OVS_VPORT_CMD_NEW);
	BUG_ON(err < 0);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	rcu_read_unlock();

	return genlmsg_reply(reply, info);

<<<<<<< HEAD
<<<<<<< HEAD
exit_unlock:
	rcu_read_unlock();
=======
exit_unlock_free:
	rcu_read_unlock();
	kfree_skb(reply);
>>>>>>> v3.18
=======
exit_unlock_free:
	rcu_read_unlock();
	kfree_skb(reply);
>>>>>>> v3.18
	return err;
}

static int ovs_vport_cmd_dump(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct ovs_header *ovs_header = genlmsg_data(nlmsg_data(cb->nlh));
	struct datapath *dp;
	int bucket = cb->args[0], skip = cb->args[1];
	int i, j = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	dp = get_dp(sock_net(skb->sk), ovs_header->dp_ifindex);
	if (!dp)
		return -ENODEV;

	rcu_read_lock();
=======
=======
>>>>>>> v3.18
	rcu_read_lock();
	dp = get_dp(sock_net(skb->sk), ovs_header->dp_ifindex);
	if (!dp) {
		rcu_read_unlock();
		return -ENODEV;
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	for (i = bucket; i < DP_VPORT_HASH_BUCKETS; i++) {
		struct vport *vport;

		j = 0;
		hlist_for_each_entry_rcu(vport, &dp->ports[i], dp_hash_node) {
			if (j >= skip &&
			    ovs_vport_cmd_fill_info(vport, skb,
						    NETLINK_CB(cb->skb).portid,
						    cb->nlh->nlmsg_seq,
						    NLM_F_MULTI,
						    OVS_VPORT_CMD_NEW) < 0)
				goto out;

			j++;
		}
		skip = 0;
	}
out:
	rcu_read_unlock();

	cb->args[0] = i;
	cb->args[1] = j;

	return skb->len;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct genl_ops dp_vport_genl_ops[] = {
=======
=======
>>>>>>> v3.18
static const struct nla_policy vport_policy[OVS_VPORT_ATTR_MAX + 1] = {
	[OVS_VPORT_ATTR_NAME] = { .type = NLA_NUL_STRING, .len = IFNAMSIZ - 1 },
	[OVS_VPORT_ATTR_STATS] = { .len = sizeof(struct ovs_vport_stats) },
	[OVS_VPORT_ATTR_PORT_NO] = { .type = NLA_U32 },
	[OVS_VPORT_ATTR_TYPE] = { .type = NLA_U32 },
	[OVS_VPORT_ATTR_UPCALL_PID] = { .type = NLA_U32 },
	[OVS_VPORT_ATTR_OPTIONS] = { .type = NLA_NESTED },
};

static const struct genl_ops dp_vport_genl_ops[] = {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	{ .cmd = OVS_VPORT_CMD_NEW,
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = vport_policy,
	  .doit = ovs_vport_cmd_new
	},
	{ .cmd = OVS_VPORT_CMD_DEL,
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = vport_policy,
	  .doit = ovs_vport_cmd_del
	},
	{ .cmd = OVS_VPORT_CMD_GET,
	  .flags = 0,		    /* OK for unprivileged users. */
	  .policy = vport_policy,
	  .doit = ovs_vport_cmd_get,
	  .dumpit = ovs_vport_cmd_dump
	},
	{ .cmd = OVS_VPORT_CMD_SET,
	  .flags = GENL_ADMIN_PERM, /* Requires CAP_NET_ADMIN privilege. */
	  .policy = vport_policy,
	  .doit = ovs_vport_cmd_set,
	},
};

<<<<<<< HEAD
<<<<<<< HEAD
struct genl_family_and_ops {
	struct genl_family *family;
	struct genl_ops *ops;
	int n_ops;
	struct genl_multicast_group *group;
};

static const struct genl_family_and_ops dp_genl_families[] = {
	{ &dp_datapath_genl_family,
	  dp_datapath_genl_ops, ARRAY_SIZE(dp_datapath_genl_ops),
	  &ovs_dp_datapath_multicast_group },
	{ &dp_vport_genl_family,
	  dp_vport_genl_ops, ARRAY_SIZE(dp_vport_genl_ops),
	  &ovs_dp_vport_multicast_group },
	{ &dp_flow_genl_family,
	  dp_flow_genl_ops, ARRAY_SIZE(dp_flow_genl_ops),
	  &ovs_dp_flow_multicast_group },
	{ &dp_packet_genl_family,
	  dp_packet_genl_ops, ARRAY_SIZE(dp_packet_genl_ops),
	  NULL },
=======
=======
>>>>>>> v3.18
struct genl_family dp_vport_genl_family = {
	.id = GENL_ID_GENERATE,
	.hdrsize = sizeof(struct ovs_header),
	.name = OVS_VPORT_FAMILY,
	.version = OVS_VPORT_VERSION,
	.maxattr = OVS_VPORT_ATTR_MAX,
	.netnsok = true,
	.parallel_ops = true,
	.ops = dp_vport_genl_ops,
	.n_ops = ARRAY_SIZE(dp_vport_genl_ops),
	.mcgrps = &ovs_dp_vport_multicast_group,
	.n_mcgrps = 1,
};

static struct genl_family * const dp_genl_families[] = {
	&dp_datapath_genl_family,
	&dp_vport_genl_family,
	&dp_flow_genl_family,
	&dp_packet_genl_family,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
};

static void dp_unregister_genl(int n_families)
{
	int i;

	for (i = 0; i < n_families; i++)
<<<<<<< HEAD
<<<<<<< HEAD
		genl_unregister_family(dp_genl_families[i].family);
=======
		genl_unregister_family(dp_genl_families[i]);
>>>>>>> v3.18
=======
		genl_unregister_family(dp_genl_families[i]);
>>>>>>> v3.18
}

static int dp_register_genl(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int n_registered;
	int err;
	int i;

	n_registered = 0;
	for (i = 0; i < ARRAY_SIZE(dp_genl_families); i++) {
		const struct genl_family_and_ops *f = &dp_genl_families[i];

		err = genl_register_family_with_ops(f->family, f->ops,
						    f->n_ops);
		if (err)
			goto error;
		n_registered++;

		if (f->group) {
			err = genl_register_mc_group(f->family, f->group);
			if (err)
				goto error;
		}
=======
=======
>>>>>>> v3.18
	int err;
	int i;

	for (i = 0; i < ARRAY_SIZE(dp_genl_families); i++) {

		err = genl_register_family(dp_genl_families[i]);
		if (err)
			goto error;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}

	return 0;

error:
<<<<<<< HEAD
<<<<<<< HEAD
	dp_unregister_genl(n_registered);
	return err;
}

static void rehash_flow_table(struct work_struct *work)
{
	struct datapath *dp;
	struct net *net;

	ovs_lock();
	rtnl_lock();
	for_each_net(net) {
		struct ovs_net *ovs_net = net_generic(net, ovs_net_id);

		list_for_each_entry(dp, &ovs_net->dps, list_node) {
			struct flow_table *old_table = ovsl_dereference(dp->table);
			struct flow_table *new_table;

			new_table = ovs_flow_tbl_rehash(old_table);
			if (!IS_ERR(new_table)) {
				rcu_assign_pointer(dp->table, new_table);
				ovs_flow_tbl_deferred_destroy(old_table);
			}
		}
	}
	rtnl_unlock();
	ovs_unlock();
	schedule_delayed_work(&rehash_flow_wq, REHASH_FLOW_INTERVAL);
}

=======
=======
>>>>>>> v3.18
	dp_unregister_genl(i);
	return err;
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static int __net_init ovs_init_net(struct net *net)
{
	struct ovs_net *ovs_net = net_generic(net, ovs_net_id);

	INIT_LIST_HEAD(&ovs_net->dps);
	INIT_WORK(&ovs_net->dp_notify_work, ovs_dp_notify_wq);
	return 0;
}

static void __net_exit ovs_exit_net(struct net *net)
{
	struct datapath *dp, *dp_next;
	struct ovs_net *ovs_net = net_generic(net, ovs_net_id);

	ovs_lock();
	list_for_each_entry_safe(dp, dp_next, &ovs_net->dps, list_node)
		__dp_destroy(dp);
	ovs_unlock();

	cancel_work_sync(&ovs_net->dp_notify_work);
}

static struct pernet_operations ovs_net_ops = {
	.init = ovs_init_net,
	.exit = ovs_exit_net,
	.id   = &ovs_net_id,
	.size = sizeof(struct ovs_net),
};

static int __init dp_init(void)
{
	int err;

	BUILD_BUG_ON(sizeof(struct ovs_skb_cb) > FIELD_SIZEOF(struct sk_buff, cb));

	pr_info("Open vSwitch switching datapath\n");

<<<<<<< HEAD
<<<<<<< HEAD
	err = ovs_flow_init();
	if (err)
		goto error;

=======
=======
>>>>>>> v3.18
	err = action_fifos_init();
	if (err)
		goto error;

	err = ovs_internal_dev_rtnl_link_register();
	if (err)
		goto error_action_fifos_exit;

	err = ovs_flow_init();
	if (err)
		goto error_unreg_rtnl_link;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	err = ovs_vport_init();
	if (err)
		goto error_flow_exit;

	err = register_pernet_device(&ovs_net_ops);
	if (err)
		goto error_vport_exit;

	err = register_netdevice_notifier(&ovs_dp_device_notifier);
	if (err)
		goto error_netns_exit;

	err = dp_register_genl();
	if (err < 0)
		goto error_unreg_notifier;

<<<<<<< HEAD
<<<<<<< HEAD
	schedule_delayed_work(&rehash_flow_wq, REHASH_FLOW_INTERVAL);

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;

error_unreg_notifier:
	unregister_netdevice_notifier(&ovs_dp_device_notifier);
error_netns_exit:
	unregister_pernet_device(&ovs_net_ops);
error_vport_exit:
	ovs_vport_exit();
error_flow_exit:
	ovs_flow_exit();
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
error_unreg_rtnl_link:
	ovs_internal_dev_rtnl_link_unregister();
error_action_fifos_exit:
	action_fifos_exit();
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
error:
	return err;
}

static void dp_cleanup(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	cancel_delayed_work_sync(&rehash_flow_wq);
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	dp_unregister_genl(ARRAY_SIZE(dp_genl_families));
	unregister_netdevice_notifier(&ovs_dp_device_notifier);
	unregister_pernet_device(&ovs_net_ops);
	rcu_barrier();
	ovs_vport_exit();
	ovs_flow_exit();
<<<<<<< HEAD
<<<<<<< HEAD
=======
	ovs_internal_dev_rtnl_link_unregister();
	action_fifos_exit();
>>>>>>> v3.18
=======
	ovs_internal_dev_rtnl_link_unregister();
	action_fifos_exit();
>>>>>>> v3.18
}

module_init(dp_init);
module_exit(dp_cleanup);

MODULE_DESCRIPTION("Open vSwitch switching datapath");
MODULE_LICENSE("GPL");
