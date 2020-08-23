/*
 * Copyright (c) 1996, 2003 VIA Networking Technologies, Inc.
 * All rights reserved.
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *
 * File: wpactl.c
 *
 * Purpose: handle wpa supplicant ioctl input/out functions
 *
 * Author: Lyndon Chen
 *
 * Date: Oct. 20, 2003
 *
 * Functions:
 *
 * Revision History:
 *
 */

#include "wpactl.h"
#include "key.h"
#include "mac.h"
#include "device.h"
#include "wmgr.h"
#include "iocmd.h"
#include "iowpa.h"
#include "rf.h"

/*---------------------  Static Definitions -------------------------*/

#define VIAWGET_WPA_MAX_BUF_SIZE 1024

static const int frequency_list[] = {
	2412, 2417, 2422, 2427, 2432, 2437, 2442,
	2447, 2452, 2457, 2462, 2467, 2472, 2484
};
/*---------------------  Static Classes  ----------------------------*/

<<<<<<< HEAD
<<<<<<< HEAD
/*---------------------  Static Variables  --------------------------*/
//static int          msglevel                =MSG_LEVEL_DEBUG;
static int msglevel = MSG_LEVEL_INFO;

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
/*---------------------  Static Functions  --------------------------*/

/*---------------------  Export Variables  --------------------------*/
static void wpadev_setup(struct net_device *dev)
{
	dev->type               = ARPHRD_IEEE80211;
	dev->hard_header_len    = ETH_HLEN;
	dev->mtu                = 2048;
	dev->addr_len           = ETH_ALEN;
	dev->tx_queue_len       = 1000;

	memset(dev->broadcast, 0xFF, ETH_ALEN);

	dev->flags              = IFF_BROADCAST|IFF_MULTICAST;
}

