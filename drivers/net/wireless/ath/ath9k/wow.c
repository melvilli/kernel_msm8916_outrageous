/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2012 Qualcomm Atheros, Inc.
=======
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
>>>>>>> v3.18
=======
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
>>>>>>> v3.18
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/export.h>
#include "ath9k.h"
#include "reg.h"
#include "hw-ops.h"

const char *ath9k_hw_wow_event_to_string(u32 wow_event)
{
	if (wow_event & AH_WOW_MAGIC_PATTERN_EN)
		return "Magic pattern";
	if (wow_event & AH_WOW_USER_PATTERN_EN)
		return "User pattern";
	if (wow_event & AH_WOW_LINK_CHANGE)
		return "Link change";
	if (wow_event & AH_WOW_BEACON_MISS)
		return "Beacon miss";

	return  "unknown reason";
}
EXPORT_SYMBOL(ath9k_hw_wow_event_to_string);

static void ath9k_hw_config_serdes_wow_sleep(struct ath_hw *ah)
{
	int i;

	for (i = 0; i < ah->iniPcieSerdesWow.ia_rows; i++)
		REG_WRITE(ah, INI_RA(&ah->iniPcieSerdesWow, i, 0),
			  INI_RA(&ah->iniPcieSerdesWow, i, 1));

	usleep_range(1000, 1500);
}

static void ath9k_hw_set_powermode_wow_sleep(struct ath_hw *ah)
{
	struct ath_common *common = ath9k_hw_common(ah);

	REG_SET_BIT(ah, AR_STA_ID1, AR_STA_ID1_PWR_SAV);

	/* set rx disable bit */
	REG_WRITE(ah, AR_CR, AR_CR_RXD);

	if (!ath9k_hw_wait(ah, AR_CR, AR_CR_RXE, 0, AH_WAIT_TIMEOUT)) {
		ath_err(common, "Failed to stop Rx DMA in 10ms AR_CR=0x%08x AR_DIAG_SW=0x%08x\n",
			REG_READ(ah, AR_CR), REG_READ(ah, AR_DIAG_SW));
		return;
	} else {
		if (!AR_SREV_9300_20_OR_LATER(ah))
			REG_WRITE(ah, AR_RXDP, 0x0);
	}

	/* AR9280 WoW has sleep issue, do not set it to sleep */
	if (AR_SREV_9280_20(ah))
		return;

	REG_WRITE(ah, AR_RTC_FORCE_WAKE, AR_RTC_FORCE_WAKE_ON_INT);
}

