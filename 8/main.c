#include <stdio.h>
#include <stdlib.h>

typedef struct Cat {
    int color;
    int appetite;
    struct Cat* next;
} Cat;

typedef struct AllCatLinkedList {
    Cat* min;
    Cat* max;
} AllCatLinkedList;

typedef struct ColorCatCircularArray {
    int size;
    Cat** cat_ptr;
} ColorCatCircularArray;

typedef struct ColorMapping {
    int color;
    ColorCatCircularArray* cc_c_arr;
} ColorMapping;

typedef struct ColorManager {
    int n_color;
    ColorMapping* cm_arr;
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

/**
 * Methods of All Cat Linked List
 */

/**
 *  Methods of Color Cat Circular Array
 */
void sortColorCat(ColorCatCircularArray* cc_c_arr);

/**
 *  Methods of Color Manager
 */
ColorManager* makeColorManager(int max_size);

void addColor(ColorManager* cm, int color);

void sortColor(ColorManager* cm);

void removeDuplicateColor(ColorManager* cm);

void buildColorCatCircularArray(ColorManager* cm, Cat* all_cats);

ColorCatCircularArray* findColor(ColorManager* cm);

int main()
{
    int N, M, color;
    scanf("%d %d", &N, &M);
    Cat* cat_ptr;
    Cat* all_cats = (Cat*)calloc(N, sizeof(Cat));
    ColorManager* color_manager = makeColorManager(N);

    for (int n = 0; n < N; n++) {
        scanf("%d", &(all_cats[n].appetite));
    }

    for (int n = 0; n < N; n++) {
        scanf("%d", &color);
        all_cats[n].color = color;
        addColor(color_manager, color);
    }

    sortColor(color_manager);
    removeDuplicateColor(color_manager);
    buildColorCatCircularArray(color_manager, all_cats);

    // TODO Sort Color Mapping Array and every Color Cat Circular Array.
}

/**
 *  Define Function
 */

/**
 * Methods of All Cat Linked List
 */

/**
 *  Methods of Color Cat Circular Array
 */
void sortColorCat(ColorCatCircularArray* cc_c_arr)
{
}

/**
 *  Methods of Color Manager
 */
ColorManager* makeColorManager(int max_size)
{
    ColorManager* color_manager = (ColorManager*)calloc(1, sizeof(ColorManager));
    ColorMapping* color_mapping = (ColorMapping*)calloc(max_size, sizeof(ColorMapping));
    color_manager->cm_arr = color_mapping;
}

int cmpColor(const void* c1, const void* c2)
{
    return ((ColorMapping*)c1)->color - ((ColorMapping*)c2)->color;
}

void addColor(ColorManager* cm, int color)
{
    int n = cm->n_color;
    cm->cm_arr[n].color = color;
    cm->n_color = n + 1;
}

void sortColor(ColorManager* cm)
{
    qsort(cm->cm_arr, cm->n_color, sizeof(ColorMapping), cmpColor);
}

void removeDuplicateColor(ColorManager* cm)
{
    int l = 1, r = 1, n_color = cm->n_color, cur_color = cm->cm_arr[0].color;
    ColorMapping* cm_arr = cm->cm_arr;
    while (r < n_color) {
        if (cm_arr[r].color != cur_color) {
            cm_arr[l].color = cm_arr[r].color;
            cur_color = cm_arr[l].color;
            l += 1;
        }
        r += 1;
    }
    cm->n_color = l;
}

void buildColorCatCircularArray(ColorManager* cm, Cat* all_cats)
{
}

ColorCatCircularArray* findColor(ColorManager* cm)
{
}