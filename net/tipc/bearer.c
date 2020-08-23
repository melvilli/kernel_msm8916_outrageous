/*
 * net/tipc/bearer.c: TIPC bearer code
 *
<<<<<<< HEAD
 * Copyright (c) 1996-2006, Ericsson AB
 * Copyright (c) 2004-2006, 2010-2011, Wind River Systems
=======
 * Copyright (c) 1996-2006, 2013, Ericsson AB
 * Copyright (c) 2004-2006, 2010-2013, Wind River Systems
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
#include "bearer.h"
#include "discover.h"

#define MAX_ADDR_STR 60

<<<<<<< HEAD
static struct tipc_media *media_list[MAX_MEDIA];
static u32 media_count;

struct tipc_bearer tipc_bearers[MAX_BEARERS];

static void bearer_disable(struct tipc_bearer *b_ptr);
=======
static struct tipc_media * const media_info_array[] = {
	&eth_media_info,
#ifdef CONFIG_TIPC_MEDIA_IB
	&ib_media_info,
#endif
	NULL
};

struct tipc_bearer __rcu *bearer_list[MAX_BEARERS + 1];

static void bearer_disable(struct tipc_bearer *b_ptr, bool shutting_down);
>>>>>>> v3.18

/**
 * tipc_media_find - locates specified media object by name
 */
struct tipc_media *tipc_media_find(const char *name)
{
	u32 i;

<<<<<<< HEAD
	for (i = 0; i < media_count; i++) {
		if (!strcmp(media_list[i]->name, name))
			return media_list[i];
	}
	return NULL;
=======
	for (i = 0; media_info_array[i] != NULL; i++) {
		if (!strcmp(media_info_array[i]->name, name))
			break;
	}
	return media_info_array[i];
>>>>>>> v3.18
}

/**
 * media_find_id - locates specified media object by type identifier
 */
static struct tipc_media *media_find_id(u8 type)
{
	u32 i;

<<<<<<< HEAD
	for (i = 0; i < media_count; i++) {
		if (media_list[i]->type_id == type)
			return media_list[i];
	}
	return NULL;
}

/**
 * tipc_register_media - register a media type
 *
 * Bearers for this media type must be activated separately at a later stage.
 */
int tipc_register_media(struct tipc_media *m_ptr)
{
	int res = -EINVAL;

	write_lock_bh(&tipc_net_lock);

	if ((strlen(m_ptr->name) + 1) > TIPC_MAX_MEDIA_NAME)
		goto exit;
	if (m_ptr->priority > TIPC_MAX_LINK_PRI)
		goto exit;
	if ((m_ptr->tolerance < TIPC_MIN_LINK_TOL) ||
	    (m_ptr->tolerance > TIPC_MAX_LINK_TOL))
		goto exit;
	if (media_count >= MAX_MEDIA)
		goto exit;
	if (tipc_media_find(m_ptr->name) || media_find_id(m_ptr->type_id))
		goto exit;

	media_list[media_count] = m_ptr;
	media_count++;
	res = 0;
exit:
	write_unlock_bh(&tipc_net_lock);
	if (res)
		pr_warn("Media <%s> registration error\n", m_ptr->name);
	return res;
=======
	for (i = 0; media_info_array[i] != NULL; i++) {
		if (media_info_array[i]->type_id == type)
			break;
	}
	return media_info_array[i];
>>>>>>> v3.18
}

/**
 * tipc_media_addr_printf - record media address in print buffer
 */
void tipc_media_addr_printf(char *buf, int len, struct tipc_media_addr *a)
{
	char addr_str[MAX_ADDR_STR];
	struct tipc_media *m_ptr;
	int ret;

	m_ptr = media_find_id(a->media_id);

	if (m_ptr && !m_ptr->addr2str(a, addr_str, sizeof(addr_str)))
		ret = tipc_snprintf(buf, len, "%s(%s)", m_ptr->name, addr_str);
	else {
		u32 i;

		ret = tipc_snprintf(buf, len, "UNKNOWN(%u)", a->media_id);
		for (i = 0; i < sizeof(a->value); i++)
			ret += tipc_snprintf(buf - ret, len + ret,
					    "-%02x", a->value[i]);
	}
}

