<<<<<<< HEAD
/* Copyright (C) 2007-2013 B.A.T.M.A.N. contributors:
=======
/* Copyright (C) 2007-2014 B.A.T.M.A.N. contributors:
>>>>>>> v3.18
 *
 * Marek Lindner, Simon Wunderlich
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
<<<<<<< HEAD
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
=======
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
>>>>>>> v3.18
 */

#include "main.h"
#include "distributed-arp-table.h"
#include "send.h"
#include "routing.h"
#include "translation-table.h"
#include "soft-interface.h"
#include "hard-interface.h"
<<<<<<< HEAD
#include "vis.h"
#include "gateway_common.h"
#include "originator.h"
#include "network-coding.h"

#include <linux/if_ether.h>
=======
#include "gateway_common.h"
#include "gateway_client.h"
#include "originator.h"
#include "network-coding.h"
#include "fragmentation.h"
#include "multicast.h"
>>>>>>> v3.18

static void batadv_send_outstanding_bcast_packet(struct work_struct *work);

/* send out an already prepared packet to the given address via the
 * specified batman interface
 */
int batadv_send_skb_packet(struct sk_buff *skb,
			   struct batadv_hard_iface *hard_iface,
			   const uint8_t *dst_addr)
{
	struct batadv_priv *bat_priv = netdev_priv(hard_iface->soft_iface);
	struct ethhdr *ethhdr;

	if (hard_iface->if_status != BATADV_IF_ACTIVE)
		goto send_skb_err;

	if (unlikely(!hard_iface->net_dev))
		goto send_skb_err;

	if (!(hard_iface->net_dev->flags & IFF_UP)) {
		pr_warn("Interface %s is not up - can't send packet via that interface!\n",
			hard_iface->net_dev->name);
		goto send_skb_err;
	}

	/* push to the ethernet header. */
	if (batadv_skb_head_push(skb, ETH_HLEN) < 0)
		goto send_skb_err;

	skb_reset_mac_header(skb);

<<<<<<< HEAD
	ethhdr = (struct ethhdr *)skb_mac_header(skb);
	memcpy(ethhdr->h_source, hard_iface->net_dev->dev_addr, ETH_ALEN);
	memcpy(ethhdr->h_dest, dst_addr, ETH_ALEN);
	ethhdr->h_proto = __constant_htons(ETH_P_BATMAN);

	skb_set_network_header(skb, ETH_HLEN);
	skb->priority = TC_PRIO_CONTROL;
	skb->protocol = __constant_htons(ETH_P_BATMAN);
=======
	ethhdr = eth_hdr(skb);
	ether_addr_copy(ethhdr->h_source, hard_iface->net_dev->dev_addr);
	ether_addr_copy(ethhdr->h_dest, dst_addr);
	ethhdr->h_proto = htons(ETH_P_BATMAN);

	skb_set_network_header(skb, ETH_HLEN);
	skb->protocol = htons(ETH_P_BATMAN);
>>>>>>> v3.18

	skb->dev = hard_iface->net_dev;

	/* Save a clone of the skb to use when decoding coded packets */
	batadv_nc_skb_store_for_decoding(bat_priv, skb);

	/* dev_queue_xmit() returns a negative result on error.	 However on
	 * congestion and traffic shaping, it drops and returns NET_XMIT_DROP
	 * (which is > 0). This will not be treated as an error.
	 */
	return dev_queue_xmit(skb);
send_skb_err:
	kfree_skb(skb);
	return NET_XMIT_DROP;
}

/**
 * batadv_send_skb_to_orig - Lookup next-hop and transmit skb.
 * @skb: Packet to be transmitted.
 * @orig_node: Final destination of the packet.
 * @recv_if: Interface used when receiving the packet (can be NULL).
 *
 * Looks up the best next-hop towards the passed originator and passes the
 * skb on for preparation of MAC header. If the packet originated from this
 * host, NULL can be passed as recv_if and no interface alternating is
 * attempted.
 *
<<<<<<< HEAD
 * Returns TRUE on success; FALSE otherwise.
 */
