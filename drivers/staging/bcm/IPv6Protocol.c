#include "headers.h"

<<<<<<< HEAD
static BOOLEAN MatchSrcIpv6Address(struct bcm_classifier_rule *pstClassifierRule,
	struct bcm_ipv6_hdr *pstIpv6Header);
static BOOLEAN MatchDestIpv6Address(struct bcm_classifier_rule *pstClassifierRule,
=======
static bool MatchSrcIpv6Address(struct bcm_classifier_rule *pstClassifierRule,
	struct bcm_ipv6_hdr *pstIpv6Header);
static bool MatchDestIpv6Address(struct bcm_classifier_rule *pstClassifierRule,
>>>>>>> v3.18
	struct bcm_ipv6_hdr *pstIpv6Header);
static VOID DumpIpv6Header(struct bcm_ipv6_hdr *pstIpv6Header);

static UCHAR *GetNextIPV6ChainedHeader(UCHAR **ppucPayload,
<<<<<<< HEAD
	UCHAR *pucNextHeader, BOOLEAN *bParseDone, USHORT *pusPayloadLength)
{
	UCHAR *pucRetHeaderPtr = NULL;
	UCHAR *pucPayloadPtr = NULL;
	USHORT  usNextHeaderOffset = 0 ;
=======
	UCHAR *pucNextHeader, bool *bParseDone, USHORT *pusPayloadLength)
{
	UCHAR *pucRetHeaderPtr = NULL;
	UCHAR *pucPayloadPtr = NULL;
	USHORT  usNextHeaderOffset = 0;
>>>>>>> v3.18
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);

	if ((ppucPayload == NULL) || (*pusPayloadLength == 0) ||
		(*bParseDone)) {
		*bParseDone = TRUE;
		return NULL;
	}

	pucRetHeaderPtr = *ppucPayload;
	pucPayloadPtr = *ppucPayload;

	if (!pucRetHeaderPtr || !pucPayloadPtr) {
		*bParseDone = TRUE;
		return NULL;
	}

	/* Get the Nextt Header Type */
<<<<<<< HEAD
	*bParseDone = FALSE;
=======
	*bParseDone = false;
>>>>>>> v3.18


	switch (*pucNextHeader) {
	case IPV6HDR_TYPE_HOPBYHOP:
<<<<<<< HEAD
		{

			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL, "\nIPv6 HopByHop Header");
			usNextHeaderOffset += sizeof(struct bcm_ipv6_options_hdr);
		}
=======
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
				DBG_LVL_ALL, "\nIPv6 HopByHop Header");
		usNextHeaderOffset += sizeof(struct bcm_ipv6_options_hdr);
>>>>>>> v3.18
		break;

	case IPV6HDR_TYPE_ROUTING:
		{
			struct bcm_ipv6_routing_hdr *pstIpv6RoutingHeader;
<<<<<<< HEAD
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL, "\nIPv6 Routing Header");
			pstIpv6RoutingHeader = (struct bcm_ipv6_routing_hdr *)pucPayloadPtr;
			usNextHeaderOffset += sizeof(struct bcm_ipv6_routing_hdr);
			usNextHeaderOffset += pstIpv6RoutingHeader->ucNumAddresses * IPV6_ADDRESS_SIZEINBYTES;

		}
		break;
	case IPV6HDR_TYPE_FRAGMENTATION:
		{
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL,
					"\nIPv6 Fragmentation Header");
			usNextHeaderOffset += sizeof(struct bcm_ipv6_fragment_hdr);

		}
		break;
	case IPV6HDR_TYPE_DESTOPTS:
		{
			struct bcm_ipv6_dest_options_hdr *pstIpv6DestOptsHdr = (struct bcm_ipv6_dest_options_hdr *)pucPayloadPtr;
			int nTotalOptions = pstIpv6DestOptsHdr->ucHdrExtLen;
=======

			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL, "\nIPv6 Routing Header");
			pstIpv6RoutingHeader =
				(struct bcm_ipv6_routing_hdr *)pucPayloadPtr;
			usNextHeaderOffset += sizeof(struct bcm_ipv6_routing_hdr);
			usNextHeaderOffset += pstIpv6RoutingHeader->ucNumAddresses *
					      IPV6_ADDRESS_SIZEINBYTES;
		}
		break;

	case IPV6HDR_TYPE_FRAGMENTATION:
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
				DBG_LVL_ALL,
				"\nIPv6 Fragmentation Header");
		usNextHeaderOffset += sizeof(struct bcm_ipv6_fragment_hdr);
		break;

	case IPV6HDR_TYPE_DESTOPTS:
		{
			struct bcm_ipv6_dest_options_hdr *pstIpv6DestOptsHdr =
				(struct bcm_ipv6_dest_options_hdr *)pucPayloadPtr;
			int nTotalOptions = pstIpv6DestOptsHdr->ucHdrExtLen;

>>>>>>> v3.18
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL,
					"\nIPv6 DestOpts Header Header");
			usNextHeaderOffset += sizeof(struct bcm_ipv6_dest_options_hdr);
