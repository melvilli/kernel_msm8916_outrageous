#ifndef __PERF_HIST_H
#define __PERF_HIST_H

#include <linux/types.h>
#include <pthread.h>
#include "callchain.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "header.h"

extern struct callchain_param callchain_param;
=======
=======
>>>>>>> v3.18
#include "evsel.h"
#include "header.h"
#include "color.h"
#include "ui/progress.h"
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

struct hist_entry;
struct addr_location;
struct symbol;

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * The kernel collects the number of events it couldn't send in a stretch and
 * when possible sends this number in a PERF_RECORD_LOST event. The number of
 * such "chunks" of lost events is stored in .nr_events[PERF_EVENT_LOST] while
 * total_lost tells exactly how many events the kernel in fact lost, i.e. it is
 * the sum of all struct lost_event.lost fields reported.
 *
 * The total_period is needed because by default auto-freq is used, so
 * multipling nr_events[PERF_EVENT_SAMPLE] by a frequency isn't possible to get
 * the total number of low level events, it is necessary to to sum all struct
 * sample_event.period and stash the result in total_period.
 */
struct events_stats {
	u64 total_period;
	u64 total_lost;
	u64 total_invalid_chains;
	u32 nr_events[PERF_RECORD_HEADER_MAX];
	u32 nr_lost_warned;
	u32 nr_unknown_events;
	u32 nr_invalid_chains;
	u32 nr_unknown_id;
	u32 nr_unprocessable_samples;
	u32 nr_unordered_events;
=======
=======
>>>>>>> v3.18
enum hist_filter {
	HIST_FILTER__DSO,
	HIST_FILTER__THREAD,
	HIST_FILTER__PARENT,
	HIST_FILTER__SYMBOL,
	HIST_FILTER__GUEST,
	HIST_FILTER__HOST,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
};

enum hist_column {
	HISTC_SYMBOL,
	HISTC_DSO,
	HISTC_THREAD,
	HISTC_COMM,
	HISTC_PARENT,
	HISTC_CPU,
<<<<<<< HEAD
<<<<<<< HEAD
	HISTC_MISPREDICT,
=======
=======
>>>>>>> v3.18
	HISTC_SRCLINE,
	HISTC_MISPREDICT,
	HISTC_IN_TX,
	HISTC_ABORT,
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	HISTC_SYMBOL_FROM,
	HISTC_SYMBOL_TO,
	HISTC_DSO_FROM,
	HISTC_DSO_TO,
<<<<<<< HEAD
<<<<<<< HEAD
	HISTC_SRCLINE,
=======
>>>>>>> v3.18
=======
>>>>>>> v3.18
	HISTC_LOCAL_WEIGHT,
	HISTC_GLOBAL_WEIGHT,
	HISTC_MEM_DADDR_SYMBOL,
	HISTC_MEM_DADDR_DSO,
	HISTC_MEM_LOCKED,
	HISTC_MEM_TLB,
	HISTC_MEM_LVL,
	HISTC_MEM_SNOOP,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	HISTC_MEM_DCACHELINE,
	HISTC_TRANSACTION,
>>>>>>> v3.18
=======
	HISTC_MEM_DCACHELINE,
	HISTC_TRANSACTION,
>>>>>>> v3.18
	HISTC_NR_COLS, /* Last entry */
};

struct thread;
struct dso;

struct hists {
	struct rb_root		entries_in_array[2];
	struct rb_root		*entries_in;
	struct rb_root		entries;
	struct rb_root		entries_collapsed;
	u64			nr_entries;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	u64			nr_non_filtered_entries;
>>>>>>> v3.18
=======
	u64			nr_non_filtered_entries;
>>>>>>> v3.18
	const struct thread	*thread_filter;
	const struct dso	*dso_filter;
	const char		*uid_filter_str;
	const char		*symbol_filter_str;
	pthread_mutex_t		lock;
	struct events_stats	stats;
	u64			event_stream;
	u16			col_len[HISTC_NR_COLS];
};

<<<<<<< HEAD
<<<<<<< HEAD
struct hist_entry *__hists__add_entry(struct hists *self,
				      struct addr_location *al,
				      struct symbol *parent, u64 period,
				      u64 weight);
int64_t hist_entry__cmp(struct hist_entry *left, struct hist_entry *right);
int64_t hist_entry__collapse(struct hist_entry *left, struct hist_entry *right);
int hist_entry__sort_snprintf(struct hist_entry *self, char *bf, size_t size,
			      struct hists *hists);
void hist_entry__free(struct hist_entry *);

struct hist_entry *__hists__add_branch_entry(struct hists *self,
					     struct addr_location *al,
					     struct symbol *sym_parent,
					     struct branch_info *bi,
					     u64 period,
					     u64 weight);

struct hist_entry *__hists__add_mem_entry(struct hists *self,
					  struct addr_location *al,
					  struct symbol *sym_parent,
					  struct mem_info *mi,
					  u64 period,
					  u64 weight);

void hists__output_resort(struct hists *self);
void hists__output_resort_threaded(struct hists *hists);
void hists__collapse_resort(struct hists *self);
void hists__collapse_resort_threaded(struct hists *hists);

void hists__decay_entries(struct hists *hists, bool zap_user, bool zap_kernel);
void hists__decay_entries_threaded(struct hists *hists, bool zap_user,
				   bool zap_kernel);
void hists__output_recalc_col_len(struct hists *hists, int max_rows);

void hists__inc_nr_entries(struct hists *hists, struct hist_entry *h);
void hists__inc_nr_events(struct hists *self, u32 type);
void events_stats__inc(struct events_stats *stats, u32 type);
size_t events_stats__fprintf(struct events_stats *stats, FILE *fp);

size_t hists__fprintf(struct hists *self, bool show_header, int max_rows,
		      int max_cols, FILE *fp);

int hist_entry__inc_addr_samples(struct hist_entry *self, int evidx, u64 addr);
int hist_entry__annotate(struct hist_entry *self, size_t privsize);
=======
=======
>>>>>>> v3.18
struct hist_entry_iter;

struct hist_iter_ops {
	int (*prepare_entry)(struct hist_entry_iter *, struct addr_location *);
	int (*add_single_entry)(struct hist_entry_iter *, struct addr_location *);
	int (*next_entry)(struct hist_entry_iter *, struct addr_location *);
	int (*add_next_entry)(struct hist_entry_iter *, struct addr_location *);
	int (*finish_entry)(struct hist_entry_iter *, struct addr_location *);
};

struct hist_entry_iter {
	int total;
	int curr;

