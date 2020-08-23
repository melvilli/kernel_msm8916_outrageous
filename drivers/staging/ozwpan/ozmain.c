/* -----------------------------------------------------------------------------
 * Copyright (c) 2011 Ozmo Inc
 * Released under the GNU General Public License Version 2 (GPLv2).
 * -----------------------------------------------------------------------------
 */
<<<<<<< HEAD
=======

>>>>>>> v3.18
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/netdevice.h>
#include <linux/errno.h>
#include <linux/ieee80211.h>
<<<<<<< HEAD
#include "ozconfig.h"
#include "ozpd.h"
#include "ozproto.h"
#include "ozcdev.h"
#include "oztrace.h"
#include "ozevent.h"
/*------------------------------------------------------------------------------
=======
#include "ozdbg.h"
#include "ozpd.h"
#include "ozproto.h"
#include "ozcdev.h"

unsigned int oz_dbg_mask = OZ_DEFAULT_DBG_MASK;

/*
>>>>>>> v3.18
 * The name of the 802.11 mac device. Empty string is the default value but a
 * value can be supplied as a parameter to the module. An empty string means
 * bind to nothing. '*' means bind to all netcards - this includes non-802.11
 * netcards. Bindings can be added later using an IOCTL.
 */
static char *g_net_dev = "";
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======
module_param(g_net_dev, charp, S_IRUGO);
MODULE_PARM_DESC(g_net_dev, "The device(s) to bind to; "
	"'*' means all, '' (empty string; default) means none.");

/*
>>>>>>> v3.18
 * Context: process
 */
static int __init ozwpan_init(void)
{
<<<<<<< HEAD
	oz_event_init();
=======
>>>>>>> v3.18
	oz_cdev_register();
	oz_protocol_init(g_net_dev);
	oz_app_enable(OZ_APPID_USB, 1);
	oz_apps_init();
<<<<<<< HEAD
#ifdef CONFIG_DEBUG_FS
	oz_debugfs_init();
#endif
	return 0;
}
/*------------------------------------------------------------------------------
=======
	return 0;
}

/*
>>>>>>> v3.18
 * Context: process
 */
static void __exit ozwpan_exit(void)
{
	oz_protocol_term();
	oz_apps_term();
	oz_cdev_deregister();
<<<<<<< HEAD
	oz_event_term();
#ifdef CONFIG_DEBUG_FS
	oz_debugfs_remove();
#endif
}
/*------------------------------------------------------------------------------
 */
module_param(g_net_dev, charp, S_IRUGO);
=======
}

>>>>>>> v3.18
module_init(ozwpan_init);
module_exit(ozwpan_exit);

MODULE_AUTHOR("Chris Kelly");
MODULE_DESCRIPTION("Ozmo Devices USB over WiFi hcd driver");
MODULE_VERSION("1.0.13");
MODULE_LICENSE("GPL");

