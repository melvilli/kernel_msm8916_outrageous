/*
 * net/tipc/node.c: TIPC node management routines
 *
<<<<<<< HEAD
 * Copyright (c) 2000-2006, 2012 Ericsson AB
 * Copyright (c) 2005-2006, 2010-2011, Wind River Systems
=======
 * Copyright (c) 2000-2006, 2012-2014, Ericsson AB
 * Copyright (c) 2005-2006, 2010-2014, Wind River Systems
>>>>>>> v3.18
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the names of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "core.h"
#include "config.h"
#include "node.h"
#include "name_distr.h"
<<<<<<< HEAD
=======
#include "socket.h"
>>>>>>> v3.18

#define NODE_HTABLE_SIZE 512

static void node_lost_contact(struct tipc_node *n_ptr);
static void node_established_contact(struct tipc_node *n_ptr);

<<<<<<< HEAD
static DEFINE_SPINLOCK(node_create_lock);

static struct hlist_head node_htable[NODE_HTABLE_SIZE];
LIST_HEAD(tipc_node_list);
static u32 tipc_num_nodes;

static atomic_t tipc_num_links = ATOMIC_INIT(0);
=======
static struct hlist_head node_htable[NODE_HTABLE_SIZE];
LIST_HEAD(tipc_node_list);
static u32 tipc_num_nodes;
static u32 tipc_num_links;
static DEFINE_SPINLOCK(node_list_lock);

struct tipc_sock_conn {
	u32 port;
	u32 peer_port;
	u32 peer_node;
	struct list_head list;
};
>>>>>>> v3.18

/*
 * A trivial power-of-two bitmask technique is used for speed, since this
 * operation is done for every incoming TIPC packet. The number of hash table
 * entries has been chosen so that no hash chain exceeds 8 nodes and will
 * usually be much smaller (typically only a single node).
 */
static unsigned int tipc_hashfn(u32 addr)
{
	return addr & (NODE_HTABLE_SIZE - 1);
}

/*
 * tipc_node_find - locate specified node object, if it exists
 */
struct tipc_node *tipc_node_find(u32 addr)
{
	struct tipc_node *node;

	if (unlikely(!in_own_cluster_exact(addr)))
		return NULL;

<<<<<<< HEAD
	hlist_for_each_entry(node, &node_htable[tipc_hashfn(addr)], hash) {
		if (node->addr == addr)
			return node;
	}
	return NULL;
}

/**
 * tipc_node_create - create neighboring node
 *
 * Currently, this routine is called by neighbor discovery code, which holds
 * net_lock for reading only.  We must take node_create_lock to ensure a node
 * isn't created twice if two different bearers discover the node at the same
 * time.  (It would be preferable to switch to holding net_lock in write mode,
 * but this is a non-trivial change.)
 */
=======
	rcu_read_lock();
	hlist_for_each_entry_rcu(node, &node_htable[tipc_hashfn(addr)], hash) {
		if (node->addr == addr) {
			rcu_read_unlock();
			return node;
		}
	}
	rcu_read_unlock();
	return NULL;
}

>>>>>>> v3.18
struct tipc_node *tipc_node_create(u32 addr)
{
	struct tipc_node *n_ptr, *temp_node;

<<<<<<< HEAD
	spin_lock_bh(&node_create_lock);

	n_ptr = tipc_node_find(addr);
	if (n_ptr) {
		spin_unlock_bh(&node_create_lock);
		return n_ptr;
	}

