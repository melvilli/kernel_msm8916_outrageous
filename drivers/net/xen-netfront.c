/*
 * Virtual network driver for conversing with remote driver backends.
 *
 * Copyright (c) 2002-2005, K A Fraser
 * Copyright (c) 2005, XenSource Ltd
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation; or, when distributed
 * separately from the Linux kernel or incorporated into other
 * software packages, subject to the following license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this source file (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

<<<<<<< HEAD
<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> v3.18
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> v3.18
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/ethtool.h>
#include <linux/if_ether.h>
#include <net/tcp.h>
#include <linux/udp.h>
#include <linux/moduleparam.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <net/ip.h>

#include <asm/xen/page.h>
#include <xen/xen.h>
#include <xen/xenbus.h>
#include <xen/events.h>
#include <xen/page.h>
#include <xen/platform_pci.h>
#include <xen/grant_table.h>

#include <xen/interface/io/netif.h>
#include <xen/interface/memory.h>
#include <xen/interface/grant_table.h>

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
/* Module parameters */
static unsigned int xennet_max_queues;
module_param_named(max_queues, xennet_max_queues, uint, 0644);
MODULE_PARM_DESC(max_queues,
		 "Maximum number of queues per virtual interface");

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static const struct ethtool_ops xennet_ethtool_ops;

struct netfront_cb {
	int pull_to;
};

#define NETFRONT_SKB_CB(skb)	((struct netfront_cb *)((skb)->cb))

#define RX_COPY_THRESHOLD 256

#define GRANT_INVALID_REF	0

#define NET_TX_RING_SIZE __CONST_RING_SIZE(xen_netif_tx, PAGE_SIZE)
#define NET_RX_RING_SIZE __CONST_RING_SIZE(xen_netif_rx, PAGE_SIZE)
#define TX_MAX_TARGET min_t(int, NET_TX_RING_SIZE, 256)

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
/* Queue name is interface name with "-qNNN" appended */
#define QUEUE_NAME_SIZE (IFNAMSIZ + 6)

/* IRQ name is queue name with "-tx" or "-rx" appended */
#define IRQ_NAME_SIZE (QUEUE_NAME_SIZE + 3)

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
struct netfront_stats {
	u64			rx_packets;
	u64			tx_packets;
	u64			rx_bytes;
	u64			tx_bytes;
	struct u64_stats_sync	syncp;
};

<<<<<<< HEAD
<<<<<<< HEAD
struct netfront_info {
	struct list_head list;
	struct net_device *netdev;

	struct napi_struct napi;

	unsigned int evtchn;
	struct xenbus_device *xbdev;
=======
=======
>>>>>>> v3.18
struct netfront_info;

struct netfront_queue {
	unsigned int id; /* Queue ID, 0-based */
	char name[QUEUE_NAME_SIZE]; /* DEVNAME-qN */
	struct netfront_info *info;

	struct napi_struct napi;

	/* Split event channels support, tx_* == rx_* when using
	 * single event channel.
	 */
	unsigned int tx_evtchn, rx_evtchn;
	unsigned int tx_irq, rx_irq;
	/* Only used when split event channels support is enabled */
	char tx_irq_name[IRQ_NAME_SIZE]; /* DEVNAME-qN-tx */
	char rx_irq_name[IRQ_NAME_SIZE]; /* DEVNAME-qN-rx */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	spinlock_t   tx_lock;
	struct xen_netif_tx_front_ring tx;
	int tx_ring_ref;

	/*
	 * {tx,rx}_skbs store outstanding skbuffs. Free tx_skb entries
	 * are linked from tx_skb_freelist through skb_entry.link.
	 *
	 *  NB. Freelist index entries are always going to be less than
	 *  PAGE_OFFSET, whereas pointers to skbs will always be equal or
	 *  greater than PAGE_OFFSET: we use this property to distinguish
	 *  them.
	 */
	union skb_entry {
		struct sk_buff *skb;
		unsigned long link;
	} tx_skbs[NET_TX_RING_SIZE];
	grant_ref_t gref_tx_head;
	grant_ref_t grant_tx_ref[NET_TX_RING_SIZE];
	struct page *grant_tx_page[NET_TX_RING_SIZE];
	unsigned tx_skb_freelist;

	spinlock_t   rx_lock ____cacheline_aligned_in_smp;
	struct xen_netif_rx_front_ring rx;
	int rx_ring_ref;

	/* Receive-ring batched refills. */
#define RX_MIN_TARGET 8
#define RX_DFL_MIN_TARGET 64
#define RX_MAX_TARGET min_t(int, NET_RX_RING_SIZE, 256)
	unsigned rx_min_target, rx_max_target, rx_target;
	struct sk_buff_head rx_batch;

	struct timer_list rx_refill_timer;

	struct sk_buff *rx_skbs[NET_RX_RING_SIZE];
	grant_ref_t gref_rx_head;
	grant_ref_t grant_rx_ref[NET_RX_RING_SIZE];

	unsigned long rx_pfn_array[NET_RX_RING_SIZE];
	struct multicall_entry rx_mcl[NET_RX_RING_SIZE+1];
	struct mmu_update rx_mmu[NET_RX_RING_SIZE];
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
};

struct netfront_info {
	struct list_head list;
	struct net_device *netdev;

	struct xenbus_device *xbdev;

