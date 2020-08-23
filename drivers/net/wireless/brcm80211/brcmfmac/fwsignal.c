/*
 * Copyright (c) 2010 Broadcom Corporation
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <linux/types.h>
#include <linux/module.h>
#include <linux/if_ether.h>
#include <linux/spinlock.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/err.h>
#include <linux/jiffies.h>
<<<<<<< HEAD
#include <uapi/linux/nl80211.h>
=======
>>>>>>> v3.18
#include <net/cfg80211.h>

#include <brcmu_utils.h>
#include <brcmu_wifi.h>
#include "dhd.h"
<<<<<<< HEAD
#include "dhd_proto.h"
=======
>>>>>>> v3.18
#include "dhd_dbg.h"
#include "dhd_bus.h"
#include "fwil.h"
#include "fwil_types.h"
#include "fweh.h"
#include "fwsignal.h"
#include "p2p.h"
#include "wl_cfg80211.h"
<<<<<<< HEAD
=======
#include "proto.h"
>>>>>>> v3.18

/**
 * DOC: Firmware Signalling
 *
 * Firmware can send signals to host and vice versa, which are passed in the
 * data packets using TLV based header. This signalling layer is on top of the
 * BDC bus protocol layer.
 */

/*
 * single definition for firmware-driver flow control tlv's.
 *
 * each tlv is specified by BRCMF_FWS_TLV_DEF(name, ID, length).
 * A length value 0 indicates variable length tlv.
 */
#define BRCMF_FWS_TLV_DEFLIST \
	BRCMF_FWS_TLV_DEF(MAC_OPEN, 1, 1) \
	BRCMF_FWS_TLV_DEF(MAC_CLOSE, 2, 1) \
	BRCMF_FWS_TLV_DEF(MAC_REQUEST_CREDIT, 3, 2) \
	BRCMF_FWS_TLV_DEF(TXSTATUS, 4, 4) \
	BRCMF_FWS_TLV_DEF(PKTTAG, 5, 4) \
	BRCMF_FWS_TLV_DEF(MACDESC_ADD,	6, 8) \
	BRCMF_FWS_TLV_DEF(MACDESC_DEL, 7, 8) \
	BRCMF_FWS_TLV_DEF(RSSI, 8, 1) \
	BRCMF_FWS_TLV_DEF(INTERFACE_OPEN, 9, 1) \
	BRCMF_FWS_TLV_DEF(INTERFACE_CLOSE, 10, 1) \
	BRCMF_FWS_TLV_DEF(FIFO_CREDITBACK, 11, 6) \
	BRCMF_FWS_TLV_DEF(PENDING_TRAFFIC_BMP, 12, 2) \
	BRCMF_FWS_TLV_DEF(MAC_REQUEST_PACKET, 13, 3) \
	BRCMF_FWS_TLV_DEF(HOST_REORDER_RXPKTS, 14, 10) \
	BRCMF_FWS_TLV_DEF(TRANS_ID, 18, 6) \
	BRCMF_FWS_TLV_DEF(COMP_TXSTATUS, 19, 1) \
	BRCMF_FWS_TLV_DEF(FILLER, 255, 0)

/*
 * enum brcmf_fws_tlv_type - definition of tlv identifiers.
 */
#define BRCMF_FWS_TLV_DEF(name, id, len) \
	BRCMF_FWS_TYPE_ ## name =  id,
enum brcmf_fws_tlv_type {
	BRCMF_FWS_TLV_DEFLIST
	BRCMF_FWS_TYPE_INVALID
};
#undef BRCMF_FWS_TLV_DEF

/*
 * enum brcmf_fws_tlv_len - definition of tlv lengths.
 */
#define BRCMF_FWS_TLV_DEF(name, id, len) \
	BRCMF_FWS_TYPE_ ## name ## _LEN = (len),
enum brcmf_fws_tlv_len {
	BRCMF_FWS_TLV_DEFLIST
};
#undef BRCMF_FWS_TLV_DEF

#ifdef DEBUG
/*
 * brcmf_fws_tlv_names - array of tlv names.
 */
#define BRCMF_FWS_TLV_DEF(name, id, len) \
	{ id, #name },
static struct {
	enum brcmf_fws_tlv_type id;
	const char *name;
} brcmf_fws_tlv_names[] = {
	BRCMF_FWS_TLV_DEFLIST
};
#undef BRCMF_FWS_TLV_DEF

<<<<<<< HEAD
=======

>>>>>>> v3.18
static const char *brcmf_fws_get_tlv_name(enum brcmf_fws_tlv_type id)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(brcmf_fws_tlv_names); i++)
		if (brcmf_fws_tlv_names[i].id == id)
			return brcmf_fws_tlv_names[i].name;

	return "INVALID";
}
#else
static const char *brcmf_fws_get_tlv_name(enum brcmf_fws_tlv_type id)
{
	return "NODEBUG";
}
#endif /* DEBUG */

/*
<<<<<<< HEAD
=======
 * The PKTTAG tlv has additional bytes when firmware-signalling
 * mode has REUSESEQ flag set.
 */
#define BRCMF_FWS_TYPE_SEQ_LEN				2

/*
>>>>>>> v3.18
 * flags used to enable tlv signalling from firmware.
 */
#define BRCMF_FWS_FLAGS_RSSI_SIGNALS			0x0001
#define BRCMF_FWS_FLAGS_XONXOFF_SIGNALS			0x0002
#define BRCMF_FWS_FLAGS_CREDIT_STATUS_SIGNALS		0x0004
#define BRCMF_FWS_FLAGS_HOST_PROPTXSTATUS_ACTIVE	0x0008
#define BRCMF_FWS_FLAGS_PSQ_GENERATIONFSM_ENABLE	0x0010
#define BRCMF_FWS_FLAGS_PSQ_ZERO_BUFFER_ENABLE		0x0020
#define BRCMF_FWS_FLAGS_HOST_RXREORDER_ACTIVE		0x0040

#define BRCMF_FWS_MAC_DESC_TABLE_SIZE			32
#define BRCMF_FWS_MAC_DESC_ID_INVALID			0xff

#define BRCMF_FWS_HOSTIF_FLOWSTATE_OFF			0
#define BRCMF_FWS_HOSTIF_FLOWSTATE_ON			1
#define BRCMF_FWS_FLOWCONTROL_HIWATER			128
#define BRCMF_FWS_FLOWCONTROL_LOWATER			64

<<<<<<< HEAD
#define BRCMF_FWS_PSQ_PREC_COUNT		((NL80211_NUM_ACS + 1) * 2)
=======
#define BRCMF_FWS_PSQ_PREC_COUNT		((BRCMF_FWS_FIFO_COUNT + 1) * 2)
>>>>>>> v3.18
#define BRCMF_FWS_PSQ_LEN				256

#define BRCMF_FWS_HTOD_FLAG_PKTFROMHOST			0x01
#define BRCMF_FWS_HTOD_FLAG_PKT_REQUESTED		0x02

<<<<<<< HEAD
#define BRCMF_FWS_RET_OK_NOSCHEDULE	0
#define BRCMF_FWS_RET_OK_SCHEDULE	1
=======
#define BRCMF_FWS_RET_OK_NOSCHEDULE			0
#define BRCMF_FWS_RET_OK_SCHEDULE			1

#define BRCMF_FWS_MODE_REUSESEQ_SHIFT			3	/* seq reuse */
#define BRCMF_FWS_MODE_SET_REUSESEQ(x, val)	((x) = \
		((x) & ~(1 << BRCMF_FWS_MODE_REUSESEQ_SHIFT)) | \
		(((val) & 1) << BRCMF_FWS_MODE_REUSESEQ_SHIFT))
#define BRCMF_FWS_MODE_GET_REUSESEQ(x)	\
		(((x) >> BRCMF_FWS_MODE_REUSESEQ_SHIFT) & 1)
>>>>>>> v3.18

/**
 * enum brcmf_fws_skb_state - indicates processing state of skb.
 *
 * @BRCMF_FWS_SKBSTATE_NEW: sk_buff is newly arrived in the driver.
 * @BRCMF_FWS_SKBSTATE_DELAYED: sk_buff had to wait on queue.
 * @BRCMF_FWS_SKBSTATE_SUPPRESSED: sk_buff has been suppressed by firmware.
<<<<<<< HEAD
=======
 * @BRCMF_FWS_SKBSTATE_TIM: allocated for TIM update info.
>>>>>>> v3.18
 */
enum brcmf_fws_skb_state {
	BRCMF_FWS_SKBSTATE_NEW,
	BRCMF_FWS_SKBSTATE_DELAYED,
<<<<<<< HEAD
	BRCMF_FWS_SKBSTATE_SUPPRESSED
=======
	BRCMF_FWS_SKBSTATE_SUPPRESSED,
	BRCMF_FWS_SKBSTATE_TIM
>>>>>>> v3.18
};

/**
 * struct brcmf_skbuff_cb - control buffer associated with skbuff.
 *
<<<<<<< HEAD
 * @if_flags: holds interface index and packet related flags.
 * @htod: host to device packet identifier (used in PKTTAG tlv).
=======
 * @bus_flags: 2 bytes reserved for bus specific parameters
 * @if_flags: holds interface index and packet related flags.
 * @htod: host to device packet identifier (used in PKTTAG tlv).
 * @htod_seq: this 16-bit is original seq number for every suppress packet.
>>>>>>> v3.18
 * @state: transmit state of the packet.
 * @mac: descriptor related to destination for this packet.
 *
 * This information is stored in control buffer struct sk_buff::cb, which
 * provides 48 bytes of storage so this structure should not exceed that.
 */
struct brcmf_skbuff_cb {
<<<<<<< HEAD
	u16 if_flags;
	u32 htod;
=======
	u16 bus_flags;
	u16 if_flags;
	u32 htod;
	u16 htod_seq;
>>>>>>> v3.18
	enum brcmf_fws_skb_state state;
	struct brcmf_fws_mac_descriptor *mac;
};

/*
 * macro casting skbuff control buffer to struct brcmf_skbuff_cb.
 */
#define brcmf_skbcb(skb)	((struct brcmf_skbuff_cb *)((skb)->cb))

/*
 * sk_buff control if flags
 *
 *	b[11]  - packet sent upon firmware request.
 *	b[10]  - packet only contains signalling data.
 *	b[9]   - packet is a tx packet.
<<<<<<< HEAD
 *	b[8]   - packet uses FIFO credit (non-pspoll).
 *	b[7]   - interface in AP mode.
 *	b[6:4] - AC FIFO number.
=======
 *	b[8]   - packet used requested credit
 *	b[7]   - interface in AP mode.
>>>>>>> v3.18
 *	b[3:0] - interface index.
 */
#define BRCMF_SKB_IF_FLAGS_REQUESTED_MASK	0x0800
#define BRCMF_SKB_IF_FLAGS_REQUESTED_SHIFT	11
#define BRCMF_SKB_IF_FLAGS_SIGNAL_ONLY_MASK	0x0400
#define BRCMF_SKB_IF_FLAGS_SIGNAL_ONLY_SHIFT	10
#define BRCMF_SKB_IF_FLAGS_TRANSMIT_MASK        0x0200
#define BRCMF_SKB_IF_FLAGS_TRANSMIT_SHIFT	9
<<<<<<< HEAD
#define BRCMF_SKB_IF_FLAGS_CREDITCHECK_MASK	0x0100
#define BRCMF_SKB_IF_FLAGS_CREDITCHECK_SHIFT	8
#define BRCMF_SKB_IF_FLAGS_IF_AP_MASK		0x0080
#define BRCMF_SKB_IF_FLAGS_IF_AP_SHIFT		7
#define BRCMF_SKB_IF_FLAGS_FIFO_MASK		0x0070
#define BRCMF_SKB_IF_FLAGS_FIFO_SHIFT		4
=======
#define BRCMF_SKB_IF_FLAGS_REQ_CREDIT_MASK	0x0100
#define BRCMF_SKB_IF_FLAGS_REQ_CREDIT_SHIFT	8
#define BRCMF_SKB_IF_FLAGS_IF_AP_MASK		0x0080
#define BRCMF_SKB_IF_FLAGS_IF_AP_SHIFT		7
>>>>>>> v3.18
#define BRCMF_SKB_IF_FLAGS_INDEX_MASK		0x000f
#define BRCMF_SKB_IF_FLAGS_INDEX_SHIFT		0

