/*
 * net/tipc/core.c: TIPC module code
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2003-2006, Ericsson AB
 * Copyright (c) 2005-2006, 2010-2011, Wind River Systems
=======
 * Copyright (c) 2003-2006, 2013, Ericsson AB
 * Copyright (c) 2005-2006, 2010-2013, Wind River Systems
>>>>>>> v3.18
=======
 * Copyright (c) 2003-2006, 2013, Ericsson AB
 * Copyright (c) 2005-2006, 2010-2013, Wind River Systems
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
<<<<<<< HEAD
<<<<<<< HEAD
#include "ref.h"
#include "name_table.h"
#include "subscr.h"
#include "config.h"
=======
=======
>>>>>>> v3.18
#include "name_table.h"
#include "subscr.h"
#include "config.h"
#include "socket.h"
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

#include <linux/module.h>

/* global variables used by multiple sub-systems within TIPC */
int tipc_random __read_mostly;

/* configurable TIPC parameters */
u32 tipc_own_addr __read_mostly;
int tipc_max_ports __read_mostly;
int tipc_net_id __read_mostly;
<<<<<<< HEAD
<<<<<<< HEAD
int tipc_remote_management __read_mostly;

=======
int sysctl_tipc_rmem[3] __read_mostly;	/* min/default/max */
>>>>>>> v3.18
=======
int sysctl_tipc_rmem[3] __read_mostly;	/* min/default/max */
>>>>>>> v3.18

/**
 * tipc_buf_acquire - creates a TIPC message buffer
 * @size: message size (including TIPC header)
 *
 * Returns a new buffer with data pointers set to the specified size.
 *
 * NOTE: Headroom is reserved to allow prepending of a data link header.
 *       There may also be unrequested tailroom present at the buffer's end.
 */
struct sk_buff *tipc_buf_acquire(u32 size)
{
	struct sk_buff *skb;
	unsigned int buf_size = (BUF_HEADROOM + size + 3) & ~3u;

	skb = alloc_skb_fclone(buf_size, GFP_ATOMIC);
	if (skb) {
		skb_reserve(skb, BUF_HEADROOM);
		skb_put(skb, size);
		skb->next = NULL;
	}
	return skb;
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * tipc_core_stop_net - shut down TIPC networking sub-systems
 */
static void tipc_core_stop_net(void)
{
	tipc_net_stop();
	tipc_eth_media_stop();
	tipc_ib_media_stop();
}

/**
 * start_net - start TIPC networking sub-systems
 */
int tipc_core_start_net(unsigned long addr)
{
	int res;

	tipc_net_start(addr);
	res = tipc_eth_media_start();
	if (res < 0)
		goto err;
	res = tipc_ib_media_start();
	if (res < 0)
		goto err;
	return res;

err:
	tipc_core_stop_net();
	return res;
}

/**
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
 * tipc_core_stop - switch TIPC from SINGLE NODE to NOT RUNNING mode
 */
static void tipc_core_stop(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	tipc_netlink_stop();
	tipc_handler_stop();
	tipc_cfg_stop();
	tipc_subscr_stop();
	tipc_nametbl_stop();
	tipc_ref_table_stop();
	tipc_socket_stop();
=======
=======
>>>>>>> v3.18
	tipc_net_stop();
	tipc_bearer_cleanup();
	tipc_netlink_stop();
	tipc_subscr_stop();
	tipc_nametbl_stop();
	tipc_sk_ref_table_stop();
	tipc_socket_stop();
	tipc_unregister_sysctl();
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/**
 * tipc_core_start - switch TIPC from NOT RUNNING to SINGLE NODE mode
 */
static int tipc_core_start(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int res;

	get_random_bytes(&tipc_random, sizeof(tipc_random));

	res = tipc_handler_start();
	if (!res)
		res = tipc_ref_table_init(tipc_max_ports, tipc_random);
	if (!res)
		res = tipc_nametbl_init();
	if (!res)
		res = tipc_subscr_start();
	if (!res)
		res = tipc_cfg_init();
	if (!res)
		res = tipc_netlink_start();
	if (!res)
		res = tipc_socket_init();
	if (res)
		tipc_core_stop();

	return res;
}

=======
=======
>>>>>>> v3.18
	int err;

	get_random_bytes(&tipc_random, sizeof(tipc_random));

	err = tipc_sk_ref_table_init(tipc_max_ports, tipc_random);
	if (err)
		goto out_reftbl;

	err = tipc_nametbl_init();
	if (err)
		goto out_nametbl;

	err = tipc_netlink_start();
	if (err)
		goto out_netlink;

	err = tipc_socket_init();
	if (err)
		goto out_socket;

	err = tipc_register_sysctl();
	if (err)
		goto out_sysctl;

	err = tipc_subscr_start();
	if (err)
		goto out_subscr;

	err = tipc_bearer_setup();
	if (err)
		goto out_bearer;

	return 0;
out_bearer:
	tipc_subscr_stop();
out_subscr:
	tipc_unregister_sysctl();
out_sysctl:
	tipc_socket_stop();
out_socket:
	tipc_netlink_stop();
out_netlink:
	tipc_nametbl_stop();
out_nametbl:
	tipc_sk_ref_table_stop();
out_reftbl:
	return err;
}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

static int __init tipc_init(void)
{
	int res;

	pr_info("Activated (version " TIPC_MOD_VER ")\n");

	tipc_own_addr = 0;
<<<<<<< HEAD
<<<<<<< HEAD
	tipc_remote_management = 1;
	tipc_max_ports = CONFIG_TIPC_PORTS;
	tipc_net_id = 4711;

=======
=======
>>>>>>> v3.18
	tipc_max_ports = CONFIG_TIPC_PORTS;
	tipc_net_id = 4711;

	sysctl_tipc_rmem[0] = TIPC_CONN_OVERLOAD_LIMIT >> 4 <<
			      TIPC_LOW_IMPORTANCE;
	sysctl_tipc_rmem[1] = TIPC_CONN_OVERLOAD_LIMIT >> 4 <<
			      TIPC_CRITICAL_IMPORTANCE;
	sysctl_tipc_rmem[2] = TIPC_CONN_OVERLOAD_LIMIT;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	res = tipc_core_start();
	if (res)
		pr_err("Unable to start in single node mode\n");
	else
		pr_info("Started in single node mode\n");
	return res;
}

static void __exit tipc_exit(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	tipc_core_stop_net();
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	tipc_core_stop();
	pr_info("Deactivated\n");
}

module_init(tipc_init);
module_exit(tipc_exit);

MODULE_DESCRIPTION("TIPC: Transparent Inter Process Communication");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION(TIPC_MOD_VER);