static void ath9k_wow_create_keep_alive_pattern(struct ath_hw *ah)
{
	struct ath_common *common = ath9k_hw_common(ah);
	u8 sta_mac_addr[ETH_ALEN], ap_mac_addr[ETH_ALEN];
	u32 ctl[13] = {0};
	u32 data_word[KAL_NUM_DATA_WORDS];
	u8 i;
	u32 wow_ka_data_word0;

	memcpy(sta_mac_addr, common->macaddr, ETH_ALEN);
	memcpy(ap_mac_addr, common->curbssid, ETH_ALEN);

	/* set the transmit buffer */
	ctl[0] = (KAL_FRAME_LEN | (MAX_RATE_POWER << 16));

	if (!(AR_SREV_9300_20_OR_LATER(ah)))
		ctl[0] += (KAL_ANTENNA_MODE << 25);

	ctl[1] = 0;
	ctl[3] = 0xb;	/* OFDM_6M hardware value for this rate */
	ctl[4] = 0;
	ctl[7] = (ah->txchainmask) << 2;

	if (AR_SREV_9300_20_OR_LATER(ah))
		ctl[2] = 0xf << 16; /* tx_tries 0 */
	else
		ctl[2] = 0x7 << 16; /* tx_tries 0 */


	for (i = 0; i < KAL_NUM_DESC_WORDS; i++)
		REG_WRITE(ah, (AR_WOW_KA_DESC_WORD2 + i * 4), ctl[i]);

	/* for AR9300 family 13 descriptor words */
	if (AR_SREV_9300_20_OR_LATER(ah))
		REG_WRITE(ah, (AR_WOW_KA_DESC_WORD2 + i * 4), ctl[i]);

	data_word[0] = (KAL_FRAME_TYPE << 2) | (KAL_FRAME_SUB_TYPE << 4) |
		       (KAL_TO_DS << 8) | (KAL_DURATION_ID << 16);
	data_word[1] = (ap_mac_addr[3] << 24) | (ap_mac_addr[2] << 16) |
		       (ap_mac_addr[1] << 8) | (ap_mac_addr[0]);
	data_word[2] = (sta_mac_addr[1] << 24) | (sta_mac_addr[0] << 16) |
		       (ap_mac_addr[5] << 8) | (ap_mac_addr[4]);
	data_word[3] = (sta_mac_addr[5] << 24) | (sta_mac_addr[4] << 16) |
		       (sta_mac_addr[3] << 8) | (sta_mac_addr[2]);
	data_word[4] = (ap_mac_addr[3] << 24) | (ap_mac_addr[2] << 16) |
		       (ap_mac_addr[1] << 8) | (ap_mac_addr[0]);
	data_word[5] = (ap_mac_addr[5] << 8) | (ap_mac_addr[4]);

	if (AR_SREV_9462_20(ah)) {
		/* AR9462 2.0 has an extra descriptor word (time based
		 * discard) compared to other chips */
		REG_WRITE(ah, (AR_WOW_KA_DESC_WORD2 + (12 * 4)), 0);
		wow_ka_data_word0 = AR_WOW_TXBUF(13);
	} else {
		wow_ka_data_word0 = AR_WOW_TXBUF(12);
	}

	for (i = 0; i < KAL_NUM_DATA_WORDS; i++)
		REG_WRITE(ah, (wow_ka_data_word0 + i*4), data_word[i]);

}

void ath9k_hw_wow_apply_pattern(struct ath_hw *ah, u8 *user_pattern,
				u8 *user_mask, int pattern_count,
				int pattern_len)
{
	int i;
	u32 pattern_val, mask_val;
	u32 set, clr;

	/* FIXME: should check count by querying the hardware capability */
	if (pattern_count >= MAX_NUM_PATTERN)
		return;

	REG_SET_BIT(ah, AR_WOW_PATTERN, BIT(pattern_count));

	/* set the registers for pattern */
	for (i = 0; i < MAX_PATTERN_SIZE; i += 4) {
		memcpy(&pattern_val, user_pattern, 4);
		REG_WRITE(ah, (AR_WOW_TB_PATTERN(pattern_count) + i),
			  pattern_val);
		user_pattern += 4;
	}

	/* set the registers for mask */
	for (i = 0; i < MAX_PATTERN_MASK_SIZE; i += 4) {
		memcpy(&mask_val, user_mask, 4);
		REG_WRITE(ah, (AR_WOW_TB_MASK(pattern_count) + i), mask_val);
		user_mask += 4;
	}

	/* set the pattern length to be matched
	 *
	 * AR_WOW_LENGTH1_REG1
	 * bit 31:24 pattern 0 length
	 * bit 23:16 pattern 1 length
	 * bit 15:8 pattern 2 length
	 * bit 7:0 pattern 3 length
	 *
	 * AR_WOW_LENGTH1_REG2
	 * bit 31:24 pattern 4 length
	 * bit 23:16 pattern 5 length
	 * bit 15:8 pattern 6 length
	 * bit 7:0 pattern 7 length
	 *
	 * the below logic writes out the new
	 * pattern length for the corresponding
	 * pattern_count, while masking out the
	 * other fields
	 */

	ah->wow_event_mask |= BIT(pattern_count + AR_WOW_PAT_FOUND_SHIFT);

	if (!AR_SREV_9285_12_OR_LATER(ah))
		return;

	if (pattern_count < 4) {
		/* Pattern 0-3 uses AR_WOW_LENGTH1 register */
		set = (pattern_len & AR_WOW_LENGTH_MAX) <<
		       AR_WOW_LEN1_SHIFT(pattern_count);
		clr = AR_WOW_LENGTH1_MASK(pattern_count);
		REG_RMW(ah, AR_WOW_LENGTH1, set, clr);
	} else {
		/* Pattern 4-7 uses AR_WOW_LENGTH2 register */
		set = (pattern_len & AR_WOW_LENGTH_MAX) <<
		       AR_WOW_LEN2_SHIFT(pattern_count);
		clr = AR_WOW_LENGTH2_MASK(pattern_count);
		REG_RMW(ah, AR_WOW_LENGTH2, set, clr);
	}

}
EXPORT_SYMBOL(ath9k_hw_wow_apply_pattern);

