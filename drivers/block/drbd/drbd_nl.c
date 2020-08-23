/*
   drbd_nl.c

   This file is part of DRBD by Philipp Reisner and Lars Ellenberg.

   Copyright (C) 2001-2008, LINBIT Information Technologies GmbH.
   Copyright (C) 1999-2008, Philipp Reisner <philipp.reisner@linbit.com>.
   Copyright (C) 2002-2008, Lars Ellenberg <lars.ellenberg@linbit.com>.

   drbd is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   drbd is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with drbd; see the file COPYING.  If not, write to
   the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

 */

<<<<<<< HEAD
<<<<<<< HEAD
=======
#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

>>>>>>> v3.18
=======
#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

>>>>>>> v3.18
#include <linux/module.h>
#include <linux/drbd.h>
#include <linux/in.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/slab.h>
#include <linux/blkpg.h>
#include <linux/cpumask.h>
#include "drbd_int.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "drbd_req.h"
#include "drbd_wrappers.h"
=======
#include "drbd_protocol.h"
#include "drbd_req.h"
>>>>>>> v3.18
=======
#include "drbd_protocol.h"
#include "drbd_req.h"
>>>>>>> v3.18
#include <asm/unaligned.h>
#include <linux/drbd_limits.h>
#include <linux/kthread.h>

#include <net/genetlink.h>

/* .doit */
// int drbd_adm_create_resource(struct sk_buff *skb, struct genl_info *info);
// int drbd_adm_delete_resource(struct sk_buff *skb, struct genl_info *info);

