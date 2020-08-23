#include "headers.h"

<<<<<<< HEAD
static UINT CreateSFToClassifierRuleMapping(B_UINT16 uiVcid, B_UINT16 uiClsId, struct bcm_phs_table *psServiceFlowTable, struct bcm_phs_rule *psPhsRule, B_UINT8 u8AssociatedPHSI);

static UINT CreateClassiferToPHSRuleMapping(B_UINT16 uiVcid, B_UINT16  uiClsId, struct bcm_phs_entry *pstServiceFlowEntry, struct bcm_phs_rule *psPhsRule, B_UINT8 u8AssociatedPHSI);

static UINT CreateClassifierPHSRule(B_UINT16  uiClsId, struct bcm_phs_classifier_table *psaClassifiertable, struct bcm_phs_rule *psPhsRule, enum bcm_phs_classifier_context eClsContext, B_UINT8 u8AssociatedPHSI);

static UINT UpdateClassifierPHSRule(B_UINT16 uiClsId, struct bcm_phs_classifier_entry *pstClassifierEntry, struct bcm_phs_classifier_table *psaClassifiertable, struct bcm_phs_rule *psPhsRule, B_UINT8 u8AssociatedPHSI);

static BOOLEAN ValidatePHSRuleComplete(struct bcm_phs_rule *psPhsRule);

static BOOLEAN DerefPhsRule(B_UINT16 uiClsId, struct bcm_phs_classifier_table *psaClassifiertable, struct bcm_phs_rule *pstPhsRule);

static UINT GetClassifierEntry(struct bcm_phs_classifier_table *pstClassifierTable, B_UINT32 uiClsid, enum bcm_phs_classifier_context eClsContext, struct bcm_phs_classifier_entry **ppstClassifierEntry);

static UINT GetPhsRuleEntry(struct bcm_phs_classifier_table *pstClassifierTable, B_UINT32 uiPHSI, enum bcm_phs_classifier_context eClsContext, struct bcm_phs_rule **ppstPhsRule);

static void free_phs_serviceflow_rules(struct bcm_phs_table *psServiceFlowRulesTable);

static int phs_compress(struct bcm_phs_rule *phs_members, unsigned char *in_buf,
			unsigned char *out_buf, unsigned int *header_size, UINT *new_header_size);

static int verify_suppress_phsf(unsigned char *in_buffer, unsigned char *out_buffer,
				unsigned char *phsf, unsigned char *phsm, unsigned int phss, unsigned int phsv, UINT *new_header_size);

static int phs_decompress(unsigned char *in_buf, unsigned char *out_buf,
			struct bcm_phs_rule *phs_rules, UINT *header_size);

static ULONG PhsCompress(void *pvContext,
			B_UINT16 uiVcid,
			B_UINT16 uiClsId,
			void *pvInputBuffer,
			void *pvOutputBuffer,
			UINT *pOldHeaderSize,
			UINT *pNewHeaderSize);

static ULONG PhsDeCompress(void *pvContext,
			B_UINT16 uiVcid,
			void *pvInputBuffer,
			void *pvOutputBuffer,
			UINT *pInHeaderSize,
			UINT *pOutHeaderSize);
=======
static UINT CreateSFToClassifierRuleMapping(B_UINT16 uiVcid,
					    B_UINT16 uiClsId,
					    struct bcm_phs_table *psServiceFlowTable,
					    struct bcm_phs_rule *psPhsRule,
					    B_UINT8 u8AssociatedPHSI);

static UINT CreateClassiferToPHSRuleMapping(B_UINT16 uiVcid,
					    B_UINT16  uiClsId,
					    struct bcm_phs_entry *pstServiceFlowEntry,
					    struct bcm_phs_rule *psPhsRule,
					    B_UINT8 u8AssociatedPHSI);

static UINT CreateClassifierPHSRule(B_UINT16  uiClsId,
				    struct bcm_phs_classifier_table *psaClassifiertable,
				    struct bcm_phs_rule *psPhsRule,
				    enum bcm_phs_classifier_context eClsContext,
				    B_UINT8 u8AssociatedPHSI);

static UINT UpdateClassifierPHSRule(B_UINT16 uiClsId,
				    struct bcm_phs_classifier_entry *pstClassifierEntry,
				    struct bcm_phs_classifier_table *psaClassifiertable,
				    struct bcm_phs_rule *psPhsRule,
				    B_UINT8 u8AssociatedPHSI);

static bool ValidatePHSRuleComplete(const struct bcm_phs_rule *psPhsRule);

static bool DerefPhsRule(B_UINT16 uiClsId,
			 struct bcm_phs_classifier_table *psaClassifiertable,
			 struct bcm_phs_rule *pstPhsRule);

static UINT GetClassifierEntry(struct bcm_phs_classifier_table *pstClassifierTable,
			       B_UINT32 uiClsid,
			       enum bcm_phs_classifier_context eClsContext,
			       struct bcm_phs_classifier_entry **ppstClassifierEntry);

static UINT GetPhsRuleEntry(struct bcm_phs_classifier_table *pstClassifierTable,
			    B_UINT32 uiPHSI,
			    enum bcm_phs_classifier_context eClsContext,
			    struct bcm_phs_rule **ppstPhsRule);

static void free_phs_serviceflow_rules(struct bcm_phs_table *psServiceFlowRulesTable);

static int phs_compress(struct bcm_phs_rule *phs_members,
			unsigned char *in_buf,
			unsigned char *out_buf,
			unsigned int *header_size,
			UINT *new_header_size);

static int verify_suppress_phsf(unsigned char *in_buffer,
				unsigned char *out_buffer,
				unsigned char *phsf,
				unsigned char *phsm,
				unsigned int phss,
				unsigned int phsv,
				UINT *new_header_size);

static int phs_decompress(unsigned char *in_buf,
			  unsigned char *out_buf,
			  struct bcm_phs_rule *phs_rules,
			  UINT *header_size);

static ULONG PhsCompress(void *pvContext,
			 B_UINT16 uiVcid,
			 B_UINT16 uiClsId,
			 void *pvInputBuffer,
			 void *pvOutputBuffer,
			 UINT *pOldHeaderSize,
			 UINT *pNewHeaderSize);

static ULONG PhsDeCompress(void *pvContext,
			   B_UINT16 uiVcid,
			   void *pvInputBuffer,
			   void *pvOutputBuffer,
			   UINT *pInHeaderSize,
			   UINT *pOutHeaderSize);
>>>>>>> v3.18

#define IN
#define OUT

/*
 * Function: PHSTransmit
 * Description:	This routine handle PHS(Payload Header Suppression for Tx path.
 *	It extracts a fragment of the NDIS_PACKET containing the header
 *	to be suppressed. It then suppresses the header by invoking PHS exported compress routine.
 *	The header data after suppression is copied back to the NDIS_PACKET.
 *
 * Input parameters: IN struct bcm_mini_adapter *Adapter         - Miniport Adapter Context
 *	IN Packet - NDIS packet containing data to be transmitted
 *	IN USHORT Vcid - vcid pertaining to connection on which the packet is being sent.Used to
 *		identify PHS rule to be applied.
 *	B_UINT16 uiClassifierRuleID - Classifier Rule ID
 *	BOOLEAN bHeaderSuppressionEnabled - indicates if header suprression is enabled for SF.
 *
 * Return:	STATUS_SUCCESS - If the send was successful.
 *	Other  - If an error occurred.
 */

int PHSTransmit(struct bcm_mini_adapter *Adapter,
		struct sk_buff **pPacket,
		USHORT Vcid,
		B_UINT16 uiClassifierRuleID,
<<<<<<< HEAD
		BOOLEAN bHeaderSuppressionEnabled,
=======
		bool bHeaderSuppressionEnabled,
>>>>>>> v3.18
		UINT *PacketLen,
		UCHAR bEthCSSupport)
{
	/* PHS Sepcific */
	UINT unPHSPktHdrBytesCopied = 0;
	UINT unPhsOldHdrSize = 0;
	UINT unPHSNewPktHeaderLen = 0;
	/* Pointer to PHS IN Hdr Buffer */
<<<<<<< HEAD
	PUCHAR pucPHSPktHdrInBuf = Adapter->stPhsTxContextInfo.ucaHdrSuppressionInBuf;
	/* Pointer to PHS OUT Hdr Buffer */
	PUCHAR pucPHSPktHdrOutBuf = Adapter->stPhsTxContextInfo.ucaHdrSuppressionOutBuf;
	UINT usPacketType;
	UINT BytesToRemove = 0;
	BOOLEAN bPHSI = 0;
=======
	PUCHAR pucPHSPktHdrInBuf =
		Adapter->stPhsTxContextInfo.ucaHdrSuppressionInBuf;
	/* Pointer to PHS OUT Hdr Buffer */
	PUCHAR pucPHSPktHdrOutBuf =
		Adapter->stPhsTxContextInfo.ucaHdrSuppressionOutBuf;
	UINT usPacketType;
	UINT BytesToRemove = 0;
	bool bPHSI = 0;
>>>>>>> v3.18
	LONG ulPhsStatus = 0;
	UINT numBytesCompressed = 0;
	struct sk_buff *newPacket = NULL;
	struct sk_buff *Packet = *pPacket;

<<<<<<< HEAD
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL, "In PHSTransmit");
=======
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL,
			"In PHSTransmit");
>>>>>>> v3.18

	if (!bEthCSSupport)
		BytesToRemove = ETH_HLEN;
	/*
	 * Accumulate the header upto the size we support suppression
	 * from NDIS packet
	 */

	usPacketType = ((struct ethhdr *)(Packet->data))->h_proto;

	pucPHSPktHdrInBuf = Packet->data + BytesToRemove;
	/* considering data after ethernet header */
	if ((*PacketLen - BytesToRemove) < MAX_PHS_LENGTHS)
		unPHSPktHdrBytesCopied = (*PacketLen - BytesToRemove);
	else
		unPHSPktHdrBytesCopied = MAX_PHS_LENGTHS;

	if ((unPHSPktHdrBytesCopied > 0) &&
		(unPHSPktHdrBytesCopied <= MAX_PHS_LENGTHS)) {

		/*
<<<<<<< HEAD
		 * Step 2 Suppress Header using PHS and fill into intermediate ucaPHSPktHdrOutBuf.
		 * Suppress only if IP Header and PHS Enabled For the Service Flow
=======
		 * Step 2 Suppress Header using PHS and fill into intermediate
		 * ucaPHSPktHdrOutBuf.
		 * Suppress only if IP Header and PHS Enabled For the
		 * Service Flow
>>>>>>> v3.18
		 */
		if (((usPacketType == ETHERNET_FRAMETYPE_IPV4) ||
				(usPacketType == ETHERNET_FRAMETYPE_IPV6)) &&
			(bHeaderSuppressionEnabled)) {

<<<<<<< HEAD
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL, "\nTrying to PHS Compress Using Classifier rule 0x%X", uiClassifierRuleID);
			unPHSNewPktHeaderLen = unPHSPktHdrBytesCopied;
			ulPhsStatus = PhsCompress(&Adapter->stBCMPhsContext,
						Vcid,
						uiClassifierRuleID,
						pucPHSPktHdrInBuf,
						pucPHSPktHdrOutBuf,
						&unPhsOldHdrSize,
						&unPHSNewPktHeaderLen);
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL, "\nPHS Old header Size : %d New Header Size  %d\n", unPhsOldHdrSize, unPHSNewPktHeaderLen);
=======
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND,
					DBG_LVL_ALL,
					"\nTrying to PHS Compress Using Classifier rule 0x%X",
					uiClassifierRuleID);
			unPHSNewPktHeaderLen = unPHSPktHdrBytesCopied;
			ulPhsStatus = PhsCompress(&Adapter->stBCMPhsContext,
						  Vcid,
						  uiClassifierRuleID,
						  pucPHSPktHdrInBuf,
						  pucPHSPktHdrOutBuf,
						  &unPhsOldHdrSize,
						  &unPHSNewPktHeaderLen);
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND,
					DBG_LVL_ALL,
					"\nPHS Old header Size : %d New Header Size  %d\n",
					unPhsOldHdrSize, unPHSNewPktHeaderLen);
>>>>>>> v3.18

			if (unPHSNewPktHeaderLen == unPhsOldHdrSize) {

				if (ulPhsStatus == STATUS_PHS_COMPRESSED)
					bPHSI = *pucPHSPktHdrOutBuf;

				ulPhsStatus = STATUS_PHS_NOCOMPRESSION;
			}

			if (ulPhsStatus == STATUS_PHS_COMPRESSED) {

<<<<<<< HEAD
				BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL, "PHS Sending packet Compressed");

				if (skb_cloned(Packet)) {
					newPacket = skb_copy(Packet, GFP_ATOMIC);
=======
				BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS,
						PHS_SEND, DBG_LVL_ALL,
						"PHS Sending packet Compressed");

				if (skb_cloned(Packet)) {
					newPacket =
						skb_copy(Packet, GFP_ATOMIC);
>>>>>>> v3.18

					if (newPacket == NULL)
						return STATUS_FAILURE;

					dev_kfree_skb(Packet);
					*pPacket = Packet = newPacket;
<<<<<<< HEAD
					pucPHSPktHdrInBuf = Packet->data  + BytesToRemove;
				}

				numBytesCompressed = unPhsOldHdrSize - (unPHSNewPktHeaderLen + PHSI_LEN);

				memcpy(pucPHSPktHdrInBuf + numBytesCompressed, pucPHSPktHdrOutBuf, unPHSNewPktHeaderLen + PHSI_LEN);
				memcpy(Packet->data + numBytesCompressed, Packet->data, BytesToRemove);
