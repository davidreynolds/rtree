#ifndef __R_TREE_NN_HEAP_HEADER__
#define __R_TREE_NN_HEAP_HEADER__

#include "rTreeIndex.h"

struct nn_entry {
    struct Node *node;
    int hasData;
    double minDist;
};

struct nn_heap;

struct nn_heap *nn_heap_new(size_t length);
void nn_heap_free(struct nn_heap *h);
int nn_heap_push(struct nn_heap *h, struct nn_entry *e);
struct nn_entry *nn_heap_first(struct nn_heap *h);
struct nn_entry *nn_heap_pop(struct nn_heap *h);
unsigned nn_heap_empty(struct nn_heap *h);

#endif
