/*
 * Helper functions for handling target threads/cpus
 *
 * Copyright (C) 2012, LG Electronics, Namhyung Kim <namhyung.kim@lge.com>
 *
 * Released under the GPL v2.
 */

#include "target.h"
#include "debug.h"

#include <pwd.h>
#include <string.h>


<<<<<<< HEAD
<<<<<<< HEAD
enum perf_target_errno perf_target__validate(struct perf_target *target)
{
	enum perf_target_errno ret = PERF_ERRNO_TARGET__SUCCESS;
=======
enum target_errno target__validate(struct target *target)
{
	enum target_errno ret = TARGET_ERRNO__SUCCESS;
>>>>>>> v3.18
=======
enum target_errno target__validate(struct target *target)
{
	enum target_errno ret = TARGET_ERRNO__SUCCESS;
>>>>>>> v3.18

	if (target->pid)
		target->tid = target->pid;

	/* CPU and PID are mutually exclusive */
	if (target->tid && target->cpu_list) {
		target->cpu_list = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
		if (ret == PERF_ERRNO_TARGET__SUCCESS)
			ret = PERF_ERRNO_TARGET__PID_OVERRIDE_CPU;
=======
		if (ret == TARGET_ERRNO__SUCCESS)
			ret = TARGET_ERRNO__PID_OVERRIDE_CPU;
>>>>>>> v3.18
=======
		if (ret == TARGET_ERRNO__SUCCESS)
			ret = TARGET_ERRNO__PID_OVERRIDE_CPU;
>>>>>>> v3.18
	}

	/* UID and PID are mutually exclusive */
	if (target->tid && target->uid_str) {
		target->uid_str = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
		if (ret == PERF_ERRNO_TARGET__SUCCESS)
			ret = PERF_ERRNO_TARGET__PID_OVERRIDE_UID;
=======
		if (ret == TARGET_ERRNO__SUCCESS)
			ret = TARGET_ERRNO__PID_OVERRIDE_UID;
>>>>>>> v3.18
=======
		if (ret == TARGET_ERRNO__SUCCESS)
			ret = TARGET_ERRNO__PID_OVERRIDE_UID;
>>>>>>> v3.18
	}

	/* UID and CPU are mutually exclusive */
	if (target->uid_str && target->cpu_list) {
		target->cpu_list = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
		if (ret == PERF_ERRNO_TARGET__SUCCESS)
			ret = PERF_ERRNO_TARGET__UID_OVERRIDE_CPU;
=======
		if (ret == TARGET_ERRNO__SUCCESS)
			ret = TARGET_ERRNO__UID_OVERRIDE_CPU;
>>>>>>> v3.18
=======
		if (ret == TARGET_ERRNO__SUCCESS)
			ret = TARGET_ERRNO__UID_OVERRIDE_CPU;
>>>>>>> v3.18
	}

	/* PID and SYSTEM are mutually exclusive */
	if (target->tid && target->system_wide) {
		target->system_wide = false;
<<<<<<< HEAD
<<<<<<< HEAD
		if (ret == PERF_ERRNO_TARGET__SUCCESS)
			ret = PERF_ERRNO_TARGET__PID_OVERRIDE_SYSTEM;
=======
		if (ret == TARGET_ERRNO__SUCCESS)
			ret = TARGET_ERRNO__PID_OVERRIDE_SYSTEM;
>>>>>>> v3.18
=======
		if (ret == TARGET_ERRNO__SUCCESS)
			ret = TARGET_ERRNO__PID_OVERRIDE_SYSTEM;
>>>>>>> v3.18
	}

	/* UID and SYSTEM are mutually exclusive */
	if (target->uid_str && target->system_wide) {
		target->system_wide = false;
<<<<<<< HEAD
<<<<<<< HEAD
		if (ret == PERF_ERRNO_TARGET__SUCCESS)
			ret = PERF_ERRNO_TARGET__UID_OVERRIDE_SYSTEM;
=======
=======
>>>>>>> v3.18
		if (ret == TARGET_ERRNO__SUCCESS)
			ret = TARGET_ERRNO__UID_OVERRIDE_SYSTEM;
	}

