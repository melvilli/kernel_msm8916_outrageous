/* ----------------------------------------------------------------------- *
 *
 *  Copyright (C) 2009 Intel Corporation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License version
 *  2 as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 *  02110-1301, USA.
 *
 *  H. Peter Anvin <hpa@linux.intel.com>
 *
 * ----------------------------------------------------------------------- */

/*
 * Compute the desired load offset from a compressed program; outputs
 * a small assembly wrapper with the appropriate symbols defined.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <tools/le_byteshift.h>

int main(int argc, char *argv[])
{
	uint32_t olen;
	long ilen;
	unsigned long offs;
<<<<<<< HEAD
<<<<<<< HEAD
	FILE *f;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s compressed_file\n", argv[0]);
		return 1;
=======
=======
>>>>>>> v3.18
	unsigned long run_size;
	FILE *f = NULL;
	int retval = 1;

	if (argc < 3) {
		fprintf(stderr, "Usage: %s compressed_file run_size\n",
				argv[0]);
		goto bail;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}

	/* Get the information for the compressed kernel image first */

	f = fopen(argv[1], "r");
	if (!f) {
		perror(argv[1]);
<<<<<<< HEAD
<<<<<<< HEAD
		return 1;
=======
		goto bail;
>>>>>>> v3.18
=======
		goto bail;
>>>>>>> v3.18
	}


	if (fseek(f, -4L, SEEK_END)) {
		perror(argv[1]);
	}

	if (fread(&olen, sizeof(olen), 1, f) != 1) {
		perror(argv[1]);
<<<<<<< HEAD
<<<<<<< HEAD
		return 1;
=======
		goto bail;
>>>>>>> v3.18
=======
		goto bail;
>>>>>>> v3.18
	}

	ilen = ftell(f);
	olen = get_unaligned_le32(&olen);
<<<<<<< HEAD
<<<<<<< HEAD
	fclose(f);
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18

	/*
	 * Now we have the input (compressed) and output (uncompressed)
	 * sizes, compute the necessary decompression offset...
	 */

	offs = (olen > ilen) ? olen - ilen : 0;
	offs += olen >> 12;	/* Add 8 bytes for each 32K block */
	offs += 64*1024 + 128;	/* Add 64K + 128 bytes slack */
	offs = (offs+4095) & ~4095; /* Round to a 4K boundary */
<<<<<<< HEAD
<<<<<<< HEAD
=======
	run_size = atoi(argv[2]);
>>>>>>> v3.18
=======
	run_size = atoi(argv[2]);
>>>>>>> v3.18

	printf(".section \".rodata..compressed\",\"a\",@progbits\n");
	printf(".globl z_input_len\n");
	printf("z_input_len = %lu\n", ilen);
	printf(".globl z_output_len\n");
	printf("z_output_len = %lu\n", (unsigned long)olen);
	printf(".globl z_extract_offset\n");
	printf("z_extract_offset = 0x%lx\n", offs);
	/* z_extract_offset_negative allows simplification of head_32.S */
	printf(".globl z_extract_offset_negative\n");
	printf("z_extract_offset_negative = -0x%lx\n", offs);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	printf(".globl z_run_size\n");
	printf("z_run_size = %lu\n", run_size);
>>>>>>> v3.18
=======
	printf(".globl z_run_size\n");
	printf("z_run_size = %lu\n", run_size);
>>>>>>> v3.18

	printf(".globl input_data, input_data_end\n");
	printf("input_data:\n");
	printf(".incbin \"%s\"\n", argv[1]);
	printf("input_data_end:\n");

<<<<<<< HEAD
<<<<<<< HEAD
	return 0;
=======
=======
>>>>>>> v3.18
	retval = 0;
bail:
	if (f)
		fclose(f);
	return retval;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}
