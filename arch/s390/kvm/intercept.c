/*
 * in-kernel handling for sie intercepts
 *
<<<<<<< HEAD
 * Copyright IBM Corp. 2008, 2009
=======
 * Copyright IBM Corp. 2008, 2014
>>>>>>> v3.18
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (version 2 only)
 * as published by the Free Software Foundation.
 *
 *    Author(s): Carsten Otte <cotte@de.ibm.com>
 *               Christian Borntraeger <borntraeger@de.ibm.com>
 */

#include <linux/kvm_host.h>
#include <linux/errno.h>
#include <linux/pagemap.h>

#include <asm/kvm_host.h>
<<<<<<< HEAD
=======
#include <asm/asm-offsets.h>
#include <asm/irq.h>
>>>>>>> v3.18

#include "kvm-s390.h"
#include "gaccess.h"
#include "trace.h"
#include "trace-s390.h"

<<<<<<< HEAD
static int handle_lctlg(struct kvm_vcpu *vcpu)
{
	int reg1 = (vcpu->arch.sie_block->ipa & 0x00f0) >> 4;
	int reg3 = vcpu->arch.sie_block->ipa & 0x000f;
	u64 useraddr;
	int reg, rc;

	vcpu->stat.instruction_lctlg++;

	useraddr = kvm_s390_get_base_disp_rsy(vcpu);

	if (useraddr & 7)
		return kvm_s390_inject_program_int(vcpu, PGM_SPECIFICATION);

	reg = reg1;

	VCPU_EVENT(vcpu, 5, "lctlg r1:%x, r3:%x, addr:%llx", reg1, reg3,
		   useraddr);
	trace_kvm_s390_handle_lctl(vcpu, 1, reg1, reg3, useraddr);

	do {
		rc = get_guest(vcpu, vcpu->arch.sie_block->gcr[reg],
			       (u64 __user *) useraddr);
		if (rc)
			return kvm_s390_inject_program_int(vcpu, PGM_ADDRESSING);
		useraddr += 8;
		if (reg == reg3)
			break;
		reg = (reg + 1) % 16;
	} while (1);
	return 0;
}

static int handle_lctl(struct kvm_vcpu *vcpu)
{
	int reg1 = (vcpu->arch.sie_block->ipa & 0x00f0) >> 4;
	int reg3 = vcpu->arch.sie_block->ipa & 0x000f;
	u64 useraddr;
	u32 val = 0;
	int reg, rc;

	vcpu->stat.instruction_lctl++;

	useraddr = kvm_s390_get_base_disp_rs(vcpu);

	if (useraddr & 3)
		return kvm_s390_inject_program_int(vcpu, PGM_SPECIFICATION);

	VCPU_EVENT(vcpu, 5, "lctl r1:%x, r3:%x, addr:%llx", reg1, reg3,
		   useraddr);
	trace_kvm_s390_handle_lctl(vcpu, 0, reg1, reg3, useraddr);

	reg = reg1;
	do {
		rc = get_guest(vcpu, val, (u32 __user *) useraddr);
		if (rc)
			return kvm_s390_inject_program_int(vcpu, PGM_ADDRESSING);
		vcpu->arch.sie_block->gcr[reg] &= 0xffffffff00000000ul;
		vcpu->arch.sie_block->gcr[reg] |= val;
		useraddr += 4;
		if (reg == reg3)
			break;
		reg = (reg + 1) % 16;
	} while (1);
	return 0;
}

static const intercept_handler_t eb_handlers[256] = {
	[0x2f] = handle_lctlg,
	[0x8a] = kvm_s390_handle_priv_eb,
};

static int handle_eb(struct kvm_vcpu *vcpu)
{
	intercept_handler_t handler;

	handler = eb_handlers[vcpu->arch.sie_block->ipb & 0xff];
	if (handler)
		return handler(vcpu);
	return -EOPNOTSUPP;
}
=======
>>>>>>> v3.18

