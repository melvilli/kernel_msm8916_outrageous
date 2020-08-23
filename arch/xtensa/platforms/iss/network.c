/*
 *
 * arch/xtensa/platforms/iss/network.c
 *
 * Platform specific initialization.
 *
 * Authors: Chris Zankel <chris@zankel.net>
 * Based on work form the UML team.
 *
 * Copyright 2005 Tensilica Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <linux/list.h>
#include <linux/irq.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/if_ether.h>
#include <linux/inetdevice.h>
#include <linux/init.h>
#include <linux/if_tun.h>
#include <linux/etherdevice.h>
#include <linux/interrupt.h>
#include <linux/ioctl.h>
#include <linux/bootmem.h>
#include <linux/ethtool.h>
#include <linux/rtnetlink.h>
#include <linux/platform_device.h>

#include <platform/simcall.h>

#define DRIVER_NAME "iss-netdev"
#define ETH_MAX_PACKET 1500
#define ETH_HEADER_OTHER 14
<<<<<<< HEAD
<<<<<<< HEAD
#define ISS_NET_TIMER_VALUE (2 * HZ)
=======
#define ISS_NET_TIMER_VALUE (HZ / 10)
>>>>>>> v3.18
=======
#define ISS_NET_TIMER_VALUE (HZ / 10)
>>>>>>> v3.18


static DEFINE_SPINLOCK(opened_lock);
static LIST_HEAD(opened);

static DEFINE_SPINLOCK(devices_lock);
static LIST_HEAD(devices);

/* ------------------------------------------------------------------------- */

/* We currently only support the TUNTAP transport protocol. */

#define TRANSPORT_TUNTAP_NAME "tuntap"
#define TRANSPORT_TUNTAP_MTU ETH_MAX_PACKET

struct tuntap_info {
	char dev_name[IFNAMSIZ];
<<<<<<< HEAD
<<<<<<< HEAD
	int fixed_config;
	unsigned char gw[ETH_ALEN];
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	int fd;
};

/* ------------------------------------------------------------------------- */


/* This structure contains out private information for the driver. */

struct iss_net_private {
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	struct list_head device_list;
	struct list_head opened_list;

	spinlock_t lock;
	struct net_device *dev;
	struct platform_device pdev;
	struct timer_list tl;
	struct net_device_stats stats;

	struct timer_list timer;
	unsigned int timer_val;

	int index;
	int mtu;

<<<<<<< HEAD
<<<<<<< HEAD
	unsigned char mac[ETH_ALEN];
	int have_mac;

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	struct {
		union {
			struct tuntap_info tuntap;
		} info;

		int (*open)(struct iss_net_private *lp);
		void (*close)(struct iss_net_private *lp);
		int (*read)(struct iss_net_private *lp, struct sk_buff **skb);
		int (*write)(struct iss_net_private *lp, struct sk_buff **skb);
		unsigned short (*protocol)(struct sk_buff *skb);
		int (*poll)(struct iss_net_private *lp);
	} tp;

};

/* ================================ HELPERS ================================ */


static char *split_if_spec(char *str, ...)
{
	char **arg, *end;
	va_list ap;

	va_start(ap, str);
	while ((arg = va_arg(ap, char**)) != NULL) {
		if (*str == '\0')
			return NULL;
		end = strchr(str, ',');
		if (end != str)
			*arg = str;
		if (end == NULL)
			return NULL;
<<<<<<< HEAD
<<<<<<< HEAD
		*end ++ = '\0';
=======
		*end++ = '\0';
>>>>>>> v3.18
=======
		*end++ = '\0';
>>>>>>> v3.18
		str = end;
	}
	va_end(ap);
	return str;
}

<<<<<<< HEAD
<<<<<<< HEAD

#if 0
/* Adjust SKB. */

struct sk_buff *ether_adjust_skb(struct sk_buff *skb, int extra)
{
	if ((skb != NULL) && (skb_tailroom(skb) < extra)) {
		struct sk_buff *skb2;

		skb2 = skb_copy_expand(skb, 0, extra, GFP_ATOMIC);
		dev_kfree_skb(skb);
		skb = skb2;
	}
	if (skb != NULL)
		skb_put(skb, extra);

	return skb;
}
#endif

/* Return the IP address as a string for a given device. */

