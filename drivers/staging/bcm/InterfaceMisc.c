#include "headers.h"

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
static int adapter_err_occurred(const struct bcm_interface_adapter *ad)
{
	if (ad->psAdapter->device_removed == TRUE) {
		BCM_DEBUG_PRINT(ad->psAdapter, DBG_TYPE_PRINTK, 0, 0,
				"Device got removed");
		return -ENODEV;
	}

	if ((ad->psAdapter->StopAllXaction == TRUE) &&
	    (ad->psAdapter->chip_id >= T3LPB)) {
		BCM_DEBUG_PRINT(ad->psAdapter, DBG_TYPE_OTHERS, RDM,
				DBG_LVL_ALL,
				"Currently Xaction is not allowed on the bus");
		return -EACCES;
	}

	if (ad->bSuspended == TRUE || ad->bPreparingForBusSuspend == TRUE) {
		BCM_DEBUG_PRINT(ad->psAdapter, DBG_TYPE_OTHERS, RDM,
				DBG_LVL_ALL,
				"Bus is in suspended states hence RDM not allowed..");
		return -EACCES;
	}

	return 0;
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
int InterfaceRDM(struct bcm_interface_adapter *psIntfAdapter,
		unsigned int addr,
		void *buff,
		int len)
{
	int bytes;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	int err = 0;
>>>>>>> v3.18
=======
	int err = 0;
>>>>>>> v3.18

	if (!psIntfAdapter)
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	if (psIntfAdapter->psAdapter->device_removed == TRUE) {
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_PRINTK, 0, 0, "Device got removed");
		return -ENODEV;
	}

	if ((psIntfAdapter->psAdapter->StopAllXaction == TRUE) && (psIntfAdapter->psAdapter->chip_id >= T3LPB))	{
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, RDM, DBG_LVL_ALL, "Currently Xaction is not allowed on the bus");
		return -EACCES;
	}

	if (psIntfAdapter->bSuspended == TRUE || psIntfAdapter->bPreparingForBusSuspend == TRUE) {
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, RDM, DBG_LVL_ALL, "Bus is in suspended states hence RDM not allowed..");
		return -EACCES;
	}
=======
=======
>>>>>>> v3.18
	err = adapter_err_occurred(psIntfAdapter);
	if (err)
		return err;

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	psIntfAdapter->psAdapter->DeviceAccess = TRUE;

	bytes = usb_control_msg(psIntfAdapter->udev,
				usb_rcvctrlpipe(psIntfAdapter->udev, 0),
				0x02,
				0xC2,
				(addr & 0xFFFF),
				((addr >> 16) & 0xFFFF),
				buff,
				len,
				5000);

	if (-ENODEV == bytes)
		psIntfAdapter->psAdapter->device_removed = TRUE;

	if (bytes < 0)
<<<<<<< HEAD
<<<<<<< HEAD
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, RDM, DBG_LVL_ALL, "RDM failed status :%d", bytes);
	else
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, RDM, DBG_LVL_ALL, "RDM sent %d", bytes);

	psIntfAdapter->psAdapter->DeviceAccess = FALSE;
=======
=======
>>>>>>> v3.18
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, RDM,
				DBG_LVL_ALL, "RDM failed status :%d", bytes);
	else
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, RDM,
				DBG_LVL_ALL, "RDM sent %d", bytes);

	psIntfAdapter->psAdapter->DeviceAccess = false;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return bytes;
}

int InterfaceWRM(struct bcm_interface_adapter *psIntfAdapter,
		unsigned int addr,
		void *buff,
		int len)
{
	int retval = 0;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	int err = 0;
>>>>>>> v3.18
=======
	int err = 0;
>>>>>>> v3.18

	if (!psIntfAdapter)
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	if (psIntfAdapter->psAdapter->device_removed == TRUE) {
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_PRINTK, 0, 0, "Device got removed");
		return -ENODEV;
	}

	if ((psIntfAdapter->psAdapter->StopAllXaction == TRUE) && (psIntfAdapter->psAdapter->chip_id >= T3LPB)) {
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, WRM, DBG_LVL_ALL, "Currently Xaction is not allowed on the bus...");
		return -EACCES;
	}

	if (psIntfAdapter->bSuspended == TRUE || psIntfAdapter->bPreparingForBusSuspend == TRUE) {
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, WRM, DBG_LVL_ALL, "Bus is in suspended states hence RDM not allowed..");
		return -EACCES;
	}
=======
	err = adapter_err_occurred(psIntfAdapter);
	if (err)
		return err;
>>>>>>> v3.18
=======
	err = adapter_err_occurred(psIntfAdapter);
	if (err)
		return err;
