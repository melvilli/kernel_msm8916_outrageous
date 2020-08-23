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
 * File: 80211mgr.c
 *
 * Purpose: Handles the 802.11 management support functions
 *
 * Author: Lyndon Chen
 *
 * Date: May 8, 2002
 *
 * Functions:
 *      vMgrEncodeBeacon - Encode the Beacon frame
 *      vMgrDecodeBeacon - Decode the Beacon frame
 *      vMgrEncodeIBSSATIM - Encode the IBSS ATIM frame
 *      vMgrDecodeIBSSATIM - Decode the IBSS ATIM frame
 *      vMgrEncodeDisassociation - Encode the Disassociation frame
 *      vMgrDecodeDisassociation - Decode the Disassociation frame
 *      vMgrEncodeAssocRequest - Encode the Association request frame
 *      vMgrDecodeAssocRequest - Decode the Association request frame
 *      vMgrEncodeAssocResponse - Encode the Association response frame
 *      vMgrDecodeAssocResponse - Decode the Association response frame
 *      vMgrEncodeReAssocRequest - Encode the ReAssociation request frame
 *      vMgrDecodeReAssocRequest - Decode the ReAssociation request frame
 *      vMgrEncodeProbeRequest - Encode the Probe request frame
 *      vMgrDecodeProbeRequest - Decode the Probe request frame
 *      vMgrEncodeProbeResponse - Encode the Probe response frame
 *      vMgrDecodeProbeResponse - Decode the Probe response frame
 *      vMgrEncodeAuthen - Encode the Authentication frame
 *      vMgrDecodeAuthen - Decode the Authentication frame
 *      vMgrEncodeDeauthen - Encode the DeAuthentication frame
 *      vMgrDecodeDeauthen - Decode the DeAuthentication frame
 *      vMgrEncodeReassocResponse - Encode the Reassociation response frame
 *      vMgrDecodeReassocResponse - Decode the Reassociation response frame
 *
 * Revision History:
 *
 */

#include "tmacro.h"
#include "tether.h"
#include "80211mgr.h"
#include "80211hdr.h"
#include "device.h"
#include "wpa.h"

/*---------------------  Static Definitions -------------------------*/

/*---------------------  Static Classes  ----------------------------*/

<<<<<<< HEAD
/*---------------------  Static Variables  --------------------------*/

static int msglevel = MSG_LEVEL_INFO;
//static int          msglevel                =MSG_LEVEL_DEBUG;
=======
>>>>>>> v3.18
/*---------------------  Static Functions  --------------------------*/

/*---------------------  Export Variables  --------------------------*/

/*---------------------  Export Functions  --------------------------*/

/*+
 *
 * Routine Description:
 * Encode Beacon frame body offset
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrEncodeBeacon(
	PWLAN_FR_BEACON  pFrame
)
{
	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;

<<<<<<< HEAD
	// Fixed Fields
	pFrame->pqwTimestamp = (PQWORD)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					+ WLAN_BEACON_OFF_TS);
	pFrame->pwBeaconInterval = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
						      + WLAN_BEACON_OFF_BCN_INT);
	pFrame->pwCapInfo = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					       + WLAN_BEACON_OFF_CAPINFO);

	pFrame->len = WLAN_HDR_ADDR3_LEN + WLAN_BEACON_OFF_SSID;

	return;
=======
	/* Fixed Fields */
	pFrame->pqwTimestamp = (__le64 *)
				(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
				 WLAN_BEACON_OFF_TS);
	pFrame->pwBeaconInterval = (unsigned short *)
				(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
				 WLAN_BEACON_OFF_BCN_INT);
	pFrame->pwCapInfo = (unsigned short *)
			    (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			     WLAN_BEACON_OFF_CAPINFO);

	pFrame->len = WLAN_HDR_ADDR3_LEN + WLAN_BEACON_OFF_SSID;
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 * Decode Beacon frame body offset
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrDecodeBeacon(
	PWLAN_FR_BEACON  pFrame
)
{
	PWLAN_IE        pItem;

	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;

<<<<<<< HEAD
	// Fixed Fields
	pFrame->pqwTimestamp = (PQWORD)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					+ WLAN_BEACON_OFF_TS);
	pFrame->pwBeaconInterval = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
						      + WLAN_BEACON_OFF_BCN_INT);
	pFrame->pwCapInfo = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					       + WLAN_BEACON_OFF_CAPINFO);

	// Information elements
	pItem = (PWLAN_IE)((unsigned char *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)))
			   + WLAN_BEACON_OFF_SSID);
=======
	/* Fixed Fields */
	pFrame->pqwTimestamp = (__le64 *)
				(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
				 WLAN_BEACON_OFF_TS);
	pFrame->pwBeaconInterval = (unsigned short *)
				   (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
				    WLAN_BEACON_OFF_BCN_INT);
	pFrame->pwCapInfo = (unsigned short *)
			    (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			     WLAN_BEACON_OFF_CAPINFO);

	/* Information elements */
	pItem = (PWLAN_IE)((unsigned char *)
			   (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))) +
			    WLAN_BEACON_OFF_SSID);
