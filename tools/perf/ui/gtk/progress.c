#include <inttypes.h>

#include "gtk.h"
#include "../progress.h"
#include "util.h"

static GtkWidget *dialog;
static GtkWidget *progress;

<<<<<<< HEAD
<<<<<<< HEAD
static void gtk_progress_update(u64 curr, u64 total, const char *title)
{
	double fraction = total ? 1.0 * curr / total : 0.0;
=======
static void gtk_ui_progress__update(struct ui_progress *p)
{
	double fraction = p->total ? 1.0 * p->curr / p->total : 0.0;
>>>>>>> v3.18
=======
static void gtk_ui_progress__update(struct ui_progress *p)
{
	double fraction = p->total ? 1.0 * p->curr / p->total : 0.0;
>>>>>>> v3.18
	char buf[1024];

	if (dialog == NULL) {
		GtkWidget *vbox = gtk_vbox_new(TRUE, 5);
<<<<<<< HEAD
<<<<<<< HEAD
		GtkWidget *label = gtk_label_new(title);
=======
		GtkWidget *label = gtk_label_new(p->title);
>>>>>>> v3.18
=======
		GtkWidget *label = gtk_label_new(p->title);
>>>>>>> v3.18

		dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		progress = gtk_progress_bar_new();

		gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, FALSE, 3);
		gtk_box_pack_start(GTK_BOX(vbox), progress, TRUE, TRUE, 3);

		gtk_container_add(GTK_CONTAINER(dialog), vbox);

		gtk_window_set_title(GTK_WINDOW(dialog), "perf");
		gtk_window_resize(GTK_WINDOW(dialog), 300, 80);
		gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

		gtk_widget_show_all(dialog);
	}

	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress), fraction);
<<<<<<< HEAD
<<<<<<< HEAD
	snprintf(buf, sizeof(buf), "%"PRIu64" / %"PRIu64, curr, total);
=======
	snprintf(buf, sizeof(buf), "%"PRIu64" / %"PRIu64, p->curr, p->total);
>>>>>>> v3.18
=======
	snprintf(buf, sizeof(buf), "%"PRIu64" / %"PRIu64, p->curr, p->total);
>>>>>>> v3.18
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress), buf);

	/* we didn't call gtk_main yet, so do it manually */
	while (gtk_events_pending())
		gtk_main_iteration();
}

<<<<<<< HEAD
<<<<<<< HEAD
static void gtk_progress_finish(void)
=======
static void gtk_ui_progress__finish(void)
>>>>>>> v3.18
=======
static void gtk_ui_progress__finish(void)
>>>>>>> v3.18
{
	/* this will also destroy all of its children */
	gtk_widget_destroy(dialog);

	dialog = NULL;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct ui_progress gtk_progress_fns = {
	.update		= gtk_progress_update,
	.finish		= gtk_progress_finish,
};

void perf_gtk__init_progress(void)
{
	progress_fns = &gtk_progress_fns;
=======
=======
>>>>>>> v3.18
static struct ui_progress_ops gtk_ui_progress__ops = {
	.update		= gtk_ui_progress__update,
	.finish		= gtk_ui_progress__finish,
};

void gtk_ui_progress__init(void)
{
	ui_progress__ops = &gtk_ui_progress__ops;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
}
