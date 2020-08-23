/*
 * irqchip.c: Common API for in kernel interrupt controllers
 * Copyright (c) 2007, Intel Corporation.
 * Copyright 2010 Red Hat, Inc. and/or its affiliates.
 * Copyright (c) 2013, Alexander Graf <agraf@suse.de>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 * This file is derived from virt/kvm/irq_comm.c.
 *
 * Authors:
 *   Yaozu (Eddie) Dong <Eddie.dong@intel.com>
 *   Alexander Graf <agraf@suse.de>
 */

#include <linux/kvm_host.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/srcu.h>
>>>>>>> v3.18
=======
#include <linux/srcu.h>
>>>>>>> v3.18
#include <linux/export.h>
#include <trace/events/kvm.h>
#include "irq.h"

<<<<<<< HEAD
<<<<<<< HEAD
bool kvm_irq_has_notifier(struct kvm *kvm, unsigned irqchip, unsigned pin)
{
	struct kvm_irq_ack_notifier *kian;
	int gsi;

	rcu_read_lock();
	gsi = rcu_dereference(kvm->irq_routing)->chip[irqchip][pin];
	if (gsi != -1)
		hlist_for_each_entry_rcu(kian, &kvm->irq_ack_notifier_list,
					 link)
			if (kian->gsi == gsi) {
				rcu_read_unlock();
				return true;
			}

	rcu_read_unlock();

	return false;
}
EXPORT_SYMBOL_GPL(kvm_irq_has_notifier);

void kvm_notify_acked_irq(struct kvm *kvm, unsigned irqchip, unsigned pin)
{
	struct kvm_irq_ack_notifier *kian;
	int gsi;

	trace_kvm_ack_irq(irqchip, pin);

	rcu_read_lock();
	gsi = rcu_dereference(kvm->irq_routing)->chip[irqchip][pin];
	if (gsi != -1)
		hlist_for_each_entry_rcu(kian, &kvm->irq_ack_notifier_list,
					 link)
			if (kian->gsi == gsi)
				kian->irq_acked(kian);
	rcu_read_unlock();
}

void kvm_register_irq_ack_notifier(struct kvm *kvm,
				   struct kvm_irq_ack_notifier *kian)
{
	mutex_lock(&kvm->irq_lock);
	hlist_add_head_rcu(&kian->link, &kvm->irq_ack_notifier_list);
	mutex_unlock(&kvm->irq_lock);
#ifdef __KVM_HAVE_IOAPIC
	kvm_vcpu_request_scan_ioapic(kvm);
#endif
}

void kvm_unregister_irq_ack_notifier(struct kvm *kvm,
				    struct kvm_irq_ack_notifier *kian)
{
	mutex_lock(&kvm->irq_lock);
	hlist_del_init_rcu(&kian->link);
	mutex_unlock(&kvm->irq_lock);
	synchronize_rcu();
#ifdef __KVM_HAVE_IOAPIC
	kvm_vcpu_request_scan_ioapic(kvm);
#endif
=======
=======
>>>>>>> v3.18
struct kvm_irq_routing_table {
	int chip[KVM_NR_IRQCHIPS][KVM_IRQCHIP_NUM_PINS];
	struct kvm_kernel_irq_routing_entry *rt_entries;
	u32 nr_rt_entries;
	/*
	 * Array indexed by gsi. Each entry contains list of irq chips
	 * the gsi is connected to.
	 */
	struct hlist_head map[0];
};

int kvm_irq_map_gsi(struct kvm *kvm,
		    struct kvm_kernel_irq_routing_entry *entries, int gsi)
{
	struct kvm_irq_routing_table *irq_rt;
	struct kvm_kernel_irq_routing_entry *e;
	int n = 0;

	irq_rt = srcu_dereference_check(kvm->irq_routing, &kvm->irq_srcu,
					lockdep_is_held(&kvm->irq_lock));
	if (gsi < irq_rt->nr_rt_entries) {
		hlist_for_each_entry(e, &irq_rt->map[gsi], link) {
			entries[n] = *e;
			++n;
		}
	}

