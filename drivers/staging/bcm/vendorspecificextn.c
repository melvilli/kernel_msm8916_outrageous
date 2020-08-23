#include "headers.h"
<<<<<<< HEAD
<<<<<<< HEAD
//-----------------------------------------------------------------------------
// Procedure:	vendorextnGetSectionInfo
//
// Description: Finds the type of NVM used.
//
// Arguments:
//		Adapter    - ptr to Adapter object instance
//		pNVMType   - ptr to NVM type.
// Returns:
//		STATUS_SUCCESS/STATUS_FAILURE
//
//-----------------------------------------------------------------------------
INT vendorextnGetSectionInfo(PVOID  pContext, struct bcm_flash2x_vendor_info *pVendorInfo)
=======
=======
>>>>>>> v3.18
/*
 * Procedure:	vendorextnGetSectionInfo
 *
 * Description: Finds the type of NVM used.
 *
 * Arguments:
 *		Adapter    - ptr to Adapter object instance
 *		pNVMType   - ptr to NVM type.
 * Returns:
 *		STATUS_SUCCESS/STATUS_FAILURE
 *
 */
INT vendorextnGetSectionInfo(PVOID  pContext,
			struct bcm_flash2x_vendor_info *pVendorInfo)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	return STATUS_FAILURE;
}

<<<<<<< HEAD
<<<<<<< HEAD
//-----------------------------------------------------------------------------
// Procedure:   vendorextnInit
//
// Description: Initializing the vendor extension NVM interface
//
// Arguments:
//              Adapter   - Pointer to MINI Adapter Structure.

// Returns:
//              STATUS_SUCCESS/STATUS_FAILURE
//
//-----------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
/*
 * Procedure:   vendorextnInit
 *
 * Description: Initializing the vendor extension NVM interface
 *
 * Arguments:
 *              Adapter   - Pointer to MINI Adapter Structure
 * Returns:
 *             STATUS_SUCCESS/STATUS_FAILURE
 *
 *
 */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
INT vendorextnInit(struct bcm_mini_adapter *Adapter)
{
	return STATUS_SUCCESS;
}

<<<<<<< HEAD
<<<<<<< HEAD
//-----------------------------------------------------------------------------
// Procedure:   vendorextnExit
//
// Description: Free the resource associated with vendor extension NVM interface
//
// Arguments:
//              Adapter   - Pointer to MINI Adapter Structure.

// Returns:
//              STATUS_SUCCESS/STATUS_FAILURE
//
//-----------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
/*
 * Procedure:   vendorextnExit
 *
 * Description: Free the resource associated with vendor extension NVM interface
 *
 * Arguments:
 *
 * Returns:
 *              STATUS_SUCCESS/STATUS_FAILURE
 *
 *
 */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
INT vendorextnExit(struct bcm_mini_adapter *Adapter)
{
	return STATUS_SUCCESS;
}

<<<<<<< HEAD
<<<<<<< HEAD
//------------------------------------------------------------------------
// Procedure:	vendorextnIoctl
//
// Description: 	execute the vendor extension specific ioctl
//
//Arguments:
//		Adapter -Beceem private Adapter Structure
//		cmd 	-vendor extension specific Ioctl commad
//		arg		-input parameter sent by vendor
//
// Returns:
//		CONTINUE_COMMON_PATH in case it is not meant to be processed by vendor ioctls
//		STATUS_SUCCESS/STATUS_FAILURE as per the IOCTL return value
//
//--------------------------------------------------------------------------
=======
=======
>>>>>>> v3.18
/*
 * Procedure:	vendorextnIoctl
 *
 * Description: execute the vendor extension specific ioctl
 *
 * Arguments:
 *		Adapter -Beceem private Adapter Structure
 *		cmd	-vendor extension specific Ioctl commad
 *		arg	-input parameter sent by vendor
 *
 * Returns:
 *		CONTINUE_COMMON_PATH in case it is not meant to be processed
 *		by vendor ioctls
 *		STATUS_SUCCESS/STATUS_FAILURE as per the IOCTL return value
 */

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
INT vendorextnIoctl(struct bcm_mini_adapter *Adapter, UINT cmd, ULONG arg)
{
	return CONTINUE_COMMON_PATH;
}



<<<<<<< HEAD
<<<<<<< HEAD
//------------------------------------------------------------------
// Procedure:	vendorextnReadSection
//
// Description: Reads from a section of NVM
//
// Arguments:
//		pContext - ptr to Adapter object instance
//		pBuffer - Read the data from Vendor Area to this buffer
//		SectionVal   - Value of type of Section
//		Offset - Read from the Offset of the Vendor Section.
//		numOfBytes - Read numOfBytes from the Vendor section to Buffer
//
// Returns:
//		STATUS_SUCCESS/STATUS_FAILURE
//
//------------------------------------------------------------------

