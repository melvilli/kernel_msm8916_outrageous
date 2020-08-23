#ifndef DECOMPRESS_UNLZMA_H
#define DECOMPRESS_UNLZMA_H

<<<<<<< HEAD
int unlzma(unsigned char *, int,
	   int(*fill)(void*, unsigned int),
	   int(*flush)(void*, unsigned int),
	   unsigned char *output,
	   int *posp,
=======
int unlzma(unsigned char *, long,
	   long (*fill)(void*, unsigned long),
	   long (*flush)(void*, unsigned long),
	   unsigned char *output,
	   long *posp,
>>>>>>> v3.18
	   void(*error)(char *x)
	);

#endif
