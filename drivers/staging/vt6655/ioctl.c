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
 * File: ioctl.c
 *
 * Purpose:  private ioctl functions
 *
 * Author: Lyndon Chen
 *
 * Date: Auguest 20, 2003
 *
 * Functions:
 *
 * Revision History:
 *
 */

#include "ioctl.h"
#include "iocmd.h"
#include "mac.h"
#include "card.h"
#include "hostap.h"
#include "wpactl.h"
#include "rf.h"

<<<<<<< HEAD
static int msglevel = MSG_LEVEL_INFO;

=======
>>>>>>> v3.18
#ifdef WPA_SM_Transtatus
SWPAResult wpa_Result;
#endif

<<<<<<< HEAD
int private_ioctl(PSDevice pDevice, struct ifreq *rq)
=======
int private_ioctl(struct vnt_private *pDevice, struct ifreq *rq)
>>>>>>> v3.18
{
	PSCmdRequest	pReq = (PSCmdRequest)rq;
	PSMgmtObject	pMgmt = pDevice->pMgmt;
	int		result = 0;
	PWLAN_IE_SSID	pItemSSID;
	SCmdBSSJoin	sJoinCmd;
	SCmdZoneTypeSet	sZoneTypeCmd;
	SCmdScan	sScanCmd;
	SCmdStartAP	sStartAPCmd;
	SCmdSetWEP	sWEPCmd;
	SCmdValue	sValue;
	SBSSIDList	sList;
	SNodeList	sNodeList;
	PSBSSIDList	pList;
	PSNodeList	pNodeList;
	unsigned int	cbListCount;
	PKnownBSS	pBSS;
	PKnownNodeDB	pNode;
	unsigned int	ii, jj;
<<<<<<< HEAD
	SCmdLinkStatus	sLinkStatus;
=======
>>>>>>> v3.18
	unsigned char	abySuppRates[] = {WLAN_EID_SUPP_RATES, 4, 0x02, 0x04, 0x0B, 0x16};
	unsigned char	abyNullAddr[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned long	dwKeyIndex = 0;
	unsigned char	abyScanSSID[WLAN_IEHDR_LEN + WLAN_SSID_MAXLEN + 1];
	long		ldBm;

	pReq->wResult = 0;

	switch (pReq->wCmdCode) {
	case WLAN_CMD_BSS_SCAN:
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "WLAN_CMD_BSS_SCAN..begin\n");
=======
		pr_debug("WLAN_CMD_BSS_SCAN..begin\n");
>>>>>>> v3.18
		if (copy_from_user(&sScanCmd, pReq->data, sizeof(SCmdScan))) {
			result = -EFAULT;
			break;
		}

		pItemSSID = (PWLAN_IE_SSID)sScanCmd.ssid;
		if (pItemSSID->len > WLAN_SSID_MAXLEN + 1)
			return -EINVAL;
		if (pItemSSID->len != 0) {
			memset(abyScanSSID, 0, WLAN_IEHDR_LEN + WLAN_SSID_MAXLEN + 1);
			memcpy(abyScanSSID, pItemSSID, pItemSSID->len + WLAN_IEHDR_LEN);
		}

		if (pDevice->bMACSuspend == true) {
			if (pDevice->bRadioOff == true)
				CARDbRadioPowerOn(pDevice);
			vMgrTimerInit(pDevice);
			MACvIntEnable(pDevice->PortOffset, IMR_MASK_VALUE);
			add_timer(&pMgmt->sTimerSecondCallback);
			pDevice->bMACSuspend = false;
		}
		spin_lock_irq(&pDevice->lock);
		if (memcmp(pMgmt->abyCurrBSSID, &abyNullAddr[0], 6) == 0)
			BSSvClearBSSList((void *)pDevice, false);
		else
			BSSvClearBSSList((void *)pDevice, pDevice->bLinkPass);

		if (pItemSSID->len != 0)
			bScheduleCommand((void *)pDevice, WLAN_CMD_BSSID_SCAN, abyScanSSID);
		else
			bScheduleCommand((void *)pDevice, WLAN_CMD_BSSID_SCAN, NULL);
		spin_unlock_irq(&pDevice->lock);
		break;

	case WLAN_CMD_ZONETYPE_SET:
		/* mike add :can't support. */
		result = -EOPNOTSUPP;
		break;

		if (copy_from_user(&sZoneTypeCmd, pReq->data, sizeof(SCmdZoneTypeSet))) {
			result = -EFAULT;
			break;
		}

		if (sZoneTypeCmd.bWrite == true) {
			/* write zonetype */
			if (sZoneTypeCmd.ZoneType == ZoneType_USA) {
				/* set to USA */
<<<<<<< HEAD
				printk("set_ZoneType:USA\n");
			} else if (sZoneTypeCmd.ZoneType == ZoneType_Japan) {
				/* set to Japan */
				printk("set_ZoneType:Japan\n");
			} else if (sZoneTypeCmd.ZoneType == ZoneType_Europe) {
				/* set to Europe */
				printk("set_ZoneType:Europe\n");
=======
				pr_debug("set_ZoneType:USA\n");
			} else if (sZoneTypeCmd.ZoneType == ZoneType_Japan) {
				/* set to Japan */
				pr_debug("set_ZoneType:Japan\n");
			} else if (sZoneTypeCmd.ZoneType == ZoneType_Europe) {
				/* set to Europe */
				pr_debug("set_ZoneType:Europe\n");
>>>>>>> v3.18
			}
		} else {
			/* read zonetype */
			unsigned char zonetype = 0;

			if (zonetype == 0x00) {		/* USA */
				sZoneTypeCmd.ZoneType = ZoneType_USA;
			} else if (zonetype == 0x01) {	/* Japan */
				sZoneTypeCmd.ZoneType = ZoneType_Japan;
			} else if (zonetype == 0x02) {	/* Europe */
				sZoneTypeCmd.ZoneType = ZoneType_Europe;
			} else {			/* Unknown ZoneType */
<<<<<<< HEAD
				printk("Error:ZoneType[%x] Unknown ???\n", zonetype);
=======
				pr_err("Error:ZoneType[%x] Unknown ???\n", zonetype);
>>>>>>> v3.18
				result = -EFAULT;
				break;
			}
			if (copy_to_user(pReq->data, &sZoneTypeCmd, sizeof(SCmdZoneTypeSet))) {
				result = -EFAULT;
				break;
			}
		}
		break;

	case WLAN_CMD_BSS_JOIN:
		if (pDevice->bMACSuspend == true) {
			if (pDevice->bRadioOff == true)
				CARDbRadioPowerOn(pDevice);
			vMgrTimerInit(pDevice);
			MACvIntEnable(pDevice->PortOffset, IMR_MASK_VALUE);
			add_timer(&pMgmt->sTimerSecondCallback);
			pDevice->bMACSuspend = false;
		}

		if (copy_from_user(&sJoinCmd, pReq->data, sizeof(SCmdBSSJoin))) {
			result = -EFAULT;
			break;
		}

		pItemSSID = (PWLAN_IE_SSID)sJoinCmd.ssid;
		if (pItemSSID->len > WLAN_SSID_MAXLEN + 1)
			return -EINVAL;
		memset(pMgmt->abyDesireSSID, 0, WLAN_IEHDR_LEN + WLAN_SSID_MAXLEN + 1);
		memcpy(pMgmt->abyDesireSSID, pItemSSID, pItemSSID->len + WLAN_IEHDR_LEN);
		if (sJoinCmd.wBSSType == ADHOC) {
			pMgmt->eConfigMode = WMAC_CONFIG_IBSS_STA;
<<<<<<< HEAD
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "ioct set to adhoc mode\n");
		} else {
			pMgmt->eConfigMode = WMAC_CONFIG_ESS_STA;
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "ioct set to STA mode\n");
=======
			pr_debug("ioct set to adhoc mode\n");
		} else {
			pMgmt->eConfigMode = WMAC_CONFIG_ESS_STA;
			pr_debug("ioct set to STA mode\n");
>>>>>>> v3.18
		}
		if (sJoinCmd.bPSEnable == true) {
			pDevice->ePSMode = WMAC_POWER_FAST;
			pMgmt->wListenInterval = 2;
<<<<<<< HEAD
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Power Saving On\n");
		} else {
			pDevice->ePSMode = WMAC_POWER_CAM;
			pMgmt->wListenInterval = 1;
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Power Saving Off\n");
=======
			pr_debug("Power Saving On\n");
		} else {
			pDevice->ePSMode = WMAC_POWER_CAM;
			pMgmt->wListenInterval = 1;
			pr_debug("Power Saving Off\n");
>>>>>>> v3.18
		}

		if (sJoinCmd.bShareKeyAuth == true) {
			pMgmt->bShareKeyAlgorithm = true;
<<<<<<< HEAD
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Share Key\n");
		} else {
			pMgmt->bShareKeyAlgorithm = false;
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Open System\n");
=======
			pr_debug("Share Key\n");
		} else {
			pMgmt->bShareKeyAlgorithm = false;
			pr_debug("Open System\n");
>>>>>>> v3.18
		}
		pDevice->uChannel = sJoinCmd.uChannel;
		netif_stop_queue(pDevice->dev);
		spin_lock_irq(&pDevice->lock);
		pMgmt->eCurrState = WMAC_STATE_IDLE;
		bScheduleCommand((void *)pDevice, WLAN_CMD_BSSID_SCAN, pMgmt->abyDesireSSID);
		bScheduleCommand((void *)pDevice, WLAN_CMD_SSID, NULL);
		spin_unlock_irq(&pDevice->lock);
		break;

	case WLAN_CMD_SET_WEP:
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "WLAN_CMD_SET_WEP Key.\n");
=======
		pr_debug("WLAN_CMD_SET_WEP Key\n");
