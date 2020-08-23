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
 * File: power.c
 *
 * Purpose: Handles 802.11 power management  functions
 *
 * Author: Lyndon Chen
 *
 * Date: July 17, 2002
 *
 * Functions:
 *      PSvEnablePowerSaving - Enable Power Saving Mode
 *      PSvDiasblePowerSaving - Disable Power Saving Mode
 *      PSbConsiderPowerDown - Decide if we can Power Down
 *      PSvSendPSPOLL - Send PS-POLL packet
 *      PSbSendNullPacket - Send Null packet
 *      PSbIsNextTBTTWakeUp - Decide if we need to wake up at next Beacon
 *
 * Revision History:
 *
 */

#include "ttype.h"
#include "mac.h"
#include "device.h"
#include "wmgr.h"
#include "power.h"
#include "wcmd.h"
#include "rxtx.h"
#include "card.h"

/*---------------------  Static Definitions -------------------------*/

/*---------------------  Static Classes  ----------------------------*/

<<<<<<< HEAD
<<<<<<< HEAD
/*---------------------  Static Variables  --------------------------*/
static int msglevel = MSG_LEVEL_INFO;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
/*---------------------  Static Functions  --------------------------*/

/*---------------------  Export Variables  --------------------------*/

/*---------------------  Export Functions  --------------------------*/

/*+
 *
 * Routine Description:
 * Enable hw power saving functions
 *
 * Return Value:
 *    None.
 *
 -*/

