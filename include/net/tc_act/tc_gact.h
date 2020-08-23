#ifndef __NET_TC_GACT_H
#define __NET_TC_GACT_H

#include <net/act_api.h>

struct tcf_gact {
	struct tcf_common	common;
#ifdef CONFIG_GACT_PROB
        u16			tcfg_ptype;
        u16			tcfg_pval;
        int			tcfg_paction;
#endif
};
<<<<<<< HEAD
<<<<<<< HEAD
#define to_gact(pc) \
	container_of(pc, struct tcf_gact, common)
=======
#define to_gact(a) \
	container_of(a->priv, struct tcf_gact, common)
>>>>>>> v3.18
=======
#define to_gact(a) \
	container_of(a->priv, struct tcf_gact, common)
>>>>>>> v3.18

#endif /* __NET_TC_GACT_H */
