/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd.
 * Authors:
 *	Inki Dae <inki.dae@samsung.com>
 *	Joonyoung Shim <jy0922.shim@samsung.com>
 *	Seung-Woo Kim <sw0312.kim@samsung.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include <drm/drmP.h>
#include <drm/drm_crtc_helper.h>

=======
=======
>>>>>>> v3.18
#include <linux/pm_runtime.h>
#include <drm/drmP.h>
#include <drm/drm_crtc_helper.h>

#include <linux/component.h>

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#include <drm/exynos_drm.h>

#include "exynos_drm_drv.h"
#include "exynos_drm_crtc.h"
#include "exynos_drm_encoder.h"
#include "exynos_drm_fbdev.h"
#include "exynos_drm_fb.h"
#include "exynos_drm_gem.h"
#include "exynos_drm_plane.h"
#include "exynos_drm_vidi.h"
#include "exynos_drm_dmabuf.h"
#include "exynos_drm_g2d.h"
#include "exynos_drm_ipp.h"
#include "exynos_drm_iommu.h"

#define DRIVER_NAME	"exynos"
#define DRIVER_DESC	"Samsung SoC DRM"
#define DRIVER_DATE	"20110530"
#define DRIVER_MAJOR	1
#define DRIVER_MINOR	0

<<<<<<< HEAD
<<<<<<< HEAD
#define VBLANK_OFF_DELAY	50000

/* platform device pointer for eynos drm device. */
static struct platform_device *exynos_drm_pdev;

=======
=======
>>>>>>> v3.18
static struct platform_device *exynos_drm_pdev;

static DEFINE_MUTEX(drm_component_lock);
static LIST_HEAD(drm_component_list);

struct component_dev {
	struct list_head list;
	struct device *crtc_dev;
	struct device *conn_dev;
	enum exynos_drm_output_type out_type;
	unsigned int dev_type_flag;
};

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
static int exynos_drm_load(struct drm_device *dev, unsigned long flags)
{
	struct exynos_drm_private *private;
	int ret;
	int nr;

<<<<<<< HEAD
<<<<<<< HEAD
	DRM_DEBUG_DRIVER("%s\n", __FILE__);

	private = kzalloc(sizeof(struct exynos_drm_private), GFP_KERNEL);
	if (!private) {
		DRM_ERROR("failed to allocate private\n");
		return -ENOMEM;
	}

	INIT_LIST_HEAD(&private->pageflip_event_list);
=======
=======
>>>>>>> v3.18
	private = kzalloc(sizeof(struct exynos_drm_private), GFP_KERNEL);
	if (!private)
		return -ENOMEM;

	INIT_LIST_HEAD(&private->pageflip_event_list);
	dev_set_drvdata(dev->dev, dev);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	dev->dev_private = (void *)private;

	/*
	 * create mapping to manage iommu table and set a pointer to iommu
	 * mapping structure to iommu_mapping of private data.
	 * also this iommu_mapping can be used to check if iommu is supported
	 * or not.
	 */
	ret = drm_create_iommu_mapping(dev);
	if (ret < 0) {
		DRM_ERROR("failed to create iommu mapping.\n");
<<<<<<< HEAD
<<<<<<< HEAD
		goto err_crtc;
=======
		goto err_free_private;
>>>>>>> v3.18
=======
		goto err_free_private;
>>>>>>> v3.18
	}

	drm_mode_config_init(dev);

<<<<<<< HEAD
<<<<<<< HEAD
	/* init kms poll for handling hpd */
	drm_kms_helper_poll_init(dev);

	exynos_drm_mode_config_init(dev);

	/*
	 * EXYNOS4 is enough to have two CRTCs and each crtc would be used
	 * without dependency of hardware.
	 */
	for (nr = 0; nr < MAX_CRTC; nr++) {
		ret = exynos_drm_crtc_create(dev, nr);
		if (ret)
			goto err_release_iommu_mapping;
	}

	for (nr = 0; nr < MAX_PLANE; nr++) {
		struct drm_plane *plane;
		unsigned int possible_crtcs = (1 << MAX_CRTC) - 1;

		plane = exynos_plane_init(dev, possible_crtcs, false);
		if (!plane)
			goto err_release_iommu_mapping;
	}

	ret = drm_vblank_init(dev, MAX_CRTC);
	if (ret)
		goto err_release_iommu_mapping;

	/*
	 * probe sub drivers such as display controller and hdmi driver,
	 * that were registered at probe() of platform driver
	 * to the sub driver and create encoder and connector for them.
	 */
	ret = exynos_drm_device_register(dev);
	if (ret)
		goto err_vblank;

	/* setup possible_clones. */
	exynos_drm_encoder_setup(dev);

	/*
	 * create and configure fb helper and also exynos specific
	 * fbdev object.
	 */
	ret = exynos_drm_fbdev_init(dev);
	if (ret) {
		DRM_ERROR("failed to initialize drm fbdev\n");
		goto err_drm_device;
	}

	drm_vblank_offdelay = VBLANK_OFF_DELAY;

	return 0;

err_drm_device:
	exynos_drm_device_unregister(dev);
err_vblank:
	drm_vblank_cleanup(dev);
err_release_iommu_mapping:
	drm_release_iommu_mapping(dev);
err_crtc:
	drm_mode_config_cleanup(dev);
=======
=======
>>>>>>> v3.18
	exynos_drm_mode_config_init(dev);

	for (nr = 0; nr < MAX_PLANE; nr++) {
		struct drm_plane *plane;
		unsigned long possible_crtcs = (1 << MAX_CRTC) - 1;

		plane = exynos_plane_init(dev, possible_crtcs,
					  DRM_PLANE_TYPE_OVERLAY);
		if (!IS_ERR(plane))
			continue;

		ret = PTR_ERR(plane);
		goto err_mode_config_cleanup;
	}

	/* setup possible_clones. */
	exynos_drm_encoder_setup(dev);

	platform_set_drvdata(dev->platformdev, dev);

	/* Try to bind all sub drivers. */
	ret = component_bind_all(dev->dev, dev);
	if (ret)
		goto err_mode_config_cleanup;

	ret = drm_vblank_init(dev, dev->mode_config.num_crtc);
	if (ret)
		goto err_unbind_all;

	/* Probe non kms sub drivers and virtual display driver. */
	ret = exynos_drm_device_subdrv_probe(dev);
	if (ret)
		goto err_cleanup_vblank;

	/*
	 * enable drm irq mode.
	 * - with irq_enabled = true, we can use the vblank feature.
	 *
	 * P.S. note that we wouldn't use drm irq handler but
	 *	just specific driver own one instead because
	 *	drm framework supports only one irq handler.
	 */
	dev->irq_enabled = true;

	/*
	 * with vblank_disable_allowed = true, vblank interrupt will be disabled
	 * by drm timer once a current process gives up ownership of
	 * vblank event.(after drm_vblank_put function is called)
	 */
	dev->vblank_disable_allowed = true;

	/* init kms poll for handling hpd */
	drm_kms_helper_poll_init(dev);

	/* force connectors detection */
	drm_helper_hpd_irq_event(dev);

	return 0;

err_cleanup_vblank:
	drm_vblank_cleanup(dev);
err_unbind_all:
	component_unbind_all(dev->dev, dev);
err_mode_config_cleanup:
	drm_mode_config_cleanup(dev);
	drm_release_iommu_mapping(dev);
err_free_private:
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	kfree(private);

	return ret;
}

