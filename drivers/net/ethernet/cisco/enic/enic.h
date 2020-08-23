/*
 * Copyright 2008-2010 Cisco Systems, Inc.  All rights reserved.
 * Copyright 2007 Nuova Systems, Inc.  All rights reserved.
 *
 * This program is free software; you may redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef _ENIC_H_
#define _ENIC_H_

#include "vnic_enet.h"
#include "vnic_dev.h"
#include "vnic_wq.h"
#include "vnic_rq.h"
#include "vnic_cq.h"
#include "vnic_intr.h"
#include "vnic_stats.h"
#include "vnic_nic.h"
#include "vnic_rss.h"
<<<<<<< HEAD

#define DRV_NAME		"enic"
#define DRV_DESCRIPTION		"Cisco VIC Ethernet NIC Driver"
#define DRV_VERSION		"2.1.1.39"
#define DRV_COPYRIGHT		"Copyright 2008-2011 Cisco Systems, Inc"

#define ENIC_BARS_MAX		6

#define ENIC_WQ_MAX		1
=======
#include <linux/irq.h>

#define DRV_NAME		"enic"
#define DRV_DESCRIPTION		"Cisco VIC Ethernet NIC Driver"
#define DRV_VERSION		"2.1.1.67"
#define DRV_COPYRIGHT		"Copyright 2008-2013 Cisco Systems, Inc"

#define ENIC_BARS_MAX		6

#define ENIC_WQ_MAX		8
>>>>>>> v3.18
#define ENIC_RQ_MAX		8
#define ENIC_CQ_MAX		(ENIC_WQ_MAX + ENIC_RQ_MAX)
#define ENIC_INTR_MAX		(ENIC_CQ_MAX + 2)

<<<<<<< HEAD
=======
#define ENIC_AIC_LARGE_PKT_DIFF	3

>>>>>>> v3.18
struct enic_msix_entry {
	int requested;
	char devname[IFNAMSIZ];
	irqreturn_t (*isr)(int, void *);
	void *devid;
};

<<<<<<< HEAD
=======
/* Store only the lower range.  Higher range is given by fw. */
struct enic_intr_mod_range {
	u32 small_pkt_range_start;
	u32 large_pkt_range_start;
};

struct enic_intr_mod_table {
	u32 rx_rate;
	u32 range_percent;
};

#define ENIC_MAX_LINK_SPEEDS		3
#define ENIC_LINK_SPEED_10G		10000
#define ENIC_LINK_SPEED_4G		4000
#define ENIC_LINK_40G_INDEX		2
#define ENIC_LINK_10G_INDEX		1
#define ENIC_LINK_4G_INDEX		0
#define ENIC_RX_COALESCE_RANGE_END	125
#define ENIC_AIC_TS_BREAK		100

struct enic_rx_coal {
	u32 small_pkt_range_start;
	u32 large_pkt_range_start;
	u32 range_end;
	u32 use_adaptive_rx_coalesce;
};

>>>>>>> v3.18
/* priv_flags */
#define ENIC_SRIOV_ENABLED		(1 << 0)

/* enic port profile set flags */
#define ENIC_PORT_REQUEST_APPLIED	(1 << 0)
#define ENIC_SET_REQUEST		(1 << 1)
#define ENIC_SET_NAME			(1 << 2)
#define ENIC_SET_INSTANCE		(1 << 3)
#define ENIC_SET_HOST			(1 << 4)

struct enic_port_profile {
	u32 set;
	u8 request;
	char name[PORT_PROFILE_MAX];
	u8 instance_uuid[PORT_UUID_MAX];
	u8 host_uuid[PORT_UUID_MAX];
	u8 vf_mac[ETH_ALEN];
	u8 mac_addr[ETH_ALEN];
};

<<<<<<< HEAD
=======
/* enic_rfs_fltr_node - rfs filter node in hash table
 *	@@keys: IPv4 5 tuple
 *	@flow_id: flow_id of clsf filter provided by kernel
 *	@fltr_id: filter id of clsf filter returned by adaptor
 *	@rq_id: desired rq index
 *	@node: hlist_node
 */
struct enic_rfs_fltr_node {
	struct flow_keys keys;
	u32 flow_id;
	u16 fltr_id;
	u16 rq_id;
	struct hlist_node node;
};

/* enic_rfs_flw_tbl - rfs flow table
 *	@max: Maximum number of filters vNIC supports
 *	@free: Number of free filters available
 *	@toclean: hash table index to clean next
 *	@ht_head: hash table list head
 *	@lock: spin lock
 *	@rfs_may_expire: timer function for enic_rps_may_expire_flow
 */
