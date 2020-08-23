/*
 * shmob_drm_backlight.h  --  SH Mobile DRM Backlight
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (C) 2012 Renesas Corporation
=======
 * Copyright (C) 2012 Renesas Electronics Corporation
>>>>>>> v3.18
=======
 * Copyright (C) 2012 Renesas Electronics Corporation
>>>>>>> v3.18
 *
 * Laurent Pinchart (laurent.pinchart@ideasonboard.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __SHMOB_DRM_BACKLIGHT_H__
#define __SHMOB_DRM_BACKLIGHT_H__

struct shmob_drm_connector;

void shmob_drm_backlight_dpms(struct shmob_drm_connector *scon, int mode);
int shmob_drm_backlight_init(struct shmob_drm_connector *scon);
void shmob_drm_backlight_exit(struct shmob_drm_connector *scon);

#endif /* __SHMOB_DRM_BACKLIGHT_H__ */