u32 ath9k_hw_wow_wakeup(struct ath_hw *ah)
{
	u32 wow_status = 0;
	u32 val = 0, rval;
	/*
	 * read the WoW status register to know
	 * the wakeup reason
	 */
	rval = REG_READ(ah, AR_WOW_PATTERN);
	val = AR_WOW_STATUS(rval);

	/*
	 * mask only the WoW events that we have enabled. Sometimes
	 * we have spurious WoW events from the AR_WOW_PATTERN
	 * register. This mask will clean it up.
	 */

	val &= ah->wow_event_mask;

	if (val) {

		if (val & AR_WOW_MAGIC_PAT_FOUND)
			wow_status |= AH_WOW_MAGIC_PATTERN_EN;

		if (AR_WOW_PATTERN_FOUND(val))
			wow_status |= AH_WOW_USER_PATTERN_EN;

		if (val & AR_WOW_KEEP_ALIVE_FAIL)
			wow_status |= AH_WOW_LINK_CHANGE;

		if (val & AR_WOW_BEACON_FAIL)
			wow_status |= AH_WOW_BEACON_MISS;

	}

	/*
	 * set and clear WOW_PME_CLEAR registers for the chip to
	 * generate next wow signal.
	 * disable D3 before accessing other registers ?
	 */

	/* do we need to check the bit value 0x01000000 (7-10) ?? */
	REG_RMW(ah, AR_PCIE_PM_CTRL, AR_PMCTRL_WOW_PME_CLR,
		AR_PMCTRL_PWR_STATE_D1D3);

	/*
	 * clear all events
	 */
	REG_WRITE(ah, AR_WOW_PATTERN,
		  AR_WOW_CLEAR_EVENTS(REG_READ(ah, AR_WOW_PATTERN)));

	/*
	 * tie reset register for AR9002 family of chipsets
	 * NB: not tieing it back might have some repurcussions.
	 */

	if (!AR_SREV_9300_20_OR_LATER(ah)) {
		REG_SET_BIT(ah, AR_WA, AR_WA_UNTIE_RESET_EN |
			    AR_WA_POR_SHORT | AR_WA_RESET_EN);
	}


	/*
	 * restore the beacon threshold to init value
	 */
	REG_WRITE(ah, AR_RSSI_THR, INIT_RSSI_THR);

	/*
	 * Restore the way the PCI-E reset, Power-On-Reset, external
	 * PCIE_POR_SHORT pins are tied to its original value.
	 * Previously just before WoW sleep, we untie the PCI-E
	 * reset to our Chip's Power On Reset so that any PCI-E
	 * reset from the bus will not reset our chip
	 */

	if (AR_SREV_9280_20_OR_LATER(ah) && ah->is_pciexpress)
		ath9k_hw_configpcipowersave(ah, false);

	ah->wow_event_mask = 0;

	return wow_status;
}
EXPORT_SYMBOL(ath9k_hw_wow_wakeup);

