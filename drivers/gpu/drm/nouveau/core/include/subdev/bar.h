#ifndef __NOUVEAU_BAR_H__
#define __NOUVEAU_BAR_H__

#include <core/subdev.h>
#include <core/device.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include <subdev/fb.h>

=======
struct nouveau_mem;
>>>>>>> v3.18
=======
struct nouveau_mem;
>>>>>>> v3.18
struct nouveau_vma;

struct nouveau_bar {
	struct nouveau_subdev base;

	int (*alloc)(struct nouveau_bar *, struct nouveau_object *,
		     struct nouveau_mem *, struct nouveau_object **);
<<<<<<< HEAD
<<<<<<< HEAD
	void __iomem *iomem;
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18

	int (*kmap)(struct nouveau_bar *, struct nouveau_mem *,
		    u32 flags, struct nouveau_vma *);
	int (*umap)(struct nouveau_bar *, struct nouveau_mem *,
		    u32 flags, struct nouveau_vma *);
	void (*unmap)(struct nouveau_bar *, struct nouveau_vma *);
	void (*flush)(struct nouveau_bar *);
<<<<<<< HEAD
<<<<<<< HEAD
=======

	/* whether the BAR supports to be ioremapped WC or should be uncached */
	bool iomap_uncached;
>>>>>>> v3.18
=======

	/* whether the BAR supports to be ioremapped WC or should be uncached */
	bool iomap_uncached;
>>>>>>> v3.18
};

static inline struct nouveau_bar *
nouveau_bar(void *obj)
{
	return (void *)nv_device(obj)->subdev[NVDEV_SUBDEV_BAR];
}

<<<<<<< HEAD
<<<<<<< HEAD
#define nouveau_bar_create(p,e,o,d)                                            \
	nouveau_bar_create_((p), (e), (o), sizeof(**d), (void **)d)
#define nouveau_bar_init(p)                                                    \
	nouveau_subdev_init(&(p)->base)
#define nouveau_bar_fini(p,s)                                                  \
	nouveau_subdev_fini(&(p)->base, (s))

int nouveau_bar_create_(struct nouveau_object *, struct nouveau_object *,
			struct nouveau_oclass *, int, void **);
void nouveau_bar_destroy(struct nouveau_bar *);

void _nouveau_bar_dtor(struct nouveau_object *);
#define _nouveau_bar_init _nouveau_subdev_init
#define _nouveau_bar_fini _nouveau_subdev_fini

extern struct nouveau_oclass nv50_bar_oclass;
extern struct nouveau_oclass nvc0_bar_oclass;

int nouveau_bar_alloc(struct nouveau_bar *, struct nouveau_object *,
		      struct nouveau_mem *, struct nouveau_object **);

void nv84_bar_flush(struct nouveau_bar *);
=======
extern struct nouveau_oclass nv50_bar_oclass;
extern struct nouveau_oclass nvc0_bar_oclass;
extern struct nouveau_oclass gk20a_bar_oclass;
>>>>>>> v3.18
=======
extern struct nouveau_oclass nv50_bar_oclass;
extern struct nouveau_oclass nvc0_bar_oclass;
extern struct nouveau_oclass gk20a_bar_oclass;
>>>>>>> v3.18

#endif