<<<<<<< HEAD
			usNextHeaderOffset += nTotalOptions * IPV6_DESTOPTS_HDR_OPTIONSIZE ;

		}
		break;
	case IPV6HDR_TYPE_AUTHENTICATION:
		{
			struct bcm_ipv6_authentication_hdr *pstIpv6AuthHdr = (struct bcm_ipv6_authentication_hdr *)pucPayloadPtr;
			int nHdrLen = pstIpv6AuthHdr->ucLength;
=======
			usNextHeaderOffset += nTotalOptions *
					      IPV6_DESTOPTS_HDR_OPTIONSIZE;
		}
		break;


	case IPV6HDR_TYPE_AUTHENTICATION:
		{
			struct bcm_ipv6_authentication_hdr *pstIpv6AuthHdr =
				(struct bcm_ipv6_authentication_hdr *)pucPayloadPtr;
			int nHdrLen = pstIpv6AuthHdr->ucLength;

>>>>>>> v3.18
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL,
					"\nIPv6 Authentication Header");
			usNextHeaderOffset += nHdrLen * 4;
		}
		break;
<<<<<<< HEAD
	case IPV6HDR_TYPE_ENCRYPTEDSECURITYPAYLOAD:
		{
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL,
					"\nIPv6 Encrypted Security Payload Header");
			*bParseDone = TRUE;

		}
		break;
	case IPV6_ICMP_HDR_TYPE:
		{
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL, "\nICMP Header");
			*bParseDone = TRUE;
		}
		break;
	case TCP_HEADER_TYPE:
		{
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL, "\nTCP Header");
			*bParseDone = TRUE;
		}
		break;
	case UDP_HEADER_TYPE:
		{
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL, "\nUDP Header");
			*bParseDone = TRUE;
		}
		break;
	default:
		{
			*bParseDone = TRUE;

		}
		break;


	}

	if (*bParseDone == FALSE) {
=======

	case IPV6HDR_TYPE_ENCRYPTEDSECURITYPAYLOAD:
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
				DBG_LVL_ALL,
				"\nIPv6 Encrypted Security Payload Header");
		*bParseDone = TRUE;
		break;

	case IPV6_ICMP_HDR_TYPE:
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
				DBG_LVL_ALL, "\nICMP Header");
		*bParseDone = TRUE;
		break;

	case TCP_HEADER_TYPE:
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
				DBG_LVL_ALL, "\nTCP Header");
		*bParseDone = TRUE;
		break;

	case UDP_HEADER_TYPE:
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
				DBG_LVL_ALL, "\nUDP Header");
		*bParseDone = TRUE;
		break;

	default:
		*bParseDone = TRUE;
		break;
	}

	if (*bParseDone == false) {
>>>>>>> v3.18
		if (*pusPayloadLength <= usNextHeaderOffset) {
			*bParseDone = TRUE;
		} else {
			*pucNextHeader = *pucPayloadPtr;
			pucPayloadPtr += usNextHeaderOffset;
			(*pusPayloadLength) -= usNextHeaderOffset;
		}

	}

	*ppucPayload = pucPayloadPtr;
	return pucRetHeaderPtr;
}