/**
 * tipc_media_get_names - record names of registered media in buffer
 */
struct sk_buff *tipc_media_get_names(void)
{
	struct sk_buff *buf;
	int i;

	buf = tipc_cfg_reply_alloc(MAX_MEDIA * TLV_SPACE(TIPC_MAX_MEDIA_NAME));
	if (!buf)
		return NULL;

<<<<<<< HEAD
	read_lock_bh(&tipc_net_lock);
	for (i = 0; i < media_count; i++) {
		tipc_cfg_append_tlv(buf, TIPC_TLV_MEDIA_NAME,
				    media_list[i]->name,
				    strlen(media_list[i]->name) + 1);
	}
	read_unlock_bh(&tipc_net_lock);
=======
	for (i = 0; media_info_array[i] != NULL; i++) {
		tipc_cfg_append_tlv(buf, TIPC_TLV_MEDIA_NAME,
				    media_info_array[i]->name,
				    strlen(media_info_array[i]->name) + 1);
	}
>>>>>>> v3.18
	return buf;
}

/**
 * bearer_name_validate - validate & (optionally) deconstruct bearer name
 * @name: ptr to bearer name string
 * @name_parts: ptr to area for bearer name components (or NULL if not needed)
 *
 * Returns 1 if bearer name is valid, otherwise 0.
 */
static int bearer_name_validate(const char *name,
				struct tipc_bearer_names *name_parts)
{
	char name_copy[TIPC_MAX_BEARER_NAME];
	char *media_name;
	char *if_name;
	u32 media_len;
	u32 if_len;

	/* copy bearer name & ensure length is OK */
	name_copy[TIPC_MAX_BEARER_NAME - 1] = 0;
	/* need above in case non-Posix strncpy() doesn't pad with nulls */
	strncpy(name_copy, name, TIPC_MAX_BEARER_NAME);
	if (name_copy[TIPC_MAX_BEARER_NAME - 1] != 0)
		return 0;

	/* ensure all component parts of bearer name are present */
	media_name = name_copy;
	if_name = strchr(media_name, ':');
	if (if_name == NULL)
		return 0;
	*(if_name++) = 0;
	media_len = if_name - media_name;
	if_len = strlen(if_name) + 1;

	/* validate component parts of bearer name */
	if ((media_len <= 1) || (media_len > TIPC_MAX_MEDIA_NAME) ||
	    (if_len <= 1) || (if_len > TIPC_MAX_IF_NAME))
		return 0;

	/* return bearer name components, if necessary */
	if (name_parts) {
		strcpy(name_parts->media_name, media_name);
		strcpy(name_parts->if_name, if_name);
	}
	return 1;
}

/**
 * tipc_bearer_find - locates bearer object with matching bearer name
 */
struct tipc_bearer *tipc_bearer_find(const char *name)
{
	struct tipc_bearer *b_ptr;
	u32 i;

<<<<<<< HEAD
	for (i = 0, b_ptr = tipc_bearers; i < MAX_BEARERS; i++, b_ptr++) {
		if (b_ptr->active && (!strcmp(b_ptr->name, name)))
			return b_ptr;
	}
	return NULL;
}

/**
 * tipc_bearer_find_interface - locates bearer object with matching interface name
 */
struct tipc_bearer *tipc_bearer_find_interface(const char *if_name)
{
	struct tipc_bearer *b_ptr;
	char *b_if_name;
	u32 i;

	for (i = 0, b_ptr = tipc_bearers; i < MAX_BEARERS; i++, b_ptr++) {
		if (!b_ptr->active)
			continue;
		b_if_name = strchr(b_ptr->name, ':') + 1;
		if (!strcmp(b_if_name, if_name))
=======
	for (i = 0; i < MAX_BEARERS; i++) {
		b_ptr = rtnl_dereference(bearer_list[i]);
		if (b_ptr && (!strcmp(b_ptr->name, name)))
>>>>>>> v3.18
			return b_ptr;
	}
	return NULL;
}

/**
 * tipc_bearer_get_names - record names of bearers in buffer
 */
struct sk_buff *tipc_bearer_get_names(void)
{
	struct sk_buff *buf;
<<<<<<< HEAD
	struct tipc_bearer *b_ptr;
=======
	struct tipc_bearer *b;
>>>>>>> v3.18
	int i, j;

