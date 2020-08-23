/*
 * Strings for the various x86 power flags
 *
 * This file must not contain any executable code.
 */

#include <asm/cpufeature.h>

const char *const x86_power_flags[32] = {
	"ts",	/* temperature sensor */
	"fid",  /* frequency id control */
	"vid",  /* voltage id control */
	"ttp",  /* thermal trip */
<<<<<<< HEAD
	"tm",
	"stc",
	"100mhzsteps",
	"hwpstate",
=======
	"tm",	/* hardware thermal control */
	"stc",	/* software thermal control */
	"100mhzsteps", /* 100 MHz multiplier control */
	"hwpstate", /* hardware P-state control */
>>>>>>> v3.18
	"",	/* tsc invariant mapped to constant_tsc */
	"cpb",  /* core performance boost */
	"eff_freq_ro", /* Readonly aperf/mperf */
};
