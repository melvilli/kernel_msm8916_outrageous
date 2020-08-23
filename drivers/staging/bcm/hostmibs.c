/*
 * File Name: hostmibs.c
 *
 * Author: Beceem Communications Pvt. Ltd
 *
 * Abstract: This file contains the routines to copy the statistics used by
 * the driver to the Host MIBS structure and giving the same to Application.
 */

#include "headers.h"

<<<<<<< HEAD
<<<<<<< HEAD
INT ProcessGetHostMibs(struct bcm_mini_adapter *Adapter, struct bcm_host_stats_mibs *pstHostMibs)
=======
INT ProcessGetHostMibs(struct bcm_mini_adapter *Adapter,
		       struct bcm_host_stats_mibs *pstHostMibs)
>>>>>>> v3.18
=======
INT ProcessGetHostMibs(struct bcm_mini_adapter *Adapter,
		       struct bcm_host_stats_mibs *pstHostMibs)
>>>>>>> v3.18
{
	struct bcm_phs_entry *pstServiceFlowEntry = NULL;
	struct bcm_phs_rule *pstPhsRule = NULL;
	struct bcm_phs_classifier_table *pstClassifierTable = NULL;
	struct bcm_phs_classifier_entry *pstClassifierRule = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
	struct bcm_phs_extension *pDeviceExtension = (struct bcm_phs_extension *) &Adapter->stBCMPhsContext;

	UINT nClassifierIndex = 0, nPhsTableIndex = 0, nSfIndex = 0, uiIndex = 0;

	if (pDeviceExtension == NULL) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, HOST_MIBS, DBG_LVL_ALL, "Invalid Device Extension\n");
=======
=======
>>>>>>> v3.18
	struct bcm_phs_extension *pDeviceExtension = &Adapter->stBCMPhsContext;
	struct bcm_mibs_host_info *host_info;
	UINT nClassifierIndex = 0;
	UINT nPhsTableIndex = 0;
	UINT nSfIndex = 0;
	UINT uiIndex = 0;

	if (pDeviceExtension == NULL) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, HOST_MIBS,
				DBG_LVL_ALL, "Invalid Device Extension\n");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return STATUS_FAILURE;
	}

	/* Copy the classifier Table */
