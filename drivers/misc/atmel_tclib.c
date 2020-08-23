#include <linux/atmel_tc.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/export.h>
#include <linux/of.h>

/*
 * This is a thin library to solve the problem of how to portably allocate
 * one of the TC blocks.  For simplicity, it doesn't currently expect to
 * share individual timers between different drivers.
 */

#if defined(CONFIG_AVR32)
/* AVR32 has these divide PBB */
const u8 atmel_tc_divisors[5] = { 0, 4, 8, 16, 32, };
EXPORT_SYMBOL(atmel_tc_divisors);

#elif defined(CONFIG_ARCH_AT91)
/* AT91 has these divide MCK */
const u8 atmel_tc_divisors[5] = { 2, 8, 32, 128, 0, };
EXPORT_SYMBOL(atmel_tc_divisors);

#endif

static DEFINE_SPINLOCK(tc_list_lock);
static LIST_HEAD(tc_list);

/**
 * atmel_tc_alloc - allocate a specified TC block
 * @block: which block to allocate
<<<<<<< HEAD
<<<<<<< HEAD
 * @name: name to be associated with the iomem resource
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
 *
 * Caller allocates a block.  If it is available, a pointer to a
 * pre-initialized struct atmel_tc is returned. The caller can access
 * the registers directly through the "regs" field.
 */
<<<<<<< HEAD
<<<<<<< HEAD
struct atmel_tc *atmel_tc_alloc(unsigned block, const char *name)
{
	struct atmel_tc		*tc;
	struct platform_device	*pdev = NULL;
	struct resource		*r;
	size_t			size;

	spin_lock(&tc_list_lock);
	list_for_each_entry(tc, &tc_list, node) {
		if (tc->pdev->dev.of_node) {
			if (of_alias_get_id(tc->pdev->dev.of_node, "tcb")
					== block) {
				pdev = tc->pdev;
				break;
			}
		} else if (tc->pdev->id == block) {
			pdev = tc->pdev;
			break;
		}
	}

	if (!pdev || tc->iomem)
		goto fail;

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r)
		goto fail;

	size = resource_size(r);
	r = request_mem_region(r->start, size, name);
	if (!r)
		goto fail;

	tc->regs = ioremap(r->start, size);
	if (!tc->regs)
		goto fail_ioremap;

	tc->iomem = r;

out:
	spin_unlock(&tc_list_lock);
	return tc;

fail_ioremap:
	release_mem_region(r->start, size);
fail:
	tc = NULL;
	goto out;
=======
=======
>>>>>>> v3.18
struct atmel_tc *atmel_tc_alloc(unsigned block)
{
	struct atmel_tc		*tc;
	struct platform_device	*pdev = NULL;

	spin_lock(&tc_list_lock);
	list_for_each_entry(tc, &tc_list, node) {
		if (tc->allocated)
			continue;

		if ((tc->pdev->dev.of_node && tc->id == block) ||
		    (tc->pdev->id == block)) {
			pdev = tc->pdev;
			tc->allocated = true;
			break;
		}
	}
	spin_unlock(&tc_list_lock);

	return pdev ? tc : NULL;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}
EXPORT_SYMBOL_GPL(atmel_tc_alloc);

/**
 * atmel_tc_free - release a specified TC block
 * @tc: Timer/counter block that was returned by atmel_tc_alloc()
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * This reverses the effect of atmel_tc_alloc(), unmapping the I/O
 * registers, invalidating the resource returned by that routine and
 * making the TC available to other drivers.
=======
 * This reverses the effect of atmel_tc_alloc(), invalidating the resource
 * returned by that routine and making the TC available to other drivers.
>>>>>>> v3.18
=======
 * This reverses the effect of atmel_tc_alloc(), invalidating the resource
 * returned by that routine and making the TC available to other drivers.
>>>>>>> v3.18
 */
void atmel_tc_free(struct atmel_tc *tc)
{
	spin_lock(&tc_list_lock);
<<<<<<< HEAD
<<<<<<< HEAD
	if (tc->regs) {
		iounmap(tc->regs);
		release_mem_region(tc->iomem->start, resource_size(tc->iomem));
		tc->regs = NULL;
		tc->iomem = NULL;
	}
=======
	if (tc->allocated)
		tc->allocated = false;
>>>>>>> v3.18
=======
	if (tc->allocated)
		tc->allocated = false;
>>>>>>> v3.18
	spin_unlock(&tc_list_lock);
}
EXPORT_SYMBOL_GPL(atmel_tc_free);

#if defined(CONFIG_OF)
static struct atmel_tcb_config tcb_rm9200_config = {
	.counter_width = 16,
};

static struct atmel_tcb_config tcb_sam9x5_config = {
	.counter_width = 32,
};