/*
 * Description:
 *      register netdev for wpa supplicant daemon
 *
 * Parameters:
 *  In:
 *      pDevice             -
 *      enable              -
 *  Out:
 *
 * Return Value:
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
static int wpa_init_wpadev(PSDevice pDevice)
{
	PSDevice wpadev_priv;
	struct net_device *dev = pDevice->dev;
	int ret = 0;

	pDevice->wpadev = alloc_netdev(sizeof(PSDevice), "vntwpa", wpadev_setup);
=======
=======
>>>>>>> v3.18
static int wpa_init_wpadev(struct vnt_private *pDevice)
{
	struct vnt_private *wpadev_priv;
	struct net_device *dev = pDevice->dev;
	int ret = 0;

	pDevice->wpadev = alloc_netdev(sizeof(*wpadev_priv), "vntwpa",
				       NET_NAME_UNKNOWN, wpadev_setup);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (pDevice->wpadev == NULL)
		return -ENOMEM;

	wpadev_priv = netdev_priv(pDevice->wpadev);
	*wpadev_priv = *pDevice;
<<<<<<< HEAD
<<<<<<< HEAD
	memcpy(pDevice->wpadev->dev_addr, dev->dev_addr, ETH_ALEN);
=======
	eth_hw_addr_inherit(pDevice->wpadev, dev);
>>>>>>> v3.18
=======
	eth_hw_addr_inherit(pDevice->wpadev, dev);
>>>>>>> v3.18
	pDevice->wpadev->base_addr = dev->base_addr;
	pDevice->wpadev->irq = dev->irq;
	pDevice->wpadev->mem_start = dev->mem_start;
	pDevice->wpadev->mem_end = dev->mem_end;
	ret = register_netdev(pDevice->wpadev);
	if (ret) {
<<<<<<< HEAD
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "%s: register_netdev(WPA) failed!\n",
			dev->name);
=======
		pr_debug("%s: register_netdev(WPA) failed!\n", dev->name);
>>>>>>> v3.18
=======
		pr_debug("%s: register_netdev(WPA) failed!\n", dev->name);
>>>>>>> v3.18
		free_netdev(pDevice->wpadev);
		return -1;
	}

	if (pDevice->skb == NULL) {
		pDevice->skb = dev_alloc_skb((int)pDevice->rx_buf_sz);
		if (pDevice->skb == NULL)
			return -ENOMEM;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "%s: Registered netdev %s for WPA management\n",
		dev->name, pDevice->wpadev->name);
=======
	pr_debug("%s: Registered netdev %s for WPA management\n",
		 dev->name, pDevice->wpadev->name);
>>>>>>> v3.18
=======
	pr_debug("%s: Registered netdev %s for WPA management\n",
		 dev->name, pDevice->wpadev->name);
>>>>>>> v3.18

	return 0;
}

/*
 * Description:
 *      unregister net_device (wpadev)
 *
 * Parameters:
 *  In:
 *      pDevice             -
 *  Out:
 *
 * Return Value:
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
static int wpa_release_wpadev(PSDevice pDevice)
=======
static int wpa_release_wpadev(struct vnt_private *pDevice)
>>>>>>> v3.18
=======
static int wpa_release_wpadev(struct vnt_private *pDevice)
>>>>>>> v3.18
{
	if (pDevice->skb) {
		dev_kfree_skb(pDevice->skb);
		pDevice->skb = NULL;
	}

	if (pDevice->wpadev) {
<<<<<<< HEAD
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "%s: Netdevice %s unregistered\n",
			pDevice->dev->name, pDevice->wpadev->name);
=======
		pr_debug("%s: Netdevice %s unregistered\n",
			 pDevice->dev->name, pDevice->wpadev->name);
>>>>>>> v3.18
=======
		pr_debug("%s: Netdevice %s unregistered\n",
			 pDevice->dev->name, pDevice->wpadev->name);
>>>>>>> v3.18
		unregister_netdev(pDevice->wpadev);
		free_netdev(pDevice->wpadev);
		pDevice->wpadev = NULL;
	}

	return 0;
}

/*
 * Description:
 *      Set enable/disable dev for wpa supplicant daemon
 *
 * Parameters:
 *  In:
 *      pDevice             -
 *      val                 -
 *  Out:
 *
 * Return Value:
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
int wpa_set_wpadev(PSDevice pDevice, int val)
=======
int wpa_set_wpadev(struct vnt_private *pDevice, int val)
>>>>>>> v3.18
=======
int wpa_set_wpadev(struct vnt_private *pDevice, int val)
>>>>>>> v3.18
{
	if (val)
		return wpa_init_wpadev(pDevice);
	else
		return wpa_release_wpadev(pDevice);
}

/*
 * Description:
 *      Set WPA algorithm & keys
 *
 * Parameters:
 *  In:
 *      pDevice -
 *      param -
 *  Out:
 *
 * Return Value:
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
int wpa_set_keys(PSDevice pDevice, void *ctx, bool fcpfkernel)
=======
int wpa_set_keys(struct vnt_private *pDevice, void *ctx,
		 bool fcpfkernel) __must_hold(&pDevice->lock)
>>>>>>> v3.18
=======
int wpa_set_keys(struct vnt_private *pDevice, void *ctx,
		 bool fcpfkernel) __must_hold(&pDevice->lock)
>>>>>>> v3.18
{
	struct viawget_wpa_param *param = ctx;
	PSMgmtObject pMgmt = pDevice->pMgmt;
	unsigned long dwKeyIndex = 0;
	unsigned char abyKey[MAX_KEY_LEN];
	unsigned char abySeq[MAX_KEY_LEN];
<<<<<<< HEAD
<<<<<<< HEAD
	QWORD   KeyRSC;
//    NDIS_802_11_KEY_RSC KeyRSC;
=======
	u64 KeyRSC;
>>>>>>> v3.18
=======
	u64 KeyRSC;
>>>>>>> v3.18
	unsigned char byKeyDecMode = KEY_CTL_WEP;
	int ret = 0;
	int uu, ii;

	if (param->u.wpa_key.alg_name > WPA_ALG_CCMP ||
<<<<<<< HEAD
<<<<<<< HEAD
	    param->u.wpa_key.key_len >= MAX_KEY_LEN ||
	    param->u.wpa_key.seq_len >= MAX_KEY_LEN)
		return -EINVAL;

	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "param->u.wpa_key.alg_name = %d \n", param->u.wpa_key.alg_name);
=======
=======
>>>>>>> v3.18
	    param->u.wpa_key.key_len > MAX_KEY_LEN ||
	    param->u.wpa_key.seq_len > MAX_KEY_LEN)
		return -EINVAL;

	pr_debug("param->u.wpa_key.alg_name = %d\n", param->u.wpa_key.alg_name);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (param->u.wpa_key.alg_name == WPA_ALG_NONE) {
		pDevice->eEncryptionStatus = Ndis802_11EncryptionDisabled;
		pDevice->bEncryptionEnable = false;
		pDevice->byKeyIndex = 0;
		pDevice->bTransmitKey = false;
		KeyvRemoveAllWEPKey(&(pDevice->sKey), pDevice->PortOffset);
<<<<<<< HEAD
<<<<<<< HEAD
		for (uu = 0; uu < MAX_KEY_TABLE; uu++) {
			MACvDisableKeyEntry(pDevice->PortOffset, uu);
		}
		return ret;
	}

	//spin_unlock_irq(&pDevice->lock);
=======
=======
>>>>>>> v3.18
		for (uu = 0; uu < MAX_KEY_TABLE; uu++)
			MACvDisableKeyEntry(pDevice->PortOffset, uu);

		return ret;
	}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (param->u.wpa_key.key && fcpfkernel) {
		memcpy(&abyKey[0], param->u.wpa_key.key, param->u.wpa_key.key_len);
	} else {
		spin_unlock_irq(&pDevice->lock);
		if (param->u.wpa_key.key &&
<<<<<<< HEAD
<<<<<<< HEAD
		    copy_from_user(&abyKey[0], param->u.wpa_key.key, param->u.wpa_key.key_len)) {
=======
		    copy_from_user(&abyKey[0],
				   (void __user *)param->u.wpa_key.key,
				   param->u.wpa_key.key_len)) {
>>>>>>> v3.18
=======
		    copy_from_user(&abyKey[0],
				   (void __user *)param->u.wpa_key.key,
				   param->u.wpa_key.key_len)) {
>>>>>>> v3.18
			spin_lock_irq(&pDevice->lock);
			return -EINVAL;
		}
		spin_lock_irq(&pDevice->lock);
	}

	dwKeyIndex = (unsigned long)(param->u.wpa_key.key_index);

	if (param->u.wpa_key.alg_name == WPA_ALG_WEP) {
		if (dwKeyIndex > 3) {
			return -EINVAL;
		} else {
			if (param->u.wpa_key.set_tx) {
				pDevice->byKeyIndex = (unsigned char)dwKeyIndex;
				pDevice->bTransmitKey = true;
				dwKeyIndex |= (1 << 31);
			}
			KeybSetDefaultKey(&(pDevice->sKey),
					  dwKeyIndex & ~(BIT30 | USE_KEYRSC),
					  param->u.wpa_key.key_len,
					  NULL,
					  abyKey,
					  KEY_CTL_WEP,
					  pDevice->PortOffset,
					  pDevice->byLocalID);

		}
		pDevice->eEncryptionStatus = Ndis802_11Encryption1Enabled;
		pDevice->bEncryptionEnable = true;
		return ret;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	//spin_unlock_irq(&pDevice->lock);
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (param->u.wpa_key.seq && fcpfkernel) {
		memcpy(&abySeq[0], param->u.wpa_key.seq, param->u.wpa_key.seq_len);
	} else {
		spin_unlock_irq(&pDevice->lock);
		if (param->u.wpa_key.seq &&
<<<<<<< HEAD
<<<<<<< HEAD
		    copy_from_user(&abySeq[0], param->u.wpa_key.seq, param->u.wpa_key.seq_len)) {
=======
		    copy_from_user(&abySeq[0],
				   (void __user *)param->u.wpa_key.seq,
				   param->u.wpa_key.seq_len)) {
>>>>>>> v3.18
=======
		    copy_from_user(&abySeq[0],
				   (void __user *)param->u.wpa_key.seq,
				   param->u.wpa_key.seq_len)) {
>>>>>>> v3.18
			spin_lock_irq(&pDevice->lock);
			return -EINVAL;
		}
		spin_lock_irq(&pDevice->lock);
	}

	if (param->u.wpa_key.seq_len > 0) {
		for (ii = 0; ii < param->u.wpa_key.seq_len; ii++) {
			if (ii < 4)
<<<<<<< HEAD
<<<<<<< HEAD
				LODWORD(KeyRSC) |= (abySeq[ii] << (ii * 8));
			else
				HIDWORD(KeyRSC) |= (abySeq[ii] << ((ii-4) * 8));
			//KeyRSC |= (abySeq[ii] << (ii * 8));
=======
				KeyRSC |= (u64)(abySeq[ii] << (ii * 8));
			else
				KeyRSC |= (u64)(abySeq[ii] << ((ii-4) * 8));
>>>>>>> v3.18
=======
				KeyRSC |= (u64)(abySeq[ii] << (ii * 8));
			else
				KeyRSC |= (u64)(abySeq[ii] << ((ii-4) * 8));
>>>>>>> v3.18
		}
		dwKeyIndex |= 1 << 29;
	}

	if (param->u.wpa_key.key_index >= MAX_GROUP_KEY) {
<<<<<<< HEAD
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "return  dwKeyIndex > 3\n");
		return -EINVAL;
	}

	if (param->u.wpa_key.alg_name == WPA_ALG_TKIP) {
		pDevice->eEncryptionStatus = Ndis802_11Encryption2Enabled;
	}

	if (param->u.wpa_key.alg_name == WPA_ALG_CCMP) {
		pDevice->eEncryptionStatus = Ndis802_11Encryption3Enabled;
	}
=======
=======
>>>>>>> v3.18
		pr_debug("return  dwKeyIndex > 3\n");
		return -EINVAL;
	}

	if (param->u.wpa_key.alg_name == WPA_ALG_TKIP)
		pDevice->eEncryptionStatus = Ndis802_11Encryption2Enabled;

	if (param->u.wpa_key.alg_name == WPA_ALG_CCMP)
		pDevice->eEncryptionStatus = Ndis802_11Encryption3Enabled;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (param->u.wpa_key.set_tx)
		dwKeyIndex |= (1 << 31);

	if (pDevice->eEncryptionStatus == Ndis802_11Encryption3Enabled)
		byKeyDecMode = KEY_CTL_CCMP;
	else if (pDevice->eEncryptionStatus == Ndis802_11Encryption2Enabled)
		byKeyDecMode = KEY_CTL_TKIP;
	else
		byKeyDecMode = KEY_CTL_WEP;

<<<<<<< HEAD
<<<<<<< HEAD
	// Fix HCT test that set 256 bits KEY and Ndis802_11Encryption3Enabled
=======
	/* Fix HCT test that set 256 bits KEY and Ndis802_11Encryption3Enabled */