>>>>>>> v3.18
	while (((unsigned char *)pItem) < (pFrame->pBuf + pFrame->len)) {
		switch (pItem->byElementID) {
		case WLAN_EID_SSID:
			if (pFrame->pSSID == NULL)
				pFrame->pSSID = (PWLAN_IE_SSID)pItem;
			break;
		case WLAN_EID_SUPP_RATES:
			if (pFrame->pSuppRates == NULL)
				pFrame->pSuppRates = (PWLAN_IE_SUPP_RATES)pItem;
			break;
		case WLAN_EID_FH_PARMS:
<<<<<<< HEAD
			//pFrame->pFHParms = (PWLAN_IE_FH_PARMS)pItem;
=======
			/* pFrame->pFHParms = (PWLAN_IE_FH_PARMS)pItem; */
>>>>>>> v3.18
			break;
		case WLAN_EID_DS_PARMS:
			if (pFrame->pDSParms == NULL)
				pFrame->pDSParms = (PWLAN_IE_DS_PARMS)pItem;
			break;
		case WLAN_EID_CF_PARMS:
			if (pFrame->pCFParms == NULL)
				pFrame->pCFParms = (PWLAN_IE_CF_PARMS)pItem;
			break;
		case WLAN_EID_IBSS_PARMS:
			if (pFrame->pIBSSParms == NULL)
				pFrame->pIBSSParms = (PWLAN_IE_IBSS_PARMS)pItem;
			break;
		case WLAN_EID_TIM:
			if (pFrame->pTIM == NULL)
				pFrame->pTIM = (PWLAN_IE_TIM)pItem;
			break;

		case WLAN_EID_RSN:
<<<<<<< HEAD
			if (pFrame->pRSN == NULL) {
				pFrame->pRSN = (PWLAN_IE_RSN)pItem;
			}
=======
			if (pFrame->pRSN == NULL)
				pFrame->pRSN = (PWLAN_IE_RSN)pItem;
>>>>>>> v3.18
			break;
		case WLAN_EID_RSN_WPA:
			if (pFrame->pRSNWPA == NULL) {
				if (WPAb_Is_RSN((PWLAN_IE_RSN_EXT)pItem) == true)
<<<<<<< HEAD
					pFrame->pRSNWPA = (PWLAN_IE_RSN_EXT)pItem;
=======
					pFrame->pRSNWPA =
						       (PWLAN_IE_RSN_EXT)pItem;
>>>>>>> v3.18
			}
			break;

		case WLAN_EID_ERP:
			if (pFrame->pERP == NULL)
				pFrame->pERP = (PWLAN_IE_ERP)pItem;
			break;
		case WLAN_EID_EXTSUPP_RATES:
			if (pFrame->pExtSuppRates == NULL)
<<<<<<< HEAD
				pFrame->pExtSuppRates = (PWLAN_IE_SUPP_RATES)pItem;
			break;

		case WLAN_EID_COUNTRY:      //7
=======
				pFrame->pExtSuppRates =
						    (PWLAN_IE_SUPP_RATES)pItem;
			break;

		case WLAN_EID_COUNTRY:      /* 7 */
>>>>>>> v3.18
			if (pFrame->pIE_Country == NULL)
				pFrame->pIE_Country = (PWLAN_IE_COUNTRY)pItem;
			break;

<<<<<<< HEAD
		case WLAN_EID_PWR_CONSTRAINT:   //32
			if (pFrame->pIE_PowerConstraint == NULL)
				pFrame->pIE_PowerConstraint = (PWLAN_IE_PW_CONST)pItem;
			break;

		case WLAN_EID_CH_SWITCH:    //37
=======
		case WLAN_EID_PWR_CONSTRAINT:   /* 32 */
			if (pFrame->pIE_PowerConstraint == NULL)
				pFrame->pIE_PowerConstraint =
						      (PWLAN_IE_PW_CONST)pItem;
			break;

		case WLAN_EID_CH_SWITCH:    /* 37 */
>>>>>>> v3.18
			if (pFrame->pIE_CHSW == NULL)
				pFrame->pIE_CHSW = (PWLAN_IE_CH_SW)pItem;
			break;

<<<<<<< HEAD
		case WLAN_EID_QUIET:        //40
=======
		case WLAN_EID_QUIET:        /* 40 */
>>>>>>> v3.18
			if (pFrame->pIE_Quiet == NULL)
				pFrame->pIE_Quiet = (PWLAN_IE_QUIET)pItem;
			break;

		case WLAN_EID_IBSS_DFS:
			if (pFrame->pIE_IBSSDFS == NULL)
				pFrame->pIE_IBSSDFS = (PWLAN_IE_IBSS_DFS)pItem;
			break;

		default:
<<<<<<< HEAD
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Unrecognized EID=%dd in beacon decode.\n", pItem->byElementID);
=======
			pr_debug("Unrecognized EID=%dd in beacon decode\n",
				 pItem->byElementID);
>>>>>>> v3.18
			break;

		}
		pItem = (PWLAN_IE)(((unsigned char *)pItem) + 2 + pItem->len);
	}
<<<<<<< HEAD

	return;