>>>>>>> v3.18
		memset(&sWEPCmd, 0, sizeof(SCmdSetWEP));
		if (copy_from_user(&sWEPCmd, pReq->data, sizeof(SCmdSetWEP))) {
			result = -EFAULT;
			break;
		}
		if (sWEPCmd.bEnableWep != true) {
			pDevice->bEncryptionEnable = false;
			pDevice->eEncryptionStatus = Ndis802_11EncryptionDisabled;
			MACvDisableDefaultKey(pDevice->PortOffset);
<<<<<<< HEAD
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "WEP function disable.\n");
=======
			pr_debug("WEP function disable\n");
>>>>>>> v3.18
			break;
		}

		for (ii = 0; ii < WLAN_WEP_NKEYS; ii++) {
			if (sWEPCmd.bWepKeyAvailable[ii]) {
				if (ii == sWEPCmd.byKeyIndex)
					dwKeyIndex = ii | (1 << 31);
				else
					dwKeyIndex = ii;

				KeybSetDefaultKey(&(pDevice->sKey),
						  dwKeyIndex,
						  sWEPCmd.auWepKeyLength[ii],
						  NULL,
						  (unsigned char *)&sWEPCmd.abyWepKey[ii][0],
						  KEY_CTL_WEP,
						  pDevice->PortOffset,
						  pDevice->byLocalID);
			}
		}
		pDevice->byKeyIndex = sWEPCmd.byKeyIndex;
		pDevice->bTransmitKey = true;
		pDevice->bEncryptionEnable = true;
		pDevice->eEncryptionStatus = Ndis802_11Encryption1Enabled;
		break;