INT vendorextnReadSection(PVOID  pContext, PUCHAR pBuffer, enum bcm_flash2x_section_val SectionVal,
			UINT offset, UINT numOfBytes)
=======
=======
>>>>>>> v3.18
/*
 * Procedure:	vendorextnReadSection
 *
 * Description: Reads from a section of NVM
 *
 * Arguments:
 *		pContext - ptr to Adapter object instance
 *		pBuffer - Read the data from Vendor Area to this buffer
 *		SectionVal   - Value of type of Section
 *		Offset - Read from the Offset of the Vendor Section.
 *		numOfBytes - Read numOfBytes from the Vendor section to Buffer
 *
 * Returns:
 *		STATUS_SUCCESS/STATUS_FAILURE
 */

INT vendorextnReadSection(PVOID  pContext, PUCHAR pBuffer,
	enum bcm_flash2x_section_val SectionVal, UINT offset, UINT numOfBytes)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	return STATUS_FAILURE;
}



<<<<<<< HEAD
<<<<<<< HEAD
//------------------------------------------------------------------
// Procedure:	vendorextnWriteSection
//
// Description: Write to a Section of NVM
//
// Arguments:
//		pContext - ptr to Adapter object instance
//		pBuffer - Write the data provided in the buffer
//		SectionVal   - Value of type of Section
//		Offset - Writes to the Offset of the Vendor Section.
//		numOfBytes - Write num Bytes after reading from pBuffer.
//		bVerify - the Buffer Written should be verified.
//
// Returns:
//		STATUS_SUCCESS/STATUS_FAILURE
//
//------------------------------------------------------------------
INT vendorextnWriteSection(PVOID  pContext, PUCHAR pBuffer, enum bcm_flash2x_section_val SectionVal,
			UINT offset, UINT numOfBytes, BOOLEAN bVerify)
=======
=======
>>>>>>> v3.18
/*
 * Procedure:	vendorextnWriteSection
 *
 * Description: Write to a Section of NVM
 *
 * Arguments:
 *		pContext - ptr to Adapter object instance
 *		pBuffer - Write the data provided in the buffer
 *		SectionVal   - Value of type of Section
 *		Offset - Writes to the Offset of the Vendor Section.
 *		numOfBytes - Write num Bytes after reading from pBuffer.
 *		bVerify - the Buffer Written should be verified.
 *
 * Returns:
 *		STATUS_SUCCESS/STATUS_FAILURE
 */
INT vendorextnWriteSection(PVOID  pContext, PUCHAR pBuffer,
	enum bcm_flash2x_section_val SectionVal, UINT offset,
	UINT numOfBytes, bool bVerify)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	return STATUS_FAILURE;
}



<<<<<<< HEAD
<<<<<<< HEAD
//------------------------------------------------------------------
// Procedure:	vendorextnWriteSectionWithoutErase
//
// Description: Write to a Section of NVM without erasing the sector
//
// Arguments:
//		pContext - ptr to Adapter object instance
//		pBuffer - Write the data provided in the buffer
//		SectionVal   - Value of type of Section
//		Offset - Writes to the Offset of the Vendor Section.
//		numOfBytes - Write num Bytes after reading from pBuffer.
//
// Returns:
//		STATUS_SUCCESS/STATUS_FAILURE
//
//------------------------------------------------------------------
INT vendorextnWriteSectionWithoutErase(PVOID  pContext, PUCHAR pBuffer, enum bcm_flash2x_section_val SectionVal,
			UINT offset, UINT numOfBytes)
{
	return STATUS_FAILURE;
}

=======
=======
>>>>>>> v3.18
/*
 * Procedure:	vendorextnWriteSectionWithoutErase
 *
 * Description: Write to a Section of NVM without erasing the sector
 *
 * Arguments:
 *		pContext - ptr to Adapter object instance
 *		pBuffer - Write the data provided in the buffer
 *		SectionVal   - Value of type of Section
 *		Offset - Writes to the Offset of the Vendor Section.
 *		numOfBytes - Write num Bytes after reading from pBuffer.
 *
 * Returns:
 *		STATUS_SUCCESS/STATUS_FAILURE
 */
INT vendorextnWriteSectionWithoutErase(PVOID  pContext, PUCHAR pBuffer,
	enum bcm_flash2x_section_val SectionVal, UINT offset, UINT numOfBytes)
{
	return STATUS_FAILURE;
}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
