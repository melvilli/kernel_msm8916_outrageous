dtb-y	+= skeleton.dtb
<<<<<<< HEAD
<<<<<<< HEAD

# Built-in dtb
builtindtb-y				:= skeleton
=======
=======
>>>>>>> v3.18
dtb-y	+= tz1090_generic.dtb

# Built-in dtb
builtindtb-y				:= skeleton
builtindtb-$(CONFIG_SOC_TZ1090)		:= tz1090_generic
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

ifneq ($(CONFIG_METAG_BUILTIN_DTB_NAME),"")
	builtindtb-y			:= $(patsubst "%",%,$(CONFIG_METAG_BUILTIN_DTB_NAME))
endif

dtb-$(CONFIG_METAG_BUILTIN_DTB)	+= $(builtindtb-y).dtb
obj-$(CONFIG_METAG_BUILTIN_DTB)	+= $(builtindtb-y).dtb.o

targets	+= dtbs
targets	+= $(dtb-y)

.SECONDARY: $(obj)/$(builtindtb-y).dtb.S

dtbs: $(addprefix $(obj)/, $(dtb-y))

clean-files += *.dtb *.dtb.S