	/* THREAD and SYSTEM/CPU are mutually exclusive */
	if (target->per_thread && (target->system_wide || target->cpu_list)) {
		target->per_thread = false;
		if (ret == TARGET_ERRNO__SUCCESS)
			ret = TARGET_ERRNO__SYSTEM_OVERRIDE_THREAD;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
enum perf_target_errno perf_target__parse_uid(struct perf_target *target)
=======
enum target_errno target__parse_uid(struct target *target)
>>>>>>> v3.18
=======
enum target_errno target__parse_uid(struct target *target)
>>>>>>> v3.18
{
	struct passwd pwd, *result;
	char buf[1024];
	const char *str = target->uid_str;

	target->uid = UINT_MAX;
	if (str == NULL)
<<<<<<< HEAD
<<<<<<< HEAD
		return PERF_ERRNO_TARGET__SUCCESS;
=======
		return TARGET_ERRNO__SUCCESS;
>>>>>>> v3.18
=======
		return TARGET_ERRNO__SUCCESS;
>>>>>>> v3.18

	/* Try user name first */
	getpwnam_r(str, &pwd, buf, sizeof(buf), &result);

	if (result == NULL) {
		/*
		 * The user name not found. Maybe it's a UID number.
		 */
		char *endptr;
		int uid = strtol(str, &endptr, 10);

		if (*endptr != '\0')
<<<<<<< HEAD
<<<<<<< HEAD
			return PERF_ERRNO_TARGET__INVALID_UID;
=======
			return TARGET_ERRNO__INVALID_UID;
>>>>>>> v3.18
=======
			return TARGET_ERRNO__INVALID_UID;
>>>>>>> v3.18

		getpwuid_r(uid, &pwd, buf, sizeof(buf), &result);

		if (result == NULL)
<<<<<<< HEAD
<<<<<<< HEAD
			return PERF_ERRNO_TARGET__USER_NOT_FOUND;
	}

	target->uid = result->pw_uid;
	return PERF_ERRNO_TARGET__SUCCESS;
}

/*
 * This must have a same ordering as the enum perf_target_errno.
 */
static const char *perf_target__error_str[] = {
=======
=======
>>>>>>> v3.18
			return TARGET_ERRNO__USER_NOT_FOUND;
	}

	target->uid = result->pw_uid;
	return TARGET_ERRNO__SUCCESS;
}

/*
 * This must have a same ordering as the enum target_errno.
 */
static const char *target__error_str[] = {
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	"PID/TID switch overriding CPU",
	"PID/TID switch overriding UID",
	"UID switch overriding CPU",
	"PID/TID switch overriding SYSTEM",
	"UID switch overriding SYSTEM",
<<<<<<< HEAD
<<<<<<< HEAD
=======
	"SYSTEM/CPU switch overriding PER-THREAD",
>>>>>>> v3.18
=======
	"SYSTEM/CPU switch overriding PER-THREAD",
>>>>>>> v3.18
	"Invalid User: %s",
	"Problems obtaining information for user %s",
};

<<<<<<< HEAD
<<<<<<< HEAD
int perf_target__strerror(struct perf_target *target, int errnum,
=======
int target__strerror(struct target *target, int errnum,
>>>>>>> v3.18
=======
int target__strerror(struct target *target, int errnum,
>>>>>>> v3.18
			  char *buf, size_t buflen)
{
	int idx;
	const char *msg;

	BUG_ON(buflen == 0);

	if (errnum >= 0) {
		const char *err = strerror_r(errnum, buf, buflen);

		if (err != buf) {
			size_t len = strlen(err);
			memcpy(buf, err, min(buflen - 1, len));
			*(buf + min(buflen - 1, len)) = '\0';
		}

		return 0;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (errnum <  __PERF_ERRNO_TARGET__START ||
	    errnum >= __PERF_ERRNO_TARGET__END)
		return -1;

	idx = errnum - __PERF_ERRNO_TARGET__START;
	msg = perf_target__error_str[idx];

	switch (errnum) {
	case PERF_ERRNO_TARGET__PID_OVERRIDE_CPU
	 ... PERF_ERRNO_TARGET__UID_OVERRIDE_SYSTEM:
		snprintf(buf, buflen, "%s", msg);
		break;

	case PERF_ERRNO_TARGET__INVALID_UID:
	case PERF_ERRNO_TARGET__USER_NOT_FOUND:
=======
=======
>>>>>>> v3.18
	if (errnum <  __TARGET_ERRNO__START || errnum >= __TARGET_ERRNO__END)
		return -1;

	idx = errnum - __TARGET_ERRNO__START;
	msg = target__error_str[idx];

	switch (errnum) {
	case TARGET_ERRNO__PID_OVERRIDE_CPU ...
	     TARGET_ERRNO__SYSTEM_OVERRIDE_THREAD:
		snprintf(buf, buflen, "%s", msg);
		break;

	case TARGET_ERRNO__INVALID_UID:
	case TARGET_ERRNO__USER_NOT_FOUND:
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		snprintf(buf, buflen, msg, target->uid_str);
		break;

	default:
		/* cannot reach here */
		break;
	}

	return 0;
}