>>>>>>> v3.18
=======
	/* Fix HCT test that set 256 bits KEY and Ndis802_11Encryption3Enabled */
>>>>>>> v3.18
	if (pDevice->eEncryptionStatus == Ndis802_11Encryption3Enabled) {
		if (param->u.wpa_key.key_len == MAX_KEY_LEN)
			byKeyDecMode = KEY_CTL_TKIP;
		else if (param->u.wpa_key.key_len == WLAN_WEP40_KEYLEN)
			byKeyDecMode = KEY_CTL_WEP;
		else if (param->u.wpa_key.key_len == WLAN_WEP104_KEYLEN)
			byKeyDecMode = KEY_CTL_WEP;
	} else if (pDevice->eEncryptionStatus == Ndis802_11Encryption2Enabled) {
		if (param->u.wpa_key.key_len == WLAN_WEP40_KEYLEN)
			byKeyDecMode = KEY_CTL_WEP;
		else if (param->u.wpa_key.key_len == WLAN_WEP104_KEYLEN)
			byKeyDecMode = KEY_CTL_WEP;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	// Check TKIP key length
	if ((byKeyDecMode == KEY_CTL_TKIP) &&
	    (param->u.wpa_key.key_len != MAX_KEY_LEN)) {
		// TKIP Key must be 256 bits
		//DBG_PRN_WLAN03(("return NDIS_STATUS_INVALID_DATA - TKIP Key must be 256 bits\n"));
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "return- TKIP Key must be 256 bits!\n");
		return -EINVAL;
	}
	// Check AES key length
	if ((byKeyDecMode == KEY_CTL_CCMP) &&
	    (param->u.wpa_key.key_len != AES_KEY_LEN)) {
		// AES Key must be 128 bits
		//DBG_PRN_WLAN03(("return NDIS_STATUS_INVALID_DATA - AES Key must be 128 bits\n"));
		return -EINVAL;
	}

	// spin_lock_irq(&pDevice->lock);
	if (is_broadcast_ether_addr(&param->addr[0]) || (param->addr == NULL)) {
		// If is_broadcast_ether_addr, set the key as every key entry's group key.
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Groupe Key Assign.\n");

		if ((KeybSetAllGroupKey(&(pDevice->sKey),
					dwKeyIndex,
					param->u.wpa_key.key_len,
					(PQWORD) &(KeyRSC),
					(unsigned char *)abyKey,
					byKeyDecMode,
					pDevice->PortOffset,
					pDevice->byLocalID) == true) &&
		    (KeybSetDefaultKey(&(pDevice->sKey),
				       dwKeyIndex,
				       param->u.wpa_key.key_len,
				       (PQWORD) &(KeyRSC),
				       (unsigned char *)abyKey,
				       byKeyDecMode,
				       pDevice->PortOffset,
				       pDevice->byLocalID) == true)) {
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "GROUP Key Assign.\n");

		} else {
			//DBG_PRN_WLAN03(("return NDIS_STATUS_INVALID_DATA -KeybSetDefaultKey Fail.0\n"));
			// spin_unlock_irq(&pDevice->lock);
=======
=======
>>>>>>> v3.18
	/* Check TKIP key length */
	if ((byKeyDecMode == KEY_CTL_TKIP) &&
	    (param->u.wpa_key.key_len != MAX_KEY_LEN)) {
		/* TKIP Key must be 256 bits */
		pr_debug("return- TKIP Key must be 256 bits!\n");
		return -EINVAL;
	}
	/* Check AES key length */
	if ((byKeyDecMode == KEY_CTL_CCMP) &&
	    (param->u.wpa_key.key_len != AES_KEY_LEN)) {
		/* AES Key must be 128 bits */
		return -EINVAL;
	}

	/* spin_lock_irq(&pDevice->lock); */
	if (is_broadcast_ether_addr(&param->addr[0]) || (param->addr == NULL)) {
		/* If is_broadcast_ether_addr, set the key as every key entry's group key. */
		pr_debug("Groupe Key Assign\n");

		if (KeybSetAllGroupKey(&(pDevice->sKey),
					dwKeyIndex,
					param->u.wpa_key.key_len,
					(u64 *) &KeyRSC,
					(unsigned char *)abyKey,
					byKeyDecMode,
					pDevice->PortOffset,
					pDevice->byLocalID) &&
		    KeybSetDefaultKey(&(pDevice->sKey),
				       dwKeyIndex,
				       param->u.wpa_key.key_len,
				       (u64 *) &KeyRSC,
				       (unsigned char *)abyKey,
				       byKeyDecMode,
				       pDevice->PortOffset,
				       pDevice->byLocalID)) {
			pr_debug("GROUP Key Assign\n");

		} else {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			return -EINVAL;
		}

	} else {
<<<<<<< HEAD
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Pairwise Key Assign.\n");
		// BSSID not 0xffffffffffff
		// Pairwise Key can't be WEP
		if (byKeyDecMode == KEY_CTL_WEP) {
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Pairwise Key can't be WEP\n");
			//spin_unlock_irq(&pDevice->lock);
			return -EINVAL;
		}

		dwKeyIndex |= (1 << 30); // set pairwise key
		if (pMgmt->eConfigMode == WMAC_CONFIG_IBSS_STA) {
			//DBG_PRN_WLAN03(("return NDIS_STATUS_INVALID_DATA - WMAC_CONFIG_IBSS_STA\n"));
			//spin_unlock_irq(&pDevice->lock);
			return -EINVAL;
		}
=======
=======
>>>>>>> v3.18
		pr_debug("Pairwise Key Assign\n");
		/* BSSID not 0xffffffffffff */
		/* Pairwise Key can't be WEP */
		if (byKeyDecMode == KEY_CTL_WEP) {
			pr_debug("Pairwise Key can't be WEP\n");
			return -EINVAL;
		}

		dwKeyIndex |= (1 << 30); /* set pairwise key */
		if (pMgmt->eConfigMode == WMAC_CONFIG_IBSS_STA)
			return -EINVAL;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		if (KeybSetKey(&(pDevice->sKey),
			       &param->addr[0],
			       dwKeyIndex,
			       param->u.wpa_key.key_len,
<<<<<<< HEAD
<<<<<<< HEAD
			       (PQWORD) &(KeyRSC),
			       (unsigned char *)abyKey,
			       byKeyDecMode,
			       pDevice->PortOffset,
			       pDevice->byLocalID) == true) {
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Pairwise Key Set\n");

		} else {
			// Key Table Full
			if (!compare_ether_addr(&param->addr[0], pDevice->abyBSSID)) {
				//DBG_PRN_WLAN03(("return NDIS_STATUS_INVALID_DATA -Key Table Full.2\n"));
				//spin_unlock_irq(&pDevice->lock);
				return -EINVAL;

			} else {
				// Save Key and configure just before associate/reassociate to BSSID
				// we do not implement now
				//spin_unlock_irq(&pDevice->lock);
				return -EINVAL;
			}
		}
	} // BSSID not 0xffffffffffff
=======
=======
>>>>>>> v3.18
			       (u64 *) &KeyRSC,
			       (unsigned char *)abyKey,
			       byKeyDecMode,
			       pDevice->PortOffset,
			       pDevice->byLocalID)) {
			pr_debug("Pairwise Key Set\n");

		} else {
			/* Key Table Full */
			return -EINVAL;
		}
	} /* BSSID not 0xffffffffffff */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if ((ret == 0) && ((param->u.wpa_key.set_tx) != 0)) {
		pDevice->byKeyIndex = (unsigned char)param->u.wpa_key.key_index;
		pDevice->bTransmitKey = true;
	}
	pDevice->bEncryptionEnable = true;
