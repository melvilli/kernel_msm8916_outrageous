/*
 *    Copyright IBM Corp. 2007
 *    Author(s): Heiko Carstens <heiko.carstens@de.ibm.com>
 */

#ifndef _ASM_S390_SCLP_H
#define _ASM_S390_SCLP_H

#include <linux/types.h>
#include <asm/chpid.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <asm/cpu.h>
>>>>>>> v3.18
=======
#include <asm/cpu.h>
>>>>>>> v3.18

#define SCLP_CHP_INFO_MASK_SIZE		32

struct sclp_chp_info {
	u8 recognized[SCLP_CHP_INFO_MASK_SIZE];
	u8 standby[SCLP_CHP_INFO_MASK_SIZE];
	u8 configured[SCLP_CHP_INFO_MASK_SIZE];
};

#define LOADPARM_LEN 8

struct sclp_ipl_info {
	int is_valid;
	int has_dump;
	char loadparm[LOADPARM_LEN];
};

struct sclp_cpu_entry {
	u8 address;
<<<<<<< HEAD
<<<<<<< HEAD
	u8 reserved0[13];
=======
=======
>>>>>>> v3.18
	u8 reserved0[2];
	u8 : 3;
	u8 siif : 1;
	u8 : 4;
	u8 reserved2[10];
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	u8 type;
	u8 reserved1;
} __attribute__((packed));

struct sclp_cpu_info {
	unsigned int configured;
	unsigned int standby;
	unsigned int combined;
	int has_cpu_type;
<<<<<<< HEAD
<<<<<<< HEAD
	struct sclp_cpu_entry cpu[255];
=======
	struct sclp_cpu_entry cpu[MAX_CPU_ADDRESS + 1];
>>>>>>> v3.18
=======
	struct sclp_cpu_entry cpu[MAX_CPU_ADDRESS + 1];
>>>>>>> v3.18
};

int sclp_get_cpu_info(struct sclp_cpu_info *info);
int sclp_cpu_configure(u8 cpu);
int sclp_cpu_deconfigure(u8 cpu);
<<<<<<< HEAD
<<<<<<< HEAD
void sclp_facilities_detect(void);
unsigned long long sclp_get_rnmax(void);
unsigned long long sclp_get_rzm(void);
=======
unsigned long long sclp_get_rnmax(void);
unsigned long long sclp_get_rzm(void);
unsigned int sclp_get_max_cpu(void);
>>>>>>> v3.18
=======
unsigned long long sclp_get_rnmax(void);
unsigned long long sclp_get_rzm(void);
unsigned int sclp_get_max_cpu(void);
>>>>>>> v3.18
int sclp_sdias_blk_count(void);
int sclp_sdias_copy(void *dest, int blk_num, int nr_blks);
int sclp_chp_configure(struct chp_id chpid);
int sclp_chp_deconfigure(struct chp_id chpid);
int sclp_chp_read_info(struct sclp_chp_info *info);
void sclp_get_ipl_info(struct sclp_ipl_info *info);
<<<<<<< HEAD
<<<<<<< HEAD
bool sclp_has_linemode(void);
bool sclp_has_vt220(void);
int sclp_pci_configure(u32 fid);
int sclp_pci_deconfigure(u32 fid);
=======
=======
>>>>>>> v3.18
bool __init sclp_has_linemode(void);
bool __init sclp_has_vt220(void);
bool sclp_has_sprp(void);
int sclp_pci_configure(u32 fid);
int sclp_pci_deconfigure(u32 fid);
int memcpy_hsa(void *dest, unsigned long src, size_t count, int mode);
unsigned long sclp_get_hsa_size(void);
void sclp_early_detect(void);
int sclp_has_siif(void);
unsigned int sclp_get_ibc(void);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

#endif /* _ASM_S390_SCLP_H */
