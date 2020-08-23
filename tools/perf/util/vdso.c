
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/kernel.h>

#include "vdso.h"
#include "util.h"
#include "symbol.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "linux/string.h"

static bool vdso_found;
static char vdso_file[] = "/tmp/perf-vdso.so-XXXXXX";
=======
=======
>>>>>>> v3.18
#include "machine.h"
#include "linux/string.h"
#include "debug.h"

#define VDSO__TEMP_FILE_NAME "/tmp/perf-vdso.so-XXXXXX"

struct vdso_file {
	bool found;
	bool error;
	char temp_file_name[sizeof(VDSO__TEMP_FILE_NAME)];
	const char *dso_name;
};

struct vdso_info {
	struct vdso_file vdso;
};

static struct vdso_info *vdso_info__new(void)
{
	static const struct vdso_info vdso_info_init = {
		.vdso    = {
			.temp_file_name = VDSO__TEMP_FILE_NAME,
			.dso_name = DSO__NAME_VDSO,
		},
	};

	return memdup(&vdso_info_init, sizeof(vdso_info_init));
}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

static int find_vdso_map(void **start, void **end)
{
	FILE *maps;
	char line[128];
	int found = 0;

	maps = fopen("/proc/self/maps", "r");
	if (!maps) {
		pr_err("vdso: cannot open maps\n");
		return -1;
	}

	while (!found && fgets(line, sizeof(line), maps)) {
		int m = -1;

		/* We care only about private r-x mappings. */
		if (2 != sscanf(line, "%p-%p r-xp %*x %*x:%*x %*u %n",
				start, end, &m))
			continue;
		if (m < 0)
			continue;

		if (!strncmp(&line[m], VDSO__MAP_NAME,
			     sizeof(VDSO__MAP_NAME) - 1))
			found = 1;
	}

	fclose(maps);
	return !found;
}

<<<<<<< HEAD
<<<<<<< HEAD
static char *get_file(void)
=======
static char *get_file(struct vdso_file *vdso_file)
>>>>>>> v3.18
=======
static char *get_file(struct vdso_file *vdso_file)
>>>>>>> v3.18
{
	char *vdso = NULL;
	char *buf = NULL;
	void *start, *end;
	size_t size;
	int fd;

<<<<<<< HEAD
<<<<<<< HEAD
	if (vdso_found)
		return vdso_file;

	if (find_vdso_map(&start, &end))
=======
=======
>>>>>>> v3.18
	if (vdso_file->found)
		return vdso_file->temp_file_name;

	if (vdso_file->error || find_vdso_map(&start, &end))
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
		return NULL;

	size = end - start;

	buf = memdup(start, size);
	if (!buf)
		return NULL;

<<<<<<< HEAD
<<<<<<< HEAD
	fd = mkstemp(vdso_file);
=======
	fd = mkstemp(vdso_file->temp_file_name);
>>>>>>> v3.18
=======
	fd = mkstemp(vdso_file->temp_file_name);
>>>>>>> v3.18
	if (fd < 0)
		goto out;

	if (size == (size_t) write(fd, buf, size))
<<<<<<< HEAD
<<<<<<< HEAD
		vdso = vdso_file;
=======
		vdso = vdso_file->temp_file_name;
>>>>>>> v3.18
=======
		vdso = vdso_file->temp_file_name;
>>>>>>> v3.18

	close(fd);

 out:
	free(buf);

<<<<<<< HEAD
<<<<<<< HEAD
	vdso_found = (vdso != NULL);
	return vdso;
}

void vdso__exit(void)
{
	if (vdso_found)
		unlink(vdso_file);
}

struct dso *vdso__dso_findnew(struct list_head *head)
{
	struct dso *dso = dsos__find(head, VDSO__MAP_NAME);

	if (!dso) {
		char *file;

		file = get_file();
		if (!file)
			return NULL;

		dso = dso__new(VDSO__MAP_NAME);
		if (dso != NULL) {
			dsos__add(head, dso);
			dso__set_long_name(dso, file);
		}
=======
=======
>>>>>>> v3.18
	vdso_file->found = (vdso != NULL);
	vdso_file->error = !vdso_file->found;
	return vdso;
}

void vdso__exit(struct machine *machine)
{
	struct vdso_info *vdso_info = machine->vdso_info;

	if (!vdso_info)
		return;

	if (vdso_info->vdso.found)
		unlink(vdso_info->vdso.temp_file_name);

	zfree(&machine->vdso_info);
}

static struct dso *vdso__new(struct machine *machine, const char *short_name,
			     const char *long_name)
{
	struct dso *dso;

	dso = dso__new(short_name);
	if (dso != NULL) {
		dsos__add(&machine->user_dsos, dso);
		dso__set_long_name(dso, long_name, false);
	}

	return dso;
}

struct dso *vdso__dso_findnew(struct machine *machine,
			      struct thread *thread __maybe_unused)
{
	struct vdso_info *vdso_info;
	struct dso *dso;

	if (!machine->vdso_info)
		machine->vdso_info = vdso_info__new();

	vdso_info = machine->vdso_info;
	if (!vdso_info)
		return NULL;

	dso = dsos__find(&machine->user_dsos, DSO__NAME_VDSO, true);
	if (!dso) {
		char *file;

		file = get_file(&vdso_info->vdso);
		if (!file)
			return NULL;

		dso = vdso__new(machine, DSO__NAME_VDSO, file);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}

	return dso;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18

bool dso__is_vdso(struct dso *dso)
{
	return !strcmp(dso->short_name, DSO__NAME_VDSO);
}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