	bool hide_unresolved;

	struct perf_evsel *evsel;
	struct perf_sample *sample;
	struct hist_entry *he;
	struct symbol *parent;
	void *priv;

	const struct hist_iter_ops *ops;
	/* user-defined callback function (optional) */
	int (*add_entry_cb)(struct hist_entry_iter *iter,
			    struct addr_location *al, bool single, void *arg);
};

extern const struct hist_iter_ops hist_iter_normal;
extern const struct hist_iter_ops hist_iter_branch;
extern const struct hist_iter_ops hist_iter_mem;
extern const struct hist_iter_ops hist_iter_cumulative;

struct hist_entry *__hists__add_entry(struct hists *hists,
				      struct addr_location *al,
				      struct symbol *parent,
				      struct branch_info *bi,
				      struct mem_info *mi, u64 period,
				      u64 weight, u64 transaction,
				      bool sample_self);
int hist_entry_iter__add(struct hist_entry_iter *iter, struct addr_location *al,
			 struct perf_evsel *evsel, struct perf_sample *sample,
			 int max_stack_depth, void *arg);

int64_t hist_entry__cmp(struct hist_entry *left, struct hist_entry *right);
int64_t hist_entry__collapse(struct hist_entry *left, struct hist_entry *right);
int hist_entry__transaction_len(void);
int hist_entry__sort_snprintf(struct hist_entry *he, char *bf, size_t size,
			      struct hists *hists);
void hist_entry__free(struct hist_entry *);

void hists__output_resort(struct hists *hists);
void hists__collapse_resort(struct hists *hists, struct ui_progress *prog);

void hists__decay_entries(struct hists *hists, bool zap_user, bool zap_kernel);
void hists__delete_entries(struct hists *hists);
void hists__output_recalc_col_len(struct hists *hists, int max_rows);

u64 hists__total_period(struct hists *hists);
void hists__reset_stats(struct hists *hists);
void hists__inc_stats(struct hists *hists, struct hist_entry *h);
void hists__inc_nr_events(struct hists *hists, u32 type);
void hists__inc_nr_samples(struct hists *hists, bool filtered);
void events_stats__inc(struct events_stats *stats, u32 type);
size_t events_stats__fprintf(struct events_stats *stats, FILE *fp);

size_t hists__fprintf(struct hists *hists, bool show_header, int max_rows,
		      int max_cols, float min_pcnt, FILE *fp);
size_t perf_evlist__fprintf_nr_events(struct perf_evlist *evlist, FILE *fp);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

void hists__filter_by_dso(struct hists *hists);
void hists__filter_by_thread(struct hists *hists);
void hists__filter_by_symbol(struct hists *hists);

<<<<<<< HEAD
<<<<<<< HEAD
u16 hists__col_len(struct hists *self, enum hist_column col);
void hists__set_col_len(struct hists *self, enum hist_column col, u16 len);
bool hists__new_col_len(struct hists *self, enum hist_column col, u16 len);
=======
=======
>>>>>>> v3.18
static inline bool hists__has_filter(struct hists *hists)
{
	return hists->thread_filter || hists->dso_filter ||
		hists->symbol_filter_str;
}

u16 hists__col_len(struct hists *hists, enum hist_column col);
void hists__set_col_len(struct hists *hists, enum hist_column col, u16 len);
bool hists__new_col_len(struct hists *hists, enum hist_column col, u16 len);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
void hists__reset_col_len(struct hists *hists);
void hists__calc_col_len(struct hists *hists, struct hist_entry *he);

void hists__match(struct hists *leader, struct hists *other);
int hists__link(struct hists *leader, struct hists *other);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> v3.18
struct hists_evsel {
	struct perf_evsel evsel;
	struct hists	  hists;
};

static inline struct perf_evsel *hists_to_evsel(struct hists *hists)
{
	struct hists_evsel *hevsel = container_of(hists, struct hists_evsel, hists);
	return &hevsel->evsel;
}

static inline struct hists *evsel__hists(struct perf_evsel *evsel)
{
	struct hists_evsel *hevsel = (struct hists_evsel *)evsel;
	return &hevsel->hists;
}

int hists__init(void);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
struct perf_hpp {
	char *buf;
	size_t size;
	const char *sep;
	void *ptr;
};

struct perf_hpp_fmt {
<<<<<<< HEAD
<<<<<<< HEAD
	int (*header)(struct perf_hpp *hpp);
	int (*width)(struct perf_hpp *hpp);
	int (*color)(struct perf_hpp *hpp, struct hist_entry *he);
	int (*entry)(struct perf_hpp *hpp, struct hist_entry *he);

