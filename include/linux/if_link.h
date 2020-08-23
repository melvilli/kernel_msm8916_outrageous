#ifndef _LINUX_IF_LINK_H
#define _LINUX_IF_LINK_H

#include <uapi/linux/if_link.h>


/* We don't want this structure exposed to user space */
struct ifla_vf_info {
	__u32 vf;
	__u8 mac[32];
	__u32 vlan;
	__u32 qos;
<<<<<<< HEAD
<<<<<<< HEAD
	__u32 tx_rate;
	__u32 spoofchk;
=======
=======
>>>>>>> v3.18
	__u32 spoofchk;
	__u32 linkstate;
	__u32 min_tx_rate;
	__u32 max_tx_rate;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
};
#endif /* _LINUX_IF_LINK_H */