bool batadv_send_skb_to_orig(struct sk_buff *skb,
			     struct batadv_orig_node *orig_node,
			     struct batadv_hard_iface *recv_if)
{
	struct batadv_priv *bat_priv = orig_node->bat_priv;
	struct batadv_neigh_node *neigh_node;
=======
 * Returns NET_XMIT_SUCCESS on success, NET_XMIT_DROP on failure, or
 * NET_XMIT_POLICED if the skb is buffered for later transmit.
 */
int batadv_send_skb_to_orig(struct sk_buff *skb,
			    struct batadv_orig_node *orig_node,
			    struct batadv_hard_iface *recv_if)
{
	struct batadv_priv *bat_priv = orig_node->bat_priv;
	struct batadv_neigh_node *neigh_node;
	int ret = NET_XMIT_DROP;
>>>>>>> v3.18

	/* batadv_find_router() increases neigh_nodes refcount if found. */
	neigh_node = batadv_find_router(bat_priv, orig_node, recv_if);
	if (!neigh_node)
<<<<<<< HEAD
		return false;

	/* route it */
	batadv_send_skb_packet(skb, neigh_node->if_incoming, neigh_node->addr);

	batadv_neigh_node_free_ref(neigh_node);
=======
		goto out;

	/* Check if the skb is too large to send in one piece and fragment
	 * it if needed.
	 */
	if (atomic_read(&bat_priv->fragmentation) &&
	    skb->len > neigh_node->if_incoming->net_dev->mtu) {
		/* Fragment and send packet. */
		if (batadv_frag_send_packet(skb, orig_node, neigh_node))
			ret = NET_XMIT_SUCCESS;

		goto out;
	}

	/* try to network code the packet, if it is received on an interface
	 * (i.e. being forwarded). If the packet originates from this node or if
	 * network coding fails, then send the packet as usual.
	 */
	if (recv_if && batadv_nc_skb_forward(skb, neigh_node)) {
		ret = NET_XMIT_POLICED;
	} else {
		batadv_send_skb_packet(skb, neigh_node->if_incoming,
				       neigh_node->addr);
		ret = NET_XMIT_SUCCESS;
	}

out:
	if (neigh_node)
		batadv_neigh_node_free_ref(neigh_node);

	return ret;
}

/**
 * batadv_send_skb_push_fill_unicast - extend the buffer and initialize the
 *  common fields for unicast packets
 * @skb: the skb carrying the unicast header to initialize
 * @hdr_size: amount of bytes to push at the beginning of the skb
 * @orig_node: the destination node
 *
 * Returns false if the buffer extension was not possible or true otherwise.
 */
static bool
batadv_send_skb_push_fill_unicast(struct sk_buff *skb, int hdr_size,
				  struct batadv_orig_node *orig_node)
{
	struct batadv_unicast_packet *unicast_packet;
	uint8_t ttvn = (uint8_t)atomic_read(&orig_node->last_ttvn);

	if (batadv_skb_head_push(skb, hdr_size) < 0)
		return false;

	unicast_packet = (struct batadv_unicast_packet *)skb->data;
	unicast_packet->version = BATADV_COMPAT_VERSION;
	/* batman packet type: unicast */
	unicast_packet->packet_type = BATADV_UNICAST;
	/* set unicast ttl */
	unicast_packet->ttl = BATADV_TTL;
	/* copy the destination for faster routing */
	ether_addr_copy(unicast_packet->dest, orig_node->orig);
	/* set the destination tt version number */
	unicast_packet->ttvn = ttvn;
>>>>>>> v3.18

	return true;
}

<<<<<<< HEAD
=======
/**
 * batadv_send_skb_prepare_unicast - encapsulate an skb with a unicast header
 * @skb: the skb containing the payload to encapsulate
 * @orig_node: the destination node
 *
 * Returns false if the payload could not be encapsulated or true otherwise.
 */
static bool batadv_send_skb_prepare_unicast(struct sk_buff *skb,
					    struct batadv_orig_node *orig_node)
{
	size_t uni_size = sizeof(struct batadv_unicast_packet);

