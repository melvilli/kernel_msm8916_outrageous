/*
 * SDHCI support for SiRF primaII and marco SoCs
 *
 * Copyright (c) 2011 Cambridge Silicon Radio Limited, a CSR plc group company.
 *
 * Licensed under GPLv2 or later.
 */

#include <linux/delay.h>
#include <linux/device.h>
#include <linux/mmc/host.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/mmc/slot-gpio.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/pinctrl/consumer.h>
#include "sdhci-pltfm.h"

=======
=======
>>>>>>> v3.18
#include "sdhci-pltfm.h"

#define SDHCI_SIRF_8BITBUS BIT(3)

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
struct sdhci_sirf_priv {
	struct clk *clk;
	int gpio_cd;
};

static unsigned int sdhci_sirf_get_max_clk(struct sdhci_host *host)
{
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
<<<<<<< HEAD
<<<<<<< HEAD
	struct sdhci_sirf_priv *priv = pltfm_host->priv;
	return clk_get_rate(priv->clk);
}

static struct sdhci_ops sdhci_sirf_ops = {
	.get_max_clock	= sdhci_sirf_get_max_clk,
=======
=======
>>>>>>> v3.18
	struct sdhci_sirf_priv *priv = sdhci_pltfm_priv(pltfm_host);
	return clk_get_rate(priv->clk);
}

static void sdhci_sirf_set_bus_width(struct sdhci_host *host, int width)
{
	u8 ctrl;

	ctrl = sdhci_readb(host, SDHCI_HOST_CONTROL);
	ctrl &= ~(SDHCI_CTRL_4BITBUS | SDHCI_SIRF_8BITBUS);

	/*
	 * CSR atlas7 and prima2 SD host version is not 3.0
	 * 8bit-width enable bit of CSR SD hosts is 3,
	 * while stardard hosts use bit 5
	 */
	if (width == MMC_BUS_WIDTH_8)
		ctrl |= SDHCI_SIRF_8BITBUS;
	else if (width == MMC_BUS_WIDTH_4)
		ctrl |= SDHCI_CTRL_4BITBUS;

	sdhci_writeb(host, ctrl, SDHCI_HOST_CONTROL);
}

static struct sdhci_ops sdhci_sirf_ops = {
	.set_clock = sdhci_set_clock,
	.get_max_clock	= sdhci_sirf_get_max_clk,
	.set_bus_width = sdhci_sirf_set_bus_width,
	.reset = sdhci_reset,
	.set_uhs_signaling = sdhci_set_uhs_signaling,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
};

static struct sdhci_pltfm_data sdhci_sirf_pdata = {
	.ops = &sdhci_sirf_ops,
	.quirks = SDHCI_QUIRK_BROKEN_TIMEOUT_VAL |
		SDHCI_QUIRK_DATA_TIMEOUT_USES_SDCLK |
		SDHCI_QUIRK_CAP_CLOCK_BASE_BROKEN |
		SDHCI_QUIRK_INVERTED_WRITE_PROTECT |
		SDHCI_QUIRK_DELAY_AFTER_POWER,
};

static int sdhci_sirf_probe(struct platform_device *pdev)
{
	struct sdhci_host *host;
	struct sdhci_pltfm_host *pltfm_host;
	struct sdhci_sirf_priv *priv;
<<<<<<< HEAD
<<<<<<< HEAD
	struct pinctrl *pinctrl;
	int ret;

	pinctrl = devm_pinctrl_get_select_default(&pdev->dev);
	if (IS_ERR(pinctrl)) {
		dev_err(&pdev->dev, "unable to get pinmux");
		return PTR_ERR(pinctrl);
	}

	priv = devm_kzalloc(&pdev->dev, sizeof(struct sdhci_sirf_priv),
		GFP_KERNEL);
	if (!priv) {
		dev_err(&pdev->dev, "unable to allocate private data");
		return -ENOMEM;
	}

	priv->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(priv->clk)) {
		dev_err(&pdev->dev, "unable to get clock");
		return PTR_ERR(priv->clk);
	}

	if (pdev->dev.of_node) {
		priv->gpio_cd = of_get_named_gpio(pdev->dev.of_node,
			"cd-gpios", 0);
	} else {
		priv->gpio_cd = -EINVAL;
	}

	host = sdhci_pltfm_init(pdev, &sdhci_sirf_pdata);
	if (IS_ERR(host)) {
		ret = PTR_ERR(host);
		goto err_sdhci_pltfm_init;
	}

	pltfm_host = sdhci_priv(host);
	pltfm_host->priv = priv;

	sdhci_get_of_property(pdev);

	clk_prepare_enable(priv->clk);