<<<<<<< HEAD
<<<<<<< HEAD
int drbd_adm_add_minor(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_delete_minor(struct sk_buff *skb, struct genl_info *info);
=======
int drbd_adm_new_minor(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_del_minor(struct sk_buff *skb, struct genl_info *info);
>>>>>>> v3.18
=======
int drbd_adm_new_minor(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_del_minor(struct sk_buff *skb, struct genl_info *info);
>>>>>>> v3.18

int drbd_adm_new_resource(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_del_resource(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_down(struct sk_buff *skb, struct genl_info *info);

int drbd_adm_set_role(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_attach(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_disk_opts(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_detach(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_connect(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_net_opts(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_resize(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_start_ov(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_new_c_uuid(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_disconnect(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_invalidate(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_invalidate_peer(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_pause_sync(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_resume_sync(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_suspend_io(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_resume_io(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_outdate(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_resource_opts(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_get_status(struct sk_buff *skb, struct genl_info *info);
int drbd_adm_get_timeout_type(struct sk_buff *skb, struct genl_info *info);
/* .dumpit */
int drbd_adm_get_status_all(struct sk_buff *skb, struct netlink_callback *cb);

#include <linux/drbd_genl_api.h>
#include "drbd_nla.h"
#include <linux/genl_magic_func.h>

/* used blkdev_get_by_path, to claim our meta data device(s) */
static char *drbd_m_holder = "Hands off! this is DRBD's meta data device.";

<<<<<<< HEAD
<<<<<<< HEAD
/* Configuration is strictly serialized, because generic netlink message
 * processing is strictly serialized by the genl_lock().
 * Which means we can use one static global drbd_config_context struct.
 */
static struct drbd_config_context {
	/* assigned from drbd_genlmsghdr */
	unsigned int minor;
	/* assigned from request attributes, if present */
	unsigned int volume;
#define VOLUME_UNSPECIFIED		(-1U)
	/* pointer into the request skb,
	 * limited lifetime! */
	char *resource_name;
	struct nlattr *my_addr;
	struct nlattr *peer_addr;

	/* reply buffer */
	struct sk_buff *reply_skb;
	/* pointer into reply buffer */
	struct drbd_genlmsghdr *reply_dh;
	/* resolved from attributes, if possible */
	struct drbd_conf *mdev;
	struct drbd_tconn *tconn;
} adm_ctx;

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
static void drbd_adm_send_reply(struct sk_buff *skb, struct genl_info *info)
{
	genlmsg_end(skb, genlmsg_data(nlmsg_data(nlmsg_hdr(skb))));
	if (genlmsg_reply(skb, info))
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "drbd: error sending genl reply\n");
=======
		pr_err("error sending genl reply\n");
>>>>>>> v3.18
=======
		pr_err("error sending genl reply\n");
>>>>>>> v3.18
}

/* Used on a fresh "drbd_adm_prepare"d reply_skb, this cannot fail: The only
 * reason it could fail was no space in skb, and there are 4k available. */
<<<<<<< HEAD
<<<<<<< HEAD
int drbd_msg_put_info(const char *info)
{
	struct sk_buff *skb = adm_ctx.reply_skb;
=======
int drbd_msg_put_info(struct sk_buff *skb, const char *info)
{
>>>>>>> v3.18
=======
int drbd_msg_put_info(struct sk_buff *skb, const char *info)
{
>>>>>>> v3.18
	struct nlattr *nla;
	int err = -EMSGSIZE;

	if (!info || !info[0])
		return 0;

	nla = nla_nest_start(skb, DRBD_NLA_CFG_REPLY);
	if (!nla)
		return err;

	err = nla_put_string(skb, T_info_text, info);
	if (err) {
		nla_nest_cancel(skb, nla);
		return err;
	} else
		nla_nest_end(skb, nla);
	return 0;
}

/* This would be a good candidate for a "pre_doit" hook,
 * and per-family private info->pointers.
 * But we need to stay compatible with older kernels.
 * If it returns successfully, adm_ctx members are valid.
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
 *
 * At this point, we still rely on the global genl_lock().
 * If we want to avoid that, and allow "genl_family.parallel_ops", we may need
 * to add additional synchronization against object destruction/modification.
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 */
#define DRBD_ADM_NEED_MINOR	1
#define DRBD_ADM_NEED_RESOURCE	2
#define DRBD_ADM_NEED_CONNECTION 4
<<<<<<< HEAD
<<<<<<< HEAD
static int drbd_adm_prepare(struct sk_buff *skb, struct genl_info *info,
		unsigned flags)
=======
static int drbd_adm_prepare(struct drbd_config_context *adm_ctx,
	struct sk_buff *skb, struct genl_info *info, unsigned flags)
>>>>>>> v3.18
=======
static int drbd_adm_prepare(struct drbd_config_context *adm_ctx,
	struct sk_buff *skb, struct genl_info *info, unsigned flags)
>>>>>>> v3.18
{
	struct drbd_genlmsghdr *d_in = info->userhdr;
	const u8 cmd = info->genlhdr->cmd;
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
	memset(&adm_ctx, 0, sizeof(adm_ctx));
=======
	memset(adm_ctx, 0, sizeof(*adm_ctx));
>>>>>>> v3.18
=======
	memset(adm_ctx, 0, sizeof(*adm_ctx));
>>>>>>> v3.18

	/* genl_rcv_msg only checks for CAP_NET_ADMIN on "GENL_ADMIN_PERM" :( */
	if (cmd != DRBD_ADM_GET_STATUS && !capable(CAP_NET_ADMIN))
	       return -EPERM;

<<<<<<< HEAD
<<<<<<< HEAD
	adm_ctx.reply_skb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!adm_ctx.reply_skb) {
=======
	adm_ctx->reply_skb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!adm_ctx->reply_skb) {
>>>>>>> v3.18
=======
	adm_ctx->reply_skb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!adm_ctx->reply_skb) {
>>>>>>> v3.18
		err = -ENOMEM;
		goto fail;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	adm_ctx.reply_dh = genlmsg_put_reply(adm_ctx.reply_skb,
					info, &drbd_genl_family, 0, cmd);
	/* put of a few bytes into a fresh skb of >= 4k will always succeed.
	 * but anyways */
	if (!adm_ctx.reply_dh) {
=======
=======
>>>>>>> v3.18
	adm_ctx->reply_dh = genlmsg_put_reply(adm_ctx->reply_skb,
					info, &drbd_genl_family, 0, cmd);
	/* put of a few bytes into a fresh skb of >= 4k will always succeed.
	 * but anyways */
	if (!adm_ctx->reply_dh) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		err = -ENOMEM;
		goto fail;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	adm_ctx.reply_dh->minor = d_in->minor;
	adm_ctx.reply_dh->ret_code = NO_ERROR;

	adm_ctx.volume = VOLUME_UNSPECIFIED;
=======
=======
>>>>>>> v3.18
	adm_ctx->reply_dh->minor = d_in->minor;
	adm_ctx->reply_dh->ret_code = NO_ERROR;

	adm_ctx->volume = VOLUME_UNSPECIFIED;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (info->attrs[DRBD_NLA_CFG_CONTEXT]) {
		struct nlattr *nla;
		/* parse and validate only */
		err = drbd_cfg_context_from_attrs(NULL, info);
		if (err)
			goto fail;

		/* It was present, and valid,
		 * copy it over to the reply skb. */
<<<<<<< HEAD
<<<<<<< HEAD
		err = nla_put_nohdr(adm_ctx.reply_skb,
=======
		err = nla_put_nohdr(adm_ctx->reply_skb,
>>>>>>> v3.18
=======
		err = nla_put_nohdr(adm_ctx->reply_skb,
>>>>>>> v3.18
				info->attrs[DRBD_NLA_CFG_CONTEXT]->nla_len,
				info->attrs[DRBD_NLA_CFG_CONTEXT]);
		if (err)
			goto fail;

<<<<<<< HEAD
<<<<<<< HEAD
		/* and assign stuff to the global adm_ctx */
		nla = nested_attr_tb[__nla_type(T_ctx_volume)];
		if (nla)
			adm_ctx.volume = nla_get_u32(nla);
		nla = nested_attr_tb[__nla_type(T_ctx_resource_name)];
		if (nla)
			adm_ctx.resource_name = nla_data(nla);
		adm_ctx.my_addr = nested_attr_tb[__nla_type(T_ctx_my_addr)];
		adm_ctx.peer_addr = nested_attr_tb[__nla_type(T_ctx_peer_addr)];
		if ((adm_ctx.my_addr &&
		     nla_len(adm_ctx.my_addr) > sizeof(adm_ctx.tconn->my_addr)) ||
		    (adm_ctx.peer_addr &&
		     nla_len(adm_ctx.peer_addr) > sizeof(adm_ctx.tconn->peer_addr))) {
=======
=======
>>>>>>> v3.18
		/* and assign stuff to the adm_ctx */
		nla = nested_attr_tb[__nla_type(T_ctx_volume)];
		if (nla)
			adm_ctx->volume = nla_get_u32(nla);
		nla = nested_attr_tb[__nla_type(T_ctx_resource_name)];
		if (nla)
			adm_ctx->resource_name = nla_data(nla);
		adm_ctx->my_addr = nested_attr_tb[__nla_type(T_ctx_my_addr)];
		adm_ctx->peer_addr = nested_attr_tb[__nla_type(T_ctx_peer_addr)];
		if ((adm_ctx->my_addr &&
		     nla_len(adm_ctx->my_addr) > sizeof(adm_ctx->connection->my_addr)) ||
		    (adm_ctx->peer_addr &&
		     nla_len(adm_ctx->peer_addr) > sizeof(adm_ctx->connection->peer_addr))) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			err = -EINVAL;
			goto fail;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	adm_ctx.minor = d_in->minor;
	adm_ctx.mdev = minor_to_mdev(d_in->minor);
	adm_ctx.tconn = conn_get_by_name(adm_ctx.resource_name);

	if (!adm_ctx.mdev && (flags & DRBD_ADM_NEED_MINOR)) {
		drbd_msg_put_info("unknown minor");
		return ERR_MINOR_INVALID;
	}
	if (!adm_ctx.tconn && (flags & DRBD_ADM_NEED_RESOURCE)) {
		drbd_msg_put_info("unknown resource");
=======
=======
>>>>>>> v3.18
	adm_ctx->minor = d_in->minor;
	adm_ctx->device = minor_to_device(d_in->minor);

	/* We are protected by the global genl_lock().
	 * But we may explicitly drop it/retake it in drbd_adm_set_role(),
	 * so make sure this object stays around. */
	if (adm_ctx->device)
		kref_get(&adm_ctx->device->kref);

	if (adm_ctx->resource_name) {
		adm_ctx->resource = drbd_find_resource(adm_ctx->resource_name);
	}

	if (!adm_ctx->device && (flags & DRBD_ADM_NEED_MINOR)) {
		drbd_msg_put_info(adm_ctx->reply_skb, "unknown minor");
		return ERR_MINOR_INVALID;
	}
	if (!adm_ctx->resource && (flags & DRBD_ADM_NEED_RESOURCE)) {
		drbd_msg_put_info(adm_ctx->reply_skb, "unknown resource");
		if (adm_ctx->resource_name)
			return ERR_RES_NOT_KNOWN;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return ERR_INVALID_REQUEST;
	}

	if (flags & DRBD_ADM_NEED_CONNECTION) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (adm_ctx.tconn && !(flags & DRBD_ADM_NEED_RESOURCE)) {
			drbd_msg_put_info("no resource name expected");
			return ERR_INVALID_REQUEST;
		}
		if (adm_ctx.mdev) {
			drbd_msg_put_info("no minor number expected");
			return ERR_INVALID_REQUEST;
		}
		if (adm_ctx.my_addr && adm_ctx.peer_addr)
			adm_ctx.tconn = conn_get_by_addrs(nla_data(adm_ctx.my_addr),
							  nla_len(adm_ctx.my_addr),
							  nla_data(adm_ctx.peer_addr),
							  nla_len(adm_ctx.peer_addr));
		if (!adm_ctx.tconn) {
			drbd_msg_put_info("unknown connection");
=======
=======
>>>>>>> v3.18
		if (adm_ctx->resource) {
			drbd_msg_put_info(adm_ctx->reply_skb, "no resource name expected");
			return ERR_INVALID_REQUEST;
		}
		if (adm_ctx->device) {
			drbd_msg_put_info(adm_ctx->reply_skb, "no minor number expected");
			return ERR_INVALID_REQUEST;
		}
		if (adm_ctx->my_addr && adm_ctx->peer_addr)
			adm_ctx->connection = conn_get_by_addrs(nla_data(adm_ctx->my_addr),
							  nla_len(adm_ctx->my_addr),
							  nla_data(adm_ctx->peer_addr),
							  nla_len(adm_ctx->peer_addr));
		if (!adm_ctx->connection) {
			drbd_msg_put_info(adm_ctx->reply_skb, "unknown connection");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			return ERR_INVALID_REQUEST;
		}
	}

	/* some more paranoia, if the request was over-determined */
<<<<<<< HEAD
<<<<<<< HEAD
	if (adm_ctx.mdev && adm_ctx.tconn &&
	    adm_ctx.mdev->tconn != adm_ctx.tconn) {
		pr_warning("request: minor=%u, resource=%s; but that minor belongs to connection %s\n",
				adm_ctx.minor, adm_ctx.resource_name,
				adm_ctx.mdev->tconn->name);
		drbd_msg_put_info("minor exists in different resource");
		return ERR_INVALID_REQUEST;
	}
	if (adm_ctx.mdev &&
	    adm_ctx.volume != VOLUME_UNSPECIFIED &&
	    adm_ctx.volume != adm_ctx.mdev->vnr) {
		pr_warning("request: minor=%u, volume=%u; but that minor is volume %u in %s\n",
				adm_ctx.minor, adm_ctx.volume,
				adm_ctx.mdev->vnr, adm_ctx.mdev->tconn->name);
		drbd_msg_put_info("minor exists as different volume");
		return ERR_INVALID_REQUEST;
	}

	return NO_ERROR;

fail:
	nlmsg_free(adm_ctx.reply_skb);
	adm_ctx.reply_skb = NULL;
	return err;
}

static int drbd_adm_finish(struct genl_info *info, int retcode)
{
	if (adm_ctx.tconn) {
		kref_put(&adm_ctx.tconn->kref, &conn_destroy);
		adm_ctx.tconn = NULL;
	}

	if (!adm_ctx.reply_skb)
		return -ENOMEM;

	adm_ctx.reply_dh->ret_code = retcode;
	drbd_adm_send_reply(adm_ctx.reply_skb, info);
	return 0;
}

static void setup_khelper_env(struct drbd_tconn *tconn, char **envp)
=======
=======
>>>>>>> v3.18
	if (adm_ctx->device && adm_ctx->resource &&
	    adm_ctx->device->resource != adm_ctx->resource) {
		pr_warning("request: minor=%u, resource=%s; but that minor belongs to resource %s\n",
				adm_ctx->minor, adm_ctx->resource->name,
				adm_ctx->device->resource->name);
		drbd_msg_put_info(adm_ctx->reply_skb, "minor exists in different resource");
		return ERR_INVALID_REQUEST;
	}
	if (adm_ctx->device &&
	    adm_ctx->volume != VOLUME_UNSPECIFIED &&
	    adm_ctx->volume != adm_ctx->device->vnr) {
		pr_warning("request: minor=%u, volume=%u; but that minor is volume %u in %s\n",
				adm_ctx->minor, adm_ctx->volume,
				adm_ctx->device->vnr,
				adm_ctx->device->resource->name);
		drbd_msg_put_info(adm_ctx->reply_skb, "minor exists as different volume");
		return ERR_INVALID_REQUEST;
	}

	/* still, provide adm_ctx->resource always, if possible. */
	if (!adm_ctx->resource) {
		adm_ctx->resource = adm_ctx->device ? adm_ctx->device->resource
			: adm_ctx->connection ? adm_ctx->connection->resource : NULL;
		if (adm_ctx->resource)
			kref_get(&adm_ctx->resource->kref);
	}

	return NO_ERROR;

fail:
	nlmsg_free(adm_ctx->reply_skb);
	adm_ctx->reply_skb = NULL;
	return err;
}

static int drbd_adm_finish(struct drbd_config_context *adm_ctx,
	struct genl_info *info, int retcode)
{
	if (adm_ctx->device) {
		kref_put(&adm_ctx->device->kref, drbd_destroy_device);
		adm_ctx->device = NULL;
	}
	if (adm_ctx->connection) {
		kref_put(&adm_ctx->connection->kref, &drbd_destroy_connection);
		adm_ctx->connection = NULL;
	}
	if (adm_ctx->resource) {
		kref_put(&adm_ctx->resource->kref, drbd_destroy_resource);
		adm_ctx->resource = NULL;
	}

	if (!adm_ctx->reply_skb)
		return -ENOMEM;

	adm_ctx->reply_dh->ret_code = retcode;
	drbd_adm_send_reply(adm_ctx->reply_skb, info);
	return 0;
}

static void setup_khelper_env(struct drbd_connection *connection, char **envp)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	char *afs;

	/* FIXME: A future version will not allow this case. */
<<<<<<< HEAD
<<<<<<< HEAD
	if (tconn->my_addr_len == 0 || tconn->peer_addr_len == 0)
		return;

	switch (((struct sockaddr *)&tconn->peer_addr)->sa_family) {
	case AF_INET6:
		afs = "ipv6";
		snprintf(envp[4], 60, "DRBD_PEER_ADDRESS=%pI6",
			 &((struct sockaddr_in6 *)&tconn->peer_addr)->sin6_addr);
=======
=======
>>>>>>> v3.18
	if (connection->my_addr_len == 0 || connection->peer_addr_len == 0)
		return;

	switch (((struct sockaddr *)&connection->peer_addr)->sa_family) {
	case AF_INET6:
		afs = "ipv6";
		snprintf(envp[4], 60, "DRBD_PEER_ADDRESS=%pI6",
			 &((struct sockaddr_in6 *)&connection->peer_addr)->sin6_addr);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		break;
	case AF_INET:
		afs = "ipv4";
		snprintf(envp[4], 60, "DRBD_PEER_ADDRESS=%pI4",
<<<<<<< HEAD
<<<<<<< HEAD
			 &((struct sockaddr_in *)&tconn->peer_addr)->sin_addr);
=======
			 &((struct sockaddr_in *)&connection->peer_addr)->sin_addr);
>>>>>>> v3.18
=======
			 &((struct sockaddr_in *)&connection->peer_addr)->sin_addr);
>>>>>>> v3.18
		break;
	default:
		afs = "ssocks";
		snprintf(envp[4], 60, "DRBD_PEER_ADDRESS=%pI4",
<<<<<<< HEAD
<<<<<<< HEAD
			 &((struct sockaddr_in *)&tconn->peer_addr)->sin_addr);
=======
			 &((struct sockaddr_in *)&connection->peer_addr)->sin_addr);
>>>>>>> v3.18
=======
			 &((struct sockaddr_in *)&connection->peer_addr)->sin_addr);
>>>>>>> v3.18
	}
	snprintf(envp[3], 20, "DRBD_PEER_AF=%s", afs);
}

<<<<<<< HEAD
<<<<<<< HEAD
int drbd_khelper(struct drbd_conf *mdev, char *cmd)
=======
int drbd_khelper(struct drbd_device *device, char *cmd)
>>>>>>> v3.18
=======
int drbd_khelper(struct drbd_device *device, char *cmd)
>>>>>>> v3.18
{
	char *envp[] = { "HOME=/",
			"TERM=linux",
			"PATH=/sbin:/usr/sbin:/bin:/usr/bin",
			 (char[20]) { }, /* address family */
			 (char[60]) { }, /* address */
			NULL };
	char mb[12];
	char *argv[] = {usermode_helper, cmd, mb, NULL };
<<<<<<< HEAD
<<<<<<< HEAD
	struct drbd_tconn *tconn = mdev->tconn;
	struct sib_info sib;
	int ret;

	if (current == tconn->worker.task)
		set_bit(CALLBACK_PENDING, &tconn->flags);

	snprintf(mb, 12, "minor-%d", mdev_to_minor(mdev));
	setup_khelper_env(tconn, envp);

	/* The helper may take some time.
	 * write out any unsynced meta data changes now */
	drbd_md_sync(mdev);

	dev_info(DEV, "helper command: %s %s %s\n", usermode_helper, cmd, mb);
	sib.sib_reason = SIB_HELPER_PRE;
	sib.helper_name = cmd;
	drbd_bcast_event(mdev, &sib);
	ret = call_usermodehelper(usermode_helper, argv, envp, UMH_WAIT_PROC);
	if (ret)
		dev_warn(DEV, "helper command: %s %s %s exit code %u (0x%x)\n",
				usermode_helper, cmd, mb,
				(ret >> 8) & 0xff, ret);
	else
		dev_info(DEV, "helper command: %s %s %s exit code %u (0x%x)\n",
=======
=======
>>>>>>> v3.18
	struct drbd_connection *connection = first_peer_device(device)->connection;
	struct sib_info sib;
	int ret;

	if (current == connection->worker.task)
		set_bit(CALLBACK_PENDING, &connection->flags);

	snprintf(mb, 12, "minor-%d", device_to_minor(device));
	setup_khelper_env(connection, envp);

	/* The helper may take some time.
	 * write out any unsynced meta data changes now */
	drbd_md_sync(device);

	drbd_info(device, "helper command: %s %s %s\n", usermode_helper, cmd, mb);
	sib.sib_reason = SIB_HELPER_PRE;
	sib.helper_name = cmd;
	drbd_bcast_event(device, &sib);
	ret = call_usermodehelper(usermode_helper, argv, envp, UMH_WAIT_PROC);
	if (ret)
		drbd_warn(device, "helper command: %s %s %s exit code %u (0x%x)\n",
				usermode_helper, cmd, mb,
				(ret >> 8) & 0xff, ret);
	else
		drbd_info(device, "helper command: %s %s %s exit code %u (0x%x)\n",
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
				usermode_helper, cmd, mb,
				(ret >> 8) & 0xff, ret);
	sib.sib_reason = SIB_HELPER_POST;
	sib.helper_exit_code = ret;
<<<<<<< HEAD
<<<<<<< HEAD
	drbd_bcast_event(mdev, &sib);

	if (current == tconn->worker.task)
		clear_bit(CALLBACK_PENDING, &tconn->flags);
=======
=======
>>>>>>> v3.18
	drbd_bcast_event(device, &sib);

	if (current == connection->worker.task)
		clear_bit(CALLBACK_PENDING, &connection->flags);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (ret < 0) /* Ignore any ERRNOs we got. */
		ret = 0;

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
int conn_khelper(struct drbd_tconn *tconn, char *cmd)
=======
static int conn_khelper(struct drbd_connection *connection, char *cmd)
>>>>>>> v3.18
=======
static int conn_khelper(struct drbd_connection *connection, char *cmd)
>>>>>>> v3.18
{
	char *envp[] = { "HOME=/",
			"TERM=linux",
			"PATH=/sbin:/usr/sbin:/bin:/usr/bin",
			 (char[20]) { }, /* address family */
			 (char[60]) { }, /* address */
			NULL };
<<<<<<< HEAD
<<<<<<< HEAD
	char *argv[] = {usermode_helper, cmd, tconn->name, NULL };
	int ret;

	setup_khelper_env(tconn, envp);
	conn_md_sync(tconn);

	conn_info(tconn, "helper command: %s %s %s\n", usermode_helper, cmd, tconn->name);
=======
=======
>>>>>>> v3.18
	char *resource_name = connection->resource->name;
	char *argv[] = {usermode_helper, cmd, resource_name, NULL };
	int ret;

	setup_khelper_env(connection, envp);
	conn_md_sync(connection);

	drbd_info(connection, "helper command: %s %s %s\n", usermode_helper, cmd, resource_name);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	/* TODO: conn_bcast_event() ?? */

	ret = call_usermodehelper(usermode_helper, argv, envp, UMH_WAIT_PROC);
	if (ret)
<<<<<<< HEAD
<<<<<<< HEAD
		conn_warn(tconn, "helper command: %s %s %s exit code %u (0x%x)\n",
			  usermode_helper, cmd, tconn->name,
			  (ret >> 8) & 0xff, ret);
	else
		conn_info(tconn, "helper command: %s %s %s exit code %u (0x%x)\n",
			  usermode_helper, cmd, tconn->name,
=======
=======
>>>>>>> v3.18
		drbd_warn(connection, "helper command: %s %s %s exit code %u (0x%x)\n",
			  usermode_helper, cmd, resource_name,
			  (ret >> 8) & 0xff, ret);
	else
		drbd_info(connection, "helper command: %s %s %s exit code %u (0x%x)\n",
			  usermode_helper, cmd, resource_name,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			  (ret >> 8) & 0xff, ret);
	/* TODO: conn_bcast_event() ?? */

	if (ret < 0) /* Ignore any ERRNOs we got. */
		ret = 0;

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static enum drbd_fencing_p highest_fencing_policy(struct drbd_tconn *tconn)
{
	enum drbd_fencing_p fp = FP_NOT_AVAIL;
	struct drbd_conf *mdev;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&tconn->volumes, mdev, vnr) {
		if (get_ldev_if_state(mdev, D_CONSISTENT)) {
			fp = max_t(enum drbd_fencing_p, fp,
				   rcu_dereference(mdev->ldev->disk_conf)->fencing);
			put_ldev(mdev);
=======
=======
>>>>>>> v3.18
static enum drbd_fencing_p highest_fencing_policy(struct drbd_connection *connection)
{
	enum drbd_fencing_p fp = FP_NOT_AVAIL;
	struct drbd_peer_device *peer_device;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&connection->peer_devices, peer_device, vnr) {
		struct drbd_device *device = peer_device->device;
		if (get_ldev_if_state(device, D_CONSISTENT)) {
			struct disk_conf *disk_conf =
				rcu_dereference(peer_device->device->ldev->disk_conf);
			fp = max_t(enum drbd_fencing_p, fp, disk_conf->fencing);
			put_ldev(device);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		}
	}
	rcu_read_unlock();

<<<<<<< HEAD
<<<<<<< HEAD
	return fp;
}

bool conn_try_outdate_peer(struct drbd_tconn *tconn)
{
=======
=======
>>>>>>> v3.18
	if (fp == FP_NOT_AVAIL) {
		/* IO Suspending works on the whole resource.
		   Do it only for one device. */
		vnr = 0;
		peer_device = idr_get_next(&connection->peer_devices, &vnr);
		drbd_change_state(peer_device->device, CS_VERBOSE | CS_HARD, NS(susp_fen, 0));
	}

	return fp;
}

bool conn_try_outdate_peer(struct drbd_connection *connection)
{
	unsigned int connect_cnt;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	union drbd_state mask = { };
	union drbd_state val = { };
	enum drbd_fencing_p fp;
	char *ex_to_string;
	int r;

<<<<<<< HEAD
<<<<<<< HEAD
	if (tconn->cstate >= C_WF_REPORT_PARAMS) {
		conn_err(tconn, "Expected cstate < C_WF_REPORT_PARAMS\n");
		return false;
	}

	fp = highest_fencing_policy(tconn);
	switch (fp) {
	case FP_NOT_AVAIL:
		conn_warn(tconn, "Not fencing peer, I'm not even Consistent myself.\n");
=======
=======
>>>>>>> v3.18
	spin_lock_irq(&connection->resource->req_lock);
	if (connection->cstate >= C_WF_REPORT_PARAMS) {
		drbd_err(connection, "Expected cstate < C_WF_REPORT_PARAMS\n");
		spin_unlock_irq(&connection->resource->req_lock);
		return false;
	}

	connect_cnt = connection->connect_cnt;
	spin_unlock_irq(&connection->resource->req_lock);

	fp = highest_fencing_policy(connection);
	switch (fp) {
	case FP_NOT_AVAIL:
		drbd_warn(connection, "Not fencing peer, I'm not even Consistent myself.\n");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		goto out;
	case FP_DONT_CARE:
		return true;
	default: ;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	r = conn_khelper(tconn, "fence-peer");
=======
	r = conn_khelper(connection, "fence-peer");
>>>>>>> v3.18
=======
	r = conn_khelper(connection, "fence-peer");
>>>>>>> v3.18

	switch ((r>>8) & 0xff) {
	case 3: /* peer is inconsistent */
		ex_to_string = "peer is inconsistent or worse";
		mask.pdsk = D_MASK;
		val.pdsk = D_INCONSISTENT;
		break;
	case 4: /* peer got outdated, or was already outdated */
		ex_to_string = "peer was fenced";
		mask.pdsk = D_MASK;
		val.pdsk = D_OUTDATED;
		break;
	case 5: /* peer was down */
<<<<<<< HEAD
<<<<<<< HEAD
		if (conn_highest_disk(tconn) == D_UP_TO_DATE) {
=======
		if (conn_highest_disk(connection) == D_UP_TO_DATE) {
>>>>>>> v3.18
=======
		if (conn_highest_disk(connection) == D_UP_TO_DATE) {
>>>>>>> v3.18
			/* we will(have) create(d) a new UUID anyways... */
			ex_to_string = "peer is unreachable, assumed to be dead";
			mask.pdsk = D_MASK;
			val.pdsk = D_OUTDATED;
		} else {
			ex_to_string = "peer unreachable, doing nothing since disk != UpToDate";
		}
		break;
	case 6: /* Peer is primary, voluntarily outdate myself.
		 * This is useful when an unconnected R_SECONDARY is asked to
		 * become R_PRIMARY, but finds the other peer being active. */
		ex_to_string = "peer is active";
<<<<<<< HEAD
<<<<<<< HEAD
		conn_warn(tconn, "Peer is primary, outdating myself.\n");
=======
		drbd_warn(connection, "Peer is primary, outdating myself.\n");
>>>>>>> v3.18
=======
		drbd_warn(connection, "Peer is primary, outdating myself.\n");
>>>>>>> v3.18
		mask.disk = D_MASK;
		val.disk = D_OUTDATED;
		break;
	case 7:
		if (fp != FP_STONITH)
<<<<<<< HEAD
<<<<<<< HEAD
			conn_err(tconn, "fence-peer() = 7 && fencing != Stonith !!!\n");
=======
			drbd_err(connection, "fence-peer() = 7 && fencing != Stonith !!!\n");
>>>>>>> v3.18
=======
			drbd_err(connection, "fence-peer() = 7 && fencing != Stonith !!!\n");
>>>>>>> v3.18
		ex_to_string = "peer was stonithed";
		mask.pdsk = D_MASK;
		val.pdsk = D_OUTDATED;
		break;
	default:
		/* The script is broken ... */
<<<<<<< HEAD
<<<<<<< HEAD
		conn_err(tconn, "fence-peer helper broken, returned %d\n", (r>>8)&0xff);
		return false; /* Eventually leave IO frozen */
	}

	conn_info(tconn, "fence-peer helper returned %d (%s)\n",
=======
=======
>>>>>>> v3.18
		drbd_err(connection, "fence-peer helper broken, returned %d\n", (r>>8)&0xff);
		return false; /* Eventually leave IO frozen */
	}

	drbd_info(connection, "fence-peer helper returned %d (%s)\n",
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		  (r>>8) & 0xff, ex_to_string);

 out:

	/* Not using
<<<<<<< HEAD
<<<<<<< HEAD
	   conn_request_state(tconn, mask, val, CS_VERBOSE);
	   here, because we might were able to re-establish the connection in the
	   meantime. */
	spin_lock_irq(&tconn->req_lock);
	if (tconn->cstate < C_WF_REPORT_PARAMS && !test_bit(STATE_SENT, &tconn->flags))
		_conn_request_state(tconn, mask, val, CS_VERBOSE);
	spin_unlock_irq(&tconn->req_lock);

	return conn_highest_pdsk(tconn) <= D_OUTDATED;
=======
=======
>>>>>>> v3.18
	   conn_request_state(connection, mask, val, CS_VERBOSE);
	   here, because we might were able to re-establish the connection in the
	   meantime. */
	spin_lock_irq(&connection->resource->req_lock);
	if (connection->cstate < C_WF_REPORT_PARAMS && !test_bit(STATE_SENT, &connection->flags)) {
		if (connection->connect_cnt != connect_cnt)
			/* In case the connection was established and droped
			   while the fence-peer handler was running, ignore it */
			drbd_info(connection, "Ignoring fence-peer exit code\n");
		else
			_conn_request_state(connection, mask, val, CS_VERBOSE);
	}
	spin_unlock_irq(&connection->resource->req_lock);

	return conn_highest_pdsk(connection) <= D_OUTDATED;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static int _try_outdate_peer_async(void *data)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct drbd_tconn *tconn = (struct drbd_tconn *)data;

	conn_try_outdate_peer(tconn);

	kref_put(&tconn->kref, &conn_destroy);
	return 0;
}

void conn_try_outdate_peer_async(struct drbd_tconn *tconn)
{
	struct task_struct *opa;

	kref_get(&tconn->kref);
=======
=======
>>>>>>> v3.18
	struct drbd_connection *connection = (struct drbd_connection *)data;

	conn_try_outdate_peer(connection);

	kref_put(&connection->kref, drbd_destroy_connection);
	return 0;
}

void conn_try_outdate_peer_async(struct drbd_connection *connection)
{
	struct task_struct *opa;

	kref_get(&connection->kref);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	/* We may just have force_sig()'ed this thread
	 * to get it out of some blocking network function.
	 * Clear signals; otherwise kthread_run(), which internally uses
	 * wait_on_completion_killable(), will mistake our pending signal
	 * for a new fatal signal and fail. */
	flush_signals(current);
<<<<<<< HEAD
<<<<<<< HEAD
	opa = kthread_run(_try_outdate_peer_async, tconn, "drbd_async_h");
	if (IS_ERR(opa)) {
		conn_err(tconn, "out of mem, failed to invoke fence-peer helper\n");
		kref_put(&tconn->kref, &conn_destroy);
=======
=======
>>>>>>> v3.18
	opa = kthread_run(_try_outdate_peer_async, connection, "drbd_async_h");
	if (IS_ERR(opa)) {
		drbd_err(connection, "out of mem, failed to invoke fence-peer helper\n");
		kref_put(&connection->kref, drbd_destroy_connection);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}
}

enum drbd_state_rv
<<<<<<< HEAD
<<<<<<< HEAD
drbd_set_role(struct drbd_conf *mdev, enum drbd_role new_role, int force)
{
=======
=======
>>>>>>> v3.18
drbd_set_role(struct drbd_device *const device, enum drbd_role new_role, int force)
{
	struct drbd_peer_device *const peer_device = first_peer_device(device);
	struct drbd_connection *const connection = peer_device ? peer_device->connection : NULL;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	const int max_tries = 4;
	enum drbd_state_rv rv = SS_UNKNOWN_ERROR;
	struct net_conf *nc;
	int try = 0;
	int forced = 0;
	union drbd_state mask, val;

<<<<<<< HEAD
<<<<<<< HEAD
	if (new_role == R_PRIMARY)
		request_ping(mdev->tconn); /* Detect a dead peer ASAP */

	mutex_lock(mdev->state_mutex);
=======
=======
>>>>>>> v3.18
	if (new_role == R_PRIMARY) {
		struct drbd_connection *connection;

		/* Detect dead peers as soon as possible.  */

		rcu_read_lock();
		for_each_connection(connection, device->resource)
			request_ping(connection);
		rcu_read_unlock();
	}

	mutex_lock(device->state_mutex);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	mask.i = 0; mask.role = R_MASK;
	val.i  = 0; val.role  = new_role;

	while (try++ < max_tries) {
<<<<<<< HEAD
<<<<<<< HEAD
		rv = _drbd_request_state(mdev, mask, val, CS_WAIT_COMPLETE);
=======
		rv = _drbd_request_state(device, mask, val, CS_WAIT_COMPLETE);
>>>>>>> v3.18
=======
		rv = _drbd_request_state(device, mask, val, CS_WAIT_COMPLETE);
>>>>>>> v3.18

		/* in case we first succeeded to outdate,
		 * but now suddenly could establish a connection */
		if (rv == SS_CW_FAILED_BY_PEER && mask.pdsk != 0) {
			val.pdsk = 0;
			mask.pdsk = 0;
			continue;
		}

		if (rv == SS_NO_UP_TO_DATE_DISK && force &&
<<<<<<< HEAD
<<<<<<< HEAD
		    (mdev->state.disk < D_UP_TO_DATE &&
		     mdev->state.disk >= D_INCONSISTENT)) {
=======
		    (device->state.disk < D_UP_TO_DATE &&
		     device->state.disk >= D_INCONSISTENT)) {
>>>>>>> v3.18
=======
		    (device->state.disk < D_UP_TO_DATE &&
		     device->state.disk >= D_INCONSISTENT)) {
>>>>>>> v3.18
			mask.disk = D_MASK;
			val.disk  = D_UP_TO_DATE;
			forced = 1;
			continue;
		}

		if (rv == SS_NO_UP_TO_DATE_DISK &&
<<<<<<< HEAD
<<<<<<< HEAD
		    mdev->state.disk == D_CONSISTENT && mask.pdsk == 0) {
			D_ASSERT(mdev->state.pdsk == D_UNKNOWN);

			if (conn_try_outdate_peer(mdev->tconn)) {
=======
=======
>>>>>>> v3.18
		    device->state.disk == D_CONSISTENT && mask.pdsk == 0) {
			D_ASSERT(device, device->state.pdsk == D_UNKNOWN);

			if (conn_try_outdate_peer(connection)) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
				val.disk = D_UP_TO_DATE;
				mask.disk = D_MASK;
			}
			continue;
		}

		if (rv == SS_NOTHING_TO_DO)
			goto out;
		if (rv == SS_PRIMARY_NOP && mask.pdsk == 0) {
<<<<<<< HEAD
<<<<<<< HEAD
			if (!conn_try_outdate_peer(mdev->tconn) && force) {
				dev_warn(DEV, "Forced into split brain situation!\n");
=======
			if (!conn_try_outdate_peer(connection) && force) {
				drbd_warn(device, "Forced into split brain situation!\n");
>>>>>>> v3.18
=======
			if (!conn_try_outdate_peer(connection) && force) {
				drbd_warn(device, "Forced into split brain situation!\n");
>>>>>>> v3.18
				mask.pdsk = D_MASK;
				val.pdsk  = D_OUTDATED;

			}
			continue;
		}
		if (rv == SS_TWO_PRIMARIES) {
			/* Maybe the peer is detected as dead very soon...
			   retry at most once more in this case. */
			int timeo;
			rcu_read_lock();
<<<<<<< HEAD
<<<<<<< HEAD
			nc = rcu_dereference(mdev->tconn->net_conf);
=======
			nc = rcu_dereference(connection->net_conf);
>>>>>>> v3.18
=======
			nc = rcu_dereference(connection->net_conf);
>>>>>>> v3.18
			timeo = nc ? (nc->ping_timeo + 1) * HZ / 10 : 1;
			rcu_read_unlock();
			schedule_timeout_interruptible(timeo);
			if (try < max_tries)
				try = max_tries - 1;
			continue;
		}
		if (rv < SS_SUCCESS) {
<<<<<<< HEAD
<<<<<<< HEAD
			rv = _drbd_request_state(mdev, mask, val,
=======
			rv = _drbd_request_state(device, mask, val,
>>>>>>> v3.18
=======
			rv = _drbd_request_state(device, mask, val,
>>>>>>> v3.18
						CS_VERBOSE + CS_WAIT_COMPLETE);
			if (rv < SS_SUCCESS)
				goto out;
		}
		break;
	}

	if (rv < SS_SUCCESS)
		goto out;

	if (forced)
<<<<<<< HEAD
<<<<<<< HEAD
		dev_warn(DEV, "Forced to consider local data as UpToDate!\n");

	/* Wait until nothing is on the fly :) */
	wait_event(mdev->misc_wait, atomic_read(&mdev->ap_pending_cnt) == 0);
=======
=======
>>>>>>> v3.18
		drbd_warn(device, "Forced to consider local data as UpToDate!\n");

	/* Wait until nothing is on the fly :) */
	wait_event(device->misc_wait, atomic_read(&device->ap_pending_cnt) == 0);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* FIXME also wait for all pending P_BARRIER_ACK? */

	if (new_role == R_SECONDARY) {
<<<<<<< HEAD
<<<<<<< HEAD
		set_disk_ro(mdev->vdisk, true);
		if (get_ldev(mdev)) {
			mdev->ldev->md.uuid[UI_CURRENT] &= ~(u64)1;
			put_ldev(mdev);
		}
	} else {
		mutex_lock(&mdev->tconn->conf_update);
		nc = mdev->tconn->net_conf;
		if (nc)
			nc->discard_my_data = 0; /* without copy; single bit op is atomic */
		mutex_unlock(&mdev->tconn->conf_update);

		set_disk_ro(mdev->vdisk, false);
		if (get_ldev(mdev)) {
			if (((mdev->state.conn < C_CONNECTED ||
			       mdev->state.pdsk <= D_FAILED)
			      && mdev->ldev->md.uuid[UI_BITMAP] == 0) || forced)
				drbd_uuid_new_current(mdev);

			mdev->ldev->md.uuid[UI_CURRENT] |=  (u64)1;
			put_ldev(mdev);
=======
=======
>>>>>>> v3.18
		if (get_ldev(device)) {
			device->ldev->md.uuid[UI_CURRENT] &= ~(u64)1;
			put_ldev(device);
		}
	} else {
		mutex_lock(&device->resource->conf_update);
		nc = connection->net_conf;
		if (nc)
			nc->discard_my_data = 0; /* without copy; single bit op is atomic */
		mutex_unlock(&device->resource->conf_update);

		if (get_ldev(device)) {
			if (((device->state.conn < C_CONNECTED ||
			       device->state.pdsk <= D_FAILED)
			      && device->ldev->md.uuid[UI_BITMAP] == 0) || forced)
				drbd_uuid_new_current(device);

			device->ldev->md.uuid[UI_CURRENT] |=  (u64)1;
			put_ldev(device);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		}
	}

	/* writeout of activity log covered areas of the bitmap
	 * to stable storage done in after state change already */

<<<<<<< HEAD
<<<<<<< HEAD
	if (mdev->state.conn >= C_WF_REPORT_PARAMS) {
		/* if this was forced, we should consider sync */
		if (forced)
			drbd_send_uuids(mdev);
		drbd_send_current_state(mdev);
	}

	drbd_md_sync(mdev);

	kobject_uevent(&disk_to_dev(mdev->vdisk)->kobj, KOBJ_CHANGE);
out:
	mutex_unlock(mdev->state_mutex);
=======
=======
>>>>>>> v3.18
	if (device->state.conn >= C_WF_REPORT_PARAMS) {
		/* if this was forced, we should consider sync */
		if (forced)
			drbd_send_uuids(peer_device);
		drbd_send_current_state(peer_device);
	}

	drbd_md_sync(device);
	set_disk_ro(device->vdisk, new_role == R_SECONDARY);
	kobject_uevent(&disk_to_dev(device->vdisk)->kobj, KOBJ_CHANGE);
out:
	mutex_unlock(device->state_mutex);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return rv;
}

static const char *from_attrs_err_to_txt(int err)
{
	return	err == -ENOMSG ? "required attribute missing" :
		err == -EOPNOTSUPP ? "unknown mandatory attribute" :
		err == -EEXIST ? "can not change invariant setting" :
		"invalid attribute value";
}

int drbd_adm_set_role(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct drbd_config_context adm_ctx;
>>>>>>> v3.18
=======
	struct drbd_config_context adm_ctx;
>>>>>>> v3.18
	struct set_role_parms parms;
	int err;
	enum drbd_ret_code retcode;

<<<<<<< HEAD
<<<<<<< HEAD
	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
=======
	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
>>>>>>> v3.18
=======
	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
>>>>>>> v3.18
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	memset(&parms, 0, sizeof(parms));
	if (info->attrs[DRBD_NLA_SET_ROLE_PARMS]) {
		err = set_role_parms_from_attrs(&parms, info);
		if (err) {
			retcode = ERR_MANDATORY_TAG;
<<<<<<< HEAD
<<<<<<< HEAD
			drbd_msg_put_info(from_attrs_err_to_txt(err));
			goto out;
		}
	}

	if (info->genlhdr->cmd == DRBD_ADM_PRIMARY)
		retcode = drbd_set_role(adm_ctx.mdev, R_PRIMARY, parms.assume_uptodate);
	else
		retcode = drbd_set_role(adm_ctx.mdev, R_SECONDARY, 0);
out:
	drbd_adm_finish(info, retcode);
=======
=======
>>>>>>> v3.18
			drbd_msg_put_info(adm_ctx.reply_skb, from_attrs_err_to_txt(err));
			goto out;
		}
	}
	genl_unlock();
	mutex_lock(&adm_ctx.resource->adm_mutex);

	if (info->genlhdr->cmd == DRBD_ADM_PRIMARY)
		retcode = drbd_set_role(adm_ctx.device, R_PRIMARY, parms.assume_uptodate);
	else
		retcode = drbd_set_role(adm_ctx.device, R_SECONDARY, 0);

	mutex_unlock(&adm_ctx.resource->adm_mutex);
	genl_lock();
out:
	drbd_adm_finish(&adm_ctx, info, retcode);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
}

/* Initializes the md.*_offset members, so we are able to find
 * the on disk meta data.
 *
 * We currently have two possible layouts:
 * external:
 *   |----------- md_size_sect ------------------|
 *   [ 4k superblock ][ activity log ][  Bitmap  ]
 *   | al_offset == 8 |
 *   | bm_offset = al_offset + X      |
 *  ==> bitmap sectors = md_size_sect - bm_offset
 *
 * internal:
 *            |----------- md_size_sect ------------------|
 * [data.....][  Bitmap  ][ activity log ][ 4k superblock ]
 *                        | al_offset < 0 |
 *            | bm_offset = al_offset - Y |
 *  ==> bitmap sectors = Y = al_offset - bm_offset
 *
 *  Activity log size used to be fixed 32kB,
 *  but is about to become configurable.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void drbd_md_set_sector_offsets(struct drbd_conf *mdev,
=======
static void drbd_md_set_sector_offsets(struct drbd_device *device,
>>>>>>> v3.18
=======
static void drbd_md_set_sector_offsets(struct drbd_device *device,
>>>>>>> v3.18
				       struct drbd_backing_dev *bdev)
{
	sector_t md_size_sect = 0;
	unsigned int al_size_sect = bdev->md.al_size_4k * 8;

	bdev->md.md_offset = drbd_md_ss(bdev);

	switch (bdev->md.meta_dev_idx) {
	default:
		/* v07 style fixed size indexed meta data */
		bdev->md.md_size_sect = MD_128MB_SECT;
		bdev->md.al_offset = MD_4kB_SECT;
		bdev->md.bm_offset = MD_4kB_SECT + al_size_sect;
		break;
	case DRBD_MD_INDEX_FLEX_EXT:
		/* just occupy the full device; unit: sectors */
		bdev->md.md_size_sect = drbd_get_capacity(bdev->md_bdev);
		bdev->md.al_offset = MD_4kB_SECT;
		bdev->md.bm_offset = MD_4kB_SECT + al_size_sect;
		break;
	case DRBD_MD_INDEX_INTERNAL:
	case DRBD_MD_INDEX_FLEX_INT:
		/* al size is still fixed */
		bdev->md.al_offset = -al_size_sect;
		/* we need (slightly less than) ~ this much bitmap sectors: */
		md_size_sect = drbd_get_capacity(bdev->backing_bdev);
		md_size_sect = ALIGN(md_size_sect, BM_SECT_PER_EXT);
		md_size_sect = BM_SECT_TO_EXT(md_size_sect);
		md_size_sect = ALIGN(md_size_sect, 8);

		/* plus the "drbd meta data super block",
		 * and the activity log; */
		md_size_sect += MD_4kB_SECT + al_size_sect;

		bdev->md.md_size_sect = md_size_sect;
		/* bitmap offset is adjusted by 'super' block size */
		bdev->md.bm_offset   = -md_size_sect + MD_4kB_SECT;
		break;
	}
}

/* input size is expected to be in KB */
char *ppsize(char *buf, unsigned long long size)
{
	/* Needs 9 bytes at max including trailing NUL:
	 * -1ULL ==> "16384 EB" */
	static char units[] = { 'K', 'M', 'G', 'T', 'P', 'E' };
	int base = 0;
	while (size >= 10000 && base < sizeof(units)-1) {
		/* shift + round */
		size = (size >> 10) + !!(size & (1<<9));
		base++;
	}
	sprintf(buf, "%u %cB", (unsigned)size, units[base]);

	return buf;
}

/* there is still a theoretical deadlock when called from receiver
 * on an D_INCONSISTENT R_PRIMARY:
 *  remote READ does inc_ap_bio, receiver would need to receive answer
 *  packet from remote to dec_ap_bio again.
 *  receiver receive_sizes(), comes here,
 *  waits for ap_bio_cnt == 0. -> deadlock.
 * but this cannot happen, actually, because:
 *  R_PRIMARY D_INCONSISTENT, and peer's disk is unreachable
 *  (not connected, or bad/no disk on peer):
 *  see drbd_fail_request_early, ap_bio_cnt is zero.
 *  R_PRIMARY D_INCONSISTENT, and C_SYNC_TARGET:
 *  peer may not initiate a resize.
 */
/* Note these are not to be confused with
 * drbd_adm_suspend_io/drbd_adm_resume_io,
 * which are (sub) state changes triggered by admin (drbdsetup),
 * and can be long lived.
<<<<<<< HEAD
<<<<<<< HEAD
 * This changes an mdev->flag, is triggered by drbd internals,
 * and should be short-lived. */
void drbd_suspend_io(struct drbd_conf *mdev)
{
	set_bit(SUSPEND_IO, &mdev->flags);
	if (drbd_suspended(mdev))
		return;
	wait_event(mdev->misc_wait, !atomic_read(&mdev->ap_bio_cnt));
}

void drbd_resume_io(struct drbd_conf *mdev)
{
	clear_bit(SUSPEND_IO, &mdev->flags);
	wake_up(&mdev->misc_wait);
=======
=======
>>>>>>> v3.18
 * This changes an device->flag, is triggered by drbd internals,
 * and should be short-lived. */
void drbd_suspend_io(struct drbd_device *device)
{
	set_bit(SUSPEND_IO, &device->flags);
	if (drbd_suspended(device))
		return;
	wait_event(device->misc_wait, !atomic_read(&device->ap_bio_cnt));
}

void drbd_resume_io(struct drbd_device *device)
{
	clear_bit(SUSPEND_IO, &device->flags);
	wake_up(&device->misc_wait);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/**
 * drbd_determine_dev_size() -  Sets the right device size obeying all constraints
<<<<<<< HEAD
<<<<<<< HEAD
 * @mdev:	DRBD device.
=======
 * @device:	DRBD device.
>>>>>>> v3.18
=======
 * @device:	DRBD device.
>>>>>>> v3.18
 *
 * Returns 0 on success, negative return values indicate errors.
 * You should call drbd_md_sync() after calling this function.
 */
<<<<<<< HEAD
<<<<<<< HEAD
enum determine_dev_size drbd_determine_dev_size(struct drbd_conf *mdev, enum dds_flags flags) __must_hold(local)
{
	sector_t prev_first_sect, prev_size; /* previous meta location */
	sector_t la_size_sect, u_size;
	sector_t size;
	char ppb[10];

	int md_moved, la_size_changed;
	enum determine_dev_size rv = unchanged;
=======
=======
>>>>>>> v3.18
enum determine_dev_size
drbd_determine_dev_size(struct drbd_device *device, enum dds_flags flags, struct resize_parms *rs) __must_hold(local)
{
	sector_t prev_first_sect, prev_size; /* previous meta location */
	sector_t la_size_sect, u_size;
	struct drbd_md *md = &device->ldev->md;
	u32 prev_al_stripe_size_4k;
	u32 prev_al_stripes;
	sector_t size;
	char ppb[10];
	void *buffer;

	int md_moved, la_size_changed;
	enum determine_dev_size rv = DS_UNCHANGED;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* race:
	 * application request passes inc_ap_bio,
	 * but then cannot get an AL-reference.
	 * this function later may wait on ap_bio_cnt == 0. -> deadlock.
	 *
	 * to avoid that:
	 * Suspend IO right here.
	 * still lock the act_log to not trigger ASSERTs there.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	drbd_suspend_io(mdev);

	/* no wait necessary anymore, actually we could assert that */
	wait_event(mdev->al_wait, lc_try_lock(mdev->act_log));

	prev_first_sect = drbd_md_first_sector(mdev->ldev);
	prev_size = mdev->ldev->md.md_size_sect;
	la_size_sect = mdev->ldev->md.la_size_sect;

	/* TODO: should only be some assert here, not (re)init... */
	drbd_md_set_sector_offsets(mdev, mdev->ldev);

	rcu_read_lock();
	u_size = rcu_dereference(mdev->ldev->disk_conf)->disk_size;
	rcu_read_unlock();
	size = drbd_new_dev_size(mdev, mdev->ldev, u_size, flags & DDSF_FORCED);

	if (drbd_get_capacity(mdev->this_bdev) != size ||
	    drbd_bm_capacity(mdev) != size) {
		int err;
		err = drbd_bm_resize(mdev, size, !(flags & DDSF_NO_RESYNC));
		if (unlikely(err)) {
			/* currently there is only one error: ENOMEM! */
			size = drbd_bm_capacity(mdev)>>1;
			if (size == 0) {
				dev_err(DEV, "OUT OF MEMORY! "
				    "Could not allocate bitmap!\n");
			} else {
				dev_err(DEV, "BM resizing failed. "
				    "Leaving size unchanged at size = %lu KB\n",
				    (unsigned long)size);
			}
			rv = dev_size_error;
		}
		/* racy, see comments above. */
		drbd_set_my_capacity(mdev, size);
		mdev->ldev->md.la_size_sect = size;
		dev_info(DEV, "size = %s (%llu KB)\n", ppsize(ppb, size>>1),
		     (unsigned long long)size>>1);
	}
	if (rv == dev_size_error)
		goto out;

	la_size_changed = (la_size_sect != mdev->ldev->md.la_size_sect);

	md_moved = prev_first_sect != drbd_md_first_sector(mdev->ldev)
		|| prev_size	   != mdev->ldev->md.md_size_sect;

	if (la_size_changed || md_moved) {
		int err;

		drbd_al_shrink(mdev); /* All extents inactive. */
		dev_info(DEV, "Writing the whole bitmap, %s\n",
			 la_size_changed && md_moved ? "size changed and md moved" :
			 la_size_changed ? "size changed" : "md moved");
		/* next line implicitly does drbd_suspend_io()+drbd_resume_io() */
		err = drbd_bitmap_io(mdev, md_moved ? &drbd_bm_write_all : &drbd_bm_write,
				     "size changed", BM_LOCKED_MASK);
		if (err) {
			rv = dev_size_error;
			goto out;
		}
		drbd_md_mark_dirty(mdev);
	}

	if (size > la_size_sect)
		rv = grew;
	if (size < la_size_sect)
		rv = shrunk;
out:
	lc_unlock(mdev->act_log);
	wake_up(&mdev->al_wait);
	drbd_resume_io(mdev);
=======
=======
>>>>>>> v3.18
	drbd_suspend_io(device);
	buffer = drbd_md_get_buffer(device, __func__); /* Lock meta-data IO */
	if (!buffer) {
		drbd_resume_io(device);
		return DS_ERROR;
	}

	/* no wait necessary anymore, actually we could assert that */
	wait_event(device->al_wait, lc_try_lock(device->act_log));

	prev_first_sect = drbd_md_first_sector(device->ldev);
	prev_size = device->ldev->md.md_size_sect;
	la_size_sect = device->ldev->md.la_size_sect;

	if (rs) {
		/* rs is non NULL if we should change the AL layout only */

		prev_al_stripes = md->al_stripes;
		prev_al_stripe_size_4k = md->al_stripe_size_4k;

		md->al_stripes = rs->al_stripes;
		md->al_stripe_size_4k = rs->al_stripe_size / 4;
		md->al_size_4k = (u64)rs->al_stripes * rs->al_stripe_size / 4;
	}

	drbd_md_set_sector_offsets(device, device->ldev);

	rcu_read_lock();
	u_size = rcu_dereference(device->ldev->disk_conf)->disk_size;
	rcu_read_unlock();
	size = drbd_new_dev_size(device, device->ldev, u_size, flags & DDSF_FORCED);

	if (size < la_size_sect) {
		if (rs && u_size == 0) {
			/* Remove "rs &&" later. This check should always be active, but
			   right now the receiver expects the permissive behavior */
			drbd_warn(device, "Implicit shrink not allowed. "
				 "Use --size=%llus for explicit shrink.\n",
				 (unsigned long long)size);
			rv = DS_ERROR_SHRINK;
		}
		if (u_size > size)
			rv = DS_ERROR_SPACE_MD;
		if (rv != DS_UNCHANGED)
			goto err_out;
	}

	if (drbd_get_capacity(device->this_bdev) != size ||
	    drbd_bm_capacity(device) != size) {
		int err;
		err = drbd_bm_resize(device, size, !(flags & DDSF_NO_RESYNC));
		if (unlikely(err)) {
			/* currently there is only one error: ENOMEM! */
			size = drbd_bm_capacity(device)>>1;
			if (size == 0) {
				drbd_err(device, "OUT OF MEMORY! "
				    "Could not allocate bitmap!\n");
			} else {
				drbd_err(device, "BM resizing failed. "
				    "Leaving size unchanged at size = %lu KB\n",
				    (unsigned long)size);
			}
			rv = DS_ERROR;
		}
		/* racy, see comments above. */
		drbd_set_my_capacity(device, size);
		device->ldev->md.la_size_sect = size;
		drbd_info(device, "size = %s (%llu KB)\n", ppsize(ppb, size>>1),
		     (unsigned long long)size>>1);
	}
	if (rv <= DS_ERROR)
		goto err_out;

	la_size_changed = (la_size_sect != device->ldev->md.la_size_sect);

	md_moved = prev_first_sect != drbd_md_first_sector(device->ldev)
		|| prev_size	   != device->ldev->md.md_size_sect;

	if (la_size_changed || md_moved || rs) {
		u32 prev_flags;

		/* We do some synchronous IO below, which may take some time.
		 * Clear the timer, to avoid scary "timer expired!" messages,
		 * "Superblock" is written out at least twice below, anyways. */
		del_timer(&device->md_sync_timer);
		drbd_al_shrink(device); /* All extents inactive. */

		prev_flags = md->flags;
		md->flags &= ~MDF_PRIMARY_IND;
		drbd_md_write(device, buffer);

		drbd_info(device, "Writing the whole bitmap, %s\n",
			 la_size_changed && md_moved ? "size changed and md moved" :
			 la_size_changed ? "size changed" : "md moved");
		/* next line implicitly does drbd_suspend_io()+drbd_resume_io() */
		drbd_bitmap_io(device, md_moved ? &drbd_bm_write_all : &drbd_bm_write,
			       "size changed", BM_LOCKED_MASK);
		drbd_initialize_al(device, buffer);

		md->flags = prev_flags;
		drbd_md_write(device, buffer);

		if (rs)
			drbd_info(device, "Changed AL layout to al-stripes = %d, al-stripe-size-kB = %d\n",
				  md->al_stripes, md->al_stripe_size_4k * 4);
	}

	if (size > la_size_sect)
		rv = la_size_sect ? DS_GREW : DS_GREW_FROM_ZERO;
	if (size < la_size_sect)
		rv = DS_SHRUNK;

	if (0) {
	err_out:
		if (rs) {
			md->al_stripes = prev_al_stripes;
			md->al_stripe_size_4k = prev_al_stripe_size_4k;
			md->al_size_4k = (u64)prev_al_stripes * prev_al_stripe_size_4k;

			drbd_md_set_sector_offsets(device, device->ldev);
		}
	}
	lc_unlock(device->act_log);
	wake_up(&device->al_wait);
	drbd_md_put_buffer(device);
	drbd_resume_io(device);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return rv;
}

sector_t
<<<<<<< HEAD
<<<<<<< HEAD
drbd_new_dev_size(struct drbd_conf *mdev, struct drbd_backing_dev *bdev,
		  sector_t u_size, int assume_peer_has_space)
{
	sector_t p_size = mdev->p_size;   /* partner's disk size. */
=======
=======
>>>>>>> v3.18
drbd_new_dev_size(struct drbd_device *device, struct drbd_backing_dev *bdev,
		  sector_t u_size, int assume_peer_has_space)
{
	sector_t p_size = device->p_size;   /* partner's disk size. */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	sector_t la_size_sect = bdev->md.la_size_sect; /* last agreed size. */
	sector_t m_size; /* my size */
	sector_t size = 0;

	m_size = drbd_get_max_capacity(bdev);

<<<<<<< HEAD
<<<<<<< HEAD
	if (mdev->state.conn < C_CONNECTED && assume_peer_has_space) {
		dev_warn(DEV, "Resize while not connected was forced by the user!\n");
=======
	if (device->state.conn < C_CONNECTED && assume_peer_has_space) {
		drbd_warn(device, "Resize while not connected was forced by the user!\n");
>>>>>>> v3.18
=======
	if (device->state.conn < C_CONNECTED && assume_peer_has_space) {
		drbd_warn(device, "Resize while not connected was forced by the user!\n");
>>>>>>> v3.18
		p_size = m_size;
	}

	if (p_size && m_size) {
		size = min_t(sector_t, p_size, m_size);
	} else {
		if (la_size_sect) {
			size = la_size_sect;
			if (m_size && m_size < size)
				size = m_size;
			if (p_size && p_size < size)
				size = p_size;
		} else {
			if (m_size)
				size = m_size;
			if (p_size)
				size = p_size;
		}
	}

	if (size == 0)
<<<<<<< HEAD
<<<<<<< HEAD
		dev_err(DEV, "Both nodes diskless!\n");

	if (u_size) {
		if (u_size > size)
			dev_err(DEV, "Requested disk size is too big (%lu > %lu)\n",
=======
=======
>>>>>>> v3.18
		drbd_err(device, "Both nodes diskless!\n");

	if (u_size) {
		if (u_size > size)
			drbd_err(device, "Requested disk size is too big (%lu > %lu)\n",
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			    (unsigned long)u_size>>1, (unsigned long)size>>1);
		else
			size = u_size;
	}

	return size;
}

/**
 * drbd_check_al_size() - Ensures that the AL is of the right size
<<<<<<< HEAD
<<<<<<< HEAD
 * @mdev:	DRBD device.
=======
 * @device:	DRBD device.
>>>>>>> v3.18
=======
 * @device:	DRBD device.
>>>>>>> v3.18
 *
 * Returns -EBUSY if current al lru is still used, -ENOMEM when allocation
 * failed, and 0 on success. You should call drbd_md_sync() after you called
 * this function.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int drbd_check_al_size(struct drbd_conf *mdev, struct disk_conf *dc)
=======
static int drbd_check_al_size(struct drbd_device *device, struct disk_conf *dc)
>>>>>>> v3.18
=======
static int drbd_check_al_size(struct drbd_device *device, struct disk_conf *dc)
>>>>>>> v3.18
{
	struct lru_cache *n, *t;
	struct lc_element *e;
	unsigned int in_use;
	int i;

<<<<<<< HEAD
<<<<<<< HEAD
	if (mdev->act_log &&
	    mdev->act_log->nr_elements == dc->al_extents)
		return 0;

	in_use = 0;
	t = mdev->act_log;
=======
=======
>>>>>>> v3.18
	if (device->act_log &&
	    device->act_log->nr_elements == dc->al_extents)
		return 0;

	in_use = 0;
	t = device->act_log;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	n = lc_create("act_log", drbd_al_ext_cache, AL_UPDATES_PER_TRANSACTION,
		dc->al_extents, sizeof(struct lc_element), 0);

	if (n == NULL) {
<<<<<<< HEAD
<<<<<<< HEAD
		dev_err(DEV, "Cannot allocate act_log lru!\n");
		return -ENOMEM;
	}
	spin_lock_irq(&mdev->al_lock);
=======
=======
>>>>>>> v3.18
		drbd_err(device, "Cannot allocate act_log lru!\n");
		return -ENOMEM;
	}
	spin_lock_irq(&device->al_lock);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (t) {
		for (i = 0; i < t->nr_elements; i++) {
			e = lc_element_by_index(t, i);
			if (e->refcnt)
<<<<<<< HEAD
<<<<<<< HEAD
				dev_err(DEV, "refcnt(%d)==%d\n",
=======
				drbd_err(device, "refcnt(%d)==%d\n",
>>>>>>> v3.18
=======
				drbd_err(device, "refcnt(%d)==%d\n",
>>>>>>> v3.18
				    e->lc_number, e->refcnt);
			in_use += e->refcnt;
		}
	}
	if (!in_use)
<<<<<<< HEAD
<<<<<<< HEAD
		mdev->act_log = n;
	spin_unlock_irq(&mdev->al_lock);
	if (in_use) {
		dev_err(DEV, "Activity log still in use!\n");
=======
=======
>>>>>>> v3.18
		device->act_log = n;
	spin_unlock_irq(&device->al_lock);
	if (in_use) {
		drbd_err(device, "Activity log still in use!\n");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		lc_destroy(n);
		return -EBUSY;
	} else {
		if (t)
			lc_destroy(t);
	}
<<<<<<< HEAD
<<<<<<< HEAD
	drbd_md_mark_dirty(mdev); /* we changed mdev->act_log->nr_elemens */
	return 0;
}

static void drbd_setup_queue_param(struct drbd_conf *mdev, unsigned int max_bio_size)
{
	struct request_queue * const q = mdev->rq_queue;
	unsigned int max_hw_sectors = max_bio_size >> 9;
	unsigned int max_segments = 0;

	if (get_ldev_if_state(mdev, D_ATTACHING)) {
		struct request_queue * const b = mdev->ldev->backing_bdev->bd_disk->queue;

		max_hw_sectors = min(queue_max_hw_sectors(b), max_bio_size >> 9);
		rcu_read_lock();
		max_segments = rcu_dereference(mdev->ldev->disk_conf)->max_bio_bvecs;
		rcu_read_unlock();
		put_ldev(mdev);
=======
=======
>>>>>>> v3.18
	drbd_md_mark_dirty(device); /* we changed device->act_log->nr_elemens */
	return 0;
}

static void drbd_setup_queue_param(struct drbd_device *device, struct drbd_backing_dev *bdev,
				   unsigned int max_bio_size)
{
	struct request_queue * const q = device->rq_queue;
	unsigned int max_hw_sectors = max_bio_size >> 9;
	unsigned int max_segments = 0;
	struct request_queue *b = NULL;

	if (bdev) {
		b = bdev->backing_bdev->bd_disk->queue;

		max_hw_sectors = min(queue_max_hw_sectors(b), max_bio_size >> 9);
		rcu_read_lock();
		max_segments = rcu_dereference(device->ldev->disk_conf)->max_bio_bvecs;
		rcu_read_unlock();

		blk_set_stacking_limits(&q->limits);
		blk_queue_max_write_same_sectors(q, 0);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}

	blk_queue_logical_block_size(q, 512);
	blk_queue_max_hw_sectors(q, max_hw_sectors);
	/* This is the workaround for "bio would need to, but cannot, be split" */
	blk_queue_max_segments(q, max_segments ? max_segments : BLK_MAX_SEGMENTS);
	blk_queue_segment_boundary(q, PAGE_CACHE_SIZE-1);

<<<<<<< HEAD
<<<<<<< HEAD
	if (get_ldev_if_state(mdev, D_ATTACHING)) {
		struct request_queue * const b = mdev->ldev->backing_bdev->bd_disk->queue;
=======
=======
>>>>>>> v3.18
	if (b) {
		struct drbd_connection *connection = first_peer_device(device)->connection;

		if (blk_queue_discard(b) &&
		    (connection->cstate < C_CONNECTED || connection->agreed_features & FF_TRIM)) {
			/* For now, don't allow more than one activity log extent worth of data
			 * to be discarded in one go. We may need to rework drbd_al_begin_io()
			 * to allow for even larger discard ranges */
			q->limits.max_discard_sectors = DRBD_MAX_DISCARD_SECTORS;

			queue_flag_set_unlocked(QUEUE_FLAG_DISCARD, q);
			/* REALLY? Is stacking secdiscard "legal"? */
			if (blk_queue_secdiscard(b))
				queue_flag_set_unlocked(QUEUE_FLAG_SECDISCARD, q);
		} else {
			q->limits.max_discard_sectors = 0;
			queue_flag_clear_unlocked(QUEUE_FLAG_DISCARD, q);
			queue_flag_clear_unlocked(QUEUE_FLAG_SECDISCARD, q);
		}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

		blk_queue_stack_limits(q, b);

		if (q->backing_dev_info.ra_pages != b->backing_dev_info.ra_pages) {
<<<<<<< HEAD
<<<<<<< HEAD
			dev_info(DEV, "Adjusting my ra_pages to backing device's (%lu -> %lu)\n",
=======
			drbd_info(device, "Adjusting my ra_pages to backing device's (%lu -> %lu)\n",
>>>>>>> v3.18
=======
			drbd_info(device, "Adjusting my ra_pages to backing device's (%lu -> %lu)\n",
>>>>>>> v3.18
				 q->backing_dev_info.ra_pages,
				 b->backing_dev_info.ra_pages);
			q->backing_dev_info.ra_pages = b->backing_dev_info.ra_pages;
		}
<<<<<<< HEAD
<<<<<<< HEAD
		put_ldev(mdev);
	}
}

void drbd_reconsider_max_bio_size(struct drbd_conf *mdev)
{
	unsigned int now, new, local, peer;

	now = queue_max_hw_sectors(mdev->rq_queue) << 9;
	local = mdev->local_max_bio_size; /* Eventually last known value, from volatile memory */
	peer = mdev->peer_max_bio_size; /* Eventually last known value, from meta data */

	if (get_ldev_if_state(mdev, D_ATTACHING)) {
		local = queue_max_hw_sectors(mdev->ldev->backing_bdev->bd_disk->queue) << 9;
		mdev->local_max_bio_size = local;
		put_ldev(mdev);
=======
=======
>>>>>>> v3.18
	}
}

void drbd_reconsider_max_bio_size(struct drbd_device *device, struct drbd_backing_dev *bdev)
{
	unsigned int now, new, local, peer;

	now = queue_max_hw_sectors(device->rq_queue) << 9;
	local = device->local_max_bio_size; /* Eventually last known value, from volatile memory */
	peer = device->peer_max_bio_size; /* Eventually last known value, from meta data */

	if (bdev) {
		local = queue_max_hw_sectors(bdev->backing_bdev->bd_disk->queue) << 9;
		device->local_max_bio_size = local;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}
	local = min(local, DRBD_MAX_BIO_SIZE);

	/* We may ignore peer limits if the peer is modern enough.
	   Because new from 8.3.8 onwards the peer can use multiple
	   BIOs for a single peer_request */
<<<<<<< HEAD
<<<<<<< HEAD
	if (mdev->state.conn >= C_CONNECTED) {
		if (mdev->tconn->agreed_pro_version < 94)
			peer = min( mdev->peer_max_bio_size, DRBD_MAX_SIZE_H80_PACKET);
			/* Correct old drbd (up to 8.3.7) if it believes it can do more than 32KiB */
		else if (mdev->tconn->agreed_pro_version == 94)
			peer = DRBD_MAX_SIZE_H80_PACKET;
		else if (mdev->tconn->agreed_pro_version < 100)
			peer = DRBD_MAX_BIO_SIZE_P95;  /* drbd 8.3.8 onwards, before 8.4.0 */
		else
			peer = DRBD_MAX_BIO_SIZE;
	}

	new = min(local, peer);

	if (mdev->state.role == R_PRIMARY && new < now)
		dev_err(DEV, "ASSERT FAILED new < now; (%u < %u)\n", new, now);

	if (new != now)
		dev_info(DEV, "max BIO size = %u\n", new);

	drbd_setup_queue_param(mdev, new);
}

/* Starts the worker thread */
static void conn_reconfig_start(struct drbd_tconn *tconn)
{
	drbd_thread_start(&tconn->worker);
	conn_flush_workqueue(tconn);
}

/* if still unconfigured, stops worker again. */
static void conn_reconfig_done(struct drbd_tconn *tconn)
{
	bool stop_threads;
	spin_lock_irq(&tconn->req_lock);
	stop_threads = conn_all_vols_unconf(tconn) &&
		tconn->cstate == C_STANDALONE;
	spin_unlock_irq(&tconn->req_lock);
	if (stop_threads) {
		/* asender is implicitly stopped by receiver
		 * in conn_disconnect() */
		drbd_thread_stop(&tconn->receiver);
		drbd_thread_stop(&tconn->worker);
=======
=======
>>>>>>> v3.18
	if (device->state.conn >= C_WF_REPORT_PARAMS) {
		if (first_peer_device(device)->connection->agreed_pro_version < 94)
			peer = min(device->peer_max_bio_size, DRBD_MAX_SIZE_H80_PACKET);
			/* Correct old drbd (up to 8.3.7) if it believes it can do more than 32KiB */
		else if (first_peer_device(device)->connection->agreed_pro_version == 94)
			peer = DRBD_MAX_SIZE_H80_PACKET;
		else if (first_peer_device(device)->connection->agreed_pro_version < 100)
			peer = DRBD_MAX_BIO_SIZE_P95;  /* drbd 8.3.8 onwards, before 8.4.0 */
		else
			peer = DRBD_MAX_BIO_SIZE;

		/* We may later detach and re-attach on a disconnected Primary.
		 * Avoid this setting to jump back in that case.
		 * We want to store what we know the peer DRBD can handle,
		 * not what the peer IO backend can handle. */
		if (peer > device->peer_max_bio_size)
			device->peer_max_bio_size = peer;
	}
	new = min(local, peer);

	if (device->state.role == R_PRIMARY && new < now)
		drbd_err(device, "ASSERT FAILED new < now; (%u < %u)\n", new, now);

	if (new != now)
		drbd_info(device, "max BIO size = %u\n", new);

	drbd_setup_queue_param(device, bdev, new);
}

/* Starts the worker thread */
static void conn_reconfig_start(struct drbd_connection *connection)
{
	drbd_thread_start(&connection->worker);
	drbd_flush_workqueue(&connection->sender_work);
}

/* if still unconfigured, stops worker again. */
static void conn_reconfig_done(struct drbd_connection *connection)
{
	bool stop_threads;
	spin_lock_irq(&connection->resource->req_lock);
	stop_threads = conn_all_vols_unconf(connection) &&
		connection->cstate == C_STANDALONE;
	spin_unlock_irq(&connection->resource->req_lock);
	if (stop_threads) {
		/* asender is implicitly stopped by receiver
		 * in conn_disconnect() */
		drbd_thread_stop(&connection->receiver);
		drbd_thread_stop(&connection->worker);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}
}

/* Make sure IO is suspended before calling this function(). */
<<<<<<< HEAD
<<<<<<< HEAD
static void drbd_suspend_al(struct drbd_conf *mdev)
{
	int s = 0;

	if (!lc_try_lock(mdev->act_log)) {
		dev_warn(DEV, "Failed to lock al in drbd_suspend_al()\n");
		return;
	}

	drbd_al_shrink(mdev);
	spin_lock_irq(&mdev->tconn->req_lock);
	if (mdev->state.conn < C_CONNECTED)
		s = !test_and_set_bit(AL_SUSPENDED, &mdev->flags);
	spin_unlock_irq(&mdev->tconn->req_lock);
	lc_unlock(mdev->act_log);

	if (s)
		dev_info(DEV, "Suspended AL updates\n");
=======
=======
>>>>>>> v3.18
static void drbd_suspend_al(struct drbd_device *device)
{
	int s = 0;

	if (!lc_try_lock(device->act_log)) {
		drbd_warn(device, "Failed to lock al in drbd_suspend_al()\n");
		return;
	}

	drbd_al_shrink(device);
	spin_lock_irq(&device->resource->req_lock);
	if (device->state.conn < C_CONNECTED)
		s = !test_and_set_bit(AL_SUSPENDED, &device->flags);
	spin_unlock_irq(&device->resource->req_lock);
	lc_unlock(device->act_log);

	if (s)
		drbd_info(device, "Suspended AL updates\n");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}


static bool should_set_defaults(struct genl_info *info)
{
	unsigned flags = ((struct drbd_genlmsghdr*)info->userhdr)->flags;
	return 0 != (flags & DRBD_GENL_F_SET_DEFAULTS);
}

static unsigned int drbd_al_extents_max(struct drbd_backing_dev *bdev)
{
	/* This is limited by 16 bit "slot" numbers,
	 * and by available on-disk context storage.
	 *
	 * Also (u16)~0 is special (denotes a "free" extent).
	 *
	 * One transaction occupies one 4kB on-disk block,
	 * we have n such blocks in the on disk ring buffer,
	 * the "current" transaction may fail (n-1),
	 * and there is 919 slot numbers context information per transaction.
	 *
	 * 72 transaction blocks amounts to more than 2**16 context slots,
	 * so cap there first.
	 */
	const unsigned int max_al_nr = DRBD_AL_EXTENTS_MAX;
	const unsigned int sufficient_on_disk =
		(max_al_nr + AL_CONTEXT_PER_TRANSACTION -1)
		/AL_CONTEXT_PER_TRANSACTION;

	unsigned int al_size_4k = bdev->md.al_size_4k;

	if (al_size_4k > sufficient_on_disk)
		return max_al_nr;

	return (al_size_4k - 1) * AL_CONTEXT_PER_TRANSACTION;
}

<<<<<<< HEAD
<<<<<<< HEAD
int drbd_adm_disk_opts(struct sk_buff *skb, struct genl_info *info)
{
	enum drbd_ret_code retcode;
	struct drbd_conf *mdev;
=======
=======
>>>>>>> v3.18
static bool write_ordering_changed(struct disk_conf *a, struct disk_conf *b)
{
	return	a->disk_barrier != b->disk_barrier ||
		a->disk_flushes != b->disk_flushes ||
		a->disk_drain != b->disk_drain;
}

int drbd_adm_disk_opts(struct sk_buff *skb, struct genl_info *info)
{
	struct drbd_config_context adm_ctx;
	enum drbd_ret_code retcode;
	struct drbd_device *device;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	struct disk_conf *new_disk_conf, *old_disk_conf;
	struct fifo_buffer *old_plan = NULL, *new_plan = NULL;
	int err, fifo_size;

<<<<<<< HEAD
<<<<<<< HEAD
	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	mdev = adm_ctx.mdev;

	/* we also need a disk
	 * to change the options on */
	if (!get_ldev(mdev)) {
=======
=======
>>>>>>> v3.18
	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto finish;

	device = adm_ctx.device;
	mutex_lock(&adm_ctx.resource->adm_mutex);

	/* we also need a disk
	 * to change the options on */
	if (!get_ldev(device)) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		retcode = ERR_NO_DISK;
		goto out;
	}

	new_disk_conf = kmalloc(sizeof(struct disk_conf), GFP_KERNEL);
	if (!new_disk_conf) {
		retcode = ERR_NOMEM;
		goto fail;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	mutex_lock(&mdev->tconn->conf_update);
	old_disk_conf = mdev->ldev->disk_conf;
=======
	mutex_lock(&device->resource->conf_update);
	old_disk_conf = device->ldev->disk_conf;
>>>>>>> v3.18
=======
	mutex_lock(&device->resource->conf_update);
	old_disk_conf = device->ldev->disk_conf;
>>>>>>> v3.18
	*new_disk_conf = *old_disk_conf;
	if (should_set_defaults(info))
		set_disk_conf_defaults(new_disk_conf);

	err = disk_conf_from_attrs_for_change(new_disk_conf, info);
	if (err && err != -ENOMSG) {
		retcode = ERR_MANDATORY_TAG;
<<<<<<< HEAD
<<<<<<< HEAD
		drbd_msg_put_info(from_attrs_err_to_txt(err));
=======
		drbd_msg_put_info(adm_ctx.reply_skb, from_attrs_err_to_txt(err));
		goto fail_unlock;
>>>>>>> v3.18
=======
		drbd_msg_put_info(adm_ctx.reply_skb, from_attrs_err_to_txt(err));
		goto fail_unlock;
>>>>>>> v3.18
	}

	if (!expect(new_disk_conf->resync_rate >= 1))
		new_disk_conf->resync_rate = 1;

	if (new_disk_conf->al_extents < DRBD_AL_EXTENTS_MIN)
		new_disk_conf->al_extents = DRBD_AL_EXTENTS_MIN;
<<<<<<< HEAD
<<<<<<< HEAD
	if (new_disk_conf->al_extents > drbd_al_extents_max(mdev->ldev))
		new_disk_conf->al_extents = drbd_al_extents_max(mdev->ldev);
=======
	if (new_disk_conf->al_extents > drbd_al_extents_max(device->ldev))
		new_disk_conf->al_extents = drbd_al_extents_max(device->ldev);
>>>>>>> v3.18
=======
	if (new_disk_conf->al_extents > drbd_al_extents_max(device->ldev))
		new_disk_conf->al_extents = drbd_al_extents_max(device->ldev);
>>>>>>> v3.18

	if (new_disk_conf->c_plan_ahead > DRBD_C_PLAN_AHEAD_MAX)
		new_disk_conf->c_plan_ahead = DRBD_C_PLAN_AHEAD_MAX;

	fifo_size = (new_disk_conf->c_plan_ahead * 10 * SLEEP_TIME) / HZ;
<<<<<<< HEAD
<<<<<<< HEAD
	if (fifo_size != mdev->rs_plan_s->size) {
		new_plan = fifo_alloc(fifo_size);
		if (!new_plan) {
			dev_err(DEV, "kmalloc of fifo_buffer failed");
=======
=======
>>>>>>> v3.18
	if (fifo_size != device->rs_plan_s->size) {
		new_plan = fifo_alloc(fifo_size);
		if (!new_plan) {
			drbd_err(device, "kmalloc of fifo_buffer failed");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			retcode = ERR_NOMEM;
			goto fail_unlock;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	drbd_suspend_io(mdev);
	wait_event(mdev->al_wait, lc_try_lock(mdev->act_log));
	drbd_al_shrink(mdev);
	err = drbd_check_al_size(mdev, new_disk_conf);
	lc_unlock(mdev->act_log);
	wake_up(&mdev->al_wait);
	drbd_resume_io(mdev);
=======
=======
>>>>>>> v3.18
	drbd_suspend_io(device);
	wait_event(device->al_wait, lc_try_lock(device->act_log));
	drbd_al_shrink(device);
	err = drbd_check_al_size(device, new_disk_conf);
	lc_unlock(device->act_log);
	wake_up(&device->al_wait);
	drbd_resume_io(device);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (err) {
		retcode = ERR_NOMEM;
		goto fail_unlock;
	}

	write_lock_irq(&global_state_lock);
<<<<<<< HEAD
<<<<<<< HEAD
	retcode = drbd_resync_after_valid(mdev, new_disk_conf->resync_after);
	if (retcode == NO_ERROR) {
		rcu_assign_pointer(mdev->ldev->disk_conf, new_disk_conf);
		drbd_resync_after_changed(mdev);
=======
=======
>>>>>>> v3.18
	retcode = drbd_resync_after_valid(device, new_disk_conf->resync_after);
	if (retcode == NO_ERROR) {
		rcu_assign_pointer(device->ldev->disk_conf, new_disk_conf);
		drbd_resync_after_changed(device);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}
	write_unlock_irq(&global_state_lock);

	if (retcode != NO_ERROR)
		goto fail_unlock;

	if (new_plan) {
<<<<<<< HEAD
<<<<<<< HEAD
		old_plan = mdev->rs_plan_s;
		rcu_assign_pointer(mdev->rs_plan_s, new_plan);
	}

	mutex_unlock(&mdev->tconn->conf_update);

	if (new_disk_conf->al_updates)
		mdev->ldev->md.flags &= ~MDF_AL_DISABLED;
	else
		mdev->ldev->md.flags |= MDF_AL_DISABLED;

	if (new_disk_conf->md_flushes)
		clear_bit(MD_NO_FUA, &mdev->flags);
	else
		set_bit(MD_NO_FUA, &mdev->flags);

	drbd_bump_write_ordering(mdev->tconn, WO_bdev_flush);

	drbd_md_sync(mdev);

	if (mdev->state.conn >= C_CONNECTED)
		drbd_send_sync_param(mdev);
=======
=======
>>>>>>> v3.18
		old_plan = device->rs_plan_s;
		rcu_assign_pointer(device->rs_plan_s, new_plan);
	}

	mutex_unlock(&device->resource->conf_update);

	if (new_disk_conf->al_updates)
		device->ldev->md.flags &= ~MDF_AL_DISABLED;
	else
		device->ldev->md.flags |= MDF_AL_DISABLED;

	if (new_disk_conf->md_flushes)
		clear_bit(MD_NO_FUA, &device->flags);
	else
		set_bit(MD_NO_FUA, &device->flags);

	if (write_ordering_changed(old_disk_conf, new_disk_conf))
		drbd_bump_write_ordering(device->resource, NULL, WO_bdev_flush);

	drbd_md_sync(device);

	if (device->state.conn >= C_CONNECTED) {
		struct drbd_peer_device *peer_device;

		for_each_peer_device(peer_device, device)
			drbd_send_sync_param(peer_device);
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	synchronize_rcu();
	kfree(old_disk_conf);
	kfree(old_plan);
<<<<<<< HEAD
<<<<<<< HEAD
	mod_timer(&mdev->request_timer, jiffies + HZ);
	goto success;

fail_unlock:
	mutex_unlock(&mdev->tconn->conf_update);
=======
=======
>>>>>>> v3.18
	mod_timer(&device->request_timer, jiffies + HZ);
	goto success;

fail_unlock:
	mutex_unlock(&device->resource->conf_update);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
 fail:
	kfree(new_disk_conf);
	kfree(new_plan);
success:
<<<<<<< HEAD
<<<<<<< HEAD
	put_ldev(mdev);
 out:
	drbd_adm_finish(info, retcode);
=======
=======
>>>>>>> v3.18
	put_ldev(device);
 out:
	mutex_unlock(&adm_ctx.resource->adm_mutex);
 finish:
	drbd_adm_finish(&adm_ctx, info, retcode);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
}

int drbd_adm_attach(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct drbd_conf *mdev;
=======
=======
>>>>>>> v3.18
	struct drbd_config_context adm_ctx;
	struct drbd_device *device;
	struct drbd_peer_device *peer_device;
	struct drbd_connection *connection;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	int err;
	enum drbd_ret_code retcode;
	enum determine_dev_size dd;
	sector_t max_possible_sectors;
	sector_t min_md_device_sectors;
	struct drbd_backing_dev *nbc = NULL; /* new_backing_conf */
	struct disk_conf *new_disk_conf = NULL;
	struct block_device *bdev;
	struct lru_cache *resync_lru = NULL;
	struct fifo_buffer *new_plan = NULL;
	union drbd_state ns, os;
	enum drbd_state_rv rv;
	struct net_conf *nc;

<<<<<<< HEAD
<<<<<<< HEAD
	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
=======
	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
>>>>>>> v3.18
=======
	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
>>>>>>> v3.18
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto finish;

<<<<<<< HEAD
<<<<<<< HEAD
	mdev = adm_ctx.mdev;
	conn_reconfig_start(mdev->tconn);

	/* if you want to reconfigure, please tear down first */
	if (mdev->state.disk > D_DISKLESS) {
=======
=======
>>>>>>> v3.18
	device = adm_ctx.device;
	mutex_lock(&adm_ctx.resource->adm_mutex);
	peer_device = first_peer_device(device);
	connection = peer_device ? peer_device->connection : NULL;
	conn_reconfig_start(connection);

	/* if you want to reconfigure, please tear down first */
	if (device->state.disk > D_DISKLESS) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		retcode = ERR_DISK_CONFIGURED;
		goto fail;
	}
	/* It may just now have detached because of IO error.  Make sure
	 * drbd_ldev_destroy is done already, we may end up here very fast,
	 * e.g. if someone calls attach from the on-io-error handler,
	 * to realize a "hot spare" feature (not that I'd recommend that) */
<<<<<<< HEAD
<<<<<<< HEAD
	wait_event(mdev->misc_wait, !atomic_read(&mdev->local_cnt));

	/* make sure there is no leftover from previous force-detach attempts */
	clear_bit(FORCE_DETACH, &mdev->flags);
	clear_bit(WAS_IO_ERROR, &mdev->flags);
	clear_bit(WAS_READ_ERROR, &mdev->flags);

	/* and no leftover from previously aborted resync or verify, either */
	mdev->rs_total = 0;
	mdev->rs_failed = 0;
	atomic_set(&mdev->rs_pending_cnt, 0);
=======
=======
>>>>>>> v3.18
	wait_event(device->misc_wait, !test_bit(GOING_DISKLESS, &device->flags));

	/* make sure there is no leftover from previous force-detach attempts */
	clear_bit(FORCE_DETACH, &device->flags);
	clear_bit(WAS_IO_ERROR, &device->flags);
	clear_bit(WAS_READ_ERROR, &device->flags);

	/* and no leftover from previously aborted resync or verify, either */
	device->rs_total = 0;
	device->rs_failed = 0;
	atomic_set(&device->rs_pending_cnt, 0);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* allocation not in the IO path, drbdsetup context */
	nbc = kzalloc(sizeof(struct drbd_backing_dev), GFP_KERNEL);
	if (!nbc) {
		retcode = ERR_NOMEM;
		goto fail;
	}
	spin_lock_init(&nbc->md.uuid_lock);

	new_disk_conf = kzalloc(sizeof(struct disk_conf), GFP_KERNEL);
	if (!new_disk_conf) {
		retcode = ERR_NOMEM;
		goto fail;
	}
	nbc->disk_conf = new_disk_conf;

	set_disk_conf_defaults(new_disk_conf);
	err = disk_conf_from_attrs(new_disk_conf, info);
	if (err) {
		retcode = ERR_MANDATORY_TAG;
<<<<<<< HEAD
<<<<<<< HEAD
		drbd_msg_put_info(from_attrs_err_to_txt(err));
=======
		drbd_msg_put_info(adm_ctx.reply_skb, from_attrs_err_to_txt(err));
>>>>>>> v3.18
=======
		drbd_msg_put_info(adm_ctx.reply_skb, from_attrs_err_to_txt(err));
>>>>>>> v3.18
		goto fail;
	}

	if (new_disk_conf->c_plan_ahead > DRBD_C_PLAN_AHEAD_MAX)
		new_disk_conf->c_plan_ahead = DRBD_C_PLAN_AHEAD_MAX;

	new_plan = fifo_alloc((new_disk_conf->c_plan_ahead * 10 * SLEEP_TIME) / HZ);
	if (!new_plan) {
		retcode = ERR_NOMEM;
		goto fail;
	}

	if (new_disk_conf->meta_dev_idx < DRBD_MD_INDEX_FLEX_INT) {
		retcode = ERR_MD_IDX_INVALID;
		goto fail;
	}

	write_lock_irq(&global_state_lock);
<<<<<<< HEAD
<<<<<<< HEAD
	retcode = drbd_resync_after_valid(mdev, new_disk_conf->resync_after);
=======
	retcode = drbd_resync_after_valid(device, new_disk_conf->resync_after);
>>>>>>> v3.18
=======
	retcode = drbd_resync_after_valid(device, new_disk_conf->resync_after);
>>>>>>> v3.18
	write_unlock_irq(&global_state_lock);
	if (retcode != NO_ERROR)
		goto fail;

	rcu_read_lock();
<<<<<<< HEAD
<<<<<<< HEAD
	nc = rcu_dereference(mdev->tconn->net_conf);
=======
	nc = rcu_dereference(connection->net_conf);
>>>>>>> v3.18
=======
	nc = rcu_dereference(connection->net_conf);
>>>>>>> v3.18
	if (nc) {
		if (new_disk_conf->fencing == FP_STONITH && nc->wire_protocol == DRBD_PROT_A) {
			rcu_read_unlock();
			retcode = ERR_STONITH_AND_PROT_A;
			goto fail;
		}
	}
	rcu_read_unlock();

	bdev = blkdev_get_by_path(new_disk_conf->backing_dev,
<<<<<<< HEAD
<<<<<<< HEAD
				  FMODE_READ | FMODE_WRITE | FMODE_EXCL, mdev);
	if (IS_ERR(bdev)) {
		dev_err(DEV, "open(\"%s\") failed with %ld\n", new_disk_conf->backing_dev,
=======
				  FMODE_READ | FMODE_WRITE | FMODE_EXCL, device);
	if (IS_ERR(bdev)) {
		drbd_err(device, "open(\"%s\") failed with %ld\n", new_disk_conf->backing_dev,
>>>>>>> v3.18
=======
				  FMODE_READ | FMODE_WRITE | FMODE_EXCL, device);
	if (IS_ERR(bdev)) {
		drbd_err(device, "open(\"%s\") failed with %ld\n", new_disk_conf->backing_dev,
>>>>>>> v3.18
			PTR_ERR(bdev));
		retcode = ERR_OPEN_DISK;
		goto fail;
	}
	nbc->backing_bdev = bdev;

	/*
	 * meta_dev_idx >= 0: external fixed size, possibly multiple
	 * drbd sharing one meta device.  TODO in that case, paranoia
	 * check that [md_bdev, meta_dev_idx] is not yet used by some
	 * other drbd minor!  (if you use drbd.conf + drbdadm, that
	 * should check it for you already; but if you don't, or
	 * someone fooled it, we need to double check here)
	 */
	bdev = blkdev_get_by_path(new_disk_conf->meta_dev,
				  FMODE_READ | FMODE_WRITE | FMODE_EXCL,
				  (new_disk_conf->meta_dev_idx < 0) ?
<<<<<<< HEAD
<<<<<<< HEAD
				  (void *)mdev : (void *)drbd_m_holder);
	if (IS_ERR(bdev)) {
		dev_err(DEV, "open(\"%s\") failed with %ld\n", new_disk_conf->meta_dev,
=======
				  (void *)device : (void *)drbd_m_holder);
	if (IS_ERR(bdev)) {
		drbd_err(device, "open(\"%s\") failed with %ld\n", new_disk_conf->meta_dev,
>>>>>>> v3.18
=======
				  (void *)device : (void *)drbd_m_holder);
	if (IS_ERR(bdev)) {
		drbd_err(device, "open(\"%s\") failed with %ld\n", new_disk_conf->meta_dev,
>>>>>>> v3.18
			PTR_ERR(bdev));
		retcode = ERR_OPEN_MD_DISK;
		goto fail;
	}
	nbc->md_bdev = bdev;

	if ((nbc->backing_bdev == nbc->md_bdev) !=
	    (new_disk_conf->meta_dev_idx == DRBD_MD_INDEX_INTERNAL ||
	     new_disk_conf->meta_dev_idx == DRBD_MD_INDEX_FLEX_INT)) {
		retcode = ERR_MD_IDX_INVALID;
		goto fail;
	}

	resync_lru = lc_create("resync", drbd_bm_ext_cache,
			1, 61, sizeof(struct bm_extent),
			offsetof(struct bm_extent, lce));
	if (!resync_lru) {
		retcode = ERR_NOMEM;
		goto fail;
	}

	/* Read our meta data super block early.
	 * This also sets other on-disk offsets. */
<<<<<<< HEAD
<<<<<<< HEAD
	retcode = drbd_md_read(mdev, nbc);
=======
	retcode = drbd_md_read(device, nbc);
>>>>>>> v3.18
=======
	retcode = drbd_md_read(device, nbc);
>>>>>>> v3.18
	if (retcode != NO_ERROR)
		goto fail;

	if (new_disk_conf->al_extents < DRBD_AL_EXTENTS_MIN)
		new_disk_conf->al_extents = DRBD_AL_EXTENTS_MIN;
	if (new_disk_conf->al_extents > drbd_al_extents_max(nbc))
		new_disk_conf->al_extents = drbd_al_extents_max(nbc);

	if (drbd_get_max_capacity(nbc) < new_disk_conf->disk_size) {
<<<<<<< HEAD
<<<<<<< HEAD
		dev_err(DEV, "max capacity %llu smaller than disk size %llu\n",
=======
		drbd_err(device, "max capacity %llu smaller than disk size %llu\n",
>>>>>>> v3.18
=======
		drbd_err(device, "max capacity %llu smaller than disk size %llu\n",
>>>>>>> v3.18
			(unsigned long long) drbd_get_max_capacity(nbc),
			(unsigned long long) new_disk_conf->disk_size);
		retcode = ERR_DISK_TOO_SMALL;
		goto fail;
	}

	if (new_disk_conf->meta_dev_idx < 0) {
		max_possible_sectors = DRBD_MAX_SECTORS_FLEX;
		/* at least one MB, otherwise it does not make sense */
		min_md_device_sectors = (2<<10);
	} else {
		max_possible_sectors = DRBD_MAX_SECTORS;
		min_md_device_sectors = MD_128MB_SECT * (new_disk_conf->meta_dev_idx + 1);
	}

	if (drbd_get_capacity(nbc->md_bdev) < min_md_device_sectors) {
		retcode = ERR_MD_DISK_TOO_SMALL;
<<<<<<< HEAD
<<<<<<< HEAD
		dev_warn(DEV, "refusing attach: md-device too small, "
=======
		drbd_warn(device, "refusing attach: md-device too small, "
>>>>>>> v3.18
=======
		drbd_warn(device, "refusing attach: md-device too small, "
>>>>>>> v3.18
		     "at least %llu sectors needed for this meta-disk type\n",
		     (unsigned long long) min_md_device_sectors);
		goto fail;
	}

	/* Make sure the new disk is big enough
	 * (we may currently be R_PRIMARY with no local disk...) */
	if (drbd_get_max_capacity(nbc) <
<<<<<<< HEAD
<<<<<<< HEAD
	    drbd_get_capacity(mdev->this_bdev)) {
=======
	    drbd_get_capacity(device->this_bdev)) {
>>>>>>> v3.18
=======
	    drbd_get_capacity(device->this_bdev)) {
>>>>>>> v3.18
		retcode = ERR_DISK_TOO_SMALL;
		goto fail;
	}

	nbc->known_size = drbd_get_capacity(nbc->backing_bdev);

	if (nbc->known_size > max_possible_sectors) {
<<<<<<< HEAD
<<<<<<< HEAD
		dev_warn(DEV, "==> truncating very big lower level device "
			"to currently maximum possible %llu sectors <==\n",
			(unsigned long long) max_possible_sectors);
		if (new_disk_conf->meta_dev_idx >= 0)
			dev_warn(DEV, "==>> using internal or flexible "
				      "meta data may help <<==\n");
	}

	drbd_suspend_io(mdev);
=======
=======
>>>>>>> v3.18
		drbd_warn(device, "==> truncating very big lower level device "
			"to currently maximum possible %llu sectors <==\n",
			(unsigned long long) max_possible_sectors);
		if (new_disk_conf->meta_dev_idx >= 0)
			drbd_warn(device, "==>> using internal or flexible "
				      "meta data may help <<==\n");
	}

	drbd_suspend_io(device);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	/* also wait for the last barrier ack. */
	/* FIXME see also https://daiquiri.linbit/cgi-bin/bugzilla/show_bug.cgi?id=171
	 * We need a way to either ignore barrier acks for barriers sent before a device
	 * was attached, or a way to wait for all pending barrier acks to come in.
	 * As barriers are counted per resource,
	 * we'd need to suspend io on all devices of a resource.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	wait_event(mdev->misc_wait, !atomic_read(&mdev->ap_pending_cnt) || drbd_suspended(mdev));
	/* and for any other previously queued work */
	drbd_flush_workqueue(mdev);

	rv = _drbd_request_state(mdev, NS(disk, D_ATTACHING), CS_VERBOSE);
	retcode = rv;  /* FIXME: Type mismatch. */
	drbd_resume_io(mdev);
	if (rv < SS_SUCCESS)
		goto fail;

	if (!get_ldev_if_state(mdev, D_ATTACHING))
		goto force_diskless;

	if (!mdev->bitmap) {
		if (drbd_bm_init(mdev)) {
=======
=======
>>>>>>> v3.18
	wait_event(device->misc_wait, !atomic_read(&device->ap_pending_cnt) || drbd_suspended(device));
	/* and for any other previously queued work */
	drbd_flush_workqueue(&connection->sender_work);

	rv = _drbd_request_state(device, NS(disk, D_ATTACHING), CS_VERBOSE);
	retcode = rv;  /* FIXME: Type mismatch. */
	drbd_resume_io(device);
	if (rv < SS_SUCCESS)
		goto fail;

	if (!get_ldev_if_state(device, D_ATTACHING))
		goto force_diskless;

	if (!device->bitmap) {
		if (drbd_bm_init(device)) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			retcode = ERR_NOMEM;
			goto force_diskless_dec;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (mdev->state.conn < C_CONNECTED &&
	    mdev->state.role == R_PRIMARY &&
	    (mdev->ed_uuid & ~((u64)1)) != (nbc->md.uuid[UI_CURRENT] & ~((u64)1))) {
		dev_err(DEV, "Can only attach to data with current UUID=%016llX\n",
		    (unsigned long long)mdev->ed_uuid);
=======
=======
>>>>>>> v3.18
	if (device->state.conn < C_CONNECTED &&
	    device->state.role == R_PRIMARY && device->ed_uuid &&
	    (device->ed_uuid & ~((u64)1)) != (nbc->md.uuid[UI_CURRENT] & ~((u64)1))) {
		drbd_err(device, "Can only attach to data with current UUID=%016llX\n",
		    (unsigned long long)device->ed_uuid);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		retcode = ERR_DATA_NOT_CURRENT;
		goto force_diskless_dec;
	}

	/* Since we are diskless, fix the activity log first... */
<<<<<<< HEAD
<<<<<<< HEAD
	if (drbd_check_al_size(mdev, new_disk_conf)) {
=======
	if (drbd_check_al_size(device, new_disk_conf)) {
>>>>>>> v3.18
=======
	if (drbd_check_al_size(device, new_disk_conf)) {
>>>>>>> v3.18
		retcode = ERR_NOMEM;
		goto force_diskless_dec;
	}

	/* Prevent shrinking of consistent devices ! */
	if (drbd_md_test_flag(nbc, MDF_CONSISTENT) &&
<<<<<<< HEAD
<<<<<<< HEAD
	    drbd_new_dev_size(mdev, nbc, nbc->disk_conf->disk_size, 0) < nbc->md.la_size_sect) {
		dev_warn(DEV, "refusing to truncate a consistent device\n");
=======
	    drbd_new_dev_size(device, nbc, nbc->disk_conf->disk_size, 0) < nbc->md.la_size_sect) {
		drbd_warn(device, "refusing to truncate a consistent device\n");
>>>>>>> v3.18
=======
	    drbd_new_dev_size(device, nbc, nbc->disk_conf->disk_size, 0) < nbc->md.la_size_sect) {
		drbd_warn(device, "refusing to truncate a consistent device\n");
>>>>>>> v3.18
		retcode = ERR_DISK_TOO_SMALL;
		goto force_diskless_dec;
	}

	/* Reset the "barriers don't work" bits here, then force meta data to
	 * be written, to ensure we determine if barriers are supported. */
	if (new_disk_conf->md_flushes)
<<<<<<< HEAD
<<<<<<< HEAD
		clear_bit(MD_NO_FUA, &mdev->flags);
	else
		set_bit(MD_NO_FUA, &mdev->flags);

	/* Point of no return reached.
	 * Devices and memory are no longer released by error cleanup below.
	 * now mdev takes over responsibility, and the state engine should
	 * clean it up somewhere.  */
	D_ASSERT(mdev->ldev == NULL);
	mdev->ldev = nbc;
	mdev->resync = resync_lru;
	mdev->rs_plan_s = new_plan;
=======
=======
>>>>>>> v3.18
		clear_bit(MD_NO_FUA, &device->flags);
	else
		set_bit(MD_NO_FUA, &device->flags);

	/* Point of no return reached.
	 * Devices and memory are no longer released by error cleanup below.
	 * now device takes over responsibility, and the state engine should
	 * clean it up somewhere.  */
	D_ASSERT(device, device->ldev == NULL);
	device->ldev = nbc;
	device->resync = resync_lru;
	device->rs_plan_s = new_plan;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	nbc = NULL;
	resync_lru = NULL;
	new_disk_conf = NULL;
	new_plan = NULL;

<<<<<<< HEAD
<<<<<<< HEAD
	drbd_bump_write_ordering(mdev->tconn, WO_bdev_flush);

	if (drbd_md_test_flag(mdev->ldev, MDF_CRASHED_PRIMARY))
		set_bit(CRASHED_PRIMARY, &mdev->flags);
	else
		clear_bit(CRASHED_PRIMARY, &mdev->flags);

	if (drbd_md_test_flag(mdev->ldev, MDF_PRIMARY_IND) &&
	    !(mdev->state.role == R_PRIMARY && mdev->tconn->susp_nod))
		set_bit(CRASHED_PRIMARY, &mdev->flags);

	mdev->send_cnt = 0;
	mdev->recv_cnt = 0;
	mdev->read_cnt = 0;
	mdev->writ_cnt = 0;

	drbd_reconsider_max_bio_size(mdev);
=======
=======
>>>>>>> v3.18
	drbd_bump_write_ordering(device->resource, device->ldev, WO_bdev_flush);

	if (drbd_md_test_flag(device->ldev, MDF_CRASHED_PRIMARY))
		set_bit(CRASHED_PRIMARY, &device->flags);
	else
		clear_bit(CRASHED_PRIMARY, &device->flags);

	if (drbd_md_test_flag(device->ldev, MDF_PRIMARY_IND) &&
	    !(device->state.role == R_PRIMARY && device->resource->susp_nod))
		set_bit(CRASHED_PRIMARY, &device->flags);

	device->send_cnt = 0;
	device->recv_cnt = 0;
	device->read_cnt = 0;
	device->writ_cnt = 0;

	drbd_reconsider_max_bio_size(device, device->ldev);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* If I am currently not R_PRIMARY,
	 * but meta data primary indicator is set,
	 * I just now recover from a hard crash,
	 * and have been R_PRIMARY before that crash.
	 *
	 * Now, if I had no connection before that crash
	 * (have been degraded R_PRIMARY), chances are that
	 * I won't find my peer now either.
	 *
	 * In that case, and _only_ in that case,
	 * we use the degr-wfc-timeout instead of the default,
	 * so we can automatically recover from a crash of a
	 * degraded but active "cluster" after a certain timeout.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	clear_bit(USE_DEGR_WFC_T, &mdev->flags);
	if (mdev->state.role != R_PRIMARY &&
	     drbd_md_test_flag(mdev->ldev, MDF_PRIMARY_IND) &&
	    !drbd_md_test_flag(mdev->ldev, MDF_CONNECTED_IND))
		set_bit(USE_DEGR_WFC_T, &mdev->flags);

	dd = drbd_determine_dev_size(mdev, 0);
	if (dd == dev_size_error) {
		retcode = ERR_NOMEM_BITMAP;
		goto force_diskless_dec;
	} else if (dd == grew)
		set_bit(RESYNC_AFTER_NEG, &mdev->flags);

	if (drbd_md_test_flag(mdev->ldev, MDF_FULL_SYNC) ||
	    (test_bit(CRASHED_PRIMARY, &mdev->flags) &&
	     drbd_md_test_flag(mdev->ldev, MDF_AL_DISABLED))) {
		dev_info(DEV, "Assuming that all blocks are out of sync "
		     "(aka FullSync)\n");
		if (drbd_bitmap_io(mdev, &drbd_bmio_set_n_write,
=======
=======
>>>>>>> v3.18
	clear_bit(USE_DEGR_WFC_T, &device->flags);
	if (device->state.role != R_PRIMARY &&
	     drbd_md_test_flag(device->ldev, MDF_PRIMARY_IND) &&
	    !drbd_md_test_flag(device->ldev, MDF_CONNECTED_IND))
		set_bit(USE_DEGR_WFC_T, &device->flags);

	dd = drbd_determine_dev_size(device, 0, NULL);
	if (dd <= DS_ERROR) {
		retcode = ERR_NOMEM_BITMAP;
		goto force_diskless_dec;
	} else if (dd == DS_GREW)
		set_bit(RESYNC_AFTER_NEG, &device->flags);

	if (drbd_md_test_flag(device->ldev, MDF_FULL_SYNC) ||
	    (test_bit(CRASHED_PRIMARY, &device->flags) &&
	     drbd_md_test_flag(device->ldev, MDF_AL_DISABLED))) {
		drbd_info(device, "Assuming that all blocks are out of sync "
		     "(aka FullSync)\n");
		if (drbd_bitmap_io(device, &drbd_bmio_set_n_write,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			"set_n_write from attaching", BM_LOCKED_MASK)) {
			retcode = ERR_IO_MD_DISK;
			goto force_diskless_dec;
		}
	} else {
<<<<<<< HEAD
<<<<<<< HEAD
		if (drbd_bitmap_io(mdev, &drbd_bm_read,
=======
		if (drbd_bitmap_io(device, &drbd_bm_read,
>>>>>>> v3.18
=======
		if (drbd_bitmap_io(device, &drbd_bm_read,
>>>>>>> v3.18
			"read from attaching", BM_LOCKED_MASK)) {
			retcode = ERR_IO_MD_DISK;
			goto force_diskless_dec;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (_drbd_bm_total_weight(mdev) == drbd_bm_bits(mdev))
		drbd_suspend_al(mdev); /* IO is still suspended here... */

	spin_lock_irq(&mdev->tconn->req_lock);
	os = drbd_read_state(mdev);
=======
=======
>>>>>>> v3.18
	if (_drbd_bm_total_weight(device) == drbd_bm_bits(device))
		drbd_suspend_al(device); /* IO is still suspended here... */

	spin_lock_irq(&device->resource->req_lock);
	os = drbd_read_state(device);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	ns = os;
	/* If MDF_CONSISTENT is not set go into inconsistent state,
	   otherwise investigate MDF_WasUpToDate...
	   If MDF_WAS_UP_TO_DATE is not set go into D_OUTDATED disk state,
	   otherwise into D_CONSISTENT state.
	*/
<<<<<<< HEAD
<<<<<<< HEAD
	if (drbd_md_test_flag(mdev->ldev, MDF_CONSISTENT)) {
		if (drbd_md_test_flag(mdev->ldev, MDF_WAS_UP_TO_DATE))
=======
	if (drbd_md_test_flag(device->ldev, MDF_CONSISTENT)) {
		if (drbd_md_test_flag(device->ldev, MDF_WAS_UP_TO_DATE))
>>>>>>> v3.18
=======
	if (drbd_md_test_flag(device->ldev, MDF_CONSISTENT)) {
		if (drbd_md_test_flag(device->ldev, MDF_WAS_UP_TO_DATE))
>>>>>>> v3.18
			ns.disk = D_CONSISTENT;
		else
			ns.disk = D_OUTDATED;
	} else {
		ns.disk = D_INCONSISTENT;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (drbd_md_test_flag(mdev->ldev, MDF_PEER_OUT_DATED))
=======
	if (drbd_md_test_flag(device->ldev, MDF_PEER_OUT_DATED))
>>>>>>> v3.18
=======
	if (drbd_md_test_flag(device->ldev, MDF_PEER_OUT_DATED))
>>>>>>> v3.18
		ns.pdsk = D_OUTDATED;

	rcu_read_lock();
	if (ns.disk == D_CONSISTENT &&
<<<<<<< HEAD
<<<<<<< HEAD
	    (ns.pdsk == D_OUTDATED || rcu_dereference(mdev->ldev->disk_conf)->fencing == FP_DONT_CARE))
=======
	    (ns.pdsk == D_OUTDATED || rcu_dereference(device->ldev->disk_conf)->fencing == FP_DONT_CARE))
>>>>>>> v3.18
=======
	    (ns.pdsk == D_OUTDATED || rcu_dereference(device->ldev->disk_conf)->fencing == FP_DONT_CARE))
>>>>>>> v3.18
		ns.disk = D_UP_TO_DATE;

	/* All tests on MDF_PRIMARY_IND, MDF_CONNECTED_IND,
	   MDF_CONSISTENT and MDF_WAS_UP_TO_DATE must happen before
	   this point, because drbd_request_state() modifies these
	   flags. */

<<<<<<< HEAD
<<<<<<< HEAD
	if (rcu_dereference(mdev->ldev->disk_conf)->al_updates)
		mdev->ldev->md.flags &= ~MDF_AL_DISABLED;
	else
		mdev->ldev->md.flags |= MDF_AL_DISABLED;
=======
=======
>>>>>>> v3.18
	if (rcu_dereference(device->ldev->disk_conf)->al_updates)
		device->ldev->md.flags &= ~MDF_AL_DISABLED;
	else
		device->ldev->md.flags |= MDF_AL_DISABLED;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	rcu_read_unlock();

	/* In case we are C_CONNECTED postpone any decision on the new disk
	   state after the negotiation phase. */
<<<<<<< HEAD
<<<<<<< HEAD
	if (mdev->state.conn == C_CONNECTED) {
		mdev->new_state_tmp.i = ns.i;
=======
	if (device->state.conn == C_CONNECTED) {
		device->new_state_tmp.i = ns.i;
>>>>>>> v3.18
=======
	if (device->state.conn == C_CONNECTED) {
		device->new_state_tmp.i = ns.i;
>>>>>>> v3.18
		ns.i = os.i;
		ns.disk = D_NEGOTIATING;

		/* We expect to receive up-to-date UUIDs soon.
		   To avoid a race in receive_state, free p_uuid while
		   holding req_lock. I.e. atomic with the state change */
<<<<<<< HEAD
<<<<<<< HEAD
		kfree(mdev->p_uuid);
		mdev->p_uuid = NULL;
	}

	rv = _drbd_set_state(mdev, ns, CS_VERBOSE, NULL);
	spin_unlock_irq(&mdev->tconn->req_lock);
=======
=======
>>>>>>> v3.18
		kfree(device->p_uuid);
		device->p_uuid = NULL;
	}

	rv = _drbd_set_state(device, ns, CS_VERBOSE, NULL);
	spin_unlock_irq(&device->resource->req_lock);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (rv < SS_SUCCESS)
		goto force_diskless_dec;

<<<<<<< HEAD
<<<<<<< HEAD
	mod_timer(&mdev->request_timer, jiffies + HZ);

	if (mdev->state.role == R_PRIMARY)
		mdev->ldev->md.uuid[UI_CURRENT] |=  (u64)1;
	else
		mdev->ldev->md.uuid[UI_CURRENT] &= ~(u64)1;

	drbd_md_mark_dirty(mdev);
	drbd_md_sync(mdev);

	kobject_uevent(&disk_to_dev(mdev->vdisk)->kobj, KOBJ_CHANGE);
	put_ldev(mdev);
	conn_reconfig_done(mdev->tconn);
	drbd_adm_finish(info, retcode);
	return 0;

 force_diskless_dec:
	put_ldev(mdev);
 force_diskless:
	drbd_force_state(mdev, NS(disk, D_DISKLESS));
	drbd_md_sync(mdev);
 fail:
	conn_reconfig_done(mdev->tconn);
=======
=======
>>>>>>> v3.18
	mod_timer(&device->request_timer, jiffies + HZ);

	if (device->state.role == R_PRIMARY)
		device->ldev->md.uuid[UI_CURRENT] |=  (u64)1;
	else
		device->ldev->md.uuid[UI_CURRENT] &= ~(u64)1;

	drbd_md_mark_dirty(device);
	drbd_md_sync(device);

	kobject_uevent(&disk_to_dev(device->vdisk)->kobj, KOBJ_CHANGE);
	put_ldev(device);
	conn_reconfig_done(connection);
	mutex_unlock(&adm_ctx.resource->adm_mutex);
	drbd_adm_finish(&adm_ctx, info, retcode);
	return 0;

 force_diskless_dec:
	put_ldev(device);
 force_diskless:
	drbd_force_state(device, NS(disk, D_DISKLESS));
	drbd_md_sync(device);
 fail:
	conn_reconfig_done(connection);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (nbc) {
		if (nbc->backing_bdev)
			blkdev_put(nbc->backing_bdev,
				   FMODE_READ | FMODE_WRITE | FMODE_EXCL);
		if (nbc->md_bdev)
			blkdev_put(nbc->md_bdev,
				   FMODE_READ | FMODE_WRITE | FMODE_EXCL);
		kfree(nbc);
	}
	kfree(new_disk_conf);
	lc_destroy(resync_lru);
	kfree(new_plan);
<<<<<<< HEAD
<<<<<<< HEAD

 finish:
	drbd_adm_finish(info, retcode);
	return 0;
}

static int adm_detach(struct drbd_conf *mdev, int force)
=======
=======
>>>>>>> v3.18
	mutex_unlock(&adm_ctx.resource->adm_mutex);
 finish:
	drbd_adm_finish(&adm_ctx, info, retcode);
	return 0;
}

static int adm_detach(struct drbd_device *device, int force)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	enum drbd_state_rv retcode;
	int ret;

	if (force) {
<<<<<<< HEAD
<<<<<<< HEAD
		set_bit(FORCE_DETACH, &mdev->flags);
		drbd_force_state(mdev, NS(disk, D_FAILED));
=======
		set_bit(FORCE_DETACH, &device->flags);
		drbd_force_state(device, NS(disk, D_FAILED));
>>>>>>> v3.18
=======
		set_bit(FORCE_DETACH, &device->flags);
		drbd_force_state(device, NS(disk, D_FAILED));
>>>>>>> v3.18
		retcode = SS_SUCCESS;
		goto out;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	drbd_suspend_io(mdev); /* so no-one is stuck in drbd_al_begin_io */
	drbd_md_get_buffer(mdev); /* make sure there is no in-flight meta-data IO */
	retcode = drbd_request_state(mdev, NS(disk, D_FAILED));
	drbd_md_put_buffer(mdev);
	/* D_FAILED will transition to DISKLESS. */
	ret = wait_event_interruptible(mdev->misc_wait,
			mdev->state.disk != D_FAILED);
	drbd_resume_io(mdev);
=======
=======
>>>>>>> v3.18
	drbd_suspend_io(device); /* so no-one is stuck in drbd_al_begin_io */
	drbd_md_get_buffer(device, __func__); /* make sure there is no in-flight meta-data IO */
	retcode = drbd_request_state(device, NS(disk, D_FAILED));
	drbd_md_put_buffer(device);
	/* D_FAILED will transition to DISKLESS. */
	ret = wait_event_interruptible(device->misc_wait,
			device->state.disk != D_FAILED);
	drbd_resume_io(device);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if ((int)retcode == (int)SS_IS_DISKLESS)
		retcode = SS_NOTHING_TO_DO;
	if (ret)
		retcode = ERR_INTR;
out:
	return retcode;
}

/* Detaching the disk is a process in multiple stages.  First we need to lock
 * out application IO, in-flight IO, IO stuck in drbd_al_begin_io.
 * Then we transition to D_DISKLESS, and wait for put_ldev() to return all
 * internal references as well.
 * Only then we have finally detached. */
int drbd_adm_detach(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct drbd_config_context adm_ctx;
>>>>>>> v3.18
=======
	struct drbd_config_context adm_ctx;
>>>>>>> v3.18
	enum drbd_ret_code retcode;
	struct detach_parms parms = { };
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
=======
	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
>>>>>>> v3.18
=======
	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
>>>>>>> v3.18
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	if (info->attrs[DRBD_NLA_DETACH_PARMS]) {
		err = detach_parms_from_attrs(&parms, info);
		if (err) {
			retcode = ERR_MANDATORY_TAG;
<<<<<<< HEAD
<<<<<<< HEAD
			drbd_msg_put_info(from_attrs_err_to_txt(err));
=======
			drbd_msg_put_info(adm_ctx.reply_skb, from_attrs_err_to_txt(err));
>>>>>>> v3.18
=======
			drbd_msg_put_info(adm_ctx.reply_skb, from_attrs_err_to_txt(err));
>>>>>>> v3.18
			goto out;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	retcode = adm_detach(adm_ctx.mdev, parms.force_detach);
out:
	drbd_adm_finish(info, retcode);
	return 0;
}

static bool conn_resync_running(struct drbd_tconn *tconn)
{
	struct drbd_conf *mdev;
=======
=======
>>>>>>> v3.18
	mutex_lock(&adm_ctx.resource->adm_mutex);
	retcode = adm_detach(adm_ctx.device, parms.force_detach);
	mutex_unlock(&adm_ctx.resource->adm_mutex);
out:
	drbd_adm_finish(&adm_ctx, info, retcode);
	return 0;
}

static bool conn_resync_running(struct drbd_connection *connection)
{
	struct drbd_peer_device *peer_device;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	bool rv = false;
	int vnr;

	rcu_read_lock();
<<<<<<< HEAD
<<<<<<< HEAD
	idr_for_each_entry(&tconn->volumes, mdev, vnr) {
		if (mdev->state.conn == C_SYNC_SOURCE ||
		    mdev->state.conn == C_SYNC_TARGET ||
		    mdev->state.conn == C_PAUSED_SYNC_S ||
		    mdev->state.conn == C_PAUSED_SYNC_T) {
=======
=======
>>>>>>> v3.18
	idr_for_each_entry(&connection->peer_devices, peer_device, vnr) {
		struct drbd_device *device = peer_device->device;
		if (device->state.conn == C_SYNC_SOURCE ||
		    device->state.conn == C_SYNC_TARGET ||
		    device->state.conn == C_PAUSED_SYNC_S ||
		    device->state.conn == C_PAUSED_SYNC_T) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			rv = true;
			break;
		}
	}
	rcu_read_unlock();

	return rv;
}

<<<<<<< HEAD
<<<<<<< HEAD
static bool conn_ov_running(struct drbd_tconn *tconn)
{
	struct drbd_conf *mdev;
=======
static bool conn_ov_running(struct drbd_connection *connection)
{
	struct drbd_peer_device *peer_device;
>>>>>>> v3.18
=======
static bool conn_ov_running(struct drbd_connection *connection)
{
	struct drbd_peer_device *peer_device;
>>>>>>> v3.18
	bool rv = false;
	int vnr;

	rcu_read_lock();
<<<<<<< HEAD
<<<<<<< HEAD
	idr_for_each_entry(&tconn->volumes, mdev, vnr) {
		if (mdev->state.conn == C_VERIFY_S ||
		    mdev->state.conn == C_VERIFY_T) {
=======
=======
>>>>>>> v3.18
	idr_for_each_entry(&connection->peer_devices, peer_device, vnr) {
		struct drbd_device *device = peer_device->device;
		if (device->state.conn == C_VERIFY_S ||
		    device->state.conn == C_VERIFY_T) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			rv = true;
			break;
		}
	}
	rcu_read_unlock();

	return rv;
}

static enum drbd_ret_code
<<<<<<< HEAD
<<<<<<< HEAD
_check_net_options(struct drbd_tconn *tconn, struct net_conf *old_conf, struct net_conf *new_conf)
{
	struct drbd_conf *mdev;
	int i;

	if (old_conf && tconn->cstate == C_WF_REPORT_PARAMS && tconn->agreed_pro_version < 100) {
		if (new_conf->wire_protocol != old_conf->wire_protocol)
			return ERR_NEED_APV_100;

		if (new_conf->two_primaries != old_conf->two_primaries)
			return ERR_NEED_APV_100;

		if (strcmp(new_conf->integrity_alg, old_conf->integrity_alg))
			return ERR_NEED_APV_100;
	}

	if (!new_conf->two_primaries &&
	    conn_highest_role(tconn) == R_PRIMARY &&
	    conn_highest_peer(tconn) == R_PRIMARY)
		return ERR_NEED_ALLOW_TWO_PRI;

	if (new_conf->two_primaries &&
	    (new_conf->wire_protocol != DRBD_PROT_C))
		return ERR_NOT_PROTO_C;

	idr_for_each_entry(&tconn->volumes, mdev, i) {
		if (get_ldev(mdev)) {
			enum drbd_fencing_p fp = rcu_dereference(mdev->ldev->disk_conf)->fencing;
			put_ldev(mdev);
			if (new_conf->wire_protocol == DRBD_PROT_A && fp == FP_STONITH)
				return ERR_STONITH_AND_PROT_A;
		}
		if (mdev->state.role == R_PRIMARY && new_conf->discard_my_data)
			return ERR_DISCARD_IMPOSSIBLE;
	}

	if (new_conf->on_congestion != OC_BLOCK && new_conf->wire_protocol != DRBD_PROT_A)
=======
=======
>>>>>>> v3.18
_check_net_options(struct drbd_connection *connection, struct net_conf *old_net_conf, struct net_conf *new_net_conf)
{
	struct drbd_peer_device *peer_device;
	int i;

	if (old_net_conf && connection->cstate == C_WF_REPORT_PARAMS && connection->agreed_pro_version < 100) {
		if (new_net_conf->wire_protocol != old_net_conf->wire_protocol)
			return ERR_NEED_APV_100;

		if (new_net_conf->two_primaries != old_net_conf->two_primaries)
			return ERR_NEED_APV_100;

		if (strcmp(new_net_conf->integrity_alg, old_net_conf->integrity_alg))
			return ERR_NEED_APV_100;
	}

	if (!new_net_conf->two_primaries &&
	    conn_highest_role(connection) == R_PRIMARY &&
	    conn_highest_peer(connection) == R_PRIMARY)
		return ERR_NEED_ALLOW_TWO_PRI;

	if (new_net_conf->two_primaries &&
	    (new_net_conf->wire_protocol != DRBD_PROT_C))
		return ERR_NOT_PROTO_C;

	idr_for_each_entry(&connection->peer_devices, peer_device, i) {
		struct drbd_device *device = peer_device->device;
		if (get_ldev(device)) {
			enum drbd_fencing_p fp = rcu_dereference(device->ldev->disk_conf)->fencing;
			put_ldev(device);
			if (new_net_conf->wire_protocol == DRBD_PROT_A && fp == FP_STONITH)
				return ERR_STONITH_AND_PROT_A;
		}
		if (device->state.role == R_PRIMARY && new_net_conf->discard_my_data)
			return ERR_DISCARD_IMPOSSIBLE;
	}

	if (new_net_conf->on_congestion != OC_BLOCK && new_net_conf->wire_protocol != DRBD_PROT_A)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return ERR_CONG_NOT_PROTO_A;

	return NO_ERROR;
}

static enum drbd_ret_code
<<<<<<< HEAD
<<<<<<< HEAD
check_net_options(struct drbd_tconn *tconn, struct net_conf *new_conf)
{
	static enum drbd_ret_code rv;
	struct drbd_conf *mdev;
	int i;

	rcu_read_lock();
	rv = _check_net_options(tconn, rcu_dereference(tconn->net_conf), new_conf);
	rcu_read_unlock();

	/* tconn->volumes protected by genl_lock() here */
	idr_for_each_entry(&tconn->volumes, mdev, i) {
		if (!mdev->bitmap) {
			if(drbd_bm_init(mdev))
=======
=======
>>>>>>> v3.18
check_net_options(struct drbd_connection *connection, struct net_conf *new_net_conf)
{
	static enum drbd_ret_code rv;
	struct drbd_peer_device *peer_device;
	int i;

	rcu_read_lock();
	rv = _check_net_options(connection, rcu_dereference(connection->net_conf), new_net_conf);
	rcu_read_unlock();

	/* connection->volumes protected by genl_lock() here */
	idr_for_each_entry(&connection->peer_devices, peer_device, i) {
		struct drbd_device *device = peer_device->device;
		if (!device->bitmap) {
			if (drbd_bm_init(device))
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
				return ERR_NOMEM;
		}
	}

	return rv;
}

struct crypto {
	struct crypto_hash *verify_tfm;
	struct crypto_hash *csums_tfm;
	struct crypto_hash *cram_hmac_tfm;
	struct crypto_hash *integrity_tfm;
};

static int
alloc_hash(struct crypto_hash **tfm, char *tfm_name, int err_alg)
{
	if (!tfm_name[0])
		return NO_ERROR;

	*tfm = crypto_alloc_hash(tfm_name, 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(*tfm)) {
		*tfm = NULL;
		return err_alg;
	}

	return NO_ERROR;
}

static enum drbd_ret_code
<<<<<<< HEAD
<<<<<<< HEAD
alloc_crypto(struct crypto *crypto, struct net_conf *new_conf)
=======
alloc_crypto(struct crypto *crypto, struct net_conf *new_net_conf)
>>>>>>> v3.18
=======
alloc_crypto(struct crypto *crypto, struct net_conf *new_net_conf)
>>>>>>> v3.18
{
	char hmac_name[CRYPTO_MAX_ALG_NAME];
	enum drbd_ret_code rv;

<<<<<<< HEAD
<<<<<<< HEAD
	rv = alloc_hash(&crypto->csums_tfm, new_conf->csums_alg,
		       ERR_CSUMS_ALG);
	if (rv != NO_ERROR)
		return rv;
	rv = alloc_hash(&crypto->verify_tfm, new_conf->verify_alg,
		       ERR_VERIFY_ALG);
	if (rv != NO_ERROR)
		return rv;
	rv = alloc_hash(&crypto->integrity_tfm, new_conf->integrity_alg,
		       ERR_INTEGRITY_ALG);
	if (rv != NO_ERROR)
		return rv;
	if (new_conf->cram_hmac_alg[0] != 0) {
		snprintf(hmac_name, CRYPTO_MAX_ALG_NAME, "hmac(%s)",
			 new_conf->cram_hmac_alg);
=======
=======
>>>>>>> v3.18
	rv = alloc_hash(&crypto->csums_tfm, new_net_conf->csums_alg,
		       ERR_CSUMS_ALG);
	if (rv != NO_ERROR)
		return rv;
	rv = alloc_hash(&crypto->verify_tfm, new_net_conf->verify_alg,
		       ERR_VERIFY_ALG);
	if (rv != NO_ERROR)
		return rv;
	rv = alloc_hash(&crypto->integrity_tfm, new_net_conf->integrity_alg,
		       ERR_INTEGRITY_ALG);
	if (rv != NO_ERROR)
		return rv;
	if (new_net_conf->cram_hmac_alg[0] != 0) {
		snprintf(hmac_name, CRYPTO_MAX_ALG_NAME, "hmac(%s)",
			 new_net_conf->cram_hmac_alg);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

		rv = alloc_hash(&crypto->cram_hmac_tfm, hmac_name,
			       ERR_AUTH_ALG);
	}

	return rv;
}

static void free_crypto(struct crypto *crypto)
{
	crypto_free_hash(crypto->cram_hmac_tfm);
	crypto_free_hash(crypto->integrity_tfm);
	crypto_free_hash(crypto->csums_tfm);
	crypto_free_hash(crypto->verify_tfm);
}

int drbd_adm_net_opts(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
	enum drbd_ret_code retcode;
	struct drbd_tconn *tconn;
	struct net_conf *old_conf, *new_conf = NULL;
=======
=======
>>>>>>> v3.18
	struct drbd_config_context adm_ctx;
	enum drbd_ret_code retcode;
	struct drbd_connection *connection;
	struct net_conf *old_net_conf, *new_net_conf = NULL;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	int err;
	int ovr; /* online verify running */
	int rsr; /* re-sync running */
	struct crypto crypto = { };

<<<<<<< HEAD
<<<<<<< HEAD
	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_CONNECTION);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	tconn = adm_ctx.tconn;

	new_conf = kzalloc(sizeof(struct net_conf), GFP_KERNEL);
	if (!new_conf) {
=======
=======
>>>>>>> v3.18
	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_CONNECTION);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto finish;

	connection = adm_ctx.connection;
	mutex_lock(&adm_ctx.resource->adm_mutex);

	new_net_conf = kzalloc(sizeof(struct net_conf), GFP_KERNEL);
	if (!new_net_conf) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		retcode = ERR_NOMEM;
		goto out;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	conn_reconfig_start(tconn);

	mutex_lock(&tconn->data.mutex);
	mutex_lock(&tconn->conf_update);
	old_conf = tconn->net_conf;

	if (!old_conf) {
		drbd_msg_put_info("net conf missing, try connect");
=======
=======
>>>>>>> v3.18
	conn_reconfig_start(connection);

	mutex_lock(&connection->data.mutex);
	mutex_lock(&connection->resource->conf_update);
	old_net_conf = connection->net_conf;

	if (!old_net_conf) {
		drbd_msg_put_info(adm_ctx.reply_skb, "net conf missing, try connect");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		retcode = ERR_INVALID_REQUEST;
		goto fail;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	*new_conf = *old_conf;
	if (should_set_defaults(info))
		set_net_conf_defaults(new_conf);

	err = net_conf_from_attrs_for_change(new_conf, info);
	if (err && err != -ENOMSG) {
		retcode = ERR_MANDATORY_TAG;
		drbd_msg_put_info(from_attrs_err_to_txt(err));
		goto fail;
	}

	retcode = check_net_options(tconn, new_conf);
=======
=======
>>>>>>> v3.18
	*new_net_conf = *old_net_conf;
	if (should_set_defaults(info))
		set_net_conf_defaults(new_net_conf);

	err = net_conf_from_attrs_for_change(new_net_conf, info);
	if (err && err != -ENOMSG) {
		retcode = ERR_MANDATORY_TAG;
		drbd_msg_put_info(adm_ctx.reply_skb, from_attrs_err_to_txt(err));
		goto fail;
	}

	retcode = check_net_options(connection, new_net_conf);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (retcode != NO_ERROR)
		goto fail;

	/* re-sync running */
<<<<<<< HEAD
<<<<<<< HEAD
	rsr = conn_resync_running(tconn);
	if (rsr && strcmp(new_conf->csums_alg, old_conf->csums_alg)) {
=======
	rsr = conn_resync_running(connection);
	if (rsr && strcmp(new_net_conf->csums_alg, old_net_conf->csums_alg)) {
>>>>>>> v3.18
=======
	rsr = conn_resync_running(connection);
	if (rsr && strcmp(new_net_conf->csums_alg, old_net_conf->csums_alg)) {
>>>>>>> v3.18
		retcode = ERR_CSUMS_RESYNC_RUNNING;
		goto fail;
	}

	/* online verify running */
<<<<<<< HEAD
<<<<<<< HEAD
	ovr = conn_ov_running(tconn);
	if (ovr && strcmp(new_conf->verify_alg, old_conf->verify_alg)) {
=======
	ovr = conn_ov_running(connection);
	if (ovr && strcmp(new_net_conf->verify_alg, old_net_conf->verify_alg)) {
>>>>>>> v3.18
=======
	ovr = conn_ov_running(connection);
	if (ovr && strcmp(new_net_conf->verify_alg, old_net_conf->verify_alg)) {
>>>>>>> v3.18
		retcode = ERR_VERIFY_RUNNING;
		goto fail;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	retcode = alloc_crypto(&crypto, new_conf);
	if (retcode != NO_ERROR)
		goto fail;

	rcu_assign_pointer(tconn->net_conf, new_conf);

	if (!rsr) {
		crypto_free_hash(tconn->csums_tfm);
		tconn->csums_tfm = crypto.csums_tfm;
		crypto.csums_tfm = NULL;
	}
	if (!ovr) {
		crypto_free_hash(tconn->verify_tfm);
		tconn->verify_tfm = crypto.verify_tfm;
		crypto.verify_tfm = NULL;
	}

	crypto_free_hash(tconn->integrity_tfm);
	tconn->integrity_tfm = crypto.integrity_tfm;
	if (tconn->cstate >= C_WF_REPORT_PARAMS && tconn->agreed_pro_version >= 100)
		/* Do this without trying to take tconn->data.mutex again.  */
		__drbd_send_protocol(tconn, P_PROTOCOL_UPDATE);

	crypto_free_hash(tconn->cram_hmac_tfm);
	tconn->cram_hmac_tfm = crypto.cram_hmac_tfm;

	mutex_unlock(&tconn->conf_update);
	mutex_unlock(&tconn->data.mutex);
	synchronize_rcu();
	kfree(old_conf);

	if (tconn->cstate >= C_WF_REPORT_PARAMS)
		drbd_send_sync_param(minor_to_mdev(conn_lowest_minor(tconn)));
=======
=======
>>>>>>> v3.18
	retcode = alloc_crypto(&crypto, new_net_conf);
	if (retcode != NO_ERROR)
		goto fail;

	rcu_assign_pointer(connection->net_conf, new_net_conf);

	if (!rsr) {
		crypto_free_hash(connection->csums_tfm);
		connection->csums_tfm = crypto.csums_tfm;
		crypto.csums_tfm = NULL;
	}
	if (!ovr) {
		crypto_free_hash(connection->verify_tfm);
		connection->verify_tfm = crypto.verify_tfm;
		crypto.verify_tfm = NULL;
	}

	crypto_free_hash(connection->integrity_tfm);
	connection->integrity_tfm = crypto.integrity_tfm;
	if (connection->cstate >= C_WF_REPORT_PARAMS && connection->agreed_pro_version >= 100)
		/* Do this without trying to take connection->data.mutex again.  */
		__drbd_send_protocol(connection, P_PROTOCOL_UPDATE);

	crypto_free_hash(connection->cram_hmac_tfm);
	connection->cram_hmac_tfm = crypto.cram_hmac_tfm;

	mutex_unlock(&connection->resource->conf_update);
	mutex_unlock(&connection->data.mutex);
	synchronize_rcu();
	kfree(old_net_conf);

	if (connection->cstate >= C_WF_REPORT_PARAMS) {
		struct drbd_peer_device *peer_device;
		int vnr;

		idr_for_each_entry(&connection->peer_devices, peer_device, vnr)
			drbd_send_sync_param(peer_device);
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	goto done;

 fail:
<<<<<<< HEAD
<<<<<<< HEAD
	mutex_unlock(&tconn->conf_update);
	mutex_unlock(&tconn->data.mutex);
	free_crypto(&crypto);
	kfree(new_conf);
 done:
	conn_reconfig_done(tconn);
 out:
	drbd_adm_finish(info, retcode);
=======
=======
>>>>>>> v3.18
	mutex_unlock(&connection->resource->conf_update);
	mutex_unlock(&connection->data.mutex);
	free_crypto(&crypto);
	kfree(new_net_conf);
 done:
	conn_reconfig_done(connection);
 out:
	mutex_unlock(&adm_ctx.resource->adm_mutex);
 finish:
	drbd_adm_finish(&adm_ctx, info, retcode);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
}

int drbd_adm_connect(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct drbd_conf *mdev;
	struct net_conf *old_conf, *new_conf = NULL;
	struct crypto crypto = { };
	struct drbd_tconn *tconn;
=======
=======
>>>>>>> v3.18
	struct drbd_config_context adm_ctx;
	struct drbd_peer_device *peer_device;
	struct net_conf *old_net_conf, *new_net_conf = NULL;
	struct crypto crypto = { };
	struct drbd_resource *resource;
	struct drbd_connection *connection;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	enum drbd_ret_code retcode;
	int i;
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_RESOURCE);
=======
	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_RESOURCE);
>>>>>>> v3.18
=======
	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_RESOURCE);
>>>>>>> v3.18

	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;
	if (!(adm_ctx.my_addr && adm_ctx.peer_addr)) {
<<<<<<< HEAD
<<<<<<< HEAD
		drbd_msg_put_info("connection endpoint(s) missing");
=======
		drbd_msg_put_info(adm_ctx.reply_skb, "connection endpoint(s) missing");
>>>>>>> v3.18
=======
		drbd_msg_put_info(adm_ctx.reply_skb, "connection endpoint(s) missing");
>>>>>>> v3.18
		retcode = ERR_INVALID_REQUEST;
		goto out;
	}

	/* No need for _rcu here. All reconfiguration is
	 * strictly serialized on genl_lock(). We are protected against
	 * concurrent reconfiguration/addition/deletion */
<<<<<<< HEAD
<<<<<<< HEAD
	list_for_each_entry(tconn, &drbd_tconns, all_tconn) {
		if (nla_len(adm_ctx.my_addr) == tconn->my_addr_len &&
		    !memcmp(nla_data(adm_ctx.my_addr), &tconn->my_addr, tconn->my_addr_len)) {
			retcode = ERR_LOCAL_ADDR;
			goto out;
		}

		if (nla_len(adm_ctx.peer_addr) == tconn->peer_addr_len &&
		    !memcmp(nla_data(adm_ctx.peer_addr), &tconn->peer_addr, tconn->peer_addr_len)) {
			retcode = ERR_PEER_ADDR;
			goto out;
		}
	}

	tconn = adm_ctx.tconn;
	conn_reconfig_start(tconn);

	if (tconn->cstate > C_STANDALONE) {
=======
=======
>>>>>>> v3.18
	for_each_resource(resource, &drbd_resources) {
		for_each_connection(connection, resource) {
			if (nla_len(adm_ctx.my_addr) == connection->my_addr_len &&
			    !memcmp(nla_data(adm_ctx.my_addr), &connection->my_addr,
				    connection->my_addr_len)) {
				retcode = ERR_LOCAL_ADDR;
				goto out;
			}

			if (nla_len(adm_ctx.peer_addr) == connection->peer_addr_len &&
			    !memcmp(nla_data(adm_ctx.peer_addr), &connection->peer_addr,
				    connection->peer_addr_len)) {
				retcode = ERR_PEER_ADDR;
				goto out;
			}
		}
	}

	mutex_lock(&adm_ctx.resource->adm_mutex);
	connection = first_connection(adm_ctx.resource);
	conn_reconfig_start(connection);

	if (connection->cstate > C_STANDALONE) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		retcode = ERR_NET_CONFIGURED;
		goto fail;
	}

	/* allocation not in the IO path, drbdsetup / netlink process context */
<<<<<<< HEAD
<<<<<<< HEAD
	new_conf = kzalloc(sizeof(*new_conf), GFP_KERNEL);
	if (!new_conf) {
=======
	new_net_conf = kzalloc(sizeof(*new_net_conf), GFP_KERNEL);
	if (!new_net_conf) {
>>>>>>> v3.18
=======
	new_net_conf = kzalloc(sizeof(*new_net_conf), GFP_KERNEL);
	if (!new_net_conf) {
>>>>>>> v3.18
		retcode = ERR_NOMEM;
		goto fail;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	set_net_conf_defaults(new_conf);

	err = net_conf_from_attrs(new_conf, info);
	if (err && err != -ENOMSG) {
		retcode = ERR_MANDATORY_TAG;
		drbd_msg_put_info(from_attrs_err_to_txt(err));
		goto fail;
	}

	retcode = check_net_options(tconn, new_conf);
	if (retcode != NO_ERROR)
		goto fail;

	retcode = alloc_crypto(&crypto, new_conf);
	if (retcode != NO_ERROR)
		goto fail;

	((char *)new_conf->shared_secret)[SHARED_SECRET_MAX-1] = 0;

	conn_flush_workqueue(tconn);

	mutex_lock(&tconn->conf_update);
	old_conf = tconn->net_conf;
	if (old_conf) {
		retcode = ERR_NET_CONFIGURED;
		mutex_unlock(&tconn->conf_update);
		goto fail;
	}
	rcu_assign_pointer(tconn->net_conf, new_conf);

	conn_free_crypto(tconn);
	tconn->cram_hmac_tfm = crypto.cram_hmac_tfm;
	tconn->integrity_tfm = crypto.integrity_tfm;
	tconn->csums_tfm = crypto.csums_tfm;
	tconn->verify_tfm = crypto.verify_tfm;

	tconn->my_addr_len = nla_len(adm_ctx.my_addr);
	memcpy(&tconn->my_addr, nla_data(adm_ctx.my_addr), tconn->my_addr_len);
	tconn->peer_addr_len = nla_len(adm_ctx.peer_addr);
	memcpy(&tconn->peer_addr, nla_data(adm_ctx.peer_addr), tconn->peer_addr_len);

	mutex_unlock(&tconn->conf_update);

	rcu_read_lock();
	idr_for_each_entry(&tconn->volumes, mdev, i) {
		mdev->send_cnt = 0;
		mdev->recv_cnt = 0;
	}
	rcu_read_unlock();

	retcode = conn_request_state(tconn, NS(conn, C_UNCONNECTED), CS_VERBOSE);

	conn_reconfig_done(tconn);
	drbd_adm_finish(info, retcode);
=======
=======
>>>>>>> v3.18
	set_net_conf_defaults(new_net_conf);

	err = net_conf_from_attrs(new_net_conf, info);
	if (err && err != -ENOMSG) {
		retcode = ERR_MANDATORY_TAG;
		drbd_msg_put_info(adm_ctx.reply_skb, from_attrs_err_to_txt(err));
		goto fail;
	}

	retcode = check_net_options(connection, new_net_conf);
	if (retcode != NO_ERROR)
		goto fail;

	retcode = alloc_crypto(&crypto, new_net_conf);
	if (retcode != NO_ERROR)
		goto fail;

	((char *)new_net_conf->shared_secret)[SHARED_SECRET_MAX-1] = 0;

	drbd_flush_workqueue(&connection->sender_work);

	mutex_lock(&adm_ctx.resource->conf_update);
	old_net_conf = connection->net_conf;
	if (old_net_conf) {
		retcode = ERR_NET_CONFIGURED;
		mutex_unlock(&adm_ctx.resource->conf_update);
		goto fail;
	}
	rcu_assign_pointer(connection->net_conf, new_net_conf);

	conn_free_crypto(connection);
	connection->cram_hmac_tfm = crypto.cram_hmac_tfm;
	connection->integrity_tfm = crypto.integrity_tfm;
	connection->csums_tfm = crypto.csums_tfm;
	connection->verify_tfm = crypto.verify_tfm;

	connection->my_addr_len = nla_len(adm_ctx.my_addr);
	memcpy(&connection->my_addr, nla_data(adm_ctx.my_addr), connection->my_addr_len);
	connection->peer_addr_len = nla_len(adm_ctx.peer_addr);
	memcpy(&connection->peer_addr, nla_data(adm_ctx.peer_addr), connection->peer_addr_len);

	mutex_unlock(&adm_ctx.resource->conf_update);

	rcu_read_lock();
	idr_for_each_entry(&connection->peer_devices, peer_device, i) {
		struct drbd_device *device = peer_device->device;
		device->send_cnt = 0;
		device->recv_cnt = 0;
	}
	rcu_read_unlock();

	retcode = conn_request_state(connection, NS(conn, C_UNCONNECTED), CS_VERBOSE);

	conn_reconfig_done(connection);
	mutex_unlock(&adm_ctx.resource->adm_mutex);
	drbd_adm_finish(&adm_ctx, info, retcode);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;

fail:
	free_crypto(&crypto);
<<<<<<< HEAD
<<<<<<< HEAD
	kfree(new_conf);

	conn_reconfig_done(tconn);
out:
	drbd_adm_finish(info, retcode);
	return 0;
}

static enum drbd_state_rv conn_try_disconnect(struct drbd_tconn *tconn, bool force)
{
	enum drbd_state_rv rv;

	rv = conn_request_state(tconn, NS(conn, C_DISCONNECTING),
=======
=======
>>>>>>> v3.18
	kfree(new_net_conf);

	conn_reconfig_done(connection);
	mutex_unlock(&adm_ctx.resource->adm_mutex);
out:
	drbd_adm_finish(&adm_ctx, info, retcode);
	return 0;
}

static enum drbd_state_rv conn_try_disconnect(struct drbd_connection *connection, bool force)
{
	enum drbd_state_rv rv;

	rv = conn_request_state(connection, NS(conn, C_DISCONNECTING),
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			force ? CS_HARD : 0);

	switch (rv) {
	case SS_NOTHING_TO_DO:
		break;
	case SS_ALREADY_STANDALONE:
		return SS_SUCCESS;
	case SS_PRIMARY_NOP:
		/* Our state checking code wants to see the peer outdated. */
<<<<<<< HEAD
<<<<<<< HEAD
		rv = conn_request_state(tconn, NS2(conn, C_DISCONNECTING, pdsk, D_OUTDATED), 0);

		if (rv == SS_OUTDATE_WO_CONN) /* lost connection before graceful disconnect succeeded */
			rv = conn_request_state(tconn, NS(conn, C_DISCONNECTING), CS_VERBOSE);
=======
=======
>>>>>>> v3.18
		rv = conn_request_state(connection, NS2(conn, C_DISCONNECTING, pdsk, D_OUTDATED), 0);

		if (rv == SS_OUTDATE_WO_CONN) /* lost connection before graceful disconnect succeeded */
			rv = conn_request_state(connection, NS(conn, C_DISCONNECTING), CS_VERBOSE);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

		break;
	case SS_CW_FAILED_BY_PEER:
		/* The peer probably wants to see us outdated. */
<<<<<<< HEAD
<<<<<<< HEAD
		rv = conn_request_state(tconn, NS2(conn, C_DISCONNECTING,
							disk, D_OUTDATED), 0);
		if (rv == SS_IS_DISKLESS || rv == SS_LOWER_THAN_OUTDATED) {
			rv = conn_request_state(tconn, NS(conn, C_DISCONNECTING),
=======
=======
>>>>>>> v3.18
		rv = conn_request_state(connection, NS2(conn, C_DISCONNECTING,
							disk, D_OUTDATED), 0);
		if (rv == SS_IS_DISKLESS || rv == SS_LOWER_THAN_OUTDATED) {
			rv = conn_request_state(connection, NS(conn, C_DISCONNECTING),
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
					CS_HARD);
		}
		break;
	default:;
		/* no special handling necessary */
	}

	if (rv >= SS_SUCCESS) {
		enum drbd_state_rv rv2;
		/* No one else can reconfigure the network while I am here.
		 * The state handling only uses drbd_thread_stop_nowait(),
		 * we want to really wait here until the receiver is no more.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		drbd_thread_stop(&adm_ctx.tconn->receiver);
=======
		drbd_thread_stop(&connection->receiver);
>>>>>>> v3.18
=======
		drbd_thread_stop(&connection->receiver);
>>>>>>> v3.18

		/* Race breaker.  This additional state change request may be
		 * necessary, if this was a forced disconnect during a receiver
		 * restart.  We may have "killed" the receiver thread just
<<<<<<< HEAD
<<<<<<< HEAD
		 * after drbdd_init() returned.  Typically, we should be
		 * C_STANDALONE already, now, and this becomes a no-op.
		 */
		rv2 = conn_request_state(tconn, NS(conn, C_STANDALONE),
				CS_VERBOSE | CS_HARD);
		if (rv2 < SS_SUCCESS)
			conn_err(tconn,
=======
=======
>>>>>>> v3.18
		 * after drbd_receiver() returned.  Typically, we should be
		 * C_STANDALONE already, now, and this becomes a no-op.
		 */
		rv2 = conn_request_state(connection, NS(conn, C_STANDALONE),
				CS_VERBOSE | CS_HARD);
		if (rv2 < SS_SUCCESS)
			drbd_err(connection,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
				"unexpected rv2=%d in conn_try_disconnect()\n",
				rv2);
	}
	return rv;
}

int drbd_adm_disconnect(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct disconnect_parms parms;
	struct drbd_tconn *tconn;
=======
	struct drbd_config_context adm_ctx;
	struct disconnect_parms parms;
	struct drbd_connection *connection;
>>>>>>> v3.18
=======
	struct drbd_config_context adm_ctx;
	struct disconnect_parms parms;
	struct drbd_connection *connection;
>>>>>>> v3.18
	enum drbd_state_rv rv;
	enum drbd_ret_code retcode;
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_CONNECTION);
=======
	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_CONNECTION);
>>>>>>> v3.18
=======
	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_CONNECTION);
>>>>>>> v3.18
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto fail;

<<<<<<< HEAD
<<<<<<< HEAD
	tconn = adm_ctx.tconn;
=======
	connection = adm_ctx.connection;
>>>>>>> v3.18
=======
	connection = adm_ctx.connection;
>>>>>>> v3.18
	memset(&parms, 0, sizeof(parms));
	if (info->attrs[DRBD_NLA_DISCONNECT_PARMS]) {
		err = disconnect_parms_from_attrs(&parms, info);
		if (err) {
			retcode = ERR_MANDATORY_TAG;
<<<<<<< HEAD
<<<<<<< HEAD
			drbd_msg_put_info(from_attrs_err_to_txt(err));
=======
			drbd_msg_put_info(adm_ctx.reply_skb, from_attrs_err_to_txt(err));
>>>>>>> v3.18
=======
			drbd_msg_put_info(adm_ctx.reply_skb, from_attrs_err_to_txt(err));
>>>>>>> v3.18
			goto fail;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	rv = conn_try_disconnect(tconn, parms.force_disconnect);
=======
	mutex_lock(&adm_ctx.resource->adm_mutex);
	rv = conn_try_disconnect(connection, parms.force_disconnect);
>>>>>>> v3.18
=======
	mutex_lock(&adm_ctx.resource->adm_mutex);
	rv = conn_try_disconnect(connection, parms.force_disconnect);
>>>>>>> v3.18
	if (rv < SS_SUCCESS)
		retcode = rv;  /* FIXME: Type mismatch. */
	else
		retcode = NO_ERROR;
<<<<<<< HEAD
<<<<<<< HEAD
 fail:
	drbd_adm_finish(info, retcode);
	return 0;
}

void resync_after_online_grow(struct drbd_conf *mdev)
{
	int iass; /* I am sync source */

	dev_info(DEV, "Resync of new storage after online grow\n");
	if (mdev->state.role != mdev->state.peer)
		iass = (mdev->state.role == R_PRIMARY);
	else
		iass = test_bit(RESOLVE_CONFLICTS, &mdev->tconn->flags);

	if (iass)
		drbd_start_resync(mdev, C_SYNC_SOURCE);
	else
		_drbd_request_state(mdev, NS(conn, C_WF_SYNC_UUID), CS_VERBOSE + CS_SERIALIZE);
=======
=======
>>>>>>> v3.18
	mutex_unlock(&adm_ctx.resource->adm_mutex);
 fail:
	drbd_adm_finish(&adm_ctx, info, retcode);
	return 0;
}

void resync_after_online_grow(struct drbd_device *device)
{
	int iass; /* I am sync source */

	drbd_info(device, "Resync of new storage after online grow\n");
	if (device->state.role != device->state.peer)
		iass = (device->state.role == R_PRIMARY);
	else
		iass = test_bit(RESOLVE_CONFLICTS, &first_peer_device(device)->connection->flags);

	if (iass)
		drbd_start_resync(device, C_SYNC_SOURCE);
	else
		_drbd_request_state(device, NS(conn, C_WF_SYNC_UUID), CS_VERBOSE + CS_SERIALIZE);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

int drbd_adm_resize(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct disk_conf *old_disk_conf, *new_disk_conf = NULL;
	struct resize_parms rs;
	struct drbd_conf *mdev;
	enum drbd_ret_code retcode;
	enum determine_dev_size dd;
=======
=======
>>>>>>> v3.18
	struct drbd_config_context adm_ctx;
	struct disk_conf *old_disk_conf, *new_disk_conf = NULL;
	struct resize_parms rs;
	struct drbd_device *device;
	enum drbd_ret_code retcode;
	enum determine_dev_size dd;
	bool change_al_layout = false;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	enum dds_flags ddsf;
	sector_t u_size;
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto fail;

	memset(&rs, 0, sizeof(struct resize_parms));
=======
=======
>>>>>>> v3.18
	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto finish;

	mutex_lock(&adm_ctx.resource->adm_mutex);
	device = adm_ctx.device;
	if (!get_ldev(device)) {
		retcode = ERR_NO_DISK;
		goto fail;
	}

	memset(&rs, 0, sizeof(struct resize_parms));
	rs.al_stripes = device->ldev->md.al_stripes;
	rs.al_stripe_size = device->ldev->md.al_stripe_size_4k * 4;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (info->attrs[DRBD_NLA_RESIZE_PARMS]) {
		err = resize_parms_from_attrs(&rs, info);
		if (err) {
			retcode = ERR_MANDATORY_TAG;
<<<<<<< HEAD
<<<<<<< HEAD
			drbd_msg_put_info(from_attrs_err_to_txt(err));
			goto fail;
		}
	}

	mdev = adm_ctx.mdev;
	if (mdev->state.conn > C_CONNECTED) {
		retcode = ERR_RESIZE_RESYNC;
		goto fail;
	}

	if (mdev->state.role == R_SECONDARY &&
	    mdev->state.peer == R_SECONDARY) {
		retcode = ERR_NO_PRIMARY;
		goto fail;
	}

	if (!get_ldev(mdev)) {
		retcode = ERR_NO_DISK;
		goto fail;
	}

	if (rs.no_resync && mdev->tconn->agreed_pro_version < 93) {
=======
=======
>>>>>>> v3.18
			drbd_msg_put_info(adm_ctx.reply_skb, from_attrs_err_to_txt(err));
			goto fail_ldev;
		}
	}

	if (device->state.conn > C_CONNECTED) {
		retcode = ERR_RESIZE_RESYNC;
		goto fail_ldev;
	}

	if (device->state.role == R_SECONDARY &&
	    device->state.peer == R_SECONDARY) {
		retcode = ERR_NO_PRIMARY;
		goto fail_ldev;
	}

	if (rs.no_resync && first_peer_device(device)->connection->agreed_pro_version < 93) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		retcode = ERR_NEED_APV_93;
		goto fail_ldev;
	}

	rcu_read_lock();
<<<<<<< HEAD
<<<<<<< HEAD
	u_size = rcu_dereference(mdev->ldev->disk_conf)->disk_size;
=======
	u_size = rcu_dereference(device->ldev->disk_conf)->disk_size;
>>>>>>> v3.18
=======
	u_size = rcu_dereference(device->ldev->disk_conf)->disk_size;
>>>>>>> v3.18
	rcu_read_unlock();
	if (u_size != (sector_t)rs.resize_size) {
		new_disk_conf = kmalloc(sizeof(struct disk_conf), GFP_KERNEL);
		if (!new_disk_conf) {
			retcode = ERR_NOMEM;
			goto fail_ldev;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (mdev->ldev->known_size != drbd_get_capacity(mdev->ldev->backing_bdev))
		mdev->ldev->known_size = drbd_get_capacity(mdev->ldev->backing_bdev);

	if (new_disk_conf) {
		mutex_lock(&mdev->tconn->conf_update);
		old_disk_conf = mdev->ldev->disk_conf;
		*new_disk_conf = *old_disk_conf;
		new_disk_conf->disk_size = (sector_t)rs.resize_size;
		rcu_assign_pointer(mdev->ldev->disk_conf, new_disk_conf);
		mutex_unlock(&mdev->tconn->conf_update);
=======
=======
>>>>>>> v3.18
	if (device->ldev->md.al_stripes != rs.al_stripes ||
	    device->ldev->md.al_stripe_size_4k != rs.al_stripe_size / 4) {
		u32 al_size_k = rs.al_stripes * rs.al_stripe_size;

		if (al_size_k > (16 * 1024 * 1024)) {
			retcode = ERR_MD_LAYOUT_TOO_BIG;
			goto fail_ldev;
		}

		if (al_size_k < MD_32kB_SECT/2) {
			retcode = ERR_MD_LAYOUT_TOO_SMALL;
			goto fail_ldev;
		}

		if (device->state.conn != C_CONNECTED && !rs.resize_force) {
			retcode = ERR_MD_LAYOUT_CONNECTED;
			goto fail_ldev;
		}

		change_al_layout = true;
	}

	if (device->ldev->known_size != drbd_get_capacity(device->ldev->backing_bdev))
		device->ldev->known_size = drbd_get_capacity(device->ldev->backing_bdev);

	if (new_disk_conf) {
		mutex_lock(&device->resource->conf_update);
		old_disk_conf = device->ldev->disk_conf;
		*new_disk_conf = *old_disk_conf;
		new_disk_conf->disk_size = (sector_t)rs.resize_size;
		rcu_assign_pointer(device->ldev->disk_conf, new_disk_conf);
		mutex_unlock(&device->resource->conf_update);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		synchronize_rcu();
		kfree(old_disk_conf);
	}

	ddsf = (rs.resize_force ? DDSF_FORCED : 0) | (rs.no_resync ? DDSF_NO_RESYNC : 0);
<<<<<<< HEAD
<<<<<<< HEAD
	dd = drbd_determine_dev_size(mdev, ddsf);
	drbd_md_sync(mdev);
	put_ldev(mdev);
	if (dd == dev_size_error) {
		retcode = ERR_NOMEM_BITMAP;
		goto fail;
	}

	if (mdev->state.conn == C_CONNECTED) {
		if (dd == grew)
			set_bit(RESIZE_PENDING, &mdev->flags);

		drbd_send_uuids(mdev);
		drbd_send_sizes(mdev, 1, ddsf);
	}

 fail:
	drbd_adm_finish(info, retcode);
	return 0;

 fail_ldev:
	put_ldev(mdev);
=======
=======
>>>>>>> v3.18
	dd = drbd_determine_dev_size(device, ddsf, change_al_layout ? &rs : NULL);
	drbd_md_sync(device);
	put_ldev(device);
	if (dd == DS_ERROR) {
		retcode = ERR_NOMEM_BITMAP;
		goto fail;
	} else if (dd == DS_ERROR_SPACE_MD) {
		retcode = ERR_MD_LAYOUT_NO_FIT;
		goto fail;
	} else if (dd == DS_ERROR_SHRINK) {
		retcode = ERR_IMPLICIT_SHRINK;
		goto fail;
	}

	if (device->state.conn == C_CONNECTED) {
		if (dd == DS_GREW)
			set_bit(RESIZE_PENDING, &device->flags);

		drbd_send_uuids(first_peer_device(device));
		drbd_send_sizes(first_peer_device(device), 1, ddsf);
	}

 fail:
	mutex_unlock(&adm_ctx.resource->adm_mutex);
 finish:
	drbd_adm_finish(&adm_ctx, info, retcode);
	return 0;

 fail_ldev:
	put_ldev(device);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	goto fail;
}

int drbd_adm_resource_opts(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
	enum drbd_ret_code retcode;
	struct drbd_tconn *tconn;
	struct res_opts res_opts;
	int err;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_RESOURCE);
=======
=======
>>>>>>> v3.18
	struct drbd_config_context adm_ctx;
	enum drbd_ret_code retcode;
	struct res_opts res_opts;
	int err;

	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_RESOURCE);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto fail;
<<<<<<< HEAD
<<<<<<< HEAD
	tconn = adm_ctx.tconn;

	res_opts = tconn->res_opts;
=======

	res_opts = adm_ctx.resource->res_opts;
>>>>>>> v3.18
=======

	res_opts = adm_ctx.resource->res_opts;
>>>>>>> v3.18
	if (should_set_defaults(info))
		set_res_opts_defaults(&res_opts);

	err = res_opts_from_attrs(&res_opts, info);
	if (err && err != -ENOMSG) {
		retcode = ERR_MANDATORY_TAG;
<<<<<<< HEAD
<<<<<<< HEAD
		drbd_msg_put_info(from_attrs_err_to_txt(err));
		goto fail;
	}

	err = set_resource_options(tconn, &res_opts);
=======
=======
>>>>>>> v3.18
		drbd_msg_put_info(adm_ctx.reply_skb, from_attrs_err_to_txt(err));
		goto fail;
	}

	mutex_lock(&adm_ctx.resource->adm_mutex);
	err = set_resource_options(adm_ctx.resource, &res_opts);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (err) {
		retcode = ERR_INVALID_REQUEST;
		if (err == -ENOMEM)
			retcode = ERR_NOMEM;
	}
<<<<<<< HEAD
<<<<<<< HEAD

fail:
	drbd_adm_finish(info, retcode);
=======
=======
>>>>>>> v3.18
	mutex_unlock(&adm_ctx.resource->adm_mutex);

fail:
	drbd_adm_finish(&adm_ctx, info, retcode);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
}

int drbd_adm_invalidate(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct drbd_conf *mdev;
	int retcode; /* enum drbd_ret_code rsp. enum drbd_state_rv */

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
=======
=======
>>>>>>> v3.18
	struct drbd_config_context adm_ctx;
	struct drbd_device *device;
	int retcode; /* enum drbd_ret_code rsp. enum drbd_state_rv */

	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

<<<<<<< HEAD
<<<<<<< HEAD
	mdev = adm_ctx.mdev;
=======
=======
>>>>>>> v3.18
	device = adm_ctx.device;
	if (!get_ldev(device)) {
		retcode = ERR_NO_DISK;
		goto out;
	}

	mutex_lock(&adm_ctx.resource->adm_mutex);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* If there is still bitmap IO pending, probably because of a previous
	 * resync just being finished, wait for it before requesting a new resync.
	 * Also wait for it's after_state_ch(). */
<<<<<<< HEAD
<<<<<<< HEAD
	drbd_suspend_io(mdev);
	wait_event(mdev->misc_wait, !test_bit(BITMAP_IO, &mdev->flags));
	drbd_flush_workqueue(mdev);
=======
	drbd_suspend_io(device);
	wait_event(device->misc_wait, !test_bit(BITMAP_IO, &device->flags));
	drbd_flush_workqueue(&first_peer_device(device)->connection->sender_work);
>>>>>>> v3.18
=======
	drbd_suspend_io(device);
	wait_event(device->misc_wait, !test_bit(BITMAP_IO, &device->flags));
	drbd_flush_workqueue(&first_peer_device(device)->connection->sender_work);
>>>>>>> v3.18

	/* If we happen to be C_STANDALONE R_SECONDARY, just change to
	 * D_INCONSISTENT, and set all bits in the bitmap.  Otherwise,
	 * try to start a resync handshake as sync target for full sync.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (mdev->state.conn == C_STANDALONE && mdev->state.role == R_SECONDARY) {
		retcode = drbd_request_state(mdev, NS(disk, D_INCONSISTENT));
		if (retcode >= SS_SUCCESS) {
			if (drbd_bitmap_io(mdev, &drbd_bmio_set_n_write,
=======
=======
>>>>>>> v3.18
	if (device->state.conn == C_STANDALONE && device->state.role == R_SECONDARY) {
		retcode = drbd_request_state(device, NS(disk, D_INCONSISTENT));
		if (retcode >= SS_SUCCESS) {
			if (drbd_bitmap_io(device, &drbd_bmio_set_n_write,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
				"set_n_write from invalidate", BM_LOCKED_MASK))
				retcode = ERR_IO_MD_DISK;
		}
	} else
<<<<<<< HEAD
<<<<<<< HEAD
		retcode = drbd_request_state(mdev, NS(conn, C_STARTING_SYNC_T));
	drbd_resume_io(mdev);

out:
	drbd_adm_finish(info, retcode);
=======
=======
>>>>>>> v3.18
		retcode = drbd_request_state(device, NS(conn, C_STARTING_SYNC_T));
	drbd_resume_io(device);
	mutex_unlock(&adm_ctx.resource->adm_mutex);
	put_ldev(device);
out:
	drbd_adm_finish(&adm_ctx, info, retcode);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
}

static int drbd_adm_simple_request_state(struct sk_buff *skb, struct genl_info *info,
		union drbd_state mask, union drbd_state val)
{
<<<<<<< HEAD
<<<<<<< HEAD
	enum drbd_ret_code retcode;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
=======
=======
>>>>>>> v3.18
	struct drbd_config_context adm_ctx;
	enum drbd_ret_code retcode;

	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

<<<<<<< HEAD
<<<<<<< HEAD
	retcode = drbd_request_state(adm_ctx.mdev, mask, val);
out:
	drbd_adm_finish(info, retcode);
	return 0;
}

static int drbd_bmio_set_susp_al(struct drbd_conf *mdev)
{
	int rv;

	rv = drbd_bmio_set_n_write(mdev);
	drbd_suspend_al(mdev);
=======
=======
>>>>>>> v3.18
	mutex_lock(&adm_ctx.resource->adm_mutex);
	retcode = drbd_request_state(adm_ctx.device, mask, val);
	mutex_unlock(&adm_ctx.resource->adm_mutex);
out:
	drbd_adm_finish(&adm_ctx, info, retcode);
	return 0;
}

static int drbd_bmio_set_susp_al(struct drbd_device *device) __must_hold(local)
{
	int rv;

	rv = drbd_bmio_set_n_write(device);
	drbd_suspend_al(device);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return rv;
}

int drbd_adm_invalidate_peer(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int retcode; /* drbd_ret_code, drbd_state_rv */
	struct drbd_conf *mdev;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
=======
=======
>>>>>>> v3.18
	struct drbd_config_context adm_ctx;
	int retcode; /* drbd_ret_code, drbd_state_rv */
	struct drbd_device *device;

	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

<<<<<<< HEAD
<<<<<<< HEAD
	mdev = adm_ctx.mdev;
=======
=======
>>>>>>> v3.18
	device = adm_ctx.device;
	if (!get_ldev(device)) {
		retcode = ERR_NO_DISK;
		goto out;
	}

	mutex_lock(&adm_ctx.resource->adm_mutex);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* If there is still bitmap IO pending, probably because of a previous
	 * resync just being finished, wait for it before requesting a new resync.
	 * Also wait for it's after_state_ch(). */
<<<<<<< HEAD
<<<<<<< HEAD
	drbd_suspend_io(mdev);
	wait_event(mdev->misc_wait, !test_bit(BITMAP_IO, &mdev->flags));
	drbd_flush_workqueue(mdev);
=======
	drbd_suspend_io(device);
	wait_event(device->misc_wait, !test_bit(BITMAP_IO, &device->flags));
	drbd_flush_workqueue(&first_peer_device(device)->connection->sender_work);
>>>>>>> v3.18
=======
	drbd_suspend_io(device);
	wait_event(device->misc_wait, !test_bit(BITMAP_IO, &device->flags));
	drbd_flush_workqueue(&first_peer_device(device)->connection->sender_work);
>>>>>>> v3.18

	/* If we happen to be C_STANDALONE R_PRIMARY, just set all bits
	 * in the bitmap.  Otherwise, try to start a resync handshake
	 * as sync source for full sync.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (mdev->state.conn == C_STANDALONE && mdev->state.role == R_PRIMARY) {
		/* The peer will get a resync upon connect anyways. Just make that
		   into a full resync. */
		retcode = drbd_request_state(mdev, NS(pdsk, D_INCONSISTENT));
		if (retcode >= SS_SUCCESS) {
			if (drbd_bitmap_io(mdev, &drbd_bmio_set_susp_al,
=======
=======
>>>>>>> v3.18
	if (device->state.conn == C_STANDALONE && device->state.role == R_PRIMARY) {
		/* The peer will get a resync upon connect anyways. Just make that
		   into a full resync. */
		retcode = drbd_request_state(device, NS(pdsk, D_INCONSISTENT));
		if (retcode >= SS_SUCCESS) {
			if (drbd_bitmap_io(device, &drbd_bmio_set_susp_al,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
				"set_n_write from invalidate_peer",
				BM_LOCKED_SET_ALLOWED))
				retcode = ERR_IO_MD_DISK;
		}
	} else
<<<<<<< HEAD
<<<<<<< HEAD
		retcode = drbd_request_state(mdev, NS(conn, C_STARTING_SYNC_S));
	drbd_resume_io(mdev);

out:
	drbd_adm_finish(info, retcode);
=======
=======
>>>>>>> v3.18
		retcode = drbd_request_state(device, NS(conn, C_STARTING_SYNC_S));
	drbd_resume_io(device);
	mutex_unlock(&adm_ctx.resource->adm_mutex);
	put_ldev(device);
out:
	drbd_adm_finish(&adm_ctx, info, retcode);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
}

int drbd_adm_pause_sync(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
	enum drbd_ret_code retcode;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
=======
=======
>>>>>>> v3.18
	struct drbd_config_context adm_ctx;
	enum drbd_ret_code retcode;

	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

<<<<<<< HEAD
<<<<<<< HEAD
	if (drbd_request_state(adm_ctx.mdev, NS(user_isp, 1)) == SS_NOTHING_TO_DO)
		retcode = ERR_PAUSE_IS_SET;
out:
	drbd_adm_finish(info, retcode);
=======
=======
>>>>>>> v3.18
	mutex_lock(&adm_ctx.resource->adm_mutex);
	if (drbd_request_state(adm_ctx.device, NS(user_isp, 1)) == SS_NOTHING_TO_DO)
		retcode = ERR_PAUSE_IS_SET;
	mutex_unlock(&adm_ctx.resource->adm_mutex);
out:
	drbd_adm_finish(&adm_ctx, info, retcode);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
}

int drbd_adm_resume_sync(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
	union drbd_dev_state s;
	enum drbd_ret_code retcode;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
=======
=======
>>>>>>> v3.18
	struct drbd_config_context adm_ctx;
	union drbd_dev_state s;
	enum drbd_ret_code retcode;

	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

<<<<<<< HEAD
<<<<<<< HEAD
	if (drbd_request_state(adm_ctx.mdev, NS(user_isp, 0)) == SS_NOTHING_TO_DO) {
		s = adm_ctx.mdev->state;
=======
	mutex_lock(&adm_ctx.resource->adm_mutex);
	if (drbd_request_state(adm_ctx.device, NS(user_isp, 0)) == SS_NOTHING_TO_DO) {
		s = adm_ctx.device->state;
>>>>>>> v3.18
=======
	mutex_lock(&adm_ctx.resource->adm_mutex);
	if (drbd_request_state(adm_ctx.device, NS(user_isp, 0)) == SS_NOTHING_TO_DO) {
		s = adm_ctx.device->state;
>>>>>>> v3.18
		if (s.conn == C_PAUSED_SYNC_S || s.conn == C_PAUSED_SYNC_T) {
			retcode = s.aftr_isp ? ERR_PIC_AFTER_DEP :
				  s.peer_isp ? ERR_PIC_PEER_DEP : ERR_PAUSE_IS_CLEAR;
		} else {
			retcode = ERR_PAUSE_IS_CLEAR;
		}
	}
<<<<<<< HEAD
<<<<<<< HEAD

out:
	drbd_adm_finish(info, retcode);
=======
	mutex_unlock(&adm_ctx.resource->adm_mutex);
out:
	drbd_adm_finish(&adm_ctx, info, retcode);
>>>>>>> v3.18
=======
	mutex_unlock(&adm_ctx.resource->adm_mutex);
out:
	drbd_adm_finish(&adm_ctx, info, retcode);
>>>>>>> v3.18
	return 0;
}

int drbd_adm_suspend_io(struct sk_buff *skb, struct genl_info *info)
{
	return drbd_adm_simple_request_state(skb, info, NS(susp, 1));
}

int drbd_adm_resume_io(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct drbd_conf *mdev;
	int retcode; /* enum drbd_ret_code rsp. enum drbd_state_rv */

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
=======
=======
>>>>>>> v3.18
	struct drbd_config_context adm_ctx;
	struct drbd_device *device;
	int retcode; /* enum drbd_ret_code rsp. enum drbd_state_rv */

	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

<<<<<<< HEAD
<<<<<<< HEAD
	mdev = adm_ctx.mdev;
	if (test_bit(NEW_CUR_UUID, &mdev->flags)) {
		drbd_uuid_new_current(mdev);
		clear_bit(NEW_CUR_UUID, &mdev->flags);
	}
	drbd_suspend_io(mdev);
	retcode = drbd_request_state(mdev, NS3(susp, 0, susp_nod, 0, susp_fen, 0));
	if (retcode == SS_SUCCESS) {
		if (mdev->state.conn < C_CONNECTED)
			tl_clear(mdev->tconn);
		if (mdev->state.disk == D_DISKLESS || mdev->state.disk == D_FAILED)
			tl_restart(mdev->tconn, FAIL_FROZEN_DISK_IO);
	}
	drbd_resume_io(mdev);

out:
	drbd_adm_finish(info, retcode);
=======
=======
>>>>>>> v3.18
	mutex_lock(&adm_ctx.resource->adm_mutex);
	device = adm_ctx.device;
	if (test_bit(NEW_CUR_UUID, &device->flags)) {
		drbd_uuid_new_current(device);
		clear_bit(NEW_CUR_UUID, &device->flags);
	}
	drbd_suspend_io(device);
	retcode = drbd_request_state(device, NS3(susp, 0, susp_nod, 0, susp_fen, 0));
	if (retcode == SS_SUCCESS) {
		if (device->state.conn < C_CONNECTED)
			tl_clear(first_peer_device(device)->connection);
		if (device->state.disk == D_DISKLESS || device->state.disk == D_FAILED)
			tl_restart(first_peer_device(device)->connection, FAIL_FROZEN_DISK_IO);
	}
	drbd_resume_io(device);
	mutex_unlock(&adm_ctx.resource->adm_mutex);
out:
	drbd_adm_finish(&adm_ctx, info, retcode);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
}

int drbd_adm_outdate(struct sk_buff *skb, struct genl_info *info)
{
	return drbd_adm_simple_request_state(skb, info, NS(disk, D_OUTDATED));
}

<<<<<<< HEAD
<<<<<<< HEAD
int nla_put_drbd_cfg_context(struct sk_buff *skb, struct drbd_tconn *tconn, unsigned vnr)
=======
=======
>>>>>>> v3.18
static int nla_put_drbd_cfg_context(struct sk_buff *skb,
				    struct drbd_resource *resource,
				    struct drbd_connection *connection,
				    struct drbd_device *device)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	struct nlattr *nla;
	nla = nla_nest_start(skb, DRBD_NLA_CFG_CONTEXT);
	if (!nla)
		goto nla_put_failure;
<<<<<<< HEAD
<<<<<<< HEAD
	if (vnr != VOLUME_UNSPECIFIED &&
	    nla_put_u32(skb, T_ctx_volume, vnr))
		goto nla_put_failure;
	if (nla_put_string(skb, T_ctx_resource_name, tconn->name))
		goto nla_put_failure;
	if (tconn->my_addr_len &&
	    nla_put(skb, T_ctx_my_addr, tconn->my_addr_len, &tconn->my_addr))
		goto nla_put_failure;
	if (tconn->peer_addr_len &&
	    nla_put(skb, T_ctx_peer_addr, tconn->peer_addr_len, &tconn->peer_addr))
		goto nla_put_failure;
=======
=======
>>>>>>> v3.18
	if (device &&
	    nla_put_u32(skb, T_ctx_volume, device->vnr))
		goto nla_put_failure;
	if (nla_put_string(skb, T_ctx_resource_name, resource->name))
		goto nla_put_failure;
	if (connection) {
		if (connection->my_addr_len &&
		    nla_put(skb, T_ctx_my_addr, connection->my_addr_len, &connection->my_addr))
			goto nla_put_failure;
		if (connection->peer_addr_len &&
		    nla_put(skb, T_ctx_peer_addr, connection->peer_addr_len, &connection->peer_addr))
			goto nla_put_failure;
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	nla_nest_end(skb, nla);
	return 0;

nla_put_failure:
	if (nla)
		nla_nest_cancel(skb, nla);
	return -EMSGSIZE;
}

<<<<<<< HEAD
<<<<<<< HEAD
int nla_put_status_info(struct sk_buff *skb, struct drbd_conf *mdev,
		const struct sib_info *sib)
{
	struct state_info *si = NULL; /* for sizeof(si->member); */
	struct net_conf *nc;
=======
=======
>>>>>>> v3.18
/*
 * Return the connection of @resource if @resource has exactly one connection.
 */
static struct drbd_connection *the_only_connection(struct drbd_resource *resource)
{
	struct list_head *connections = &resource->connections;

	if (list_empty(connections) || connections->next->next != connections)
		return NULL;
	return list_first_entry(&resource->connections, struct drbd_connection, connections);
}

static int nla_put_status_info(struct sk_buff *skb, struct drbd_device *device,
		const struct sib_info *sib)
{
	struct drbd_resource *resource = device->resource;
	struct state_info *si = NULL; /* for sizeof(si->member); */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	struct nlattr *nla;
	int got_ldev;
	int err = 0;
	int exclude_sensitive;

	/* If sib != NULL, this is drbd_bcast_event, which anyone can listen
	 * to.  So we better exclude_sensitive information.
	 *
	 * If sib == NULL, this is drbd_adm_get_status, executed synchronously
	 * in the context of the requesting user process. Exclude sensitive
	 * information, unless current has superuser.
	 *
	 * NOTE: for drbd_adm_get_status_all(), this is a netlink dump, and
	 * relies on the current implementation of netlink_dump(), which
	 * executes the dump callback successively from netlink_recvmsg(),
	 * always in the context of the receiving process */
	exclude_sensitive = sib || !capable(CAP_SYS_ADMIN);

<<<<<<< HEAD
<<<<<<< HEAD
	got_ldev = get_ldev(mdev);

	/* We need to add connection name and volume number information still.
	 * Minor number is in drbd_genlmsghdr. */
	if (nla_put_drbd_cfg_context(skb, mdev->tconn, mdev->vnr))
		goto nla_put_failure;

	if (res_opts_to_skb(skb, &mdev->tconn->res_opts, exclude_sensitive))
		goto nla_put_failure;

	rcu_read_lock();
	if (got_ldev)
		if (disk_conf_to_skb(skb, rcu_dereference(mdev->ldev->disk_conf), exclude_sensitive))
			goto nla_put_failure;

	nc = rcu_dereference(mdev->tconn->net_conf);
	if (nc)
		err = net_conf_to_skb(skb, nc, exclude_sensitive);
=======
=======
>>>>>>> v3.18
	got_ldev = get_ldev(device);

	/* We need to add connection name and volume number information still.
	 * Minor number is in drbd_genlmsghdr. */
	if (nla_put_drbd_cfg_context(skb, resource, the_only_connection(resource), device))
		goto nla_put_failure;

	if (res_opts_to_skb(skb, &device->resource->res_opts, exclude_sensitive))
		goto nla_put_failure;

	rcu_read_lock();
	if (got_ldev) {
		struct disk_conf *disk_conf;

		disk_conf = rcu_dereference(device->ldev->disk_conf);
		err = disk_conf_to_skb(skb, disk_conf, exclude_sensitive);
	}
	if (!err) {
		struct net_conf *nc;

		nc = rcu_dereference(first_peer_device(device)->connection->net_conf);
		if (nc)
			err = net_conf_to_skb(skb, nc, exclude_sensitive);
	}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	rcu_read_unlock();
	if (err)
		goto nla_put_failure;

	nla = nla_nest_start(skb, DRBD_NLA_STATE_INFO);
	if (!nla)
		goto nla_put_failure;
	if (nla_put_u32(skb, T_sib_reason, sib ? sib->sib_reason : SIB_GET_STATUS_REPLY) ||
<<<<<<< HEAD
<<<<<<< HEAD
	    nla_put_u32(skb, T_current_state, mdev->state.i) ||
	    nla_put_u64(skb, T_ed_uuid, mdev->ed_uuid) ||
	    nla_put_u64(skb, T_capacity, drbd_get_capacity(mdev->this_bdev)) ||
	    nla_put_u64(skb, T_send_cnt, mdev->send_cnt) ||
	    nla_put_u64(skb, T_recv_cnt, mdev->recv_cnt) ||
	    nla_put_u64(skb, T_read_cnt, mdev->read_cnt) ||
	    nla_put_u64(skb, T_writ_cnt, mdev->writ_cnt) ||
	    nla_put_u64(skb, T_al_writ_cnt, mdev->al_writ_cnt) ||
	    nla_put_u64(skb, T_bm_writ_cnt, mdev->bm_writ_cnt) ||
	    nla_put_u32(skb, T_ap_bio_cnt, atomic_read(&mdev->ap_bio_cnt)) ||
	    nla_put_u32(skb, T_ap_pending_cnt, atomic_read(&mdev->ap_pending_cnt)) ||
	    nla_put_u32(skb, T_rs_pending_cnt, atomic_read(&mdev->rs_pending_cnt)))
=======
=======
>>>>>>> v3.18
	    nla_put_u32(skb, T_current_state, device->state.i) ||
	    nla_put_u64(skb, T_ed_uuid, device->ed_uuid) ||
	    nla_put_u64(skb, T_capacity, drbd_get_capacity(device->this_bdev)) ||
	    nla_put_u64(skb, T_send_cnt, device->send_cnt) ||
	    nla_put_u64(skb, T_recv_cnt, device->recv_cnt) ||
	    nla_put_u64(skb, T_read_cnt, device->read_cnt) ||
	    nla_put_u64(skb, T_writ_cnt, device->writ_cnt) ||
	    nla_put_u64(skb, T_al_writ_cnt, device->al_writ_cnt) ||
	    nla_put_u64(skb, T_bm_writ_cnt, device->bm_writ_cnt) ||
	    nla_put_u32(skb, T_ap_bio_cnt, atomic_read(&device->ap_bio_cnt)) ||
	    nla_put_u32(skb, T_ap_pending_cnt, atomic_read(&device->ap_pending_cnt)) ||
	    nla_put_u32(skb, T_rs_pending_cnt, atomic_read(&device->rs_pending_cnt)))
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		goto nla_put_failure;

	if (got_ldev) {
		int err;

<<<<<<< HEAD
<<<<<<< HEAD
		spin_lock_irq(&mdev->ldev->md.uuid_lock);
		err = nla_put(skb, T_uuids, sizeof(si->uuids), mdev->ldev->md.uuid);
		spin_unlock_irq(&mdev->ldev->md.uuid_lock);
=======
		spin_lock_irq(&device->ldev->md.uuid_lock);
		err = nla_put(skb, T_uuids, sizeof(si->uuids), device->ldev->md.uuid);
		spin_unlock_irq(&device->ldev->md.uuid_lock);
>>>>>>> v3.18
=======
		spin_lock_irq(&device->ldev->md.uuid_lock);
		err = nla_put(skb, T_uuids, sizeof(si->uuids), device->ldev->md.uuid);
		spin_unlock_irq(&device->ldev->md.uuid_lock);
>>>>>>> v3.18

		if (err)
			goto nla_put_failure;

<<<<<<< HEAD
<<<<<<< HEAD
		if (nla_put_u32(skb, T_disk_flags, mdev->ldev->md.flags) ||
		    nla_put_u64(skb, T_bits_total, drbd_bm_bits(mdev)) ||
		    nla_put_u64(skb, T_bits_oos, drbd_bm_total_weight(mdev)))
			goto nla_put_failure;
		if (C_SYNC_SOURCE <= mdev->state.conn &&
		    C_PAUSED_SYNC_T >= mdev->state.conn) {
			if (nla_put_u64(skb, T_bits_rs_total, mdev->rs_total) ||
			    nla_put_u64(skb, T_bits_rs_failed, mdev->rs_failed))
=======
=======
>>>>>>> v3.18
		if (nla_put_u32(skb, T_disk_flags, device->ldev->md.flags) ||
		    nla_put_u64(skb, T_bits_total, drbd_bm_bits(device)) ||
		    nla_put_u64(skb, T_bits_oos, drbd_bm_total_weight(device)))
			goto nla_put_failure;
		if (C_SYNC_SOURCE <= device->state.conn &&
		    C_PAUSED_SYNC_T >= device->state.conn) {
			if (nla_put_u64(skb, T_bits_rs_total, device->rs_total) ||
			    nla_put_u64(skb, T_bits_rs_failed, device->rs_failed))
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
				goto nla_put_failure;
		}
	}

	if (sib) {
		switch(sib->sib_reason) {
		case SIB_SYNC_PROGRESS:
		case SIB_GET_STATUS_REPLY:
			break;
		case SIB_STATE_CHANGE:
			if (nla_put_u32(skb, T_prev_state, sib->os.i) ||
			    nla_put_u32(skb, T_new_state, sib->ns.i))
				goto nla_put_failure;
			break;
		case SIB_HELPER_POST:
			if (nla_put_u32(skb, T_helper_exit_code,
					sib->helper_exit_code))
				goto nla_put_failure;
			/* fall through */
		case SIB_HELPER_PRE:
			if (nla_put_string(skb, T_helper, sib->helper_name))
				goto nla_put_failure;
			break;
		}
	}
	nla_nest_end(skb, nla);

	if (0)
nla_put_failure:
		err = -EMSGSIZE;
	if (got_ldev)
<<<<<<< HEAD
<<<<<<< HEAD
		put_ldev(mdev);
=======
		put_ldev(device);
>>>>>>> v3.18
=======
		put_ldev(device);
>>>>>>> v3.18
	return err;
}

int drbd_adm_get_status(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
	enum drbd_ret_code retcode;
	int err;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
=======
=======
>>>>>>> v3.18
	struct drbd_config_context adm_ctx;
	enum drbd_ret_code retcode;
	int err;

	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

<<<<<<< HEAD
<<<<<<< HEAD
	err = nla_put_status_info(adm_ctx.reply_skb, adm_ctx.mdev, NULL);
=======
	err = nla_put_status_info(adm_ctx.reply_skb, adm_ctx.device, NULL);
>>>>>>> v3.18
=======
	err = nla_put_status_info(adm_ctx.reply_skb, adm_ctx.device, NULL);
>>>>>>> v3.18
	if (err) {
		nlmsg_free(adm_ctx.reply_skb);
		return err;
	}
out:
<<<<<<< HEAD
<<<<<<< HEAD
	drbd_adm_finish(info, retcode);
	return 0;
}

int get_one_status(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct drbd_conf *mdev;
	struct drbd_genlmsghdr *dh;
	struct drbd_tconn *pos = (struct drbd_tconn*)cb->args[0];
	struct drbd_tconn *tconn = NULL;
	struct drbd_tconn *tmp;
	unsigned volume = cb->args[1];

	/* Open coded, deferred, iteration:
	 * list_for_each_entry_safe(tconn, tmp, &drbd_tconns, all_tconn) {
	 *	idr_for_each_entry(&tconn->volumes, mdev, i) {
	 *	  ...
	 *	}
	 * }
	 * where tconn is cb->args[0];
=======
=======
>>>>>>> v3.18
	drbd_adm_finish(&adm_ctx, info, retcode);
	return 0;
}

static int get_one_status(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct drbd_device *device;
	struct drbd_genlmsghdr *dh;
	struct drbd_resource *pos = (struct drbd_resource *)cb->args[0];
	struct drbd_resource *resource = NULL;
	struct drbd_resource *tmp;
	unsigned volume = cb->args[1];

	/* Open coded, deferred, iteration:
	 * for_each_resource_safe(resource, tmp, &drbd_resources) {
	 *      connection = "first connection of resource or undefined";
	 *	idr_for_each_entry(&resource->devices, device, i) {
	 *	  ...
	 *	}
	 * }
	 * where resource is cb->args[0];
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	 * and i is cb->args[1];
	 *
	 * cb->args[2] indicates if we shall loop over all resources,
	 * or just dump all volumes of a single resource.
	 *
	 * This may miss entries inserted after this dump started,
	 * or entries deleted before they are reached.
	 *
<<<<<<< HEAD
<<<<<<< HEAD
	 * We need to make sure the mdev won't disappear while
=======
	 * We need to make sure the device won't disappear while
>>>>>>> v3.18
=======
	 * We need to make sure the device won't disappear while
>>>>>>> v3.18
	 * we are looking at it, and revalidate our iterators
	 * on each iteration.
	 */

<<<<<<< HEAD
<<<<<<< HEAD
	/* synchronize with conn_create()/conn_destroy() */
	rcu_read_lock();
	/* revalidate iterator position */
	list_for_each_entry_rcu(tmp, &drbd_tconns, all_tconn) {
		if (pos == NULL) {
			/* first iteration */
			pos = tmp;
			tconn = pos;
			break;
		}
		if (tmp == pos) {
			tconn = pos;
			break;
		}
	}
	if (tconn) {
next_tconn:
		mdev = idr_get_next(&tconn->volumes, &volume);
		if (!mdev) {
			/* No more volumes to dump on this tconn.
			 * Advance tconn iterator. */
			pos = list_entry_rcu(tconn->all_tconn.next,
					     struct drbd_tconn, all_tconn);
			/* Did we dump any volume on this tconn yet? */
=======
=======
>>>>>>> v3.18
	/* synchronize with conn_create()/drbd_destroy_connection() */
	rcu_read_lock();
	/* revalidate iterator position */
	for_each_resource_rcu(tmp, &drbd_resources) {
		if (pos == NULL) {
			/* first iteration */
			pos = tmp;
			resource = pos;
			break;
		}
		if (tmp == pos) {
			resource = pos;
			break;
		}
	}
	if (resource) {
next_resource:
		device = idr_get_next(&resource->devices, &volume);
		if (!device) {
			/* No more volumes to dump on this resource.
			 * Advance resource iterator. */
			pos = list_entry_rcu(resource->resources.next,
					     struct drbd_resource, resources);
			/* Did we dump any volume of this resource yet? */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			if (volume != 0) {
				/* If we reached the end of the list,
				 * or only a single resource dump was requested,
				 * we are done. */
<<<<<<< HEAD
<<<<<<< HEAD
				if (&pos->all_tconn == &drbd_tconns || cb->args[2])
					goto out;
				volume = 0;
				tconn = pos;
				goto next_tconn;
=======
=======
>>>>>>> v3.18
				if (&pos->resources == &drbd_resources || cb->args[2])
					goto out;
				volume = 0;
				resource = pos;
				goto next_resource;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			}
		}

		dh = genlmsg_put(skb, NETLINK_CB(cb->skb).portid,
				cb->nlh->nlmsg_seq, &drbd_genl_family,
				NLM_F_MULTI, DRBD_ADM_GET_STATUS);
		if (!dh)
			goto out;

<<<<<<< HEAD
<<<<<<< HEAD
		if (!mdev) {
			/* This is a tconn without a single volume.
			 * Suprisingly enough, it may have a network
			 * configuration. */
			struct net_conf *nc;
			dh->minor = -1U;
			dh->ret_code = NO_ERROR;
			if (nla_put_drbd_cfg_context(skb, tconn, VOLUME_UNSPECIFIED))
				goto cancel;
			nc = rcu_dereference(tconn->net_conf);
			if (nc && net_conf_to_skb(skb, nc, 1) != 0)
				goto cancel;
			goto done;
		}

		D_ASSERT(mdev->vnr == volume);
		D_ASSERT(mdev->tconn == tconn);

		dh->minor = mdev_to_minor(mdev);
		dh->ret_code = NO_ERROR;

		if (nla_put_status_info(skb, mdev, NULL)) {
=======
=======
>>>>>>> v3.18
		if (!device) {
			/* This is a connection without a single volume.
			 * Suprisingly enough, it may have a network
			 * configuration. */
			struct drbd_connection *connection;

			dh->minor = -1U;
			dh->ret_code = NO_ERROR;
			connection = the_only_connection(resource);
			if (nla_put_drbd_cfg_context(skb, resource, connection, NULL))
				goto cancel;
			if (connection) {
				struct net_conf *nc;

				nc = rcu_dereference(connection->net_conf);
				if (nc && net_conf_to_skb(skb, nc, 1) != 0)
					goto cancel;
			}
			goto done;
		}

		D_ASSERT(device, device->vnr == volume);
		D_ASSERT(device, device->resource == resource);

		dh->minor = device_to_minor(device);
		dh->ret_code = NO_ERROR;

		if (nla_put_status_info(skb, device, NULL)) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
cancel:
			genlmsg_cancel(skb, dh);
			goto out;
		}
done:
		genlmsg_end(skb, dh);
<<<<<<< HEAD
<<<<<<< HEAD
        }
=======
	}
>>>>>>> v3.18
=======
	}
>>>>>>> v3.18

out:
	rcu_read_unlock();
	/* where to start the next iteration */
<<<<<<< HEAD
<<<<<<< HEAD
        cb->args[0] = (long)pos;
        cb->args[1] = (pos == tconn) ? volume + 1 : 0;

	/* No more tconns/volumes/minors found results in an empty skb.
=======
=======
>>>>>>> v3.18
	cb->args[0] = (long)pos;
	cb->args[1] = (pos == resource) ? volume + 1 : 0;

	/* No more resources/volumes/minors found results in an empty skb.
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	 * Which will terminate the dump. */
        return skb->len;
}

/*
 * Request status of all resources, or of all volumes within a single resource.
 *
 * This is a dump, as the answer may not fit in a single reply skb otherwise.
 * Which means we cannot use the family->attrbuf or other such members, because
 * dump is NOT protected by the genl_lock().  During dump, we only have access
 * to the incoming skb, and need to opencode "parsing" of the nlattr payload.
 *
 * Once things are setup properly, we call into get_one_status().
 */
int drbd_adm_get_status_all(struct sk_buff *skb, struct netlink_callback *cb)
{
	const unsigned hdrlen = GENL_HDRLEN + GENL_MAGIC_FAMILY_HDRSZ;
	struct nlattr *nla;
	const char *resource_name;
<<<<<<< HEAD
<<<<<<< HEAD
	struct drbd_tconn *tconn;
=======
	struct drbd_resource *resource;
>>>>>>> v3.18
=======
	struct drbd_resource *resource;
>>>>>>> v3.18
	int maxtype;

	/* Is this a followup call? */
	if (cb->args[0]) {
		/* ... of a single resource dump,
		 * and the resource iterator has been advanced already? */
		if (cb->args[2] && cb->args[2] != cb->args[0])
			return 0; /* DONE. */
		goto dump;
	}

	/* First call (from netlink_dump_start).  We need to figure out
	 * which resource(s) the user wants us to dump. */
	nla = nla_find(nlmsg_attrdata(cb->nlh, hdrlen),
			nlmsg_attrlen(cb->nlh, hdrlen),
			DRBD_NLA_CFG_CONTEXT);

	/* No explicit context given.  Dump all. */
	if (!nla)
		goto dump;
	maxtype = ARRAY_SIZE(drbd_cfg_context_nl_policy) - 1;
	nla = drbd_nla_find_nested(maxtype, nla, __nla_type(T_ctx_resource_name));
	if (IS_ERR(nla))
		return PTR_ERR(nla);
	/* context given, but no name present? */
	if (!nla)
		return -EINVAL;
	resource_name = nla_data(nla);
<<<<<<< HEAD
<<<<<<< HEAD
	tconn = conn_get_by_name(resource_name);

	if (!tconn)
		return -ENODEV;

	kref_put(&tconn->kref, &conn_destroy); /* get_one_status() (re)validates tconn by itself */

	/* prime iterators, and set "filter" mode mark:
	 * only dump this tconn. */
	cb->args[0] = (long)tconn;
	/* cb->args[1] = 0; passed in this way. */
	cb->args[2] = (long)tconn;
=======
=======
>>>>>>> v3.18
	if (!*resource_name)
		return -ENODEV;
	resource = drbd_find_resource(resource_name);
	if (!resource)
		return -ENODEV;

	kref_put(&resource->kref, drbd_destroy_resource); /* get_one_status() revalidates the resource */

	/* prime iterators, and set "filter" mode mark:
	 * only dump this connection. */
	cb->args[0] = (long)resource;
	/* cb->args[1] = 0; passed in this way. */
	cb->args[2] = (long)resource;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

dump:
	return get_one_status(skb, cb);
}

int drbd_adm_get_timeout_type(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct drbd_config_context adm_ctx;
>>>>>>> v3.18
=======
	struct drbd_config_context adm_ctx;
>>>>>>> v3.18
	enum drbd_ret_code retcode;
	struct timeout_parms tp;
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
=======
	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
>>>>>>> v3.18
=======
	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
>>>>>>> v3.18
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	tp.timeout_type =
<<<<<<< HEAD
<<<<<<< HEAD
		adm_ctx.mdev->state.pdsk == D_OUTDATED ? UT_PEER_OUTDATED :
		test_bit(USE_DEGR_WFC_T, &adm_ctx.mdev->flags) ? UT_DEGRADED :
=======
		adm_ctx.device->state.pdsk == D_OUTDATED ? UT_PEER_OUTDATED :
		test_bit(USE_DEGR_WFC_T, &adm_ctx.device->flags) ? UT_DEGRADED :
>>>>>>> v3.18
=======
		adm_ctx.device->state.pdsk == D_OUTDATED ? UT_PEER_OUTDATED :
		test_bit(USE_DEGR_WFC_T, &adm_ctx.device->flags) ? UT_DEGRADED :
>>>>>>> v3.18
		UT_DEFAULT;

	err = timeout_parms_to_priv_skb(adm_ctx.reply_skb, &tp);
	if (err) {
		nlmsg_free(adm_ctx.reply_skb);
		return err;
	}
out:
<<<<<<< HEAD
<<<<<<< HEAD
	drbd_adm_finish(info, retcode);
=======
	drbd_adm_finish(&adm_ctx, info, retcode);
>>>>>>> v3.18
=======
	drbd_adm_finish(&adm_ctx, info, retcode);
>>>>>>> v3.18
	return 0;
}

int drbd_adm_start_ov(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct drbd_conf *mdev;
	enum drbd_ret_code retcode;
	struct start_ov_parms parms;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
=======
=======
>>>>>>> v3.18
	struct drbd_config_context adm_ctx;
	struct drbd_device *device;
	enum drbd_ret_code retcode;
	struct start_ov_parms parms;

	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

<<<<<<< HEAD
<<<<<<< HEAD
	mdev = adm_ctx.mdev;

	/* resume from last known position, if possible */
	parms.ov_start_sector = mdev->ov_start_sector;
=======
=======
>>>>>>> v3.18
	device = adm_ctx.device;

	/* resume from last known position, if possible */
	parms.ov_start_sector = device->ov_start_sector;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	parms.ov_stop_sector = ULLONG_MAX;
	if (info->attrs[DRBD_NLA_START_OV_PARMS]) {
		int err = start_ov_parms_from_attrs(&parms, info);
		if (err) {
			retcode = ERR_MANDATORY_TAG;
<<<<<<< HEAD
<<<<<<< HEAD
			drbd_msg_put_info(from_attrs_err_to_txt(err));
			goto out;
		}
	}
	/* w_make_ov_request expects position to be aligned */
	mdev->ov_start_sector = parms.ov_start_sector & ~(BM_SECT_PER_BIT-1);
	mdev->ov_stop_sector = parms.ov_stop_sector;

	/* If there is still bitmap IO pending, e.g. previous resync or verify
	 * just being finished, wait for it before requesting a new resync. */
	drbd_suspend_io(mdev);
	wait_event(mdev->misc_wait, !test_bit(BITMAP_IO, &mdev->flags));
	retcode = drbd_request_state(mdev,NS(conn,C_VERIFY_S));
	drbd_resume_io(mdev);
out:
	drbd_adm_finish(info, retcode);
=======
=======
>>>>>>> v3.18
			drbd_msg_put_info(adm_ctx.reply_skb, from_attrs_err_to_txt(err));
			goto out;
		}
	}
	mutex_lock(&adm_ctx.resource->adm_mutex);

	/* w_make_ov_request expects position to be aligned */
	device->ov_start_sector = parms.ov_start_sector & ~(BM_SECT_PER_BIT-1);
	device->ov_stop_sector = parms.ov_stop_sector;

	/* If there is still bitmap IO pending, e.g. previous resync or verify
	 * just being finished, wait for it before requesting a new resync. */
	drbd_suspend_io(device);
	wait_event(device->misc_wait, !test_bit(BITMAP_IO, &device->flags));
	retcode = drbd_request_state(device, NS(conn, C_VERIFY_S));
	drbd_resume_io(device);

	mutex_unlock(&adm_ctx.resource->adm_mutex);
out:
	drbd_adm_finish(&adm_ctx, info, retcode);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
}


int drbd_adm_new_c_uuid(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct drbd_conf *mdev;
=======
	struct drbd_config_context adm_ctx;
	struct drbd_device *device;
>>>>>>> v3.18
=======
	struct drbd_config_context adm_ctx;
	struct drbd_device *device;
>>>>>>> v3.18
	enum drbd_ret_code retcode;
	int skip_initial_sync = 0;
	int err;
	struct new_c_uuid_parms args;

<<<<<<< HEAD
<<<<<<< HEAD
	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
=======
	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
>>>>>>> v3.18
=======
	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
>>>>>>> v3.18
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out_nolock;

<<<<<<< HEAD
<<<<<<< HEAD
	mdev = adm_ctx.mdev;
=======
	device = adm_ctx.device;
>>>>>>> v3.18
=======
	device = adm_ctx.device;
>>>>>>> v3.18
	memset(&args, 0, sizeof(args));
	if (info->attrs[DRBD_NLA_NEW_C_UUID_PARMS]) {
		err = new_c_uuid_parms_from_attrs(&args, info);
		if (err) {
			retcode = ERR_MANDATORY_TAG;
<<<<<<< HEAD
<<<<<<< HEAD
			drbd_msg_put_info(from_attrs_err_to_txt(err));
=======
			drbd_msg_put_info(adm_ctx.reply_skb, from_attrs_err_to_txt(err));
>>>>>>> v3.18
=======
			drbd_msg_put_info(adm_ctx.reply_skb, from_attrs_err_to_txt(err));
>>>>>>> v3.18
			goto out_nolock;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	mutex_lock(mdev->state_mutex); /* Protects us against serialized state changes. */

	if (!get_ldev(mdev)) {
=======
=======
>>>>>>> v3.18
	mutex_lock(&adm_ctx.resource->adm_mutex);
	mutex_lock(device->state_mutex); /* Protects us against serialized state changes. */

	if (!get_ldev(device)) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		retcode = ERR_NO_DISK;
		goto out;
	}

	/* this is "skip initial sync", assume to be clean */
<<<<<<< HEAD
<<<<<<< HEAD
	if (mdev->state.conn == C_CONNECTED && mdev->tconn->agreed_pro_version >= 90 &&
	    mdev->ldev->md.uuid[UI_CURRENT] == UUID_JUST_CREATED && args.clear_bm) {
		dev_info(DEV, "Preparing to skip initial sync\n");
		skip_initial_sync = 1;
	} else if (mdev->state.conn != C_STANDALONE) {
=======
=======
>>>>>>> v3.18
	if (device->state.conn == C_CONNECTED &&
	    first_peer_device(device)->connection->agreed_pro_version >= 90 &&
	    device->ldev->md.uuid[UI_CURRENT] == UUID_JUST_CREATED && args.clear_bm) {
		drbd_info(device, "Preparing to skip initial sync\n");
		skip_initial_sync = 1;
	} else if (device->state.conn != C_STANDALONE) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		retcode = ERR_CONNECTED;
		goto out_dec;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	drbd_uuid_set(mdev, UI_BITMAP, 0); /* Rotate UI_BITMAP to History 1, etc... */
	drbd_uuid_new_current(mdev); /* New current, previous to UI_BITMAP */

	if (args.clear_bm) {
		err = drbd_bitmap_io(mdev, &drbd_bmio_clear_n_write,
			"clear_n_write from new_c_uuid", BM_LOCKED_MASK);
		if (err) {
			dev_err(DEV, "Writing bitmap failed with %d\n",err);
			retcode = ERR_IO_MD_DISK;
		}
		if (skip_initial_sync) {
			drbd_send_uuids_skip_initial_sync(mdev);
			_drbd_uuid_set(mdev, UI_BITMAP, 0);
			drbd_print_uuids(mdev, "cleared bitmap UUID");
			spin_lock_irq(&mdev->tconn->req_lock);
			_drbd_set_state(_NS2(mdev, disk, D_UP_TO_DATE, pdsk, D_UP_TO_DATE),
					CS_VERBOSE, NULL);
			spin_unlock_irq(&mdev->tconn->req_lock);
		}
	}

	drbd_md_sync(mdev);
out_dec:
	put_ldev(mdev);
out:
	mutex_unlock(mdev->state_mutex);
out_nolock:
	drbd_adm_finish(info, retcode);
=======
=======
>>>>>>> v3.18
	drbd_uuid_set(device, UI_BITMAP, 0); /* Rotate UI_BITMAP to History 1, etc... */
	drbd_uuid_new_current(device); /* New current, previous to UI_BITMAP */

	if (args.clear_bm) {
		err = drbd_bitmap_io(device, &drbd_bmio_clear_n_write,
			"clear_n_write from new_c_uuid", BM_LOCKED_MASK);
		if (err) {
			drbd_err(device, "Writing bitmap failed with %d\n", err);
			retcode = ERR_IO_MD_DISK;
		}
		if (skip_initial_sync) {
			drbd_send_uuids_skip_initial_sync(first_peer_device(device));
			_drbd_uuid_set(device, UI_BITMAP, 0);
			drbd_print_uuids(device, "cleared bitmap UUID");
			spin_lock_irq(&device->resource->req_lock);
			_drbd_set_state(_NS2(device, disk, D_UP_TO_DATE, pdsk, D_UP_TO_DATE),
					CS_VERBOSE, NULL);
			spin_unlock_irq(&device->resource->req_lock);
		}
	}

	drbd_md_sync(device);
out_dec:
	put_ldev(device);
out:
	mutex_unlock(device->state_mutex);
	mutex_unlock(&adm_ctx.resource->adm_mutex);
out_nolock:
	drbd_adm_finish(&adm_ctx, info, retcode);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
}

static enum drbd_ret_code
<<<<<<< HEAD
<<<<<<< HEAD
drbd_check_resource_name(const char *name)
{
	if (!name || !name[0]) {
		drbd_msg_put_info("resource name missing");
=======
=======
>>>>>>> v3.18
drbd_check_resource_name(struct drbd_config_context *adm_ctx)
{
	const char *name = adm_ctx->resource_name;
	if (!name || !name[0]) {
		drbd_msg_put_info(adm_ctx->reply_skb, "resource name missing");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return ERR_MANDATORY_TAG;
	}
	/* if we want to use these in sysfs/configfs/debugfs some day,
	 * we must not allow slashes */
	if (strchr(name, '/')) {
<<<<<<< HEAD
<<<<<<< HEAD
		drbd_msg_put_info("invalid resource name");
=======
		drbd_msg_put_info(adm_ctx->reply_skb, "invalid resource name");
>>>>>>> v3.18
=======
		drbd_msg_put_info(adm_ctx->reply_skb, "invalid resource name");
>>>>>>> v3.18
		return ERR_INVALID_REQUEST;
	}
	return NO_ERROR;
}

int drbd_adm_new_resource(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct drbd_config_context adm_ctx;
>>>>>>> v3.18
=======
	struct drbd_config_context adm_ctx;
>>>>>>> v3.18
	enum drbd_ret_code retcode;
	struct res_opts res_opts;
	int err;

<<<<<<< HEAD
<<<<<<< HEAD
	retcode = drbd_adm_prepare(skb, info, 0);
=======
	retcode = drbd_adm_prepare(&adm_ctx, skb, info, 0);
>>>>>>> v3.18
=======
	retcode = drbd_adm_prepare(&adm_ctx, skb, info, 0);
>>>>>>> v3.18
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	set_res_opts_defaults(&res_opts);
	err = res_opts_from_attrs(&res_opts, info);
	if (err && err != -ENOMSG) {
		retcode = ERR_MANDATORY_TAG;
<<<<<<< HEAD
<<<<<<< HEAD
		drbd_msg_put_info(from_attrs_err_to_txt(err));
		goto out;
	}

	retcode = drbd_check_resource_name(adm_ctx.resource_name);
	if (retcode != NO_ERROR)
		goto out;

	if (adm_ctx.tconn) {
		if (info->nlhdr->nlmsg_flags & NLM_F_EXCL) {
			retcode = ERR_INVALID_REQUEST;
			drbd_msg_put_info("resource exists");
=======
=======
>>>>>>> v3.18
		drbd_msg_put_info(adm_ctx.reply_skb, from_attrs_err_to_txt(err));
		goto out;
	}

	retcode = drbd_check_resource_name(&adm_ctx);
	if (retcode != NO_ERROR)
		goto out;

	if (adm_ctx.resource) {
		if (info->nlhdr->nlmsg_flags & NLM_F_EXCL) {
			retcode = ERR_INVALID_REQUEST;
			drbd_msg_put_info(adm_ctx.reply_skb, "resource exists");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		}
		/* else: still NO_ERROR */
		goto out;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (!conn_create(adm_ctx.resource_name, &res_opts))
		retcode = ERR_NOMEM;
out:
	drbd_adm_finish(info, retcode);
	return 0;
}

int drbd_adm_add_minor(struct sk_buff *skb, struct genl_info *info)
{
	struct drbd_genlmsghdr *dh = info->userhdr;
	enum drbd_ret_code retcode;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_RESOURCE);
=======
=======
>>>>>>> v3.18
	/* not yet safe for genl_family.parallel_ops */
	if (!conn_create(adm_ctx.resource_name, &res_opts))
		retcode = ERR_NOMEM;
out:
	drbd_adm_finish(&adm_ctx, info, retcode);
	return 0;
}

int drbd_adm_new_minor(struct sk_buff *skb, struct genl_info *info)
{
	struct drbd_config_context adm_ctx;
	struct drbd_genlmsghdr *dh = info->userhdr;
	enum drbd_ret_code retcode;

	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_RESOURCE);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	if (dh->minor > MINORMASK) {
<<<<<<< HEAD
<<<<<<< HEAD
		drbd_msg_put_info("requested minor out of range");
=======
		drbd_msg_put_info(adm_ctx.reply_skb, "requested minor out of range");
>>>>>>> v3.18
=======
		drbd_msg_put_info(adm_ctx.reply_skb, "requested minor out of range");
>>>>>>> v3.18
		retcode = ERR_INVALID_REQUEST;
		goto out;
	}
	if (adm_ctx.volume > DRBD_VOLUME_MAX) {
<<<<<<< HEAD
<<<<<<< HEAD
		drbd_msg_put_info("requested volume id out of range");
=======
		drbd_msg_put_info(adm_ctx.reply_skb, "requested volume id out of range");
>>>>>>> v3.18
=======
		drbd_msg_put_info(adm_ctx.reply_skb, "requested volume id out of range");
>>>>>>> v3.18
		retcode = ERR_INVALID_REQUEST;
		goto out;
	}

	/* drbd_adm_prepare made sure already
<<<<<<< HEAD
<<<<<<< HEAD
	 * that mdev->tconn and mdev->vnr match the request. */
	if (adm_ctx.mdev) {
=======
	 * that first_peer_device(device)->connection and device->vnr match the request. */
	if (adm_ctx.device) {
>>>>>>> v3.18
=======
	 * that first_peer_device(device)->connection and device->vnr match the request. */
	if (adm_ctx.device) {
>>>>>>> v3.18
		if (info->nlhdr->nlmsg_flags & NLM_F_EXCL)
			retcode = ERR_MINOR_EXISTS;
		/* else: still NO_ERROR */
		goto out;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	retcode = conn_new_minor(adm_ctx.tconn, dh->minor, adm_ctx.volume);
out:
	drbd_adm_finish(info, retcode);
	return 0;
}

static enum drbd_ret_code adm_delete_minor(struct drbd_conf *mdev)
{
	if (mdev->state.disk == D_DISKLESS &&
	    /* no need to be mdev->state.conn == C_STANDALONE &&
	     * we may want to delete a minor from a live replication group.
	     */
	    mdev->state.role == R_SECONDARY) {
		_drbd_request_state(mdev, NS(conn, C_WF_REPORT_PARAMS),
				    CS_VERBOSE + CS_WAIT_COMPLETE);
		idr_remove(&mdev->tconn->volumes, mdev->vnr);
		idr_remove(&minors, mdev_to_minor(mdev));
		destroy_workqueue(mdev->submit.wq);
		del_gendisk(mdev->vdisk);
		synchronize_rcu();
		kref_put(&mdev->kref, &drbd_minor_destroy);
=======
=======
>>>>>>> v3.18
	mutex_lock(&adm_ctx.resource->adm_mutex);
	retcode = drbd_create_device(&adm_ctx, dh->minor);
	mutex_unlock(&adm_ctx.resource->adm_mutex);
out:
	drbd_adm_finish(&adm_ctx, info, retcode);
	return 0;
}

static enum drbd_ret_code adm_del_minor(struct drbd_device *device)
{
	if (device->state.disk == D_DISKLESS &&
	    /* no need to be device->state.conn == C_STANDALONE &&
	     * we may want to delete a minor from a live replication group.
	     */
	    device->state.role == R_SECONDARY) {
		_drbd_request_state(device, NS(conn, C_WF_REPORT_PARAMS),
				    CS_VERBOSE + CS_WAIT_COMPLETE);
		drbd_delete_device(device);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return NO_ERROR;
	} else
		return ERR_MINOR_CONFIGURED;
}

<<<<<<< HEAD
<<<<<<< HEAD
int drbd_adm_delete_minor(struct sk_buff *skb, struct genl_info *info)
{
	enum drbd_ret_code retcode;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_MINOR);
=======
=======
>>>>>>> v3.18
int drbd_adm_del_minor(struct sk_buff *skb, struct genl_info *info)
{
	struct drbd_config_context adm_ctx;
	enum drbd_ret_code retcode;

	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_MINOR);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

<<<<<<< HEAD
<<<<<<< HEAD
	retcode = adm_delete_minor(adm_ctx.mdev);
out:
	drbd_adm_finish(info, retcode);
=======
=======
>>>>>>> v3.18
	mutex_lock(&adm_ctx.resource->adm_mutex);
	retcode = adm_del_minor(adm_ctx.device);
	mutex_unlock(&adm_ctx.resource->adm_mutex);
out:
	drbd_adm_finish(&adm_ctx, info, retcode);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
}

int drbd_adm_down(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int retcode; /* enum drbd_ret_code rsp. enum drbd_state_rv */
	struct drbd_conf *mdev;
	unsigned i;

	retcode = drbd_adm_prepare(skb, info, 0);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	if (!adm_ctx.tconn) {
		retcode = ERR_RES_NOT_KNOWN;
		goto out;
	}

	/* demote */
	idr_for_each_entry(&adm_ctx.tconn->volumes, mdev, i) {
		retcode = drbd_set_role(mdev, R_SECONDARY, 0);
		if (retcode < SS_SUCCESS) {
			drbd_msg_put_info("failed to demote");
=======
=======
>>>>>>> v3.18
	struct drbd_config_context adm_ctx;
	struct drbd_resource *resource;
	struct drbd_connection *connection;
	struct drbd_device *device;
	int retcode; /* enum drbd_ret_code rsp. enum drbd_state_rv */
	unsigned i;

	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_RESOURCE);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto finish;

	resource = adm_ctx.resource;
	mutex_lock(&resource->adm_mutex);
	/* demote */
	for_each_connection(connection, resource) {
		struct drbd_peer_device *peer_device;

		idr_for_each_entry(&connection->peer_devices, peer_device, i) {
			retcode = drbd_set_role(peer_device->device, R_SECONDARY, 0);
			if (retcode < SS_SUCCESS) {
				drbd_msg_put_info(adm_ctx.reply_skb, "failed to demote");
				goto out;
			}
		}

		retcode = conn_try_disconnect(connection, 0);
		if (retcode < SS_SUCCESS) {
			drbd_msg_put_info(adm_ctx.reply_skb, "failed to disconnect");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			goto out;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	retcode = conn_try_disconnect(adm_ctx.tconn, 0);
	if (retcode < SS_SUCCESS) {
		drbd_msg_put_info("failed to disconnect");
		goto out;
	}

	/* detach */
	idr_for_each_entry(&adm_ctx.tconn->volumes, mdev, i) {
		retcode = adm_detach(mdev, 0);
		if (retcode < SS_SUCCESS || retcode > NO_ERROR) {
			drbd_msg_put_info("failed to detach");
=======
=======
>>>>>>> v3.18
	/* detach */
	idr_for_each_entry(&resource->devices, device, i) {
		retcode = adm_detach(device, 0);
		if (retcode < SS_SUCCESS || retcode > NO_ERROR) {
			drbd_msg_put_info(adm_ctx.reply_skb, "failed to detach");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			goto out;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	/* If we reach this, all volumes (of this tconn) are Secondary,
	 * Disconnected, Diskless, aka Unconfigured. Make sure all threads have
	 * actually stopped, state handling only does drbd_thread_stop_nowait(). */
	drbd_thread_stop(&adm_ctx.tconn->worker);
=======
=======
>>>>>>> v3.18
	/* If we reach this, all volumes (of this connection) are Secondary,
	 * Disconnected, Diskless, aka Unconfigured. Make sure all threads have
	 * actually stopped, state handling only does drbd_thread_stop_nowait(). */
	for_each_connection(connection, resource)
		drbd_thread_stop(&connection->worker);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* Now, nothing can fail anymore */

	/* delete volumes */
<<<<<<< HEAD
<<<<<<< HEAD
	idr_for_each_entry(&adm_ctx.tconn->volumes, mdev, i) {
		retcode = adm_delete_minor(mdev);
		if (retcode != NO_ERROR) {
			/* "can not happen" */
			drbd_msg_put_info("failed to delete volume");
=======
=======
>>>>>>> v3.18
	idr_for_each_entry(&resource->devices, device, i) {
		retcode = adm_del_minor(device);
		if (retcode != NO_ERROR) {
			/* "can not happen" */
			drbd_msg_put_info(adm_ctx.reply_skb, "failed to delete volume");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			goto out;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	/* delete connection */
	if (conn_lowest_minor(adm_ctx.tconn) < 0) {
		list_del_rcu(&adm_ctx.tconn->all_tconn);
		synchronize_rcu();
		kref_put(&adm_ctx.tconn->kref, &conn_destroy);

		retcode = NO_ERROR;
	} else {
		/* "can not happen" */
		retcode = ERR_RES_IN_USE;
		drbd_msg_put_info("failed to delete connection");
	}
	goto out;
out:
	drbd_adm_finish(info, retcode);
=======
=======
>>>>>>> v3.18
	list_del_rcu(&resource->resources);
	synchronize_rcu();
	drbd_free_resource(resource);
	retcode = NO_ERROR;
out:
	mutex_unlock(&resource->adm_mutex);
finish:
	drbd_adm_finish(&adm_ctx, info, retcode);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return 0;
}

int drbd_adm_del_resource(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
	enum drbd_ret_code retcode;

	retcode = drbd_adm_prepare(skb, info, DRBD_ADM_NEED_RESOURCE);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto out;

	if (conn_lowest_minor(adm_ctx.tconn) < 0) {
		list_del_rcu(&adm_ctx.tconn->all_tconn);
		synchronize_rcu();
		kref_put(&adm_ctx.tconn->kref, &conn_destroy);

		retcode = NO_ERROR;
	} else {
		retcode = ERR_RES_IN_USE;
	}

	if (retcode == NO_ERROR)
		drbd_thread_stop(&adm_ctx.tconn->worker);
out:
	drbd_adm_finish(info, retcode);
	return 0;
}

void drbd_bcast_event(struct drbd_conf *mdev, const struct sib_info *sib)
=======
=======
>>>>>>> v3.18
	struct drbd_config_context adm_ctx;
	struct drbd_resource *resource;
	struct drbd_connection *connection;
	enum drbd_ret_code retcode;

	retcode = drbd_adm_prepare(&adm_ctx, skb, info, DRBD_ADM_NEED_RESOURCE);
	if (!adm_ctx.reply_skb)
		return retcode;
	if (retcode != NO_ERROR)
		goto finish;

	resource = adm_ctx.resource;
	mutex_lock(&resource->adm_mutex);
	for_each_connection(connection, resource) {
		if (connection->cstate > C_STANDALONE) {
			retcode = ERR_NET_CONFIGURED;
			goto out;
		}
	}
	if (!idr_is_empty(&resource->devices)) {
		retcode = ERR_RES_IN_USE;
		goto out;
	}

	list_del_rcu(&resource->resources);
	for_each_connection(connection, resource)
		drbd_thread_stop(&connection->worker);
	synchronize_rcu();
	drbd_free_resource(resource);
	retcode = NO_ERROR;
out:
	mutex_unlock(&resource->adm_mutex);
finish:
	drbd_adm_finish(&adm_ctx, info, retcode);
	return 0;
}

void drbd_bcast_event(struct drbd_device *device, const struct sib_info *sib)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	static atomic_t drbd_genl_seq = ATOMIC_INIT(2); /* two. */
	struct sk_buff *msg;
	struct drbd_genlmsghdr *d_out;
	unsigned seq;
	int err = -ENOMEM;

<<<<<<< HEAD
<<<<<<< HEAD
	if (sib->sib_reason == SIB_SYNC_PROGRESS) {
		if (time_after(jiffies, mdev->rs_last_bcast + HZ))
			mdev->rs_last_bcast = jiffies;
		else
			return;
	}

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	seq = atomic_inc_return(&drbd_genl_seq);
	msg = genlmsg_new(NLMSG_GOODSIZE, GFP_NOIO);
	if (!msg)
		goto failed;

	err = -EMSGSIZE;
	d_out = genlmsg_put(msg, 0, seq, &drbd_genl_family, 0, DRBD_EVENT);
	if (!d_out) /* cannot happen, but anyways. */
		goto nla_put_failure;
<<<<<<< HEAD
<<<<<<< HEAD
	d_out->minor = mdev_to_minor(mdev);
	d_out->ret_code = NO_ERROR;

	if (nla_put_status_info(msg, mdev, sib))
=======
=======
>>>>>>> v3.18
	d_out->minor = device_to_minor(device);
	d_out->ret_code = NO_ERROR;

	if (nla_put_status_info(msg, device, sib))
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		goto nla_put_failure;
	genlmsg_end(msg, d_out);
	err = drbd_genl_multicast_events(msg, 0);
	/* msg has been consumed or freed in netlink_broadcast() */
	if (err && err != -ESRCH)
		goto failed;

	return;

nla_put_failure:
	nlmsg_free(msg);
failed:
<<<<<<< HEAD
<<<<<<< HEAD
	dev_err(DEV, "Error %d while broadcasting event. "
=======
	drbd_err(device, "Error %d while broadcasting event. "
>>>>>>> v3.18
=======
	drbd_err(device, "Error %d while broadcasting event. "
>>>>>>> v3.18
			"Event seq:%u sib_reason:%u\n",
			err, seq, sib->sib_reason);
}
