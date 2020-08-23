/*
 *	IPV6 GSO/GRO offload support
 *	Linux INET6 implementation
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/socket.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/printk.h>

#include <net/protocol.h>
#include <net/ipv6.h>

#include "ip6_offload.h"

static int ipv6_gso_pull_exthdrs(struct sk_buff *skb, int proto)
{
	const struct net_offload *ops = NULL;

	for (;;) {
		struct ipv6_opt_hdr *opth;
		int len;

		if (proto != NEXTHDR_HOP) {
			ops = rcu_dereference(inet6_offloads[proto]);

			if (unlikely(!ops))
				break;

			if (!(ops->flags & INET6_PROTO_GSO_EXTHDR))
				break;
		}

		if (unlikely(!pskb_may_pull(skb, 8)))
			break;

		opth = (void *)skb->data;
		len = ipv6_optlen(opth);

		if (unlikely(!pskb_may_pull(skb, len)))
			break;

<<<<<<< HEAD
=======
		opth = (void *)skb->data;
>>>>>>> v3.18
		proto = opth->nexthdr;
		__skb_pull(skb, len);
	}

	return proto;
}

<<<<<<< HEAD
static int ipv6_gso_send_check(struct sk_buff *skb)
{
	const struct ipv6hdr *ipv6h;
	const struct net_offload *ops;
	int err = -EINVAL;

	if (unlikely(!pskb_may_pull(skb, sizeof(*ipv6h))))
		goto out;

	ipv6h = ipv6_hdr(skb);
	__skb_pull(skb, sizeof(*ipv6h));
	err = -EPROTONOSUPPORT;

	rcu_read_lock();
	ops = rcu_dereference(inet6_offloads[
		ipv6_gso_pull_exthdrs(skb, ipv6h->nexthdr)]);

	if (likely(ops && ops->callbacks.gso_send_check)) {
		skb_reset_transport_header(skb);
		err = ops->callbacks.gso_send_check(skb);
	}
	rcu_read_unlock();

out:
	return err;
}

=======
>>>>>>> v3.18
static struct sk_buff *ipv6_gso_segment(struct sk_buff *skb,
	netdev_features_t features)
{
	struct sk_buff *segs = ERR_PTR(-EINVAL);
	struct ipv6hdr *ipv6h;
	const struct net_offload *ops;
	int proto;
	struct frag_hdr *fptr;
	unsigned int unfrag_ip6hlen;
	u8 *prevhdr;
	int offset = 0;
<<<<<<< HEAD

	if (unlikely(skb_shinfo(skb)->gso_type &
		     ~(SKB_GSO_UDP |
		       SKB_GSO_DODGY |
		       SKB_GSO_TCP_ECN |
		       SKB_GSO_GRE |
		       SKB_GSO_UDP_TUNNEL |
=======
	bool encap, udpfrag;
	int nhoff;

	if (unlikely(skb_shinfo(skb)->gso_type &
		     ~(SKB_GSO_TCPV4 |
		       SKB_GSO_UDP |
		       SKB_GSO_DODGY |
		       SKB_GSO_TCP_ECN |
		       SKB_GSO_GRE |
		       SKB_GSO_GRE_CSUM |
		       SKB_GSO_IPIP |
		       SKB_GSO_SIT |
		       SKB_GSO_UDP_TUNNEL |
		       SKB_GSO_UDP_TUNNEL_CSUM |
		       SKB_GSO_MPLS |
>>>>>>> v3.18
		       SKB_GSO_TCPV6 |
		       0)))
		goto out;

<<<<<<< HEAD
	if (unlikely(!pskb_may_pull(skb, sizeof(*ipv6h))))
		goto out;

=======
	skb_reset_network_header(skb);
	nhoff = skb_network_header(skb) - skb_mac_header(skb);
	if (unlikely(!pskb_may_pull(skb, sizeof(*ipv6h))))
		goto out;

	encap = SKB_GSO_CB(skb)->encap_level > 0;
	if (encap)
		features &= skb->dev->hw_enc_features;
	SKB_GSO_CB(skb)->encap_level += sizeof(*ipv6h);

>>>>>>> v3.18
	ipv6h = ipv6_hdr(skb);
	__skb_pull(skb, sizeof(*ipv6h));
	segs = ERR_PTR(-EPROTONOSUPPORT);

	proto = ipv6_gso_pull_exthdrs(skb, ipv6h->nexthdr);
<<<<<<< HEAD
	rcu_read_lock();
=======

	if (skb->encapsulation &&
	    skb_shinfo(skb)->gso_type & (SKB_GSO_SIT|SKB_GSO_IPIP))
		udpfrag = proto == IPPROTO_UDP && encap;
	else
		udpfrag = proto == IPPROTO_UDP && !skb->encapsulation;

>>>>>>> v3.18
	ops = rcu_dereference(inet6_offloads[proto]);
	if (likely(ops && ops->callbacks.gso_segment)) {
		skb_reset_transport_header(skb);
		segs = ops->callbacks.gso_segment(skb, features);
	}
<<<<<<< HEAD
	rcu_read_unlock();
=======
>>>>>>> v3.18

	if (IS_ERR(segs))
		goto out;

	for (skb = segs; skb; skb = skb->next) {
<<<<<<< HEAD
		ipv6h = ipv6_hdr(skb);
		ipv6h->payload_len = htons(skb->len - skb->mac_len -
					   sizeof(*ipv6h));
		if (proto == IPPROTO_UDP) {
			unfrag_ip6hlen = ip6_find_1stfragopt(skb, &prevhdr);
                        if (unfrag_ip6hlen < 0)
                                return ERR_PTR(unfrag_ip6hlen);
			fptr = (struct frag_hdr *)(skb_network_header(skb) +
				unfrag_ip6hlen);
=======
		ipv6h = (struct ipv6hdr *)(skb_mac_header(skb) + nhoff);
		ipv6h->payload_len = htons(skb->len - nhoff - sizeof(*ipv6h));
		skb->network_header = (u8 *)ipv6h - skb->head;

		if (udpfrag) {
			unfrag_ip6hlen = ip6_find_1stfragopt(skb, &prevhdr);
			fptr = (struct frag_hdr *)((u8 *)ipv6h + unfrag_ip6hlen);
>>>>>>> v3.18
			fptr->frag_off = htons(offset);
			if (skb->next != NULL)
				fptr->frag_off |= htons(IP6_MF);
			offset += (ntohs(ipv6h->payload_len) -
				   sizeof(struct frag_hdr));
		}
<<<<<<< HEAD
=======
		if (encap)
			skb_reset_inner_headers(skb);
>>>>>>> v3.18
	}

out:
	return segs;
}

<<<<<<< HEAD
=======
/* Return the total length of all the extension hdrs, following the same
 * logic in ipv6_gso_pull_exthdrs() when parsing ext-hdrs.
 */