=======
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 *  Encode IBSS ATIM
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrEncodeIBSSATIM(
	PWLAN_FR_IBSSATIM   pFrame
)
{
	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;
	pFrame->len = WLAN_HDR_ADDR3_LEN;
<<<<<<< HEAD

	return;
=======
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 *  Decode IBSS ATIM
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrDecodeIBSSATIM(
	PWLAN_FR_IBSSATIM   pFrame
)
{
	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;
<<<<<<< HEAD

	return;
=======
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 *  Encode Disassociation
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrEncodeDisassociation(
	PWLAN_FR_DISASSOC  pFrame
)
{
	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;

<<<<<<< HEAD
	// Fixed Fields
	pFrame->pwReason = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					      + WLAN_DISASSOC_OFF_REASON);
	pFrame->len = WLAN_HDR_ADDR3_LEN + WLAN_DISASSOC_OFF_REASON + sizeof(*(pFrame->pwReason));

	return;
=======
	/* Fixed Fields */
	pFrame->pwReason = (unsigned short *)
			   (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			    WLAN_DISASSOC_OFF_REASON);
	pFrame->len = WLAN_HDR_ADDR3_LEN + WLAN_DISASSOC_OFF_REASON +
		      sizeof(*(pFrame->pwReason));
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 *  Decode Disassociation
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrDecodeDisassociation(
	PWLAN_FR_DISASSOC  pFrame
)
{
	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;

<<<<<<< HEAD
	// Fixed Fields
	pFrame->pwReason = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					      + WLAN_DISASSOC_OFF_REASON);

	return;
=======
	/* Fixed Fields */
	pFrame->pwReason = (unsigned short *)
			   (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			    WLAN_DISASSOC_OFF_REASON);
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 *  Encode Association Request
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrEncodeAssocRequest(
	PWLAN_FR_ASSOCREQ  pFrame
)
{
	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;
<<<<<<< HEAD
	// Fixed Fields
	pFrame->pwCapInfo = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					       + WLAN_ASSOCREQ_OFF_CAP_INFO);
	pFrame->pwListenInterval = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
						      + WLAN_ASSOCREQ_OFF_LISTEN_INT);
	pFrame->len = WLAN_HDR_ADDR3_LEN + WLAN_ASSOCREQ_OFF_LISTEN_INT + sizeof(*(pFrame->pwListenInterval));
	return;
=======
	/* Fixed Fields */
	pFrame->pwCapInfo = (unsigned short *)
			    (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			     WLAN_ASSOCREQ_OFF_CAP_INFO);
	pFrame->pwListenInterval = (unsigned short *)
				   (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
				    WLAN_ASSOCREQ_OFF_LISTEN_INT);
	pFrame->len = WLAN_HDR_ADDR3_LEN + WLAN_ASSOCREQ_OFF_LISTEN_INT +
		      sizeof(*(pFrame->pwListenInterval));
>>>>>>> v3.18
}

/*+
 *
 * Routine Description: (AP)
 *  Decode Association Request
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrDecodeAssocRequest(
	PWLAN_FR_ASSOCREQ  pFrame
)
{
	PWLAN_IE   pItem;

	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;
<<<<<<< HEAD
	// Fixed Fields
	pFrame->pwCapInfo = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					       + WLAN_ASSOCREQ_OFF_CAP_INFO);
	pFrame->pwListenInterval = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
						      + WLAN_ASSOCREQ_OFF_LISTEN_INT);

	// Information elements
=======
	/* Fixed Fields */
	pFrame->pwCapInfo = (unsigned short *)
			    (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			     WLAN_ASSOCREQ_OFF_CAP_INFO);
	pFrame->pwListenInterval = (unsigned short *)
				   (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
				    WLAN_ASSOCREQ_OFF_LISTEN_INT);

	/* Information elements */
>>>>>>> v3.18
	pItem = (PWLAN_IE)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
			   + WLAN_ASSOCREQ_OFF_SSID);

	while (((unsigned char *)pItem) < (pFrame->pBuf + pFrame->len)) {
		switch (pItem->byElementID) {
		case WLAN_EID_SSID:
			if (pFrame->pSSID == NULL)
				pFrame->pSSID = (PWLAN_IE_SSID)pItem;
			break;
		case WLAN_EID_SUPP_RATES:
			if (pFrame->pSuppRates == NULL)
<<<<<<< HEAD
				pFrame->pSuppRates = (PWLAN_IE_SUPP_RATES)pItem;
			break;

		case WLAN_EID_RSN:
			if (pFrame->pRSN == NULL) {
				pFrame->pRSN = (PWLAN_IE_RSN)pItem;
			}
=======
				pFrame->pSuppRates =
						   (PWLAN_IE_SUPP_RATES)pItem;
			break;

		case WLAN_EID_RSN:
			if (pFrame->pRSN == NULL)
				pFrame->pRSN = (PWLAN_IE_RSN)pItem;
>>>>>>> v3.18
			break;
		case WLAN_EID_RSN_WPA:
			if (pFrame->pRSNWPA == NULL) {
				if (WPAb_Is_RSN((PWLAN_IE_RSN_EXT)pItem) == true)
<<<<<<< HEAD
					pFrame->pRSNWPA = (PWLAN_IE_RSN_EXT)pItem;
=======
					pFrame->pRSNWPA =
						       (PWLAN_IE_RSN_EXT)pItem;
>>>>>>> v3.18
			}
			break;
		case WLAN_EID_EXTSUPP_RATES:
			if (pFrame->pExtSuppRates == NULL)
<<<<<<< HEAD
				pFrame->pExtSuppRates = (PWLAN_IE_SUPP_RATES)pItem;
			break;

		default:
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Unrecognized EID=%dd in assocreq decode.\n",
				pItem->byElementID);
=======
				pFrame->pExtSuppRates =
						    (PWLAN_IE_SUPP_RATES)pItem;
			break;

		default:
			pr_debug("Unrecognized EID=%dd in assocreq decode\n",
				 pItem->byElementID);
>>>>>>> v3.18
			break;
		}
		pItem = (PWLAN_IE)(((unsigned char *)pItem) + 2 + pItem->len);
	}
