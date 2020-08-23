/*
 * Copyright 2012 Freescale Semiconductor, Inc.
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/delay.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include "ci13xxx_imx.h"

#define USB_DEV_MAX 4
=======
#include "ci_hdrc_imx.h"
>>>>>>> v3.18
=======
#include "ci_hdrc_imx.h"
>>>>>>> v3.18

#define MX25_USB_PHY_CTRL_OFFSET	0x08
#define MX25_BM_EXTERNAL_VBUS_DIVIDER	BIT(23)

<<<<<<< HEAD
<<<<<<< HEAD
#define MX53_USB_OTG_PHY_CTRL_0_OFFSET	0x08
=======
=======
>>>>>>> v3.18
#define MX25_EHCI_INTERFACE_SINGLE_UNI	(2 << 0)
#define MX25_EHCI_INTERFACE_DIFF_UNI	(0 << 0)
#define MX25_EHCI_INTERFACE_MASK	(0xf)

#define MX25_OTG_SIC_SHIFT		29
#define MX25_OTG_SIC_MASK		(0x3 << MX25_OTG_SIC_SHIFT)
#define MX25_OTG_PM_BIT			BIT(24)
#define MX25_OTG_PP_BIT			BIT(11)
#define MX25_OTG_OCPOL_BIT		BIT(3)

#define MX25_H1_SIC_SHIFT		21
#define MX25_H1_SIC_MASK		(0x3 << MX25_H1_SIC_SHIFT)
#define MX25_H1_PP_BIT			BIT(18)
#define MX25_H1_PM_BIT			BIT(16)
#define MX25_H1_IPPUE_UP_BIT		BIT(7)
#define MX25_H1_IPPUE_DOWN_BIT		BIT(6)
#define MX25_H1_TLL_BIT			BIT(5)
#define MX25_H1_USBTE_BIT		BIT(4)
#define MX25_H1_OCPOL_BIT		BIT(2)

#define MX27_H1_PM_BIT			BIT(8)
#define MX27_H2_PM_BIT			BIT(16)
#define MX27_OTG_PM_BIT			BIT(24)

#define MX53_USB_OTG_PHY_CTRL_0_OFFSET	0x08
#define MX53_USB_OTG_PHY_CTRL_1_OFFSET	0x0c
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#define MX53_USB_UH2_CTRL_OFFSET	0x14
#define MX53_USB_UH3_CTRL_OFFSET	0x18
#define MX53_BM_OVER_CUR_DIS_H1		BIT(5)
#define MX53_BM_OVER_CUR_DIS_OTG	BIT(8)
#define MX53_BM_OVER_CUR_DIS_UHx	BIT(30)
<<<<<<< HEAD
<<<<<<< HEAD

#define MX6_BM_OVER_CUR_DIS		BIT(7)

=======
=======
>>>>>>> v3.18
#define MX53_USB_PHYCTRL1_PLLDIV_MASK	0x3
#define MX53_USB_PLL_DIV_24_MHZ		0x01

#define MX6_BM_OVER_CUR_DIS		BIT(7)

#define VF610_OVER_CUR_DIS		BIT(7)

struct usbmisc_ops {
	/* It's called once when probe a usb device */
	int (*init)(struct imx_usbmisc_data *data);
	/* It's called once after adding a usb device */
	int (*post)(struct imx_usbmisc_data *data);
};

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
struct imx_usbmisc {
	void __iomem *base;
	spinlock_t lock;
	struct clk *clk;
<<<<<<< HEAD
<<<<<<< HEAD
	struct usbmisc_usb_device usbdev[USB_DEV_MAX];
	const struct usbmisc_ops *ops;
};

static struct imx_usbmisc *usbmisc;

static struct usbmisc_usb_device *get_usbdev(struct device *dev)
{
	int i, ret;

	for (i = 0; i < USB_DEV_MAX; i++) {
		if (usbmisc->usbdev[i].dev == dev)
			return &usbmisc->usbdev[i];
		else if (!usbmisc->usbdev[i].dev)
			break;
	}

	if (i >= USB_DEV_MAX)
		return ERR_PTR(-EBUSY);

	ret = usbmisc_get_init_data(dev, &usbmisc->usbdev[i]);
	if (ret)
		return ERR_PTR(ret);

	return &usbmisc->usbdev[i];
}

static int usbmisc_imx25_post(struct device *dev)
{
	struct usbmisc_usb_device *usbdev;
=======
=======
>>>>>>> v3.18
	const struct usbmisc_ops *ops;
};

