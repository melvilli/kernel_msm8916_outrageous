/* -----------------------------------------------------------------------------
 * Copyright (c) 2011 Ozmo Inc
 * Released under the GNU General Public License Version 2 (GPLv2).
 *
 * This file provides the implementation of a USB host controller device that
 * does not have any associated hardware. Instead the virtual device is
 * connected to the WiFi network and emulates the operation of a USB hcd by
 * receiving and sending network frames.
 * Note:
 * We take great pains to reduce the amount of code where interrupts need to be
 * disabled and in this respect we are different from standard HCD's. In
 * particular we don't want in_irq() code bleeding over to the protocol side of
 * the driver.
 * The troublesome functions are the urb enqueue and dequeue functions both of
 * which can be called in_irq(). So for these functions we put the urbs into a
 * queue and request a tasklet to process them. This means that a spinlock with
 * interrupts disabled must be held for insertion and removal but most code is
 * is in tasklet or soft irq context. The lock that protects this list is called
 * the tasklet lock and serves the purpose of the 'HCD lock' which must be held
 * when calling the following functions.
 *   usb_hcd_link_urb_to_ep()
 *   usb_hcd_unlink_urb_from_ep()
 *   usb_hcd_flush_endpoint()
 *   usb_hcd_check_unlink_urb()
 * -----------------------------------------------------------------------------
 */
#include <linux/platform_device.h>
#include <linux/usb.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/jiffies.h>
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
#include <linux/slab.h>
#include <linux/export.h>
#include "linux/usb/hcd.h"
#include <asm/unaligned.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include "ozconfig.h"
#include "ozusbif.h"
#include "oztrace.h"
#include "ozurbparanoia.h"
#include "ozevent.h"
#include "ozhcd.h"
/*------------------------------------------------------------------------------
 * Number of units of buffering to capture for an isochronous IN endpoint before
 * allowing data to be indicated up.
 */
#define OZ_IN_BUFFERING_UNITS	50
/* Name of our platform device.
 */
#define OZ_PLAT_DEV_NAME	"ozwpan"
/* Maximum number of free urb links that can be kept in the pool.
 */
#define OZ_MAX_LINK_POOL_SIZE	16
/* Get endpoint object from the containing link.
 */
#define ep_from_link(__e) container_of((__e), struct oz_endpoint, link)
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
#include "ozdbg.h"
#include "ozusbif.h"
#include "ozurbparanoia.h"
#include "ozhcd.h"

/*
 * Number of units of buffering to capture for an isochronous IN endpoint before
 * allowing data to be indicated up.
 */
#define OZ_IN_BUFFERING_UNITS	100

/* Name of our platform device.
 */
#define OZ_PLAT_DEV_NAME	"ozwpan"

/*EP0 timeout before ep0 request is again added to TX queue. (13*8 = 98mSec)
 */
#define EP0_TIMEOUT_COUNTER 13

/* Debounce time HCD driver should wait before unregistering.
 */
#define OZ_HUB_DEBOUNCE_TIMEOUT 1500

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Used to link urbs together and also store some status information for each
 * urb.
 * A cache of these are kept in a pool to reduce number of calls to kmalloc.
 */
struct oz_urb_link {
	struct list_head link;
	struct urb *urb;
	struct oz_port *port;
	u8 req_id;
	u8 ep_num;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long submit_jiffies;
};

/* Holds state information about a USB endpoint.
 */
=======
=======
>>>>>>> v3.18
	unsigned submit_counter;
};

static struct kmem_cache *oz_urb_link_cache;

/* Holds state information about a USB endpoint.
 */
#define OZ_EP_BUFFER_SIZE_ISOC  (1024 * 24)
#define OZ_EP_BUFFER_SIZE_INT   512
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
struct oz_endpoint {
	struct list_head urb_list;	/* List of oz_urb_link items. */
	struct list_head link;		/* For isoc ep, links in to isoc
					   lists of oz_port. */
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long last_jiffies;
=======
	struct timespec timestamp;
>>>>>>> v3.18
=======
	struct timespec timestamp;
>>>>>>> v3.18
	int credit;
	int credit_ceiling;
	u8 ep_num;
	u8 attrib;
	u8 *buffer;
	int buffer_size;
	int in_ix;
	int out_ix;
	int buffered_units;
	unsigned flags;
	int start_frame;
};
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
/* Bits in the flags field. */
#define OZ_F_EP_BUFFERING	0x1
#define OZ_F_EP_HAVE_STREAM	0x2

/* Holds state information about a USB interface.
 */
struct oz_interface {
	unsigned ep_mask;
	u8 alt;
};

/* Holds state information about an hcd port.
 */
#define OZ_NB_ENDPOINTS	16
struct oz_port {
	unsigned flags;
	unsigned status;
	void *hpd;
	struct oz_hcd *ozhcd;
	spinlock_t port_lock;
	u8 bus_addr;
	u8 next_req_id;
	u8 config_num;
	int num_iface;
	struct oz_interface *iface;
	struct oz_endpoint *out_ep[OZ_NB_ENDPOINTS];
	struct oz_endpoint *in_ep[OZ_NB_ENDPOINTS];
	struct list_head isoc_out_ep;
	struct list_head isoc_in_ep;
};
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
#define OZ_PORT_F_PRESENT	0x1
#define OZ_PORT_F_CHANGED	0x2
#define OZ_PORT_F_DYING		0x4

/* Data structure in the private context area of struct usb_hcd.
 */
#define OZ_NB_PORTS	8
struct oz_hcd {
	spinlock_t hcd_lock;
	struct list_head urb_pending_list;
	struct list_head urb_cancel_list;
	struct list_head orphanage;
	int conn_port; /* Port that is currently connecting, -1 if none.*/
	struct oz_port ports[OZ_NB_PORTS];
	uint flags;
	struct usb_hcd *hcd;
};
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
/* Bits in flags field.
 */
#define OZ_HDC_F_SUSPENDED	0x1

<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======
/*
>>>>>>> v3.18
=======
/*
>>>>>>> v3.18
 * Static function prototypes.
 */
static int oz_hcd_start(struct usb_hcd *hcd);
static void oz_hcd_stop(struct usb_hcd *hcd);
static void oz_hcd_shutdown(struct usb_hcd *hcd);
static int oz_hcd_urb_enqueue(struct usb_hcd *hcd, struct urb *urb,
				gfp_t mem_flags);
static int oz_hcd_urb_dequeue(struct usb_hcd *hcd, struct urb *urb, int status);
static void oz_hcd_endpoint_disable(struct usb_hcd *hcd,
				struct usb_host_endpoint *ep);
static void oz_hcd_endpoint_reset(struct usb_hcd *hcd,
				struct usb_host_endpoint *ep);
static int oz_hcd_get_frame_number(struct usb_hcd *hcd);
static int oz_hcd_hub_status_data(struct usb_hcd *hcd, char *buf);
static int oz_hcd_hub_control(struct usb_hcd *hcd, u16 req_type, u16 wvalue,
				u16 windex, char *buf, u16 wlength);
static int oz_hcd_bus_suspend(struct usb_hcd *hcd);
static int oz_hcd_bus_resume(struct usb_hcd *hcd);
static int oz_plat_probe(struct platform_device *dev);
static int oz_plat_remove(struct platform_device *dev);
static void oz_plat_shutdown(struct platform_device *dev);
static int oz_plat_suspend(struct platform_device *dev, pm_message_t msg);
static int oz_plat_resume(struct platform_device *dev);
static void oz_urb_process_tasklet(unsigned long unused);
static int oz_build_endpoints_for_config(struct usb_hcd *hcd,
		struct oz_port *port, struct usb_host_config *config,
		gfp_t mem_flags);
static void oz_clean_endpoints_for_config(struct usb_hcd *hcd,
				struct oz_port *port);
static int oz_build_endpoints_for_interface(struct usb_hcd *hcd,
			struct oz_port *port,
			struct usb_host_interface *intf, gfp_t mem_flags);
static void oz_clean_endpoints_for_interface(struct usb_hcd *hcd,
			struct oz_port *port, int if_ix);
static void oz_process_ep0_urb(struct oz_hcd *ozhcd, struct urb *urb,
		gfp_t mem_flags);
static struct oz_urb_link *oz_remove_urb(struct oz_endpoint *ep,
		struct urb *urb);
static void oz_hcd_clear_orphanage(struct oz_hcd *ozhcd, int status);
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Static external variables.
 */
static struct platform_device *g_plat_dev;
static struct oz_hcd *g_ozhcd;
static DEFINE_SPINLOCK(g_hcdlock);	/* Guards g_ozhcd. */
static const char g_hcd_name[] = "Ozmo WPAN";
<<<<<<< HEAD
<<<<<<< HEAD
static struct list_head *g_link_pool;
static int g_link_pool_size;
static DEFINE_SPINLOCK(g_link_lock);
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
static DEFINE_SPINLOCK(g_tasklet_lock);
static struct tasklet_struct g_urb_process_tasklet;
static struct tasklet_struct g_urb_cancel_tasklet;
static atomic_t g_pending_urbs = ATOMIC_INIT(0);
<<<<<<< HEAD
<<<<<<< HEAD
=======
static atomic_t g_usb_frame_number = ATOMIC_INIT(0);
>>>>>>> v3.18
=======
static atomic_t g_usb_frame_number = ATOMIC_INIT(0);
>>>>>>> v3.18
static const struct hc_driver g_oz_hc_drv = {
	.description =		g_hcd_name,
	.product_desc =		"Ozmo Devices WPAN",
	.hcd_priv_size =	sizeof(struct oz_hcd),
	.flags =		HCD_USB11,
	.start =		oz_hcd_start,
	.stop =			oz_hcd_stop,
	.shutdown =		oz_hcd_shutdown,
	.urb_enqueue =		oz_hcd_urb_enqueue,
	.urb_dequeue =		oz_hcd_urb_dequeue,
	.endpoint_disable =	oz_hcd_endpoint_disable,
	.endpoint_reset =	oz_hcd_endpoint_reset,
	.get_frame_number =	oz_hcd_get_frame_number,
	.hub_status_data =	oz_hcd_hub_status_data,
	.hub_control =		oz_hcd_hub_control,
	.bus_suspend =		oz_hcd_bus_suspend,
	.bus_resume =		oz_hcd_bus_resume,
};

static struct platform_driver g_oz_plat_drv = {
	.probe = oz_plat_probe,
	.remove = oz_plat_remove,
	.shutdown = oz_plat_shutdown,
	.suspend = oz_plat_suspend,
	.resume = oz_plat_resume,
	.driver = {
		.name = OZ_PLAT_DEV_NAME,
		.owner = THIS_MODULE,
	},
};
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Gets our private context area (which is of type struct oz_hcd) from the
 * usb_hcd structure.
 * Context: any
 */
