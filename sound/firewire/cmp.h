#ifndef SOUND_FIREWIRE_CMP_H_INCLUDED
#define SOUND_FIREWIRE_CMP_H_INCLUDED

#include <linux/mutex.h>
#include <linux/types.h>
#include "iso-resources.h"

struct fw_unit;

<<<<<<< HEAD
=======
enum cmp_direction {
	CMP_INPUT = 0,
	CMP_OUTPUT,
};

>>>>>>> v3.18
/**
 * struct cmp_connection - manages an isochronous connection to a device
 * @speed: the connection's actual speed
 *
<<<<<<< HEAD
 * This structure manages (using CMP) an isochronous stream from the local
 * computer to a device's input plug (iPCR).
=======
 * This structure manages (using CMP) an isochronous stream between the local
 * computer and a device's input plug (iPCR) and output plug (oPCR).
>>>>>>> v3.18
 *
 * There is no corresponding oPCR created on the local computer, so it is not
 * possible to overlay connections on top of this one.
 */
struct cmp_connection {
	int speed;
	/* private: */
	bool connected;
	struct mutex mutex;
	struct fw_iso_resources resources;
	__be32 last_pcr_value;
	unsigned int pcr_index;
	unsigned int max_speed;
<<<<<<< HEAD
=======
	enum cmp_direction direction;
>>>>>>> v3.18
};

int cmp_connection_init(struct cmp_connection *connection,
			struct fw_unit *unit,
<<<<<<< HEAD
			unsigned int ipcr_index);
=======
			enum cmp_direction direction,
			unsigned int pcr_index);
int cmp_connection_check_used(struct cmp_connection *connection, bool *used);
>>>>>>> v3.18
void cmp_connection_destroy(struct cmp_connection *connection);

int cmp_connection_establish(struct cmp_connection *connection,
			     unsigned int max_payload);
int cmp_connection_update(struct cmp_connection *connection);
void cmp_connection_break(struct cmp_connection *connection);

#endif
