#include <linux/init.h>
#include <linux/interval_tree.h>
#include <linux/interval_tree_generic.h>
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> v3.18

#define START(node) ((node)->start)
#define LAST(node)  ((node)->last)

INTERVAL_TREE_DEFINE(struct interval_tree_node, rb,
		     unsigned long, __subtree_last,
		     START, LAST,, interval_tree)
<<<<<<< HEAD
=======

EXPORT_SYMBOL_GPL(interval_tree_insert);
EXPORT_SYMBOL_GPL(interval_tree_remove);
EXPORT_SYMBOL_GPL(interval_tree_iter_first);
EXPORT_SYMBOL_GPL(interval_tree_iter_next);
>>>>>>> v3.18