<<<<<<< HEAD
	case WLAN_CMD_GET_LINK:
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "WLAN_CMD_GET_LINK status.\n");

		memset(sLinkStatus.abySSID, 0 , WLAN_SSID_MAXLEN + 1);
=======
	case WLAN_CMD_GET_LINK: {
		SCmdLinkStatus sLinkStatus;

		pr_debug("WLAN_CMD_GET_LINK status\n");

		memset(&sLinkStatus, 0, sizeof(sLinkStatus));
>>>>>>> v3.18

		if (pMgmt->eCurrMode == WMAC_MODE_IBSS_STA)
			sLinkStatus.wBSSType = ADHOC;
		else
			sLinkStatus.wBSSType = INFRA;

		if (pMgmt->eCurrState == WMAC_STATE_JOINTED)
			sLinkStatus.byState = ADHOC_JOINTED;
		else
			sLinkStatus.byState = ADHOC_STARTED;

		sLinkStatus.uChannel = pMgmt->uCurrChannel;
		if (pDevice->bLinkPass == true) {
			sLinkStatus.bLink = true;
			pItemSSID = (PWLAN_IE_SSID)pMgmt->abyCurrSSID;
			memcpy(sLinkStatus.abySSID, pItemSSID->abySSID, pItemSSID->len);
			memcpy(sLinkStatus.abyBSSID, pMgmt->abyCurrBSSID, WLAN_BSSID_LEN);
			sLinkStatus.uLinkRate = pMgmt->sNodeDBTable[0].wTxDataRate;
<<<<<<< HEAD
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO " Link Success!\n");
=======
			pr_debug(" Link Success!\n");
>>>>>>> v3.18
		} else {
			sLinkStatus.bLink = false;
			sLinkStatus.uLinkRate = 0;
		}
		if (copy_to_user(pReq->data, &sLinkStatus, sizeof(SCmdLinkStatus))) {
			result = -EFAULT;
			break;
		}
		break;