	/* Multi-queue support */
	struct netfront_queue *queues;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* Statistics */
	struct netfront_stats __percpu *stats;

<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long rx_gso_checksum_fixup;
=======
	atomic_t rx_gso_checksum_fixup;
>>>>>>> v3.18
=======
	atomic_t rx_gso_checksum_fixup;
>>>>>>> v3.18
};

struct netfront_rx_info {
	struct xen_netif_rx_response rx;
	struct xen_netif_extra_info extras[XEN_NETIF_EXTRA_TYPE_MAX - 1];
};

static void skb_entry_set_link(union skb_entry *list, unsigned short id)
{
	list->link = id;
}

static int skb_entry_is_link(const union skb_entry *list)
{
	BUILD_BUG_ON(sizeof(list->skb) != sizeof(list->link));
	return (unsigned long)list->skb < PAGE_OFFSET;
}

/*
 * Access macros for acquiring freeing slots in tx_skbs[].
 */

static void add_id_to_freelist(unsigned *head, union skb_entry *list,
			       unsigned short id)
{
	skb_entry_set_link(&list[id], *head);
	*head = id;
}

static unsigned short get_id_from_freelist(unsigned *head,
					   union skb_entry *list)
{
	unsigned int id = *head;
	*head = list[id].link;
	return id;
}

static int xennet_rxidx(RING_IDX idx)
{
	return idx & (NET_RX_RING_SIZE - 1);
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct sk_buff *xennet_get_rx_skb(struct netfront_info *np,
					 RING_IDX ri)
{
	int i = xennet_rxidx(ri);
	struct sk_buff *skb = np->rx_skbs[i];
	np->rx_skbs[i] = NULL;
	return skb;
}

static grant_ref_t xennet_get_rx_ref(struct netfront_info *np,
					    RING_IDX ri)
{
	int i = xennet_rxidx(ri);
	grant_ref_t ref = np->grant_rx_ref[i];
	np->grant_rx_ref[i] = GRANT_INVALID_REF;
=======
=======
>>>>>>> v3.18
static struct sk_buff *xennet_get_rx_skb(struct netfront_queue *queue,
					 RING_IDX ri)
{
	int i = xennet_rxidx(ri);
	struct sk_buff *skb = queue->rx_skbs[i];
	queue->rx_skbs[i] = NULL;
	return skb;
}

static grant_ref_t xennet_get_rx_ref(struct netfront_queue *queue,
					    RING_IDX ri)
{
	int i = xennet_rxidx(ri);
	grant_ref_t ref = queue->grant_rx_ref[i];
	queue->grant_rx_ref[i] = GRANT_INVALID_REF;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return ref;
}

#ifdef CONFIG_SYSFS
static int xennet_sysfs_addif(struct net_device *netdev);
static void xennet_sysfs_delif(struct net_device *netdev);
#else /* !CONFIG_SYSFS */
#define xennet_sysfs_addif(dev) (0)
#define xennet_sysfs_delif(dev) do { } while (0)
#endif

static bool xennet_can_sg(struct net_device *dev)
{
	return dev->features & NETIF_F_SG;
}


static void rx_refill_timeout(unsigned long data)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct net_device *dev = (struct net_device *)data;
	struct netfront_info *np = netdev_priv(dev);
	napi_schedule(&np->napi);
}

static int netfront_tx_slot_available(struct netfront_info *np)
{
	return (np->tx.req_prod_pvt - np->tx.rsp_cons) <
		(TX_MAX_TARGET - MAX_SKB_FRAGS - 2);
}

static void xennet_maybe_wake_tx(struct net_device *dev)
{
	struct netfront_info *np = netdev_priv(dev);

	if (unlikely(netif_queue_stopped(dev)) &&
	    netfront_tx_slot_available(np) &&
	    likely(netif_running(dev)))
		netif_wake_queue(dev);
}

static void xennet_alloc_rx_buffers(struct net_device *dev)
{
	unsigned short id;
	struct netfront_info *np = netdev_priv(dev);
	struct sk_buff *skb;
	struct page *page;
	int i, batch_target, notify;
	RING_IDX req_prod = np->rx.req_prod_pvt;
=======
=======
>>>>>>> v3.18
	struct netfront_queue *queue = (struct netfront_queue *)data;
	napi_schedule(&queue->napi);
}

static int netfront_tx_slot_available(struct netfront_queue *queue)
{
	return (queue->tx.req_prod_pvt - queue->tx.rsp_cons) <
		(TX_MAX_TARGET - MAX_SKB_FRAGS - 2);
}

static void xennet_maybe_wake_tx(struct netfront_queue *queue)
{
	struct net_device *dev = queue->info->netdev;
	struct netdev_queue *dev_queue = netdev_get_tx_queue(dev, queue->id);

	if (unlikely(netif_tx_queue_stopped(dev_queue)) &&
	    netfront_tx_slot_available(queue) &&
	    likely(netif_running(dev)))
		netif_tx_wake_queue(netdev_get_tx_queue(dev, queue->id));
}

static void xennet_alloc_rx_buffers(struct netfront_queue *queue)
{
	unsigned short id;
	struct sk_buff *skb;
	struct page *page;
	int i, batch_target, notify;
	RING_IDX req_prod = queue->rx.req_prod_pvt;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	grant_ref_t ref;
	unsigned long pfn;
	void *vaddr;
	struct xen_netif_rx_request *req;

<<<<<<< HEAD
<<<<<<< HEAD
	if (unlikely(!netif_carrier_ok(dev)))
=======
	if (unlikely(!netif_carrier_ok(queue->info->netdev)))
>>>>>>> v3.18
=======
	if (unlikely(!netif_carrier_ok(queue->info->netdev)))
>>>>>>> v3.18
		return;

	/*
	 * Allocate skbuffs greedily, even though we batch updates to the
	 * receive ring. This creates a less bursty demand on the memory
	 * allocator, so should reduce the chance of failed allocation requests
	 * both for ourself and for other kernel subsystems.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	batch_target = np->rx_target - (req_prod - np->rx.rsp_cons);
	for (i = skb_queue_len(&np->rx_batch); i < batch_target; i++) {
		skb = __netdev_alloc_skb(dev, RX_COPY_THRESHOLD + NET_IP_ALIGN,
=======
=======
>>>>>>> v3.18
	batch_target = queue->rx_target - (req_prod - queue->rx.rsp_cons);
	for (i = skb_queue_len(&queue->rx_batch); i < batch_target; i++) {
		skb = __netdev_alloc_skb(queue->info->netdev,
					 RX_COPY_THRESHOLD + NET_IP_ALIGN,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
					 GFP_ATOMIC | __GFP_NOWARN);
		if (unlikely(!skb))
			goto no_skb;

		/* Align ip header to a 16 bytes boundary */
		skb_reserve(skb, NET_IP_ALIGN);

		page = alloc_page(GFP_ATOMIC | __GFP_NOWARN);
		if (!page) {
			kfree_skb(skb);
no_skb:
<<<<<<< HEAD
<<<<<<< HEAD
			/* Any skbuffs queued for refill? Force them out. */
			if (i != 0)
				goto refill;
			/* Could not allocate any skbuffs. Try again later. */
			mod_timer(&np->rx_refill_timer,
				  jiffies + (HZ/10));
=======
=======
>>>>>>> v3.18
			/* Could not allocate any skbuffs. Try again later. */
			mod_timer(&queue->rx_refill_timer,
				  jiffies + (HZ/10));

			/* Any skbuffs queued for refill? Force them out. */
			if (i != 0)
				goto refill;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			break;
		}

		skb_add_rx_frag(skb, 0, page, 0, 0, PAGE_SIZE);
<<<<<<< HEAD
<<<<<<< HEAD
		__skb_queue_tail(&np->rx_batch, skb);
	}

	/* Is the batch large enough to be worthwhile? */
	if (i < (np->rx_target/2)) {
		if (req_prod > np->rx.sring->req_prod)
=======
=======
>>>>>>> v3.18
		__skb_queue_tail(&queue->rx_batch, skb);
	}

	/* Is the batch large enough to be worthwhile? */
	if (i < (queue->rx_target/2)) {
		if (req_prod > queue->rx.sring->req_prod)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			goto push;
		return;
	}

	/* Adjust our fill target if we risked running out of buffers. */
<<<<<<< HEAD
<<<<<<< HEAD
	if (((req_prod - np->rx.sring->rsp_prod) < (np->rx_target / 4)) &&
	    ((np->rx_target *= 2) > np->rx_max_target))
		np->rx_target = np->rx_max_target;

 refill:
	for (i = 0; ; i++) {
		skb = __skb_dequeue(&np->rx_batch);
		if (skb == NULL)
			break;

		skb->dev = dev;

		id = xennet_rxidx(req_prod + i);

		BUG_ON(np->rx_skbs[id]);
		np->rx_skbs[id] = skb;

		ref = gnttab_claim_grant_reference(&np->gref_rx_head);
		BUG_ON((signed short)ref < 0);
		np->grant_rx_ref[id] = ref;
=======
=======
>>>>>>> v3.18
	if (((req_prod - queue->rx.sring->rsp_prod) < (queue->rx_target / 4)) &&
	    ((queue->rx_target *= 2) > queue->rx_max_target))
		queue->rx_target = queue->rx_max_target;

 refill:
	for (i = 0; ; i++) {
		skb = __skb_dequeue(&queue->rx_batch);
		if (skb == NULL)
			break;

		skb->dev = queue->info->netdev;

		id = xennet_rxidx(req_prod + i);

		BUG_ON(queue->rx_skbs[id]);
		queue->rx_skbs[id] = skb;

		ref = gnttab_claim_grant_reference(&queue->gref_rx_head);
		BUG_ON((signed short)ref < 0);
		queue->grant_rx_ref[id] = ref;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

		pfn = page_to_pfn(skb_frag_page(&skb_shinfo(skb)->frags[0]));
		vaddr = page_address(skb_frag_page(&skb_shinfo(skb)->frags[0]));

<<<<<<< HEAD
<<<<<<< HEAD
		req = RING_GET_REQUEST(&np->rx, req_prod + i);
		gnttab_grant_foreign_access_ref(ref,
						np->xbdev->otherend_id,
=======
		req = RING_GET_REQUEST(&queue->rx, req_prod + i);
		gnttab_grant_foreign_access_ref(ref,
						queue->info->xbdev->otherend_id,
>>>>>>> v3.18
=======
		req = RING_GET_REQUEST(&queue->rx, req_prod + i);
		gnttab_grant_foreign_access_ref(ref,
						queue->info->xbdev->otherend_id,
>>>>>>> v3.18
						pfn_to_mfn(pfn),
						0);

		req->id = id;
		req->gref = ref;
	}

	wmb();		/* barrier so backend seens requests */

	/* Above is a suitable barrier to ensure backend will see requests. */
<<<<<<< HEAD
<<<<<<< HEAD
	np->rx.req_prod_pvt = req_prod + i;
 push:
	RING_PUSH_REQUESTS_AND_CHECK_NOTIFY(&np->rx, notify);
	if (notify)
		notify_remote_via_irq(np->netdev->irq);
=======
=======
>>>>>>> v3.18
	queue->rx.req_prod_pvt = req_prod + i;
 push:
	RING_PUSH_REQUESTS_AND_CHECK_NOTIFY(&queue->rx, notify);
	if (notify)
		notify_remote_via_irq(queue->rx_irq);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static int xennet_open(struct net_device *dev)
{
	struct netfront_info *np = netdev_priv(dev);
<<<<<<< HEAD
<<<<<<< HEAD

	napi_enable(&np->napi);

	spin_lock_bh(&np->rx_lock);
	if (netif_carrier_ok(dev)) {
		xennet_alloc_rx_buffers(dev);
		np->rx.sring->rsp_event = np->rx.rsp_cons + 1;
		if (RING_HAS_UNCONSUMED_RESPONSES(&np->rx))
			napi_schedule(&np->napi);
	}
	spin_unlock_bh(&np->rx_lock);

	netif_start_queue(dev);
=======
=======
>>>>>>> v3.18
	unsigned int num_queues = dev->real_num_tx_queues;
	unsigned int i = 0;
	struct netfront_queue *queue = NULL;

	for (i = 0; i < num_queues; ++i) {
		queue = &np->queues[i];
		napi_enable(&queue->napi);

		spin_lock_bh(&queue->rx_lock);
		if (netif_carrier_ok(dev)) {
			xennet_alloc_rx_buffers(queue);
			queue->rx.sring->rsp_event = queue->rx.rsp_cons + 1;
			if (RING_HAS_UNCONSUMED_RESPONSES(&queue->rx))
				napi_schedule(&queue->napi);
		}
		spin_unlock_bh(&queue->rx_lock);
	}

	netif_tx_start_all_queues(dev);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void xennet_tx_buf_gc(struct net_device *dev)
{
	RING_IDX cons, prod;
	unsigned short id;
	struct netfront_info *np = netdev_priv(dev);
	struct sk_buff *skb;

	BUG_ON(!netif_carrier_ok(dev));

	do {
		prod = np->tx.sring->rsp_prod;
		rmb(); /* Ensure we see responses up to 'rp'. */

		for (cons = np->tx.rsp_cons; cons != prod; cons++) {
			struct xen_netif_tx_response *txrsp;

			txrsp = RING_GET_RESPONSE(&np->tx, cons);
=======
=======
>>>>>>> v3.18
static void xennet_tx_buf_gc(struct netfront_queue *queue)
{
	RING_IDX cons, prod;
	unsigned short id;
	struct sk_buff *skb;

	BUG_ON(!netif_carrier_ok(queue->info->netdev));

	do {
		prod = queue->tx.sring->rsp_prod;
		rmb(); /* Ensure we see responses up to 'rp'. */

		for (cons = queue->tx.rsp_cons; cons != prod; cons++) {
			struct xen_netif_tx_response *txrsp;

			txrsp = RING_GET_RESPONSE(&queue->tx, cons);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			if (txrsp->status == XEN_NETIF_RSP_NULL)
				continue;

			id  = txrsp->id;
<<<<<<< HEAD
<<<<<<< HEAD
			skb = np->tx_skbs[id].skb;
			if (unlikely(gnttab_query_foreign_access(
				np->grant_tx_ref[id]) != 0)) {
				printk(KERN_ALERT "xennet_tx_buf_gc: warning "
				       "-- grant still in use by backend "
				       "domain.\n");
				BUG();
			}
			gnttab_end_foreign_access_ref(
				np->grant_tx_ref[id], GNTMAP_readonly);
			gnttab_release_grant_reference(
				&np->gref_tx_head, np->grant_tx_ref[id]);
			np->grant_tx_ref[id] = GRANT_INVALID_REF;
			np->grant_tx_page[id] = NULL;
			add_id_to_freelist(&np->tx_skb_freelist, np->tx_skbs, id);
			dev_kfree_skb_irq(skb);
		}

		np->tx.rsp_cons = prod;
=======
=======
>>>>>>> v3.18
			skb = queue->tx_skbs[id].skb;
			if (unlikely(gnttab_query_foreign_access(
				queue->grant_tx_ref[id]) != 0)) {
				pr_alert("%s: warning -- grant still in use by backend domain\n",
					 __func__);
				BUG();
			}
			gnttab_end_foreign_access_ref(
				queue->grant_tx_ref[id], GNTMAP_readonly);
			gnttab_release_grant_reference(
				&queue->gref_tx_head, queue->grant_tx_ref[id]);
			queue->grant_tx_ref[id] = GRANT_INVALID_REF;
			queue->grant_tx_page[id] = NULL;
			add_id_to_freelist(&queue->tx_skb_freelist, queue->tx_skbs, id);
			dev_kfree_skb_irq(skb);
		}

		queue->tx.rsp_cons = prod;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

		/*
		 * Set a new event, then check for race with update of tx_cons.
		 * Note that it is essential to schedule a callback, no matter
		 * how few buffers are pending. Even if there is space in the
		 * transmit ring, higher layers may be blocked because too much
		 * data is outstanding: in such cases notification from Xen is
		 * likely to be the only kick that we'll get.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		np->tx.sring->rsp_event =
			prod + ((np->tx.sring->req_prod - prod) >> 1) + 1;
		mb();		/* update shared area */
	} while ((cons == prod) && (prod != np->tx.sring->rsp_prod));

	xennet_maybe_wake_tx(dev);
}

static void xennet_make_frags(struct sk_buff *skb, struct net_device *dev,
			      struct xen_netif_tx_request *tx)
{
	struct netfront_info *np = netdev_priv(dev);
	char *data = skb->data;
	unsigned long mfn;
	RING_IDX prod = np->tx.req_prod_pvt;
=======
=======
>>>>>>> v3.18
		queue->tx.sring->rsp_event =
			prod + ((queue->tx.sring->req_prod - prod) >> 1) + 1;
		mb();		/* update shared area */
	} while ((cons == prod) && (prod != queue->tx.sring->rsp_prod));

	xennet_maybe_wake_tx(queue);
}

static void xennet_make_frags(struct sk_buff *skb, struct netfront_queue *queue,
			      struct xen_netif_tx_request *tx)
{
	char *data = skb->data;
	unsigned long mfn;
	RING_IDX prod = queue->tx.req_prod_pvt;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	int frags = skb_shinfo(skb)->nr_frags;
	unsigned int offset = offset_in_page(data);
	unsigned int len = skb_headlen(skb);
	unsigned int id;
	grant_ref_t ref;
	int i;

	/* While the header overlaps a page boundary (including being
	   larger than a page), split it it into page-sized chunks. */
	while (len > PAGE_SIZE - offset) {
		tx->size = PAGE_SIZE - offset;
		tx->flags |= XEN_NETTXF_more_data;
		len -= tx->size;
		data += tx->size;
		offset = 0;

<<<<<<< HEAD
<<<<<<< HEAD
		id = get_id_from_freelist(&np->tx_skb_freelist, np->tx_skbs);
		np->tx_skbs[id].skb = skb_get(skb);
		tx = RING_GET_REQUEST(&np->tx, prod++);
		tx->id = id;
		ref = gnttab_claim_grant_reference(&np->gref_tx_head);
		BUG_ON((signed short)ref < 0);

		mfn = virt_to_mfn(data);
		gnttab_grant_foreign_access_ref(ref, np->xbdev->otherend_id,
						mfn, GNTMAP_readonly);

		np->grant_tx_page[id] = virt_to_page(data);
		tx->gref = np->grant_tx_ref[id] = ref;
=======
=======
>>>>>>> v3.18
		id = get_id_from_freelist(&queue->tx_skb_freelist, queue->tx_skbs);
		queue->tx_skbs[id].skb = skb_get(skb);
		tx = RING_GET_REQUEST(&queue->tx, prod++);
		tx->id = id;
		ref = gnttab_claim_grant_reference(&queue->gref_tx_head);
		BUG_ON((signed short)ref < 0);

		mfn = virt_to_mfn(data);
		gnttab_grant_foreign_access_ref(ref, queue->info->xbdev->otherend_id,
						mfn, GNTMAP_readonly);

		queue->grant_tx_page[id] = virt_to_page(data);
		tx->gref = queue->grant_tx_ref[id] = ref;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		tx->offset = offset;
		tx->size = len;
		tx->flags = 0;
	}

	/* Grant backend access to each skb fragment page. */
	for (i = 0; i < frags; i++) {
		skb_frag_t *frag = skb_shinfo(skb)->frags + i;
		struct page *page = skb_frag_page(frag);

		len = skb_frag_size(frag);
		offset = frag->page_offset;

		/* Skip unused frames from start of page */
		page += offset >> PAGE_SHIFT;
		offset &= ~PAGE_MASK;

		while (len > 0) {
			unsigned long bytes;

			bytes = PAGE_SIZE - offset;
			if (bytes > len)
				bytes = len;

			tx->flags |= XEN_NETTXF_more_data;

<<<<<<< HEAD
<<<<<<< HEAD
			id = get_id_from_freelist(&np->tx_skb_freelist,
						  np->tx_skbs);
			np->tx_skbs[id].skb = skb_get(skb);
			tx = RING_GET_REQUEST(&np->tx, prod++);
			tx->id = id;
			ref = gnttab_claim_grant_reference(&np->gref_tx_head);
=======
=======
>>>>>>> v3.18
			id = get_id_from_freelist(&queue->tx_skb_freelist,
						  queue->tx_skbs);
			queue->tx_skbs[id].skb = skb_get(skb);
			tx = RING_GET_REQUEST(&queue->tx, prod++);
			tx->id = id;
			ref = gnttab_claim_grant_reference(&queue->gref_tx_head);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			BUG_ON((signed short)ref < 0);

			mfn = pfn_to_mfn(page_to_pfn(page));
			gnttab_grant_foreign_access_ref(ref,
<<<<<<< HEAD
<<<<<<< HEAD
							np->xbdev->otherend_id,
							mfn, GNTMAP_readonly);

			np->grant_tx_page[id] = page;
			tx->gref = np->grant_tx_ref[id] = ref;
=======
=======
>>>>>>> v3.18
							queue->info->xbdev->otherend_id,
							mfn, GNTMAP_readonly);

			queue->grant_tx_page[id] = page;
			tx->gref = queue->grant_tx_ref[id] = ref;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			tx->offset = offset;
			tx->size = bytes;
			tx->flags = 0;

			offset += bytes;
			len -= bytes;

			/* Next frame */
			if (offset == PAGE_SIZE && len) {
				BUG_ON(!PageCompound(page));
				page++;
				offset = 0;
			}
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	np->tx.req_prod_pvt = prod;
=======
	queue->tx.req_prod_pvt = prod;
>>>>>>> v3.18
=======
	queue->tx.req_prod_pvt = prod;
>>>>>>> v3.18
}

/*
 * Count how many ring slots are required to send the frags of this
 * skb. Each frag might be a compound page.
 */
static int xennet_count_skb_frag_slots(struct sk_buff *skb)
{
	int i, frags = skb_shinfo(skb)->nr_frags;
	int pages = 0;

	for (i = 0; i < frags; i++) {
		skb_frag_t *frag = skb_shinfo(skb)->frags + i;
		unsigned long size = skb_frag_size(frag);
		unsigned long offset = frag->page_offset;

		/* Skip unused frames from start of page */
		offset &= ~PAGE_MASK;

		pages += PFN_UP(offset + size);
	}

	return pages;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
static u16 xennet_select_queue(struct net_device *dev, struct sk_buff *skb,
			       void *accel_priv, select_queue_fallback_t fallback)
{
	unsigned int num_queues = dev->real_num_tx_queues;
	u32 hash;
	u16 queue_idx;

	/* First, check if there is only one queue */
	if (num_queues == 1) {
		queue_idx = 0;
	} else {
		hash = skb_get_hash(skb);
		queue_idx = hash % num_queues;
	}

	return queue_idx;
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static int xennet_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	unsigned short id;
	struct netfront_info *np = netdev_priv(dev);
	struct netfront_stats *stats = this_cpu_ptr(np->stats);
	struct xen_netif_tx_request *tx;
	char *data = skb->data;
	RING_IDX i;
	grant_ref_t ref;
	unsigned long mfn;
	int notify;
	int slots;
	unsigned int offset = offset_in_page(data);
	unsigned int len = skb_headlen(skb);
	unsigned long flags;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	struct netfront_queue *queue = NULL;
	unsigned int num_queues = dev->real_num_tx_queues;
	u16 queue_index;

	/* Drop the packet if no queues are set up */
	if (num_queues < 1)
		goto drop;
	/* Determine which queue to transmit this SKB on */
	queue_index = skb_get_queue_mapping(skb);
	queue = &np->queues[queue_index];
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* If skb->len is too big for wire format, drop skb and alert
	 * user about misconfiguration.
	 */
	if (unlikely(skb->len > XEN_NETIF_MAX_TX_SIZE)) {
		net_alert_ratelimited(
			"xennet: skb->len = %u, too big for wire format\n",
			skb->len);
		goto drop;
	}

	slots = DIV_ROUND_UP(offset + len, PAGE_SIZE) +
		xennet_count_skb_frag_slots(skb);
	if (unlikely(slots > MAX_SKB_FRAGS + 1)) {
<<<<<<< HEAD
<<<<<<< HEAD
		net_alert_ratelimited(
			"xennet: skb rides the rocket: %d slots\n", slots);
		goto drop;
	}

	spin_lock_irqsave(&np->tx_lock, flags);

	if (unlikely(!netif_carrier_ok(dev) ||
		     (slots > 1 && !xennet_can_sg(dev)) ||
		     netif_needs_gso(skb, netif_skb_features(skb)))) {
		spin_unlock_irqrestore(&np->tx_lock, flags);
		goto drop;
	}

	i = np->tx.req_prod_pvt;

	id = get_id_from_freelist(&np->tx_skb_freelist, np->tx_skbs);
	np->tx_skbs[id].skb = skb;

	tx = RING_GET_REQUEST(&np->tx, i);

	tx->id   = id;
	ref = gnttab_claim_grant_reference(&np->gref_tx_head);
	BUG_ON((signed short)ref < 0);
	mfn = virt_to_mfn(data);
	gnttab_grant_foreign_access_ref(
		ref, np->xbdev->otherend_id, mfn, GNTMAP_readonly);
	np->grant_tx_page[id] = virt_to_page(data);
	tx->gref = np->grant_tx_ref[id] = ref;
=======
=======
>>>>>>> v3.18
		net_dbg_ratelimited("xennet: skb rides the rocket: %d slots, %d bytes\n",
				    slots, skb->len);
		if (skb_linearize(skb))
			goto drop;
	}

	spin_lock_irqsave(&queue->tx_lock, flags);

	if (unlikely(!netif_carrier_ok(dev) ||
		     (slots > 1 && !xennet_can_sg(dev)) ||
		     netif_needs_gso(dev, skb, netif_skb_features(skb)))) {
		spin_unlock_irqrestore(&queue->tx_lock, flags);
		goto drop;
	}

	i = queue->tx.req_prod_pvt;

	id = get_id_from_freelist(&queue->tx_skb_freelist, queue->tx_skbs);
	queue->tx_skbs[id].skb = skb;

	tx = RING_GET_REQUEST(&queue->tx, i);

	tx->id   = id;
	ref = gnttab_claim_grant_reference(&queue->gref_tx_head);
	BUG_ON((signed short)ref < 0);
	mfn = virt_to_mfn(data);
	gnttab_grant_foreign_access_ref(
		ref, queue->info->xbdev->otherend_id, mfn, GNTMAP_readonly);
	queue->grant_tx_page[id] = virt_to_page(data);
	tx->gref = queue->grant_tx_ref[id] = ref;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	tx->offset = offset;
	tx->size = len;

	tx->flags = 0;
	if (skb->ip_summed == CHECKSUM_PARTIAL)
		/* local packet? */
		tx->flags |= XEN_NETTXF_csum_blank | XEN_NETTXF_data_validated;
	else if (skb->ip_summed == CHECKSUM_UNNECESSARY)
		/* remote but checksummed. */
		tx->flags |= XEN_NETTXF_data_validated;

	if (skb_shinfo(skb)->gso_size) {
		struct xen_netif_extra_info *gso;

		gso = (struct xen_netif_extra_info *)
<<<<<<< HEAD
<<<<<<< HEAD
			RING_GET_REQUEST(&np->tx, ++i);
=======
			RING_GET_REQUEST(&queue->tx, ++i);
>>>>>>> v3.18
=======
			RING_GET_REQUEST(&queue->tx, ++i);
>>>>>>> v3.18

		tx->flags |= XEN_NETTXF_extra_info;

		gso->u.gso.size = skb_shinfo(skb)->gso_size;
<<<<<<< HEAD
<<<<<<< HEAD
		gso->u.gso.type = XEN_NETIF_GSO_TYPE_TCPV4;
=======
		gso->u.gso.type = (skb_shinfo(skb)->gso_type & SKB_GSO_TCPV6) ?
			XEN_NETIF_GSO_TYPE_TCPV6 :
			XEN_NETIF_GSO_TYPE_TCPV4;
>>>>>>> v3.18
=======
		gso->u.gso.type = (skb_shinfo(skb)->gso_type & SKB_GSO_TCPV6) ?
			XEN_NETIF_GSO_TYPE_TCPV6 :
			XEN_NETIF_GSO_TYPE_TCPV4;
>>>>>>> v3.18
		gso->u.gso.pad = 0;
		gso->u.gso.features = 0;

		gso->type = XEN_NETIF_EXTRA_TYPE_GSO;
		gso->flags = 0;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	np->tx.req_prod_pvt = i + 1;

	xennet_make_frags(skb, dev, tx);
	tx->size = skb->len;

	RING_PUSH_REQUESTS_AND_CHECK_NOTIFY(&np->tx, notify);
	if (notify)
		notify_remote_via_irq(np->netdev->irq);
=======
=======
>>>>>>> v3.18
	queue->tx.req_prod_pvt = i + 1;

	xennet_make_frags(skb, queue, tx);
	tx->size = skb->len;

	RING_PUSH_REQUESTS_AND_CHECK_NOTIFY(&queue->tx, notify);
	if (notify)
		notify_remote_via_irq(queue->tx_irq);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	u64_stats_update_begin(&stats->syncp);
	stats->tx_bytes += skb->len;
	stats->tx_packets++;
	u64_stats_update_end(&stats->syncp);

	/* Note: It is not safe to access skb after xennet_tx_buf_gc()! */
<<<<<<< HEAD
<<<<<<< HEAD
	xennet_tx_buf_gc(dev);

	if (!netfront_tx_slot_available(np))
		netif_stop_queue(dev);

	spin_unlock_irqrestore(&np->tx_lock, flags);
=======
=======
>>>>>>> v3.18
	xennet_tx_buf_gc(queue);

	if (!netfront_tx_slot_available(queue))
		netif_tx_stop_queue(netdev_get_tx_queue(dev, queue->id));

	spin_unlock_irqrestore(&queue->tx_lock, flags);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return NETDEV_TX_OK;

 drop:
	dev->stats.tx_dropped++;
<<<<<<< HEAD
<<<<<<< HEAD
	dev_kfree_skb(skb);
=======
	dev_kfree_skb_any(skb);
>>>>>>> v3.18
=======
	dev_kfree_skb_any(skb);
>>>>>>> v3.18
	return NETDEV_TX_OK;
}

static int xennet_close(struct net_device *dev)
{
	struct netfront_info *np = netdev_priv(dev);
<<<<<<< HEAD
<<<<<<< HEAD
	netif_stop_queue(np->netdev);
	napi_disable(&np->napi);
	return 0;
}

static void xennet_move_rx_slot(struct netfront_info *np, struct sk_buff *skb,
				grant_ref_t ref)
{
	int new = xennet_rxidx(np->rx.req_prod_pvt);

	BUG_ON(np->rx_skbs[new]);
	np->rx_skbs[new] = skb;
	np->grant_rx_ref[new] = ref;
	RING_GET_REQUEST(&np->rx, np->rx.req_prod_pvt)->id = new;
	RING_GET_REQUEST(&np->rx, np->rx.req_prod_pvt)->gref = ref;
	np->rx.req_prod_pvt++;
}

static int xennet_get_extras(struct netfront_info *np,
=======
=======
>>>>>>> v3.18
	unsigned int num_queues = dev->real_num_tx_queues;
	unsigned int i;
	struct netfront_queue *queue;
	netif_tx_stop_all_queues(np->netdev);
	for (i = 0; i < num_queues; ++i) {
		queue = &np->queues[i];
		napi_disable(&queue->napi);
	}
	return 0;
}

static void xennet_move_rx_slot(struct netfront_queue *queue, struct sk_buff *skb,
				grant_ref_t ref)
{
	int new = xennet_rxidx(queue->rx.req_prod_pvt);

	BUG_ON(queue->rx_skbs[new]);
	queue->rx_skbs[new] = skb;
	queue->grant_rx_ref[new] = ref;
	RING_GET_REQUEST(&queue->rx, queue->rx.req_prod_pvt)->id = new;
	RING_GET_REQUEST(&queue->rx, queue->rx.req_prod_pvt)->gref = ref;
	queue->rx.req_prod_pvt++;
}

static int xennet_get_extras(struct netfront_queue *queue,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			     struct xen_netif_extra_info *extras,
			     RING_IDX rp)

{
	struct xen_netif_extra_info *extra;
<<<<<<< HEAD
<<<<<<< HEAD
	struct device *dev = &np->netdev->dev;
	RING_IDX cons = np->rx.rsp_cons;
=======
	struct device *dev = &queue->info->netdev->dev;
	RING_IDX cons = queue->rx.rsp_cons;
>>>>>>> v3.18
=======
	struct device *dev = &queue->info->netdev->dev;
	RING_IDX cons = queue->rx.rsp_cons;
>>>>>>> v3.18
	int err = 0;

	do {
		struct sk_buff *skb;
		grant_ref_t ref;

		if (unlikely(cons + 1 == rp)) {
			if (net_ratelimit())
				dev_warn(dev, "Missing extra info\n");
			err = -EBADR;
			break;
		}

		extra = (struct xen_netif_extra_info *)
<<<<<<< HEAD
<<<<<<< HEAD
			RING_GET_RESPONSE(&np->rx, ++cons);
=======
			RING_GET_RESPONSE(&queue->rx, ++cons);
>>>>>>> v3.18
=======
			RING_GET_RESPONSE(&queue->rx, ++cons);
>>>>>>> v3.18

		if (unlikely(!extra->type ||
			     extra->type >= XEN_NETIF_EXTRA_TYPE_MAX)) {
			if (net_ratelimit())
				dev_warn(dev, "Invalid extra type: %d\n",
					extra->type);
			err = -EINVAL;
		} else {
			memcpy(&extras[extra->type - 1], extra,
			       sizeof(*extra));
		}

<<<<<<< HEAD
<<<<<<< HEAD
		skb = xennet_get_rx_skb(np, cons);
		ref = xennet_get_rx_ref(np, cons);
		xennet_move_rx_slot(np, skb, ref);
	} while (extra->flags & XEN_NETIF_EXTRA_FLAG_MORE);

	np->rx.rsp_cons = cons;
	return err;
}

static int xennet_get_responses(struct netfront_info *np,
=======
=======
>>>>>>> v3.18
		skb = xennet_get_rx_skb(queue, cons);
		ref = xennet_get_rx_ref(queue, cons);
		xennet_move_rx_slot(queue, skb, ref);
	} while (extra->flags & XEN_NETIF_EXTRA_FLAG_MORE);

	queue->rx.rsp_cons = cons;
	return err;
}

static int xennet_get_responses(struct netfront_queue *queue,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
				struct netfront_rx_info *rinfo, RING_IDX rp,
				struct sk_buff_head *list)
{
	struct xen_netif_rx_response *rx = &rinfo->rx;
	struct xen_netif_extra_info *extras = rinfo->extras;
<<<<<<< HEAD
<<<<<<< HEAD
	struct device *dev = &np->netdev->dev;
	RING_IDX cons = np->rx.rsp_cons;
	struct sk_buff *skb = xennet_get_rx_skb(np, cons);
	grant_ref_t ref = xennet_get_rx_ref(np, cons);
=======
=======
>>>>>>> v3.18
	struct device *dev = &queue->info->netdev->dev;
	RING_IDX cons = queue->rx.rsp_cons;
	struct sk_buff *skb = xennet_get_rx_skb(queue, cons);
	grant_ref_t ref = xennet_get_rx_ref(queue, cons);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	int max = MAX_SKB_FRAGS + (rx->status <= RX_COPY_THRESHOLD);
	int slots = 1;
	int err = 0;
	unsigned long ret;

	if (rx->flags & XEN_NETRXF_extra_info) {
<<<<<<< HEAD
<<<<<<< HEAD
		err = xennet_get_extras(np, extras, rp);
		cons = np->rx.rsp_cons;
=======
		err = xennet_get_extras(queue, extras, rp);
		cons = queue->rx.rsp_cons;
>>>>>>> v3.18
=======
		err = xennet_get_extras(queue, extras, rp);
		cons = queue->rx.rsp_cons;
>>>>>>> v3.18
	}

	for (;;) {
		if (unlikely(rx->status < 0 ||
			     rx->offset + rx->status > PAGE_SIZE)) {
			if (net_ratelimit())
				dev_warn(dev, "rx->offset: %x, size: %u\n",
					 rx->offset, rx->status);
<<<<<<< HEAD
<<<<<<< HEAD
			xennet_move_rx_slot(np, skb, ref);
=======
			xennet_move_rx_slot(queue, skb, ref);
>>>>>>> v3.18
=======
			xennet_move_rx_slot(queue, skb, ref);
>>>>>>> v3.18
			err = -EINVAL;
			goto next;
		}

		/*
		 * This definitely indicates a bug, either in this driver or in
		 * the backend driver. In future this should flag the bad
		 * situation to the system controller to reboot the backend.
		 */
		if (ref == GRANT_INVALID_REF) {
			if (net_ratelimit())
				dev_warn(dev, "Bad rx response id %d.\n",
					 rx->id);
			err = -EINVAL;
			goto next;
		}

		ret = gnttab_end_foreign_access_ref(ref, 0);
		BUG_ON(!ret);

<<<<<<< HEAD
<<<<<<< HEAD
		gnttab_release_grant_reference(&np->gref_rx_head, ref);
=======
		gnttab_release_grant_reference(&queue->gref_rx_head, ref);
>>>>>>> v3.18
=======
		gnttab_release_grant_reference(&queue->gref_rx_head, ref);
>>>>>>> v3.18

		__skb_queue_tail(list, skb);

next:
		if (!(rx->flags & XEN_NETRXF_more_data))
			break;

		if (cons + slots == rp) {
			if (net_ratelimit())
				dev_warn(dev, "Need more slots\n");
			err = -ENOENT;
			break;
		}

<<<<<<< HEAD
<<<<<<< HEAD
		rx = RING_GET_RESPONSE(&np->rx, cons + slots);
		skb = xennet_get_rx_skb(np, cons + slots);
		ref = xennet_get_rx_ref(np, cons + slots);
=======
		rx = RING_GET_RESPONSE(&queue->rx, cons + slots);
		skb = xennet_get_rx_skb(queue, cons + slots);
		ref = xennet_get_rx_ref(queue, cons + slots);
>>>>>>> v3.18
=======
		rx = RING_GET_RESPONSE(&queue->rx, cons + slots);
		skb = xennet_get_rx_skb(queue, cons + slots);
		ref = xennet_get_rx_ref(queue, cons + slots);
>>>>>>> v3.18
		slots++;
	}

	if (unlikely(slots > max)) {
		if (net_ratelimit())
			dev_warn(dev, "Too many slots\n");
		err = -E2BIG;
	}

	if (unlikely(err))
<<<<<<< HEAD
<<<<<<< HEAD
		np->rx.rsp_cons = cons + slots;
=======
		queue->rx.rsp_cons = cons + slots;
>>>>>>> v3.18
=======
		queue->rx.rsp_cons = cons + slots;
>>>>>>> v3.18

	return err;
}

static int xennet_set_skb_gso(struct sk_buff *skb,
			      struct xen_netif_extra_info *gso)
{
	if (!gso->u.gso.size) {
		if (net_ratelimit())
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_WARNING "GSO size must not be zero.\n");
		return -EINVAL;
	}

	/* Currently only TCPv4 S.O. is supported. */
	if (gso->u.gso.type != XEN_NETIF_GSO_TYPE_TCPV4) {
		if (net_ratelimit())
			printk(KERN_WARNING "Bad GSO type %d.\n", gso->u.gso.type);
=======
=======
>>>>>>> v3.18
			pr_warn("GSO size must not be zero\n");
		return -EINVAL;
	}

	if (gso->u.gso.type != XEN_NETIF_GSO_TYPE_TCPV4 &&
	    gso->u.gso.type != XEN_NETIF_GSO_TYPE_TCPV6) {
		if (net_ratelimit())
			pr_warn("Bad GSO type %d\n", gso->u.gso.type);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return -EINVAL;
	}

	skb_shinfo(skb)->gso_size = gso->u.gso.size;
<<<<<<< HEAD
<<<<<<< HEAD
	skb_shinfo(skb)->gso_type = SKB_GSO_TCPV4;
=======
=======
>>>>>>> v3.18
	skb_shinfo(skb)->gso_type =
		(gso->u.gso.type == XEN_NETIF_GSO_TYPE_TCPV4) ?
		SKB_GSO_TCPV4 :
		SKB_GSO_TCPV6;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* Header must be checked, and gso_segs computed. */
	skb_shinfo(skb)->gso_type |= SKB_GSO_DODGY;
	skb_shinfo(skb)->gso_segs = 0;

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static RING_IDX xennet_fill_frags(struct netfront_info *np,
=======
static RING_IDX xennet_fill_frags(struct netfront_queue *queue,
>>>>>>> v3.18
=======
static RING_IDX xennet_fill_frags(struct netfront_queue *queue,
>>>>>>> v3.18
				  struct sk_buff *skb,
				  struct sk_buff_head *list)
{
	struct skb_shared_info *shinfo = skb_shinfo(skb);
<<<<<<< HEAD
<<<<<<< HEAD
	RING_IDX cons = np->rx.rsp_cons;
=======
	RING_IDX cons = queue->rx.rsp_cons;
>>>>>>> v3.18
=======
	RING_IDX cons = queue->rx.rsp_cons;
>>>>>>> v3.18
	struct sk_buff *nskb;

	while ((nskb = __skb_dequeue(list))) {
		struct xen_netif_rx_response *rx =
<<<<<<< HEAD
<<<<<<< HEAD
			RING_GET_RESPONSE(&np->rx, ++cons);
=======
			RING_GET_RESPONSE(&queue->rx, ++cons);
>>>>>>> v3.18
=======
			RING_GET_RESPONSE(&queue->rx, ++cons);
>>>>>>> v3.18
		skb_frag_t *nfrag = &skb_shinfo(nskb)->frags[0];

		if (shinfo->nr_frags == MAX_SKB_FRAGS) {
			unsigned int pull_to = NETFRONT_SKB_CB(skb)->pull_to;

			BUG_ON(pull_to <= skb_headlen(skb));
			__pskb_pull_tail(skb, pull_to - skb_headlen(skb));
		}
		BUG_ON(shinfo->nr_frags >= MAX_SKB_FRAGS);

		skb_add_rx_frag(skb, shinfo->nr_frags, skb_frag_page(nfrag),
				rx->offset, rx->status, PAGE_SIZE);

		skb_shinfo(nskb)->nr_frags = 0;
		kfree_skb(nskb);
	}

	return cons;
}

static int checksum_setup(struct net_device *dev, struct sk_buff *skb)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct iphdr *iph;
	unsigned char *th;
	int err = -EPROTO;
	int recalculate_partial_csum = 0;
=======
	bool recalculate_partial_csum = false;
>>>>>>> v3.18
=======
	bool recalculate_partial_csum = false;
>>>>>>> v3.18

	/*
	 * A GSO SKB must be CHECKSUM_PARTIAL. However some buggy
	 * peers can fail to set NETRXF_csum_blank when sending a GSO
	 * frame. In this case force the SKB to CHECKSUM_PARTIAL and
	 * recalculate the partial checksum.
	 */
	if (skb->ip_summed != CHECKSUM_PARTIAL && skb_is_gso(skb)) {
		struct netfront_info *np = netdev_priv(dev);
<<<<<<< HEAD
<<<<<<< HEAD
		np->rx_gso_checksum_fixup++;
		skb->ip_summed = CHECKSUM_PARTIAL;
		recalculate_partial_csum = 1;
=======
		atomic_inc(&np->rx_gso_checksum_fixup);
		skb->ip_summed = CHECKSUM_PARTIAL;
		recalculate_partial_csum = true;
>>>>>>> v3.18
=======
		atomic_inc(&np->rx_gso_checksum_fixup);
		skb->ip_summed = CHECKSUM_PARTIAL;
		recalculate_partial_csum = true;
>>>>>>> v3.18
	}

	/* A non-CHECKSUM_PARTIAL SKB does not require setup. */
	if (skb->ip_summed != CHECKSUM_PARTIAL)
		return 0;

<<<<<<< HEAD
<<<<<<< HEAD
	if (skb->protocol != htons(ETH_P_IP))
		goto out;

	iph = (void *)skb->data;
	th = skb->data + 4 * iph->ihl;
	if (th >= skb_tail_pointer(skb))
		goto out;

	skb->csum_start = th - skb->head;
	switch (iph->protocol) {
	case IPPROTO_TCP:
		skb->csum_offset = offsetof(struct tcphdr, check);

		if (recalculate_partial_csum) {
			struct tcphdr *tcph = (struct tcphdr *)th;
			tcph->check = ~csum_tcpudp_magic(iph->saddr, iph->daddr,
							 skb->len - iph->ihl*4,
							 IPPROTO_TCP, 0);
		}
		break;
	case IPPROTO_UDP:
		skb->csum_offset = offsetof(struct udphdr, check);

		if (recalculate_partial_csum) {
			struct udphdr *udph = (struct udphdr *)th;
			udph->check = ~csum_tcpudp_magic(iph->saddr, iph->daddr,
							 skb->len - iph->ihl*4,
							 IPPROTO_UDP, 0);
		}
		break;
	default:
		if (net_ratelimit())
			printk(KERN_ERR "Attempting to checksum a non-"
			       "TCP/UDP packet, dropping a protocol"
			       " %d packet", iph->protocol);
		goto out;
	}

	if ((th + skb->csum_offset + 2) > skb_tail_pointer(skb))
		goto out;

	err = 0;

out:
	return err;
}

static int handle_incoming_queue(struct net_device *dev,
				 struct sk_buff_head *rxq)
{
	struct netfront_info *np = netdev_priv(dev);
	struct netfront_stats *stats = this_cpu_ptr(np->stats);
=======
=======
>>>>>>> v3.18
	return skb_checksum_setup(skb, recalculate_partial_csum);
}

static int handle_incoming_queue(struct netfront_queue *queue,
				 struct sk_buff_head *rxq)
{
	struct netfront_stats *stats = this_cpu_ptr(queue->info->stats);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	int packets_dropped = 0;
	struct sk_buff *skb;

	while ((skb = __skb_dequeue(rxq)) != NULL) {
		int pull_to = NETFRONT_SKB_CB(skb)->pull_to;

		if (pull_to > skb_headlen(skb))
			__pskb_pull_tail(skb, pull_to - skb_headlen(skb));

		/* Ethernet work: Delayed to here as it peeks the header. */
<<<<<<< HEAD
<<<<<<< HEAD
		skb->protocol = eth_type_trans(skb, dev);

		if (checksum_setup(dev, skb)) {
			kfree_skb(skb);
			packets_dropped++;
			dev->stats.rx_errors++;
=======
=======
>>>>>>> v3.18
		skb->protocol = eth_type_trans(skb, queue->info->netdev);
		skb_reset_network_header(skb);

		if (checksum_setup(queue->info->netdev, skb)) {
			kfree_skb(skb);
			packets_dropped++;
			queue->info->netdev->stats.rx_errors++;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			continue;
		}

		u64_stats_update_begin(&stats->syncp);
		stats->rx_packets++;
		stats->rx_bytes += skb->len;
		u64_stats_update_end(&stats->syncp);

		/* Pass it up. */
<<<<<<< HEAD
<<<<<<< HEAD
		netif_receive_skb(skb);
=======
		napi_gro_receive(&queue->napi, skb);
>>>>>>> v3.18
=======
		napi_gro_receive(&queue->napi, skb);
>>>>>>> v3.18
	}

	return packets_dropped;
}

static int xennet_poll(struct napi_struct *napi, int budget)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct netfront_info *np = container_of(napi, struct netfront_info, napi);
	struct net_device *dev = np->netdev;
=======
	struct netfront_queue *queue = container_of(napi, struct netfront_queue, napi);
	struct net_device *dev = queue->info->netdev;
>>>>>>> v3.18
=======
	struct netfront_queue *queue = container_of(napi, struct netfront_queue, napi);
	struct net_device *dev = queue->info->netdev;
>>>>>>> v3.18
	struct sk_buff *skb;
	struct netfront_rx_info rinfo;
	struct xen_netif_rx_response *rx = &rinfo.rx;
	struct xen_netif_extra_info *extras = rinfo.extras;
	RING_IDX i, rp;
	int work_done;
	struct sk_buff_head rxq;
	struct sk_buff_head errq;
	struct sk_buff_head tmpq;
	unsigned long flags;
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock(&np->rx_lock);
=======
	spin_lock(&queue->rx_lock);
>>>>>>> v3.18
=======
	spin_lock(&queue->rx_lock);
>>>>>>> v3.18

	skb_queue_head_init(&rxq);
	skb_queue_head_init(&errq);
	skb_queue_head_init(&tmpq);

<<<<<<< HEAD
<<<<<<< HEAD
	rp = np->rx.sring->rsp_prod;
	rmb(); /* Ensure we see queued responses up to 'rp'. */

	i = np->rx.rsp_cons;
	work_done = 0;
	while ((i != rp) && (work_done < budget)) {
		memcpy(rx, RING_GET_RESPONSE(&np->rx, i), sizeof(*rx));
		memset(extras, 0, sizeof(rinfo.extras));

		err = xennet_get_responses(np, &rinfo, rp, &tmpq);
=======
=======
>>>>>>> v3.18
	rp = queue->rx.sring->rsp_prod;
	rmb(); /* Ensure we see queued responses up to 'rp'. */

	i = queue->rx.rsp_cons;
	work_done = 0;
	while ((i != rp) && (work_done < budget)) {
		memcpy(rx, RING_GET_RESPONSE(&queue->rx, i), sizeof(*rx));
		memset(extras, 0, sizeof(rinfo.extras));

		err = xennet_get_responses(queue, &rinfo, rp, &tmpq);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

		if (unlikely(err)) {
err:
			while ((skb = __skb_dequeue(&tmpq)))
				__skb_queue_tail(&errq, skb);
			dev->stats.rx_errors++;
<<<<<<< HEAD
<<<<<<< HEAD
			i = np->rx.rsp_cons;
=======
			i = queue->rx.rsp_cons;
>>>>>>> v3.18
=======
			i = queue->rx.rsp_cons;
>>>>>>> v3.18
			continue;
		}

		skb = __skb_dequeue(&tmpq);

		if (extras[XEN_NETIF_EXTRA_TYPE_GSO - 1].type) {
			struct xen_netif_extra_info *gso;
			gso = &extras[XEN_NETIF_EXTRA_TYPE_GSO - 1];

			if (unlikely(xennet_set_skb_gso(skb, gso))) {
				__skb_queue_head(&tmpq, skb);
<<<<<<< HEAD
<<<<<<< HEAD
				np->rx.rsp_cons += skb_queue_len(&tmpq);
=======
				queue->rx.rsp_cons += skb_queue_len(&tmpq);
>>>>>>> v3.18
=======
				queue->rx.rsp_cons += skb_queue_len(&tmpq);
>>>>>>> v3.18
				goto err;
			}
		}

		NETFRONT_SKB_CB(skb)->pull_to = rx->status;
		if (NETFRONT_SKB_CB(skb)->pull_to > RX_COPY_THRESHOLD)
			NETFRONT_SKB_CB(skb)->pull_to = RX_COPY_THRESHOLD;

		skb_shinfo(skb)->frags[0].page_offset = rx->offset;
		skb_frag_size_set(&skb_shinfo(skb)->frags[0], rx->status);
		skb->data_len = rx->status;
		skb->len += rx->status;

<<<<<<< HEAD
<<<<<<< HEAD
		i = xennet_fill_frags(np, skb, &tmpq);
=======
		i = xennet_fill_frags(queue, skb, &tmpq);
>>>>>>> v3.18
=======
		i = xennet_fill_frags(queue, skb, &tmpq);
>>>>>>> v3.18

		if (rx->flags & XEN_NETRXF_csum_blank)
			skb->ip_summed = CHECKSUM_PARTIAL;
		else if (rx->flags & XEN_NETRXF_data_validated)
			skb->ip_summed = CHECKSUM_UNNECESSARY;

		__skb_queue_tail(&rxq, skb);

<<<<<<< HEAD
<<<<<<< HEAD
		np->rx.rsp_cons = ++i;
=======
		queue->rx.rsp_cons = ++i;
>>>>>>> v3.18
=======
		queue->rx.rsp_cons = ++i;
>>>>>>> v3.18
		work_done++;
	}

	__skb_queue_purge(&errq);

<<<<<<< HEAD
<<<<<<< HEAD
	work_done -= handle_incoming_queue(dev, &rxq);

	/* If we get a callback with very few responses, reduce fill target. */
	/* NB. Note exponential increase, linear decrease. */
	if (((np->rx.req_prod_pvt - np->rx.sring->rsp_prod) >
	     ((3*np->rx_target) / 4)) &&
	    (--np->rx_target < np->rx_min_target))
		np->rx_target = np->rx_min_target;

	xennet_alloc_rx_buffers(dev);
=======
=======
>>>>>>> v3.18
	work_done -= handle_incoming_queue(queue, &rxq);

	/* If we get a callback with very few responses, reduce fill target. */
	/* NB. Note exponential increase, linear decrease. */
	if (((queue->rx.req_prod_pvt - queue->rx.sring->rsp_prod) >
	     ((3*queue->rx_target) / 4)) &&
	    (--queue->rx_target < queue->rx_min_target))
		queue->rx_target = queue->rx_min_target;

	xennet_alloc_rx_buffers(queue);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (work_done < budget) {
		int more_to_do = 0;

<<<<<<< HEAD
<<<<<<< HEAD
		local_irq_save(flags);

		RING_FINAL_CHECK_FOR_RESPONSES(&np->rx, more_to_do);
=======
=======
>>>>>>> v3.18
		napi_gro_flush(napi, false);

		local_irq_save(flags);

		RING_FINAL_CHECK_FOR_RESPONSES(&queue->rx, more_to_do);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		if (!more_to_do)
			__napi_complete(napi);

		local_irq_restore(flags);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	spin_unlock(&np->rx_lock);
=======
	spin_unlock(&queue->rx_lock);
>>>>>>> v3.18
=======
	spin_unlock(&queue->rx_lock);
>>>>>>> v3.18

	return work_done;
}

static int xennet_change_mtu(struct net_device *dev, int mtu)
{
	int max = xennet_can_sg(dev) ?
		XEN_NETIF_MAX_TX_SIZE - MAX_TCP_HEADER : ETH_DATA_LEN;

	if (mtu > max)
		return -EINVAL;
	dev->mtu = mtu;
	return 0;
}

static struct rtnl_link_stats64 *xennet_get_stats64(struct net_device *dev,
						    struct rtnl_link_stats64 *tot)
{
	struct netfront_info *np = netdev_priv(dev);
	int cpu;

	for_each_possible_cpu(cpu) {
		struct netfront_stats *stats = per_cpu_ptr(np->stats, cpu);
		u64 rx_packets, rx_bytes, tx_packets, tx_bytes;
		unsigned int start;

		do {
<<<<<<< HEAD
<<<<<<< HEAD
			start = u64_stats_fetch_begin_bh(&stats->syncp);
=======
			start = u64_stats_fetch_begin_irq(&stats->syncp);
>>>>>>> v3.18
=======
			start = u64_stats_fetch_begin_irq(&stats->syncp);
>>>>>>> v3.18

			rx_packets = stats->rx_packets;
			tx_packets = stats->tx_packets;
			rx_bytes = stats->rx_bytes;
			tx_bytes = stats->tx_bytes;
<<<<<<< HEAD
<<<<<<< HEAD
		} while (u64_stats_fetch_retry_bh(&stats->syncp, start));
=======
		} while (u64_stats_fetch_retry_irq(&stats->syncp, start));
>>>>>>> v3.18
=======
		} while (u64_stats_fetch_retry_irq(&stats->syncp, start));
>>>>>>> v3.18

		tot->rx_packets += rx_packets;
		tot->tx_packets += tx_packets;
		tot->rx_bytes   += rx_bytes;
		tot->tx_bytes   += tx_bytes;
	}

	tot->rx_errors  = dev->stats.rx_errors;
	tot->tx_dropped = dev->stats.tx_dropped;

	return tot;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void xennet_release_tx_bufs(struct netfront_info *np)
=======
static void xennet_release_tx_bufs(struct netfront_queue *queue)
>>>>>>> v3.18
=======
static void xennet_release_tx_bufs(struct netfront_queue *queue)
>>>>>>> v3.18
{
	struct sk_buff *skb;
	int i;

	for (i = 0; i < NET_TX_RING_SIZE; i++) {
		/* Skip over entries which are actually freelist references */
<<<<<<< HEAD
<<<<<<< HEAD
		if (skb_entry_is_link(&np->tx_skbs[i]))
			continue;

		skb = np->tx_skbs[i].skb;
		get_page(np->grant_tx_page[i]);
		gnttab_end_foreign_access(np->grant_tx_ref[i],
					  GNTMAP_readonly,
					  (unsigned long)page_address(np->grant_tx_page[i]));
		np->grant_tx_page[i] = NULL;
		np->grant_tx_ref[i] = GRANT_INVALID_REF;
		add_id_to_freelist(&np->tx_skb_freelist, np->tx_skbs, i);
=======
=======
>>>>>>> v3.18
		if (skb_entry_is_link(&queue->tx_skbs[i]))
			continue;

		skb = queue->tx_skbs[i].skb;
		get_page(queue->grant_tx_page[i]);
		gnttab_end_foreign_access(queue->grant_tx_ref[i],
					  GNTMAP_readonly,
					  (unsigned long)page_address(queue->grant_tx_page[i]));
		queue->grant_tx_page[i] = NULL;
		queue->grant_tx_ref[i] = GRANT_INVALID_REF;
		add_id_to_freelist(&queue->tx_skb_freelist, queue->tx_skbs, i);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		dev_kfree_skb_irq(skb);
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static void xennet_release_rx_bufs(struct netfront_info *np)
{
	int id, ref;

	spin_lock_bh(&np->rx_lock);
=======
=======
>>>>>>> v3.18
static void xennet_release_rx_bufs(struct netfront_queue *queue)
{
	int id, ref;

	spin_lock_bh(&queue->rx_lock);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	for (id = 0; id < NET_RX_RING_SIZE; id++) {
		struct sk_buff *skb;
		struct page *page;

<<<<<<< HEAD
<<<<<<< HEAD
		skb = np->rx_skbs[id];
		if (!skb)
			continue;

		ref = np->grant_rx_ref[id];
=======
=======
>>>>>>> v3.18
		skb = queue->rx_skbs[id];
		if (!skb)
			continue;

		ref = queue->grant_rx_ref[id];
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		if (ref == GRANT_INVALID_REF)
			continue;

		page = skb_frag_page(&skb_shinfo(skb)->frags[0]);

		/* gnttab_end_foreign_access() needs a page ref until
		 * foreign access is ended (which may be deferred).
		 */
		get_page(page);
		gnttab_end_foreign_access(ref, 0,
					  (unsigned long)page_address(page));
<<<<<<< HEAD
<<<<<<< HEAD
		np->grant_rx_ref[id] = GRANT_INVALID_REF;
=======
		queue->grant_rx_ref[id] = GRANT_INVALID_REF;
>>>>>>> v3.18
=======
		queue->grant_rx_ref[id] = GRANT_INVALID_REF;
>>>>>>> v3.18

		kfree_skb(skb);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	spin_unlock_bh(&np->rx_lock);
}

static void xennet_uninit(struct net_device *dev)
{
	struct netfront_info *np = netdev_priv(dev);
	xennet_release_tx_bufs(np);
	xennet_release_rx_bufs(np);
	gnttab_free_grant_references(np->gref_tx_head);
	gnttab_free_grant_references(np->gref_rx_head);
=======
	spin_unlock_bh(&queue->rx_lock);
>>>>>>> v3.18
=======
	spin_unlock_bh(&queue->rx_lock);
>>>>>>> v3.18
}

static netdev_features_t xennet_fix_features(struct net_device *dev,
	netdev_features_t features)
{
	struct netfront_info *np = netdev_priv(dev);
	int val;

	if (features & NETIF_F_SG) {
		if (xenbus_scanf(XBT_NIL, np->xbdev->otherend, "feature-sg",
				 "%d", &val) < 0)
			val = 0;

		if (!val)
			features &= ~NETIF_F_SG;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	if (features & NETIF_F_IPV6_CSUM) {
		if (xenbus_scanf(XBT_NIL, np->xbdev->otherend,
				 "feature-ipv6-csum-offload", "%d", &val) < 0)
			val = 0;

		if (!val)
			features &= ~NETIF_F_IPV6_CSUM;
	}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (features & NETIF_F_TSO) {
		if (xenbus_scanf(XBT_NIL, np->xbdev->otherend,
				 "feature-gso-tcpv4", "%d", &val) < 0)
			val = 0;

		if (!val)
			features &= ~NETIF_F_TSO;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	if (features & NETIF_F_TSO6) {
		if (xenbus_scanf(XBT_NIL, np->xbdev->otherend,
				 "feature-gso-tcpv6", "%d", &val) < 0)
			val = 0;

		if (!val)
			features &= ~NETIF_F_TSO6;
	}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return features;
}

static int xennet_set_features(struct net_device *dev,
	netdev_features_t features)
{
	if (!(features & NETIF_F_SG) && dev->mtu > ETH_DATA_LEN) {
		netdev_info(dev, "Reducing MTU because no SG offload");
		dev->mtu = ETH_DATA_LEN;
	}

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static irqreturn_t xennet_interrupt(int irq, void *dev_id)
{
	struct net_device *dev = dev_id;
	struct netfront_info *np = netdev_priv(dev);
	unsigned long flags;

	spin_lock_irqsave(&np->tx_lock, flags);

	if (likely(netif_carrier_ok(dev))) {
		xennet_tx_buf_gc(dev);
		/* Under tx_lock: protects access to rx shared-ring indexes. */
		if (RING_HAS_UNCONSUMED_RESPONSES(&np->rx))
			napi_schedule(&np->napi);
	}

	spin_unlock_irqrestore(&np->tx_lock, flags);
=======
=======
>>>>>>> v3.18
static irqreturn_t xennet_tx_interrupt(int irq, void *dev_id)
{
	struct netfront_queue *queue = dev_id;
	unsigned long flags;

	spin_lock_irqsave(&queue->tx_lock, flags);
	xennet_tx_buf_gc(queue);
	spin_unlock_irqrestore(&queue->tx_lock, flags);

	return IRQ_HANDLED;
}

static irqreturn_t xennet_rx_interrupt(int irq, void *dev_id)
{
	struct netfront_queue *queue = dev_id;
	struct net_device *dev = queue->info->netdev;

	if (likely(netif_carrier_ok(dev) &&
		   RING_HAS_UNCONSUMED_RESPONSES(&queue->rx)))
		napi_schedule(&queue->napi);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return IRQ_HANDLED;
}

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_NET_POLL_CONTROLLER
static void xennet_poll_controller(struct net_device *dev)
{
	xennet_interrupt(0, dev);
=======
=======
>>>>>>> v3.18
static irqreturn_t xennet_interrupt(int irq, void *dev_id)
{
	xennet_tx_interrupt(irq, dev_id);
	xennet_rx_interrupt(irq, dev_id);
	return IRQ_HANDLED;
}

#ifdef CONFIG_NET_POLL_CONTROLLER
static void xennet_poll_controller(struct net_device *dev)
{
	/* Poll each queue */
	struct netfront_info *info = netdev_priv(dev);
	unsigned int num_queues = dev->real_num_tx_queues;
	unsigned int i;
	for (i = 0; i < num_queues; ++i)
		xennet_interrupt(0, &info->queues[i]);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}
#endif

static const struct net_device_ops xennet_netdev_ops = {
	.ndo_open            = xennet_open,
<<<<<<< HEAD
<<<<<<< HEAD
	.ndo_uninit          = xennet_uninit,
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	.ndo_stop            = xennet_close,
	.ndo_start_xmit      = xennet_start_xmit,
	.ndo_change_mtu	     = xennet_change_mtu,
	.ndo_get_stats64     = xennet_get_stats64,
	.ndo_set_mac_address = eth_mac_addr,
	.ndo_validate_addr   = eth_validate_addr,
	.ndo_fix_features    = xennet_fix_features,
	.ndo_set_features    = xennet_set_features,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	.ndo_select_queue    = xennet_select_queue,
>>>>>>> v3.18
=======
	.ndo_select_queue    = xennet_select_queue,
>>>>>>> v3.18
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller = xennet_poll_controller,
#endif
};

static struct net_device *xennet_create_dev(struct xenbus_device *dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int i, err;
	struct net_device *netdev;
	struct netfront_info *np;

	netdev = alloc_etherdev(sizeof(struct netfront_info));
=======
=======
>>>>>>> v3.18
	int err;
	struct net_device *netdev;
	struct netfront_info *np;

	netdev = alloc_etherdev_mq(sizeof(struct netfront_info), xennet_max_queues);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (!netdev)
		return ERR_PTR(-ENOMEM);

	np                   = netdev_priv(netdev);
	np->xbdev            = dev;

<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock_init(&np->tx_lock);
	spin_lock_init(&np->rx_lock);

	skb_queue_head_init(&np->rx_batch);
	np->rx_target     = RX_DFL_MIN_TARGET;
	np->rx_min_target = RX_DFL_MIN_TARGET;
	np->rx_max_target = RX_MAX_TARGET;

	init_timer(&np->rx_refill_timer);
	np->rx_refill_timer.data = (unsigned long)netdev;
	np->rx_refill_timer.function = rx_refill_timeout;

	err = -ENOMEM;
	np->stats = alloc_percpu(struct netfront_stats);
	if (np->stats == NULL)
		goto exit;

	/* Initialise tx_skbs as a free chain containing every entry. */
	np->tx_skb_freelist = 0;
	for (i = 0; i < NET_TX_RING_SIZE; i++) {
		skb_entry_set_link(&np->tx_skbs[i], i+1);
		np->grant_tx_ref[i] = GRANT_INVALID_REF;
	}

	/* Clear out rx_skbs */
	for (i = 0; i < NET_RX_RING_SIZE; i++) {
		np->rx_skbs[i] = NULL;
		np->grant_rx_ref[i] = GRANT_INVALID_REF;
		np->grant_tx_page[i] = NULL;
	}

	/* A grant for every tx ring slot */
	if (gnttab_alloc_grant_references(TX_MAX_TARGET,
					  &np->gref_tx_head) < 0) {
		printk(KERN_ALERT "#### netfront can't alloc tx grant refs\n");
		err = -ENOMEM;
		goto exit_free_stats;
	}
	/* A grant for every rx ring slot */
	if (gnttab_alloc_grant_references(RX_MAX_TARGET,
					  &np->gref_rx_head) < 0) {
		printk(KERN_ALERT "#### netfront can't alloc rx grant refs\n");
		err = -ENOMEM;
		goto exit_free_tx;
	}

	netdev->netdev_ops	= &xennet_netdev_ops;

	netif_napi_add(netdev, &np->napi, xennet_poll, 64);
	netdev->features        = NETIF_F_IP_CSUM | NETIF_F_RXCSUM |
				  NETIF_F_GSO_ROBUST;
	netdev->hw_features	= NETIF_F_IP_CSUM | NETIF_F_SG | NETIF_F_TSO;
=======
=======
>>>>>>> v3.18
	/* No need to use rtnl_lock() before the call below as it
	 * happens before register_netdev().
	 */
	netif_set_real_num_tx_queues(netdev, 0);
	np->queues = NULL;

	err = -ENOMEM;
	np->stats = netdev_alloc_pcpu_stats(struct netfront_stats);
	if (np->stats == NULL)
		goto exit;

	netdev->netdev_ops	= &xennet_netdev_ops;

	netdev->features        = NETIF_F_IP_CSUM | NETIF_F_RXCSUM |
				  NETIF_F_GSO_ROBUST;
	netdev->hw_features	= NETIF_F_SG |
				  NETIF_F_IPV6_CSUM |
				  NETIF_F_TSO | NETIF_F_TSO6;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/*
         * Assume that all hw features are available for now. This set
         * will be adjusted by the call to netdev_update_features() in
         * xennet_connect() which is the earliest point where we can
         * negotiate with the backend regarding supported features.
         */
	netdev->features |= netdev->hw_features;

<<<<<<< HEAD
<<<<<<< HEAD
	SET_ETHTOOL_OPS(netdev, &xennet_ethtool_ops);
=======
	netdev->ethtool_ops = &xennet_ethtool_ops;
>>>>>>> v3.18
=======
	netdev->ethtool_ops = &xennet_ethtool_ops;
>>>>>>> v3.18
	SET_NETDEV_DEV(netdev, &dev->dev);

	netif_set_gso_max_size(netdev, XEN_NETIF_MAX_TX_SIZE - MAX_TCP_HEADER);

	np->netdev = netdev;

	netif_carrier_off(netdev);

	return netdev;

<<<<<<< HEAD
<<<<<<< HEAD
 exit_free_tx:
	gnttab_free_grant_references(np->gref_tx_head);
 exit_free_stats:
	free_percpu(np->stats);
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
 exit:
	free_netdev(netdev);
	return ERR_PTR(err);
}

/**
 * Entry point to this code when a new device is created.  Allocate the basic
 * structures and the ring buffers for communication with the backend, and
 * inform the backend of the appropriate details for those.
 */
static int netfront_probe(struct xenbus_device *dev,
			  const struct xenbus_device_id *id)
{
	int err;
	struct net_device *netdev;
	struct netfront_info *info;

	netdev = xennet_create_dev(dev);
	if (IS_ERR(netdev)) {
		err = PTR_ERR(netdev);
		xenbus_dev_fatal(dev, err, "creating netdev");
		return err;
	}

	info = netdev_priv(netdev);
	dev_set_drvdata(&dev->dev, info);

	err = register_netdev(info->netdev);
	if (err) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_WARNING "%s: register_netdev err=%d\n",
		       __func__, err);
=======
		pr_warn("%s: register_netdev err=%d\n", __func__, err);
>>>>>>> v3.18
=======
		pr_warn("%s: register_netdev err=%d\n", __func__, err);
>>>>>>> v3.18
		goto fail;
	}

	err = xennet_sysfs_addif(info->netdev);
	if (err) {
		unregister_netdev(info->netdev);
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_WARNING "%s: add sysfs failed err=%d\n",
		       __func__, err);
=======
		pr_warn("%s: add sysfs failed err=%d\n", __func__, err);
>>>>>>> v3.18
=======
		pr_warn("%s: add sysfs failed err=%d\n", __func__, err);
>>>>>>> v3.18
		goto fail;
	}

	return 0;

 fail:
	free_netdev(netdev);
	dev_set_drvdata(&dev->dev, NULL);
	return err;
}

static void xennet_end_access(int ref, void *page)
{
	/* This frees the page as a side-effect */
	if (ref != GRANT_INVALID_REF)
		gnttab_end_foreign_access(ref, 0, (unsigned long)page);
}

static void xennet_disconnect_backend(struct netfront_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
	/* Stop old i/f to prevent errors whilst we rebuild the state. */
	spin_lock_bh(&info->rx_lock);
	spin_lock_irq(&info->tx_lock);
	netif_carrier_off(info->netdev);
	spin_unlock_irq(&info->tx_lock);
	spin_unlock_bh(&info->rx_lock);

	if (info->netdev->irq)
		unbind_from_irqhandler(info->netdev->irq, info->netdev);
	info->evtchn = info->netdev->irq = 0;

	/* End access and free the pages */
	xennet_end_access(info->tx_ring_ref, info->tx.sring);
	xennet_end_access(info->rx_ring_ref, info->rx.sring);

	info->tx_ring_ref = GRANT_INVALID_REF;
	info->rx_ring_ref = GRANT_INVALID_REF;
	info->tx.sring = NULL;
	info->rx.sring = NULL;
=======
=======
>>>>>>> v3.18
	unsigned int i = 0;
	unsigned int num_queues = info->netdev->real_num_tx_queues;

	netif_carrier_off(info->netdev);

	for (i = 0; i < num_queues; ++i) {
		struct netfront_queue *queue = &info->queues[i];

		if (queue->tx_irq && (queue->tx_irq == queue->rx_irq))
			unbind_from_irqhandler(queue->tx_irq, queue);
		if (queue->tx_irq && (queue->tx_irq != queue->rx_irq)) {
			unbind_from_irqhandler(queue->tx_irq, queue);
			unbind_from_irqhandler(queue->rx_irq, queue);
		}
		queue->tx_evtchn = queue->rx_evtchn = 0;
		queue->tx_irq = queue->rx_irq = 0;

		napi_synchronize(&queue->napi);

		xennet_release_tx_bufs(queue);
		xennet_release_rx_bufs(queue);
		gnttab_free_grant_references(queue->gref_tx_head);
		gnttab_free_grant_references(queue->gref_rx_head);

		/* End access and free the pages */
		xennet_end_access(queue->tx_ring_ref, queue->tx.sring);
		xennet_end_access(queue->rx_ring_ref, queue->rx.sring);

		queue->tx_ring_ref = GRANT_INVALID_REF;
		queue->rx_ring_ref = GRANT_INVALID_REF;
		queue->tx.sring = NULL;
		queue->rx.sring = NULL;
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/**
 * We are reconnecting to the backend, due to a suspend/resume, or a backend
 * driver restart.  We tear down our netif structure and recreate it, but
 * leave the device-layer structures intact so that this is transparent to the
 * rest of the kernel.
 */
static int netfront_resume(struct xenbus_device *dev)
{
	struct netfront_info *info = dev_get_drvdata(&dev->dev);

	dev_dbg(&dev->dev, "%s\n", dev->nodename);

	xennet_disconnect_backend(info);
	return 0;
}

static int xen_net_read_mac(struct xenbus_device *dev, u8 mac[])
{
	char *s, *e, *macstr;
	int i;

	macstr = s = xenbus_read(XBT_NIL, dev->nodename, "mac", NULL);
	if (IS_ERR(macstr))
		return PTR_ERR(macstr);

	for (i = 0; i < ETH_ALEN; i++) {
		mac[i] = simple_strtoul(s, &e, 16);
		if ((s == e) || (*e != ((i == ETH_ALEN-1) ? '\0' : ':'))) {
			kfree(macstr);
			return -ENOENT;
		}
		s = e+1;
	}

	kfree(macstr);
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int setup_netfront(struct xenbus_device *dev, struct netfront_info *info)
{
	struct xen_netif_tx_sring *txs;
	struct xen_netif_rx_sring *rxs;
	int err;
	struct net_device *netdev = info->netdev;

	info->tx_ring_ref = GRANT_INVALID_REF;
	info->rx_ring_ref = GRANT_INVALID_REF;
	info->rx.sring = NULL;
	info->tx.sring = NULL;
	netdev->irq = 0;

	err = xen_net_read_mac(dev, netdev->dev_addr);
	if (err) {
		xenbus_dev_fatal(dev, err, "parsing %s/mac", dev->nodename);
		goto fail;
	}
=======
=======
>>>>>>> v3.18
static int setup_netfront_single(struct netfront_queue *queue)
{
	int err;

	err = xenbus_alloc_evtchn(queue->info->xbdev, &queue->tx_evtchn);
	if (err < 0)
		goto fail;

	err = bind_evtchn_to_irqhandler(queue->tx_evtchn,
					xennet_interrupt,
					0, queue->info->netdev->name, queue);
	if (err < 0)
		goto bind_fail;
	queue->rx_evtchn = queue->tx_evtchn;
	queue->rx_irq = queue->tx_irq = err;

	return 0;

bind_fail:
	xenbus_free_evtchn(queue->info->xbdev, queue->tx_evtchn);
	queue->tx_evtchn = 0;
fail:
	return err;
}

static int setup_netfront_split(struct netfront_queue *queue)
{
	int err;

	err = xenbus_alloc_evtchn(queue->info->xbdev, &queue->tx_evtchn);
	if (err < 0)
		goto fail;
	err = xenbus_alloc_evtchn(queue->info->xbdev, &queue->rx_evtchn);
	if (err < 0)
		goto alloc_rx_evtchn_fail;

	snprintf(queue->tx_irq_name, sizeof(queue->tx_irq_name),
		 "%s-tx", queue->name);
	err = bind_evtchn_to_irqhandler(queue->tx_evtchn,
					xennet_tx_interrupt,
					0, queue->tx_irq_name, queue);
	if (err < 0)
		goto bind_tx_fail;
	queue->tx_irq = err;

	snprintf(queue->rx_irq_name, sizeof(queue->rx_irq_name),
		 "%s-rx", queue->name);
	err = bind_evtchn_to_irqhandler(queue->rx_evtchn,
					xennet_rx_interrupt,
					0, queue->rx_irq_name, queue);
	if (err < 0)
		goto bind_rx_fail;
	queue->rx_irq = err;

	return 0;

bind_rx_fail:
	unbind_from_irqhandler(queue->tx_irq, queue);
	queue->tx_irq = 0;
bind_tx_fail:
	xenbus_free_evtchn(queue->info->xbdev, queue->rx_evtchn);
	queue->rx_evtchn = 0;
alloc_rx_evtchn_fail:
	xenbus_free_evtchn(queue->info->xbdev, queue->tx_evtchn);
	queue->tx_evtchn = 0;
fail:
	return err;
}

static int setup_netfront(struct xenbus_device *dev,
			struct netfront_queue *queue, unsigned int feature_split_evtchn)
{
	struct xen_netif_tx_sring *txs;
	struct xen_netif_rx_sring *rxs;
	int err;

	queue->tx_ring_ref = GRANT_INVALID_REF;
	queue->rx_ring_ref = GRANT_INVALID_REF;
	queue->rx.sring = NULL;
	queue->tx.sring = NULL;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	txs = (struct xen_netif_tx_sring *)get_zeroed_page(GFP_NOIO | __GFP_HIGH);
	if (!txs) {
		err = -ENOMEM;
		xenbus_dev_fatal(dev, err, "allocating tx ring page");
		goto fail;
	}
	SHARED_RING_INIT(txs);
<<<<<<< HEAD
<<<<<<< HEAD
	FRONT_RING_INIT(&info->tx, txs, PAGE_SIZE);

	err = xenbus_grant_ring(dev, virt_to_mfn(txs));
	if (err < 0) {
		free_page((unsigned long)txs);
		goto fail;
	}

	info->tx_ring_ref = err;
=======
=======
>>>>>>> v3.18
	FRONT_RING_INIT(&queue->tx, txs, PAGE_SIZE);

	err = xenbus_grant_ring(dev, virt_to_mfn(txs));
	if (err < 0)
		goto grant_tx_ring_fail;
	queue->tx_ring_ref = err;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	rxs = (struct xen_netif_rx_sring *)get_zeroed_page(GFP_NOIO | __GFP_HIGH);
	if (!rxs) {
		err = -ENOMEM;
		xenbus_dev_fatal(dev, err, "allocating rx ring page");
<<<<<<< HEAD
<<<<<<< HEAD
		goto fail;
	}
	SHARED_RING_INIT(rxs);
	FRONT_RING_INIT(&info->rx, rxs, PAGE_SIZE);

	err = xenbus_grant_ring(dev, virt_to_mfn(rxs));
	if (err < 0) {
		free_page((unsigned long)rxs);
		goto fail;
	}
	info->rx_ring_ref = err;

	err = xenbus_alloc_evtchn(dev, &info->evtchn);
	if (err)
		goto fail;

	err = bind_evtchn_to_irqhandler(info->evtchn, xennet_interrupt,
					0, netdev->name, netdev);
	if (err < 0)
		goto fail;
	netdev->irq = err;
	return 0;

 fail:
	return err;
}

=======
=======
>>>>>>> v3.18
		goto alloc_rx_ring_fail;
	}
	SHARED_RING_INIT(rxs);
	FRONT_RING_INIT(&queue->rx, rxs, PAGE_SIZE);

	err = xenbus_grant_ring(dev, virt_to_mfn(rxs));
	if (err < 0)
		goto grant_rx_ring_fail;
	queue->rx_ring_ref = err;

	if (feature_split_evtchn)
		err = setup_netfront_split(queue);
	/* setup single event channel if
	 *  a) feature-split-event-channels == 0
	 *  b) feature-split-event-channels == 1 but failed to setup
	 */
	if (!feature_split_evtchn || (feature_split_evtchn && err))
		err = setup_netfront_single(queue);

	if (err)
		goto alloc_evtchn_fail;

	return 0;

	/* If we fail to setup netfront, it is safe to just revoke access to
	 * granted pages because backend is not accessing it at this point.
	 */
alloc_evtchn_fail:
	gnttab_end_foreign_access_ref(queue->rx_ring_ref, 0);
grant_rx_ring_fail:
	free_page((unsigned long)rxs);
alloc_rx_ring_fail:
	gnttab_end_foreign_access_ref(queue->tx_ring_ref, 0);
grant_tx_ring_fail:
	free_page((unsigned long)txs);
fail:
	return err;
}

/* Queue-specific initialisation
 * This used to be done in xennet_create_dev() but must now
 * be run per-queue.
 */
static int xennet_init_queue(struct netfront_queue *queue)
{
	unsigned short i;
	int err = 0;

	spin_lock_init(&queue->tx_lock);
	spin_lock_init(&queue->rx_lock);

	skb_queue_head_init(&queue->rx_batch);
	queue->rx_target     = RX_DFL_MIN_TARGET;
	queue->rx_min_target = RX_DFL_MIN_TARGET;
	queue->rx_max_target = RX_MAX_TARGET;

	init_timer(&queue->rx_refill_timer);
	queue->rx_refill_timer.data = (unsigned long)queue;
	queue->rx_refill_timer.function = rx_refill_timeout;

	snprintf(queue->name, sizeof(queue->name), "%s-q%u",
		 queue->info->netdev->name, queue->id);

	/* Initialise tx_skbs as a free chain containing every entry. */
	queue->tx_skb_freelist = 0;
	for (i = 0; i < NET_TX_RING_SIZE; i++) {
		skb_entry_set_link(&queue->tx_skbs[i], i+1);
		queue->grant_tx_ref[i] = GRANT_INVALID_REF;
		queue->grant_tx_page[i] = NULL;
	}

	/* Clear out rx_skbs */
	for (i = 0; i < NET_RX_RING_SIZE; i++) {
		queue->rx_skbs[i] = NULL;
		queue->grant_rx_ref[i] = GRANT_INVALID_REF;
	}

	/* A grant for every tx ring slot */
	if (gnttab_alloc_grant_references(TX_MAX_TARGET,
					  &queue->gref_tx_head) < 0) {
		pr_alert("can't alloc tx grant refs\n");
		err = -ENOMEM;
		goto exit;
	}

	/* A grant for every rx ring slot */
	if (gnttab_alloc_grant_references(RX_MAX_TARGET,
					  &queue->gref_rx_head) < 0) {
		pr_alert("can't alloc rx grant refs\n");
		err = -ENOMEM;
		goto exit_free_tx;
	}

	return 0;

 exit_free_tx:
	gnttab_free_grant_references(queue->gref_tx_head);
 exit:
	return err;
}

static int write_queue_xenstore_keys(struct netfront_queue *queue,
			   struct xenbus_transaction *xbt, int write_hierarchical)
{
	/* Write the queue-specific keys into XenStore in the traditional
	 * way for a single queue, or in a queue subkeys for multiple
	 * queues.
	 */
	struct xenbus_device *dev = queue->info->xbdev;
	int err;
	const char *message;
	char *path;
	size_t pathsize;

	/* Choose the correct place to write the keys */
	if (write_hierarchical) {
		pathsize = strlen(dev->nodename) + 10;
		path = kzalloc(pathsize, GFP_KERNEL);
		if (!path) {
			err = -ENOMEM;
			message = "out of memory while writing ring references";
			goto error;
		}
		snprintf(path, pathsize, "%s/queue-%u",
				dev->nodename, queue->id);
	} else {
		path = (char *)dev->nodename;
	}

	/* Write ring references */
	err = xenbus_printf(*xbt, path, "tx-ring-ref", "%u",
			queue->tx_ring_ref);
	if (err) {
		message = "writing tx-ring-ref";
		goto error;
	}

	err = xenbus_printf(*xbt, path, "rx-ring-ref", "%u",
			queue->rx_ring_ref);
	if (err) {
		message = "writing rx-ring-ref";
		goto error;
	}

	/* Write event channels; taking into account both shared
	 * and split event channel scenarios.
	 */
	if (queue->tx_evtchn == queue->rx_evtchn) {
		/* Shared event channel */
		err = xenbus_printf(*xbt, path,
				"event-channel", "%u", queue->tx_evtchn);
		if (err) {
			message = "writing event-channel";
			goto error;
		}
	} else {
		/* Split event channels */
		err = xenbus_printf(*xbt, path,
				"event-channel-tx", "%u", queue->tx_evtchn);
		if (err) {
			message = "writing event-channel-tx";
			goto error;
		}

		err = xenbus_printf(*xbt, path,
				"event-channel-rx", "%u", queue->rx_evtchn);
		if (err) {
			message = "writing event-channel-rx";
			goto error;
		}
	}

	if (write_hierarchical)
		kfree(path);
	return 0;

error:
	if (write_hierarchical)
		kfree(path);
	xenbus_dev_fatal(dev, err, "%s", message);
	return err;
}

static void xennet_destroy_queues(struct netfront_info *info)
{
	unsigned int i;

	rtnl_lock();

	for (i = 0; i < info->netdev->real_num_tx_queues; i++) {
		struct netfront_queue *queue = &info->queues[i];

		if (netif_running(info->netdev))
			napi_disable(&queue->napi);
		netif_napi_del(&queue->napi);
	}

	rtnl_unlock();

	kfree(info->queues);
	info->queues = NULL;
}

static int xennet_create_queues(struct netfront_info *info,
				unsigned int num_queues)
{
	unsigned int i;
	int ret;

	info->queues = kcalloc(num_queues, sizeof(struct netfront_queue),
			       GFP_KERNEL);
	if (!info->queues)
		return -ENOMEM;

	rtnl_lock();

	for (i = 0; i < num_queues; i++) {
		struct netfront_queue *queue = &info->queues[i];

		queue->id = i;
		queue->info = info;

		ret = xennet_init_queue(queue);
		if (ret < 0) {
			dev_warn(&info->netdev->dev,
				 "only created %d queues\n", i);
			num_queues = i;
			break;
		}

		netif_napi_add(queue->info->netdev, &queue->napi,
			       xennet_poll, 64);
		if (netif_running(info->netdev))
			napi_enable(&queue->napi);
	}

	netif_set_real_num_tx_queues(info->netdev, num_queues);

	rtnl_unlock();

	if (num_queues == 0) {
		dev_err(&info->netdev->dev, "no queues\n");
		return -EINVAL;
	}
	return 0;
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
/* Common code used when first setting up, and when resuming. */
static int talk_to_netback(struct xenbus_device *dev,
			   struct netfront_info *info)
{
	const char *message;
	struct xenbus_transaction xbt;
	int err;
<<<<<<< HEAD
<<<<<<< HEAD

	/* Create shared ring, alloc event channel. */
	err = setup_netfront(dev, info);
	if (err)
		goto out;
=======
=======
>>>>>>> v3.18
	unsigned int feature_split_evtchn;
	unsigned int i = 0;
	unsigned int max_queues = 0;
	struct netfront_queue *queue = NULL;
	unsigned int num_queues = 1;

	info->netdev->irq = 0;

	/* Check if backend supports multiple queues */
	err = xenbus_scanf(XBT_NIL, info->xbdev->otherend,
			   "multi-queue-max-queues", "%u", &max_queues);
	if (err < 0)
		max_queues = 1;
	num_queues = min(max_queues, xennet_max_queues);

	/* Check feature-split-event-channels */
	err = xenbus_scanf(XBT_NIL, info->xbdev->otherend,
			   "feature-split-event-channels", "%u",
			   &feature_split_evtchn);
	if (err < 0)
		feature_split_evtchn = 0;

	/* Read mac addr. */
	err = xen_net_read_mac(dev, info->netdev->dev_addr);
	if (err) {
		xenbus_dev_fatal(dev, err, "parsing %s/mac", dev->nodename);
		goto out;
	}

	if (info->queues)
		xennet_destroy_queues(info);

	err = xennet_create_queues(info, num_queues);
	if (err < 0)
		goto destroy_ring;

	/* Create shared ring, alloc event channel -- for each queue */
	for (i = 0; i < num_queues; ++i) {
		queue = &info->queues[i];
		err = setup_netfront(dev, queue, feature_split_evtchn);
		if (err) {
			/* setup_netfront() will tidy up the current
			 * queue on error, but we need to clean up
			 * those already allocated.
			 */
			if (i > 0) {
				rtnl_lock();
				netif_set_real_num_tx_queues(info->netdev, i);
				rtnl_unlock();
				goto destroy_ring;
			} else {
				goto out;
			}
		}
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

again:
	err = xenbus_transaction_start(&xbt);
	if (err) {
		xenbus_dev_fatal(dev, err, "starting transaction");
		goto destroy_ring;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	err = xenbus_printf(xbt, dev->nodename, "tx-ring-ref", "%u",
			    info->tx_ring_ref);
	if (err) {
		message = "writing tx ring-ref";
		goto abort_transaction;
	}
	err = xenbus_printf(xbt, dev->nodename, "rx-ring-ref", "%u",
			    info->rx_ring_ref);
	if (err) {
		message = "writing rx ring-ref";
		goto abort_transaction;
	}
	err = xenbus_printf(xbt, dev->nodename,
			    "event-channel", "%u", info->evtchn);
	if (err) {
		message = "writing event-channel";
		goto abort_transaction;
	}

=======
=======
>>>>>>> v3.18
	if (num_queues == 1) {
		err = write_queue_xenstore_keys(&info->queues[0], &xbt, 0); /* flat */
		if (err)
			goto abort_transaction_no_dev_fatal;
	} else {
		/* Write the number of queues */
		err = xenbus_printf(xbt, dev->nodename, "multi-queue-num-queues",
				    "%u", num_queues);
		if (err) {
			message = "writing multi-queue-num-queues";
			goto abort_transaction_no_dev_fatal;
		}

		/* Write the keys for each queue */
		for (i = 0; i < num_queues; ++i) {
			queue = &info->queues[i];
			err = write_queue_xenstore_keys(queue, &xbt, 1); /* hierarchical */
			if (err)
				goto abort_transaction_no_dev_fatal;
		}
	}

	/* The remaining keys are not queue-specific */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	err = xenbus_printf(xbt, dev->nodename, "request-rx-copy", "%u",
			    1);
	if (err) {
		message = "writing request-rx-copy";
		goto abort_transaction;
	}

	err = xenbus_printf(xbt, dev->nodename, "feature-rx-notify", "%d", 1);
	if (err) {
		message = "writing feature-rx-notify";
		goto abort_transaction;
	}

	err = xenbus_printf(xbt, dev->nodename, "feature-sg", "%d", 1);
	if (err) {
		message = "writing feature-sg";
		goto abort_transaction;
	}

	err = xenbus_printf(xbt, dev->nodename, "feature-gso-tcpv4", "%d", 1);
	if (err) {
		message = "writing feature-gso-tcpv4";
		goto abort_transaction;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	err = xenbus_write(xbt, dev->nodename, "feature-gso-tcpv6", "1");
	if (err) {
		message = "writing feature-gso-tcpv6";
		goto abort_transaction;
	}

	err = xenbus_write(xbt, dev->nodename, "feature-ipv6-csum-offload",
			   "1");
	if (err) {
		message = "writing feature-ipv6-csum-offload";
		goto abort_transaction;
	}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	err = xenbus_transaction_end(xbt, 0);
	if (err) {
		if (err == -EAGAIN)
			goto again;
		xenbus_dev_fatal(dev, err, "completing transaction");
		goto destroy_ring;
	}

	return 0;

 abort_transaction:
<<<<<<< HEAD
<<<<<<< HEAD
	xenbus_transaction_end(xbt, 1);
	xenbus_dev_fatal(dev, err, "%s", message);
 destroy_ring:
	xennet_disconnect_backend(info);
=======
=======
>>>>>>> v3.18
	xenbus_dev_fatal(dev, err, "%s", message);
abort_transaction_no_dev_fatal:
	xenbus_transaction_end(xbt, 1);
 destroy_ring:
	xennet_disconnect_backend(info);
	kfree(info->queues);
	info->queues = NULL;
	rtnl_lock();
	netif_set_real_num_tx_queues(info->netdev, 0);
	rtnl_unlock();
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 out:
	return err;
}

static int xennet_connect(struct net_device *dev)
{
	struct netfront_info *np = netdev_priv(dev);
<<<<<<< HEAD
<<<<<<< HEAD
	int i, requeue_idx, err;
	struct sk_buff *skb;
	grant_ref_t ref;
	struct xen_netif_rx_request *req;
	unsigned int feature_rx_copy;
=======
=======
>>>>>>> v3.18
	unsigned int num_queues = 0;
	int err;
	unsigned int feature_rx_copy;
	unsigned int j = 0;
	struct netfront_queue *queue = NULL;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	err = xenbus_scanf(XBT_NIL, np->xbdev->otherend,
			   "feature-rx-copy", "%u", &feature_rx_copy);
	if (err != 1)
		feature_rx_copy = 0;

	if (!feature_rx_copy) {
		dev_info(&dev->dev,
			 "backend does not support copying receive path\n");
		return -ENODEV;
	}

	err = talk_to_netback(np->xbdev, np);
	if (err)
		return err;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	/* talk_to_netback() sets the correct number of queues */
	num_queues = dev->real_num_tx_queues;

>>>>>>> v3.18
=======
	/* talk_to_netback() sets the correct number of queues */
	num_queues = dev->real_num_tx_queues;

>>>>>>> v3.18
	rtnl_lock();
	netdev_update_features(dev);
	rtnl_unlock();

<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock_bh(&np->rx_lock);
	spin_lock_irq(&np->tx_lock);

	/* Step 1: Discard all pending TX packet fragments. */
	xennet_release_tx_bufs(np);

	/* Step 2: Rebuild the RX buffer freelist and the RX ring itself. */
	for (requeue_idx = 0, i = 0; i < NET_RX_RING_SIZE; i++) {
		skb_frag_t *frag;
		const struct page *page;
		if (!np->rx_skbs[i])
			continue;

		skb = np->rx_skbs[requeue_idx] = xennet_get_rx_skb(np, i);
		ref = np->grant_rx_ref[requeue_idx] = xennet_get_rx_ref(np, i);
		req = RING_GET_REQUEST(&np->rx, requeue_idx);

		frag = &skb_shinfo(skb)->frags[0];
		page = skb_frag_page(frag);
		gnttab_grant_foreign_access_ref(
			ref, np->xbdev->otherend_id,
			pfn_to_mfn(page_to_pfn(page)),
			0);
		req->gref = ref;
		req->id   = requeue_idx;

		requeue_idx++;
	}

	np->rx.req_prod_pvt = requeue_idx;

	/*
	 * Step 3: All public and private state should now be sane.  Get
=======
	/*
	 * All public and private state should now be sane.  Get
>>>>>>> v3.18
=======
	/*
	 * All public and private state should now be sane.  Get
>>>>>>> v3.18
	 * ready to start sending and receiving packets and give the driver
	 * domain a kick because we've probably just requeued some
	 * packets.
	 */
	netif_carrier_on(np->netdev);
<<<<<<< HEAD
<<<<<<< HEAD
	notify_remote_via_irq(np->netdev->irq);
	xennet_tx_buf_gc(dev);
	xennet_alloc_rx_buffers(dev);

	spin_unlock_irq(&np->tx_lock);
	spin_unlock_bh(&np->rx_lock);
=======
=======
>>>>>>> v3.18
	for (j = 0; j < num_queues; ++j) {
		queue = &np->queues[j];

		notify_remote_via_irq(queue->tx_irq);
		if (queue->tx_irq != queue->rx_irq)
			notify_remote_via_irq(queue->rx_irq);

		spin_lock_irq(&queue->tx_lock);
		xennet_tx_buf_gc(queue);
		spin_unlock_irq(&queue->tx_lock);

		spin_lock_bh(&queue->rx_lock);
		xennet_alloc_rx_buffers(queue);
		spin_unlock_bh(&queue->rx_lock);
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return 0;
}

/**
 * Callback received when the backend's state changes.
 */
static void netback_changed(struct xenbus_device *dev,
			    enum xenbus_state backend_state)
{
	struct netfront_info *np = dev_get_drvdata(&dev->dev);
	struct net_device *netdev = np->netdev;

	dev_dbg(&dev->dev, "%s\n", xenbus_strstate(backend_state));

	switch (backend_state) {
	case XenbusStateInitialising:
	case XenbusStateInitialised:
	case XenbusStateReconfiguring:
	case XenbusStateReconfigured:
	case XenbusStateUnknown:
<<<<<<< HEAD
<<<<<<< HEAD
	case XenbusStateClosed:
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
		break;

	case XenbusStateInitWait:
		if (dev->state != XenbusStateInitialising)
			break;
		if (xennet_connect(netdev) != 0)
			break;
		xenbus_switch_state(dev, XenbusStateConnected);
		break;

	case XenbusStateConnected:
		netdev_notify_peers(netdev);
		break;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	case XenbusStateClosed:
		if (dev->state == XenbusStateClosed)
			break;
		/* Missed the backend's CLOSING state -- fallthrough */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	case XenbusStateClosing:
		xenbus_frontend_closed(dev);
		break;
	}
}

static const struct xennet_stat {
	char name[ETH_GSTRING_LEN];
	u16 offset;
} xennet_stats[] = {
	{
		"rx_gso_checksum_fixup",
		offsetof(struct netfront_info, rx_gso_checksum_fixup)
	},
};

static int xennet_get_sset_count(struct net_device *dev, int string_set)
{
	switch (string_set) {
	case ETH_SS_STATS:
		return ARRAY_SIZE(xennet_stats);
	default:
		return -EINVAL;
	}
}

static void xennet_get_ethtool_stats(struct net_device *dev,
				     struct ethtool_stats *stats, u64 * data)
{
	void *np = netdev_priv(dev);
	int i;

	for (i = 0; i < ARRAY_SIZE(xennet_stats); i++)
<<<<<<< HEAD
<<<<<<< HEAD
		data[i] = *(unsigned long *)(np + xennet_stats[i].offset);
=======
		data[i] = atomic_read((atomic_t *)(np + xennet_stats[i].offset));
>>>>>>> v3.18
=======
		data[i] = atomic_read((atomic_t *)(np + xennet_stats[i].offset));
>>>>>>> v3.18
}

static void xennet_get_strings(struct net_device *dev, u32 stringset, u8 * data)
{
	int i;

	switch (stringset) {
	case ETH_SS_STATS:
		for (i = 0; i < ARRAY_SIZE(xennet_stats); i++)
			memcpy(data + i * ETH_GSTRING_LEN,
			       xennet_stats[i].name, ETH_GSTRING_LEN);
		break;
	}
}

static const struct ethtool_ops xennet_ethtool_ops =
{
	.get_link = ethtool_op_get_link,

	.get_sset_count = xennet_get_sset_count,
	.get_ethtool_stats = xennet_get_ethtool_stats,
	.get_strings = xennet_get_strings,
};

#ifdef CONFIG_SYSFS
static ssize_t show_rxbuf_min(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
	struct net_device *netdev = to_net_dev(dev);
	struct netfront_info *info = netdev_priv(netdev);
<<<<<<< HEAD
<<<<<<< HEAD

	return sprintf(buf, "%u\n", info->rx_min_target);
=======
=======
>>>>>>> v3.18
	unsigned int num_queues = netdev->real_num_tx_queues;

	if (num_queues)
		return sprintf(buf, "%u\n", info->queues[0].rx_min_target);
	else
		return sprintf(buf, "%u\n", RX_MIN_TARGET);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static ssize_t store_rxbuf_min(struct device *dev,
			       struct device_attribute *attr,
			       const char *buf, size_t len)
{
	struct net_device *netdev = to_net_dev(dev);
	struct netfront_info *np = netdev_priv(netdev);
<<<<<<< HEAD
<<<<<<< HEAD
	char *endp;
	unsigned long target;
=======
=======
>>>>>>> v3.18
	unsigned int num_queues = netdev->real_num_tx_queues;
	char *endp;
	unsigned long target;
	unsigned int i;
	struct netfront_queue *queue;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	target = simple_strtoul(buf, &endp, 0);
	if (endp == buf)
		return -EBADMSG;

	if (target < RX_MIN_TARGET)
		target = RX_MIN_TARGET;
	if (target > RX_MAX_TARGET)
		target = RX_MAX_TARGET;

<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock_bh(&np->rx_lock);
	if (target > np->rx_max_target)
		np->rx_max_target = target;
	np->rx_min_target = target;
	if (target > np->rx_target)
		np->rx_target = target;

	xennet_alloc_rx_buffers(netdev);

	spin_unlock_bh(&np->rx_lock);
=======
=======
>>>>>>> v3.18
	for (i = 0; i < num_queues; ++i) {
		queue = &np->queues[i];
		spin_lock_bh(&queue->rx_lock);
		if (target > queue->rx_max_target)
			queue->rx_max_target = target;
		queue->rx_min_target = target;
		if (target > queue->rx_target)
			queue->rx_target = target;

		xennet_alloc_rx_buffers(queue);

		spin_unlock_bh(&queue->rx_lock);
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return len;
}

static ssize_t show_rxbuf_max(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
	struct net_device *netdev = to_net_dev(dev);
	struct netfront_info *info = netdev_priv(netdev);
<<<<<<< HEAD
<<<<<<< HEAD

	return sprintf(buf, "%u\n", info->rx_max_target);
=======
=======
>>>>>>> v3.18
	unsigned int num_queues = netdev->real_num_tx_queues;

	if (num_queues)
		return sprintf(buf, "%u\n", info->queues[0].rx_max_target);
	else
		return sprintf(buf, "%u\n", RX_MAX_TARGET);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static ssize_t store_rxbuf_max(struct device *dev,
			       struct device_attribute *attr,
			       const char *buf, size_t len)
{
	struct net_device *netdev = to_net_dev(dev);
	struct netfront_info *np = netdev_priv(netdev);
<<<<<<< HEAD
<<<<<<< HEAD
	char *endp;
	unsigned long target;
=======
=======
>>>>>>> v3.18
	unsigned int num_queues = netdev->real_num_tx_queues;
	char *endp;
	unsigned long target;
	unsigned int i = 0;
	struct netfront_queue *queue = NULL;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	target = simple_strtoul(buf, &endp, 0);
	if (endp == buf)
		return -EBADMSG;

	if (target < RX_MIN_TARGET)
		target = RX_MIN_TARGET;
	if (target > RX_MAX_TARGET)
		target = RX_MAX_TARGET;

<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock_bh(&np->rx_lock);
	if (target < np->rx_min_target)
		np->rx_min_target = target;
	np->rx_max_target = target;
	if (target < np->rx_target)
		np->rx_target = target;

	xennet_alloc_rx_buffers(netdev);

	spin_unlock_bh(&np->rx_lock);
=======
=======
>>>>>>> v3.18
	for (i = 0; i < num_queues; ++i) {
		queue = &np->queues[i];
		spin_lock_bh(&queue->rx_lock);
		if (target < queue->rx_min_target)
			queue->rx_min_target = target;
		queue->rx_max_target = target;
		if (target < queue->rx_target)
			queue->rx_target = target;

		xennet_alloc_rx_buffers(queue);

		spin_unlock_bh(&queue->rx_lock);
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return len;
}

static ssize_t show_rxbuf_cur(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
	struct net_device *netdev = to_net_dev(dev);
	struct netfront_info *info = netdev_priv(netdev);
<<<<<<< HEAD
<<<<<<< HEAD

	return sprintf(buf, "%u\n", info->rx_target);
=======
=======
>>>>>>> v3.18
	unsigned int num_queues = netdev->real_num_tx_queues;

	if (num_queues)
		return sprintf(buf, "%u\n", info->queues[0].rx_target);
	else
		return sprintf(buf, "0\n");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static struct device_attribute xennet_attrs[] = {
	__ATTR(rxbuf_min, S_IRUGO|S_IWUSR, show_rxbuf_min, store_rxbuf_min),
	__ATTR(rxbuf_max, S_IRUGO|S_IWUSR, show_rxbuf_max, store_rxbuf_max),
	__ATTR(rxbuf_cur, S_IRUGO, show_rxbuf_cur, NULL),
};

static int xennet_sysfs_addif(struct net_device *netdev)
{
	int i;
	int err;

	for (i = 0; i < ARRAY_SIZE(xennet_attrs); i++) {
		err = device_create_file(&netdev->dev,
					   &xennet_attrs[i]);
		if (err)
			goto fail;
	}
	return 0;

 fail:
	while (--i >= 0)
		device_remove_file(&netdev->dev, &xennet_attrs[i]);
	return err;
}

static void xennet_sysfs_delif(struct net_device *netdev)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(xennet_attrs); i++)
		device_remove_file(&netdev->dev, &xennet_attrs[i]);
}

#endif /* CONFIG_SYSFS */

<<<<<<< HEAD
<<<<<<< HEAD
static const struct xenbus_device_id netfront_ids[] = {
	{ "vif" },
	{ "" }
};


static int xennet_remove(struct xenbus_device *dev)
{
	struct netfront_info *info = dev_get_drvdata(&dev->dev);
=======
=======
>>>>>>> v3.18
static int xennet_remove(struct xenbus_device *dev)
{
	struct netfront_info *info = dev_get_drvdata(&dev->dev);
	unsigned int num_queues = info->netdev->real_num_tx_queues;
	struct netfront_queue *queue = NULL;
	unsigned int i = 0;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	dev_dbg(&dev->dev, "%s\n", dev->nodename);

	xennet_disconnect_backend(info);

	xennet_sysfs_delif(info->netdev);

	unregister_netdev(info->netdev);

<<<<<<< HEAD
<<<<<<< HEAD
	del_timer_sync(&info->rx_refill_timer);
=======
=======
>>>>>>> v3.18
	for (i = 0; i < num_queues; ++i) {
		queue = &info->queues[i];
		del_timer_sync(&queue->rx_refill_timer);
	}

	if (num_queues) {
		kfree(info->queues);
		info->queues = NULL;
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	free_percpu(info->stats);

	free_netdev(info->netdev);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static DEFINE_XENBUS_DRIVER(netfront, ,
=======
=======
>>>>>>> v3.18
static const struct xenbus_device_id netfront_ids[] = {
	{ "vif" },
	{ "" }
};

static struct xenbus_driver netfront_driver = {
	.ids = netfront_ids,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	.probe = netfront_probe,
	.remove = xennet_remove,
	.resume = netfront_resume,
	.otherend_changed = netback_changed,
<<<<<<< HEAD
<<<<<<< HEAD
);
=======
};
>>>>>>> v3.18
=======
};
>>>>>>> v3.18

static int __init netif_init(void)
{
	if (!xen_domain())
		return -ENODEV;

<<<<<<< HEAD
<<<<<<< HEAD
	if (xen_hvm_domain() && !xen_platform_pci_unplug)
		return -ENODEV;

	printk(KERN_INFO "Initialising Xen virtual ethernet driver.\n");
=======
=======
>>>>>>> v3.18
	if (!xen_has_pv_nic_devices())
		return -ENODEV;

	pr_info("Initialising Xen virtual ethernet driver\n");

	/* Allow as many queues as there are CPUs, by default */
	xennet_max_queues = num_online_cpus();
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return xenbus_register_frontend(&netfront_driver);
}
module_init(netif_init);


static void __exit netif_exit(void)
{
	xenbus_unregister_driver(&netfront_driver);
}
module_exit(netif_exit);

MODULE_DESCRIPTION("Xen virtual network device frontend");
MODULE_LICENSE("GPL");
MODULE_ALIAS("xen:vif");
MODULE_ALIAS("xennet");