>>>>>>> v3.18

	psIntfAdapter->psAdapter->DeviceAccess = TRUE;

	retval = usb_control_msg(psIntfAdapter->udev,
				usb_sndctrlpipe(psIntfAdapter->udev, 0),
				0x01,
				0x42,
				(addr & 0xFFFF),
				((addr >> 16) & 0xFFFF),
				buff,
				len,
				5000);

	if (-ENODEV == retval)
		psIntfAdapter->psAdapter->device_removed = TRUE;

	if (retval < 0)	{
<<<<<<< HEAD
<<<<<<< HEAD
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, WRM, DBG_LVL_ALL, "WRM failed status :%d", retval);
		psIntfAdapter->psAdapter->DeviceAccess = FALSE;
		return retval;
	} else {
		psIntfAdapter->psAdapter->DeviceAccess = FALSE;
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, WRM, DBG_LVL_ALL, "WRM sent %d", retval);
=======
=======
>>>>>>> v3.18
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, WRM,
				DBG_LVL_ALL, "WRM failed status :%d", retval);
		psIntfAdapter->psAdapter->DeviceAccess = false;
		return retval;
	} else {
		psIntfAdapter->psAdapter->DeviceAccess = false;
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, WRM,
				DBG_LVL_ALL, "WRM sent %d", retval);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return STATUS_SUCCESS;
	}
}

int BcmRDM(void *arg,
	unsigned int addr,
	void *buff,
	int len)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return InterfaceRDM((struct bcm_interface_adapter*)arg, addr, buff, len);
=======
	return InterfaceRDM((struct bcm_interface_adapter *)arg, addr, buff,
			    len);
>>>>>>> v3.18
=======
	return InterfaceRDM((struct bcm_interface_adapter *)arg, addr, buff,
			    len);
>>>>>>> v3.18
}

int BcmWRM(void *arg,
	unsigned int addr,
	void *buff,
	int len)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return InterfaceWRM((struct bcm_interface_adapter *)arg, addr, buff, len);
=======
	return InterfaceWRM((struct bcm_interface_adapter *)arg, addr, buff,
			    len);
>>>>>>> v3.18
=======
	return InterfaceWRM((struct bcm_interface_adapter *)arg, addr, buff,
			    len);
>>>>>>> v3.18
}

int Bcm_clear_halt_of_endpoints(struct bcm_mini_adapter *Adapter)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct bcm_interface_adapter *psIntfAdapter = (struct bcm_interface_adapter *)(Adapter->pvInterfaceAdapter);
=======
	struct bcm_interface_adapter *psIntfAdapter =
		(struct bcm_interface_adapter *)(Adapter->pvInterfaceAdapter);
>>>>>>> v3.18
=======
	struct bcm_interface_adapter *psIntfAdapter =
		(struct bcm_interface_adapter *)(Adapter->pvInterfaceAdapter);
>>>>>>> v3.18
	int status = STATUS_SUCCESS;

	/*
	 * usb_clear_halt - tells device to clear endpoint halt/stall condition
	 * @dev: device whose endpoint is halted
	 * @pipe: endpoint "pipe" being cleared
	 * @ Context: !in_interrupt ()
	 *
<<<<<<< HEAD
<<<<<<< HEAD
	 * usb_clear_halt is the synchrnous call and returns 0 on success else returns with error code.
	 * This is used to clear halt conditions for bulk and interrupt endpoints only.
	 * Control and isochronous endpoints never halts.
	 *
	 * Any URBs  queued for such an endpoint should normally be unlinked by the driver
	 * before clearing the halt condition.
=======
=======
>>>>>>> v3.18
	 * usb_clear_halt is the synchrnous call and returns 0 on success else
	 * returns with error code.
	 * This is used to clear halt conditions for bulk and interrupt
	 * endpoints only.
	 * Control and isochronous endpoints never halts.
	 *
	 * Any URBs  queued for such an endpoint should normally be unlinked by
	 * the driver before clearing the halt condition.
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	 *
	 */

	/* Killing all the submitted urbs to different end points. */
	Bcm_kill_all_URBs(psIntfAdapter);

	/* clear the halted/stalled state for every end point */