<<<<<<< HEAD

=======
	}
>>>>>>> v3.18
	case WLAN_CMD_GET_LISTLEN:
		cbListCount = 0;
		pBSS = &(pMgmt->sBSSList[0]);
		for (ii = 0; ii < MAX_BSS_NUM; ii++) {
			pBSS = &(pMgmt->sBSSList[ii]);
			if (!pBSS->bActive)
				continue;
			cbListCount++;
		}
		sList.uItem = cbListCount;
		if (copy_to_user(pReq->data, &sList, sizeof(SBSSIDList))) {
			result = -EFAULT;
			break;
		}
		pReq->wResult = 0;
		break;

	case WLAN_CMD_GET_LIST:
		if (copy_from_user(&sList, pReq->data, sizeof(SBSSIDList))) {
			result = -EFAULT;
			break;
		}
		if (sList.uItem > (ULONG_MAX - sizeof(SBSSIDList)) / sizeof(SBSSIDItem)) {
			result = -EINVAL;
			break;
		}
<<<<<<< HEAD
		pList = (PSBSSIDList)kmalloc(sizeof(SBSSIDList) + (sList.uItem * sizeof(SBSSIDItem)), (int)GFP_ATOMIC);
=======
		pList = (PSBSSIDList)kmalloc(sizeof(SBSSIDList) + (sList.uItem * sizeof(SBSSIDItem)),
					     GFP_ATOMIC);
>>>>>>> v3.18
		if (pList == NULL) {
			result = -ENOMEM;
			break;
		}
		pList->uItem = sList.uItem;
		pBSS = &(pMgmt->sBSSList[0]);
		for (ii = 0, jj = 0; jj < MAX_BSS_NUM; jj++) {
			pBSS = &(pMgmt->sBSSList[jj]);
			if (pBSS->bActive) {
				pList->sBSSIDList[ii].uChannel = pBSS->uChannel;
				pList->sBSSIDList[ii].wBeaconInterval = pBSS->wBeaconInterval;
				pList->sBSSIDList[ii].wCapInfo = pBSS->wCapInfo;
<<<<<<< HEAD
				/* pList->sBSSIDList[ii].uRSSI = pBSS->uRSSI; */
=======
>>>>>>> v3.18
				RFvRSSITodBm(pDevice, (unsigned char)(pBSS->uRSSI), &ldBm);
				pList->sBSSIDList[ii].uRSSI = (unsigned int)ldBm;
				memcpy(pList->sBSSIDList[ii].abyBSSID, pBSS->abyBSSID, WLAN_BSSID_LEN);
				pItemSSID = (PWLAN_IE_SSID)pBSS->abySSID;
				memset(pList->sBSSIDList[ii].abySSID, 0, WLAN_SSID_MAXLEN + 1);
				memcpy(pList->sBSSIDList[ii].abySSID, pItemSSID->abySSID, pItemSSID->len);
				if (WLAN_GET_CAP_INFO_ESS(pBSS->wCapInfo))
					pList->sBSSIDList[ii].byNetType = INFRA;
				else
					pList->sBSSIDList[ii].byNetType = ADHOC;

				if (WLAN_GET_CAP_INFO_PRIVACY(pBSS->wCapInfo))
					pList->sBSSIDList[ii].bWEPOn = true;
				else
					pList->sBSSIDList[ii].bWEPOn = false;

				ii++;
				if (ii >= pList->uItem)
					break;
			}
		}

		if (copy_to_user(pReq->data, pList, sizeof(SBSSIDList) + (sList.uItem * sizeof(SBSSIDItem)))) {
			result = -EFAULT;
			break;
		}
		kfree(pList);
		pReq->wResult = 0;
		break;

	case WLAN_CMD_GET_MIB:
		if (copy_to_user(pReq->data, &(pDevice->s802_11Counter), sizeof(SDot11MIBCount))) {
			result = -EFAULT;
			break;
		}
		break;

	case WLAN_CMD_GET_STAT:
		if (copy_to_user(pReq->data, &(pDevice->scStatistic), sizeof(SStatCounter))) {
			result = -EFAULT;
			break;
		}
		break;

	case WLAN_CMD_STOP_MAC:
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "WLAN_CMD_STOP_MAC\n");
=======
		pr_debug("WLAN_CMD_STOP_MAC\n");
