#ifndef __DRIVERS_USB_CHIPIDEA_HOST_H
#define __DRIVERS_USB_CHIPIDEA_HOST_H

#ifdef CONFIG_USB_CHIPIDEA_HOST

<<<<<<< HEAD
int ci_hdrc_host_init(struct ci13xxx *ci);

#else

static inline int ci_hdrc_host_init(struct ci13xxx *ci)
=======
int ci_hdrc_host_init(struct ci_hdrc *ci);
void ci_hdrc_host_destroy(struct ci_hdrc *ci);

#else

static inline int ci_hdrc_host_init(struct ci_hdrc *ci)
>>>>>>> v3.18
{
	return -ENXIO;
}

<<<<<<< HEAD
=======
static inline void ci_hdrc_host_destroy(struct ci_hdrc *ci)
{

}

>>>>>>> v3.18
#endif

#endif /* __DRIVERS_USB_CHIPIDEA_HOST_H */
