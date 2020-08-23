#ifndef __NVKM_EVENT_H__
#define __NVKM_EVENT_H__

<<<<<<< HEAD
/* return codes from event handlers */
#define NVKM_EVENT_DROP 0
#define NVKM_EVENT_KEEP 1

struct nouveau_eventh {
	struct list_head head;
	int (*func)(struct nouveau_eventh *, int index);
};

struct nouveau_event {
	spinlock_t lock;

	void *priv;
	void (*enable)(struct nouveau_event *, int index);
	void (*disable)(struct nouveau_event *, int index);

	int index_nr;
	struct {
		struct list_head list;
		int refs;
	} index[];
};

int  nouveau_event_create(int index_nr, struct nouveau_event **);
void nouveau_event_destroy(struct nouveau_event **);
void nouveau_event_trigger(struct nouveau_event *, int index);

void nouveau_event_get(struct nouveau_event *, int index,
		       struct nouveau_eventh *);
void nouveau_event_put(struct nouveau_event *, int index,
		       struct nouveau_eventh *);
=======
#include <core/notify.h>

struct nvkm_event_func {
	int  (*ctor)(struct nouveau_object *, void *data, u32 size,
		     struct nvkm_notify *);
	void (*send)(void *data, u32 size, struct nvkm_notify *);
	void (*init)(struct nvkm_event *, int type, int index);
	void (*fini)(struct nvkm_event *, int type, int index);
};

struct nvkm_event {
	const struct nvkm_event_func *func;

	int types_nr;
	int index_nr;

	spinlock_t refs_lock;
	spinlock_t list_lock;
	struct list_head list;
	int *refs;
};

int  nvkm_event_init(const struct nvkm_event_func *func,
		     int types_nr, int index_nr,
		     struct nvkm_event *);
void nvkm_event_fini(struct nvkm_event *);
void nvkm_event_get(struct nvkm_event *, u32 types, int index);
void nvkm_event_put(struct nvkm_event *, u32 types, int index);
void nvkm_event_send(struct nvkm_event *, u32 types, int index,
		     void *data, u32 size);
>>>>>>> v3.18

#endif
