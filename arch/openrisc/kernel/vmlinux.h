#ifndef __OPENRISC_VMLINUX_H_
#define __OPENRISC_VMLINUX_H_

<<<<<<< HEAD
<<<<<<< HEAD
extern char _stext, _etext, _edata, _end;
#ifdef CONFIG_BLK_DEV_INITRD
extern char __initrd_start, __initrd_end;
extern char __initramfs_start;
#endif

extern u32 __dtb_start[];

=======
#ifdef CONFIG_BLK_DEV_INITRD
extern char __initrd_start, __initrd_end;
#endif

>>>>>>> v3.18
=======
#ifdef CONFIG_BLK_DEV_INITRD
extern char __initrd_start, __initrd_end;
#endif

>>>>>>> v3.18
#endif
