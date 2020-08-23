#include <net/tcp.h>
#include <net/tcp_memcontrol.h>
#include <net/sock.h>
#include <net/ip.h>
#include <linux/nsproxy.h>
#include <linux/memcontrol.h>
#include <linux/module.h>

<<<<<<< HEAD
<<<<<<< HEAD
static inline struct tcp_memcontrol *tcp_from_cgproto(struct cg_proto *cg_proto)
{
	return container_of(cg_proto, struct tcp_memcontrol, cg_proto);
}

static void memcg_tcp_enter_memory_pressure(struct sock *sk)
{
	if (sk->sk_cgrp->memory_pressure)
		*sk->sk_cgrp->memory_pressure = 1;
}
EXPORT_SYMBOL(memcg_tcp_enter_memory_pressure);

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
int tcp_init_cgroup(struct mem_cgroup *memcg, struct cgroup_subsys *ss)
{
	/*
	 * The root cgroup does not use res_counters, but rather,
	 * rely on the data already collected by the network
	 * subsystem
	 */
	struct res_counter *res_parent = NULL;
	struct cg_proto *cg_proto, *parent_cg;
<<<<<<< HEAD
<<<<<<< HEAD
	struct tcp_memcontrol *tcp;
	struct mem_cgroup *parent = parent_mem_cgroup(memcg);
	struct net *net = current->nsproxy->net_ns;
=======
	struct mem_cgroup *parent = parent_mem_cgroup(memcg);
>>>>>>> v3.18
=======
	struct mem_cgroup *parent = parent_mem_cgroup(memcg);
>>>>>>> v3.18

	cg_proto = tcp_prot.proto_cgroup(memcg);
	if (!cg_proto)
		return 0;

<<<<<<< HEAD
<<<<<<< HEAD
	tcp = tcp_from_cgproto(cg_proto);

	tcp->tcp_prot_mem[0] = net->ipv4.sysctl_tcp_mem[0];
	tcp->tcp_prot_mem[1] = net->ipv4.sysctl_tcp_mem[1];
	tcp->tcp_prot_mem[2] = net->ipv4.sysctl_tcp_mem[2];
	tcp->tcp_memory_pressure = 0;

	parent_cg = tcp_prot.proto_cgroup(parent);
	if (parent_cg)
		res_parent = parent_cg->memory_allocated;

	res_counter_init(&tcp->tcp_memory_allocated, res_parent);
	percpu_counter_init(&tcp->tcp_sockets_allocated, 0);

	cg_proto->enter_memory_pressure = memcg_tcp_enter_memory_pressure;
	cg_proto->memory_pressure = &tcp->tcp_memory_pressure;
	cg_proto->sysctl_mem = tcp->tcp_prot_mem;
	cg_proto->memory_allocated = &tcp->tcp_memory_allocated;
	cg_proto->sockets_allocated = &tcp->tcp_sockets_allocated;
	cg_proto->memcg = memcg;
=======
=======
>>>>>>> v3.18
	cg_proto->sysctl_mem[0] = sysctl_tcp_mem[0];
	cg_proto->sysctl_mem[1] = sysctl_tcp_mem[1];
	cg_proto->sysctl_mem[2] = sysctl_tcp_mem[2];
	cg_proto->memory_pressure = 0;
	cg_proto->memcg = memcg;

	parent_cg = tcp_prot.proto_cgroup(parent);
	if (parent_cg)
		res_parent = &parent_cg->memory_allocated;

	res_counter_init(&cg_proto->memory_allocated, res_parent);
	percpu_counter_init(&cg_proto->sockets_allocated, 0, GFP_KERNEL);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return 0;
}
EXPORT_SYMBOL(tcp_init_cgroup);