<<<<<<< HEAD
	return;
=======
>>>>>>> v3.18
}

/*+
 *
 * Routine Description: (AP)
 *  Encode Association Response
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrEncodeAssocResponse(
	PWLAN_FR_ASSOCRESP  pFrame
)
{
	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;

<<<<<<< HEAD
	// Fixed Fields
	pFrame->pwCapInfo = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					       + WLAN_ASSOCRESP_OFF_CAP_INFO);
	pFrame->pwStatus = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					      + WLAN_ASSOCRESP_OFF_STATUS);
	pFrame->pwAid = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					   + WLAN_ASSOCRESP_OFF_AID);
	pFrame->len = WLAN_HDR_ADDR3_LEN + WLAN_ASSOCRESP_OFF_AID
		+ sizeof(*(pFrame->pwAid));

	return;
=======
	/* Fixed Fields */
	pFrame->pwCapInfo = (unsigned short *)
			    (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			     WLAN_ASSOCRESP_OFF_CAP_INFO);
	pFrame->pwStatus = (unsigned short *)
			   (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			    WLAN_ASSOCRESP_OFF_STATUS);
	pFrame->pwAid = (unsigned short *)
			(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			 WLAN_ASSOCRESP_OFF_AID);
	pFrame->len = WLAN_HDR_ADDR3_LEN + WLAN_ASSOCRESP_OFF_AID +
		      sizeof(*(pFrame->pwAid));
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 *  Decode Association Response
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrDecodeAssocResponse(
	PWLAN_FR_ASSOCRESP  pFrame
)
{
	PWLAN_IE   pItem;

	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;

<<<<<<< HEAD
	// Fixed Fields
	pFrame->pwCapInfo = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					       + WLAN_ASSOCRESP_OFF_CAP_INFO);
	pFrame->pwStatus = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					      + WLAN_ASSOCRESP_OFF_STATUS);
	pFrame->pwAid = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					   + WLAN_ASSOCRESP_OFF_AID);

	// Information elements
	pFrame->pSuppRates  = (PWLAN_IE_SUPP_RATES)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
						    + WLAN_ASSOCRESP_OFF_SUPP_RATES);
=======
	/* Fixed Fields */
	pFrame->pwCapInfo = (unsigned short *)
			    (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			     WLAN_ASSOCRESP_OFF_CAP_INFO);
	pFrame->pwStatus = (unsigned short *)
			   (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			    WLAN_ASSOCRESP_OFF_STATUS);
	pFrame->pwAid = (unsigned short *)
			(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			 WLAN_ASSOCRESP_OFF_AID);

	/* Information elements */
	pFrame->pSuppRates  = (PWLAN_IE_SUPP_RATES)
			      (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			       WLAN_ASSOCRESP_OFF_SUPP_RATES);
>>>>>>> v3.18

	pItem = (PWLAN_IE)(pFrame->pSuppRates);
	pItem = (PWLAN_IE)(((unsigned char *)pItem) + 2 + pItem->len);

	if ((((unsigned char *)pItem) < (pFrame->pBuf + pFrame->len)) &&
	    (pItem->byElementID == WLAN_EID_EXTSUPP_RATES)) {
		pFrame->pExtSuppRates = (PWLAN_IE_SUPP_RATES)pItem;
<<<<<<< HEAD
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "pFrame->pExtSuppRates=[%p].\n", pItem);
	} else {
		pFrame->pExtSuppRates = NULL;
	}
	return;
=======
		pr_debug("pFrame->pExtSuppRates=[%p]\n", pItem);
	} else {
		pFrame->pExtSuppRates = NULL;
	}
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 *  Encode Reassociation Request
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrEncodeReassocRequest(
	PWLAN_FR_REASSOCREQ  pFrame
)
{
	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;

<<<<<<< HEAD
	// Fixed Fields
	pFrame->pwCapInfo = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					       + WLAN_REASSOCREQ_OFF_CAP_INFO);
	pFrame->pwListenInterval = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
						      + WLAN_REASSOCREQ_OFF_LISTEN_INT);
	pFrame->pAddrCurrAP = (PIEEE_ADDR)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					   + WLAN_REASSOCREQ_OFF_CURR_AP);
	pFrame->len = WLAN_HDR_ADDR3_LEN + WLAN_REASSOCREQ_OFF_CURR_AP + sizeof(*(pFrame->pAddrCurrAP));

	return;
=======
	/* Fixed Fields */
	pFrame->pwCapInfo = (unsigned short *)
			    (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			     WLAN_REASSOCREQ_OFF_CAP_INFO);
	pFrame->pwListenInterval = (unsigned short *)
				   (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
				    WLAN_REASSOCREQ_OFF_LISTEN_INT);
	pFrame->pAddrCurrAP = (PIEEE_ADDR)
			      (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			       WLAN_REASSOCREQ_OFF_CURR_AP);
	pFrame->len = WLAN_HDR_ADDR3_LEN + WLAN_REASSOCREQ_OFF_CURR_AP +
		      sizeof(*(pFrame->pAddrCurrAP));
