/* X.509 certificate parser internal definitions
 *
 * Copyright (C) 2012 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/time.h>
>>>>>>> v3.18
=======
#include <linux/time.h>
>>>>>>> v3.18
#include <crypto/public_key.h>

struct x509_certificate {
	struct x509_certificate *next;
<<<<<<< HEAD
<<<<<<< HEAD
	struct public_key *pub;			/* Public key details */
	char		*issuer;		/* Name of certificate issuer */
	char		*subject;		/* Name of certificate subject */
	char		*fingerprint;		/* Key fingerprint as hex */
	char		*authority;		/* Authority key fingerprint as hex */
	struct tm	valid_from;
	struct tm	valid_to;
	enum pkey_algo	pkey_algo : 8;		/* Public key algorithm */
	enum pkey_algo	sig_pkey_algo : 8;	/* Signature public key algorithm */
	enum pkey_hash_algo sig_hash_algo : 8;	/* Signature hash algorithm */
	const void	*tbs;			/* Signed data */
	size_t		tbs_size;		/* Size of signed data */
	const void	*sig;			/* Signature data */
	size_t		sig_size;		/* Size of sigature */
=======
=======
>>>>>>> v3.18
	struct x509_certificate *signer;	/* Certificate that signed this one */
	struct public_key *pub;			/* Public key details */
	struct public_key_signature sig;	/* Signature parameters */
	char		*issuer;		/* Name of certificate issuer */
	char		*subject;		/* Name of certificate subject */
	struct asymmetric_key_id *id;		/* Serial number + issuer */
	struct asymmetric_key_id *skid;		/* Subject + subjectKeyId (optional) */
	struct asymmetric_key_id *authority;	/* Authority key identifier (optional) */
	struct tm	valid_from;
	struct tm	valid_to;
	const void	*tbs;			/* Signed data */
	unsigned	tbs_size;		/* Size of signed data */
	unsigned	raw_sig_size;		/* Size of sigature */
	const void	*raw_sig;		/* Signature data */
	const void	*raw_serial;		/* Raw serial number in ASN.1 */
	unsigned	raw_serial_size;
	unsigned	raw_issuer_size;
	const void	*raw_issuer;		/* Raw issuer name in ASN.1 */
	const void	*raw_subject;		/* Raw subject name in ASN.1 */
	unsigned	raw_subject_size;
	unsigned	raw_skid_size;
	const void	*raw_skid;		/* Raw subjectKeyId in ASN.1 */
	unsigned	index;
	bool		seen;			/* Infinite recursion prevention */
	bool		verified;
	bool		trusted;
	bool		unsupported_crypto;	/* T if can't be verified due to missing crypto */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
};

/*
 * x509_cert_parser.c
 */
extern void x509_free_certificate(struct x509_certificate *cert);
extern struct x509_certificate *x509_cert_parse(const void *data, size_t datalen);
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18

/*
 * x509_public_key.c
 */
extern int x509_get_sig_params(struct x509_certificate *cert);
extern int x509_check_signature(const struct public_key *pub,
				struct x509_certificate *cert);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
