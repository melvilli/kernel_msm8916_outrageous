/*
 * Copyright 2012 Red Hat Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

<<<<<<< HEAD
#include <core/object.h>
#include <core/client.h>
#include <core/device.h>
#include <core/class.h>
#include <core/mm.h>

#include <subdev/fb.h>
#include <subdev/timer.h>
#include <subdev/instmem.h>
#include <engine/graph.h>
=======
#include <nvif/client.h>
#include <nvif/driver.h>
#include <nvif/ioctl.h>
#include <nvif/class.h>
>>>>>>> v3.18

#include "nouveau_drm.h"
#include "nouveau_dma.h"
#include "nouveau_gem.h"
#include "nouveau_chan.h"
#include "nouveau_abi16.h"

struct nouveau_abi16 *
nouveau_abi16_get(struct drm_file *file_priv, struct drm_device *dev)
{
	struct nouveau_cli *cli = nouveau_cli(file_priv);
	mutex_lock(&cli->mutex);
	if (!cli->abi16) {
		struct nouveau_abi16 *abi16;
		cli->abi16 = abi16 = kzalloc(sizeof(*abi16), GFP_KERNEL);
		if (cli->abi16) {
<<<<<<< HEAD
			INIT_LIST_HEAD(&abi16->channels);
			abi16->client = nv_object(cli);
=======
			struct nv_device_v0 args = {
				.device = ~0ULL,
			};

			INIT_LIST_HEAD(&abi16->channels);
>>>>>>> v3.18

			/* allocate device object targeting client's default
			 * device (ie. the one that belongs to the fd it
			 * opened)
			 */
<<<<<<< HEAD
			if (nouveau_object_new(abi16->client, NVDRM_CLIENT,
					       NVDRM_DEVICE, 0x0080,
					       &(struct nv_device_class) {
						.device = ~0ULL,
					       },
					       sizeof(struct nv_device_class),
					       &abi16->device) == 0)
=======
			if (nvif_device_init(&cli->base.base, NULL,
					     NOUVEAU_ABI16_DEVICE, NV_DEVICE,
					     &args, sizeof(args),
					     &abi16->device) == 0)
>>>>>>> v3.18
				return cli->abi16;

			kfree(cli->abi16);
			cli->abi16 = NULL;
		}

		mutex_unlock(&cli->mutex);
	}
	return cli->abi16;
}

int
nouveau_abi16_put(struct nouveau_abi16 *abi16, int ret)
{
<<<<<<< HEAD
	struct nouveau_cli *cli = (void *)abi16->client;
=======
	struct nouveau_cli *cli = (void *)nvif_client(&abi16->device.base);
>>>>>>> v3.18
	mutex_unlock(&cli->mutex);
	return ret;
}

