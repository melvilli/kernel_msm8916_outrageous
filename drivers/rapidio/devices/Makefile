#
# Makefile for RapidIO devices
#

<<<<<<< HEAD
obj-$(CONFIG_RAPIDIO_TSI721)	+= tsi721.o
ifeq ($(CONFIG_RAPIDIO_DMA_ENGINE),y)
obj-$(CONFIG_RAPIDIO_TSI721)	+= tsi721_dma.o
endif
=======
obj-$(CONFIG_RAPIDIO_TSI721)	+= tsi721_mport.o
tsi721_mport-y			:= tsi721.o
tsi721_mport-$(CONFIG_RAPIDIO_DMA_ENGINE) += tsi721_dma.o
>>>>>>> v3.18