void tcp_destroy_cgroup(struct mem_cgroup *memcg)
{
	struct cg_proto *cg_proto;
<<<<<<< HEAD
<<<<<<< HEAD
	struct tcp_memcontrol *tcp;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18

	cg_proto = tcp_prot.proto_cgroup(memcg);
	if (!cg_proto)
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	tcp = tcp_from_cgproto(cg_proto);
	percpu_counter_destroy(&tcp->tcp_sockets_allocated);
=======
	percpu_counter_destroy(&cg_proto->sockets_allocated);
>>>>>>> v3.18
=======
	percpu_counter_destroy(&cg_proto->sockets_allocated);
>>>>>>> v3.18
}
EXPORT_SYMBOL(tcp_destroy_cgroup);

static int tcp_update_limit(struct mem_cgroup *memcg, u64 val)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct net *net = current->nsproxy->net_ns;
	struct tcp_memcontrol *tcp;
	struct cg_proto *cg_proto;
	u64 old_lim;
=======
	struct cg_proto *cg_proto;
>>>>>>> v3.18
=======
	struct cg_proto *cg_proto;
>>>>>>> v3.18
	int i;
	int ret;

	cg_proto = tcp_prot.proto_cgroup(memcg);
	if (!cg_proto)
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	if (val > RESOURCE_MAX)
		val = RESOURCE_MAX;

	tcp = tcp_from_cgproto(cg_proto);

	old_lim = res_counter_read_u64(&tcp->tcp_memory_allocated, RES_LIMIT);
	ret = res_counter_set_limit(&tcp->tcp_memory_allocated, val);
=======
=======
>>>>>>> v3.18
	if (val > RES_COUNTER_MAX)
		val = RES_COUNTER_MAX;

	ret = res_counter_set_limit(&cg_proto->memory_allocated, val);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (ret)
		return ret;

	for (i = 0; i < 3; i++)
