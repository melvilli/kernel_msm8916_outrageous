obj-$(CONFIG_IWLMVM)   += iwlmvm.o
iwlmvm-y += fw.o mac80211.o nvm.o ops.o phy-ctxt.o mac-ctxt.o
<<<<<<< HEAD
<<<<<<< HEAD
iwlmvm-y += utils.o rx.o tx.o binding.o quota.o sta.o
iwlmvm-y += scan.o time-event.o rs.o
iwlmvm-y += power.o bt-coex.o
iwlmvm-y += led.o
iwlmvm-$(CONFIG_IWLWIFI_DEBUGFS) += debugfs.o
=======
=======
>>>>>>> v3.18
iwlmvm-y += utils.o rx.o tx.o binding.o quota.o sta.o sf.o
iwlmvm-y += scan.o time-event.o rs.o
iwlmvm-y += power.o coex.o coex_legacy.o
iwlmvm-y += tt.o offloading.o tdls.o
iwlmvm-$(CONFIG_IWLWIFI_DEBUGFS) += debugfs.o debugfs-vif.o
iwlmvm-$(CONFIG_IWLWIFI_LEDS) += led.o
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
iwlmvm-$(CONFIG_PM_SLEEP) += d3.o

ccflags-y += -D__CHECK_ENDIAN__ -I$(src)/../