static int exynos_drm_unload(struct drm_device *dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	DRM_DEBUG_DRIVER("%s\n", __FILE__);

	exynos_drm_fbdev_fini(dev);
	exynos_drm_device_unregister(dev);
	drm_vblank_cleanup(dev);
	drm_kms_helper_poll_fini(dev);
	drm_mode_config_cleanup(dev);

	drm_release_iommu_mapping(dev);
	kfree(dev->dev_private);

=======
=======
>>>>>>> v3.18
	exynos_drm_device_subdrv_remove(dev);

	exynos_drm_fbdev_fini(dev);
	drm_kms_helper_poll_fini(dev);

	drm_vblank_cleanup(dev);
	component_unbind_all(dev->dev, dev);
	drm_mode_config_cleanup(dev);
	drm_release_iommu_mapping(dev);

	kfree(dev->dev_private);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	dev->dev_private = NULL;

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int exynos_drm_open(struct drm_device *dev, struct drm_file *file)
{
	struct drm_exynos_file_private *file_priv;

	DRM_DEBUG_DRIVER("%s\n", __FILE__);
=======
=======
>>>>>>> v3.18
static int exynos_drm_suspend(struct drm_device *dev, pm_message_t state)
{
	struct drm_connector *connector;

	drm_modeset_lock_all(dev);
	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		int old_dpms = connector->dpms;

		if (connector->funcs->dpms)
			connector->funcs->dpms(connector, DRM_MODE_DPMS_OFF);

		/* Set the old mode back to the connector for resume */
		connector->dpms = old_dpms;
	}
	drm_modeset_unlock_all(dev);

	return 0;
}

static int exynos_drm_resume(struct drm_device *dev)
{
	struct drm_connector *connector;

	drm_modeset_lock_all(dev);
	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		if (connector->funcs->dpms) {
			int dpms = connector->dpms;

			connector->dpms = DRM_MODE_DPMS_OFF;
			connector->funcs->dpms(connector, dpms);
		}
	}
	drm_modeset_unlock_all(dev);

	drm_helper_resume_force_mode(dev);

	return 0;
}