static int usbmisc_imx25_init(struct imx_usbmisc_data *data)
{
	struct imx_usbmisc *usbmisc = dev_get_drvdata(data->dev);
	unsigned long flags;
	u32 val = 0;

	if (data->index > 1)
		return -EINVAL;

	spin_lock_irqsave(&usbmisc->lock, flags);
	switch (data->index) {
	case 0:
		val = readl(usbmisc->base);
		val &= ~(MX25_OTG_SIC_MASK | MX25_OTG_PP_BIT);
		val |= (MX25_EHCI_INTERFACE_DIFF_UNI & MX25_EHCI_INTERFACE_MASK) << MX25_OTG_SIC_SHIFT;
		val |= (MX25_OTG_PM_BIT | MX25_OTG_OCPOL_BIT);
		writel(val, usbmisc->base);
		break;
	case 1:
		val = readl(usbmisc->base);
		val &= ~(MX25_H1_SIC_MASK | MX25_H1_PP_BIT |  MX25_H1_IPPUE_UP_BIT);
		val |= (MX25_EHCI_INTERFACE_SINGLE_UNI & MX25_EHCI_INTERFACE_MASK) << MX25_H1_SIC_SHIFT;
		val |= (MX25_H1_PM_BIT | MX25_H1_OCPOL_BIT | MX25_H1_TLL_BIT |
			MX25_H1_USBTE_BIT | MX25_H1_IPPUE_DOWN_BIT);

		writel(val, usbmisc->base);

		break;
	}
	spin_unlock_irqrestore(&usbmisc->lock, flags);

	return 0;
}