void
PSvEnablePowerSaving(
	void *hDeviceContext,
	unsigned short wListenInterval
)
{
<<<<<<< HEAD
<<<<<<< HEAD
	PSDevice        pDevice = (PSDevice)hDeviceContext;
=======
	struct vnt_private *pDevice = hDeviceContext;
>>>>>>> v3.18
=======
	struct vnt_private *pDevice = hDeviceContext;
>>>>>>> v3.18
	PSMgmtObject    pMgmt = pDevice->pMgmt;
	unsigned short wAID = pMgmt->wCurrAID | BIT14 | BIT15;

	// set period of power up before TBTT
	VNSvOutPortW(pDevice->PortOffset + MAC_REG_PWBT, C_PWBT);
<<<<<<< HEAD
<<<<<<< HEAD
	if (pDevice->eOPMode != OP_MODE_ADHOC) {
=======
	if (pDevice->op_mode != NL80211_IFTYPE_ADHOC) {
>>>>>>> v3.18
=======
	if (pDevice->op_mode != NL80211_IFTYPE_ADHOC) {
>>>>>>> v3.18
		// set AID
		VNSvOutPortW(pDevice->PortOffset + MAC_REG_AIDATIM, wAID);
	} else {
		// set ATIM Window
		MACvWriteATIMW(pDevice->PortOffset, pMgmt->wCurrATIMWindow);
	}
	// Set AutoSleep
	MACvRegBitsOn(pDevice->PortOffset, MAC_REG_PSCFG, PSCFG_AUTOSLEEP);
	// Set HWUTSF
	MACvRegBitsOn(pDevice->PortOffset, MAC_REG_TFTCTL, TFTCTL_HWUTSF);

	if (wListenInterval >= 2) {
		// clear always listen beacon
		MACvRegBitsOff(pDevice->PortOffset, MAC_REG_PSCTL, PSCTL_ALBCN);
<<<<<<< HEAD
<<<<<<< HEAD
		//pDevice->wCFG &= ~CFG_ALB;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
		// first time set listen next beacon
		MACvRegBitsOn(pDevice->PortOffset, MAC_REG_PSCTL, PSCTL_LNBCN);
		pMgmt->wCountToWakeUp = wListenInterval;
	} else {
		// always listen beacon
		MACvRegBitsOn(pDevice->PortOffset, MAC_REG_PSCTL, PSCTL_ALBCN);
<<<<<<< HEAD
<<<<<<< HEAD
		//pDevice->wCFG |= CFG_ALB;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
		pMgmt->wCountToWakeUp = 0;
	}

	// enable power saving hw function
	MACvRegBitsOn(pDevice->PortOffset, MAC_REG_PSCTL, PSCTL_PSEN);
	pDevice->bEnablePSMode = true;

<<<<<<< HEAD
<<<<<<< HEAD
	if (pDevice->eOPMode == OP_MODE_ADHOC) {
//        bMgrPrepareBeaconToSend((void *)pDevice, pMgmt);
	}
	// We don't send null pkt in ad hoc mode since beacon will handle this.
	else if (pDevice->eOPMode == OP_MODE_INFRASTRUCTURE) {
		PSbSendNullPacket(pDevice);
	}
	pDevice->bPWBitOn = true;
	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "PS:Power Saving Mode Enable... \n");
	return;
=======
=======
>>>>>>> v3.18
	/* We don't send null pkt in ad hoc mode since beacon will handle this. */
	if (pDevice->op_mode != NL80211_IFTYPE_ADHOC &&
	    pDevice->op_mode == NL80211_IFTYPE_STATION)
		PSbSendNullPacket(pDevice);

	pDevice->bPWBitOn = true;
	pr_debug("PS:Power Saving Mode Enable...\n");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 * Disable hw power saving functions
 *
 * Return Value:
 *    None.
 *
 -*/

void
PSvDisablePowerSaving(
	void *hDeviceContext
)
{
<<<<<<< HEAD
<<<<<<< HEAD
	PSDevice        pDevice = (PSDevice)hDeviceContext;
//    PSMgmtObject    pMgmt = pDevice->pMgmt;
=======
	struct vnt_private *pDevice = hDeviceContext;
>>>>>>> v3.18
=======
	struct vnt_private *pDevice = hDeviceContext;
>>>>>>> v3.18

	// disable power saving hw function
	MACbPSWakeup(pDevice->PortOffset);
	//clear AutoSleep
	MACvRegBitsOff(pDevice->PortOffset, MAC_REG_PSCFG, PSCFG_AUTOSLEEP);
	//clear HWUTSF
	MACvRegBitsOff(pDevice->PortOffset, MAC_REG_TFTCTL, TFTCTL_HWUTSF);
	// set always listen beacon
	MACvRegBitsOn(pDevice->PortOffset, MAC_REG_PSCTL, PSCTL_ALBCN);

	pDevice->bEnablePSMode = false;

<<<<<<< HEAD
<<<<<<< HEAD
	if (pDevice->eOPMode == OP_MODE_INFRASTRUCTURE) {
		PSbSendNullPacket(pDevice);
	}
	pDevice->bPWBitOn = false;
	return;
=======
=======
>>>>>>> v3.18
	if (pDevice->op_mode == NL80211_IFTYPE_STATION)
		PSbSendNullPacket(pDevice);

	pDevice->bPWBitOn = false;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 * Consider to power down when no more packets to tx or rx.
 *
 * Return Value:
 *    true, if power down success
 *    false, if fail
 -*/

bool
PSbConsiderPowerDown(
	void *hDeviceContext,
	bool bCheckRxDMA,
	bool bCheckCountToWakeUp
)
{
<<<<<<< HEAD
<<<<<<< HEAD
	PSDevice        pDevice = (PSDevice)hDeviceContext;
=======
	struct vnt_private *pDevice = hDeviceContext;
>>>>>>> v3.18
=======
	struct vnt_private *pDevice = hDeviceContext;
>>>>>>> v3.18
	PSMgmtObject    pMgmt = pDevice->pMgmt;
	unsigned int uIdx;

	// check if already in Doze mode
	if (MACbIsRegBitsOn(pDevice->PortOffset, MAC_REG_PSCTL, PSCTL_PS))
		return true;

	if (pMgmt->eCurrMode != WMAC_MODE_IBSS_STA) {
		// check if in TIM wake period
		if (pMgmt->bInTIMWake)
			return false;
	}

	// check scan state
	if (pDevice->bCmdRunning)
		return false;

	// Force PSEN on
	MACvRegBitsOn(pDevice->PortOffset, MAC_REG_PSCTL, PSCTL_PSEN);

	// check if all TD are empty,
	for (uIdx = 0; uIdx < TYPE_MAXTD; uIdx++) {
		if (pDevice->iTDUsed[uIdx] != 0)
			return false;
	}

	// check if rx isr is clear
	if (bCheckRxDMA &&
	    ((pDevice->dwIsr & ISR_RXDMA0) != 0) &&
	    ((pDevice->dwIsr & ISR_RXDMA1) != 0)) {
		return false;
	}

	if (pMgmt->eCurrMode != WMAC_MODE_IBSS_STA) {
		if (bCheckCountToWakeUp &&
		    (pMgmt->wCountToWakeUp == 0 || pMgmt->wCountToWakeUp == 1)) {
			return false;
		}
	}

	// no Tx, no Rx isr, now go to Doze
	MACvRegBitsOn(pDevice->PortOffset, MAC_REG_PSCTL, PSCTL_GO2DOZE);
<<<<<<< HEAD
<<<<<<< HEAD
	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Go to Doze ZZZZZZZZZZZZZZZ\n");
=======
	pr_debug("Go to Doze ZZZZZZZZZZZZZZZ\n");
>>>>>>> v3.18
=======
	pr_debug("Go to Doze ZZZZZZZZZZZZZZZ\n");
>>>>>>> v3.18
	return true;
}

/*+
 *
 * Routine Description:
 * Send PS-POLL packet
 *
 * Return Value:
 *    None.
 *
 -*/

void
PSvSendPSPOLL(
	void *hDeviceContext
)
{
<<<<<<< HEAD
<<<<<<< HEAD
	PSDevice            pDevice = (PSDevice)hDeviceContext;
=======
	struct vnt_private *pDevice = hDeviceContext;
>>>>>>> v3.18
=======
	struct vnt_private *pDevice = hDeviceContext;
>>>>>>> v3.18
	PSMgmtObject        pMgmt = pDevice->pMgmt;
	PSTxMgmtPacket      pTxPacket = NULL;

	memset(pMgmt->pbyPSPacketPool, 0, sizeof(STxMgmtPacket) + WLAN_HDR_ADDR2_LEN);
	pTxPacket = (PSTxMgmtPacket)pMgmt->pbyPSPacketPool;
	pTxPacket->p80211Header = (PUWLAN_80211HDR)((unsigned char *)pTxPacket + sizeof(STxMgmtPacket));
	pTxPacket->p80211Header->sA2.wFrameCtl = cpu_to_le16(
		(
			WLAN_SET_FC_FTYPE(WLAN_TYPE_CTL) |
			WLAN_SET_FC_FSTYPE(WLAN_FSTYPE_PSPOLL) |
			WLAN_SET_FC_PWRMGT(0)
));
	pTxPacket->p80211Header->sA2.wDurationID = pMgmt->wCurrAID | BIT14 | BIT15;
	memcpy(pTxPacket->p80211Header->sA2.abyAddr1, pMgmt->abyCurrBSSID, WLAN_ADDR_LEN);
	memcpy(pTxPacket->p80211Header->sA2.abyAddr2, pMgmt->abyMACAddr, WLAN_ADDR_LEN);
	pTxPacket->cbMPDULen = WLAN_HDR_ADDR2_LEN;
	pTxPacket->cbPayloadLen = 0;
	// send the frame
<<<<<<< HEAD
<<<<<<< HEAD
	if (csMgmt_xmit(pDevice, pTxPacket) != CMD_STATUS_PENDING) {
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Send PS-Poll packet failed..\n");
	} else {
//        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Send PS-Poll packet success..\n");
	};

	return;
=======
	if (csMgmt_xmit(pDevice, pTxPacket) != CMD_STATUS_PENDING)
		pr_debug("Send PS-Poll packet failed..\n");
>>>>>>> v3.18
=======
	if (csMgmt_xmit(pDevice, pTxPacket) != CMD_STATUS_PENDING)
		pr_debug("Send PS-Poll packet failed..\n");
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 * Send NULL packet to AP for notification power state of STA
 *
 * Return Value:
 *    None.
 *
 -*/
bool
PSbSendNullPacket(
	void *hDeviceContext
)
{
<<<<<<< HEAD
<<<<<<< HEAD
	PSDevice            pDevice = (PSDevice)hDeviceContext;
=======
	struct vnt_private *pDevice = hDeviceContext;
>>>>>>> v3.18
=======
	struct vnt_private *pDevice = hDeviceContext;
>>>>>>> v3.18
	PSTxMgmtPacket      pTxPacket = NULL;
	PSMgmtObject        pMgmt = pDevice->pMgmt;
	unsigned int uIdx;

<<<<<<< HEAD
<<<<<<< HEAD
	if (pDevice->bLinkPass == false) {
		return false;
	}
#ifdef TxInSleep
	if ((pDevice->bEnablePSMode == false) &&
	    (pDevice->fTxDataInSleep == false)) {
		return false;
	}
#else
	if (pDevice->bEnablePSMode == false) {
		return false;
	}
#endif
=======
=======
>>>>>>> v3.18
	if (!pDevice->bLinkPass)
		return false;

	if (!pDevice->bEnablePSMode && !pDevice->fTxDataInSleep)
		return false;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (pDevice->bEnablePSMode) {
		for (uIdx = 0; uIdx < TYPE_MAXTD; uIdx++) {
			if (pDevice->iTDUsed[uIdx] != 0)
				return false;
		}
	}

	memset(pMgmt->pbyPSPacketPool, 0, sizeof(STxMgmtPacket) + WLAN_NULLDATA_FR_MAXLEN);
	pTxPacket = (PSTxMgmtPacket)pMgmt->pbyPSPacketPool;
	pTxPacket->p80211Header = (PUWLAN_80211HDR)((unsigned char *)pTxPacket + sizeof(STxMgmtPacket));

	if (pDevice->bEnablePSMode) {
		pTxPacket->p80211Header->sA3.wFrameCtl = cpu_to_le16(
			(
				WLAN_SET_FC_FTYPE(WLAN_TYPE_DATA) |
				WLAN_SET_FC_FSTYPE(WLAN_FSTYPE_NULL) |
				WLAN_SET_FC_PWRMGT(1)
));
	} else {
		pTxPacket->p80211Header->sA3.wFrameCtl = cpu_to_le16(
			(
				WLAN_SET_FC_FTYPE(WLAN_TYPE_DATA) |
				WLAN_SET_FC_FSTYPE(WLAN_FSTYPE_NULL) |
				WLAN_SET_FC_PWRMGT(0)
));
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (pMgmt->eCurrMode != WMAC_MODE_IBSS_STA) {
		pTxPacket->p80211Header->sA3.wFrameCtl |= cpu_to_le16((unsigned short)WLAN_SET_FC_TODS(1));
	}
=======
	if (pMgmt->eCurrMode != WMAC_MODE_IBSS_STA)
		pTxPacket->p80211Header->sA3.wFrameCtl |= cpu_to_le16((unsigned short)WLAN_SET_FC_TODS(1));
>>>>>>> v3.18
=======
	if (pMgmt->eCurrMode != WMAC_MODE_IBSS_STA)
		pTxPacket->p80211Header->sA3.wFrameCtl |= cpu_to_le16((unsigned short)WLAN_SET_FC_TODS(1));
>>>>>>> v3.18

	memcpy(pTxPacket->p80211Header->sA3.abyAddr1, pMgmt->abyCurrBSSID, WLAN_ADDR_LEN);
	memcpy(pTxPacket->p80211Header->sA3.abyAddr2, pMgmt->abyMACAddr, WLAN_ADDR_LEN);
	memcpy(pTxPacket->p80211Header->sA3.abyAddr3, pMgmt->abyCurrBSSID, WLAN_BSSID_LEN);
	pTxPacket->cbMPDULen = WLAN_HDR_ADDR3_LEN;
	pTxPacket->cbPayloadLen = 0;
	// send the frame
	if (csMgmt_xmit(pDevice, pTxPacket) != CMD_STATUS_PENDING) {
<<<<<<< HEAD
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Send Null Packet failed !\n");
		return false;
	} else {
//            DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Send Null Packet success....\n");
=======
		pr_debug("Send Null Packet failed !\n");
		return false;
>>>>>>> v3.18
=======
		pr_debug("Send Null Packet failed !\n");
		return false;
>>>>>>> v3.18
	}

	return true;
}

/*+
 *
 * Routine Description:
 * Check if Next TBTT must wake up
 *
 * Return Value:
 *    None.
 *
 -*/

bool
PSbIsNextTBTTWakeUp(
	void *hDeviceContext
)
{
<<<<<<< HEAD
<<<<<<< HEAD
	PSDevice         pDevice = (PSDevice)hDeviceContext;
=======
	struct vnt_private *pDevice = hDeviceContext;
>>>>>>> v3.18
=======
	struct vnt_private *pDevice = hDeviceContext;
>>>>>>> v3.18
	PSMgmtObject        pMgmt = pDevice->pMgmt;
	bool bWakeUp = false;

	if (pMgmt->wListenInterval >= 2) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (pMgmt->wCountToWakeUp == 0) {
			pMgmt->wCountToWakeUp = pMgmt->wListenInterval;
		}
=======
		if (pMgmt->wCountToWakeUp == 0)
			pMgmt->wCountToWakeUp = pMgmt->wListenInterval;
>>>>>>> v3.18
=======
		if (pMgmt->wCountToWakeUp == 0)
			pMgmt->wCountToWakeUp = pMgmt->wListenInterval;
>>>>>>> v3.18

		pMgmt->wCountToWakeUp--;

		if (pMgmt->wCountToWakeUp == 1) {
			// Turn on wake up to listen next beacon
			MACvRegBitsOn(pDevice->PortOffset, MAC_REG_PSCTL, PSCTL_LNBCN);
			bWakeUp = true;
		}

	}

	return bWakeUp;
}