static const intercept_handler_t instruction_handlers[256] = {
	[0x01] = kvm_s390_handle_01,
	[0x82] = kvm_s390_handle_lpsw,
	[0x83] = kvm_s390_handle_diag,
	[0xae] = kvm_s390_handle_sigp,
	[0xb2] = kvm_s390_handle_b2,
<<<<<<< HEAD
	[0xb7] = handle_lctl,
	[0xb9] = kvm_s390_handle_b9,
	[0xe5] = kvm_s390_handle_e5,
	[0xeb] = handle_eb,
=======
	[0xb6] = kvm_s390_handle_stctl,
	[0xb7] = kvm_s390_handle_lctl,
	[0xb9] = kvm_s390_handle_b9,
	[0xe5] = kvm_s390_handle_e5,
	[0xeb] = kvm_s390_handle_eb,
>>>>>>> v3.18
};

static int handle_noop(struct kvm_vcpu *vcpu)
{
	switch (vcpu->arch.sie_block->icptcode) {
	case 0x0:
		vcpu->stat.exit_null++;
		break;
	case 0x10:
		vcpu->stat.exit_external_request++;
		break;
<<<<<<< HEAD
	case 0x14:
		vcpu->stat.exit_external_interrupt++;
		break;
=======
>>>>>>> v3.18
	default:
		break; /* nothing */
	}
	return 0;
}

static int handle_stop(struct kvm_vcpu *vcpu)
{
	int rc = 0;
<<<<<<< HEAD

	vcpu->stat.exit_stop_request++;
	spin_lock_bh(&vcpu->arch.local_int.lock);

	trace_kvm_s390_stop_request(vcpu->arch.local_int.action_bits);

	if (vcpu->arch.local_int.action_bits & ACTION_RELOADVCPU_ON_STOP) {
		vcpu->arch.local_int.action_bits &= ~ACTION_RELOADVCPU_ON_STOP;
		rc = SIE_INTERCEPT_RERUNVCPU;
		vcpu->run->exit_reason = KVM_EXIT_INTR;
	}

	if (vcpu->arch.local_int.action_bits & ACTION_STOP_ON_STOP) {
		atomic_set_mask(CPUSTAT_STOPPED,
				&vcpu->arch.sie_block->cpuflags);
		vcpu->arch.local_int.action_bits &= ~ACTION_STOP_ON_STOP;
		VCPU_EVENT(vcpu, 3, "%s", "cpu stopped");
		rc = -EOPNOTSUPP;
	}

	if (vcpu->arch.local_int.action_bits & ACTION_STORE_ON_STOP) {
		vcpu->arch.local_int.action_bits &= ~ACTION_STORE_ON_STOP;
		/* store status must be called unlocked. Since local_int.lock
		 * only protects local_int.* and not guest memory we can give
		 * up the lock here */
		spin_unlock_bh(&vcpu->arch.local_int.lock);
		rc = kvm_s390_vcpu_store_status(vcpu,
						KVM_S390_STORE_STATUS_NOADDR);
		if (rc >= 0)
			rc = -EOPNOTSUPP;
	} else
		spin_unlock_bh(&vcpu->arch.local_int.lock);
	return rc;
=======
	unsigned int action_bits;

	vcpu->stat.exit_stop_request++;
	trace_kvm_s390_stop_request(vcpu->arch.local_int.action_bits);

	action_bits = vcpu->arch.local_int.action_bits;

	if (!(action_bits & ACTION_STOP_ON_STOP))
		return 0;

	if (action_bits & ACTION_STORE_ON_STOP) {
		rc = kvm_s390_vcpu_store_status(vcpu,
						KVM_S390_STORE_STATUS_NOADDR);
		if (rc)
			return rc;
	}

	if (!kvm_s390_user_cpu_state_ctrl(vcpu->kvm))
		kvm_s390_vcpu_stop(vcpu);
	return -EOPNOTSUPP;
>>>>>>> v3.18
}