	struct list_head list;
};

extern struct list_head perf_hpp__list;
=======
=======
>>>>>>> v3.18
	const char *name;
	int (*header)(struct perf_hpp_fmt *fmt, struct perf_hpp *hpp,
		      struct perf_evsel *evsel);
	int (*width)(struct perf_hpp_fmt *fmt, struct perf_hpp *hpp,
		     struct perf_evsel *evsel);
	int (*color)(struct perf_hpp_fmt *fmt, struct perf_hpp *hpp,
		     struct hist_entry *he);
	int (*entry)(struct perf_hpp_fmt *fmt, struct perf_hpp *hpp,
		     struct hist_entry *he);
	int64_t (*cmp)(struct hist_entry *a, struct hist_entry *b);
	int64_t (*collapse)(struct hist_entry *a, struct hist_entry *b);
	int64_t (*sort)(struct hist_entry *a, struct hist_entry *b);

	struct list_head list;
	struct list_head sort_list;
	bool elide;
	int len;
	int user_len;
};

extern struct list_head perf_hpp__list;
extern struct list_head perf_hpp__sort_list;
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

#define perf_hpp__for_each_format(format) \
	list_for_each_entry(format, &perf_hpp__list, list)

<<<<<<< HEAD
<<<<<<< HEAD
extern struct perf_hpp_fmt perf_hpp__format[];