static UCHAR GetIpv6ProtocolPorts(UCHAR *pucPayload, USHORT *pusSrcPort,
	USHORT *pusDestPort, USHORT usPayloadLength, UCHAR ucNextHeader)
{
	UCHAR *pIpv6HdrScanContext = pucPayload;
<<<<<<< HEAD
	BOOLEAN bDone = FALSE;
=======
	bool bDone = false;
>>>>>>> v3.18
	UCHAR ucHeaderType = 0;
	UCHAR *pucNextHeader = NULL;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);

	if (!pucPayload || (usPayloadLength == 0))
		return 0;

	*pusSrcPort = *pusDestPort = 0;
	ucHeaderType = ucNextHeader;
	while (!bDone) {
		pucNextHeader = GetNextIPV6ChainedHeader(&pIpv6HdrScanContext,
<<<<<<< HEAD
					&ucHeaderType, &bDone, &usPayloadLength);
=======
							 &ucHeaderType,
							 &bDone,
							 &usPayloadLength);
>>>>>>> v3.18
		if (bDone) {
			if ((ucHeaderType == TCP_HEADER_TYPE) ||
				(ucHeaderType == UDP_HEADER_TYPE)) {
				*pusSrcPort = *((PUSHORT)(pucNextHeader));
				*pusDestPort = *((PUSHORT)(pucNextHeader+2));
				BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
						DBG_LVL_ALL,
						"\nProtocol Ports - Src Port :0x%x Dest Port : 0x%x",
						ntohs(*pusSrcPort),
						ntohs(*pusDestPort));
			}
			break;

		}
	}
	return ucHeaderType;
}


/*
<<<<<<< HEAD
 * Arg 1 struct bcm_mini_adapter *Adapter is a pointer ot the driver contorl structure
 * Arg 2 PVOID pcIpHeader is a pointer to the IP header of the packet
 */
USHORT	IpVersion6(struct bcm_mini_adapter *Adapter, PVOID pcIpHeader,
					struct bcm_classifier_rule *pstClassifierRule)
=======
 * Arg 1 struct bcm_mini_adapter *Adapter is a pointer ot the driver control
 * structure
 * Arg 2 PVOID pcIpHeader is a pointer to the IP header of the packet
 */
USHORT	IpVersion6(struct bcm_mini_adapter *Adapter, PVOID pcIpHeader,
		   struct bcm_classifier_rule *pstClassifierRule)
>>>>>>> v3.18
{
	USHORT	ushDestPort = 0;
	USHORT	ushSrcPort = 0;
	UCHAR   ucNextProtocolAboveIP = 0;
	struct bcm_ipv6_hdr *pstIpv6Header = NULL;
<<<<<<< HEAD
	BOOLEAN bClassificationSucceed = FALSE;
=======
	bool bClassificationSucceed = false;
>>>>>>> v3.18

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
			DBG_LVL_ALL, "IpVersion6 ==========>\n");

<<<<<<< HEAD
	pstIpv6Header = (struct bcm_ipv6_hdr *)pcIpHeader;
=======
	pstIpv6Header = pcIpHeader;
>>>>>>> v3.18

	DumpIpv6Header(pstIpv6Header);

	/*
	 * Try to get the next higher layer protocol
	 * and the Ports Nos if TCP or UDP
	 */
<<<<<<< HEAD
	ucNextProtocolAboveIP = GetIpv6ProtocolPorts((UCHAR *)(pcIpHeader + sizeof(struct bcm_ipv6_hdr)),
							&ushSrcPort,
							&ushDestPort,
							pstIpv6Header->usPayloadLength,
							pstIpv6Header->ucNextHeader);