u16
nouveau_abi16_swclass(struct nouveau_drm *drm)
{
<<<<<<< HEAD
	switch (nv_device(drm->device)->card_type) {
	case NV_04:
		return 0x006e;
	case NV_10:
	case NV_20:
	case NV_30:
	case NV_40:
		return 0x016e;
	case NV_50:
		return 0x506e;
	case NV_C0:
	case NV_D0:
	case NV_E0:
=======
	switch (drm->device.info.family) {
	case NV_DEVICE_INFO_V0_TNT:
		return 0x006e;
	case NV_DEVICE_INFO_V0_CELSIUS:
	case NV_DEVICE_INFO_V0_KELVIN:
	case NV_DEVICE_INFO_V0_RANKINE:
	case NV_DEVICE_INFO_V0_CURIE:
		return 0x016e;
	case NV_DEVICE_INFO_V0_TESLA:
		return 0x506e;
	case NV_DEVICE_INFO_V0_FERMI:
	case NV_DEVICE_INFO_V0_KEPLER:
	case NV_DEVICE_INFO_V0_MAXWELL:
>>>>>>> v3.18
		return 0x906e;
	}

	return 0x0000;
}

static void
nouveau_abi16_ntfy_fini(struct nouveau_abi16_chan *chan,
			struct nouveau_abi16_ntfy *ntfy)
{
	nouveau_mm_free(&chan->heap, &ntfy->node);
	list_del(&ntfy->head);
	kfree(ntfy);
}

static void
nouveau_abi16_chan_fini(struct nouveau_abi16 *abi16,
			struct nouveau_abi16_chan *chan)
{
	struct nouveau_abi16_ntfy *ntfy, *temp;

	/* wait for all activity to stop before releasing notify object, which
	 * may be still in use */
	if (chan->chan && chan->ntfy)
		nouveau_channel_idle(chan->chan);

	/* cleanup notifier state */
	list_for_each_entry_safe(ntfy, temp, &chan->notifiers, head) {
		nouveau_abi16_ntfy_fini(chan, ntfy);
	}

	if (chan->ntfy) {
		nouveau_bo_vma_del(chan->ntfy, &chan->ntfy_vma);
<<<<<<< HEAD
		drm_gem_object_unreference_unlocked(chan->ntfy->gem);
=======
		nouveau_bo_unpin(chan->ntfy);
		drm_gem_object_unreference_unlocked(&chan->ntfy->gem);
>>>>>>> v3.18
	}

	if (chan->heap.block_size)
		nouveau_mm_fini(&chan->heap);

	/* destroy channel object, all children will be killed too */
	if (chan->chan) {
<<<<<<< HEAD
		abi16->handles &= ~(1 << (chan->chan->handle & 0xffff));
=======
		abi16->handles &= ~(1ULL << (chan->chan->object->handle & 0xffff));
>>>>>>> v3.18
		nouveau_channel_del(&chan->chan);
	}

	list_del(&chan->head);
	kfree(chan);
}

void
nouveau_abi16_fini(struct nouveau_abi16 *abi16)
{
<<<<<<< HEAD
	struct nouveau_cli *cli = (void *)abi16->client;
=======
	struct nouveau_cli *cli = (void *)nvif_client(&abi16->device.base);
>>>>>>> v3.18
	struct nouveau_abi16_chan *chan, *temp;

	/* cleanup channels */
	list_for_each_entry_safe(chan, temp, &abi16->channels, head) {
		nouveau_abi16_chan_fini(abi16, chan);
	}

	/* destroy the device object */
<<<<<<< HEAD
	nouveau_object_del(abi16->client, NVDRM_CLIENT, NVDRM_DEVICE);
=======
	nvif_device_fini(&abi16->device);
>>>>>>> v3.18

	kfree(cli->abi16);
	cli->abi16 = NULL;
}

int
nouveau_abi16_ioctl_getparam(ABI16_IOCTL_ARGS)
{
<<<<<<< HEAD
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nouveau_device *device = nv_device(drm->device);
	struct nouveau_timer *ptimer = nouveau_timer(device);
	struct nouveau_graph *graph = (void *)nouveau_engine(device, NVDEV_ENGINE_GR);
=======
	struct nouveau_cli *cli = nouveau_cli(file_priv);
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nvif_device *device = &drm->device;
	struct nouveau_timer *ptimer = nvkm_timer(device);
	struct nouveau_graph *graph = nvkm_gr(device);
>>>>>>> v3.18
	struct drm_nouveau_getparam *getparam = data;

	switch (getparam->param) {
	case NOUVEAU_GETPARAM_CHIPSET_ID:
<<<<<<< HEAD
		getparam->value = device->chipset;
		break;
	case NOUVEAU_GETPARAM_PCI_VENDOR:
		getparam->value = dev->pci_vendor;
		break;
	case NOUVEAU_GETPARAM_PCI_DEVICE:
		getparam->value = dev->pci_device;
		break;
	case NOUVEAU_GETPARAM_BUS_TYPE:
=======
		getparam->value = device->info.chipset;
		break;
	case NOUVEAU_GETPARAM_PCI_VENDOR:
		if (nv_device_is_pci(nvkm_device(device)))
			getparam->value = dev->pdev->vendor;
		else
			getparam->value = 0;
		break;
	case NOUVEAU_GETPARAM_PCI_DEVICE:
		if (nv_device_is_pci(nvkm_device(device)))
			getparam->value = dev->pdev->device;
		else
			getparam->value = 0;
		break;
	case NOUVEAU_GETPARAM_BUS_TYPE:
		if (!nv_device_is_pci(nvkm_device(device)))
			getparam->value = 3;
		else
>>>>>>> v3.18
		if (drm_pci_device_is_agp(dev))
			getparam->value = 0;
		else
		if (!pci_is_pcie(dev->pdev))
			getparam->value = 1;
		else
			getparam->value = 2;
		break;
	case NOUVEAU_GETPARAM_FB_SIZE:
		getparam->value = drm->gem.vram_available;
		break;
	case NOUVEAU_GETPARAM_AGP_SIZE:
		getparam->value = drm->gem.gart_available;
		break;
	case NOUVEAU_GETPARAM_VM_VRAM_BASE:
		getparam->value = 0; /* deprecated */
		break;
	case NOUVEAU_GETPARAM_PTIMER_TIME:
		getparam->value = ptimer->read(ptimer);
		break;
	case NOUVEAU_GETPARAM_HAS_BO_USAGE:
		getparam->value = 1;
		break;
	case NOUVEAU_GETPARAM_HAS_PAGEFLIP:
		getparam->value = 1;
		break;
	case NOUVEAU_GETPARAM_GRAPH_UNITS:
		getparam->value = graph->units ? graph->units(graph) : 0;
		break;
	default:
<<<<<<< HEAD
		nv_debug(device, "unknown parameter %lld\n", getparam->param);
=======
		NV_PRINTK(debug, cli, "unknown parameter %lld\n", getparam->param);
>>>>>>> v3.18
		return -EINVAL;
	}

	return 0;
}

int
nouveau_abi16_ioctl_setparam(ABI16_IOCTL_ARGS)
{
	return -EINVAL;
}

int
nouveau_abi16_ioctl_channel_alloc(ABI16_IOCTL_ARGS)
{
	struct drm_nouveau_channel_alloc *init = data;
	struct nouveau_cli *cli = nouveau_cli(file_priv);
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nouveau_abi16 *abi16 = nouveau_abi16_get(file_priv, dev);
	struct nouveau_abi16_chan *chan;
<<<<<<< HEAD
	struct nouveau_client *client;
	struct nouveau_device *device;
	struct nouveau_instmem *imem;
	struct nouveau_fb *pfb;
=======
	struct nvif_device *device;
>>>>>>> v3.18
	int ret;

	if (unlikely(!abi16))
		return -ENOMEM;

	if (!drm->channel)
		return nouveau_abi16_put(abi16, -ENODEV);

<<<<<<< HEAD
	client = nv_client(abi16->client);
	device = nv_device(abi16->device);
	imem   = nouveau_instmem(device);
	pfb    = nouveau_fb(device);

	/* hack to allow channel engine type specification on kepler */
	if (device->card_type >= NV_E0) {
		if (init->fb_ctxdma_handle != ~0)
			init->fb_ctxdma_handle = NVE0_CHANNEL_IND_ENGINE_GR;
=======
	device = &abi16->device;

	/* hack to allow channel engine type specification on kepler */
	if (device->info.family >= NV_DEVICE_INFO_V0_KEPLER) {
		if (init->fb_ctxdma_handle != ~0)
			init->fb_ctxdma_handle = KEPLER_CHANNEL_GPFIFO_A_V0_ENGINE_GR;
>>>>>>> v3.18
		else
			init->fb_ctxdma_handle = init->tt_ctxdma_handle;

		/* allow flips to be executed if this is a graphics channel */
		init->tt_ctxdma_handle = 0;
<<<<<<< HEAD
		if (init->fb_ctxdma_handle == NVE0_CHANNEL_IND_ENGINE_GR)
=======
		if (init->fb_ctxdma_handle == KEPLER_CHANNEL_GPFIFO_A_V0_ENGINE_GR)
>>>>>>> v3.18
			init->tt_ctxdma_handle = 1;
	}

	if (init->fb_ctxdma_handle == ~0 || init->tt_ctxdma_handle == ~0)
		return nouveau_abi16_put(abi16, -EINVAL);

	/* allocate "abi16 channel" data and make up a handle for it */
<<<<<<< HEAD
	init->channel = ffsll(~abi16->handles);
	if (!init->channel--)
=======
	init->channel = __ffs64(~abi16->handles);
	if (~abi16->handles == 0)
>>>>>>> v3.18
		return nouveau_abi16_put(abi16, -ENOSPC);

	chan = kzalloc(sizeof(*chan), GFP_KERNEL);
	if (!chan)
		return nouveau_abi16_put(abi16, -ENOMEM);

	INIT_LIST_HEAD(&chan->notifiers);
	list_add(&chan->head, &abi16->channels);
<<<<<<< HEAD
	abi16->handles |= (1 << init->channel);

	/* create channel object and initialise dma and fence management */
	ret = nouveau_channel_new(drm, cli, NVDRM_DEVICE, NVDRM_CHAN |
				  init->channel, init->fb_ctxdma_handle,
=======
	abi16->handles |= (1ULL << init->channel);

	/* create channel object and initialise dma and fence management */
	ret = nouveau_channel_new(drm, device,
				  NOUVEAU_ABI16_CHAN(init->channel),
				  init->fb_ctxdma_handle,
>>>>>>> v3.18
				  init->tt_ctxdma_handle, &chan->chan);
	if (ret)
		goto done;

<<<<<<< HEAD
	if (device->card_type >= NV_50)
=======
	if (device->info.family >= NV_DEVICE_INFO_V0_TESLA)
>>>>>>> v3.18
		init->pushbuf_domains = NOUVEAU_GEM_DOMAIN_VRAM |
					NOUVEAU_GEM_DOMAIN_GART;
	else
	if (chan->chan->push.buffer->bo.mem.mem_type == TTM_PL_VRAM)
		init->pushbuf_domains = NOUVEAU_GEM_DOMAIN_VRAM;
	else
		init->pushbuf_domains = NOUVEAU_GEM_DOMAIN_GART;

<<<<<<< HEAD
	if (device->card_type < NV_C0) {
		init->subchan[0].handle = 0x00000000;
		init->subchan[0].grclass = 0x0000;
		init->subchan[1].handle = NvSw;
=======
	if (device->info.family < NV_DEVICE_INFO_V0_CELSIUS) {
		init->subchan[0].handle = 0x00000000;
		init->subchan[0].grclass = 0x0000;
		init->subchan[1].handle = chan->chan->nvsw.handle;
>>>>>>> v3.18
		init->subchan[1].grclass = 0x506e;
		init->nr_subchan = 2;
	}

	/* Named memory object area */
	ret = nouveau_gem_new(dev, PAGE_SIZE, 0, NOUVEAU_GEM_DOMAIN_GART,
			      0, 0, &chan->ntfy);
	if (ret == 0)
		ret = nouveau_bo_pin(chan->ntfy, TTM_PL_FLAG_TT);
	if (ret)
		goto done;

<<<<<<< HEAD
	if (device->card_type >= NV_50) {
		ret = nouveau_bo_vma_add(chan->ntfy, client->vm,
=======
	if (device->info.family >= NV_DEVICE_INFO_V0_TESLA) {
		ret = nouveau_bo_vma_add(chan->ntfy, cli->vm,
>>>>>>> v3.18
					&chan->ntfy_vma);
		if (ret)
			goto done;
	}

<<<<<<< HEAD
	ret = drm_gem_handle_create(file_priv, chan->ntfy->gem,
=======
	ret = drm_gem_handle_create(file_priv, &chan->ntfy->gem,
>>>>>>> v3.18
				    &init->notifier_handle);
	if (ret)
		goto done;

	ret = nouveau_mm_init(&chan->heap, 0, PAGE_SIZE, 1);
done:
	if (ret)
		nouveau_abi16_chan_fini(abi16, chan);
	return nouveau_abi16_put(abi16, ret);
}

<<<<<<< HEAD
=======
static struct nouveau_abi16_chan *
nouveau_abi16_chan(struct nouveau_abi16 *abi16, int channel)
{
	struct nouveau_abi16_chan *chan;

	list_for_each_entry(chan, &abi16->channels, head) {
		if (chan->chan->object->handle == NOUVEAU_ABI16_CHAN(channel))
			return chan;
	}

	return NULL;
}
>>>>>>> v3.18

int
nouveau_abi16_ioctl_channel_free(ABI16_IOCTL_ARGS)
{
	struct drm_nouveau_channel_free *req = data;
	struct nouveau_abi16 *abi16 = nouveau_abi16_get(file_priv, dev);
	struct nouveau_abi16_chan *chan;
<<<<<<< HEAD
	int ret = -ENOENT;
=======
>>>>>>> v3.18

	if (unlikely(!abi16))
		return -ENOMEM;

<<<<<<< HEAD
	list_for_each_entry(chan, &abi16->channels, head) {
		if (chan->chan->handle == (NVDRM_CHAN | req->channel)) {
			nouveau_abi16_chan_fini(abi16, chan);
			return nouveau_abi16_put(abi16, 0);
		}
	}

	return nouveau_abi16_put(abi16, ret);
=======
	chan = nouveau_abi16_chan(abi16, req->channel);
	if (!chan)
		return nouveau_abi16_put(abi16, -ENOENT);
	nouveau_abi16_chan_fini(abi16, chan);
	return nouveau_abi16_put(abi16, 0);
>>>>>>> v3.18
}

int
nouveau_abi16_ioctl_grobj_alloc(ABI16_IOCTL_ARGS)
{
	struct drm_nouveau_grobj_alloc *init = data;
<<<<<<< HEAD
	struct nouveau_abi16 *abi16 = nouveau_abi16_get(file_priv, dev);
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nouveau_object *object;
=======
	struct {
		struct nvif_ioctl_v0 ioctl;
		struct nvif_ioctl_new_v0 new;
	} args = {
		.ioctl.owner = NVIF_IOCTL_V0_OWNER_ANY,
		.ioctl.type = NVIF_IOCTL_V0_NEW,
		.ioctl.path_nr = 3,
		.ioctl.path[2] = NOUVEAU_ABI16_CLIENT,
		.ioctl.path[1] = NOUVEAU_ABI16_DEVICE,
		.ioctl.path[0] = NOUVEAU_ABI16_CHAN(init->channel),
		.new.route = NVDRM_OBJECT_ABI16,
		.new.handle = init->handle,
		.new.oclass = init->class,
	};
	struct nouveau_abi16 *abi16 = nouveau_abi16_get(file_priv, dev);
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nvif_client *client;
>>>>>>> v3.18
	int ret;

	if (unlikely(!abi16))
		return -ENOMEM;

	if (init->handle == ~0)
		return nouveau_abi16_put(abi16, -EINVAL);
<<<<<<< HEAD
=======
	client = nvif_client(nvif_object(&abi16->device));
>>>>>>> v3.18

	/* compatibility with userspace that assumes 506e for all chipsets */
	if (init->class == 0x506e) {
		init->class = nouveau_abi16_swclass(drm);
		if (init->class == 0x906e)
			return nouveau_abi16_put(abi16, 0);
	}

<<<<<<< HEAD
	ret = nouveau_object_new(abi16->client, NVDRM_CHAN | init->channel,
				  init->handle, init->class, NULL, 0, &object);
=======
	ret = nvif_client_ioctl(client, &args, sizeof(args));
>>>>>>> v3.18
	return nouveau_abi16_put(abi16, ret);
}

int
nouveau_abi16_ioctl_notifierobj_alloc(ABI16_IOCTL_ARGS)
{
	struct drm_nouveau_notifierobj_alloc *info = data;
<<<<<<< HEAD
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nouveau_device *device = nv_device(drm->device);
	struct nouveau_abi16 *abi16 = nouveau_abi16_get(file_priv, dev);
	struct nouveau_abi16_chan *chan = NULL, *temp;
	struct nouveau_abi16_ntfy *ntfy;
	struct nouveau_object *object;
	struct nv_dma_class args = {};
=======
	struct {
		struct nvif_ioctl_v0 ioctl;
		struct nvif_ioctl_new_v0 new;
		struct nv_dma_v0 ctxdma;
	} args = {
		.ioctl.owner = NVIF_IOCTL_V0_OWNER_ANY,
		.ioctl.type = NVIF_IOCTL_V0_NEW,
		.ioctl.path_nr = 3,
		.ioctl.path[2] = NOUVEAU_ABI16_CLIENT,
		.ioctl.path[1] = NOUVEAU_ABI16_DEVICE,
		.ioctl.path[0] = NOUVEAU_ABI16_CHAN(info->channel),
		.new.route = NVDRM_OBJECT_ABI16,
		.new.handle = info->handle,
		.new.oclass = NV_DMA_IN_MEMORY,
	};
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nouveau_abi16 *abi16 = nouveau_abi16_get(file_priv, dev);
	struct nouveau_abi16_chan *chan;
	struct nouveau_abi16_ntfy *ntfy;
	struct nvif_device *device = &abi16->device;
	struct nvif_client *client;
>>>>>>> v3.18
	int ret;

	if (unlikely(!abi16))
		return -ENOMEM;

	/* completely unnecessary for these chipsets... */
<<<<<<< HEAD
	if (unlikely(nv_device(abi16->device)->card_type >= NV_C0))
		return nouveau_abi16_put(abi16, -EINVAL);

	list_for_each_entry(temp, &abi16->channels, head) {
		if (temp->chan->handle == (NVDRM_CHAN | info->channel)) {
			chan = temp;
			break;
		}
	}

=======
	if (unlikely(device->info.family >= NV_DEVICE_INFO_V0_FERMI))
		return nouveau_abi16_put(abi16, -EINVAL);
	client = nvif_client(nvif_object(&abi16->device));

	chan = nouveau_abi16_chan(abi16, info->channel);
>>>>>>> v3.18
	if (!chan)
		return nouveau_abi16_put(abi16, -ENOENT);

	ntfy = kzalloc(sizeof(*ntfy), GFP_KERNEL);
	if (!ntfy)
		return nouveau_abi16_put(abi16, -ENOMEM);

	list_add(&ntfy->head, &chan->notifiers);
	ntfy->handle = info->handle;

<<<<<<< HEAD
	ret = nouveau_mm_head(&chan->heap, 1, info->size, info->size, 1,
=======
	ret = nouveau_mm_head(&chan->heap, 0, 1, info->size, info->size, 1,
>>>>>>> v3.18
			      &ntfy->node);
	if (ret)
		goto done;

<<<<<<< HEAD
	args.start = ntfy->node->offset;
	args.limit = ntfy->node->offset + ntfy->node->length - 1;
	if (device->card_type >= NV_50) {
		args.flags  = NV_DMA_TARGET_VM | NV_DMA_ACCESS_VM;
		args.start += chan->ntfy_vma.offset;
		args.limit += chan->ntfy_vma.offset;
	} else
	if (drm->agp.stat == ENABLED) {
		args.flags  = NV_DMA_TARGET_AGP | NV_DMA_ACCESS_RDWR;
		args.start += drm->agp.base + chan->ntfy->bo.offset;
		args.limit += drm->agp.base + chan->ntfy->bo.offset;
	} else {
		args.flags  = NV_DMA_TARGET_VM | NV_DMA_ACCESS_RDWR;
		args.start += chan->ntfy->bo.offset;
		args.limit += chan->ntfy->bo.offset;
	}

	ret = nouveau_object_new(abi16->client, chan->chan->handle,
				 ntfy->handle, 0x003d, &args,
				 sizeof(args), &object);
	if (ret)
		goto done;

=======
	args.ctxdma.start = ntfy->node->offset;
	args.ctxdma.limit = ntfy->node->offset + ntfy->node->length - 1;
	if (device->info.family >= NV_DEVICE_INFO_V0_TESLA) {
		args.ctxdma.target = NV_DMA_V0_TARGET_VM;
		args.ctxdma.access = NV_DMA_V0_ACCESS_VM;
		args.ctxdma.start += chan->ntfy_vma.offset;
		args.ctxdma.limit += chan->ntfy_vma.offset;
	} else
	if (drm->agp.stat == ENABLED) {
		args.ctxdma.target = NV_DMA_V0_TARGET_AGP;
		args.ctxdma.access = NV_DMA_V0_ACCESS_RDWR;
		args.ctxdma.start += drm->agp.base + chan->ntfy->bo.offset;
		args.ctxdma.limit += drm->agp.base + chan->ntfy->bo.offset;
		client->super = true;
	} else {
		args.ctxdma.target = NV_DMA_V0_TARGET_VM;
		args.ctxdma.access = NV_DMA_V0_ACCESS_RDWR;
		args.ctxdma.start += chan->ntfy->bo.offset;
		args.ctxdma.limit += chan->ntfy->bo.offset;
	}

	ret = nvif_client_ioctl(client, &args, sizeof(args));
	client->super = false;
	if (ret)
		goto done;

	info->offset = ntfy->node->offset;

>>>>>>> v3.18
done:
	if (ret)
		nouveau_abi16_ntfy_fini(chan, ntfy);
	return nouveau_abi16_put(abi16, ret);
}

int
nouveau_abi16_ioctl_gpuobj_free(ABI16_IOCTL_ARGS)
{
	struct drm_nouveau_gpuobj_free *fini = data;
<<<<<<< HEAD
	struct nouveau_abi16 *abi16 = nouveau_abi16_get(file_priv, dev);
	struct nouveau_abi16_chan *chan = NULL, *temp;
	struct nouveau_abi16_ntfy *ntfy;
=======
	struct {
		struct nvif_ioctl_v0 ioctl;
		struct nvif_ioctl_del del;
	} args = {
		.ioctl.owner = NVDRM_OBJECT_ABI16,
		.ioctl.type = NVIF_IOCTL_V0_DEL,
		.ioctl.path_nr = 4,
		.ioctl.path[3] = NOUVEAU_ABI16_CLIENT,
		.ioctl.path[2] = NOUVEAU_ABI16_DEVICE,
		.ioctl.path[1] = NOUVEAU_ABI16_CHAN(fini->channel),
		.ioctl.path[0] = fini->handle,
	};
	struct nouveau_abi16 *abi16 = nouveau_abi16_get(file_priv, dev);
	struct nouveau_abi16_chan *chan;
	struct nouveau_abi16_ntfy *ntfy;
	struct nvif_client *client;
>>>>>>> v3.18
	int ret;

	if (unlikely(!abi16))
		return -ENOMEM;

<<<<<<< HEAD
	list_for_each_entry(temp, &abi16->channels, head) {
		if (temp->chan->handle == (NVDRM_CHAN | fini->channel)) {
			chan = temp;
			break;
		}
	}

	if (!chan)
		return nouveau_abi16_put(abi16, -ENOENT);
=======
	chan = nouveau_abi16_chan(abi16, fini->channel);
	if (!chan)
		return nouveau_abi16_put(abi16, -ENOENT);
	client = nvif_client(nvif_object(&abi16->device));
>>>>>>> v3.18

	/* synchronize with the user channel and destroy the gpu object */
	nouveau_channel_idle(chan->chan);

<<<<<<< HEAD
	ret = nouveau_object_del(abi16->client, chan->chan->handle, fini->handle);
=======
	ret = nvif_client_ioctl(client, &args, sizeof(args));
>>>>>>> v3.18
	if (ret)
		return nouveau_abi16_put(abi16, ret);

	/* cleanup extra state if this object was a notifier */
	list_for_each_entry(ntfy, &chan->notifiers, head) {
		if (ntfy->handle == fini->handle) {
			nouveau_mm_free(&chan->heap, &ntfy->node);
			list_del(&ntfy->head);
			break;
		}
	}

	return nouveau_abi16_put(abi16, 0);
}