void ath9k_hw_wow_enable(struct ath_hw *ah, u32 pattern_enable)
{
	u32 wow_event_mask;
	u32 set, clr;

	/*
	 * wow_event_mask is a mask to the AR_WOW_PATTERN register to
	 * indicate which WoW events we have enabled. The WoW events
	 * are from the 'pattern_enable' in this function and
	 * 'pattern_count' of ath9k_hw_wow_apply_pattern()
	 */

	wow_event_mask = ah->wow_event_mask;

	/*
	 * Untie Power-on-Reset from the PCI-E-Reset. When we are in
	 * WOW sleep, we do want the Reset from the PCI-E to disturb
	 * our hw state
	 */

	if (ah->is_pciexpress) {

		/*
		 * we need to untie the internal POR (power-on-reset)
		 * to the external PCI-E reset. We also need to tie
		 * the PCI-E Phy reset to the PCI-E reset.
		 */

		if (AR_SREV_9300_20_OR_LATER(ah)) {
			set = AR_WA_RESET_EN | AR_WA_POR_SHORT;
			clr = AR_WA_UNTIE_RESET_EN | AR_WA_D3_L1_DISABLE;
			REG_RMW(ah, AR_WA, set, clr);
		} else {
			if (AR_SREV_9285(ah) || AR_SREV_9287(ah))
				set = AR9285_WA_DEFAULT;
			else
				set = AR9280_WA_DEFAULT;

			/*
			 * In AR9280 and AR9285, bit 14 in WA register
			 * (disable L1) should only be set when device
			 * enters D3 state and be cleared when device
			 * comes back to D0
			 */

			if (ah->config.pcie_waen & AR_WA_D3_L1_DISABLE)
				set |= AR_WA_D3_L1_DISABLE;

			clr = AR_WA_UNTIE_RESET_EN;
			set |= AR_WA_RESET_EN | AR_WA_POR_SHORT;
			REG_RMW(ah, AR_WA, set, clr);

			/*
			 * for WoW sleep, we reprogram the SerDes so that the
			 * PLL and CLK REQ are both enabled. This uses more
			 * power but otherwise WoW sleep is unstable and the
			 * chip may disappear.
			 */

			if (AR_SREV_9285_12_OR_LATER(ah))
				ath9k_hw_config_serdes_wow_sleep(ah);

		}
	}

	/*
	 * set the power states appropriately and enable PME
	 */
	set = AR_PMCTRL_HOST_PME_EN | AR_PMCTRL_PWR_PM_CTRL_ENA |
	      AR_PMCTRL_AUX_PWR_DET | AR_PMCTRL_WOW_PME_CLR;

	/*
	 * set and clear WOW_PME_CLEAR registers for the chip
	 * to generate next wow signal.
	 */
	REG_SET_BIT(ah, AR_PCIE_PM_CTRL, set);
	clr = AR_PMCTRL_WOW_PME_CLR;
	REG_CLR_BIT(ah, AR_PCIE_PM_CTRL, clr);

	/*
	 * Setup for:
	 *	- beacon misses
	 *	- magic pattern
	 *	- keep alive timeout
	 *	- pattern matching
	 */

	/*
	 * Program default values for pattern backoff, aifs/slot/KAL count,
	 * beacon miss timeout, KAL timeout, etc.
	 */

	set = AR_WOW_BACK_OFF_SHIFT(AR_WOW_PAT_BACKOFF);
	REG_SET_BIT(ah, AR_WOW_PATTERN, set);

	set = AR_WOW_AIFS_CNT(AR_WOW_CNT_AIFS_CNT) |
	      AR_WOW_SLOT_CNT(AR_WOW_CNT_SLOT_CNT) |
	      AR_WOW_KEEP_ALIVE_CNT(AR_WOW_CNT_KA_CNT);
	REG_SET_BIT(ah, AR_WOW_COUNT, set);

	if (pattern_enable & AH_WOW_BEACON_MISS)
		set = AR_WOW_BEACON_TIMO;
	/* We are not using beacon miss, program a large value */
	else
		set = AR_WOW_BEACON_TIMO_MAX;

	REG_WRITE(ah, AR_WOW_BCN_TIMO, set);

	/*
	 * Keep alive timo in ms except AR9280
	 */
	if (!pattern_enable || AR_SREV_9280(ah))
		set = AR_WOW_KEEP_ALIVE_NEVER;
	else
		set = KAL_TIMEOUT * 32;

	REG_WRITE(ah, AR_WOW_KEEP_ALIVE_TIMO, set);

	/*
	 * Keep alive delay in us. based on 'power on clock',
	 * therefore in usec
	 */
	set = KAL_DELAY * 1000;
	REG_WRITE(ah, AR_WOW_KEEP_ALIVE_DELAY, set);

	/*
	 * Create keep alive pattern to respond to beacons
	 */
	ath9k_wow_create_keep_alive_pattern(ah);

	/*
	 * Configure MAC WoW Registers
	 */

	set = 0;
	/* Send keep alive timeouts anyway */
	clr = AR_WOW_KEEP_ALIVE_AUTO_DIS;

	if (pattern_enable & AH_WOW_LINK_CHANGE)
		wow_event_mask |= AR_WOW_KEEP_ALIVE_FAIL;
	else
		set = AR_WOW_KEEP_ALIVE_FAIL_DIS;

	/*
	 * FIXME: For now disable keep alive frame
	 * failure. This seems to sometimes trigger
	 * unnecessary wake up with AR9485 chipsets.
	 */
	set = AR_WOW_KEEP_ALIVE_FAIL_DIS;

	REG_RMW(ah, AR_WOW_KEEP_ALIVE, set, clr);


	/*
	 * we are relying on a bmiss failure. ensure we have
	 * enough threshold to prevent false positives
	 */
	REG_RMW_FIELD(ah, AR_RSSI_THR, AR_RSSI_THR_BM_THR,
		      AR_WOW_BMISSTHRESHOLD);

	set = 0;
	clr = 0;

	if (pattern_enable & AH_WOW_BEACON_MISS) {
		set = AR_WOW_BEACON_FAIL_EN;
		wow_event_mask |= AR_WOW_BEACON_FAIL;
	} else {
		clr = AR_WOW_BEACON_FAIL_EN;
	}

	REG_RMW(ah, AR_WOW_BCN_EN, set, clr);

	set = 0;
	clr = 0;
	/*
	 * Enable the magic packet registers
	 */
	if (pattern_enable & AH_WOW_MAGIC_PATTERN_EN) {
		set = AR_WOW_MAGIC_EN;
		wow_event_mask |= AR_WOW_MAGIC_PAT_FOUND;
	} else {
		clr = AR_WOW_MAGIC_EN;
	}
	set |= AR_WOW_MAC_INTR_EN;
	REG_RMW(ah, AR_WOW_PATTERN, set, clr);

	/*
	 * For AR9285 and later version of chipsets
	 * enable WoW pattern match for packets less
	 * than 256 bytes for all patterns
	 */
	if (AR_SREV_9285_12_OR_LATER(ah))
		REG_WRITE(ah, AR_WOW_PATTERN_MATCH_LT_256B,
			  AR_WOW_PATTERN_SUPPORTED);

	/*
	 * Set the power states appropriately and enable PME
	 */
	clr = 0;
	set = AR_PMCTRL_PWR_STATE_D1D3 | AR_PMCTRL_HOST_PME_EN |
	      AR_PMCTRL_PWR_PM_CTRL_ENA;
	/*
	 * This is needed for AR9300 chipsets to wake-up
	 * the host.
	 */
	if (AR_SREV_9300_20_OR_LATER(ah))
		clr = AR_PCIE_PM_CTRL_ENA;

	REG_RMW(ah, AR_PCIE_PM_CTRL, set, clr);

	if (AR_SREV_9462(ah) || AR_SREV_9565(ah)) {
		/*
		 * this is needed to prevent the chip waking up
		 * the host within 3-4 seconds with certain
		 * platform/BIOS. The fix is to enable
		 * D1 & D3 to match original definition and
		 * also match the OTP value. Anyway this
		 * is more related to SW WOW.
		 */
		clr = AR_PMCTRL_PWR_STATE_D1D3;
		REG_CLR_BIT(ah, AR_PCIE_PM_CTRL, clr);

		set = AR_PMCTRL_PWR_STATE_D1D3_REAL;
		REG_SET_BIT(ah, AR_PCIE_PM_CTRL, set);
	}



	REG_CLR_BIT(ah, AR_STA_ID1, AR_STA_ID1_PRESERVE_SEQNUM);

	if (AR_SREV_9300_20_OR_LATER(ah)) {
		/* to bring down WOW power low margin */
		set = BIT(13);
		REG_SET_BIT(ah, AR_PCIE_PHY_REG3, set);
		/* HW WoW */
		clr = BIT(5);
		REG_CLR_BIT(ah, AR_PCU_MISC_MODE3, clr);
	}

	ath9k_hw_set_powermode_wow_sleep(ah);
	ah->wow_event_mask = wow_event_mask;
}
EXPORT_SYMBOL(ath9k_hw_wow_enable);
=======
=======
>>>>>>> v3.18
#include "ath9k.h"

