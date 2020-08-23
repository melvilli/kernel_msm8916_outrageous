#ifndef DECOMPRESS_UNLZO_H
#define DECOMPRESS_UNLZO_H

<<<<<<< HEAD
int unlzo(unsigned char *inbuf, int len,
	int(*fill)(void*, unsigned int),
	int(*flush)(void*, unsigned int),
	unsigned char *output,
	int *pos,
=======
int unlzo(unsigned char *inbuf, long len,
	long (*fill)(void*, unsigned long),
	long (*flush)(void*, unsigned long),
	unsigned char *output,
	long *pos,
>>>>>>> v3.18
	void(*error)(char *x));
#endif
