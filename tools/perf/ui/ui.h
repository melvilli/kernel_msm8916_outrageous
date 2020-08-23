#ifndef _PERF_UI_H_
#define _PERF_UI_H_ 1

#include <pthread.h>
#include <stdbool.h>
#include <linux/compiler.h>

extern pthread_mutex_t ui__lock;
<<<<<<< HEAD
<<<<<<< HEAD
=======
extern void *perf_gtk_handle;
>>>>>>> v3.18
=======
extern void *perf_gtk_handle;
>>>>>>> v3.18

extern int use_browser;

void setup_browser(bool fallback_to_pager);
void exit_browser(bool wait_for_ok);

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef SLANG_SUPPORT
=======
#ifdef HAVE_SLANG_SUPPORT
>>>>>>> v3.18
=======
#ifdef HAVE_SLANG_SUPPORT
>>>>>>> v3.18
int ui__init(void);
void ui__exit(bool wait_for_ok);
#else
static inline int ui__init(void)
{
	return -1;
}
static inline void ui__exit(bool wait_for_ok __maybe_unused) {}
#endif

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef GTK2_SUPPORT
int perf_gtk__init(void);
void perf_gtk__exit(bool wait_for_ok);
#else
static inline int perf_gtk__init(void)
{
	return -1;
}
static inline void perf_gtk__exit(bool wait_for_ok __maybe_unused) {}
#endif

=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
void ui__refresh_dimensions(bool force);

#endif /* _PERF_UI_H_ */