static const struct wiphy_wowlan_support ath9k_wowlan_support = {
	.flags = WIPHY_WOWLAN_MAGIC_PKT | WIPHY_WOWLAN_DISCONNECT,
	.n_patterns = MAX_NUM_USER_PATTERN,
	.pattern_min_len = 1,
	.pattern_max_len = MAX_PATTERN_SIZE,
};

static void ath9k_wow_map_triggers(struct ath_softc *sc,
				   struct cfg80211_wowlan *wowlan,
				   u32 *wow_triggers)
{
	if (wowlan->disconnect)
		*wow_triggers |= AH_WOW_LINK_CHANGE |
				 AH_WOW_BEACON_MISS;
	if (wowlan->magic_pkt)
		*wow_triggers |= AH_WOW_MAGIC_PATTERN_EN;

	if (wowlan->n_patterns)
		*wow_triggers |= AH_WOW_USER_PATTERN_EN;

	sc->wow_enabled = *wow_triggers;

}

static void ath9k_wow_add_disassoc_deauth_pattern(struct ath_softc *sc)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	int pattern_count = 0;
	int i, byte_cnt;
	u8 dis_deauth_pattern[MAX_PATTERN_SIZE];
	u8 dis_deauth_mask[MAX_PATTERN_SIZE];

	memset(dis_deauth_pattern, 0, MAX_PATTERN_SIZE);
	memset(dis_deauth_mask, 0, MAX_PATTERN_SIZE);

	/*
	 * Create Dissassociate / Deauthenticate packet filter
	 *
	 *     2 bytes        2 byte    6 bytes   6 bytes  6 bytes
	 *  +--------------+----------+---------+--------+--------+----
	 *  + Frame Control+ Duration +   DA    +  SA    +  BSSID +
	 *  +--------------+----------+---------+--------+--------+----
	 *
	 * The above is the management frame format for disassociate/
	 * deauthenticate pattern, from this we need to match the first byte
	 * of 'Frame Control' and DA, SA, and BSSID fields
	 * (skipping 2nd byte of FC and Duration feild.
	 *
	 * Disassociate pattern
	 * --------------------
	 * Frame control = 00 00 1010
	 * DA, SA, BSSID = x:x:x:x:x:x
	 * Pattern will be A0000000 | x:x:x:x:x:x | x:x:x:x:x:x
	 *			    | x:x:x:x:x:x  -- 22 bytes
	 *
	 * Deauthenticate pattern
	 * ----------------------
	 * Frame control = 00 00 1100
	 * DA, SA, BSSID = x:x:x:x:x:x
	 * Pattern will be C0000000 | x:x:x:x:x:x | x:x:x:x:x:x
	 *			    | x:x:x:x:x:x  -- 22 bytes
	 */

	/* Create Disassociate Pattern first */

	byte_cnt = 0;

	/* Fill out the mask with all FF's */

	for (i = 0; i < MAX_PATTERN_MASK_SIZE; i++)
		dis_deauth_mask[i] = 0xff;

	/* copy the first byte of frame control field */
	dis_deauth_pattern[byte_cnt] = 0xa0;
	byte_cnt++;

	/* skip 2nd byte of frame control and Duration field */
	byte_cnt += 3;

	/*
	 * need not match the destination mac address, it can be a broadcast
	 * mac address or an unicast to this station
	 */
	byte_cnt += 6;

	/* copy the source mac address */
	memcpy((dis_deauth_pattern + byte_cnt), common->curbssid, ETH_ALEN);

	byte_cnt += 6;

	/* copy the bssid, its same as the source mac address */

	memcpy((dis_deauth_pattern + byte_cnt), common->curbssid, ETH_ALEN);

	/* Create Disassociate pattern mask */

	dis_deauth_mask[0] = 0xfe;
	dis_deauth_mask[1] = 0x03;
	dis_deauth_mask[2] = 0xc0;

	ath_dbg(common, WOW, "Adding disassoc/deauth patterns for WoW\n");

	ath9k_hw_wow_apply_pattern(ah, dis_deauth_pattern, dis_deauth_mask,
				   pattern_count, byte_cnt);

	pattern_count++;
	/*
	 * for de-authenticate pattern, only the first byte of the frame
	 * control field gets changed from 0xA0 to 0xC0
	 */
	dis_deauth_pattern[0] = 0xC0;

	ath9k_hw_wow_apply_pattern(ah, dis_deauth_pattern, dis_deauth_mask,
				   pattern_count, byte_cnt);

}

