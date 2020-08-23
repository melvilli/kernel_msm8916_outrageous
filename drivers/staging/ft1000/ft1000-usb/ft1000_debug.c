<<<<<<< HEAD
<<<<<<< HEAD
//---------------------------------------------------------------------------
// FT1000 driver for Flarion Flash OFDM NIC Device
//
// Copyright (C) 2006 Flarion Technologies, All rights reserved.
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option) any
// later version. This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details. You should have received a copy of the GNU General Public
// License along with this program; if not, write to the
// Free Software Foundation, Inc., 59 Temple Place -
// Suite 330, Boston, MA 02111-1307, USA.
//---------------------------------------------------------------------------
//
// File:         ft1000_chdev.c
//
// Description:  Custom character device dispatch routines.
//
// History:
// 8/29/02    Whc                Ported to Linux.
// 6/05/06    Whc                Porting to Linux 2.6.9
//
//---------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
/*
*---------------------------------------------------------------------------
* FT1000 driver for Flarion Flash OFDM NIC Device
*
* Copyright (C) 2006 Flarion Technologies, All rights reserved.
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the Free
* Software Foundation; either version 2 of the License, or (at your option) any
* later version. This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
* more details. You should have received a copy of the GNU General Public
* License along with this program; if not, write to the
* Free Software Foundation, Inc., 59 Temple Place -
* Suite 330, Boston, MA 02111-1307, USA.
*---------------------------------------------------------------------------
*
* File:         ft1000_chdev.c
*
* Description:  Custom character device dispatch routines.
*
* History:
* 8/29/02    Whc                Ported to Linux.
* 6/05/06    Whc                Porting to Linux 2.6.9
*
*---------------------------------------------------------------------------
*/
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/poll.h>
#include <linux/netdevice.h>
#include <linux/delay.h>

#include <linux/ioctl.h>
#include <linux/debugfs.h>
#include "ft1000_usb.h"

static int ft1000_flarion_cnt = 0;

<<<<<<< HEAD
<<<<<<< HEAD
static int ft1000_open (struct inode *inode, struct file *file);
static unsigned int ft1000_poll_dev(struct file *file, poll_table *wait);
static long ft1000_ioctl(struct file *file, unsigned int command,
                           unsigned long argument);
static int ft1000_release (struct inode *inode, struct file *file);

// List to free receive command buffer pool
struct list_head freercvpool;

// lock to arbitrate free buffer list for receive command data
=======
=======
>>>>>>> v3.18
static int ft1000_open(struct inode *inode, struct file *file);
static unsigned int ft1000_poll_dev(struct file *file, poll_table *wait);
static long ft1000_ioctl(struct file *file, unsigned int command,
                           unsigned long argument);
static int ft1000_release(struct inode *inode, struct file *file);

/* List to free receive command buffer pool */
struct list_head freercvpool;

/* lock to arbitrate free buffer list for receive command data */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
spinlock_t free_buff_lock;

int numofmsgbuf = 0;

