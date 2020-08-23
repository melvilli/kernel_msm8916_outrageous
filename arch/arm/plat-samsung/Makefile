# arch/arm/plat-samsung/Makefile
#
# Copyright 2009 Simtec Electronics
#
# Licensed under GPLv2

<<<<<<< HEAD
<<<<<<< HEAD
obj-y				:=
obj-m				:=
obj-n				:= dummy.o
obj-				:=
=======
ccflags-$(CONFIG_ARCH_MULTI_V7) += -I$(srctree)/$(src)/include
>>>>>>> v3.18
=======
ccflags-$(CONFIG_ARCH_MULTI_V7) += -I$(srctree)/$(src)/include
>>>>>>> v3.18

# Objects we always build independent of SoC choice

obj-y				+= init.o cpu.o
<<<<<<< HEAD
<<<<<<< HEAD
obj-$(CONFIG_SAMSUNG_HRT) 	+= samsung-time.o

obj-$(CONFIG_SAMSUNG_CLOCK)	+= clock.o
obj-$(CONFIG_SAMSUNG_CLOCK)	+= pwm-clock.o

obj-$(CONFIG_SAMSUNG_CLKSRC)	+= clock-clksrc.o
obj-$(CONFIG_S5P_CLOCK)		+= s5p-clock.o

obj-$(CONFIG_SAMSUNG_IRQ_VIC_TIMER) += irq-vic-timer.o
obj-$(CONFIG_S5P_IRQ)		+= s5p-irq.o
obj-$(CONFIG_S5P_EXT_INT)	+= s5p-irq-eint.o
obj-$(CONFIG_S5P_GPIO_INT)	+= s5p-irq-gpioint.o
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18

# ADC

obj-$(CONFIG_S3C_ADC)	+= adc.o

# devices

<<<<<<< HEAD
<<<<<<< HEAD
obj-y				+= platformdata.o

obj-y				+= devs.o
obj-y				+= dev-uart.o
obj-$(CONFIG_S5P_DEV_MFC)	+= s5p-dev-mfc.o
obj-$(CONFIG_S5P_DEV_UART)	+= s5p-dev-uart.o
=======
=======
>>>>>>> v3.18
obj-$(CONFIG_SAMSUNG_ATAGS)	+= platformdata.o

obj-$(CONFIG_SAMSUNG_ATAGS)	+= devs.o
obj-$(CONFIG_SAMSUNG_ATAGS)	+= dev-uart.o
obj-$(CONFIG_S5P_DEV_MFC)	+= s5p-dev-mfc.o
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

obj-$(CONFIG_SAMSUNG_DEV_BACKLIGHT)	+= dev-backlight.o

obj-$(CONFIG_S3C_SETUP_CAMIF)	+= setup-camif.o
<<<<<<< HEAD
<<<<<<< HEAD
obj-$(CONFIG_S5P_SETUP_MIPIPHY)	+= setup-mipiphy.o
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18

# DMA support

obj-$(CONFIG_S3C_DMA)		+= dma.o s3c-dma-ops.o

obj-$(CONFIG_SAMSUNG_DMADEV)	+= dma-ops.o

# PM support

<<<<<<< HEAD
<<<<<<< HEAD
obj-$(CONFIG_PM)		+= pm.o
obj-$(CONFIG_PM)		+= pm-gpio.o
obj-$(CONFIG_SAMSUNG_PM_CHECK)	+= pm-check.o

obj-$(CONFIG_SAMSUNG_WAKEMASK)	+= wakeup-mask.o

obj-$(CONFIG_S5P_PM)		+= s5p-pm.o s5p-irq-pm.o
obj-$(CONFIG_S5P_SLEEP)		+= s5p-sleep.o
=======
=======
>>>>>>> v3.18
obj-$(CONFIG_PM_SLEEP)		+= pm-common.o
obj-$(CONFIG_SAMSUNG_PM)	+= pm.o
obj-$(CONFIG_SAMSUNG_PM_GPIO)	+= pm-gpio.o
obj-$(CONFIG_SAMSUNG_PM_CHECK)	+= pm-check.o
obj-$(CONFIG_SAMSUNG_PM_DEBUG)	+= pm-debug.o

obj-$(CONFIG_SAMSUNG_WAKEMASK)	+= wakeup-mask.o
obj-$(CONFIG_SAMSUNG_WDT_RESET)	+= watchdog-reset.o
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
