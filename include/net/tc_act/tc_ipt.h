#ifndef __NET_TC_IPT_H
#define __NET_TC_IPT_H

#include <net/act_api.h>

struct xt_entry_target;

struct tcf_ipt {
	struct tcf_common	common;
	u32			tcfi_hook;
	char			*tcfi_tname;
	struct xt_entry_target	*tcfi_t;
};
<<<<<<< HEAD
<<<<<<< HEAD
#define to_ipt(pc) \
	container_of(pc, struct tcf_ipt, common)
=======
#define to_ipt(a) \
	container_of(a->priv, struct tcf_ipt, common)
>>>>>>> v3.18
=======
#define to_ipt(a) \
	container_of(a->priv, struct tcf_ipt, common)
>>>>>>> v3.18

#endif /* __NET_TC_IPT_H */