	return batadv_send_skb_push_fill_unicast(skb, uni_size, orig_node);
}

/**
 * batadv_send_skb_prepare_unicast_4addr - encapsulate an skb with a
 *  unicast 4addr header
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: the skb containing the payload to encapsulate
 * @orig_node: the destination node
 * @packet_subtype: the unicast 4addr packet subtype to use
 *
 * Returns false if the payload could not be encapsulated or true otherwise.
 */
bool batadv_send_skb_prepare_unicast_4addr(struct batadv_priv *bat_priv,
					   struct sk_buff *skb,
					   struct batadv_orig_node *orig,
					   int packet_subtype)
{
	struct batadv_hard_iface *primary_if;
	struct batadv_unicast_4addr_packet *uc_4addr_packet;
	bool ret = false;

	primary_if = batadv_primary_if_get_selected(bat_priv);
	if (!primary_if)
		goto out;

	/* Pull the header space and fill the unicast_packet substructure.
	 * We can do that because the first member of the uc_4addr_packet
	 * is of type struct unicast_packet
	 */
	if (!batadv_send_skb_push_fill_unicast(skb, sizeof(*uc_4addr_packet),
					       orig))
		goto out;

	uc_4addr_packet = (struct batadv_unicast_4addr_packet *)skb->data;
	uc_4addr_packet->u.packet_type = BATADV_UNICAST_4ADDR;
	ether_addr_copy(uc_4addr_packet->src, primary_if->net_dev->dev_addr);
	uc_4addr_packet->subtype = packet_subtype;
	uc_4addr_packet->reserved = 0;

	ret = true;
out:
	if (primary_if)
		batadv_hardif_free_ref(primary_if);
	return ret;
}

/**
 * batadv_send_skb_unicast - encapsulate and send an skb via unicast
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: payload to send
 * @packet_type: the batman unicast packet type to use
 * @packet_subtype: the unicast 4addr packet subtype (only relevant for unicast
 *  4addr packets)
 * @orig_node: the originator to send the packet to
 * @vid: the vid to be used to search the translation table
 *
 * Wrap the given skb into a batman-adv unicast or unicast-4addr header
 * depending on whether BATADV_UNICAST or BATADV_UNICAST_4ADDR was supplied
 * as packet_type. Then send this frame to the given orig_node and release a
 * reference to this orig_node.
 *
 * Returns NET_XMIT_DROP in case of error or NET_XMIT_SUCCESS otherwise.
 */
int batadv_send_skb_unicast(struct batadv_priv *bat_priv,
			    struct sk_buff *skb, int packet_type,
			    int packet_subtype,
			    struct batadv_orig_node *orig_node,
			    unsigned short vid)
{
	struct ethhdr *ethhdr;
	struct batadv_unicast_packet *unicast_packet;
	int ret = NET_XMIT_DROP;

	if (!orig_node)
		goto out;

	switch (packet_type) {
	case BATADV_UNICAST:
		if (!batadv_send_skb_prepare_unicast(skb, orig_node))
			goto out;
		break;
	case BATADV_UNICAST_4ADDR:
		if (!batadv_send_skb_prepare_unicast_4addr(bat_priv, skb,
							   orig_node,
							   packet_subtype))
			goto out;
		break;
	default:
		/* this function supports UNICAST and UNICAST_4ADDR only. It
		 * should never be invoked with any other packet type
		 */
		goto out;
	}

	/* skb->data might have been reallocated by
	 * batadv_send_skb_prepare_unicast{,_4addr}()
	 */
	ethhdr = eth_hdr(skb);
	unicast_packet = (struct batadv_unicast_packet *)skb->data;

	/* inform the destination node that we are still missing a correct route
	 * for this client. The destination will receive this packet and will
	 * try to reroute it because the ttvn contained in the header is less
	 * than the current one
	 */
	if (batadv_tt_global_client_is_roaming(bat_priv, ethhdr->h_dest, vid))
		unicast_packet->ttvn = unicast_packet->ttvn - 1;

	if (batadv_send_skb_to_orig(skb, orig_node, NULL) != NET_XMIT_DROP)
		ret = NET_XMIT_SUCCESS;

out:
	if (orig_node)
		batadv_orig_node_free_ref(orig_node);
	if (ret == NET_XMIT_DROP)
		kfree_skb(skb);
	return ret;
}

/**
 * batadv_send_skb_via_tt_generic - send an skb via TT lookup
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: payload to send
 * @packet_type: the batman unicast packet type to use
 * @packet_subtype: the unicast 4addr packet subtype (only relevant for unicast
 *  4addr packets)
 * @dst_hint: can be used to override the destination contained in the skb
 * @vid: the vid to be used to search the translation table
 *
 * Look up the recipient node for the destination address in the ethernet
 * header via the translation table. Wrap the given skb into a batman-adv
 * unicast or unicast-4addr header depending on whether BATADV_UNICAST or
 * BATADV_UNICAST_4ADDR was supplied as packet_type. Then send this frame
 * to the according destination node.
 *
 * Returns NET_XMIT_DROP in case of error or NET_XMIT_SUCCESS otherwise.
 */
int batadv_send_skb_via_tt_generic(struct batadv_priv *bat_priv,
				   struct sk_buff *skb, int packet_type,
				   int packet_subtype, uint8_t *dst_hint,
				   unsigned short vid)
{
	struct ethhdr *ethhdr = (struct ethhdr *)skb->data;
	struct batadv_orig_node *orig_node;
	uint8_t *src, *dst;

	src = ethhdr->h_source;
	dst = ethhdr->h_dest;

	/* if we got an hint! let's send the packet to this client (if any) */
	if (dst_hint) {
		src = NULL;
		dst = dst_hint;
	}
	orig_node = batadv_transtable_search(bat_priv, src, dst, vid);

	return batadv_send_skb_unicast(bat_priv, skb, packet_type,
				       packet_subtype, orig_node, vid);
}

/**
 * batadv_send_skb_via_gw - send an skb via gateway lookup
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: payload to send
 * @vid: the vid to be used to search the translation table
 *
 * Look up the currently selected gateway. Wrap the given skb into a batman-adv
 * unicast header and send this frame to this gateway node.
 *
 * Returns NET_XMIT_DROP in case of error or NET_XMIT_SUCCESS otherwise.
 */
int batadv_send_skb_via_gw(struct batadv_priv *bat_priv, struct sk_buff *skb,
			   unsigned short vid)
{
	struct batadv_orig_node *orig_node;

	orig_node = batadv_gw_get_selected_orig(bat_priv);
	return batadv_send_skb_unicast(bat_priv, skb, BATADV_UNICAST, 0,
				       orig_node, vid);
}

>>>>>>> v3.18
void batadv_schedule_bat_ogm(struct batadv_hard_iface *hard_iface)
{
	struct batadv_priv *bat_priv = netdev_priv(hard_iface->soft_iface);

	if ((hard_iface->if_status == BATADV_IF_NOT_IN_USE) ||
	    (hard_iface->if_status == BATADV_IF_TO_BE_REMOVED))
		return;

	/* the interface gets activated here to avoid race conditions between
	 * the moment of activating the interface in
	 * hardif_activate_interface() where the originator mac is set and
	 * outdated packets (especially uninitialized mac addresses) in the
	 * packet queue
	 */
	if (hard_iface->if_status == BATADV_IF_TO_BE_ACTIVATED)
		hard_iface->if_status = BATADV_IF_ACTIVE;

	bat_priv->bat_algo_ops->bat_ogm_schedule(hard_iface);
}

static void batadv_forw_packet_free(struct batadv_forw_packet *forw_packet)
{
	if (forw_packet->skb)
		kfree_skb(forw_packet->skb);
	if (forw_packet->if_incoming)
		batadv_hardif_free_ref(forw_packet->if_incoming);
<<<<<<< HEAD
=======
	if (forw_packet->if_outgoing)
		batadv_hardif_free_ref(forw_packet->if_outgoing);
>>>>>>> v3.18
	kfree(forw_packet);
}

static void
_batadv_add_bcast_packet_to_list(struct batadv_priv *bat_priv,
				 struct batadv_forw_packet *forw_packet,
				 unsigned long send_time)
{
<<<<<<< HEAD
	INIT_HLIST_NODE(&forw_packet->list);

=======
>>>>>>> v3.18
	/* add new packet to packet list */
	spin_lock_bh(&bat_priv->forw_bcast_list_lock);
	hlist_add_head(&forw_packet->list, &bat_priv->forw_bcast_list);
	spin_unlock_bh(&bat_priv->forw_bcast_list_lock);

	/* start timer for this packet */
	queue_delayed_work(batadv_event_workqueue, &forw_packet->delayed_work,
			   send_time);
}

/* add a broadcast packet to the queue and setup timers. broadcast packets
 * are sent multiple times to increase probability for being received.
 *
 * This function returns NETDEV_TX_OK on success and NETDEV_TX_BUSY on
 * errors.
 *
 * The skb is not consumed, so the caller should make sure that the
 * skb is freed.
 */
int batadv_add_bcast_packet_to_list(struct batadv_priv *bat_priv,
				    const struct sk_buff *skb,
				    unsigned long delay)
{
	struct batadv_hard_iface *primary_if = NULL;
	struct batadv_forw_packet *forw_packet;
	struct batadv_bcast_packet *bcast_packet;
	struct sk_buff *newskb;

	if (!batadv_atomic_dec_not_zero(&bat_priv->bcast_queue_left)) {
		batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
			   "bcast packet queue full\n");
		goto out;
	}

