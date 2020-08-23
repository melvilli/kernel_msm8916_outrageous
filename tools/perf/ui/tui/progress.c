#include "../cache.h"
#include "../progress.h"
#include "../libslang.h"
#include "../ui.h"
<<<<<<< HEAD
#include "../browser.h"

static void tui_progress__update(u64 curr, u64 total, const char *title)
=======
#include "tui.h"
#include "../browser.h"

static void tui_progress__update(struct ui_progress *p)
>>>>>>> v3.18
{
	int bar, y;
	/*
	 * FIXME: We should have a per UI backend way of showing progress,
	 * stdio will just show a percentage as NN%, etc.
	 */
	if (use_browser <= 0)
		return;

<<<<<<< HEAD
	if (total == 0)
		return;

	ui__refresh_dimensions(true);
=======
	if (p->total == 0)
		return;

	ui__refresh_dimensions(false);
>>>>>>> v3.18
	pthread_mutex_lock(&ui__lock);
	y = SLtt_Screen_Rows / 2 - 2;
	SLsmg_set_color(0);
	SLsmg_draw_box(y, 0, 3, SLtt_Screen_Cols);
	SLsmg_gotorc(y++, 1);
<<<<<<< HEAD
	SLsmg_write_string((char *)title);
	SLsmg_set_color(HE_COLORSET_SELECTED);
	bar = ((SLtt_Screen_Cols - 2) * curr) / total;
=======
	SLsmg_write_string((char *)p->title);
	SLsmg_fill_region(y, 1, 1, SLtt_Screen_Cols - 2, ' ');
	SLsmg_set_color(HE_COLORSET_SELECTED);
	bar = ((SLtt_Screen_Cols - 2) * p->curr) / p->total;
>>>>>>> v3.18
	SLsmg_fill_region(y, 1, 1, bar, ' ');
	SLsmg_refresh();
	pthread_mutex_unlock(&ui__lock);
}

<<<<<<< HEAD
static struct ui_progress tui_progress_fns =
=======
static struct ui_progress_ops tui_progress__ops =
>>>>>>> v3.18
{
	.update		= tui_progress__update,
};

<<<<<<< HEAD
void ui_progress__init(void)
{
	progress_fns = &tui_progress_fns;
=======
void tui_progress__init(void)
{
	ui_progress__ops = &tui_progress__ops;
>>>>>>> v3.18
}