>>>>>>> v3.18
		netif_stop_queue(pDevice->dev);

		spin_lock_irq(&pDevice->lock);
		if (pDevice->bRadioOff == false)
			CARDbRadioPowerOff(pDevice);

		pDevice->bLinkPass = false;
		memset(pMgmt->abyCurrBSSID, 0, 6);
		pMgmt->eCurrState = WMAC_STATE_IDLE;
		del_timer(&pDevice->sTimerCommand);
		del_timer(&pMgmt->sTimerSecondCallback);
		pDevice->bCmdRunning = false;
		pDevice->bMACSuspend = true;
		MACvIntDisable(pDevice->PortOffset);
		spin_unlock_irq(&pDevice->lock);
		break;

	case WLAN_CMD_START_MAC:
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "WLAN_CMD_START_MAC\n");
=======
		pr_debug("WLAN_CMD_START_MAC\n");
>>>>>>> v3.18

		if (pDevice->bMACSuspend == true) {
			if (pDevice->bRadioOff == true)
				CARDbRadioPowerOn(pDevice);
			vMgrTimerInit(pDevice);
			MACvIntEnable(pDevice->PortOffset, IMR_MASK_VALUE);
			add_timer(&pMgmt->sTimerSecondCallback);
			pDevice->bMACSuspend = false;
		}
		break;

	case WLAN_CMD_SET_HOSTAPD:
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "WLAN_CMD_SET_HOSTAPD\n");
=======
		pr_debug("WLAN_CMD_SET_HOSTAPD\n");
>>>>>>> v3.18

		if (copy_from_user(&sValue, pReq->data, sizeof(SCmdValue))) {
			result = -EFAULT;
			break;
		}
		if (sValue.dwValue == 1) {
			if (vt6655_hostap_set_hostapd(pDevice, 1, 1) == 0) {
<<<<<<< HEAD
				DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Enable HOSTAP\n");
=======
				pr_debug("Enable HOSTAP\n");
>>>>>>> v3.18
			} else {
				result = -EFAULT;
				break;
			}
		} else {
			vt6655_hostap_set_hostapd(pDevice, 0, 1);
<<<<<<< HEAD
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Disable HOSTAP\n");
=======
			pr_debug("Disable HOSTAP\n");
>>>>>>> v3.18
		}
		break;

	case WLAN_CMD_SET_HOSTAPD_STA:
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "WLAN_CMD_SET_HOSTAPD_STA\n");
		break;

	case WLAN_CMD_SET_802_1X:
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "WLAN_CMD_SET_802_1X\n");
=======
		pr_debug("WLAN_CMD_SET_HOSTAPD_STA\n");
		break;

	case WLAN_CMD_SET_802_1X:
		pr_debug("WLAN_CMD_SET_802_1X\n");
>>>>>>> v3.18
		if (copy_from_user(&sValue, pReq->data, sizeof(SCmdValue))) {
			result = -EFAULT;
			break;
		}

		if (sValue.dwValue == 1) {
			pDevice->bEnable8021x = true;
<<<<<<< HEAD
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Enable 802.1x\n");
		} else {
			pDevice->bEnable8021x = false;
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Disable 802.1x\n");
=======
			pr_debug("Enable 802.1x\n");
		} else {
			pDevice->bEnable8021x = false;
			pr_debug("Disable 802.1x\n");
>>>>>>> v3.18
		}
		break;

	case WLAN_CMD_SET_HOST_WEP:
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "WLAN_CMD_SET_HOST_WEP\n");
=======
		pr_debug("WLAN_CMD_SET_HOST_WEP\n");
