/*
 * linux/fs/hfsplus/xattr_trusted.c
 *
 * Vyacheslav Dubeyko <slava@dubeyko.com>
 *
 * Handler for storing security labels as extended attributes.
 */

#include <linux/security.h>
<<<<<<< HEAD
#include "hfsplus_fs.h"
#include "xattr.h"
=======
#include <linux/nls.h>

#include "hfsplus_fs.h"
#include "xattr.h"
#include "acl.h"
>>>>>>> v3.18

static int hfsplus_security_getxattr(struct dentry *dentry, const char *name,
					void *buffer, size_t size, int type)
{
<<<<<<< HEAD
	char xattr_name[HFSPLUS_ATTR_MAX_STRLEN + 1] = {0};
	size_t len = strlen(name);
=======
	char *xattr_name;
	int res;
>>>>>>> v3.18

	if (!strcmp(name, ""))
		return -EINVAL;

<<<<<<< HEAD
	if (len + XATTR_SECURITY_PREFIX_LEN > HFSPLUS_ATTR_MAX_STRLEN)
		return -EOPNOTSUPP;

	strcpy(xattr_name, XATTR_SECURITY_PREFIX);
	strcpy(xattr_name + XATTR_SECURITY_PREFIX_LEN, name);

	return hfsplus_getxattr(dentry, xattr_name, buffer, size);
=======
	xattr_name = kmalloc(NLS_MAX_CHARSET_SIZE * HFSPLUS_ATTR_MAX_STRLEN + 1,
		GFP_KERNEL);
	if (!xattr_name)
		return -ENOMEM;
	strcpy(xattr_name, XATTR_SECURITY_PREFIX);
	strcpy(xattr_name + XATTR_SECURITY_PREFIX_LEN, name);

	res = hfsplus_getxattr(dentry, xattr_name, buffer, size);
	kfree(xattr_name);
	return res;
>>>>>>> v3.18
}

static int hfsplus_security_setxattr(struct dentry *dentry, const char *name,
		const void *buffer, size_t size, int flags, int type)
{
<<<<<<< HEAD
	char xattr_name[HFSPLUS_ATTR_MAX_STRLEN + 1] = {0};
	size_t len = strlen(name);
=======
	char *xattr_name;
	int res;
>>>>>>> v3.18

	if (!strcmp(name, ""))
		return -EINVAL;

<<<<<<< HEAD
	if (len + XATTR_SECURITY_PREFIX_LEN > HFSPLUS_ATTR_MAX_STRLEN)
		return -EOPNOTSUPP;

	strcpy(xattr_name, XATTR_SECURITY_PREFIX);
	strcpy(xattr_name + XATTR_SECURITY_PREFIX_LEN, name);

	return hfsplus_setxattr(dentry, xattr_name, buffer, size, flags);
=======
	xattr_name = kmalloc(NLS_MAX_CHARSET_SIZE * HFSPLUS_ATTR_MAX_STRLEN + 1,
		GFP_KERNEL);
	if (!xattr_name)
		return -ENOMEM;
	strcpy(xattr_name, XATTR_SECURITY_PREFIX);
	strcpy(xattr_name + XATTR_SECURITY_PREFIX_LEN, name);

	res = hfsplus_setxattr(dentry, xattr_name, buffer, size, flags);
	kfree(xattr_name);
	return res;
>>>>>>> v3.18
}

static size_t hfsplus_security_listxattr(struct dentry *dentry, char *list,
		size_t list_size, const char *name, size_t name_len, int type)
{
	/*
	 * This method is not used.
	 * It is used hfsplus_listxattr() instead of generic_listxattr().
	 */
	return -EOPNOTSUPP;
}

static int hfsplus_initxattrs(struct inode *inode,
				const struct xattr *xattr_array,
				void *fs_info)
{
	const struct xattr *xattr;
<<<<<<< HEAD
	char xattr_name[HFSPLUS_ATTR_MAX_STRLEN + 1] = {0};
	size_t xattr_name_len;
	int err = 0;

	for (xattr = xattr_array; xattr->name != NULL; xattr++) {
		xattr_name_len = strlen(xattr->name);

		if (xattr_name_len == 0)
			continue;

		if (xattr_name_len + XATTR_SECURITY_PREFIX_LEN >
				HFSPLUS_ATTR_MAX_STRLEN)
			return -EOPNOTSUPP;

=======
	char *xattr_name;
	int err = 0;

	xattr_name = kmalloc(NLS_MAX_CHARSET_SIZE * HFSPLUS_ATTR_MAX_STRLEN + 1,
		GFP_KERNEL);
	if (!xattr_name)
		return -ENOMEM;
	for (xattr = xattr_array; xattr->name != NULL; xattr++) {

		if (!strcmp(xattr->name, ""))
			continue;

>>>>>>> v3.18
		strcpy(xattr_name, XATTR_SECURITY_PREFIX);
		strcpy(xattr_name +
			XATTR_SECURITY_PREFIX_LEN, xattr->name);
		memset(xattr_name +
<<<<<<< HEAD
			XATTR_SECURITY_PREFIX_LEN + xattr_name_len, 0, 1);
=======
			XATTR_SECURITY_PREFIX_LEN + strlen(xattr->name), 0, 1);
>>>>>>> v3.18

		err = __hfsplus_setxattr(inode, xattr_name,
					xattr->value, xattr->value_len, 0);
		if (err)
			break;
	}
<<<<<<< HEAD
=======
	kfree(xattr_name);
>>>>>>> v3.18
	return err;
}

int hfsplus_init_security(struct inode *inode, struct inode *dir,
				const struct qstr *qstr)
{
	return security_inode_init_security(inode, dir, qstr,
					&hfsplus_initxattrs, NULL);
}

<<<<<<< HEAD
=======
int hfsplus_init_inode_security(struct inode *inode,
						struct inode *dir,
						const struct qstr *qstr)
{
	int err;

	err = hfsplus_init_posix_acl(inode, dir);
	if (!err)
		err = hfsplus_init_security(inode, dir, qstr);
	return err;
}

>>>>>>> v3.18
const struct xattr_handler hfsplus_xattr_security_handler = {
	.prefix	= XATTR_SECURITY_PREFIX,
	.list	= hfsplus_security_listxattr,
	.get	= hfsplus_security_getxattr,
	.set	= hfsplus_security_setxattr,
};