<<<<<<< HEAD
<<<<<<< HEAD
	//spin_unlock_irq(&pDevice->lock);

/*
  DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO " key=%x-%x-%x-%x-%x-xxxxx \n",
  pMgmt->sNodeDBTable[iNodeIndex].abyWepKey[byKeyIndex][0],
  pMgmt->sNodeDBTable[iNodeIndex].abyWepKey[byKeyIndex][1],
  pMgmt->sNodeDBTable[iNodeIndex].abyWepKey[byKeyIndex][2],
  pMgmt->sNodeDBTable[iNodeIndex].abyWepKey[byKeyIndex][3],
  pMgmt->sNodeDBTable[iNodeIndex].abyWepKey[byKeyIndex][4]
);
*/
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return ret;
}

/*
 * Description:
 *      enable wpa auth & mode
 *
 * Parameters:
 *  In:
 *      pDevice   -
 *      param     -
 *  Out:
 *
 * Return Value:
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
static int wpa_set_wpa(PSDevice pDevice,
		       struct viawget_wpa_param *param)
{
	PSMgmtObject    pMgmt = pDevice->pMgmt;
	int ret = 0;
=======
=======
>>>>>>> v3.18
static int wpa_set_wpa(struct vnt_private *pDevice,
		       struct viawget_wpa_param *param)
{
	PSMgmtObject    pMgmt = pDevice->pMgmt;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	pMgmt->eAuthenMode = WMAC_AUTH_OPEN;
	pMgmt->bShareKeyAlgorithm = false;

<<<<<<< HEAD
<<<<<<< HEAD
	return ret;
=======
	return 0;
>>>>>>> v3.18
=======
	return 0;
>>>>>>> v3.18
}

/*
 * Description:
 *      set disassociate
 *
 * Parameters:
 *  In:
 *      pDevice   -
 *      param     -
 *  Out:
 *
 * Return Value:
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
static int wpa_set_disassociate(PSDevice pDevice,
				struct viawget_wpa_param *param)
{
	PSMgmtObject    pMgmt = pDevice->pMgmt;
	int ret = 0;
=======
=======
>>>>>>> v3.18
static int wpa_set_disassociate(struct vnt_private *pDevice,
				struct viawget_wpa_param *param)
{
	PSMgmtObject    pMgmt = pDevice->pMgmt;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	spin_lock_irq(&pDevice->lock);
	if (pDevice->bLinkPass) {
		if (!memcmp(param->addr, pMgmt->abyCurrBSSID, 6))
			bScheduleCommand((void *)pDevice, WLAN_CMD_DISASSOCIATE, NULL);
	}
	spin_unlock_irq(&pDevice->lock);

<<<<<<< HEAD
<<<<<<< HEAD
	return ret;
=======
	return 0;
>>>>>>> v3.18
=======
	return 0;
>>>>>>> v3.18
}

/*
 * Description:
 *      enable scan process
 *
 * Parameters:
 *  In:
 *      pDevice   -
 *      param     -
 *  Out:
 *
 * Return Value:
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
static int wpa_set_scan(PSDevice pDevice,
			struct viawget_wpa_param *param)
{
	int ret = 0;

=======
static int wpa_set_scan(struct vnt_private *pDevice,
			struct viawget_wpa_param *param)
{
>>>>>>> v3.18
=======
static int wpa_set_scan(struct vnt_private *pDevice,
			struct viawget_wpa_param *param)
{
>>>>>>> v3.18
	spin_lock_irq(&pDevice->lock);
	BSSvClearBSSList((void *)pDevice, pDevice->bLinkPass);
	bScheduleCommand((void *)pDevice, WLAN_CMD_BSSID_SCAN, NULL);
	spin_unlock_irq(&pDevice->lock);

<<<<<<< HEAD
<<<<<<< HEAD
	return ret;
=======
	return 0;
>>>>>>> v3.18
=======
	return 0;
>>>>>>> v3.18
}

/*
 * Description:
 *      get bssid
 *
 * Parameters:
 *  In:
 *      pDevice   -
 *      param     -
 *  Out:
 *
 * Return Value:
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
static int wpa_get_bssid(PSDevice pDevice,
			 struct viawget_wpa_param *param)
{
	PSMgmtObject        pMgmt = pDevice->pMgmt;
	int ret = 0;

	memcpy(param->u.wpa_associate.bssid, pMgmt->abyCurrBSSID , 6);

	return ret;
=======
=======
>>>>>>> v3.18
static int wpa_get_bssid(struct vnt_private *pDevice,
			 struct viawget_wpa_param *param)
{
	PSMgmtObject        pMgmt = pDevice->pMgmt;

	memcpy(param->u.wpa_associate.bssid, pMgmt->abyCurrBSSID , 6);

	return 0;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/*
 * Description:
 *      get bssid
 *
 * Parameters:
 *  In:
 *      pDevice   -
 *      param     -
 *  Out:
 *
 * Return Value:
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
static int wpa_get_ssid(PSDevice pDevice,
=======
static int wpa_get_ssid(struct vnt_private *pDevice,
>>>>>>> v3.18
=======
static int wpa_get_ssid(struct vnt_private *pDevice,
>>>>>>> v3.18
			struct viawget_wpa_param *param)
{
	PSMgmtObject        pMgmt = pDevice->pMgmt;
	PWLAN_IE_SSID       pItemSSID;
<<<<<<< HEAD
<<<<<<< HEAD
	int ret = 0;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18

	pItemSSID = (PWLAN_IE_SSID)pMgmt->abyCurrSSID;

	memcpy(param->u.wpa_associate.ssid, pItemSSID->abySSID , pItemSSID->len);
	param->u.wpa_associate.ssid_len = pItemSSID->len;

<<<<<<< HEAD
<<<<<<< HEAD
	return ret;
=======
	return 0;
>>>>>>> v3.18
=======
	return 0;
>>>>>>> v3.18
}

/*
 * Description:
 *      get scan results
 *
 * Parameters:
 *  In:
 *      pDevice   -
 *      param     -
 *  Out:
 *
 * Return Value:
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
static int wpa_get_scan(PSDevice pDevice,
=======
static int wpa_get_scan(struct vnt_private *pDevice,
>>>>>>> v3.18
=======
static int wpa_get_scan(struct vnt_private *pDevice,
>>>>>>> v3.18
			struct viawget_wpa_param *param)
{
	struct viawget_scan_result *scan_buf;
	PSMgmtObject    pMgmt = pDevice->pMgmt;
	PWLAN_IE_SSID   pItemSSID;
	PKnownBSS pBSS;
	unsigned char *pBuf;
	int ret = 0;
	u16 count = 0;
	u16 ii, jj;
#if 1

	unsigned char *ptempBSS;

<<<<<<< HEAD
<<<<<<< HEAD
	ptempBSS = kmalloc(sizeof(KnownBSS), (int)GFP_ATOMIC);

	if (ptempBSS == NULL) {
		printk("bubble sort kmalloc memory fail@@@\n");
=======
=======
>>>>>>> v3.18
	ptempBSS = kmalloc(sizeof(KnownBSS), GFP_ATOMIC);

	if (ptempBSS == NULL) {
		pr_err("bubble sort kmalloc memory fail@@@\n");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

		ret = -ENOMEM;

		return ret;

	}

	for (ii = 0; ii < MAX_BSS_NUM; ii++) {
		for (jj = 0; jj < MAX_BSS_NUM - ii - 1; jj++) {
			if ((pMgmt->sBSSList[jj].bActive != true) ||

			    ((pMgmt->sBSSList[jj].uRSSI > pMgmt->sBSSList[jj + 1].uRSSI) && (pMgmt->sBSSList[jj + 1].bActive != false))) {
				memcpy(ptempBSS, &pMgmt->sBSSList[jj], sizeof(KnownBSS));

				memcpy(&pMgmt->sBSSList[jj], &pMgmt->sBSSList[jj + 1], sizeof(KnownBSS));

				memcpy(&pMgmt->sBSSList[jj + 1], ptempBSS, sizeof(KnownBSS));

			}

		}

	}

	kfree(ptempBSS);
#endif

//******mike:bubble sort by stronger RSSI*****//

	count = 0;
	pBSS = &(pMgmt->sBSSList[0]);
	for (ii = 0; ii < MAX_BSS_NUM; ii++) {
		pBSS = &(pMgmt->sBSSList[ii]);
		if (!pBSS->bActive)
			continue;
		count++;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	pBuf = kcalloc(count, sizeof(struct viawget_scan_result), (int)GFP_ATOMIC);
=======
	pBuf = kcalloc(count, sizeof(struct viawget_scan_result), GFP_ATOMIC);
>>>>>>> v3.18
=======
	pBuf = kcalloc(count, sizeof(struct viawget_scan_result), GFP_ATOMIC);
>>>>>>> v3.18

	if (pBuf == NULL) {
		ret = -ENOMEM;
		return ret;
	}
	scan_buf = (struct viawget_scan_result *)pBuf;
	pBSS = &(pMgmt->sBSSList[0]);
	for (ii = 0, jj = 0; ii < MAX_BSS_NUM; ii++) {
		pBSS = &(pMgmt->sBSSList[ii]);
		if (pBSS->bActive) {
			if (jj >= count)
				break;
			memcpy(scan_buf->bssid, pBSS->abyBSSID, WLAN_BSSID_LEN);
			pItemSSID = (PWLAN_IE_SSID)pBSS->abySSID;
			memcpy(scan_buf->ssid, pItemSSID->abySSID, pItemSSID->len);
			scan_buf->ssid_len = pItemSSID->len;
			scan_buf->freq = frequency_list[pBSS->uChannel-1];
			scan_buf->caps = pBSS->wCapInfo;
<<<<<<< HEAD
<<<<<<< HEAD
			//scan_buf->caps = pBSS->wCapInfo;
			//scan_buf->qual =
			//scan_buf->noise =
			//scan_buf->level =
			//scan_buf->maxrate =
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
			if (pBSS->wWPALen != 0) {
				scan_buf->wpa_ie_len = pBSS->wWPALen;
				memcpy(scan_buf->wpa_ie, pBSS->byWPAIE, pBSS->wWPALen);
			}
			if (pBSS->wRSNLen != 0) {
				scan_buf->rsn_ie_len = pBSS->wRSNLen;
				memcpy(scan_buf->rsn_ie, pBSS->byRSNIE, pBSS->wRSNLen);
			}
			scan_buf = (struct viawget_scan_result *)((unsigned char *)scan_buf + sizeof(struct viawget_scan_result));
			jj++;
		}
	}

	if (jj < count)
		count = jj;

<<<<<<< HEAD
<<<<<<< HEAD
	if (copy_to_user(param->u.scan_results.buf, pBuf, sizeof(struct viawget_scan_result) * count)) {
		ret = -EFAULT;
	}
	param->u.scan_results.scan_count = count;
	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO " param->u.scan_results.scan_count = %d\n", count)
