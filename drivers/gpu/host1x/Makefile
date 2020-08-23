<<<<<<< HEAD
ccflags-y = -Idrivers/gpu/host1x

host1x-y = \
=======
host1x-y = \
	bus.o \
>>>>>>> v3.18
	syncpt.o \
	dev.o \
	intr.o \
	cdma.o \
	channel.o \
	job.o \
	debug.o \
<<<<<<< HEAD
	hw/host1x01.o

ccflags-y += -Iinclude/drm
ccflags-$(CONFIG_DRM_TEGRA_DEBUG) += -DDEBUG

host1x-$(CONFIG_DRM_TEGRA) += drm/drm.o drm/fb.o drm/dc.o
host1x-$(CONFIG_DRM_TEGRA) += drm/output.o drm/rgb.o drm/hdmi.o
host1x-$(CONFIG_DRM_TEGRA) += drm/gem.o
host1x-$(CONFIG_DRM_TEGRA) += drm/gr2d.o
=======
	mipi.o \
	hw/host1x01.o \
	hw/host1x02.o \
	hw/host1x04.o

>>>>>>> v3.18
obj-$(CONFIG_TEGRA_HOST1X) += host1x.o
