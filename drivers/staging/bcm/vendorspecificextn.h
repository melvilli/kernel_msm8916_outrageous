
#ifndef __VENDOR_EXTN_NVM_H__
#define __VENDOR_EXTN_NVM_H__

#define CONTINUE_COMMON_PATH 0xFFFF

INT vendorextnGetSectionInfo(PVOID  pContext, struct bcm_flash2x_vendor_info *pVendorInfo);
INT vendorextnExit(struct bcm_mini_adapter *Adapter);
INT vendorextnInit(struct bcm_mini_adapter *Adapter);
INT vendorextnIoctl(struct bcm_mini_adapter *Adapter, UINT cmd, ULONG arg);
INT vendorextnReadSection(PVOID  pContext, PUCHAR pBuffer, enum bcm_flash2x_section_val SectionVal,
			UINT offset, UINT numOfBytes);
INT vendorextnWriteSection(PVOID  pContext, PUCHAR pBuffer, enum bcm_flash2x_section_val SectionVal,
<<<<<<< HEAD
<<<<<<< HEAD
			UINT offset, UINT numOfBytes, BOOLEAN bVerify);
=======
			UINT offset, UINT numOfBytes, bool bVerify);
>>>>>>> v3.18
=======
			UINT offset, UINT numOfBytes, bool bVerify);
>>>>>>> v3.18
INT vendorextnWriteSectionWithoutErase(PVOID  pContext, PUCHAR pBuffer, enum bcm_flash2x_section_val SectionVal,
			UINT offset, UINT numOfBytes);

#endif /*  */
