#ifndef DECOMPRESS_BUNZIP2_H
#define DECOMPRESS_BUNZIP2_H

<<<<<<< HEAD
int bunzip2(unsigned char *inbuf, int len,
	    int(*fill)(void*, unsigned int),
	    int(*flush)(void*, unsigned int),
	    unsigned char *output,
	    int *pos,
=======
int bunzip2(unsigned char *inbuf, long len,
	    long (*fill)(void*, unsigned long),
	    long (*flush)(void*, unsigned long),
	    unsigned char *output,
	    long *pos,
>>>>>>> v3.18
	    void(*error)(char *x));
#endif
