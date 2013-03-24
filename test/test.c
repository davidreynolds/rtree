#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "rTreeIndex.h"

static double monoseconds(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + 1e-9 * ts.tv_nsec;
}

/** xmin, ymin, xmax, ymax (for 2 dimensional RTree) */
/*
struct Rect rects[] = {
    {0, 0, 2, 2},
    {5, 5, 7, 7},
    {8, 5, 9, 6},
    {7, 1, 9, 2}
};
int nrects = sizeof(rects) / sizeof(rects[0]);
*/
struct Rect *rects;
int nrects = 10000;
/** search will find above rects that this one overlaps */
struct Rect search_rect = {
    {6, 4, 10, 6}
};

int MySearchCallback(int id, void* arg) 
{
    /* Note: -1 to make up for the +1 when data was inserted */
/*    printf("Hit data rect %d\n", id-1);*/
/*
    printf("cb: %f,%f %f,%f\n", rects[id-1].boundary[0], rects[id-1].boundary[1],
           rects[id-1].boundary[2], rects[id-1].boundary[3]);
*/
    return 0;
}

int main()
{
    struct Node* root = RTreeNewIndex();
    int i, nhits;
    double t1, t2;

    srand(1364097750);

    rects = calloc(nrects, sizeof *rects);

    printf("nrects = %d\n", nrects);
    /*
     * Insert all the data rects.
     * Notes about the arguments:
     * parameter 1 is the rect being inserted,
     * parameter 2 is its ID. NOTE: *** ID MUST NEVER BE ZERO ***, hence the +1,
     * parameter 3 is the root of the tree. Note: its address is passed
     * because it can change as a result of this call, therefore no other parts
     * of this code should stash its address since it could change undernieth.
     * parameter 4 is always zero which means to add from the root.
     */

    t1 = monoseconds();
    for(i=0; i<nrects; i++) {
        /* i+1 is rect ID. Note: root can change */
        rects[i].boundary[0] = rand() % 180;
        rects[i].boundary[1] = rand() % 90;
        rects[i].boundary[2] = rects[i].boundary[0] + rand() % 180;
        rects[i].boundary[3] = rects[i].boundary[1] + rand() % 90;
        RTreeInsertRect(&rects[i], i+1, &root, 0);
    }
    t2 = monoseconds();

    printf("load time: %fms\n", (t2 - t1) * 1000);

    t1 = monoseconds();
    nhits = RTreeSearch(root, &rects[100], MySearchCallback, 0);
    t2 = monoseconds();
    printf("time: %fus\n", (t2 - t1) * 1e6);
    printf("Search resulted in %d hits\n", nhits);
    printf("cb: %f,%f %f,%f\n", rects[100].boundary[0], rects[100].boundary[1],
           rects[100].boundary[2], rects[100].boundary[3]);

    return 0;
}