>>>>>>> v3.18
}

/*+
 *
 * Routine Description: (AP)
 *  Decode Reassociation Request
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrDecodeReassocRequest(
	PWLAN_FR_REASSOCREQ  pFrame
)
{
	PWLAN_IE   pItem;
<<<<<<< HEAD
	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;

	// Fixed Fields
	pFrame->pwCapInfo = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					       + WLAN_REASSOCREQ_OFF_CAP_INFO);
	pFrame->pwListenInterval = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
						      + WLAN_REASSOCREQ_OFF_LISTEN_INT);
	pFrame->pAddrCurrAP = (PIEEE_ADDR)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					   + WLAN_REASSOCREQ_OFF_CURR_AP);

	// Information elements
=======

	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;

	/* Fixed Fields */
	pFrame->pwCapInfo = (unsigned short *)
			    (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			     WLAN_REASSOCREQ_OFF_CAP_INFO);
	pFrame->pwListenInterval = (unsigned short *)
				   (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
				    WLAN_REASSOCREQ_OFF_LISTEN_INT);
	pFrame->pAddrCurrAP = (PIEEE_ADDR)
			      (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			       WLAN_REASSOCREQ_OFF_CURR_AP);

	/* Information elements */
>>>>>>> v3.18
	pItem = (PWLAN_IE)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
			   + WLAN_REASSOCREQ_OFF_SSID);

	while (((unsigned char *)pItem) < (pFrame->pBuf + pFrame->len)) {
		switch (pItem->byElementID) {
		case WLAN_EID_SSID:
			if (pFrame->pSSID == NULL)
				pFrame->pSSID = (PWLAN_IE_SSID)pItem;
			break;
		case WLAN_EID_SUPP_RATES:
			if (pFrame->pSuppRates == NULL)
<<<<<<< HEAD
				pFrame->pSuppRates = (PWLAN_IE_SUPP_RATES)pItem;
			break;

		case WLAN_EID_RSN:
			if (pFrame->pRSN == NULL) {
				pFrame->pRSN = (PWLAN_IE_RSN)pItem;
			}
=======
				pFrame->pSuppRates =
						    (PWLAN_IE_SUPP_RATES)pItem;
			break;

		case WLAN_EID_RSN:
			if (pFrame->pRSN == NULL)
				pFrame->pRSN = (PWLAN_IE_RSN)pItem;
>>>>>>> v3.18
			break;
		case WLAN_EID_RSN_WPA:
			if (pFrame->pRSNWPA == NULL) {
				if (WPAb_Is_RSN((PWLAN_IE_RSN_EXT)pItem) == true)
<<<<<<< HEAD
					pFrame->pRSNWPA = (PWLAN_IE_RSN_EXT)pItem;
=======
					pFrame->pRSNWPA =
						       (PWLAN_IE_RSN_EXT)pItem;
>>>>>>> v3.18
			}
			break;

		case WLAN_EID_EXTSUPP_RATES:
			if (pFrame->pExtSuppRates == NULL)
<<<<<<< HEAD
				pFrame->pExtSuppRates = (PWLAN_IE_SUPP_RATES)pItem;
			break;
		default:
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Unrecognized EID=%dd in reassocreq decode.\n",
				pItem->byElementID);
=======
				pFrame->pExtSuppRates =
						    (PWLAN_IE_SUPP_RATES)pItem;
			break;
		default:
			pr_debug("Unrecognized EID=%dd in reassocreq decode\n",
				 pItem->byElementID);
>>>>>>> v3.18
			break;
		}
		pItem = (PWLAN_IE)(((unsigned char *)pItem) + 2 + pItem->len);
	}
<<<<<<< HEAD
	return;
=======
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 *  Encode Probe Request
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrEncodeProbeRequest(
	PWLAN_FR_PROBEREQ  pFrame
)
{
	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;
	pFrame->len = WLAN_HDR_ADDR3_LEN;
<<<<<<< HEAD
	return;
=======
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 *  Decode Probe Request
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrDecodeProbeRequest(
	PWLAN_FR_PROBEREQ  pFrame
)
{
	PWLAN_IE   pItem;

	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;

<<<<<<< HEAD
	// Information elements
=======
	/* Information elements */
>>>>>>> v3.18
	pItem = (PWLAN_IE)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)));

	while (((unsigned char *)pItem) < (pFrame->pBuf + pFrame->len)) {
		switch (pItem->byElementID) {
		case WLAN_EID_SSID:
			if (pFrame->pSSID == NULL)
				pFrame->pSSID = (PWLAN_IE_SSID)pItem;
			break;

		case WLAN_EID_SUPP_RATES:
			if (pFrame->pSuppRates == NULL)
<<<<<<< HEAD
				pFrame->pSuppRates = (PWLAN_IE_SUPP_RATES)pItem;
=======
				pFrame->pSuppRates =
						   (PWLAN_IE_SUPP_RATES)pItem;
>>>>>>> v3.18
			break;

		case WLAN_EID_EXTSUPP_RATES:
			if (pFrame->pExtSuppRates == NULL)
<<<<<<< HEAD
				pFrame->pExtSuppRates = (PWLAN_IE_SUPP_RATES)pItem;
			break;

		default:
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Bad EID=%dd in probereq\n", pItem->byElementID);
=======
				pFrame->pExtSuppRates =
						    (PWLAN_IE_SUPP_RATES)pItem;
			break;

		default:
			pr_debug("Bad EID=%dd in probereq\n",
				 pItem->byElementID);
>>>>>>> v3.18
			break;
		}

		pItem = (PWLAN_IE)(((unsigned char *)pItem) + 2 +  pItem->len);
	}