=======
=======
>>>>>>> v3.18
	if (copy_to_user(param->u.scan_results.buf, pBuf, sizeof(struct viawget_scan_result) * count))
		ret = -EFAULT;

	param->u.scan_results.scan_count = count;
	pr_debug(" param->u.scan_results.scan_count = %d\n", count);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

		kfree(pBuf);
	return ret;
}

/*
 * Description:
 *      set associate with AP
 *
 * Parameters:
 *  In:
 *      pDevice   -
 *      param     -
 *  Out:
 *
 * Return Value:
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
static int wpa_set_associate(PSDevice pDevice,
=======
static int wpa_set_associate(struct vnt_private *pDevice,
>>>>>>> v3.18
=======
static int wpa_set_associate(struct vnt_private *pDevice,
>>>>>>> v3.18
			     struct viawget_wpa_param *param)
{
	PSMgmtObject    pMgmt = pDevice->pMgmt;
	PWLAN_IE_SSID   pItemSSID;
	unsigned char abyNullAddr[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char abyWPAIE[64];
<<<<<<< HEAD
<<<<<<< HEAD
	int ret = 0;
	bool bWepEnabled = false;

	// set key type & algorithm
	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "pairwise_suite = %d\n", param->u.wpa_associate.pairwise_suite);
	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "group_suite = %d\n", param->u.wpa_associate.group_suite);
	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "key_mgmt_suite = %d\n", param->u.wpa_associate.key_mgmt_suite);
	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "auth_alg = %d\n", param->u.wpa_associate.auth_alg);
	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "mode = %d\n", param->u.wpa_associate.mode);
	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "wpa_ie_len = %d\n", param->u.wpa_associate.wpa_ie_len);
=======
=======
>>>>>>> v3.18
	bool bWepEnabled = false;

	/* set key type & algorithm */
	pr_debug("pairwise_suite = %d\n",
		 param->u.wpa_associate.pairwise_suite);
	pr_debug("group_suite = %d\n", param->u.wpa_associate.group_suite);
	pr_debug("key_mgmt_suite = %d\n",
		 param->u.wpa_associate.key_mgmt_suite);
	pr_debug("auth_alg = %d\n", param->u.wpa_associate.auth_alg);
	pr_debug("mode = %d\n", param->u.wpa_associate.mode);
	pr_debug("wpa_ie_len = %d\n", param->u.wpa_associate.wpa_ie_len);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (param->u.wpa_associate.wpa_ie_len) {
		if (!param->u.wpa_associate.wpa_ie)
			return -EINVAL;
		if (param->u.wpa_associate.wpa_ie_len > sizeof(abyWPAIE))
			return -EINVAL;
		if (copy_from_user(&abyWPAIE[0], param->u.wpa_associate.wpa_ie, param->u.wpa_associate.wpa_ie_len))
			return -EFAULT;
	}

	if (param->u.wpa_associate.mode == 1)
		pMgmt->eConfigMode = WMAC_CONFIG_IBSS_STA;
	else
		pMgmt->eConfigMode = WMAC_CONFIG_ESS_STA;