	buf = tipc_cfg_reply_alloc(MAX_BEARERS * TLV_SPACE(TIPC_MAX_BEARER_NAME));
	if (!buf)
		return NULL;

<<<<<<< HEAD
	read_lock_bh(&tipc_net_lock);
	for (i = 0; i < media_count; i++) {
		for (j = 0; j < MAX_BEARERS; j++) {
			b_ptr = &tipc_bearers[j];
			if (b_ptr->active && (b_ptr->media == media_list[i])) {
				tipc_cfg_append_tlv(buf, TIPC_TLV_BEARER_NAME,
						    b_ptr->name,
						    strlen(b_ptr->name) + 1);
			}
		}
	}
	read_unlock_bh(&tipc_net_lock);
	return buf;
}

void tipc_bearer_add_dest(struct tipc_bearer *b_ptr, u32 dest)
{
	tipc_nmap_add(&b_ptr->nodes, dest);
	tipc_bcbearer_sort();
	tipc_disc_add_dest(b_ptr->link_req);
}

void tipc_bearer_remove_dest(struct tipc_bearer *b_ptr, u32 dest)
{
	tipc_nmap_remove(&b_ptr->nodes, dest);
	tipc_bcbearer_sort();
	tipc_disc_remove_dest(b_ptr->link_req);
}

/*
 * Interrupt enabling new requests after bearer blocking:
 * See bearer_send().
 */
void tipc_continue(struct tipc_bearer *b)
{
	spin_lock_bh(&b->lock);
	b->blocked = 0;
	spin_unlock_bh(&b->lock);
}

/*
 * tipc_bearer_blocked - determines if bearer is currently blocked
 */
int tipc_bearer_blocked(struct tipc_bearer *b)
{
	int res;

	spin_lock_bh(&b->lock);
	res = b->blocked;
	spin_unlock_bh(&b->lock);

	return res;
=======
	for (i = 0; media_info_array[i] != NULL; i++) {
		for (j = 0; j < MAX_BEARERS; j++) {
			b = rtnl_dereference(bearer_list[j]);
			if (!b)
				continue;
			if (b->media == media_info_array[i]) {
				tipc_cfg_append_tlv(buf, TIPC_TLV_BEARER_NAME,
						    b->name,
						    strlen(b->name) + 1);
			}
		}
	}
	return buf;
}

void tipc_bearer_add_dest(u32 bearer_id, u32 dest)
{
	struct tipc_bearer *b_ptr;

	rcu_read_lock();
	b_ptr = rcu_dereference_rtnl(bearer_list[bearer_id]);
	if (b_ptr) {
		tipc_bcbearer_sort(&b_ptr->nodes, dest, true);
		tipc_disc_add_dest(b_ptr->link_req);
	}
	rcu_read_unlock();
}

void tipc_bearer_remove_dest(u32 bearer_id, u32 dest)
{
	struct tipc_bearer *b_ptr;

	rcu_read_lock();
	b_ptr = rcu_dereference_rtnl(bearer_list[bearer_id]);
	if (b_ptr) {
		tipc_bcbearer_sort(&b_ptr->nodes, dest, false);
		tipc_disc_remove_dest(b_ptr->link_req);
	}
	rcu_read_unlock();
>>>>>>> v3.18
}

/**
 * tipc_enable_bearer - enable bearer with the given name
 */
int tipc_enable_bearer(const char *name, u32 disc_domain, u32 priority)
{
	struct tipc_bearer *b_ptr;
	struct tipc_media *m_ptr;
	struct tipc_bearer_names b_names;
	char addr_string[16];
	u32 bearer_id;
	u32 with_this_prio;
	u32 i;
	int res = -EINVAL;

	if (!tipc_own_addr) {
		pr_warn("Bearer <%s> rejected, not supported in standalone mode\n",
			name);
		return -ENOPROTOOPT;
	}
	if (!bearer_name_validate(name, &b_names)) {
		pr_warn("Bearer <%s> rejected, illegal name\n", name);
		return -EINVAL;
	}
	if (tipc_addr_domain_valid(disc_domain) &&
	    (disc_domain != tipc_own_addr)) {
		if (tipc_in_scope(disc_domain, tipc_own_addr)) {
			disc_domain = tipc_own_addr & TIPC_CLUSTER_MASK;
			res = 0;   /* accept any node in own cluster */
		} else if (in_own_cluster_exact(disc_domain))
			res = 0;   /* accept specified node in own cluster */
	}
	if (res) {
		pr_warn("Bearer <%s> rejected, illegal discovery domain\n",
			name);
		return -EINVAL;
	}
	if ((priority > TIPC_MAX_LINK_PRI) &&
	    (priority != TIPC_MEDIA_LINK_PRI)) {
		pr_warn("Bearer <%s> rejected, illegal priority\n", name);
		return -EINVAL;
	}

<<<<<<< HEAD
	write_lock_bh(&tipc_net_lock);

=======
>>>>>>> v3.18
	m_ptr = tipc_media_find(b_names.media_name);
	if (!m_ptr) {
		pr_warn("Bearer <%s> rejected, media <%s> not registered\n",
			name, b_names.media_name);
<<<<<<< HEAD
		goto exit;
=======
		return -EINVAL;
>>>>>>> v3.18
	}

	if (priority == TIPC_MEDIA_LINK_PRI)
		priority = m_ptr->priority;

restart:
	bearer_id = MAX_BEARERS;
	with_this_prio = 1;
	for (i = MAX_BEARERS; i-- != 0; ) {
<<<<<<< HEAD
		if (!tipc_bearers[i].active) {
			bearer_id = i;
			continue;
		}
		if (!strcmp(name, tipc_bearers[i].name)) {
			pr_warn("Bearer <%s> rejected, already enabled\n",
				name);
			goto exit;
		}
		if ((tipc_bearers[i].priority == priority) &&
=======
		b_ptr = rtnl_dereference(bearer_list[i]);
		if (!b_ptr) {
			bearer_id = i;
			continue;
		}
		if (!strcmp(name, b_ptr->name)) {
			pr_warn("Bearer <%s> rejected, already enabled\n",
				name);
			return -EINVAL;
		}
		if ((b_ptr->priority == priority) &&
>>>>>>> v3.18
		    (++with_this_prio > 2)) {
			if (priority-- == 0) {
				pr_warn("Bearer <%s> rejected, duplicate priority\n",
					name);
<<<<<<< HEAD
				goto exit;
=======
				return -EINVAL;
>>>>>>> v3.18
			}
			pr_warn("Bearer <%s> priority adjustment required %u->%u\n",
				name, priority + 1, priority);
			goto restart;
		}
	}
	if (bearer_id >= MAX_BEARERS) {
		pr_warn("Bearer <%s> rejected, bearer limit reached (%u)\n",
			name, MAX_BEARERS);
<<<<<<< HEAD
		goto exit;
	}

	b_ptr = &tipc_bearers[bearer_id];
	strcpy(b_ptr->name, name);
	res = m_ptr->enable_bearer(b_ptr);
	if (res) {
		pr_warn("Bearer <%s> rejected, enable failure (%d)\n",
			name, -res);
		goto exit;
	}

	b_ptr->identity = bearer_id;
	b_ptr->media = m_ptr;
	b_ptr->tolerance = m_ptr->tolerance;
	b_ptr->window = m_ptr->window;
	b_ptr->net_plane = bearer_id + 'A';
	b_ptr->active = 1;
	b_ptr->priority = priority;
	INIT_LIST_HEAD(&b_ptr->links);
	spin_lock_init(&b_ptr->lock);

	res = tipc_disc_create(b_ptr, &b_ptr->bcast_addr, disc_domain);
	if (res) {
		bearer_disable(b_ptr);
		pr_warn("Bearer <%s> rejected, discovery object creation failed\n",
			name);
		goto exit;
	}
	pr_info("Enabled bearer <%s>, discovery domain %s, priority %u\n",
		name,
		tipc_addr_string_fill(addr_string, disc_domain), priority);
exit:
	write_unlock_bh(&tipc_net_lock);
=======
		return -EINVAL;
	}

	b_ptr = kzalloc(sizeof(*b_ptr), GFP_ATOMIC);
	if (!b_ptr)
		return -ENOMEM;

	strcpy(b_ptr->name, name);
	b_ptr->media = m_ptr;
	res = m_ptr->enable_media(b_ptr);
	if (res) {
		pr_warn("Bearer <%s> rejected, enable failure (%d)\n",
			name, -res);
		return -EINVAL;
	}

	b_ptr->identity = bearer_id;
	b_ptr->tolerance = m_ptr->tolerance;
	b_ptr->window = m_ptr->window;
	b_ptr->domain = disc_domain;
	b_ptr->net_plane = bearer_id + 'A';
	b_ptr->priority = priority;

	res = tipc_disc_create(b_ptr, &b_ptr->bcast_addr);
	if (res) {
		bearer_disable(b_ptr, false);
		pr_warn("Bearer <%s> rejected, discovery object creation failed\n",
			name);
		return -EINVAL;
	}

	rcu_assign_pointer(bearer_list[bearer_id], b_ptr);

	pr_info("Enabled bearer <%s>, discovery domain %s, priority %u\n",
		name,
		tipc_addr_string_fill(addr_string, disc_domain), priority);
>>>>>>> v3.18
	return res;
}

/**
<<<<<<< HEAD
 * tipc_block_bearer - Block the bearer with the given name, and reset all its links
 */
int tipc_block_bearer(const char *name)
{
	struct tipc_bearer *b_ptr = NULL;
	struct tipc_link *l_ptr;
	struct tipc_link *temp_l_ptr;

	read_lock_bh(&tipc_net_lock);
	b_ptr = tipc_bearer_find(name);
	if (!b_ptr) {
		pr_warn("Attempt to block unknown bearer <%s>\n", name);
		read_unlock_bh(&tipc_net_lock);
		return -EINVAL;
	}

	pr_info("Blocking bearer <%s>\n", name);
	spin_lock_bh(&b_ptr->lock);
	b_ptr->blocked = 1;
	list_for_each_entry_safe(l_ptr, temp_l_ptr, &b_ptr->links, link_list) {
		struct tipc_node *n_ptr = l_ptr->owner;

		spin_lock_bh(&n_ptr->lock);
		tipc_link_reset(l_ptr);
		spin_unlock_bh(&n_ptr->lock);
	}
	spin_unlock_bh(&b_ptr->lock);
	read_unlock_bh(&tipc_net_lock);
=======
 * tipc_reset_bearer - Reset all links established over this bearer
 */
static int tipc_reset_bearer(struct tipc_bearer *b_ptr)
{
	pr_info("Resetting bearer <%s>\n", b_ptr->name);
	tipc_link_reset_list(b_ptr->identity);
	tipc_disc_reset(b_ptr);
>>>>>>> v3.18
	return 0;
}

/**
 * bearer_disable
 *
<<<<<<< HEAD
 * Note: This routine assumes caller holds tipc_net_lock.
 */
static void bearer_disable(struct tipc_bearer *b_ptr)
{
	struct tipc_link *l_ptr;
	struct tipc_link *temp_l_ptr;

	pr_info("Disabling bearer <%s>\n", b_ptr->name);
	spin_lock_bh(&b_ptr->lock);
	b_ptr->blocked = 1;
	b_ptr->media->disable_bearer(b_ptr);
	list_for_each_entry_safe(l_ptr, temp_l_ptr, &b_ptr->links, link_list) {
		tipc_link_delete(l_ptr);
	}
	if (b_ptr->link_req)
		tipc_disc_delete(b_ptr->link_req);
	spin_unlock_bh(&b_ptr->lock);
	memset(b_ptr, 0, sizeof(struct tipc_bearer));
=======
 * Note: This routine assumes caller holds RTNL lock.
 */
static void bearer_disable(struct tipc_bearer *b_ptr, bool shutting_down)
{
	u32 i;

	pr_info("Disabling bearer <%s>\n", b_ptr->name);
	b_ptr->media->disable_media(b_ptr);

	tipc_link_delete_list(b_ptr->identity, shutting_down);
	if (b_ptr->link_req)
		tipc_disc_delete(b_ptr->link_req);

	for (i = 0; i < MAX_BEARERS; i++) {
		if (b_ptr == rtnl_dereference(bearer_list[i])) {
			RCU_INIT_POINTER(bearer_list[i], NULL);
			break;
		}
	}
	kfree_rcu(b_ptr, rcu);
>>>>>>> v3.18
}

int tipc_disable_bearer(const char *name)
{
	struct tipc_bearer *b_ptr;
	int res;

<<<<<<< HEAD
	write_lock_bh(&tipc_net_lock);
=======
>>>>>>> v3.18
	b_ptr = tipc_bearer_find(name);
	if (b_ptr == NULL) {
		pr_warn("Attempt to disable unknown bearer <%s>\n", name);
		res = -EINVAL;
	} else {
<<<<<<< HEAD
		bearer_disable(b_ptr);
		res = 0;
	}
	write_unlock_bh(&tipc_net_lock);
	return res;
}



void tipc_bearer_stop(void)
{
	u32 i;

	for (i = 0; i < MAX_BEARERS; i++) {
		if (tipc_bearers[i].active)
			bearer_disable(&tipc_bearers[i]);
	}
	media_count = 0;
=======
		bearer_disable(b_ptr, false);
		res = 0;
	}
	return res;
}

int tipc_enable_l2_media(struct tipc_bearer *b)
{
	struct net_device *dev;
	char *driver_name = strchr((const char *)b->name, ':') + 1;

	/* Find device with specified name */
	dev = dev_get_by_name(&init_net, driver_name);
	if (!dev)
		return -ENODEV;

	/* Associate TIPC bearer with L2 bearer */
	rcu_assign_pointer(b->media_ptr, dev);
	memset(&b->bcast_addr, 0, sizeof(b->bcast_addr));
	memcpy(b->bcast_addr.value, dev->broadcast, b->media->hwaddr_len);
	b->bcast_addr.media_id = b->media->type_id;
	b->bcast_addr.broadcast = 1;
	b->mtu = dev->mtu;
	b->media->raw2addr(b, &b->addr, (char *)dev->dev_addr);
	rcu_assign_pointer(dev->tipc_ptr, b);
	return 0;
}

/* tipc_disable_l2_media - detach TIPC bearer from an L2 interface
 *
 * Mark L2 bearer as inactive so that incoming buffers are thrown away,
 * then get worker thread to complete bearer cleanup.  (Can't do cleanup
 * here because cleanup code needs to sleep and caller holds spinlocks.)
 */
void tipc_disable_l2_media(struct tipc_bearer *b)
{
	struct net_device *dev;

	dev = (struct net_device *)rtnl_dereference(b->media_ptr);
	RCU_INIT_POINTER(b->media_ptr, NULL);
	RCU_INIT_POINTER(dev->tipc_ptr, NULL);
	synchronize_net();
	dev_put(dev);
}

/**
 * tipc_l2_send_msg - send a TIPC packet out over an L2 interface
 * @buf: the packet to be sent
 * @b_ptr: the bearer through which the packet is to be sent
 * @dest: peer destination address
 */
int tipc_l2_send_msg(struct sk_buff *buf, struct tipc_bearer *b,
		     struct tipc_media_addr *dest)
{
	struct sk_buff *clone;
	struct net_device *dev;
	int delta;

	dev = (struct net_device *)rcu_dereference_rtnl(b->media_ptr);
	if (!dev)
		return 0;

	clone = skb_clone(buf, GFP_ATOMIC);
	if (!clone)
		return 0;

	delta = dev->hard_header_len - skb_headroom(buf);
	if ((delta > 0) &&
	    pskb_expand_head(clone, SKB_DATA_ALIGN(delta), 0, GFP_ATOMIC)) {
		kfree_skb(clone);
		return 0;
	}

	skb_reset_network_header(clone);
	clone->dev = dev;
	clone->protocol = htons(ETH_P_TIPC);
	dev_hard_header(clone, dev, ETH_P_TIPC, dest->value,
			dev->dev_addr, clone->len);
	dev_queue_xmit(clone);
	return 0;
}

/* tipc_bearer_send- sends buffer to destination over bearer
 *
 * IMPORTANT:
 * The media send routine must not alter the buffer being passed in
 * as it may be needed for later retransmission!
 */
void tipc_bearer_send(u32 bearer_id, struct sk_buff *buf,
		      struct tipc_media_addr *dest)
{
	struct tipc_bearer *b_ptr;

	rcu_read_lock();
	b_ptr = rcu_dereference_rtnl(bearer_list[bearer_id]);
	if (likely(b_ptr))
		b_ptr->media->send_msg(buf, b_ptr, dest);
	rcu_read_unlock();
}

/**
 * tipc_l2_rcv_msg - handle incoming TIPC message from an interface
 * @buf: the received packet
 * @dev: the net device that the packet was received on
 * @pt: the packet_type structure which was used to register this handler
 * @orig_dev: the original receive net device in case the device is a bond
 *
 * Accept only packets explicitly sent to this node, or broadcast packets;
 * ignores packets sent using interface multicast, and traffic sent to other
 * nodes (which can happen if interface is running in promiscuous mode).
 */
static int tipc_l2_rcv_msg(struct sk_buff *buf, struct net_device *dev,
			   struct packet_type *pt, struct net_device *orig_dev)
{
	struct tipc_bearer *b_ptr;

	if (!net_eq(dev_net(dev), &init_net)) {
		kfree_skb(buf);
		return NET_RX_DROP;
	}

	rcu_read_lock();
	b_ptr = rcu_dereference_rtnl(dev->tipc_ptr);
	if (likely(b_ptr)) {
		if (likely(buf->pkt_type <= PACKET_BROADCAST)) {
			buf->next = NULL;
			tipc_rcv(buf, b_ptr);
			rcu_read_unlock();
			return NET_RX_SUCCESS;
		}
	}
	rcu_read_unlock();

	kfree_skb(buf);
	return NET_RX_DROP;
}

/**
 * tipc_l2_device_event - handle device events from network device
 * @nb: the context of the notification
 * @evt: the type of event
 * @ptr: the net device that the event was on
 *
 * This function is called by the Ethernet driver in case of link
 * change event.
 */
static int tipc_l2_device_event(struct notifier_block *nb, unsigned long evt,
				void *ptr)
{
	struct tipc_bearer *b_ptr;
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);

	if (!net_eq(dev_net(dev), &init_net))
		return NOTIFY_DONE;

	b_ptr = rtnl_dereference(dev->tipc_ptr);
	if (!b_ptr)
		return NOTIFY_DONE;

	b_ptr->mtu = dev->mtu;

	switch (evt) {
	case NETDEV_CHANGE:
		if (netif_carrier_ok(dev))
			break;
	case NETDEV_DOWN:
	case NETDEV_CHANGEMTU:
		tipc_reset_bearer(b_ptr);
		break;
	case NETDEV_CHANGEADDR:
		b_ptr->media->raw2addr(b_ptr, &b_ptr->addr,
				       (char *)dev->dev_addr);
		tipc_reset_bearer(b_ptr);
		break;
	case NETDEV_UNREGISTER:
	case NETDEV_CHANGENAME:
		bearer_disable(b_ptr, false);
		break;
	}
	return NOTIFY_OK;
}

static struct packet_type tipc_packet_type __read_mostly = {
	.type = htons(ETH_P_TIPC),
	.func = tipc_l2_rcv_msg,
};

static struct notifier_block notifier = {
	.notifier_call  = tipc_l2_device_event,
	.priority	= 0,
};

int tipc_bearer_setup(void)
{
	int err;

	err = register_netdevice_notifier(&notifier);
	if (err)
		return err;
	dev_add_pack(&tipc_packet_type);
	return 0;
}

void tipc_bearer_cleanup(void)
{
	unregister_netdevice_notifier(&notifier);
	dev_remove_pack(&tipc_packet_type);
}

void tipc_bearer_stop(void)
{
	struct tipc_bearer *b_ptr;
	u32 i;

	for (i = 0; i < MAX_BEARERS; i++) {
		b_ptr = rtnl_dereference(bearer_list[i]);
		if (b_ptr) {
			bearer_disable(b_ptr, true);
			bearer_list[i] = NULL;
		}
	}
>>>>>>> v3.18
}