<<<<<<< HEAD
	return;
=======
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 *  Encode Probe Response
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrEncodeProbeResponse(
	PWLAN_FR_PROBERESP  pFrame
)
{
	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;

<<<<<<< HEAD
	// Fixed Fields
	pFrame->pqwTimestamp = (PQWORD)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					+ WLAN_PROBERESP_OFF_TS);
	pFrame->pwBeaconInterval = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
						      + WLAN_PROBERESP_OFF_BCN_INT);
	pFrame->pwCapInfo = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					       + WLAN_PROBERESP_OFF_CAP_INFO);

	pFrame->len = WLAN_HDR_ADDR3_LEN + WLAN_PROBERESP_OFF_CAP_INFO +
		sizeof(*(pFrame->pwCapInfo));

	return;
=======
	/* Fixed Fields */
	pFrame->pqwTimestamp = (__le64 *)
			       (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
				WLAN_PROBERESP_OFF_TS);
	pFrame->pwBeaconInterval = (unsigned short *)
				   (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
				    WLAN_PROBERESP_OFF_BCN_INT);
	pFrame->pwCapInfo = (unsigned short *)
			    (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			     WLAN_PROBERESP_OFF_CAP_INFO);

	pFrame->len = WLAN_HDR_ADDR3_LEN + WLAN_PROBERESP_OFF_CAP_INFO +
		      sizeof(*(pFrame->pwCapInfo));
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 *  Decode Probe Response
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrDecodeProbeResponse(
	PWLAN_FR_PROBERESP  pFrame
)
{
	PWLAN_IE    pItem;

	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;

<<<<<<< HEAD
	// Fixed Fields
	pFrame->pqwTimestamp = (PQWORD)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					+ WLAN_PROBERESP_OFF_TS);
	pFrame->pwBeaconInterval = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
						      + WLAN_PROBERESP_OFF_BCN_INT);
	pFrame->pwCapInfo = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					       + WLAN_PROBERESP_OFF_CAP_INFO);

	// Information elements
=======
	/* Fixed Fields */
	pFrame->pqwTimestamp = (__le64 *)
			       (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
				WLAN_PROBERESP_OFF_TS);
	pFrame->pwBeaconInterval = (unsigned short *)
				   (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
				    WLAN_PROBERESP_OFF_BCN_INT);
	pFrame->pwCapInfo = (unsigned short *)
			    (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			     WLAN_PROBERESP_OFF_CAP_INFO);

	/* Information elements */
