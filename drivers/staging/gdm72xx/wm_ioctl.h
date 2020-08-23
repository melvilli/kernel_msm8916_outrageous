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
<<<<<<< HEAD
#if !defined(WM_IOCTL_H_20080714)
#define WM_IOCTL_H_20080714
=======
#ifndef __GDM72XX_WM_IOCTL_H__
#define __GDM72XX_WM_IOCTL_H__

>>>>>>> v3.18
=======
#ifndef __GDM72XX_WM_IOCTL_H__
#define __GDM72XX_WM_IOCTL_H__

>>>>>>> v3.18
#if !defined(__KERNEL__)
#include <net/if.h>
#endif

#define NETLINK_WIMAX	31

<<<<<<< HEAD
<<<<<<< HEAD
#define SIOCWMIOCTL			SIOCDEVPRIVATE

#define SIOCG_DATA			0x8D10
#define SIOCS_DATA			0x8D11
=======
=======
>>>>>>> v3.18
#define SIOCWMIOCTL	SIOCDEVPRIVATE

#define SIOCG_DATA	0x8D10
#define SIOCS_DATA	0x8D11
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

enum {
	SIOC_DATA_FSM,
	SIOC_DATA_NETLIST,
	SIOC_DATA_CONNNSP,
	SIOC_DATA_CONNCOMP,
	SIOC_DATA_PROFILEID,

	SIOC_DATA_END
};

<<<<<<< HEAD
<<<<<<< HEAD
#define SIOC_DATA_MAX			16
=======
#define SIOC_DATA_MAX	16
>>>>>>> v3.18
=======
#define SIOC_DATA_MAX	16
>>>>>>> v3.18

/* FSM */
enum {
	M_INIT = 0,
	M_OPEN_OFF,
	M_OPEN_ON,
	M_SCAN,
	M_CONNECTING,
	M_CONNECTED,
	M_FSM_END,

	C_INIT = 0,
	C_CONNSTART,
	C_ASSOCSTART,
	C_RNG,
	C_SBC,
	C_AUTH,
	C_REG,
	C_DSX,
	C_ASSOCCOMPLETE,
	C_CONNCOMPLETE,
	C_FSM_END,

	D_INIT = 0,
	D_READY,
	D_LISTEN,
	D_IPACQUISITION,

	END_FSM
};

struct fsm_s {
<<<<<<< HEAD
<<<<<<< HEAD
	int		m_status;	/*main status*/
	int		c_status;	/*connection status*/
	int		d_status;	/*oma-dm status*/
};

struct data_s {
	int		size;
=======
=======
>>>>>>> v3.18
	int	m_status;	/*main status*/
	int	c_status;	/*connection status*/
	int	d_status;	/*oma-dm status*/
};

struct data_s {
	int	size;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	void	*buf;
};

struct wm_req_s {
	union {
<<<<<<< HEAD
<<<<<<< HEAD
		char	ifrn_name[IFNAMSIZ];
	} ifr_ifrn;

	unsigned short	cmd;

=======
		char ifrn_name[IFNAMSIZ];
	} ifr_ifrn;
	unsigned short	cmd;
>>>>>>> v3.18
=======
		char ifrn_name[IFNAMSIZ];
	} ifr_ifrn;
	unsigned short	cmd;
>>>>>>> v3.18
	unsigned short	data_id;
	struct data_s	data;

/* NOTE: sizeof(struct wm_req_s) must be less than sizeof(struct ifreq). */
};

#ifndef ifr_name
<<<<<<< HEAD
<<<<<<< HEAD
#define ifr_name	ifr_ifrn.ifrn_name
#endif

#endif
=======
=======
>>>>>>> v3.18
#define ifr_name ifr_ifrn.ifrn_name
#endif

#endif /* __GDM72XX_WM_IOCTL_H__ */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