static inline struct oz_hcd *oz_hcd_private(struct usb_hcd *hcd)
{
	return (struct oz_hcd *)hcd->hcd_priv;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Searches list of ports to find the index of the one with a specified  USB
 * bus address. If none of the ports has the bus address then the connection
 * port is returned, if there is one or -1 otherwise.
 * Context: any
 */
static int oz_get_port_from_addr(struct oz_hcd *ozhcd, u8 bus_addr)
{
	int i;
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
	for (i = 0; i < OZ_NB_PORTS; i++) {
		if (ozhcd->ports[i].bus_addr == bus_addr)
			return i;
	}
	return ozhcd->conn_port;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
 * Allocates an urb link, first trying the pool but going to heap if empty.
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: any
 */
static struct oz_urb_link *oz_alloc_urb_link(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct oz_urb_link *urbl = NULL;
	unsigned long irq_state;
	spin_lock_irqsave(&g_link_lock, irq_state);
	if (g_link_pool) {
		urbl = container_of(g_link_pool, struct oz_urb_link, link);
		g_link_pool = urbl->link.next;
		--g_link_pool_size;
	}
	spin_unlock_irqrestore(&g_link_lock, irq_state);
	if (urbl == NULL)
		urbl = kmalloc(sizeof(struct oz_urb_link), GFP_ATOMIC);
	return urbl;
}
/*------------------------------------------------------------------------------
 * Frees an urb link by putting it in the pool if there is enough space or
 * deallocating it to heap otherwise.
=======
=======
>>>>>>> v3.18
	return kmem_cache_alloc(oz_urb_link_cache, GFP_ATOMIC);
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: any
 */
static void oz_free_urb_link(struct oz_urb_link *urbl)
{
<<<<<<< HEAD
<<<<<<< HEAD
	if (urbl) {
		unsigned long irq_state;
		spin_lock_irqsave(&g_link_lock, irq_state);
		if (g_link_pool_size < OZ_MAX_LINK_POOL_SIZE) {
			urbl->link.next = g_link_pool;
			g_link_pool = &urbl->link;
			urbl = NULL;
			g_link_pool_size++;
		}
		spin_unlock_irqrestore(&g_link_lock, irq_state);
		kfree(urbl);
	}
}
/*------------------------------------------------------------------------------
 * Deallocates all the urb links in the pool.
 * Context: unknown
 */
static void oz_empty_link_pool(void)
{
	struct list_head *e;
	unsigned long irq_state;
	spin_lock_irqsave(&g_link_lock, irq_state);
	e = g_link_pool;
	g_link_pool = NULL;
	g_link_pool_size = 0;
	spin_unlock_irqrestore(&g_link_lock, irq_state);
	while (e) {
		struct oz_urb_link *urbl =
			container_of(e, struct oz_urb_link, link);
		e = e->next;
		kfree(urbl);
	}
}
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
	if (!urbl)
		return;

	kmem_cache_free(oz_urb_link_cache, urbl);
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Allocates endpoint structure and optionally a buffer. If a buffer is
 * allocated it immediately follows the endpoint structure.
 * Context: softirq
 */
<<<<<<< HEAD
<<<<<<< HEAD
static struct oz_endpoint *oz_ep_alloc(gfp_t mem_flags, int buffer_size)
=======
static struct oz_endpoint *oz_ep_alloc(int buffer_size, gfp_t mem_flags)
>>>>>>> v3.18
=======
static struct oz_endpoint *oz_ep_alloc(int buffer_size, gfp_t mem_flags)
>>>>>>> v3.18
{
	struct oz_endpoint *ep =
		kzalloc(sizeof(struct oz_endpoint)+buffer_size, mem_flags);
	if (ep) {
		INIT_LIST_HEAD(&ep->urb_list);
		INIT_LIST_HEAD(&ep->link);
		ep->credit = -1;
		if (buffer_size) {
			ep->buffer_size = buffer_size;
			ep->buffer = (u8 *)(ep+1);
		}
	}
	return ep;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Pre-condition: Must be called with g_tasklet_lock held and interrupts
 * disabled.
 * Context: softirq or process
 */
<<<<<<< HEAD
<<<<<<< HEAD
static struct oz_urb_link *oz_uncancel_urb(struct oz_hcd *ozhcd, struct urb *urb)
{
	struct oz_urb_link *urbl;
	struct list_head *e;
	list_for_each(e, &ozhcd->urb_cancel_list) {
		urbl = container_of(e, struct oz_urb_link, link);
		if (urb == urbl->urb) {
			list_del_init(e);
=======
=======
>>>>>>> v3.18
static struct oz_urb_link *oz_uncancel_urb(struct oz_hcd *ozhcd,
		struct urb *urb)
{
	struct oz_urb_link *urbl;

	list_for_each_entry(urbl, &ozhcd->urb_cancel_list, link) {
		if (urb == urbl->urb) {
			list_del_init(&urbl->link);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			return urbl;
		}
	}
	return NULL;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * This is called when we have finished processing an urb. It unlinks it from
 * the ep and returns it to the core.
 * Context: softirq or process
 */
static void oz_complete_urb(struct usb_hcd *hcd, struct urb *urb,
<<<<<<< HEAD
<<<<<<< HEAD
		int status, unsigned long submit_jiffies)
{
	struct oz_hcd *ozhcd = oz_hcd_private(hcd);
	unsigned long irq_state;
	struct oz_urb_link *cancel_urbl = NULL;
=======
=======
>>>>>>> v3.18
		int status)
{
	struct oz_hcd *ozhcd = oz_hcd_private(hcd);
	unsigned long irq_state;
	struct oz_urb_link *cancel_urbl;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	spin_lock_irqsave(&g_tasklet_lock, irq_state);
	usb_hcd_unlink_urb_from_ep(hcd, urb);
	/* Clear hcpriv which will prevent it being put in the cancel list
	 * in the event that an attempt is made to cancel it.
	 */
	urb->hcpriv = NULL;
	/* Walk the cancel list in case the urb is already sitting there.
	 * Since we process the cancel list in a tasklet rather than in
	 * the dequeue function this could happen.
	 */
	cancel_urbl = oz_uncancel_urb(ozhcd, urb);
	/* Note: we release lock but do not enable local irqs.
	 * It appears that usb_hcd_giveback_urb() expects irqs to be disabled,
	 * or at least other host controllers disable interrupts at this point
	 * so we do the same. We must, however, release the lock otherwise a
	 * deadlock will occur if an urb is submitted to our driver in the urb
	 * completion function. Because we disable interrupts it is possible
	 * that the urb_enqueue function can be called with them disabled.
	 */
	spin_unlock(&g_tasklet_lock);
	if (oz_forget_urb(urb)) {
<<<<<<< HEAD
<<<<<<< HEAD
		oz_trace("OZWPAN: ERROR Unknown URB %p\n", urb);
	} else {
		static unsigned long last_time;
		atomic_dec(&g_pending_urbs);
		oz_trace2(OZ_TRACE_URB,
			"%lu: giveback_urb(%p,%x) %lu %lu pending:%d\n",
			jiffies, urb, status, jiffies-submit_jiffies,
			jiffies-last_time, atomic_read(&g_pending_urbs));
		last_time = jiffies;
		oz_event_log(OZ_EVT_URB_DONE, 0, 0, urb, status);
=======
		oz_dbg(ON, "ERROR Unknown URB %p\n", urb);
	} else {
		atomic_dec(&g_pending_urbs);
>>>>>>> v3.18
=======
		oz_dbg(ON, "ERROR Unknown URB %p\n", urb);
	} else {
		atomic_dec(&g_pending_urbs);
>>>>>>> v3.18
		usb_hcd_giveback_urb(hcd, urb, status);
	}
	spin_lock(&g_tasklet_lock);
	spin_unlock_irqrestore(&g_tasklet_lock, irq_state);
	if (cancel_urbl)
		oz_free_urb_link(cancel_urbl);
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Deallocates an endpoint including deallocating any associated stream and
 * returning any queued urbs to the core.
 * Context: softirq
 */
static void oz_ep_free(struct oz_port *port, struct oz_endpoint *ep)
{
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("oz_ep_free()\n");
	if (port) {
		struct list_head list;
		struct oz_hcd *ozhcd = port->ozhcd;
		INIT_LIST_HEAD(&list);
=======
=======
>>>>>>> v3.18
	if (port) {
		LIST_HEAD(list);
		struct oz_hcd *ozhcd = port->ozhcd;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		if (ep->flags & OZ_F_EP_HAVE_STREAM)
			oz_usb_stream_delete(port->hpd, ep->ep_num);
		/* Transfer URBs to the orphanage while we hold the lock. */
		spin_lock_bh(&ozhcd->hcd_lock);
		/* Note: this works even if ep->urb_list is empty.*/
		list_replace_init(&ep->urb_list, &list);
		/* Put the URBs in the orphanage. */
		list_splice_tail(&list, &ozhcd->orphanage);
		spin_unlock_bh(&ozhcd->hcd_lock);
	}
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("Freeing endpoint memory\n");
	kfree(ep);
}
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
	oz_dbg(ON, "Freeing endpoint memory\n");
	kfree(ep);
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: softirq
 */
static void oz_complete_buffered_urb(struct oz_port *port,
			struct oz_endpoint *ep,
			struct urb *urb)
{
<<<<<<< HEAD
<<<<<<< HEAD
	u8 data_len, available_space, copy_len;

	memcpy(&data_len, &ep->buffer[ep->out_ix], sizeof(u8));
=======
	int data_len, available_space, copy_len;

	data_len = ep->buffer[ep->out_ix];
>>>>>>> v3.18
=======
	int data_len, available_space, copy_len;

	data_len = ep->buffer[ep->out_ix];
>>>>>>> v3.18
	if (data_len <= urb->transfer_buffer_length)
		available_space = data_len;
	else
		available_space = urb->transfer_buffer_length;

	if (++ep->out_ix == ep->buffer_size)
		ep->out_ix = 0;
	copy_len = ep->buffer_size - ep->out_ix;
	if (copy_len >= available_space)
		copy_len = available_space;
	memcpy(urb->transfer_buffer, &ep->buffer[ep->out_ix], copy_len);

	if (copy_len < available_space) {
		memcpy((urb->transfer_buffer + copy_len), ep->buffer,
						(available_space - copy_len));
		ep->out_ix = available_space - copy_len;
	} else {
		ep->out_ix += copy_len;
	}
	urb->actual_length = available_space;
	if (ep->out_ix == ep->buffer_size)
		ep->out_ix = 0;

	ep->buffered_units--;
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("Trying to give back buffered frame of size=%d\n",
						available_space);
	oz_complete_urb(port->ozhcd->hcd, urb, 0, 0);
}

/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
	oz_dbg(ON, "Trying to give back buffered frame of size=%d\n",
	       available_space);
	oz_complete_urb(port->ozhcd->hcd, urb, 0);
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: softirq
 */
static int oz_enqueue_ep_urb(struct oz_port *port, u8 ep_addr, int in_dir,
			struct urb *urb, u8 req_id)
{
	struct oz_urb_link *urbl;
<<<<<<< HEAD
<<<<<<< HEAD
	struct oz_endpoint *ep;
	int err = 0;
	if (ep_addr >= OZ_NB_ENDPOINTS) {
		oz_trace("Invalid endpoint number in oz_enqueue_ep_urb().\n");
=======
=======
>>>>>>> v3.18
	struct oz_endpoint *ep = NULL;
	int err = 0;

	if (ep_addr >= OZ_NB_ENDPOINTS) {
		oz_dbg(ON, "%s: Invalid endpoint number\n", __func__);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return -EINVAL;
	}
	urbl = oz_alloc_urb_link();
	if (!urbl)
		return -ENOMEM;
<<<<<<< HEAD
<<<<<<< HEAD
	urbl->submit_jiffies = jiffies;
=======
	urbl->submit_counter = 0;
>>>>>>> v3.18
=======
	urbl->submit_counter = 0;
>>>>>>> v3.18
	urbl->urb = urb;
	urbl->req_id = req_id;
	urbl->ep_num = ep_addr;
	/* Hold lock while we insert the URB into the list within the
	 * endpoint structure.
	 */
	spin_lock_bh(&port->ozhcd->hcd_lock);
	/* If the urb has been unlinked while out of any list then
	 * complete it now.
	 */
	if (urb->unlinked) {
		spin_unlock_bh(&port->ozhcd->hcd_lock);
<<<<<<< HEAD
<<<<<<< HEAD
		oz_trace("urb %p unlinked so complete immediately\n", urb);
		oz_complete_urb(port->ozhcd->hcd, urb, 0, 0);
		oz_free_urb_link(urbl);
		return 0;
	}
=======
=======
>>>>>>> v3.18
		oz_dbg(ON, "urb %p unlinked so complete immediately\n", urb);
		oz_complete_urb(port->ozhcd->hcd, urb, 0);
		oz_free_urb_link(urbl);
		return 0;
	}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (in_dir)
		ep = port->in_ep[ep_addr];
	else
		ep = port->out_ep[ep_addr];
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
	if (!ep) {
		err = -ENOMEM;
		goto out;
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/*For interrupt endpoint check for buffered data
	* & complete urb
	*/
	if (((ep->attrib & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_INT)
						 && ep->buffered_units > 0) {
		oz_free_urb_link(urbl);
		spin_unlock_bh(&port->ozhcd->hcd_lock);
		oz_complete_buffered_urb(port, ep, urb);
		return 0;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (ep && port->hpd) {
		list_add_tail(&urbl->link, &ep->urb_list);
		if (!in_dir && ep_addr && (ep->credit < 0)) {
			ep->last_jiffies = jiffies;
			ep->credit = 0;
			oz_event_log(OZ_EVT_EP_CREDIT, ep->ep_num,
					0, NULL, ep->credit);
=======
=======
>>>>>>> v3.18
	if (port->hpd) {
		list_add_tail(&urbl->link, &ep->urb_list);
		if (!in_dir && ep_addr && (ep->credit < 0)) {
			getrawmonotonic(&ep->timestamp);
			ep->credit = 0;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		}
	} else {
		err = -EPIPE;
	}
<<<<<<< HEAD
<<<<<<< HEAD
=======
out:
>>>>>>> v3.18
=======
out:
>>>>>>> v3.18
	spin_unlock_bh(&port->ozhcd->hcd_lock);
	if (err)
		oz_free_urb_link(urbl);
	return err;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Removes an urb from the queue in the endpoint.
 * Returns 0 if it is found and -EIDRM otherwise.
 * Context: softirq
 */
static int oz_dequeue_ep_urb(struct oz_port *port, u8 ep_addr, int in_dir,
			struct urb *urb)
{
	struct oz_urb_link *urbl = NULL;
	struct oz_endpoint *ep;
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
	spin_lock_bh(&port->ozhcd->hcd_lock);
	if (in_dir)
		ep = port->in_ep[ep_addr];
	else
		ep = port->out_ep[ep_addr];
	if (ep) {
		struct list_head *e;
<<<<<<< HEAD
<<<<<<< HEAD
		list_for_each(e, &ep->urb_list) {
			urbl = container_of(e, struct oz_urb_link, link);
=======

		list_for_each(e, &ep->urb_list) {
			urbl = list_entry(e, struct oz_urb_link, link);
>>>>>>> v3.18
=======

		list_for_each(e, &ep->urb_list) {
			urbl = list_entry(e, struct oz_urb_link, link);
>>>>>>> v3.18
			if (urbl->urb == urb) {
				list_del_init(e);
				break;
			}
			urbl = NULL;
		}
	}
	spin_unlock_bh(&port->ozhcd->hcd_lock);
	if (urbl)
		oz_free_urb_link(urbl);
	return urbl ? 0 : -EIDRM;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Finds an urb given its request id.
 * Context: softirq
 */
static struct urb *oz_find_urb_by_id(struct oz_port *port, int ep_ix,
		u8 req_id)
{
	struct oz_hcd *ozhcd = port->ozhcd;
	struct urb *urb = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
	struct oz_urb_link *urbl = NULL;
=======
	struct oz_urb_link *urbl;
>>>>>>> v3.18
=======
	struct oz_urb_link *urbl;
>>>>>>> v3.18
	struct oz_endpoint *ep;

	spin_lock_bh(&ozhcd->hcd_lock);
	ep = port->out_ep[ep_ix];
	if (ep) {
		struct list_head *e;
<<<<<<< HEAD
<<<<<<< HEAD
		list_for_each(e, &ep->urb_list) {
			urbl = container_of(e, struct oz_urb_link, link);
=======

		list_for_each(e, &ep->urb_list) {
			urbl = list_entry(e, struct oz_urb_link, link);
>>>>>>> v3.18
=======

		list_for_each(e, &ep->urb_list) {
			urbl = list_entry(e, struct oz_urb_link, link);
>>>>>>> v3.18
			if (urbl->req_id == req_id) {
				urb = urbl->urb;
				list_del_init(e);
				break;
			}
		}
	}
	spin_unlock_bh(&ozhcd->hcd_lock);
	/* If urb is non-zero then we we must have an urb link to delete.
	 */
	if (urb)
		oz_free_urb_link(urbl);
	return urb;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Pre-condition: Port lock must be held.
 * Context: softirq
 */
static void oz_acquire_port(struct oz_port *port, void *hpd)
{
	INIT_LIST_HEAD(&port->isoc_out_ep);
	INIT_LIST_HEAD(&port->isoc_in_ep);
	port->flags |= OZ_PORT_F_PRESENT | OZ_PORT_F_CHANGED;
	port->status |= USB_PORT_STAT_CONNECTION |
			(USB_PORT_STAT_C_CONNECTION << 16);
	oz_usb_get(hpd);
	port->hpd = hpd;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: softirq
 */
static struct oz_hcd *oz_hcd_claim(void)
{
	struct oz_hcd *ozhcd;
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
	spin_lock_bh(&g_hcdlock);
	ozhcd = g_ozhcd;
	if (ozhcd)
		usb_get_hcd(ozhcd->hcd);
	spin_unlock_bh(&g_hcdlock);
	return ozhcd;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: softirq
 */
static inline void oz_hcd_put(struct oz_hcd *ozhcd)
{
	if (ozhcd)
		usb_put_hcd(ozhcd->hcd);
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * This is called by the protocol handler to notify that a PD has arrived.
 * We allocate a port to associate with the PD and create a structure for
 * endpoint 0. This port is made the connection port.
 * In the event that one of the other port is already a connection port then
 * we fail.
 * TODO We should be able to do better than fail and should be able remember
 * that this port needs configuring and make it the connection port once the
 * current connection port has been assigned an address. Collisions here are
 * probably very rare indeed.
 * Context: softirq
 */
<<<<<<< HEAD
<<<<<<< HEAD
void *oz_hcd_pd_arrived(void *hpd)
{
	int i;
	void *hport = NULL;
	struct oz_hcd *ozhcd = NULL;
	struct oz_endpoint *ep;
	oz_trace("oz_hcd_pd_arrived()\n");
	ozhcd = oz_hcd_claim();
	if (ozhcd == NULL)
=======
=======
>>>>>>> v3.18
struct oz_port *oz_hcd_pd_arrived(void *hpd)
{
	int i;
	struct oz_port *hport;
	struct oz_hcd *ozhcd;
	struct oz_endpoint *ep;

	ozhcd = oz_hcd_claim();
	if (!ozhcd)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return NULL;
	/* Allocate an endpoint object in advance (before holding hcd lock) to
	 * use for out endpoint 0.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	ep = oz_ep_alloc(GFP_ATOMIC, 0);
	spin_lock_bh(&ozhcd->hcd_lock);
	if (ozhcd->conn_port >= 0) {
		spin_unlock_bh(&ozhcd->hcd_lock);
		oz_trace("conn_port >= 0\n");
		goto out;
	}
	for (i = 0; i < OZ_NB_PORTS; i++) {
		struct oz_port *port = &ozhcd->ports[i];
		spin_lock(&port->port_lock);
		if ((port->flags & OZ_PORT_F_PRESENT) == 0) {
=======
=======
>>>>>>> v3.18
	ep = oz_ep_alloc(0, GFP_ATOMIC);
	if (!ep)
		goto err_put;

	spin_lock_bh(&ozhcd->hcd_lock);
	if (ozhcd->conn_port >= 0)
		goto err_unlock;

	for (i = 0; i < OZ_NB_PORTS; i++) {
		struct oz_port *port = &ozhcd->ports[i];

		spin_lock(&port->port_lock);
		if (!(port->flags & (OZ_PORT_F_PRESENT | OZ_PORT_F_CHANGED))) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			oz_acquire_port(port, hpd);
			spin_unlock(&port->port_lock);
			break;
		}
		spin_unlock(&port->port_lock);
	}
<<<<<<< HEAD
<<<<<<< HEAD
	if (i < OZ_NB_PORTS) {
		oz_trace("Setting conn_port = %d\n", i);
		ozhcd->conn_port = i;
		/* Attach out endpoint 0.
		 */
		ozhcd->ports[i].out_ep[0] = ep;
		ep = NULL;
		hport = &ozhcd->ports[i];
		spin_unlock_bh(&ozhcd->hcd_lock);
		if (ozhcd->flags & OZ_HDC_F_SUSPENDED) {
			oz_trace("Resuming root hub\n");
			usb_hcd_resume_root_hub(ozhcd->hcd);
		}
		usb_hcd_poll_rh_status(ozhcd->hcd);
	} else {
		spin_unlock_bh(&ozhcd->hcd_lock);
	}
out:
	if (ep) /* ep is non-null if not used. */
		oz_ep_free(NULL, ep);
	oz_hcd_put(ozhcd);
	return hport;
}
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
	if (i == OZ_NB_PORTS)
		goto err_unlock;

	ozhcd->conn_port = i;
	hport = &ozhcd->ports[i];
	hport->out_ep[0] = ep;
	spin_unlock_bh(&ozhcd->hcd_lock);
	if (ozhcd->flags & OZ_HDC_F_SUSPENDED)
		usb_hcd_resume_root_hub(ozhcd->hcd);
	usb_hcd_poll_rh_status(ozhcd->hcd);
	oz_hcd_put(ozhcd);

	return hport;

err_unlock:
	spin_unlock_bh(&ozhcd->hcd_lock);
	oz_ep_free(NULL, ep);
err_put:
	oz_hcd_put(ozhcd);
	return NULL;
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * This is called by the protocol handler to notify that the PD has gone away.
 * We need to deallocate all resources and then request that the root hub is
 * polled. We release the reference we hold on the PD.
 * Context: softirq
 */
<<<<<<< HEAD
<<<<<<< HEAD
void oz_hcd_pd_departed(void *hport)
{
	struct oz_port *port = (struct oz_port *)hport;
=======
void oz_hcd_pd_departed(struct oz_port *port)
{
>>>>>>> v3.18
=======
void oz_hcd_pd_departed(struct oz_port *port)
{
>>>>>>> v3.18
	struct oz_hcd *ozhcd;
	void *hpd;
	struct oz_endpoint *ep = NULL;

<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("oz_hcd_pd_departed()\n");
	if (port == NULL) {
		oz_trace("oz_hcd_pd_departed() port = 0\n");
=======
	if (port == NULL) {
		oz_dbg(ON, "%s: port = 0\n", __func__);
>>>>>>> v3.18
=======
	if (port == NULL) {
		oz_dbg(ON, "%s: port = 0\n", __func__);
>>>>>>> v3.18
		return;
	}
	ozhcd = port->ozhcd;
	if (ozhcd == NULL)
		return;
	/* Check if this is the connection port - if so clear it.
	 */
	spin_lock_bh(&ozhcd->hcd_lock);
	if ((ozhcd->conn_port >= 0) &&
		(port == &ozhcd->ports[ozhcd->conn_port])) {
<<<<<<< HEAD
<<<<<<< HEAD
		oz_trace("Clearing conn_port\n");
=======
		oz_dbg(ON, "Clearing conn_port\n");
>>>>>>> v3.18
=======
		oz_dbg(ON, "Clearing conn_port\n");
>>>>>>> v3.18
		ozhcd->conn_port = -1;
	}
	spin_lock(&port->port_lock);
	port->flags |= OZ_PORT_F_DYING;
	spin_unlock(&port->port_lock);
	spin_unlock_bh(&ozhcd->hcd_lock);

	oz_clean_endpoints_for_config(ozhcd->hcd, port);
	spin_lock_bh(&port->port_lock);
	hpd = port->hpd;
	port->hpd = NULL;
	port->bus_addr = 0xff;
<<<<<<< HEAD
<<<<<<< HEAD
	port->flags &= ~(OZ_PORT_F_PRESENT | OZ_PORT_F_DYING);
	port->flags |= OZ_PORT_F_CHANGED;
	port->status &= ~USB_PORT_STAT_CONNECTION;
=======
=======
>>>>>>> v3.18
	port->config_num = 0;
	port->flags &= ~(OZ_PORT_F_PRESENT | OZ_PORT_F_DYING);
	port->flags |= OZ_PORT_F_CHANGED;
	port->status &= ~(USB_PORT_STAT_CONNECTION | USB_PORT_STAT_ENABLE);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	port->status |= (USB_PORT_STAT_C_CONNECTION << 16);
	/* If there is an endpont 0 then clear the pointer while we hold
	 * the spinlock be we deallocate it after releasing the lock.
	 */
	if (port->out_ep[0]) {
		ep = port->out_ep[0];
		port->out_ep[0] = NULL;
	}
	spin_unlock_bh(&port->port_lock);
	if (ep)
		oz_ep_free(port, ep);
	usb_hcd_poll_rh_status(ozhcd->hcd);
	oz_usb_put(hpd);
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: softirq
 */
void oz_hcd_pd_reset(void *hpd, void *hport)
{
	/* Cleanup the current configuration and report reset to the core.
	 */
	struct oz_port *port = (struct oz_port *)hport;
	struct oz_hcd *ozhcd = port->ozhcd;
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("PD Reset\n");
=======

	oz_dbg(ON, "PD Reset\n");
>>>>>>> v3.18
=======

	oz_dbg(ON, "PD Reset\n");
>>>>>>> v3.18
	spin_lock_bh(&port->port_lock);
	port->flags |= OZ_PORT_F_CHANGED;
	port->status |= USB_PORT_STAT_RESET;
	port->status |= (USB_PORT_STAT_C_RESET << 16);
	spin_unlock_bh(&port->port_lock);
	oz_clean_endpoints_for_config(ozhcd->hcd, port);
	usb_hcd_poll_rh_status(ozhcd->hcd);
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: softirq
 */
void oz_hcd_get_desc_cnf(void *hport, u8 req_id, int status, const u8 *desc,
			int length, int offset, int total_size)
{
	struct oz_port *port = (struct oz_port *)hport;
	struct urb *urb;
	int err = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	oz_event_log(OZ_EVT_CTRL_CNF, 0, req_id, NULL, status);
	oz_trace("oz_hcd_get_desc_cnf length = %d offs = %d tot_size = %d\n",
			length, offset, total_size);
=======
	oz_dbg(ON, "oz_hcd_get_desc_cnf length = %d offs = %d tot_size = %d\n",
	       length, offset, total_size);
>>>>>>> v3.18
=======
	oz_dbg(ON, "oz_hcd_get_desc_cnf length = %d offs = %d tot_size = %d\n",
	       length, offset, total_size);
>>>>>>> v3.18
	urb = oz_find_urb_by_id(port, 0, req_id);
	if (!urb)
		return;
	if (status == 0) {
		int copy_len;
		int required_size = urb->transfer_buffer_length;
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
		if (required_size > total_size)
			required_size = total_size;
		copy_len = required_size-offset;
		if (length <= copy_len)
			copy_len = length;
		memcpy(urb->transfer_buffer+offset, desc, copy_len);
		offset += copy_len;
		if (offset < required_size) {
			struct usb_ctrlrequest *setup =
				(struct usb_ctrlrequest *)urb->setup_packet;
			unsigned wvalue = le16_to_cpu(setup->wValue);
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
			if (oz_enqueue_ep_urb(port, 0, 0, urb, req_id))
				err = -ENOMEM;
			else if (oz_usb_get_desc_req(port->hpd, req_id,
					setup->bRequestType, (u8)(wvalue>>8),
					(u8)wvalue, setup->wIndex, offset,
					required_size-offset)) {
				oz_dequeue_ep_urb(port, 0, 0, urb);
				err = -ENOMEM;
			}
			if (err == 0)
				return;
		}
	}
	urb->actual_length = total_size;
<<<<<<< HEAD
<<<<<<< HEAD
	oz_complete_urb(port->ozhcd->hcd, urb, 0, 0);
}
/*------------------------------------------------------------------------------
 * Context: softirq
 */
#ifdef WANT_TRACE
=======
=======
>>>>>>> v3.18
	oz_complete_urb(port->ozhcd->hcd, urb, 0);
}

/*
 * Context: softirq
 */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static void oz_display_conf_type(u8 t)
{
	switch (t) {
	case USB_REQ_GET_STATUS:
<<<<<<< HEAD
<<<<<<< HEAD
		oz_trace("USB_REQ_GET_STATUS - cnf\n");
		break;
	case USB_REQ_CLEAR_FEATURE:
		oz_trace("USB_REQ_CLEAR_FEATURE - cnf\n");
		break;
	case USB_REQ_SET_FEATURE:
		oz_trace("USB_REQ_SET_FEATURE - cnf\n");
		break;
	case USB_REQ_SET_ADDRESS:
		oz_trace("USB_REQ_SET_ADDRESS - cnf\n");
		break;
	case USB_REQ_GET_DESCRIPTOR:
		oz_trace("USB_REQ_GET_DESCRIPTOR - cnf\n");
		break;
	case USB_REQ_SET_DESCRIPTOR:
		oz_trace("USB_REQ_SET_DESCRIPTOR - cnf\n");
		break;
	case USB_REQ_GET_CONFIGURATION:
		oz_trace("USB_REQ_GET_CONFIGURATION - cnf\n");
		break;
	case USB_REQ_SET_CONFIGURATION:
		oz_trace("USB_REQ_SET_CONFIGURATION - cnf\n");
		break;
	case USB_REQ_GET_INTERFACE:
		oz_trace("USB_REQ_GET_INTERFACE - cnf\n");
		break;
	case USB_REQ_SET_INTERFACE:
		oz_trace("USB_REQ_SET_INTERFACE - cnf\n");
		break;
	case USB_REQ_SYNCH_FRAME:
		oz_trace("USB_REQ_SYNCH_FRAME - cnf\n");
		break;
	}
}
#else
#define oz_display_conf_type(__x)
#endif /* WANT_TRACE */
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
		oz_dbg(ON, "USB_REQ_GET_STATUS - cnf\n");
		break;
	case USB_REQ_CLEAR_FEATURE:
		oz_dbg(ON, "USB_REQ_CLEAR_FEATURE - cnf\n");
		break;
	case USB_REQ_SET_FEATURE:
		oz_dbg(ON, "USB_REQ_SET_FEATURE - cnf\n");
		break;
	case USB_REQ_SET_ADDRESS:
		oz_dbg(ON, "USB_REQ_SET_ADDRESS - cnf\n");
		break;
	case USB_REQ_GET_DESCRIPTOR:
		oz_dbg(ON, "USB_REQ_GET_DESCRIPTOR - cnf\n");
		break;
	case USB_REQ_SET_DESCRIPTOR:
		oz_dbg(ON, "USB_REQ_SET_DESCRIPTOR - cnf\n");
		break;
	case USB_REQ_GET_CONFIGURATION:
		oz_dbg(ON, "USB_REQ_GET_CONFIGURATION - cnf\n");
		break;
	case USB_REQ_SET_CONFIGURATION:
		oz_dbg(ON, "USB_REQ_SET_CONFIGURATION - cnf\n");
		break;
	case USB_REQ_GET_INTERFACE:
		oz_dbg(ON, "USB_REQ_GET_INTERFACE - cnf\n");
		break;
	case USB_REQ_SET_INTERFACE:
		oz_dbg(ON, "USB_REQ_SET_INTERFACE - cnf\n");
		break;
	case USB_REQ_SYNCH_FRAME:
		oz_dbg(ON, "USB_REQ_SYNCH_FRAME - cnf\n");
		break;
	}
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: softirq
 */
static void oz_hcd_complete_set_config(struct oz_port *port, struct urb *urb,
		u8 rcode, u8 config_num)
{
	int rc = 0;
	struct usb_hcd *hcd = port->ozhcd->hcd;
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
	if (rcode == 0) {
		port->config_num = config_num;
		oz_clean_endpoints_for_config(hcd, port);
		if (oz_build_endpoints_for_config(hcd, port,
			&urb->dev->config[port->config_num-1], GFP_ATOMIC)) {
			rc = -ENOMEM;
		}
	} else {
		rc = -ENOMEM;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	oz_complete_urb(hcd, urb, rc, 0);
}
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
	oz_complete_urb(hcd, urb, rc);
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: softirq
 */
static void oz_hcd_complete_set_interface(struct oz_port *port, struct urb *urb,
		u8 rcode, u8 if_num, u8 alt)
{
	struct usb_hcd *hcd = port->ozhcd->hcd;
	int rc = 0;
<<<<<<< HEAD
<<<<<<< HEAD
	if (rcode == 0) {
		struct usb_host_config *config;
		struct usb_host_interface *intf;
		oz_trace("Set interface %d alt %d\n", if_num, alt);
=======
=======
>>>>>>> v3.18

	if ((rcode == 0) && (port->config_num > 0)) {
		struct usb_host_config *config;
		struct usb_host_interface *intf;

		oz_dbg(ON, "Set interface %d alt %d\n", if_num, alt);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		oz_clean_endpoints_for_interface(hcd, port, if_num);
		config = &urb->dev->config[port->config_num-1];
		intf = &config->intf_cache[if_num]->altsetting[alt];
		if (oz_build_endpoints_for_interface(hcd, port, intf,
			GFP_ATOMIC))
			rc = -ENOMEM;
		else
			port->iface[if_num].alt = alt;
	} else {
		rc = -ENOMEM;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	oz_complete_urb(hcd, urb, rc, 0);
}
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
	oz_complete_urb(hcd, urb, rc);
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: softirq
 */
void oz_hcd_control_cnf(void *hport, u8 req_id, u8 rcode, const u8 *data,
	int data_len)
{
	struct oz_port *port = (struct oz_port *)hport;
	struct urb *urb;
	struct usb_ctrlrequest *setup;
	struct usb_hcd *hcd = port->ozhcd->hcd;
	unsigned windex;
	unsigned wvalue;

<<<<<<< HEAD
<<<<<<< HEAD
	oz_event_log(OZ_EVT_CTRL_CNF, 0, req_id, NULL, rcode);
	oz_trace("oz_hcd_control_cnf rcode=%u len=%d\n", rcode, data_len);
	urb = oz_find_urb_by_id(port, 0, req_id);
	if (!urb) {
		oz_trace("URB not found\n");
=======
=======
>>>>>>> v3.18
	oz_dbg(ON, "oz_hcd_control_cnf rcode=%u len=%d\n", rcode, data_len);
	urb = oz_find_urb_by_id(port, 0, req_id);
	if (!urb) {
		oz_dbg(ON, "URB not found\n");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return;
	}
	setup = (struct usb_ctrlrequest *)urb->setup_packet;
	windex = le16_to_cpu(setup->wIndex);
	wvalue = le16_to_cpu(setup->wValue);
	if ((setup->bRequestType & USB_TYPE_MASK) == USB_TYPE_STANDARD) {
		/* Standard requests */
		oz_display_conf_type(setup->bRequest);
		switch (setup->bRequest) {
		case USB_REQ_SET_CONFIGURATION:
			oz_hcd_complete_set_config(port, urb, rcode,
				(u8)wvalue);
			break;
		case USB_REQ_SET_INTERFACE:
			oz_hcd_complete_set_interface(port, urb, rcode,
				(u8)windex, (u8)wvalue);
			break;
		default:
<<<<<<< HEAD
<<<<<<< HEAD
			oz_complete_urb(hcd, urb, 0, 0);
=======
			oz_complete_urb(hcd, urb, 0);
>>>>>>> v3.18
=======
			oz_complete_urb(hcd, urb, 0);
>>>>>>> v3.18
		}

	} else {
		int copy_len;
<<<<<<< HEAD
<<<<<<< HEAD
		oz_trace("VENDOR-CLASS - cnf\n");
=======

		oz_dbg(ON, "VENDOR-CLASS - cnf\n");
>>>>>>> v3.18
=======

		oz_dbg(ON, "VENDOR-CLASS - cnf\n");
>>>>>>> v3.18
		if (data_len) {
			if (data_len <= urb->transfer_buffer_length)
				copy_len = data_len;
			else
				copy_len = urb->transfer_buffer_length;
			memcpy(urb->transfer_buffer, data, copy_len);
			urb->actual_length = copy_len;
		}
<<<<<<< HEAD
<<<<<<< HEAD
		oz_complete_urb(hcd, urb, 0, 0);
	}
}
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
		oz_complete_urb(hcd, urb, 0);
	}
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: softirq-serialized
 */
static int oz_hcd_buffer_data(struct oz_endpoint *ep, const u8 *data,
			      int data_len)
{
	int space;
	int copy_len;
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
	if (!ep->buffer)
		return -1;
	space = ep->out_ix-ep->in_ix-1;
	if (space < 0)
		space += ep->buffer_size;
	if (space < (data_len+1)) {
<<<<<<< HEAD
<<<<<<< HEAD
		oz_trace("Buffer full\n");
=======
		oz_dbg(ON, "Buffer full\n");
>>>>>>> v3.18
=======
		oz_dbg(ON, "Buffer full\n");
>>>>>>> v3.18
		return -1;
	}
	ep->buffer[ep->in_ix] = (u8)data_len;
	if (++ep->in_ix == ep->buffer_size)
		ep->in_ix = 0;
	copy_len = ep->buffer_size - ep->in_ix;
	if (copy_len > data_len)
		copy_len = data_len;
	memcpy(&ep->buffer[ep->in_ix], data, copy_len);

	if (copy_len < data_len) {
		memcpy(ep->buffer, data+copy_len, data_len-copy_len);
		ep->in_ix = data_len-copy_len;
	} else {
		ep->in_ix += copy_len;
	}
	if (ep->in_ix == ep->buffer_size)
		ep->in_ix = 0;
	ep->buffered_units++;
	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: softirq-serialized
 */
void oz_hcd_data_ind(void *hport, u8 endpoint, const u8 *data, int data_len)
{
	struct oz_port *port = (struct oz_port *)hport;
	struct oz_endpoint *ep;
	struct oz_hcd *ozhcd = port->ozhcd;
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
	spin_lock_bh(&ozhcd->hcd_lock);
	ep = port->in_ep[endpoint & USB_ENDPOINT_NUMBER_MASK];
	if (ep == NULL)
		goto done;
	switch (ep->attrib & USB_ENDPOINT_XFERTYPE_MASK) {
	case USB_ENDPOINT_XFER_INT:
	case USB_ENDPOINT_XFER_BULK:
		if (!list_empty(&ep->urb_list)) {
			struct oz_urb_link *urbl =
				list_first_entry(&ep->urb_list,
					struct oz_urb_link, link);
			struct urb *urb;
			int copy_len;
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
			list_del_init(&urbl->link);
			spin_unlock_bh(&ozhcd->hcd_lock);
			urb = urbl->urb;
			oz_free_urb_link(urbl);
			if (data_len <= urb->transfer_buffer_length)
				copy_len = data_len;
			else
				copy_len = urb->transfer_buffer_length;
			memcpy(urb->transfer_buffer, data, copy_len);
			urb->actual_length = copy_len;
<<<<<<< HEAD
<<<<<<< HEAD
			oz_complete_urb(port->ozhcd->hcd, urb, 0, 0);
			return;
		} else {
			oz_trace("buffering frame as URB is not available\n");
			oz_hcd_buffer_data(ep, data, data_len);
		}
=======
=======
>>>>>>> v3.18
			oz_complete_urb(port->ozhcd->hcd, urb, 0);
			return;
		}
		oz_dbg(ON, "buffering frame as URB is not available\n");
		oz_hcd_buffer_data(ep, data, data_len);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		break;
	case USB_ENDPOINT_XFER_ISOC:
		oz_hcd_buffer_data(ep, data, data_len);
		break;
	}
done:
	spin_unlock_bh(&ozhcd->hcd_lock);
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: unknown
 */
static inline int oz_usb_get_frame_number(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return jiffies_to_msecs(get_jiffies_64());
}
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
	return atomic_inc_return(&g_usb_frame_number);
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: softirq
 */
int oz_hcd_heartbeat(void *hport)
{
	int rc = 0;
	struct oz_port *port = (struct oz_port *)hport;
	struct oz_hcd *ozhcd = port->ozhcd;
<<<<<<< HEAD
<<<<<<< HEAD
	struct oz_urb_link *urbl;
	struct list_head xfr_list;
	struct list_head *e;
	struct list_head *n;
	struct urb *urb;
	struct oz_endpoint *ep;
	unsigned long now = jiffies;
	INIT_LIST_HEAD(&xfr_list);
	/* Check the OUT isoc endpoints to see if any URB data can be sent.
	 */
	spin_lock_bh(&ozhcd->hcd_lock);
	list_for_each(e, &port->isoc_out_ep) {
		ep = ep_from_link(e);
		if (ep->credit < 0)
			continue;
		ep->credit += jiffies_to_msecs(now - ep->last_jiffies);
		if (ep->credit > ep->credit_ceiling)
			ep->credit = ep->credit_ceiling;
		oz_event_log(OZ_EVT_EP_CREDIT, ep->ep_num, 0, NULL,
			     ep->credit);
		ep->last_jiffies = now;
=======
=======
>>>>>>> v3.18
	struct oz_urb_link *urbl, *n;
	LIST_HEAD(xfr_list);
	struct urb *urb;
	struct oz_endpoint *ep;
	struct timespec ts, delta;

	getrawmonotonic(&ts);
	/* Check the OUT isoc endpoints to see if any URB data can be sent.
	 */
	spin_lock_bh(&ozhcd->hcd_lock);
	list_for_each_entry(ep, &port->isoc_out_ep, link) {
		if (ep->credit < 0)
			continue;
		delta = timespec_sub(ts, ep->timestamp);
		ep->credit += div_u64(timespec_to_ns(&delta), NSEC_PER_MSEC);
		if (ep->credit > ep->credit_ceiling)
			ep->credit = ep->credit_ceiling;
		ep->timestamp = ts;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		while (ep->credit && !list_empty(&ep->urb_list)) {
			urbl = list_first_entry(&ep->urb_list,
				struct oz_urb_link, link);
			urb = urbl->urb;
			if ((ep->credit + 1) < urb->number_of_packets)
				break;
			ep->credit -= urb->number_of_packets;
<<<<<<< HEAD
<<<<<<< HEAD
			oz_event_log(OZ_EVT_EP_CREDIT, ep->ep_num, 0, NULL,
				     ep->credit);
=======
			if (ep->credit < 0)
				ep->credit = 0;
>>>>>>> v3.18
=======
			if (ep->credit < 0)
				ep->credit = 0;
>>>>>>> v3.18
			list_move_tail(&urbl->link, &xfr_list);
		}
	}
	spin_unlock_bh(&ozhcd->hcd_lock);
	/* Send to PD and complete URBs.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	list_for_each_safe(e, n, &xfr_list) {
		unsigned long t;
		urbl = container_of(e, struct oz_urb_link, link);
		urb = urbl->urb;
		t = urbl->submit_jiffies;
		list_del_init(e);
=======
	list_for_each_entry_safe(urbl, n, &xfr_list, link) {
		urb = urbl->urb;
		list_del_init(&urbl->link);
>>>>>>> v3.18
=======
	list_for_each_entry_safe(urbl, n, &xfr_list, link) {
		urb = urbl->urb;
		list_del_init(&urbl->link);
>>>>>>> v3.18
		urb->error_count = 0;
		urb->start_frame = oz_usb_get_frame_number();
		oz_usb_send_isoc(port->hpd, urbl->ep_num, urb);
		oz_free_urb_link(urbl);
<<<<<<< HEAD
<<<<<<< HEAD
		oz_complete_urb(port->ozhcd->hcd, urb, 0, t);
=======
		oz_complete_urb(port->ozhcd->hcd, urb, 0);
>>>>>>> v3.18
=======
		oz_complete_urb(port->ozhcd->hcd, urb, 0);
>>>>>>> v3.18
	}
	/* Check the IN isoc endpoints to see if any URBs can be completed.
	 */
	spin_lock_bh(&ozhcd->hcd_lock);
<<<<<<< HEAD
<<<<<<< HEAD
	list_for_each(e, &port->isoc_in_ep) {
		struct oz_endpoint *ep = ep_from_link(e);
=======
	list_for_each_entry(ep, &port->isoc_in_ep, link) {
>>>>>>> v3.18
=======
	list_for_each_entry(ep, &port->isoc_in_ep, link) {
>>>>>>> v3.18
		if (ep->flags & OZ_F_EP_BUFFERING) {
			if (ep->buffered_units >= OZ_IN_BUFFERING_UNITS) {
				ep->flags &= ~OZ_F_EP_BUFFERING;
				ep->credit = 0;
<<<<<<< HEAD
<<<<<<< HEAD
				oz_event_log(OZ_EVT_EP_CREDIT,
					ep->ep_num | USB_DIR_IN,
					0, NULL, ep->credit);
				ep->last_jiffies = now;
				ep->start_frame = 0;
				oz_event_log(OZ_EVT_EP_BUFFERING,
					ep->ep_num | USB_DIR_IN, 0, NULL, 0);
			}
			continue;
		}
		ep->credit += jiffies_to_msecs(now - ep->last_jiffies);
		oz_event_log(OZ_EVT_EP_CREDIT, ep->ep_num | USB_DIR_IN,
			0, NULL, ep->credit);
		ep->last_jiffies = now;
		while (!list_empty(&ep->urb_list)) {
			struct oz_urb_link *urbl =
				list_first_entry(&ep->urb_list,
					struct oz_urb_link, link);
=======
=======
>>>>>>> v3.18
				ep->timestamp = ts;
				ep->start_frame = 0;
			}
			continue;
		}
		delta = timespec_sub(ts, ep->timestamp);
		ep->credit += div_u64(timespec_to_ns(&delta), NSEC_PER_MSEC);
		ep->timestamp = ts;
		list_for_each_entry_safe(urbl, n, &ep->urb_list, link) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			struct urb *urb = urbl->urb;
			int len = 0;
			int copy_len;
			int i;
<<<<<<< HEAD
<<<<<<< HEAD
			if ((ep->credit + 1) < urb->number_of_packets)
=======

			if (ep->credit  < urb->number_of_packets)
>>>>>>> v3.18
=======

			if (ep->credit  < urb->number_of_packets)
>>>>>>> v3.18
				break;
			if (ep->buffered_units < urb->number_of_packets)
				break;
			urb->actual_length = 0;
			for (i = 0; i < urb->number_of_packets; i++) {
				len = ep->buffer[ep->out_ix];
				if (++ep->out_ix == ep->buffer_size)
					ep->out_ix = 0;
				copy_len = ep->buffer_size - ep->out_ix;
				if (copy_len > len)
					copy_len = len;
				memcpy(urb->transfer_buffer,
					&ep->buffer[ep->out_ix], copy_len);
				if (copy_len < len) {
					memcpy(urb->transfer_buffer+copy_len,
						ep->buffer, len-copy_len);
					ep->out_ix = len-copy_len;
				} else
					ep->out_ix += copy_len;
				if (ep->out_ix == ep->buffer_size)
					ep->out_ix = 0;
				urb->iso_frame_desc[i].offset =
					urb->actual_length;
				urb->actual_length += len;
				urb->iso_frame_desc[i].actual_length = len;
				urb->iso_frame_desc[i].status = 0;
			}
			ep->buffered_units -= urb->number_of_packets;
			urb->error_count = 0;
			urb->start_frame = ep->start_frame;
			ep->start_frame += urb->number_of_packets;
			list_move_tail(&urbl->link, &xfr_list);
			ep->credit -= urb->number_of_packets;
<<<<<<< HEAD
<<<<<<< HEAD
			oz_event_log(OZ_EVT_EP_CREDIT, ep->ep_num | USB_DIR_IN,
				0, NULL, ep->credit);
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
		}
	}
	if (!list_empty(&port->isoc_out_ep) || !list_empty(&port->isoc_in_ep))
		rc = 1;
	spin_unlock_bh(&ozhcd->hcd_lock);
	/* Complete the filled URBs.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	list_for_each_safe(e, n, &xfr_list) {
		urbl = container_of(e, struct oz_urb_link, link);
		urb = urbl->urb;
		list_del_init(e);
		oz_free_urb_link(urbl);
		oz_complete_urb(port->ozhcd->hcd, urb, 0, 0);
=======
=======
>>>>>>> v3.18
	list_for_each_entry_safe(urbl, n, &xfr_list, link) {
		urb = urbl->urb;
		list_del_init(&urbl->link);
		oz_free_urb_link(urbl);
		oz_complete_urb(port->ozhcd->hcd, urb, 0);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}
	/* Check if there are any ep0 requests that have timed out.
	 * If so resent to PD.
	 */
	ep = port->out_ep[0];
	if (ep) {
<<<<<<< HEAD
<<<<<<< HEAD
		struct list_head *e;
		struct list_head *n;
		spin_lock_bh(&ozhcd->hcd_lock);
		list_for_each_safe(e, n, &ep->urb_list) {
			urbl = container_of(e, struct oz_urb_link, link);
			if (time_after(now, urbl->submit_jiffies+HZ/2)) {
				oz_trace("%ld: Request 0x%p timeout\n",
						now, urbl->urb);
				urbl->submit_jiffies = now;
				list_move_tail(e, &xfr_list);
=======
=======
>>>>>>> v3.18
		spin_lock_bh(&ozhcd->hcd_lock);
		list_for_each_entry_safe(urbl, n, &ep->urb_list, link) {
			if (urbl->submit_counter > EP0_TIMEOUT_COUNTER) {
				oz_dbg(ON, "Request 0x%p timeout\n", urbl->urb);
				list_move_tail(&urbl->link, &xfr_list);
				urbl->submit_counter = 0;
			} else {
				urbl->submit_counter++;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			}
		}
		if (!list_empty(&ep->urb_list))
			rc = 1;
		spin_unlock_bh(&ozhcd->hcd_lock);
<<<<<<< HEAD
<<<<<<< HEAD
		e = xfr_list.next;
		while (e != &xfr_list) {
			urbl = container_of(e, struct oz_urb_link, link);
			e = e->next;
			oz_trace("Resending request to PD.\n");
=======
		list_for_each_entry_safe(urbl, n, &xfr_list, link) {
			oz_dbg(ON, "Resending request to PD\n");
>>>>>>> v3.18
=======
		list_for_each_entry_safe(urbl, n, &xfr_list, link) {
			oz_dbg(ON, "Resending request to PD\n");
>>>>>>> v3.18
			oz_process_ep0_urb(ozhcd, urbl->urb, GFP_ATOMIC);
			oz_free_urb_link(urbl);
		}
	}
	return rc;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: softirq
 */
static int oz_build_endpoints_for_interface(struct usb_hcd *hcd,
		struct oz_port *port,
		struct usb_host_interface *intf, gfp_t mem_flags)
{
	struct oz_hcd *ozhcd = port->ozhcd;
	int i;
	int if_ix = intf->desc.bInterfaceNumber;
	int request_heartbeat = 0;
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("interface[%d] = %p\n", if_ix, intf);
=======
=======
>>>>>>> v3.18

	oz_dbg(ON, "interface[%d] = %p\n", if_ix, intf);
	if (if_ix >= port->num_iface || port->iface == NULL)
		return -ENOMEM;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	for (i = 0; i < intf->desc.bNumEndpoints; i++) {
		struct usb_host_endpoint *hep = &intf->endpoint[i];
		u8 ep_addr = hep->desc.bEndpointAddress;
		u8 ep_num = ep_addr & USB_ENDPOINT_NUMBER_MASK;
		struct oz_endpoint *ep;
		int buffer_size = 0;

<<<<<<< HEAD
<<<<<<< HEAD
		oz_trace("%d bEndpointAddress = %x\n", i, ep_addr);
=======
		oz_dbg(ON, "%d bEndpointAddress = %x\n", i, ep_addr);
>>>>>>> v3.18
=======
		oz_dbg(ON, "%d bEndpointAddress = %x\n", i, ep_addr);
>>>>>>> v3.18
		if (ep_addr & USB_ENDPOINT_DIR_MASK) {
			switch (hep->desc.bmAttributes &
						USB_ENDPOINT_XFERTYPE_MASK) {
			case USB_ENDPOINT_XFER_ISOC:
<<<<<<< HEAD
<<<<<<< HEAD
				buffer_size = 24*1024;
				break;
			case USB_ENDPOINT_XFER_INT:
				buffer_size = 128;
=======
=======
>>>>>>> v3.18
				buffer_size = OZ_EP_BUFFER_SIZE_ISOC;
				break;
			case USB_ENDPOINT_XFER_INT:
				buffer_size = OZ_EP_BUFFER_SIZE_INT;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
				break;
			}
		}

<<<<<<< HEAD
<<<<<<< HEAD
		ep = oz_ep_alloc(mem_flags, buffer_size);
=======
		ep = oz_ep_alloc(buffer_size, mem_flags);
>>>>>>> v3.18
=======
		ep = oz_ep_alloc(buffer_size, mem_flags);
>>>>>>> v3.18
		if (!ep) {
			oz_clean_endpoints_for_interface(hcd, port, if_ix);
			return -ENOMEM;
		}
		ep->attrib = hep->desc.bmAttributes;
		ep->ep_num = ep_num;
		if ((ep->attrib & USB_ENDPOINT_XFERTYPE_MASK)
			== USB_ENDPOINT_XFER_ISOC) {
<<<<<<< HEAD
<<<<<<< HEAD
			oz_trace("wMaxPacketSize = %d\n",
				hep->desc.wMaxPacketSize);
			ep->credit_ceiling = 200;
			if (ep_addr & USB_ENDPOINT_DIR_MASK) {
				ep->flags |= OZ_F_EP_BUFFERING;
				oz_event_log(OZ_EVT_EP_BUFFERING,
					ep->ep_num | USB_DIR_IN, 1, NULL, 0);
=======
=======
>>>>>>> v3.18
			oz_dbg(ON, "wMaxPacketSize = %d\n",
			       usb_endpoint_maxp(&hep->desc));
			ep->credit_ceiling = 200;
			if (ep_addr & USB_ENDPOINT_DIR_MASK) {
				ep->flags |= OZ_F_EP_BUFFERING;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			} else {
				ep->flags |= OZ_F_EP_HAVE_STREAM;
				if (oz_usb_stream_create(port->hpd, ep_num))
					ep->flags &= ~OZ_F_EP_HAVE_STREAM;
			}
		}
		spin_lock_bh(&ozhcd->hcd_lock);
		if (ep_addr & USB_ENDPOINT_DIR_MASK) {
			port->in_ep[ep_num] = ep;
			port->iface[if_ix].ep_mask |=
				(1<<(ep_num+OZ_NB_ENDPOINTS));
			if ((ep->attrib & USB_ENDPOINT_XFERTYPE_MASK)
				 == USB_ENDPOINT_XFER_ISOC) {
				list_add_tail(&ep->link, &port->isoc_in_ep);
				request_heartbeat = 1;
			}
		} else {
			port->out_ep[ep_num] = ep;
			port->iface[if_ix].ep_mask |= (1<<ep_num);
			if ((ep->attrib & USB_ENDPOINT_XFERTYPE_MASK)
				== USB_ENDPOINT_XFER_ISOC) {
				list_add_tail(&ep->link, &port->isoc_out_ep);
				request_heartbeat = 1;
			}
		}
		spin_unlock_bh(&ozhcd->hcd_lock);
		if (request_heartbeat && port->hpd)
			oz_usb_request_heartbeat(port->hpd);
	}
	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: softirq
 */
static void oz_clean_endpoints_for_interface(struct usb_hcd *hcd,
			struct oz_port *port, int if_ix)
{
	struct oz_hcd *ozhcd = port->ozhcd;
	unsigned mask;
	int i;
<<<<<<< HEAD
<<<<<<< HEAD
	struct list_head ep_list;

	oz_trace("Deleting endpoints for interface %d\n", if_ix);
	if (if_ix >= port->num_iface)
		return;
	INIT_LIST_HEAD(&ep_list);
=======
=======
>>>>>>> v3.18
	LIST_HEAD(ep_list);
	struct oz_endpoint *ep, *n;

	oz_dbg(ON, "Deleting endpoints for interface %d\n", if_ix);
	if (if_ix >= port->num_iface)
		return;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	spin_lock_bh(&ozhcd->hcd_lock);
	mask = port->iface[if_ix].ep_mask;
	port->iface[if_ix].ep_mask = 0;
	for (i = 0; i < OZ_NB_ENDPOINTS; i++) {
		struct list_head *e;
		/* Gather OUT endpoints.
		 */
		if ((mask & (1<<i)) && port->out_ep[i]) {
			e = &port->out_ep[i]->link;
			port->out_ep[i] = NULL;
			/* Remove from isoc list if present.
			 */
			list_move_tail(e, &ep_list);
		}
		/* Gather IN endpoints.
		 */
		if ((mask & (1<<(i+OZ_NB_ENDPOINTS))) && port->in_ep[i]) {
			e = &port->in_ep[i]->link;
			port->in_ep[i] = NULL;
			list_move_tail(e, &ep_list);
		}
	}
	spin_unlock_bh(&ozhcd->hcd_lock);
<<<<<<< HEAD
<<<<<<< HEAD
	while (!list_empty(&ep_list)) {
		struct oz_endpoint *ep =
			list_first_entry(&ep_list, struct oz_endpoint, link);
=======
	list_for_each_entry_safe(ep, n, &ep_list, link) {
>>>>>>> v3.18
=======
	list_for_each_entry_safe(ep, n, &ep_list, link) {
>>>>>>> v3.18
		list_del_init(&ep->link);
		oz_ep_free(port, ep);
	}
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: softirq
 */
static int oz_build_endpoints_for_config(struct usb_hcd *hcd,
		struct oz_port *port, struct usb_host_config *config,
		gfp_t mem_flags)
{
	struct oz_hcd *ozhcd = port->ozhcd;
	int i;
	int num_iface = config->desc.bNumInterfaces;
<<<<<<< HEAD
<<<<<<< HEAD
	if (num_iface) {
		struct oz_interface *iface;

		iface = kmalloc(num_iface*sizeof(struct oz_interface),
				mem_flags | __GFP_ZERO);
=======
=======
>>>>>>> v3.18

	if (num_iface) {
		struct oz_interface *iface;

		iface = kmalloc_array(num_iface, sizeof(struct oz_interface),
					mem_flags | __GFP_ZERO);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		if (!iface)
			return -ENOMEM;
		spin_lock_bh(&ozhcd->hcd_lock);
		port->iface = iface;
		port->num_iface = num_iface;
		spin_unlock_bh(&ozhcd->hcd_lock);
	}
	for (i = 0; i < num_iface; i++) {
		struct usb_host_interface *intf =
			&config->intf_cache[i]->altsetting[0];
		if (oz_build_endpoints_for_interface(hcd, port, intf,
			mem_flags))
			goto fail;
	}
	return 0;
fail:
	oz_clean_endpoints_for_config(hcd, port);
	return -1;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: softirq
 */
static void oz_clean_endpoints_for_config(struct usb_hcd *hcd,
			struct oz_port *port)
{
	struct oz_hcd *ozhcd = port->ozhcd;
	int i;
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("Deleting endpoints for configuration.\n");
=======

	oz_dbg(ON, "Deleting endpoints for configuration\n");
>>>>>>> v3.18
=======

	oz_dbg(ON, "Deleting endpoints for configuration\n");
>>>>>>> v3.18
	for (i = 0; i < port->num_iface; i++)
		oz_clean_endpoints_for_interface(hcd, port, i);
	spin_lock_bh(&ozhcd->hcd_lock);
	if (port->iface) {
<<<<<<< HEAD
<<<<<<< HEAD
		oz_trace("Freeing interfaces object.\n");
=======
		oz_dbg(ON, "Freeing interfaces object\n");
>>>>>>> v3.18
=======
		oz_dbg(ON, "Freeing interfaces object\n");
>>>>>>> v3.18
		kfree(port->iface);
		port->iface = NULL;
	}
	port->num_iface = 0;
	spin_unlock_bh(&ozhcd->hcd_lock);
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: tasklet
 */
static void *oz_claim_hpd(struct oz_port *port)
{
<<<<<<< HEAD
<<<<<<< HEAD
	void *hpd = NULL;
	struct oz_hcd *ozhcd = port->ozhcd;
=======
	void *hpd;
	struct oz_hcd *ozhcd = port->ozhcd;

>>>>>>> v3.18
=======
	void *hpd;
	struct oz_hcd *ozhcd = port->ozhcd;

>>>>>>> v3.18
	spin_lock_bh(&ozhcd->hcd_lock);
	hpd = port->hpd;
	if (hpd)
		oz_usb_get(hpd);
	spin_unlock_bh(&ozhcd->hcd_lock);
	return hpd;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: tasklet
 */
static void oz_process_ep0_urb(struct oz_hcd *ozhcd, struct urb *urb,
		gfp_t mem_flags)
{
	struct usb_ctrlrequest *setup;
	unsigned windex;
	unsigned wvalue;
	unsigned wlength;
<<<<<<< HEAD
<<<<<<< HEAD
	void *hpd = NULL;
=======
	void *hpd;
>>>>>>> v3.18
=======
	void *hpd;
>>>>>>> v3.18
	u8 req_id;
	int rc = 0;
	unsigned complete = 0;

	int port_ix = -1;
	struct oz_port *port = NULL;

<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace2(OZ_TRACE_URB, "%lu: oz_process_ep0_urb(%p)\n", jiffies, urb);
=======
	oz_dbg(URB, "[%s]:(%p)\n", __func__, urb);
>>>>>>> v3.18
=======
	oz_dbg(URB, "[%s]:(%p)\n", __func__, urb);
>>>>>>> v3.18
	port_ix = oz_get_port_from_addr(ozhcd, urb->dev->devnum);
	if (port_ix < 0) {
		rc = -EPIPE;
		goto out;
	}
	port =  &ozhcd->ports[port_ix];
	if (((port->flags & OZ_PORT_F_PRESENT) == 0)
		|| (port->flags & OZ_PORT_F_DYING)) {
<<<<<<< HEAD
<<<<<<< HEAD
		oz_trace("Refusing URB port_ix = %d devnum = %d\n",
			port_ix, urb->dev->devnum);
=======
		oz_dbg(ON, "Refusing URB port_ix = %d devnum = %d\n",
		       port_ix, urb->dev->devnum);
>>>>>>> v3.18
=======
		oz_dbg(ON, "Refusing URB port_ix = %d devnum = %d\n",
		       port_ix, urb->dev->devnum);
>>>>>>> v3.18
		rc = -EPIPE;
		goto out;
	}
	/* Store port in private context data.
	 */
	urb->hcpriv = port;
	setup = (struct usb_ctrlrequest *)urb->setup_packet;
	windex = le16_to_cpu(setup->wIndex);
	wvalue = le16_to_cpu(setup->wValue);
	wlength = le16_to_cpu(setup->wLength);
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace2(OZ_TRACE_CTRL_DETAIL, "bRequestType = %x\n",
		setup->bRequestType);
	oz_trace2(OZ_TRACE_CTRL_DETAIL, "bRequest = %x\n", setup->bRequest);
	oz_trace2(OZ_TRACE_CTRL_DETAIL, "wValue = %x\n", wvalue);
	oz_trace2(OZ_TRACE_CTRL_DETAIL, "wIndex = %x\n", windex);
	oz_trace2(OZ_TRACE_CTRL_DETAIL, "wLength = %x\n", wlength);
=======
=======
>>>>>>> v3.18
	oz_dbg(CTRL_DETAIL, "bRequestType = %x\n", setup->bRequestType);
	oz_dbg(CTRL_DETAIL, "bRequest = %x\n", setup->bRequest);
	oz_dbg(CTRL_DETAIL, "wValue = %x\n", wvalue);
	oz_dbg(CTRL_DETAIL, "wIndex = %x\n", windex);
	oz_dbg(CTRL_DETAIL, "wLength = %x\n", wlength);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	req_id = port->next_req_id++;
	hpd = oz_claim_hpd(port);
	if (hpd == NULL) {
<<<<<<< HEAD
<<<<<<< HEAD
		oz_trace("Cannot claim port\n");
=======
		oz_dbg(ON, "Cannot claim port\n");
>>>>>>> v3.18
=======
		oz_dbg(ON, "Cannot claim port\n");
>>>>>>> v3.18
		rc = -EPIPE;
		goto out;
	}

	if ((setup->bRequestType & USB_TYPE_MASK) == USB_TYPE_STANDARD) {
		/* Standard requests
		 */
		switch (setup->bRequest) {
		case USB_REQ_GET_DESCRIPTOR:
<<<<<<< HEAD
<<<<<<< HEAD
			oz_trace("USB_REQ_GET_DESCRIPTOR - req\n");
			break;
		case USB_REQ_SET_ADDRESS:
			oz_event_log(OZ_EVT_CTRL_LOCAL, setup->bRequest,
				0, NULL, setup->bRequestType);
			oz_trace("USB_REQ_SET_ADDRESS - req\n");
			oz_trace("Port %d address is 0x%x\n", ozhcd->conn_port,
				(u8)le16_to_cpu(setup->wValue));
=======
=======
>>>>>>> v3.18
			oz_dbg(ON, "USB_REQ_GET_DESCRIPTOR - req\n");
			break;
		case USB_REQ_SET_ADDRESS:
			oz_dbg(ON, "USB_REQ_SET_ADDRESS - req\n");
			oz_dbg(ON, "Port %d address is 0x%x\n",
			       ozhcd->conn_port,
			       (u8)le16_to_cpu(setup->wValue));
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			spin_lock_bh(&ozhcd->hcd_lock);
			if (ozhcd->conn_port >= 0) {
				ozhcd->ports[ozhcd->conn_port].bus_addr =
					(u8)le16_to_cpu(setup->wValue);
<<<<<<< HEAD
<<<<<<< HEAD
				oz_trace("Clearing conn_port\n");
=======
				oz_dbg(ON, "Clearing conn_port\n");
>>>>>>> v3.18
=======
				oz_dbg(ON, "Clearing conn_port\n");
>>>>>>> v3.18
				ozhcd->conn_port = -1;
			}
			spin_unlock_bh(&ozhcd->hcd_lock);
			complete = 1;
			break;
		case USB_REQ_SET_CONFIGURATION:
<<<<<<< HEAD
<<<<<<< HEAD
			oz_trace("USB_REQ_SET_CONFIGURATION - req\n");
=======
			oz_dbg(ON, "USB_REQ_SET_CONFIGURATION - req\n");
>>>>>>> v3.18
=======
			oz_dbg(ON, "USB_REQ_SET_CONFIGURATION - req\n");
>>>>>>> v3.18
			break;
		case USB_REQ_GET_CONFIGURATION:
			/* We short circuit this case and reply directly since
			 * we have the selected configuration number cached.
			 */
<<<<<<< HEAD
<<<<<<< HEAD
			oz_event_log(OZ_EVT_CTRL_LOCAL, setup->bRequest, 0,
				     NULL, setup->bRequestType);
			oz_trace("USB_REQ_GET_CONFIGURATION - reply now\n");
=======
			oz_dbg(ON, "USB_REQ_GET_CONFIGURATION - reply now\n");
>>>>>>> v3.18
=======
			oz_dbg(ON, "USB_REQ_GET_CONFIGURATION - reply now\n");
>>>>>>> v3.18
			if (urb->transfer_buffer_length >= 1) {
				urb->actual_length = 1;
				*((u8 *)urb->transfer_buffer) =
					port->config_num;
				complete = 1;
			} else {
				rc = -EPIPE;
			}
			break;
		case USB_REQ_GET_INTERFACE:
			/* We short circuit this case and reply directly since
			 * we have the selected interface alternative cached.
			 */
<<<<<<< HEAD
<<<<<<< HEAD
			oz_event_log(OZ_EVT_CTRL_LOCAL, setup->bRequest, 0,
				     NULL, setup->bRequestType);
			oz_trace("USB_REQ_GET_INTERFACE - reply now\n");
=======
			oz_dbg(ON, "USB_REQ_GET_INTERFACE - reply now\n");
>>>>>>> v3.18
=======
			oz_dbg(ON, "USB_REQ_GET_INTERFACE - reply now\n");
>>>>>>> v3.18
			if (urb->transfer_buffer_length >= 1) {
				urb->actual_length = 1;
				*((u8 *)urb->transfer_buffer) =
					port->iface[(u8)windex].alt;
<<<<<<< HEAD
<<<<<<< HEAD
				oz_trace("interface = %d alt = %d\n",
					windex, port->iface[(u8)windex].alt);
=======
				oz_dbg(ON, "interface = %d alt = %d\n",
				       windex, port->iface[(u8)windex].alt);
>>>>>>> v3.18
=======
				oz_dbg(ON, "interface = %d alt = %d\n",
				       windex, port->iface[(u8)windex].alt);
>>>>>>> v3.18
				complete = 1;
			} else {
				rc = -EPIPE;
			}
			break;
		case USB_REQ_SET_INTERFACE:
<<<<<<< HEAD
<<<<<<< HEAD
			oz_trace("USB_REQ_SET_INTERFACE - req\n");
=======
			oz_dbg(ON, "USB_REQ_SET_INTERFACE - req\n");
>>>>>>> v3.18
=======
			oz_dbg(ON, "USB_REQ_SET_INTERFACE - req\n");
>>>>>>> v3.18
			break;
		}
	}
	if (!rc && !complete) {
		int data_len = 0;
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
		if ((setup->bRequestType & USB_DIR_IN) == 0)
			data_len = wlength;
		urb->actual_length = data_len;
		if (oz_usb_control_req(port->hpd, req_id, setup,
				urb->transfer_buffer, data_len)) {
			rc = -ENOMEM;
		} else {
			/* Note: we are queuing the request after we have
			 * submitted it to be transmitted. If the request were
			 * to complete before we queued it then it would not
			 * be found in the queue. It seems impossible for
			 * this to happen but if it did the request would
			 * be resubmitted so the problem would hopefully
			 * resolve itself. Putting the request into the
			 * queue before it has been sent is worse since the
			 * urb could be cancelled while we are using it
			 * to build the request.
			 */
			if (oz_enqueue_ep_urb(port, 0, 0, urb, req_id))
				rc = -ENOMEM;
		}
	}
	oz_usb_put(hpd);
out:
	if (rc || complete) {
<<<<<<< HEAD
<<<<<<< HEAD
		oz_trace("Completing request locally\n");
		oz_complete_urb(ozhcd->hcd, urb, rc, 0);
=======
		oz_dbg(ON, "Completing request locally\n");
		oz_complete_urb(ozhcd->hcd, urb, rc);
>>>>>>> v3.18
=======
		oz_dbg(ON, "Completing request locally\n");
		oz_complete_urb(ozhcd->hcd, urb, rc);
>>>>>>> v3.18
	} else {
		oz_usb_request_heartbeat(port->hpd);
	}
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: tasklet
 */
static int oz_urb_process(struct oz_hcd *ozhcd, struct urb *urb)
{
	int rc = 0;
	struct oz_port *port = urb->hcpriv;
	u8 ep_addr;
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
	/* When we are paranoid we keep a list of urbs which we check against
	 * before handing one back. This is just for debugging during
	 * development and should be turned off in the released driver.
	 */
	oz_remember_urb(urb);
	/* Check buffer is valid.
	 */
	if (!urb->transfer_buffer && urb->transfer_buffer_length)
		return -EINVAL;
	/* Check if there is a device at the port - refuse if not.
	 */
	if ((port->flags & OZ_PORT_F_PRESENT) == 0)
		return -EPIPE;
	ep_addr = usb_pipeendpoint(urb->pipe);
	if (ep_addr) {
		/* If the request is not for EP0 then queue it.
		 */
		if (oz_enqueue_ep_urb(port, ep_addr, usb_pipein(urb->pipe),
			urb, 0))
			rc = -EPIPE;
	} else {
		oz_process_ep0_urb(ozhcd, urb, GFP_ATOMIC);
	}
	return rc;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: tasklet
 */
static void oz_urb_process_tasklet(unsigned long unused)
{
	unsigned long irq_state;
	struct urb *urb;
	struct oz_hcd *ozhcd = oz_hcd_claim();
<<<<<<< HEAD
<<<<<<< HEAD
	int rc = 0;
=======
	struct oz_urb_link *urbl, *n;
	int rc = 0;

>>>>>>> v3.18
=======
	struct oz_urb_link *urbl, *n;
	int rc = 0;

>>>>>>> v3.18
	if (ozhcd == NULL)
		return;
	/* This is called from a tasklet so is in softirq context but the urb
	 * list is filled from any context so we need to lock
	 * appropriately while removing urbs.
	 */
	spin_lock_irqsave(&g_tasklet_lock, irq_state);
<<<<<<< HEAD
<<<<<<< HEAD
	while (!list_empty(&ozhcd->urb_pending_list)) {
		struct oz_urb_link *urbl =
			list_first_entry(&ozhcd->urb_pending_list,
				struct oz_urb_link, link);
=======
	list_for_each_entry_safe(urbl, n, &ozhcd->urb_pending_list, link) {
>>>>>>> v3.18
=======
	list_for_each_entry_safe(urbl, n, &ozhcd->urb_pending_list, link) {
>>>>>>> v3.18
		list_del_init(&urbl->link);
		spin_unlock_irqrestore(&g_tasklet_lock, irq_state);
		urb = urbl->urb;
		oz_free_urb_link(urbl);
		rc = oz_urb_process(ozhcd, urb);
		if (rc)
<<<<<<< HEAD
<<<<<<< HEAD
			oz_complete_urb(ozhcd->hcd, urb, rc, 0);
=======
			oz_complete_urb(ozhcd->hcd, urb, rc);
>>>>>>> v3.18
=======
			oz_complete_urb(ozhcd->hcd, urb, rc);
>>>>>>> v3.18
		spin_lock_irqsave(&g_tasklet_lock, irq_state);
	}
	spin_unlock_irqrestore(&g_tasklet_lock, irq_state);
	oz_hcd_put(ozhcd);
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * This function searches for the urb in any of the lists it could be in.
 * If it is found it is removed from the list and completed. If the urb is
 * being processed then it won't be in a list so won't be found. However, the
 * call to usb_hcd_check_unlink_urb() will set the value of the unlinked field
 * to a non-zero value. When an attempt is made to put the urb back in a list
 * the unlinked field will be checked and the urb will then be completed.
 * Context: tasklet
 */
static void oz_urb_cancel(struct oz_port *port, u8 ep_num, struct urb *urb)
{
	struct oz_urb_link *urbl = NULL;
	struct list_head *e;
	struct oz_hcd *ozhcd;
	unsigned long irq_state;
	u8 ix;
<<<<<<< HEAD
<<<<<<< HEAD
	if (port == NULL) {
		oz_trace("ERRORERROR: oz_urb_cancel(%p) port is null\n", urb);
=======

	if (port == NULL) {
		oz_dbg(ON, "%s: ERROR: (%p) port is null\n", __func__, urb);
>>>>>>> v3.18
=======

	if (port == NULL) {
		oz_dbg(ON, "%s: ERROR: (%p) port is null\n", __func__, urb);
>>>>>>> v3.18
		return;
	}
	ozhcd = port->ozhcd;
	if (ozhcd == NULL) {
<<<<<<< HEAD
<<<<<<< HEAD
		oz_trace("ERRORERROR: oz_urb_cancel(%p) ozhcd is null\n", urb);
=======
		oz_dbg(ON, "%s; ERROR: (%p) ozhcd is null\n", __func__, urb);
>>>>>>> v3.18
=======
		oz_dbg(ON, "%s; ERROR: (%p) ozhcd is null\n", __func__, urb);
>>>>>>> v3.18
		return;
	}

	/* Look in the tasklet queue.
	 */
	spin_lock_irqsave(&g_tasklet_lock, irq_state);
	list_for_each(e, &ozhcd->urb_cancel_list) {
<<<<<<< HEAD
<<<<<<< HEAD
		urbl = container_of(e, struct oz_urb_link, link);
=======
		urbl = list_entry(e, struct oz_urb_link, link);
>>>>>>> v3.18
=======
		urbl = list_entry(e, struct oz_urb_link, link);
>>>>>>> v3.18
		if (urb == urbl->urb) {
			list_del_init(e);
			spin_unlock_irqrestore(&g_tasklet_lock, irq_state);
			goto out2;
		}
	}
	spin_unlock_irqrestore(&g_tasklet_lock, irq_state);
	urbl = NULL;

	/* Look in the orphanage.
	 */
	spin_lock_irqsave(&ozhcd->hcd_lock, irq_state);
	list_for_each(e, &ozhcd->orphanage) {
<<<<<<< HEAD
<<<<<<< HEAD
		urbl = container_of(e, struct oz_urb_link, link);
		if (urbl->urb == urb) {
			list_del(e);
			oz_trace("Found urb in orphanage\n");
=======
=======
>>>>>>> v3.18
		urbl = list_entry(e, struct oz_urb_link, link);
		if (urbl->urb == urb) {
			list_del(e);
			oz_dbg(ON, "Found urb in orphanage\n");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			goto out;
		}
	}
	ix = (ep_num & 0xf);
	urbl = NULL;
	if ((ep_num & USB_DIR_IN) && ix)
		urbl = oz_remove_urb(port->in_ep[ix], urb);
	else
		urbl = oz_remove_urb(port->out_ep[ix], urb);
out:
	spin_unlock_irqrestore(&ozhcd->hcd_lock, irq_state);
out2:
	if (urbl) {
		urb->actual_length = 0;
		oz_free_urb_link(urbl);
<<<<<<< HEAD
<<<<<<< HEAD
		oz_complete_urb(ozhcd->hcd, urb, -EPIPE, 0);
	}
}
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
		oz_complete_urb(ozhcd->hcd, urb, -EPIPE);
	}
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: tasklet
 */
static void oz_urb_cancel_tasklet(unsigned long unused)
{
	unsigned long irq_state;
	struct urb *urb;
<<<<<<< HEAD
<<<<<<< HEAD
	struct oz_hcd *ozhcd = oz_hcd_claim();
	if (ozhcd == NULL)
		return;
	spin_lock_irqsave(&g_tasklet_lock, irq_state);
	while (!list_empty(&ozhcd->urb_cancel_list)) {
		struct oz_urb_link *urbl =
			list_first_entry(&ozhcd->urb_cancel_list,
				struct oz_urb_link, link);
=======
=======
>>>>>>> v3.18
	struct oz_urb_link *urbl, *n;
	struct oz_hcd *ozhcd = oz_hcd_claim();

	if (ozhcd == NULL)
		return;
	spin_lock_irqsave(&g_tasklet_lock, irq_state);
	list_for_each_entry_safe(urbl, n, &ozhcd->urb_cancel_list, link) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		list_del_init(&urbl->link);
		spin_unlock_irqrestore(&g_tasklet_lock, irq_state);
		urb = urbl->urb;
		if (urb->unlinked)
			oz_urb_cancel(urbl->port, urbl->ep_num, urb);
		oz_free_urb_link(urbl);
		spin_lock_irqsave(&g_tasklet_lock, irq_state);
	}
	spin_unlock_irqrestore(&g_tasklet_lock, irq_state);
	oz_hcd_put(ozhcd);
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: unknown
 */
static void oz_hcd_clear_orphanage(struct oz_hcd *ozhcd, int status)
{
	if (ozhcd) {
<<<<<<< HEAD
<<<<<<< HEAD
		struct oz_urb_link *urbl;
		while (!list_empty(&ozhcd->orphanage)) {
			urbl = list_first_entry(&ozhcd->orphanage,
				struct oz_urb_link, link);
			list_del(&urbl->link);
			oz_complete_urb(ozhcd->hcd, urbl->urb, status, 0);
=======
=======
>>>>>>> v3.18
		struct oz_urb_link *urbl, *n;

		list_for_each_entry_safe(urbl, n, &ozhcd->orphanage, link) {
			list_del(&urbl->link);
			oz_complete_urb(ozhcd->hcd, urbl->urb, status);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			oz_free_urb_link(urbl);
		}
	}
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: unknown
 */
static int oz_hcd_start(struct usb_hcd *hcd)
{
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("oz_hcd_start()\n");
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	hcd->power_budget = 200;
	hcd->state = HC_STATE_RUNNING;
	hcd->uses_new_polling = 1;
	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: unknown
 */
static void oz_hcd_stop(struct usb_hcd *hcd)
{
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("oz_hcd_stop()\n");
}
/*------------------------------------------------------------------------------
=======
}

/*
>>>>>>> v3.18
=======
}

/*
>>>>>>> v3.18
 * Context: unknown
 */
static void oz_hcd_shutdown(struct usb_hcd *hcd)
{
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("oz_hcd_shutdown()\n");
}
/*------------------------------------------------------------------------------
 * Context: any
 */
#ifdef WANT_EVENT_TRACE
static u8 oz_get_irq_ctx(void)
{
	u8 irq_info = 0;
	if (in_interrupt())
		irq_info |= 1;
	if (in_irq())
		irq_info |= 2;
	return irq_info;
}
#endif /* WANT_EVENT_TRACE */
/*------------------------------------------------------------------------------
=======
}

/*
>>>>>>> v3.18
=======
}

/*
>>>>>>> v3.18
 * Called to queue an urb for the device.
 * This function should return a non-zero error code if it fails the urb but
 * should not call usb_hcd_giveback_urb().
 * Context: any
 */
static int oz_hcd_urb_enqueue(struct usb_hcd *hcd, struct urb *urb,
				gfp_t mem_flags)
{
	struct oz_hcd *ozhcd = oz_hcd_private(hcd);
<<<<<<< HEAD
<<<<<<< HEAD
	int rc = 0;
=======
	int rc;
>>>>>>> v3.18
=======
	int rc;
>>>>>>> v3.18
	int port_ix;
	struct oz_port *port;
	unsigned long irq_state;
	struct oz_urb_link *urbl;
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace2(OZ_TRACE_URB, "%lu: oz_hcd_urb_enqueue(%p)\n",
		jiffies, urb);
	oz_event_log(OZ_EVT_URB_SUBMIT, oz_get_irq_ctx(),
		(u16)urb->number_of_packets, urb, urb->pipe);
	if (unlikely(ozhcd == NULL)) {
		oz_trace2(OZ_TRACE_URB, "%lu: Refused urb(%p) not ozhcd.\n",
			jiffies, urb);
		return -EPIPE;
	}
	if (unlikely(hcd->state != HC_STATE_RUNNING)) {
		oz_trace2(OZ_TRACE_URB, "%lu: Refused urb(%p) not running.\n",
			jiffies, urb);
=======
=======
>>>>>>> v3.18

	oz_dbg(URB, "%s: (%p)\n",  __func__, urb);
	if (unlikely(ozhcd == NULL)) {
		oz_dbg(URB, "Refused urb(%p) not ozhcd\n", urb);
		return -EPIPE;
	}
	if (unlikely(hcd->state != HC_STATE_RUNNING)) {
		oz_dbg(URB, "Refused urb(%p) not running\n", urb);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return -EPIPE;
	}
	port_ix = oz_get_port_from_addr(ozhcd, urb->dev->devnum);
	if (port_ix < 0)
		return -EPIPE;
	port =  &ozhcd->ports[port_ix];
	if (port == NULL)
		return -EPIPE;
<<<<<<< HEAD
<<<<<<< HEAD
	if ((port->flags & OZ_PORT_F_PRESENT) == 0) {
		oz_trace("Refusing URB port_ix = %d devnum = %d\n",
			port_ix, urb->dev->devnum);
=======
=======
>>>>>>> v3.18
	if (!(port->flags & OZ_PORT_F_PRESENT) ||
				(port->flags & OZ_PORT_F_CHANGED)) {
		oz_dbg(ON, "Refusing URB port_ix = %d devnum = %d\n",
		       port_ix, urb->dev->devnum);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return -EPIPE;
	}
	urb->hcpriv = port;
	/* Put request in queue for processing by tasklet.
	 */
	urbl = oz_alloc_urb_link();
	if (unlikely(urbl == NULL))
		return -ENOMEM;
	urbl->urb = urb;
	spin_lock_irqsave(&g_tasklet_lock, irq_state);
	rc = usb_hcd_link_urb_to_ep(hcd, urb);
	if (unlikely(rc)) {
		spin_unlock_irqrestore(&g_tasklet_lock, irq_state);
		oz_free_urb_link(urbl);
		return rc;
	}
	list_add_tail(&urbl->link, &ozhcd->urb_pending_list);
	spin_unlock_irqrestore(&g_tasklet_lock, irq_state);
	tasklet_schedule(&g_urb_process_tasklet);
	atomic_inc(&g_pending_urbs);
	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: tasklet
 */
static struct oz_urb_link *oz_remove_urb(struct oz_endpoint *ep,
				struct urb *urb)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct oz_urb_link *urbl = NULL;
	struct list_head *e;
	if (unlikely(ep == NULL))
		return NULL;
	list_for_each(e, &ep->urb_list) {
		urbl = container_of(e, struct oz_urb_link, link);
		if (urbl->urb == urb) {
			list_del_init(e);
=======
=======
>>>>>>> v3.18
	struct oz_urb_link *urbl;

	if (unlikely(ep == NULL))
		return NULL;

	list_for_each_entry(urbl, &ep->urb_list, link) {
		if (urbl->urb == urb) {
			list_del_init(&urbl->link);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			if (usb_pipeisoc(urb->pipe)) {
				ep->credit -= urb->number_of_packets;
				if (ep->credit < 0)
					ep->credit = 0;
<<<<<<< HEAD
<<<<<<< HEAD
				oz_event_log(OZ_EVT_EP_CREDIT,
					usb_pipein(urb->pipe) ?
					(ep->ep_num | USB_DIR_IN) : ep->ep_num,
					0, NULL, ep->credit);
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
			}
			return urbl;
		}
	}
	return NULL;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Called to dequeue a previously submitted urb for the device.
 * Context: any
 */
static int oz_hcd_urb_dequeue(struct usb_hcd *hcd, struct urb *urb, int status)
{
	struct oz_hcd *ozhcd = oz_hcd_private(hcd);
<<<<<<< HEAD
<<<<<<< HEAD
	struct oz_urb_link *urbl = NULL;
	int rc;
	unsigned long irq_state;
	oz_trace2(OZ_TRACE_URB, "%lu: oz_hcd_urb_dequeue(%p)\n", jiffies, urb);
=======
=======
>>>>>>> v3.18
	struct oz_urb_link *urbl;
	int rc;
	unsigned long irq_state;

	oz_dbg(URB, "%s: (%p)\n",  __func__, urb);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	urbl = oz_alloc_urb_link();
	if (unlikely(urbl == NULL))
		return -ENOMEM;
	spin_lock_irqsave(&g_tasklet_lock, irq_state);
	/* The following function checks the urb is still in the queue
	 * maintained by the core and that the unlinked field is zero.
	 * If both are true the function sets the unlinked field and returns
	 * zero. Otherwise it returns an error.
	 */
	rc = usb_hcd_check_unlink_urb(hcd, urb, status);
	/* We have to check we haven't completed the urb or are about
	 * to complete it. When we do we set hcpriv to 0 so if this has
	 * already happened we don't put the urb in the cancel queue.
	 */
	if ((rc == 0) && urb->hcpriv) {
		urbl->urb = urb;
		urbl->port = (struct oz_port *)urb->hcpriv;
		urbl->ep_num = usb_pipeendpoint(urb->pipe);
		if (usb_pipein(urb->pipe))
			urbl->ep_num |= USB_DIR_IN;
		list_add_tail(&urbl->link, &ozhcd->urb_cancel_list);
		spin_unlock_irqrestore(&g_tasklet_lock, irq_state);
		tasklet_schedule(&g_urb_cancel_tasklet);
	} else {
		spin_unlock_irqrestore(&g_tasklet_lock, irq_state);
		oz_free_urb_link(urbl);
	}
	return rc;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: unknown
 */
static void oz_hcd_endpoint_disable(struct usb_hcd *hcd,
				struct usb_host_endpoint *ep)
{
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("oz_hcd_endpoint_disable\n");
}
/*------------------------------------------------------------------------------
=======
}

/*
>>>>>>> v3.18
=======
}

/*
>>>>>>> v3.18
 * Context: unknown
 */
static void oz_hcd_endpoint_reset(struct usb_hcd *hcd,
				struct usb_host_endpoint *ep)
{
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("oz_hcd_endpoint_reset\n");
}
/*------------------------------------------------------------------------------
=======
}

/*
>>>>>>> v3.18
=======
}

/*
>>>>>>> v3.18
 * Context: unknown
 */
static int oz_hcd_get_frame_number(struct usb_hcd *hcd)
{
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("oz_hcd_get_frame_number\n");
	return oz_usb_get_frame_number();
}
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
	oz_dbg(ON, "oz_hcd_get_frame_number\n");
	return oz_usb_get_frame_number();
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: softirq
 * This is called as a consquence of us calling usb_hcd_poll_rh_status() and we
 * always do that in softirq context.
 */
static int oz_hcd_hub_status_data(struct usb_hcd *hcd, char *buf)
{
	struct oz_hcd *ozhcd = oz_hcd_private(hcd);
	int i;

<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace2(OZ_TRACE_HUB, "oz_hcd_hub_status_data()\n");
	buf[0] = 0;
=======
	buf[0] = 0;
	buf[1] = 0;
>>>>>>> v3.18
=======
	buf[0] = 0;
	buf[1] = 0;
>>>>>>> v3.18

	spin_lock_bh(&ozhcd->hcd_lock);
	for (i = 0; i < OZ_NB_PORTS; i++) {
		if (ozhcd->ports[i].flags & OZ_PORT_F_CHANGED) {
<<<<<<< HEAD
<<<<<<< HEAD
			oz_trace2(OZ_TRACE_HUB, "Port %d changed\n", i);
			ozhcd->ports[i].flags &= ~OZ_PORT_F_CHANGED;
			buf[0] |= 1<<(i+1);
		}
	}
	spin_unlock_bh(&ozhcd->hcd_lock);
	return buf[0] ? 1 : 0;
}
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
			oz_dbg(HUB, "Port %d changed\n", i);
			ozhcd->ports[i].flags &= ~OZ_PORT_F_CHANGED;
			if (i < 7)
				buf[0] |= 1 << (i + 1);
			else
				buf[1] |= 1 << (i - 7);
		}
	}
	spin_unlock_bh(&ozhcd->hcd_lock);
	if (buf[0] != 0 || buf[1] != 0)
		return 2;
	return 0;
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: process
 */
static void oz_get_hub_descriptor(struct usb_hcd *hcd,
				struct usb_hub_descriptor *desc)
{
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace2(OZ_TRACE_HUB, "GetHubDescriptor\n");
	memset(desc, 0, sizeof(*desc));
	desc->bDescriptorType = 0x29;
	desc->bDescLength = 9;
	desc->wHubCharacteristics = (__force __u16)
			__constant_cpu_to_le16(0x0001);
	desc->bNbrPorts = OZ_NB_PORTS;
}
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
	memset(desc, 0, sizeof(*desc));
	desc->bDescriptorType = 0x29;
	desc->bDescLength = 9;
	desc->wHubCharacteristics = (__force __u16)cpu_to_le16(0x0001);
	desc->bNbrPorts = OZ_NB_PORTS;
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: process
 */
static int oz_set_port_feature(struct usb_hcd *hcd, u16 wvalue, u16 windex)
{
	struct oz_port *port;
<<<<<<< HEAD
<<<<<<< HEAD
	int err = 0;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	u8 port_id = (u8)windex;
	struct oz_hcd *ozhcd = oz_hcd_private(hcd);
	unsigned set_bits = 0;
	unsigned clear_bits = 0;
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace2(OZ_TRACE_HUB, "SetPortFeature\n");
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
	if ((port_id < 1) || (port_id > OZ_NB_PORTS))
		return -EPIPE;
	port = &ozhcd->ports[port_id-1];
	switch (wvalue) {
	case USB_PORT_FEAT_CONNECTION:
<<<<<<< HEAD
<<<<<<< HEAD
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_CONNECTION\n");
		break;
	case USB_PORT_FEAT_ENABLE:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_ENABLE\n");
		break;
	case USB_PORT_FEAT_SUSPEND:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_SUSPEND\n");
		break;
	case USB_PORT_FEAT_OVER_CURRENT:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_OVER_CURRENT\n");
		break;
	case USB_PORT_FEAT_RESET:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_RESET\n");
=======
=======
>>>>>>> v3.18
		oz_dbg(HUB, "USB_PORT_FEAT_CONNECTION\n");
		break;
	case USB_PORT_FEAT_ENABLE:
		oz_dbg(HUB, "USB_PORT_FEAT_ENABLE\n");
		break;
	case USB_PORT_FEAT_SUSPEND:
		oz_dbg(HUB, "USB_PORT_FEAT_SUSPEND\n");
		break;
	case USB_PORT_FEAT_OVER_CURRENT:
		oz_dbg(HUB, "USB_PORT_FEAT_OVER_CURRENT\n");
		break;
	case USB_PORT_FEAT_RESET:
		oz_dbg(HUB, "USB_PORT_FEAT_RESET\n");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		set_bits = USB_PORT_STAT_ENABLE | (USB_PORT_STAT_C_RESET<<16);
		clear_bits = USB_PORT_STAT_RESET;
		ozhcd->ports[port_id-1].bus_addr = 0;
		break;
	case USB_PORT_FEAT_POWER:
<<<<<<< HEAD
<<<<<<< HEAD
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_POWER\n");
		set_bits |= USB_PORT_STAT_POWER;
		break;
	case USB_PORT_FEAT_LOWSPEED:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_LOWSPEED\n");
		break;
	case USB_PORT_FEAT_C_CONNECTION:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_C_CONNECTION\n");
		break;
	case USB_PORT_FEAT_C_ENABLE:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_C_ENABLE\n");
		break;
	case USB_PORT_FEAT_C_SUSPEND:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_C_SUSPEND\n");
		break;
	case USB_PORT_FEAT_C_OVER_CURRENT:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_C_OVER_CURRENT\n");
		break;
	case USB_PORT_FEAT_C_RESET:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_C_RESET\n");
		break;
	case USB_PORT_FEAT_TEST:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_TEST\n");
		break;
	case USB_PORT_FEAT_INDICATOR:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_INDICATOR\n");
		break;
	default:
		oz_trace2(OZ_TRACE_HUB, "Other %d\n", wvalue);
=======
=======
>>>>>>> v3.18
		oz_dbg(HUB, "USB_PORT_FEAT_POWER\n");
		set_bits |= USB_PORT_STAT_POWER;
		break;
	case USB_PORT_FEAT_LOWSPEED:
		oz_dbg(HUB, "USB_PORT_FEAT_LOWSPEED\n");
		break;
	case USB_PORT_FEAT_C_CONNECTION:
		oz_dbg(HUB, "USB_PORT_FEAT_C_CONNECTION\n");
		break;
	case USB_PORT_FEAT_C_ENABLE:
		oz_dbg(HUB, "USB_PORT_FEAT_C_ENABLE\n");
		break;
	case USB_PORT_FEAT_C_SUSPEND:
		oz_dbg(HUB, "USB_PORT_FEAT_C_SUSPEND\n");
		break;
	case USB_PORT_FEAT_C_OVER_CURRENT:
		oz_dbg(HUB, "USB_PORT_FEAT_C_OVER_CURRENT\n");
		break;
	case USB_PORT_FEAT_C_RESET:
		oz_dbg(HUB, "USB_PORT_FEAT_C_RESET\n");
		break;
	case USB_PORT_FEAT_TEST:
		oz_dbg(HUB, "USB_PORT_FEAT_TEST\n");
		break;
	case USB_PORT_FEAT_INDICATOR:
		oz_dbg(HUB, "USB_PORT_FEAT_INDICATOR\n");
		break;
	default:
		oz_dbg(HUB, "Other %d\n", wvalue);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		break;
	}
	if (set_bits || clear_bits) {
		spin_lock_bh(&port->port_lock);
		port->status &= ~clear_bits;
		port->status |= set_bits;
		spin_unlock_bh(&port->port_lock);
	}
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace2(OZ_TRACE_HUB, "Port[%d] status = 0x%x\n", port_id,
		port->status);
	return err;
}
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
	oz_dbg(HUB, "Port[%d] status = 0x%x\n", port_id, port->status);
	return 0;
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: process
 */
static int oz_clear_port_feature(struct usb_hcd *hcd, u16 wvalue, u16 windex)
{
	struct oz_port *port;
<<<<<<< HEAD
<<<<<<< HEAD
	int err = 0;
	u8 port_id = (u8)windex;
	struct oz_hcd *ozhcd = oz_hcd_private(hcd);
	unsigned clear_bits = 0;
	oz_trace2(OZ_TRACE_HUB, "ClearPortFeature\n");
=======
=======
>>>>>>> v3.18
	u8 port_id = (u8)windex;
	struct oz_hcd *ozhcd = oz_hcd_private(hcd);
	unsigned clear_bits = 0;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if ((port_id < 1) || (port_id > OZ_NB_PORTS))
		return -EPIPE;
	port = &ozhcd->ports[port_id-1];
	switch (wvalue) {
	case USB_PORT_FEAT_CONNECTION:
<<<<<<< HEAD
<<<<<<< HEAD
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_CONNECTION\n");
		break;
	case USB_PORT_FEAT_ENABLE:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_ENABLE\n");
		clear_bits = USB_PORT_STAT_ENABLE;
		break;
	case USB_PORT_FEAT_SUSPEND:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_SUSPEND\n");
		break;
	case USB_PORT_FEAT_OVER_CURRENT:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_OVER_CURRENT\n");
		break;
	case USB_PORT_FEAT_RESET:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_RESET\n");
		break;
	case USB_PORT_FEAT_POWER:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_POWER\n");
		clear_bits |= USB_PORT_STAT_POWER;
		break;
	case USB_PORT_FEAT_LOWSPEED:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_LOWSPEED\n");
		break;
	case USB_PORT_FEAT_C_CONNECTION:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_C_CONNECTION\n");
		clear_bits = (USB_PORT_STAT_C_CONNECTION << 16);
		break;
	case USB_PORT_FEAT_C_ENABLE:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_C_ENABLE\n");
		clear_bits = (USB_PORT_STAT_C_ENABLE << 16);
		break;
	case USB_PORT_FEAT_C_SUSPEND:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_C_SUSPEND\n");
		break;
	case USB_PORT_FEAT_C_OVER_CURRENT:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_C_OVER_CURRENT\n");
		break;
	case USB_PORT_FEAT_C_RESET:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_C_RESET\n");
		clear_bits = (USB_PORT_FEAT_C_RESET << 16);
		break;
	case USB_PORT_FEAT_TEST:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_TEST\n");
		break;
	case USB_PORT_FEAT_INDICATOR:
		oz_trace2(OZ_TRACE_HUB, "USB_PORT_FEAT_INDICATOR\n");
		break;
	default:
		oz_trace2(OZ_TRACE_HUB, "Other %d\n", wvalue);
=======
=======
>>>>>>> v3.18
		oz_dbg(HUB, "USB_PORT_FEAT_CONNECTION\n");
		break;
	case USB_PORT_FEAT_ENABLE:
		oz_dbg(HUB, "USB_PORT_FEAT_ENABLE\n");
		clear_bits = USB_PORT_STAT_ENABLE;
		break;
	case USB_PORT_FEAT_SUSPEND:
		oz_dbg(HUB, "USB_PORT_FEAT_SUSPEND\n");
		break;
	case USB_PORT_FEAT_OVER_CURRENT:
		oz_dbg(HUB, "USB_PORT_FEAT_OVER_CURRENT\n");
		break;
	case USB_PORT_FEAT_RESET:
		oz_dbg(HUB, "USB_PORT_FEAT_RESET\n");
		break;
	case USB_PORT_FEAT_POWER:
		oz_dbg(HUB, "USB_PORT_FEAT_POWER\n");
		clear_bits |= USB_PORT_STAT_POWER;
		break;
	case USB_PORT_FEAT_LOWSPEED:
		oz_dbg(HUB, "USB_PORT_FEAT_LOWSPEED\n");
		break;
	case USB_PORT_FEAT_C_CONNECTION:
		oz_dbg(HUB, "USB_PORT_FEAT_C_CONNECTION\n");
		clear_bits = (USB_PORT_STAT_C_CONNECTION << 16);
		break;
	case USB_PORT_FEAT_C_ENABLE:
		oz_dbg(HUB, "USB_PORT_FEAT_C_ENABLE\n");
		clear_bits = (USB_PORT_STAT_C_ENABLE << 16);
		break;
	case USB_PORT_FEAT_C_SUSPEND:
		oz_dbg(HUB, "USB_PORT_FEAT_C_SUSPEND\n");
		break;
	case USB_PORT_FEAT_C_OVER_CURRENT:
		oz_dbg(HUB, "USB_PORT_FEAT_C_OVER_CURRENT\n");
		break;
	case USB_PORT_FEAT_C_RESET:
		oz_dbg(HUB, "USB_PORT_FEAT_C_RESET\n");
		clear_bits = (USB_PORT_FEAT_C_RESET << 16);
		break;
	case USB_PORT_FEAT_TEST:
		oz_dbg(HUB, "USB_PORT_FEAT_TEST\n");
		break;
	case USB_PORT_FEAT_INDICATOR:
		oz_dbg(HUB, "USB_PORT_FEAT_INDICATOR\n");
		break;
	default:
		oz_dbg(HUB, "Other %d\n", wvalue);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		break;
	}
	if (clear_bits) {
		spin_lock_bh(&port->port_lock);
		port->status &= ~clear_bits;
		spin_unlock_bh(&port->port_lock);
	}
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace2(OZ_TRACE_HUB, "Port[%d] status = 0x%x\n", port_id,
		ozhcd->ports[port_id-1].status);
	return err;
}
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
	oz_dbg(HUB, "Port[%d] status = 0x%x\n",
	       port_id, ozhcd->ports[port_id-1].status);
	return 0;
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: process
 */
static int oz_get_port_status(struct usb_hcd *hcd, u16 windex, char *buf)
{
	struct oz_hcd *ozhcd;
<<<<<<< HEAD
<<<<<<< HEAD
	u32 status = 0;
	if ((windex < 1) || (windex > OZ_NB_PORTS))
		return -EPIPE;
	ozhcd = oz_hcd_private(hcd);
	oz_trace2(OZ_TRACE_HUB, "GetPortStatus windex = %d\n", windex);
	status = ozhcd->ports[windex-1].status;
	put_unaligned(cpu_to_le32(status), (__le32 *)buf);
	oz_trace2(OZ_TRACE_HUB, "Port[%d] status = %x\n", windex, status);
	return 0;
}
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
	u32 status;

	if ((windex < 1) || (windex > OZ_NB_PORTS))
		return -EPIPE;
	ozhcd = oz_hcd_private(hcd);
	oz_dbg(HUB, "GetPortStatus windex = %d\n", windex);
	status = ozhcd->ports[windex-1].status;
	put_unaligned(cpu_to_le32(status), (__le32 *)buf);
	oz_dbg(HUB, "Port[%d] status = %x\n", windex, status);
	return 0;
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: process
 */
static int oz_hcd_hub_control(struct usb_hcd *hcd, u16 req_type, u16 wvalue,
				u16 windex, char *buf, u16 wlength)
{
	int err = 0;
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace2(OZ_TRACE_HUB, "oz_hcd_hub_control()\n");
	switch (req_type) {
	case ClearHubFeature:
		oz_trace2(OZ_TRACE_HUB, "ClearHubFeature: %d\n", req_type);
=======
=======
>>>>>>> v3.18

	switch (req_type) {
	case ClearHubFeature:
		oz_dbg(HUB, "ClearHubFeature: %d\n", req_type);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		break;
	case ClearPortFeature:
		err = oz_clear_port_feature(hcd, wvalue, windex);
		break;
	case GetHubDescriptor:
		oz_get_hub_descriptor(hcd, (struct usb_hub_descriptor *)buf);
		break;
	case GetHubStatus:
<<<<<<< HEAD
<<<<<<< HEAD
		oz_trace2(OZ_TRACE_HUB, "GetHubStatus: req_type = 0x%x\n",
			req_type);
		put_unaligned(__constant_cpu_to_le32(0), (__le32 *)buf);
=======
		oz_dbg(HUB, "GetHubStatus: req_type = 0x%x\n", req_type);
		put_unaligned(cpu_to_le32(0), (__le32 *)buf);
>>>>>>> v3.18
=======
		oz_dbg(HUB, "GetHubStatus: req_type = 0x%x\n", req_type);
		put_unaligned(cpu_to_le32(0), (__le32 *)buf);
>>>>>>> v3.18
		break;
	case GetPortStatus:
		err = oz_get_port_status(hcd, windex, buf);
		break;
	case SetHubFeature:
<<<<<<< HEAD
<<<<<<< HEAD
		oz_trace2(OZ_TRACE_HUB, "SetHubFeature: %d\n", req_type);
=======
		oz_dbg(HUB, "SetHubFeature: %d\n", req_type);
>>>>>>> v3.18
=======
		oz_dbg(HUB, "SetHubFeature: %d\n", req_type);
>>>>>>> v3.18
		break;
	case SetPortFeature:
		err = oz_set_port_feature(hcd, wvalue, windex);
		break;
	default:
<<<<<<< HEAD
<<<<<<< HEAD
		oz_trace2(OZ_TRACE_HUB, "Other: %d\n", req_type);
=======
		oz_dbg(HUB, "Other: %d\n", req_type);
>>>>>>> v3.18
=======
		oz_dbg(HUB, "Other: %d\n", req_type);
>>>>>>> v3.18
		break;
	}
	return err;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: process
 */
static int oz_hcd_bus_suspend(struct usb_hcd *hcd)
{
	struct oz_hcd *ozhcd;
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace2(OZ_TRACE_HUB, "oz_hcd_hub_suspend()\n");
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
	ozhcd = oz_hcd_private(hcd);
	spin_lock_bh(&ozhcd->hcd_lock);
	hcd->state = HC_STATE_SUSPENDED;
	ozhcd->flags |= OZ_HDC_F_SUSPENDED;
	spin_unlock_bh(&ozhcd->hcd_lock);
	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: process
 */
static int oz_hcd_bus_resume(struct usb_hcd *hcd)
{
	struct oz_hcd *ozhcd;
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace2(OZ_TRACE_HUB, "oz_hcd_hub_resume()\n");
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
	ozhcd = oz_hcd_private(hcd);
	spin_lock_bh(&ozhcd->hcd_lock);
	ozhcd->flags &= ~OZ_HDC_F_SUSPENDED;
	hcd->state = HC_STATE_RUNNING;
	spin_unlock_bh(&ozhcd->hcd_lock);
	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
 */
static void oz_plat_shutdown(struct platform_device *dev)
{
	oz_trace("oz_plat_shutdown()\n");
}
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18

static void oz_plat_shutdown(struct platform_device *dev)
{
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: process
 */
static int oz_plat_probe(struct platform_device *dev)
{
	int i;
	int err;
	struct usb_hcd *hcd;
	struct oz_hcd *ozhcd;
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("oz_plat_probe()\n");
	hcd = usb_create_hcd(&g_oz_hc_drv, &dev->dev, dev_name(&dev->dev));
	if (hcd == NULL) {
		oz_trace("Failed to created hcd object OK\n");
=======
=======
>>>>>>> v3.18

	hcd = usb_create_hcd(&g_oz_hc_drv, &dev->dev, dev_name(&dev->dev));
	if (hcd == NULL) {
		oz_dbg(ON, "Failed to created hcd object OK\n");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return -ENOMEM;
	}
	ozhcd = oz_hcd_private(hcd);
	memset(ozhcd, 0, sizeof(*ozhcd));
	INIT_LIST_HEAD(&ozhcd->urb_pending_list);
	INIT_LIST_HEAD(&ozhcd->urb_cancel_list);
	INIT_LIST_HEAD(&ozhcd->orphanage);
	ozhcd->hcd = hcd;
	ozhcd->conn_port = -1;
	spin_lock_init(&ozhcd->hcd_lock);
	for (i = 0; i < OZ_NB_PORTS; i++) {
		struct oz_port *port = &ozhcd->ports[i];
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
		port->ozhcd = ozhcd;
		port->flags = 0;
		port->status = 0;
		port->bus_addr = 0xff;
		spin_lock_init(&port->port_lock);
	}
	err = usb_add_hcd(hcd, 0, 0);
	if (err) {
<<<<<<< HEAD
<<<<<<< HEAD
		oz_trace("Failed to add hcd object OK\n");
		usb_put_hcd(hcd);
		return -1;
	}
=======
=======
>>>>>>> v3.18
		oz_dbg(ON, "Failed to add hcd object OK\n");
		usb_put_hcd(hcd);
		return -1;
	}
	device_wakeup_enable(hcd->self.controller);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	spin_lock_bh(&g_hcdlock);
	g_ozhcd = ozhcd;
	spin_unlock_bh(&g_hcdlock);
	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> v3.18
=======

/*
>>>>>>> v3.18
 * Context: unknown
 */
static int oz_plat_remove(struct platform_device *dev)
{
	struct usb_hcd *hcd = platform_get_drvdata(dev);
	struct oz_hcd *ozhcd;
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("oz_plat_remove()\n");
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
	if (hcd == NULL)
		return -1;
	ozhcd = oz_hcd_private(hcd);
	spin_lock_bh(&g_hcdlock);
	if (ozhcd == g_ozhcd)
		g_ozhcd = NULL;
	spin_unlock_bh(&g_hcdlock);
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("Clearing orphanage\n");
	oz_hcd_clear_orphanage(ozhcd, -EPIPE);
	oz_trace("Removing hcd\n");
	usb_remove_hcd(hcd);
	usb_put_hcd(hcd);
	oz_empty_link_pool();
	return 0;
}
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
	oz_dbg(ON, "Clearing orphanage\n");
	oz_hcd_clear_orphanage(ozhcd, -EPIPE);
	oz_dbg(ON, "Removing hcd\n");
	usb_remove_hcd(hcd);
	usb_put_hcd(hcd);
	return 0;
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: unknown
 */
static int oz_plat_suspend(struct platform_device *dev, pm_message_t msg)
{
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("oz_plat_suspend()\n");
	return 0;
}
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
	return 0;
}


/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: unknown
 */
static int oz_plat_resume(struct platform_device *dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("oz_plat_resume()\n");
	return 0;
}
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
	return 0;
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: process
 */
int oz_hcd_init(void)
{
	int err;
<<<<<<< HEAD
<<<<<<< HEAD
	if (usb_disabled())
		return -ENODEV;
	tasklet_init(&g_urb_process_tasklet, oz_urb_process_tasklet, 0);
	tasklet_init(&g_urb_cancel_tasklet, oz_urb_cancel_tasklet, 0);
	err = platform_driver_register(&g_oz_plat_drv);
	oz_trace("platform_driver_register() returned %d\n", err);
=======
=======
>>>>>>> v3.18

	if (usb_disabled())
		return -ENODEV;

	oz_urb_link_cache = KMEM_CACHE(oz_urb_link, 0);
	if (!oz_urb_link_cache)
		return -ENOMEM;

	tasklet_init(&g_urb_process_tasklet, oz_urb_process_tasklet, 0);
	tasklet_init(&g_urb_cancel_tasklet, oz_urb_cancel_tasklet, 0);
	err = platform_driver_register(&g_oz_plat_drv);
	oz_dbg(ON, "platform_driver_register() returned %d\n", err);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (err)
		goto error;
	g_plat_dev = platform_device_alloc(OZ_PLAT_DEV_NAME, -1);
	if (g_plat_dev == NULL) {
		err = -ENOMEM;
		goto error1;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("platform_device_alloc() succeeded\n");
	err = platform_device_add(g_plat_dev);
	if (err)
		goto error2;
	oz_trace("platform_device_add() succeeded\n");
=======
=======
>>>>>>> v3.18
	oz_dbg(ON, "platform_device_alloc() succeeded\n");
	err = platform_device_add(g_plat_dev);
	if (err)
		goto error2;
	oz_dbg(ON, "platform_device_add() succeeded\n");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
error2:
	platform_device_put(g_plat_dev);
error1:
	platform_driver_unregister(&g_oz_plat_drv);
error:
	tasklet_disable(&g_urb_process_tasklet);
	tasklet_disable(&g_urb_cancel_tasklet);
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("oz_hcd_init() failed %d\n", err);
	return err;
}
/*------------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
	oz_dbg(ON, "oz_hcd_init() failed %d\n", err);
	return err;
}

/*
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * Context: process
 */
void oz_hcd_term(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	msleep(OZ_HUB_DEBOUNCE_TIMEOUT);
>>>>>>> v3.18
=======
	msleep(OZ_HUB_DEBOUNCE_TIMEOUT);
>>>>>>> v3.18
	tasklet_kill(&g_urb_process_tasklet);
	tasklet_kill(&g_urb_cancel_tasklet);
	platform_device_unregister(g_plat_dev);
	platform_driver_unregister(&g_oz_plat_drv);
<<<<<<< HEAD
<<<<<<< HEAD
	oz_trace("Pending urbs:%d\n", atomic_read(&g_pending_urbs));
=======
	oz_dbg(ON, "Pending urbs:%d\n", atomic_read(&g_pending_urbs));
	kmem_cache_destroy(oz_urb_link_cache);
>>>>>>> v3.18
=======
	oz_dbg(ON, "Pending urbs:%d\n", atomic_read(&g_pending_urbs));
	kmem_cache_destroy(oz_urb_link_cache);
>>>>>>> v3.18
}