<<<<<<< HEAD
<<<<<<< HEAD
	// set ssid
=======
	/* set ssid */
>>>>>>> v3.18
=======
	/* set ssid */
>>>>>>> v3.18
	memset(pMgmt->abyDesireSSID, 0, WLAN_IEHDR_LEN + WLAN_SSID_MAXLEN + 1);
	pItemSSID = (PWLAN_IE_SSID)pMgmt->abyDesireSSID;
	pItemSSID->byElementID = WLAN_EID_SSID;
	pItemSSID->len = param->u.wpa_associate.ssid_len;
	memcpy(pItemSSID->abySSID, param->u.wpa_associate.ssid, pItemSSID->len);
<<<<<<< HEAD
<<<<<<< HEAD
	// set bssid
	if (memcmp(param->u.wpa_associate.bssid, &abyNullAddr[0], 6) != 0)
		memcpy(pMgmt->abyDesireBSSID, param->u.wpa_associate.bssid, 6);
	else {
		bScheduleCommand((void *)pDevice, WLAN_CMD_BSSID_SCAN, pItemSSID->abySSID);
	}
=======
=======
>>>>>>> v3.18
	/* set bssid */
	if (memcmp(param->u.wpa_associate.bssid, &abyNullAddr[0], 6) != 0)
		memcpy(pMgmt->abyDesireBSSID, param->u.wpa_associate.bssid, 6);
	else
		bScheduleCommand((void *)pDevice, WLAN_CMD_BSSID_SCAN, pItemSSID->abySSID);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	if (param->u.wpa_associate.wpa_ie_len == 0) {
		if (param->u.wpa_associate.auth_alg & AUTH_ALG_SHARED_KEY)
			pMgmt->eAuthenMode = WMAC_AUTH_SHAREKEY;
		else
			pMgmt->eAuthenMode = WMAC_AUTH_OPEN;
	} else if (abyWPAIE[0] == RSN_INFO_ELEM) {
		if (param->u.wpa_associate.key_mgmt_suite == KEY_MGMT_PSK)
			pMgmt->eAuthenMode = WMAC_AUTH_WPA2PSK;
		else
			pMgmt->eAuthenMode = WMAC_AUTH_WPA2;
	} else {
		if (param->u.wpa_associate.key_mgmt_suite == KEY_MGMT_WPA_NONE)
			pMgmt->eAuthenMode = WMAC_AUTH_WPANONE;
		else if (param->u.wpa_associate.key_mgmt_suite == KEY_MGMT_PSK)
			pMgmt->eAuthenMode = WMAC_AUTH_WPAPSK;
		else
			pMgmt->eAuthenMode = WMAC_AUTH_WPA;
	}

	switch (param->u.wpa_associate.pairwise_suite) {
	case CIPHER_CCMP:
		pDevice->eEncryptionStatus = Ndis802_11Encryption3Enabled;
		break;
	case CIPHER_TKIP:
		pDevice->eEncryptionStatus = Ndis802_11Encryption2Enabled;
		break;
	case CIPHER_WEP40:
	case CIPHER_WEP104:
		pDevice->eEncryptionStatus = Ndis802_11Encryption1Enabled;
		bWepEnabled = true;
		break;
	case CIPHER_NONE:
		if (param->u.wpa_associate.group_suite == CIPHER_CCMP)
			pDevice->eEncryptionStatus = Ndis802_11Encryption3Enabled;
		else
			pDevice->eEncryptionStatus = Ndis802_11Encryption2Enabled;
		break;
	default:
		pDevice->eEncryptionStatus = Ndis802_11EncryptionDisabled;
	}