<<<<<<< HEAD
<<<<<<< HEAD
//
// Table of entry-point routines for char device
//
static const struct file_operations ft1000fops =
{
=======
=======
>>>>>>> v3.18
/*
* Table of entry-point routines for char device
*/
static const struct file_operations ft1000fops = {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	.unlocked_ioctl	= ft1000_ioctl,
	.poll		= ft1000_poll_dev,
	.open		= ft1000_open,
	.release	= ft1000_release,
	.llseek		= no_llseek,
};

<<<<<<< HEAD
<<<<<<< HEAD
//---------------------------------------------------------------------------
// Function:    ft1000_get_buffer
//
// Parameters:
//
// Returns:
//
// Description:
//
// Notes:
//
//---------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
/*
---------------------------------------------------------------------------
* Function:    ft1000_get_buffer
*
* Parameters:
*
* Returns:
*
* Description:
*
* Notes:
*
*---------------------------------------------------------------------------
*/
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
struct dpram_blk *ft1000_get_buffer(struct list_head *bufflist)
{
    unsigned long flags;
	struct dpram_blk *ptr;

    spin_lock_irqsave(&free_buff_lock, flags);
<<<<<<< HEAD
<<<<<<< HEAD
    // Check if buffer is available
    if ( list_empty(bufflist) ) {
        DEBUG("ft1000_get_buffer:  No more buffer - %d\n", numofmsgbuf);
        ptr = NULL;
    }
    else {
        numofmsgbuf--;
	ptr = list_entry(bufflist->next, struct dpram_blk, list);
        list_del(&ptr->list);
        //DEBUG("ft1000_get_buffer: number of free msg buffers = %d\n", numofmsgbuf);
=======
=======
>>>>>>> v3.18
    /* Check if buffer is available */
    if (list_empty(bufflist)) {
        DEBUG("ft1000_get_buffer:  No more buffer - %d\n", numofmsgbuf);
        ptr = NULL;
    } else {
        numofmsgbuf--;
	ptr = list_entry(bufflist->next, struct dpram_blk, list);
        list_del(&ptr->list);
        /* DEBUG("ft1000_get_buffer: number of free msg buffers = %d\n", numofmsgbuf); */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
    }
    spin_unlock_irqrestore(&free_buff_lock, flags);

    return ptr;
}




<<<<<<< HEAD
<<<<<<< HEAD
//---------------------------------------------------------------------------
// Function:    ft1000_free_buffer
//
// Parameters:
//
// Returns:
//
// Description:
//
// Notes:
//
//---------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
/*
*---------------------------------------------------------------------------
* Function:    ft1000_free_buffer
*
* Parameters:
*
* Returns:
*
* Description:
*
* Notes:
*
*---------------------------------------------------------------------------
*/
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
void ft1000_free_buffer(struct dpram_blk *pdpram_blk, struct list_head *plist)
{
    unsigned long flags;

    spin_lock_irqsave(&free_buff_lock, flags);
<<<<<<< HEAD
<<<<<<< HEAD
    // Put memory back to list
    list_add_tail(&pdpram_blk->list, plist);
    numofmsgbuf++;
    //DEBUG("ft1000_free_buffer: number of free msg buffers = %d\n", numofmsgbuf);
    spin_unlock_irqrestore(&free_buff_lock, flags);
}

//---------------------------------------------------------------------------
// Function:    ft1000_CreateDevice
//
// Parameters:  dev - pointer to adapter object
//
// Returns:     0 if successful
//
// Description: Creates a private char device.
//
// Notes:       Only called by init_module().
//
//---------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
    /* Put memory back to list */
    list_add_tail(&pdpram_blk->list, plist);
    numofmsgbuf++;
    /*DEBUG("ft1000_free_buffer: number of free msg buffers = %d\n", numofmsgbuf); */
    spin_unlock_irqrestore(&free_buff_lock, flags);
}

/*
*---------------------------------------------------------------------------
* Function:    ft1000_CreateDevice
*
* Parameters:  dev - pointer to adapter object
*
* Returns:     0 if successful
*
* Description: Creates a private char device.
*
* Notes:       Only called by init_module().
*
*---------------------------------------------------------------------------
*/
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
int ft1000_create_dev(struct ft1000_usb *dev)
{
    int result;
    int i;
	struct dentry *dir, *file;
	struct ft1000_debug_dirs *tmp;

<<<<<<< HEAD
<<<<<<< HEAD
    // make a new device name
=======
    /* make a new device name */
>>>>>>> v3.18
=======
    /* make a new device name */
>>>>>>> v3.18
    sprintf(dev->DeviceName, "%s%d", "FT1000_", dev->CardNumber);

    DEBUG("%s: number of instance = %d\n", __func__, ft1000_flarion_cnt);
    DEBUG("DeviceCreated = %x\n", dev->DeviceCreated);

<<<<<<< HEAD
<<<<<<< HEAD
    if (dev->DeviceCreated)
    {
=======
    if (dev->DeviceCreated) {
>>>>>>> v3.18
=======
    if (dev->DeviceCreated) {
>>>>>>> v3.18
	DEBUG("%s: \"%s\" already registered\n", __func__, dev->DeviceName);
	return -EIO;
    }


<<<<<<< HEAD
<<<<<<< HEAD
    // register the device
=======
    /* register the device */
>>>>>>> v3.18
=======
    /* register the device */
>>>>>>> v3.18
    DEBUG("%s: \"%s\" debugfs device registration\n", __func__, dev->DeviceName);

	tmp = kmalloc(sizeof(struct ft1000_debug_dirs), GFP_KERNEL);
	if (tmp == NULL) {
		result = -1;
		goto fail;
	}

	dir = debugfs_create_dir(dev->DeviceName, NULL);
	if (IS_ERR(dir)) {
		result = PTR_ERR(dir);
		goto debug_dir_fail;
	}

	file = debugfs_create_file("device", S_IRUGO | S_IWUSR, dir,
					dev, &ft1000fops);
	if (IS_ERR(file)) {
		result = PTR_ERR(file);
		goto debug_file_fail;
	}

	tmp->dent = dir;
	tmp->file = file;
	tmp->int_number = dev->CardNumber;
	list_add(&(tmp->list), &(dev->nodes.list));

    DEBUG("%s: registered debugfs directory \"%s\"\n", __func__, dev->DeviceName);

<<<<<<< HEAD
<<<<<<< HEAD
    // initialize application information
=======
    /* initialize application information */
>>>>>>> v3.18
=======
    /* initialize application information */
>>>>>>> v3.18
    dev->appcnt = 0;
    for (i=0; i<MAX_NUM_APP; i++) {
        dev->app_info[i].nTxMsg = 0;
        dev->app_info[i].nRxMsg = 0;
        dev->app_info[i].nTxMsgReject = 0;
        dev->app_info[i].nRxMsgMiss = 0;
        dev->app_info[i].fileobject = NULL;
        dev->app_info[i].app_id = i+1;
        dev->app_info[i].DspBCMsgFlag = 0;
        dev->app_info[i].NumOfMsg = 0;
        init_waitqueue_head(&dev->app_info[i].wait_dpram_msg);
<<<<<<< HEAD
<<<<<<< HEAD
        INIT_LIST_HEAD (&dev->app_info[i].app_sqlist);
=======
        INIT_LIST_HEAD(&dev->app_info[i].app_sqlist);
>>>>>>> v3.18
=======
        INIT_LIST_HEAD(&dev->app_info[i].app_sqlist);
>>>>>>> v3.18
    }

    dev->DeviceCreated = TRUE;
    ft1000_flarion_cnt++;

	return 0;

debug_file_fail:
	debugfs_remove(dir);
debug_dir_fail:
	kfree(tmp);
fail:
	return result;
}

<<<<<<< HEAD
<<<<<<< HEAD
//---------------------------------------------------------------------------
// Function:    ft1000_DestroyDeviceDEBUG
//
// Parameters:  dev - pointer to adapter object
//
// Description: Destroys a private char device.
//
// Notes:       Only called by cleanup_module().
//
//---------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
/*
*---------------------------------------------------------------------------
* Function:    ft1000_DestroyDeviceDEBUG
*
* Parameters:  dev - pointer to adapter object
*
* Description: Destroys a private char device.
*
* Notes:       Only called by cleanup_module().
*
*---------------------------------------------------------------------------
*/
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
void ft1000_destroy_dev(struct net_device *netdev)
{
	struct ft1000_info *info = netdev_priv(netdev);
	struct ft1000_usb *dev = info->priv;
		int i;
	struct dpram_blk *pdpram_blk;
	struct dpram_blk *ptr;
	struct list_head *pos, *q;
	struct ft1000_debug_dirs *dir;

    DEBUG("%s called\n", __func__);



<<<<<<< HEAD
<<<<<<< HEAD
    if (dev->DeviceCreated)
	{
=======
    if (dev->DeviceCreated) {
>>>>>>> v3.18
=======
    if (dev->DeviceCreated) {
>>>>>>> v3.18
        ft1000_flarion_cnt--;
		list_for_each_safe(pos, q, &dev->nodes.list) {
			dir = list_entry(pos, struct ft1000_debug_dirs, list);
			if (dir->int_number == dev->CardNumber) {
				debugfs_remove(dir->file);
				debugfs_remove(dir->dent);
				list_del(pos);
				kfree(dir);
			}
		}
		DEBUG("%s: unregistered device \"%s\"\n", __func__,
					   dev->DeviceName);

<<<<<<< HEAD
<<<<<<< HEAD
        // Make sure we free any memory reserve for slow Queue
=======
        /* Make sure we free any memory reserve for slow Queue */
>>>>>>> v3.18
=======
        /* Make sure we free any memory reserve for slow Queue */
>>>>>>> v3.18
        for (i=0; i<MAX_NUM_APP; i++) {
            while (list_empty(&dev->app_info[i].app_sqlist) == 0) {
                pdpram_blk = list_entry(dev->app_info[i].app_sqlist.next, struct dpram_blk, list);
                list_del(&pdpram_blk->list);
                ft1000_free_buffer(pdpram_blk, &freercvpool);

            }
            wake_up_interruptible(&dev->app_info[i].wait_dpram_msg);
        }

<<<<<<< HEAD
<<<<<<< HEAD
        // Remove buffer allocated for receive command data
=======
        /* Remove buffer allocated for receive command data */
>>>>>>> v3.18
=======
        /* Remove buffer allocated for receive command data */
>>>>>>> v3.18
        if (ft1000_flarion_cnt == 0) {
            while (list_empty(&freercvpool) == 0) {
		ptr = list_entry(freercvpool.next, struct dpram_blk, list);
                list_del(&ptr->list);
                kfree(ptr->pbuffer);
                kfree(ptr);
            }
        }
		dev->DeviceCreated = FALSE;
	}


}

<<<<<<< HEAD
<<<<<<< HEAD
//---------------------------------------------------------------------------
// Function:    ft1000_open
//
// Parameters:
//
// Description:
//
// Notes:
//
//---------------------------------------------------------------------------
static int ft1000_open (struct inode *inode, struct file *file)
=======
=======
>>>>>>> v3.18
/*
*---------------------------------------------------------------------------
* Function:    ft1000_open
*
* Parameters:
*
* Description:
*
* Notes:
*
*---------------------------------------------------------------------------
*/
static int ft1000_open(struct inode *inode, struct file *file)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	struct ft1000_info *info;
	struct ft1000_usb *dev = (struct ft1000_usb *)inode->i_private;
    int i,num;

    DEBUG("%s called\n", __func__);
    num = (MINOR(inode->i_rdev) & 0xf);
    DEBUG("ft1000_open: minor number=%d\n", num);

	info = file->private_data = netdev_priv(dev->net);

<<<<<<< HEAD
<<<<<<< HEAD
    DEBUG("f_owner = %p number of application = %d\n", (&file->f_owner), dev->appcnt );

    // Check if maximum number of application exceeded
=======
    DEBUG("f_owner = %p number of application = %d\n", (&file->f_owner), dev->appcnt);

    /* Check if maximum number of application exceeded */
>>>>>>> v3.18
=======
    DEBUG("f_owner = %p number of application = %d\n", (&file->f_owner), dev->appcnt);

    /* Check if maximum number of application exceeded */
>>>>>>> v3.18
    if (dev->appcnt > MAX_NUM_APP) {
        DEBUG("Maximum number of application exceeded\n");
        return -EACCES;
    }

<<<<<<< HEAD
<<<<<<< HEAD
    // Search for available application info block
    for (i=0; i<MAX_NUM_APP; i++) {
        if ( (dev->app_info[i].fileobject == NULL) ) {
=======
    /* Search for available application info block */
    for (i=0; i<MAX_NUM_APP; i++) {
        if ((dev->app_info[i].fileobject == NULL)) {
>>>>>>> v3.18
=======
    /* Search for available application info block */
    for (i=0; i<MAX_NUM_APP; i++) {
        if ((dev->app_info[i].fileobject == NULL)) {
>>>>>>> v3.18
            break;
        }
    }

<<<<<<< HEAD
<<<<<<< HEAD
    // Fail due to lack of application info block
=======
    /* Fail due to lack of application info block */
>>>>>>> v3.18
=======
    /* Fail due to lack of application info block */
>>>>>>> v3.18
    if (i == MAX_NUM_APP) {
        DEBUG("Could not find an application info block\n");
        return -EACCES;
    }

    dev->appcnt++;
    dev->app_info[i].fileobject = &file->f_owner;
    dev->app_info[i].nTxMsg = 0;
    dev->app_info[i].nRxMsg = 0;
    dev->app_info[i].nTxMsgReject = 0;
    dev->app_info[i].nRxMsgMiss = 0;

	nonseekable_open(inode, file);
    return 0;
}


<<<<<<< HEAD
<<<<<<< HEAD
//---------------------------------------------------------------------------
// Function:    ft1000_poll_dev
//
// Parameters:
//
// Description:
//
// Notes:
//
//---------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
/*
*---------------------------------------------------------------------------
* Function:    ft1000_poll_dev
*
* Parameters:
*
* Description:
*
* Notes:
*
*---------------------------------------------------------------------------
*/
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

static unsigned int ft1000_poll_dev(struct file *file, poll_table *wait)
{
    struct net_device *netdev = file->private_data;
	struct ft1000_info *info = netdev_priv(netdev);
	struct ft1000_usb *dev = info->priv;
    int i;

<<<<<<< HEAD
<<<<<<< HEAD
    //DEBUG("ft1000_poll_dev called\n");
=======
    /* DEBUG("ft1000_poll_dev called\n"); */
>>>>>>> v3.18
=======
    /* DEBUG("ft1000_poll_dev called\n"); */
>>>>>>> v3.18
    if (ft1000_flarion_cnt == 0) {
        DEBUG("FT1000:ft1000_poll_dev called when ft1000_flarion_cnt is zero\n");
        return (-EBADF);
    }

<<<<<<< HEAD
<<<<<<< HEAD
    // Search for matching file object
    for (i=0; i<MAX_NUM_APP; i++) {
        if ( dev->app_info[i].fileobject == &file->f_owner) {
            //DEBUG("FT1000:ft1000_ioctl: Message is for AppId = %d\n", dev->app_info[i].app_id);
=======
=======
>>>>>>> v3.18
    /* Search for matching file object */
    for (i=0; i<MAX_NUM_APP; i++) {
        if (dev->app_info[i].fileobject == &file->f_owner) {
            /* DEBUG("FT1000:ft1000_ioctl: Message is for AppId = %d\n", dev->app_info[i].app_id); */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
            break;
        }
    }

<<<<<<< HEAD
<<<<<<< HEAD
    // Could not find application info block
    if (i == MAX_NUM_APP) {
        DEBUG("FT1000:ft1000_ioctl:Could not find application info block\n");
        return ( -EACCES );
=======
=======
>>>>>>> v3.18
    /* Could not find application info block */
    if (i == MAX_NUM_APP) {
        DEBUG("FT1000:ft1000_ioctl:Could not find application info block\n");
        return (-EACCES);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
    }

    if (list_empty(&dev->app_info[i].app_sqlist) == 0) {
        DEBUG("FT1000:ft1000_poll_dev:Message detected in slow queue\n");
        return(POLLIN | POLLRDNORM | POLLPRI);
    }

<<<<<<< HEAD
<<<<<<< HEAD
    poll_wait (file, &dev->app_info[i].wait_dpram_msg, wait);
    //DEBUG("FT1000:ft1000_poll_dev:Polling for data from DSP\n");

    return (0);
}

//---------------------------------------------------------------------------
// Function:    ft1000_ioctl
//
// Parameters:
//
// Description:
//
// Notes:
//
//---------------------------------------------------------------------------
static long ft1000_ioctl (struct file *file, unsigned int command,
=======
=======
>>>>>>> v3.18
    poll_wait(file, &dev->app_info[i].wait_dpram_msg, wait);
    /* DEBUG("FT1000:ft1000_poll_dev:Polling for data from DSP\n"); */

	return 0;
}

/*
*---------------------------------------------------------------------------
* Function:    ft1000_ioctl
*
* Parameters:
*
* Description:
*
* Notes:
*
*---------------------------------------------------------------------------
*/
static long ft1000_ioctl(struct file *file, unsigned int command,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
                           unsigned long argument)
{
    void __user *argp = (void __user *)argument;
	struct ft1000_info *info;
    struct ft1000_usb *ft1000dev;
    int result=0;
    int cmd;
    int i;
    u16 tempword;
    unsigned long flags;
    struct timeval tv;
<<<<<<< HEAD
<<<<<<< HEAD
    IOCTL_GET_VER get_ver_data;
    IOCTL_GET_DSP_STAT get_stat_data;
=======
	struct IOCTL_GET_VER get_ver_data;
	struct IOCTL_GET_DSP_STAT get_stat_data;
>>>>>>> v3.18
=======
	struct IOCTL_GET_VER get_ver_data;
	struct IOCTL_GET_DSP_STAT get_stat_data;
>>>>>>> v3.18
    u8 ConnectionMsg[] = {0x00,0x44,0x10,0x20,0x80,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x93,0x64,
                          0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x0a,
                          0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                          0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                          0x00,0x00,0x02,0x37,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x01,0x00,0x01,0x7f,0x00,
                          0x00,0x01,0x00,0x00};

    unsigned short ledStat=0;
    unsigned short conStat=0;

<<<<<<< HEAD
<<<<<<< HEAD
    //DEBUG("ft1000_ioctl called\n");
=======
    /* DEBUG("ft1000_ioctl called\n"); */
>>>>>>> v3.18
=======
    /* DEBUG("ft1000_ioctl called\n"); */
>>>>>>> v3.18

    if (ft1000_flarion_cnt == 0) {
        DEBUG("FT1000:ft1000_ioctl called when ft1000_flarion_cnt is zero\n");
        return (-EBADF);
    }

<<<<<<< HEAD
<<<<<<< HEAD
    //DEBUG("FT1000:ft1000_ioctl:command = 0x%x argument = 0x%8x\n", command, (u32)argument);
=======
    /* DEBUG("FT1000:ft1000_ioctl:command = 0x%x argument = 0x%8x\n", command, (u32)argument); */
>>>>>>> v3.18
=======
    /* DEBUG("FT1000:ft1000_ioctl:command = 0x%x argument = 0x%8x\n", command, (u32)argument); */
>>>>>>> v3.18

	info = file->private_data;
	ft1000dev = info->priv;
    cmd = _IOC_NR(command);
<<<<<<< HEAD
<<<<<<< HEAD
    //DEBUG("FT1000:ft1000_ioctl:cmd = 0x%x\n", cmd);

    // process the command
=======
    /* DEBUG("FT1000:ft1000_ioctl:cmd = 0x%x\n", cmd); */

    /* process the command */
>>>>>>> v3.18
=======
    /* DEBUG("FT1000:ft1000_ioctl:cmd = 0x%x\n", cmd); */

    /* process the command */
>>>>>>> v3.18
    switch (cmd) {
    case IOCTL_REGISTER_CMD:
            DEBUG("FT1000:ft1000_ioctl: IOCTL_FT1000_REGISTER called\n");
            result = get_user(tempword, (__u16 __user*)argp);
            if (result) {
                DEBUG("result = %d failed to get_user\n", result);
                break;
            }
            if (tempword == DSPBCMSGID) {
<<<<<<< HEAD
<<<<<<< HEAD
                // Search for matching file object
=======
                /* Search for matching file object */
>>>>>>> v3.18
=======
                /* Search for matching file object */
>>>>>>> v3.18
                for (i=0; i<MAX_NUM_APP; i++) {
                    if (ft1000dev->app_info[i].fileobject == &file->f_owner) {
                        ft1000dev->app_info[i].DspBCMsgFlag = 1;
                        DEBUG("FT1000:ft1000_ioctl:Registered for broadcast messages\n");
                        break;
                    }
                }
            }
            break;

    case IOCTL_GET_VER_CMD:
        DEBUG("FT1000:ft1000_ioctl: IOCTL_FT1000_GET_VER called\n");

        get_ver_data.drv_ver = FT1000_DRV_VER;

<<<<<<< HEAD
<<<<<<< HEAD
        if (copy_to_user(argp, &get_ver_data, sizeof(get_ver_data)) ) {
=======
        if (copy_to_user(argp, &get_ver_data, sizeof(get_ver_data))) {
>>>>>>> v3.18
=======
        if (copy_to_user(argp, &get_ver_data, sizeof(get_ver_data))) {
>>>>>>> v3.18
            DEBUG("FT1000:ft1000_ioctl: copy fault occurred\n");
            result = -EFAULT;
            break;
        }

        DEBUG("FT1000:ft1000_ioctl:driver version = 0x%x\n",(unsigned int)get_ver_data.drv_ver);

        break;
    case IOCTL_CONNECT:
<<<<<<< HEAD
<<<<<<< HEAD
        // Connect Message
        DEBUG("FT1000:ft1000_ioctl: IOCTL_FT1000_CONNECT\n");
        ConnectionMsg[79] = 0xfc;
			   card_send_command(ft1000dev, (unsigned short *)ConnectionMsg, 0x4c);

        break;
    case IOCTL_DISCONNECT:
        // Disconnect Message
        DEBUG("FT1000:ft1000_ioctl: IOCTL_FT1000_DISCONNECT\n");
        ConnectionMsg[79] = 0xfd;
			   card_send_command(ft1000dev, (unsigned short *)ConnectionMsg, 0x4c);
        break;
    case IOCTL_GET_DSP_STAT_CMD:
        //DEBUG("FT1000:ft1000_ioctl: IOCTL_FT1000_GET_DSP_STAT called\n");
=======
=======
>>>>>>> v3.18
        /* Connect Message */
        DEBUG("FT1000:ft1000_ioctl: IOCTL_FT1000_CONNECT\n");
        ConnectionMsg[79] = 0xfc;
			   result = card_send_command(ft1000dev, (unsigned short *)ConnectionMsg, 0x4c);

        break;
    case IOCTL_DISCONNECT:
        /* Disconnect Message */
        DEBUG("FT1000:ft1000_ioctl: IOCTL_FT1000_DISCONNECT\n");
        ConnectionMsg[79] = 0xfd;
			   result = card_send_command(ft1000dev, (unsigned short *)ConnectionMsg, 0x4c);
        break;
    case IOCTL_GET_DSP_STAT_CMD:
        /* DEBUG("FT1000:ft1000_ioctl: IOCTL_FT1000_GET_DSP_STAT called\n"); */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	memset(&get_stat_data, 0, sizeof(get_stat_data));
        memcpy(get_stat_data.DspVer, info->DspVer, DSPVERSZ);
        memcpy(get_stat_data.HwSerNum, info->HwSerNum, HWSERNUMSZ);
        memcpy(get_stat_data.Sku, info->Sku, SKUSZ);
        memcpy(get_stat_data.eui64, info->eui64, EUISZ);

            if (info->ProgConStat != 0xFF) {
                ft1000_read_dpram16(ft1000dev, FT1000_MAG_DSP_LED, (u8 *)&ledStat, FT1000_MAG_DSP_LED_INDX);
                get_stat_data.LedStat = ntohs(ledStat);
                DEBUG("FT1000:ft1000_ioctl: LedStat = 0x%x\n", get_stat_data.LedStat);
                ft1000_read_dpram16(ft1000dev, FT1000_MAG_DSP_CON_STATE, (u8 *)&conStat, FT1000_MAG_DSP_CON_STATE_INDX);
                get_stat_data.ConStat = ntohs(conStat);
                DEBUG("FT1000:ft1000_ioctl: ConStat = 0x%x\n", get_stat_data.ConStat);
<<<<<<< HEAD
<<<<<<< HEAD
            }
            else {
=======
            } else {
>>>>>>> v3.18
=======
            } else {
>>>>>>> v3.18
                get_stat_data.ConStat = 0x0f;
            }


        get_stat_data.nTxPkts = info->stats.tx_packets;
        get_stat_data.nRxPkts = info->stats.rx_packets;
        get_stat_data.nTxBytes = info->stats.tx_bytes;
        get_stat_data.nRxBytes = info->stats.rx_bytes;
<<<<<<< HEAD
<<<<<<< HEAD
        do_gettimeofday ( &tv );
        get_stat_data.ConTm = (u32)(tv.tv_sec - info->ConTm);
        DEBUG("Connection Time = %d\n", (int)get_stat_data.ConTm);
        if (copy_to_user(argp, &get_stat_data, sizeof(get_stat_data)) ) {
=======
=======
>>>>>>> v3.18
        do_gettimeofday(&tv);
        get_stat_data.ConTm = (u32)(tv.tv_sec - info->ConTm);
        DEBUG("Connection Time = %d\n", (int)get_stat_data.ConTm);
        if (copy_to_user(argp, &get_stat_data, sizeof(get_stat_data))) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
            DEBUG("FT1000:ft1000_ioctl: copy fault occurred\n");
            result = -EFAULT;
            break;
        }
        DEBUG("ft1000_chioctl: GET_DSP_STAT succeed\n");
        break;
    case IOCTL_SET_DPRAM_CMD:
        {
<<<<<<< HEAD
<<<<<<< HEAD
            IOCTL_DPRAM_BLK *dpram_data = NULL;
            //IOCTL_DPRAM_COMMAND dpram_command;
=======
		struct IOCTL_DPRAM_BLK *dpram_data = NULL;
		/* struct IOCTL_DPRAM_COMMAND dpram_command; */
>>>>>>> v3.18
=======
		struct IOCTL_DPRAM_BLK *dpram_data = NULL;
		/* struct IOCTL_DPRAM_COMMAND dpram_command; */
>>>>>>> v3.18
            u16 qtype;
            u16 msgsz;
		struct pseudo_hdr *ppseudo_hdr;
            u16 *pmsg;
            u16 total_len;
            u16 app_index;
            u16 status;

<<<<<<< HEAD
<<<<<<< HEAD
            //DEBUG("FT1000:ft1000_ioctl: IOCTL_FT1000_SET_DPRAM called\n");
=======
            /* DEBUG("FT1000:ft1000_ioctl: IOCTL_FT1000_SET_DPRAM called\n");*/
>>>>>>> v3.18
=======
            /* DEBUG("FT1000:ft1000_ioctl: IOCTL_FT1000_SET_DPRAM called\n");*/
>>>>>>> v3.18


            if (ft1000_flarion_cnt == 0) {
                return (-EBADF);
            }

            if (ft1000dev->DrvMsgPend) {
                return (-ENOTTY);
            }

            if (ft1000dev->fProvComplete == 0) {
                return (-EACCES);
            }

            ft1000dev->fAppMsgPend = 1;

            if (info->CardReady) {

<<<<<<< HEAD
<<<<<<< HEAD
               //DEBUG("FT1000:ft1000_ioctl: try to SET_DPRAM \n");

                // Get the length field to see how many bytes to copy
                result = get_user(msgsz, (__u16 __user *)argp);
                msgsz = ntohs (msgsz);
                //DEBUG("FT1000:ft1000_ioctl: length of message = %d\n", msgsz);
=======
=======
>>>>>>> v3.18
               /* DEBUG("FT1000:ft1000_ioctl: try to SET_DPRAM \n"); */

                /* Get the length field to see how many bytes to copy */
                result = get_user(msgsz, (__u16 __user *)argp);
		if (result)
			break;
                msgsz = ntohs(msgsz);
                /* DEBUG("FT1000:ft1000_ioctl: length of message = %d\n", msgsz); */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

                if (msgsz > MAX_CMD_SQSIZE) {
                    DEBUG("FT1000:ft1000_ioctl: bad message length = %d\n", msgsz);
                    result = -EINVAL;
                    break;
                }

		result = -ENOMEM;
		dpram_data = kmalloc(msgsz + 2, GFP_KERNEL);
		if (!dpram_data)
			break;

<<<<<<< HEAD
<<<<<<< HEAD
                if ( copy_from_user(dpram_data, argp, msgsz+2) ) {
                    DEBUG("FT1000:ft1000_ChIoctl: copy fault occurred\n");
                    result = -EFAULT;
                }
                else {
                    // Check if this message came from a registered application
=======
=======
>>>>>>> v3.18
                if (copy_from_user(dpram_data, argp, msgsz+2)) {
                    DEBUG("FT1000:ft1000_ChIoctl: copy fault occurred\n");
                    result = -EFAULT;
                } else {
                    /* Check if this message came from a registered application */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
                    for (i=0; i<MAX_NUM_APP; i++) {
                        if (ft1000dev->app_info[i].fileobject == &file->f_owner) {
                            break;
                        }
                    }
                    if (i==MAX_NUM_APP) {
                        DEBUG("FT1000:No matching application fileobject\n");
                        result = -EINVAL;
			kfree(dpram_data);
                        break;
                    }
                    app_index = i;

<<<<<<< HEAD
<<<<<<< HEAD
                    // Check message qtype type which is the lower byte within qos_class
                    qtype = ntohs(dpram_data->pseudohdr.qos_class) & 0xff;
                    //DEBUG("FT1000_ft1000_ioctl: qtype = %d\n", qtype);
                    if (qtype) {
                    }
                    else {
                        // Put message into Slow Queue
                        // Only put a message into the DPRAM if msg doorbell is available
                        status = ft1000_read_register(ft1000dev, &tempword, FT1000_REG_DOORBELL);
                        //DEBUG("FT1000_ft1000_ioctl: READ REGISTER tempword=%x\n", tempword);
                        if (tempword & FT1000_DB_DPRAM_TX) {
                            // Suspend for 2ms and try again due to DSP doorbell busy
                            mdelay(2);
                            status = ft1000_read_register(ft1000dev, &tempword, FT1000_REG_DOORBELL);
                            if (tempword & FT1000_DB_DPRAM_TX) {
                                // Suspend for 1ms and try again due to DSP doorbell busy
=======
=======
>>>>>>> v3.18
                    /* Check message qtype type which is the lower byte within qos_class */
                    qtype = ntohs(dpram_data->pseudohdr.qos_class) & 0xff;
                    /* DEBUG("FT1000_ft1000_ioctl: qtype = %d\n", qtype); */
                    if (qtype) {
                    } else {
                        /* Put message into Slow Queue */
                        /* Only put a message into the DPRAM if msg doorbell is available */
                        status = ft1000_read_register(ft1000dev, &tempword, FT1000_REG_DOORBELL);
                        /* DEBUG("FT1000_ft1000_ioctl: READ REGISTER tempword=%x\n", tempword); */
                        if (tempword & FT1000_DB_DPRAM_TX) {
                            /* Suspend for 2ms and try again due to DSP doorbell busy */
                            mdelay(2);
                            status = ft1000_read_register(ft1000dev, &tempword, FT1000_REG_DOORBELL);
                            if (tempword & FT1000_DB_DPRAM_TX) {
                                /* Suspend for 1ms and try again due to DSP doorbell busy */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
                                mdelay(1);
                                status = ft1000_read_register(ft1000dev, &tempword, FT1000_REG_DOORBELL);
                                if (tempword & FT1000_DB_DPRAM_TX) {
                                    status = ft1000_read_register(ft1000dev, &tempword, FT1000_REG_DOORBELL);
                                    if (tempword & FT1000_DB_DPRAM_TX) {
<<<<<<< HEAD
<<<<<<< HEAD
                                        // Suspend for 3ms and try again due to DSP doorbell busy
=======
                                        /* Suspend for 3ms and try again due to DSP doorbell busy */
>>>>>>> v3.18
=======
                                        /* Suspend for 3ms and try again due to DSP doorbell busy */
>>>>>>> v3.18
                                        mdelay(3);
                                        status = ft1000_read_register(ft1000dev, &tempword, FT1000_REG_DOORBELL);
                                        if (tempword & FT1000_DB_DPRAM_TX) {
                                            DEBUG("FT1000:ft1000_ioctl:Doorbell not available\n");
                                            result = -ENOTTY;
						kfree(dpram_data);
                                            break;
                                        }
                                    }
                                }
                            }
                        }

<<<<<<< HEAD
<<<<<<< HEAD
                        //DEBUG("FT1000_ft1000_ioctl: finished reading register\n");

                        // Make sure we are within the limits of the slow queue memory limitation
                        if ( (msgsz < MAX_CMD_SQSIZE) && (msgsz > PSEUDOSZ) ) {
                            // Need to put sequence number plus new checksum for message
=======
=======
>>>>>>> v3.18
                        /*DEBUG("FT1000_ft1000_ioctl: finished reading register\n"); */

                        /* Make sure we are within the limits of the slow queue memory limitation */
                        if ((msgsz < MAX_CMD_SQSIZE) && (msgsz > PSEUDOSZ)) {
                            /* Need to put sequence number plus new checksum for message */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
                            pmsg = (u16 *)&dpram_data->pseudohdr;
				ppseudo_hdr = (struct pseudo_hdr *)pmsg;
                            total_len = msgsz+2;
                            if (total_len & 0x1) {
                                total_len++;
                            }

<<<<<<< HEAD
<<<<<<< HEAD
                            // Insert slow queue sequence number
                            ppseudo_hdr->seq_num = info->squeseqnum++;
                            ppseudo_hdr->portsrc = ft1000dev->app_info[app_index].app_id;
                            // Calculate new checksum
                            ppseudo_hdr->checksum = *pmsg++;
                            //DEBUG("checksum = 0x%x\n", ppseudo_hdr->checksum);
                            for (i=1; i<7; i++) {
                                ppseudo_hdr->checksum ^= *pmsg++;
                                //DEBUG("checksum = 0x%x\n", ppseudo_hdr->checksum);
                            }
                            pmsg++;
				ppseudo_hdr = (struct pseudo_hdr *)pmsg;
                           card_send_command(ft1000dev,(unsigned short*)dpram_data,total_len+2);


                            ft1000dev->app_info[app_index].nTxMsg++;
                        }
                        else {
=======
=======
>>>>>>> v3.18
                            /* Insert slow queue sequence number */
                            ppseudo_hdr->seq_num = info->squeseqnum++;
                            ppseudo_hdr->portsrc = ft1000dev->app_info[app_index].app_id;
                            /* Calculate new checksum */
                            ppseudo_hdr->checksum = *pmsg++;
                            /* DEBUG("checksum = 0x%x\n", ppseudo_hdr->checksum); */
                            for (i=1; i<7; i++) {
                                ppseudo_hdr->checksum ^= *pmsg++;
                                /* DEBUG("checksum = 0x%x\n", ppseudo_hdr->checksum); */
                            }
                            pmsg++;
				ppseudo_hdr = (struct pseudo_hdr *)pmsg;
                           result = card_send_command(ft1000dev,(unsigned short*)dpram_data,total_len+2);


                            ft1000dev->app_info[app_index].nTxMsg++;
                        } else {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
                            result = -EINVAL;
                        }
                    }
                }
<<<<<<< HEAD
<<<<<<< HEAD
            }
            else {
=======
            } else {
>>>>>>> v3.18
=======
            } else {
>>>>>>> v3.18
                DEBUG("FT1000:ft1000_ioctl: Card not ready take messages\n");
                result = -EACCES;
            }
	    kfree(dpram_data);

        }
        break;
    case IOCTL_GET_DPRAM_CMD:
        {
		struct dpram_blk *pdpram_blk;
<<<<<<< HEAD
<<<<<<< HEAD
            IOCTL_DPRAM_BLK __user *pioctl_dpram;
            int msglen;

            //DEBUG("FT1000:ft1000_ioctl: IOCTL_FT1000_GET_DPRAM called\n");
=======
=======
>>>>>>> v3.18
		struct IOCTL_DPRAM_BLK __user *pioctl_dpram;
            int msglen;

            /* DEBUG("FT1000:ft1000_ioctl: IOCTL_FT1000_GET_DPRAM called\n"); */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

            if (ft1000_flarion_cnt == 0) {
                return (-EBADF);
            }

<<<<<<< HEAD
<<<<<<< HEAD
            // Search for matching file object
            for (i=0; i<MAX_NUM_APP; i++) {
                if (ft1000dev->app_info[i].fileobject == &file->f_owner) {
                    //DEBUG("FT1000:ft1000_ioctl: Message is for AppId = %d\n", ft1000dev->app_info[i].app_id);
=======
=======
>>>>>>> v3.18
            /* Search for matching file object */
            for (i=0; i<MAX_NUM_APP; i++) {
                if (ft1000dev->app_info[i].fileobject == &file->f_owner) {
                    /*DEBUG("FT1000:ft1000_ioctl: Message is for AppId = %d\n", ft1000dev->app_info[i].app_id); */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
                    break;
                }
            }

<<<<<<< HEAD
<<<<<<< HEAD
            // Could not find application info block
=======
            /* Could not find application info block */
>>>>>>> v3.18
=======
            /* Could not find application info block */
>>>>>>> v3.18
            if (i == MAX_NUM_APP) {
                DEBUG("FT1000:ft1000_ioctl:Could not find application info block\n");
                result = -EBADF;
                break;
            }

            result = 0;
            pioctl_dpram = argp;
            if (list_empty(&ft1000dev->app_info[i].app_sqlist) == 0) {
<<<<<<< HEAD
<<<<<<< HEAD
                //DEBUG("FT1000:ft1000_ioctl:Message detected in slow queue\n");
=======
                /* DEBUG("FT1000:ft1000_ioctl:Message detected in slow queue\n"); */
>>>>>>> v3.18
=======
                /* DEBUG("FT1000:ft1000_ioctl:Message detected in slow queue\n"); */
>>>>>>> v3.18
                spin_lock_irqsave(&free_buff_lock, flags);
                pdpram_blk = list_entry(ft1000dev->app_info[i].app_sqlist.next, struct dpram_blk, list);
                list_del(&pdpram_blk->list);
                ft1000dev->app_info[i].NumOfMsg--;
<<<<<<< HEAD
<<<<<<< HEAD
                //DEBUG("FT1000:ft1000_ioctl:NumOfMsg for app %d = %d\n", i, ft1000dev->app_info[i].NumOfMsg);
=======
                /* DEBUG("FT1000:ft1000_ioctl:NumOfMsg for app %d = %d\n", i, ft1000dev->app_info[i].NumOfMsg); */
>>>>>>> v3.18
=======
                /* DEBUG("FT1000:ft1000_ioctl:NumOfMsg for app %d = %d\n", i, ft1000dev->app_info[i].NumOfMsg); */
>>>>>>> v3.18
                spin_unlock_irqrestore(&free_buff_lock, flags);
                msglen = ntohs(*(u16 *)pdpram_blk->pbuffer) + PSEUDOSZ;
                result = get_user(msglen, &pioctl_dpram->total_len);
		if (result)
			break;
		msglen = htons(msglen);
<<<<<<< HEAD
<<<<<<< HEAD
                //DEBUG("FT1000:ft1000_ioctl:msg length = %x\n", msglen);
                if(copy_to_user (&pioctl_dpram->pseudohdr, pdpram_blk->pbuffer, msglen))
				{
					DEBUG("FT1000:ft1000_ioctl: copy fault occurred\n");
	             	result = -EFAULT;
	             	break;
=======
=======
>>>>>>> v3.18
                /* DEBUG("FT1000:ft1000_ioctl:msg length = %x\n", msglen); */
                if (copy_to_user (&pioctl_dpram->pseudohdr, pdpram_blk->pbuffer, msglen)) {
					DEBUG("FT1000:ft1000_ioctl: copy fault occurred\n");
			result = -EFAULT;
			break;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
				}

                ft1000_free_buffer(pdpram_blk, &freercvpool);
                result = msglen;
            }
<<<<<<< HEAD
<<<<<<< HEAD
            //DEBUG("FT1000:ft1000_ioctl: IOCTL_FT1000_GET_DPRAM no message\n");
=======
            /* DEBUG("FT1000:ft1000_ioctl: IOCTL_FT1000_GET_DPRAM no message\n"); */
>>>>>>> v3.18
=======
            /* DEBUG("FT1000:ft1000_ioctl: IOCTL_FT1000_GET_DPRAM no message\n"); */
>>>>>>> v3.18
        }
        break;

    default:
        DEBUG("FT1000:ft1000_ioctl:unknown command: 0x%x\n", command);
        result = -ENOTTY;
        break;
    }
    ft1000dev->fAppMsgPend = 0;
    return result;
}

<<<<<<< HEAD
<<<<<<< HEAD
//---------------------------------------------------------------------------
// Function:    ft1000_release
//
// Parameters:
//
// Description:
//
// Notes:
//
//---------------------------------------------------------------------------
static int ft1000_release (struct inode *inode, struct file *file)
=======
=======
>>>>>>> v3.18
/*
*---------------------------------------------------------------------------
* Function:    ft1000_release
*
* Parameters:
*
* Description:
*
* Notes:
*
*---------------------------------------------------------------------------
*/
static int ft1000_release(struct inode *inode, struct file *file)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	struct ft1000_info *info;
    struct net_device *dev;
    struct ft1000_usb *ft1000dev;
    int i;
	struct dpram_blk *pdpram_blk;

    DEBUG("ft1000_release called\n");

    dev = file->private_data;
	info = netdev_priv(dev);
	ft1000dev = info->priv;

    if (ft1000_flarion_cnt == 0) {
        ft1000dev->appcnt--;
        return (-EBADF);
    }

<<<<<<< HEAD
<<<<<<< HEAD
    // Search for matching file object
    for (i=0; i<MAX_NUM_APP; i++) {
        if ( ft1000dev->app_info[i].fileobject == &file->f_owner) {
            //DEBUG("FT1000:ft1000_ioctl: Message is for AppId = %d\n", ft1000dev->app_info[i].app_id);
=======
=======
>>>>>>> v3.18
    /* Search for matching file object */
    for (i=0; i<MAX_NUM_APP; i++) {
        if (ft1000dev->app_info[i].fileobject == &file->f_owner) {
            /* DEBUG("FT1000:ft1000_ioctl: Message is for AppId = %d\n", ft1000dev->app_info[i].app_id); */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
            break;
        }
    }

    if (i==MAX_NUM_APP)
	    return 0;

    while (list_empty(&ft1000dev->app_info[i].app_sqlist) == 0) {
        DEBUG("Remove and free memory queue up on slow queue\n");
        pdpram_blk = list_entry(ft1000dev->app_info[i].app_sqlist.next, struct dpram_blk, list);
        list_del(&pdpram_blk->list);
        ft1000_free_buffer(pdpram_blk, &freercvpool);
    }

<<<<<<< HEAD
<<<<<<< HEAD
    // initialize application information
    ft1000dev->appcnt--;
    DEBUG("ft1000_chdev:%s:appcnt = %d\n", __FUNCTION__, ft1000dev->appcnt);
=======
    /* initialize application information */
    ft1000dev->appcnt--;
    DEBUG("ft1000_chdev:%s:appcnt = %d\n", __func__, ft1000dev->appcnt);
>>>>>>> v3.18
=======
    /* initialize application information */
    ft1000dev->appcnt--;
    DEBUG("ft1000_chdev:%s:appcnt = %d\n", __func__, ft1000dev->appcnt);
>>>>>>> v3.18
    ft1000dev->app_info[i].fileobject = NULL;

    return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