static void ath9k_wow_add_pattern(struct ath_softc *sc,
				  struct cfg80211_wowlan *wowlan)
{
	struct ath_hw *ah = sc->sc_ah;
	struct ath9k_wow_pattern *wow_pattern = NULL;
	struct cfg80211_pkt_pattern *patterns = wowlan->patterns;
	int mask_len;
	s8 i = 0;

	if (!wowlan->n_patterns)
		return;

	/*
	 * Add the new user configured patterns
	 */
	for (i = 0; i < wowlan->n_patterns; i++) {

		wow_pattern = kzalloc(sizeof(*wow_pattern), GFP_KERNEL);

		if (!wow_pattern)
			return;

		/*
		 * TODO: convert the generic user space pattern to
		 * appropriate chip specific/802.11 pattern.
		 */

		mask_len = DIV_ROUND_UP(wowlan->patterns[i].pattern_len, 8);
		memset(wow_pattern->pattern_bytes, 0, MAX_PATTERN_SIZE);
		memset(wow_pattern->mask_bytes, 0, MAX_PATTERN_SIZE);
		memcpy(wow_pattern->pattern_bytes, patterns[i].pattern,
		       patterns[i].pattern_len);
		memcpy(wow_pattern->mask_bytes, patterns[i].mask, mask_len);
		wow_pattern->pattern_len = patterns[i].pattern_len;

		/*
		 * just need to take care of deauth and disssoc pattern,
		 * make sure we don't overwrite them.
		 */

		ath9k_hw_wow_apply_pattern(ah, wow_pattern->pattern_bytes,
					   wow_pattern->mask_bytes,
					   i + 2,
					   wow_pattern->pattern_len);
		kfree(wow_pattern);

	}

}

