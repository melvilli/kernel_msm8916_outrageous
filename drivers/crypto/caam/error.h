/*
 * CAAM Error Reporting code header
 *
 * Copyright 2009-2011 Freescale Semiconductor, Inc.
 */

#ifndef CAAM_ERROR_H
#define CAAM_ERROR_H
#define CAAM_ERROR_STR_MAX 302
<<<<<<< HEAD
extern char *caam_jr_strstatus(char *outstr, u32 status);
=======
void caam_jr_strstatus(struct device *jrdev, u32 status);
>>>>>>> v3.18
#endif /* CAAM_ERROR_H */