static void dev_ip_addr(void *d, char *buf, char *bin_buf)
{
	struct net_device *dev = d;
	struct in_device *ip = dev->ip_ptr;
	struct in_ifaddr *in;
	__be32 addr;

	if ((ip == NULL) || ((in = ip->ifa_list) == NULL)) {
		printk(KERN_WARNING "Device not assigned an IP address!\n");
		return;
	}

	addr = in->ifa_address;
	sprintf(buf, "%d.%d.%d.%d", addr & 0xff, (addr >> 8) & 0xff,
		(addr >> 16) & 0xff, addr >> 24);

	if (bin_buf) {
		bin_buf[0] = addr & 0xff;
		bin_buf[1] = (addr >> 8) & 0xff;
		bin_buf[2] = (addr >> 16) & 0xff;
		bin_buf[3] = addr >> 24;
	}
}

/* Set Ethernet address of the specified device. */

static void inline set_ether_mac(void *d, unsigned char *addr)
{
	struct net_device *dev = d;
	memcpy(dev->dev_addr, addr, ETH_ALEN);
}


=======
=======
>>>>>>> v3.18
/* Set Ethernet address of the specified device. */

static void setup_etheraddr(struct net_device *dev, char *str)
{
	unsigned char *addr = dev->dev_addr;

	if (str == NULL)
		goto random;

	if (!mac_pton(str, addr)) {
		pr_err("%s: failed to parse '%s' as an ethernet address\n",
		       dev->name, str);
		goto random;
	}
	if (is_multicast_ether_addr(addr)) {
		pr_err("%s: attempt to assign a multicast ethernet address\n",
		       dev->name);
		goto random;
	}
	if (!is_valid_ether_addr(addr)) {
		pr_err("%s: attempt to assign an invalid ethernet address\n",
		       dev->name);
		goto random;
	}
	if (!is_local_ether_addr(addr))
		pr_warn("%s: assigning a globally valid ethernet address\n",
			dev->name);
	return;

random:
	pr_info("%s: choosing a random ethernet address\n",
		dev->name);
	eth_hw_addr_random(dev);
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
/* ======================= TUNTAP TRANSPORT INTERFACE ====================== */

static int tuntap_open(struct iss_net_private *lp)
{
	struct ifreq ifr;
	char *dev_name = lp->tp.info.tuntap.dev_name;
	int err = -EINVAL;
	int fd;

<<<<<<< HEAD
<<<<<<< HEAD
	/* We currently only support a fixed configuration. */

	if (!lp->tp.info.tuntap.fixed_config)
		return -EINVAL;

	if ((fd = simc_open("/dev/net/tun", 02, 0)) < 0) {	/* O_RDWR */
		printk("Failed to open /dev/net/tun, returned %d "
		       "(errno = %d)\n", fd, errno);
		return fd;
	}

	memset(&ifr, 0, sizeof ifr);
	ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
	strlcpy(ifr.ifr_name, dev_name, sizeof ifr.ifr_name);

	if ((err = simc_ioctl(fd, TUNSETIFF, (void*) &ifr)) < 0) {
		printk("Failed to set interface, returned %d "
		       "(errno = %d)\n", err, errno);
=======
=======
>>>>>>> v3.18
	fd = simc_open("/dev/net/tun", 02, 0); /* O_RDWR */
	if (fd < 0) {
		pr_err("%s: failed to open /dev/net/tun, returned %d (errno = %d)\n",
		       lp->dev->name, fd, errno);
		return fd;
	}

	memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
	strlcpy(ifr.ifr_name, dev_name, sizeof(ifr.ifr_name));

	err = simc_ioctl(fd, TUNSETIFF, &ifr);
	if (err < 0) {
		pr_err("%s: failed to set interface %s, returned %d (errno = %d)\n",
		       lp->dev->name, dev_name, err, errno);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		simc_close(fd);
		return err;
	}

	lp->tp.info.tuntap.fd = fd;
	return err;
}

static void tuntap_close(struct iss_net_private *lp)
{
<<<<<<< HEAD
<<<<<<< HEAD
#if 0
	if (lp->tp.info.tuntap.fixed_config)
		iter_addresses(lp->tp.info.tuntap.dev, close_addr, lp->host.dev_name);
#endif
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	simc_close(lp->tp.info.tuntap.fd);
	lp->tp.info.tuntap.fd = -1;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int tuntap_read (struct iss_net_private *lp, struct sk_buff **skb)
{
#if 0
	*skb = ether_adjust_skb(*skb, ETH_HEADER_OTHER);
	if (*skb == NULL)
		return -ENOMEM;
#endif

=======
static int tuntap_read(struct iss_net_private *lp, struct sk_buff **skb)
{
>>>>>>> v3.18
=======
static int tuntap_read(struct iss_net_private *lp, struct sk_buff **skb)
{
>>>>>>> v3.18
	return simc_read(lp->tp.info.tuntap.fd,
			(*skb)->data, (*skb)->dev->mtu + ETH_HEADER_OTHER);
}

<<<<<<< HEAD
<<<<<<< HEAD
static int tuntap_write (struct iss_net_private *lp, struct sk_buff **skb)
=======
static int tuntap_write(struct iss_net_private *lp, struct sk_buff **skb)
>>>>>>> v3.18
=======
static int tuntap_write(struct iss_net_private *lp, struct sk_buff **skb)
>>>>>>> v3.18
{
	return simc_write(lp->tp.info.tuntap.fd, (*skb)->data, (*skb)->len);
}

unsigned short tuntap_protocol(struct sk_buff *skb)
{
	return eth_type_trans(skb, skb->dev);
}

static int tuntap_poll(struct iss_net_private *lp)
{
	return simc_poll(lp->tp.info.tuntap.fd);
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Currently only a device name is supported.
 * ethX=tuntap[,[mac address][,[device name]]]
=======
 * ethX=tuntap,[mac address],device name
>>>>>>> v3.18
=======
 * ethX=tuntap,[mac address],device name
>>>>>>> v3.18
 */

static int tuntap_probe(struct iss_net_private *lp, int index, char *init)
{
<<<<<<< HEAD
<<<<<<< HEAD
	const int len = strlen(TRANSPORT_TUNTAP_NAME);
=======
	struct net_device *dev = lp->dev;
>>>>>>> v3.18
=======
	struct net_device *dev = lp->dev;
>>>>>>> v3.18
	char *dev_name = NULL, *mac_str = NULL, *rem = NULL;

	/* Transport should be 'tuntap': ethX=tuntap,mac,dev_name */

<<<<<<< HEAD
<<<<<<< HEAD
	if (strncmp(init, TRANSPORT_TUNTAP_NAME, len))
		return 0;

	if (*(init += strlen(TRANSPORT_TUNTAP_NAME)) == ',') {
		if ((rem=split_if_spec(init+1, &mac_str, &dev_name)) != NULL) {
			printk("Extra garbage on specification : '%s'\n", rem);
			return 0;
		}
	} else if (*init != '\0') {
		printk("Invalid argument: %s. Skipping device!\n", init);
		return 0;
	}

	if (dev_name) {
		strncpy(lp->tp.info.tuntap.dev_name, dev_name,
			 sizeof lp->tp.info.tuntap.dev_name);
		lp->tp.info.tuntap.fixed_config = 1;
	} else
		strcpy(lp->tp.info.tuntap.dev_name, TRANSPORT_TUNTAP_NAME);


#if 0
	if (setup_etheraddr(mac_str, lp->mac))
		lp->have_mac = 1;
#endif
	lp->mtu = TRANSPORT_TUNTAP_MTU;

	//lp->info.tuntap.gate_addr = gate_addr;
=======
=======
>>>>>>> v3.18
	if (strncmp(init, TRANSPORT_TUNTAP_NAME,
		    sizeof(TRANSPORT_TUNTAP_NAME) - 1))
		return 0;

	init += sizeof(TRANSPORT_TUNTAP_NAME) - 1;
	if (*init == ',') {
		rem = split_if_spec(init + 1, &mac_str, &dev_name);
		if (rem != NULL) {
			pr_err("%s: extra garbage on specification : '%s'\n",
			       dev->name, rem);
			return 0;
		}
	} else if (*init != '\0') {
		pr_err("%s: invalid argument: %s. Skipping device!\n",
		       dev->name, init);
		return 0;
	}

	if (!dev_name) {
		pr_err("%s: missing tuntap device name\n", dev->name);
		return 0;
	}

	strlcpy(lp->tp.info.tuntap.dev_name, dev_name,
		sizeof(lp->tp.info.tuntap.dev_name));

	setup_etheraddr(dev, mac_str);

	lp->mtu = TRANSPORT_TUNTAP_MTU;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	lp->tp.info.tuntap.fd = -1;

	lp->tp.open = tuntap_open;
	lp->tp.close = tuntap_close;
	lp->tp.read = tuntap_read;
	lp->tp.write = tuntap_write;
	lp->tp.protocol = tuntap_protocol;
	lp->tp.poll = tuntap_poll;

<<<<<<< HEAD
<<<<<<< HEAD
	printk("TUN/TAP backend - ");
#if 0
	if (lp->host.gate_addr != NULL)
		printk("IP = %s", lp->host.gate_addr);
#endif
	printk("\n");

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 1;
}

/* ================================ ISS NET ================================ */

static int iss_net_rx(struct net_device *dev)
{
	struct iss_net_private *lp = netdev_priv(dev);
	int pkt_len;
	struct sk_buff *skb;

	/* Check if there is any new data. */

	if (lp->tp.poll(lp) == 0)
		return 0;

	/* Try to allocate memory, if it fails, try again next round. */

<<<<<<< HEAD
<<<<<<< HEAD
	if ((skb = dev_alloc_skb(dev->mtu + 2 + ETH_HEADER_OTHER)) == NULL) {
=======
	skb = dev_alloc_skb(dev->mtu + 2 + ETH_HEADER_OTHER);
	if (skb == NULL) {
>>>>>>> v3.18
=======
	skb = dev_alloc_skb(dev->mtu + 2 + ETH_HEADER_OTHER);
	if (skb == NULL) {
>>>>>>> v3.18
		lp->stats.rx_dropped++;
		return 0;
	}

	skb_reserve(skb, 2);

	/* Setup skb */

	skb->dev = dev;
	skb_reset_mac_header(skb);
	pkt_len = lp->tp.read(lp, &skb);
	skb_put(skb, pkt_len);

	if (pkt_len > 0) {
		skb_trim(skb, pkt_len);
		skb->protocol = lp->tp.protocol(skb);

		lp->stats.rx_bytes += skb->len;
		lp->stats.rx_packets++;
<<<<<<< HEAD
<<<<<<< HEAD
	//	netif_rx(skb);
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
		netif_rx_ni(skb);
		return pkt_len;
	}
	kfree_skb(skb);
	return pkt_len;
}

static int iss_net_poll(void)
{
	struct list_head *ele;
	int err, ret = 0;

	spin_lock(&opened_lock);

	list_for_each(ele, &opened) {
		struct iss_net_private *lp;

		lp = list_entry(ele, struct iss_net_private, opened_list);

		if (!netif_running(lp->dev))
			break;

		spin_lock(&lp->lock);

		while ((err = iss_net_rx(lp->dev)) > 0)
			ret++;

		spin_unlock(&lp->lock);

		if (err < 0) {
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_ERR "Device '%s' read returned %d, "
			       "shutting it down\n", lp->dev->name, err);
			dev_close(lp->dev);
		} else {
			// FIXME reactivate_fd(lp->fd, ISS_ETH_IRQ);
=======
=======
>>>>>>> v3.18
			pr_err("Device '%s' read returned %d, shutting it down\n",
			       lp->dev->name, err);
			dev_close(lp->dev);
		} else {
			/* FIXME reactivate_fd(lp->fd, ISS_ETH_IRQ); */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		}
	}

	spin_unlock(&opened_lock);
	return ret;
}


static void iss_net_timer(unsigned long priv)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct iss_net_private* lp = (struct iss_net_private*) priv;

	spin_lock(&lp->lock);

	iss_net_poll();

	mod_timer(&lp->timer, jiffies + lp->timer_val);

=======
=======
>>>>>>> v3.18
	struct iss_net_private *lp = (struct iss_net_private *)priv;

	spin_lock(&lp->lock);
	iss_net_poll();
	mod_timer(&lp->timer, jiffies + lp->timer_val);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	spin_unlock(&lp->lock);
}


static int iss_net_open(struct net_device *dev)
{
	struct iss_net_private *lp = netdev_priv(dev);
<<<<<<< HEAD
<<<<<<< HEAD
	char addr[sizeof "255.255.255.255\0"];
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	int err;

	spin_lock(&lp->lock);

<<<<<<< HEAD
<<<<<<< HEAD
	if ((err = lp->tp.open(lp)) < 0)
		goto out;

	if (!lp->have_mac) {
		dev_ip_addr(dev, addr, &lp->mac[2]);
		set_ether_mac(dev, lp->mac);
	}

=======
=======
>>>>>>> v3.18
	err = lp->tp.open(lp);
	if (err < 0)
		goto out;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	netif_start_queue(dev);

	/* clear buffer - it can happen that the host side of the interface
	 * is full when we get here. In this case, new data is never queued,
	 * SIGIOs never arrive, and the net never works.
	 */
	while ((err = iss_net_rx(dev)) > 0)
		;

	spin_lock(&opened_lock);
	list_add(&lp->opened_list, &opened);
	spin_unlock(&opened_lock);

	init_timer(&lp->timer);
	lp->timer_val = ISS_NET_TIMER_VALUE;
	lp->timer.data = (unsigned long) lp;
	lp->timer.function = iss_net_timer;
	mod_timer(&lp->timer, jiffies + lp->timer_val);

out:
	spin_unlock(&lp->lock);
	return err;
}

static int iss_net_close(struct net_device *dev)
{
	struct iss_net_private *lp = netdev_priv(dev);
<<<<<<< HEAD
<<<<<<< HEAD
printk("iss_net_close!\n");
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	netif_stop_queue(dev);
	spin_lock(&lp->lock);

	spin_lock(&opened_lock);
	list_del(&opened);
	spin_unlock(&opened_lock);

	del_timer_sync(&lp->timer);

	lp->tp.close(lp);

	spin_unlock(&lp->lock);
	return 0;
}

static int iss_net_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct iss_net_private *lp = netdev_priv(dev);
	unsigned long flags;
	int len;

	netif_stop_queue(dev);
	spin_lock_irqsave(&lp->lock, flags);

	len = lp->tp.write(lp, &skb);

	if (len == skb->len) {
		lp->stats.tx_packets++;
		lp->stats.tx_bytes += skb->len;
		dev->trans_start = jiffies;
		netif_start_queue(dev);

		/* this is normally done in the interrupt when tx finishes */
		netif_wake_queue(dev);

	} else if (len == 0) {
		netif_start_queue(dev);
		lp->stats.tx_dropped++;

	} else {
		netif_start_queue(dev);
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "iss_net_start_xmit: failed(%d)\n", len);
=======
		pr_err("%s: %s failed(%d)\n", dev->name, __func__, len);
>>>>>>> v3.18
=======
		pr_err("%s: %s failed(%d)\n", dev->name, __func__, len);
>>>>>>> v3.18
	}

	spin_unlock_irqrestore(&lp->lock, flags);

	dev_kfree_skb(skb);
	return NETDEV_TX_OK;
}


static struct net_device_stats *iss_net_get_stats(struct net_device *dev)
{
	struct iss_net_private *lp = netdev_priv(dev);
	return &lp->stats;
}

static void iss_net_set_multicast_list(struct net_device *dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
#if 0
	if (dev->flags & IFF_PROMISC)
		return;
	else if (!netdev_mc_empty(dev))
		dev->flags |= IFF_ALLMULTI;
	else
		dev->flags &= ~IFF_ALLMULTI;
#endif
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static void iss_net_tx_timeout(struct net_device *dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
#if 0
	dev->trans_start = jiffies;
	netif_wake_queue(dev);
#endif
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static int iss_net_set_mac(struct net_device *dev, void *addr)
{
<<<<<<< HEAD
<<<<<<< HEAD
#if 0
	struct iss_net_private *lp = netdev_priv(dev);
	struct sockaddr *hwaddr = addr;

	spin_lock(&lp->lock);
	memcpy(dev->dev_addr, hwaddr->sa_data, ETH_ALEN);
	spin_unlock(&lp->lock);
#endif

=======
=======
>>>>>>> v3.18
	struct iss_net_private *lp = netdev_priv(dev);
	struct sockaddr *hwaddr = addr;

	if (!is_valid_ether_addr(hwaddr->sa_data))
		return -EADDRNOTAVAIL;
	spin_lock(&lp->lock);
	memcpy(dev->dev_addr, hwaddr->sa_data, ETH_ALEN);
	spin_unlock(&lp->lock);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
}

static int iss_net_change_mtu(struct net_device *dev, int new_mtu)
{
<<<<<<< HEAD
<<<<<<< HEAD
#if 0
	struct iss_net_private *lp = netdev_priv(dev);
	int err = 0;

	spin_lock(&lp->lock);

	// FIXME not needed new_mtu = transport_set_mtu(new_mtu, &lp->user);

	if (new_mtu < 0)
		err = new_mtu;
	else
		dev->mtu = new_mtu;

	spin_unlock(&lp->lock);
	return err;
#endif
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return -EINVAL;
}

void iss_net_user_timer_expire(unsigned long _conn)
{
}


static struct platform_driver iss_net_driver = {
	.driver = {
		.name  = DRIVER_NAME,
	},
};

static int driver_registered;

static const struct net_device_ops iss_netdev_ops = {
	.ndo_open		= iss_net_open,
	.ndo_stop		= iss_net_close,
	.ndo_get_stats		= iss_net_get_stats,
	.ndo_start_xmit		= iss_net_start_xmit,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_change_mtu		= iss_net_change_mtu,
	.ndo_set_mac_address	= iss_net_set_mac,
<<<<<<< HEAD
<<<<<<< HEAD
	//.ndo_do_ioctl		= iss_net_ioctl,
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	.ndo_tx_timeout		= iss_net_tx_timeout,
	.ndo_set_rx_mode	= iss_net_set_multicast_list,
};

static int iss_net_configure(int index, char *init)
{
	struct net_device *dev;
	struct iss_net_private *lp;
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
	if ((dev = alloc_etherdev(sizeof *lp)) == NULL) {
		printk(KERN_ERR "eth_configure: failed to allocate device\n");
=======
	dev = alloc_etherdev(sizeof(*lp));
	if (dev == NULL) {
		pr_err("eth_configure: failed to allocate device\n");
>>>>>>> v3.18
=======
	dev = alloc_etherdev(sizeof(*lp));
	if (dev == NULL) {
		pr_err("eth_configure: failed to allocate device\n");
>>>>>>> v3.18
		return 1;
	}

	/* Initialize private element. */

	lp = netdev_priv(dev);
<<<<<<< HEAD
<<<<<<< HEAD
	*lp = ((struct iss_net_private) {
=======
	*lp = (struct iss_net_private) {
>>>>>>> v3.18
=======
	*lp = (struct iss_net_private) {
>>>>>>> v3.18
		.device_list		= LIST_HEAD_INIT(lp->device_list),
		.opened_list		= LIST_HEAD_INIT(lp->opened_list),
		.lock			= __SPIN_LOCK_UNLOCKED(lp.lock),
		.dev			= dev,
		.index			= index,
<<<<<<< HEAD
<<<<<<< HEAD
		//.fd                   = -1,
		.mac			= { 0xfe, 0xfd, 0x0, 0x0, 0x0, 0x0 },
		.have_mac		= 0,
		});
=======
=======
>>>>>>> v3.18
		};

	/*
	 * If this name ends up conflicting with an existing registered
	 * netdevice, that is OK, register_netdev{,ice}() will notice this
	 * and fail.
	 */
	snprintf(dev->name, sizeof(dev->name), "eth%d", index);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/*
	 * Try all transport protocols.
	 * Note: more protocols can be added by adding '&& !X_init(lp, eth)'.
	 */

	if (!tuntap_probe(lp, index, init)) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk("Invalid arguments. Skipping device!\n");
		goto errout;
	}

	printk(KERN_INFO "Netdevice %d ", index);
	if (lp->have_mac)
		printk("(%pM) ", lp->mac);
	printk(": ");
=======
=======
>>>>>>> v3.18
		pr_err("%s: invalid arguments. Skipping device!\n",
		       dev->name);
		goto errout;
	}

	pr_info("Netdevice %d (%pM)\n", index, dev->dev_addr);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* sysfs register */

	if (!driver_registered) {
		platform_driver_register(&iss_net_driver);
		driver_registered = 1;
	}

	spin_lock(&devices_lock);
	list_add(&lp->device_list, &devices);
	spin_unlock(&devices_lock);

	lp->pdev.id = index;
	lp->pdev.name = DRIVER_NAME;
	platform_device_register(&lp->pdev);
<<<<<<< HEAD
<<<<<<< HEAD
	SET_NETDEV_DEV(dev,&lp->pdev.dev);

	/*
	 * If this name ends up conflicting with an existing registered
	 * netdevice, that is OK, register_netdev{,ice}() will notice this
	 * and fail.
	 */
	snprintf(dev->name, sizeof dev->name, "eth%d", index);
=======
	SET_NETDEV_DEV(dev, &lp->pdev.dev);
>>>>>>> v3.18
=======
	SET_NETDEV_DEV(dev, &lp->pdev.dev);
>>>>>>> v3.18

	dev->netdev_ops = &iss_netdev_ops;
	dev->mtu = lp->mtu;
	dev->watchdog_timeo = (HZ >> 1);
	dev->irq = -1;

	rtnl_lock();
	err = register_netdevice(dev);
	rtnl_unlock();

	if (err) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk("Error registering net device!\n");
=======
		pr_err("%s: error registering net device!\n", dev->name);
>>>>>>> v3.18
=======
		pr_err("%s: error registering net device!\n", dev->name);
>>>>>>> v3.18
		/* XXX: should we call ->remove() here? */
		free_netdev(dev);
		return 1;
	}

	init_timer(&lp->tl);
	lp->tl.function = iss_net_user_timer_expire;

<<<<<<< HEAD
<<<<<<< HEAD
#if 0
	if (lp->have_mac)
		set_ether_mac(dev, lp->mac);
#endif
	return 0;

errout:
	// FIXME: unregister; free, etc..
	return -EIO;

=======
=======
>>>>>>> v3.18
	return 0;

errout:
	/* FIXME: unregister; free, etc.. */
	return -EIO;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/* ------------------------------------------------------------------------- */

/* Filled in during early boot */

struct list_head eth_cmd_line = LIST_HEAD_INIT(eth_cmd_line);

struct iss_net_init {
	struct list_head list;
	char *init;		/* init string */
	int index;
};

/*
 * Parse the command line and look for 'ethX=...' fields, and register all
 * those fields. They will be later initialized in iss_net_init.
 */

#define ERR KERN_ERR "iss_net_setup: "

<<<<<<< HEAD
<<<<<<< HEAD
static int iss_net_setup(char *str)
=======
static int __init iss_net_setup(char *str)
>>>>>>> v3.18
=======
static int __init iss_net_setup(char *str)
>>>>>>> v3.18
{
	struct iss_net_private *device = NULL;
	struct iss_net_init *new;
	struct list_head *ele;
	char *end;
<<<<<<< HEAD
<<<<<<< HEAD
	int n;

	n = simple_strtoul(str, &end, 0);
	if (end == str) {
		printk(ERR "Failed to parse '%s'\n", str);
		return 1;
	}
	if (n < 0) {
		printk(ERR "Device %d is negative\n", n);
		return 1;
	}
	if (*(str = end) != '=') {
		printk(ERR "Expected '=' after device number\n");
		return 1;
	}
=======
=======
>>>>>>> v3.18
	int rc;
	unsigned n;

	end = strchr(str, '=');
	if (!end) {
		printk(ERR "Expected '=' after device number\n");
		return 1;
	}
	*end = 0;
	rc = kstrtouint(str, 0, &n);
	*end = '=';
	if (rc < 0) {
		printk(ERR "Failed to parse '%s'\n", str);
		return 1;
	}
	str = end;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	spin_lock(&devices_lock);

	list_for_each(ele, &devices) {
		device = list_entry(ele, struct iss_net_private, device_list);
		if (device->index == n)
			break;
	}

	spin_unlock(&devices_lock);

	if (device && device->index == n) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(ERR "Device %d already configured\n", n);
		return 1;
	}

	if ((new = alloc_bootmem(sizeof new)) == NULL) {
		printk("Alloc_bootmem failed\n");
=======
=======
>>>>>>> v3.18
		printk(ERR "Device %u already configured\n", n);
		return 1;
	}

	new = alloc_bootmem(sizeof(*new));
	if (new == NULL) {
		printk(ERR "Alloc_bootmem failed\n");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return 1;
	}

	INIT_LIST_HEAD(&new->list);
	new->index = n;
	new->init = str + 1;

	list_add_tail(&new->list, &eth_cmd_line);
	return 1;
}

#undef ERR

<<<<<<< HEAD
<<<<<<< HEAD
__setup("eth=", iss_net_setup);
=======
__setup("eth", iss_net_setup);
>>>>>>> v3.18
=======
__setup("eth", iss_net_setup);
>>>>>>> v3.18

/*
 * Initialize all ISS Ethernet devices previously registered in iss_net_setup.
 */

static int iss_net_init(void)
{
	struct list_head *ele, *next;

	/* Walk through all Ethernet devices specified in the command line. */

	list_for_each_safe(ele, next, &eth_cmd_line) {
		struct iss_net_init *eth;
		eth = list_entry(ele, struct iss_net_init, list);
		iss_net_configure(eth->index, eth->init);
	}

	return 1;
}

module_init(iss_net_init);