static const struct of_device_id atmel_tcb_dt_ids[] = {
	{
		.compatible = "atmel,at91rm9200-tcb",
		.data = &tcb_rm9200_config,
	}, {
		.compatible = "atmel,at91sam9x5-tcb",
		.data = &tcb_sam9x5_config,
	}, {
		/* sentinel */
	}
};

MODULE_DEVICE_TABLE(of, atmel_tcb_dt_ids);
#endif

static int __init tc_probe(struct platform_device *pdev)
{
	struct atmel_tc *tc;
	struct clk	*clk;
	int		irq;
<<<<<<< HEAD
<<<<<<< HEAD

	if (!platform_get_resource(pdev, IORESOURCE_MEM, 0))
		return -EINVAL;
=======
	struct resource	*r;
	unsigned int	i;
>>>>>>> v3.18
=======
	struct resource	*r;
	unsigned int	i;
>>>>>>> v3.18

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	tc = kzalloc(sizeof(struct atmel_tc), GFP_KERNEL);
=======
	tc = devm_kzalloc(&pdev->dev, sizeof(struct atmel_tc), GFP_KERNEL);
>>>>>>> v3.18
=======
	tc = devm_kzalloc(&pdev->dev, sizeof(struct atmel_tc), GFP_KERNEL);
>>>>>>> v3.18
	if (!tc)
		return -ENOMEM;

	tc->pdev = pdev;

<<<<<<< HEAD
<<<<<<< HEAD
	clk = clk_get(&pdev->dev, "t0_clk");
	if (IS_ERR(clk)) {
		kfree(tc);
		return -EINVAL;
	}
=======
=======
>>>>>>> v3.18
	clk = devm_clk_get(&pdev->dev, "t0_clk");
	if (IS_ERR(clk))
		return PTR_ERR(clk);

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	tc->regs = devm_ioremap_resource(&pdev->dev, r);
	if (IS_ERR(tc->regs))
		return PTR_ERR(tc->regs);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/* Now take SoC information if available */
	if (pdev->dev.of_node) {
		const struct of_device_id *match;
		match = of_match_node(atmel_tcb_dt_ids, pdev->dev.of_node);
		if (match)
			tc->tcb_config = match->data;
<<<<<<< HEAD
<<<<<<< HEAD
	}

	tc->clk[0] = clk;
	tc->clk[1] = clk_get(&pdev->dev, "t1_clk");
	if (IS_ERR(tc->clk[1]))
		tc->clk[1] = clk;
	tc->clk[2] = clk_get(&pdev->dev, "t2_clk");
=======
=======
>>>>>>> v3.18

		tc->id = of_alias_get_id(tc->pdev->dev.of_node, "tcb");
	} else {
		tc->id = pdev->id;
	}

	tc->clk[0] = clk;
	tc->clk[1] = devm_clk_get(&pdev->dev, "t1_clk");
	if (IS_ERR(tc->clk[1]))
		tc->clk[1] = clk;
	tc->clk[2] = devm_clk_get(&pdev->dev, "t2_clk");
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	if (IS_ERR(tc->clk[2]))
		tc->clk[2] = clk;

	tc->irq[0] = irq;
	tc->irq[1] = platform_get_irq(pdev, 1);
	if (tc->irq[1] < 0)
		tc->irq[1] = irq;
	tc->irq[2] = platform_get_irq(pdev, 2);
	if (tc->irq[2] < 0)
		tc->irq[2] = irq;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	for (i = 0; i < 3; i++)
		writel(ATMEL_TC_ALL_IRQ, tc->regs + ATMEL_TC_REG(i, IDR));

>>>>>>> v3.18
=======
	for (i = 0; i < 3; i++)
		writel(ATMEL_TC_ALL_IRQ, tc->regs + ATMEL_TC_REG(i, IDR));

>>>>>>> v3.18
	spin_lock(&tc_list_lock);
	list_add_tail(&tc->node, &tc_list);
	spin_unlock(&tc_list_lock);

<<<<<<< HEAD
<<<<<<< HEAD
	return 0;
}

=======
=======
>>>>>>> v3.18
	platform_set_drvdata(pdev, tc);

	return 0;
}

static void tc_shutdown(struct platform_device *pdev)
{
	int i;
	struct atmel_tc *tc = platform_get_drvdata(pdev);

	for (i = 0; i < 3; i++)
		writel(ATMEL_TC_ALL_IRQ, tc->regs + ATMEL_TC_REG(i, IDR));
}

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static struct platform_driver tc_driver = {
	.driver = {
		.name	= "atmel_tcb",
		.of_match_table	= of_match_ptr(atmel_tcb_dt_ids),
	},
<<<<<<< HEAD
<<<<<<< HEAD
=======
	.shutdown = tc_shutdown,
>>>>>>> v3.18
=======
	.shutdown = tc_shutdown,
>>>>>>> v3.18
};

static int __init tc_init(void)
{
	return platform_driver_probe(&tc_driver, tc_probe);
}
arch_initcall(tc_init);