=======
=======
>>>>>>> v3.18
	struct clk *clk;
	int gpio_cd;
	int ret;

	clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(clk)) {
		dev_err(&pdev->dev, "unable to get clock");
		return PTR_ERR(clk);
	}

	if (pdev->dev.of_node)
		gpio_cd = of_get_named_gpio(pdev->dev.of_node, "cd-gpios", 0);
	else
		gpio_cd = -EINVAL;

	host = sdhci_pltfm_init(pdev, &sdhci_sirf_pdata, sizeof(struct sdhci_sirf_priv));
	if (IS_ERR(host))
		return PTR_ERR(host);

	pltfm_host = sdhci_priv(host);
	priv = sdhci_pltfm_priv(pltfm_host);
	priv->clk = clk;
	priv->gpio_cd = gpio_cd;

	sdhci_get_of_property(pdev);

	ret = clk_prepare_enable(priv->clk);
	if (ret)
		goto err_clk_prepare;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	ret = sdhci_add_host(host);
	if (ret)
		goto err_sdhci_add;

	/*
	 * We must request the IRQ after sdhci_add_host(), as the tasklet only
	 * gets setup in sdhci_add_host() and we oops.
	 */
	if (gpio_is_valid(priv->gpio_cd)) {
<<<<<<< HEAD
<<<<<<< HEAD
		ret = mmc_gpio_request_cd(host->mmc, priv->gpio_cd);
=======
		ret = mmc_gpio_request_cd(host->mmc, priv->gpio_cd, 0);
>>>>>>> v3.18
=======
		ret = mmc_gpio_request_cd(host->mmc, priv->gpio_cd, 0);
>>>>>>> v3.18
		if (ret) {
			dev_err(&pdev->dev, "card detect irq request failed: %d\n",
				ret);
			goto err_request_cd;
		}
<<<<<<< HEAD
<<<<<<< HEAD
=======
		mmc_gpiod_request_cd_irq(host->mmc);
>>>>>>> v3.18
=======
		mmc_gpiod_request_cd_irq(host->mmc);
>>>>>>> v3.18
	}

	return 0;

err_request_cd:
	sdhci_remove_host(host, 0);
err_sdhci_add:
	clk_disable_unprepare(priv->clk);
<<<<<<< HEAD
<<<<<<< HEAD
	sdhci_pltfm_free(pdev);
err_sdhci_pltfm_init:
=======
err_clk_prepare:
	sdhci_pltfm_free(pdev);
>>>>>>> v3.18
=======
err_clk_prepare:
	sdhci_pltfm_free(pdev);
>>>>>>> v3.18
	return ret;
}

static int sdhci_sirf_remove(struct platform_device *pdev)
{
	struct sdhci_host *host = platform_get_drvdata(pdev);
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
<<<<<<< HEAD
<<<<<<< HEAD
	struct sdhci_sirf_priv *priv = pltfm_host->priv;
=======
	struct sdhci_sirf_priv *priv = sdhci_pltfm_priv(pltfm_host);
>>>>>>> v3.18
=======
	struct sdhci_sirf_priv *priv = sdhci_pltfm_priv(pltfm_host);
>>>>>>> v3.18

	sdhci_pltfm_unregister(pdev);

	if (gpio_is_valid(priv->gpio_cd))
		mmc_gpio_free_cd(host->mmc);

	clk_disable_unprepare(priv->clk);
	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int sdhci_sirf_suspend(struct device *dev)
{
	struct sdhci_host *host = dev_get_drvdata(dev);
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
<<<<<<< HEAD
<<<<<<< HEAD
	struct sdhci_sirf_priv *priv = pltfm_host->priv;
=======
	struct sdhci_sirf_priv *priv = sdhci_pltfm_priv(pltfm_host);
>>>>>>> v3.18
=======
	struct sdhci_sirf_priv *priv = sdhci_pltfm_priv(pltfm_host);
>>>>>>> v3.18
	int ret;

	ret = sdhci_suspend_host(host);
	if (ret)
		return ret;

	clk_disable(priv->clk);

	return 0;
}

static int sdhci_sirf_resume(struct device *dev)
{
	struct sdhci_host *host = dev_get_drvdata(dev);
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
<<<<<<< HEAD
<<<<<<< HEAD
	struct sdhci_sirf_priv *priv = pltfm_host->priv;
=======
	struct sdhci_sirf_priv *priv = sdhci_pltfm_priv(pltfm_host);
>>>>>>> v3.18
=======
	struct sdhci_sirf_priv *priv = sdhci_pltfm_priv(pltfm_host);
>>>>>>> v3.18
	int ret;

	ret = clk_enable(priv->clk);
	if (ret) {
		dev_dbg(dev, "Resume: Error enabling clock\n");
		return ret;
	}

	return sdhci_resume_host(host);
}

static SIMPLE_DEV_PM_OPS(sdhci_sirf_pm_ops, sdhci_sirf_suspend, sdhci_sirf_resume);
#endif

static const struct of_device_id sdhci_sirf_of_match[] = {
	{ .compatible = "sirf,prima2-sdhc" },
	{ }
};
MODULE_DEVICE_TABLE(of, sdhci_sirf_of_match);

static struct platform_driver sdhci_sirf_driver = {
	.driver		= {
		.name	= "sdhci-sirf",
<<<<<<< HEAD
<<<<<<< HEAD
		.owner	= THIS_MODULE,
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
		.of_match_table = sdhci_sirf_of_match,
#ifdef CONFIG_PM_SLEEP
		.pm	= &sdhci_sirf_pm_ops,
#endif
	},
	.probe		= sdhci_sirf_probe,
	.remove		= sdhci_sirf_remove,
};

module_platform_driver(sdhci_sirf_driver);

MODULE_DESCRIPTION("SDHCI driver for SiRFprimaII/SiRFmarco");
MODULE_AUTHOR("Barry Song <21cnbao@gmail.com>");
MODULE_LICENSE("GPL v2");