<<<<<<< HEAD
<<<<<<< HEAD
		tcp->tcp_prot_mem[i] = min_t(long, val >> PAGE_SHIFT,
					     net->ipv4.sysctl_tcp_mem[i]);

	if (val == RESOURCE_MAX)
		clear_bit(MEMCG_SOCK_ACTIVE, &cg_proto->flags);
	else if (val != RESOURCE_MAX) {
=======
=======
>>>>>>> v3.18
		cg_proto->sysctl_mem[i] = min_t(long, val >> PAGE_SHIFT,
						sysctl_tcp_mem[i]);

	if (val == RES_COUNTER_MAX)
		clear_bit(MEMCG_SOCK_ACTIVE, &cg_proto->flags);
	else if (val != RES_COUNTER_MAX) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		/*
		 * The active bit needs to be written after the static_key
		 * update. This is what guarantees that the socket activation
		 * function is the last one to run. See sock_update_memcg() for
		 * details, and note that we don't mark any socket as belonging
		 * to this memcg until that flag is up.
		 *
		 * We need to do this, because static_keys will span multiple
		 * sites, but we can't control their order. If we mark a socket
		 * as accounted, but the accounting functions are not patched in
		 * yet, we'll lose accounting.
		 *
		 * We never race with the readers in sock_update_memcg(),
		 * because when this value change, the code to process it is not
		 * patched in yet.
		 *
		 * The activated bit is used to guarantee that no two writers
		 * will do the update in the same memcg. Without that, we can't
		 * properly shutdown the static key.
		 */
		if (!test_and_set_bit(MEMCG_SOCK_ACTIVATED, &cg_proto->flags))
			static_key_slow_inc(&memcg_socket_limit_enabled);
		set_bit(MEMCG_SOCK_ACTIVE, &cg_proto->flags);
	}

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int tcp_cgroup_write(struct cgroup *cont, struct cftype *cft,
			    const char *buffer)
{
	struct mem_cgroup *memcg = mem_cgroup_from_cont(cont);
	unsigned long long val;
	int ret = 0;

	switch (cft->private) {
	case RES_LIMIT:
		/* see memcontrol.c */
		ret = res_counter_memparse_write_strategy(buffer, &val);
=======
=======
>>>>>>> v3.18
static ssize_t tcp_cgroup_write(struct kernfs_open_file *of,
				char *buf, size_t nbytes, loff_t off)
{
	struct mem_cgroup *memcg = mem_cgroup_from_css(of_css(of));
	unsigned long long val;
	int ret = 0;

	buf = strstrip(buf);

	switch (of_cft(of)->private) {
	case RES_LIMIT:
		/* see memcontrol.c */
		ret = res_counter_memparse_write_strategy(buf, &val);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		if (ret)
			break;
		ret = tcp_update_limit(memcg, val);
		break;
	default:
		ret = -EINVAL;
		break;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	return ret;
=======
	return ret ?: nbytes;
>>>>>>> v3.18
=======
	return ret ?: nbytes;
>>>>>>> v3.18
}

static u64 tcp_read_stat(struct mem_cgroup *memcg, int type, u64 default_val)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct tcp_memcontrol *tcp;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	struct cg_proto *cg_proto;

	cg_proto = tcp_prot.proto_cgroup(memcg);
	if (!cg_proto)
		return default_val;

<<<<<<< HEAD
<<<<<<< HEAD
	tcp = tcp_from_cgproto(cg_proto);
	return res_counter_read_u64(&tcp->tcp_memory_allocated, type);
=======
	return res_counter_read_u64(&cg_proto->memory_allocated, type);
>>>>>>> v3.18
=======
	return res_counter_read_u64(&cg_proto->memory_allocated, type);
>>>>>>> v3.18
}

static u64 tcp_read_usage(struct mem_cgroup *memcg)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct tcp_memcontrol *tcp;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	struct cg_proto *cg_proto;

	cg_proto = tcp_prot.proto_cgroup(memcg);
	if (!cg_proto)
		return atomic_long_read(&tcp_memory_allocated) << PAGE_SHIFT;

<<<<<<< HEAD
<<<<<<< HEAD
	tcp = tcp_from_cgproto(cg_proto);
	return res_counter_read_u64(&tcp->tcp_memory_allocated, RES_USAGE);
}

static u64 tcp_cgroup_read(struct cgroup *cont, struct cftype *cft)
{
	struct mem_cgroup *memcg = mem_cgroup_from_cont(cont);
=======
=======
>>>>>>> v3.18
	return res_counter_read_u64(&cg_proto->memory_allocated, RES_USAGE);
}

static u64 tcp_cgroup_read(struct cgroup_subsys_state *css, struct cftype *cft)
{
	struct mem_cgroup *memcg = mem_cgroup_from_css(css);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	u64 val;

	switch (cft->private) {
	case RES_LIMIT:
<<<<<<< HEAD
<<<<<<< HEAD
		val = tcp_read_stat(memcg, RES_LIMIT, RESOURCE_MAX);
=======
		val = tcp_read_stat(memcg, RES_LIMIT, RES_COUNTER_MAX);
>>>>>>> v3.18
=======
		val = tcp_read_stat(memcg, RES_LIMIT, RES_COUNTER_MAX);
>>>>>>> v3.18
		break;
	case RES_USAGE:
		val = tcp_read_usage(memcg);
		break;
	case RES_FAILCNT:
	case RES_MAX_USAGE:
		val = tcp_read_stat(memcg, cft->private, 0);
		break;
	default:
		BUG();
	}
	return val;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int tcp_cgroup_reset(struct cgroup *cont, unsigned int event)
{
	struct mem_cgroup *memcg;
	struct tcp_memcontrol *tcp;
	struct cg_proto *cg_proto;

	memcg = mem_cgroup_from_cont(cont);
	cg_proto = tcp_prot.proto_cgroup(memcg);
	if (!cg_proto)
		return 0;
	tcp = tcp_from_cgproto(cg_proto);

	switch (event) {
	case RES_MAX_USAGE:
		res_counter_reset_max(&tcp->tcp_memory_allocated);
		break;
	case RES_FAILCNT:
		res_counter_reset_failcnt(&tcp->tcp_memory_allocated);
		break;
	}

	return 0;
}

unsigned long long tcp_max_memory(const struct mem_cgroup *memcg)
{
	struct tcp_memcontrol *tcp;
	struct cg_proto *cg_proto;

	cg_proto = tcp_prot.proto_cgroup((struct mem_cgroup *)memcg);
	if (!cg_proto)
		return 0;

	tcp = tcp_from_cgproto(cg_proto);
	return res_counter_read_u64(&tcp->tcp_memory_allocated, RES_LIMIT);
}

void tcp_prot_mem(struct mem_cgroup *memcg, long val, int idx)
{
	struct tcp_memcontrol *tcp;
	struct cg_proto *cg_proto;

	cg_proto = tcp_prot.proto_cgroup(memcg);
	if (!cg_proto)
		return;

	tcp = tcp_from_cgproto(cg_proto);

	tcp->tcp_prot_mem[idx] = val;
=======
=======
>>>>>>> v3.18
static ssize_t tcp_cgroup_reset(struct kernfs_open_file *of,
				char *buf, size_t nbytes, loff_t off)
{
	struct mem_cgroup *memcg;
	struct cg_proto *cg_proto;

	memcg = mem_cgroup_from_css(of_css(of));
	cg_proto = tcp_prot.proto_cgroup(memcg);
	if (!cg_proto)
		return nbytes;

	switch (of_cft(of)->private) {
	case RES_MAX_USAGE:
		res_counter_reset_max(&cg_proto->memory_allocated);
		break;
	case RES_FAILCNT:
		res_counter_reset_failcnt(&cg_proto->memory_allocated);
		break;
	}

	return nbytes;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static struct cftype tcp_files[] = {
	{
		.name = "kmem.tcp.limit_in_bytes",
<<<<<<< HEAD
<<<<<<< HEAD
		.write_string = tcp_cgroup_write,
=======
		.write = tcp_cgroup_write,
>>>>>>> v3.18
=======
		.write = tcp_cgroup_write,
>>>>>>> v3.18
		.read_u64 = tcp_cgroup_read,
		.private = RES_LIMIT,
	},
	{
		.name = "kmem.tcp.usage_in_bytes",
		.read_u64 = tcp_cgroup_read,
		.private = RES_USAGE,
	},
	{
		.name = "kmem.tcp.failcnt",
		.private = RES_FAILCNT,
<<<<<<< HEAD
<<<<<<< HEAD
		.trigger = tcp_cgroup_reset,
=======
		.write = tcp_cgroup_reset,
>>>>>>> v3.18
=======
		.write = tcp_cgroup_reset,
>>>>>>> v3.18
		.read_u64 = tcp_cgroup_read,
	},
	{
		.name = "kmem.tcp.max_usage_in_bytes",
		.private = RES_MAX_USAGE,
<<<<<<< HEAD
<<<<<<< HEAD
		.trigger = tcp_cgroup_reset,
=======
		.write = tcp_cgroup_reset,
>>>>>>> v3.18
=======
		.write = tcp_cgroup_reset,
>>>>>>> v3.18
		.read_u64 = tcp_cgroup_read,
	},
	{ }	/* terminate */
};

static int __init tcp_memcontrol_init(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	WARN_ON(cgroup_add_cftypes(&mem_cgroup_subsys, tcp_files));
=======
	WARN_ON(cgroup_add_legacy_cftypes(&memory_cgrp_subsys, tcp_files));
>>>>>>> v3.18
=======
	WARN_ON(cgroup_add_legacy_cftypes(&memory_cgrp_subsys, tcp_files));
>>>>>>> v3.18
	return 0;
}
__initcall(tcp_memcontrol_init);