static int ipv6_exthdrs_len(struct ipv6hdr *iph,
			    const struct net_offload **opps)
{
	struct ipv6_opt_hdr *opth = (void *)iph;
	int len = 0, proto, optlen = sizeof(*iph);

	proto = iph->nexthdr;
	for (;;) {
		if (proto != NEXTHDR_HOP) {
			*opps = rcu_dereference(inet6_offloads[proto]);
			if (unlikely(!(*opps)))
				break;
			if (!((*opps)->flags & INET6_PROTO_GSO_EXTHDR))
				break;
		}
		opth = (void *)opth + optlen;
		optlen = ipv6_optlen(opth);
		len += optlen;
		proto = opth->nexthdr;
	}
	return len;
}

>>>>>>> v3.18
static struct sk_buff **ipv6_gro_receive(struct sk_buff **head,
					 struct sk_buff *skb)
{
	const struct net_offload *ops;
	struct sk_buff **pp = NULL;
	struct sk_buff *p;
	struct ipv6hdr *iph;
	unsigned int nlen;
	unsigned int hlen;
	unsigned int off;
<<<<<<< HEAD
	int flush = 1;
	int proto;
	__wsum csum;
=======
	u16 flush = 1;
	int proto;
>>>>>>> v3.18

	off = skb_gro_offset(skb);
	hlen = off + sizeof(*iph);
	iph = skb_gro_header_fast(skb, off);
	if (skb_gro_header_hard(skb, hlen)) {
		iph = skb_gro_header_slow(skb, hlen, off);
		if (unlikely(!iph))
			goto out;
	}

<<<<<<< HEAD
=======
	skb_set_network_header(skb, off);
>>>>>>> v3.18
	skb_gro_pull(skb, sizeof(*iph));
	skb_set_transport_header(skb, skb_gro_offset(skb));

	flush += ntohs(iph->payload_len) != skb_gro_len(skb);

	rcu_read_lock();
	proto = iph->nexthdr;
	ops = rcu_dereference(inet6_offloads[proto]);
	if (!ops || !ops->callbacks.gro_receive) {
		__pskb_pull(skb, skb_gro_offset(skb));
<<<<<<< HEAD
		skb_gro_frag0_invalidate(skb);
=======
>>>>>>> v3.18
		proto = ipv6_gso_pull_exthdrs(skb, proto);
		skb_gro_pull(skb, -skb_transport_offset(skb));
		skb_reset_transport_header(skb);
		__skb_push(skb, skb_gro_offset(skb));

		ops = rcu_dereference(inet6_offloads[proto]);
		if (!ops || !ops->callbacks.gro_receive)
			goto out_unlock;

		iph = ipv6_hdr(skb);
	}

	NAPI_GRO_CB(skb)->proto = proto;

	flush--;
	nlen = skb_network_header_len(skb);