>>>>>>> v3.18
		if (copy_from_user(&sValue, pReq->data, sizeof(SCmdValue))) {
			result = -EFAULT;
			break;
		}

		if (sValue.dwValue == 1) {
			pDevice->bEnableHostWEP = true;
<<<<<<< HEAD
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Enable HostWEP\n");
		} else {
			pDevice->bEnableHostWEP = false;
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Disable HostWEP\n");
=======
			pr_debug("Enable HostWEP\n");
		} else {
			pDevice->bEnableHostWEP = false;
			pr_debug("Disable HostWEP\n");
>>>>>>> v3.18
		}
		break;

	case WLAN_CMD_SET_WPA:
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "WLAN_CMD_SET_WPA\n");
=======
		pr_debug("WLAN_CMD_SET_WPA\n");
>>>>>>> v3.18

		if (copy_from_user(&sValue, pReq->data, sizeof(SCmdValue))) {
			result = -EFAULT;
			break;
		}
		if (sValue.dwValue == 1) {
<<<<<<< HEAD
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "up wpadev\n");
			memcpy(pDevice->wpadev->dev_addr, pDevice->dev->dev_addr, ETH_ALEN);
			pDevice->bWPADEVUp = true;
		} else {
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "close wpadev\n");
=======
			pr_debug("up wpadev\n");
			eth_hw_addr_inherit(pDevice->wpadev, pDevice->dev);
			pDevice->bWPADEVUp = true;
		} else {
			pr_debug("close wpadev\n");
>>>>>>> v3.18
			pDevice->bWPADEVUp = false;
		}
		break;

	case WLAN_CMD_AP_START:
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "WLAN_CMD_AP_START\n");
=======
		pr_debug("WLAN_CMD_AP_START\n");
>>>>>>> v3.18
		if (pDevice->bRadioOff == true) {
			CARDbRadioPowerOn(pDevice);
			vMgrTimerInit(pDevice);
			MACvIntEnable(pDevice->PortOffset, IMR_MASK_VALUE);
			add_timer(&pMgmt->sTimerSecondCallback);
		}
		if (copy_from_user(&sStartAPCmd, pReq->data, sizeof(SCmdStartAP))) {
			result = -EFAULT;
			break;
		}

		if (sStartAPCmd.wBSSType == AP) {
			pMgmt->eConfigMode = WMAC_CONFIG_AP;
<<<<<<< HEAD
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "ioct set to AP mode\n");
		} else {
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "ioct BSS type not set to AP mode\n");
=======
			pr_debug("ioct set to AP mode\n");
		} else {
			pr_debug("ioct BSS type not set to AP mode\n");
>>>>>>> v3.18
			result = -EFAULT;
			break;
		}

		if (sStartAPCmd.wBBPType == PHY80211g)
			pMgmt->byAPBBType = PHY_TYPE_11G;
		else if (sStartAPCmd.wBBPType == PHY80211a)
			pMgmt->byAPBBType = PHY_TYPE_11A;
		else
			pMgmt->byAPBBType = PHY_TYPE_11B;

		pItemSSID = (PWLAN_IE_SSID)sStartAPCmd.ssid;
		if (pItemSSID->len > WLAN_SSID_MAXLEN + 1)
			return -EINVAL;
		memset(pMgmt->abyDesireSSID, 0, WLAN_IEHDR_LEN + WLAN_SSID_MAXLEN + 1);
		memcpy(pMgmt->abyDesireSSID, pItemSSID, pItemSSID->len + WLAN_IEHDR_LEN);

		if ((sStartAPCmd.uChannel > 0) && (sStartAPCmd.uChannel <= 14))
			pDevice->uChannel = sStartAPCmd.uChannel;

		if ((sStartAPCmd.uBeaconInt >= 20) && (sStartAPCmd.uBeaconInt <= 1000))
			pMgmt->wIBSSBeaconPeriod = sStartAPCmd.uBeaconInt;
		else
			pMgmt->wIBSSBeaconPeriod = 100;

		if (sStartAPCmd.bShareKeyAuth == true) {
			pMgmt->bShareKeyAlgorithm = true;
<<<<<<< HEAD
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Share Key\n");
		} else {
			pMgmt->bShareKeyAlgorithm = false;
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Open System\n");
=======
			pr_debug("Share Key\n");
		} else {
			pMgmt->bShareKeyAlgorithm = false;
			pr_debug("Open System\n");
>>>>>>> v3.18
		}
		memcpy(pMgmt->abyIBSSSuppRates, abySuppRates, 6);

		if (sStartAPCmd.byBasicRate & BIT3) {
			pMgmt->abyIBSSSuppRates[2] |= BIT7;
			pMgmt->abyIBSSSuppRates[3] |= BIT7;
			pMgmt->abyIBSSSuppRates[4] |= BIT7;
			pMgmt->abyIBSSSuppRates[5] |= BIT7;
		} else if (sStartAPCmd.byBasicRate & BIT2) {
			pMgmt->abyIBSSSuppRates[2] |= BIT7;
			pMgmt->abyIBSSSuppRates[3] |= BIT7;
			pMgmt->abyIBSSSuppRates[4] |= BIT7;
		} else if (sStartAPCmd.byBasicRate & BIT1) {
			pMgmt->abyIBSSSuppRates[2] |= BIT7;
			pMgmt->abyIBSSSuppRates[3] |= BIT7;
		} else if (sStartAPCmd.byBasicRate & BIT1) {
			pMgmt->abyIBSSSuppRates[2] |= BIT7;
		} else {
			/* default 1,2M */
			pMgmt->abyIBSSSuppRates[2] |= BIT7;
			pMgmt->abyIBSSSuppRates[3] |= BIT7;
		}