=======
					pucPHSPktHdrInBuf =
						Packet->data + BytesToRemove;
				}

				numBytesCompressed = unPhsOldHdrSize -
					(unPHSNewPktHeaderLen + PHSI_LEN);

				memcpy(pucPHSPktHdrInBuf + numBytesCompressed,
				       pucPHSPktHdrOutBuf,
				       unPHSNewPktHeaderLen + PHSI_LEN);
				memcpy(Packet->data + numBytesCompressed,
				       Packet->data, BytesToRemove);
>>>>>>> v3.18
				skb_pull(Packet, numBytesCompressed);

				return STATUS_SUCCESS;
			} else {
<<<<<<< HEAD
				/* if one byte headroom is not available, increase it through skb_cow */
				if (!(skb_headroom(Packet) > 0)) {

					if (skb_cow(Packet, 1)) {
						BCM_DEBUG_PRINT(Adapter, DBG_TYPE_PRINTK, 0, 0, "SKB Cow Failed\n");
=======
				/* if one byte headroom is not available,
				 * increase it through skb_cow
				 */
				if (!(skb_headroom(Packet) > 0)) {

					if (skb_cow(Packet, 1)) {
						BCM_DEBUG_PRINT(Adapter,
								DBG_TYPE_PRINTK,
								0, 0,
								"SKB Cow Failed\n");
>>>>>>> v3.18
						return STATUS_FAILURE;
					}
				}
				skb_push(Packet, 1);

				/*
				 * CAUTION: The MAC Header is getting corrupted
				 * here for IP CS - can be saved by copying 14
				 * Bytes.  not needed .... hence corrupting it.
				 */
				*(Packet->data + BytesToRemove) = bPHSI;
				return STATUS_SUCCESS;
			}
		} else {

			if (!bHeaderSuppressionEnabled)
<<<<<<< HEAD
				BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL, "\nHeader Suppression Disabled For SF: No PHS\n");
=======
				BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS,
						PHS_SEND, DBG_LVL_ALL,
						"\nHeader Suppression Disabled For SF: No PHS\n");
>>>>>>> v3.18

			return STATUS_SUCCESS;
		}
	}

<<<<<<< HEAD
	/* BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL,"PHSTransmit : Dumping data packet After PHS"); */
=======
	/* BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL,
	 * "PHSTransmit : Dumping data packet After PHS"); */
>>>>>>> v3.18
	return STATUS_SUCCESS;
}

int PHSReceive(struct bcm_mini_adapter *Adapter,
<<<<<<< HEAD
	USHORT usVcid,
	struct sk_buff *packet,
	UINT *punPacketLen,
	UCHAR *pucEthernetHdr,
	UINT bHeaderSuppressionEnabled)
=======
	       USHORT usVcid,
	       struct sk_buff *packet,
	       UINT *punPacketLen,
	       UCHAR *pucEthernetHdr,
	       UINT bHeaderSuppressionEnabled)
>>>>>>> v3.18
{
	u32 nStandardPktHdrLen = 0;
	u32 nTotalsuppressedPktHdrBytes = 0;
	int ulPhsStatus	= 0;
	PUCHAR pucInBuff = NULL;
	UINT TotalBytesAdded = 0;

	if (!bHeaderSuppressionEnabled) {
<<<<<<< HEAD
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_RECEIVE, DBG_LVL_ALL, "\nPhs Disabled for incoming packet");
=======
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_RECEIVE,
				DBG_LVL_ALL,
				"\nPhs Disabled for incoming packet");
>>>>>>> v3.18
		return ulPhsStatus;
	}

	pucInBuff = packet->data;

	/* Restore PHS suppressed header */
	nStandardPktHdrLen = packet->len;
	ulPhsStatus = PhsDeCompress(&Adapter->stBCMPhsContext,
<<<<<<< HEAD
				usVcid,
				pucInBuff,
				Adapter->ucaPHSPktRestoreBuf,
				&nTotalsuppressedPktHdrBytes,
				&nStandardPktHdrLen);

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_RECEIVE, DBG_LVL_ALL, "\nSuppressed PktHdrLen : 0x%x Restored PktHdrLen : 0x%x",
=======
				    usVcid,
				    pucInBuff,
				    Adapter->ucaPHSPktRestoreBuf,
				    &nTotalsuppressedPktHdrBytes,
				    &nStandardPktHdrLen);

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_RECEIVE, DBG_LVL_ALL,
			"\nSuppressed PktHdrLen : 0x%x Restored PktHdrLen : 0x%x",
>>>>>>> v3.18
			nTotalsuppressedPktHdrBytes, nStandardPktHdrLen);

	if (ulPhsStatus != STATUS_PHS_COMPRESSED) {
		skb_pull(packet, 1);
		return STATUS_SUCCESS;
	} else {
<<<<<<< HEAD
		TotalBytesAdded = nStandardPktHdrLen - nTotalsuppressedPktHdrBytes - PHSI_LEN;
=======
		TotalBytesAdded = nStandardPktHdrLen -
			nTotalsuppressedPktHdrBytes - PHSI_LEN;
>>>>>>> v3.18

		if (TotalBytesAdded) {
			if (skb_headroom(packet) >= (SKB_RESERVE_ETHERNET_HEADER + TotalBytesAdded))
				skb_push(packet, TotalBytesAdded);
			else {
				if (skb_cow(packet, skb_headroom(packet) + TotalBytesAdded)) {
<<<<<<< HEAD
					BCM_DEBUG_PRINT(Adapter, DBG_TYPE_PRINTK, 0, 0, "cow failed in receive\n");
=======
					BCM_DEBUG_PRINT(Adapter,
							DBG_TYPE_PRINTK, 0, 0,
							"cow failed in receive\n");
>>>>>>> v3.18
					return STATUS_FAILURE;
				}

				skb_push(packet, TotalBytesAdded);
			}
		}

<<<<<<< HEAD
		memcpy(packet->data, Adapter->ucaPHSPktRestoreBuf, nStandardPktHdrLen);
=======
		memcpy(packet->data, Adapter->ucaPHSPktRestoreBuf,
		       nStandardPktHdrLen);
>>>>>>> v3.18
	}

	return STATUS_SUCCESS;
}

void DumpFullPacket(UCHAR *pBuf, UINT nPktLen)
{
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);

<<<<<<< HEAD
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, "Dumping Data Packet");
	BCM_DEBUG_PRINT_BUFFER(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL, pBuf, nPktLen);
=======
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,
			"Dumping Data Packet");
	BCM_DEBUG_PRINT_BUFFER(Adapter, DBG_TYPE_TX, IPV4_DBG, DBG_LVL_ALL,
			       pBuf, nPktLen);
>>>>>>> v3.18
}

/*
 * Procedure:   phs_init
 *
<<<<<<< HEAD
 * Description: This routine is responsible for allocating memory for classifier and
 * PHS rules.
 *
 * Arguments:
 * pPhsdeviceExtension - ptr to Device extension containing PHS Classifier rules and PHS Rules , RX, TX buffer etc
=======
 * Description: This routine is responsible for allocating memory for classifier
 * and PHS rules.
 *
 * Arguments:
 * pPhsdeviceExtension - ptr to Device extension containing PHS Classifier rules
 * and PHS Rules , RX, TX buffer etc
>>>>>>> v3.18
 *
 * Returns:
 * TRUE(1)	-If allocation of memory was successful.
 * FALSE	-If allocation of memory fails.
 */
<<<<<<< HEAD
int phs_init(struct bcm_phs_extension *pPhsdeviceExtension, struct bcm_mini_adapter *Adapter)
=======
int phs_init(struct bcm_phs_extension *pPhsdeviceExtension,
	     struct bcm_mini_adapter *Adapter)
>>>>>>> v3.18
{
	int i;
	struct bcm_phs_table *pstServiceFlowTable;

<<<<<<< HEAD
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "\nPHS:phs_init function");
=======
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL,
			"\nPHS:phs_init function");
>>>>>>> v3.18

	if (pPhsdeviceExtension->pstServiceFlowPhsRulesTable)
		return -EINVAL;

<<<<<<< HEAD
	pPhsdeviceExtension->pstServiceFlowPhsRulesTable = kzalloc(sizeof(struct bcm_phs_table), GFP_KERNEL);

	if (!pPhsdeviceExtension->pstServiceFlowPhsRulesTable) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "\nAllocation ServiceFlowPhsRulesTable failed");
=======
	pPhsdeviceExtension->pstServiceFlowPhsRulesTable =
		kzalloc(sizeof(struct bcm_phs_table), GFP_KERNEL);

	if (!pPhsdeviceExtension->pstServiceFlowPhsRulesTable) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH,
				DBG_LVL_ALL,
				"\nAllocation ServiceFlowPhsRulesTable failed");
>>>>>>> v3.18
		return -ENOMEM;
	}

	pstServiceFlowTable = pPhsdeviceExtension->pstServiceFlowPhsRulesTable;
	for (i = 0; i < MAX_SERVICEFLOWS; i++) {
<<<<<<< HEAD
		struct bcm_phs_entry sServiceFlow = pstServiceFlowTable->stSFList[i];
		sServiceFlow.pstClassifierTable = kzalloc(sizeof(struct bcm_phs_classifier_table), GFP_KERNEL);
		if (!sServiceFlow.pstClassifierTable) {
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "\nAllocation failed");
=======
		struct bcm_phs_entry sServiceFlow =
			pstServiceFlowTable->stSFList[i];
		sServiceFlow.pstClassifierTable =
			kzalloc(sizeof(struct bcm_phs_classifier_table),
				GFP_KERNEL);
		if (!sServiceFlow.pstClassifierTable) {
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH,
					DBG_LVL_ALL, "\nAllocation failed");
>>>>>>> v3.18
			free_phs_serviceflow_rules(pPhsdeviceExtension->pstServiceFlowPhsRulesTable);
			pPhsdeviceExtension->pstServiceFlowPhsRulesTable = NULL;
			return -ENOMEM;
		}
	}

	pPhsdeviceExtension->CompressedTxBuffer = kmalloc(PHS_BUFFER_SIZE, GFP_KERNEL);
	if (pPhsdeviceExtension->CompressedTxBuffer == NULL) {
<<<<<<< HEAD
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "\nAllocation failed");
=======
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH,
				DBG_LVL_ALL, "\nAllocation failed");
>>>>>>> v3.18
		free_phs_serviceflow_rules(pPhsdeviceExtension->pstServiceFlowPhsRulesTable);
		pPhsdeviceExtension->pstServiceFlowPhsRulesTable = NULL;
		return -ENOMEM;
	}

<<<<<<< HEAD
	pPhsdeviceExtension->UnCompressedRxBuffer = kmalloc(PHS_BUFFER_SIZE, GFP_KERNEL);
	if (pPhsdeviceExtension->UnCompressedRxBuffer == NULL) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "\nAllocation failed");
=======
	pPhsdeviceExtension->UnCompressedRxBuffer =
		kmalloc(PHS_BUFFER_SIZE, GFP_KERNEL);
	if (pPhsdeviceExtension->UnCompressedRxBuffer == NULL) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH,
				DBG_LVL_ALL, "\nAllocation failed");
>>>>>>> v3.18
		kfree(pPhsdeviceExtension->CompressedTxBuffer);
		free_phs_serviceflow_rules(pPhsdeviceExtension->pstServiceFlowPhsRulesTable);
		pPhsdeviceExtension->pstServiceFlowPhsRulesTable = NULL;
		return -ENOMEM;
	}

<<<<<<< HEAD
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "\n phs_init Successful");
=======
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL,
			"\n phs_init Successful");
>>>>>>> v3.18
	return STATUS_SUCCESS;
}

int PhsCleanup(IN struct bcm_phs_extension *pPHSDeviceExt)
{
	if (pPHSDeviceExt->pstServiceFlowPhsRulesTable) {
		free_phs_serviceflow_rules(pPHSDeviceExt->pstServiceFlowPhsRulesTable);
		pPHSDeviceExt->pstServiceFlowPhsRulesTable = NULL;
	}

	kfree(pPHSDeviceExt->CompressedTxBuffer);
	pPHSDeviceExt->CompressedTxBuffer = NULL;

	kfree(pPHSDeviceExt->UnCompressedRxBuffer);
	pPHSDeviceExt->UnCompressedRxBuffer = NULL;

	return 0;
}

