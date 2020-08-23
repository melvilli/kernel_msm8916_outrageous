#ifndef __NET_TC_CSUM_H
#define __NET_TC_CSUM_H

#include <linux/types.h>
#include <net/act_api.h>

struct tcf_csum {
	struct tcf_common common;

	u32 update_flags;
};
<<<<<<< HEAD
<<<<<<< HEAD
#define to_tcf_csum(pc) \
	container_of(pc,struct tcf_csum,common)
=======
#define to_tcf_csum(a) \
	container_of(a->priv,struct tcf_csum,common)
>>>>>>> v3.18
=======
#define to_tcf_csum(a) \
	container_of(a->priv,struct tcf_csum,common)
>>>>>>> v3.18

#endif /* __NET_TC_CSUM_H */
