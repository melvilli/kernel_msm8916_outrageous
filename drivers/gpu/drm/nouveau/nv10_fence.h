#ifndef __NV10_FENCE_H_
#define __NV10_FENCE_H_

<<<<<<< HEAD
<<<<<<< HEAD
#include <core/os.h>
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
#include "nouveau_fence.h"
#include "nouveau_bo.h"

struct nv10_fence_chan {
	struct nouveau_fence_chan base;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct nvif_object sema;
	struct nvif_object head[4];
>>>>>>> v3.18
=======
	struct nvif_object sema;
	struct nvif_object head[4];
>>>>>>> v3.18
};

struct nv10_fence_priv {
	struct nouveau_fence_priv base;
	struct nouveau_bo *bo;
	spinlock_t lock;
	u32 sequence;
};

#endif