/*
 * PHS functions
 * PhsUpdateClassifierRule
 *
 * Routine Description:
 *   Exported function to add or modify a PHS Rule.
 *
 * Arguments:
 *	IN void* pvContext - PHS Driver Specific Context
 *	IN B_UINT16 uiVcid    - The Service Flow ID for which the PHS rule applies
 *	IN B_UINT16  uiClsId   - The Classifier ID within the Service Flow for which the PHS rule applies.
 *	IN struct bcm_phs_rule *psPhsRule - The PHS Rule strcuture to be added to the PHS Rule table.
 *
 * Return Value:
 *
 * 0 if successful,
 * >0 Error.
 */
ULONG PhsUpdateClassifierRule(IN void *pvContext,
<<<<<<< HEAD
			IN B_UINT16 uiVcid ,
			IN B_UINT16 uiClsId   ,
			IN struct bcm_phs_rule *psPhsRule,
			IN B_UINT8 u8AssociatedPHSI)
=======
			      IN B_UINT16 uiVcid ,
			      IN B_UINT16 uiClsId   ,
			      IN struct bcm_phs_rule *psPhsRule,
			      IN B_UINT8 u8AssociatedPHSI)
>>>>>>> v3.18
{
	ULONG lStatus = 0;
	UINT nSFIndex = 0;
	struct bcm_phs_entry *pstServiceFlowEntry = NULL;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
<<<<<<< HEAD
	struct bcm_phs_extension *pDeviceExtension = (struct bcm_phs_extension *)pvContext;

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "PHS With Corr2 Changes\n");

	if (pDeviceExtension == NULL) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "Invalid Device Extension\n");
=======
	struct bcm_phs_extension *pDeviceExtension =
		(struct bcm_phs_extension *)pvContext;

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL,
			"PHS With Corr2 Changes\n");

	if (pDeviceExtension == NULL) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH,
				DBG_LVL_ALL, "Invalid Device Extension\n");
>>>>>>> v3.18
		return ERR_PHS_INVALID_DEVICE_EXETENSION;
	}

	if (u8AssociatedPHSI == 0)
		return ERR_PHS_INVALID_PHS_RULE;

	/* Retrieve the SFID Entry Index for requested Service Flow */
	nSFIndex = GetServiceFlowEntry(pDeviceExtension->pstServiceFlowPhsRulesTable,
<<<<<<< HEAD
				uiVcid, &pstServiceFlowEntry);
=======
				       uiVcid, &pstServiceFlowEntry);
>>>>>>> v3.18

	if (nSFIndex == PHS_INVALID_TABLE_INDEX) {
		/* This is a new SF. Create a mapping entry for this */
		lStatus = CreateSFToClassifierRuleMapping(uiVcid, uiClsId,
<<<<<<< HEAD
							pDeviceExtension->pstServiceFlowPhsRulesTable, psPhsRule, u8AssociatedPHSI);
=======
							  pDeviceExtension->pstServiceFlowPhsRulesTable,
							  psPhsRule,
							  u8AssociatedPHSI);
>>>>>>> v3.18
		return lStatus;
	}

	/* SF already Exists Add PHS Rule to existing SF */
	lStatus = CreateClassiferToPHSRuleMapping(uiVcid, uiClsId,
<<<<<<< HEAD
						pstServiceFlowEntry, psPhsRule, u8AssociatedPHSI);
=======
						  pstServiceFlowEntry,
						  psPhsRule,
						  u8AssociatedPHSI);
>>>>>>> v3.18

	return lStatus;
}

/*
 * PhsDeletePHSRule
 *
 * Routine Description:
 *   Deletes the specified phs Rule within Vcid
 *
 * Arguments:
 *	IN void* pvContext - PHS Driver Specific Context
 *	IN B_UINT16  uiVcid    - The Service Flow ID for which the PHS rule applies
 *	IN B_UINT8  u8PHSI   - the PHS Index identifying PHS rule to be deleted.
 *
 * Return Value:
 *
 * 0 if successful,
 * >0 Error.
 */
<<<<<<< HEAD
ULONG PhsDeletePHSRule(IN void *pvContext, IN B_UINT16 uiVcid, IN B_UINT8 u8PHSI)
{
	ULONG lStatus = 0;
=======
ULONG PhsDeletePHSRule(IN void *pvContext,
		       IN B_UINT16 uiVcid,
		       IN B_UINT8 u8PHSI)
{
>>>>>>> v3.18
	UINT nSFIndex = 0, nClsidIndex = 0;
	struct bcm_phs_entry *pstServiceFlowEntry = NULL;
	struct bcm_phs_classifier_table *pstClassifierRulesTable = NULL;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
	struct bcm_phs_extension *pDeviceExtension = (struct bcm_phs_extension *)pvContext;
<<<<<<< HEAD

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "======>\n");

	if (pDeviceExtension) {
		/* Retrieve the SFID Entry Index for requested Service Flow */
		nSFIndex = GetServiceFlowEntry(pDeviceExtension->pstServiceFlowPhsRulesTable, uiVcid, &pstServiceFlowEntry);

		if (nSFIndex == PHS_INVALID_TABLE_INDEX) {
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "SFID Match Failed\n");
=======
	struct bcm_phs_classifier_entry *curr_entry;

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL,
			"======>\n");

	if (pDeviceExtension) {
		/* Retrieve the SFID Entry Index for requested Service Flow */
		nSFIndex = GetServiceFlowEntry(pDeviceExtension->pstServiceFlowPhsRulesTable,
					       uiVcid, &pstServiceFlowEntry);

		if (nSFIndex == PHS_INVALID_TABLE_INDEX) {
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH,
					DBG_LVL_ALL, "SFID Match Failed\n");
>>>>>>> v3.18
			return ERR_SF_MATCH_FAIL;
		}

		pstClassifierRulesTable = pstServiceFlowEntry->pstClassifierTable;
		if (pstClassifierRulesTable) {
			for (nClsidIndex = 0; nClsidIndex < MAX_PHSRULE_PER_SF; nClsidIndex++) {
<<<<<<< HEAD
				if (pstClassifierRulesTable->stActivePhsRulesList[nClsidIndex].bUsed && pstClassifierRulesTable->stActivePhsRulesList[nClsidIndex].pstPhsRule) {
					if (pstClassifierRulesTable->stActivePhsRulesList[nClsidIndex].pstPhsRule->u8PHSI == u8PHSI) {

						if (pstClassifierRulesTable->stActivePhsRulesList[nClsidIndex].pstPhsRule->u8RefCnt)
							pstClassifierRulesTable->stActivePhsRulesList[nClsidIndex].pstPhsRule->u8RefCnt--;

						if (0 == pstClassifierRulesTable->stActivePhsRulesList[nClsidIndex].pstPhsRule->u8RefCnt)
							kfree(pstClassifierRulesTable->stActivePhsRulesList[nClsidIndex].pstPhsRule);

						memset(&pstClassifierRulesTable->stActivePhsRulesList[nClsidIndex], 0,
							sizeof(struct bcm_phs_classifier_entry));
					}
=======
				curr_entry = &pstClassifierRulesTable->stActivePhsRulesList[nClsidIndex];
				if (curr_entry->bUsed &&
				    curr_entry->pstPhsRule &&
				    (curr_entry->pstPhsRule->u8PHSI == u8PHSI)) {

					if (curr_entry->pstPhsRule->u8RefCnt)
						curr_entry->pstPhsRule->u8RefCnt--;

					if (0 == curr_entry->pstPhsRule->u8RefCnt)
						kfree(curr_entry->pstPhsRule);

					memset(curr_entry,
					       0,
					       sizeof(struct bcm_phs_classifier_entry));
>>>>>>> v3.18
				}
			}
		}
	}
<<<<<<< HEAD
	return lStatus;
=======
	return 0;
>>>>>>> v3.18
}

/*
 * PhsDeleteClassifierRule
 *
 * Routine Description:
 *    Exported function to Delete a PHS Rule for the SFID,CLSID Pair.
 *
 * Arguments:
 *	IN void* pvContext - PHS Driver Specific Context
 *	IN B_UINT16  uiVcid    - The Service Flow ID for which the PHS rule applies
 *	IN B_UINT16  uiClsId   - The Classifier ID within the Service Flow for which the PHS rule applies.
 *
 * Return Value:
 *
 * 0 if successful,
 * >0 Error.
 */
<<<<<<< HEAD
ULONG PhsDeleteClassifierRule(IN void *pvContext, IN B_UINT16 uiVcid, IN B_UINT16 uiClsId)
{
	ULONG lStatus = 0;
=======
ULONG PhsDeleteClassifierRule(IN void *pvContext,
			      IN B_UINT16 uiVcid,
			      IN B_UINT16 uiClsId)
{
>>>>>>> v3.18
	UINT nSFIndex = 0, nClsidIndex = 0;
	struct bcm_phs_entry *pstServiceFlowEntry = NULL;
	struct bcm_phs_classifier_entry *pstClassifierEntry = NULL;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
<<<<<<< HEAD
	struct bcm_phs_extension *pDeviceExtension = (struct bcm_phs_extension *)pvContext;

	if (pDeviceExtension) {
		/* Retrieve the SFID Entry Index for requested Service Flow */
		nSFIndex = GetServiceFlowEntry(pDeviceExtension->pstServiceFlowPhsRulesTable, uiVcid, &pstServiceFlowEntry);
		if (nSFIndex == PHS_INVALID_TABLE_INDEX) {
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "SFID Match Failed\n");
			return ERR_SF_MATCH_FAIL;
		}

		nClsidIndex = GetClassifierEntry(pstServiceFlowEntry->pstClassifierTable,
						uiClsId, eActiveClassifierRuleContext, &pstClassifierEntry);

		if ((nClsidIndex != PHS_INVALID_TABLE_INDEX) && (!pstClassifierEntry->bUnclassifiedPHSRule)) {
			if (pstClassifierEntry->pstPhsRule) {
				if (pstClassifierEntry->pstPhsRule->u8RefCnt)
					pstClassifierEntry->pstPhsRule->u8RefCnt--;

				if (0 == pstClassifierEntry->pstPhsRule->u8RefCnt)
					kfree(pstClassifierEntry->pstPhsRule);
			}
			memset(pstClassifierEntry, 0, sizeof(struct bcm_phs_classifier_entry));
		}

		nClsidIndex = GetClassifierEntry(pstServiceFlowEntry->pstClassifierTable,
						uiClsId, eOldClassifierRuleContext, &pstClassifierEntry);

		if ((nClsidIndex != PHS_INVALID_TABLE_INDEX) && (!pstClassifierEntry->bUnclassifiedPHSRule)) {
			kfree(pstClassifierEntry->pstPhsRule);
			memset(pstClassifierEntry, 0, sizeof(struct bcm_phs_classifier_entry));
		}
	}
	return lStatus;
=======
	struct bcm_phs_extension *pDeviceExtension =
		(struct bcm_phs_extension *)pvContext;

	if (!pDeviceExtension)
		goto out;

	/* Retrieve the SFID Entry Index for requested Service Flow */
	nSFIndex = GetServiceFlowEntry(pDeviceExtension->pstServiceFlowPhsRulesTable,
				       uiVcid, &pstServiceFlowEntry);
	if (nSFIndex == PHS_INVALID_TABLE_INDEX) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH,
				DBG_LVL_ALL, "SFID Match Failed\n");
		return ERR_SF_MATCH_FAIL;
	}

	nClsidIndex =
		GetClassifierEntry(pstServiceFlowEntry->pstClassifierTable,
				   uiClsId,
				   eActiveClassifierRuleContext,
				   &pstClassifierEntry);

	if ((nClsidIndex != PHS_INVALID_TABLE_INDEX) &&
			(!pstClassifierEntry->bUnclassifiedPHSRule)) {
		if (pstClassifierEntry->pstPhsRule) {
			if (pstClassifierEntry->pstPhsRule->u8RefCnt)
				pstClassifierEntry->pstPhsRule->u8RefCnt--;

			if (0 == pstClassifierEntry->pstPhsRule->u8RefCnt)
				kfree(pstClassifierEntry->pstPhsRule);
		}
		memset(pstClassifierEntry, 0,
		       sizeof(struct bcm_phs_classifier_entry));
	}

	nClsidIndex =
		GetClassifierEntry(pstServiceFlowEntry->pstClassifierTable,
				   uiClsId,
				   eOldClassifierRuleContext,
				   &pstClassifierEntry);

	if ((nClsidIndex != PHS_INVALID_TABLE_INDEX) &&
			(!pstClassifierEntry->bUnclassifiedPHSRule)) {
		kfree(pstClassifierEntry->pstPhsRule);
		memset(pstClassifierEntry, 0,
		       sizeof(struct bcm_phs_classifier_entry));
	}

out:
	return 0;
>>>>>>> v3.18
}

/*
 * PhsDeleteSFRules
 *
 * Routine Description:
 *    Exported function to Delete a all PHS Rules for the SFID.
 *
 * Arguments:
 *	IN void* pvContext - PHS Driver Specific Context
 *	IN B_UINT16 uiVcid   - The Service Flow ID for which the PHS rules need to be deleted
 *
 * Return Value:
 *
 * 0 if successful,
 * >0 Error.
 */