=======
	ucNextProtocolAboveIP = GetIpv6ProtocolPorts((UCHAR *)(pcIpHeader +
						     sizeof(struct bcm_ipv6_hdr)),
						     &ushSrcPort,
						     &ushDestPort,
						     pstIpv6Header->usPayloadLength,
						     pstIpv6Header->ucNextHeader);
>>>>>>> v3.18

	do {
		if (pstClassifierRule->ucDirection == 0) {
			/*
			 * cannot be processed for classification.
			 * it is a down link connection
			 */
			break;
		}

		if (!pstClassifierRule->bIpv6Protocol) {
			/*
			 * We are looking for Ipv6 Classifiers
			 * Lets ignore this classifier and try the next one
			 */
			break;
		}

		bClassificationSucceed = MatchSrcIpv6Address(pstClassifierRule,
<<<<<<< HEAD
								pstIpv6Header);
=======
							     pstIpv6Header);
>>>>>>> v3.18
		if (!bClassificationSucceed)
			break;

		bClassificationSucceed = MatchDestIpv6Address(pstClassifierRule,
<<<<<<< HEAD
								pstIpv6Header);
=======
							      pstIpv6Header);
>>>>>>> v3.18
		if (!bClassificationSucceed)
			break;

		/*
		 * Match the protocol type.
		 * For IPv6 the next protocol at end of
		 * Chain of IPv6 prot headers
		 */
		bClassificationSucceed = MatchProtocol(pstClassifierRule,
<<<<<<< HEAD
							ucNextProtocolAboveIP);
=======
						       ucNextProtocolAboveIP);
>>>>>>> v3.18
		if (!bClassificationSucceed)
			break;

		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
				DBG_LVL_ALL, "\nIPv6 Protocol Matched");

		if ((ucNextProtocolAboveIP == TCP_HEADER_TYPE) ||
			(ucNextProtocolAboveIP == UDP_HEADER_TYPE)) {
			/* Match Src Port */
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL, "\nIPv6 Source Port:%x\n",
					ntohs(ushSrcPort));
			bClassificationSucceed = MatchSrcPort(pstClassifierRule,
<<<<<<< HEAD
							ntohs(ushSrcPort));
=======
							      ntohs(ushSrcPort));
>>>>>>> v3.18
			if (!bClassificationSucceed)
				break;

			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL, "\nIPv6 Src Port Matched");

			/* Match Dest Port */
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
<<<<<<< HEAD
					DBG_LVL_ALL, "\nIPv6 Destination Port:%x\n",
					ntohs(ushDestPort));
			bClassificationSucceed = MatchDestPort(pstClassifierRule,
							ntohs(ushDestPort));
			if (!bClassificationSucceed)
				break;
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL, "\nIPv6 Dest Port Matched");
=======
					DBG_LVL_ALL,
					"\nIPv6 Destination Port:%x\n",
					ntohs(ushDestPort));
			bClassificationSucceed = MatchDestPort(pstClassifierRule,
							       ntohs(ushDestPort));
			if (!bClassificationSucceed)
				break;
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
					DBG_LVL_ALL,
					"\nIPv6 Dest Port Matched");
>>>>>>> v3.18
		}
	} while (0);

	if (bClassificationSucceed == TRUE) {
		INT iMatchedSFQueueIndex = 0;
<<<<<<< HEAD
		iMatchedSFQueueIndex = SearchSfid(Adapter, pstClassifierRule->ulSFID);
		if (iMatchedSFQueueIndex >= NO_OF_QUEUES) {
			bClassificationSucceed = FALSE;
		} else {
			if (Adapter->PackInfo[iMatchedSFQueueIndex].bActive == FALSE)
				bClassificationSucceed = FALSE;
		}
=======

		iMatchedSFQueueIndex = SearchSfid(Adapter,
						  pstClassifierRule->ulSFID);
		if ((iMatchedSFQueueIndex >= NO_OF_QUEUES) ||
		    (Adapter->PackInfo[iMatchedSFQueueIndex].bActive == false))
			bClassificationSucceed = false;
>>>>>>> v3.18
	}

	return bClassificationSucceed;
}


