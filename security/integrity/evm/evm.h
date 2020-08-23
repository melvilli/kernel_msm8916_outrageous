/*
 * Copyright (C) 2005-2010 IBM Corporation
 *
 * Authors:
 * Mimi Zohar <zohar@us.ibm.com>
 * Kylene Hall <kjhall@us.ibm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
 * File: evm.h
 *
 */

#ifndef __INTEGRITY_EVM_H
#define __INTEGRITY_EVM_H

#include <linux/xattr.h>
#include <linux/security.h>

#include "../integrity.h"

extern int evm_initialized;
extern char *evm_hmac;
extern char *evm_hash;
<<<<<<< HEAD
<<<<<<< HEAD
extern int evm_hmac_version;
=======
=======
>>>>>>> v3.18

#define EVM_ATTR_FSUUID		0x0001

extern int evm_hmac_attrs;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

extern struct crypto_shash *hmac_tfm;
extern struct crypto_shash *hash_tfm;

/* List of EVM protected security xattrs */
extern char *evm_config_xattrnames[];

<<<<<<< HEAD
<<<<<<< HEAD
extern int evm_init_key(void);
extern int evm_update_evmxattr(struct dentry *dentry,
			       const char *req_xattr_name,
			       const char *req_xattr_value,
			       size_t req_xattr_value_len);
extern int evm_calc_hmac(struct dentry *dentry, const char *req_xattr_name,
			 const char *req_xattr_value,
			 size_t req_xattr_value_len, char *digest);
extern int evm_calc_hash(struct dentry *dentry, const char *req_xattr_name,
			 const char *req_xattr_value,
			 size_t req_xattr_value_len, char *digest);
extern int evm_init_hmac(struct inode *inode, const struct xattr *xattr,
			 char *hmac_val);
extern int evm_init_secfs(void);
=======
=======
>>>>>>> v3.18
int evm_init_key(void);
int evm_update_evmxattr(struct dentry *dentry,
			const char *req_xattr_name,
			const char *req_xattr_value,
			size_t req_xattr_value_len);
int evm_calc_hmac(struct dentry *dentry, const char *req_xattr_name,
		  const char *req_xattr_value,
		  size_t req_xattr_value_len, char *digest);
int evm_calc_hash(struct dentry *dentry, const char *req_xattr_name,
		  const char *req_xattr_value,
		  size_t req_xattr_value_len, char *digest);
int evm_init_hmac(struct inode *inode, const struct xattr *xattr,
		  char *hmac_val);
int evm_init_secfs(void);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

#endif
