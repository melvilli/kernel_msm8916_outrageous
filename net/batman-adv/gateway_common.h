<<<<<<< HEAD
<<<<<<< HEAD
/* Copyright (C) 2009-2013 B.A.T.M.A.N. contributors:
=======
/* Copyright (C) 2009-2014 B.A.T.M.A.N. contributors:
>>>>>>> v3.18
=======
/* Copyright (C) 2009-2014 B.A.T.M.A.N. contributors:
>>>>>>> v3.18
 *
 * Marek Lindner
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
<<<<<<< HEAD
<<<<<<< HEAD
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
=======
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
>>>>>>> v3.18
=======
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
>>>>>>> v3.18
 */

#ifndef _NET_BATMAN_ADV_GATEWAY_COMMON_H_
#define _NET_BATMAN_ADV_GATEWAY_COMMON_H_

enum batadv_gw_modes {
	BATADV_GW_MODE_OFF,
	BATADV_GW_MODE_CLIENT,
	BATADV_GW_MODE_SERVER,
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
/**
 * enum batadv_bandwidth_units - bandwidth unit types
 * @BATADV_BW_UNIT_KBIT: unit type kbit
 * @BATADV_BW_UNIT_MBIT: unit type mbit
 */
enum batadv_bandwidth_units {
	BATADV_BW_UNIT_KBIT,
	BATADV_BW_UNIT_MBIT,
};

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#define BATADV_GW_MODE_OFF_NAME	"off"
#define BATADV_GW_MODE_CLIENT_NAME	"client"
#define BATADV_GW_MODE_SERVER_NAME	"server"

<<<<<<< HEAD
<<<<<<< HEAD
void batadv_gw_bandwidth_to_kbit(uint8_t gw_class, int *down, int *up);
ssize_t batadv_gw_bandwidth_set(struct net_device *net_dev, char *buff,
				size_t count);
=======
=======
>>>>>>> v3.18
ssize_t batadv_gw_bandwidth_set(struct net_device *net_dev, char *buff,
				size_t count);
void batadv_gw_tvlv_container_update(struct batadv_priv *bat_priv);
void batadv_gw_init(struct batadv_priv *bat_priv);
void batadv_gw_free(struct batadv_priv *bat_priv);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

#endif /* _NET_BATMAN_ADV_GATEWAY_COMMON_H_ */