int ath9k_suspend(struct ieee80211_hw *hw,
		  struct cfg80211_wowlan *wowlan)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	u32 wow_triggers_enabled = 0;
	int ret = 0;

	ath9k_deinit_channel_context(sc);

	mutex_lock(&sc->mutex);

	ath_cancel_work(sc);
	ath_stop_ani(sc);

	if (test_bit(ATH_OP_INVALID, &common->op_flags)) {
		ath_dbg(common, ANY, "Device not present\n");
		ret = -EINVAL;
		goto fail_wow;
	}

	if (WARN_ON(!wowlan)) {
		ath_dbg(common, WOW, "None of the WoW triggers enabled\n");
		ret = -EINVAL;
		goto fail_wow;
	}

	if (!device_can_wakeup(sc->dev)) {
		ath_dbg(common, WOW, "device_can_wakeup failed, WoW is not enabled\n");
		ret = 1;
		goto fail_wow;
	}

	/*
	 * none of the sta vifs are associated
	 * and we are not currently handling multivif
	 * cases, for instance we have to seperately
	 * configure 'keep alive frame' for each
	 * STA.
	 */

	if (!test_bit(ATH_OP_PRIM_STA_VIF, &common->op_flags)) {
		ath_dbg(common, WOW, "None of the STA vifs are associated\n");
		ret = 1;
		goto fail_wow;
	}

	if (sc->cur_chan->nvifs > 1) {
		ath_dbg(common, WOW, "WoW for multivif is not yet supported\n");
		ret = 1;
		goto fail_wow;
	}

	ath9k_wow_map_triggers(sc, wowlan, &wow_triggers_enabled);

	ath_dbg(common, WOW, "WoW triggers enabled 0x%x\n",
		wow_triggers_enabled);

	ath9k_ps_wakeup(sc);

	ath9k_stop_btcoex(sc);

	/*
	 * Enable wake up on recieving disassoc/deauth
	 * frame by default.
	 */
	ath9k_wow_add_disassoc_deauth_pattern(sc);

	if (wow_triggers_enabled & AH_WOW_USER_PATTERN_EN)
		ath9k_wow_add_pattern(sc, wowlan);

	spin_lock_bh(&sc->sc_pcu_lock);
	/*
	 * To avoid false wake, we enable beacon miss interrupt only
	 * when we go to sleep. We save the current interrupt mask
	 * so we can restore it after the system wakes up
	 */
	sc->wow_intr_before_sleep = ah->imask;
	ah->imask &= ~ATH9K_INT_GLOBAL;
	ath9k_hw_disable_interrupts(ah);
	ah->imask = ATH9K_INT_BMISS | ATH9K_INT_GLOBAL;
	ath9k_hw_set_interrupts(ah);
	ath9k_hw_enable_interrupts(ah);

	spin_unlock_bh(&sc->sc_pcu_lock);

	/*
	 * we can now sync irq and kill any running tasklets, since we already
	 * disabled interrupts and not holding a spin lock
	 */
	synchronize_irq(sc->irq);
	tasklet_kill(&sc->intr_tq);

	ath9k_hw_wow_enable(ah, wow_triggers_enabled);

	ath9k_ps_restore(sc);
	ath_dbg(common, ANY, "WoW enabled in ath9k\n");
	atomic_inc(&sc->wow_sleep_proc_intr);