ULONG PhsDeleteSFRules(IN void *pvContext, IN B_UINT16 uiVcid)
{
<<<<<<< HEAD
	ULONG lStatus = 0;
=======
>>>>>>> v3.18
	UINT nSFIndex = 0, nClsidIndex = 0;
	struct bcm_phs_entry *pstServiceFlowEntry = NULL;
	struct bcm_phs_classifier_table *pstClassifierRulesTable = NULL;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
<<<<<<< HEAD
	struct bcm_phs_extension *pDeviceExtension = (struct bcm_phs_extension *)pvContext;

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "====>\n");

	if (pDeviceExtension) {
		/* Retrieve the SFID Entry Index for requested Service Flow */
		nSFIndex = GetServiceFlowEntry(pDeviceExtension->pstServiceFlowPhsRulesTable,
					uiVcid, &pstServiceFlowEntry);
		if (nSFIndex == PHS_INVALID_TABLE_INDEX) {
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "SFID Match Failed\n");
			return ERR_SF_MATCH_FAIL;
		}

		pstClassifierRulesTable = pstServiceFlowEntry->pstClassifierTable;
		if (pstClassifierRulesTable) {
			for (nClsidIndex = 0; nClsidIndex < MAX_PHSRULE_PER_SF; nClsidIndex++) {
				if (pstClassifierRulesTable->stActivePhsRulesList[nClsidIndex].pstPhsRule) {

					if (pstClassifierRulesTable->stActivePhsRulesList[nClsidIndex].pstPhsRule->u8RefCnt)
						pstClassifierRulesTable->stActivePhsRulesList[nClsidIndex].pstPhsRule->u8RefCnt--;

					if (0 == pstClassifierRulesTable->stActivePhsRulesList[nClsidIndex].pstPhsRule->u8RefCnt)
						kfree(pstClassifierRulesTable->stActivePhsRulesList[nClsidIndex].pstPhsRule);

					pstClassifierRulesTable->stActivePhsRulesList[nClsidIndex].pstPhsRule = NULL;
				}
				memset(&pstClassifierRulesTable->stActivePhsRulesList[nClsidIndex], 0, sizeof(struct bcm_phs_classifier_entry));
				if (pstClassifierRulesTable->stOldPhsRulesList[nClsidIndex].pstPhsRule) {

					if (pstClassifierRulesTable->stOldPhsRulesList[nClsidIndex].pstPhsRule->u8RefCnt)
						pstClassifierRulesTable->stOldPhsRulesList[nClsidIndex].pstPhsRule->u8RefCnt--;

					if (0 == pstClassifierRulesTable->stOldPhsRulesList[nClsidIndex].pstPhsRule->u8RefCnt)
						kfree(pstClassifierRulesTable->stOldPhsRulesList[nClsidIndex].pstPhsRule);

					pstClassifierRulesTable->stOldPhsRulesList[nClsidIndex].pstPhsRule = NULL;
				}
				memset(&pstClassifierRulesTable->stOldPhsRulesList[nClsidIndex], 0, sizeof(struct bcm_phs_classifier_entry));
			}
		}
		pstServiceFlowEntry->bUsed = FALSE;
		pstServiceFlowEntry->uiVcid = 0;
	}

	return lStatus;
=======
	struct bcm_phs_extension *pDeviceExtension =
		(struct bcm_phs_extension *)pvContext;
	struct bcm_phs_classifier_entry *curr_clsf_entry;
	struct bcm_phs_classifier_entry *curr_rules_list;

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL,
			"====>\n");

	if (!pDeviceExtension)
		goto out;

	/* Retrieve the SFID Entry Index for requested Service Flow */
	nSFIndex = GetServiceFlowEntry(pDeviceExtension->pstServiceFlowPhsRulesTable,
				       uiVcid, &pstServiceFlowEntry);
	if (nSFIndex == PHS_INVALID_TABLE_INDEX) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH,
				DBG_LVL_ALL, "SFID Match Failed\n");
		return ERR_SF_MATCH_FAIL;
	}

	pstClassifierRulesTable = pstServiceFlowEntry->pstClassifierTable;
	if (pstClassifierRulesTable) {
		for (nClsidIndex = 0; nClsidIndex < MAX_PHSRULE_PER_SF; nClsidIndex++) {
			curr_clsf_entry =
				&pstClassifierRulesTable->stActivePhsRulesList[nClsidIndex];

			curr_rules_list =
				&pstClassifierRulesTable->stOldPhsRulesList[nClsidIndex];

			if (curr_clsf_entry->pstPhsRule) {

				if (curr_clsf_entry->pstPhsRule->u8RefCnt)
					curr_clsf_entry->pstPhsRule->u8RefCnt--;

				if (0 == curr_clsf_entry->pstPhsRule->u8RefCnt)
					kfree(curr_clsf_entry->pstPhsRule);

				curr_clsf_entry->pstPhsRule = NULL;
			}
			memset(curr_clsf_entry, 0,
			       sizeof(struct bcm_phs_classifier_entry));
			if (curr_rules_list->pstPhsRule) {

				if (curr_rules_list->pstPhsRule->u8RefCnt)
					curr_rules_list->pstPhsRule->u8RefCnt--;

				if (0 == curr_rules_list->pstPhsRule->u8RefCnt)
					kfree(curr_rules_list->pstPhsRule);

				curr_rules_list->pstPhsRule = NULL;
			}
			memset(curr_rules_list, 0,
			       sizeof(struct bcm_phs_classifier_entry));
		}
	}
	pstServiceFlowEntry->bUsed = false;
	pstServiceFlowEntry->uiVcid = 0;

out:
	return 0;
>>>>>>> v3.18
}

/*
 * PhsCompress
 *
 * Routine Description:
 *    Exported function to compress the data using PHS.
 *
 * Arguments:
<<<<<<< HEAD
 *	IN void* pvContext - PHS Driver Specific Context.
 *	IN B_UINT16 uiVcid    - The Service Flow ID to which current packet header compression applies.
 *	IN UINT  uiClsId   - The Classifier ID to which current packet header compression applies.
 *	IN void *pvInputBuffer - The Input buffer containg packet header data
 *	IN void *pvOutputBuffer - The output buffer returned by this function after PHS
 *	IN UINT *pOldHeaderSize  - The actual size of the header before PHS
 *	IN UINT *pNewHeaderSize - The new size of the header after applying PHS
=======
 *	IN void* pvContext	    - PHS Driver Specific Context.
 *	IN B_UINT16 uiVcid	    - The Service Flow ID to which current
 *				      packet header compression applies.
 *	IN UINT  uiClsId	    - The Classifier ID to which current packet
 *				      header compression applies.
 *	IN void *pvInputBuffer	    - The Input buffer containg packet header
 *				      data
 *	IN void *pvOutputBuffer     - The output buffer returned by this
 *				      function after PHS
 *	IN UINT *pOldHeaderSize	    - The actual size of the header before PHS
 *	IN UINT *pNewHeaderSize	    - The new size of the header after applying
 *				      PHS
>>>>>>> v3.18
 *
 * Return Value:
 *
 * 0 if successful,
 * >0 Error.
 */
<<<<<<< HEAD
ULONG PhsCompress(IN void *pvContext,
		IN B_UINT16 uiVcid,
		IN B_UINT16 uiClsId,
		IN void *pvInputBuffer,
		OUT void *pvOutputBuffer,
		OUT UINT *pOldHeaderSize,
		OUT UINT *pNewHeaderSize)
=======
static ULONG PhsCompress(IN void *pvContext,
			 IN B_UINT16 uiVcid,
			 IN B_UINT16 uiClsId,
			 IN void *pvInputBuffer,
			 OUT void *pvOutputBuffer,
			 OUT UINT *pOldHeaderSize,
			 OUT UINT *pNewHeaderSize)
>>>>>>> v3.18
{
	UINT nSFIndex = 0, nClsidIndex = 0;
	struct bcm_phs_entry *pstServiceFlowEntry = NULL;
	struct bcm_phs_classifier_entry *pstClassifierEntry = NULL;
	struct bcm_phs_rule *pstPhsRule = NULL;
	ULONG lStatus = 0;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
<<<<<<< HEAD
	struct bcm_phs_extension *pDeviceExtension = (struct bcm_phs_extension *)pvContext;

	if (pDeviceExtension == NULL) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL, "Invalid Device Extension\n");
=======
	struct bcm_phs_extension *pDeviceExtension =
		(struct bcm_phs_extension *)pvContext;

	if (pDeviceExtension == NULL) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL,
				"Invalid Device Extension\n");
>>>>>>> v3.18
		lStatus = STATUS_PHS_NOCOMPRESSION;
		return lStatus;
	}

<<<<<<< HEAD
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL, "Suppressing header\n");

	/* Retrieve the SFID Entry Index for requested Service Flow */
	nSFIndex = GetServiceFlowEntry(pDeviceExtension->pstServiceFlowPhsRulesTable,
				uiVcid, &pstServiceFlowEntry);
	if (nSFIndex == PHS_INVALID_TABLE_INDEX) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL, "SFID Match Failed\n");
=======
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL,
			"Suppressing header\n");

	/* Retrieve the SFID Entry Index for requested Service Flow */
	nSFIndex = GetServiceFlowEntry(pDeviceExtension->pstServiceFlowPhsRulesTable,
				       uiVcid, &pstServiceFlowEntry);
	if (nSFIndex == PHS_INVALID_TABLE_INDEX) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL,
				"SFID Match Failed\n");
>>>>>>> v3.18
		lStatus = STATUS_PHS_NOCOMPRESSION;
		return lStatus;
	}

	nClsidIndex = GetClassifierEntry(pstServiceFlowEntry->pstClassifierTable,
<<<<<<< HEAD
					uiClsId, eActiveClassifierRuleContext, &pstClassifierEntry);

	if (nClsidIndex == PHS_INVALID_TABLE_INDEX) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL, "No PHS Rule Defined For Classifier\n");
=======
					 uiClsId, eActiveClassifierRuleContext,
					 &pstClassifierEntry);

	if (nClsidIndex == PHS_INVALID_TABLE_INDEX) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL,
				"No PHS Rule Defined For Classifier\n");
>>>>>>> v3.18
		lStatus =  STATUS_PHS_NOCOMPRESSION;
		return lStatus;
	}

	/* get rule from SF id,Cls ID pair and proceed */
	pstPhsRule = pstClassifierEntry->pstPhsRule;
	if (!ValidatePHSRuleComplete(pstPhsRule)) {
<<<<<<< HEAD
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "PHS Rule Defined For Classifier But Not Complete\n");
=======
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL,
				"PHS Rule Defined For Classifier But Not Complete\n");
>>>>>>> v3.18
		lStatus = STATUS_PHS_NOCOMPRESSION;
		return lStatus;
	}

	/* Compress Packet */
<<<<<<< HEAD
	lStatus = phs_compress(pstPhsRule, (PUCHAR)pvInputBuffer,
			(PUCHAR)pvOutputBuffer, pOldHeaderSize, pNewHeaderSize);

	if (lStatus == STATUS_PHS_COMPRESSED) {
		pstPhsRule->PHSModifiedBytes += *pOldHeaderSize - *pNewHeaderSize - 1;
		pstPhsRule->PHSModifiedNumPackets++;
	} else
		pstPhsRule->PHSErrorNumPackets++;
=======
	lStatus = phs_compress(pstPhsRule,
			       (PUCHAR)pvInputBuffer,
			       (PUCHAR)pvOutputBuffer,
			       pOldHeaderSize,
			       pNewHeaderSize);

	if (lStatus == STATUS_PHS_COMPRESSED) {
		pstPhsRule->PHSModifiedBytes +=
			*pOldHeaderSize - *pNewHeaderSize - 1;
		pstPhsRule->PHSModifiedNumPackets++;
	} else {
		pstPhsRule->PHSErrorNumPackets++;
	}
>>>>>>> v3.18

	return lStatus;
}

/*
 * PhsDeCompress
 *
 * Routine Description:
 *    Exported function to restore the packet header in Rx path.
 *
 * Arguments:
<<<<<<< HEAD
 *	IN void* pvContext - PHS Driver Specific Context.
 *	IN B_UINT16 uiVcid    - The Service Flow ID to which current packet header restoration applies.
 *	IN  void *pvInputBuffer - The Input buffer containg suppressed packet header data
 *	OUT void *pvOutputBuffer - The output buffer returned by this function after restoration
 *	OUT UINT *pHeaderSize   - The packet header size after restoration is returned in this parameter.
=======
 *	IN void* pvContext	    - PHS Driver Specific Context.
 *	IN B_UINT16 uiVcid	    - The Service Flow ID to which current
 *				      packet header restoration applies.
 *	IN  void *pvInputBuffer	    - The Input buffer containg suppressed
 *				      packet header data
 *	OUT void *pvOutputBuffer    - The output buffer returned by this
 *				      function after restoration
 *	OUT UINT *pHeaderSize	    - The packet header size after restoration
 *				      is returned in this parameter.
>>>>>>> v3.18
 *
 * Return Value:
 *
 * 0 if successful,
 * >0 Error.
 */