	return n;
}

int kvm_irq_map_chip_pin(struct kvm *kvm, unsigned irqchip, unsigned pin)
{
	struct kvm_irq_routing_table *irq_rt;

	irq_rt = srcu_dereference(kvm->irq_routing, &kvm->irq_srcu);
	return irq_rt->chip[irqchip][pin];
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

int kvm_send_userspace_msi(struct kvm *kvm, struct kvm_msi *msi)
{
	struct kvm_kernel_irq_routing_entry route;

	if (!irqchip_in_kernel(kvm) || msi->flags != 0)
		return -EINVAL;

	route.msi.address_lo = msi->address_lo;
	route.msi.address_hi = msi->address_hi;
	route.msi.data = msi->data;

	return kvm_set_msi(&route, kvm, KVM_USERSPACE_IRQ_SOURCE_ID, 1, false);
}

/*
 * Return value:
 *  < 0   Interrupt was ignored (masked or not delivered for other reasons)
 *  = 0   Interrupt was coalesced (previous irq is still pending)
 *  > 0   Number of CPUs interrupt was delivered to
 */
int kvm_set_irq(struct kvm *kvm, int irq_source_id, u32 irq, int level,
		bool line_status)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct kvm_kernel_irq_routing_entry *e, irq_set[KVM_NR_IRQCHIPS];
	int ret = -1, i = 0;
	struct kvm_irq_routing_table *irq_rt;
=======
	struct kvm_kernel_irq_routing_entry irq_set[KVM_NR_IRQCHIPS];
	int ret = -1, i, idx;
>>>>>>> v3.18
=======
	struct kvm_kernel_irq_routing_entry irq_set[KVM_NR_IRQCHIPS];
	int ret = -1, i, idx;
>>>>>>> v3.18

	trace_kvm_set_irq(irq, level, irq_source_id);

	/* Not possible to detect if the guest uses the PIC or the
	 * IOAPIC.  So set the bit in both. The guest will ignore
	 * writes to the unused one.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	rcu_read_lock();
	irq_rt = rcu_dereference(kvm->irq_routing);
	if (irq < irq_rt->nr_rt_entries)
		hlist_for_each_entry(e, &irq_rt->map[irq], link)
			irq_set[i++] = *e;
	rcu_read_unlock();
=======
	idx = srcu_read_lock(&kvm->irq_srcu);
	i = kvm_irq_map_gsi(kvm, irq_set, irq);
	srcu_read_unlock(&kvm->irq_srcu, idx);
>>>>>>> v3.18
=======
	idx = srcu_read_lock(&kvm->irq_srcu);
	i = kvm_irq_map_gsi(kvm, irq_set, irq);
	srcu_read_unlock(&kvm->irq_srcu, idx);
>>>>>>> v3.18

	while(i--) {
		int r;
		r = irq_set[i].set(&irq_set[i], kvm, irq_source_id, level,
				   line_status);
		if (r < 0)
			continue;

		ret = r + ((ret < 0) ? 0 : ret);
	}

	return ret;
}

void kvm_free_irq_routing(struct kvm *kvm)
{
	/* Called only during vm destruction. Nobody can use the pointer
	   at this stage */
	kfree(kvm->irq_routing);
}

static int setup_routing_entry(struct kvm_irq_routing_table *rt,
			       struct kvm_kernel_irq_routing_entry *e,
			       const struct kvm_irq_routing_entry *ue)
{
	int r = -EINVAL;
	struct kvm_kernel_irq_routing_entry *ei;

	/*
	 * Do not allow GSI to be mapped to the same irqchip more than once.
	 * Allow only one to one mapping between GSI and MSI.
	 */
	hlist_for_each_entry(ei, &rt->map[ue->gsi], link)
		if (ei->type == KVM_IRQ_ROUTING_MSI ||
		    ue->type == KVM_IRQ_ROUTING_MSI ||
		    ue->u.irqchip.irqchip == ei->irqchip.irqchip)
			return r;

	e->gsi = ue->gsi;
	e->type = ue->type;
<<<<<<< HEAD
<<<<<<< HEAD
	r = kvm_set_routing_entry(rt, e, ue);
	if (r)
		goto out;
=======
=======
>>>>>>> v3.18
	r = kvm_set_routing_entry(e, ue);
	if (r)
		goto out;
	if (e->type == KVM_IRQ_ROUTING_IRQCHIP)
		rt->chip[e->irqchip.irqchip][e->irqchip.pin] = e->gsi;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	hlist_add_head(&e->link, &rt->map[e->gsi]);
	r = 0;
out:
	return r;
}

int kvm_set_irq_routing(struct kvm *kvm,
			const struct kvm_irq_routing_entry *ue,
			unsigned nr,
			unsigned flags)
{
	struct kvm_irq_routing_table *new, *old;
	u32 i, j, nr_rt_entries = 0;
	int r;

	for (i = 0; i < nr; ++i) {
		if (ue[i].gsi >= KVM_MAX_IRQ_ROUTES)
			return -EINVAL;
		nr_rt_entries = max(nr_rt_entries, ue[i].gsi);
	}

	nr_rt_entries += 1;

	new = kzalloc(sizeof(*new) + (nr_rt_entries * sizeof(struct hlist_head))
		      + (nr * sizeof(struct kvm_kernel_irq_routing_entry)),
		      GFP_KERNEL);

	if (!new)
		return -ENOMEM;

	new->rt_entries = (void *)&new->map[nr_rt_entries];

	new->nr_rt_entries = nr_rt_entries;
	for (i = 0; i < KVM_NR_IRQCHIPS; i++)
		for (j = 0; j < KVM_IRQCHIP_NUM_PINS; j++)
			new->chip[i][j] = -1;

	for (i = 0; i < nr; ++i) {
		r = -EINVAL;
		if (ue->flags)
			goto out;
		r = setup_routing_entry(new, &new->rt_entries[i], ue);
		if (r)
			goto out;
		++ue;
	}

	mutex_lock(&kvm->irq_lock);
	old = kvm->irq_routing;
<<<<<<< HEAD
<<<<<<< HEAD
	kvm_irq_routing_update(kvm, new);
	mutex_unlock(&kvm->irq_lock);

	synchronize_rcu();
=======
=======
>>>>>>> v3.18
	rcu_assign_pointer(kvm->irq_routing, new);
	kvm_irq_routing_update(kvm);
	mutex_unlock(&kvm->irq_lock);

	synchronize_srcu_expedited(&kvm->irq_srcu);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	new = old;
	r = 0;

out:
	kfree(new);
	return r;
}