//DavidWang add for WPA_supplicant support open/share mode

	if (pMgmt->eAuthenMode == WMAC_AUTH_SHAREKEY) {
		pDevice->eEncryptionStatus = Ndis802_11Encryption1Enabled;
<<<<<<< HEAD
<<<<<<< HEAD
		//pMgmt->eAuthenMode = WMAC_AUTH_SHAREKEY;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
		pMgmt->bShareKeyAlgorithm = true;
	} else if (pMgmt->eAuthenMode == WMAC_AUTH_OPEN) {
		if (!bWepEnabled)  pDevice->eEncryptionStatus = Ndis802_11EncryptionDisabled;
		else pDevice->eEncryptionStatus = Ndis802_11Encryption1Enabled;
<<<<<<< HEAD
<<<<<<< HEAD
		//pMgmt->eAuthenMode = WMAC_AUTH_OPEN;
		//pMgmt->bShareKeyAlgorithm = false; //20080717-06,<Modify> by chester//Fix Open mode, WEP encryption
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}
//mike save old encryption status
	pDevice->eOldEncryptionStatus = pDevice->eEncryptionStatus;

	if (pDevice->eEncryptionStatus !=  Ndis802_11EncryptionDisabled)
		pDevice->bEncryptionEnable = true;
	else
		pDevice->bEncryptionEnable = false;
	if (!((pMgmt->eAuthenMode == WMAC_AUTH_SHAREKEY) ||
<<<<<<< HEAD
<<<<<<< HEAD
	      ((pMgmt->eAuthenMode == WMAC_AUTH_OPEN) && (bWepEnabled == true))))  //DavidWang  //20080717-06,<Modify> by chester//Not to initial WEP
=======
	      ((pMgmt->eAuthenMode == WMAC_AUTH_OPEN) && bWepEnabled)))  //DavidWang  //20080717-06,<Modify> by chester//Not to initial WEP
>>>>>>> v3.18
=======
	      ((pMgmt->eAuthenMode == WMAC_AUTH_OPEN) && bWepEnabled)))  //DavidWang  //20080717-06,<Modify> by chester//Not to initial WEP
>>>>>>> v3.18
		KeyvInitTable(&pDevice->sKey, pDevice->PortOffset);
	spin_lock_irq(&pDevice->lock);
	pDevice->bLinkPass = false;
	memset(pMgmt->abyCurrBSSID, 0, 6);
	pMgmt->eCurrState = WMAC_STATE_IDLE;
	netif_stop_queue(pDevice->dev);
	//20080701-02,<Add> by Mike Liu
/*******search if ap_scan=2 ,which is associating request in hidden ssid mode ****/
	{
		PKnownBSS       pCurr = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> v3.18
=======

>>>>>>> v3.18
		pCurr = BSSpSearchBSSList(pDevice,
					  pMgmt->abyDesireBSSID,
					  pMgmt->abyDesireSSID,
					  pMgmt->eConfigPHYMode
);

		if (pCurr == NULL) {
<<<<<<< HEAD
<<<<<<< HEAD
			printk("wpa_set_associate---->hidden mode site survey before associate.......\n");
=======
			pr_debug("wpa_set_associate---->hidden mode site survey before associate.......\n");
>>>>>>> v3.18
=======
			pr_debug("wpa_set_associate---->hidden mode site survey before associate.......\n");
>>>>>>> v3.18
			bScheduleCommand((void *)pDevice, WLAN_CMD_BSSID_SCAN, pMgmt->abyDesireSSID);
		}
	}
