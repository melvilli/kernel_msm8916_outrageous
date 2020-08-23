<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> v3.18
#include <linux/pci.h>

#include "../comedidev.h"
#include "comedi_fc.h"
#include "amcc_s5933.h"

#include "addi-data/addi_common.h"

#include "addi-data/addi_eeprom.c"
#include "addi-data/hwdrv_apci1500.c"
#include "addi-data/addi_common.c"

static const struct addi_board apci1500_boardtypes[] = {
	{
		.pc_DriverName		= "apci1500",
		.i_IorangeBase1		= APCI1500_ADDRESS_RANGE,
<<<<<<< HEAD
		.i_PCIEeprom		= ADDIDATA_NO_EEPROM,
=======
		.i_PCIEeprom		= 0,
>>>>>>> v3.18
		.i_NbrDiChannel		= 16,
		.i_NbrDoChannel		= 16,
		.i_DoMaxdata		= 0xffff,
		.i_Timer		= 1,
<<<<<<< HEAD
		.interrupt		= v_APCI1500_Interrupt,
		.reset			= i_APCI1500_Reset,
		.di_config		= i_APCI1500_ConfigDigitalInputEvent,
		.di_read		= i_APCI1500_Initialisation,
		.di_write		= i_APCI1500_StartStopInputEvent,
		.di_bits		= apci1500_di_insn_bits,
		.do_config		= i_APCI1500_ConfigDigitalOutputErrorInterrupt,
		.do_write		= i_APCI1500_WriteDigitalOutput,
		.do_bits		= i_APCI1500_ConfigureInterrupt,
		.timer_config		= i_APCI1500_ConfigCounterTimerWatchdog,
		.timer_write		= i_APCI1500_StartStopTriggerTimerCounterWatchdog,
		.timer_read		= i_APCI1500_ReadInterruptMask,
		.timer_bits		= i_APCI1500_ReadCounterTimerWatchdog,
=======
		.interrupt		= apci1500_interrupt,
		.reset			= apci1500_reset,
		.di_config		= apci1500_di_config,
		.di_read		= apci1500_di_read,
		.di_write		= apci1500_di_write,
		.di_bits		= apci1500_di_insn_bits,
		.do_config		= apci1500_do_config,
		.do_write		= apci1500_do_write,
		.do_bits		= apci1500_do_bits,
		.timer_config		= apci1500_timer_config,
		.timer_write		= apci1500_timer_write,
		.timer_read		= apci1500_timer_read,
		.timer_bits		= apci1500_timer_bits,
>>>>>>> v3.18
	},
};

static int apci1500_auto_attach(struct comedi_device *dev,
				unsigned long context)
{
	dev->board_ptr = &apci1500_boardtypes[0];

	return addi_auto_attach(dev, context);
}

static struct comedi_driver apci1500_driver = {
	.driver_name	= "addi_apci_1500",
	.module		= THIS_MODULE,
	.auto_attach	= apci1500_auto_attach,
	.detach		= i_ADDI_Detach,
};

static int apci1500_pci_probe(struct pci_dev *dev,
			      const struct pci_device_id *id)
{
	return comedi_pci_auto_config(dev, &apci1500_driver, id->driver_data);
}

<<<<<<< HEAD
static DEFINE_PCI_DEVICE_TABLE(apci1500_pci_table) = {
=======
static const struct pci_device_id apci1500_pci_table[] = {
>>>>>>> v3.18
	{ PCI_DEVICE(PCI_VENDOR_ID_AMCC, 0x80fc) },
	{ 0 }
};
MODULE_DEVICE_TABLE(pci, apci1500_pci_table);

static struct pci_driver apci1500_pci_driver = {
	.name		= "addi_apci_1500",
	.id_table	= apci1500_pci_table,
	.probe		= apci1500_pci_probe,
	.remove		= comedi_pci_auto_unconfig,
};
module_comedi_pci_driver(apci1500_driver, apci1500_pci_driver);

MODULE_AUTHOR("Comedi http://www.comedi.org");
<<<<<<< HEAD
MODULE_DESCRIPTION("Comedi low-level driver");
=======
MODULE_DESCRIPTION("ADDI-DATA APCI-1500, 16 channel DI / 16 channel DO boards");
>>>>>>> v3.18
MODULE_LICENSE("GPL");
