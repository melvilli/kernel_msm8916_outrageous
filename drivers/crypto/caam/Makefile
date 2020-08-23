#
# Makefile for the CAAM backend and dependent components
#
<<<<<<< HEAD

obj-$(CONFIG_CRYPTO_DEV_FSL_CAAM) += caam.o
=======
ifeq ($(CONFIG_CRYPTO_DEV_FSL_CAAM_DEBUG), y)
	EXTRA_CFLAGS := -DDEBUG
endif

obj-$(CONFIG_CRYPTO_DEV_FSL_CAAM) += caam.o
obj-$(CONFIG_CRYPTO_DEV_FSL_CAAM_JR) += caam_jr.o
>>>>>>> v3.18
obj-$(CONFIG_CRYPTO_DEV_FSL_CAAM_CRYPTO_API) += caamalg.o
obj-$(CONFIG_CRYPTO_DEV_FSL_CAAM_AHASH_API) += caamhash.o
obj-$(CONFIG_CRYPTO_DEV_FSL_CAAM_RNG_API) += caamrng.o

<<<<<<< HEAD
caam-objs := ctrl.o jr.o error.o key_gen.o
=======
caam-objs := ctrl.o
caam_jr-objs := jr.o key_gen.o error.o
>>>>>>> v3.18