<<<<<<< HEAD
static BOOLEAN MatchSrcIpv6Address(struct bcm_classifier_rule *pstClassifierRule,
	struct bcm_ipv6_hdr *pstIpv6Header)
=======
static bool MatchSrcIpv6Address(struct bcm_classifier_rule *pstClassifierRule,
				struct bcm_ipv6_hdr *pstIpv6Header)
>>>>>>> v3.18
{
	UINT uiLoopIndex = 0;
	UINT uiIpv6AddIndex = 0;
	UINT uiIpv6AddrNoLongWords = 4;
	ULONG aulSrcIP[4];
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
<<<<<<< HEAD
=======
	union u_ip_address *src_addr = &pstClassifierRule->stSrcIpAddress;

>>>>>>> v3.18
	/*
	 * This is the no. of Src Addresses ie Range of IP Addresses contained
	 * in the classifier rule for which we need to match
	 */
<<<<<<< HEAD
	UINT  uiCountIPSrcAddresses = (UINT)pstClassifierRule->ucIPSourceAddressLength;
=======
	UINT  uiCountIPSrcAddresses =
		(UINT)pstClassifierRule->ucIPSourceAddressLength;
>>>>>>> v3.18


	if (uiCountIPSrcAddresses == 0)
		return TRUE;


	/* First Convert the Ip Address in the packet to Host Endian order */
<<<<<<< HEAD
	for (uiIpv6AddIndex = 0; uiIpv6AddIndex < uiIpv6AddrNoLongWords; uiIpv6AddIndex++)
		aulSrcIP[uiIpv6AddIndex] = ntohl(pstIpv6Header->ulSrcIpAddress[uiIpv6AddIndex]);

	for (uiLoopIndex = 0; uiLoopIndex < uiCountIPSrcAddresses; uiLoopIndex += uiIpv6AddrNoLongWords) {
=======
	for (uiIpv6AddIndex = 0;
	     uiIpv6AddIndex < uiIpv6AddrNoLongWords;
	     uiIpv6AddIndex++)
		aulSrcIP[uiIpv6AddIndex] =
			ntohl(pstIpv6Header->ulSrcIpAddress[uiIpv6AddIndex]);

	for (uiLoopIndex = 0;
	     uiLoopIndex < uiCountIPSrcAddresses;
	     uiLoopIndex += uiIpv6AddrNoLongWords) {
>>>>>>> v3.18
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
				"\n Src Ipv6 Address In Received Packet :\n ");
		DumpIpv6Address(aulSrcIP);
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
				"\n Src Ipv6 Mask In Classifier Rule:\n");
<<<<<<< HEAD
		DumpIpv6Address(&pstClassifierRule->stSrcIpAddress.ulIpv6Mask[uiLoopIndex]);
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
				"\n Src Ipv6 Address In Classifier Rule :\n");
		DumpIpv6Address(&pstClassifierRule->stSrcIpAddress.ulIpv6Addr[uiLoopIndex]);

		for (uiIpv6AddIndex = 0; uiIpv6AddIndex < uiIpv6AddrNoLongWords; uiIpv6AddIndex++) {
			if ((pstClassifierRule->stSrcIpAddress.ulIpv6Mask[uiLoopIndex+uiIpv6AddIndex] & aulSrcIP[uiIpv6AddIndex])
				!= pstClassifierRule->stSrcIpAddress.ulIpv6Addr[uiLoopIndex+uiIpv6AddIndex]) {
=======
		DumpIpv6Address(&src_addr->ulIpv6Mask[uiLoopIndex]);
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
				"\n Src Ipv6 Address In Classifier Rule :\n");
		DumpIpv6Address(&src_addr->ulIpv6Addr[uiLoopIndex]);

		for (uiIpv6AddIndex = 0;
		     uiIpv6AddIndex < uiIpv6AddrNoLongWords;
		     uiIpv6AddIndex++) {
			if ((src_addr->ulIpv6Mask[uiLoopIndex+uiIpv6AddIndex] &
				aulSrcIP[uiIpv6AddIndex]) !=
			    src_addr->ulIpv6Addr[uiLoopIndex+uiIpv6AddIndex]) {
>>>>>>> v3.18
				/*
				 * Match failed for current Ipv6 Address
				 * Try next Ipv6 Address
				 */
				break;
			}

			if (uiIpv6AddIndex ==  uiIpv6AddrNoLongWords-1) {
				/* Match Found */
				BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
						DBG_LVL_ALL,
						"Ipv6 Src Ip Address Matched\n");
				return TRUE;
			}
		}
	}