	primary_if = batadv_primary_if_get_selected(bat_priv);
	if (!primary_if)
		goto out_and_inc;

	forw_packet = kmalloc(sizeof(*forw_packet), GFP_ATOMIC);

	if (!forw_packet)
		goto out_and_inc;

	newskb = skb_copy(skb, GFP_ATOMIC);
	if (!newskb)
		goto packet_free;

	/* as we have a copy now, it is safe to decrease the TTL */
	bcast_packet = (struct batadv_bcast_packet *)newskb->data;
<<<<<<< HEAD
	bcast_packet->header.ttl--;
=======
	bcast_packet->ttl--;
>>>>>>> v3.18

	skb_reset_mac_header(newskb);

	forw_packet->skb = newskb;
	forw_packet->if_incoming = primary_if;
<<<<<<< HEAD
=======
	forw_packet->if_outgoing = NULL;
>>>>>>> v3.18

	/* how often did we send the bcast packet ? */
	forw_packet->num_packets = 0;

	INIT_DELAYED_WORK(&forw_packet->delayed_work,
			  batadv_send_outstanding_bcast_packet);

	_batadv_add_bcast_packet_to_list(bat_priv, forw_packet, delay);
	return NETDEV_TX_OK;

packet_free:
	kfree(forw_packet);
out_and_inc:
	atomic_inc(&bat_priv->bcast_queue_left);
out:
	if (primary_if)
		batadv_hardif_free_ref(primary_if);
	return NETDEV_TX_BUSY;
}

