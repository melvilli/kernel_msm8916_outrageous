/* linux/drivers/iommu/exynos_iommu.c
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifdef CONFIG_EXYNOS_IOMMU_DEBUG
#define DEBUG
#endif

#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/pm_runtime.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/mm.h>
#include <linux/iommu.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/memblock.h>
#include <linux/export.h>

#include <asm/cacheflush.h>
#include <asm/pgtable.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include <mach/sysmmu.h>

/* We does not consider super section mapping (16MB) */
=======
=======
>>>>>>> v3.18
typedef u32 sysmmu_iova_t;
typedef u32 sysmmu_pte_t;

/* We do not consider super section mapping (16MB) */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#define SECT_ORDER 20
#define LPAGE_ORDER 16
#define SPAGE_ORDER 12

#define SECT_SIZE (1 << SECT_ORDER)
#define LPAGE_SIZE (1 << LPAGE_ORDER)
#define SPAGE_SIZE (1 << SPAGE_ORDER)

#define SECT_MASK (~(SECT_SIZE - 1))
#define LPAGE_MASK (~(LPAGE_SIZE - 1))
#define SPAGE_MASK (~(SPAGE_SIZE - 1))

<<<<<<< HEAD
<<<<<<< HEAD
#define lv1ent_fault(sent) (((*(sent) & 3) == 0) || ((*(sent) & 3) == 3))
#define lv1ent_page(sent) ((*(sent) & 3) == 1)
=======
=======
>>>>>>> v3.18
#define lv1ent_fault(sent) ((*(sent) == ZERO_LV2LINK) || \
			   ((*(sent) & 3) == 0) || ((*(sent) & 3) == 3))
#define lv1ent_zero(sent) (*(sent) == ZERO_LV2LINK)
#define lv1ent_page_zero(sent) ((*(sent) & 3) == 1)
#define lv1ent_page(sent) ((*(sent) != ZERO_LV2LINK) && \
			  ((*(sent) & 3) == 1))
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#define lv1ent_section(sent) ((*(sent) & 3) == 2)

#define lv2ent_fault(pent) ((*(pent) & 3) == 0)
#define lv2ent_small(pent) ((*(pent) & 2) == 2)
#define lv2ent_large(pent) ((*(pent) & 3) == 1)

<<<<<<< HEAD
<<<<<<< HEAD
#define section_phys(sent) (*(sent) & SECT_MASK)
#define section_offs(iova) ((iova) & 0xFFFFF)
#define lpage_phys(pent) (*(pent) & LPAGE_MASK)
#define lpage_offs(iova) ((iova) & 0xFFFF)
#define spage_phys(pent) (*(pent) & SPAGE_MASK)
#define spage_offs(iova) ((iova) & 0xFFF)

#define lv1ent_offset(iova) ((iova) >> SECT_ORDER)
#define lv2ent_offset(iova) (((iova) & 0xFF000) >> SPAGE_ORDER)

#define NUM_LV1ENTRIES 4096
#define NUM_LV2ENTRIES 256

#define LV2TABLE_SIZE (NUM_LV2ENTRIES * sizeof(long))
=======
=======
>>>>>>> v3.18
static u32 sysmmu_page_offset(sysmmu_iova_t iova, u32 size)
{
	return iova & (size - 1);
}

#define section_phys(sent) (*(sent) & SECT_MASK)
#define section_offs(iova) sysmmu_page_offset((iova), SECT_SIZE)
#define lpage_phys(pent) (*(pent) & LPAGE_MASK)
#define lpage_offs(iova) sysmmu_page_offset((iova), LPAGE_SIZE)
#define spage_phys(pent) (*(pent) & SPAGE_MASK)
#define spage_offs(iova) sysmmu_page_offset((iova), SPAGE_SIZE)

#define NUM_LV1ENTRIES 4096
#define NUM_LV2ENTRIES (SECT_SIZE / SPAGE_SIZE)

static u32 lv1ent_offset(sysmmu_iova_t iova)
{
	return iova >> SECT_ORDER;
}

static u32 lv2ent_offset(sysmmu_iova_t iova)
{
	return (iova >> SPAGE_ORDER) & (NUM_LV2ENTRIES - 1);
}

#define LV2TABLE_SIZE (NUM_LV2ENTRIES * sizeof(sysmmu_pte_t))
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

#define SPAGES_PER_LPAGE (LPAGE_SIZE / SPAGE_SIZE)

#define lv2table_base(sent) (*(sent) & 0xFFFFFC00)

#define mk_lv1ent_sect(pa) ((pa) | 2)
#define mk_lv1ent_page(pa) ((pa) | 1)
#define mk_lv2ent_lpage(pa) ((pa) | 1)
#define mk_lv2ent_spage(pa) ((pa) | 2)

#define CTRL_ENABLE	0x5
#define CTRL_BLOCK	0x7
#define CTRL_DISABLE	0x0

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
#define CFG_LRU		0x1
#define CFG_QOS(n)	((n & 0xF) << 7)
#define CFG_MASK	0x0150FFFF /* Selecting bit 0-15, 20, 22 and 24 */
#define CFG_ACGEN	(1 << 24) /* System MMU 3.3 only */
#define CFG_SYSSEL	(1 << 22) /* System MMU 3.2 only */
#define CFG_FLPDCACHE	(1 << 20) /* System MMU 3.2+ only */

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#define REG_MMU_CTRL		0x000
#define REG_MMU_CFG		0x004
#define REG_MMU_STATUS		0x008
#define REG_MMU_FLUSH		0x00C
#define REG_MMU_FLUSH_ENTRY	0x010
#define REG_PT_BASE_ADDR	0x014
#define REG_INT_STATUS		0x018
#define REG_INT_CLEAR		0x01C

#define REG_PAGE_FAULT_ADDR	0x024
#define REG_AW_FAULT_ADDR	0x028
#define REG_AR_FAULT_ADDR	0x02C
#define REG_DEFAULT_SLAVE_ADDR	0x030

#define REG_MMU_VERSION		0x034

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
#define MMU_MAJ_VER(val)	((val) >> 7)
#define MMU_MIN_VER(val)	((val) & 0x7F)
#define MMU_RAW_VER(reg)	(((reg) >> 21) & ((1 << 11) - 1)) /* 11 bits */

#define MAKE_MMU_VER(maj, min)	((((maj) & 0xF) << 7) | ((min) & 0x7F))

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#define REG_PB0_SADDR		0x04C
#define REG_PB0_EADDR		0x050
#define REG_PB1_SADDR		0x054
#define REG_PB1_EADDR		0x058

<<<<<<< HEAD
<<<<<<< HEAD
static unsigned long *section_entry(unsigned long *pgtable, unsigned long iova)
=======
=======
>>>>>>> v3.18
#define has_sysmmu(dev)		(dev->archdata.iommu != NULL)

static struct kmem_cache *lv2table_kmem_cache;
static sysmmu_pte_t *zero_lv2_table;
#define ZERO_LV2LINK mk_lv1ent_page(virt_to_phys(zero_lv2_table))

static sysmmu_pte_t *section_entry(sysmmu_pte_t *pgtable, sysmmu_iova_t iova)
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
{
	return pgtable + lv1ent_offset(iova);
}

