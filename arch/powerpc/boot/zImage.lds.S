<<<<<<< HEAD
<<<<<<< HEAD
OUTPUT_ARCH(powerpc:common)
=======
=======
>>>>>>> v3.18
#include <asm-generic/vmlinux.lds.h>

#ifdef CONFIG_PPC64_BOOT_WRAPPER
OUTPUT_ARCH(powerpc:common64)
#else
OUTPUT_ARCH(powerpc:common)
#endif
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
ENTRY(_zimage_start)
EXTERN(_zimage_start)
SECTIONS
{
  .text      :
  {
    _start = .;
    *(.text)
    *(.fixup)
    _etext = .;
  }
  . = ALIGN(4096);
  .data    :
  {
    *(.rodata*)
    *(.data*)
    *(.sdata*)
<<<<<<< HEAD
<<<<<<< HEAD
    *(.got2)
=======
#ifndef CONFIG_PPC64_BOOT_WRAPPER
    *(.got2)
#endif
>>>>>>> v3.18
=======
#ifndef CONFIG_PPC64_BOOT_WRAPPER
    *(.got2)
#endif
>>>>>>> v3.18
  }
  .dynsym : { *(.dynsym) }
  .dynstr : { *(.dynstr) }
  .dynamic :
  {
    __dynamic_start = .;
    *(.dynamic)
  }
  .hash : { *(.hash) }
  .interp : { *(.interp) }
<<<<<<< HEAD
<<<<<<< HEAD
  .rela.dyn : { *(.rela*) }
=======
=======
>>>>>>> v3.18
  .rela.dyn :
  {
#ifdef CONFIG_PPC64_BOOT_WRAPPER
    __rela_dyn_start = .;
#endif
    *(.rela*)
  }
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

  . = ALIGN(8);
  .kernel:dtb :
  {
    _dtb_start = .;
    *(.kernel:dtb)
    _dtb_end = .;
  }

  . = ALIGN(4096);
  .kernel:vmlinux.strip :
  {
    _vmlinux_start =  .;
    *(.kernel:vmlinux.strip)
    _vmlinux_end =  .;
  }

  . = ALIGN(4096);
  .kernel:initrd :
  {
    _initrd_start =  .;
    *(.kernel:initrd)
    _initrd_end =  .;
  }

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
#ifdef CONFIG_PPC64_BOOT_WRAPPER
  .got :
  {
    __toc_start = .;
    *(.got)
    *(.toc)
  }
#endif

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
  . = ALIGN(4096);
  .bss       :
  {
    _edata  =  .;
    __bss_start = .;
    *(.sbss)
    *(.bss)
    *(COMMON)
    _end = . ;
  }
}
