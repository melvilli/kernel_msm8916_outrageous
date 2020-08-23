#ifndef __NOUVEAU_IOCTL_H__
#define __NOUVEAU_IOCTL_H__

long nouveau_compat_ioctl(struct file *, unsigned int cmd, unsigned long arg);
<<<<<<< HEAD
<<<<<<< HEAD
=======
long nouveau_drm_ioctl(struct file *, unsigned int cmd, unsigned long arg);
>>>>>>> v3.18
=======
long nouveau_drm_ioctl(struct file *, unsigned int cmd, unsigned long arg);
>>>>>>> v3.18

#endif
