<<<<<<< HEAD
#include "annotate.h"
=======
>>>>>>> v3.18
#include "util.h"
#include "build-id.h"
#include "hist.h"
#include "session.h"
#include "sort.h"
<<<<<<< HEAD
#include "evsel.h"
=======
#include "evlist.h"
#include "evsel.h"
#include "annotate.h"
>>>>>>> v3.18
#include <math.h>

static bool hists__filter_entry_by_dso(struct hists *hists,
				       struct hist_entry *he);
static bool hists__filter_entry_by_thread(struct hists *hists,
					  struct hist_entry *he);
static bool hists__filter_entry_by_symbol(struct hists *hists,
					  struct hist_entry *he);

<<<<<<< HEAD
enum hist_filter {
	HIST_FILTER__DSO,
	HIST_FILTER__THREAD,
	HIST_FILTER__PARENT,
	HIST_FILTER__SYMBOL,
};

struct callchain_param	callchain_param = {
	.mode	= CHAIN_GRAPH_REL,
	.min_percent = 0.5,
	.order  = ORDER_CALLEE
};

=======
>>>>>>> v3.18
u16 hists__col_len(struct hists *hists, enum hist_column col)
{
	return hists->col_len[col];
}

void hists__set_col_len(struct hists *hists, enum hist_column col, u16 len)
{
	hists->col_len[col] = len;
}

bool hists__new_col_len(struct hists *hists, enum hist_column col, u16 len)
{
	if (len > hists__col_len(hists, col)) {
		hists__set_col_len(hists, col, len);
		return true;
	}
	return false;
}

void hists__reset_col_len(struct hists *hists)
{
	enum hist_column col;

	for (col = 0; col < HISTC_NR_COLS; ++col)
		hists__set_col_len(hists, col, 0);
}

static void hists__set_unres_dso_col_len(struct hists *hists, int dso)
{
	const unsigned int unresolved_col_width = BITS_PER_LONG / 4;

	if (hists__col_len(hists, dso) < unresolved_col_width &&
	    !symbol_conf.col_width_list_str && !symbol_conf.field_sep &&
	    !symbol_conf.dso_list)
		hists__set_col_len(hists, dso, unresolved_col_width);
}

