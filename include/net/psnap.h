#ifndef _NET_PSNAP_H
#define _NET_PSNAP_H

<<<<<<< HEAD
<<<<<<< HEAD
extern struct datalink_proto *
=======
struct datalink_proto *
>>>>>>> v3.18
=======
struct datalink_proto *
>>>>>>> v3.18
register_snap_client(const unsigned char *desc,
		     int (*rcvfunc)(struct sk_buff *, struct net_device *,
				    struct packet_type *,
				    struct net_device *orig_dev));
<<<<<<< HEAD
<<<<<<< HEAD
extern void unregister_snap_client(struct datalink_proto *proto);
=======
void unregister_snap_client(struct datalink_proto *proto);
>>>>>>> v3.18
=======
void unregister_snap_client(struct datalink_proto *proto);
>>>>>>> v3.18

#endif
