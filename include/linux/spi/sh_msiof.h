#ifndef __SPI_SH_MSIOF_H__
#define __SPI_SH_MSIOF_H__

struct sh_msiof_spi_info {
	int tx_fifo_override;
	int rx_fifo_override;
	u16 num_chipselect;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	unsigned int dma_tx_id;
	unsigned int dma_rx_id;
>>>>>>> v3.18
=======
	unsigned int dma_tx_id;
	unsigned int dma_rx_id;
>>>>>>> v3.18
};

#endif /* __SPI_SH_MSIOF_H__ */
