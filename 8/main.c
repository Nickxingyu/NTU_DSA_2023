#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int color;
    int appetite;
} Cat;

typedef struct {
    Cat min;
    Cat max;
} AllCatLinkedList;

typedef struct {
    int size;
    Cat** cat_ptr;
} ColorCatCircularArray;

typedef struct {
    int color;
    ColorCatCircularArray* cc_c_arr;
} ColorMapping;

typedef struct {
    int n_color;
    ColorMapping cm_arr;
} ColorManager;

/**
 * Operations
 *
 * - AllCatLinkedList
 *      1. swap
 *      2. magic - remove
 *      3. magic - insert to max or min
 *
 * - ColorCatCircularArray
 *      1. sort
 *      2. swap
 *      3. magic
 *      4. binary search on circular array
 *
 * - ColorManager
 *      1. sort
 *      2. binary search on array
 */

int main()
{
    int N, M, color, appetite;
    scanf("%d %d", &N, &M);
    Cat* cat_ptr;
    Cat* all_cat = (Cat*)calloc(N, sizeof(Cat));

    for (int n = 0; n < N; n++) {
        scanf("%d", &(all_cat[n].appetite));
    }

    for (int n = 0; n < N; n++) {
        /**
         * Build Color Manager
         */
    }

    // TODO Sort Color Mapping Array and every Color Cat Circular Array.
}