<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Support Rate= %*ph\n",
			4, pMgmt->abyIBSSSuppRates + 2);
=======
		pr_debug("Support Rate= %*ph\n",
			 4, pMgmt->abyIBSSSuppRates + 2);
>>>>>>> v3.18

		netif_stop_queue(pDevice->dev);
		spin_lock_irq(&pDevice->lock);
		bScheduleCommand((void *)pDevice, WLAN_CMD_RUN_AP, NULL);
		spin_unlock_irq(&pDevice->lock);
		break;

	case WLAN_CMD_GET_NODE_CNT:
		cbListCount = 0;
		pNode = &(pMgmt->sNodeDBTable[0]);
		for (ii = 0; ii < (MAX_NODE_NUM + 1); ii++) {
			pNode = &(pMgmt->sNodeDBTable[ii]);
			if (!pNode->bActive)
				continue;
			cbListCount++;
		}

		sNodeList.uItem = cbListCount;
		if (copy_to_user(pReq->data, &sNodeList, sizeof(SNodeList))) {
			result = -EFAULT;
			break;
		}
		pReq->wResult = 0;
		break;

	case WLAN_CMD_GET_NODE_LIST:
		if (copy_from_user(&sNodeList, pReq->data, sizeof(SNodeList))) {
			result = -EFAULT;
			break;
		}
		if (sNodeList.uItem > (ULONG_MAX - sizeof(SNodeList)) / sizeof(SNodeItem)) {
			result = -EINVAL;
			break;
		}
<<<<<<< HEAD
		pNodeList = (PSNodeList)kmalloc(sizeof(SNodeList) + (sNodeList.uItem * sizeof(SNodeItem)), (int)GFP_ATOMIC);
=======
		pNodeList = (PSNodeList)kmalloc(sizeof(SNodeList) + (sNodeList.uItem * sizeof(SNodeItem)),
						GFP_ATOMIC);