struct enic_rfs_flw_tbl {
	u16 max;
	int free;

#define ENIC_RFS_FLW_BITSHIFT	(10)
#define ENIC_RFS_FLW_MASK	((1 << ENIC_RFS_FLW_BITSHIFT) - 1)
	u16 toclean:ENIC_RFS_FLW_BITSHIFT;
	struct hlist_head ht_head[1 << ENIC_RFS_FLW_BITSHIFT];
	spinlock_t lock;
	struct timer_list rfs_may_expire;
};

>>>>>>> v3.18
/* Per-instance private data structure */
struct enic {
	struct net_device *netdev;
	struct pci_dev *pdev;
	struct vnic_enet_config config;
	struct vnic_dev_bar bar[ENIC_BARS_MAX];
	struct vnic_dev *vdev;
	struct timer_list notify_timer;
	struct work_struct reset;
	struct work_struct change_mtu_work;
	struct msix_entry msix_entry[ENIC_INTR_MAX];
	struct enic_msix_entry msix[ENIC_INTR_MAX];
	u32 msg_enable;
	spinlock_t devcmd_lock;
	u8 mac_addr[ETH_ALEN];
<<<<<<< HEAD
	u8 mc_addr[ENIC_MULTICAST_PERFECT_FILTERS][ETH_ALEN];
	u8 uc_addr[ENIC_UNICAST_PERFECT_FILTERS][ETH_ALEN];
=======
>>>>>>> v3.18
	unsigned int flags;
	unsigned int priv_flags;
	unsigned int mc_count;
	unsigned int uc_count;
	u32 port_mtu;
<<<<<<< HEAD
=======
	struct enic_rx_coal rx_coalesce_setting;
>>>>>>> v3.18
	u32 rx_coalesce_usecs;
	u32 tx_coalesce_usecs;
#ifdef CONFIG_PCI_IOV
	u16 num_vfs;
#endif
<<<<<<< HEAD
=======
	spinlock_t enic_api_lock;
>>>>>>> v3.18
	struct enic_port_profile *pp;

	/* work queue cache line section */
	____cacheline_aligned struct vnic_wq wq[ENIC_WQ_MAX];
	spinlock_t wq_lock[ENIC_WQ_MAX];
	unsigned int wq_count;
	u16 loop_enable;
	u16 loop_tag;

	/* receive queue cache line section */
	____cacheline_aligned struct vnic_rq rq[ENIC_RQ_MAX];
	unsigned int rq_count;
	u64 rq_truncated_pkts;
	u64 rq_bad_fcs;
<<<<<<< HEAD
	struct napi_struct napi[ENIC_RQ_MAX];
=======
	struct napi_struct napi[ENIC_RQ_MAX + ENIC_WQ_MAX];
>>>>>>> v3.18

	/* interrupt resource cache line section */
	____cacheline_aligned struct vnic_intr intr[ENIC_INTR_MAX];
	unsigned int intr_count;
	u32 __iomem *legacy_pba;		/* memory-mapped */

	/* completion queue cache line section */
	____cacheline_aligned struct vnic_cq cq[ENIC_CQ_MAX];
	unsigned int cq_count;
<<<<<<< HEAD
=======
	struct enic_rfs_flw_tbl rfs_h;
	u32 rx_copybreak;
>>>>>>> v3.18
};

static inline struct device *enic_get_dev(struct enic *enic)
{
	return &(enic->pdev->dev);
}

<<<<<<< HEAD
=======
static inline unsigned int enic_cq_rq(struct enic *enic, unsigned int rq)
{
	return rq;
}

static inline unsigned int enic_cq_wq(struct enic *enic, unsigned int wq)
{
	return enic->rq_count + wq;
}

static inline unsigned int enic_legacy_io_intr(void)
{
	return 0;
}

static inline unsigned int enic_legacy_err_intr(void)
{
	return 1;
}

static inline unsigned int enic_legacy_notify_intr(void)
{
	return 2;
}

static inline unsigned int enic_msix_rq_intr(struct enic *enic,
	unsigned int rq)
{
	return enic->cq[enic_cq_rq(enic, rq)].interrupt_offset;
}

static inline unsigned int enic_msix_wq_intr(struct enic *enic,
	unsigned int wq)
{
	return enic->cq[enic_cq_wq(enic, wq)].interrupt_offset;
}

static inline unsigned int enic_msix_err_intr(struct enic *enic)
{
	return enic->rq_count + enic->wq_count;
}

static inline unsigned int enic_msix_notify_intr(struct enic *enic)
{
	return enic->rq_count + enic->wq_count + 1;
}

>>>>>>> v3.18
void enic_reset_addr_lists(struct enic *enic);
int enic_sriov_enabled(struct enic *enic);
int enic_is_valid_vf(struct enic *enic, int vf);
int enic_is_dynamic(struct enic *enic);
<<<<<<< HEAD
=======
void enic_set_ethtool_ops(struct net_device *netdev);
>>>>>>> v3.18

#endif /* _ENIC_H_ */