>>>>>>> v3.18
	pItem = (PWLAN_IE)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
			   + WLAN_PROBERESP_OFF_SSID);

	while (((unsigned char *)pItem) < (pFrame->pBuf + pFrame->len)) {
		switch (pItem->byElementID) {
		case WLAN_EID_SSID:
			if (pFrame->pSSID == NULL)
				pFrame->pSSID = (PWLAN_IE_SSID)pItem;
			break;
		case WLAN_EID_SUPP_RATES:
			if (pFrame->pSuppRates == NULL)
<<<<<<< HEAD
				pFrame->pSuppRates = (PWLAN_IE_SUPP_RATES)pItem;
=======
				pFrame->pSuppRates =
						   (PWLAN_IE_SUPP_RATES)pItem;
>>>>>>> v3.18
			break;
		case WLAN_EID_FH_PARMS:
			break;
		case WLAN_EID_DS_PARMS:
			if (pFrame->pDSParms == NULL)
				pFrame->pDSParms = (PWLAN_IE_DS_PARMS)pItem;
			break;
		case WLAN_EID_CF_PARMS:
			if (pFrame->pCFParms == NULL)
				pFrame->pCFParms = (PWLAN_IE_CF_PARMS)pItem;
			break;
		case WLAN_EID_IBSS_PARMS:
			if (pFrame->pIBSSParms == NULL)
<<<<<<< HEAD
				pFrame->pIBSSParms = (PWLAN_IE_IBSS_PARMS)pItem;
			break;

		case WLAN_EID_RSN:
			if (pFrame->pRSN == NULL) {
				pFrame->pRSN = (PWLAN_IE_RSN)pItem;
			}
=======
				pFrame->pIBSSParms =
						   (PWLAN_IE_IBSS_PARMS)pItem;
			break;

		case WLAN_EID_RSN:
			if (pFrame->pRSN == NULL)
				pFrame->pRSN = (PWLAN_IE_RSN)pItem;
>>>>>>> v3.18
			break;
		case WLAN_EID_RSN_WPA:
			if (pFrame->pRSNWPA == NULL) {
				if (WPAb_Is_RSN((PWLAN_IE_RSN_EXT)pItem) == true)
<<<<<<< HEAD
					pFrame->pRSNWPA = (PWLAN_IE_RSN_EXT)pItem;
=======
					pFrame->pRSNWPA =
						       (PWLAN_IE_RSN_EXT)pItem;
>>>>>>> v3.18
			}
			break;
		case WLAN_EID_ERP:
			if (pFrame->pERP == NULL)
				pFrame->pERP = (PWLAN_IE_ERP)pItem;
			break;
		case WLAN_EID_EXTSUPP_RATES:
			if (pFrame->pExtSuppRates == NULL)
<<<<<<< HEAD
				pFrame->pExtSuppRates = (PWLAN_IE_SUPP_RATES)pItem;
			break;

		case WLAN_EID_COUNTRY:      //7
=======
				pFrame->pExtSuppRates =
						    (PWLAN_IE_SUPP_RATES)pItem;
			break;

		case WLAN_EID_COUNTRY:      /* 7 */
>>>>>>> v3.18
			if (pFrame->pIE_Country == NULL)
				pFrame->pIE_Country = (PWLAN_IE_COUNTRY)pItem;
			break;

<<<<<<< HEAD
		case WLAN_EID_PWR_CONSTRAINT:   //32
			if (pFrame->pIE_PowerConstraint == NULL)
				pFrame->pIE_PowerConstraint = (PWLAN_IE_PW_CONST)pItem;
			break;

		case WLAN_EID_CH_SWITCH:    //37
=======
		case WLAN_EID_PWR_CONSTRAINT:   /* 32 */
			if (pFrame->pIE_PowerConstraint == NULL)
				pFrame->pIE_PowerConstraint =
						      (PWLAN_IE_PW_CONST)pItem;
			break;

		case WLAN_EID_CH_SWITCH:    /* 37 */
>>>>>>> v3.18
			if (pFrame->pIE_CHSW == NULL)
				pFrame->pIE_CHSW = (PWLAN_IE_CH_SW)pItem;
			break;

<<<<<<< HEAD
		case WLAN_EID_QUIET:        //40
=======
		case WLAN_EID_QUIET:        /* 40 */
>>>>>>> v3.18
			if (pFrame->pIE_Quiet == NULL)
				pFrame->pIE_Quiet = (PWLAN_IE_QUIET)pItem;
			break;

		case WLAN_EID_IBSS_DFS:
			if (pFrame->pIE_IBSSDFS == NULL)
				pFrame->pIE_IBSSDFS = (PWLAN_IE_IBSS_DFS)pItem;
			break;

		default:
<<<<<<< HEAD
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Bad EID=%dd in proberesp\n", pItem->byElementID);
=======
			pr_debug("Bad EID=%dd in proberesp\n",
				 pItem->byElementID);
>>>>>>> v3.18
			break;
		}

		pItem = (PWLAN_IE)(((unsigned char *)pItem) + 2 +  pItem->len);
	}
<<<<<<< HEAD
	return;
=======
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 *     Encode Authentication frame
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrEncodeAuthen(
	PWLAN_FR_AUTHEN  pFrame
)
{
	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;

<<<<<<< HEAD
	// Fixed Fields
	pFrame->pwAuthAlgorithm = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
						     + WLAN_AUTHEN_OFF_AUTH_ALG);
	pFrame->pwAuthSequence = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
						    + WLAN_AUTHEN_OFF_AUTH_SEQ);
	pFrame->pwStatus = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					      + WLAN_AUTHEN_OFF_STATUS);
	pFrame->len = WLAN_HDR_ADDR3_LEN + WLAN_AUTHEN_OFF_STATUS + sizeof(*(pFrame->pwStatus));

	return;
=======
	/* Fixed Fields */
	pFrame->pwAuthAlgorithm = (unsigned short *)
				  (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
				   WLAN_AUTHEN_OFF_AUTH_ALG);
	pFrame->pwAuthSequence = (unsigned short *)
				 (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
				  WLAN_AUTHEN_OFF_AUTH_SEQ);
	pFrame->pwStatus = (unsigned short *)
			   (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			    WLAN_AUTHEN_OFF_STATUS);
	pFrame->len = WLAN_HDR_ADDR3_LEN + WLAN_AUTHEN_OFF_STATUS +
		      sizeof(*(pFrame->pwStatus));
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 *   Decode Authentication
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrDecodeAuthen(
	PWLAN_FR_AUTHEN  pFrame
)
{
	PWLAN_IE    pItem;

	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;

<<<<<<< HEAD
	// Fixed Fields
	pFrame->pwAuthAlgorithm = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
						     + WLAN_AUTHEN_OFF_AUTH_ALG);
	pFrame->pwAuthSequence = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
						    + WLAN_AUTHEN_OFF_AUTH_SEQ);
	pFrame->pwStatus = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					      + WLAN_AUTHEN_OFF_STATUS);

	// Information elements
	pItem = (PWLAN_IE)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
			   + WLAN_AUTHEN_OFF_CHALLENGE);

	if ((((unsigned char *)pItem) < (pFrame->pBuf + pFrame->len)) && (pItem->byElementID == WLAN_EID_CHALLENGE)) {
		pFrame->pChallenge = (PWLAN_IE_CHALLENGE)pItem;
	}

	return;
