#
# Makefile for key management
#

#
# Core
#
obj-y := \
	gc.o \
	key.o \
	keyring.o \
	keyctl.o \
	permission.o \
	process_keys.o \
	request_key.o \
	request_key_auth.o \
	user_defined.o
obj-$(CONFIG_KEYS_COMPAT) += compat.o
obj-$(CONFIG_PROC_FS) += proc.o
obj-$(CONFIG_SYSCTL) += sysctl.o
<<<<<<< HEAD
=======
obj-$(CONFIG_PERSISTENT_KEYRINGS) += persistent.o
>>>>>>> v3.18

#
# Key types
#
<<<<<<< HEAD
=======
obj-$(CONFIG_BIG_KEYS) += big_key.o
>>>>>>> v3.18
obj-$(CONFIG_TRUSTED_KEYS) += trusted.o
obj-$(CONFIG_ENCRYPTED_KEYS) += encrypted-keys/
