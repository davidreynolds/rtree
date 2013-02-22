#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include "nn_heap.h"

#define ROOT_IDX 1

struct nn_heap {
    struct nn_entry **entries;
    unsigned next;
    unsigned length;
};

static int nn_heap_cmp(struct nn_heap *h, unsigned u, unsigned v);
static unsigned nn_heap_parent(unsigned u);
static void nn_heap_children(unsigned u, unsigned *a, unsigned *b);
static void bubble_up(struct nn_heap *h, unsigned u);
static void bubble_down(struct nn_heap *h, unsigned u);
static void nn_heap_swap(struct nn_heap *h, unsigned u, unsigned v);

struct nn_heap *
nn_heap_new(size_t length)
{
    struct nn_heap *heap;
    assert(length > 0);
    heap = calloc(1, sizeof *heap);
    if (!heap) {
        errno = ENOMEM;
        return NULL;
    }
    heap->length = length;
    heap->entries = calloc(heap->length, sizeof *heap->entries);
    if (!heap->entries) {
        errno = ENOMEM;
        free(heap);
        return NULL;
    }
    heap->next = ROOT_IDX;
    return heap;
}

void
nn_heap_free(struct nn_heap *h)
{
    unsigned u;
    for (u = ROOT_IDX; u < h->next; u++) {
        free(h->entries[u]);
    }
    free(h->entries);
    free(h);
}

unsigned
nn_heap_empty(struct nn_heap *h)
{
    assert(h != NULL);
    return h->next <= ROOT_IDX;
}

int
nn_heap_push(struct nn_heap *h, struct nn_entry *e)
{
    unsigned u;
    assert(h != NULL);
    if (h->next >= h->length) {
        u = h->length*2;
        h->entries = realloc(h->entries, u * sizeof *h->entries);
        if (!h->entries) {
            errno = ENOMEM;
            return -errno;
        }
        while (h->length < u)
            h->entries[h->length++] = NULL;
    }
    h->entries[h->next] = e;
    bubble_up(h, h->next);
    h->next++;
    return 0;
}

struct nn_entry *
nn_heap_first(struct nn_heap *h)
{
    assert(h != NULL);
    return h->entries[ROOT_IDX];
}

struct nn_entry *
nn_heap_pop(struct nn_heap *h)
{
    struct nn_entry *e = NULL;
    if (h->next > ROOT_IDX) {
        e = h->entries[ROOT_IDX];
        h->entries[ROOT_IDX] = h->entries[--h->next];
        bubble_down(h, ROOT_IDX);
    }
    return e;
}

static int
nn_heap_cmp(struct nn_heap *h, unsigned u, unsigned v)
{
    assert(h != NULL);
    return h->entries[u]->minDist < h->entries[v]->minDist;
}

static unsigned
nn_heap_parent(unsigned u)
{
    return u / 2;
}

static void
nn_heap_children(unsigned u, unsigned *a, unsigned *b)
{
    *a = u * 2;
    *b = *a + 1;
}

static void
nn_heap_swap(struct nn_heap *h, unsigned u, unsigned v)
{
    struct nn_entry *t;
    assert(h != NULL);
    t = h->entries[u];
    h->entries[u] = h->entries[v];
    h->entries[v] = t;
}

static void
bubble_up(struct nn_heap *h, unsigned u)
{
    unsigned v;
    while (u > ROOT_IDX) {
        v = nn_heap_parent(u);
        if (!nn_heap_cmp(h, u, v))
            break;
        nn_heap_swap(h, u, v);
        u = v;
    }
}

static void
bubble_down(struct nn_heap *h, unsigned u)
{
    unsigned a, b, c;
    while (1) {
        nn_heap_children(u, &a, &b);
        if (a >= h->next)
            break;
        c = nn_heap_cmp(h, a, b) ? a : b;
        if (nn_heap_cmp(h, u, c))
            break;
        nn_heap_swap(h, u, c);
        u = c;
    }
}