<<<<<<< HEAD
ULONG PhsDeCompress(IN void *pvContext,
		IN B_UINT16 uiVcid,
		IN void *pvInputBuffer,
		OUT void *pvOutputBuffer,
		OUT UINT *pInHeaderSize,
		OUT UINT *pOutHeaderSize)
=======
static ULONG PhsDeCompress(IN void *pvContext,
			   IN B_UINT16 uiVcid,
			   IN void *pvInputBuffer,
			   OUT void *pvOutputBuffer,
			   OUT UINT *pInHeaderSize,
			   OUT UINT *pOutHeaderSize)
>>>>>>> v3.18
{
	UINT nSFIndex = 0, nPhsRuleIndex = 0;
	struct bcm_phs_entry *pstServiceFlowEntry = NULL;
	struct bcm_phs_rule *pstPhsRule = NULL;
	UINT phsi;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
<<<<<<< HEAD
	struct bcm_phs_extension *pDeviceExtension = (struct bcm_phs_extension *)pvContext;

	*pInHeaderSize = 0;
	if (pDeviceExtension == NULL) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_RECEIVE, DBG_LVL_ALL, "Invalid Device Extension\n");
		return ERR_PHS_INVALID_DEVICE_EXETENSION;
	}

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_RECEIVE, DBG_LVL_ALL, "Restoring header\n");

	phsi = *((unsigned char *)(pvInputBuffer));
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_RECEIVE, DBG_LVL_ALL, "PHSI To Be Used For restore : %x\n", phsi);
=======
	struct bcm_phs_extension *pDeviceExtension =
		(struct bcm_phs_extension *)pvContext;

	*pInHeaderSize = 0;
	if (pDeviceExtension == NULL) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_RECEIVE,
				DBG_LVL_ALL, "Invalid Device Extension\n");
		return ERR_PHS_INVALID_DEVICE_EXETENSION;
	}

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_RECEIVE, DBG_LVL_ALL,
			"Restoring header\n");

	phsi = *((unsigned char *)(pvInputBuffer));
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_RECEIVE, DBG_LVL_ALL,
			"PHSI To Be Used For restore : %x\n", phsi);
>>>>>>> v3.18
	if (phsi == UNCOMPRESSED_PACKET)
		return STATUS_PHS_NOCOMPRESSION;

	/* Retrieve the SFID Entry Index for requested Service Flow */
	nSFIndex = GetServiceFlowEntry(pDeviceExtension->pstServiceFlowPhsRulesTable,
<<<<<<< HEAD
				uiVcid, &pstServiceFlowEntry);
	if (nSFIndex == PHS_INVALID_TABLE_INDEX) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_RECEIVE, DBG_LVL_ALL, "SFID Match Failed During Lookup\n");
		return ERR_SF_MATCH_FAIL;
	}

	nPhsRuleIndex = GetPhsRuleEntry(pstServiceFlowEntry->pstClassifierTable, phsi,
					eActiveClassifierRuleContext, &pstPhsRule);
	if (nPhsRuleIndex == PHS_INVALID_TABLE_INDEX) {
		/* Phs Rule does not exist in  active rules table. Lets try in the old rules table. */
		nPhsRuleIndex = GetPhsRuleEntry(pstServiceFlowEntry->pstClassifierTable,
						phsi, eOldClassifierRuleContext, &pstPhsRule);
=======
				       uiVcid, &pstServiceFlowEntry);
	if (nSFIndex == PHS_INVALID_TABLE_INDEX) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_RECEIVE,
				DBG_LVL_ALL,
				"SFID Match Failed During Lookup\n");
		return ERR_SF_MATCH_FAIL;
	}

	nPhsRuleIndex = GetPhsRuleEntry(pstServiceFlowEntry->pstClassifierTable,
					phsi,
					eActiveClassifierRuleContext,
					&pstPhsRule);
	if (nPhsRuleIndex == PHS_INVALID_TABLE_INDEX) {
		/* Phs Rule does not exist in  active rules table. Lets try
		 * in the old rules table. */
		nPhsRuleIndex = GetPhsRuleEntry(pstServiceFlowEntry->pstClassifierTable,
						phsi,
						eOldClassifierRuleContext,
						&pstPhsRule);
>>>>>>> v3.18
		if (nPhsRuleIndex == PHS_INVALID_TABLE_INDEX)
			return ERR_PHSRULE_MATCH_FAIL;
	}

	*pInHeaderSize = phs_decompress((PUCHAR)pvInputBuffer,
<<<<<<< HEAD
					(PUCHAR)pvOutputBuffer, pstPhsRule, pOutHeaderSize);
=======
					(PUCHAR)pvOutputBuffer,
					pstPhsRule,
					pOutHeaderSize);
>>>>>>> v3.18

	pstPhsRule->PHSModifiedBytes += *pOutHeaderSize - *pInHeaderSize - 1;

	pstPhsRule->PHSModifiedNumPackets++;
	return STATUS_PHS_COMPRESSED;
}

/*
 * Procedure:   free_phs_serviceflow_rules
 *
<<<<<<< HEAD
 * Description: This routine is responsible for freeing memory allocated for PHS rules.
=======
 * Description: This routine is responsible for freeing memory allocated for
 * PHS rules.
>>>>>>> v3.18
 *
 * Arguments:
 * rules	- ptr to S_SERVICEFLOW_TABLE structure.
 *
 * Returns:
 * Does not return any value.
 */
static void free_phs_serviceflow_rules(struct bcm_phs_table *psServiceFlowRulesTable)
{
	int i, j;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
<<<<<<< HEAD

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "=======>\n");

	if (psServiceFlowRulesTable) {
		for (i = 0; i < MAX_SERVICEFLOWS; i++) {
			struct bcm_phs_entry stServiceFlowEntry = psServiceFlowRulesTable->stSFList[i];
			struct bcm_phs_classifier_table *pstClassifierRulesTable = stServiceFlowEntry.pstClassifierTable;

			if (pstClassifierRulesTable) {
				for (j = 0; j < MAX_PHSRULE_PER_SF; j++) {
					if (pstClassifierRulesTable->stActivePhsRulesList[j].pstPhsRule) {

						if (pstClassifierRulesTable->stActivePhsRulesList[j].pstPhsRule->u8RefCnt)
							pstClassifierRulesTable->stActivePhsRulesList[j].pstPhsRule->u8RefCnt--;

						if (0 == pstClassifierRulesTable->stActivePhsRulesList[j].pstPhsRule->u8RefCnt)
							kfree(pstClassifierRulesTable->stActivePhsRulesList[j].pstPhsRule);

						pstClassifierRulesTable->stActivePhsRulesList[j].pstPhsRule = NULL;
					}

					if (pstClassifierRulesTable->stOldPhsRulesList[j].pstPhsRule) {

						if (pstClassifierRulesTable->stOldPhsRulesList[j].pstPhsRule->u8RefCnt)
							pstClassifierRulesTable->stOldPhsRulesList[j].pstPhsRule->u8RefCnt--;

						if (0 == pstClassifierRulesTable->stOldPhsRulesList[j].pstPhsRule->u8RefCnt)
							kfree(pstClassifierRulesTable->stOldPhsRulesList[j].pstPhsRule);

						pstClassifierRulesTable->stOldPhsRulesList[j].pstPhsRule = NULL;
					}
				}
				kfree(pstClassifierRulesTable);
				stServiceFlowEntry.pstClassifierTable = pstClassifierRulesTable = NULL;
			}
		}
	}

=======
	struct bcm_phs_classifier_entry *curr_act_rules_list;
	struct bcm_phs_classifier_entry *curr_old_rules_list;

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL,
			"=======>\n");

	if (!psServiceFlowRulesTable)
		goto out;

	for (i = 0; i < MAX_SERVICEFLOWS; i++) {
		struct bcm_phs_entry stServiceFlowEntry =
			psServiceFlowRulesTable->stSFList[i];
		struct bcm_phs_classifier_table *pstClassifierRulesTable =
			stServiceFlowEntry.pstClassifierTable;

		if (pstClassifierRulesTable) {
			for (j = 0; j < MAX_PHSRULE_PER_SF; j++) {
				curr_act_rules_list =
					&pstClassifierRulesTable->stActivePhsRulesList[j];

				curr_old_rules_list =
					&pstClassifierRulesTable->stOldPhsRulesList[j];

				if (curr_act_rules_list->pstPhsRule) {

					if (curr_act_rules_list->pstPhsRule->u8RefCnt)
						curr_act_rules_list->pstPhsRule->u8RefCnt--;

					if (0 == curr_act_rules_list->pstPhsRule->u8RefCnt)
						kfree(curr_act_rules_list->pstPhsRule);

					curr_act_rules_list->pstPhsRule = NULL;
				}

				if (curr_old_rules_list->pstPhsRule) {

					if (curr_old_rules_list->pstPhsRule->u8RefCnt)
						curr_old_rules_list->pstPhsRule->u8RefCnt--;

					if (0 == curr_old_rules_list->pstPhsRule->u8RefCnt)
						kfree(curr_old_rules_list->pstPhsRule);

					curr_old_rules_list->pstPhsRule = NULL;
				}
			}
			kfree(pstClassifierRulesTable);
			stServiceFlowEntry.pstClassifierTable =
				pstClassifierRulesTable = NULL;
		}
	}

out:

>>>>>>> v3.18
	kfree(psServiceFlowRulesTable);
	psServiceFlowRulesTable = NULL;
}

<<<<<<< HEAD
static BOOLEAN ValidatePHSRuleComplete(IN struct bcm_phs_rule *psPhsRule)
{
	if (psPhsRule) {
		if (!psPhsRule->u8PHSI) {
			/* PHSI is not valid */
			return FALSE;
		}

		if (!psPhsRule->u8PHSS) {
			/* PHSS Is Undefined */
			return FALSE;
		}

		/* Check if PHSF is defines for the PHS Rule */
		if (!psPhsRule->u8PHSFLength) /* If any part of PHSF is valid then Rule contains valid PHSF */
			return FALSE;

		return TRUE;
	} else
		return FALSE;
}

UINT GetServiceFlowEntry(IN struct bcm_phs_table *psServiceFlowTable,
			IN B_UINT16 uiVcid,
			struct bcm_phs_entry **ppstServiceFlowEntry)
{
	int i;