enum {
	PERF_HPP__BASELINE,
=======
=======
>>>>>>> v3.18
#define perf_hpp__for_each_format_safe(format, tmp)	\
	list_for_each_entry_safe(format, tmp, &perf_hpp__list, list)

#define perf_hpp__for_each_sort_list(format) \
	list_for_each_entry(format, &perf_hpp__sort_list, sort_list)

#define perf_hpp__for_each_sort_list_safe(format, tmp)	\
	list_for_each_entry_safe(format, tmp, &perf_hpp__sort_list, sort_list)

extern struct perf_hpp_fmt perf_hpp__format[];

enum {
	/* Matches perf_hpp__format array. */
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	PERF_HPP__OVERHEAD,
	PERF_HPP__OVERHEAD_SYS,
	PERF_HPP__OVERHEAD_US,
	PERF_HPP__OVERHEAD_GUEST_SYS,
	PERF_HPP__OVERHEAD_GUEST_US,
<<<<<<< HEAD
<<<<<<< HEAD
	PERF_HPP__SAMPLES,
	PERF_HPP__PERIOD,
	PERF_HPP__PERIOD_BASELINE,
	PERF_HPP__DELTA,
	PERF_HPP__RATIO,
	PERF_HPP__WEIGHTED_DIFF,
	PERF_HPP__FORMULA,
=======
	PERF_HPP__OVERHEAD_ACC,
	PERF_HPP__SAMPLES,
	PERF_HPP__PERIOD,
>>>>>>> v3.18
=======
	PERF_HPP__OVERHEAD_ACC,
	PERF_HPP__SAMPLES,
	PERF_HPP__PERIOD,
>>>>>>> v3.18