static int exynos_drm_open(struct drm_device *dev, struct drm_file *file)
{
	struct drm_exynos_file_private *file_priv;
	int ret;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	file_priv = kzalloc(sizeof(*file_priv), GFP_KERNEL);
	if (!file_priv)
		return -ENOMEM;

	file->driver_priv = file_priv;

<<<<<<< HEAD
<<<<<<< HEAD
	return exynos_drm_subdrv_open(dev, file);
=======
=======
>>>>>>> v3.18
	ret = exynos_drm_subdrv_open(dev, file);
	if (ret)
		goto err_file_priv_free;

	return ret;

err_file_priv_free:
	kfree(file_priv);
	file->driver_priv = NULL;
	return ret;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

static void exynos_drm_preclose(struct drm_device *dev,
					struct drm_file *file)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct exynos_drm_private *private = dev->dev_private;
	struct drm_pending_vblank_event *e, *t;
	unsigned long flags;

	DRM_DEBUG_DRIVER("%s\n", __FILE__);

	/* release events of current file */
	spin_lock_irqsave(&dev->event_lock, flags);
	list_for_each_entry_safe(e, t, &private->pageflip_event_list,
			base.link) {
		if (e->base.file_priv == file) {
			list_del(&e->base.link);
			e->base.destroy(&e->base);
		}
	}
	spin_unlock_irqrestore(&dev->event_lock, flags);

	exynos_drm_subdrv_close(dev, file);
}

static void exynos_drm_postclose(struct drm_device *dev, struct drm_file *file)
{
	DRM_DEBUG_DRIVER("%s\n", __FILE__);

	if (!file->driver_priv)
		return;
=======
=======
>>>>>>> v3.18
	exynos_drm_subdrv_close(dev, file);
}

static void exynos_drm_postclose(struct drm_device *dev, struct drm_file *file)
{
	struct exynos_drm_private *private = dev->dev_private;
	struct drm_pending_vblank_event *v, *vt;
	struct drm_pending_event *e, *et;
	unsigned long flags;

	if (!file->driver_priv)
		return;

	/* Release all events not unhandled by page flip handler. */
	spin_lock_irqsave(&dev->event_lock, flags);
	list_for_each_entry_safe(v, vt, &private->pageflip_event_list,
			base.link) {
		if (v->base.file_priv == file) {
			list_del(&v->base.link);
			drm_vblank_put(dev, v->pipe);
			v->base.destroy(&v->base);
		}
	}

	/* Release all events handled by page flip handler but not freed. */
	list_for_each_entry_safe(e, et, &file->event_list, link) {
		list_del(&e->link);
		e->destroy(e);
	}
	spin_unlock_irqrestore(&dev->event_lock, flags);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	kfree(file->driver_priv);
	file->driver_priv = NULL;
}

static void exynos_drm_lastclose(struct drm_device *dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	DRM_DEBUG_DRIVER("%s\n", __FILE__);

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	exynos_drm_fbdev_restore_mode(dev);
}

static const struct vm_operations_struct exynos_drm_gem_vm_ops = {
	.fault = exynos_drm_gem_fault,
	.open = drm_gem_vm_open,
	.close = drm_gem_vm_close,
};

