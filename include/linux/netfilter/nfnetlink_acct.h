#ifndef _NFNL_ACCT_H_
#define _NFNL_ACCT_H_

#include <uapi/linux/netfilter/nfnetlink_acct.h>

<<<<<<< HEAD
<<<<<<< HEAD

struct nf_acct;

extern struct nf_acct *nfnl_acct_find_get(const char *filter_name);
extern void nfnl_acct_put(struct nf_acct *acct);
extern void nfnl_acct_update(const struct sk_buff *skb, struct nf_acct *nfacct);

=======
=======
>>>>>>> v3.18
enum {
	NFACCT_NO_QUOTA		= -1,
	NFACCT_UNDERQUOTA,
	NFACCT_OVERQUOTA,
};

struct nf_acct;

struct nf_acct *nfnl_acct_find_get(const char *filter_name);
void nfnl_acct_put(struct nf_acct *acct);
void nfnl_acct_update(const struct sk_buff *skb, struct nf_acct *nfacct);
extern int nfnl_acct_overquota(const struct sk_buff *skb,
			      struct nf_acct *nfacct);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif /* _NFNL_ACCT_H */