>>>>>>> v3.18
		if (pNodeList == NULL) {
			result = -ENOMEM;
			break;
		}
		pNodeList->uItem = sNodeList.uItem;
		pNode = &(pMgmt->sNodeDBTable[0]);
		for (ii = 0, jj = 0; ii < (MAX_NODE_NUM + 1); ii++) {
			pNode = &(pMgmt->sNodeDBTable[ii]);
			if (pNode->bActive) {
				pNodeList->sNodeList[jj].wAID = pNode->wAID;
				memcpy(pNodeList->sNodeList[jj].abyMACAddr, pNode->abyMACAddr, WLAN_ADDR_LEN);
				pNodeList->sNodeList[jj].wTxDataRate = pNode->wTxDataRate;
				pNodeList->sNodeList[jj].wInActiveCount = (unsigned short)pNode->uInActiveCount;
				pNodeList->sNodeList[jj].wEnQueueCnt = (unsigned short)pNode->wEnQueueCnt;
				pNodeList->sNodeList[jj].wFlags = (unsigned short)pNode->dwFlags;
				pNodeList->sNodeList[jj].bPWBitOn = pNode->bPSEnable;
				pNodeList->sNodeList[jj].byKeyIndex = pNode->byKeyIndex;
				pNodeList->sNodeList[jj].wWepKeyLength = pNode->uWepKeyLength;
				memcpy(&(pNodeList->sNodeList[jj].abyWepKey[0]), &(pNode->abyWepKey[0]), WEP_KEYMAXLEN);
<<<<<<< HEAD
				DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "key= %2.2X:%2.2X:%2.2X:%2.2X:%2.2X\n",
					pNodeList->sNodeList[jj].abyWepKey[0],
					pNodeList->sNodeList[jj].abyWepKey[1],
					pNodeList->sNodeList[jj].abyWepKey[2],
					pNodeList->sNodeList[jj].abyWepKey[3],
					pNodeList->sNodeList[jj].abyWepKey[4]);
=======
				pr_debug("key= %2.2X:%2.2X:%2.2X:%2.2X:%2.2X\n",
					 pNodeList->sNodeList[jj].abyWepKey[0],
					 pNodeList->sNodeList[jj].abyWepKey[1],
					 pNodeList->sNodeList[jj].abyWepKey[2],
					 pNodeList->sNodeList[jj].abyWepKey[3],
					 pNodeList->sNodeList[jj].abyWepKey[4]);
>>>>>>> v3.18
				pNodeList->sNodeList[jj].bIsInFallback = pNode->bIsInFallback;
				pNodeList->sNodeList[jj].uTxFailures = pNode->uTxFailures;
				pNodeList->sNodeList[jj].uTxAttempts = pNode->uTxAttempts;
				pNodeList->sNodeList[jj].wFailureRatio = (unsigned short)pNode->uFailureRatio;
				jj++;
				if (jj >= pNodeList->uItem)
					break;
			}
		}
		if (copy_to_user(pReq->data, pNodeList, sizeof(SNodeList) + (sNodeList.uItem * sizeof(SNodeItem)))) {
			result = -EFAULT;
			break;
		}
		kfree(pNodeList);
		pReq->wResult = 0;
		break;

#ifdef WPA_SM_Transtatus
	case 0xFF:
		memset(wpa_Result.ifname, 0, sizeof(wpa_Result.ifname));
		wpa_Result.proto = 0;
		wpa_Result.key_mgmt = 0;
		wpa_Result.eap_type = 0;
		wpa_Result.authenticated = false;
		pDevice->fWPA_Authened = false;
		if (copy_from_user(&wpa_Result, pReq->data, sizeof(wpa_Result))) {
			result = -EFAULT;
			break;
		}

		if (wpa_Result.authenticated == true) {
#ifdef SndEvt_ToAPI
			{
				union iwreq_data wrqu;

				pItemSSID = (PWLAN_IE_SSID)pMgmt->abyCurrSSID;

				memset(&wrqu, 0, sizeof(wrqu));
				wrqu.data.flags = RT_WPACONNECTED_EVENT_FLAG;
				wrqu.data.length = pItemSSID->len;
				wireless_send_event(pDevice->dev, IWEVCUSTOM, &wrqu, pItemSSID->abySSID);
			}
#endif
			pDevice->fWPA_Authened = true; /* is successful peer to wpa_Result.authenticated? */
		}
		pReq->wResult = 0;
		break;
#endif

	default:
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Private command not support..\n");
=======
		pr_debug("Private command not support..\n");
>>>>>>> v3.18
	}

	return result;
}