static void batadv_send_outstanding_bcast_packet(struct work_struct *work)
{
	struct batadv_hard_iface *hard_iface;
	struct delayed_work *delayed_work;
	struct batadv_forw_packet *forw_packet;
	struct sk_buff *skb1;
	struct net_device *soft_iface;
	struct batadv_priv *bat_priv;

	delayed_work = container_of(work, struct delayed_work, work);
	forw_packet = container_of(delayed_work, struct batadv_forw_packet,
				   delayed_work);
	soft_iface = forw_packet->if_incoming->soft_iface;
	bat_priv = netdev_priv(soft_iface);

	spin_lock_bh(&bat_priv->forw_bcast_list_lock);
	hlist_del(&forw_packet->list);
	spin_unlock_bh(&bat_priv->forw_bcast_list_lock);

	if (atomic_read(&bat_priv->mesh_state) == BATADV_MESH_DEACTIVATING)
		goto out;

	if (batadv_dat_drop_broadcast_packet(bat_priv, forw_packet))
		goto out;

	/* rebroadcast packet */
	rcu_read_lock();
	list_for_each_entry_rcu(hard_iface, &batadv_hardif_list, list) {
		if (hard_iface->soft_iface != soft_iface)
			continue;

<<<<<<< HEAD
=======
		if (forw_packet->num_packets >= hard_iface->num_bcasts)
			continue;

>>>>>>> v3.18
		/* send a copy of the saved skb */
		skb1 = skb_clone(forw_packet->skb, GFP_ATOMIC);
		if (skb1)
			batadv_send_skb_packet(skb1, hard_iface,
					       batadv_broadcast_addr);
	}
	rcu_read_unlock();

	forw_packet->num_packets++;

	/* if we still have some more bcasts to send */
<<<<<<< HEAD
	if (forw_packet->num_packets < 3) {
=======
	if (forw_packet->num_packets < BATADV_NUM_BCASTS_MAX) {
>>>>>>> v3.18
		_batadv_add_bcast_packet_to_list(bat_priv, forw_packet,
						 msecs_to_jiffies(5));
		return;
	}

out:
	batadv_forw_packet_free(forw_packet);
	atomic_inc(&bat_priv->bcast_queue_left);
}

void batadv_send_outstanding_bat_ogm_packet(struct work_struct *work)
{
	struct delayed_work *delayed_work;
	struct batadv_forw_packet *forw_packet;
	struct batadv_priv *bat_priv;

	delayed_work = container_of(work, struct delayed_work, work);
	forw_packet = container_of(delayed_work, struct batadv_forw_packet,
				   delayed_work);
	bat_priv = netdev_priv(forw_packet->if_incoming->soft_iface);
	spin_lock_bh(&bat_priv->forw_bat_list_lock);
	hlist_del(&forw_packet->list);
	spin_unlock_bh(&bat_priv->forw_bat_list_lock);

	if (atomic_read(&bat_priv->mesh_state) == BATADV_MESH_DEACTIVATING)
		goto out;

	bat_priv->bat_algo_ops->bat_ogm_emit(forw_packet);

<<<<<<< HEAD
	/* we have to have at least one packet in the queue
	 * to determine the queues wake up time unless we are
	 * shutting down
	 */
	if (forw_packet->own)
=======
	/* we have to have at least one packet in the queue to determine the
	 * queues wake up time unless we are shutting down.
	 *
	 * only re-schedule if this is the "original" copy, e.g. the OGM of the
	 * primary interface should only be rescheduled once per period, but
	 * this function will be called for the forw_packet instances of the
	 * other secondary interfaces as well.
	 */
	if (forw_packet->own &&
	    forw_packet->if_incoming == forw_packet->if_outgoing)
>>>>>>> v3.18
		batadv_schedule_bat_ogm(forw_packet->if_incoming);

out:
	/* don't count own packet */
	if (!forw_packet->own)
		atomic_inc(&bat_priv->batman_queue_left);

	batadv_forw_packet_free(forw_packet);
}

void
batadv_purge_outstanding_packets(struct batadv_priv *bat_priv,
				 const struct batadv_hard_iface *hard_iface)
{
	struct batadv_forw_packet *forw_packet;
	struct hlist_node *safe_tmp_node;
	bool pending;

	if (hard_iface)
		batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
			   "purge_outstanding_packets(): %s\n",
			   hard_iface->net_dev->name);
	else
		batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
			   "purge_outstanding_packets()\n");

	/* free bcast list */
	spin_lock_bh(&bat_priv->forw_bcast_list_lock);
	hlist_for_each_entry_safe(forw_packet, safe_tmp_node,
				  &bat_priv->forw_bcast_list, list) {
		/* if purge_outstanding_packets() was called with an argument
		 * we delete only packets belonging to the given interface
		 */
		if ((hard_iface) &&
		    (forw_packet->if_incoming != hard_iface))
			continue;

		spin_unlock_bh(&bat_priv->forw_bcast_list_lock);

		/* batadv_send_outstanding_bcast_packet() will lock the list to
		 * delete the item from the list
		 */
		pending = cancel_delayed_work_sync(&forw_packet->delayed_work);
		spin_lock_bh(&bat_priv->forw_bcast_list_lock);

		if (pending) {
			hlist_del(&forw_packet->list);
<<<<<<< HEAD
			if (!forw_packet->own)
				atomic_inc(&bat_priv->batman_queue_left);

=======
>>>>>>> v3.18
			batadv_forw_packet_free(forw_packet);
		}
	}
	spin_unlock_bh(&bat_priv->forw_bcast_list_lock);

	/* free batman packet list */
	spin_lock_bh(&bat_priv->forw_bat_list_lock);
	hlist_for_each_entry_safe(forw_packet, safe_tmp_node,
				  &bat_priv->forw_bat_list, list) {
		/* if purge_outstanding_packets() was called with an argument
		 * we delete only packets belonging to the given interface
		 */
		if ((hard_iface) &&
<<<<<<< HEAD
		    (forw_packet->if_incoming != hard_iface))
=======
		    (forw_packet->if_incoming != hard_iface) &&
		    (forw_packet->if_outgoing != hard_iface))
>>>>>>> v3.18
			continue;

		spin_unlock_bh(&bat_priv->forw_bat_list_lock);

		/* send_outstanding_bat_packet() will lock the list to
		 * delete the item from the list
		 */
		pending = cancel_delayed_work_sync(&forw_packet->delayed_work);
		spin_lock_bh(&bat_priv->forw_bat_list_lock);

		if (pending) {
			hlist_del(&forw_packet->list);
<<<<<<< HEAD
			if (!forw_packet->own)
				atomic_inc(&bat_priv->bcast_queue_left);

=======
>>>>>>> v3.18
			batadv_forw_packet_free(forw_packet);
		}
	}
	spin_unlock_bh(&bat_priv->forw_bat_list_lock);
}