<<<<<<< HEAD
<<<<<<< HEAD
	status = usb_clear_halt(psIntfAdapter->udev, psIntfAdapter->sIntrIn.int_in_pipe);
	if (status != STATUS_SUCCESS)
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, INTF_INIT, DBG_LVL_ALL, "Unable to Clear Halt of Interrupt IN end point. :%d ", status);

	status = usb_clear_halt(psIntfAdapter->udev, psIntfAdapter->sBulkIn.bulk_in_pipe);
	if (status != STATUS_SUCCESS)
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, INTF_INIT, DBG_LVL_ALL, "Unable to Clear Halt of Bulk IN end point. :%d ", status);

	status = usb_clear_halt(psIntfAdapter->udev, psIntfAdapter->sBulkOut.bulk_out_pipe);
	if (status != STATUS_SUCCESS)
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, INTF_INIT, DBG_LVL_ALL, "Unable to Clear Halt of Bulk OUT end point. :%d ", status);
=======
=======
>>>>>>> v3.18
	status = usb_clear_halt(psIntfAdapter->udev,
				psIntfAdapter->sIntrIn.int_in_pipe);
	if (status != STATUS_SUCCESS)
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, INTF_INIT,
				DBG_LVL_ALL,
				"Unable to Clear Halt of Interrupt IN end point. :%d ",
				status);

	status = usb_clear_halt(psIntfAdapter->udev,
				psIntfAdapter->sBulkIn.bulk_in_pipe);
	if (status != STATUS_SUCCESS)
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, INTF_INIT,
				DBG_LVL_ALL,
				"Unable to Clear Halt of Bulk IN end point. :%d ",
				status);

	status = usb_clear_halt(psIntfAdapter->udev,
				psIntfAdapter->sBulkOut.bulk_out_pipe);
	if (status != STATUS_SUCCESS)
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, INTF_INIT,
				DBG_LVL_ALL,
				"Unable to Clear Halt of Bulk OUT end point. :%d ",
				status);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return status;
}

void Bcm_kill_all_URBs(struct bcm_interface_adapter *psIntfAdapter)
{
	struct urb *tempUrb = NULL;
	unsigned int i;

	/*
	 * usb_kill_urb - cancel a transfer request and wait for it to finish
	 * @urb: pointer to URB describing a previously submitted request,
	 * returns nothing as it is void returned API.
	 *
	 * This routine cancels an in-progress request. It is guaranteed that
	 * upon return all completion handlers will have finished and the URB
	 * will be totally idle and available for reuse
	 *
<<<<<<< HEAD
<<<<<<< HEAD
	 * This routine may not be used in an interrupt context (such as a bottom
	 * half or a completion handler), or when holding a spinlock, or in other
	 * situations where the caller can't schedule().
=======
	 * This routine may not be used in an interrupt context (such as a
	 * bottom half or a completion handler), or when holding a spinlock, or
	 * in other situations where the caller can't schedule().
>>>>>>> v3.18
=======
	 * This routine may not be used in an interrupt context (such as a
	 * bottom half or a completion handler), or when holding a spinlock, or
	 * in other situations where the caller can't schedule().
>>>>>>> v3.18
	 *
	 */

	/* Cancel submitted Interrupt-URB's */
	if (psIntfAdapter->psInterruptUrb) {
		if (psIntfAdapter->psInterruptUrb->status == -EINPROGRESS)
			usb_kill_urb(psIntfAdapter->psInterruptUrb);
	}

	/* Cancel All submitted TX URB's */
	for (i = 0; i < MAXIMUM_USB_TCB; i++) {
		tempUrb = psIntfAdapter->asUsbTcb[i].urb;
		if (tempUrb) {
			if (tempUrb->status == -EINPROGRESS)
				usb_kill_urb(tempUrb);
		}
	}

	for (i = 0; i < MAXIMUM_USB_RCB; i++) {
		tempUrb = psIntfAdapter->asUsbRcb[i].urb;
		if (tempUrb) {
			if (tempUrb->status == -EINPROGRESS)
				usb_kill_urb(tempUrb);
		}
	}

	atomic_set(&psIntfAdapter->uNumTcbUsed, 0);
	atomic_set(&psIntfAdapter->uCurrTcb, 0);

	atomic_set(&psIntfAdapter->uNumRcbUsed, 0);
	atomic_set(&psIntfAdapter->uCurrRcb, 0);
}

void putUsbSuspend(struct work_struct *work)
{
	struct bcm_interface_adapter *psIntfAdapter = NULL;
	struct usb_interface *intf = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
	psIntfAdapter = container_of(work, struct bcm_interface_adapter, usbSuspendWork);
	intf = psIntfAdapter->interface;

	if (psIntfAdapter->bSuspended == FALSE)
=======
=======
>>>>>>> v3.18

	psIntfAdapter = container_of(work, struct bcm_interface_adapter,
				     usbSuspendWork);
	intf = psIntfAdapter->interface;

	if (psIntfAdapter->bSuspended == false)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		usb_autopm_put_interface(intf);
}

