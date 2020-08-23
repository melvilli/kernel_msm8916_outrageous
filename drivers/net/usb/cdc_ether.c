/*
 * CDC Ethernet based networking peripherals
 * Copyright (C) 2003-2005 by David Brownell
 * Copyright (C) 2006 by Ole Andre Vadla Ravnas (ActiveSync)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
<<<<<<< HEAD
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
>>>>>>> v3.18
 */

// #define	DEBUG			// error path messages, extra info
// #define	VERBOSE			// more; success messages

#include <linux/module.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> v3.18
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/workqueue.h>
#include <linux/mii.h>
#include <linux/usb.h>
#include <linux/usb/cdc.h>
#include <linux/usb/usbnet.h>


<<<<<<< HEAD
#if defined(CONFIG_USB_NET_RNDIS_HOST) || defined(CONFIG_USB_NET_RNDIS_HOST_MODULE)
=======
#if IS_ENABLED(CONFIG_USB_NET_RNDIS_HOST)
>>>>>>> v3.18

static int is_rndis(struct usb_interface_descriptor *desc)
{
	return (desc->bInterfaceClass == USB_CLASS_COMM &&
		desc->bInterfaceSubClass == 2 &&
		desc->bInterfaceProtocol == 0xff);
}

static int is_activesync(struct usb_interface_descriptor *desc)
{
	return (desc->bInterfaceClass == USB_CLASS_MISC &&
		desc->bInterfaceSubClass == 1 &&
		desc->bInterfaceProtocol == 1);
}

static int is_wireless_rndis(struct usb_interface_descriptor *desc)
{
	return (desc->bInterfaceClass == USB_CLASS_WIRELESS_CONTROLLER &&
		desc->bInterfaceSubClass == 1 &&
		desc->bInterfaceProtocol == 3);
}

#else

#define is_rndis(desc)		0
#define is_activesync(desc)	0
#define is_wireless_rndis(desc)	0

#endif

static const u8 mbm_guid[16] = {
	0xa3, 0x17, 0xa8, 0x8b, 0x04, 0x5e, 0x4f, 0x01,
	0xa6, 0x07, 0xc0, 0xff, 0xcb, 0x7e, 0x39, 0x2a,
};

<<<<<<< HEAD
/*
 * probes control interface, claims data interface, collects the bulk
=======
static void usbnet_cdc_update_filter(struct usbnet *dev)
{
	struct cdc_state	*info = (void *) &dev->data;
	struct usb_interface	*intf = info->control;

	u16 cdc_filter =
	    USB_CDC_PACKET_TYPE_ALL_MULTICAST | USB_CDC_PACKET_TYPE_DIRECTED |
	    USB_CDC_PACKET_TYPE_BROADCAST;

	if (dev->net->flags & IFF_PROMISC)
		cdc_filter |= USB_CDC_PACKET_TYPE_PROMISCUOUS;

	/* FIXME cdc-ether has some multicast code too, though it complains
	 * in routine cases.  info->ether describes the multicast support.
	 * Implement that here, manipulating the cdc filter as needed.
	 */

	usb_control_msg(dev->udev,
			usb_sndctrlpipe(dev->udev, 0),
			USB_CDC_SET_ETHERNET_PACKET_FILTER,
			USB_TYPE_CLASS | USB_RECIP_INTERFACE,
			cdc_filter,
			intf->cur_altsetting->desc.bInterfaceNumber,
			NULL,
			0,
			USB_CTRL_SET_TIMEOUT
		);
}

/* probes control interface, claims data interface, collects the bulk
>>>>>>> v3.18
 * endpoints, activates data interface (if needed), maybe sets MTU.
 * all pure cdc, except for certain firmware workarounds, and knowing
 * that rndis uses one different rule.
 */