	for (i = 0; i < MAX_SERVICEFLOWS; i++) {
		if (psServiceFlowTable->stSFList[i].bUsed) {
			if (psServiceFlowTable->stSFList[i].uiVcid == uiVcid) {
				*ppstServiceFlowEntry = &psServiceFlowTable->stSFList[i];
				return i;
			}
=======
static bool ValidatePHSRuleComplete(IN const struct bcm_phs_rule *psPhsRule)
{
	return (psPhsRule &&
		psPhsRule->u8PHSI &&
		psPhsRule->u8PHSS &&
		psPhsRule->u8PHSFLength);
}

UINT GetServiceFlowEntry(IN struct bcm_phs_table *psServiceFlowTable,
			 IN B_UINT16 uiVcid,
			 struct bcm_phs_entry **ppstServiceFlowEntry)
{
	int i;
	struct bcm_phs_entry *curr_sf_list;

	for (i = 0; i < MAX_SERVICEFLOWS; i++) {
		curr_sf_list = &psServiceFlowTable->stSFList[i];
		if (curr_sf_list->bUsed && (curr_sf_list->uiVcid == uiVcid)) {
			*ppstServiceFlowEntry = curr_sf_list;
			return i;
>>>>>>> v3.18
		}
	}

	*ppstServiceFlowEntry = NULL;
	return PHS_INVALID_TABLE_INDEX;
}

<<<<<<< HEAD
UINT GetClassifierEntry(IN struct bcm_phs_classifier_table *pstClassifierTable,
			IN B_UINT32 uiClsid, enum bcm_phs_classifier_context eClsContext,
			OUT struct bcm_phs_classifier_entry **ppstClassifierEntry)
=======
static UINT GetClassifierEntry(IN struct bcm_phs_classifier_table *pstClassifierTable,
			       IN B_UINT32 uiClsid,
			       enum bcm_phs_classifier_context eClsContext,
			       OUT struct bcm_phs_classifier_entry **ppstClassifierEntry)
>>>>>>> v3.18
{
	int  i;
	struct bcm_phs_classifier_entry *psClassifierRules = NULL;

	for (i = 0; i < MAX_PHSRULE_PER_SF; i++) {

		if (eClsContext == eActiveClassifierRuleContext)
<<<<<<< HEAD
			psClassifierRules = &pstClassifierTable->stActivePhsRulesList[i];
		else
			psClassifierRules = &pstClassifierTable->stOldPhsRulesList[i];

		if (psClassifierRules->bUsed) {
			if (psClassifierRules->uiClassifierRuleId == uiClsid) {
				*ppstClassifierEntry = psClassifierRules;
				return i;
			}
=======
			psClassifierRules =
				&pstClassifierTable->stActivePhsRulesList[i];
		else
			psClassifierRules =
				&pstClassifierTable->stOldPhsRulesList[i];

		if (psClassifierRules->bUsed &&
		   (psClassifierRules->uiClassifierRuleId == uiClsid)) {
			*ppstClassifierEntry = psClassifierRules;
			return i;
>>>>>>> v3.18
		}
	}

	*ppstClassifierEntry = NULL;
	return PHS_INVALID_TABLE_INDEX;
}

static UINT GetPhsRuleEntry(IN struct bcm_phs_classifier_table *pstClassifierTable,
<<<<<<< HEAD
			IN B_UINT32 uiPHSI, enum bcm_phs_classifier_context eClsContext,
			OUT struct bcm_phs_rule **ppstPhsRule)
=======
			    IN B_UINT32 uiPHSI,
			    enum bcm_phs_classifier_context eClsContext,
			    OUT struct bcm_phs_rule **ppstPhsRule)
>>>>>>> v3.18
{
	int  i;
	struct bcm_phs_classifier_entry *pstClassifierRule = NULL;

	for (i = 0; i < MAX_PHSRULE_PER_SF; i++) {
		if (eClsContext == eActiveClassifierRuleContext)
<<<<<<< HEAD
			pstClassifierRule = &pstClassifierTable->stActivePhsRulesList[i];
		else
			pstClassifierRule = &pstClassifierTable->stOldPhsRulesList[i];

		if (pstClassifierRule->bUsed) {
			if (pstClassifierRule->u8PHSI == uiPHSI) {
				*ppstPhsRule = pstClassifierRule->pstPhsRule;
				return i;
			}
=======
			pstClassifierRule =
				&pstClassifierTable->stActivePhsRulesList[i];
		else
			pstClassifierRule =
				&pstClassifierTable->stOldPhsRulesList[i];

		if (pstClassifierRule->bUsed &&
		   (pstClassifierRule->u8PHSI == uiPHSI)) {
			*ppstPhsRule = pstClassifierRule->pstPhsRule;
			return i;
>>>>>>> v3.18
		}
	}

	*ppstPhsRule = NULL;
	return PHS_INVALID_TABLE_INDEX;
}

<<<<<<< HEAD
UINT CreateSFToClassifierRuleMapping(IN B_UINT16 uiVcid, IN B_UINT16  uiClsId,
				IN struct bcm_phs_table *psServiceFlowTable,
				struct bcm_phs_rule *psPhsRule,
				B_UINT8 u8AssociatedPHSI)
=======
static UINT CreateSFToClassifierRuleMapping(IN B_UINT16 uiVcid,
					    IN B_UINT16  uiClsId,
					    IN struct bcm_phs_table *psServiceFlowTable,
					    struct bcm_phs_rule *psPhsRule,
					    B_UINT8 u8AssociatedPHSI)
>>>>>>> v3.18
{
	struct bcm_phs_classifier_table *psaClassifiertable = NULL;
	UINT uiStatus = 0;
	int iSfIndex;
<<<<<<< HEAD
	BOOLEAN bFreeEntryFound = FALSE;

	/* Check for a free entry in SFID table */
	for (iSfIndex = 0; iSfIndex < MAX_SERVICEFLOWS; iSfIndex++) {
		if (!psServiceFlowTable->stSFList[iSfIndex].bUsed) {
=======
	bool bFreeEntryFound = false;
	struct bcm_phs_entry *curr_list;

	/* Check for a free entry in SFID table */
	for (iSfIndex = 0; iSfIndex < MAX_SERVICEFLOWS; iSfIndex++) {
		curr_list = &psServiceFlowTable->stSFList[iSfIndex];
		if (!curr_list->bUsed) {
>>>>>>> v3.18
			bFreeEntryFound = TRUE;
			break;
		}
	}

	if (!bFreeEntryFound)
		return ERR_SFTABLE_FULL;

<<<<<<< HEAD
	psaClassifiertable = psServiceFlowTable->stSFList[iSfIndex].pstClassifierTable;
	uiStatus = CreateClassifierPHSRule(uiClsId, psaClassifiertable, psPhsRule,
					eActiveClassifierRuleContext, u8AssociatedPHSI);
	if (uiStatus == PHS_SUCCESS) {
		/* Add entry at free index to the SF */
		psServiceFlowTable->stSFList[iSfIndex].bUsed = TRUE;
		psServiceFlowTable->stSFList[iSfIndex].uiVcid = uiVcid;
=======
	psaClassifiertable = curr_list->pstClassifierTable;
	uiStatus = CreateClassifierPHSRule(uiClsId,
					   psaClassifiertable,
					   psPhsRule,
					   eActiveClassifierRuleContext,
					   u8AssociatedPHSI);
	if (uiStatus == PHS_SUCCESS) {
		/* Add entry at free index to the SF */
		curr_list->bUsed = TRUE;
		curr_list->uiVcid = uiVcid;
>>>>>>> v3.18
	}

	return uiStatus;
}

<<<<<<< HEAD
UINT CreateClassiferToPHSRuleMapping(IN B_UINT16 uiVcid,
				IN B_UINT16 uiClsId,
				IN struct bcm_phs_entry *pstServiceFlowEntry,
				struct bcm_phs_rule *psPhsRule,
				B_UINT8 u8AssociatedPHSI)
=======
static UINT CreateClassiferToPHSRuleMapping(IN B_UINT16 uiVcid,
					    IN B_UINT16 uiClsId,
					    IN struct bcm_phs_entry *pstServiceFlowEntry,
					    struct bcm_phs_rule *psPhsRule,
					    B_UINT8 u8AssociatedPHSI)
>>>>>>> v3.18
{
	struct bcm_phs_classifier_entry *pstClassifierEntry = NULL;
	UINT uiStatus = PHS_SUCCESS;
	UINT nClassifierIndex = 0;
	struct bcm_phs_classifier_table *psaClassifiertable = NULL;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);

	psaClassifiertable = pstServiceFlowEntry->pstClassifierTable;

<<<<<<< HEAD
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "==>");
=======
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL,
			"==>");
>>>>>>> v3.18

	/* Check if the supplied Classifier already exists */
	nClassifierIndex = GetClassifierEntry(
		pstServiceFlowEntry->pstClassifierTable,
		uiClsId,
		eActiveClassifierRuleContext,
		&pstClassifierEntry);

	if (nClassifierIndex == PHS_INVALID_TABLE_INDEX) {
		/*
<<<<<<< HEAD
		 * The Classifier doesn't exist. So its a new classifier being added.
=======
		 * The Classifier doesn't exist. So its a new classifier being
		 * added.
>>>>>>> v3.18
		 * Add new entry to associate PHS Rule to the Classifier
		 */

		uiStatus = CreateClassifierPHSRule(uiClsId, psaClassifiertable,
<<<<<<< HEAD
						psPhsRule,
						eActiveClassifierRuleContext,
						u8AssociatedPHSI);
=======
						   psPhsRule,
						   eActiveClassifierRuleContext,
						   u8AssociatedPHSI);
>>>>>>> v3.18
		return uiStatus;
	}

	/*
	 * The Classifier exists.The PHS Rule for this classifier
	 * is being modified
	 */

	if (pstClassifierEntry->u8PHSI == psPhsRule->u8PHSI) {
		if (pstClassifierEntry->pstPhsRule == NULL)
			return ERR_PHS_INVALID_PHS_RULE;

		/*
<<<<<<< HEAD
		 * This rule already exists if any fields are changed for this PHS
		 * rule update them.
=======
		 * This rule already exists if any fields are changed for this
		 * PHS rule update them.
>>>>>>> v3.18
		 */
		/* If any part of PHSF is valid then we update PHSF */
		if (psPhsRule->u8PHSFLength) {
			/* update PHSF */
			memcpy(pstClassifierEntry->pstPhsRule->u8PHSF,
<<<<<<< HEAD
				psPhsRule->u8PHSF, MAX_PHS_LENGTHS);
=======
			       psPhsRule->u8PHSF,
			       MAX_PHS_LENGTHS);
>>>>>>> v3.18
		}

		if (psPhsRule->u8PHSFLength) {
			/* update PHSFLen */
<<<<<<< HEAD
			pstClassifierEntry->pstPhsRule->u8PHSFLength = psPhsRule->u8PHSFLength;
=======
			pstClassifierEntry->pstPhsRule->u8PHSFLength =
				psPhsRule->u8PHSFLength;
>>>>>>> v3.18
		}

		if (psPhsRule->u8PHSMLength) {
			/* update PHSM */
			memcpy(pstClassifierEntry->pstPhsRule->u8PHSM,
<<<<<<< HEAD
				psPhsRule->u8PHSM, MAX_PHS_LENGTHS);
=======
			       psPhsRule->u8PHSM,
			       MAX_PHS_LENGTHS);
>>>>>>> v3.18
		}

		if (psPhsRule->u8PHSMLength) {
			/* update PHSM Len */
			pstClassifierEntry->pstPhsRule->u8PHSMLength =
				psPhsRule->u8PHSMLength;
		}

		if (psPhsRule->u8PHSS) {
			/* update PHSS */
<<<<<<< HEAD
			pstClassifierEntry->pstPhsRule->u8PHSS = psPhsRule->u8PHSS;
=======
			pstClassifierEntry->pstPhsRule->u8PHSS =
				psPhsRule->u8PHSS;
>>>>>>> v3.18
		}

		/* update PHSV */
		pstClassifierEntry->pstPhsRule->u8PHSV = psPhsRule->u8PHSV;
	} else {
		/* A new rule is being set for this classifier. */
<<<<<<< HEAD
		uiStatus = UpdateClassifierPHSRule(uiClsId, pstClassifierEntry,
						psaClassifiertable, psPhsRule, u8AssociatedPHSI);
=======
		uiStatus = UpdateClassifierPHSRule(uiClsId,
						   pstClassifierEntry,
						   psaClassifiertable,
						   psPhsRule,
						   u8AssociatedPHSI);
>>>>>>> v3.18
	}

	return uiStatus;
}

static UINT CreateClassifierPHSRule(IN B_UINT16  uiClsId,
<<<<<<< HEAD
				struct bcm_phs_classifier_table *psaClassifiertable,
				struct bcm_phs_rule *psPhsRule,
				enum bcm_phs_classifier_context eClsContext,
				B_UINT8 u8AssociatedPHSI)
{
	UINT iClassifierIndex = 0;
	BOOLEAN bFreeEntryFound = FALSE;
=======
				    struct bcm_phs_classifier_table *psaClassifiertable,
				    struct bcm_phs_rule *psPhsRule,
				    enum bcm_phs_classifier_context eClsContext,
				    B_UINT8 u8AssociatedPHSI)
{
	UINT iClassifierIndex = 0;
	bool bFreeEntryFound = false;
>>>>>>> v3.18
	struct bcm_phs_classifier_entry *psClassifierRules = NULL;
	UINT nStatus = PHS_SUCCESS;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);

<<<<<<< HEAD
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "Inside CreateClassifierPHSRule");
=======
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL,
			"Inside CreateClassifierPHSRule");
>>>>>>> v3.18

	if (psaClassifiertable == NULL)
		return ERR_INVALID_CLASSIFIERTABLE_FOR_SF;

	if (eClsContext == eOldClassifierRuleContext) {
		/*
		 * If An Old Entry for this classifier ID already exists in the
		 * old rules table replace it.
		 */

<<<<<<< HEAD
		iClassifierIndex =
			GetClassifierEntry(psaClassifiertable, uiClsId,
					eClsContext, &psClassifierRules);
=======
		iClassifierIndex = GetClassifierEntry(psaClassifiertable,
						      uiClsId,
						      eClsContext,
						      &psClassifierRules);
>>>>>>> v3.18

		if (iClassifierIndex != PHS_INVALID_TABLE_INDEX) {
			/*
			 * The Classifier already exists in the old rules table
			 * Lets replace the old classifier with the new one.
			 */
			bFreeEntryFound = TRUE;
		}
	}

	if (!bFreeEntryFound) {
		/* Continue to search for a free location to add the rule */
		for (iClassifierIndex = 0; iClassifierIndex <
			     MAX_PHSRULE_PER_SF; iClassifierIndex++) {
			if (eClsContext == eActiveClassifierRuleContext)
				psClassifierRules = &psaClassifiertable->stActivePhsRulesList[iClassifierIndex];
			else
				psClassifierRules = &psaClassifiertable->stOldPhsRulesList[iClassifierIndex];

			if (!psClassifierRules->bUsed) {
				bFreeEntryFound = TRUE;
				break;
			}
		}
	}

	if (!bFreeEntryFound) {

		if (eClsContext == eActiveClassifierRuleContext)
			return ERR_CLSASSIFIER_TABLE_FULL;
		else {
<<<<<<< HEAD
			/* Lets replace the oldest rule if we are looking in old Rule table */
			if (psaClassifiertable->uiOldestPhsRuleIndex >= MAX_PHSRULE_PER_SF)
				psaClassifiertable->uiOldestPhsRuleIndex = 0;

			iClassifierIndex = psaClassifiertable->uiOldestPhsRuleIndex;
			psClassifierRules = &psaClassifiertable->stOldPhsRulesList[iClassifierIndex];
=======
			/* Lets replace the oldest rule if we are looking in
			 * old Rule table */
			if (psaClassifiertable->uiOldestPhsRuleIndex >= MAX_PHSRULE_PER_SF)
				psaClassifiertable->uiOldestPhsRuleIndex = 0;

			iClassifierIndex =
				psaClassifiertable->uiOldestPhsRuleIndex;
			psClassifierRules =
				&psaClassifiertable->stOldPhsRulesList[iClassifierIndex];
>>>>>>> v3.18

			(psaClassifiertable->uiOldestPhsRuleIndex)++;
		}
	}

	if (eClsContext == eOldClassifierRuleContext) {

		if (psClassifierRules->pstPhsRule == NULL) {

<<<<<<< HEAD
			psClassifierRules->pstPhsRule = kmalloc(sizeof(struct bcm_phs_rule), GFP_KERNEL);
=======
			psClassifierRules->pstPhsRule =
				kmalloc(sizeof(struct bcm_phs_rule),
					GFP_KERNEL);
>>>>>>> v3.18

			if (NULL == psClassifierRules->pstPhsRule)
				return ERR_PHSRULE_MEMALLOC_FAIL;
		}

		psClassifierRules->bUsed = TRUE;
		psClassifierRules->uiClassifierRuleId = uiClsId;
		psClassifierRules->u8PHSI = psPhsRule->u8PHSI;
<<<<<<< HEAD
		psClassifierRules->bUnclassifiedPHSRule = psPhsRule->bUnclassifiedPHSRule;

		/* Update The PHS rule */
		memcpy(psClassifierRules->pstPhsRule, psPhsRule, sizeof(struct bcm_phs_rule));
	} else
		nStatus = UpdateClassifierPHSRule(uiClsId, psClassifierRules,
						psaClassifiertable, psPhsRule, u8AssociatedPHSI);
