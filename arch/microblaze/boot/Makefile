#
# arch/microblaze/boot/Makefile
#

<<<<<<< HEAD
targets := linux.bin linux.bin.gz simpleImage.%
=======
targets := linux.bin linux.bin.gz linux.bin.ub simpleImage.%
>>>>>>> v3.18

OBJCOPYFLAGS := -R .note -R .comment -R .note.gnu.build-id -O binary

$(obj)/linux.bin: vmlinux FORCE
	$(call if_changed,objcopy)
<<<<<<< HEAD
=======
	@echo 'Kernel: $@ is ready' ' (#'`cat .version`')'

$(obj)/linux.bin.ub: $(obj)/linux.bin FORCE
>>>>>>> v3.18
	$(call if_changed,uimage)
	@echo 'Kernel: $@ is ready' ' (#'`cat .version`')'

$(obj)/linux.bin.gz: $(obj)/linux.bin FORCE
	$(call if_changed,gzip)
	@echo 'Kernel: $@ is ready' ' (#'`cat .version`')'

quiet_cmd_cp = CP      $< $@$2
	cmd_cp = cat $< >$@$2 || (rm -f $@ && echo false)

quiet_cmd_strip = STRIP   $@
	cmd_strip = $(STRIP) -K microblaze_start -K _end -K __log_buf \
				-K _fdt_start vmlinux -o $@

<<<<<<< HEAD
UIMAGE_IN = $@
UIMAGE_OUT = $@.ub
=======
>>>>>>> v3.18
UIMAGE_LOADADDR = $(CONFIG_KERNEL_BASE_ADDR)

$(obj)/simpleImage.%: vmlinux FORCE
	$(call if_changed,cp,.unstrip)
	$(call if_changed,objcopy)
	$(call if_changed,uimage)
	$(call if_changed,strip)
	@echo 'Kernel: $@ is ready' ' (#'`cat .version`')'


clean-files += simpleImage.*.unstrip linux.bin.ub