fail_wow:
	mutex_unlock(&sc->mutex);
	return ret;
}

int ath9k_resume(struct ieee80211_hw *hw)
{
	struct ath_softc *sc = hw->priv;
	struct ath_hw *ah = sc->sc_ah;
	struct ath_common *common = ath9k_hw_common(ah);
	u32 wow_status;

	mutex_lock(&sc->mutex);

	ath9k_ps_wakeup(sc);

	spin_lock_bh(&sc->sc_pcu_lock);

	ath9k_hw_disable_interrupts(ah);
	ah->imask = sc->wow_intr_before_sleep;
	ath9k_hw_set_interrupts(ah);
	ath9k_hw_enable_interrupts(ah);

	spin_unlock_bh(&sc->sc_pcu_lock);

	wow_status = ath9k_hw_wow_wakeup(ah);

	if (atomic_read(&sc->wow_got_bmiss_intr) == 0) {
		/*
		 * some devices may not pick beacon miss
		 * as the reason they woke up so we add
		 * that here for that shortcoming.
		 */
		wow_status |= AH_WOW_BEACON_MISS;
		atomic_dec(&sc->wow_got_bmiss_intr);
		ath_dbg(common, ANY, "Beacon miss interrupt picked up during WoW sleep\n");
	}

	atomic_dec(&sc->wow_sleep_proc_intr);

	if (wow_status) {
		ath_dbg(common, ANY, "Waking up due to WoW triggers %s with WoW status = %x\n",
			ath9k_hw_wow_event_to_string(wow_status), wow_status);
	}

	ath_restart_work(sc);
	ath9k_start_btcoex(sc);

	ath9k_ps_restore(sc);
	mutex_unlock(&sc->mutex);

	return 0;
}

void ath9k_set_wakeup(struct ieee80211_hw *hw, bool enabled)
{
	struct ath_softc *sc = hw->priv;

	mutex_lock(&sc->mutex);
	device_init_wakeup(sc->dev, 1);
	device_set_wakeup_enable(sc->dev, enabled);
	mutex_unlock(&sc->mutex);
}

void ath9k_init_wow(struct ieee80211_hw *hw)
{
	struct ath_softc *sc = hw->priv;

	if ((sc->sc_ah->caps.hw_caps & ATH9K_HW_WOW_DEVICE_CAPABLE) &&
	    (sc->driver_data & ATH9K_PCI_WOW) &&
	    device_can_wakeup(sc->dev))
		hw->wiphy->wowlan = &ath9k_wowlan_support;

	atomic_set(&sc->wow_sleep_proc_intr, -1);
	atomic_set(&sc->wow_got_bmiss_intr, -1);
}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
