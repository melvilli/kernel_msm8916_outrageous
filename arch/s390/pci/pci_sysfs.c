/*
 * Copyright IBM Corp. 2012
 *
 * Author(s):
 *   Jan Glauber <jang@linux.vnet.ibm.com>
 */

<<<<<<< HEAD
<<<<<<< HEAD
#define COMPONENT "zPCI"
#define pr_fmt(fmt) COMPONENT ": " fmt
=======
#define KMSG_COMPONENT "zpci"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt
>>>>>>> v3.18
=======
#define KMSG_COMPONENT "zpci"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt
>>>>>>> v3.18

#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/pci.h>

<<<<<<< HEAD
<<<<<<< HEAD
static ssize_t show_fid(struct device *dev, struct device_attribute *attr,
			char *buf)
{
	struct zpci_dev *zdev = get_zdev(container_of(dev, struct pci_dev, dev));

	sprintf(buf, "0x%08x\n", zdev->fid);
	return strlen(buf);
}
static DEVICE_ATTR(function_id, S_IRUGO, show_fid, NULL);

static ssize_t show_fh(struct device *dev, struct device_attribute *attr,
		       char *buf)
{
	struct zpci_dev *zdev = get_zdev(container_of(dev, struct pci_dev, dev));

	sprintf(buf, "0x%08x\n", zdev->fh);
	return strlen(buf);
}
static DEVICE_ATTR(function_handle, S_IRUGO, show_fh, NULL);

static ssize_t show_pchid(struct device *dev, struct device_attribute *attr,
			  char *buf)
{
	struct zpci_dev *zdev = get_zdev(container_of(dev, struct pci_dev, dev));

	sprintf(buf, "0x%04x\n", zdev->pchid);
	return strlen(buf);
}
static DEVICE_ATTR(pchid, S_IRUGO, show_pchid, NULL);

static ssize_t show_pfgid(struct device *dev, struct device_attribute *attr,
			  char *buf)
{
	struct zpci_dev *zdev = get_zdev(container_of(dev, struct pci_dev, dev));

	sprintf(buf, "0x%02x\n", zdev->pfgid);
	return strlen(buf);
}
static DEVICE_ATTR(pfgid, S_IRUGO, show_pfgid, NULL);

static struct device_attribute *zpci_dev_attrs[] = {
	&dev_attr_function_id,
	&dev_attr_function_handle,
	&dev_attr_pchid,
	&dev_attr_pfgid,
	NULL,
};

int zpci_sysfs_add_device(struct device *dev)
{
	int i, rc = 0;

	for (i = 0; zpci_dev_attrs[i]; i++) {
		rc = device_create_file(dev, zpci_dev_attrs[i]);
		if (rc)
			goto error;
	}
	return 0;

error:
	while (--i >= 0)
		device_remove_file(dev, zpci_dev_attrs[i]);
	return rc;
}

void zpci_sysfs_remove_device(struct device *dev)
{
	int i;

	for (i = 0; zpci_dev_attrs[i]; i++)
		device_remove_file(dev, zpci_dev_attrs[i]);
}
=======
=======
>>>>>>> v3.18
#define zpci_attr(name, fmt, member)					\
static ssize_t name##_show(struct device *dev,				\
			   struct device_attribute *attr, char *buf)	\
{									\
	struct zpci_dev *zdev = get_zdev(to_pci_dev(dev));		\
									\
	return sprintf(buf, fmt, zdev->member);				\
}									\
static DEVICE_ATTR_RO(name)

zpci_attr(function_id, "0x%08x\n", fid);
zpci_attr(function_handle, "0x%08x\n", fh);
zpci_attr(pchid, "0x%04x\n", pchid);
zpci_attr(pfgid, "0x%02x\n", pfgid);
zpci_attr(vfn, "0x%04x\n", vfn);
zpci_attr(pft, "0x%02x\n", pft);
zpci_attr(uid, "0x%x\n", uid);
zpci_attr(segment0, "0x%02x\n", pfip[0]);
zpci_attr(segment1, "0x%02x\n", pfip[1]);
zpci_attr(segment2, "0x%02x\n", pfip[2]);
zpci_attr(segment3, "0x%02x\n", pfip[3]);

static ssize_t recover_store(struct device *dev, struct device_attribute *attr,
			     const char *buf, size_t count)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	struct zpci_dev *zdev = get_zdev(pdev);
	int ret;

	if (!device_remove_file_self(dev, attr))
		return count;

	pci_stop_and_remove_bus_device(pdev);
	ret = zpci_disable_device(zdev);
	if (ret)
		return ret;

	ret = zpci_enable_device(zdev);
	if (ret)
		return ret;

	pci_rescan_bus(zdev->bus);
	return count;
}
static DEVICE_ATTR_WO(recover);

static ssize_t util_string_read(struct file *filp, struct kobject *kobj,
				struct bin_attribute *attr, char *buf,
				loff_t off, size_t count)
{
	struct device *dev = kobj_to_dev(kobj);
	struct pci_dev *pdev = to_pci_dev(dev);
	struct zpci_dev *zdev = get_zdev(pdev);

	return memory_read_from_buffer(buf, count, &off, zdev->util_str,
				       sizeof(zdev->util_str));
}
static BIN_ATTR_RO(util_string, CLP_UTIL_STR_LEN);
static struct bin_attribute *zpci_bin_attrs[] = {
	&bin_attr_util_string,
	NULL,
};

static struct attribute *zpci_dev_attrs[] = {
	&dev_attr_function_id.attr,
	&dev_attr_function_handle.attr,
	&dev_attr_pchid.attr,
	&dev_attr_pfgid.attr,
	&dev_attr_pft.attr,
	&dev_attr_vfn.attr,
	&dev_attr_uid.attr,
	&dev_attr_recover.attr,
	NULL,
};
static struct attribute_group zpci_attr_group = {
	.attrs = zpci_dev_attrs,
	.bin_attrs = zpci_bin_attrs,
};

static struct attribute *pfip_attrs[] = {
	&dev_attr_segment0.attr,
	&dev_attr_segment1.attr,
	&dev_attr_segment2.attr,
	&dev_attr_segment3.attr,
	NULL,
};
static struct attribute_group pfip_attr_group = {
	.name = "pfip",
	.attrs = pfip_attrs,
};

const struct attribute_group *zpci_attr_groups[] = {
	&zpci_attr_group,
	&pfip_attr_group,
	NULL,
};
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