void hists__calc_col_len(struct hists *hists, struct hist_entry *h)
{
	const unsigned int unresolved_col_width = BITS_PER_LONG / 4;
	int symlen;
	u16 len;

<<<<<<< HEAD
	if (h->ms.sym)
		hists__new_col_len(hists, HISTC_SYMBOL, h->ms.sym->namelen + 4);
	else {
=======
	/*
	 * +4 accounts for '[x] ' priv level info
	 * +2 accounts for 0x prefix on raw addresses
	 * +3 accounts for ' y ' symtab origin info
	 */
	if (h->ms.sym) {
		symlen = h->ms.sym->namelen + 4;
		if (verbose)
			symlen += BITS_PER_LONG / 4 + 2 + 3;
		hists__new_col_len(hists, HISTC_SYMBOL, symlen);
	} else {
>>>>>>> v3.18
		symlen = unresolved_col_width + 4 + 2;
		hists__new_col_len(hists, HISTC_SYMBOL, symlen);
		hists__set_unres_dso_col_len(hists, HISTC_DSO);
	}

	len = thread__comm_len(h->thread);
	if (hists__new_col_len(hists, HISTC_COMM, len))
		hists__set_col_len(hists, HISTC_THREAD, len + 6);

	if (h->ms.map) {
		len = dso__name_len(h->ms.map->dso);
		hists__new_col_len(hists, HISTC_DSO, len);
	}

	if (h->parent)
		hists__new_col_len(hists, HISTC_PARENT, h->parent->namelen);

	if (h->branch_info) {
<<<<<<< HEAD
		/*
		 * +4 accounts for '[x] ' priv level info
		 * +2 account of 0x prefix on raw addresses
		 */
		if (h->branch_info->from.sym) {
			symlen = (int)h->branch_info->from.sym->namelen + 4;
=======
		if (h->branch_info->from.sym) {
			symlen = (int)h->branch_info->from.sym->namelen + 4;
			if (verbose)
				symlen += BITS_PER_LONG / 4 + 2 + 3;
>>>>>>> v3.18
			hists__new_col_len(hists, HISTC_SYMBOL_FROM, symlen);

			symlen = dso__name_len(h->branch_info->from.map->dso);
			hists__new_col_len(hists, HISTC_DSO_FROM, symlen);
		} else {
			symlen = unresolved_col_width + 4 + 2;
			hists__new_col_len(hists, HISTC_SYMBOL_FROM, symlen);
			hists__set_unres_dso_col_len(hists, HISTC_DSO_FROM);
		}

		if (h->branch_info->to.sym) {
			symlen = (int)h->branch_info->to.sym->namelen + 4;
<<<<<<< HEAD
=======
			if (verbose)
				symlen += BITS_PER_LONG / 4 + 2 + 3;
>>>>>>> v3.18
			hists__new_col_len(hists, HISTC_SYMBOL_TO, symlen);

			symlen = dso__name_len(h->branch_info->to.map->dso);
			hists__new_col_len(hists, HISTC_DSO_TO, symlen);
		} else {
			symlen = unresolved_col_width + 4 + 2;
			hists__new_col_len(hists, HISTC_SYMBOL_TO, symlen);
			hists__set_unres_dso_col_len(hists, HISTC_DSO_TO);
		}
	}

	if (h->mem_info) {
<<<<<<< HEAD
		/*
		 * +4 accounts for '[x] ' priv level info
		 * +2 account of 0x prefix on raw addresses
		 */
=======
>>>>>>> v3.18
		if (h->mem_info->daddr.sym) {
			symlen = (int)h->mem_info->daddr.sym->namelen + 4
			       + unresolved_col_width + 2;
			hists__new_col_len(hists, HISTC_MEM_DADDR_SYMBOL,
					   symlen);
<<<<<<< HEAD
=======
			hists__new_col_len(hists, HISTC_MEM_DCACHELINE,
					   symlen + 1);
>>>>>>> v3.18
		} else {
			symlen = unresolved_col_width + 4 + 2;
			hists__new_col_len(hists, HISTC_MEM_DADDR_SYMBOL,
					   symlen);
		}
		if (h->mem_info->daddr.map) {
			symlen = dso__name_len(h->mem_info->daddr.map->dso);
			hists__new_col_len(hists, HISTC_MEM_DADDR_DSO,
					   symlen);
		} else {
			symlen = unresolved_col_width + 4 + 2;
			hists__set_unres_dso_col_len(hists, HISTC_MEM_DADDR_DSO);
		}
	} else {
		symlen = unresolved_col_width + 4 + 2;
		hists__new_col_len(hists, HISTC_MEM_DADDR_SYMBOL, symlen);
		hists__set_unres_dso_col_len(hists, HISTC_MEM_DADDR_DSO);
	}

	hists__new_col_len(hists, HISTC_MEM_LOCKED, 6);
	hists__new_col_len(hists, HISTC_MEM_TLB, 22);
	hists__new_col_len(hists, HISTC_MEM_SNOOP, 12);
	hists__new_col_len(hists, HISTC_MEM_LVL, 21 + 3);
	hists__new_col_len(hists, HISTC_LOCAL_WEIGHT, 12);
	hists__new_col_len(hists, HISTC_GLOBAL_WEIGHT, 12);
<<<<<<< HEAD
=======

	if (h->transaction)
		hists__new_col_len(hists, HISTC_TRANSACTION,
				   hist_entry__transaction_len());
>>>>>>> v3.18
}

void hists__output_recalc_col_len(struct hists *hists, int max_rows)
{
	struct rb_node *next = rb_first(&hists->entries);
	struct hist_entry *n;
	int row = 0;

	hists__reset_col_len(hists);

	while (next && row++ < max_rows) {
		n = rb_entry(next, struct hist_entry, rb_node);
		if (!n->filtered)
			hists__calc_col_len(hists, n);
		next = rb_next(&n->rb_node);
	}
}

<<<<<<< HEAD
static void hist_entry__add_cpumode_period(struct hist_entry *he,
					   unsigned int cpumode, u64 period)
{
	switch (cpumode) {
	case PERF_RECORD_MISC_KERNEL:
		he->stat.period_sys += period;
		break;
	case PERF_RECORD_MISC_USER:
		he->stat.period_us += period;
		break;
	case PERF_RECORD_MISC_GUEST_KERNEL:
		he->stat.period_guest_sys += period;
		break;
	case PERF_RECORD_MISC_GUEST_USER:
		he->stat.period_guest_us += period;
=======
static void he_stat__add_cpumode_period(struct he_stat *he_stat,
					unsigned int cpumode, u64 period)
{
	switch (cpumode) {
	case PERF_RECORD_MISC_KERNEL:
		he_stat->period_sys += period;
		break;
	case PERF_RECORD_MISC_USER:
		he_stat->period_us += period;
		break;
	case PERF_RECORD_MISC_GUEST_KERNEL:
		he_stat->period_guest_sys += period;
		break;
	case PERF_RECORD_MISC_GUEST_USER:
		he_stat->period_guest_us += period;
>>>>>>> v3.18
		break;
	default:
		break;
	}
}

static void he_stat__add_period(struct he_stat *he_stat, u64 period,
				u64 weight)
{

	he_stat->period		+= period;
	he_stat->weight		+= weight;
	he_stat->nr_events	+= 1;
}

static void he_stat__add_stat(struct he_stat *dest, struct he_stat *src)
{
	dest->period		+= src->period;
	dest->period_sys	+= src->period_sys;
	dest->period_us		+= src->period_us;
	dest->period_guest_sys	+= src->period_guest_sys;
	dest->period_guest_us	+= src->period_guest_us;
	dest->nr_events		+= src->nr_events;
	dest->weight		+= src->weight;
}

<<<<<<< HEAD
static void hist_entry__decay(struct hist_entry *he)
{
	he->stat.period = (he->stat.period * 7) / 8;
	he->stat.nr_events = (he->stat.nr_events * 7) / 8;
=======
static void he_stat__decay(struct he_stat *he_stat)
{
	he_stat->period = (he_stat->period * 7) / 8;
	he_stat->nr_events = (he_stat->nr_events * 7) / 8;
>>>>>>> v3.18
	/* XXX need decay for weight too? */
}

static bool hists__decay_entry(struct hists *hists, struct hist_entry *he)
{
	u64 prev_period = he->stat.period;
<<<<<<< HEAD
=======
	u64 diff;
>>>>>>> v3.18

	if (prev_period == 0)
		return true;

<<<<<<< HEAD
	hist_entry__decay(he);

	if (!he->filtered)
		hists->stats.total_period -= prev_period - he->stat.period;
=======
	he_stat__decay(&he->stat);
	if (symbol_conf.cumulate_callchain)
		he_stat__decay(he->stat_acc);

	diff = prev_period - he->stat.period;

	hists->stats.total_period -= diff;
	if (!he->filtered)
		hists->stats.total_non_filtered_period -= diff;
>>>>>>> v3.18

	return he->stat.period == 0;
}

<<<<<<< HEAD
static void __hists__decay_entries(struct hists *hists, bool zap_user,
				   bool zap_kernel, bool threaded)
=======
void hists__decay_entries(struct hists *hists, bool zap_user, bool zap_kernel)
>>>>>>> v3.18
{
	struct rb_node *next = rb_first(&hists->entries);
	struct hist_entry *n;

	while (next) {
		n = rb_entry(next, struct hist_entry, rb_node);
		next = rb_next(&n->rb_node);
		/*
		 * We may be annotating this, for instance, so keep it here in
		 * case some it gets new samples, we'll eventually free it when
		 * the user stops browsing and it agains gets fully decayed.
		 */
		if (((zap_user && n->level == '.') ||
		     (zap_kernel && n->level != '.') ||
		     hists__decay_entry(hists, n)) &&
		    !n->used) {
			rb_erase(&n->rb_node, &hists->entries);

<<<<<<< HEAD
			if (sort__need_collapse || threaded)
				rb_erase(&n->rb_node_in, &hists->entries_collapsed);

			hist_entry__free(n);
			--hists->nr_entries;
=======
			if (sort__need_collapse)
				rb_erase(&n->rb_node_in, &hists->entries_collapsed);

			--hists->nr_entries;
			if (!n->filtered)
				--hists->nr_non_filtered_entries;

			hist_entry__free(n);
>>>>>>> v3.18
		}
	}
}

<<<<<<< HEAD
void hists__decay_entries(struct hists *hists, bool zap_user, bool zap_kernel)
{
	return __hists__decay_entries(hists, zap_user, zap_kernel, false);
}

void hists__decay_entries_threaded(struct hists *hists,
				   bool zap_user, bool zap_kernel)
{
	return __hists__decay_entries(hists, zap_user, zap_kernel, true);
=======
void hists__delete_entries(struct hists *hists)
{
	struct rb_node *next = rb_first(&hists->entries);
	struct hist_entry *n;

	while (next) {
		n = rb_entry(next, struct hist_entry, rb_node);
		next = rb_next(&n->rb_node);

		rb_erase(&n->rb_node, &hists->entries);

		if (sort__need_collapse)
			rb_erase(&n->rb_node_in, &hists->entries_collapsed);

		--hists->nr_entries;
		if (!n->filtered)
			--hists->nr_non_filtered_entries;

		hist_entry__free(n);
	}
>>>>>>> v3.18
}

/*
 * histogram, sorted on item, collects periods
 */

<<<<<<< HEAD
static struct hist_entry *hist_entry__new(struct hist_entry *template)
{
	size_t callchain_size = symbol_conf.use_callchain ? sizeof(struct callchain_root) : 0;
	struct hist_entry *he = zalloc(sizeof(*he) + callchain_size);
=======
static struct hist_entry *hist_entry__new(struct hist_entry *template,
					  bool sample_self)
{
	size_t callchain_size = 0;
	struct hist_entry *he;

	if (symbol_conf.use_callchain || symbol_conf.cumulate_callchain)
		callchain_size = sizeof(struct callchain_root);

	he = zalloc(sizeof(*he) + callchain_size);
>>>>>>> v3.18

	if (he != NULL) {
		*he = *template;

<<<<<<< HEAD
=======
		if (symbol_conf.cumulate_callchain) {
			he->stat_acc = malloc(sizeof(he->stat));
			if (he->stat_acc == NULL) {
				free(he);
				return NULL;
			}
			memcpy(he->stat_acc, &he->stat, sizeof(he->stat));
			if (!sample_self)
				memset(&he->stat, 0, sizeof(he->stat));
		}

>>>>>>> v3.18
		if (he->ms.map)
			he->ms.map->referenced = true;

		if (he->branch_info) {
<<<<<<< HEAD
=======
			/*
			 * This branch info is (a part of) allocated from
			 * sample__resolve_bstack() and will be freed after
			 * adding new entries.  So we need to save a copy.
			 */
			he->branch_info = malloc(sizeof(*he->branch_info));
			if (he->branch_info == NULL) {
				free(he->stat_acc);
				free(he);
				return NULL;
			}

			memcpy(he->branch_info, template->branch_info,
			       sizeof(*he->branch_info));

>>>>>>> v3.18
			if (he->branch_info->from.map)
				he->branch_info->from.map->referenced = true;
			if (he->branch_info->to.map)
				he->branch_info->to.map->referenced = true;
		}

		if (he->mem_info) {
			if (he->mem_info->iaddr.map)
				he->mem_info->iaddr.map->referenced = true;
			if (he->mem_info->daddr.map)
				he->mem_info->daddr.map->referenced = true;
		}

		if (symbol_conf.use_callchain)
			callchain_init(he->callchain);

		INIT_LIST_HEAD(&he->pairs.node);
	}

	return he;
}

<<<<<<< HEAD
void hists__inc_nr_entries(struct hists *hists, struct hist_entry *h)
{
	if (!h->filtered) {
		hists__calc_col_len(hists, h);
		++hists->nr_entries;
		hists->stats.total_period += h->stat.period;
	}
}

=======
>>>>>>> v3.18
static u8 symbol__parent_filter(const struct symbol *parent)
{
	if (symbol_conf.exclude_other && parent == NULL)
		return 1 << HIST_FILTER__PARENT;
	return 0;
}

static struct hist_entry *add_hist_entry(struct hists *hists,
<<<<<<< HEAD
				      struct hist_entry *entry,
				      struct addr_location *al,
				      u64 period,
				      u64 weight)
=======
					 struct hist_entry *entry,
					 struct addr_location *al,
					 bool sample_self)
>>>>>>> v3.18
{
	struct rb_node **p;
	struct rb_node *parent = NULL;
	struct hist_entry *he;
<<<<<<< HEAD
	int cmp;

	pthread_mutex_lock(&hists->lock);
=======
	int64_t cmp;
	u64 period = entry->stat.period;
	u64 weight = entry->stat.weight;
>>>>>>> v3.18

	p = &hists->entries_in->rb_node;

	while (*p != NULL) {
		parent = *p;
		he = rb_entry(parent, struct hist_entry, rb_node_in);

		/*
		 * Make sure that it receives arguments in a same order as
		 * hist_entry__collapse() so that we can use an appropriate
		 * function when searching an entry regardless which sort
		 * keys were used.
		 */
		cmp = hist_entry__cmp(he, entry);

		if (!cmp) {
<<<<<<< HEAD
			he_stat__add_period(&he->stat, period, weight);
=======
			if (sample_self)
				he_stat__add_period(&he->stat, period, weight);
			if (symbol_conf.cumulate_callchain)
				he_stat__add_period(he->stat_acc, period, weight);

			/*
			 * This mem info was allocated from sample__resolve_mem
			 * and will not be used anymore.
			 */
			zfree(&entry->mem_info);
>>>>>>> v3.18

			/* If the map of an existing hist_entry has
			 * become out-of-date due to an exec() or
			 * similar, update it.  Otherwise we will
			 * mis-adjust symbol addresses when computing
			 * the history counter to increment.
			 */
			if (he->ms.map != entry->ms.map) {
				he->ms.map = entry->ms.map;
				if (he->ms.map)
					he->ms.map->referenced = true;
			}
			goto out;
		}

		if (cmp < 0)
			p = &(*p)->rb_left;
		else
			p = &(*p)->rb_right;
	}

<<<<<<< HEAD
	he = hist_entry__new(entry);
	if (!he)
		goto out_unlock;
=======
	he = hist_entry__new(entry, sample_self);
	if (!he)
		return NULL;
>>>>>>> v3.18

	rb_link_node(&he->rb_node_in, parent, p);
	rb_insert_color(&he->rb_node_in, hists->entries_in);
out:
<<<<<<< HEAD
	hist_entry__add_cpumode_period(he, al->cpumode, period);
out_unlock:
	pthread_mutex_unlock(&hists->lock);
	return he;
}

struct hist_entry *__hists__add_mem_entry(struct hists *self,
					  struct addr_location *al,
					  struct symbol *sym_parent,
					  struct mem_info *mi,
					  u64 period,
					  u64 weight)
{
	struct hist_entry entry = {
		.thread	= al->thread,
=======
	if (sample_self)
		he_stat__add_cpumode_period(&he->stat, al->cpumode, period);
	if (symbol_conf.cumulate_callchain)
		he_stat__add_cpumode_period(he->stat_acc, al->cpumode, period);
	return he;
}

struct hist_entry *__hists__add_entry(struct hists *hists,
				      struct addr_location *al,
				      struct symbol *sym_parent,
				      struct branch_info *bi,
				      struct mem_info *mi,
				      u64 period, u64 weight, u64 transaction,
				      bool sample_self)
{
	struct hist_entry entry = {
		.thread	= al->thread,
		.comm = thread__comm(al->thread),
>>>>>>> v3.18
		.ms = {
			.map	= al->map,
			.sym	= al->sym,
		},
<<<<<<< HEAD
		.stat = {
			.period	= period,
			.weight = weight,
			.nr_events = 1,
		},
		.cpu	= al->cpu,
		.ip	= al->addr,
		.level	= al->level,
		.parent = sym_parent,
		.filtered = symbol__parent_filter(sym_parent),
		.hists = self,
		.mem_info = mi,
		.branch_info = NULL,
	};
	return add_hist_entry(self, &entry, al, period, weight);
}

struct hist_entry *__hists__add_branch_entry(struct hists *self,
					     struct addr_location *al,
					     struct symbol *sym_parent,
					     struct branch_info *bi,
					     u64 period,
					     u64 weight)
{
	struct hist_entry entry = {
		.thread	= al->thread,
		.ms = {
			.map	= bi->to.map,
			.sym	= bi->to.sym,
		},
		.cpu	= al->cpu,
		.ip	= bi->to.addr,
		.level	= al->level,
		.stat = {
			.period	= period,
			.nr_events = 1,
			.weight = weight,
		},
		.parent = sym_parent,
		.filtered = symbol__parent_filter(sym_parent),
		.branch_info = bi,
		.hists	= self,
		.mem_info = NULL,
	};

	return add_hist_entry(self, &entry, al, period, weight);
}

struct hist_entry *__hists__add_entry(struct hists *self,
				      struct addr_location *al,
				      struct symbol *sym_parent, u64 period,
				      u64 weight)
{
	struct hist_entry entry = {
		.thread	= al->thread,
		.ms = {
			.map	= al->map,
			.sym	= al->sym,
		},
		.cpu	= al->cpu,
		.ip	= al->addr,
		.level	= al->level,
		.stat = {
			.period	= period,
			.nr_events = 1,
			.weight = weight,
		},
		.parent = sym_parent,
		.filtered = symbol__parent_filter(sym_parent),
		.hists	= self,
		.branch_info = NULL,
		.mem_info = NULL,
	};

	return add_hist_entry(self, &entry, al, period, weight);
=======
		.cpu	 = al->cpu,
		.cpumode = al->cpumode,
		.ip	 = al->addr,
		.level	 = al->level,
		.stat = {
			.nr_events = 1,
			.period	= period,
			.weight = weight,
		},
		.parent = sym_parent,
		.filtered = symbol__parent_filter(sym_parent) | al->filtered,
		.hists	= hists,
		.branch_info = bi,
		.mem_info = mi,
		.transaction = transaction,
	};

	return add_hist_entry(hists, &entry, al, sample_self);
}

static int
iter_next_nop_entry(struct hist_entry_iter *iter __maybe_unused,
		    struct addr_location *al __maybe_unused)
{
	return 0;
}

static int
iter_add_next_nop_entry(struct hist_entry_iter *iter __maybe_unused,
			struct addr_location *al __maybe_unused)
{
	return 0;
}

static int
iter_prepare_mem_entry(struct hist_entry_iter *iter, struct addr_location *al)
{
	struct perf_sample *sample = iter->sample;
	struct mem_info *mi;

	mi = sample__resolve_mem(sample, al);
	if (mi == NULL)
		return -ENOMEM;

	iter->priv = mi;
	return 0;
}

static int
iter_add_single_mem_entry(struct hist_entry_iter *iter, struct addr_location *al)
{
	u64 cost;
	struct mem_info *mi = iter->priv;
	struct hists *hists = evsel__hists(iter->evsel);
	struct hist_entry *he;

	if (mi == NULL)
		return -EINVAL;

	cost = iter->sample->weight;
	if (!cost)
		cost = 1;

	/*
	 * must pass period=weight in order to get the correct
	 * sorting from hists__collapse_resort() which is solely
	 * based on periods. We want sorting be done on nr_events * weight
	 * and this is indirectly achieved by passing period=weight here
	 * and the he_stat__add_period() function.
	 */
	he = __hists__add_entry(hists, al, iter->parent, NULL, mi,
				cost, cost, 0, true);
	if (!he)
		return -ENOMEM;

	iter->he = he;
	return 0;
}

static int
iter_finish_mem_entry(struct hist_entry_iter *iter,
		      struct addr_location *al __maybe_unused)
{
	struct perf_evsel *evsel = iter->evsel;
	struct hists *hists = evsel__hists(evsel);
	struct hist_entry *he = iter->he;
	int err = -EINVAL;

	if (he == NULL)
		goto out;

	hists__inc_nr_samples(hists, he->filtered);

	err = hist_entry__append_callchain(he, iter->sample);

out:
	/*
	 * We don't need to free iter->priv (mem_info) here since
	 * the mem info was either already freed in add_hist_entry() or
	 * passed to a new hist entry by hist_entry__new().
	 */
	iter->priv = NULL;

	iter->he = NULL;
	return err;
}

static int
iter_prepare_branch_entry(struct hist_entry_iter *iter, struct addr_location *al)
{
	struct branch_info *bi;
	struct perf_sample *sample = iter->sample;

	bi = sample__resolve_bstack(sample, al);
	if (!bi)
		return -ENOMEM;

	iter->curr = 0;
	iter->total = sample->branch_stack->nr;

	iter->priv = bi;
	return 0;
}

static int
iter_add_single_branch_entry(struct hist_entry_iter *iter __maybe_unused,
			     struct addr_location *al __maybe_unused)
{
	/* to avoid calling callback function */
	iter->he = NULL;

	return 0;
}

static int
iter_next_branch_entry(struct hist_entry_iter *iter, struct addr_location *al)
{
	struct branch_info *bi = iter->priv;
	int i = iter->curr;

	if (bi == NULL)
		return 0;

	if (iter->curr >= iter->total)
		return 0;

	al->map = bi[i].to.map;
	al->sym = bi[i].to.sym;
	al->addr = bi[i].to.addr;
	return 1;
}

static int
iter_add_next_branch_entry(struct hist_entry_iter *iter, struct addr_location *al)
{
	struct branch_info *bi;
	struct perf_evsel *evsel = iter->evsel;
	struct hists *hists = evsel__hists(evsel);
	struct hist_entry *he = NULL;
	int i = iter->curr;
	int err = 0;

	bi = iter->priv;

	if (iter->hide_unresolved && !(bi[i].from.sym && bi[i].to.sym))
		goto out;

	/*
	 * The report shows the percentage of total branches captured
	 * and not events sampled. Thus we use a pseudo period of 1.
	 */
	he = __hists__add_entry(hists, al, iter->parent, &bi[i], NULL,
				1, 1, 0, true);
	if (he == NULL)
		return -ENOMEM;

	hists__inc_nr_samples(hists, he->filtered);

out:
	iter->he = he;
	iter->curr++;
	return err;
}

static int
iter_finish_branch_entry(struct hist_entry_iter *iter,
			 struct addr_location *al __maybe_unused)
{
	zfree(&iter->priv);
	iter->he = NULL;

	return iter->curr >= iter->total ? 0 : -1;
}

static int
iter_prepare_normal_entry(struct hist_entry_iter *iter __maybe_unused,
			  struct addr_location *al __maybe_unused)
{
	return 0;
}

static int
iter_add_single_normal_entry(struct hist_entry_iter *iter, struct addr_location *al)
{
	struct perf_evsel *evsel = iter->evsel;
	struct perf_sample *sample = iter->sample;
	struct hist_entry *he;

	he = __hists__add_entry(evsel__hists(evsel), al, iter->parent, NULL, NULL,
				sample->period, sample->weight,
				sample->transaction, true);
	if (he == NULL)
		return -ENOMEM;

	iter->he = he;
	return 0;
}

static int
iter_finish_normal_entry(struct hist_entry_iter *iter,
			 struct addr_location *al __maybe_unused)
{
	struct hist_entry *he = iter->he;
	struct perf_evsel *evsel = iter->evsel;
	struct perf_sample *sample = iter->sample;

	if (he == NULL)
		return 0;

	iter->he = NULL;

	hists__inc_nr_samples(evsel__hists(evsel), he->filtered);

	return hist_entry__append_callchain(he, sample);
}

static int
iter_prepare_cumulative_entry(struct hist_entry_iter *iter __maybe_unused,
			      struct addr_location *al __maybe_unused)
{
	struct hist_entry **he_cache;

	callchain_cursor_commit(&callchain_cursor);

	/*
	 * This is for detecting cycles or recursions so that they're
	 * cumulated only one time to prevent entries more than 100%
	 * overhead.
	 */
	he_cache = malloc(sizeof(*he_cache) * (PERF_MAX_STACK_DEPTH + 1));
	if (he_cache == NULL)
		return -ENOMEM;

	iter->priv = he_cache;
	iter->curr = 0;

	return 0;
}

static int
iter_add_single_cumulative_entry(struct hist_entry_iter *iter,
				 struct addr_location *al)
{
	struct perf_evsel *evsel = iter->evsel;
	struct hists *hists = evsel__hists(evsel);
	struct perf_sample *sample = iter->sample;
	struct hist_entry **he_cache = iter->priv;
	struct hist_entry *he;
	int err = 0;

	he = __hists__add_entry(hists, al, iter->parent, NULL, NULL,
				sample->period, sample->weight,
				sample->transaction, true);
	if (he == NULL)
		return -ENOMEM;

	iter->he = he;
	he_cache[iter->curr++] = he;

	callchain_append(he->callchain, &callchain_cursor, sample->period);

	/*
	 * We need to re-initialize the cursor since callchain_append()
	 * advanced the cursor to the end.
	 */
	callchain_cursor_commit(&callchain_cursor);

	hists__inc_nr_samples(hists, he->filtered);

	return err;
}

static int
iter_next_cumulative_entry(struct hist_entry_iter *iter,
			   struct addr_location *al)
{
	struct callchain_cursor_node *node;

	node = callchain_cursor_current(&callchain_cursor);
	if (node == NULL)
		return 0;

	return fill_callchain_info(al, node, iter->hide_unresolved);
}

static int
iter_add_next_cumulative_entry(struct hist_entry_iter *iter,
			       struct addr_location *al)
{
	struct perf_evsel *evsel = iter->evsel;
	struct perf_sample *sample = iter->sample;
	struct hist_entry **he_cache = iter->priv;
	struct hist_entry *he;
	struct hist_entry he_tmp = {
		.cpu = al->cpu,
		.thread = al->thread,
		.comm = thread__comm(al->thread),
		.ip = al->addr,
		.ms = {
			.map = al->map,
			.sym = al->sym,
		},
		.parent = iter->parent,
	};
	int i;
	struct callchain_cursor cursor;

	callchain_cursor_snapshot(&cursor, &callchain_cursor);

	callchain_cursor_advance(&callchain_cursor);

	/*
	 * Check if there's duplicate entries in the callchain.
	 * It's possible that it has cycles or recursive calls.
	 */
	for (i = 0; i < iter->curr; i++) {
		if (hist_entry__cmp(he_cache[i], &he_tmp) == 0) {
			/* to avoid calling callback function */
			iter->he = NULL;
			return 0;
		}
	}

	he = __hists__add_entry(evsel__hists(evsel), al, iter->parent, NULL, NULL,
				sample->period, sample->weight,
				sample->transaction, false);
	if (he == NULL)
		return -ENOMEM;

	iter->he = he;
	he_cache[iter->curr++] = he;

	callchain_append(he->callchain, &cursor, sample->period);
	return 0;
}

static int
iter_finish_cumulative_entry(struct hist_entry_iter *iter,
			     struct addr_location *al __maybe_unused)
{
	zfree(&iter->priv);
	iter->he = NULL;

	return 0;
}

const struct hist_iter_ops hist_iter_mem = {
	.prepare_entry 		= iter_prepare_mem_entry,
	.add_single_entry 	= iter_add_single_mem_entry,
	.next_entry 		= iter_next_nop_entry,
	.add_next_entry 	= iter_add_next_nop_entry,
	.finish_entry 		= iter_finish_mem_entry,
};

const struct hist_iter_ops hist_iter_branch = {
	.prepare_entry 		= iter_prepare_branch_entry,
	.add_single_entry 	= iter_add_single_branch_entry,
	.next_entry 		= iter_next_branch_entry,
	.add_next_entry 	= iter_add_next_branch_entry,
	.finish_entry 		= iter_finish_branch_entry,
};

const struct hist_iter_ops hist_iter_normal = {
	.prepare_entry 		= iter_prepare_normal_entry,
	.add_single_entry 	= iter_add_single_normal_entry,
	.next_entry 		= iter_next_nop_entry,
	.add_next_entry 	= iter_add_next_nop_entry,
	.finish_entry 		= iter_finish_normal_entry,
};

const struct hist_iter_ops hist_iter_cumulative = {
	.prepare_entry 		= iter_prepare_cumulative_entry,
	.add_single_entry 	= iter_add_single_cumulative_entry,
	.next_entry 		= iter_next_cumulative_entry,
	.add_next_entry 	= iter_add_next_cumulative_entry,
	.finish_entry 		= iter_finish_cumulative_entry,
};

int hist_entry_iter__add(struct hist_entry_iter *iter, struct addr_location *al,
			 struct perf_evsel *evsel, struct perf_sample *sample,
			 int max_stack_depth, void *arg)
{
	int err, err2;

	err = sample__resolve_callchain(sample, &iter->parent, evsel, al,
					max_stack_depth);
	if (err)
		return err;

	iter->evsel = evsel;
	iter->sample = sample;

	err = iter->ops->prepare_entry(iter, al);
	if (err)
		goto out;

	err = iter->ops->add_single_entry(iter, al);
	if (err)
		goto out;

	if (iter->he && iter->add_entry_cb) {
		err = iter->add_entry_cb(iter, al, true, arg);
		if (err)
			goto out;
	}

	while (iter->ops->next_entry(iter, al)) {
		err = iter->ops->add_next_entry(iter, al);
		if (err)
			break;

		if (iter->he && iter->add_entry_cb) {
			err = iter->add_entry_cb(iter, al, false, arg);
			if (err)
				goto out;
		}
	}

out:
	err2 = iter->ops->finish_entry(iter, al);
	if (!err)
		err = err2;

	return err;
>>>>>>> v3.18
}

int64_t
hist_entry__cmp(struct hist_entry *left, struct hist_entry *right)
{
<<<<<<< HEAD
	struct sort_entry *se;
	int64_t cmp = 0;

	list_for_each_entry(se, &hist_entry__sort_list, list) {
		cmp = se->se_cmp(left, right);
=======
	struct perf_hpp_fmt *fmt;
	int64_t cmp = 0;

	perf_hpp__for_each_sort_list(fmt) {
		if (perf_hpp__should_skip(fmt))
			continue;

		cmp = fmt->cmp(left, right);
>>>>>>> v3.18
		if (cmp)
			break;
	}

	return cmp;
}

int64_t
hist_entry__collapse(struct hist_entry *left, struct hist_entry *right)
{
<<<<<<< HEAD
	struct sort_entry *se;
	int64_t cmp = 0;

	list_for_each_entry(se, &hist_entry__sort_list, list) {
		int64_t (*f)(struct hist_entry *, struct hist_entry *);

		f = se->se_collapse ?: se->se_cmp;

		cmp = f(left, right);
=======
	struct perf_hpp_fmt *fmt;
	int64_t cmp = 0;

	perf_hpp__for_each_sort_list(fmt) {
		if (perf_hpp__should_skip(fmt))
			continue;

		cmp = fmt->collapse(left, right);
>>>>>>> v3.18
		if (cmp)
			break;
	}

	return cmp;
}

void hist_entry__free(struct hist_entry *he)
{
<<<<<<< HEAD
	free(he->branch_info);
	free(he->mem_info);
=======
	zfree(&he->branch_info);
	zfree(&he->mem_info);
	zfree(&he->stat_acc);
	free_srcline(he->srcline);
>>>>>>> v3.18
	free(he);
}

/*
 * collapse the histogram
 */

static bool hists__collapse_insert_entry(struct hists *hists __maybe_unused,
					 struct rb_root *root,
					 struct hist_entry *he)
{
	struct rb_node **p = &root->rb_node;
	struct rb_node *parent = NULL;
	struct hist_entry *iter;
	int64_t cmp;

	while (*p != NULL) {
		parent = *p;
		iter = rb_entry(parent, struct hist_entry, rb_node_in);

		cmp = hist_entry__collapse(iter, he);

		if (!cmp) {
			he_stat__add_stat(&iter->stat, &he->stat);
<<<<<<< HEAD
=======
			if (symbol_conf.cumulate_callchain)
				he_stat__add_stat(iter->stat_acc, he->stat_acc);
>>>>>>> v3.18

			if (symbol_conf.use_callchain) {
				callchain_cursor_reset(&callchain_cursor);
				callchain_merge(&callchain_cursor,
						iter->callchain,
						he->callchain);
			}
			hist_entry__free(he);
			return false;
		}

		if (cmp < 0)
			p = &(*p)->rb_left;
		else
			p = &(*p)->rb_right;
	}

	rb_link_node(&he->rb_node_in, parent, p);
	rb_insert_color(&he->rb_node_in, root);
	return true;
}

static struct rb_root *hists__get_rotate_entries_in(struct hists *hists)
{
	struct rb_root *root;

	pthread_mutex_lock(&hists->lock);

	root = hists->entries_in;
	if (++hists->entries_in > &hists->entries_in_array[1])
		hists->entries_in = &hists->entries_in_array[0];

	pthread_mutex_unlock(&hists->lock);

	return root;
}

static void hists__apply_filters(struct hists *hists, struct hist_entry *he)
{
	hists__filter_entry_by_dso(hists, he);
	hists__filter_entry_by_thread(hists, he);
	hists__filter_entry_by_symbol(hists, he);
}

<<<<<<< HEAD
static void __hists__collapse_resort(struct hists *hists, bool threaded)
=======
void hists__collapse_resort(struct hists *hists, struct ui_progress *prog)
>>>>>>> v3.18
{
	struct rb_root *root;
	struct rb_node *next;
	struct hist_entry *n;

<<<<<<< HEAD
	if (!sort__need_collapse && !threaded)
=======
	if (!sort__need_collapse)
>>>>>>> v3.18
		return;

	root = hists__get_rotate_entries_in(hists);
	next = rb_first(root);

	while (next) {
<<<<<<< HEAD
=======
		if (session_done())
			break;
>>>>>>> v3.18
		n = rb_entry(next, struct hist_entry, rb_node_in);
		next = rb_next(&n->rb_node_in);

		rb_erase(&n->rb_node_in, root);
		if (hists__collapse_insert_entry(hists, &hists->entries_collapsed, n)) {
			/*
			 * If it wasn't combined with one of the entries already
			 * collapsed, we need to apply the filters that may have
			 * been set by, say, the hist_browser.
			 */
			hists__apply_filters(hists, n);
		}
<<<<<<< HEAD
	}
}

void hists__collapse_resort(struct hists *hists)
{
	return __hists__collapse_resort(hists, false);
}

void hists__collapse_resort_threaded(struct hists *hists)
{
	return __hists__collapse_resort(hists, true);
}

/*
 * reverse the map, sort on period.
 */

static int period_cmp(u64 period_a, u64 period_b)
{
	if (period_a > period_b)
		return 1;
	if (period_a < period_b)
		return -1;
	return 0;
}

static int hist_entry__sort_on_period(struct hist_entry *a,
				      struct hist_entry *b)
{
	int ret;
	int i, nr_members;
	struct perf_evsel *evsel;
	struct hist_entry *pair;
	u64 *periods_a, *periods_b;

	ret = period_cmp(a->stat.period, b->stat.period);
	if (ret || !symbol_conf.event_group)
		return ret;

	evsel = hists_to_evsel(a->hists);
	nr_members = evsel->nr_members;
	if (nr_members <= 1)
		return ret;

	periods_a = zalloc(sizeof(periods_a) * nr_members);
	periods_b = zalloc(sizeof(periods_b) * nr_members);

	if (!periods_a || !periods_b)
		goto out;

	list_for_each_entry(pair, &a->pairs.head, pairs.node) {
		evsel = hists_to_evsel(pair->hists);
		periods_a[perf_evsel__group_idx(evsel)] = pair->stat.period;
	}

	list_for_each_entry(pair, &b->pairs.head, pairs.node) {
		evsel = hists_to_evsel(pair->hists);
		periods_b[perf_evsel__group_idx(evsel)] = pair->stat.period;
	}

	for (i = 1; i < nr_members; i++) {
		ret = period_cmp(periods_a[i], periods_b[i]);
		if (ret)
			break;
	}

out:
	free(periods_a);
	free(periods_b);

	return ret;
=======
		if (prog)
			ui_progress__update(prog, 1);
	}
}

static int hist_entry__sort(struct hist_entry *a, struct hist_entry *b)
{
	struct perf_hpp_fmt *fmt;
	int64_t cmp = 0;

	perf_hpp__for_each_sort_list(fmt) {
		if (perf_hpp__should_skip(fmt))
			continue;

		cmp = fmt->sort(a, b);
		if (cmp)
			break;
	}

	return cmp;
}

static void hists__reset_filter_stats(struct hists *hists)
{
	hists->nr_non_filtered_entries = 0;
	hists->stats.total_non_filtered_period = 0;
}

void hists__reset_stats(struct hists *hists)
{
	hists->nr_entries = 0;
	hists->stats.total_period = 0;

	hists__reset_filter_stats(hists);
}

static void hists__inc_filter_stats(struct hists *hists, struct hist_entry *h)
{
	hists->nr_non_filtered_entries++;
	hists->stats.total_non_filtered_period += h->stat.period;
}

void hists__inc_stats(struct hists *hists, struct hist_entry *h)
{
	if (!h->filtered)
		hists__inc_filter_stats(hists, h);

	hists->nr_entries++;
	hists->stats.total_period += h->stat.period;
>>>>>>> v3.18
}

static void __hists__insert_output_entry(struct rb_root *entries,
					 struct hist_entry *he,
					 u64 min_callchain_hits)
{
	struct rb_node **p = &entries->rb_node;
	struct rb_node *parent = NULL;
	struct hist_entry *iter;

	if (symbol_conf.use_callchain)
		callchain_param.sort(&he->sorted_chain, he->callchain,
				      min_callchain_hits, &callchain_param);

	while (*p != NULL) {
		parent = *p;
		iter = rb_entry(parent, struct hist_entry, rb_node);

<<<<<<< HEAD
		if (hist_entry__sort_on_period(he, iter) > 0)
=======
		if (hist_entry__sort(he, iter) > 0)
>>>>>>> v3.18
			p = &(*p)->rb_left;
		else
			p = &(*p)->rb_right;
	}

	rb_link_node(&he->rb_node, parent, p);
	rb_insert_color(&he->rb_node, entries);
}

<<<<<<< HEAD
static void __hists__output_resort(struct hists *hists, bool threaded)
=======
void hists__output_resort(struct hists *hists)
>>>>>>> v3.18
{
	struct rb_root *root;
	struct rb_node *next;
	struct hist_entry *n;
	u64 min_callchain_hits;

	min_callchain_hits = hists->stats.total_period * (callchain_param.min_percent / 100);

<<<<<<< HEAD
	if (sort__need_collapse || threaded)
=======
	if (sort__need_collapse)
>>>>>>> v3.18
		root = &hists->entries_collapsed;
	else
		root = hists->entries_in;

	next = rb_first(root);
	hists->entries = RB_ROOT;

<<<<<<< HEAD
	hists->nr_entries = 0;
	hists->stats.total_period = 0;
=======
	hists__reset_stats(hists);
>>>>>>> v3.18
	hists__reset_col_len(hists);

	while (next) {
		n = rb_entry(next, struct hist_entry, rb_node_in);
		next = rb_next(&n->rb_node_in);

		__hists__insert_output_entry(&hists->entries, n, min_callchain_hits);
<<<<<<< HEAD
		hists__inc_nr_entries(hists, n);
	}
}

void hists__output_resort(struct hists *hists)
{
	return __hists__output_resort(hists, false);
}

void hists__output_resort_threaded(struct hists *hists)
{
	return __hists__output_resort(hists, true);
=======
		hists__inc_stats(hists, n);

		if (!n->filtered)
			hists__calc_col_len(hists, n);
	}
>>>>>>> v3.18
}

static void hists__remove_entry_filter(struct hists *hists, struct hist_entry *h,
				       enum hist_filter filter)
{
	h->filtered &= ~(1 << filter);
	if (h->filtered)
		return;

<<<<<<< HEAD
	++hists->nr_entries;
	if (h->ms.unfolded)
		hists->nr_entries += h->nr_rows;
	h->row_offset = 0;
	hists->stats.total_period += h->stat.period;
	hists->stats.nr_events[PERF_RECORD_SAMPLE] += h->stat.nr_events;

=======
	/* force fold unfiltered entry for simplicity */
	h->ms.unfolded = false;
	h->row_offset = 0;

	hists->stats.nr_non_filtered_samples += h->stat.nr_events;

	hists__inc_filter_stats(hists, h);
>>>>>>> v3.18
	hists__calc_col_len(hists, h);
}


static bool hists__filter_entry_by_dso(struct hists *hists,
				       struct hist_entry *he)
{
	if (hists->dso_filter != NULL &&
	    (he->ms.map == NULL || he->ms.map->dso != hists->dso_filter)) {
		he->filtered |= (1 << HIST_FILTER__DSO);
		return true;
	}

	return false;
}

void hists__filter_by_dso(struct hists *hists)
{
	struct rb_node *nd;

<<<<<<< HEAD
	hists->nr_entries = hists->stats.total_period = 0;
	hists->stats.nr_events[PERF_RECORD_SAMPLE] = 0;
=======
	hists->stats.nr_non_filtered_samples = 0;

	hists__reset_filter_stats(hists);
>>>>>>> v3.18
	hists__reset_col_len(hists);

	for (nd = rb_first(&hists->entries); nd; nd = rb_next(nd)) {
		struct hist_entry *h = rb_entry(nd, struct hist_entry, rb_node);

		if (symbol_conf.exclude_other && !h->parent)
			continue;

		if (hists__filter_entry_by_dso(hists, h))
			continue;

		hists__remove_entry_filter(hists, h, HIST_FILTER__DSO);
	}
}

static bool hists__filter_entry_by_thread(struct hists *hists,
					  struct hist_entry *he)
{
	if (hists->thread_filter != NULL &&
	    he->thread != hists->thread_filter) {
		he->filtered |= (1 << HIST_FILTER__THREAD);
		return true;
	}

	return false;
}

void hists__filter_by_thread(struct hists *hists)
{
	struct rb_node *nd;

<<<<<<< HEAD
	hists->nr_entries = hists->stats.total_period = 0;
	hists->stats.nr_events[PERF_RECORD_SAMPLE] = 0;
=======
	hists->stats.nr_non_filtered_samples = 0;

	hists__reset_filter_stats(hists);
>>>>>>> v3.18
	hists__reset_col_len(hists);

	for (nd = rb_first(&hists->entries); nd; nd = rb_next(nd)) {
		struct hist_entry *h = rb_entry(nd, struct hist_entry, rb_node);

		if (hists__filter_entry_by_thread(hists, h))
			continue;

		hists__remove_entry_filter(hists, h, HIST_FILTER__THREAD);
	}
}

static bool hists__filter_entry_by_symbol(struct hists *hists,
					  struct hist_entry *he)
{
	if (hists->symbol_filter_str != NULL &&
	    (!he->ms.sym || strstr(he->ms.sym->name,
				   hists->symbol_filter_str) == NULL)) {
		he->filtered |= (1 << HIST_FILTER__SYMBOL);
		return true;
	}

	return false;
}

void hists__filter_by_symbol(struct hists *hists)
{
	struct rb_node *nd;

<<<<<<< HEAD
	hists->nr_entries = hists->stats.total_period = 0;
	hists->stats.nr_events[PERF_RECORD_SAMPLE] = 0;
=======
	hists->stats.nr_non_filtered_samples = 0;

	hists__reset_filter_stats(hists);
>>>>>>> v3.18
	hists__reset_col_len(hists);

	for (nd = rb_first(&hists->entries); nd; nd = rb_next(nd)) {
		struct hist_entry *h = rb_entry(nd, struct hist_entry, rb_node);

		if (hists__filter_entry_by_symbol(hists, h))
			continue;

		hists__remove_entry_filter(hists, h, HIST_FILTER__SYMBOL);
	}
}

<<<<<<< HEAD
int hist_entry__inc_addr_samples(struct hist_entry *he, int evidx, u64 ip)
{
	return symbol__inc_addr_samples(he->ms.sym, he->ms.map, evidx, ip);
}

int hist_entry__annotate(struct hist_entry *he, size_t privsize)
{
	return symbol__annotate(he->ms.sym, he->ms.map, privsize);
}

=======
>>>>>>> v3.18
void events_stats__inc(struct events_stats *stats, u32 type)
{
	++stats->nr_events[0];
	++stats->nr_events[type];
}

void hists__inc_nr_events(struct hists *hists, u32 type)
{
	events_stats__inc(&hists->stats, type);
}

<<<<<<< HEAD
=======
void hists__inc_nr_samples(struct hists *hists, bool filtered)
{
	events_stats__inc(&hists->stats, PERF_RECORD_SAMPLE);
	if (!filtered)
		hists->stats.nr_non_filtered_samples++;
}

>>>>>>> v3.18
static struct hist_entry *hists__add_dummy_entry(struct hists *hists,
						 struct hist_entry *pair)
{
	struct rb_root *root;
	struct rb_node **p;
	struct rb_node *parent = NULL;
	struct hist_entry *he;
<<<<<<< HEAD
	int cmp;
=======
	int64_t cmp;
>>>>>>> v3.18

	if (sort__need_collapse)
		root = &hists->entries_collapsed;
	else
		root = hists->entries_in;

	p = &root->rb_node;

	while (*p != NULL) {
		parent = *p;
		he = rb_entry(parent, struct hist_entry, rb_node_in);

		cmp = hist_entry__collapse(he, pair);

		if (!cmp)
			goto out;

		if (cmp < 0)
			p = &(*p)->rb_left;
		else
			p = &(*p)->rb_right;
	}

<<<<<<< HEAD
	he = hist_entry__new(pair);
=======
	he = hist_entry__new(pair, true);
>>>>>>> v3.18
	if (he) {
		memset(&he->stat, 0, sizeof(he->stat));
		he->hists = hists;
		rb_link_node(&he->rb_node_in, parent, p);
		rb_insert_color(&he->rb_node_in, root);
<<<<<<< HEAD
		hists__inc_nr_entries(hists, he);
=======
		hists__inc_stats(hists, he);
		he->dummy = true;
>>>>>>> v3.18
	}
out:
	return he;
}

static struct hist_entry *hists__find_entry(struct hists *hists,
					    struct hist_entry *he)
{
	struct rb_node *n;

	if (sort__need_collapse)
		n = hists->entries_collapsed.rb_node;
	else
		n = hists->entries_in->rb_node;

	while (n) {
		struct hist_entry *iter = rb_entry(n, struct hist_entry, rb_node_in);
		int64_t cmp = hist_entry__collapse(iter, he);

		if (cmp < 0)
			n = n->rb_left;
		else if (cmp > 0)
			n = n->rb_right;
		else
			return iter;
	}

	return NULL;
}

/*
 * Look for pairs to link to the leader buckets (hist_entries):
 */
void hists__match(struct hists *leader, struct hists *other)
{
	struct rb_root *root;
	struct rb_node *nd;
	struct hist_entry *pos, *pair;

	if (sort__need_collapse)
		root = &leader->entries_collapsed;
	else
		root = leader->entries_in;

	for (nd = rb_first(root); nd; nd = rb_next(nd)) {
		pos  = rb_entry(nd, struct hist_entry, rb_node_in);
		pair = hists__find_entry(other, pos);

		if (pair)
			hist_entry__add_pair(pair, pos);
	}
}

/*
 * Look for entries in the other hists that are not present in the leader, if
 * we find them, just add a dummy entry on the leader hists, with period=0,
 * nr_events=0, to serve as the list header.
 */
int hists__link(struct hists *leader, struct hists *other)
{
	struct rb_root *root;
	struct rb_node *nd;
	struct hist_entry *pos, *pair;

	if (sort__need_collapse)
		root = &other->entries_collapsed;
	else
		root = other->entries_in;

	for (nd = rb_first(root); nd; nd = rb_next(nd)) {
		pos = rb_entry(nd, struct hist_entry, rb_node_in);

		if (!hist_entry__has_pairs(pos)) {
			pair = hists__add_dummy_entry(leader, pos);
			if (pair == NULL)
				return -1;
			hist_entry__add_pair(pos, pair);
		}
	}

	return 0;
}
<<<<<<< HEAD
=======


size_t perf_evlist__fprintf_nr_events(struct perf_evlist *evlist, FILE *fp)
{
	struct perf_evsel *pos;
	size_t ret = 0;

	evlist__for_each(evlist, pos) {
		ret += fprintf(fp, "%s stats:\n", perf_evsel__name(pos));
		ret += events_stats__fprintf(&evsel__hists(pos)->stats, fp);
	}

	return ret;
}


u64 hists__total_period(struct hists *hists)
{
	return symbol_conf.filter_relative ? hists->stats.total_non_filtered_period :
		hists->stats.total_period;
}

int parse_filter_percentage(const struct option *opt __maybe_unused,
			    const char *arg, int unset __maybe_unused)
{
	if (!strcmp(arg, "relative"))
		symbol_conf.filter_relative = true;
	else if (!strcmp(arg, "absolute"))
		symbol_conf.filter_relative = false;
	else
		return -1;

	return 0;
}

int perf_hist_config(const char *var, const char *value)
{
	if (!strcmp(var, "hist.percentage"))
		return parse_filter_percentage(NULL, value, 0);

	return 0;
}

static int hists_evsel__init(struct perf_evsel *evsel)
{
	struct hists *hists = evsel__hists(evsel);

	memset(hists, 0, sizeof(*hists));
	hists->entries_in_array[0] = hists->entries_in_array[1] = RB_ROOT;
	hists->entries_in = &hists->entries_in_array[0];
	hists->entries_collapsed = RB_ROOT;
	hists->entries = RB_ROOT;
	pthread_mutex_init(&hists->lock, NULL);
	return 0;
}

/*
 * XXX We probably need a hists_evsel__exit() to free the hist_entries
 * stored in the rbtree...
 */

int hists__init(void)
{
	int err = perf_evsel__object_config(sizeof(struct hists_evsel),
					    hists_evsel__init, NULL);
	if (err)
		fputs("FATAL ERROR: Couldn't setup hists class\n", stderr);

	return err;
}
>>>>>>> v3.18