int usbnet_generic_cdc_bind(struct usbnet *dev, struct usb_interface *intf)
{
	u8				*buf = intf->cur_altsetting->extra;
	int				len = intf->cur_altsetting->extralen;
	struct usb_interface_descriptor	*d;
	struct cdc_state		*info = (void *) &dev->data;
	int				status;
	int				rndis;
	bool				android_rndis_quirk = false;
	struct usb_driver		*driver = driver_of(intf);
	struct usb_cdc_mdlm_desc	*desc = NULL;
	struct usb_cdc_mdlm_detail_desc *detail = NULL;

<<<<<<< HEAD
	if (sizeof dev->data < sizeof *info)
=======
	if (sizeof(dev->data) < sizeof(*info))
>>>>>>> v3.18
		return -EDOM;

	/* expect strict spec conformance for the descriptors, but
	 * cope with firmware which stores them in the wrong place
	 */
	if (len == 0 && dev->udev->actconfig->extralen) {
		/* Motorola SB4100 (and others: Brad Hards says it's
		 * from a Broadcom design) put CDC descriptors here
		 */
		buf = dev->udev->actconfig->extra;
		len = dev->udev->actconfig->extralen;
		dev_dbg(&intf->dev, "CDC descriptors on config\n");
	}

	/* Maybe CDC descriptors are after the endpoint?  This bug has
	 * been seen on some 2Wire Inc RNDIS-ish products.
	 */
	if (len == 0) {
		struct usb_host_endpoint	*hep;

		hep = intf->cur_altsetting->endpoint;
		if (hep) {
			buf = hep->extra;
			len = hep->extralen;
		}
		if (len)
			dev_dbg(&intf->dev,
				"CDC descriptors on endpoint\n");
	}

	/* this assumes that if there's a non-RNDIS vendor variant
	 * of cdc-acm, it'll fail RNDIS requests cleanly.
	 */
	rndis = (is_rndis(&intf->cur_altsetting->desc) ||
		 is_activesync(&intf->cur_altsetting->desc) ||
		 is_wireless_rndis(&intf->cur_altsetting->desc));

<<<<<<< HEAD
	memset(info, 0, sizeof *info);
	info->control = intf;
	while (len > 3) {
		if (buf [1] != USB_DT_CS_INTERFACE)
=======
	memset(info, 0, sizeof(*info));
	info->control = intf;
	while (len > 3) {
		if (buf[1] != USB_DT_CS_INTERFACE)
>>>>>>> v3.18
			goto next_desc;

		/* use bDescriptorSubType to identify the CDC descriptors.
		 * We expect devices with CDC header and union descriptors.
		 * For CDC Ethernet we need the ethernet descriptor.
		 * For RNDIS, ignore two (pointless) CDC modem descriptors
		 * in favor of a complicated OID-based RPC scheme doing what
		 * CDC Ethernet achieves with a simple descriptor.
		 */
<<<<<<< HEAD
		switch (buf [2]) {
=======
		switch (buf[2]) {
>>>>>>> v3.18
		case USB_CDC_HEADER_TYPE:
			if (info->header) {
				dev_dbg(&intf->dev, "extra CDC header\n");
				goto bad_desc;
			}
			info->header = (void *) buf;
<<<<<<< HEAD
			if (info->header->bLength != sizeof *info->header) {
=======
			if (info->header->bLength != sizeof(*info->header)) {
>>>>>>> v3.18
				dev_dbg(&intf->dev, "CDC header len %u\n",
					info->header->bLength);
				goto bad_desc;
			}
			break;
		case USB_CDC_ACM_TYPE:
			/* paranoia:  disambiguate a "real" vendor-specific
			 * modem interface from an RNDIS non-modem.
			 */
			if (rndis) {
				struct usb_cdc_acm_descriptor *acm;

				acm = (void *) buf;
				if (acm->bmCapabilities) {
					dev_dbg(&intf->dev,
						"ACM capabilities %02x, "
						"not really RNDIS?\n",
						acm->bmCapabilities);
					goto bad_desc;
				}
			}
			break;
		case USB_CDC_UNION_TYPE:
			if (info->u) {
				dev_dbg(&intf->dev, "extra CDC union\n");
				goto bad_desc;
			}
			info->u = (void *) buf;
<<<<<<< HEAD
			if (info->u->bLength != sizeof *info->u) {
=======
			if (info->u->bLength != sizeof(*info->u)) {
>>>>>>> v3.18
				dev_dbg(&intf->dev, "CDC union len %u\n",
					info->u->bLength);
				goto bad_desc;
			}

			/* we need a master/control interface (what we're
			 * probed with) and a slave/data interface; union
			 * descriptors sort this all out.
			 */
			info->control = usb_ifnum_to_if(dev->udev,
						info->u->bMasterInterface0);
			info->data = usb_ifnum_to_if(dev->udev,
						info->u->bSlaveInterface0);
			if (!info->control || !info->data) {
				dev_dbg(&intf->dev,
<<<<<<< HEAD
					"master #%u/%pK slave #%u/%pK\n",
=======
					"master #%u/%p slave #%u/%p\n",
>>>>>>> v3.18
					info->u->bMasterInterface0,
					info->control,
					info->u->bSlaveInterface0,
					info->data);
				/* fall back to hard-wiring for RNDIS */
				if (rndis) {
					android_rndis_quirk = true;
					goto next_desc;
				}
				goto bad_desc;
			}
			if (info->control != intf) {
				dev_dbg(&intf->dev, "bogus CDC Union\n");
				/* Ambit USB Cable Modem (and maybe others)
				 * interchanges master and slave interface.
				 */
				if (info->data == intf) {
					info->data = info->control;
					info->control = intf;
				} else
					goto bad_desc;
			}

<<<<<<< HEAD
=======
			/* some devices merge these - skip class check */
			if (info->control == info->data)
				goto next_desc;

>>>>>>> v3.18
			/* a data interface altsetting does the real i/o */
			d = &info->data->cur_altsetting->desc;
			if (d->bInterfaceClass != USB_CLASS_CDC_DATA) {
				dev_dbg(&intf->dev, "slave class %u\n",
					d->bInterfaceClass);
				goto bad_desc;
			}
			break;
		case USB_CDC_ETHERNET_TYPE:
			if (info->ether) {
				dev_dbg(&intf->dev, "extra CDC ether\n");
				goto bad_desc;
			}
			info->ether = (void *) buf;
<<<<<<< HEAD
			if (info->ether->bLength != sizeof *info->ether) {
=======
			if (info->ether->bLength != sizeof(*info->ether)) {
>>>>>>> v3.18
				dev_dbg(&intf->dev, "CDC ether len %u\n",
					info->ether->bLength);
				goto bad_desc;
			}
			dev->hard_mtu = le16_to_cpu(
						info->ether->wMaxSegmentSize);
			/* because of Zaurus, we may be ignoring the host
			 * side link address we were given.
			 */
			break;
		case USB_CDC_MDLM_TYPE:
			if (desc) {
				dev_dbg(&intf->dev, "extra MDLM descriptor\n");
				goto bad_desc;
			}

			desc = (void *)buf;

			if (desc->bLength != sizeof(*desc))
				goto bad_desc;

			if (memcmp(&desc->bGUID, mbm_guid, 16))
				goto bad_desc;
			break;
		case USB_CDC_MDLM_DETAIL_TYPE:
			if (detail) {
				dev_dbg(&intf->dev, "extra MDLM detail descriptor\n");
				goto bad_desc;
			}

			detail = (void *)buf;

			if (detail->bGuidDescriptorType == 0) {
				if (detail->bLength < (sizeof(*detail) + 1))
					goto bad_desc;
			} else
				goto bad_desc;
			break;
		}
next_desc:
<<<<<<< HEAD
		len -= buf [0];	/* bLength */
		buf += buf [0];
=======
		len -= buf[0];	/* bLength */
		buf += buf[0];
>>>>>>> v3.18
	}

	/* Microsoft ActiveSync based and some regular RNDIS devices lack the
	 * CDC descriptors, so we'll hard-wire the interfaces and not check
	 * for descriptors.
	 *
	 * Some Android RNDIS devices have a CDC Union descriptor pointing
	 * to non-existing interfaces.  Ignore that and attempt the same
	 * hard-wired 0 and 1 interfaces.
	 */
	if (rndis && (!info->u || android_rndis_quirk)) {
		info->control = usb_ifnum_to_if(dev->udev, 0);
		info->data = usb_ifnum_to_if(dev->udev, 1);
		if (!info->control || !info->data || info->control != intf) {
			dev_dbg(&intf->dev,
<<<<<<< HEAD
				"rndis: master #0/%pK slave #1/%pK\n",
=======
				"rndis: master #0/%p slave #1/%p\n",
>>>>>>> v3.18
				info->control,
				info->data);
			goto bad_desc;
		}

	} else if (!info->header || !info->u || (!rndis && !info->ether)) {
		dev_dbg(&intf->dev, "missing cdc %s%s%sdescriptor\n",
			info->header ? "" : "header ",
			info->u ? "" : "union ",
			info->ether ? "" : "ether ");
		goto bad_desc;
	}

	/* claim data interface and set it up ... with side effects.
	 * network traffic can't flow until an altsetting is enabled.
	 */
<<<<<<< HEAD
	status = usb_driver_claim_interface(driver, info->data, dev);
	if (status < 0)
		return status;
=======
	if (info->data != info->control) {
		status = usb_driver_claim_interface(driver, info->data, dev);
		if (status < 0)
			return status;
	}
>>>>>>> v3.18
	status = usbnet_get_endpoints(dev, info->data);
	if (status < 0) {
		/* ensure immediate exit from usbnet_disconnect */
		usb_set_intfdata(info->data, NULL);
<<<<<<< HEAD
		usb_driver_release_interface(driver, info->data);
=======
		if (info->data != info->control)
			usb_driver_release_interface(driver, info->data);
>>>>>>> v3.18
		return status;
	}

	/* status endpoint: optional for CDC Ethernet, not RNDIS (or ACM) */
<<<<<<< HEAD
	dev->status = NULL;
=======
	if (info->data != info->control)
		dev->status = NULL;
>>>>>>> v3.18
	if (info->control->cur_altsetting->desc.bNumEndpoints == 1) {
		struct usb_endpoint_descriptor	*desc;

		dev->status = &info->control->cur_altsetting->endpoint [0];
		desc = &dev->status->desc;
		if (!usb_endpoint_is_int_in(desc) ||
		    (le16_to_cpu(desc->wMaxPacketSize)
		     < sizeof(struct usb_cdc_notification)) ||
		    !desc->bInterval) {
			dev_dbg(&intf->dev, "bad notification endpoint\n");
			dev->status = NULL;
		}
	}
	if (rndis && !dev->status) {
		dev_dbg(&intf->dev, "missing RNDIS status endpoint\n");
		usb_set_intfdata(info->data, NULL);
		usb_driver_release_interface(driver, info->data);
		return -ENODEV;
	}
<<<<<<< HEAD
=======

	/* Some devices don't initialise properly. In particular
	 * the packet filter is not reset. There are devices that
	 * don't do reset all the way. So the packet filter should
	 * be set to a sane initial value.
	 */
	usbnet_cdc_update_filter(dev);

>>>>>>> v3.18
	return 0;

bad_desc:
	dev_info(&dev->udev->dev, "bad CDC descriptors\n");
	return -ENODEV;
}
EXPORT_SYMBOL_GPL(usbnet_generic_cdc_bind);

void usbnet_cdc_unbind(struct usbnet *dev, struct usb_interface *intf)
{
	struct cdc_state		*info = (void *) &dev->data;
	struct usb_driver		*driver = driver_of(intf);

<<<<<<< HEAD
=======
	/* combined interface - nothing  to do */
	if (info->data == info->control)
		return;

>>>>>>> v3.18
	/* disconnect master --> disconnect slave */
	if (intf == info->control && info->data) {
		/* ensure immediate exit from usbnet_disconnect */
		usb_set_intfdata(info->data, NULL);
		usb_driver_release_interface(driver, info->data);
		info->data = NULL;
	}

	/* and vice versa (just in case) */
	else if (intf == info->data && info->control) {
		/* ensure immediate exit from usbnet_disconnect */
		usb_set_intfdata(info->control, NULL);
		usb_driver_release_interface(driver, info->control);
		info->control = NULL;
	}
}
EXPORT_SYMBOL_GPL(usbnet_cdc_unbind);

<<<<<<< HEAD
/*-------------------------------------------------------------------------
 *
 * Communications Device Class, Ethernet Control model
=======
/* Communications Device Class, Ethernet Control model
>>>>>>> v3.18
 *
 * Takes two interfaces.  The DATA interface is inactive till an altsetting
 * is selected.  Configuration data includes class descriptors.  There's
 * an optional status endpoint on the control interface.
 *
 * This should interop with whatever the 2.4 "CDCEther.c" driver
 * (by Brad Hards) talked with, with more functionality.
<<<<<<< HEAD
 *
 *-------------------------------------------------------------------------*/
=======
 */
>>>>>>> v3.18

static void dumpspeed(struct usbnet *dev, __le32 *speeds)
{
	netif_info(dev, timer, dev->net,
		   "link speeds: %u kbps up, %u kbps down\n",
		   __le32_to_cpu(speeds[0]) / 1000,
		   __le32_to_cpu(speeds[1]) / 1000);
}

void usbnet_cdc_status(struct usbnet *dev, struct urb *urb)
{
	struct usb_cdc_notification	*event;

<<<<<<< HEAD
	if (urb->actual_length < sizeof *event)
=======
	if (urb->actual_length < sizeof(*event))
>>>>>>> v3.18
		return;

	/* SPEED_CHANGE can get split into two 8-byte packets */
	if (test_and_clear_bit(EVENT_STS_SPLIT, &dev->flags)) {
		dumpspeed(dev, (__le32 *) urb->transfer_buffer);
		return;
	}

	event = urb->transfer_buffer;
	switch (event->bNotificationType) {
	case USB_CDC_NOTIFY_NETWORK_CONNECTION:
		netif_dbg(dev, timer, dev->net, "CDC: carrier %s\n",
			  event->wValue ? "on" : "off");
		usbnet_link_change(dev, !!event->wValue, 0);
		break;
	case USB_CDC_NOTIFY_SPEED_CHANGE:	/* tx/rx rates */
		netif_dbg(dev, timer, dev->net, "CDC: speed change (len %d)\n",
			  urb->actual_length);
<<<<<<< HEAD
		if (urb->actual_length != (sizeof *event + 8))
=======
		if (urb->actual_length != (sizeof(*event) + 8))
>>>>>>> v3.18
			set_bit(EVENT_STS_SPLIT, &dev->flags);
		else
			dumpspeed(dev, (__le32 *) &event[1]);
		break;
	/* USB_CDC_NOTIFY_RESPONSE_AVAILABLE can happen too (e.g. RNDIS),
	 * but there are no standard formats for the response data.
	 */
	default:
		netdev_err(dev->net, "CDC: unexpected notification %02x!\n",
			   event->bNotificationType);
		break;
	}
}
EXPORT_SYMBOL_GPL(usbnet_cdc_status);

int usbnet_cdc_bind(struct usbnet *dev, struct usb_interface *intf)
{
	int				status;
	struct cdc_state		*info = (void *) &dev->data;

	BUILD_BUG_ON((sizeof(((struct usbnet *)0)->data)
			< sizeof(struct cdc_state)));

	status = usbnet_generic_cdc_bind(dev, intf);
	if (status < 0)
		return status;

	status = usbnet_get_ethernet_addr(dev, info->ether->iMACAddress);
	if (status < 0) {
		usb_set_intfdata(info->data, NULL);
		usb_driver_release_interface(driver_of(intf), info->data);
		return status;
	}

<<<<<<< HEAD
	/* FIXME cdc-ether has some multicast code too, though it complains
	 * in routine cases.  info->ether describes the multicast support.
	 * Implement that here, manipulating the cdc filter as needed.
	 */
=======
>>>>>>> v3.18
	return 0;
}
EXPORT_SYMBOL_GPL(usbnet_cdc_bind);

static const struct driver_info	cdc_info = {
	.description =	"CDC Ethernet Device",
	.flags =	FLAG_ETHER | FLAG_POINTTOPOINT,
<<<<<<< HEAD
	// .check_connect = cdc_check_connect,
	.bind =		usbnet_cdc_bind,
	.unbind =	usbnet_cdc_unbind,
	.status =	usbnet_cdc_status,
=======
	.bind =		usbnet_cdc_bind,
	.unbind =	usbnet_cdc_unbind,
	.status =	usbnet_cdc_status,
	.set_rx_mode =	usbnet_cdc_update_filter,
>>>>>>> v3.18
	.manage_power =	usbnet_manage_power,
};

static const struct driver_info wwan_info = {
	.description =	"Mobile Broadband Network Device",
	.flags =	FLAG_WWAN,
	.bind =		usbnet_cdc_bind,
	.unbind =	usbnet_cdc_unbind,
	.status =	usbnet_cdc_status,
<<<<<<< HEAD
=======
	.set_rx_mode =	usbnet_cdc_update_filter,
>>>>>>> v3.18
	.manage_power =	usbnet_manage_power,
};

/*-------------------------------------------------------------------------*/

#define HUAWEI_VENDOR_ID	0x12D1
#define NOVATEL_VENDOR_ID	0x1410
#define ZTE_VENDOR_ID		0x19D2
#define DELL_VENDOR_ID		0x413C
#define REALTEK_VENDOR_ID	0x0bda
<<<<<<< HEAD

static const struct usb_device_id	products [] = {
/*
 * BLACKLIST !!
=======
#define SAMSUNG_VENDOR_ID	0x04e8

static const struct usb_device_id	products[] = {
/* BLACKLIST !!
>>>>>>> v3.18
 *
 * First blacklist any products that are egregiously nonconformant
 * with the CDC Ethernet specs.  Minor braindamage we cope with; when
 * they're not even trying, needing a separate driver is only the first
 * of the differences to show up.
 */

#define	ZAURUS_MASTER_INTERFACE \
	.bInterfaceClass	= USB_CLASS_COMM, \
	.bInterfaceSubClass	= USB_CDC_SUBCLASS_ETHERNET, \
	.bInterfaceProtocol	= USB_CDC_PROTO_NONE

/* SA-1100 based Sharp Zaurus ("collie"), or compatible;
 * wire-incompatible with true CDC Ethernet implementations.
 * (And, it seems, needlessly so...)
 */
{
	.match_flags	=   USB_DEVICE_ID_MATCH_INT_INFO
			  | USB_DEVICE_ID_MATCH_DEVICE,
	.idVendor		= 0x04DD,
	.idProduct		= 0x8004,
	ZAURUS_MASTER_INTERFACE,
	.driver_info		= 0,
},

/* PXA-25x based Sharp Zaurii.  Note that it seems some of these
 * (later models especially) may have shipped only with firmware
 * advertising false "CDC MDLM" compatibility ... but we're not
 * clear which models did that, so for now let's assume the worst.
 */
{
	.match_flags	=   USB_DEVICE_ID_MATCH_INT_INFO
			  | USB_DEVICE_ID_MATCH_DEVICE,
	.idVendor		= 0x04DD,
	.idProduct		= 0x8005,	/* A-300 */
	ZAURUS_MASTER_INTERFACE,
	.driver_info		= 0,
}, {
	.match_flags	=   USB_DEVICE_ID_MATCH_INT_INFO
			  | USB_DEVICE_ID_MATCH_DEVICE,
	.idVendor		= 0x04DD,
	.idProduct		= 0x8006,	/* B-500/SL-5600 */
	ZAURUS_MASTER_INTERFACE,
	.driver_info		= 0,
}, {
	.match_flags    =   USB_DEVICE_ID_MATCH_INT_INFO
<<<<<<< HEAD
	          | USB_DEVICE_ID_MATCH_DEVICE,
=======
			  | USB_DEVICE_ID_MATCH_DEVICE,
>>>>>>> v3.18
	.idVendor		= 0x04DD,
	.idProduct		= 0x8007,	/* C-700 */
	ZAURUS_MASTER_INTERFACE,
	.driver_info		= 0,
}, {
	.match_flags    =   USB_DEVICE_ID_MATCH_INT_INFO
		 | USB_DEVICE_ID_MATCH_DEVICE,
	.idVendor               = 0x04DD,
	.idProduct              = 0x9031,	/* C-750 C-760 */
	ZAURUS_MASTER_INTERFACE,
	.driver_info		= 0,
}, {
	.match_flags    =   USB_DEVICE_ID_MATCH_INT_INFO
		 | USB_DEVICE_ID_MATCH_DEVICE,
	.idVendor               = 0x04DD,
	.idProduct              = 0x9032,	/* SL-6000 */
	ZAURUS_MASTER_INTERFACE,
	.driver_info		= 0,
}, {
	.match_flags    =   USB_DEVICE_ID_MATCH_INT_INFO
		 | USB_DEVICE_ID_MATCH_DEVICE,
	.idVendor               = 0x04DD,
	/* reported with some C860 units */
	.idProduct              = 0x9050,	/* C-860 */
	ZAURUS_MASTER_INTERFACE,
	.driver_info		= 0,
},

/* Olympus has some models with a Zaurus-compatible option.
 * R-1000 uses a FreeScale i.MXL cpu (ARMv4T)
 */
{
	.match_flags    =   USB_DEVICE_ID_MATCH_INT_INFO
		 | USB_DEVICE_ID_MATCH_DEVICE,
	.idVendor               = 0x07B4,
	.idProduct              = 0x0F02,	/* R-1000 */
	ZAURUS_MASTER_INTERFACE,
	.driver_info		= 0,
},

/* LG Electronics VL600 wants additional headers on every frame */
{
	USB_DEVICE_AND_INTERFACE_INFO(0x1004, 0x61aa, USB_CLASS_COMM,
			USB_CDC_SUBCLASS_ETHERNET, USB_CDC_PROTO_NONE),
	.driver_info = 0,
},

/* Logitech Harmony 900 - uses the pseudo-MDLM (BLAN) driver */
{
	USB_DEVICE_AND_INTERFACE_INFO(0x046d, 0xc11f, USB_CLASS_COMM,
			USB_CDC_SUBCLASS_MDLM, USB_CDC_PROTO_NONE),
	.driver_info		= 0,
},

/* Novatel USB551L and MC551 - handled by qmi_wwan */
{
	USB_DEVICE_AND_INTERFACE_INFO(NOVATEL_VENDOR_ID, 0xB001, USB_CLASS_COMM,
			USB_CDC_SUBCLASS_ETHERNET, USB_CDC_PROTO_NONE),
	.driver_info = 0,
},

/* Novatel E362 - handled by qmi_wwan */
{
	USB_DEVICE_AND_INTERFACE_INFO(NOVATEL_VENDOR_ID, 0x9010, USB_CLASS_COMM,
			USB_CDC_SUBCLASS_ETHERNET, USB_CDC_PROTO_NONE),
	.driver_info = 0,
},

/* Dell Wireless 5800 (Novatel E362) - handled by qmi_wwan */
{
	USB_DEVICE_AND_INTERFACE_INFO(DELL_VENDOR_ID, 0x8195, USB_CLASS_COMM,
			USB_CDC_SUBCLASS_ETHERNET, USB_CDC_PROTO_NONE),
	.driver_info = 0,
},

/* Dell Wireless 5800 (Novatel E362) - handled by qmi_wwan */
{
	USB_DEVICE_AND_INTERFACE_INFO(DELL_VENDOR_ID, 0x8196, USB_CLASS_COMM,
			USB_CDC_SUBCLASS_ETHERNET, USB_CDC_PROTO_NONE),
	.driver_info = 0,
},

/* Dell Wireless 5804 (Novatel E371) - handled by qmi_wwan */
{
	USB_DEVICE_AND_INTERFACE_INFO(DELL_VENDOR_ID, 0x819b, USB_CLASS_COMM,
			USB_CDC_SUBCLASS_ETHERNET, USB_CDC_PROTO_NONE),
	.driver_info = 0,
},

<<<<<<< HEAD
=======
/* Novatel Expedite E371 - handled by qmi_wwan */
{
	USB_DEVICE_AND_INTERFACE_INFO(NOVATEL_VENDOR_ID, 0x9011, USB_CLASS_COMM,
			USB_CDC_SUBCLASS_ETHERNET, USB_CDC_PROTO_NONE),
	.driver_info = 0,
},

>>>>>>> v3.18
/* AnyDATA ADU960S - handled by qmi_wwan */
{
	USB_DEVICE_AND_INTERFACE_INFO(0x16d5, 0x650a, USB_CLASS_COMM,
			USB_CDC_SUBCLASS_ETHERNET, USB_CDC_PROTO_NONE),
	.driver_info = 0,
},

/* Huawei E1820 - handled by qmi_wwan */
{
	USB_DEVICE_INTERFACE_NUMBER(HUAWEI_VENDOR_ID, 0x14ac, 1),
	.driver_info = 0,
},

/* Realtek RTL8152 Based USB 2.0 Ethernet Adapters */
<<<<<<< HEAD
#if defined(CONFIG_USB_RTL8152) || defined(CONFIG_USB_RTL8152_MODULE)
=======
>>>>>>> v3.18
{
	USB_DEVICE_AND_INTERFACE_INFO(REALTEK_VENDOR_ID, 0x8152, USB_CLASS_COMM,
			USB_CDC_SUBCLASS_ETHERNET, USB_CDC_PROTO_NONE),
	.driver_info = 0,
},
<<<<<<< HEAD
#endif

/*
 * WHITELIST!!!
=======

/* Realtek RTL8153 Based USB 3.0 Ethernet Adapters */
{
	USB_DEVICE_AND_INTERFACE_INFO(REALTEK_VENDOR_ID, 0x8153, USB_CLASS_COMM,
			USB_CDC_SUBCLASS_ETHERNET, USB_CDC_PROTO_NONE),
	.driver_info = 0,
},

/* Samsung USB Ethernet Adapters */
{
	USB_DEVICE_AND_INTERFACE_INFO(SAMSUNG_VENDOR_ID, 0xa101, USB_CLASS_COMM,
			USB_CDC_SUBCLASS_ETHERNET, USB_CDC_PROTO_NONE),
	.driver_info = 0,
},

/* WHITELIST!!!
>>>>>>> v3.18
 *
 * CDC Ether uses two interfaces, not necessarily consecutive.
 * We match the main interface, ignoring the optional device
 * class so we could handle devices that aren't exclusively
 * CDC ether.
 *
 * NOTE:  this match must come AFTER entries blacklisting devices
 * because of bugs/quirks in a given product (like Zaurus, above).
 */
{
	/* ZTE (Vodafone) K3805-Z */
<<<<<<< HEAD
	.match_flags    =   USB_DEVICE_ID_MATCH_VENDOR
		 | USB_DEVICE_ID_MATCH_PRODUCT
		 | USB_DEVICE_ID_MATCH_INT_INFO,
	.idVendor               = ZTE_VENDOR_ID,
	.idProduct		= 0x1003,
	.bInterfaceClass	= USB_CLASS_COMM,
	.bInterfaceSubClass	= USB_CDC_SUBCLASS_ETHERNET,
	.bInterfaceProtocol	= USB_CDC_PROTO_NONE,
	.driver_info = (unsigned long)&wwan_info,
}, {
	/* ZTE (Vodafone) K3806-Z */
	.match_flags    =   USB_DEVICE_ID_MATCH_VENDOR
		 | USB_DEVICE_ID_MATCH_PRODUCT
		 | USB_DEVICE_ID_MATCH_INT_INFO,
	.idVendor               = ZTE_VENDOR_ID,
	.idProduct		= 0x1015,
	.bInterfaceClass	= USB_CLASS_COMM,
	.bInterfaceSubClass	= USB_CDC_SUBCLASS_ETHERNET,
	.bInterfaceProtocol	= USB_CDC_PROTO_NONE,
	.driver_info = (unsigned long)&wwan_info,
}, {
	/* ZTE (Vodafone) K4510-Z */
	.match_flags    =   USB_DEVICE_ID_MATCH_VENDOR
		 | USB_DEVICE_ID_MATCH_PRODUCT
		 | USB_DEVICE_ID_MATCH_INT_INFO,
	.idVendor               = ZTE_VENDOR_ID,
	.idProduct		= 0x1173,
	.bInterfaceClass	= USB_CLASS_COMM,
	.bInterfaceSubClass	= USB_CDC_SUBCLASS_ETHERNET,
	.bInterfaceProtocol	= USB_CDC_PROTO_NONE,
	.driver_info = (unsigned long)&wwan_info,
}, {
	/* ZTE (Vodafone) K3770-Z */
	.match_flags    =   USB_DEVICE_ID_MATCH_VENDOR
		 | USB_DEVICE_ID_MATCH_PRODUCT
		 | USB_DEVICE_ID_MATCH_INT_INFO,
	.idVendor               = ZTE_VENDOR_ID,
	.idProduct		= 0x1177,
	.bInterfaceClass	= USB_CLASS_COMM,
	.bInterfaceSubClass	= USB_CDC_SUBCLASS_ETHERNET,
	.bInterfaceProtocol	= USB_CDC_PROTO_NONE,
	.driver_info = (unsigned long)&wwan_info,
}, {
	/* ZTE (Vodafone) K3772-Z */
	.match_flags    =   USB_DEVICE_ID_MATCH_VENDOR
		 | USB_DEVICE_ID_MATCH_PRODUCT
		 | USB_DEVICE_ID_MATCH_INT_INFO,
	.idVendor               = ZTE_VENDOR_ID,
	.idProduct		= 0x1181,
	.bInterfaceClass	= USB_CLASS_COMM,
	.bInterfaceSubClass	= USB_CDC_SUBCLASS_ETHERNET,
	.bInterfaceProtocol	= USB_CDC_PROTO_NONE,
=======
	USB_DEVICE_AND_INTERFACE_INFO(ZTE_VENDOR_ID, 0x1003, USB_CLASS_COMM,
				      USB_CDC_SUBCLASS_ETHERNET,
				      USB_CDC_PROTO_NONE),
	.driver_info = (unsigned long)&wwan_info,
}, {
	/* ZTE (Vodafone) K3806-Z */
	USB_DEVICE_AND_INTERFACE_INFO(ZTE_VENDOR_ID, 0x1015, USB_CLASS_COMM,
				      USB_CDC_SUBCLASS_ETHERNET,
				      USB_CDC_PROTO_NONE),
	.driver_info = (unsigned long)&wwan_info,
}, {
	/* ZTE (Vodafone) K4510-Z */
	USB_DEVICE_AND_INTERFACE_INFO(ZTE_VENDOR_ID, 0x1173, USB_CLASS_COMM,
				      USB_CDC_SUBCLASS_ETHERNET,
				      USB_CDC_PROTO_NONE),
	.driver_info = (unsigned long)&wwan_info,
}, {
	/* ZTE (Vodafone) K3770-Z */
	USB_DEVICE_AND_INTERFACE_INFO(ZTE_VENDOR_ID, 0x1177, USB_CLASS_COMM,
				      USB_CDC_SUBCLASS_ETHERNET,
				      USB_CDC_PROTO_NONE),
	.driver_info = (unsigned long)&wwan_info,
}, {
	/* ZTE (Vodafone) K3772-Z */
	USB_DEVICE_AND_INTERFACE_INFO(ZTE_VENDOR_ID, 0x1181, USB_CLASS_COMM,
				      USB_CDC_SUBCLASS_ETHERNET,
				      USB_CDC_PROTO_NONE),
>>>>>>> v3.18
	.driver_info = (unsigned long)&wwan_info,
}, {
	/* Telit modules */
	USB_VENDOR_AND_INTERFACE_INFO(0x1bc7, USB_CLASS_COMM,
			USB_CDC_SUBCLASS_ETHERNET, USB_CDC_PROTO_NONE),
	.driver_info = (kernel_ulong_t) &wwan_info,
}, {
	USB_INTERFACE_INFO(USB_CLASS_COMM, USB_CDC_SUBCLASS_ETHERNET,
			USB_CDC_PROTO_NONE),
	.driver_info = (unsigned long) &cdc_info,
}, {
	USB_INTERFACE_INFO(USB_CLASS_COMM, USB_CDC_SUBCLASS_MDLM,
			USB_CDC_PROTO_NONE),
	.driver_info = (unsigned long)&wwan_info,

}, {
	/* Various Huawei modems with a network port like the UMG1831 */
<<<<<<< HEAD
	.match_flags    =   USB_DEVICE_ID_MATCH_VENDOR
		 | USB_DEVICE_ID_MATCH_INT_INFO,
	.idVendor               = HUAWEI_VENDOR_ID,
	.bInterfaceClass	= USB_CLASS_COMM,
	.bInterfaceSubClass	= USB_CDC_SUBCLASS_ETHERNET,
	.bInterfaceProtocol	= 255,
	.driver_info = (unsigned long)&wwan_info,
},
	{ },		// END
=======
	USB_VENDOR_AND_INTERFACE_INFO(HUAWEI_VENDOR_ID, USB_CLASS_COMM,
				      USB_CDC_SUBCLASS_ETHERNET, 255),
	.driver_info = (unsigned long)&wwan_info,
},
	{ },		/* END */
>>>>>>> v3.18
};
MODULE_DEVICE_TABLE(usb, products);

static struct usb_driver cdc_driver = {
	.name =		"cdc_ether",
	.id_table =	products,
	.probe =	usbnet_probe,
	.disconnect =	usbnet_disconnect,
	.suspend =	usbnet_suspend,
	.resume =	usbnet_resume,
	.reset_resume =	usbnet_resume,
	.supports_autosuspend = 1,
	.disable_hub_initiated_lpm = 1,
};

module_usb_driver(cdc_driver);

MODULE_AUTHOR("David Brownell");
MODULE_DESCRIPTION("USB CDC Ethernet devices");
MODULE_LICENSE("GPL");