	n_ptr = kzalloc(sizeof(*n_ptr), GFP_ATOMIC);
	if (!n_ptr) {
		spin_unlock_bh(&node_create_lock);
=======
	spin_lock_bh(&node_list_lock);

	n_ptr = kzalloc(sizeof(*n_ptr), GFP_ATOMIC);
	if (!n_ptr) {
		spin_unlock_bh(&node_list_lock);
>>>>>>> v3.18
		pr_warn("Node creation failed, no memory\n");
		return NULL;
	}

	n_ptr->addr = addr;
	spin_lock_init(&n_ptr->lock);
	INIT_HLIST_NODE(&n_ptr->hash);
	INIT_LIST_HEAD(&n_ptr->list);
	INIT_LIST_HEAD(&n_ptr->nsub);
<<<<<<< HEAD

	hlist_add_head(&n_ptr->hash, &node_htable[tipc_hashfn(addr)]);

	list_for_each_entry(temp_node, &tipc_node_list, list) {
		if (n_ptr->addr < temp_node->addr)
			break;
	}
	list_add_tail(&n_ptr->list, &temp_node->list);
	n_ptr->block_setup = WAIT_PEER_DOWN;
=======
	INIT_LIST_HEAD(&n_ptr->conn_sks);
	__skb_queue_head_init(&n_ptr->waiting_sks);

	hlist_add_head_rcu(&n_ptr->hash, &node_htable[tipc_hashfn(addr)]);

	list_for_each_entry_rcu(temp_node, &tipc_node_list, list) {
		if (n_ptr->addr < temp_node->addr)
			break;
	}
	list_add_tail_rcu(&n_ptr->list, &temp_node->list);
	n_ptr->action_flags = TIPC_WAIT_PEER_LINKS_DOWN;
>>>>>>> v3.18
	n_ptr->signature = INVALID_NODE_SIG;

	tipc_num_nodes++;

<<<<<<< HEAD
	spin_unlock_bh(&node_create_lock);
	return n_ptr;
}

void tipc_node_delete(struct tipc_node *n_ptr)
{
	list_del(&n_ptr->list);
	hlist_del(&n_ptr->hash);
	kfree(n_ptr);
=======
	spin_unlock_bh(&node_list_lock);
	return n_ptr;
}

static void tipc_node_delete(struct tipc_node *n_ptr)
{
	list_del_rcu(&n_ptr->list);
	hlist_del_rcu(&n_ptr->hash);
	kfree_rcu(n_ptr, rcu);
>>>>>>> v3.18

	tipc_num_nodes--;
}

<<<<<<< HEAD
=======
void tipc_node_stop(void)
{
	struct tipc_node *node, *t_node;

	spin_lock_bh(&node_list_lock);
	list_for_each_entry_safe(node, t_node, &tipc_node_list, list)
		tipc_node_delete(node);
	spin_unlock_bh(&node_list_lock);
}

int tipc_node_add_conn(u32 dnode, u32 port, u32 peer_port)
{
	struct tipc_node *node;
	struct tipc_sock_conn *conn;

	if (in_own_node(dnode))
		return 0;

	node = tipc_node_find(dnode);
	if (!node) {
		pr_warn("Connecting sock to node 0x%x failed\n", dnode);
		return -EHOSTUNREACH;
	}
	conn = kmalloc(sizeof(*conn), GFP_ATOMIC);
	if (!conn)
		return -EHOSTUNREACH;
	conn->peer_node = dnode;
	conn->port = port;
	conn->peer_port = peer_port;

	tipc_node_lock(node);
	list_add_tail(&conn->list, &node->conn_sks);
	tipc_node_unlock(node);
	return 0;
}

void tipc_node_remove_conn(u32 dnode, u32 port)
{
	struct tipc_node *node;
	struct tipc_sock_conn *conn, *safe;

	if (in_own_node(dnode))
		return;

	node = tipc_node_find(dnode);
	if (!node)
		return;

	tipc_node_lock(node);
	list_for_each_entry_safe(conn, safe, &node->conn_sks, list) {
		if (port != conn->port)
			continue;
		list_del(&conn->list);
		kfree(conn);
	}
	tipc_node_unlock(node);
}

void tipc_node_abort_sock_conns(struct list_head *conns)
{
	struct tipc_sock_conn *conn, *safe;
	struct sk_buff *buf;

	list_for_each_entry_safe(conn, safe, conns, list) {
		buf = tipc_msg_create(TIPC_CRITICAL_IMPORTANCE, TIPC_CONN_MSG,
				      SHORT_H_SIZE, 0, tipc_own_addr,
				      conn->peer_node, conn->port,
				      conn->peer_port, TIPC_ERR_NO_NODE);
		if (likely(buf))
			tipc_sk_rcv(buf);
		list_del(&conn->list);
		kfree(conn);
	}
}

>>>>>>> v3.18
/**
 * tipc_node_link_up - handle addition of link
 *
 * Link becomes active (alone or shared) or standby, depending on its priority.
 */
void tipc_node_link_up(struct tipc_node *n_ptr, struct tipc_link *l_ptr)
{
	struct tipc_link **active = &n_ptr->active_links[0];

	n_ptr->working_links++;
<<<<<<< HEAD

	pr_info("Established link <%s> on network plane %c\n",
		l_ptr->name, l_ptr->b_ptr->net_plane);
=======
	n_ptr->action_flags |= TIPC_NOTIFY_LINK_UP;
	n_ptr->link_id = l_ptr->peer_bearer_id << 16 | l_ptr->bearer_id;

	pr_info("Established link <%s> on network plane %c\n",
		l_ptr->name, l_ptr->net_plane);
>>>>>>> v3.18

	if (!active[0]) {
		active[0] = active[1] = l_ptr;
		node_established_contact(n_ptr);
<<<<<<< HEAD
		return;
	}
	if (l_ptr->priority < active[0]->priority) {
		pr_info("New link <%s> becomes standby\n", l_ptr->name);
		return;
	}
	tipc_link_send_duplicate(active[0], l_ptr);
	if (l_ptr->priority == active[0]->priority) {
		active[0] = l_ptr;
		return;
=======
		goto exit;
	}
	if (l_ptr->priority < active[0]->priority) {
		pr_info("New link <%s> becomes standby\n", l_ptr->name);
		goto exit;
	}
	tipc_link_dup_queue_xmit(active[0], l_ptr);
	if (l_ptr->priority == active[0]->priority) {
		active[0] = l_ptr;
		goto exit;
>>>>>>> v3.18
	}
	pr_info("Old link <%s> becomes standby\n", active[0]->name);
	if (active[1] != active[0])
		pr_info("Old link <%s> becomes standby\n", active[1]->name);
	active[0] = active[1] = l_ptr;
<<<<<<< HEAD
=======
exit:
	/* Leave room for changeover header when returning 'mtu' to users: */
	n_ptr->act_mtus[0] = active[0]->max_pkt - INT_H_SIZE;
	n_ptr->act_mtus[1] = active[1]->max_pkt - INT_H_SIZE;
>>>>>>> v3.18
}

/**
 * node_select_active_links - select active link
 */
static void node_select_active_links(struct tipc_node *n_ptr)
{
	struct tipc_link **active = &n_ptr->active_links[0];
	u32 i;
	u32 highest_prio = 0;

	active[0] = active[1] = NULL;

	for (i = 0; i < MAX_BEARERS; i++) {
		struct tipc_link *l_ptr = n_ptr->links[i];

		if (!l_ptr || !tipc_link_is_up(l_ptr) ||
		    (l_ptr->priority < highest_prio))
			continue;

		if (l_ptr->priority > highest_prio) {
			highest_prio = l_ptr->priority;
			active[0] = active[1] = l_ptr;
		} else {
			active[1] = l_ptr;
		}
	}
}

/**
 * tipc_node_link_down - handle loss of link
 */
void tipc_node_link_down(struct tipc_node *n_ptr, struct tipc_link *l_ptr)
{
	struct tipc_link **active;

	n_ptr->working_links--;
<<<<<<< HEAD

	if (!tipc_link_is_active(l_ptr)) {
		pr_info("Lost standby link <%s> on network plane %c\n",
			l_ptr->name, l_ptr->b_ptr->net_plane);
		return;
	}
	pr_info("Lost link <%s> on network plane %c\n",
		l_ptr->name, l_ptr->b_ptr->net_plane);
=======
	n_ptr->action_flags |= TIPC_NOTIFY_LINK_DOWN;
	n_ptr->link_id = l_ptr->peer_bearer_id << 16 | l_ptr->bearer_id;

	if (!tipc_link_is_active(l_ptr)) {
		pr_info("Lost standby link <%s> on network plane %c\n",
			l_ptr->name, l_ptr->net_plane);
		return;
	}
	pr_info("Lost link <%s> on network plane %c\n",
		l_ptr->name, l_ptr->net_plane);
>>>>>>> v3.18

	active = &n_ptr->active_links[0];
	if (active[0] == l_ptr)
		active[0] = active[1];
	if (active[1] == l_ptr)
		active[1] = active[0];
	if (active[0] == l_ptr)
		node_select_active_links(n_ptr);
	if (tipc_node_is_up(n_ptr))
<<<<<<< HEAD
		tipc_link_changeover(l_ptr);
	else
		node_lost_contact(n_ptr);
=======
		tipc_link_failover_send_queue(l_ptr);
	else
		node_lost_contact(n_ptr);

	/* Leave room for changeover header when returning 'mtu' to users: */
	if (active[0]) {
		n_ptr->act_mtus[0] = active[0]->max_pkt - INT_H_SIZE;
		n_ptr->act_mtus[1] = active[1]->max_pkt - INT_H_SIZE;
		return;
	}

	/* Loopback link went down? No fragmentation needed from now on. */
	if (n_ptr->addr == tipc_own_addr) {
		n_ptr->act_mtus[0] = MAX_MSG_SIZE;
		n_ptr->act_mtus[1] = MAX_MSG_SIZE;
	}
>>>>>>> v3.18
}

int tipc_node_active_links(struct tipc_node *n_ptr)
{
	return n_ptr->active_links[0] != NULL;
}

<<<<<<< HEAD
int tipc_node_redundant_links(struct tipc_node *n_ptr)
{
	return n_ptr->working_links > 1;
}

=======
>>>>>>> v3.18
int tipc_node_is_up(struct tipc_node *n_ptr)
{
	return tipc_node_active_links(n_ptr);
}

void tipc_node_attach_link(struct tipc_node *n_ptr, struct tipc_link *l_ptr)
{
<<<<<<< HEAD
	n_ptr->links[l_ptr->b_ptr->identity] = l_ptr;
	atomic_inc(&tipc_num_links);
=======
	n_ptr->links[l_ptr->bearer_id] = l_ptr;
	spin_lock_bh(&node_list_lock);
	tipc_num_links++;
	spin_unlock_bh(&node_list_lock);
>>>>>>> v3.18
	n_ptr->link_cnt++;
}

void tipc_node_detach_link(struct tipc_node *n_ptr, struct tipc_link *l_ptr)
{
<<<<<<< HEAD
	n_ptr->links[l_ptr->b_ptr->identity] = NULL;
	atomic_dec(&tipc_num_links);
	n_ptr->link_cnt--;
=======
	int i;

	for (i = 0; i < MAX_BEARERS; i++) {
		if (l_ptr != n_ptr->links[i])
			continue;
		n_ptr->links[i] = NULL;
		spin_lock_bh(&node_list_lock);
		tipc_num_links--;
		spin_unlock_bh(&node_list_lock);
		n_ptr->link_cnt--;
	}
>>>>>>> v3.18
}

static void node_established_contact(struct tipc_node *n_ptr)
{
<<<<<<< HEAD
	tipc_k_signal((Handler)tipc_named_node_up, n_ptr->addr);
=======
	n_ptr->action_flags |= TIPC_NOTIFY_NODE_UP;
>>>>>>> v3.18
	n_ptr->bclink.oos_state = 0;
	n_ptr->bclink.acked = tipc_bclink_get_last_sent();
	tipc_bclink_add_node(n_ptr->addr);
}

<<<<<<< HEAD
static void node_name_purge_complete(unsigned long node_addr)
{
	struct tipc_node *n_ptr;

	read_lock_bh(&tipc_net_lock);
	n_ptr = tipc_node_find(node_addr);
	if (n_ptr) {
		tipc_node_lock(n_ptr);
		n_ptr->block_setup &= ~WAIT_NAMES_GONE;
		tipc_node_unlock(n_ptr);
	}
	read_unlock_bh(&tipc_net_lock);
}

=======
>>>>>>> v3.18
static void node_lost_contact(struct tipc_node *n_ptr)
{
	char addr_string[16];
	u32 i;

	pr_info("Lost contact with %s\n",
		tipc_addr_string_fill(addr_string, n_ptr->addr));

	/* Flush broadcast link info associated with lost node */
	if (n_ptr->bclink.recv_permitted) {
<<<<<<< HEAD
		while (n_ptr->bclink.deferred_head) {
			struct sk_buff *buf = n_ptr->bclink.deferred_head;
			n_ptr->bclink.deferred_head = buf->next;
			kfree_skb(buf);
		}
		n_ptr->bclink.deferred_size = 0;

		if (n_ptr->bclink.defragm) {
			kfree_skb(n_ptr->bclink.defragm);
			n_ptr->bclink.defragm = NULL;
=======
		kfree_skb_list(n_ptr->bclink.deferred_head);
		n_ptr->bclink.deferred_size = 0;

		if (n_ptr->bclink.reasm_buf) {
			kfree_skb(n_ptr->bclink.reasm_buf);
			n_ptr->bclink.reasm_buf = NULL;
>>>>>>> v3.18
		}

		tipc_bclink_remove_node(n_ptr->addr);
		tipc_bclink_acknowledge(n_ptr, INVALID_LINK_SEQ);

		n_ptr->bclink.recv_permitted = false;
	}

	/* Abort link changeover */
	for (i = 0; i < MAX_BEARERS; i++) {
		struct tipc_link *l_ptr = n_ptr->links[i];
		if (!l_ptr)
			continue;
		l_ptr->reset_checkpoint = l_ptr->next_in_no;
		l_ptr->exp_msg_count = 0;
		tipc_link_reset_fragments(l_ptr);
	}

<<<<<<< HEAD
	/* Notify subscribers */
	tipc_nodesub_notify(n_ptr);

	/* Prevent re-contact with node until cleanup is done */
	n_ptr->block_setup = WAIT_PEER_DOWN | WAIT_NAMES_GONE;
	tipc_k_signal((Handler)node_name_purge_complete, n_ptr->addr);
=======
	n_ptr->action_flags &= ~TIPC_WAIT_OWN_LINKS_DOWN;

	/* Notify subscribers and prevent re-contact with node until
	 * cleanup is done.
	 */
	n_ptr->action_flags |= TIPC_WAIT_PEER_LINKS_DOWN |
			       TIPC_NOTIFY_NODE_DOWN;
>>>>>>> v3.18
}

struct sk_buff *tipc_node_get_nodes(const void *req_tlv_area, int req_tlv_space)
{
	u32 domain;
	struct sk_buff *buf;
	struct tipc_node *n_ptr;
	struct tipc_node_info node_info;
	u32 payload_size;

	if (!TLV_CHECK(req_tlv_area, req_tlv_space, TIPC_TLV_NET_ADDR))
		return tipc_cfg_reply_error_string(TIPC_CFG_TLV_ERROR);

	domain = ntohl(*(__be32 *)TLV_DATA(req_tlv_area));
	if (!tipc_addr_domain_valid(domain))
		return tipc_cfg_reply_error_string(TIPC_CFG_INVALID_VALUE
						   " (network address)");

<<<<<<< HEAD
	read_lock_bh(&tipc_net_lock);
	if (!tipc_num_nodes) {
		read_unlock_bh(&tipc_net_lock);
=======
	spin_lock_bh(&node_list_lock);
	if (!tipc_num_nodes) {
		spin_unlock_bh(&node_list_lock);
>>>>>>> v3.18
		return tipc_cfg_reply_none();
	}

	/* For now, get space for all other nodes */
	payload_size = TLV_SPACE(sizeof(node_info)) * tipc_num_nodes;
	if (payload_size > 32768u) {
<<<<<<< HEAD
		read_unlock_bh(&tipc_net_lock);
		return tipc_cfg_reply_error_string(TIPC_CFG_NOT_SUPPORTED
						   " (too many nodes)");
	}
	buf = tipc_cfg_reply_alloc(payload_size);
	if (!buf) {
		read_unlock_bh(&tipc_net_lock);
		return NULL;
	}

	/* Add TLVs for all nodes in scope */
	list_for_each_entry(n_ptr, &tipc_node_list, list) {
=======
		spin_unlock_bh(&node_list_lock);
		return tipc_cfg_reply_error_string(TIPC_CFG_NOT_SUPPORTED
						   " (too many nodes)");
	}
	spin_unlock_bh(&node_list_lock);

	buf = tipc_cfg_reply_alloc(payload_size);
	if (!buf)
		return NULL;

	/* Add TLVs for all nodes in scope */
	rcu_read_lock();
	list_for_each_entry_rcu(n_ptr, &tipc_node_list, list) {
>>>>>>> v3.18
		if (!tipc_in_scope(domain, n_ptr->addr))
			continue;
		node_info.addr = htonl(n_ptr->addr);
		node_info.up = htonl(tipc_node_is_up(n_ptr));
		tipc_cfg_append_tlv(buf, TIPC_TLV_NODE_INFO,
				    &node_info, sizeof(node_info));
	}
<<<<<<< HEAD

	read_unlock_bh(&tipc_net_lock);
=======
	rcu_read_unlock();
>>>>>>> v3.18
	return buf;
}

struct sk_buff *tipc_node_get_links(const void *req_tlv_area, int req_tlv_space)
{
	u32 domain;
	struct sk_buff *buf;
	struct tipc_node *n_ptr;
	struct tipc_link_info link_info;
	u32 payload_size;

	if (!TLV_CHECK(req_tlv_area, req_tlv_space, TIPC_TLV_NET_ADDR))
		return tipc_cfg_reply_error_string(TIPC_CFG_TLV_ERROR);

	domain = ntohl(*(__be32 *)TLV_DATA(req_tlv_area));
	if (!tipc_addr_domain_valid(domain))
		return tipc_cfg_reply_error_string(TIPC_CFG_INVALID_VALUE
						   " (network address)");

	if (!tipc_own_addr)
		return tipc_cfg_reply_none();

<<<<<<< HEAD
	read_lock_bh(&tipc_net_lock);

	/* Get space for all unicast links + broadcast link */
	payload_size = TLV_SPACE(sizeof(link_info)) *
		(atomic_read(&tipc_num_links) + 1);
	if (payload_size > 32768u) {
		read_unlock_bh(&tipc_net_lock);
		return tipc_cfg_reply_error_string(TIPC_CFG_NOT_SUPPORTED
						   " (too many links)");
	}
	buf = tipc_cfg_reply_alloc(payload_size);
	if (!buf) {
		read_unlock_bh(&tipc_net_lock);
		return NULL;
	}
=======
	spin_lock_bh(&node_list_lock);
	/* Get space for all unicast links + broadcast link */
	payload_size = TLV_SPACE((sizeof(link_info)) * (tipc_num_links + 1));
	if (payload_size > 32768u) {
		spin_unlock_bh(&node_list_lock);
		return tipc_cfg_reply_error_string(TIPC_CFG_NOT_SUPPORTED
						   " (too many links)");
	}
	spin_unlock_bh(&node_list_lock);

	buf = tipc_cfg_reply_alloc(payload_size);
	if (!buf)
		return NULL;
>>>>>>> v3.18

	/* Add TLV for broadcast link */
	link_info.dest = htonl(tipc_cluster_mask(tipc_own_addr));
	link_info.up = htonl(1);
	strlcpy(link_info.str, tipc_bclink_name, TIPC_MAX_LINK_NAME);
	tipc_cfg_append_tlv(buf, TIPC_TLV_LINK_INFO, &link_info, sizeof(link_info));

	/* Add TLVs for any other links in scope */
<<<<<<< HEAD
	list_for_each_entry(n_ptr, &tipc_node_list, list) {
=======
	rcu_read_lock();
	list_for_each_entry_rcu(n_ptr, &tipc_node_list, list) {
>>>>>>> v3.18
		u32 i;

		if (!tipc_in_scope(domain, n_ptr->addr))
			continue;
		tipc_node_lock(n_ptr);
		for (i = 0; i < MAX_BEARERS; i++) {
			if (!n_ptr->links[i])
				continue;
			link_info.dest = htonl(n_ptr->addr);
			link_info.up = htonl(tipc_link_is_up(n_ptr->links[i]));
			strcpy(link_info.str, n_ptr->links[i]->name);
			tipc_cfg_append_tlv(buf, TIPC_TLV_LINK_INFO,
					    &link_info, sizeof(link_info));
		}
		tipc_node_unlock(n_ptr);
	}
<<<<<<< HEAD

	read_unlock_bh(&tipc_net_lock);
	return buf;
}
=======
	rcu_read_unlock();
	return buf;
}

/**
 * tipc_node_get_linkname - get the name of a link
 *
 * @bearer_id: id of the bearer
 * @node: peer node address
 * @linkname: link name output buffer
 *
 * Returns 0 on success
 */
int tipc_node_get_linkname(u32 bearer_id, u32 addr, char *linkname, size_t len)
{
	struct tipc_link *link;
	struct tipc_node *node = tipc_node_find(addr);

	if ((bearer_id >= MAX_BEARERS) || !node)
		return -EINVAL;
	tipc_node_lock(node);
	link = node->links[bearer_id];
	if (link) {
		strncpy(linkname, link->name, len);
		tipc_node_unlock(node);
		return 0;
	}
	tipc_node_unlock(node);
	return -EINVAL;
}

void tipc_node_unlock(struct tipc_node *node)
{
	LIST_HEAD(nsub_list);
	LIST_HEAD(conn_sks);
	struct sk_buff_head waiting_sks;
	u32 addr = 0;
	int flags = node->action_flags;
	u32 link_id = 0;

	if (likely(!flags)) {
		spin_unlock_bh(&node->lock);
		return;
	}

	addr = node->addr;
	link_id = node->link_id;
	__skb_queue_head_init(&waiting_sks);

	if (flags & TIPC_WAKEUP_USERS)
		skb_queue_splice_init(&node->waiting_sks, &waiting_sks);

	if (flags & TIPC_NOTIFY_NODE_DOWN) {
		list_replace_init(&node->nsub, &nsub_list);
		list_replace_init(&node->conn_sks, &conn_sks);
	}
	node->action_flags &= ~(TIPC_WAKEUP_USERS | TIPC_NOTIFY_NODE_DOWN |
				TIPC_NOTIFY_NODE_UP | TIPC_NOTIFY_LINK_UP |
				TIPC_NOTIFY_LINK_DOWN |
				TIPC_WAKEUP_BCAST_USERS);

	spin_unlock_bh(&node->lock);

	while (!skb_queue_empty(&waiting_sks))
		tipc_sk_rcv(__skb_dequeue(&waiting_sks));

	if (!list_empty(&conn_sks))
		tipc_node_abort_sock_conns(&conn_sks);

	if (!list_empty(&nsub_list))
		tipc_nodesub_notify(&nsub_list);

	if (flags & TIPC_WAKEUP_BCAST_USERS)
		tipc_bclink_wakeup_users();

	if (flags & TIPC_NOTIFY_NODE_UP)
		tipc_named_node_up(addr);

	if (flags & TIPC_NOTIFY_LINK_UP)
		tipc_nametbl_publish(TIPC_LINK_STATE, addr, addr,
				     TIPC_NODE_SCOPE, link_id, addr);

	if (flags & TIPC_NOTIFY_LINK_DOWN)
		tipc_nametbl_withdraw(TIPC_LINK_STATE, addr,
				      link_id, addr);
}
>>>>>>> v3.18
