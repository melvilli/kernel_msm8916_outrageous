/*
 * Copyright (c) 2012 GCT Semiconductor, Inc. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

<<<<<<< HEAD
#ifndef __GDM_SDIO_H__
#define __GDM_SDIO_H__
=======
#ifndef __GDM72XX_GDM_SDIO_H__
#define __GDM72XX_GDM_SDIO_H__
>>>>>>> v3.18

#include <linux/types.h>
#include <linux/time.h>

#define MAX_NR_SDU_BUF  64

struct sdio_tx {
	struct list_head	list;
	struct tx_cxt		*tx_cxt;
<<<<<<< HEAD

	u8	*buf;
	int	len;

=======
	u8			*buf;
	int			len;
>>>>>>> v3.18
	void (*callback)(void *cb_data);
	void *cb_data;
};

struct tx_cxt {
	struct list_head	free_list;
	struct list_head	sdu_list;
	struct list_head	hci_list;
	struct timeval		sdu_stamp;
<<<<<<< HEAD

	u8	*sdu_buf;

	spinlock_t			lock;
	int	can_send;
	int stop_sdu_tx;
=======
	u8			*sdu_buf;
	spinlock_t		lock;
	int			can_send;
	int			stop_sdu_tx;
>>>>>>> v3.18
};

struct sdio_rx {
	struct list_head	list;
	struct rx_cxt		*rx_cxt;
<<<<<<< HEAD

=======
>>>>>>> v3.18
	void (*callback)(void *cb_data, void *data, int len);
	void *cb_data;
};

struct rx_cxt {
	struct list_head	free_list;
	struct list_head	req_list;
<<<<<<< HEAD

	u8		*rx_buf;

	spinlock_t			lock;
=======
	u8			*rx_buf;
	spinlock_t		lock;
>>>>>>> v3.18
};

struct sdiowm_dev {
	struct sdio_func	*func;
<<<<<<< HEAD

	struct tx_cxt	tx;
	struct rx_cxt	rx;

	struct work_struct	ws;
};

#endif /* __GDM_SDIO_H__ */
=======
	struct tx_cxt		tx;
	struct rx_cxt		rx;
	struct work_struct	ws;
};

#endif /* __GDM72XX_GDM_SDIO_H__ */
>>>>>>> v3.18