=======
		psClassifierRules->bUnclassifiedPHSRule =
			psPhsRule->bUnclassifiedPHSRule;

		/* Update The PHS rule */
		memcpy(psClassifierRules->pstPhsRule, psPhsRule,
		       sizeof(struct bcm_phs_rule));
	} else
		nStatus = UpdateClassifierPHSRule(uiClsId,
						  psClassifierRules,
						  psaClassifiertable,
						  psPhsRule,
						  u8AssociatedPHSI);
>>>>>>> v3.18

	return nStatus;
}

static UINT UpdateClassifierPHSRule(IN B_UINT16  uiClsId,
<<<<<<< HEAD
				IN struct bcm_phs_classifier_entry *pstClassifierEntry,
				struct bcm_phs_classifier_table *psaClassifiertable,
				struct bcm_phs_rule *psPhsRule,
				B_UINT8 u8AssociatedPHSI)
{
	struct bcm_phs_rule *pstAddPhsRule = NULL;
	UINT nPhsRuleIndex = 0;
	BOOLEAN bPHSRuleOrphaned = FALSE;
=======
				    IN struct bcm_phs_classifier_entry *pstClassifierEntry,
				    struct bcm_phs_classifier_table *psaClassifiertable,
				    struct bcm_phs_rule *psPhsRule,
				    B_UINT8 u8AssociatedPHSI)
{
	struct bcm_phs_rule *pstAddPhsRule = NULL;
	UINT nPhsRuleIndex = 0;
	bool bPHSRuleOrphaned = false;
>>>>>>> v3.18
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);

	psPhsRule->u8RefCnt = 0;

	/* Step 1 Deref Any Exisiting PHS Rule in this classifier Entry */
	bPHSRuleOrphaned = DerefPhsRule(uiClsId, psaClassifiertable,
					pstClassifierEntry->pstPhsRule);

<<<<<<< HEAD
	/* Step 2 Search if there is a PHS Rule with u8AssociatedPHSI in Classifier table for this SF */
	nPhsRuleIndex = GetPhsRuleEntry(psaClassifiertable, u8AssociatedPHSI,
					eActiveClassifierRuleContext, &pstAddPhsRule);
	if (PHS_INVALID_TABLE_INDEX == nPhsRuleIndex) {

		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "\nAdding New PHSRuleEntry For Classifier");

		if (psPhsRule->u8PHSI == 0) {
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "\nError PHSI is Zero\n");
			return ERR_PHS_INVALID_PHS_RULE;
		}

		/* Step 2.a PHS Rule Does Not Exist .Create New PHS Rule for uiClsId */
		if (FALSE == bPHSRuleOrphaned) {

			pstClassifierEntry->pstPhsRule = kmalloc(sizeof(struct bcm_phs_rule), GFP_KERNEL);
			if (NULL == pstClassifierEntry->pstPhsRule)
				return ERR_PHSRULE_MEMALLOC_FAIL;
		}
		memcpy(pstClassifierEntry->pstPhsRule, psPhsRule, sizeof(struct bcm_phs_rule));
	} else {
		/* Step 2.b PHS Rule  Exists Tie uiClsId with the existing PHS Rule */
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH, DBG_LVL_ALL, "\nTying Classifier to Existing PHS Rule");
=======
	/* Step 2 Search if there is a PHS Rule with u8AssociatedPHSI in
	 * Classifier table for this SF */
	nPhsRuleIndex = GetPhsRuleEntry(psaClassifiertable, u8AssociatedPHSI,
					eActiveClassifierRuleContext,
					&pstAddPhsRule);
	if (PHS_INVALID_TABLE_INDEX == nPhsRuleIndex) {

		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH,
				DBG_LVL_ALL,
				"\nAdding New PHSRuleEntry For Classifier");

		if (psPhsRule->u8PHSI == 0) {
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH,
					DBG_LVL_ALL, "\nError PHSI is Zero\n");
			return ERR_PHS_INVALID_PHS_RULE;
		}

		/* Step 2.a PHS Rule Does Not Exist .Create New PHS Rule for
		 * uiClsId */
		if (false == bPHSRuleOrphaned) {

			pstClassifierEntry->pstPhsRule =
				kmalloc(sizeof(struct bcm_phs_rule),
					GFP_KERNEL);
			if (NULL == pstClassifierEntry->pstPhsRule)
				return ERR_PHSRULE_MEMALLOC_FAIL;
		}
		memcpy(pstClassifierEntry->pstPhsRule, psPhsRule,
		       sizeof(struct bcm_phs_rule));
	} else {
		/* Step 2.b PHS Rule  Exists Tie uiClsId with the existing
		 * PHS Rule */
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_DISPATCH,
				DBG_LVL_ALL,
				"\nTying Classifier to Existing PHS Rule");
>>>>>>> v3.18
		if (bPHSRuleOrphaned) {
			kfree(pstClassifierEntry->pstPhsRule);
			pstClassifierEntry->pstPhsRule = NULL;
		}
		pstClassifierEntry->pstPhsRule = pstAddPhsRule;
	}

	pstClassifierEntry->bUsed = TRUE;
	pstClassifierEntry->u8PHSI = pstClassifierEntry->pstPhsRule->u8PHSI;
	pstClassifierEntry->uiClassifierRuleId = uiClsId;
	pstClassifierEntry->pstPhsRule->u8RefCnt++;
<<<<<<< HEAD
	pstClassifierEntry->bUnclassifiedPHSRule = pstClassifierEntry->pstPhsRule->bUnclassifiedPHSRule;
=======
	pstClassifierEntry->bUnclassifiedPHSRule =
		pstClassifierEntry->pstPhsRule->bUnclassifiedPHSRule;
>>>>>>> v3.18

	return PHS_SUCCESS;
}