<<<<<<< HEAD
<<<<<<< HEAD
	for (nClassifierIndex = 0; nClassifierIndex < MAX_CLASSIFIERS; nClassifierIndex++) {
		if (Adapter->astClassifierTable[nClassifierIndex].bUsed == TRUE)
			memcpy((PVOID) & pstHostMibs->
			       astClassifierTable[nClassifierIndex],
			       (PVOID) & Adapter->
			       astClassifierTable[nClassifierIndex],
=======
=======
>>>>>>> v3.18
	for (nClassifierIndex = 0; nClassifierIndex < MAX_CLASSIFIERS;
							nClassifierIndex++) {
		if (Adapter->astClassifierTable[nClassifierIndex].bUsed == TRUE)
			memcpy(&pstHostMibs->astClassifierTable[nClassifierIndex],
			       &Adapter->astClassifierTable[nClassifierIndex],
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			       sizeof(struct bcm_mibs_classifier_rule));
	}

	/* Copy the SF Table */
	for (nSfIndex = 0; nSfIndex < NO_OF_QUEUES; nSfIndex++) {
		if (Adapter->PackInfo[nSfIndex].bValid) {
<<<<<<< HEAD
<<<<<<< HEAD
			memcpy((PVOID) & pstHostMibs->astSFtable[nSfIndex],
			       (PVOID) & Adapter->PackInfo[nSfIndex],
				sizeof(struct bcm_mibs_table));
=======
			memcpy(&pstHostMibs->astSFtable[nSfIndex],
			       &Adapter->PackInfo[nSfIndex],
			       sizeof(struct bcm_mibs_table));
>>>>>>> v3.18
=======
			memcpy(&pstHostMibs->astSFtable[nSfIndex],
			       &Adapter->PackInfo[nSfIndex],
			       sizeof(struct bcm_mibs_table));
>>>>>>> v3.18
		} else {
			/* If index in not valid,
			 * don't process this for the PHS table.
			 * Go For the next entry.
			 */
			continue;
		}

		/* Retrieve the SFID Entry Index for requested Service Flow */
		if (PHS_INVALID_TABLE_INDEX ==
		    GetServiceFlowEntry(pDeviceExtension->
					pstServiceFlowPhsRulesTable,
					Adapter->PackInfo[nSfIndex].
					usVCID_Value, &pstServiceFlowEntry))

			continue;

		pstClassifierTable = pstServiceFlowEntry->pstClassifierTable;

		for (uiIndex = 0; uiIndex < MAX_PHSRULE_PER_SF; uiIndex++) {
			pstClassifierRule = &pstClassifierTable->stActivePhsRulesList[uiIndex];

			if (pstClassifierRule->bUsed) {
				pstPhsRule = pstClassifierRule->pstPhsRule;

				pstHostMibs->astPhsRulesTable[nPhsTableIndex].
				    ulSFID = Adapter->PackInfo[nSfIndex].ulSFID;

<<<<<<< HEAD
<<<<<<< HEAD
				memcpy(&pstHostMibs->
				       astPhsRulesTable[nPhsTableIndex].u8PHSI,
				       &pstPhsRule->u8PHSI, sizeof(struct bcm_phs_rule));
=======
				memcpy(&pstHostMibs->astPhsRulesTable[nPhsTableIndex].u8PHSI,
				       &pstPhsRule->u8PHSI,
				       sizeof(struct bcm_phs_rule));
>>>>>>> v3.18
=======
				memcpy(&pstHostMibs->astPhsRulesTable[nPhsTableIndex].u8PHSI,
				       &pstPhsRule->u8PHSI,
				       sizeof(struct bcm_phs_rule));
>>>>>>> v3.18
				nPhsTableIndex++;

			}

		}

	}

	/* Copy other Host Statistics parameters */
<<<<<<< HEAD
<<<<<<< HEAD
	pstHostMibs->stHostInfo.GoodTransmits = Adapter->dev->stats.tx_packets;
	pstHostMibs->stHostInfo.GoodReceives = Adapter->dev->stats.rx_packets;
	pstHostMibs->stHostInfo.CurrNumFreeDesc = atomic_read(&Adapter->CurrNumFreeTxDesc);
	pstHostMibs->stHostInfo.BEBucketSize = Adapter->BEBucketSize;
	pstHostMibs->stHostInfo.rtPSBucketSize = Adapter->rtPSBucketSize;
	pstHostMibs->stHostInfo.TimerActive = Adapter->TimerActive;
	pstHostMibs->stHostInfo.u32TotalDSD = Adapter->u32TotalDSD;

	memcpy(pstHostMibs->stHostInfo.aTxPktSizeHist, Adapter->aTxPktSizeHist, sizeof(UINT32) * MIBS_MAX_HIST_ENTRIES);
	memcpy(pstHostMibs->stHostInfo.aRxPktSizeHist, Adapter->aRxPktSizeHist, sizeof(UINT32) * MIBS_MAX_HIST_ENTRIES);
=======
=======
>>>>>>> v3.18
	host_info = &pstHostMibs->stHostInfo;
	host_info->GoodTransmits    = Adapter->dev->stats.tx_packets;
	host_info->GoodReceives	    = Adapter->dev->stats.rx_packets;
	host_info->CurrNumFreeDesc  = atomic_read(&Adapter->CurrNumFreeTxDesc);
	host_info->BEBucketSize	    = Adapter->BEBucketSize;
	host_info->rtPSBucketSize   = Adapter->rtPSBucketSize;
	host_info->TimerActive	    = Adapter->TimerActive;
	host_info->u32TotalDSD	    = Adapter->u32TotalDSD;

	memcpy(host_info->aTxPktSizeHist, Adapter->aTxPktSizeHist,
	       sizeof(UINT32) * MIBS_MAX_HIST_ENTRIES);
	memcpy(host_info->aRxPktSizeHist, Adapter->aRxPktSizeHist,
	       sizeof(UINT32) * MIBS_MAX_HIST_ENTRIES);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return STATUS_SUCCESS;
}

<<<<<<< HEAD
<<<<<<< HEAD
VOID GetDroppedAppCntrlPktMibs(struct bcm_host_stats_mibs *pstHostMibs, struct bcm_tarang_data *pTarang)
=======
VOID GetDroppedAppCntrlPktMibs(struct bcm_host_stats_mibs *pstHostMibs,
			       struct bcm_tarang_data *pTarang)
>>>>>>> v3.18
=======
VOID GetDroppedAppCntrlPktMibs(struct bcm_host_stats_mibs *pstHostMibs,
			       struct bcm_tarang_data *pTarang)
>>>>>>> v3.18
{
	memcpy(&(pstHostMibs->stDroppedAppCntrlMsgs),
	       &(pTarang->stDroppedAppCntrlMsgs),
	       sizeof(struct bcm_mibs_dropped_cntrl_msg));
}

<<<<<<< HEAD
<<<<<<< HEAD
VOID CopyMIBSExtendedSFParameters(struct bcm_mini_adapter *Adapter, struct bcm_connect_mgr_params *psfLocalSet, UINT uiSearchRuleIndex)
{
	struct bcm_mibs_parameters *t = &Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable;

	t->wmanIfSfid = psfLocalSet->u32SFID;
	t->wmanIfCmnCpsMaxSustainedRate = psfLocalSet->u32MaxSustainedTrafficRate;
=======
=======
>>>>>>> v3.18
VOID CopyMIBSExtendedSFParameters(struct bcm_mini_adapter *Adapter,
				  struct bcm_connect_mgr_params *psfLocalSet,
				  UINT uiSearchRuleIndex)
{
	struct bcm_mibs_parameters *t =
		&Adapter->PackInfo[uiSearchRuleIndex].stMibsExtServiceFlowTable;

	t->wmanIfSfid = psfLocalSet->u32SFID;
	t->wmanIfCmnCpsMaxSustainedRate =
		psfLocalSet->u32MaxSustainedTrafficRate;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	t->wmanIfCmnCpsMaxTrafficBurst = psfLocalSet->u32MaxTrafficBurst;
	t->wmanIfCmnCpsMinReservedRate = psfLocalSet->u32MinReservedTrafficRate;
	t->wmanIfCmnCpsToleratedJitter = psfLocalSet->u32ToleratedJitter;
	t->wmanIfCmnCpsMaxLatency = psfLocalSet->u32MaximumLatency;
<<<<<<< HEAD
<<<<<<< HEAD
	t->wmanIfCmnCpsFixedVsVariableSduInd = psfLocalSet->u8FixedLengthVSVariableLengthSDUIndicator;
	t->wmanIfCmnCpsFixedVsVariableSduInd = ntohl(t->wmanIfCmnCpsFixedVsVariableSduInd);
	t->wmanIfCmnCpsSduSize = psfLocalSet->u8SDUSize;
	t->wmanIfCmnCpsSduSize = ntohl(t->wmanIfCmnCpsSduSize);
	t->wmanIfCmnCpsSfSchedulingType = psfLocalSet->u8ServiceFlowSchedulingType;
	t->wmanIfCmnCpsSfSchedulingType = ntohl(t->wmanIfCmnCpsSfSchedulingType);
=======
=======
>>>>>>> v3.18
	t->wmanIfCmnCpsFixedVsVariableSduInd =
		psfLocalSet->u8FixedLengthVSVariableLengthSDUIndicator;
	t->wmanIfCmnCpsFixedVsVariableSduInd =
		ntohl(t->wmanIfCmnCpsFixedVsVariableSduInd);
	t->wmanIfCmnCpsSduSize = psfLocalSet->u8SDUSize;
	t->wmanIfCmnCpsSduSize = ntohl(t->wmanIfCmnCpsSduSize);
	t->wmanIfCmnCpsSfSchedulingType =
		psfLocalSet->u8ServiceFlowSchedulingType;
	t->wmanIfCmnCpsSfSchedulingType =
		ntohl(t->wmanIfCmnCpsSfSchedulingType);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	t->wmanIfCmnCpsArqEnable = psfLocalSet->u8ARQEnable;
	t->wmanIfCmnCpsArqEnable = ntohl(t->wmanIfCmnCpsArqEnable);
	t->wmanIfCmnCpsArqWindowSize = ntohs(psfLocalSet->u16ARQWindowSize);
	t->wmanIfCmnCpsArqWindowSize = ntohl(t->wmanIfCmnCpsArqWindowSize);
<<<<<<< HEAD
<<<<<<< HEAD
	t->wmanIfCmnCpsArqBlockLifetime = ntohs(psfLocalSet->u16ARQBlockLifeTime);
	t->wmanIfCmnCpsArqBlockLifetime = ntohl(t->wmanIfCmnCpsArqBlockLifetime);
	t->wmanIfCmnCpsArqSyncLossTimeout = ntohs(psfLocalSet->u16ARQSyncLossTimeOut);
	t->wmanIfCmnCpsArqSyncLossTimeout = ntohl(t->wmanIfCmnCpsArqSyncLossTimeout);
	t->wmanIfCmnCpsArqDeliverInOrder = psfLocalSet->u8ARQDeliverInOrder;
	t->wmanIfCmnCpsArqDeliverInOrder = ntohl(t->wmanIfCmnCpsArqDeliverInOrder);
	t->wmanIfCmnCpsArqRxPurgeTimeout = ntohs(psfLocalSet->u16ARQRxPurgeTimeOut);
	t->wmanIfCmnCpsArqRxPurgeTimeout = ntohl(t->wmanIfCmnCpsArqRxPurgeTimeout);
=======
=======
>>>>>>> v3.18
	t->wmanIfCmnCpsArqBlockLifetime =
		ntohs(psfLocalSet->u16ARQBlockLifeTime);
	t->wmanIfCmnCpsArqBlockLifetime =
		ntohl(t->wmanIfCmnCpsArqBlockLifetime);
	t->wmanIfCmnCpsArqSyncLossTimeout =
		ntohs(psfLocalSet->u16ARQSyncLossTimeOut);
	t->wmanIfCmnCpsArqSyncLossTimeout =
		ntohl(t->wmanIfCmnCpsArqSyncLossTimeout);
	t->wmanIfCmnCpsArqDeliverInOrder = psfLocalSet->u8ARQDeliverInOrder;
	t->wmanIfCmnCpsArqDeliverInOrder =
		ntohl(t->wmanIfCmnCpsArqDeliverInOrder);
	t->wmanIfCmnCpsArqRxPurgeTimeout =
		ntohs(psfLocalSet->u16ARQRxPurgeTimeOut);
	t->wmanIfCmnCpsArqRxPurgeTimeout =
		ntohl(t->wmanIfCmnCpsArqRxPurgeTimeout);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	t->wmanIfCmnCpsArqBlockSize = ntohs(psfLocalSet->u16ARQBlockSize);
	t->wmanIfCmnCpsArqBlockSize = ntohl(t->wmanIfCmnCpsArqBlockSize);
	t->wmanIfCmnCpsReqTxPolicy = psfLocalSet->u8RequesttransmissionPolicy;
	t->wmanIfCmnCpsReqTxPolicy = ntohl(t->wmanIfCmnCpsReqTxPolicy);
	t->wmanIfCmnSfCsSpecification = psfLocalSet->u8CSSpecification;
	t->wmanIfCmnSfCsSpecification = ntohl(t->wmanIfCmnSfCsSpecification);
	t->wmanIfCmnCpsTargetSaid = ntohs(psfLocalSet->u16TargetSAID);
	t->wmanIfCmnCpsTargetSaid = ntohl(t->wmanIfCmnCpsTargetSaid);

}