/****************************************************************/
	bScheduleCommand((void *)pDevice, WLAN_CMD_SSID, NULL);
	spin_unlock_irq(&pDevice->lock);

<<<<<<< HEAD
<<<<<<< HEAD
	return ret;
=======
	return 0;
>>>>>>> v3.18
=======
	return 0;
>>>>>>> v3.18
}

/*
 * Description:
 *      wpa_ioctl main function supported for wpa supplicant
 *
 * Parameters:
 *  In:
 *      pDevice   -
 *      iw_point  -
 *  Out:
 *
 * Return Value:
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
int wpa_ioctl(PSDevice pDevice, struct iw_point *p)
=======
int wpa_ioctl(struct vnt_private *pDevice, struct iw_point *p)
>>>>>>> v3.18
=======
int wpa_ioctl(struct vnt_private *pDevice, struct iw_point *p)
>>>>>>> v3.18
{
	struct viawget_wpa_param *param;
	int ret = 0;
	int wpa_ioctl = 0;

	if (p->length < sizeof(struct viawget_wpa_param) ||
	    p->length > VIAWGET_WPA_MAX_BUF_SIZE || !p->pointer)
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	param = kmalloc((int)p->length, (int)GFP_KERNEL);
=======
	param = kmalloc((int)p->length, GFP_KERNEL);
>>>>>>> v3.18
=======
	param = kmalloc((int)p->length, GFP_KERNEL);
>>>>>>> v3.18
	if (param == NULL)
		return -ENOMEM;

	if (copy_from_user(param, p->pointer, p->length)) {
		ret = -EFAULT;
		goto out;
	}

	switch (param->cmd) {
	case VIAWGET_SET_WPA:
		ret = wpa_set_wpa(pDevice, param);
<<<<<<< HEAD
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "VIAWGET_SET_WPA \n");
		break;

	case VIAWGET_SET_KEY:
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "VIAWGET_SET_KEY \n");
=======
=======
>>>>>>> v3.18
		pr_debug("VIAWGET_SET_WPA\n");
		break;

	case VIAWGET_SET_KEY:
		pr_debug("VIAWGET_SET_KEY\n");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		spin_lock_irq(&pDevice->lock);
		ret = wpa_set_keys(pDevice, param, false);
		spin_unlock_irq(&pDevice->lock);
		break;

	case VIAWGET_SET_SCAN:
<<<<<<< HEAD
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "VIAWGET_SET_SCAN \n");
=======
		pr_debug("VIAWGET_SET_SCAN\n");
>>>>>>> v3.18
=======
		pr_debug("VIAWGET_SET_SCAN\n");
>>>>>>> v3.18
		ret = wpa_set_scan(pDevice, param);
		break;

	case VIAWGET_GET_SCAN:
<<<<<<< HEAD
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "VIAWGET_GET_SCAN\n");
=======
		pr_debug("VIAWGET_GET_SCAN\n");
>>>>>>> v3.18
=======
		pr_debug("VIAWGET_GET_SCAN\n");
>>>>>>> v3.18
		ret = wpa_get_scan(pDevice, param);
		wpa_ioctl = 1;
		break;

	case VIAWGET_GET_SSID:
<<<<<<< HEAD
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "VIAWGET_GET_SSID \n");
=======
		pr_debug("VIAWGET_GET_SSID\n");
>>>>>>> v3.18
=======
		pr_debug("VIAWGET_GET_SSID\n");
>>>>>>> v3.18
		ret = wpa_get_ssid(pDevice, param);
		wpa_ioctl = 1;
		break;

	case VIAWGET_GET_BSSID:
<<<<<<< HEAD
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "VIAWGET_GET_BSSID \n");
=======
		pr_debug("VIAWGET_GET_BSSID\n");
>>>>>>> v3.18
=======
		pr_debug("VIAWGET_GET_BSSID\n");
>>>>>>> v3.18
		ret = wpa_get_bssid(pDevice, param);
		wpa_ioctl = 1;
		break;

	case VIAWGET_SET_ASSOCIATE:
<<<<<<< HEAD
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "VIAWGET_SET_ASSOCIATE \n");
=======
		pr_debug("VIAWGET_SET_ASSOCIATE\n");
>>>>>>> v3.18
=======
		pr_debug("VIAWGET_SET_ASSOCIATE\n");
>>>>>>> v3.18
		ret = wpa_set_associate(pDevice, param);
		break;

	case VIAWGET_SET_DISASSOCIATE:
<<<<<<< HEAD
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "VIAWGET_SET_DISASSOCIATE \n");
=======
		pr_debug("VIAWGET_SET_DISASSOCIATE\n");
>>>>>>> v3.18
=======
		pr_debug("VIAWGET_SET_DISASSOCIATE\n");
>>>>>>> v3.18
		ret = wpa_set_disassociate(pDevice, param);
		break;

	case VIAWGET_SET_DROP_UNENCRYPT:
<<<<<<< HEAD
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "VIAWGET_SET_DROP_UNENCRYPT \n");
		break;

	case VIAWGET_SET_DEAUTHENTICATE:
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "VIAWGET_SET_DEAUTHENTICATE \n");
		break;

	default:
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "wpa_ioctl: unknown cmd=%d\n",
			param->cmd);
		return -EOPNOTSUPP;
		break;
=======
=======
>>>>>>> v3.18
		pr_debug("VIAWGET_SET_DROP_UNENCRYPT\n");
		break;

	case VIAWGET_SET_DEAUTHENTICATE:
		pr_debug("VIAWGET_SET_DEAUTHENTICATE\n");
		break;

	default:
		pr_debug("wpa_ioctl: unknown cmd=%d\n",
			 param->cmd);
		ret = -EOPNOTSUPP;
		goto out;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}

	if ((ret == 0) && wpa_ioctl) {
		if (copy_to_user(p->pointer, param, p->length)) {
			ret = -EFAULT;
			goto out;
		}
	}

out:
	kfree(param);

	return ret;
}