<<<<<<< HEAD
<<<<<<< HEAD
static unsigned long *page_entry(unsigned long *sent, unsigned long iova)
{
	return (unsigned long *)__va(lv2table_base(sent)) + lv2ent_offset(iova);
=======
=======
>>>>>>> v3.18
static sysmmu_pte_t *page_entry(sysmmu_pte_t *sent, sysmmu_iova_t iova)
{
	return (sysmmu_pte_t *)phys_to_virt(
				lv2table_base(sent)) + lv2ent_offset(iova);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

enum exynos_sysmmu_inttype {
	SYSMMU_PAGEFAULT,
	SYSMMU_AR_MULTIHIT,
	SYSMMU_AW_MULTIHIT,
	SYSMMU_BUSERROR,
	SYSMMU_AR_SECURITY,
	SYSMMU_AR_ACCESS,
	SYSMMU_AW_SECURITY,
	SYSMMU_AW_PROTECTION, /* 7 */
	SYSMMU_FAULT_UNKNOWN,
	SYSMMU_FAULTS_NUM
};

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * @itype: type of fault.
 * @pgtable_base: the physical address of page table base. This is 0 if @itype
 *                is SYSMMU_BUSERROR.
 * @fault_addr: the device (virtual) address that the System MMU tried to
 *             translated. This is 0 if @itype is SYSMMU_BUSERROR.
 */
typedef int (*sysmmu_fault_handler_t)(enum exynos_sysmmu_inttype itype,
			unsigned long pgtable_base, unsigned long fault_addr);

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
static unsigned short fault_reg_offset[SYSMMU_FAULTS_NUM] = {
	REG_PAGE_FAULT_ADDR,
	REG_AR_FAULT_ADDR,
	REG_AW_FAULT_ADDR,
	REG_DEFAULT_SLAVE_ADDR,
	REG_AR_FAULT_ADDR,
	REG_AR_FAULT_ADDR,
	REG_AW_FAULT_ADDR,
	REG_AW_FAULT_ADDR
};

static char *sysmmu_fault_name[SYSMMU_FAULTS_NUM] = {
	"PAGE FAULT",
	"AR MULTI-HIT FAULT",
	"AW MULTI-HIT FAULT",
	"BUS ERROR",
	"AR SECURITY PROTECTION FAULT",
	"AR ACCESS PROTECTION FAULT",
	"AW SECURITY PROTECTION FAULT",
	"AW ACCESS PROTECTION FAULT",
	"UNKNOWN FAULT"
};

<<<<<<< HEAD
<<<<<<< HEAD
struct exynos_iommu_domain {
	struct list_head clients; /* list of sysmmu_drvdata.node */
	unsigned long *pgtable; /* lv1 page table, 16KB */
=======
=======
>>>>>>> v3.18
/* attached to dev.archdata.iommu of the master device */
struct exynos_iommu_owner {
	struct list_head client; /* entry of exynos_iommu_domain.clients */
	struct device *dev;
	struct device *sysmmu;
	struct iommu_domain *domain;
	void *vmm_data;         /* IO virtual memory manager's data */
	spinlock_t lock;        /* Lock to preserve consistency of System MMU */
};

struct exynos_iommu_domain {
	struct list_head clients; /* list of sysmmu_drvdata.node */
	sysmmu_pte_t *pgtable; /* lv1 page table, 16KB */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	short *lv2entcnt; /* free lv2 entry counter for each section */
	spinlock_t lock; /* lock for this structure */
	spinlock_t pgtablelock; /* lock for modifying page table @ pgtable */
};

struct sysmmu_drvdata {
<<<<<<< HEAD
<<<<<<< HEAD
	struct list_head node; /* entry of exynos_iommu_domain.clients */
	struct device *sysmmu;	/* System MMU's device descriptor */
	struct device *dev;	/* Owner of system MMU */
	char *dbgname;
	int nsfrs;
	void __iomem **sfrbases;
	struct clk *clk[2];
	int activations;
	rwlock_t lock;
	struct iommu_domain *domain;
	sysmmu_fault_handler_t fault_handler;
	unsigned long pgtable;
=======
=======
>>>>>>> v3.18
	struct device *sysmmu;	/* System MMU's device descriptor */
	struct device *master;	/* Owner of system MMU */
	void __iomem *sfrbase;
	struct clk *clk;
	struct clk *clk_master;
	int activations;
	spinlock_t lock;
	struct iommu_domain *domain;
	phys_addr_t pgtable;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
};

static bool set_sysmmu_active(struct sysmmu_drvdata *data)
{
	/* return true if the System MMU was not active previously
	   and it needs to be initialized */
	return ++data->activations == 1;
}

static bool set_sysmmu_inactive(struct sysmmu_drvdata *data)
{
	/* return true if the System MMU is needed to be disabled */
	BUG_ON(data->activations < 1);
	return --data->activations == 0;
}

static bool is_sysmmu_active(struct sysmmu_drvdata *data)
{
	return data->activations > 0;
}

static void sysmmu_unblock(void __iomem *sfrbase)
{
	__raw_writel(CTRL_ENABLE, sfrbase + REG_MMU_CTRL);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
static unsigned int __raw_sysmmu_version(struct sysmmu_drvdata *data)
{
	return MMU_RAW_VER(__raw_readl(data->sfrbase + REG_MMU_VERSION));
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static bool sysmmu_block(void __iomem *sfrbase)
{
	int i = 120;

	__raw_writel(CTRL_BLOCK, sfrbase + REG_MMU_CTRL);
	while ((i > 0) && !(__raw_readl(sfrbase + REG_MMU_STATUS) & 1))
		--i;

	if (!(__raw_readl(sfrbase + REG_MMU_STATUS) & 1)) {
		sysmmu_unblock(sfrbase);
		return false;
	}

	return true;
}

static void __sysmmu_tlb_invalidate(void __iomem *sfrbase)
{
	__raw_writel(0x1, sfrbase + REG_MMU_FLUSH);
}

static void __sysmmu_tlb_invalidate_entry(void __iomem *sfrbase,
<<<<<<< HEAD
<<<<<<< HEAD
						unsigned long iova)
{
	__raw_writel((iova & SPAGE_MASK) | 1, sfrbase + REG_MMU_FLUSH_ENTRY);
}

static void __sysmmu_set_ptbase(void __iomem *sfrbase,
				       unsigned long pgd)
{
	__raw_writel(0x1, sfrbase + REG_MMU_CFG); /* 16KB LV1, LRU */
=======
=======
>>>>>>> v3.18
				sysmmu_iova_t iova, unsigned int num_inv)
{
	unsigned int i;

	for (i = 0; i < num_inv; i++) {
		__raw_writel((iova & SPAGE_MASK) | 1,
				sfrbase + REG_MMU_FLUSH_ENTRY);
		iova += SPAGE_SIZE;
	}
}

static void __sysmmu_set_ptbase(void __iomem *sfrbase,
				       phys_addr_t pgd)
{
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	__raw_writel(pgd, sfrbase + REG_PT_BASE_ADDR);

	__sysmmu_tlb_invalidate(sfrbase);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void __sysmmu_set_prefbuf(void __iomem *sfrbase, unsigned long base,
						unsigned long size, int idx)
{
	__raw_writel(base, sfrbase + REG_PB0_SADDR + idx * 8);
	__raw_writel(size - 1 + base,  sfrbase + REG_PB0_EADDR + idx * 8);
}

void exynos_sysmmu_set_prefbuf(struct device *dev,
				unsigned long base0, unsigned long size0,
				unsigned long base1, unsigned long size1)
{
	struct sysmmu_drvdata *data = dev_get_drvdata(dev->archdata.iommu);
	unsigned long flags;
	int i;

	BUG_ON((base0 + size0) <= base0);
	BUG_ON((size1 > 0) && ((base1 + size1) <= base1));

	read_lock_irqsave(&data->lock, flags);
	if (!is_sysmmu_active(data))
		goto finish;

	for (i = 0; i < data->nsfrs; i++) {
		if ((readl(data->sfrbases[i] + REG_MMU_VERSION) >> 28) == 3) {
			if (!sysmmu_block(data->sfrbases[i]))
				continue;

			if (size1 == 0) {
				if (size0 <= SZ_128K) {
					base1 = base0;
					size1 = size0;
				} else {
					size1 = size0 -
						ALIGN(size0 / 2, SZ_64K);
					size0 = size0 - size1;
					base1 = base0 + size0;
				}
			}

			__sysmmu_set_prefbuf(
					data->sfrbases[i], base0, size0, 0);
			__sysmmu_set_prefbuf(
					data->sfrbases[i], base1, size1, 1);

			sysmmu_unblock(data->sfrbases[i]);
		}
	}
finish:
	read_unlock_irqrestore(&data->lock, flags);
}

static void __set_fault_handler(struct sysmmu_drvdata *data,
					sysmmu_fault_handler_t handler)
{
	unsigned long flags;

	write_lock_irqsave(&data->lock, flags);
	data->fault_handler = handler;
	write_unlock_irqrestore(&data->lock, flags);
}

void exynos_sysmmu_set_fault_handler(struct device *dev,
					sysmmu_fault_handler_t handler)
{
	struct sysmmu_drvdata *data = dev_get_drvdata(dev->archdata.iommu);

	__set_fault_handler(data, handler);
}

static int default_fault_handler(enum exynos_sysmmu_inttype itype,
		     unsigned long pgtable_base, unsigned long fault_addr)
{
	unsigned long *ent;

	if ((itype >= SYSMMU_FAULTS_NUM) || (itype < SYSMMU_PAGEFAULT))
		itype = SYSMMU_FAULT_UNKNOWN;

	pr_err("%s occurred at 0x%lx(Page table base: 0x%lx)\n",
			sysmmu_fault_name[itype], fault_addr, pgtable_base);

	ent = section_entry(__va(pgtable_base), fault_addr);
	pr_err("\tLv1 entry: 0x%lx\n", *ent);

	if (lv1ent_page(ent)) {
		ent = page_entry(ent, fault_addr);
		pr_err("\t Lv2 entry: 0x%lx\n", *ent);
	}

	pr_err("Generating Kernel OOPS... because it is unrecoverable.\n");

	BUG();

	return 0;
}

static irqreturn_t exynos_sysmmu_irq(int irq, void *dev_id)
{
	/* SYSMMU is in blocked when interrupt occurred. */
	struct sysmmu_drvdata *data = dev_id;
	struct resource *irqres;
	struct platform_device *pdev;
	enum exynos_sysmmu_inttype itype;
	unsigned long addr = -1;

	int i, ret = -ENOSYS;

	read_lock(&data->lock);

	WARN_ON(!is_sysmmu_active(data));

	pdev = to_platform_device(data->sysmmu);
	for (i = 0; i < (pdev->num_resources / 2); i++) {
		irqres = platform_get_resource(pdev, IORESOURCE_IRQ, i);
		if (irqres && ((int)irqres->start == irq))
			break;
	}

	if (i == pdev->num_resources) {
		itype = SYSMMU_FAULT_UNKNOWN;
	} else {
		itype = (enum exynos_sysmmu_inttype)
			__ffs(__raw_readl(data->sfrbases[i] + REG_INT_STATUS));
		if (WARN_ON(!((itype >= 0) && (itype < SYSMMU_FAULT_UNKNOWN))))
			itype = SYSMMU_FAULT_UNKNOWN;
		else
			addr = __raw_readl(
				data->sfrbases[i] + fault_reg_offset[itype]);
	}

	if (data->domain)
		ret = report_iommu_fault(data->domain, data->dev,
				addr, itype);

	if ((ret == -ENOSYS) && data->fault_handler) {
		unsigned long base = data->pgtable;
		if (itype != SYSMMU_FAULT_UNKNOWN)
			base = __raw_readl(
					data->sfrbases[i] + REG_PT_BASE_ADDR);
		ret = data->fault_handler(itype, base, addr);
	}

	if (!ret && (itype != SYSMMU_FAULT_UNKNOWN))
		__raw_writel(1 << itype, data->sfrbases[i] + REG_INT_CLEAR);
	else
		dev_dbg(data->sysmmu, "(%s) %s is not handled.\n",
				data->dbgname, sysmmu_fault_name[itype]);

	if (itype != SYSMMU_FAULT_UNKNOWN)
		sysmmu_unblock(data->sfrbases[i]);

	read_unlock(&data->lock);

	return IRQ_HANDLED;
}

static bool __exynos_sysmmu_disable(struct sysmmu_drvdata *data)
{
	unsigned long flags;
	bool disabled = false;
	int i;

	write_lock_irqsave(&data->lock, flags);

	if (!set_sysmmu_inactive(data))
		goto finish;

	for (i = 0; i < data->nsfrs; i++)
		__raw_writel(CTRL_DISABLE, data->sfrbases[i] + REG_MMU_CTRL);

	if (data->clk[1])
		clk_disable(data->clk[1]);
	if (data->clk[0])
		clk_disable(data->clk[0]);

	disabled = true;
	data->pgtable = 0;
	data->domain = NULL;
finish:
	write_unlock_irqrestore(&data->lock, flags);

	if (disabled)
		dev_dbg(data->sysmmu, "(%s) Disabled\n", data->dbgname);
	else
		dev_dbg(data->sysmmu, "(%s) %d times left to be disabled\n",
					data->dbgname, data->activations);

	return disabled;
=======
=======
>>>>>>> v3.18
static void show_fault_information(const char *name,
		enum exynos_sysmmu_inttype itype,
		phys_addr_t pgtable_base, sysmmu_iova_t fault_addr)
{
	sysmmu_pte_t *ent;

	if ((itype >= SYSMMU_FAULTS_NUM) || (itype < SYSMMU_PAGEFAULT))
		itype = SYSMMU_FAULT_UNKNOWN;

	pr_err("%s occurred at %#x by %s(Page table base: %pa)\n",
		sysmmu_fault_name[itype], fault_addr, name, &pgtable_base);

	ent = section_entry(phys_to_virt(pgtable_base), fault_addr);
	pr_err("\tLv1 entry: %#x\n", *ent);

	if (lv1ent_page(ent)) {
		ent = page_entry(ent, fault_addr);
		pr_err("\t Lv2 entry: %#x\n", *ent);
	}
}

static irqreturn_t exynos_sysmmu_irq(int irq, void *dev_id)
{
	/* SYSMMU is in blocked state when interrupt occurred. */
	struct sysmmu_drvdata *data = dev_id;
	enum exynos_sysmmu_inttype itype;
	sysmmu_iova_t addr = -1;
	int ret = -ENOSYS;

	WARN_ON(!is_sysmmu_active(data));

	spin_lock(&data->lock);

	if (!IS_ERR(data->clk_master))
		clk_enable(data->clk_master);

	itype = (enum exynos_sysmmu_inttype)
		__ffs(__raw_readl(data->sfrbase + REG_INT_STATUS));
	if (WARN_ON(!((itype >= 0) && (itype < SYSMMU_FAULT_UNKNOWN))))
		itype = SYSMMU_FAULT_UNKNOWN;
	else
		addr = __raw_readl(data->sfrbase + fault_reg_offset[itype]);

	if (itype == SYSMMU_FAULT_UNKNOWN) {
		pr_err("%s: Fault is not occurred by System MMU '%s'!\n",
			__func__, dev_name(data->sysmmu));
		pr_err("%s: Please check if IRQ is correctly configured.\n",
			__func__);
		BUG();
	} else {
		unsigned int base =
				__raw_readl(data->sfrbase + REG_PT_BASE_ADDR);
		show_fault_information(dev_name(data->sysmmu),
					itype, base, addr);
		if (data->domain)
			ret = report_iommu_fault(data->domain,
					data->master, addr, itype);
	}

	/* fault is not recovered by fault handler */
	BUG_ON(ret != 0);

	__raw_writel(1 << itype, data->sfrbase + REG_INT_CLEAR);

	sysmmu_unblock(data->sfrbase);

	if (!IS_ERR(data->clk_master))
		clk_disable(data->clk_master);

	spin_unlock(&data->lock);

	return IRQ_HANDLED;
}

static void __sysmmu_disable_nocount(struct sysmmu_drvdata *data)
{
	if (!IS_ERR(data->clk_master))
		clk_enable(data->clk_master);

	__raw_writel(CTRL_DISABLE, data->sfrbase + REG_MMU_CTRL);
	__raw_writel(0, data->sfrbase + REG_MMU_CFG);

	clk_disable(data->clk);
	if (!IS_ERR(data->clk_master))
		clk_disable(data->clk_master);
}

static bool __sysmmu_disable(struct sysmmu_drvdata *data)
{
	bool disabled;
	unsigned long flags;

	spin_lock_irqsave(&data->lock, flags);

	disabled = set_sysmmu_inactive(data);

	if (disabled) {
		data->pgtable = 0;
		data->domain = NULL;

		__sysmmu_disable_nocount(data);

		dev_dbg(data->sysmmu, "Disabled\n");
	} else  {
		dev_dbg(data->sysmmu, "%d times left to disable\n",
					data->activations);
	}

	spin_unlock_irqrestore(&data->lock, flags);

	return disabled;
}

static void __sysmmu_init_config(struct sysmmu_drvdata *data)
{
	unsigned int cfg = CFG_LRU | CFG_QOS(15);
	unsigned int ver;

	ver = __raw_sysmmu_version(data);
	if (MMU_MAJ_VER(ver) == 3) {
		if (MMU_MIN_VER(ver) >= 2) {
			cfg |= CFG_FLPDCACHE;
			if (MMU_MIN_VER(ver) == 3) {
				cfg |= CFG_ACGEN;
				cfg &= ~CFG_LRU;
			} else {
				cfg |= CFG_SYSSEL;
			}
		}
	}

	__raw_writel(cfg, data->sfrbase + REG_MMU_CFG);
}

static void __sysmmu_enable_nocount(struct sysmmu_drvdata *data)
{
	if (!IS_ERR(data->clk_master))
		clk_enable(data->clk_master);
	clk_enable(data->clk);

	__raw_writel(CTRL_BLOCK, data->sfrbase + REG_MMU_CTRL);

	__sysmmu_init_config(data);

	__sysmmu_set_ptbase(data->sfrbase, data->pgtable);

	__raw_writel(CTRL_ENABLE, data->sfrbase + REG_MMU_CTRL);

	if (!IS_ERR(data->clk_master))
		clk_disable(data->clk_master);
}

static int __sysmmu_enable(struct sysmmu_drvdata *data,
			phys_addr_t pgtable, struct iommu_domain *domain)
{
	int ret = 0;
	unsigned long flags;

	spin_lock_irqsave(&data->lock, flags);
	if (set_sysmmu_active(data)) {
		data->pgtable = pgtable;
		data->domain = domain;

		__sysmmu_enable_nocount(data);

		dev_dbg(data->sysmmu, "Enabled\n");
	} else {
		ret = (pgtable == data->pgtable) ? 1 : -EBUSY;

		dev_dbg(data->sysmmu, "already enabled\n");
	}

	if (WARN_ON(ret < 0))
		set_sysmmu_inactive(data); /* decrement count */

	spin_unlock_irqrestore(&data->lock, flags);

	return ret;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

/* __exynos_sysmmu_enable: Enables System MMU
 *
 * returns -error if an error occurred and System MMU is not enabled,
 * 0 if the System MMU has been just enabled and 1 if System MMU was already
 * enabled before.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int __exynos_sysmmu_enable(struct sysmmu_drvdata *data,
			unsigned long pgtable, struct iommu_domain *domain)
{
	int i, ret = 0;
	unsigned long flags;

	write_lock_irqsave(&data->lock, flags);

	if (!set_sysmmu_active(data)) {
		if (WARN_ON(pgtable != data->pgtable)) {
			ret = -EBUSY;
			set_sysmmu_inactive(data);
		} else {
			ret = 1;
		}

		dev_dbg(data->sysmmu, "(%s) Already enabled\n", data->dbgname);
		goto finish;
	}

	if (data->clk[0])
		clk_enable(data->clk[0]);
	if (data->clk[1])
		clk_enable(data->clk[1]);

	data->pgtable = pgtable;

	for (i = 0; i < data->nsfrs; i++) {
		__sysmmu_set_ptbase(data->sfrbases[i], pgtable);

		if ((readl(data->sfrbases[i] + REG_MMU_VERSION) >> 28) == 3) {
			/* System MMU version is 3.x */
			__raw_writel((1 << 12) | (2 << 28),
					data->sfrbases[i] + REG_MMU_CFG);
			__sysmmu_set_prefbuf(data->sfrbases[i], 0, -1, 0);
			__sysmmu_set_prefbuf(data->sfrbases[i], 0, -1, 1);
		}

		__raw_writel(CTRL_ENABLE, data->sfrbases[i] + REG_MMU_CTRL);
	}

	data->domain = domain;

	dev_dbg(data->sysmmu, "(%s) Enabled\n", data->dbgname);
finish:
	write_unlock_irqrestore(&data->lock, flags);

	return ret;
}

int exynos_sysmmu_enable(struct device *dev, unsigned long pgtable)
{
	struct sysmmu_drvdata *data = dev_get_drvdata(dev->archdata.iommu);
	int ret;

	BUG_ON(!memblock_is_memory(pgtable));

	ret = pm_runtime_get_sync(data->sysmmu);
	if (ret < 0) {
		dev_dbg(data->sysmmu, "(%s) Failed to enable\n", data->dbgname);
		return ret;
	}

	ret = __exynos_sysmmu_enable(data, pgtable, NULL);
	if (WARN_ON(ret < 0)) {
		pm_runtime_put(data->sysmmu);
		dev_err(data->sysmmu,
			"(%s) Already enabled with page table %#lx\n",
			data->dbgname, data->pgtable);
	} else {
		data->dev = dev;
	}

	return ret;
}

static bool exynos_sysmmu_disable(struct device *dev)
{
	struct sysmmu_drvdata *data = dev_get_drvdata(dev->archdata.iommu);
	bool disabled;

	disabled = __exynos_sysmmu_disable(data);
	pm_runtime_put(data->sysmmu);

	return disabled;
}

static void sysmmu_tlb_invalidate_entry(struct device *dev, unsigned long iova)
{
	unsigned long flags;
	struct sysmmu_drvdata *data = dev_get_drvdata(dev->archdata.iommu);

	read_lock_irqsave(&data->lock, flags);

	if (is_sysmmu_active(data)) {
		int i;
		for (i = 0; i < data->nsfrs; i++) {
			if (sysmmu_block(data->sfrbases[i])) {
				__sysmmu_tlb_invalidate_entry(
						data->sfrbases[i], iova);
				sysmmu_unblock(data->sfrbases[i]);
			}
		}
	} else {
		dev_dbg(data->sysmmu,
			"(%s) Disabled. Skipping invalidating TLB.\n",
			data->dbgname);
	}

	read_unlock_irqrestore(&data->lock, flags);
=======
=======
>>>>>>> v3.18
static int __exynos_sysmmu_enable(struct device *dev, phys_addr_t pgtable,
				  struct iommu_domain *domain)
{
	int ret = 0;
	unsigned long flags;
	struct exynos_iommu_owner *owner = dev->archdata.iommu;
	struct sysmmu_drvdata *data;

	BUG_ON(!has_sysmmu(dev));

	spin_lock_irqsave(&owner->lock, flags);

	data = dev_get_drvdata(owner->sysmmu);

	ret = __sysmmu_enable(data, pgtable, domain);
	if (ret >= 0)
		data->master = dev;

	spin_unlock_irqrestore(&owner->lock, flags);

	return ret;
}

int exynos_sysmmu_enable(struct device *dev, phys_addr_t pgtable)
{
	BUG_ON(!memblock_is_memory(pgtable));

	return __exynos_sysmmu_enable(dev, pgtable, NULL);
}

static bool exynos_sysmmu_disable(struct device *dev)
{
	unsigned long flags;
	bool disabled = true;
	struct exynos_iommu_owner *owner = dev->archdata.iommu;
	struct sysmmu_drvdata *data;

	BUG_ON(!has_sysmmu(dev));

	spin_lock_irqsave(&owner->lock, flags);

	data = dev_get_drvdata(owner->sysmmu);

	disabled = __sysmmu_disable(data);
	if (disabled)
		data->master = NULL;

	spin_unlock_irqrestore(&owner->lock, flags);

	return disabled;
}

static void __sysmmu_tlb_invalidate_flpdcache(struct sysmmu_drvdata *data,
					      sysmmu_iova_t iova)
{
	if (__raw_sysmmu_version(data) == MAKE_MMU_VER(3, 3))
		__raw_writel(iova | 0x1, data->sfrbase + REG_MMU_FLUSH_ENTRY);
}

static void sysmmu_tlb_invalidate_flpdcache(struct device *dev,
					    sysmmu_iova_t iova)
{
	unsigned long flags;
	struct exynos_iommu_owner *owner = dev->archdata.iommu;
	struct sysmmu_drvdata *data = dev_get_drvdata(owner->sysmmu);

	if (!IS_ERR(data->clk_master))
		clk_enable(data->clk_master);

	spin_lock_irqsave(&data->lock, flags);
	if (is_sysmmu_active(data))
		__sysmmu_tlb_invalidate_flpdcache(data, iova);
	spin_unlock_irqrestore(&data->lock, flags);

	if (!IS_ERR(data->clk_master))
		clk_disable(data->clk_master);
}

static void sysmmu_tlb_invalidate_entry(struct device *dev, sysmmu_iova_t iova,
					size_t size)
{
	struct exynos_iommu_owner *owner = dev->archdata.iommu;
	unsigned long flags;
	struct sysmmu_drvdata *data;

	data = dev_get_drvdata(owner->sysmmu);

	spin_lock_irqsave(&data->lock, flags);
	if (is_sysmmu_active(data)) {
		unsigned int num_inv = 1;

		if (!IS_ERR(data->clk_master))
			clk_enable(data->clk_master);

		/*
		 * L2TLB invalidation required
		 * 4KB page: 1 invalidation
		 * 64KB page: 16 invalidations
		 * 1MB page: 64 invalidations
		 * because it is set-associative TLB
		 * with 8-way and 64 sets.
		 * 1MB page can be cached in one of all sets.
		 * 64KB page can be one of 16 consecutive sets.
		 */
		if (MMU_MAJ_VER(__raw_sysmmu_version(data)) == 2)
			num_inv = min_t(unsigned int, size / PAGE_SIZE, 64);

		if (sysmmu_block(data->sfrbase)) {
			__sysmmu_tlb_invalidate_entry(
				data->sfrbase, iova, num_inv);
			sysmmu_unblock(data->sfrbase);
		}
		if (!IS_ERR(data->clk_master))
			clk_disable(data->clk_master);
	} else {
		dev_dbg(dev, "disabled. Skipping TLB invalidation @ %#x\n",
			iova);
	}
	spin_unlock_irqrestore(&data->lock, flags);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

void exynos_sysmmu_tlb_invalidate(struct device *dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long flags;
	struct sysmmu_drvdata *data = dev_get_drvdata(dev->archdata.iommu);

	read_lock_irqsave(&data->lock, flags);

	if (is_sysmmu_active(data)) {
		int i;
		for (i = 0; i < data->nsfrs; i++) {
			if (sysmmu_block(data->sfrbases[i])) {
				__sysmmu_tlb_invalidate(data->sfrbases[i]);
				sysmmu_unblock(data->sfrbases[i]);
			}
		}
	} else {
		dev_dbg(data->sysmmu,
			"(%s) Disabled. Skipping invalidating TLB.\n",
			data->dbgname);
	}

	read_unlock_irqrestore(&data->lock, flags);
}

static int exynos_sysmmu_probe(struct platform_device *pdev)
{
	int i, ret;
	struct device *dev;
	struct sysmmu_drvdata *data;

	dev = &pdev->dev;

	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (!data) {
		dev_dbg(dev, "Not enough memory\n");
		ret = -ENOMEM;
		goto err_alloc;
	}

	ret = dev_set_drvdata(dev, data);
	if (ret) {
		dev_dbg(dev, "Unabled to initialize driver data\n");
		goto err_init;
	}

	data->nsfrs = pdev->num_resources / 2;
	data->sfrbases = kmalloc(sizeof(*data->sfrbases) * data->nsfrs,
								GFP_KERNEL);
	if (data->sfrbases == NULL) {
		dev_dbg(dev, "Not enough memory\n");
		ret = -ENOMEM;
		goto err_init;
	}

	for (i = 0; i < data->nsfrs; i++) {
		struct resource *res;
		res = platform_get_resource(pdev, IORESOURCE_MEM, i);
		if (!res) {
			dev_dbg(dev, "Unable to find IOMEM region\n");
			ret = -ENOENT;
			goto err_res;
		}

		data->sfrbases[i] = ioremap(res->start, resource_size(res));
		if (!data->sfrbases[i]) {
			dev_dbg(dev, "Unable to map IOMEM @ PA:%#x\n",
							res->start);
			ret = -ENOENT;
			goto err_res;
		}
	}

	for (i = 0; i < data->nsfrs; i++) {
		ret = platform_get_irq(pdev, i);
		if (ret <= 0) {
			dev_dbg(dev, "Unable to find IRQ resource\n");
			goto err_irq;
		}

		ret = request_irq(ret, exynos_sysmmu_irq, 0,
					dev_name(dev), data);
		if (ret) {
			dev_dbg(dev, "Unabled to register interrupt handler\n");
			goto err_irq;
		}
	}

	if (dev_get_platdata(dev)) {
		char *deli, *beg;
		struct sysmmu_platform_data *platdata = dev_get_platdata(dev);

		beg = platdata->clockname;

		for (deli = beg; (*deli != '\0') && (*deli != ','); deli++)
			/* NOTHING */;

		if (*deli == '\0')
			deli = NULL;
		else
			*deli = '\0';

		data->clk[0] = clk_get(dev, beg);
		if (IS_ERR(data->clk[0])) {
			data->clk[0] = NULL;
			dev_dbg(dev, "No clock descriptor registered\n");
		}

		if (data->clk[0] && deli) {
			*deli = ',';
			data->clk[1] = clk_get(dev, deli + 1);
			if (IS_ERR(data->clk[1]))
				data->clk[1] = NULL;
		}

		data->dbgname = platdata->dbgname;
	}

	data->sysmmu = dev;
	rwlock_init(&data->lock);
	INIT_LIST_HEAD(&data->node);

	__set_fault_handler(data, &default_fault_handler);

	if (dev->parent)
		pm_runtime_enable(dev);

	dev_dbg(dev, "(%s) Initialized\n", data->dbgname);
	return 0;
err_irq:
	while (i-- > 0) {
		int irq;

		irq = platform_get_irq(pdev, i);
		free_irq(irq, data);
	}
err_res:
	while (data->nsfrs-- > 0)
		iounmap(data->sfrbases[data->nsfrs]);
	kfree(data->sfrbases);
err_init:
	kfree(data);
err_alloc:
	dev_err(dev, "Failed to initialize\n");
	return ret;
}

static struct platform_driver exynos_sysmmu_driver = {
	.probe		= exynos_sysmmu_probe,
	.driver		= {
		.owner		= THIS_MODULE,
		.name		= "exynos-sysmmu",
=======
=======
>>>>>>> v3.18
	struct exynos_iommu_owner *owner = dev->archdata.iommu;
	unsigned long flags;
	struct sysmmu_drvdata *data;

	data = dev_get_drvdata(owner->sysmmu);

	spin_lock_irqsave(&data->lock, flags);
	if (is_sysmmu_active(data)) {
		if (!IS_ERR(data->clk_master))
			clk_enable(data->clk_master);
		if (sysmmu_block(data->sfrbase)) {
			__sysmmu_tlb_invalidate(data->sfrbase);
			sysmmu_unblock(data->sfrbase);
		}
		if (!IS_ERR(data->clk_master))
			clk_disable(data->clk_master);
	} else {
		dev_dbg(dev, "disabled. Skipping TLB invalidation\n");
	}
	spin_unlock_irqrestore(&data->lock, flags);
}

static int __init exynos_sysmmu_probe(struct platform_device *pdev)
{
	int irq, ret;
	struct device *dev = &pdev->dev;
	struct sysmmu_drvdata *data;
	struct resource *res;

	data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	data->sfrbase = devm_ioremap_resource(dev, res);
	if (IS_ERR(data->sfrbase))
		return PTR_ERR(data->sfrbase);

	irq = platform_get_irq(pdev, 0);
	if (irq <= 0) {
		dev_err(dev, "Unable to find IRQ resource\n");
		return irq;
	}

	ret = devm_request_irq(dev, irq, exynos_sysmmu_irq, 0,
				dev_name(dev), data);
	if (ret) {
		dev_err(dev, "Unabled to register handler of irq %d\n", irq);
		return ret;
	}

	data->clk = devm_clk_get(dev, "sysmmu");
	if (IS_ERR(data->clk)) {
		dev_err(dev, "Failed to get clock!\n");
		return PTR_ERR(data->clk);
	} else  {
		ret = clk_prepare(data->clk);
		if (ret) {
			dev_err(dev, "Failed to prepare clk\n");
			return ret;
		}
	}

	data->clk_master = devm_clk_get(dev, "master");
	if (!IS_ERR(data->clk_master)) {
		ret = clk_prepare(data->clk_master);
		if (ret) {
			clk_unprepare(data->clk);
			dev_err(dev, "Failed to prepare master's clk\n");
			return ret;
		}
	}

	data->sysmmu = dev;
	spin_lock_init(&data->lock);

	platform_set_drvdata(pdev, data);

	pm_runtime_enable(dev);

	return 0;
}

static const struct of_device_id sysmmu_of_match[] __initconst = {
	{ .compatible	= "samsung,exynos-sysmmu", },
	{ },
};

static struct platform_driver exynos_sysmmu_driver __refdata = {
	.probe	= exynos_sysmmu_probe,
	.driver	= {
		.owner		= THIS_MODULE,
		.name		= "exynos-sysmmu",
		.of_match_table	= sysmmu_of_match,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}
};

static inline void pgtable_flush(void *vastart, void *vaend)
{
	dmac_flush_range(vastart, vaend);
	outer_flush_range(virt_to_phys(vastart),
				virt_to_phys(vaend));
}

static int exynos_iommu_domain_init(struct iommu_domain *domain)
{
	struct exynos_iommu_domain *priv;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	int i;
>>>>>>> v3.18
=======
	int i;
>>>>>>> v3.18

	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

<<<<<<< HEAD
<<<<<<< HEAD
	priv->pgtable = (unsigned long *)__get_free_pages(
						GFP_KERNEL | __GFP_ZERO, 2);
	if (!priv->pgtable)
		goto err_pgtable;

	priv->lv2entcnt = (short *)__get_free_pages(
						GFP_KERNEL | __GFP_ZERO, 1);
	if (!priv->lv2entcnt)
		goto err_counter;

=======
=======
>>>>>>> v3.18
	priv->pgtable = (sysmmu_pte_t *)__get_free_pages(GFP_KERNEL, 2);
	if (!priv->pgtable)
		goto err_pgtable;

	priv->lv2entcnt = (short *)__get_free_pages(GFP_KERNEL | __GFP_ZERO, 1);
	if (!priv->lv2entcnt)
		goto err_counter;

	/* Workaround for System MMU v3.3 to prevent caching 1MiB mapping */
	for (i = 0; i < NUM_LV1ENTRIES; i += 8) {
		priv->pgtable[i + 0] = ZERO_LV2LINK;
		priv->pgtable[i + 1] = ZERO_LV2LINK;
		priv->pgtable[i + 2] = ZERO_LV2LINK;
		priv->pgtable[i + 3] = ZERO_LV2LINK;
		priv->pgtable[i + 4] = ZERO_LV2LINK;
		priv->pgtable[i + 5] = ZERO_LV2LINK;
		priv->pgtable[i + 6] = ZERO_LV2LINK;
		priv->pgtable[i + 7] = ZERO_LV2LINK;
	}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	pgtable_flush(priv->pgtable, priv->pgtable + NUM_LV1ENTRIES);

	spin_lock_init(&priv->lock);
	spin_lock_init(&priv->pgtablelock);
	INIT_LIST_HEAD(&priv->clients);

	domain->geometry.aperture_start = 0;
	domain->geometry.aperture_end   = ~0UL;
	domain->geometry.force_aperture = true;

	domain->priv = priv;
	return 0;

err_counter:
	free_pages((unsigned long)priv->pgtable, 2);
err_pgtable:
	kfree(priv);
	return -ENOMEM;
}

static void exynos_iommu_domain_destroy(struct iommu_domain *domain)
{
	struct exynos_iommu_domain *priv = domain->priv;
<<<<<<< HEAD
<<<<<<< HEAD
	struct sysmmu_drvdata *data;
=======
	struct exynos_iommu_owner *owner;
>>>>>>> v3.18
=======
	struct exynos_iommu_owner *owner;
>>>>>>> v3.18
	unsigned long flags;
	int i;

	WARN_ON(!list_empty(&priv->clients));

	spin_lock_irqsave(&priv->lock, flags);

<<<<<<< HEAD
<<<<<<< HEAD
	list_for_each_entry(data, &priv->clients, node) {
		while (!exynos_sysmmu_disable(data->dev))
			; /* until System MMU is actually disabled */
	}

=======
=======
>>>>>>> v3.18
	list_for_each_entry(owner, &priv->clients, client) {
		while (!exynos_sysmmu_disable(owner->dev))
			; /* until System MMU is actually disabled */
	}

	while (!list_empty(&priv->clients))
		list_del_init(priv->clients.next);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	spin_unlock_irqrestore(&priv->lock, flags);

	for (i = 0; i < NUM_LV1ENTRIES; i++)
		if (lv1ent_page(priv->pgtable + i))
<<<<<<< HEAD
<<<<<<< HEAD
			kfree(__va(lv2table_base(priv->pgtable + i)));
=======
			kmem_cache_free(lv2table_kmem_cache,
				phys_to_virt(lv2table_base(priv->pgtable + i)));
>>>>>>> v3.18
=======
			kmem_cache_free(lv2table_kmem_cache,
				phys_to_virt(lv2table_base(priv->pgtable + i)));
>>>>>>> v3.18

	free_pages((unsigned long)priv->pgtable, 2);
	free_pages((unsigned long)priv->lv2entcnt, 1);
	kfree(domain->priv);
	domain->priv = NULL;
}

static int exynos_iommu_attach_device(struct iommu_domain *domain,
				   struct device *dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct sysmmu_drvdata *data = dev_get_drvdata(dev->archdata.iommu);
	struct exynos_iommu_domain *priv = domain->priv;
	unsigned long flags;
	int ret;

	ret = pm_runtime_get_sync(data->sysmmu);
	if (ret < 0)
		return ret;

	ret = 0;

	spin_lock_irqsave(&priv->lock, flags);

	ret = __exynos_sysmmu_enable(data, __pa(priv->pgtable), domain);

	if (ret == 0) {
		/* 'data->node' must not be appeared in priv->clients */
		BUG_ON(!list_empty(&data->node));
		data->dev = dev;
		list_add_tail(&data->node, &priv->clients);
=======
=======
>>>>>>> v3.18
	struct exynos_iommu_owner *owner = dev->archdata.iommu;
	struct exynos_iommu_domain *priv = domain->priv;
	phys_addr_t pagetable = virt_to_phys(priv->pgtable);
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&priv->lock, flags);

	ret = __exynos_sysmmu_enable(dev, pagetable, domain);
	if (ret == 0) {
		list_add_tail(&owner->client, &priv->clients);
		owner->domain = domain;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}

	spin_unlock_irqrestore(&priv->lock, flags);

	if (ret < 0) {
<<<<<<< HEAD
<<<<<<< HEAD
		dev_err(dev, "%s: Failed to attach IOMMU with pgtable %#lx\n",
				__func__, __pa(priv->pgtable));
		pm_runtime_put(data->sysmmu);
	} else if (ret > 0) {
		dev_dbg(dev, "%s: IOMMU with pgtable 0x%lx already attached\n",
					__func__, __pa(priv->pgtable));
	} else {
		dev_dbg(dev, "%s: Attached new IOMMU with pgtable 0x%lx\n",
					__func__, __pa(priv->pgtable));
	}

=======
=======
>>>>>>> v3.18
		dev_err(dev, "%s: Failed to attach IOMMU with pgtable %pa\n",
					__func__, &pagetable);
		return ret;
	}

	dev_dbg(dev, "%s: Attached IOMMU with pgtable %pa %s\n",
		__func__, &pagetable, (ret == 0) ? "" : ", again");

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return ret;
}

static void exynos_iommu_detach_device(struct iommu_domain *domain,
				    struct device *dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct sysmmu_drvdata *data = dev_get_drvdata(dev->archdata.iommu);
	struct exynos_iommu_domain *priv = domain->priv;
	struct list_head *pos;
	unsigned long flags;
	bool found = false;

	spin_lock_irqsave(&priv->lock, flags);

	list_for_each(pos, &priv->clients) {
		if (list_entry(pos, struct sysmmu_drvdata, node) == data) {
			found = true;
=======
=======
>>>>>>> v3.18
	struct exynos_iommu_owner *owner;
	struct exynos_iommu_domain *priv = domain->priv;
	phys_addr_t pagetable = virt_to_phys(priv->pgtable);
	unsigned long flags;

	spin_lock_irqsave(&priv->lock, flags);

	list_for_each_entry(owner, &priv->clients, client) {
		if (owner == dev->archdata.iommu) {
			if (exynos_sysmmu_disable(dev)) {
				list_del_init(&owner->client);
				owner->domain = NULL;
			}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			break;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (!found)
		goto finish;

	if (__exynos_sysmmu_disable(data)) {
		dev_dbg(dev, "%s: Detached IOMMU with pgtable %#lx\n",
					__func__, __pa(priv->pgtable));
		list_del_init(&data->node);

	} else {
		dev_dbg(dev, "%s: Detaching IOMMU with pgtable %#lx delayed",
					__func__, __pa(priv->pgtable));
	}

finish:
	spin_unlock_irqrestore(&priv->lock, flags);

	if (found)
		pm_runtime_put(data->sysmmu);
}

static unsigned long *alloc_lv2entry(unsigned long *sent, unsigned long iova,
					short *pgcounter)
{
	if (lv1ent_fault(sent)) {
		unsigned long *pent;

		pent = kzalloc(LV2TABLE_SIZE, GFP_ATOMIC);
		BUG_ON((unsigned long)pent & (LV2TABLE_SIZE - 1));
		if (!pent)
			return NULL;

		*sent = mk_lv1ent_page(__pa(pent));
		*pgcounter = NUM_LV2ENTRIES;
		pgtable_flush(pent, pent + NUM_LV2ENTRIES);
		pgtable_flush(sent, sent + 1);
=======
=======
>>>>>>> v3.18
	spin_unlock_irqrestore(&priv->lock, flags);

	if (owner == dev->archdata.iommu)
		dev_dbg(dev, "%s: Detached IOMMU with pgtable %pa\n",
					__func__, &pagetable);
	else
		dev_err(dev, "%s: No IOMMU is attached\n", __func__);
}

static sysmmu_pte_t *alloc_lv2entry(struct exynos_iommu_domain *priv,
		sysmmu_pte_t *sent, sysmmu_iova_t iova, short *pgcounter)
{
	if (lv1ent_section(sent)) {
		WARN(1, "Trying mapping on %#08x mapped with 1MiB page", iova);
		return ERR_PTR(-EADDRINUSE);
	}

	if (lv1ent_fault(sent)) {
		sysmmu_pte_t *pent;
		bool need_flush_flpd_cache = lv1ent_zero(sent);

		pent = kmem_cache_zalloc(lv2table_kmem_cache, GFP_ATOMIC);
		BUG_ON((unsigned int)pent & (LV2TABLE_SIZE - 1));
		if (!pent)
			return ERR_PTR(-ENOMEM);

		*sent = mk_lv1ent_page(virt_to_phys(pent));
		*pgcounter = NUM_LV2ENTRIES;
		pgtable_flush(pent, pent + NUM_LV2ENTRIES);
		pgtable_flush(sent, sent + 1);

		/*
		 * If pre-fetched SLPD is a faulty SLPD in zero_l2_table,
		 * FLPD cache may cache the address of zero_l2_table. This
		 * function replaces the zero_l2_table with new L2 page table
		 * to write valid mappings.
		 * Accessing the valid area may cause page fault since FLPD
		 * cache may still cache zero_l2_table for the valid area
		 * instead of new L2 page table that has the mapping
		 * information of the valid area.
		 * Thus any replacement of zero_l2_table with other valid L2
		 * page table must involve FLPD cache invalidation for System
		 * MMU v3.3.
		 * FLPD cache invalidation is performed with TLB invalidation
		 * by VPN without blocking. It is safe to invalidate TLB without
		 * blocking because the target address of TLB invalidation is
		 * not currently mapped.
		 */
		if (need_flush_flpd_cache) {
			struct exynos_iommu_owner *owner;

			spin_lock(&priv->lock);
			list_for_each_entry(owner, &priv->clients, client)
				sysmmu_tlb_invalidate_flpdcache(
							owner->dev, iova);
			spin_unlock(&priv->lock);
		}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}

	return page_entry(sent, iova);
}

<<<<<<< HEAD
<<<<<<< HEAD
static int lv1set_section(unsigned long *sent, phys_addr_t paddr, short *pgcnt)
{
	if (lv1ent_section(sent))
		return -EADDRINUSE;

	if (lv1ent_page(sent)) {
		if (*pgcnt != NUM_LV2ENTRIES)
			return -EADDRINUSE;

		kfree(page_entry(sent, 0));

=======
=======
>>>>>>> v3.18
static int lv1set_section(struct exynos_iommu_domain *priv,
			  sysmmu_pte_t *sent, sysmmu_iova_t iova,
			  phys_addr_t paddr, short *pgcnt)
{
	if (lv1ent_section(sent)) {
		WARN(1, "Trying mapping on 1MiB@%#08x that is mapped",
			iova);
		return -EADDRINUSE;
	}

	if (lv1ent_page(sent)) {
		if (*pgcnt != NUM_LV2ENTRIES) {
			WARN(1, "Trying mapping on 1MiB@%#08x that is mapped",
				iova);
			return -EADDRINUSE;
		}

		kmem_cache_free(lv2table_kmem_cache, page_entry(sent, 0));
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		*pgcnt = 0;
	}

	*sent = mk_lv1ent_sect(paddr);

	pgtable_flush(sent, sent + 1);

<<<<<<< HEAD
<<<<<<< HEAD
	return 0;
}

static int lv2set_page(unsigned long *pent, phys_addr_t paddr, size_t size,
								short *pgcnt)
{
	if (size == SPAGE_SIZE) {
		if (!lv2ent_fault(pent))
=======
=======
>>>>>>> v3.18
	spin_lock(&priv->lock);
	if (lv1ent_page_zero(sent)) {
		struct exynos_iommu_owner *owner;
		/*
		 * Flushing FLPD cache in System MMU v3.3 that may cache a FLPD
		 * entry by speculative prefetch of SLPD which has no mapping.
		 */
		list_for_each_entry(owner, &priv->clients, client)
			sysmmu_tlb_invalidate_flpdcache(owner->dev, iova);
	}
	spin_unlock(&priv->lock);

	return 0;
}

static int lv2set_page(sysmmu_pte_t *pent, phys_addr_t paddr, size_t size,
								short *pgcnt)
{
	if (size == SPAGE_SIZE) {
		if (WARN_ON(!lv2ent_fault(pent)))
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
			return -EADDRINUSE;

		*pent = mk_lv2ent_spage(paddr);
		pgtable_flush(pent, pent + 1);
		*pgcnt -= 1;
	} else { /* size == LPAGE_SIZE */
		int i;
<<<<<<< HEAD
<<<<<<< HEAD
		for (i = 0; i < SPAGES_PER_LPAGE; i++, pent++) {
			if (!lv2ent_fault(pent)) {
				memset(pent, 0, sizeof(*pent) * i);
=======
=======
>>>>>>> v3.18

		for (i = 0; i < SPAGES_PER_LPAGE; i++, pent++) {
			if (WARN_ON(!lv2ent_fault(pent))) {
				if (i > 0)
					memset(pent - i, 0, sizeof(*pent) * i);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
				return -EADDRINUSE;
			}

			*pent = mk_lv2ent_lpage(paddr);
		}
		pgtable_flush(pent - SPAGES_PER_LPAGE, pent);
		*pgcnt -= SPAGES_PER_LPAGE;
	}

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int exynos_iommu_map(struct iommu_domain *domain, unsigned long iova,
			 phys_addr_t paddr, size_t size, int prot)
{
	struct exynos_iommu_domain *priv = domain->priv;
	unsigned long *entry;
=======
=======
>>>>>>> v3.18
/*
 * *CAUTION* to the I/O virtual memory managers that support exynos-iommu:
 *
 * System MMU v3.x has advanced logic to improve address translation
 * performance with caching more page table entries by a page table walk.
 * However, the logic has a bug that while caching faulty page table entries,
 * System MMU reports page fault if the cached fault entry is hit even though
 * the fault entry is updated to a valid entry after the entry is cached.
 * To prevent caching faulty page table entries which may be updated to valid
 * entries later, the virtual memory manager should care about the workaround
 * for the problem. The following describes the workaround.
 *
 * Any two consecutive I/O virtual address regions must have a hole of 128KiB
 * at maximum to prevent misbehavior of System MMU 3.x (workaround for h/w bug).
 *
 * Precisely, any start address of I/O virtual region must be aligned with
 * the following sizes for System MMU v3.1 and v3.2.
 * System MMU v3.1: 128KiB
 * System MMU v3.2: 256KiB
 *
 * Because System MMU v3.3 caches page table entries more aggressively, it needs
 * more workarounds.
 * - Any two consecutive I/O virtual regions must have a hole of size larger
 *   than or equal to 128KiB.
 * - Start address of an I/O virtual region must be aligned by 128KiB.
 */
static int exynos_iommu_map(struct iommu_domain *domain, unsigned long l_iova,
			 phys_addr_t paddr, size_t size, int prot)
{
	struct exynos_iommu_domain *priv = domain->priv;
	sysmmu_pte_t *entry;
	sysmmu_iova_t iova = (sysmmu_iova_t)l_iova;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	unsigned long flags;
	int ret = -ENOMEM;

	BUG_ON(priv->pgtable == NULL);

	spin_lock_irqsave(&priv->pgtablelock, flags);

	entry = section_entry(priv->pgtable, iova);

	if (size == SECT_SIZE) {
<<<<<<< HEAD
<<<<<<< HEAD
		ret = lv1set_section(entry, paddr,
					&priv->lv2entcnt[lv1ent_offset(iova)]);
	} else {
		unsigned long *pent;

		pent = alloc_lv2entry(entry, iova,
					&priv->lv2entcnt[lv1ent_offset(iova)]);

		if (!pent)
			ret = -ENOMEM;
=======
=======
>>>>>>> v3.18
		ret = lv1set_section(priv, entry, iova, paddr,
					&priv->lv2entcnt[lv1ent_offset(iova)]);
	} else {
		sysmmu_pte_t *pent;

		pent = alloc_lv2entry(priv, entry, iova,
					&priv->lv2entcnt[lv1ent_offset(iova)]);

		if (IS_ERR(pent))
			ret = PTR_ERR(pent);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		else
			ret = lv2set_page(pent, paddr, size,
					&priv->lv2entcnt[lv1ent_offset(iova)]);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (ret) {
		pr_debug("%s: Failed to map iova 0x%lx/0x%x bytes\n",
							__func__, iova, size);
	}
=======
	if (ret)
		pr_err("%s: Failed(%d) to map %#zx bytes @ %#x\n",
			__func__, ret, size, iova);
>>>>>>> v3.18
=======
	if (ret)
		pr_err("%s: Failed(%d) to map %#zx bytes @ %#x\n",
			__func__, ret, size, iova);
>>>>>>> v3.18

	spin_unlock_irqrestore(&priv->pgtablelock, flags);

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static size_t exynos_iommu_unmap(struct iommu_domain *domain,
					       unsigned long iova, size_t size)
{
	struct exynos_iommu_domain *priv = domain->priv;
	struct sysmmu_drvdata *data;
	unsigned long flags;
	unsigned long *ent;
=======
=======
>>>>>>> v3.18
static void exynos_iommu_tlb_invalidate_entry(struct exynos_iommu_domain *priv,
						sysmmu_iova_t iova, size_t size)
{
	struct exynos_iommu_owner *owner;
	unsigned long flags;

	spin_lock_irqsave(&priv->lock, flags);

	list_for_each_entry(owner, &priv->clients, client)
		sysmmu_tlb_invalidate_entry(owner->dev, iova, size);

	spin_unlock_irqrestore(&priv->lock, flags);
}

static size_t exynos_iommu_unmap(struct iommu_domain *domain,
					unsigned long l_iova, size_t size)
{
	struct exynos_iommu_domain *priv = domain->priv;
	sysmmu_iova_t iova = (sysmmu_iova_t)l_iova;
	sysmmu_pte_t *ent;
	size_t err_pgsize;
	unsigned long flags;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	BUG_ON(priv->pgtable == NULL);

	spin_lock_irqsave(&priv->pgtablelock, flags);

	ent = section_entry(priv->pgtable, iova);

	if (lv1ent_section(ent)) {
<<<<<<< HEAD
<<<<<<< HEAD
		BUG_ON(size < SECT_SIZE);

		*ent = 0;
=======
=======
>>>>>>> v3.18
		if (WARN_ON(size < SECT_SIZE)) {
			err_pgsize = SECT_SIZE;
			goto err;
		}

		/* workaround for h/w bug in System MMU v3.3 */
		*ent = ZERO_LV2LINK;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		pgtable_flush(ent, ent + 1);
		size = SECT_SIZE;
		goto done;
	}

	if (unlikely(lv1ent_fault(ent))) {
		if (size > SECT_SIZE)
			size = SECT_SIZE;
		goto done;
	}

	/* lv1ent_page(sent) == true here */

	ent = page_entry(ent, iova);

	if (unlikely(lv2ent_fault(ent))) {
		size = SPAGE_SIZE;
		goto done;
	}

	if (lv2ent_small(ent)) {
		*ent = 0;
		size = SPAGE_SIZE;
<<<<<<< HEAD
<<<<<<< HEAD
=======
		pgtable_flush(ent, ent + 1);
>>>>>>> v3.18
=======
		pgtable_flush(ent, ent + 1);
>>>>>>> v3.18
		priv->lv2entcnt[lv1ent_offset(iova)] += 1;
		goto done;
	}

	/* lv1ent_large(ent) == true here */
<<<<<<< HEAD
<<<<<<< HEAD
	BUG_ON(size < LPAGE_SIZE);

	memset(ent, 0, sizeof(*ent) * SPAGES_PER_LPAGE);
=======
=======
>>>>>>> v3.18
	if (WARN_ON(size < LPAGE_SIZE)) {
		err_pgsize = LPAGE_SIZE;
		goto err;
	}

	memset(ent, 0, sizeof(*ent) * SPAGES_PER_LPAGE);
	pgtable_flush(ent, ent + SPAGES_PER_LPAGE);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	size = LPAGE_SIZE;
	priv->lv2entcnt[lv1ent_offset(iova)] += SPAGES_PER_LPAGE;
done:
	spin_unlock_irqrestore(&priv->pgtablelock, flags);

<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock_irqsave(&priv->lock, flags);
	list_for_each_entry(data, &priv->clients, node)
		sysmmu_tlb_invalidate_entry(data->dev, iova);
	spin_unlock_irqrestore(&priv->lock, flags);


	return size;
=======
=======
>>>>>>> v3.18
	exynos_iommu_tlb_invalidate_entry(priv, iova, size);

	return size;
err:
	spin_unlock_irqrestore(&priv->pgtablelock, flags);

	pr_err("%s: Failed: size(%#zx) @ %#x is smaller than page size %#zx\n",
		__func__, size, iova, err_pgsize);

	return 0;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static phys_addr_t exynos_iommu_iova_to_phys(struct iommu_domain *domain,
					  dma_addr_t iova)
{
	struct exynos_iommu_domain *priv = domain->priv;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long *entry;
=======
	sysmmu_pte_t *entry;
>>>>>>> v3.18
=======
	sysmmu_pte_t *entry;
>>>>>>> v3.18
	unsigned long flags;
	phys_addr_t phys = 0;

	spin_lock_irqsave(&priv->pgtablelock, flags);

	entry = section_entry(priv->pgtable, iova);

	if (lv1ent_section(entry)) {
		phys = section_phys(entry) + section_offs(iova);
	} else if (lv1ent_page(entry)) {
		entry = page_entry(entry, iova);

		if (lv2ent_large(entry))
			phys = lpage_phys(entry) + lpage_offs(iova);
		else if (lv2ent_small(entry))
			phys = spage_phys(entry) + spage_offs(iova);
	}

	spin_unlock_irqrestore(&priv->pgtablelock, flags);

	return phys;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct iommu_ops exynos_iommu_ops = {
	.domain_init = &exynos_iommu_domain_init,
	.domain_destroy = &exynos_iommu_domain_destroy,
	.attach_dev = &exynos_iommu_attach_device,
	.detach_dev = &exynos_iommu_detach_device,
	.map = &exynos_iommu_map,
	.unmap = &exynos_iommu_unmap,
	.iova_to_phys = &exynos_iommu_iova_to_phys,
=======
=======
>>>>>>> v3.18
static int exynos_iommu_add_device(struct device *dev)
{
	struct iommu_group *group;
	int ret;

	group = iommu_group_get(dev);

	if (!group) {
		group = iommu_group_alloc();
		if (IS_ERR(group)) {
			dev_err(dev, "Failed to allocate IOMMU group\n");
			return PTR_ERR(group);
		}
	}

	ret = iommu_group_add_device(group, dev);
	iommu_group_put(group);

	return ret;
}

static void exynos_iommu_remove_device(struct device *dev)
{
	iommu_group_remove_device(dev);
}

static const struct iommu_ops exynos_iommu_ops = {
	.domain_init = exynos_iommu_domain_init,
	.domain_destroy = exynos_iommu_domain_destroy,
	.attach_dev = exynos_iommu_attach_device,
	.detach_dev = exynos_iommu_detach_device,
	.map = exynos_iommu_map,
	.unmap = exynos_iommu_unmap,
	.iova_to_phys = exynos_iommu_iova_to_phys,
	.add_device = exynos_iommu_add_device,
	.remove_device = exynos_iommu_remove_device,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	.pgsize_bitmap = SECT_SIZE | LPAGE_SIZE | SPAGE_SIZE,
};

static int __init exynos_iommu_init(void)
{
	int ret;

<<<<<<< HEAD
<<<<<<< HEAD
	ret = platform_driver_register(&exynos_sysmmu_driver);

	if (ret == 0)
		bus_set_iommu(&platform_bus_type, &exynos_iommu_ops);

=======
=======
>>>>>>> v3.18
	lv2table_kmem_cache = kmem_cache_create("exynos-iommu-lv2table",
				LV2TABLE_SIZE, LV2TABLE_SIZE, 0, NULL);
	if (!lv2table_kmem_cache) {
		pr_err("%s: Failed to create kmem cache\n", __func__);
		return -ENOMEM;
	}

	ret = platform_driver_register(&exynos_sysmmu_driver);
	if (ret) {
		pr_err("%s: Failed to register driver\n", __func__);
		goto err_reg_driver;
	}

	zero_lv2_table = kmem_cache_zalloc(lv2table_kmem_cache, GFP_KERNEL);
	if (zero_lv2_table == NULL) {
		pr_err("%s: Failed to allocate zero level2 page table\n",
			__func__);
		ret = -ENOMEM;
		goto err_zero_lv2;
	}

	ret = bus_set_iommu(&platform_bus_type, &exynos_iommu_ops);
	if (ret) {
		pr_err("%s: Failed to register exynos-iommu driver.\n",
								__func__);
		goto err_set_iommu;
	}

	return 0;
err_set_iommu:
	kmem_cache_free(lv2table_kmem_cache, zero_lv2_table);
err_zero_lv2:
	platform_driver_unregister(&exynos_sysmmu_driver);
err_reg_driver:
	kmem_cache_destroy(lv2table_kmem_cache);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	return ret;
}
subsys_initcall(exynos_iommu_init);