<<<<<<< HEAD
	return FALSE;
}

static BOOLEAN MatchDestIpv6Address(struct bcm_classifier_rule *pstClassifierRule,
	struct bcm_ipv6_hdr *pstIpv6Header)
=======
	return false;
}

static bool MatchDestIpv6Address(struct bcm_classifier_rule *pstClassifierRule,
				 struct bcm_ipv6_hdr *pstIpv6Header)
>>>>>>> v3.18
{
	UINT uiLoopIndex = 0;
	UINT uiIpv6AddIndex = 0;
	UINT uiIpv6AddrNoLongWords = 4;
	ULONG aulDestIP[4];
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
<<<<<<< HEAD
=======
	union u_ip_address *dest_addr = &pstClassifierRule->stDestIpAddress;

>>>>>>> v3.18
	/*
	 * This is the no. of Destination Addresses
	 * ie Range of IP Addresses contained in the classifier rule
	 * for which we need to match
	 */
<<<<<<< HEAD
	UINT  uiCountIPDestinationAddresses = (UINT)pstClassifierRule->ucIPDestinationAddressLength;

=======
	UINT uiCountIPDestinationAddresses =
		(UINT)pstClassifierRule->ucIPDestinationAddressLength;
>>>>>>> v3.18

	if (uiCountIPDestinationAddresses == 0)
		return TRUE;


	/* First Convert the Ip Address in the packet to Host Endian order */
<<<<<<< HEAD
	for (uiIpv6AddIndex = 0; uiIpv6AddIndex < uiIpv6AddrNoLongWords; uiIpv6AddIndex++)
		aulDestIP[uiIpv6AddIndex] = ntohl(pstIpv6Header->ulDestIpAddress[uiIpv6AddIndex]);

	for (uiLoopIndex = 0; uiLoopIndex < uiCountIPDestinationAddresses; uiLoopIndex += uiIpv6AddrNoLongWords) {
=======
	for (uiIpv6AddIndex = 0;
	     uiIpv6AddIndex < uiIpv6AddrNoLongWords;
	     uiIpv6AddIndex++)
		aulDestIP[uiIpv6AddIndex] =
			ntohl(pstIpv6Header->ulDestIpAddress[uiIpv6AddIndex]);

	for (uiLoopIndex = 0;
	     uiLoopIndex < uiCountIPDestinationAddresses;
	     uiLoopIndex += uiIpv6AddrNoLongWords) {
>>>>>>> v3.18
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
				"\n Destination Ipv6 Address In Received Packet :\n ");
		DumpIpv6Address(aulDestIP);
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
				"\n Destination Ipv6 Mask In Classifier Rule :\n");
<<<<<<< HEAD
		DumpIpv6Address(&pstClassifierRule->stDestIpAddress.ulIpv6Mask[uiLoopIndex]);
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
				"\n Destination Ipv6 Address In Classifier Rule :\n");
		DumpIpv6Address(&pstClassifierRule->stDestIpAddress.ulIpv6Addr[uiLoopIndex]);

		for (uiIpv6AddIndex = 0; uiIpv6AddIndex < uiIpv6AddrNoLongWords; uiIpv6AddIndex++) {
			if ((pstClassifierRule->stDestIpAddress.ulIpv6Mask[uiLoopIndex+uiIpv6AddIndex] & aulDestIP[uiIpv6AddIndex])
				!= pstClassifierRule->stDestIpAddress.ulIpv6Addr[uiLoopIndex+uiIpv6AddIndex]) {
=======
		DumpIpv6Address(&dest_addr->ulIpv6Mask[uiLoopIndex]);
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
				"\n Destination Ipv6 Address In Classifier Rule :\n");
		DumpIpv6Address(&dest_addr->ulIpv6Addr[uiLoopIndex]);

		for (uiIpv6AddIndex = 0;
		     uiIpv6AddIndex < uiIpv6AddrNoLongWords;
		     uiIpv6AddIndex++) {
			if ((dest_addr->ulIpv6Mask[uiLoopIndex+uiIpv6AddIndex] &
				aulDestIP[uiIpv6AddIndex]) !=
			    dest_addr->ulIpv6Addr[uiLoopIndex+uiIpv6AddIndex]) {
>>>>>>> v3.18
				/*
				 * Match failed for current Ipv6 Address.
				 * Try next Ipv6 Address
				 */
				break;
			}

			if (uiIpv6AddIndex ==  uiIpv6AddrNoLongWords-1) {
				/* Match Found */
				BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG,
						DBG_LVL_ALL,
						"Ipv6 Destination Ip Address Matched\n");
				return TRUE;
			}
		}
	}
