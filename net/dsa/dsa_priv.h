/*
 * net/dsa/dsa_priv.h - Hardware switch handling
 * Copyright (c) 2008-2009 Marvell Semiconductor
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __DSA_PRIV_H
#define __DSA_PRIV_H

#include <linux/phy.h>
<<<<<<< HEAD
#include <net/dsa.h>
=======
#include <linux/netdevice.h>

struct dsa_device_ops {
	netdev_tx_t (*xmit)(struct sk_buff *skb, struct net_device *dev);
	int (*rcv)(struct sk_buff *skb, struct net_device *dev,
		   struct packet_type *pt, struct net_device *orig_dev);
};
>>>>>>> v3.18

struct dsa_slave_priv {
	/*
	 * The linux network interface corresponding to this
	 * switch port.
	 */
	struct net_device	*dev;
<<<<<<< HEAD
=======
	netdev_tx_t		(*xmit)(struct sk_buff *skb,
					struct net_device *dev);
>>>>>>> v3.18

	/*
	 * Which switch this port is a part of, and the port index
	 * for this port.
	 */
	struct dsa_switch	*parent;
	u8			port;

	/*
	 * The phylib phy_device pointer for the PHY connected
	 * to this port.
	 */
	struct phy_device	*phy;
<<<<<<< HEAD
=======
	phy_interface_t		phy_interface;
	int			old_link;
	int			old_pause;
	int			old_duplex;
>>>>>>> v3.18
};

/* dsa.c */
extern char dsa_driver_version[];

/* slave.c */
<<<<<<< HEAD
=======
extern const struct dsa_device_ops notag_netdev_ops;
>>>>>>> v3.18
void dsa_slave_mii_bus_init(struct dsa_switch *ds);
struct net_device *dsa_slave_create(struct dsa_switch *ds,
				    struct device *parent,
				    int port, char *name);
<<<<<<< HEAD

/* tag_dsa.c */
netdev_tx_t dsa_xmit(struct sk_buff *skb, struct net_device *dev);
extern struct packet_type dsa_packet_type;

/* tag_edsa.c */
netdev_tx_t edsa_xmit(struct sk_buff *skb, struct net_device *dev);
extern struct packet_type edsa_packet_type;

/* tag_trailer.c */
netdev_tx_t trailer_xmit(struct sk_buff *skb, struct net_device *dev);
extern struct packet_type trailer_packet_type;
=======
int dsa_slave_suspend(struct net_device *slave_dev);
int dsa_slave_resume(struct net_device *slave_dev);

/* tag_dsa.c */
extern const struct dsa_device_ops dsa_netdev_ops;

/* tag_edsa.c */
extern const struct dsa_device_ops edsa_netdev_ops;

/* tag_trailer.c */
extern const struct dsa_device_ops trailer_netdev_ops;

/* tag_brcm.c */
extern const struct dsa_device_ops brcm_netdev_ops;
>>>>>>> v3.18


#endif
