/*
 * linux/fs/hfsplus/xattr.h
 *
 * Vyacheslav Dubeyko <slava@dubeyko.com>
 *
 * Logic of processing extended attributes
 */

#ifndef _LINUX_HFSPLUS_XATTR_H
#define _LINUX_HFSPLUS_XATTR_H

#include <linux/xattr.h>

extern const struct xattr_handler hfsplus_xattr_osx_handler;
extern const struct xattr_handler hfsplus_xattr_user_handler;
extern const struct xattr_handler hfsplus_xattr_trusted_handler;
<<<<<<< HEAD
<<<<<<< HEAD
/*extern const struct xattr_handler hfsplus_xattr_acl_access_handler;*/
/*extern const struct xattr_handler hfsplus_xattr_acl_default_handler;*/
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
extern const struct xattr_handler hfsplus_xattr_security_handler;

extern const struct xattr_handler *hfsplus_xattr_handlers[];

int __hfsplus_setxattr(struct inode *inode, const char *name,
			const void *value, size_t size, int flags);

static inline int hfsplus_setxattr(struct dentry *dentry, const char *name,
			const void *value, size_t size, int flags)
{
	return __hfsplus_setxattr(dentry->d_inode, name, value, size, flags);
}

<<<<<<< HEAD
<<<<<<< HEAD
ssize_t hfsplus_getxattr(struct dentry *dentry, const char *name,
			void *value, size_t size);

ssize_t hfsplus_listxattr(struct dentry *dentry, char *buffer, size_t size);

int hfsplus_removexattr(struct dentry *dentry, const char *name);
=======
=======
>>>>>>> v3.18
ssize_t __hfsplus_getxattr(struct inode *inode, const char *name,
			void *value, size_t size);

static inline ssize_t hfsplus_getxattr(struct dentry *dentry,
					const char *name,
					void *value,
					size_t size)
{
	return __hfsplus_getxattr(dentry->d_inode, name, value, size);
}

ssize_t hfsplus_listxattr(struct dentry *dentry, char *buffer, size_t size);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

int hfsplus_init_security(struct inode *inode, struct inode *dir,
				const struct qstr *qstr);

<<<<<<< HEAD
<<<<<<< HEAD
static inline int hfsplus_init_acl(struct inode *inode, struct inode *dir)
{
	/*TODO: implement*/
	return 0;
}

static inline int hfsplus_init_inode_security(struct inode *inode,
						struct inode *dir,
						const struct qstr *qstr)
{
	int err;

	err = hfsplus_init_acl(inode, dir);
	if (!err)
		err = hfsplus_init_security(inode, dir, qstr);
	return err;
}
=======
int hfsplus_init_inode_security(struct inode *inode, struct inode *dir,
				const struct qstr *qstr);
>>>>>>> v3.18
=======
int hfsplus_init_inode_security(struct inode *inode, struct inode *dir,
				const struct qstr *qstr);
>>>>>>> v3.18

#endif