=======
	/* Fixed Fields */
	pFrame->pwAuthAlgorithm = (unsigned short *)
				  (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
				   WLAN_AUTHEN_OFF_AUTH_ALG);
	pFrame->pwAuthSequence = (unsigned short *)
				 (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
				  WLAN_AUTHEN_OFF_AUTH_SEQ);
	pFrame->pwStatus = (unsigned short *)
			   (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			    WLAN_AUTHEN_OFF_STATUS);

	/* Information elements */
	pItem = (PWLAN_IE)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
			   + WLAN_AUTHEN_OFF_CHALLENGE);

	if (((unsigned char *)pItem) < (pFrame->pBuf + pFrame->len) &&
	    pItem->byElementID == WLAN_EID_CHALLENGE)
		pFrame->pChallenge = (PWLAN_IE_CHALLENGE)pItem;
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 *   Encode Authentication
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrEncodeDeauthen(
	PWLAN_FR_DEAUTHEN  pFrame
)
{
	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;

<<<<<<< HEAD
	// Fixed Fields
	pFrame->pwReason = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					      + WLAN_DEAUTHEN_OFF_REASON);
	pFrame->len = WLAN_HDR_ADDR3_LEN + WLAN_DEAUTHEN_OFF_REASON + sizeof(*(pFrame->pwReason));

	return;
=======
	/* Fixed Fields */
	pFrame->pwReason = (unsigned short *)
			   (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			    WLAN_DEAUTHEN_OFF_REASON);
	pFrame->len = WLAN_HDR_ADDR3_LEN + WLAN_DEAUTHEN_OFF_REASON +
		      sizeof(*(pFrame->pwReason));
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 *   Decode Deauthentication
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrDecodeDeauthen(
	PWLAN_FR_DEAUTHEN  pFrame
)
{
	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;

<<<<<<< HEAD
	// Fixed Fields
	pFrame->pwReason = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					      + WLAN_DEAUTHEN_OFF_REASON);

	return;
=======
	/* Fixed Fields */
	pFrame->pwReason = (unsigned short *)
			   (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			    WLAN_DEAUTHEN_OFF_REASON);
>>>>>>> v3.18
}

/*+
 *
 * Routine Description: (AP)
 *   Encode Reassociation Response
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrEncodeReassocResponse(
	PWLAN_FR_REASSOCRESP  pFrame
)
{
	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;

<<<<<<< HEAD
	// Fixed Fields
	pFrame->pwCapInfo = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					       + WLAN_REASSOCRESP_OFF_CAP_INFO);
	pFrame->pwStatus = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					      + WLAN_REASSOCRESP_OFF_STATUS);
	pFrame->pwAid = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					   + WLAN_REASSOCRESP_OFF_AID);

	pFrame->len = WLAN_HDR_ADDR3_LEN + WLAN_REASSOCRESP_OFF_AID + sizeof(*(pFrame->pwAid));

	return;
=======
	/* Fixed Fields */
	pFrame->pwCapInfo = (unsigned short *)
			    (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			     WLAN_REASSOCRESP_OFF_CAP_INFO);
	pFrame->pwStatus = (unsigned short *)
			   (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			    WLAN_REASSOCRESP_OFF_STATUS);
	pFrame->pwAid = (unsigned short *)
			(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			 WLAN_REASSOCRESP_OFF_AID);

	pFrame->len = WLAN_HDR_ADDR3_LEN + WLAN_REASSOCRESP_OFF_AID +
		      sizeof(*(pFrame->pwAid));
>>>>>>> v3.18
}

/*+
 *
 * Routine Description:
 *   Decode Reassociation Response
 *
 *
 * Return Value:
 *    None.
 *
 -*/

void
vMgrDecodeReassocResponse(
	PWLAN_FR_REASSOCRESP  pFrame
)
{
	PWLAN_IE   pItem;

	pFrame->pHdr = (PUWLAN_80211HDR)pFrame->pBuf;

<<<<<<< HEAD
	// Fixed Fields
	pFrame->pwCapInfo = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					       + WLAN_REASSOCRESP_OFF_CAP_INFO);
	pFrame->pwStatus = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					      + WLAN_REASSOCRESP_OFF_STATUS);
	pFrame->pwAid = (unsigned short *)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
					   + WLAN_REASSOCRESP_OFF_AID);

	//Information elements
	pFrame->pSuppRates = (PWLAN_IE_SUPP_RATES)(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3))
						   + WLAN_REASSOCRESP_OFF_SUPP_RATES);
=======
	/* Fixed Fields */
	pFrame->pwCapInfo = (unsigned short *)
			    (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			     WLAN_REASSOCRESP_OFF_CAP_INFO);
	pFrame->pwStatus = (unsigned short *)
			   (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			    WLAN_REASSOCRESP_OFF_STATUS);
	pFrame->pwAid = (unsigned short *)
			(WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			 WLAN_REASSOCRESP_OFF_AID);

	/* Information elements */
	pFrame->pSuppRates = (PWLAN_IE_SUPP_RATES)
			     (WLAN_HDR_A3_DATA_PTR(&(pFrame->pHdr->sA3)) +
			      WLAN_REASSOCRESP_OFF_SUPP_RATES);
>>>>>>> v3.18

	pItem = (PWLAN_IE)(pFrame->pSuppRates);
	pItem = (PWLAN_IE)(((unsigned char *)pItem) + 2 + pItem->len);

	if ((((unsigned char *)pItem) < (pFrame->pBuf + pFrame->len)) &&
	    (pItem->byElementID == WLAN_EID_EXTSUPP_RATES)) {
		pFrame->pExtSuppRates = (PWLAN_IE_SUPP_RATES)pItem;
	}
<<<<<<< HEAD
	return;
=======
>>>>>>> v3.18
}