	PERF_HPP__MAX_INDEX
};

void perf_hpp__init(void);
void perf_hpp__column_register(struct perf_hpp_fmt *format);
<<<<<<< HEAD
<<<<<<< HEAD
void perf_hpp__column_enable(unsigned col);
int hist_entry__period_snprintf(struct perf_hpp *hpp, struct hist_entry *he,
				bool color);
=======
=======
>>>>>>> v3.18
void perf_hpp__column_unregister(struct perf_hpp_fmt *format);
void perf_hpp__column_enable(unsigned col);
void perf_hpp__column_disable(unsigned col);
void perf_hpp__cancel_cumulate(void);

void perf_hpp__register_sort_field(struct perf_hpp_fmt *format);
void perf_hpp__setup_output_field(void);
void perf_hpp__reset_output_field(void);
void perf_hpp__append_sort_keys(void);

bool perf_hpp__is_sort_entry(struct perf_hpp_fmt *format);
bool perf_hpp__same_sort_entry(struct perf_hpp_fmt *a, struct perf_hpp_fmt *b);

static inline bool perf_hpp__should_skip(struct perf_hpp_fmt *format)
{
	return format->elide;
}

void perf_hpp__reset_width(struct perf_hpp_fmt *fmt, struct hists *hists);
void perf_hpp__reset_sort_width(struct perf_hpp_fmt *fmt, struct hists *hists);
void perf_hpp__set_user_width(const char *width_list_str);

typedef u64 (*hpp_field_fn)(struct hist_entry *he);
typedef int (*hpp_callback_fn)(struct perf_hpp *hpp, bool front);
typedef int (*hpp_snprint_fn)(struct perf_hpp *hpp, const char *fmt, ...);

int hpp__fmt(struct perf_hpp_fmt *fmt, struct perf_hpp *hpp,
	     struct hist_entry *he, hpp_field_fn get_field,
	     const char *fmtstr, hpp_snprint_fn print_fn, bool fmt_percent);
int hpp__fmt_acc(struct perf_hpp_fmt *fmt, struct perf_hpp *hpp,
		 struct hist_entry *he, hpp_field_fn get_field,
		 const char *fmtstr, hpp_snprint_fn print_fn, bool fmt_percent);

static inline void advance_hpp(struct perf_hpp *hpp, int inc)
{
	hpp->buf  += inc;
	hpp->size -= inc;
}

static inline size_t perf_hpp__use_color(void)
{
	return !symbol_conf.field_sep;
}

static inline size_t perf_hpp__color_overhead(void)
{
	return perf_hpp__use_color() ?
	       (COLOR_MAXLEN + sizeof(PERF_COLOR_RESET)) * PERF_HPP__MAX_INDEX
	       : 0;
}
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18

struct perf_evlist;

struct hist_browser_timer {
	void (*timer)(void *arg);
	void *arg;
	int refresh;
};

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef SLANG_SUPPORT
=======
#ifdef HAVE_SLANG_SUPPORT
>>>>>>> v3.18
=======
#ifdef HAVE_SLANG_SUPPORT
>>>>>>> v3.18
#include "../ui/keysyms.h"
int hist_entry__tui_annotate(struct hist_entry *he, struct perf_evsel *evsel,
			     struct hist_browser_timer *hbt);

int perf_evlist__tui_browse_hists(struct perf_evlist *evlist, const char *help,
				  struct hist_browser_timer *hbt,
<<<<<<< HEAD
<<<<<<< HEAD
=======
				  float min_pcnt,
>>>>>>> v3.18
=======
				  float min_pcnt,
>>>>>>> v3.18
				  struct perf_session_env *env);
int script_browse(const char *script_opt);
#else
static inline
int perf_evlist__tui_browse_hists(struct perf_evlist *evlist __maybe_unused,
				  const char *help __maybe_unused,
				  struct hist_browser_timer *hbt __maybe_unused,
<<<<<<< HEAD
<<<<<<< HEAD
=======
				  float min_pcnt __maybe_unused,
>>>>>>> v3.18
=======
				  float min_pcnt __maybe_unused,
>>>>>>> v3.18
				  struct perf_session_env *env __maybe_unused)
{
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline int hist_entry__tui_annotate(struct hist_entry *self
					   __maybe_unused,
					   struct perf_evsel *evsel
					   __maybe_unused,
					   struct hist_browser_timer *hbt
					   __maybe_unused)
=======
static inline int hist_entry__tui_annotate(struct hist_entry *he __maybe_unused,
					   struct perf_evsel *evsel __maybe_unused,
					   struct hist_browser_timer *hbt __maybe_unused)
>>>>>>> v3.18
=======
static inline int hist_entry__tui_annotate(struct hist_entry *he __maybe_unused,
					   struct perf_evsel *evsel __maybe_unused,
					   struct hist_browser_timer *hbt __maybe_unused)
>>>>>>> v3.18
{
	return 0;
}

static inline int script_browse(const char *script_opt __maybe_unused)
{
	return 0;
}

#define K_LEFT  -1000
#define K_RIGHT -2000
#define K_SWITCH_INPUT_DATA -3000
#endif

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef GTK2_SUPPORT
int perf_evlist__gtk_browse_hists(struct perf_evlist *evlist, const char *help,
				  struct hist_browser_timer *hbt __maybe_unused);
#else
static inline
int perf_evlist__gtk_browse_hists(struct perf_evlist *evlist __maybe_unused,
				  const char *help __maybe_unused,
				  struct hist_browser_timer *hbt __maybe_unused)
{
	return 0;
}
#endif

unsigned int hists__sort_list_width(struct hists *self);

double perf_diff__compute_delta(struct hist_entry *he, struct hist_entry *pair);
double perf_diff__compute_ratio(struct hist_entry *he, struct hist_entry *pair);
s64 perf_diff__compute_wdiff(struct hist_entry *he, struct hist_entry *pair);
int perf_diff__formula(struct hist_entry *he, struct hist_entry *pair,
		       char *buf, size_t size);
double perf_diff__period_percent(struct hist_entry *he, u64 period);
=======
=======
>>>>>>> v3.18
unsigned int hists__sort_list_width(struct hists *hists);

struct option;
int parse_filter_percentage(const struct option *opt __maybe_unused,
			    const char *arg, int unset __maybe_unused);
int perf_hist_config(const char *var, const char *value);

<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
#endif	/* __PERF_HIST_H */