<<<<<<< HEAD
	return FALSE;
=======
	return false;
>>>>>>> v3.18

}

VOID DumpIpv6Address(ULONG *puIpv6Address)
{
	UINT uiIpv6AddrNoLongWords = 4;
	UINT uiIpv6AddIndex = 0;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
<<<<<<< HEAD
	for (uiIpv6AddIndex = 0; uiIpv6AddIndex < uiIpv6AddrNoLongWords; uiIpv6AddIndex++) {
=======

	for (uiIpv6AddIndex = 0;
	     uiIpv6AddIndex < uiIpv6AddrNoLongWords;
	     uiIpv6AddIndex++) {
>>>>>>> v3.18
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
				":%lx", puIpv6Address[uiIpv6AddIndex]);
	}

}

static VOID DumpIpv6Header(struct bcm_ipv6_hdr *pstIpv6Header)
{
	UCHAR ucVersion;
	UCHAR ucPrio;
	struct bcm_mini_adapter *Adapter = GET_BCM_ADAPTER(gblpnetdev);
<<<<<<< HEAD
=======

>>>>>>> v3.18
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
			"----Ipv6 Header---");
	ucVersion = pstIpv6Header->ucVersionPrio & 0xf0;
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
			"Version : %x\n", ucVersion);
	ucPrio = pstIpv6Header->ucVersionPrio & 0x0f;
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
			"Priority : %x\n", ucPrio);
	/*
	 * BCM_DEBUG_PRINT( Adapter,DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
	 * "Flow Label : %x\n",(pstIpv6Header->ucVersionPrio &0xf0);
	 */
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
			"Payload Length : %x\n",
			ntohs(pstIpv6Header->usPayloadLength));
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
			"Next Header : %x\n", pstIpv6Header->ucNextHeader);
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
			"Hop Limit : %x\n", pstIpv6Header->ucHopLimit);
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
			"Src Address :\n");
	DumpIpv6Address(pstIpv6Header->ulSrcIpAddress);
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
			"Dest Address :\n");
	DumpIpv6Address(pstIpv6Header->ulDestIpAddress);
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_TX, IPV6_DBG, DBG_LVL_ALL,
			"----Ipv6 Header End---");


}