static int usbmisc_imx25_post(struct imx_usbmisc_data *data)
{
	struct imx_usbmisc *usbmisc = dev_get_drvdata(data->dev);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	void __iomem *reg;
	unsigned long flags;
	u32 val;

<<<<<<< HEAD
<<<<<<< HEAD
	usbdev = get_usbdev(dev);
	if (IS_ERR(usbdev))
		return PTR_ERR(usbdev);

	reg = usbmisc->base + MX25_USB_PHY_CTRL_OFFSET;

	if (usbdev->evdo) {
=======
=======
>>>>>>> v3.18
	if (data->index > 2)
		return -EINVAL;

	reg = usbmisc->base + MX25_USB_PHY_CTRL_OFFSET;

	if (data->evdo) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		spin_lock_irqsave(&usbmisc->lock, flags);
		val = readl(reg);
		writel(val | MX25_BM_EXTERNAL_VBUS_DIVIDER, reg);
		spin_unlock_irqrestore(&usbmisc->lock, flags);
		usleep_range(5000, 10000); /* needed to stabilize voltage */
	}

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int usbmisc_imx53_init(struct device *dev)
{
	struct usbmisc_usb_device *usbdev;
=======
=======
>>>>>>> v3.18
static int usbmisc_imx27_init(struct imx_usbmisc_data *data)
{
	struct imx_usbmisc *usbmisc = dev_get_drvdata(data->dev);
	unsigned long flags;
	u32 val;

	switch (data->index) {
	case 0:
		val = MX27_OTG_PM_BIT;
		break;
	case 1:
		val = MX27_H1_PM_BIT;
		break;
	case 2:
		val = MX27_H2_PM_BIT;
		break;
	default:
		return -EINVAL;
	};

	spin_lock_irqsave(&usbmisc->lock, flags);
	if (data->disable_oc)
		val = readl(usbmisc->base) | val;
	else
		val = readl(usbmisc->base) & ~val;
	writel(val, usbmisc->base);
	spin_unlock_irqrestore(&usbmisc->lock, flags);

	return 0;
}

static int usbmisc_imx53_init(struct imx_usbmisc_data *data)
{
	struct imx_usbmisc *usbmisc = dev_get_drvdata(data->dev);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	void __iomem *reg = NULL;
	unsigned long flags;
	u32 val = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	usbdev = get_usbdev(dev);
	if (IS_ERR(usbdev))
		return PTR_ERR(usbdev);

	if (usbdev->disable_oc) {
		spin_lock_irqsave(&usbmisc->lock, flags);
		switch (usbdev->index) {
=======
=======
>>>>>>> v3.18
	if (data->index > 3)
		return -EINVAL;

	/* Select a 24 MHz reference clock for the PHY  */
	reg = usbmisc->base + MX53_USB_OTG_PHY_CTRL_1_OFFSET;
	val = readl(reg);
	val &= ~MX53_USB_PHYCTRL1_PLLDIV_MASK;
	val |= MX53_USB_PLL_DIV_24_MHZ;
	writel(val, usbmisc->base + MX53_USB_OTG_PHY_CTRL_1_OFFSET);

	if (data->disable_oc) {
		spin_lock_irqsave(&usbmisc->lock, flags);
		switch (data->index) {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		case 0:
			reg = usbmisc->base + MX53_USB_OTG_PHY_CTRL_0_OFFSET;
			val = readl(reg) | MX53_BM_OVER_CUR_DIS_OTG;
			break;
		case 1:
			reg = usbmisc->base + MX53_USB_OTG_PHY_CTRL_0_OFFSET;
			val = readl(reg) | MX53_BM_OVER_CUR_DIS_H1;
			break;
		case 2:
			reg = usbmisc->base + MX53_USB_UH2_CTRL_OFFSET;
			val = readl(reg) | MX53_BM_OVER_CUR_DIS_UHx;
			break;
		case 3:
			reg = usbmisc->base + MX53_USB_UH3_CTRL_OFFSET;
			val = readl(reg) | MX53_BM_OVER_CUR_DIS_UHx;
			break;
		}
		if (reg && val)
			writel(val, reg);
		spin_unlock_irqrestore(&usbmisc->lock, flags);
	}

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int usbmisc_imx6q_init(struct device *dev)
{

	struct usbmisc_usb_device *usbdev;
	unsigned long flags;
	u32 reg;

	usbdev = get_usbdev(dev);
	if (IS_ERR(usbdev))
		return PTR_ERR(usbdev);

	if (usbdev->disable_oc) {
		spin_lock_irqsave(&usbmisc->lock, flags);
		reg = readl(usbmisc->base + usbdev->index * 4);
		writel(reg | MX6_BM_OVER_CUR_DIS,
			usbmisc->base + usbdev->index * 4);
=======
=======
>>>>>>> v3.18
static int usbmisc_imx6q_init(struct imx_usbmisc_data *data)
{
	struct imx_usbmisc *usbmisc = dev_get_drvdata(data->dev);
	unsigned long flags;
	u32 reg;

	if (data->index > 3)
		return -EINVAL;

	if (data->disable_oc) {
		spin_lock_irqsave(&usbmisc->lock, flags);
		reg = readl(usbmisc->base + data->index * 4);
		writel(reg | MX6_BM_OVER_CUR_DIS,
			usbmisc->base + data->index * 4);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		spin_unlock_irqrestore(&usbmisc->lock, flags);
	}

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static const struct usbmisc_ops imx25_usbmisc_ops = {
	.post = usbmisc_imx25_post,
};

=======
=======
>>>>>>> v3.18
static int usbmisc_vf610_init(struct imx_usbmisc_data *data)
{
	struct imx_usbmisc *usbmisc = dev_get_drvdata(data->dev);
	u32 reg;

	/*
	 * Vybrid only has one misc register set, but in two different
	 * areas. These is reflected in two instances of this driver.
	 */
	if (data->index >= 1)
		return -EINVAL;

	if (data->disable_oc) {
		reg = readl(usbmisc->base);
		writel(reg | VF610_OVER_CUR_DIS, usbmisc->base);
	}

	return 0;
}

static const struct usbmisc_ops imx25_usbmisc_ops = {
	.init = usbmisc_imx25_init,
	.post = usbmisc_imx25_post,
};

static const struct usbmisc_ops imx27_usbmisc_ops = {
	.init = usbmisc_imx27_init,
};

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static const struct usbmisc_ops imx53_usbmisc_ops = {
	.init = usbmisc_imx53_init,
};

static const struct usbmisc_ops imx6q_usbmisc_ops = {
	.init = usbmisc_imx6q_init,
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
static const struct usbmisc_ops vf610_usbmisc_ops = {
	.init = usbmisc_vf610_init,
};

int imx_usbmisc_init(struct imx_usbmisc_data *data)
{
	struct imx_usbmisc *usbmisc = dev_get_drvdata(data->dev);

	if (!usbmisc->ops->init)
		return 0;
	return usbmisc->ops->init(data);
}
EXPORT_SYMBOL_GPL(imx_usbmisc_init);

int imx_usbmisc_init_post(struct imx_usbmisc_data *data)
{
	struct imx_usbmisc *usbmisc = dev_get_drvdata(data->dev);

	if (!usbmisc->ops->post)
		return 0;
	return usbmisc->ops->post(data);
}
EXPORT_SYMBOL_GPL(imx_usbmisc_init_post);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static const struct of_device_id usbmisc_imx_dt_ids[] = {
	{
		.compatible = "fsl,imx25-usbmisc",
		.data = &imx25_usbmisc_ops,
	},
	{
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
		.compatible = "fsl,imx35-usbmisc",
		.data = &imx25_usbmisc_ops,
	},
	{
		.compatible = "fsl,imx27-usbmisc",
		.data = &imx27_usbmisc_ops,
	},
	{
		.compatible = "fsl,imx51-usbmisc",
		.data = &imx53_usbmisc_ops,
	},
	{
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		.compatible = "fsl,imx53-usbmisc",
		.data = &imx53_usbmisc_ops,
	},
	{
		.compatible = "fsl,imx6q-usbmisc",
		.data = &imx6q_usbmisc_ops,
	},
<<<<<<< HEAD
<<<<<<< HEAD
	{ /* sentinel */ }
};
=======
=======
>>>>>>> v3.18
	{
		.compatible = "fsl,vf610-usbmisc",
		.data = &vf610_usbmisc_ops,
	},
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, usbmisc_imx_dt_ids);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

static int usbmisc_imx_probe(struct platform_device *pdev)
{
	struct resource	*res;
	struct imx_usbmisc *data;
	int ret;
	struct of_device_id *tmp_dev;

<<<<<<< HEAD
<<<<<<< HEAD
	if (usbmisc)
		return -EBUSY;

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	data = devm_kzalloc(&pdev->dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	spin_lock_init(&data->lock);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	data->base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(data->base))
		return PTR_ERR(data->base);

	data->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(data->clk)) {
		dev_err(&pdev->dev,
			"failed to get clock, err=%ld\n", PTR_ERR(data->clk));
		return PTR_ERR(data->clk);
	}

	ret = clk_prepare_enable(data->clk);
	if (ret) {
		dev_err(&pdev->dev,
			"clk_prepare_enable failed, err=%d\n", ret);
		return ret;
	}

	tmp_dev = (struct of_device_id *)
		of_match_device(usbmisc_imx_dt_ids, &pdev->dev);
	data->ops = (const struct usbmisc_ops *)tmp_dev->data;
<<<<<<< HEAD
<<<<<<< HEAD
	usbmisc = data;
	ret = usbmisc_set_ops(data->ops);
	if (ret) {
		usbmisc = NULL;
		clk_disable_unprepare(data->clk);
		return ret;
	}
=======
	platform_set_drvdata(pdev, data);
>>>>>>> v3.18
=======
	platform_set_drvdata(pdev, data);
>>>>>>> v3.18

	return 0;
}

static int usbmisc_imx_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	usbmisc_unset_ops(usbmisc->ops);
	clk_disable_unprepare(usbmisc->clk);
	usbmisc = NULL;
=======
	struct imx_usbmisc *usbmisc = dev_get_drvdata(&pdev->dev);
	clk_disable_unprepare(usbmisc->clk);
>>>>>>> v3.18
=======
	struct imx_usbmisc *usbmisc = dev_get_drvdata(&pdev->dev);
	clk_disable_unprepare(usbmisc->clk);
>>>>>>> v3.18
	return 0;
}

static struct platform_driver usbmisc_imx_driver = {
	.probe = usbmisc_imx_probe,
	.remove = usbmisc_imx_remove,
	.driver = {
		.name = "usbmisc_imx",
		.owner = THIS_MODULE,
		.of_match_table = usbmisc_imx_dt_ids,
	 },
};

<<<<<<< HEAD
<<<<<<< HEAD
int usbmisc_imx_drv_init(void)
{
	return platform_driver_register(&usbmisc_imx_driver);
}
subsys_initcall(usbmisc_imx_drv_init);

void usbmisc_imx_drv_exit(void)
{
	platform_driver_unregister(&usbmisc_imx_driver);
}
module_exit(usbmisc_imx_drv_exit);
=======
module_platform_driver(usbmisc_imx_driver);
>>>>>>> v3.18
=======
module_platform_driver(usbmisc_imx_driver);
>>>>>>> v3.18

MODULE_ALIAS("platform:usbmisc-imx");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("driver for imx usb non-core registers");
MODULE_AUTHOR("Richard Zhao <richard.zhao@freescale.com>");
