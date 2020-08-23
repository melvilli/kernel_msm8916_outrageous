<<<<<<< HEAD
<<<<<<< HEAD
obj-y				+= setup.o nlm_hal.o
obj-$(CONFIG_SMP)		+= wakeup.o
obj-$(CONFIG_USB)		+= usb-init.o
=======
=======
>>>>>>> v3.18
obj-y				+= setup.o nlm_hal.o cop2-ex.o dt.o
obj-$(CONFIG_SMP)		+= wakeup.o
ifdef CONFIG_USB
obj-y				+= usb-init.o
obj-y				+= usb-init-xlp2.o
endif
ifdef CONFIG_SATA_AHCI
obj-y				+= ahci-init.o
obj-y				+= ahci-init-xlp2.o
endif
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