	for (p = *head; p; p = p->next) {
		const struct ipv6hdr *iph2;
		__be32 first_word; /* <Version:4><Traffic_Class:8><Flow_Label:20> */

		if (!NAPI_GRO_CB(p)->same_flow)
			continue;

<<<<<<< HEAD
		iph2 = ipv6_hdr(p);
		first_word = *(__be32 *)iph ^ *(__be32 *)iph2 ;

		/* All fields must match except length and Traffic Class. */
		if (nlen != skb_network_header_len(p) ||
		    (first_word & htonl(0xF00FFFFF)) ||
=======
		iph2 = (struct ipv6hdr *)(p->data + off);
		first_word = *(__be32 *)iph ^ *(__be32 *)iph2;

		/* All fields must match except length and Traffic Class.
		 * XXX skbs on the gro_list have all been parsed and pulled
		 * already so we don't need to compare nlen
		 * (nlen != (sizeof(*iph2) + ipv6_exthdrs_len(iph2, &ops)))
		 * memcmp() alone below is suffcient, right?
		 */
		 if ((first_word & htonl(0xF00FFFFF)) ||
>>>>>>> v3.18
		    memcmp(&iph->nexthdr, &iph2->nexthdr,
			   nlen - offsetof(struct ipv6hdr, nexthdr))) {
			NAPI_GRO_CB(p)->same_flow = 0;
			continue;
		}
		/* flush if Traffic Class fields are different */
		NAPI_GRO_CB(p)->flush |= !!(first_word & htonl(0x0FF00000));
		NAPI_GRO_CB(p)->flush |= flush;
<<<<<<< HEAD
=======

		/* Clear flush_id, there's really no concept of ID in IPv6. */
		NAPI_GRO_CB(p)->flush_id = 0;
>>>>>>> v3.18
	}

	NAPI_GRO_CB(skb)->flush |= flush;

<<<<<<< HEAD
	csum = skb->csum;
	skb_postpull_rcsum(skb, iph, skb_network_header_len(skb));

	pp = ops->callbacks.gro_receive(head, skb);

	skb->csum = csum;

=======
	skb_gro_postpull_rcsum(skb, iph, nlen);

	pp = ops->callbacks.gro_receive(head, skb);

>>>>>>> v3.18
out_unlock:
	rcu_read_unlock();

out:
	NAPI_GRO_CB(skb)->flush |= flush;

	return pp;
}

<<<<<<< HEAD
static int ipv6_gro_complete(struct sk_buff *skb)
{
	const struct net_offload *ops;
	struct ipv6hdr *iph = ipv6_hdr(skb);
	int err = -ENOSYS;

	iph->payload_len = htons(skb->len - skb_network_offset(skb) -
				 sizeof(*iph));

	rcu_read_lock();
	ops = rcu_dereference(inet6_offloads[NAPI_GRO_CB(skb)->proto]);
	if (WARN_ON(!ops || !ops->callbacks.gro_complete))
		goto out_unlock;

	err = ops->callbacks.gro_complete(skb);
=======
static int ipv6_gro_complete(struct sk_buff *skb, int nhoff)
{
	const struct net_offload *ops;
	struct ipv6hdr *iph = (struct ipv6hdr *)(skb->data + nhoff);
	int err = -ENOSYS;

	iph->payload_len = htons(skb->len - nhoff - sizeof(*iph));

	rcu_read_lock();

	nhoff += sizeof(*iph) + ipv6_exthdrs_len(iph, &ops);
	if (WARN_ON(!ops || !ops->callbacks.gro_complete))
		goto out_unlock;

	err = ops->callbacks.gro_complete(skb, nhoff);
>>>>>>> v3.18

out_unlock:
	rcu_read_unlock();

	return err;
}

static struct packet_offload ipv6_packet_offload __read_mostly = {
	.type = cpu_to_be16(ETH_P_IPV6),
	.callbacks = {
<<<<<<< HEAD
		.gso_send_check = ipv6_gso_send_check,
=======
>>>>>>> v3.18
		.gso_segment = ipv6_gso_segment,
		.gro_receive = ipv6_gro_receive,
		.gro_complete = ipv6_gro_complete,
	},
};

<<<<<<< HEAD
=======
static const struct net_offload sit_offload = {
	.callbacks = {
		.gso_segment	= ipv6_gso_segment,
		.gro_receive	= ipv6_gro_receive,
		.gro_complete	= ipv6_gro_complete,
	},
};

>>>>>>> v3.18
static int __init ipv6_offload_init(void)
{

	if (tcpv6_offload_init() < 0)
		pr_crit("%s: Cannot add TCP protocol offload\n", __func__);
	if (udp_offload_init() < 0)
		pr_crit("%s: Cannot add UDP protocol offload\n", __func__);
	if (ipv6_exthdrs_offload_init() < 0)
		pr_crit("%s: Cannot add EXTHDRS protocol offload\n", __func__);

	dev_add_offload(&ipv6_packet_offload);
<<<<<<< HEAD
=======

	inet_add_offload(&sit_offload, IPPROTO_IPV6);

>>>>>>> v3.18
	return 0;
}

fs_initcall(ipv6_offload_init);