<<<<<<< HEAD
static BOOLEAN DerefPhsRule(IN B_UINT16  uiClsId, struct bcm_phs_classifier_table *psaClassifiertable, struct bcm_phs_rule *pstPhsRule)
{
	if (pstPhsRule == NULL)
		return FALSE;
=======
static bool DerefPhsRule(IN B_UINT16  uiClsId,
			 struct bcm_phs_classifier_table *psaClassifiertable,
			 struct bcm_phs_rule *pstPhsRule)
{
	if (pstPhsRule == NULL)
		return false;
>>>>>>> v3.18

	if (pstPhsRule->u8RefCnt)
		pstPhsRule->u8RefCnt--;

<<<<<<< HEAD
	if (0 == pstPhsRule->u8RefCnt) {
		/*
		 * if(pstPhsRule->u8PHSI)
		 * Store the currently active rule into the old rules list
		 * CreateClassifierPHSRule(uiClsId,psaClassifiertable,pstPhsRule,eOldClassifierRuleContext,pstPhsRule->u8PHSI);
		 */
		return TRUE;
	} else
		return FALSE;
=======
	return (0 == pstPhsRule->u8RefCnt);
}

static void dbg_print_st_cls_entry(struct bcm_mini_adapter *ad,
				   struct bcm_phs_entry *st_serv_flow_entry,
				   struct bcm_phs_classifier_entry *st_cls_entry)
{
	int k;

	BCM_DEBUG_PRINT(ad, DBG_TYPE_OTHERS, DUMP_INFO, DBG_LVL_ALL, "\n VCID  : %#X", st_serv_flow_entry->uiVcid);
	BCM_DEBUG_PRINT(ad, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n ClassifierID  : %#X", st_cls_entry->uiClassifierRuleId);
	BCM_DEBUG_PRINT(ad, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n PHSRuleID  : %#X", st_cls_entry->u8PHSI);
	BCM_DEBUG_PRINT(ad, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n****************PHS Rule********************\n");
	BCM_DEBUG_PRINT(ad, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n PHSI  : %#X", st_cls_entry->pstPhsRule->u8PHSI);
	BCM_DEBUG_PRINT(ad, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n PHSFLength : %#X ", st_cls_entry->pstPhsRule->u8PHSFLength);
	BCM_DEBUG_PRINT(ad, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n PHSF : ");

	for (k = 0 ; k < st_cls_entry->pstPhsRule->u8PHSFLength; k++)
		BCM_DEBUG_PRINT(ad, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "%#X  ", st_cls_entry->pstPhsRule->u8PHSF[k]);
	BCM_DEBUG_PRINT(ad, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n PHSMLength  : %#X", st_cls_entry->pstPhsRule->u8PHSMLength);
	BCM_DEBUG_PRINT(ad, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n PHSM :");

	for (k = 0; k < st_cls_entry->pstPhsRule->u8PHSMLength; k++)
		BCM_DEBUG_PRINT(ad, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "%#X  ", st_cls_entry->pstPhsRule->u8PHSM[k]);
	BCM_DEBUG_PRINT(ad, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n PHSS : %#X ", st_cls_entry->pstPhsRule->u8PHSS);
	BCM_DEBUG_PRINT(ad, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n PHSV  : %#X", st_cls_entry->pstPhsRule->u8PHSV);
	BCM_DEBUG_PRINT(ad, DBG_TYPE_OTHERS, DUMP_INFO, DBG_LVL_ALL, "\n********************************************\n");
}

static void phsrules_per_sf_dbg_print(struct bcm_mini_adapter *ad,
				      struct bcm_phs_entry *st_serv_flow_entry)
{
	int j, l;
	struct bcm_phs_classifier_entry st_cls_entry;

	for (j = 0; j < MAX_PHSRULE_PER_SF; j++) {

		for (l = 0; l < 2; l++) {

			if (l == 0) {
				st_cls_entry = st_serv_flow_entry->pstClassifierTable->stActivePhsRulesList[j];
				if (st_cls_entry.bUsed)
					BCM_DEBUG_PRINT(ad,
							DBG_TYPE_OTHERS,
							DUMP_INFO,
							(DBG_LVL_ALL | DBG_NO_FUNC_PRINT),
							"\n Active PHS Rule :\n");
			} else {
				st_cls_entry = st_serv_flow_entry->pstClassifierTable->stOldPhsRulesList[j];
				if (st_cls_entry.bUsed)
					BCM_DEBUG_PRINT(ad,
							DBG_TYPE_OTHERS,
							DUMP_INFO,
							(DBG_LVL_ALL | DBG_NO_FUNC_PRINT),
							"\n Old PHS Rule :\n");
			}

			if (st_cls_entry.bUsed) {
				dbg_print_st_cls_entry(ad,
						       st_serv_flow_entry,
						       &st_cls_entry);
			}
		}
	}
>>>>>>> v3.18
}

void DumpPhsRules(struct bcm_phs_extension *pDeviceExtension)
{
<<<<<<< HEAD
	int i, j, k, l;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, DBG_LVL_ALL, "\n Dumping PHS Rules :\n");
=======
	int i;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, DBG_LVL_ALL,
			"\n Dumping PHS Rules :\n");
>>>>>>> v3.18

	for (i = 0; i < MAX_SERVICEFLOWS; i++) {

		struct bcm_phs_entry stServFlowEntry =
			pDeviceExtension->pstServiceFlowPhsRulesTable->stSFList[i];
<<<<<<< HEAD
		if (stServFlowEntry.bUsed) {

			for (j = 0; j < MAX_PHSRULE_PER_SF; j++) {

				for (l = 0; l < 2; l++) {
					struct bcm_phs_classifier_entry stClsEntry;

					if (l == 0) {
						stClsEntry = stServFlowEntry.pstClassifierTable->stActivePhsRulesList[j];
						if (stClsEntry.bUsed)
							BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n Active PHS Rule :\n");
					} else {
						stClsEntry = stServFlowEntry.pstClassifierTable->stOldPhsRulesList[j];
						if (stClsEntry.bUsed)
							BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n Old PHS Rule :\n");
					}

					if (stClsEntry.bUsed) {
						BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, DBG_LVL_ALL, "\n VCID  : %#X", stServFlowEntry.uiVcid);
						BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n ClassifierID  : %#X", stClsEntry.uiClassifierRuleId);
						BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n PHSRuleID  : %#X", stClsEntry.u8PHSI);
						BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n****************PHS Rule********************\n");
						BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n PHSI  : %#X", stClsEntry.pstPhsRule->u8PHSI);
						BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n PHSFLength : %#X ", stClsEntry.pstPhsRule->u8PHSFLength);
						BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n PHSF : ");

						for (k = 0 ; k < stClsEntry.pstPhsRule->u8PHSFLength; k++)
							BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "%#X  ", stClsEntry.pstPhsRule->u8PHSF[k]);
						BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n PHSMLength  : %#X", stClsEntry.pstPhsRule->u8PHSMLength);
						BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n PHSM :");

						for (k = 0; k < stClsEntry.pstPhsRule->u8PHSMLength; k++)
							BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "%#X  ", stClsEntry.pstPhsRule->u8PHSM[k]);
						BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n PHSS : %#X ", stClsEntry.pstPhsRule->u8PHSS);
						BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, (DBG_LVL_ALL|DBG_NO_FUNC_PRINT), "\n PHSV  : %#X", stClsEntry.pstPhsRule->u8PHSV);
						BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, DUMP_INFO, DBG_LVL_ALL, "\n********************************************\n");
					}
				}
			}
		}
=======

		if (!stServFlowEntry.bUsed)
			continue;

		phsrules_per_sf_dbg_print(Adapter, &stServFlowEntry);
>>>>>>> v3.18
	}
}

/*
 * Procedure:   phs_decompress
 *
 * Description: This routine restores the static fields within the packet.
 *
 * Arguments:
 *	in_buf			- ptr to incoming packet buffer.
<<<<<<< HEAD
 *	out_buf			- ptr to output buffer where the suppressed header is copied.
 *	decomp_phs_rules - ptr to PHS rule.
 *	header_size		- ptr to field which holds the phss or phsf_length.
 *
 * Returns:
 *	size -The number of bytes of dynamic fields present with in the incoming packet
 *			header.
 *	0	-If PHS rule is NULL.If PHSI is 0 indicateing packet as uncompressed.
 */
int phs_decompress(unsigned char *in_buf,
		unsigned char *out_buf,
		struct bcm_phs_rule *decomp_phs_rules,
		UINT *header_size)
=======
 *	out_buf			- ptr to output buffer where the suppressed
 *				  header is copied.
 *	decomp_phs_rules	- ptr to PHS rule.
 *	header_size		- ptr to field which holds the phss or
 *				  phsf_length.
 *
 * Returns:
 *	size	- The number of bytes of dynamic fields present with in the
 *		  incoming packet header.
 *	0	- If PHS rule is NULL.If PHSI is 0 indicateing packet as
 *		  uncompressed.
 */
static int phs_decompress(unsigned char *in_buf,
			  unsigned char *out_buf,
			  struct bcm_phs_rule *decomp_phs_rules,
			  UINT *header_size)
>>>>>>> v3.18
{
	int phss, size = 0;
	struct bcm_phs_rule *tmp_memb;
	int bit, i = 0;
	unsigned char *phsf, *phsm;
	int in_buf_len = *header_size - 1;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);

	in_buf++;

<<<<<<< HEAD
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_RECEIVE, DBG_LVL_ALL, "====>\n");
	*header_size = 0;

	if ((decomp_phs_rules == NULL))
=======
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_RECEIVE, DBG_LVL_ALL,
			"====>\n");
	*header_size = 0;

	if (decomp_phs_rules == NULL)
>>>>>>> v3.18
		return 0;

	tmp_memb = decomp_phs_rules;
	/*
<<<<<<< HEAD
	 * BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, PHS_RECEIVE,DBG_LVL_ALL,"\nDECOMP:In phs_decompress PHSI 1  %d",phsi));
=======
	 * BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, PHS_RECEIVE,DBG_LVL_ALL,
	 * "\nDECOMP:In phs_decompress PHSI 1  %d",phsi));
>>>>>>> v3.18
	 * header_size = tmp_memb->u8PHSFLength;
	 */
	phss = tmp_memb->u8PHSS;
	phsf = tmp_memb->u8PHSF;
	phsm = tmp_memb->u8PHSM;

	if (phss > MAX_PHS_LENGTHS)
		phss = MAX_PHS_LENGTHS;

	/*
<<<<<<< HEAD
	 * BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, PHS_RECEIVE,DBG_LVL_ALL,"\nDECOMP:
=======
	 * BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, PHS_RECEIVE,DBG_LVL_ALL,
	 * "\nDECOMP:
>>>>>>> v3.18
	 * In phs_decompress PHSI  %d phss %d index %d",phsi,phss,index));
	 */
	while ((phss > 0) && (size < in_buf_len)) {
		bit = ((*phsm << i) & SUPPRESS);

		if (bit == SUPPRESS) {
			*out_buf = *phsf;
<<<<<<< HEAD
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_RECEIVE, DBG_LVL_ALL, "\nDECOMP:In phss  %d phsf %d ouput %d",
					phss, *phsf, *out_buf);
		} else {
			*out_buf = *in_buf;
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_RECEIVE, DBG_LVL_ALL, "\nDECOMP:In phss  %d input %d ouput %d",
=======
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_RECEIVE,
					DBG_LVL_ALL,
					"\nDECOMP:In phss  %d phsf %d output %d",
					phss, *phsf, *out_buf);
		} else {
			*out_buf = *in_buf;
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_RECEIVE,
					DBG_LVL_ALL,
					"\nDECOMP:In phss  %d input %d output %d",
>>>>>>> v3.18
					phss, *in_buf, *out_buf);
			in_buf++;
			size++;
		}
		out_buf++;
		phsf++;
		phss--;
		i++;
		*header_size = *header_size + 1;

		if (i > MAX_NO_BIT) {
			i = 0;
			phsm++;
		}
	}

	return size;
}

/*
 * Procedure:   phs_compress
 *
<<<<<<< HEAD
 * Description: This routine suppresses the static fields within the packet.Before
 * that it will verify the fields to be suppressed with the corresponding fields in the
 * phsf. For verification it checks the phsv field of PHS rule. If set and verification
 * succeeds it suppresses the field.If any one static field is found different none of
 * the static fields are suppressed then the packet is sent as uncompressed packet with
 * phsi=0.
=======
 * Description: This routine suppresses the static fields within the packet.
 * Before that it will verify the fields to be suppressed with the corresponding
 * fields in the phsf. For verification it checks the phsv field of PHS rule.
 * If set and verification succeeds it suppresses the field.If any one static
 * field is found different none of the static fields are suppressed then the
 * packet is sent as uncompressed packet with phsi=0.
>>>>>>> v3.18
 *
 * Arguments:
 *	phs_rule - ptr to PHS rule.
 *	in_buf		- ptr to incoming packet buffer.
<<<<<<< HEAD
 *	out_buf		- ptr to output buffer where the suppressed header is copied.
 *	header_size	- ptr to field which holds the phss.
 *
 * Returns:
 *	size-The number of bytes copied into the output buffer i.e dynamic fields
 *	0	-If PHS rule is NULL.If PHSV field is not set.If the verification fails.
=======
 *	out_buf		- ptr to output buffer where the suppressed header is
 *			  copied.
 *	header_size	- ptr to field which holds the phss.
 *
 * Returns:
 *	size	- The number of bytes copied into the output buffer i.e
 *		  dynamic fields
 *	0	- If PHS rule is NULL.If PHSV field is not set. If the
 *		  verification fails.
>>>>>>> v3.18
 */
static int phs_compress(struct bcm_phs_rule *phs_rule,
			unsigned char *in_buf,
			unsigned char *out_buf,
			UINT *header_size,
			UINT *new_header_size)
{
	unsigned char *old_addr = out_buf;
	int suppress = 0;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);

	if (phs_rule == NULL) {
<<<<<<< HEAD
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL, "\nphs_compress(): phs_rule null!");
=======
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL,
				"\nphs_compress(): phs_rule null!");
>>>>>>> v3.18
		*out_buf = ZERO_PHSI;
		return STATUS_PHS_NOCOMPRESSION;
	}

	if (phs_rule->u8PHSS <= *new_header_size)
		*header_size = phs_rule->u8PHSS;
	else
		*header_size = *new_header_size;

	/* To copy PHSI */
	out_buf++;
	suppress = verify_suppress_phsf(in_buf, out_buf, phs_rule->u8PHSF,
					phs_rule->u8PHSM, phs_rule->u8PHSS,
					phs_rule->u8PHSV, new_header_size);

	if (suppress == STATUS_PHS_COMPRESSED) {
		*old_addr = (unsigned char)phs_rule->u8PHSI;
<<<<<<< HEAD
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL, "\nCOMP:In phs_compress phsi %d", phs_rule->u8PHSI);
	} else {
		*old_addr = ZERO_PHSI;
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL, "\nCOMP:In phs_compress PHSV Verification failed");
=======
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL,
				"\nCOMP:In phs_compress phsi %d",
				phs_rule->u8PHSI);
	} else {
		*old_addr = ZERO_PHSI;
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL,
				"\nCOMP:In phs_compress PHSV Verification failed");
>>>>>>> v3.18
	}

	return suppress;
}

/*
 * Procedure:	verify_suppress_phsf
 *
 * Description: This routine verifies the fields of the packet and if all the
 * static fields are equal it adds the phsi of that PHS rule.If any static
 * field differs it woun't suppress any field.
 *
 * Arguments:
 * rules_set	- ptr to classifier_rules.
 * in_buffer	- ptr to incoming packet buffer.
 * out_buffer	- ptr to output buffer where the suppressed header is copied.
<<<<<<< HEAD
 * phsf			- ptr to phsf.
 * phsm			- ptr to phsm.
 * phss			- variable holding phss.
 *
 * Returns:
 *	size-The number of bytes copied into the output buffer i.e dynamic fields.
 *	0	-Packet has failed the verification.
=======
 * phsf		- ptr to phsf.
 * phsm		- ptr to phsm.
 * phss		- variable holding phss.
 *
 * Returns:
 *	size    - The number of bytes copied into the output buffer i.e dynamic
 *		  fields.
 *	0	- Packet has failed the verification.
>>>>>>> v3.18
 */
static int verify_suppress_phsf(unsigned char *in_buffer,
				unsigned char *out_buffer,
				unsigned char *phsf,
				unsigned char *phsm,
				unsigned int phss,
				unsigned int phsv,
				UINT *new_header_size)
{
	unsigned int size = 0;
	int bit, i = 0;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);

<<<<<<< HEAD
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL, "\nCOMP:In verify_phsf PHSM - 0x%X", *phsm);
=======
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL,
			"\nCOMP:In verify_phsf PHSM - 0x%X", *phsm);
>>>>>>> v3.18

	if (phss > (*new_header_size))
		phss = *new_header_size;

	while (phss > 0) {
		bit = ((*phsm << i) & SUPPRESS);
		if (bit == SUPPRESS) {
			if (*in_buffer != *phsf) {
				if (phsv == VERIFY) {
					BCM_DEBUG_PRINT(Adapter,
							DBG_TYPE_OTHERS,
							PHS_SEND,
							DBG_LVL_ALL,
							"\nCOMP:In verify_phsf failed for field  %d buf  %d phsf %d",
							phss,
							*in_buffer,
							*phsf);
					return STATUS_PHS_NOCOMPRESSION;
				}
			} else
				BCM_DEBUG_PRINT(Adapter,
						DBG_TYPE_OTHERS,
						PHS_SEND,
						DBG_LVL_ALL,
						"\nCOMP:In verify_phsf success for field  %d buf  %d phsf %d",
						phss,
						*in_buffer,
						*phsf);
		} else {
			*out_buffer = *in_buffer;
			BCM_DEBUG_PRINT(Adapter,
					DBG_TYPE_OTHERS,
					PHS_SEND,
					DBG_LVL_ALL,
					"\nCOMP:In copying_header input %d  out %d",
					*in_buffer,
					*out_buffer);
			out_buffer++;
			size++;
		}

		in_buffer++;
		phsf++;
		phss--;
		i++;

		if (i > MAX_NO_BIT) {
			i = 0;
			phsm++;
		}
	}
<<<<<<< HEAD
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL, "\nCOMP:In verify_phsf success");
=======
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, PHS_SEND, DBG_LVL_ALL,
			"\nCOMP:In verify_phsf success");
>>>>>>> v3.18
	*new_header_size = size;
	return STATUS_PHS_COMPRESSED;
}