static int handle_validity(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
	unsigned long vmaddr;
	int viwhy = vcpu->arch.sie_block->ipb >> 16;
	int rc;

	vcpu->stat.exit_validity++;
	trace_kvm_s390_intercept_validity(vcpu, viwhy);
	if (viwhy == 0x37) {
		vmaddr = gmap_fault(vcpu->arch.sie_block->prefix,
				    vcpu->arch.gmap);
		if (IS_ERR_VALUE(vmaddr)) {
			rc = -EOPNOTSUPP;
			goto out;
		}
		rc = fault_in_pages_writeable((char __user *) vmaddr,
			 PAGE_SIZE);
		if (rc) {
			/* user will receive sigsegv, exit to user */
			rc = -EOPNOTSUPP;
			goto out;
		}
		vmaddr = gmap_fault(vcpu->arch.sie_block->prefix + PAGE_SIZE,
				    vcpu->arch.gmap);
		if (IS_ERR_VALUE(vmaddr)) {
			rc = -EOPNOTSUPP;
			goto out;
		}
		rc = fault_in_pages_writeable((char __user *) vmaddr,
			 PAGE_SIZE);
		if (rc) {
			/* user will receive sigsegv, exit to user */
			rc = -EOPNOTSUPP;
			goto out;
		}
	} else
		rc = -EOPNOTSUPP;

out:
	if (rc)
		VCPU_EVENT(vcpu, 2, "unhandled validity intercept code %d",
			   viwhy);
	return rc;
=======
	int viwhy = vcpu->arch.sie_block->ipb >> 16;

	vcpu->stat.exit_validity++;
	trace_kvm_s390_intercept_validity(vcpu, viwhy);
	WARN_ONCE(true, "kvm: unhandled validity intercept 0x%x\n", viwhy);
	return -EOPNOTSUPP;
>>>>>>> v3.18
}

static int handle_instruction(struct kvm_vcpu *vcpu)
{
	intercept_handler_t handler;

	vcpu->stat.exit_instruction++;
	trace_kvm_s390_intercept_instruction(vcpu,
					     vcpu->arch.sie_block->ipa,
					     vcpu->arch.sie_block->ipb);
	handler = instruction_handlers[vcpu->arch.sie_block->ipa >> 8];
	if (handler)
		return handler(vcpu);
	return -EOPNOTSUPP;
}