<<<<<<< HEAD
<<<<<<< HEAD
static struct drm_ioctl_desc exynos_ioctls[] = {
	DRM_IOCTL_DEF_DRV(EXYNOS_GEM_CREATE, exynos_drm_gem_create_ioctl,
			DRM_UNLOCKED | DRM_AUTH),
	DRM_IOCTL_DEF_DRV(EXYNOS_GEM_MAP_OFFSET,
			exynos_drm_gem_map_offset_ioctl, DRM_UNLOCKED |
			DRM_AUTH),
	DRM_IOCTL_DEF_DRV(EXYNOS_GEM_MMAP,
			exynos_drm_gem_mmap_ioctl, DRM_UNLOCKED | DRM_AUTH),
=======
static const struct drm_ioctl_desc exynos_ioctls[] = {
	DRM_IOCTL_DEF_DRV(EXYNOS_GEM_CREATE, exynos_drm_gem_create_ioctl,
			DRM_UNLOCKED | DRM_AUTH),
>>>>>>> v3.18
=======
static const struct drm_ioctl_desc exynos_ioctls[] = {
	DRM_IOCTL_DEF_DRV(EXYNOS_GEM_CREATE, exynos_drm_gem_create_ioctl,
			DRM_UNLOCKED | DRM_AUTH),
>>>>>>> v3.18
	DRM_IOCTL_DEF_DRV(EXYNOS_GEM_GET,
			exynos_drm_gem_get_ioctl, DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(EXYNOS_VIDI_CONNECTION,
			vidi_connection_ioctl, DRM_UNLOCKED | DRM_AUTH),
	DRM_IOCTL_DEF_DRV(EXYNOS_G2D_GET_VER,
			exynos_g2d_get_ver_ioctl, DRM_UNLOCKED | DRM_AUTH),
	DRM_IOCTL_DEF_DRV(EXYNOS_G2D_SET_CMDLIST,
			exynos_g2d_set_cmdlist_ioctl, DRM_UNLOCKED | DRM_AUTH),
	DRM_IOCTL_DEF_DRV(EXYNOS_G2D_EXEC,
			exynos_g2d_exec_ioctl, DRM_UNLOCKED | DRM_AUTH),
	DRM_IOCTL_DEF_DRV(EXYNOS_IPP_GET_PROPERTY,
			exynos_drm_ipp_get_property, DRM_UNLOCKED | DRM_AUTH),
	DRM_IOCTL_DEF_DRV(EXYNOS_IPP_SET_PROPERTY,
			exynos_drm_ipp_set_property, DRM_UNLOCKED | DRM_AUTH),
	DRM_IOCTL_DEF_DRV(EXYNOS_IPP_QUEUE_BUF,
			exynos_drm_ipp_queue_buf, DRM_UNLOCKED | DRM_AUTH),
	DRM_IOCTL_DEF_DRV(EXYNOS_IPP_CMD_CTRL,
			exynos_drm_ipp_cmd_ctrl, DRM_UNLOCKED | DRM_AUTH),
};

static const struct file_operations exynos_drm_driver_fops = {
	.owner		= THIS_MODULE,
	.open		= drm_open,
	.mmap		= exynos_drm_gem_mmap,
	.poll		= drm_poll,
	.read		= drm_read,
	.unlocked_ioctl	= drm_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = drm_compat_ioctl,
#endif
	.release	= drm_release,
};

static struct drm_driver exynos_drm_driver = {
<<<<<<< HEAD
<<<<<<< HEAD
	.driver_features	= DRIVER_HAVE_IRQ | DRIVER_MODESET |
					DRIVER_GEM | DRIVER_PRIME,
	.load			= exynos_drm_load,
	.unload			= exynos_drm_unload,
=======
=======
>>>>>>> v3.18
	.driver_features	= DRIVER_MODESET | DRIVER_GEM | DRIVER_PRIME,
	.load			= exynos_drm_load,
	.unload			= exynos_drm_unload,
	.suspend		= exynos_drm_suspend,
	.resume			= exynos_drm_resume,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	.open			= exynos_drm_open,
	.preclose		= exynos_drm_preclose,
	.lastclose		= exynos_drm_lastclose,
	.postclose		= exynos_drm_postclose,
<<<<<<< HEAD
<<<<<<< HEAD
	.get_vblank_counter	= drm_vblank_count,
	.enable_vblank		= exynos_drm_crtc_enable_vblank,
	.disable_vblank		= exynos_drm_crtc_disable_vblank,
	.gem_init_object	= exynos_drm_gem_init_object,
=======
=======
>>>>>>> v3.18
	.set_busid		= drm_platform_set_busid,
	.get_vblank_counter	= drm_vblank_count,
	.enable_vblank		= exynos_drm_crtc_enable_vblank,
	.disable_vblank		= exynos_drm_crtc_disable_vblank,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	.gem_free_object	= exynos_drm_gem_free_object,
	.gem_vm_ops		= &exynos_drm_gem_vm_ops,
	.dumb_create		= exynos_drm_gem_dumb_create,
	.dumb_map_offset	= exynos_drm_gem_dumb_map_offset,
<<<<<<< HEAD
<<<<<<< HEAD
	.dumb_destroy		= exynos_drm_gem_dumb_destroy,
=======
	.dumb_destroy		= drm_gem_dumb_destroy,
>>>>>>> v3.18
=======
	.dumb_destroy		= drm_gem_dumb_destroy,
>>>>>>> v3.18
	.prime_handle_to_fd	= drm_gem_prime_handle_to_fd,
	.prime_fd_to_handle	= drm_gem_prime_fd_to_handle,
	.gem_prime_export	= exynos_dmabuf_prime_export,
	.gem_prime_import	= exynos_dmabuf_prime_import,
	.ioctls			= exynos_ioctls,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	.num_ioctls		= ARRAY_SIZE(exynos_ioctls),
>>>>>>> v3.18
=======
	.num_ioctls		= ARRAY_SIZE(exynos_ioctls),
>>>>>>> v3.18
	.fops			= &exynos_drm_driver_fops,
	.name	= DRIVER_NAME,
	.desc	= DRIVER_DESC,
	.date	= DRIVER_DATE,
	.major	= DRIVER_MAJOR,
	.minor	= DRIVER_MINOR,
};

<<<<<<< HEAD
<<<<<<< HEAD
static int exynos_drm_platform_probe(struct platform_device *pdev)
{
	DRM_DEBUG_DRIVER("%s\n", __FILE__);

	pdev->dev.coherent_dma_mask = DMA_BIT_MASK(32);
	exynos_drm_driver.num_ioctls = DRM_ARRAY_SIZE(exynos_ioctls);

	return drm_platform_init(&exynos_drm_driver, pdev);
}

static int exynos_drm_platform_remove(struct platform_device *pdev)
{
	DRM_DEBUG_DRIVER("%s\n", __FILE__);

	drm_platform_exit(&exynos_drm_driver, pdev);
=======
=======
>>>>>>> v3.18
#ifdef CONFIG_PM_SLEEP
static int exynos_drm_sys_suspend(struct device *dev)
{
	struct drm_device *drm_dev = dev_get_drvdata(dev);
	pm_message_t message;

	if (pm_runtime_suspended(dev) || !drm_dev)
		return 0;

	message.event = PM_EVENT_SUSPEND;
	return exynos_drm_suspend(drm_dev, message);
}

static int exynos_drm_sys_resume(struct device *dev)
{
	struct drm_device *drm_dev = dev_get_drvdata(dev);

	if (pm_runtime_suspended(dev) || !drm_dev)
		return 0;

	return exynos_drm_resume(drm_dev);
}
#endif

static const struct dev_pm_ops exynos_drm_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(exynos_drm_sys_suspend, exynos_drm_sys_resume)
};

int exynos_drm_component_add(struct device *dev,
				enum exynos_drm_device_type dev_type,
				enum exynos_drm_output_type out_type)
{
	struct component_dev *cdev;

	if (dev_type != EXYNOS_DEVICE_TYPE_CRTC &&
			dev_type != EXYNOS_DEVICE_TYPE_CONNECTOR) {
		DRM_ERROR("invalid device type.\n");
		return -EINVAL;
	}

	mutex_lock(&drm_component_lock);

	/*
	 * Make sure to check if there is a component which has two device
	 * objects, for connector and for encoder/connector.
	 * It should make sure that crtc and encoder/connector drivers are
	 * ready before exynos drm core binds them.
	 */
	list_for_each_entry(cdev, &drm_component_list, list) {
		if (cdev->out_type == out_type) {
			/*
			 * If crtc and encoder/connector device objects are
			 * added already just return.
			 */
			if (cdev->dev_type_flag == (EXYNOS_DEVICE_TYPE_CRTC |
						EXYNOS_DEVICE_TYPE_CONNECTOR)) {
				mutex_unlock(&drm_component_lock);
				return 0;
			}

			if (dev_type == EXYNOS_DEVICE_TYPE_CRTC) {
				cdev->crtc_dev = dev;
				cdev->dev_type_flag |= dev_type;
			}

			if (dev_type == EXYNOS_DEVICE_TYPE_CONNECTOR) {
				cdev->conn_dev = dev;
				cdev->dev_type_flag |= dev_type;
			}

			mutex_unlock(&drm_component_lock);
			return 0;
		}
	}

	mutex_unlock(&drm_component_lock);

	cdev = kzalloc(sizeof(*cdev), GFP_KERNEL);
	if (!cdev)
		return -ENOMEM;

	if (dev_type == EXYNOS_DEVICE_TYPE_CRTC)
		cdev->crtc_dev = dev;
	if (dev_type == EXYNOS_DEVICE_TYPE_CONNECTOR)
		cdev->conn_dev = dev;

	cdev->out_type = out_type;
	cdev->dev_type_flag = dev_type;

	mutex_lock(&drm_component_lock);
	list_add_tail(&cdev->list, &drm_component_list);
	mutex_unlock(&drm_component_lock);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct platform_driver exynos_drm_platform_driver = {
	.probe		= exynos_drm_platform_probe,
	.remove		= exynos_drm_platform_remove,
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= "exynos-drm",
	},
};

static int __init exynos_drm_init(void)
{
	int ret;

	DRM_DEBUG_DRIVER("%s\n", __FILE__);
=======
=======
>>>>>>> v3.18
void exynos_drm_component_del(struct device *dev,
				enum exynos_drm_device_type dev_type)
{
	struct component_dev *cdev, *next;

	mutex_lock(&drm_component_lock);

	list_for_each_entry_safe(cdev, next, &drm_component_list, list) {
		if (dev_type == EXYNOS_DEVICE_TYPE_CRTC) {
			if (cdev->crtc_dev == dev) {
				cdev->crtc_dev = NULL;
				cdev->dev_type_flag &= ~dev_type;
			}
		}

		if (dev_type == EXYNOS_DEVICE_TYPE_CONNECTOR) {
			if (cdev->conn_dev == dev) {
				cdev->conn_dev = NULL;
				cdev->dev_type_flag &= ~dev_type;
			}
		}

		/*
		 * Release cdev object only in case that both of crtc and
		 * encoder/connector device objects are NULL.
		 */
		if (!cdev->crtc_dev && !cdev->conn_dev) {
			list_del(&cdev->list);
			kfree(cdev);
		}

		break;
	}

	mutex_unlock(&drm_component_lock);
}

static int compare_dev(struct device *dev, void *data)
{
	return dev == (struct device *)data;
}

static struct component_match *exynos_drm_match_add(struct device *dev)
{
	struct component_match *match = NULL;
	struct component_dev *cdev;
	unsigned int attach_cnt = 0;

	mutex_lock(&drm_component_lock);

	/* Do not retry to probe if there is no any kms driver regitered. */
	if (list_empty(&drm_component_list)) {
		mutex_unlock(&drm_component_lock);
		return ERR_PTR(-ENODEV);
	}

	list_for_each_entry(cdev, &drm_component_list, list) {
		/*
		 * Add components to master only in case that crtc and
		 * encoder/connector device objects exist.
		 */
		if (!cdev->crtc_dev || !cdev->conn_dev)
			continue;

		attach_cnt++;

		mutex_unlock(&drm_component_lock);

		/*
		 * fimd and dpi modules have same device object so add
		 * only crtc device object in this case.
		 */
		if (cdev->crtc_dev == cdev->conn_dev) {
			component_match_add(dev, &match, compare_dev,
						cdev->crtc_dev);
			goto out_lock;
		}

		/*
		 * Do not chage below call order.
		 * crtc device first should be added to master because
		 * connector/encoder need pipe number of crtc when they
		 * are created.
		 */
		component_match_add(dev, &match, compare_dev, cdev->crtc_dev);
		component_match_add(dev, &match, compare_dev, cdev->conn_dev);

out_lock:
		mutex_lock(&drm_component_lock);
	}

	mutex_unlock(&drm_component_lock);

	return attach_cnt ? match : ERR_PTR(-EPROBE_DEFER);
}

static int exynos_drm_bind(struct device *dev)
{
	return drm_platform_init(&exynos_drm_driver, to_platform_device(dev));
}

static void exynos_drm_unbind(struct device *dev)
{
	drm_put_dev(dev_get_drvdata(dev));
}

static const struct component_master_ops exynos_drm_ops = {
	.bind		= exynos_drm_bind,
	.unbind		= exynos_drm_unbind,
};

static int exynos_drm_platform_probe(struct platform_device *pdev)
{
	struct component_match *match;
	int ret;

	pdev->dev.coherent_dma_mask = DMA_BIT_MASK(32);
	exynos_drm_driver.num_ioctls = ARRAY_SIZE(exynos_ioctls);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

#ifdef CONFIG_DRM_EXYNOS_FIMD
	ret = platform_driver_register(&fimd_driver);
	if (ret < 0)
<<<<<<< HEAD
<<<<<<< HEAD
		goto out_fimd;
#endif

#ifdef CONFIG_DRM_EXYNOS_HDMI
	ret = platform_driver_register(&hdmi_driver);
	if (ret < 0)
		goto out_hdmi;
	ret = platform_driver_register(&mixer_driver);
	if (ret < 0)
		goto out_mixer;
	ret = platform_driver_register(&exynos_drm_common_hdmi_driver);
	if (ret < 0)
		goto out_common_hdmi;

	ret = exynos_platform_device_hdmi_register();
	if (ret < 0)
		goto out_common_hdmi_dev;
#endif

#ifdef CONFIG_DRM_EXYNOS_VIDI
	ret = platform_driver_register(&vidi_driver);
	if (ret < 0)
		goto out_vidi;
#endif

#ifdef CONFIG_DRM_EXYNOS_G2D
	ret = platform_driver_register(&g2d_driver);
	if (ret < 0)
		goto out_g2d;
=======
=======
>>>>>>> v3.18
		return ret;
#endif

#ifdef CONFIG_DRM_EXYNOS_DP
	ret = platform_driver_register(&dp_driver);
	if (ret < 0)
		goto err_unregister_fimd_drv;
#endif

#ifdef CONFIG_DRM_EXYNOS_DSI
	ret = platform_driver_register(&dsi_driver);
	if (ret < 0)
		goto err_unregister_dp_drv;
#endif

#ifdef CONFIG_DRM_EXYNOS_HDMI
	ret = platform_driver_register(&mixer_driver);
	if (ret < 0)
		goto err_unregister_dsi_drv;
	ret = platform_driver_register(&hdmi_driver);
	if (ret < 0)
		goto err_unregister_mixer_drv;
#endif

	match = exynos_drm_match_add(&pdev->dev);
	if (IS_ERR(match)) {
		ret = PTR_ERR(match);
		goto err_unregister_hdmi_drv;
	}

	ret = component_master_add_with_match(&pdev->dev, &exynos_drm_ops,
						match);
	if (ret < 0)
		goto err_unregister_hdmi_drv;

#ifdef CONFIG_DRM_EXYNOS_G2D
	ret = platform_driver_register(&g2d_driver);
	if (ret < 0)
		goto err_del_component_master;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif

#ifdef CONFIG_DRM_EXYNOS_FIMC
	ret = platform_driver_register(&fimc_driver);
	if (ret < 0)
<<<<<<< HEAD
<<<<<<< HEAD
		goto out_fimc;
=======
		goto err_unregister_g2d_drv;
>>>>>>> v3.18
=======
		goto err_unregister_g2d_drv;
>>>>>>> v3.18
#endif

#ifdef CONFIG_DRM_EXYNOS_ROTATOR
	ret = platform_driver_register(&rotator_driver);
	if (ret < 0)
<<<<<<< HEAD
<<<<<<< HEAD
		goto out_rotator;
=======
		goto err_unregister_fimc_drv;
>>>>>>> v3.18
=======
		goto err_unregister_fimc_drv;
>>>>>>> v3.18
#endif

#ifdef CONFIG_DRM_EXYNOS_GSC
	ret = platform_driver_register(&gsc_driver);
	if (ret < 0)
<<<<<<< HEAD
<<<<<<< HEAD
		goto out_gsc;
=======
		goto err_unregister_rotator_drv;
>>>>>>> v3.18
=======
		goto err_unregister_rotator_drv;
>>>>>>> v3.18
#endif

#ifdef CONFIG_DRM_EXYNOS_IPP
	ret = platform_driver_register(&ipp_driver);
	if (ret < 0)
<<<<<<< HEAD
<<<<<<< HEAD
		goto out_ipp;

	ret = exynos_platform_device_ipp_register();
	if (ret < 0)
		goto out_ipp_dev;
#endif

	ret = platform_driver_register(&exynos_drm_platform_driver);
	if (ret < 0)
		goto out_drm;

	exynos_drm_pdev = platform_device_register_simple("exynos-drm", -1,
				NULL, 0);
	if (IS_ERR(exynos_drm_pdev)) {
		ret = PTR_ERR(exynos_drm_pdev);
		goto out;
	}

	return 0;

out:
	platform_driver_unregister(&exynos_drm_platform_driver);

out_drm:
#ifdef CONFIG_DRM_EXYNOS_IPP
	exynos_platform_device_ipp_unregister();
out_ipp_dev:
	platform_driver_unregister(&ipp_driver);
out_ipp:
=======
=======
>>>>>>> v3.18
		goto err_unregister_gsc_drv;

	ret = exynos_platform_device_ipp_register();
	if (ret < 0)
		goto err_unregister_ipp_drv;
#endif

	return ret;

#ifdef CONFIG_DRM_EXYNOS_IPP
err_unregister_ipp_drv:
	platform_driver_unregister(&ipp_driver);
err_unregister_gsc_drv:
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif

#ifdef CONFIG_DRM_EXYNOS_GSC
	platform_driver_unregister(&gsc_driver);
<<<<<<< HEAD
<<<<<<< HEAD
out_gsc:
=======
err_unregister_rotator_drv:
>>>>>>> v3.18
=======
err_unregister_rotator_drv:
>>>>>>> v3.18
#endif

#ifdef CONFIG_DRM_EXYNOS_ROTATOR
	platform_driver_unregister(&rotator_driver);
<<<<<<< HEAD
<<<<<<< HEAD
out_rotator:
=======
err_unregister_fimc_drv:
>>>>>>> v3.18
=======
err_unregister_fimc_drv:
>>>>>>> v3.18
#endif

#ifdef CONFIG_DRM_EXYNOS_FIMC
	platform_driver_unregister(&fimc_driver);
<<<<<<< HEAD
<<<<<<< HEAD
out_fimc:
=======
err_unregister_g2d_drv:
>>>>>>> v3.18
=======
err_unregister_g2d_drv:
>>>>>>> v3.18
#endif

#ifdef CONFIG_DRM_EXYNOS_G2D
	platform_driver_unregister(&g2d_driver);
<<<<<<< HEAD
<<<<<<< HEAD
out_g2d:
#endif

#ifdef CONFIG_DRM_EXYNOS_VIDI
	platform_driver_unregister(&vidi_driver);
out_vidi:
#endif

#ifdef CONFIG_DRM_EXYNOS_HDMI
	exynos_platform_device_hdmi_unregister();
out_common_hdmi_dev:
	platform_driver_unregister(&exynos_drm_common_hdmi_driver);
out_common_hdmi:
	platform_driver_unregister(&mixer_driver);
out_mixer:
	platform_driver_unregister(&hdmi_driver);
out_hdmi:
=======
=======
>>>>>>> v3.18
err_del_component_master:
#endif
	component_master_del(&pdev->dev, &exynos_drm_ops);

err_unregister_hdmi_drv:
#ifdef CONFIG_DRM_EXYNOS_HDMI
	platform_driver_unregister(&hdmi_driver);
err_unregister_mixer_drv:
	platform_driver_unregister(&mixer_driver);
err_unregister_dsi_drv:
#endif

#ifdef CONFIG_DRM_EXYNOS_DSI
	platform_driver_unregister(&dsi_driver);
err_unregister_dp_drv:
#endif

#ifdef CONFIG_DRM_EXYNOS_DP
	platform_driver_unregister(&dp_driver);
err_unregister_fimd_drv:
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif

#ifdef CONFIG_DRM_EXYNOS_FIMD
	platform_driver_unregister(&fimd_driver);
<<<<<<< HEAD
<<<<<<< HEAD
out_fimd:
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif
	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void __exit exynos_drm_exit(void)
{
	DRM_DEBUG_DRIVER("%s\n", __FILE__);

	platform_device_unregister(exynos_drm_pdev);

	platform_driver_unregister(&exynos_drm_platform_driver);

=======
static int exynos_drm_platform_remove(struct platform_device *pdev)
{
>>>>>>> v3.18
=======
static int exynos_drm_platform_remove(struct platform_device *pdev)
{
>>>>>>> v3.18
#ifdef CONFIG_DRM_EXYNOS_IPP
	exynos_platform_device_ipp_unregister();
	platform_driver_unregister(&ipp_driver);
#endif

#ifdef CONFIG_DRM_EXYNOS_GSC
	platform_driver_unregister(&gsc_driver);
#endif

#ifdef CONFIG_DRM_EXYNOS_ROTATOR
	platform_driver_unregister(&rotator_driver);
#endif

#ifdef CONFIG_DRM_EXYNOS_FIMC
	platform_driver_unregister(&fimc_driver);
#endif

#ifdef CONFIG_DRM_EXYNOS_G2D
	platform_driver_unregister(&g2d_driver);
#endif

#ifdef CONFIG_DRM_EXYNOS_HDMI
<<<<<<< HEAD
<<<<<<< HEAD
	exynos_platform_device_hdmi_unregister();
	platform_driver_unregister(&exynos_drm_common_hdmi_driver);
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	platform_driver_unregister(&mixer_driver);
	platform_driver_unregister(&hdmi_driver);
#endif

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_DRM_EXYNOS_VIDI
	platform_driver_unregister(&vidi_driver);
#endif

#ifdef CONFIG_DRM_EXYNOS_FIMD
	platform_driver_unregister(&fimd_driver);
#endif
=======
=======
>>>>>>> v3.18
#ifdef CONFIG_DRM_EXYNOS_FIMD
	platform_driver_unregister(&fimd_driver);
#endif

#ifdef CONFIG_DRM_EXYNOS_DSI
	platform_driver_unregister(&dsi_driver);
#endif

#ifdef CONFIG_DRM_EXYNOS_DP
	platform_driver_unregister(&dp_driver);
#endif
	component_master_del(&pdev->dev, &exynos_drm_ops);
	return 0;
}

static struct platform_driver exynos_drm_platform_driver = {
	.probe	= exynos_drm_platform_probe,
	.remove	= exynos_drm_platform_remove,
	.driver	= {
		.owner	= THIS_MODULE,
		.name	= "exynos-drm",
		.pm	= &exynos_drm_pm_ops,
	},
};

static int exynos_drm_init(void)
{
	int ret;

	/*
	 * Register device object only in case of Exynos SoC.
	 *
	 * Below codes resolves temporarily infinite loop issue incurred
	 * by Exynos drm driver when using multi-platform kernel.
	 * So these codes will be replaced with more generic way later.
	 */
	if (!of_machine_is_compatible("samsung,exynos3") &&
			!of_machine_is_compatible("samsung,exynos4") &&
			!of_machine_is_compatible("samsung,exynos5"))
		return -ENODEV;

	exynos_drm_pdev = platform_device_register_simple("exynos-drm", -1,
								NULL, 0);
	if (IS_ERR(exynos_drm_pdev))
		return PTR_ERR(exynos_drm_pdev);

#ifdef CONFIG_DRM_EXYNOS_VIDI
	ret = exynos_drm_probe_vidi();
	if (ret < 0)
		goto err_unregister_pd;
#endif

	ret = platform_driver_register(&exynos_drm_platform_driver);
	if (ret)
		goto err_remove_vidi;

	return 0;

err_remove_vidi:
#ifdef CONFIG_DRM_EXYNOS_VIDI
	exynos_drm_remove_vidi();

err_unregister_pd:
#endif
	platform_device_unregister(exynos_drm_pdev);

	return ret;
}

static void exynos_drm_exit(void)
{
	platform_driver_unregister(&exynos_drm_platform_driver);
#ifdef CONFIG_DRM_EXYNOS_VIDI
	exynos_drm_remove_vidi();
#endif
	platform_device_unregister(exynos_drm_pdev);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}

module_init(exynos_drm_init);
module_exit(exynos_drm_exit);

MODULE_AUTHOR("Inki Dae <inki.dae@samsung.com>");
MODULE_AUTHOR("Joonyoung Shim <jy0922.shim@samsung.com>");
MODULE_AUTHOR("Seung-Woo Kim <sw0312.kim@samsung.com>");
MODULE_DESCRIPTION("Samsung SoC DRM Driver");
MODULE_LICENSE("GPL");