#define brcmf_skb_if_flags_set_field(skb, field, value) \
	brcmu_maskset16(&(brcmf_skbcb(skb)->if_flags), \
			BRCMF_SKB_IF_FLAGS_ ## field ## _MASK, \
			BRCMF_SKB_IF_FLAGS_ ## field ## _SHIFT, (value))
#define brcmf_skb_if_flags_get_field(skb, field) \
	brcmu_maskget16(brcmf_skbcb(skb)->if_flags, \
			BRCMF_SKB_IF_FLAGS_ ## field ## _MASK, \
			BRCMF_SKB_IF_FLAGS_ ## field ## _SHIFT)

/*
 * sk_buff control packet identifier
 *
 * 32-bit packet identifier used in PKTTAG tlv from host to dongle.
 *
 * - Generated at the host (e.g. dhd)
 * - Seen as a generic sequence number by firmware except for the flags field.
 *
 * Generation	: b[31]	=> generation number for this packet [host->fw]
 *			   OR, current generation number [fw->host]
 * Flags	: b[30:27] => command, status flags
 * FIFO-AC	: b[26:24] => AC-FIFO id
 * h-slot	: b[23:8] => hanger-slot
 * freerun	: b[7:0] => A free running counter
 */
#define BRCMF_SKB_HTOD_TAG_GENERATION_MASK		0x80000000
#define BRCMF_SKB_HTOD_TAG_GENERATION_SHIFT		31
#define BRCMF_SKB_HTOD_TAG_FLAGS_MASK			0x78000000
#define BRCMF_SKB_HTOD_TAG_FLAGS_SHIFT			27
#define BRCMF_SKB_HTOD_TAG_FIFO_MASK			0x07000000
#define BRCMF_SKB_HTOD_TAG_FIFO_SHIFT			24
#define BRCMF_SKB_HTOD_TAG_HSLOT_MASK			0x00ffff00
#define BRCMF_SKB_HTOD_TAG_HSLOT_SHIFT			8
#define BRCMF_SKB_HTOD_TAG_FREERUN_MASK			0x000000ff
<<<<<<< HEAD
#define BRCMF_SKB_HTOD_TAG_FREERUN_SHIFT			0
=======
#define BRCMF_SKB_HTOD_TAG_FREERUN_SHIFT		0
>>>>>>> v3.18

#define brcmf_skb_htod_tag_set_field(skb, field, value) \
	brcmu_maskset32(&(brcmf_skbcb(skb)->htod), \
			BRCMF_SKB_HTOD_TAG_ ## field ## _MASK, \
			BRCMF_SKB_HTOD_TAG_ ## field ## _SHIFT, (value))
#define brcmf_skb_htod_tag_get_field(skb, field) \
	brcmu_maskget32(brcmf_skbcb(skb)->htod, \
			BRCMF_SKB_HTOD_TAG_ ## field ## _MASK, \
			BRCMF_SKB_HTOD_TAG_ ## field ## _SHIFT)

<<<<<<< HEAD
=======
#define BRCMF_SKB_HTOD_SEQ_FROMFW_MASK			0x2000
#define BRCMF_SKB_HTOD_SEQ_FROMFW_SHIFT			13
#define BRCMF_SKB_HTOD_SEQ_FROMDRV_MASK			0x1000
#define BRCMF_SKB_HTOD_SEQ_FROMDRV_SHIFT		12
#define BRCMF_SKB_HTOD_SEQ_NR_MASK			0x0fff
#define BRCMF_SKB_HTOD_SEQ_NR_SHIFT			0

#define brcmf_skb_htod_seq_set_field(skb, field, value) \
	brcmu_maskset16(&(brcmf_skbcb(skb)->htod_seq), \
			BRCMF_SKB_HTOD_SEQ_ ## field ## _MASK, \
			BRCMF_SKB_HTOD_SEQ_ ## field ## _SHIFT, (value))
#define brcmf_skb_htod_seq_get_field(skb, field) \
	brcmu_maskget16(brcmf_skbcb(skb)->htod_seq, \
			BRCMF_SKB_HTOD_SEQ_ ## field ## _MASK, \
			BRCMF_SKB_HTOD_SEQ_ ## field ## _SHIFT)

>>>>>>> v3.18
#define BRCMF_FWS_TXSTAT_GENERATION_MASK	0x80000000
#define BRCMF_FWS_TXSTAT_GENERATION_SHIFT	31
#define BRCMF_FWS_TXSTAT_FLAGS_MASK		0x78000000
#define BRCMF_FWS_TXSTAT_FLAGS_SHIFT		27
#define BRCMF_FWS_TXSTAT_FIFO_MASK		0x07000000
#define BRCMF_FWS_TXSTAT_FIFO_SHIFT		24
#define BRCMF_FWS_TXSTAT_HSLOT_MASK		0x00FFFF00
#define BRCMF_FWS_TXSTAT_HSLOT_SHIFT		8
<<<<<<< HEAD
#define BRCMF_FWS_TXSTAT_PKTID_MASK		0x00FFFFFF
#define BRCMF_FWS_TXSTAT_PKTID_SHIFT		0
=======
#define BRCMF_FWS_TXSTAT_FREERUN_MASK		0x000000FF
#define BRCMF_FWS_TXSTAT_FREERUN_SHIFT		0
>>>>>>> v3.18

#define brcmf_txstatus_get_field(txs, field) \
	brcmu_maskget32(txs, BRCMF_FWS_TXSTAT_ ## field ## _MASK, \
			BRCMF_FWS_TXSTAT_ ## field ## _SHIFT)

/* How long to defer borrowing in jiffies */
#define BRCMF_FWS_BORROW_DEFER_PERIOD		(HZ / 10)

/**
 * enum brcmf_fws_fifo - fifo indices used by dongle firmware.
 *
<<<<<<< HEAD
=======
 * @BRCMF_FWS_FIFO_FIRST: first fifo, ie. background.
>>>>>>> v3.18
 * @BRCMF_FWS_FIFO_AC_BK: fifo for background traffic.
 * @BRCMF_FWS_FIFO_AC_BE: fifo for best-effort traffic.
 * @BRCMF_FWS_FIFO_AC_VI: fifo for video traffic.
 * @BRCMF_FWS_FIFO_AC_VO: fifo for voice traffic.
 * @BRCMF_FWS_FIFO_BCMC: fifo for broadcast/multicast (AP only).
 * @BRCMF_FWS_FIFO_ATIM: fifo for ATIM (AP only).
 * @BRCMF_FWS_FIFO_COUNT: number of fifos.
 */
enum brcmf_fws_fifo {
<<<<<<< HEAD
	BRCMF_FWS_FIFO_AC_BK,
=======
	BRCMF_FWS_FIFO_FIRST,
	BRCMF_FWS_FIFO_AC_BK = BRCMF_FWS_FIFO_FIRST,
>>>>>>> v3.18
	BRCMF_FWS_FIFO_AC_BE,
	BRCMF_FWS_FIFO_AC_VI,
	BRCMF_FWS_FIFO_AC_VO,
	BRCMF_FWS_FIFO_BCMC,
	BRCMF_FWS_FIFO_ATIM,
	BRCMF_FWS_FIFO_COUNT
};

/**
 * enum brcmf_fws_txstatus - txstatus flag values.
 *
 * @BRCMF_FWS_TXSTATUS_DISCARD:
 *	host is free to discard the packet.
 * @BRCMF_FWS_TXSTATUS_CORE_SUPPRESS:
 *	802.11 core suppressed the packet.
 * @BRCMF_FWS_TXSTATUS_FW_PS_SUPPRESS:
 *	firmware suppress the packet as device is already in PS mode.
 * @BRCMF_FWS_TXSTATUS_FW_TOSSED:
 *	firmware tossed the packet.
<<<<<<< HEAD
=======
 * @BRCMF_FWS_TXSTATUS_HOST_TOSSED:
 *	host tossed the packet.
>>>>>>> v3.18
 */
enum brcmf_fws_txstatus {
	BRCMF_FWS_TXSTATUS_DISCARD,
	BRCMF_FWS_TXSTATUS_CORE_SUPPRESS,
	BRCMF_FWS_TXSTATUS_FW_PS_SUPPRESS,
<<<<<<< HEAD
	BRCMF_FWS_TXSTATUS_FW_TOSSED
=======
	BRCMF_FWS_TXSTATUS_FW_TOSSED,
	BRCMF_FWS_TXSTATUS_HOST_TOSSED
>>>>>>> v3.18
};

enum brcmf_fws_fcmode {
	BRCMF_FWS_FCMODE_NONE,
	BRCMF_FWS_FCMODE_IMPLIED_CREDIT,
	BRCMF_FWS_FCMODE_EXPLICIT_CREDIT
};

enum brcmf_fws_mac_desc_state {
	BRCMF_FWS_STATE_OPEN = 1,
	BRCMF_FWS_STATE_CLOSE
};

/**
 * struct brcmf_fws_mac_descriptor - firmware signalling data per node/interface
 *
 * @occupied: slot is in use.
 * @mac_handle: handle for mac entry determined by firmware.
 * @interface_id: interface index.
 * @state: current state.
 * @suppressed: mac entry is suppressed.
 * @generation: generation bit.
 * @ac_bitmap: ac queue bitmap.
 * @requested_credit: credits requested by firmware.
 * @ea: ethernet address.
 * @seq: per-node free-running sequence.
 * @psq: power-save queue.
 * @transit_count: packet in transit to firmware.
 */
struct brcmf_fws_mac_descriptor {
<<<<<<< HEAD
=======
	char name[16];
>>>>>>> v3.18
	u8 occupied;
	u8 mac_handle;
	u8 interface_id;
	u8 state;
	bool suppressed;
	u8 generation;
	u8 ac_bitmap;
	u8 requested_credit;
	u8 requested_packet;
	u8 ea[ETH_ALEN];
	u8 seq[BRCMF_FWS_FIFO_COUNT];
	struct pktq psq;
	int transit_count;
<<<<<<< HEAD
	int suppress_count;
=======
>>>>>>> v3.18
	int suppr_transit_count;
	bool send_tim_signal;
	u8 traffic_pending_bmp;
	u8 traffic_lastreported_bmp;
};

#define BRCMF_FWS_HANGER_MAXITEMS	1024

/**
 * enum brcmf_fws_hanger_item_state - state of hanger item.
 *
 * @BRCMF_FWS_HANGER_ITEM_STATE_FREE: item is free for use.
 * @BRCMF_FWS_HANGER_ITEM_STATE_INUSE: item is in use.
 * @BRCMF_FWS_HANGER_ITEM_STATE_INUSE_SUPPRESSED: item was suppressed.
 */
enum brcmf_fws_hanger_item_state {
	BRCMF_FWS_HANGER_ITEM_STATE_FREE = 1,
	BRCMF_FWS_HANGER_ITEM_STATE_INUSE,
	BRCMF_FWS_HANGER_ITEM_STATE_INUSE_SUPPRESSED
};


/**
 * struct brcmf_fws_hanger_item - single entry for tx pending packet.
 *
 * @state: entry is either free or occupied.
<<<<<<< HEAD
 * @gen: generation.
=======
>>>>>>> v3.18
 * @pkt: packet itself.
 */
struct brcmf_fws_hanger_item {
	enum brcmf_fws_hanger_item_state state;
<<<<<<< HEAD
	u8 gen;
=======
>>>>>>> v3.18
	struct sk_buff *pkt;
};

/**
 * struct brcmf_fws_hanger - holds packets awaiting firmware txstatus.
 *
 * @pushed: packets pushed to await txstatus.
 * @popped: packets popped upon handling txstatus.
 * @failed_to_push: packets that could not be pushed.
 * @failed_to_pop: packets that could not be popped.
 * @failed_slotfind: packets for which failed to find an entry.
 * @slot_pos: last returned item index for a free entry.
 * @items: array of hanger items.
 */
struct brcmf_fws_hanger {
	u32 pushed;
	u32 popped;
	u32 failed_to_push;
	u32 failed_to_pop;
	u32 failed_slotfind;
	u32 slot_pos;
	struct brcmf_fws_hanger_item items[BRCMF_FWS_HANGER_MAXITEMS];
};

struct brcmf_fws_macdesc_table {
	struct brcmf_fws_mac_descriptor nodes[BRCMF_FWS_MAC_DESC_TABLE_SIZE];
	struct brcmf_fws_mac_descriptor iface[BRCMF_MAX_IFS];
	struct brcmf_fws_mac_descriptor other;
};

<<<<<<< HEAD
struct brcmf_fws_info {
	struct brcmf_pub *drvr;
	struct brcmf_fws_stats stats;
	struct brcmf_fws_hanger hanger;
	enum brcmf_fws_fcmode fcmode;
=======
struct brcmf_fws_stats {
	u32 tlv_parse_failed;
	u32 tlv_invalid_type;
	u32 header_only_pkt;
	u32 header_pulls;
	u32 pkt2bus;
	u32 send_pkts[5];
	u32 requested_sent[5];
	u32 generic_error;
	u32 mac_update_failed;
	u32 mac_ps_update_failed;
	u32 if_update_failed;
	u32 packet_request_failed;
	u32 credit_request_failed;
	u32 rollback_success;
	u32 rollback_failed;
	u32 delayq_full_error;
	u32 supprq_full_error;
	u32 txs_indicate;
	u32 txs_discard;
	u32 txs_supp_core;
	u32 txs_supp_ps;
	u32 txs_tossed;
	u32 txs_host_tossed;
	u32 bus_flow_block;
	u32 fws_flow_block;
};

struct brcmf_fws_info {
	struct brcmf_pub *drvr;
	spinlock_t spinlock;
	ulong flags;
	struct brcmf_fws_stats stats;
	struct brcmf_fws_hanger hanger;
	enum brcmf_fws_fcmode fcmode;
	bool fw_signals;
	bool bcmc_credit_check;
>>>>>>> v3.18
	struct brcmf_fws_macdesc_table desc;
	struct workqueue_struct *fws_wq;
	struct work_struct fws_dequeue_work;
	u32 fifo_enqpkt[BRCMF_FWS_FIFO_COUNT];
	int fifo_credit[BRCMF_FWS_FIFO_COUNT];
	int credits_borrowed[BRCMF_FWS_FIFO_AC_VO + 1];
	int deq_node_pos[BRCMF_FWS_FIFO_COUNT];
	u32 fifo_credit_map;
	u32 fifo_delay_map;
	unsigned long borrow_defer_timestamp;
<<<<<<< HEAD
=======
	bool bus_flow_blocked;
	bool creditmap_received;
	u8 mode;
	bool avoid_queueing;
>>>>>>> v3.18
};

/*
 * brcmf_fws_prio2fifo - mapping from 802.1d priority to firmware fifo index.
 */
static const int brcmf_fws_prio2fifo[] = {
	BRCMF_FWS_FIFO_AC_BE,
	BRCMF_FWS_FIFO_AC_BK,
	BRCMF_FWS_FIFO_AC_BK,
	BRCMF_FWS_FIFO_AC_BE,
	BRCMF_FWS_FIFO_AC_VI,
	BRCMF_FWS_FIFO_AC_VI,
	BRCMF_FWS_FIFO_AC_VO,
	BRCMF_FWS_FIFO_AC_VO
};

static int fcmode;
module_param(fcmode, int, S_IRUSR);
MODULE_PARM_DESC(fcmode, "mode of firmware signalled flow control");

#define BRCMF_FWS_TLV_DEF(name, id, len) \
	case BRCMF_FWS_TYPE_ ## name: \
		return len;

/**
 * brcmf_fws_get_tlv_len() - returns defined length for given tlv id.
 *
 * @fws: firmware-signalling information.
 * @id: identifier of the TLV.
 *
 * Return: the specified length for the given TLV; Otherwise -EINVAL.
 */
static int brcmf_fws_get_tlv_len(struct brcmf_fws_info *fws,
				 enum brcmf_fws_tlv_type id)
{
	switch (id) {
	BRCMF_FWS_TLV_DEFLIST
	default:
		fws->stats.tlv_invalid_type++;
		break;
	}
	return -EINVAL;
}
#undef BRCMF_FWS_TLV_DEF

<<<<<<< HEAD
=======
static void brcmf_fws_lock(struct brcmf_fws_info *fws)
		__acquires(&fws->spinlock)
{
	spin_lock_irqsave(&fws->spinlock, fws->flags);
}

static void brcmf_fws_unlock(struct brcmf_fws_info *fws)
		__releases(&fws->spinlock)
{
	spin_unlock_irqrestore(&fws->spinlock, fws->flags);
}

>>>>>>> v3.18
static bool brcmf_fws_ifidx_match(struct sk_buff *skb, void *arg)
{
	u32 ifidx = brcmf_skb_if_flags_get_field(skb, INDEX);
	return ifidx == *(int *)arg;
}

static void brcmf_fws_psq_flush(struct brcmf_fws_info *fws, struct pktq *q,
				int ifidx)
{
	bool (*matchfn)(struct sk_buff *, void *) = NULL;
	struct sk_buff *skb;
	int prec;

	if (ifidx != -1)
		matchfn = brcmf_fws_ifidx_match;
	for (prec = 0; prec < q->num_prec; prec++) {
		skb = brcmu_pktq_pdeq_match(q, prec, matchfn, &ifidx);
		while (skb) {
			brcmu_pkt_buf_free_skb(skb);
			skb = brcmu_pktq_pdeq_match(q, prec, matchfn, &ifidx);
		}
	}
}

static void brcmf_fws_hanger_init(struct brcmf_fws_hanger *hanger)
{
	int i;

<<<<<<< HEAD
	brcmf_dbg(TRACE, "enter\n");
=======
>>>>>>> v3.18
	memset(hanger, 0, sizeof(*hanger));
	for (i = 0; i < ARRAY_SIZE(hanger->items); i++)
		hanger->items[i].state = BRCMF_FWS_HANGER_ITEM_STATE_FREE;
}

static u32 brcmf_fws_hanger_get_free_slot(struct brcmf_fws_hanger *h)
{
	u32 i;

<<<<<<< HEAD
	brcmf_dbg(TRACE, "enter\n");
=======
>>>>>>> v3.18
	i = (h->slot_pos + 1) % BRCMF_FWS_HANGER_MAXITEMS;

	while (i != h->slot_pos) {
		if (h->items[i].state == BRCMF_FWS_HANGER_ITEM_STATE_FREE) {
			h->slot_pos = i;
			goto done;
		}
		i++;
		if (i == BRCMF_FWS_HANGER_MAXITEMS)
			i = 0;
	}
	brcmf_err("all slots occupied\n");
	h->failed_slotfind++;
	i = BRCMF_FWS_HANGER_MAXITEMS;
done:
<<<<<<< HEAD
	brcmf_dbg(TRACE, "exit: %d\n", i);
=======
>>>>>>> v3.18
	return i;
}

static int brcmf_fws_hanger_pushpkt(struct brcmf_fws_hanger *h,
<<<<<<< HEAD
					   struct sk_buff *pkt, u32 slot_id)
{
	brcmf_dbg(TRACE, "enter\n");
=======
				    struct sk_buff *pkt, u32 slot_id)
{
>>>>>>> v3.18
	if (slot_id >= BRCMF_FWS_HANGER_MAXITEMS)
		return -ENOENT;

	if (h->items[slot_id].state != BRCMF_FWS_HANGER_ITEM_STATE_FREE) {
		brcmf_err("slot is not free\n");
		h->failed_to_push++;
		return -EINVAL;
	}

	h->items[slot_id].state = BRCMF_FWS_HANGER_ITEM_STATE_INUSE;
	h->items[slot_id].pkt = pkt;
	h->pushed++;
	return 0;
}

static int brcmf_fws_hanger_poppkt(struct brcmf_fws_hanger *h,
					  u32 slot_id, struct sk_buff **pktout,
					  bool remove_item)
{
<<<<<<< HEAD
	brcmf_dbg(TRACE, "enter\n");
=======
>>>>>>> v3.18
	if (slot_id >= BRCMF_FWS_HANGER_MAXITEMS)
		return -ENOENT;

	if (h->items[slot_id].state == BRCMF_FWS_HANGER_ITEM_STATE_FREE) {
		brcmf_err("entry not in use\n");
		h->failed_to_pop++;
		return -EINVAL;
	}

	*pktout = h->items[slot_id].pkt;
	if (remove_item) {
		h->items[slot_id].state = BRCMF_FWS_HANGER_ITEM_STATE_FREE;
		h->items[slot_id].pkt = NULL;
<<<<<<< HEAD
		h->items[slot_id].gen = 0xff;
=======
>>>>>>> v3.18
		h->popped++;
	}
	return 0;
}

static int brcmf_fws_hanger_mark_suppressed(struct brcmf_fws_hanger *h,
<<<<<<< HEAD
						   u32 slot_id, u8 gen)
{
	brcmf_dbg(TRACE, "enter\n");

	if (slot_id >= BRCMF_FWS_HANGER_MAXITEMS)
		return -ENOENT;

	h->items[slot_id].gen = gen;

	if (h->items[slot_id].state != BRCMF_FWS_HANGER_ITEM_STATE_INUSE) {
=======
					    u32 slot_id)
{
	if (slot_id >= BRCMF_FWS_HANGER_MAXITEMS)
		return -ENOENT;

	if (h->items[slot_id].state == BRCMF_FWS_HANGER_ITEM_STATE_FREE) {
>>>>>>> v3.18
		brcmf_err("entry not in use\n");
		return -EINVAL;
	}

	h->items[slot_id].state = BRCMF_FWS_HANGER_ITEM_STATE_INUSE_SUPPRESSED;
	return 0;
}

<<<<<<< HEAD
static int brcmf_fws_hanger_get_genbit(struct brcmf_fws_hanger *hanger,
					      struct sk_buff *pkt, u32 slot_id,
					      int *gen)
{
	brcmf_dbg(TRACE, "enter\n");
	*gen = 0xff;

	if (slot_id >= BRCMF_FWS_HANGER_MAXITEMS)
		return -ENOENT;

	if (hanger->items[slot_id].state == BRCMF_FWS_HANGER_ITEM_STATE_FREE) {
		brcmf_err("slot not in use\n");
		return -EINVAL;
	}

	*gen = hanger->items[slot_id].gen;
	return 0;
}

=======
>>>>>>> v3.18
static void brcmf_fws_hanger_cleanup(struct brcmf_fws_info *fws,
				     bool (*fn)(struct sk_buff *, void *),
				     int ifidx)
{
	struct brcmf_fws_hanger *h = &fws->hanger;
	struct sk_buff *skb;
	int i;
	enum brcmf_fws_hanger_item_state s;

<<<<<<< HEAD
	brcmf_dbg(TRACE, "enter: ifidx=%d\n", ifidx);
=======
>>>>>>> v3.18
	for (i = 0; i < ARRAY_SIZE(h->items); i++) {
		s = h->items[i].state;
		if (s == BRCMF_FWS_HANGER_ITEM_STATE_INUSE ||
		    s == BRCMF_FWS_HANGER_ITEM_STATE_INUSE_SUPPRESSED) {
			skb = h->items[i].pkt;
			if (fn == NULL || fn(skb, &ifidx)) {
				/* suppress packets freed from psq */
				if (s == BRCMF_FWS_HANGER_ITEM_STATE_INUSE)
					brcmu_pkt_buf_free_skb(skb);
				h->items[i].state =
					BRCMF_FWS_HANGER_ITEM_STATE_FREE;
			}
		}
	}
}

<<<<<<< HEAD
static void brcmf_fws_init_mac_descriptor(struct brcmf_fws_mac_descriptor *desc,
					  u8 *addr, u8 ifidx)
=======
static void brcmf_fws_macdesc_set_name(struct brcmf_fws_info *fws,
				       struct brcmf_fws_mac_descriptor *desc)
{
	if (desc == &fws->desc.other)
		strlcpy(desc->name, "MAC-OTHER", sizeof(desc->name));
	else if (desc->mac_handle)
		scnprintf(desc->name, sizeof(desc->name), "MAC-%d:%d",
			  desc->mac_handle, desc->interface_id);
	else
		scnprintf(desc->name, sizeof(desc->name), "MACIF:%d",
			  desc->interface_id);
}

static void brcmf_fws_macdesc_init(struct brcmf_fws_mac_descriptor *desc,
				   u8 *addr, u8 ifidx)
>>>>>>> v3.18
{
	brcmf_dbg(TRACE,
		  "enter: desc %p ea=%pM, ifidx=%u\n", desc, addr, ifidx);
	desc->occupied = 1;
	desc->state = BRCMF_FWS_STATE_OPEN;
	desc->requested_credit = 0;
<<<<<<< HEAD
=======
	desc->requested_packet = 0;
>>>>>>> v3.18
	/* depending on use may need ifp->bssidx instead */
	desc->interface_id = ifidx;
	desc->ac_bitmap = 0xff; /* update this when handling APSD */
	if (addr)
		memcpy(&desc->ea[0], addr, ETH_ALEN);
}

static
<<<<<<< HEAD
void brcmf_fws_clear_mac_descriptor(struct brcmf_fws_mac_descriptor *desc)
=======
void brcmf_fws_macdesc_deinit(struct brcmf_fws_mac_descriptor *desc)
>>>>>>> v3.18
{
	brcmf_dbg(TRACE,
		  "enter: ea=%pM, ifidx=%u\n", desc->ea, desc->interface_id);
	desc->occupied = 0;
	desc->state = BRCMF_FWS_STATE_CLOSE;
	desc->requested_credit = 0;
<<<<<<< HEAD
}

static struct brcmf_fws_mac_descriptor *
brcmf_fws_mac_descriptor_lookup(struct brcmf_fws_info *fws, u8 *ea)
=======
	desc->requested_packet = 0;
}

static struct brcmf_fws_mac_descriptor *
brcmf_fws_macdesc_lookup(struct brcmf_fws_info *fws, u8 *ea)
>>>>>>> v3.18
{
	struct brcmf_fws_mac_descriptor *entry;
	int i;

<<<<<<< HEAD
	brcmf_dbg(TRACE, "enter: ea=%pM\n", ea);
=======
>>>>>>> v3.18
	if (ea == NULL)
		return ERR_PTR(-EINVAL);

	entry = &fws->desc.nodes[0];
	for (i = 0; i < ARRAY_SIZE(fws->desc.nodes); i++) {
		if (entry->occupied && !memcmp(entry->ea, ea, ETH_ALEN))
			return entry;
		entry++;
	}

	return ERR_PTR(-ENOENT);
}

static struct brcmf_fws_mac_descriptor*
<<<<<<< HEAD
brcmf_fws_find_mac_desc(struct brcmf_fws_info *fws, struct brcmf_if *ifp,
			u8 *da)
{
	struct brcmf_fws_mac_descriptor *entry = &fws->desc.other;
	bool multicast;
	enum nl80211_iftype iftype;

	brcmf_dbg(TRACE, "enter: idx=%d\n", ifp->bssidx);

	multicast = is_multicast_ether_addr(da);
	iftype = brcmf_cfg80211_get_iftype(ifp);

	/* Multicast destination and P2P clients get the interface entry.
	 * STA gets the interface entry if there is no exact match. For
	 * example, TDLS destinations have their own entry.
	 */
	entry = NULL;
	if ((multicast || iftype == NL80211_IFTYPE_STATION ||
	     iftype == NL80211_IFTYPE_P2P_CLIENT) && ifp->fws_desc)
		entry = ifp->fws_desc;

	if (entry != NULL && iftype != NL80211_IFTYPE_STATION)
		goto done;

	entry = brcmf_fws_mac_descriptor_lookup(fws, da);
	if (IS_ERR(entry))
		entry = &fws->desc.other;

done:
	brcmf_dbg(TRACE, "exit: entry=%p\n", entry);
	return entry;
}

static bool brcmf_fws_mac_desc_closed(struct brcmf_fws_info *fws,
				      struct brcmf_fws_mac_descriptor *entry,
				      int fifo)
=======
brcmf_fws_macdesc_find(struct brcmf_fws_info *fws, struct brcmf_if *ifp, u8 *da)
{
	struct brcmf_fws_mac_descriptor *entry = &fws->desc.other;
	bool multicast;

	multicast = is_multicast_ether_addr(da);

	/* Multicast destination, STA and P2P clients get the interface entry.
	 * STA/GC gets the Mac Entry for TDLS destinations, TDLS destinations
	 * have their own entry.
	 */
	if (multicast && ifp->fws_desc) {
		entry = ifp->fws_desc;
		goto done;
	}

	entry = brcmf_fws_macdesc_lookup(fws, da);
	if (IS_ERR(entry))
		entry = ifp->fws_desc;

done:
	return entry;
}

static bool brcmf_fws_macdesc_closed(struct brcmf_fws_info *fws,
				     struct brcmf_fws_mac_descriptor *entry,
				     int fifo)
>>>>>>> v3.18
{
	struct brcmf_fws_mac_descriptor *if_entry;
	bool closed;

	/* for unique destination entries the related interface
	 * may be closed.
	 */
	if (entry->mac_handle) {
		if_entry = &fws->desc.iface[entry->interface_id];
		if (if_entry->state == BRCMF_FWS_STATE_CLOSE)
			return true;
	}
	/* an entry is closed when the state is closed and
	 * the firmware did not request anything.
	 */
	closed = entry->state == BRCMF_FWS_STATE_CLOSE &&
		 !entry->requested_credit && !entry->requested_packet;

	/* Or firmware does not allow traffic for given fifo */
	return closed || !(entry->ac_bitmap & BIT(fifo));
}

<<<<<<< HEAD
static void brcmf_fws_mac_desc_cleanup(struct brcmf_fws_info *fws,
				       struct brcmf_fws_mac_descriptor *entry,
				       int ifidx)
{
	brcmf_dbg(TRACE, "enter: entry=(ea=%pM, ifid=%d), ifidx=%d\n",
		  entry->ea, entry->interface_id, ifidx);
	if (entry->occupied && (ifidx == -1 || ifidx == entry->interface_id)) {
		brcmf_dbg(TRACE, "flush psq: ifidx=%d, qlen=%d\n",
			  ifidx, entry->psq.len);
=======
static void brcmf_fws_macdesc_cleanup(struct brcmf_fws_info *fws,
				      struct brcmf_fws_mac_descriptor *entry,
				      int ifidx)
{
	if (entry->occupied && (ifidx == -1 || ifidx == entry->interface_id)) {
>>>>>>> v3.18
		brcmf_fws_psq_flush(fws, &entry->psq, ifidx);
		entry->occupied = !!(entry->psq.len);
	}
}

static void brcmf_fws_bus_txq_cleanup(struct brcmf_fws_info *fws,
				      bool (*fn)(struct sk_buff *, void *),
				      int ifidx)
{
	struct brcmf_fws_hanger_item *hi;
	struct pktq *txq;
	struct sk_buff *skb;
	int prec;
	u32 hslot;

<<<<<<< HEAD
	brcmf_dbg(TRACE, "enter: ifidx=%d\n", ifidx);
=======
>>>>>>> v3.18
	txq = brcmf_bus_gettxq(fws->drvr->bus_if);
	if (IS_ERR(txq)) {
		brcmf_dbg(TRACE, "no txq to clean up\n");
		return;
	}

	for (prec = 0; prec < txq->num_prec; prec++) {
		skb = brcmu_pktq_pdeq_match(txq, prec, fn, &ifidx);
		while (skb) {
			hslot = brcmf_skb_htod_tag_get_field(skb, HSLOT);
			hi = &fws->hanger.items[hslot];
			WARN_ON(skb != hi->pkt);
			hi->state = BRCMF_FWS_HANGER_ITEM_STATE_FREE;
			brcmu_pkt_buf_free_skb(skb);
			skb = brcmu_pktq_pdeq_match(txq, prec, fn, &ifidx);
		}
	}
}

static void brcmf_fws_cleanup(struct brcmf_fws_info *fws, int ifidx)
{
	int i;
	struct brcmf_fws_mac_descriptor *table;
	bool (*matchfn)(struct sk_buff *, void *) = NULL;

<<<<<<< HEAD
	brcmf_dbg(TRACE, "enter: ifidx=%d\n", ifidx);
=======
>>>>>>> v3.18
	if (fws == NULL)
		return;

	if (ifidx != -1)
		matchfn = brcmf_fws_ifidx_match;

	/* cleanup individual nodes */
	table = &fws->desc.nodes[0];
	for (i = 0; i < ARRAY_SIZE(fws->desc.nodes); i++)
<<<<<<< HEAD
		brcmf_fws_mac_desc_cleanup(fws, &table[i], ifidx);

	brcmf_fws_mac_desc_cleanup(fws, &fws->desc.other, ifidx);
=======
		brcmf_fws_macdesc_cleanup(fws, &table[i], ifidx);

	brcmf_fws_macdesc_cleanup(fws, &fws->desc.other, ifidx);
>>>>>>> v3.18
	brcmf_fws_bus_txq_cleanup(fws, matchfn, ifidx);
	brcmf_fws_hanger_cleanup(fws, matchfn, ifidx);
}

<<<<<<< HEAD
static void brcmf_fws_tim_update(struct brcmf_fws_info *ctx,
				 struct brcmf_fws_mac_descriptor *entry,
				 int prec)
{
	brcmf_dbg(TRACE, "enter: ea=%pM\n", entry->ea);
	if (entry->state == BRCMF_FWS_STATE_CLOSE) {
		/* check delayedQ and suppressQ in one call using bitmap */
		if (brcmu_pktq_mlen(&entry->psq, 3 << (prec * 2)) == 0)
			entry->traffic_pending_bmp =
				entry->traffic_pending_bmp & ~NBITVAL(prec);
		else
			entry->traffic_pending_bmp =
				entry->traffic_pending_bmp | NBITVAL(prec);
	}
	/* request a TIM update to firmware at the next piggyback opportunity */
	if (entry->traffic_lastreported_bmp != entry->traffic_pending_bmp)
		entry->send_tim_signal = true;
=======
static u8 brcmf_fws_hdrpush(struct brcmf_fws_info *fws, struct sk_buff *skb)
{
	struct brcmf_fws_mac_descriptor *entry = brcmf_skbcb(skb)->mac;
	u8 *wlh;
	u16 data_offset = 0;
	u8 fillers;
	__le32 pkttag = cpu_to_le32(brcmf_skbcb(skb)->htod);
	__le16 pktseq = cpu_to_le16(brcmf_skbcb(skb)->htod_seq);

	brcmf_dbg(TRACE, "enter: %s, idx=%d hslot=%d htod %X seq %X\n",
		  entry->name, brcmf_skb_if_flags_get_field(skb, INDEX),
		  (le32_to_cpu(pkttag) >> 8) & 0xffff,
		  brcmf_skbcb(skb)->htod, brcmf_skbcb(skb)->htod_seq);
	if (entry->send_tim_signal)
		data_offset += 2 + BRCMF_FWS_TYPE_PENDING_TRAFFIC_BMP_LEN;
	if (BRCMF_FWS_MODE_GET_REUSESEQ(fws->mode))
		data_offset += BRCMF_FWS_TYPE_SEQ_LEN;
	/* +2 is for Type[1] and Len[1] in TLV, plus TIM signal */
	data_offset += 2 + BRCMF_FWS_TYPE_PKTTAG_LEN;
	fillers = round_up(data_offset, 4) - data_offset;
	data_offset += fillers;

	skb_push(skb, data_offset);
	wlh = skb->data;

	wlh[0] = BRCMF_FWS_TYPE_PKTTAG;
	wlh[1] = BRCMF_FWS_TYPE_PKTTAG_LEN;
	memcpy(&wlh[2], &pkttag, sizeof(pkttag));
	if (BRCMF_FWS_MODE_GET_REUSESEQ(fws->mode)) {
		wlh[1] += BRCMF_FWS_TYPE_SEQ_LEN;
		memcpy(&wlh[2 + BRCMF_FWS_TYPE_PKTTAG_LEN], &pktseq,
		       sizeof(pktseq));
	}
	wlh += wlh[1] + 2;

	if (entry->send_tim_signal) {
		entry->send_tim_signal = 0;
		wlh[0] = BRCMF_FWS_TYPE_PENDING_TRAFFIC_BMP;
		wlh[1] = BRCMF_FWS_TYPE_PENDING_TRAFFIC_BMP_LEN;
		wlh[2] = entry->mac_handle;
		wlh[3] = entry->traffic_pending_bmp;
		brcmf_dbg(TRACE, "adding TIM info: handle %d bmp 0x%X\n",
			  entry->mac_handle, entry->traffic_pending_bmp);
		wlh += BRCMF_FWS_TYPE_PENDING_TRAFFIC_BMP_LEN + 2;
		entry->traffic_lastreported_bmp = entry->traffic_pending_bmp;
	}
	if (fillers)
		memset(wlh, BRCMF_FWS_TYPE_FILLER, fillers);

	return (u8)(data_offset >> 2);
}

static bool brcmf_fws_tim_update(struct brcmf_fws_info *fws,
				 struct brcmf_fws_mac_descriptor *entry,
				 int fifo, bool send_immediately)
{
	struct sk_buff *skb;
	struct brcmf_skbuff_cb *skcb;
	s32 err;
	u32 len;
	u8 data_offset;
	int ifidx;

	/* check delayedQ and suppressQ in one call using bitmap */
	if (brcmu_pktq_mlen(&entry->psq, 3 << (fifo * 2)) == 0)
		entry->traffic_pending_bmp &= ~NBITVAL(fifo);
	else
		entry->traffic_pending_bmp |= NBITVAL(fifo);

	entry->send_tim_signal = false;
	if (entry->traffic_lastreported_bmp != entry->traffic_pending_bmp)
		entry->send_tim_signal = true;
	if (send_immediately && entry->send_tim_signal &&
	    entry->state == BRCMF_FWS_STATE_CLOSE) {
		/* create a dummy packet and sent that. The traffic          */
		/* bitmap info will automatically be attached to that packet */
		len = BRCMF_FWS_TYPE_PKTTAG_LEN + 2 +
		      BRCMF_FWS_TYPE_SEQ_LEN +
		      BRCMF_FWS_TYPE_PENDING_TRAFFIC_BMP_LEN + 2 +
		      4 + fws->drvr->hdrlen;
		skb = brcmu_pkt_buf_get_skb(len);
		if (skb == NULL)
			return false;
		skb_pull(skb, len);
		skcb = brcmf_skbcb(skb);
		skcb->mac = entry;
		skcb->state = BRCMF_FWS_SKBSTATE_TIM;
		skcb->htod = 0;
		skcb->htod_seq = 0;
		data_offset = brcmf_fws_hdrpush(fws, skb);
		ifidx = brcmf_skb_if_flags_get_field(skb, INDEX);
		brcmf_fws_unlock(fws);
		err = brcmf_proto_txdata(fws->drvr, ifidx, data_offset, skb);
		brcmf_fws_lock(fws);
		if (err)
			brcmu_pkt_buf_free_skb(skb);
		return true;
	}
	return false;
>>>>>>> v3.18
}

static void
brcmf_fws_flow_control_check(struct brcmf_fws_info *fws, struct pktq *pq,
			     u8 if_id)
{
<<<<<<< HEAD
	struct brcmf_if *ifp = fws->drvr->iflist[if_id];
=======
	struct brcmf_if *ifp = fws->drvr->iflist[!if_id ? 0 : if_id + 1];
>>>>>>> v3.18

	if (WARN_ON(!ifp))
		return;

<<<<<<< HEAD
	brcmf_dbg(TRACE,
		  "enter: bssidx=%d, ifidx=%d\n", ifp->bssidx, ifp->ifidx);

=======
>>>>>>> v3.18
	if ((ifp->netif_stop & BRCMF_NETIF_STOP_REASON_FWS_FC) &&
	    pq->len <= BRCMF_FWS_FLOWCONTROL_LOWATER)
		brcmf_txflowblock_if(ifp,
				     BRCMF_NETIF_STOP_REASON_FWS_FC, false);
	if (!(ifp->netif_stop & BRCMF_NETIF_STOP_REASON_FWS_FC) &&
<<<<<<< HEAD
	    pq->len >= BRCMF_FWS_FLOWCONTROL_HIWATER)
		brcmf_txflowblock_if(ifp, BRCMF_NETIF_STOP_REASON_FWS_FC, true);
=======
	    pq->len >= BRCMF_FWS_FLOWCONTROL_HIWATER) {
		fws->stats.fws_flow_block++;
		brcmf_txflowblock_if(ifp, BRCMF_NETIF_STOP_REASON_FWS_FC, true);
	}
>>>>>>> v3.18
	return;
}

static int brcmf_fws_rssi_indicate(struct brcmf_fws_info *fws, s8 rssi)
{
	brcmf_dbg(CTL, "rssi %d\n", rssi);
	return 0;
}

static
int brcmf_fws_macdesc_indicate(struct brcmf_fws_info *fws, u8 type, u8 *data)
{
	struct brcmf_fws_mac_descriptor *entry, *existing;
	u8 mac_handle;
	u8 ifidx;
	u8 *addr;

	mac_handle = *data++;
	ifidx = *data++;
	addr = data;

	entry = &fws->desc.nodes[mac_handle & 0x1F];
	if (type == BRCMF_FWS_TYPE_MACDESC_DEL) {
<<<<<<< HEAD
		brcmf_dbg(TRACE, "deleting mac %pM idx %d\n", addr, ifidx);
		if (entry->occupied) {
			brcmf_fws_mac_desc_cleanup(fws, entry, -1);
			brcmf_fws_clear_mac_descriptor(entry);
=======
		if (entry->occupied) {
			brcmf_dbg(TRACE, "deleting %s mac %pM\n",
				  entry->name, addr);
			brcmf_fws_lock(fws);
			brcmf_fws_macdesc_cleanup(fws, entry, -1);
			brcmf_fws_macdesc_deinit(entry);
			brcmf_fws_unlock(fws);
>>>>>>> v3.18
		} else
			fws->stats.mac_update_failed++;
		return 0;
	}

<<<<<<< HEAD
	brcmf_dbg(TRACE,
		  "add mac %pM handle %u idx %d\n", addr, mac_handle, ifidx);
	existing = brcmf_fws_mac_descriptor_lookup(fws, addr);
	if (IS_ERR(existing)) {
		if (!entry->occupied) {
			entry->mac_handle = mac_handle;
			brcmf_fws_init_mac_descriptor(entry, addr, ifidx);
			brcmu_pktq_init(&entry->psq, BRCMF_FWS_PSQ_PREC_COUNT,
					BRCMF_FWS_PSQ_LEN);
=======
	existing = brcmf_fws_macdesc_lookup(fws, addr);
	if (IS_ERR(existing)) {
		if (!entry->occupied) {
			brcmf_fws_lock(fws);
			entry->mac_handle = mac_handle;
			brcmf_fws_macdesc_init(entry, addr, ifidx);
			brcmf_fws_macdesc_set_name(fws, entry);
			brcmu_pktq_init(&entry->psq, BRCMF_FWS_PSQ_PREC_COUNT,
					BRCMF_FWS_PSQ_LEN);
			brcmf_fws_unlock(fws);
			brcmf_dbg(TRACE, "add %s mac %pM\n", entry->name, addr);
>>>>>>> v3.18
		} else {
			fws->stats.mac_update_failed++;
		}
	} else {
		if (entry != existing) {
<<<<<<< HEAD
			brcmf_dbg(TRACE, "relocate mac\n");
			memcpy(entry, existing,
			       offsetof(struct brcmf_fws_mac_descriptor, psq));
			entry->mac_handle = mac_handle;
			brcmf_fws_clear_mac_descriptor(existing);
=======
			brcmf_dbg(TRACE, "copy mac %s\n", existing->name);
			brcmf_fws_lock(fws);
			memcpy(entry, existing,
			       offsetof(struct brcmf_fws_mac_descriptor, psq));
			entry->mac_handle = mac_handle;
			brcmf_fws_macdesc_deinit(existing);
			brcmf_fws_macdesc_set_name(fws, entry);
			brcmf_fws_unlock(fws);
			brcmf_dbg(TRACE, "relocate %s mac %pM\n", entry->name,
				  addr);
>>>>>>> v3.18
		} else {
			brcmf_dbg(TRACE, "use existing\n");
			WARN_ON(entry->mac_handle != mac_handle);
			/* TODO: what should we do here: continue, reinit, .. */
		}
	}
	return 0;
}

static int brcmf_fws_macdesc_state_indicate(struct brcmf_fws_info *fws,
					    u8 type, u8 *data)
{
	struct brcmf_fws_mac_descriptor *entry;
	u8 mac_handle;
<<<<<<< HEAD
	int i;
=======
	int ret;
>>>>>>> v3.18

	mac_handle = data[0];
	entry = &fws->desc.nodes[mac_handle & 0x1F];
	if (!entry->occupied) {
		fws->stats.mac_ps_update_failed++;
		return -ESRCH;
	}
<<<<<<< HEAD

	/* a state update should wipe old credits? */
	entry->requested_credit = 0;
	if (type == BRCMF_FWS_TYPE_MAC_OPEN) {
		entry->state = BRCMF_FWS_STATE_OPEN;
		return BRCMF_FWS_RET_OK_SCHEDULE;
	} else {
		entry->state = BRCMF_FWS_STATE_CLOSE;
		for (i = BRCMF_FWS_FIFO_AC_BE; i < NL80211_NUM_ACS; i++)
			brcmf_fws_tim_update(fws, entry, i);
	}
	return BRCMF_FWS_RET_OK_NOSCHEDULE;
=======
	brcmf_fws_lock(fws);
	/* a state update should wipe old credits */
	entry->requested_credit = 0;
	entry->requested_packet = 0;
	if (type == BRCMF_FWS_TYPE_MAC_OPEN) {
		entry->state = BRCMF_FWS_STATE_OPEN;
		ret = BRCMF_FWS_RET_OK_SCHEDULE;
	} else {
		entry->state = BRCMF_FWS_STATE_CLOSE;
		brcmf_fws_tim_update(fws, entry, BRCMF_FWS_FIFO_AC_BK, false);
		brcmf_fws_tim_update(fws, entry, BRCMF_FWS_FIFO_AC_BE, false);
		brcmf_fws_tim_update(fws, entry, BRCMF_FWS_FIFO_AC_VI, false);
		brcmf_fws_tim_update(fws, entry, BRCMF_FWS_FIFO_AC_VO, true);
		ret = BRCMF_FWS_RET_OK_NOSCHEDULE;
	}
	brcmf_fws_unlock(fws);
	return ret;
>>>>>>> v3.18
}

static int brcmf_fws_interface_state_indicate(struct brcmf_fws_info *fws,
					      u8 type, u8 *data)
{
	struct brcmf_fws_mac_descriptor *entry;
	u8 ifidx;
	int ret;

	ifidx = data[0];

<<<<<<< HEAD
	brcmf_dbg(TRACE, "enter: ifidx=%d\n", ifidx);
=======
>>>>>>> v3.18
	if (ifidx >= BRCMF_MAX_IFS) {
		ret = -ERANGE;
		goto fail;
	}

	entry = &fws->desc.iface[ifidx];
	if (!entry->occupied) {
		ret = -ESRCH;
		goto fail;
	}

<<<<<<< HEAD
	switch (type) {
	case BRCMF_FWS_TYPE_INTERFACE_OPEN:
		entry->state = BRCMF_FWS_STATE_OPEN;
		return BRCMF_FWS_RET_OK_SCHEDULE;
	case BRCMF_FWS_TYPE_INTERFACE_CLOSE:
		entry->state = BRCMF_FWS_STATE_CLOSE;
		return BRCMF_FWS_RET_OK_NOSCHEDULE;
	default:
		ret = -EINVAL;
		break;
	}
=======
	brcmf_dbg(TRACE, "%s (%d): %s\n", brcmf_fws_get_tlv_name(type), type,
		  entry->name);
	brcmf_fws_lock(fws);
	switch (type) {
	case BRCMF_FWS_TYPE_INTERFACE_OPEN:
		entry->state = BRCMF_FWS_STATE_OPEN;
		ret = BRCMF_FWS_RET_OK_SCHEDULE;
		break;
	case BRCMF_FWS_TYPE_INTERFACE_CLOSE:
		entry->state = BRCMF_FWS_STATE_CLOSE;
		ret = BRCMF_FWS_RET_OK_NOSCHEDULE;
		break;
	default:
		ret = -EINVAL;
		brcmf_fws_unlock(fws);
		goto fail;
	}
	brcmf_fws_unlock(fws);
	return ret;

>>>>>>> v3.18
fail:
	fws->stats.if_update_failed++;
	return ret;
}

static int brcmf_fws_request_indicate(struct brcmf_fws_info *fws, u8 type,
				      u8 *data)
{
	struct brcmf_fws_mac_descriptor *entry;

	entry = &fws->desc.nodes[data[1] & 0x1F];
	if (!entry->occupied) {
		if (type == BRCMF_FWS_TYPE_MAC_REQUEST_CREDIT)
			fws->stats.credit_request_failed++;
		else
			fws->stats.packet_request_failed++;
		return -ESRCH;
	}

<<<<<<< HEAD
=======
	brcmf_dbg(TRACE, "%s (%d): %s cnt %d bmp %d\n",
		  brcmf_fws_get_tlv_name(type), type, entry->name,
		  data[0], data[2]);
	brcmf_fws_lock(fws);
>>>>>>> v3.18
	if (type == BRCMF_FWS_TYPE_MAC_REQUEST_CREDIT)
		entry->requested_credit = data[0];
	else
		entry->requested_packet = data[0];

	entry->ac_bitmap = data[2];
<<<<<<< HEAD
	return BRCMF_FWS_RET_OK_SCHEDULE;
}

=======
	brcmf_fws_unlock(fws);
	return BRCMF_FWS_RET_OK_SCHEDULE;
}

static void
brcmf_fws_macdesc_use_req_credit(struct brcmf_fws_mac_descriptor *entry,
				 struct sk_buff *skb)
{
	if (entry->requested_credit > 0) {
		entry->requested_credit--;
		brcmf_skb_if_flags_set_field(skb, REQUESTED, 1);
		brcmf_skb_if_flags_set_field(skb, REQ_CREDIT, 1);
		if (entry->state != BRCMF_FWS_STATE_CLOSE)
			brcmf_err("requested credit set while mac not closed!\n");
	} else if (entry->requested_packet > 0) {
		entry->requested_packet--;
		brcmf_skb_if_flags_set_field(skb, REQUESTED, 1);
		brcmf_skb_if_flags_set_field(skb, REQ_CREDIT, 0);
		if (entry->state != BRCMF_FWS_STATE_CLOSE)
			brcmf_err("requested packet set while mac not closed!\n");
	} else {
		brcmf_skb_if_flags_set_field(skb, REQUESTED, 0);
		brcmf_skb_if_flags_set_field(skb, REQ_CREDIT, 0);
	}
}

static void brcmf_fws_macdesc_return_req_credit(struct sk_buff *skb)
{
	struct brcmf_fws_mac_descriptor *entry = brcmf_skbcb(skb)->mac;

	if ((brcmf_skb_if_flags_get_field(skb, REQ_CREDIT)) &&
	    (entry->state == BRCMF_FWS_STATE_CLOSE))
		entry->requested_credit++;
}

>>>>>>> v3.18
static void brcmf_fws_return_credits(struct brcmf_fws_info *fws,
				     u8 fifo, u8 credits)
{
	int lender_ac;
	int *borrowed;
	int *fifo_credit;

	if (!credits)
		return;

<<<<<<< HEAD
=======
	fws->fifo_credit_map |= 1 << fifo;

>>>>>>> v3.18
	if ((fifo == BRCMF_FWS_FIFO_AC_BE) &&
	    (fws->credits_borrowed[0])) {
		for (lender_ac = BRCMF_FWS_FIFO_AC_VO; lender_ac >= 0;
		     lender_ac--) {
			borrowed = &fws->credits_borrowed[lender_ac];
			if (*borrowed) {
				fws->fifo_credit_map |= (1 << lender_ac);
				fifo_credit = &fws->fifo_credit[lender_ac];
				if (*borrowed >= credits) {
					*borrowed -= credits;
					*fifo_credit += credits;
					return;
				} else {
					credits -= *borrowed;
					*fifo_credit += *borrowed;
					*borrowed = 0;
				}
			}
		}
	}

<<<<<<< HEAD
	fws->fifo_credit_map |= 1 << fifo;
=======
>>>>>>> v3.18
	fws->fifo_credit[fifo] += credits;
}

static void brcmf_fws_schedule_deq(struct brcmf_fws_info *fws)
{
	/* only schedule dequeue when there are credits for delayed traffic */
<<<<<<< HEAD
	if (fws->fifo_credit_map & fws->fifo_delay_map)
		queue_work(fws->fws_wq, &fws->fws_dequeue_work);
}

static void brcmf_skb_pick_up_credit(struct brcmf_fws_info *fws, int fifo,
				     struct sk_buff *p)
{
	struct brcmf_fws_mac_descriptor *entry = brcmf_skbcb(p)->mac;

	if (brcmf_skbcb(p)->if_flags & BRCMF_SKB_IF_FLAGS_CREDITCHECK_MASK) {
		if (fws->fcmode != BRCMF_FWS_FCMODE_IMPLIED_CREDIT)
			return;
		brcmf_fws_return_credits(fws, fifo, 1);
	} else {
		/*
		 * if this packet did not count against FIFO credit, it
		 * must have taken a requested_credit from the destination
		 * entry (for pspoll etc.)
		 */
		if (!brcmf_skb_if_flags_get_field(p, REQUESTED))
			entry->requested_credit++;
	}
	brcmf_fws_schedule_deq(fws);
}

=======
	if ((fws->fifo_credit_map & fws->fifo_delay_map) ||
	    (!brcmf_fws_fc_active(fws) && fws->fifo_delay_map))
		queue_work(fws->fws_wq, &fws->fws_dequeue_work);
}

>>>>>>> v3.18
static int brcmf_fws_enq(struct brcmf_fws_info *fws,
			 enum brcmf_fws_skb_state state, int fifo,
			 struct sk_buff *p)
{
	int prec = 2 * fifo;
	u32 *qfull_stat = &fws->stats.delayq_full_error;
<<<<<<< HEAD

	struct brcmf_fws_mac_descriptor *entry;
=======
	struct brcmf_fws_mac_descriptor *entry;
	struct pktq *pq;
	struct sk_buff_head *queue;
	struct sk_buff *p_head;
	struct sk_buff *p_tail;
	u32 fr_new;
	u32 fr_compare;
>>>>>>> v3.18

	entry = brcmf_skbcb(p)->mac;
	if (entry == NULL) {
		brcmf_err("no mac descriptor found for skb %p\n", p);
		return -ENOENT;
	}

<<<<<<< HEAD
	brcmf_dbg(TRACE, "enter: ea=%pM, qlen=%d\n", entry->ea, entry->psq.len);
	if (state == BRCMF_FWS_SKBSTATE_SUPPRESSED) {
		prec += 1;
		qfull_stat = &fws->stats.supprq_full_error;
	}

	if (brcmu_pktq_penq(&entry->psq, prec, p) == NULL) {
=======
	brcmf_dbg(DATA, "enter: fifo %d skb %p\n", fifo, p);
	if (state == BRCMF_FWS_SKBSTATE_SUPPRESSED) {
		prec += 1;
		qfull_stat = &fws->stats.supprq_full_error;

		/* Fix out of order delivery of frames. Dont assume frame    */
		/* can be inserted at the end, but look for correct position */
		pq = &entry->psq;
		if (pktq_full(pq) || pktq_pfull(pq, prec)) {
			*qfull_stat += 1;
			return -ENFILE;
		}
		queue = &pq->q[prec].skblist;

		p_head = skb_peek(queue);
		p_tail = skb_peek_tail(queue);
		fr_new = brcmf_skb_htod_tag_get_field(p, FREERUN);

		while (p_head != p_tail) {
			fr_compare = brcmf_skb_htod_tag_get_field(p_tail,
								  FREERUN);
			/* be sure to handle wrap of 256 */
			if (((fr_new > fr_compare) &&
			     ((fr_new - fr_compare) < 128)) ||
			    ((fr_new < fr_compare) &&
			     ((fr_compare - fr_new) > 128)))
				break;
			p_tail = skb_queue_prev(queue, p_tail);
		}
		/* Position found. Determine what to do */
		if (p_tail == NULL) {
			/* empty list */
			__skb_queue_tail(queue, p);
		} else {
			fr_compare = brcmf_skb_htod_tag_get_field(p_tail,
								  FREERUN);
			if (((fr_new > fr_compare) &&
			     ((fr_new - fr_compare) < 128)) ||
			    ((fr_new < fr_compare) &&
			     ((fr_compare - fr_new) > 128))) {
				/* After tail */
				__skb_queue_after(queue, p_tail, p);
			} else {
				/* Before tail */
				__skb_insert(p, p_tail->prev, p_tail, queue);
			}
		}

		/* Complete the counters and statistics */
		pq->len++;
		if (pq->hi_prec < prec)
			pq->hi_prec = (u8) prec;
	} else if (brcmu_pktq_penq(&entry->psq, prec, p) == NULL) {
>>>>>>> v3.18
		*qfull_stat += 1;
		return -ENFILE;
	}

	/* increment total enqueued packet count */
	fws->fifo_delay_map |= 1 << fifo;
	fws->fifo_enqpkt[fifo]++;

	/* update the sk_buff state */
	brcmf_skbcb(p)->state = state;
<<<<<<< HEAD
	if (state == BRCMF_FWS_SKBSTATE_SUPPRESSED)
		entry->suppress_count++;
=======
>>>>>>> v3.18

	/*
	 * A packet has been pushed so update traffic
	 * availability bitmap, if applicable
	 */
<<<<<<< HEAD
	brcmf_fws_tim_update(fws, entry, fifo);
=======
	brcmf_fws_tim_update(fws, entry, fifo, true);
>>>>>>> v3.18
	brcmf_fws_flow_control_check(fws, &entry->psq,
				     brcmf_skb_if_flags_get_field(p, INDEX));
	return 0;
}

static struct sk_buff *brcmf_fws_deq(struct brcmf_fws_info *fws, int fifo)
{
	struct brcmf_fws_mac_descriptor *table;
	struct brcmf_fws_mac_descriptor *entry;
	struct sk_buff *p;
<<<<<<< HEAD
	int use_credit = 1;
=======
>>>>>>> v3.18
	int num_nodes;
	int node_pos;
	int prec_out;
	int pmsk;
	int i;

	table = (struct brcmf_fws_mac_descriptor *)&fws->desc;
	num_nodes = sizeof(fws->desc) / sizeof(struct brcmf_fws_mac_descriptor);
	node_pos = fws->deq_node_pos[fifo];

	for (i = 0; i < num_nodes; i++) {
		entry = &table[(node_pos + i) % num_nodes];
		if (!entry->occupied ||
<<<<<<< HEAD
		    brcmf_fws_mac_desc_closed(fws, entry, fifo))
=======
		    brcmf_fws_macdesc_closed(fws, entry, fifo))
>>>>>>> v3.18
			continue;

		if (entry->suppressed)
			pmsk = 2;
		else
			pmsk = 3;
		p = brcmu_pktq_mdeq(&entry->psq, pmsk << (fifo * 2), &prec_out);
		if (p == NULL) {
			if (entry->suppressed) {
<<<<<<< HEAD
				if (entry->suppr_transit_count >
				    entry->suppress_count)
					return NULL;
=======
				if (entry->suppr_transit_count)
					continue;
>>>>>>> v3.18
				entry->suppressed = false;
				p = brcmu_pktq_mdeq(&entry->psq,
						    1 << (fifo * 2), &prec_out);
			}
		}
		if  (p == NULL)
			continue;

<<<<<<< HEAD
		/* did the packet come from suppress sub-queue? */
		if (entry->requested_credit > 0) {
			entry->requested_credit--;
			/*
			 * if the packet was pulled out while destination is in
			 * closed state but had a non-zero packets requested,
			 * then this should not count against the FIFO credit.
			 * That is due to the fact that the firmware will
			 * most likely hold onto this packet until a suitable
			 * time later to push it to the appropriate AC FIFO.
			 */
			if (entry->state == BRCMF_FWS_STATE_CLOSE)
				use_credit = 0;
		} else if (entry->requested_packet > 0) {
			entry->requested_packet--;
			brcmf_skb_if_flags_set_field(p, REQUESTED, 1);
			if (entry->state == BRCMF_FWS_STATE_CLOSE)
				use_credit = 0;
		}
		brcmf_skb_if_flags_set_field(p, CREDITCHECK, use_credit);
=======
		brcmf_fws_macdesc_use_req_credit(entry, p);
>>>>>>> v3.18

		/* move dequeue position to ensure fair round-robin */
		fws->deq_node_pos[fifo] = (node_pos + i + 1) % num_nodes;
		brcmf_fws_flow_control_check(fws, &entry->psq,
					     brcmf_skb_if_flags_get_field(p,
									  INDEX)
					     );
		/*
		 * A packet has been picked up, update traffic
		 * availability bitmap, if applicable
		 */
<<<<<<< HEAD
		brcmf_fws_tim_update(fws, entry, fifo);
=======
		brcmf_fws_tim_update(fws, entry, fifo, false);
>>>>>>> v3.18

		/*
		 * decrement total enqueued fifo packets and
		 * clear delay bitmap if done.
		 */
		fws->fifo_enqpkt[fifo]--;
		if (fws->fifo_enqpkt[fifo] == 0)
			fws->fifo_delay_map &= ~(1 << fifo);
		goto done;
	}
	p = NULL;
done:
<<<<<<< HEAD
	brcmf_dbg(TRACE, "exit: fifo %d skb %p\n", fifo, p);
=======
	brcmf_dbg(DATA, "exit: fifo %d skb %p\n", fifo, p);
>>>>>>> v3.18
	return p;
}

static int brcmf_fws_txstatus_suppressed(struct brcmf_fws_info *fws, int fifo,
<<<<<<< HEAD
					 struct sk_buff *skb, u32 genbit)
=======
					 struct sk_buff *skb, u8 ifidx,
					 u32 genbit, u16 seq)
>>>>>>> v3.18
{
	struct brcmf_fws_mac_descriptor *entry = brcmf_skbcb(skb)->mac;
	u32 hslot;
	int ret;

	hslot = brcmf_skb_htod_tag_get_field(skb, HSLOT);

	/* this packet was suppressed */
<<<<<<< HEAD
	if (!entry->suppressed || entry->generation != genbit) {
		entry->suppressed = true;
		entry->suppress_count = brcmu_pktq_mlen(&entry->psq,
							1 << (fifo * 2 + 1));
		entry->suppr_transit_count = entry->transit_count;
=======
	if (!entry->suppressed) {
		entry->suppressed = true;
		entry->suppr_transit_count = entry->transit_count;
		brcmf_dbg(DATA, "suppress %s: transit %d\n",
			  entry->name, entry->transit_count);
>>>>>>> v3.18
	}

	entry->generation = genbit;

<<<<<<< HEAD
	ret = brcmf_fws_enq(fws, BRCMF_FWS_SKBSTATE_SUPPRESSED, fifo, skb);
	if (ret != 0) {
		/* suppress q is full, drop this packet */
		brcmf_fws_hanger_poppkt(&fws->hanger, hslot, &skb,
					true);
	} else {
		/*
		 * Mark suppressed to avoid a double free during
		 * wlfc cleanup
		 */
		brcmf_fws_hanger_mark_suppressed(&fws->hanger, hslot,
						 genbit);
		entry->suppress_count++;
=======
	brcmf_skb_htod_tag_set_field(skb, GENERATION, genbit);
	brcmf_skbcb(skb)->htod_seq = seq;
	if (brcmf_skb_htod_seq_get_field(skb, FROMFW)) {
		brcmf_skb_htod_seq_set_field(skb, FROMDRV, 1);
		brcmf_skb_htod_seq_set_field(skb, FROMFW, 0);
	} else {
		brcmf_skb_htod_seq_set_field(skb, FROMDRV, 0);
	}
	ret = brcmf_fws_enq(fws, BRCMF_FWS_SKBSTATE_SUPPRESSED, fifo, skb);

	if (ret != 0) {
		/* suppress q is full drop this packet */
		brcmf_fws_hanger_poppkt(&fws->hanger, hslot, &skb, true);
	} else {
		/* Mark suppressed to avoid a double free during wlfc cleanup */
		brcmf_fws_hanger_mark_suppressed(&fws->hanger, hslot);
>>>>>>> v3.18
	}

	return ret;
}

static int
<<<<<<< HEAD
brcmf_fws_txstatus_process(struct brcmf_fws_info *fws, u8 flags, u32 hslot,
			   u32 genbit)
=======
brcmf_fws_txs_process(struct brcmf_fws_info *fws, u8 flags, u32 hslot,
		      u32 genbit, u16 seq)
>>>>>>> v3.18
{
	u32 fifo;
	int ret;
	bool remove_from_hanger = true;
	struct sk_buff *skb;
<<<<<<< HEAD
	struct brcmf_fws_mac_descriptor *entry = NULL;

	brcmf_dbg(TRACE, "status: flags=0x%X, hslot=%d\n",
		  flags, hslot);
=======
	struct brcmf_skbuff_cb *skcb;
	struct brcmf_fws_mac_descriptor *entry = NULL;
	u8 ifidx;

	brcmf_dbg(DATA, "flags %d\n", flags);
>>>>>>> v3.18

	if (flags == BRCMF_FWS_TXSTATUS_DISCARD)
		fws->stats.txs_discard++;
	else if (flags == BRCMF_FWS_TXSTATUS_CORE_SUPPRESS) {
		fws->stats.txs_supp_core++;
		remove_from_hanger = false;
	} else if (flags == BRCMF_FWS_TXSTATUS_FW_PS_SUPPRESS) {
		fws->stats.txs_supp_ps++;
		remove_from_hanger = false;
	} else if (flags == BRCMF_FWS_TXSTATUS_FW_TOSSED)
		fws->stats.txs_tossed++;
<<<<<<< HEAD
=======
	else if (flags == BRCMF_FWS_TXSTATUS_HOST_TOSSED)
		fws->stats.txs_host_tossed++;
>>>>>>> v3.18
	else
		brcmf_err("unexpected txstatus\n");

	ret = brcmf_fws_hanger_poppkt(&fws->hanger, hslot, &skb,
				      remove_from_hanger);
	if (ret != 0) {
		brcmf_err("no packet in hanger slot: hslot=%d\n", hslot);
		return ret;
	}

<<<<<<< HEAD
	entry = brcmf_skbcb(skb)->mac;
=======
	skcb = brcmf_skbcb(skb);
	entry = skcb->mac;
>>>>>>> v3.18
	if (WARN_ON(!entry)) {
		brcmu_pkt_buf_free_skb(skb);
		return -EINVAL;
	}
<<<<<<< HEAD

	/* pick up the implicit credit from this packet */
	fifo = brcmf_skb_htod_tag_get_field(skb, FIFO);
	brcmf_skb_pick_up_credit(fws, fifo, skb);

	if (!remove_from_hanger)
		ret = brcmf_fws_txstatus_suppressed(fws, fifo, skb, genbit);

	if (remove_from_hanger || ret) {
		entry->transit_count--;
		if (entry->suppressed)
			entry->suppr_transit_count--;

		brcmf_txfinalize(fws->drvr, skb, true);
	}
=======
	entry->transit_count--;
	if (entry->suppressed && entry->suppr_transit_count)
		entry->suppr_transit_count--;

	brcmf_dbg(DATA, "%s flags %d htod %X seq %X\n", entry->name, flags,
		  skcb->htod, seq);

	/* pick up the implicit credit from this packet */
	fifo = brcmf_skb_htod_tag_get_field(skb, FIFO);
	if ((fws->fcmode == BRCMF_FWS_FCMODE_IMPLIED_CREDIT) ||
	    (brcmf_skb_if_flags_get_field(skb, REQ_CREDIT)) ||
	    (flags == BRCMF_FWS_TXSTATUS_HOST_TOSSED)) {
		brcmf_fws_return_credits(fws, fifo, 1);
		brcmf_fws_schedule_deq(fws);
	}
	brcmf_fws_macdesc_return_req_credit(skb);

	if (brcmf_proto_hdrpull(fws->drvr, false, &ifidx, skb)) {
		brcmu_pkt_buf_free_skb(skb);
		return -EINVAL;
	}
	if (!remove_from_hanger)
		ret = brcmf_fws_txstatus_suppressed(fws, fifo, skb, ifidx,
						    genbit, seq);
	if (remove_from_hanger || ret)
		brcmf_txfinalize(fws->drvr, skb, ifidx, true);

>>>>>>> v3.18
	return 0;
}

static int brcmf_fws_fifocreditback_indicate(struct brcmf_fws_info *fws,
					     u8 *data)
{
	int i;

	if (fws->fcmode != BRCMF_FWS_FCMODE_EXPLICIT_CREDIT) {
		brcmf_dbg(INFO, "ignored\n");
		return BRCMF_FWS_RET_OK_NOSCHEDULE;
	}

<<<<<<< HEAD
	brcmf_dbg(TRACE, "enter: data %pM\n", data);
	for (i = 0; i < BRCMF_FWS_FIFO_COUNT; i++)
		brcmf_fws_return_credits(fws, i, data[i]);

	brcmf_dbg(INFO, "map: credit %x delay %x\n", fws->fifo_credit_map,
		  fws->fifo_delay_map);
=======
	brcmf_dbg(DATA, "enter: data %pM\n", data);
	brcmf_fws_lock(fws);
	for (i = 0; i < BRCMF_FWS_FIFO_COUNT; i++)
		brcmf_fws_return_credits(fws, i, data[i]);

	brcmf_dbg(DATA, "map: credit %x delay %x\n", fws->fifo_credit_map,
		  fws->fifo_delay_map);
	brcmf_fws_unlock(fws);
>>>>>>> v3.18
	return BRCMF_FWS_RET_OK_SCHEDULE;
}

static int brcmf_fws_txstatus_indicate(struct brcmf_fws_info *fws, u8 *data)
{
	__le32 status_le;
<<<<<<< HEAD
=======
	__le16 seq_le;
>>>>>>> v3.18
	u32 status;
	u32 hslot;
	u32 genbit;
	u8 flags;
<<<<<<< HEAD
=======
	u16 seq;
>>>>>>> v3.18

	fws->stats.txs_indicate++;
	memcpy(&status_le, data, sizeof(status_le));
	status = le32_to_cpu(status_le);
	flags = brcmf_txstatus_get_field(status, FLAGS);
	hslot = brcmf_txstatus_get_field(status, HSLOT);
	genbit = brcmf_txstatus_get_field(status, GENERATION);
<<<<<<< HEAD

	return brcmf_fws_txstatus_process(fws, flags, hslot, genbit);
=======
	if (BRCMF_FWS_MODE_GET_REUSESEQ(fws->mode)) {
		memcpy(&seq_le, &data[BRCMF_FWS_TYPE_PKTTAG_LEN],
		       sizeof(seq_le));
		seq = le16_to_cpu(seq_le);
	} else {
		seq = 0;
	}

	brcmf_fws_lock(fws);
	brcmf_fws_txs_process(fws, flags, hslot, genbit, seq);
	brcmf_fws_unlock(fws);
	return BRCMF_FWS_RET_OK_NOSCHEDULE;
>>>>>>> v3.18
}

static int brcmf_fws_dbg_seqnum_check(struct brcmf_fws_info *fws, u8 *data)
{
	__le32 timestamp;

	memcpy(&timestamp, &data[2], sizeof(timestamp));
<<<<<<< HEAD
	brcmf_dbg(INFO, "received: seq %d, timestamp %d\n", data[1],
=======
	brcmf_dbg(CTL, "received: seq %d, timestamp %d\n", data[1],
>>>>>>> v3.18
		  le32_to_cpu(timestamp));
	return 0;
}

<<<<<<< HEAD
/* using macro so sparse checking does not complain
 * about locking imbalance.
 */
#define brcmf_fws_lock(drvr, flags)				\
do {								\
	flags = 0;						\
	spin_lock_irqsave(&((drvr)->fws_spinlock), (flags));	\
} while (0)

/* using macro so sparse checking does not complain
 * about locking imbalance.
 */
#define brcmf_fws_unlock(drvr, flags) \
	spin_unlock_irqrestore(&((drvr)->fws_spinlock), (flags))

=======
>>>>>>> v3.18
static int brcmf_fws_notify_credit_map(struct brcmf_if *ifp,
				       const struct brcmf_event_msg *e,
				       void *data)
{
	struct brcmf_fws_info *fws = ifp->drvr->fws;
	int i;
<<<<<<< HEAD
	ulong flags;
=======
>>>>>>> v3.18
	u8 *credits = data;

	if (e->datalen < BRCMF_FWS_FIFO_COUNT) {
		brcmf_err("event payload too small (%d)\n", e->datalen);
		return -EINVAL;
	}
<<<<<<< HEAD

	brcmf_dbg(TRACE, "enter: credits %pM\n", credits);
	brcmf_fws_lock(ifp->drvr, flags);
=======
	if (fws->creditmap_received)
		return 0;

	fws->creditmap_received = true;

	brcmf_dbg(TRACE, "enter: credits %pM\n", credits);
	brcmf_fws_lock(fws);
>>>>>>> v3.18
	for (i = 0; i < ARRAY_SIZE(fws->fifo_credit); i++) {
		if (*credits)
			fws->fifo_credit_map |= 1 << i;
		else
			fws->fifo_credit_map &= ~(1 << i);
		fws->fifo_credit[i] = *credits++;
	}
	brcmf_fws_schedule_deq(fws);
<<<<<<< HEAD
	brcmf_fws_unlock(ifp->drvr, flags);
=======
	brcmf_fws_unlock(fws);
	return 0;
}

static int brcmf_fws_notify_bcmc_credit_support(struct brcmf_if *ifp,
						const struct brcmf_event_msg *e,
						void *data)
{
	struct brcmf_fws_info *fws = ifp->drvr->fws;

	brcmf_fws_lock(fws);
	if (fws)
		fws->bcmc_credit_check = true;
	brcmf_fws_unlock(fws);
>>>>>>> v3.18
	return 0;
}

int brcmf_fws_hdrpull(struct brcmf_pub *drvr, int ifidx, s16 signal_len,
		      struct sk_buff *skb)
{
<<<<<<< HEAD
	struct brcmf_fws_info *fws = drvr->fws;
	ulong flags;
=======
	struct brcmf_skb_reorder_data *rd;
	struct brcmf_fws_info *fws = drvr->fws;
>>>>>>> v3.18
	u8 *signal_data;
	s16 data_len;
	u8 type;
	u8 len;
	u8 *data;
	s32 status;
	s32 err;

<<<<<<< HEAD
	brcmf_dbg(TRACE, "enter: ifidx %d, skblen %u, sig %d\n",
=======
	brcmf_dbg(HDRS, "enter: ifidx %d, skblen %u, sig %d\n",
>>>>>>> v3.18
		  ifidx, skb->len, signal_len);

	WARN_ON(signal_len > skb->len);

<<<<<<< HEAD
	/* if flow control disabled, skip to packet data and leave */
	if (!signal_len || !drvr->fw_signals) {
=======
	if (!signal_len)
		return 0;
	/* if flow control disabled, skip to packet data and leave */
	if ((!fws) || (!fws->fw_signals)) {
>>>>>>> v3.18
		skb_pull(skb, signal_len);
		return 0;
	}

<<<<<<< HEAD
	/* lock during tlv parsing */
	brcmf_fws_lock(drvr, flags);

=======
>>>>>>> v3.18
	fws->stats.header_pulls++;
	data_len = signal_len;
	signal_data = skb->data;

	status = BRCMF_FWS_RET_OK_NOSCHEDULE;
	while (data_len > 0) {
		/* extract tlv info */
		type = signal_data[0];

		/* FILLER type is actually not a TLV, but
		 * a single byte that can be skipped.
		 */
		if (type == BRCMF_FWS_TYPE_FILLER) {
			signal_data += 1;
			data_len -= 1;
			continue;
		}
		len = signal_data[1];
		data = signal_data + 2;

<<<<<<< HEAD
		brcmf_dbg(INFO, "tlv type=%d (%s), len=%d, data[0]=%d\n", type,
			  brcmf_fws_get_tlv_name(type), len, *data);
=======
		brcmf_dbg(HDRS, "tlv type=%s (%d), len=%d (%d)\n",
			  brcmf_fws_get_tlv_name(type), type, len,
			  brcmf_fws_get_tlv_len(fws, type));
>>>>>>> v3.18

		/* abort parsing when length invalid */
		if (data_len < len + 2)
			break;

<<<<<<< HEAD
		if (len != brcmf_fws_get_tlv_len(fws, type))
=======
		if (len < brcmf_fws_get_tlv_len(fws, type))
>>>>>>> v3.18
			break;

		err = BRCMF_FWS_RET_OK_NOSCHEDULE;
		switch (type) {
<<<<<<< HEAD
		case BRCMF_FWS_TYPE_HOST_REORDER_RXPKTS:
		case BRCMF_FWS_TYPE_COMP_TXSTATUS:
			break;
=======
		case BRCMF_FWS_TYPE_COMP_TXSTATUS:
			break;
		case BRCMF_FWS_TYPE_HOST_REORDER_RXPKTS:
			rd = (struct brcmf_skb_reorder_data *)skb->cb;
			rd->reorder = data;
			break;
>>>>>>> v3.18
		case BRCMF_FWS_TYPE_MACDESC_ADD:
		case BRCMF_FWS_TYPE_MACDESC_DEL:
			brcmf_fws_macdesc_indicate(fws, type, data);
			break;
		case BRCMF_FWS_TYPE_MAC_OPEN:
		case BRCMF_FWS_TYPE_MAC_CLOSE:
			err = brcmf_fws_macdesc_state_indicate(fws, type, data);
			break;
		case BRCMF_FWS_TYPE_INTERFACE_OPEN:
		case BRCMF_FWS_TYPE_INTERFACE_CLOSE:
			err = brcmf_fws_interface_state_indicate(fws, type,
								 data);
			break;
		case BRCMF_FWS_TYPE_MAC_REQUEST_CREDIT:
		case BRCMF_FWS_TYPE_MAC_REQUEST_PACKET:
			err = brcmf_fws_request_indicate(fws, type, data);
			break;
		case BRCMF_FWS_TYPE_TXSTATUS:
			brcmf_fws_txstatus_indicate(fws, data);
			break;
		case BRCMF_FWS_TYPE_FIFO_CREDITBACK:
			err = brcmf_fws_fifocreditback_indicate(fws, data);
			break;
		case BRCMF_FWS_TYPE_RSSI:
			brcmf_fws_rssi_indicate(fws, *data);
			break;
		case BRCMF_FWS_TYPE_TRANS_ID:
			brcmf_fws_dbg_seqnum_check(fws, data);
			break;
		case BRCMF_FWS_TYPE_PKTTAG:
		case BRCMF_FWS_TYPE_PENDING_TRAFFIC_BMP:
		default:
			fws->stats.tlv_invalid_type++;
			break;
		}
		if (err == BRCMF_FWS_RET_OK_SCHEDULE)
			status = BRCMF_FWS_RET_OK_SCHEDULE;
		signal_data += len + 2;
		data_len -= len + 2;
	}

	if (data_len != 0)
		fws->stats.tlv_parse_failed++;

	if (status == BRCMF_FWS_RET_OK_SCHEDULE)
		brcmf_fws_schedule_deq(fws);

	/* signalling processing result does
	 * not affect the actual ethernet packet.
	 */
	skb_pull(skb, signal_len);

	/* this may be a signal-only packet
	 */
	if (skb->len == 0)
		fws->stats.header_only_pkt++;

<<<<<<< HEAD
	brcmf_fws_unlock(drvr, flags);
	return 0;
}

static int brcmf_fws_hdrpush(struct brcmf_fws_info *fws, struct sk_buff *skb)
{
	struct brcmf_fws_mac_descriptor *entry = brcmf_skbcb(skb)->mac;
	u8 *wlh;
	u16 data_offset = 0;
	u8 fillers;
	__le32 pkttag = cpu_to_le32(brcmf_skbcb(skb)->htod);

	brcmf_dbg(TRACE, "enter: ea=%pM, ifidx=%u, pkttag=0x%08X\n",
		  entry->ea, entry->interface_id, le32_to_cpu(pkttag));
	if (entry->send_tim_signal)
		data_offset += 2 + BRCMF_FWS_TYPE_PENDING_TRAFFIC_BMP_LEN;

	/* +2 is for Type[1] and Len[1] in TLV, plus TIM signal */
	data_offset += 2 + BRCMF_FWS_TYPE_PKTTAG_LEN;
	fillers = round_up(data_offset, 4) - data_offset;
	data_offset += fillers;

	skb_push(skb, data_offset);
	wlh = skb->data;

	wlh[0] = BRCMF_FWS_TYPE_PKTTAG;
	wlh[1] = BRCMF_FWS_TYPE_PKTTAG_LEN;
	memcpy(&wlh[2], &pkttag, sizeof(pkttag));
	wlh += BRCMF_FWS_TYPE_PKTTAG_LEN + 2;

	if (entry->send_tim_signal) {
		entry->send_tim_signal = 0;
		wlh[0] = BRCMF_FWS_TYPE_PENDING_TRAFFIC_BMP;
		wlh[1] = BRCMF_FWS_TYPE_PENDING_TRAFFIC_BMP_LEN;
		wlh[2] = entry->mac_handle;
		wlh[3] = entry->traffic_pending_bmp;
		wlh += BRCMF_FWS_TYPE_PENDING_TRAFFIC_BMP_LEN + 2;
		entry->traffic_lastreported_bmp = entry->traffic_pending_bmp;
	}
	if (fillers)
		memset(wlh, BRCMF_FWS_TYPE_FILLER, fillers);

	brcmf_proto_hdrpush(fws->drvr, brcmf_skb_if_flags_get_field(skb, INDEX),
			    data_offset >> 2, skb);
	return 0;
}

static int brcmf_fws_precommit_skb(struct brcmf_fws_info *fws, int fifo,
=======
	return 0;
}

static u8 brcmf_fws_precommit_skb(struct brcmf_fws_info *fws, int fifo,
>>>>>>> v3.18
				   struct sk_buff *p)
{
	struct brcmf_skbuff_cb *skcb = brcmf_skbcb(p);
	struct brcmf_fws_mac_descriptor *entry = skcb->mac;
<<<<<<< HEAD
	int rc = 0;
	bool header_needed;
	int hslot = BRCMF_FWS_HANGER_MAXITEMS;
	u8 free_ctr;
	u8 ifidx;
	u8 flags;

	header_needed = skcb->state != BRCMF_FWS_SKBSTATE_SUPPRESSED;

	if (header_needed) {
		/* obtaining free slot may fail, but that will be caught
		 * by the hanger push. This assures the packet has a BDC
		 * header upon return.
		 */
		hslot = brcmf_fws_hanger_get_free_slot(&fws->hanger);
		free_ctr = entry->seq[fifo];
		brcmf_skb_htod_tag_set_field(p, HSLOT, hslot);
		brcmf_skb_htod_tag_set_field(p, FREERUN, free_ctr);
		brcmf_skb_htod_tag_set_field(p, GENERATION, 1);
		entry->transit_count++;
	}
	brcmf_skb_if_flags_set_field(p, TRANSMIT, 1);
	brcmf_skb_htod_tag_set_field(p, FIFO, fifo);

	flags = BRCMF_FWS_HTOD_FLAG_PKTFROMHOST;
	if (!(skcb->if_flags & BRCMF_SKB_IF_FLAGS_CREDITCHECK_MASK)) {
		/*
		Indicate that this packet is being sent in response to an
		explicit request from the firmware side.
		*/
		flags |= BRCMF_FWS_HTOD_FLAG_PKT_REQUESTED;
	}
	brcmf_skb_htod_tag_set_field(p, FLAGS, flags);
	if (header_needed) {
		brcmf_fws_hdrpush(fws, p);
		rc = brcmf_fws_hanger_pushpkt(&fws->hanger, p, hslot);
		if (rc)
			brcmf_err("hanger push failed: rc=%d\n", rc);
	} else {
		int gen;

		/* remove old header */
		rc = brcmf_proto_hdrpull(fws->drvr, false, &ifidx, p);
		if (rc == 0) {
			hslot = brcmf_skb_htod_tag_get_field(p, HSLOT);
			brcmf_fws_hanger_get_genbit(&fws->hanger, p,
						    hslot, &gen);
			brcmf_skb_htod_tag_set_field(p, GENERATION, gen);

			/* push new header */
			brcmf_fws_hdrpush(fws, p);
		}
	}

	return rc;
}

static void
brcmf_fws_rollback_toq(struct brcmf_fws_info *fws, struct sk_buff *skb)
{
	/*
	put the packet back to the head of queue

	- suppressed packet goes back to suppress sub-queue
	- pull out the header, if new or delayed packet

	Note: hslot is used only when header removal is done.
	*/
	struct brcmf_fws_mac_descriptor *entry;
	enum brcmf_fws_skb_state state;
	struct sk_buff *pktout;
	int rc = 0;
	int fifo;
	int hslot;
	u8 ifidx;

	fifo = brcmf_skb_if_flags_get_field(skb, FIFO);
	state = brcmf_skbcb(skb)->state;
	entry = brcmf_skbcb(skb)->mac;

	if (entry != NULL) {
		if (state == BRCMF_FWS_SKBSTATE_SUPPRESSED) {
			/* wl-header is saved for suppressed packets */
			pktout = brcmu_pktq_penq_head(&entry->psq, 2 * fifo + 1,
						      skb);
			if (pktout == NULL) {
				brcmf_err("suppress queue full\n");
				rc = -ENOSPC;
			}
		} else {
			hslot = brcmf_skb_htod_tag_get_field(skb, HSLOT);

			/* remove header first */
			rc = brcmf_proto_hdrpull(fws->drvr, false, &ifidx, skb);
			if (rc) {
				brcmf_err("header removal failed\n");
				/* free the hanger slot */
				brcmf_fws_hanger_poppkt(&fws->hanger, hslot,
							&pktout, true);
				rc = -EINVAL;
				goto fail;
			}

			/* delay-q packets are going to delay-q */
			pktout = brcmu_pktq_penq_head(&entry->psq,
						      2 * fifo, skb);
			if (pktout == NULL) {
				brcmf_err("delay queue full\n");
				rc = -ENOSPC;
			}

			/* free the hanger slot */
			brcmf_fws_hanger_poppkt(&fws->hanger, hslot, &pktout,
						true);

			/* decrement sequence count */
			entry->seq[fifo]--;
		}
		/*
		if this packet did not count against FIFO credit, it must have
		taken a requested_credit from the firmware (for pspoll etc.)
		*/
		if (!(brcmf_skbcb(skb)->if_flags &
		      BRCMF_SKB_IF_FLAGS_CREDITCHECK_MASK))
			entry->requested_credit++;
	} else {
		brcmf_err("no mac entry linked\n");
		rc = -ENOENT;
	}


fail:
	if (rc) {
		brcmf_txfinalize(fws->drvr, skb, false);
		fws->stats.rollback_failed++;
	} else
		fws->stats.rollback_success++;
=======
	u8 flags;

	if (skcb->state != BRCMF_FWS_SKBSTATE_SUPPRESSED)
		brcmf_skb_htod_tag_set_field(p, GENERATION, entry->generation);
	flags = BRCMF_FWS_HTOD_FLAG_PKTFROMHOST;
	if (brcmf_skb_if_flags_get_field(p, REQUESTED)) {
		/*
		 * Indicate that this packet is being sent in response to an
		 * explicit request from the firmware side.
		 */
		flags |= BRCMF_FWS_HTOD_FLAG_PKT_REQUESTED;
	}
	brcmf_skb_htod_tag_set_field(p, FLAGS, flags);
	return brcmf_fws_hdrpush(fws, p);
}

static void brcmf_fws_rollback_toq(struct brcmf_fws_info *fws,
				   struct sk_buff *skb, int fifo)
{
	struct brcmf_fws_mac_descriptor *entry;
	struct sk_buff *pktout;
	int qidx, hslot;
	int rc = 0;

	entry = brcmf_skbcb(skb)->mac;
	if (entry->occupied) {
		qidx = 2 * fifo;
		if (brcmf_skbcb(skb)->state == BRCMF_FWS_SKBSTATE_SUPPRESSED)
			qidx++;

		pktout = brcmu_pktq_penq_head(&entry->psq, qidx, skb);
		if (pktout == NULL) {
			brcmf_err("%s queue %d full\n", entry->name, qidx);
			rc = -ENOSPC;
		}
	} else {
		brcmf_err("%s entry removed\n", entry->name);
		rc = -ENOENT;
	}

	if (rc) {
		fws->stats.rollback_failed++;
		hslot = brcmf_skb_htod_tag_get_field(skb, HSLOT);
		brcmf_fws_txs_process(fws, BRCMF_FWS_TXSTATUS_HOST_TOSSED,
				      hslot, 0, 0);
	} else {
		fws->stats.rollback_success++;
		brcmf_fws_return_credits(fws, fifo, 1);
		brcmf_fws_macdesc_return_req_credit(skb);
	}
>>>>>>> v3.18
}

static int brcmf_fws_borrow_credit(struct brcmf_fws_info *fws)
{
	int lender_ac;

<<<<<<< HEAD
	if (time_after(fws->borrow_defer_timestamp, jiffies))
		return -ENAVAIL;
=======
	if (time_after(fws->borrow_defer_timestamp, jiffies)) {
		fws->fifo_credit_map &= ~(1 << BRCMF_FWS_FIFO_AC_BE);
		return -ENAVAIL;
	}
>>>>>>> v3.18

	for (lender_ac = 0; lender_ac <= BRCMF_FWS_FIFO_AC_VO; lender_ac++) {
		if (fws->fifo_credit[lender_ac]) {
			fws->credits_borrowed[lender_ac]++;
			fws->fifo_credit[lender_ac]--;
			if (fws->fifo_credit[lender_ac] == 0)
				fws->fifo_credit_map &= ~(1 << lender_ac);
<<<<<<< HEAD
			brcmf_dbg(TRACE, "borrow credit from: %d\n", lender_ac);
			return 0;
		}
	}
	return -ENAVAIL;
}

static int brcmf_fws_consume_credit(struct brcmf_fws_info *fws, int fifo,
				    struct sk_buff *skb)
{
	struct brcmf_fws_mac_descriptor *entry = brcmf_skbcb(skb)->mac;
	int *credit = &fws->fifo_credit[fifo];
	int use_credit = 1;

	brcmf_dbg(TRACE, "enter: ac=%d, credits=%d\n", fifo, *credit);

	if (entry->requested_credit > 0) {
		/*
		 * if the packet was pulled out while destination is in
		 * closed state but had a non-zero packets requested,
		 * then this should not count against the FIFO credit.
		 * That is due to the fact that the firmware will
		 * most likely hold onto this packet until a suitable
		 * time later to push it to the appropriate AC FIFO.
		 */
		entry->requested_credit--;
		if (entry->state == BRCMF_FWS_STATE_CLOSE)
			use_credit = 0;
	} else if (entry->requested_packet > 0) {
		entry->requested_packet--;
		brcmf_skb_if_flags_set_field(skb, REQUESTED, 1);
		if (entry->state == BRCMF_FWS_STATE_CLOSE)
			use_credit = 0;
	}
	brcmf_skb_if_flags_set_field(skb, CREDITCHECK, use_credit);
	if (!use_credit) {
		brcmf_dbg(TRACE, "exit: no creditcheck set\n");
		return 0;
	}

	if (fifo != BRCMF_FWS_FIFO_AC_BE)
		fws->borrow_defer_timestamp = jiffies +
					      BRCMF_FWS_BORROW_DEFER_PERIOD;

	if (!(*credit)) {
		/* Try to borrow a credit from other queue */
		if (fifo == BRCMF_FWS_FIFO_AC_BE &&
		    brcmf_fws_borrow_credit(fws) == 0)
			return 0;

		brcmf_dbg(TRACE, "exit: ac=%d, credits depleted\n", fifo);
		return -ENAVAIL;
	}
	(*credit)--;
	if (!(*credit))
		fws->fifo_credit_map &= ~(1 << fifo);
	brcmf_dbg(TRACE, "exit: ac=%d, credits=%d\n", fifo, *credit);
	return 0;
}

=======
			fws->fifo_credit_map |= (1 << BRCMF_FWS_FIFO_AC_BE);
			brcmf_dbg(DATA, "borrow credit from: %d\n", lender_ac);
			return 0;
		}
	}
	fws->fifo_credit_map &= ~(1 << BRCMF_FWS_FIFO_AC_BE);
	return -ENAVAIL;
}

>>>>>>> v3.18
static int brcmf_fws_commit_skb(struct brcmf_fws_info *fws, int fifo,
				struct sk_buff *skb)
{
	struct brcmf_skbuff_cb *skcb = brcmf_skbcb(skb);
	struct brcmf_fws_mac_descriptor *entry;
<<<<<<< HEAD
	struct brcmf_bus *bus = fws->drvr->bus_if;
	int rc;
=======
	int rc;
	u8 ifidx;
	u8 data_offset;
>>>>>>> v3.18

	entry = skcb->mac;
	if (IS_ERR(entry))
		return PTR_ERR(entry);

<<<<<<< HEAD
	rc = brcmf_fws_precommit_skb(fws, fifo, skb);
	if (rc < 0) {
		fws->stats.generic_error++;
		goto rollback;
	}

	rc = brcmf_bus_txdata(bus, skb);
	if (rc < 0)
		goto rollback;

	entry->seq[fifo]++;
	fws->stats.pkt2bus++;
	if (brcmf_skbcb(skb)->if_flags & BRCMF_SKB_IF_FLAGS_CREDITCHECK_MASK) {
		fws->stats.send_pkts[fifo]++;
		fws->stats.fifo_credits_sent[fifo]++;
	}
=======
	data_offset = brcmf_fws_precommit_skb(fws, fifo, skb);
	entry->transit_count++;
	if (entry->suppressed)
		entry->suppr_transit_count++;
	ifidx = brcmf_skb_if_flags_get_field(skb, INDEX);
	brcmf_fws_unlock(fws);
	rc = brcmf_proto_txdata(fws->drvr, ifidx, data_offset, skb);
	brcmf_fws_lock(fws);
	brcmf_dbg(DATA, "%s flags %X htod %X bus_tx %d\n", entry->name,
		  skcb->if_flags, skcb->htod, rc);
	if (rc < 0) {
		entry->transit_count--;
		if (entry->suppressed)
			entry->suppr_transit_count--;
		brcmf_proto_hdrpull(fws->drvr, false, &ifidx, skb);
		goto rollback;
	}

	fws->stats.pkt2bus++;
	fws->stats.send_pkts[fifo]++;
	if (brcmf_skb_if_flags_get_field(skb, REQUESTED))
		fws->stats.requested_sent[fifo]++;
>>>>>>> v3.18

	return rc;

rollback:
<<<<<<< HEAD
	brcmf_fws_rollback_toq(fws, skb);
=======
	brcmf_fws_rollback_toq(fws, skb, fifo);
	return rc;
}

static int brcmf_fws_assign_htod(struct brcmf_fws_info *fws, struct sk_buff *p,
				  int fifo)
{
	struct brcmf_skbuff_cb *skcb = brcmf_skbcb(p);
	int rc, hslot;

	skcb->htod = 0;
	skcb->htod_seq = 0;
	hslot = brcmf_fws_hanger_get_free_slot(&fws->hanger);
	brcmf_skb_htod_tag_set_field(p, HSLOT, hslot);
	brcmf_skb_htod_tag_set_field(p, FREERUN, skcb->mac->seq[fifo]);
	brcmf_skb_htod_tag_set_field(p, FIFO, fifo);
	rc = brcmf_fws_hanger_pushpkt(&fws->hanger, p, hslot);
	if (!rc)
		skcb->mac->seq[fifo]++;
	else
		fws->stats.generic_error++;
>>>>>>> v3.18
	return rc;
}

int brcmf_fws_process_skb(struct brcmf_if *ifp, struct sk_buff *skb)
{
	struct brcmf_pub *drvr = ifp->drvr;
	struct brcmf_fws_info *fws = drvr->fws;
	struct brcmf_skbuff_cb *skcb = brcmf_skbcb(skb);
	struct ethhdr *eh = (struct ethhdr *)(skb->data);
<<<<<<< HEAD
	ulong flags;
	int fifo = BRCMF_FWS_FIFO_BCMC;
	bool multicast = is_multicast_ether_addr(eh->h_dest);

=======
	int fifo = BRCMF_FWS_FIFO_BCMC;
	bool multicast = is_multicast_ether_addr(eh->h_dest);
	int rc = 0;

	brcmf_dbg(DATA, "tx proto=0x%X\n", ntohs(eh->h_proto));
>>>>>>> v3.18
	/* determine the priority */
	if (!skb->priority)
		skb->priority = cfg80211_classify8021d(skb, NULL);

	drvr->tx_multicast += !!multicast;
<<<<<<< HEAD
	if (ntohs(eh->h_proto) == ETH_P_PAE)
		atomic_inc(&ifp->pend_8021x_cnt);

	if (!brcmf_fws_fc_active(fws)) {
		/* If the protocol uses a data header, apply it */
		brcmf_proto_hdrpush(drvr, ifp->ifidx, 0, skb);

		/* Use bus module to send data frame */
		return brcmf_bus_txdata(drvr->bus_if, skb);
=======

	if (fws->avoid_queueing) {
		rc = brcmf_proto_txdata(drvr, ifp->ifidx, 0, skb);
		if (rc < 0)
			brcmf_txfinalize(drvr, skb, ifp->ifidx, false);
		return rc;
>>>>>>> v3.18
	}

	/* set control buffer information */
	skcb->if_flags = 0;
<<<<<<< HEAD
	skcb->mac = brcmf_fws_find_mac_desc(fws, ifp, eh->h_dest);
=======
>>>>>>> v3.18
	skcb->state = BRCMF_FWS_SKBSTATE_NEW;
	brcmf_skb_if_flags_set_field(skb, INDEX, ifp->ifidx);
	if (!multicast)
		fifo = brcmf_fws_prio2fifo[skb->priority];
<<<<<<< HEAD
	brcmf_skb_if_flags_set_field(skb, FIFO, fifo);

	brcmf_dbg(TRACE, "ea=%pM, multi=%d, fifo=%d\n", eh->h_dest,
		  multicast, fifo);

	brcmf_fws_lock(drvr, flags);
	if (skcb->mac->suppressed ||
	    brcmf_fws_mac_desc_closed(fws, skcb->mac, fifo) ||
	    brcmu_pktq_mlen(&skcb->mac->psq, 3 << (fifo * 2)) ||
	    (!multicast &&
	     brcmf_fws_consume_credit(fws, fifo, skb) < 0)) {
		/* enqueue the packet in delayQ */
		drvr->fws->fifo_delay_map |= 1 << fifo;
		brcmf_fws_enq(fws, BRCMF_FWS_SKBSTATE_DELAYED, fifo, skb);
	} else {
		if (brcmf_fws_commit_skb(fws, fifo, skb))
			if (!multicast)
				brcmf_skb_pick_up_credit(fws, fifo, skb);
	}
	brcmf_fws_unlock(drvr, flags);
	return 0;
=======

	brcmf_fws_lock(fws);
	if (fifo != BRCMF_FWS_FIFO_AC_BE && fifo < BRCMF_FWS_FIFO_BCMC)
		fws->borrow_defer_timestamp = jiffies +
					      BRCMF_FWS_BORROW_DEFER_PERIOD;

	skcb->mac = brcmf_fws_macdesc_find(fws, ifp, eh->h_dest);
	brcmf_dbg(DATA, "%s mac %pM multi %d fifo %d\n", skcb->mac->name,
		  eh->h_dest, multicast, fifo);
	if (!brcmf_fws_assign_htod(fws, skb, fifo)) {
		brcmf_fws_enq(fws, BRCMF_FWS_SKBSTATE_DELAYED, fifo, skb);
		brcmf_fws_schedule_deq(fws);
	} else {
		brcmf_err("drop skb: no hanger slot\n");
		brcmf_txfinalize(drvr, skb, ifp->ifidx, false);
		rc = -ENOMEM;
	}
	brcmf_fws_unlock(fws);

	return rc;
>>>>>>> v3.18
}

void brcmf_fws_reset_interface(struct brcmf_if *ifp)
{
	struct brcmf_fws_mac_descriptor *entry = ifp->fws_desc;

	brcmf_dbg(TRACE, "enter: idx=%d\n", ifp->bssidx);
	if (!entry)
		return;

<<<<<<< HEAD
	brcmf_fws_init_mac_descriptor(entry, ifp->mac_addr, ifp->ifidx);
=======
	brcmf_fws_macdesc_init(entry, ifp->mac_addr, ifp->ifidx);
>>>>>>> v3.18
}

void brcmf_fws_add_interface(struct brcmf_if *ifp)
{
	struct brcmf_fws_info *fws = ifp->drvr->fws;
	struct brcmf_fws_mac_descriptor *entry;

<<<<<<< HEAD
	brcmf_dbg(TRACE, "enter: idx=%d, mac=%pM\n",
		  ifp->bssidx, ifp->mac_addr);
	if (!ifp->ndev || !ifp->drvr->fw_signals)
=======
	if (!ifp->ndev)
>>>>>>> v3.18
		return;

	entry = &fws->desc.iface[ifp->ifidx];
	ifp->fws_desc = entry;
<<<<<<< HEAD
	brcmf_fws_init_mac_descriptor(entry, ifp->mac_addr, ifp->ifidx);
	brcmu_pktq_init(&entry->psq, BRCMF_FWS_PSQ_PREC_COUNT,
			BRCMF_FWS_PSQ_LEN);
=======
	brcmf_fws_macdesc_init(entry, ifp->mac_addr, ifp->ifidx);
	brcmf_fws_macdesc_set_name(fws, entry);
	brcmu_pktq_init(&entry->psq, BRCMF_FWS_PSQ_PREC_COUNT,
			BRCMF_FWS_PSQ_LEN);
	brcmf_dbg(TRACE, "added %s\n", entry->name);
>>>>>>> v3.18
}

void brcmf_fws_del_interface(struct brcmf_if *ifp)
{
	struct brcmf_fws_mac_descriptor *entry = ifp->fws_desc;
<<<<<<< HEAD
	ulong flags;

	brcmf_dbg(TRACE, "enter: idx=%d\n", ifp->bssidx);
	if (!entry)
		return;

	brcmf_fws_lock(ifp->drvr, flags);
	ifp->fws_desc = NULL;
	brcmf_fws_clear_mac_descriptor(entry);
	brcmf_fws_cleanup(ifp->drvr->fws, ifp->ifidx);
	brcmf_fws_unlock(ifp->drvr, flags);
=======

	if (!entry)
		return;

	brcmf_fws_lock(ifp->drvr->fws);
	ifp->fws_desc = NULL;
	brcmf_dbg(TRACE, "deleting %s\n", entry->name);
	brcmf_fws_macdesc_deinit(entry);
	brcmf_fws_cleanup(ifp->drvr->fws, ifp->ifidx);
	brcmf_fws_unlock(ifp->drvr->fws);
>>>>>>> v3.18
}

static void brcmf_fws_dequeue_worker(struct work_struct *worker)
{
	struct brcmf_fws_info *fws;
<<<<<<< HEAD
	struct sk_buff *skb;
	ulong flags;
	int fifo;
	int credit;

	fws = container_of(worker, struct brcmf_fws_info, fws_dequeue_work);

	brcmf_dbg(TRACE, "enter: fws=%p\n", fws);
	brcmf_fws_lock(fws->drvr, flags);
	for (fifo = NL80211_NUM_ACS; fifo >= 0; fifo--) {
		brcmf_dbg(TRACE, "fifo %d credit %d\n", fifo,
			  fws->fifo_credit[fifo]);
		for (credit = 0; credit < fws->fifo_credit[fifo]; /* nop */) {
			skb = brcmf_fws_deq(fws, fifo);
			if (!skb || brcmf_fws_commit_skb(fws, fifo, skb))
				break;
			if (brcmf_skbcb(skb)->if_flags &
			    BRCMF_SKB_IF_FLAGS_CREDITCHECK_MASK)
				credit++;
		}
		if ((fifo == BRCMF_FWS_FIFO_AC_BE) &&
		    (credit == fws->fifo_credit[fifo])) {
			fws->fifo_credit[fifo] -= credit;
=======
	struct brcmf_pub *drvr;
	struct sk_buff *skb;
	int fifo;
	u32 hslot;
	u32 ifidx;
	int ret;

	fws = container_of(worker, struct brcmf_fws_info, fws_dequeue_work);
	drvr = fws->drvr;

	brcmf_fws_lock(fws);
	for (fifo = BRCMF_FWS_FIFO_BCMC; fifo >= 0 && !fws->bus_flow_blocked;
	     fifo--) {
		if (!brcmf_fws_fc_active(fws)) {
			while ((skb = brcmf_fws_deq(fws, fifo)) != NULL) {
				hslot = brcmf_skb_htod_tag_get_field(skb,
								     HSLOT);
				brcmf_fws_hanger_poppkt(&fws->hanger, hslot,
							&skb, true);
				ifidx = brcmf_skb_if_flags_get_field(skb,
								     INDEX);
				/* Use proto layer to send data frame */
				brcmf_fws_unlock(fws);
				ret = brcmf_proto_txdata(drvr, ifidx, 0, skb);
				brcmf_fws_lock(fws);
				if (ret < 0)
					brcmf_txfinalize(drvr, skb, ifidx,
							 false);
				if (fws->bus_flow_blocked)
					break;
			}
			continue;
		}
		while ((fws->fifo_credit[fifo]) || ((!fws->bcmc_credit_check) &&
		       (fifo == BRCMF_FWS_FIFO_BCMC))) {
			skb = brcmf_fws_deq(fws, fifo);
			if (!skb)
				break;
			fws->fifo_credit[fifo]--;
			if (brcmf_fws_commit_skb(fws, fifo, skb))
				break;
			if (fws->bus_flow_blocked)
				break;
		}
		if ((fifo == BRCMF_FWS_FIFO_AC_BE) &&
		    (fws->fifo_credit[fifo] == 0) &&
		    (!fws->bus_flow_blocked)) {
>>>>>>> v3.18
			while (brcmf_fws_borrow_credit(fws) == 0) {
				skb = brcmf_fws_deq(fws, fifo);
				if (!skb) {
					brcmf_fws_return_credits(fws, fifo, 1);
					break;
				}
<<<<<<< HEAD
				if (brcmf_fws_commit_skb(fws, fifo, skb)) {
					brcmf_fws_return_credits(fws, fifo, 1);
					break;
				}
			}
		} else {
			fws->fifo_credit[fifo] -= credit;
		}
	}
	brcmf_fws_unlock(fws->drvr, flags);
}

int brcmf_fws_init(struct brcmf_pub *drvr)
{
	u32 tlv = BRCMF_FWS_FLAGS_RSSI_SIGNALS;
	int rc;

	if (!drvr->fw_signals)
		return 0;

	spin_lock_init(&drvr->fws_spinlock);
=======
				if (brcmf_fws_commit_skb(fws, fifo, skb))
					break;
				if (fws->bus_flow_blocked)
					break;
			}
		}
	}
	brcmf_fws_unlock(fws);
}

#ifdef DEBUG
static int brcmf_debugfs_fws_stats_read(struct seq_file *seq, void *data)
{
	struct brcmf_bus *bus_if = dev_get_drvdata(seq->private);
	struct brcmf_fws_stats *fwstats = &bus_if->drvr->fws->stats;

	seq_printf(seq,
		   "header_pulls:      %u\n"
		   "header_only_pkt:   %u\n"
		   "tlv_parse_failed:  %u\n"
		   "tlv_invalid_type:  %u\n"
		   "mac_update_fails:  %u\n"
		   "ps_update_fails:   %u\n"
		   "if_update_fails:   %u\n"
		   "pkt2bus:           %u\n"
		   "generic_error:     %u\n"
		   "rollback_success:  %u\n"
		   "rollback_failed:   %u\n"
		   "delayq_full:       %u\n"
		   "supprq_full:       %u\n"
		   "txs_indicate:      %u\n"
		   "txs_discard:       %u\n"
		   "txs_suppr_core:    %u\n"
		   "txs_suppr_ps:      %u\n"
		   "txs_tossed:        %u\n"
		   "txs_host_tossed:   %u\n"
		   "bus_flow_block:    %u\n"
		   "fws_flow_block:    %u\n"
		   "send_pkts:         BK:%u BE:%u VO:%u VI:%u BCMC:%u\n"
		   "requested_sent:    BK:%u BE:%u VO:%u VI:%u BCMC:%u\n",
		   fwstats->header_pulls,
		   fwstats->header_only_pkt,
		   fwstats->tlv_parse_failed,
		   fwstats->tlv_invalid_type,
		   fwstats->mac_update_failed,
		   fwstats->mac_ps_update_failed,
		   fwstats->if_update_failed,
		   fwstats->pkt2bus,
		   fwstats->generic_error,
		   fwstats->rollback_success,
		   fwstats->rollback_failed,
		   fwstats->delayq_full_error,
		   fwstats->supprq_full_error,
		   fwstats->txs_indicate,
		   fwstats->txs_discard,
		   fwstats->txs_supp_core,
		   fwstats->txs_supp_ps,
		   fwstats->txs_tossed,
		   fwstats->txs_host_tossed,
		   fwstats->bus_flow_block,
		   fwstats->fws_flow_block,
		   fwstats->send_pkts[0], fwstats->send_pkts[1],
		   fwstats->send_pkts[2], fwstats->send_pkts[3],
		   fwstats->send_pkts[4],
		   fwstats->requested_sent[0],
		   fwstats->requested_sent[1],
		   fwstats->requested_sent[2],
		   fwstats->requested_sent[3],
		   fwstats->requested_sent[4]);

	return 0;
}
#else
static int brcmf_debugfs_fws_stats_read(struct seq_file *seq, void *data)
{
	return 0;
}
#endif

int brcmf_fws_init(struct brcmf_pub *drvr)
{
	struct brcmf_fws_info *fws;
	u32 tlv = BRCMF_FWS_FLAGS_RSSI_SIGNALS;
	int rc;
	u32 mode;
>>>>>>> v3.18

	drvr->fws = kzalloc(sizeof(*(drvr->fws)), GFP_KERNEL);
	if (!drvr->fws) {
		rc = -ENOMEM;
		goto fail;
	}

<<<<<<< HEAD
	/* set linkage back */
	drvr->fws->drvr = drvr;
	drvr->fws->fcmode = fcmode;

	drvr->fws->fws_wq = create_singlethread_workqueue("brcmf_fws_wq");
	if (drvr->fws->fws_wq == NULL) {
=======
	fws = drvr->fws;

	spin_lock_init(&fws->spinlock);

	/* set linkage back */
	fws->drvr = drvr;
	fws->fcmode = fcmode;

	if ((drvr->bus_if->always_use_fws_queue == false) &&
	    (fcmode == BRCMF_FWS_FCMODE_NONE)) {
		fws->avoid_queueing = true;
		brcmf_dbg(INFO, "FWS queueing will be avoided\n");
		return 0;
	}

	fws->fws_wq = create_singlethread_workqueue("brcmf_fws_wq");
	if (fws->fws_wq == NULL) {
>>>>>>> v3.18
		brcmf_err("workqueue creation failed\n");
		rc = -EBADF;
		goto fail;
	}
<<<<<<< HEAD
	INIT_WORK(&drvr->fws->fws_dequeue_work, brcmf_fws_dequeue_worker);

	/* enable firmware signalling if fcmode active */
	if (drvr->fws->fcmode != BRCMF_FWS_FCMODE_NONE)
		tlv |= BRCMF_FWS_FLAGS_XONXOFF_SIGNALS |
		       BRCMF_FWS_FLAGS_CREDIT_STATUS_SIGNALS |
		       BRCMF_FWS_FLAGS_HOST_PROPTXSTATUS_ACTIVE;
=======
	INIT_WORK(&fws->fws_dequeue_work, brcmf_fws_dequeue_worker);

	/* enable firmware signalling if fcmode active */
	if (fws->fcmode != BRCMF_FWS_FCMODE_NONE)
		tlv |= BRCMF_FWS_FLAGS_XONXOFF_SIGNALS |
		       BRCMF_FWS_FLAGS_CREDIT_STATUS_SIGNALS |
		       BRCMF_FWS_FLAGS_HOST_PROPTXSTATUS_ACTIVE |
		       BRCMF_FWS_FLAGS_HOST_RXREORDER_ACTIVE;
>>>>>>> v3.18

	rc = brcmf_fweh_register(drvr, BRCMF_E_FIFO_CREDIT_MAP,
				 brcmf_fws_notify_credit_map);
	if (rc < 0) {
		brcmf_err("register credit map handler failed\n");
		goto fail;
	}
<<<<<<< HEAD

	/* setting the iovar may fail if feature is unsupported
	 * so leave the rc as is so driver initialization can
	 * continue.
	 */
	if (brcmf_fil_iovar_int_set(drvr->iflist[0], "tlv", tlv)) {
		brcmf_err("failed to set bdcv2 tlv signaling\n");
		goto fail_event;
	}

	brcmf_fws_hanger_init(&drvr->fws->hanger);
	brcmf_fws_init_mac_descriptor(&drvr->fws->desc.other, NULL, 0);
	brcmu_pktq_init(&drvr->fws->desc.other.psq, BRCMF_FWS_PSQ_PREC_COUNT,
			BRCMF_FWS_PSQ_LEN);

	/* create debugfs file for statistics */
	brcmf_debugfs_create_fws_stats(drvr, &drvr->fws->stats);

	/* TODO: remove upon feature delivery */
	brcmf_err("%s bdcv2 tlv signaling [%x]\n",
		  drvr->fw_signals ? "enabled" : "disabled", tlv);
	return 0;

fail_event:
	brcmf_fweh_unregister(drvr, BRCMF_E_FIFO_CREDIT_MAP);
=======
	rc = brcmf_fweh_register(drvr, BRCMF_E_BCMC_CREDIT_SUPPORT,
				 brcmf_fws_notify_bcmc_credit_support);
	if (rc < 0) {
		brcmf_err("register bcmc credit handler failed\n");
		brcmf_fweh_unregister(drvr, BRCMF_E_FIFO_CREDIT_MAP);
		goto fail;
	}

	/* Setting the iovar may fail if feature is unsupported
	 * so leave the rc as is so driver initialization can
	 * continue. Set mode back to none indicating not enabled.
	 */
	fws->fw_signals = true;
	if (brcmf_fil_iovar_int_set(drvr->iflist[0], "tlv", tlv)) {
		brcmf_err("failed to set bdcv2 tlv signaling\n");
		fws->fcmode = BRCMF_FWS_FCMODE_NONE;
		fws->fw_signals = false;
	}

	if (brcmf_fil_iovar_int_set(drvr->iflist[0], "ampdu_hostreorder", 1))
		brcmf_dbg(INFO, "enabling AMPDU host-reorder failed\n");

	/* Enable seq number reuse, if supported */
	if (brcmf_fil_iovar_int_get(drvr->iflist[0], "wlfc_mode", &mode) == 0) {
		if (BRCMF_FWS_MODE_GET_REUSESEQ(mode)) {
			mode = 0;
			BRCMF_FWS_MODE_SET_REUSESEQ(mode, 1);
			if (brcmf_fil_iovar_int_set(drvr->iflist[0],
						    "wlfc_mode", mode) == 0) {
				BRCMF_FWS_MODE_SET_REUSESEQ(fws->mode, 1);
			}
		}
	}

	brcmf_fws_hanger_init(&fws->hanger);
	brcmf_fws_macdesc_init(&fws->desc.other, NULL, 0);
	brcmf_fws_macdesc_set_name(fws, &fws->desc.other);
	brcmu_pktq_init(&fws->desc.other.psq, BRCMF_FWS_PSQ_PREC_COUNT,
			BRCMF_FWS_PSQ_LEN);

	/* create debugfs file for statistics */
	brcmf_debugfs_add_entry(drvr, "fws_stats",
				brcmf_debugfs_fws_stats_read);

	brcmf_dbg(INFO, "%s bdcv2 tlv signaling [%x]\n",
		  fws->fw_signals ? "enabled" : "disabled", tlv);
	return 0;

>>>>>>> v3.18
fail:
	brcmf_fws_deinit(drvr);
	return rc;
}

void brcmf_fws_deinit(struct brcmf_pub *drvr)
{
	struct brcmf_fws_info *fws = drvr->fws;
<<<<<<< HEAD
	ulong flags;
=======
>>>>>>> v3.18

	if (!fws)
		return;

<<<<<<< HEAD
	/* disable firmware signalling entirely
	 * to avoid using the workqueue.
	 */
	drvr->fw_signals = false;

=======
>>>>>>> v3.18
	if (drvr->fws->fws_wq)
		destroy_workqueue(drvr->fws->fws_wq);

	/* cleanup */
<<<<<<< HEAD
	brcmf_fws_lock(drvr, flags);
	brcmf_fws_cleanup(fws, -1);
	drvr->fws = NULL;
	brcmf_fws_unlock(drvr, flags);
=======
	brcmf_fws_lock(fws);
	brcmf_fws_cleanup(fws, -1);
	drvr->fws = NULL;
	brcmf_fws_unlock(fws);
>>>>>>> v3.18

	/* free top structure */
	kfree(fws);
}

bool brcmf_fws_fc_active(struct brcmf_fws_info *fws)
{
<<<<<<< HEAD
	if (!fws)
		return false;

	brcmf_dbg(TRACE, "enter: mode=%d\n", fws->fcmode);
=======
	if (!fws->creditmap_received)
		return false;

>>>>>>> v3.18
	return fws->fcmode != BRCMF_FWS_FCMODE_NONE;
}

void brcmf_fws_bustxfail(struct brcmf_fws_info *fws, struct sk_buff *skb)
{
<<<<<<< HEAD
	ulong flags;

	brcmf_fws_lock(fws->drvr, flags);
	brcmf_fws_txstatus_process(fws, BRCMF_FWS_TXSTATUS_FW_TOSSED,
				   brcmf_skb_htod_tag_get_field(skb, HSLOT), 0);
	/* the packet never reached firmware so reclaim credit */
	if (fws->fcmode == BRCMF_FWS_FCMODE_EXPLICIT_CREDIT &&
	    brcmf_skbcb(skb)->if_flags & BRCMF_SKB_IF_FLAGS_CREDITCHECK_MASK) {
		brcmf_fws_return_credits(fws,
					 brcmf_skb_htod_tag_get_field(skb,
								      FIFO),
					 1);
		brcmf_fws_schedule_deq(fws);
	}
	brcmf_fws_unlock(fws->drvr, flags);
=======
	u32 hslot;

	if (brcmf_skbcb(skb)->state == BRCMF_FWS_SKBSTATE_TIM) {
		brcmu_pkt_buf_free_skb(skb);
		return;
	}
	brcmf_fws_lock(fws);
	hslot = brcmf_skb_htod_tag_get_field(skb, HSLOT);
	brcmf_fws_txs_process(fws, BRCMF_FWS_TXSTATUS_HOST_TOSSED, hslot, 0, 0);
	brcmf_fws_unlock(fws);
}

void brcmf_fws_bus_blocked(struct brcmf_pub *drvr, bool flow_blocked)
{
	struct brcmf_fws_info *fws = drvr->fws;

	fws->bus_flow_blocked = flow_blocked;
	if (!flow_blocked)
		brcmf_fws_schedule_deq(fws);
	else
		fws->stats.bus_flow_block++;
>>>>>>> v3.18
}