<<<<<<< HEAD
static int handle_prog(struct kvm_vcpu *vcpu)
{
	vcpu->stat.exit_program_interruption++;
	trace_kvm_s390_intercept_prog(vcpu, vcpu->arch.sie_block->iprcc);
	return kvm_s390_inject_program_int(vcpu, vcpu->arch.sie_block->iprcc);
=======
static void __extract_prog_irq(struct kvm_vcpu *vcpu,
			       struct kvm_s390_pgm_info *pgm_info)
{
	memset(pgm_info, 0, sizeof(struct kvm_s390_pgm_info));
	pgm_info->code = vcpu->arch.sie_block->iprcc;

	switch (vcpu->arch.sie_block->iprcc & ~PGM_PER) {
	case PGM_AFX_TRANSLATION:
	case PGM_ASX_TRANSLATION:
	case PGM_EX_TRANSLATION:
	case PGM_LFX_TRANSLATION:
	case PGM_LSTE_SEQUENCE:
	case PGM_LSX_TRANSLATION:
	case PGM_LX_TRANSLATION:
	case PGM_PRIMARY_AUTHORITY:
	case PGM_SECONDARY_AUTHORITY:
	case PGM_SPACE_SWITCH:
		pgm_info->trans_exc_code = vcpu->arch.sie_block->tecmc;
		break;
	case PGM_ALEN_TRANSLATION:
	case PGM_ALE_SEQUENCE:
	case PGM_ASTE_INSTANCE:
	case PGM_ASTE_SEQUENCE:
	case PGM_ASTE_VALIDITY:
	case PGM_EXTENDED_AUTHORITY:
		pgm_info->exc_access_id = vcpu->arch.sie_block->eai;
		break;
	case PGM_ASCE_TYPE:
	case PGM_PAGE_TRANSLATION:
	case PGM_REGION_FIRST_TRANS:
	case PGM_REGION_SECOND_TRANS:
	case PGM_REGION_THIRD_TRANS:
	case PGM_SEGMENT_TRANSLATION:
		pgm_info->trans_exc_code = vcpu->arch.sie_block->tecmc;
		pgm_info->exc_access_id  = vcpu->arch.sie_block->eai;
		pgm_info->op_access_id  = vcpu->arch.sie_block->oai;
		break;
	case PGM_MONITOR:
		pgm_info->mon_class_nr = vcpu->arch.sie_block->mcn;
		pgm_info->mon_code = vcpu->arch.sie_block->tecmc;
		break;
	case PGM_DATA:
		pgm_info->data_exc_code = vcpu->arch.sie_block->dxc;
		break;
	case PGM_PROTECTION:
		pgm_info->trans_exc_code = vcpu->arch.sie_block->tecmc;
		pgm_info->exc_access_id  = vcpu->arch.sie_block->eai;
		break;
	default:
		break;
	}

	if (vcpu->arch.sie_block->iprcc & PGM_PER) {
		pgm_info->per_code = vcpu->arch.sie_block->perc;
		pgm_info->per_atmid = vcpu->arch.sie_block->peratmid;
		pgm_info->per_address = vcpu->arch.sie_block->peraddr;
		pgm_info->per_access_id = vcpu->arch.sie_block->peraid;
	}
}

/*
 * restore ITDB to program-interruption TDB in guest lowcore
 * and set TX abort indication if required
*/
static int handle_itdb(struct kvm_vcpu *vcpu)
{
	struct kvm_s390_itdb *itdb;
	int rc;

	if (!IS_TE_ENABLED(vcpu) || !IS_ITDB_VALID(vcpu))
		return 0;
	if (current->thread.per_flags & PER_FLAG_NO_TE)
		return 0;
	itdb = (struct kvm_s390_itdb *)vcpu->arch.sie_block->itdba;
	rc = write_guest_lc(vcpu, __LC_PGM_TDB, itdb, sizeof(*itdb));
	if (rc)
		return rc;
	memset(itdb, 0, sizeof(*itdb));

	return 0;
}

#define per_event(vcpu) (vcpu->arch.sie_block->iprcc & PGM_PER)

static int handle_prog(struct kvm_vcpu *vcpu)
{
	struct kvm_s390_pgm_info pgm_info;
	psw_t psw;
	int rc;

	vcpu->stat.exit_program_interruption++;

	if (guestdbg_enabled(vcpu) && per_event(vcpu)) {
		kvm_s390_handle_per_event(vcpu);
		/* the interrupt might have been filtered out completely */
		if (vcpu->arch.sie_block->iprcc == 0)
			return 0;
	}

	trace_kvm_s390_intercept_prog(vcpu, vcpu->arch.sie_block->iprcc);
	if (vcpu->arch.sie_block->iprcc == PGM_SPECIFICATION) {
		rc = read_guest_lc(vcpu, __LC_PGM_NEW_PSW, &psw, sizeof(psw_t));
		if (rc)
			return rc;
		/* Avoid endless loops of specification exceptions */
		if (!is_valid_psw(&psw))
			return -EOPNOTSUPP;
	}
	rc = handle_itdb(vcpu);
	if (rc)
		return rc;

	__extract_prog_irq(vcpu, &pgm_info);
	return kvm_s390_inject_prog_irq(vcpu, &pgm_info);
>>>>>>> v3.18
}

static int handle_instruction_and_prog(struct kvm_vcpu *vcpu)
{
	int rc, rc2;

	vcpu->stat.exit_instr_and_program++;
	rc = handle_instruction(vcpu);
	rc2 = handle_prog(vcpu);

	if (rc == -EOPNOTSUPP)
		vcpu->arch.sie_block->icptcode = 0x04;
	if (rc)
		return rc;
	return rc2;
}

<<<<<<< HEAD
=======
/**
 * handle_external_interrupt - used for external interruption interceptions
 *
 * This interception only occurs if the CPUSTAT_EXT_INT bit was set, or if
 * the new PSW does not have external interrupts disabled. In the first case,
 * we've got to deliver the interrupt manually, and in the second case, we
 * drop to userspace to handle the situation there.
 */
static int handle_external_interrupt(struct kvm_vcpu *vcpu)
{
	u16 eic = vcpu->arch.sie_block->eic;
	struct kvm_s390_interrupt irq;
	psw_t newpsw;
	int rc;

	vcpu->stat.exit_external_interrupt++;

	rc = read_guest_lc(vcpu, __LC_EXT_NEW_PSW, &newpsw, sizeof(psw_t));
	if (rc)
		return rc;
	/* We can not handle clock comparator or timer interrupt with bad PSW */
	if ((eic == EXT_IRQ_CLK_COMP || eic == EXT_IRQ_CPU_TIMER) &&
	    (newpsw.mask & PSW_MASK_EXT))
		return -EOPNOTSUPP;

	switch (eic) {
	case EXT_IRQ_CLK_COMP:
		irq.type = KVM_S390_INT_CLOCK_COMP;
		break;
	case EXT_IRQ_CPU_TIMER:
		irq.type = KVM_S390_INT_CPU_TIMER;
		break;
	case EXT_IRQ_EXTERNAL_CALL:
		if (kvm_s390_si_ext_call_pending(vcpu))
			return 0;
		irq.type = KVM_S390_INT_EXTERNAL_CALL;
		irq.parm = vcpu->arch.sie_block->extcpuaddr;
		break;
	default:
		return -EOPNOTSUPP;
	}

	return kvm_s390_inject_vcpu(vcpu, &irq);
}

/**
 * Handle MOVE PAGE partial execution interception.
 *
 * This interception can only happen for guests with DAT disabled and
 * addresses that are currently not mapped in the host. Thus we try to
 * set up the mappings for the corresponding user pages here (or throw
 * addressing exceptions in case of illegal guest addresses).
 */
static int handle_mvpg_pei(struct kvm_vcpu *vcpu)
{
	psw_t *psw = &vcpu->arch.sie_block->gpsw;
	unsigned long srcaddr, dstaddr;
	int reg1, reg2, rc;

	kvm_s390_get_regs_rre(vcpu, &reg1, &reg2);

	/* Make sure that the source is paged-in */
	srcaddr = kvm_s390_real_to_abs(vcpu, vcpu->run->s.regs.gprs[reg2]);
	if (kvm_is_error_gpa(vcpu->kvm, srcaddr))
		return kvm_s390_inject_program_int(vcpu, PGM_ADDRESSING);
	rc = kvm_arch_fault_in_page(vcpu, srcaddr, 0);
	if (rc != 0)
		return rc;

	/* Make sure that the destination is paged-in */
	dstaddr = kvm_s390_real_to_abs(vcpu, vcpu->run->s.regs.gprs[reg1]);
	if (kvm_is_error_gpa(vcpu->kvm, dstaddr))
		return kvm_s390_inject_program_int(vcpu, PGM_ADDRESSING);
	rc = kvm_arch_fault_in_page(vcpu, dstaddr, 1);
	if (rc != 0)
		return rc;

	psw->addr = __rewind_psw(*psw, 4);

	return 0;
}

static int handle_partial_execution(struct kvm_vcpu *vcpu)
{
	if (vcpu->arch.sie_block->ipa == 0xb254)	/* MVPG */
		return handle_mvpg_pei(vcpu);
	if (vcpu->arch.sie_block->ipa >> 8 == 0xae)	/* SIGP */
		return kvm_s390_handle_sigp_pei(vcpu);

	return -EOPNOTSUPP;
}

>>>>>>> v3.18
static const intercept_handler_t intercept_funcs[] = {
	[0x00 >> 2] = handle_noop,
	[0x04 >> 2] = handle_instruction,
	[0x08 >> 2] = handle_prog,
	[0x0C >> 2] = handle_instruction_and_prog,
	[0x10 >> 2] = handle_noop,
<<<<<<< HEAD
	[0x14 >> 2] = handle_noop,
=======
	[0x14 >> 2] = handle_external_interrupt,
>>>>>>> v3.18
	[0x18 >> 2] = handle_noop,
	[0x1C >> 2] = kvm_s390_handle_wait,
	[0x20 >> 2] = handle_validity,
	[0x28 >> 2] = handle_stop,
<<<<<<< HEAD
=======
	[0x38 >> 2] = handle_partial_execution,
>>>>>>> v3.18
};

int kvm_handle_sie_intercept(struct kvm_vcpu *vcpu)
{
	intercept_handler_t func;
	u8 code = vcpu->arch.sie_block->icptcode;

	if (code & 3 || (code >> 2) >= ARRAY_SIZE(intercept_funcs))
		return -EOPNOTSUPP;
	func = intercept_funcs[code >> 2];
	if (func)
		return func(vcpu);
	return -EOPNOTSUPP;
}
