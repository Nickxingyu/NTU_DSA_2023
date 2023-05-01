#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HIGHEST 1
#define LOWEST 0
#define SUCCESS 1
#define FAILURE 0

typedef struct Cat {
    int idx;
    int color;
    int appetite;
    struct Cat* next;
    struct Cat* pre;
} Cat;

typedef struct CatsLinkedList {
    int size;
    Cat* min;
    Cat* max;
    Cat* arr;
} CatsLinkedList;

typedef struct CatCircularArray {
    int color;
    int size;
    int head;
    Cat** base;
} CatCircularArray;

typedef struct ColorManager {
    int n_color;
    int* cca_idx;
    int* cat_idx_in_cca;
    CatCircularArray* cc_arr;
} ColorManager;

/**
 * Operations
 *
 * - CatsLinkedList
 *      1. swap
 *      2. magic - remove
 *      3. magic - insert to max or min
 *
 * - CatsCircularArray
 *      1. swap
 *      2. magic
 *      3. binary search on circular array
 *
 * - ColorManager
 *      1. binary search on array
 */
void show(CatsLinkedList* c_ll, ColorManager* cm)
{
    Cat* tmp;
    int idx = 0;
    tmp = c_ll->min;
    while (tmp != NULL) {
        printf("%d -> ", tmp->idx);
        tmp = tmp->next;
    }
    printf("NULL\n");

    tmp = c_ll->min;
    while (tmp != NULL) {
        printf("%d -> ", tmp->appetite);
        tmp = tmp->next;
    }
    printf("NULL\n");

    for (int n = 0; n < cm->n_color; n++) {
        printf("Color: %d\n", cm->cc_arr[n].color);
        for (int c = 0; c < cm->cc_arr[n].size; c++) {
            idx = (cm->cc_arr[n].head + c) % cm->cc_arr[n].size;
            printf("%d %d %d\n", idx, cm->cc_arr[n].base[idx]->color, cm->cc_arr[n].base[idx]->idx);
        }
    }
}

void swap_op(CatsLinkedList* c_ll, ColorManager* cm, int idx);
void magic_op(CatsLinkedList* c_ll, ColorManager* cm, int color, int dir, int success);

/**
 *  Methods of Cat
 */

void sortCatsPtr(Cat** all_cat_ptr, int N);

/**
 * Methods of Cat Linked List
 */

CatsLinkedList* makeCatsLinkedList(Cat* all_cats, Cat** all_cats_ptr, int N);

void removeCat(CatsLinkedList* c_ll, Cat* target);

void insertAfter(CatsLinkedList* c_ll, Cat* target, Cat* cat);

void insertBefore(CatsLinkedList* c_ll, Cat* target, Cat* cat);

void swap_appetite(Cat* c1, Cat* c2);

void swap_on_ll(CatsLinkedList* c_ll, int idx);

Cat* getCat(CatsLinkedList* c_ll, int idx);

void magic_on_ll(CatsLinkedList* c_ll, int idx, int success);

/**
 *  Methods of Cat Circular Array
 */

/**
 *  Methods of Color Manager
 */
ColorManager* makeColorManager(int max_size);

void sortColorCats(Cat** cat, int N);

void buildColorCatCircularArray(ColorManager* cm, Cat** all_cats_ptr, int N);

CatCircularArray* findColor(ColorManager* cm, int color);

void swap_on_cca(ColorManager* cm, int idx);

void magic_on_cca(CatCircularArray* cca, int dir, int success, int appetite);

int main()
{
    int N, M, color;
    scanf("%d %d", &N, &M);
    // Cat* cat_ptr;
    Cat *all_cats = (Cat*)calloc(N, sizeof(Cat)), *tmp;
    Cat** all_cats_ptr = (Cat**)calloc(N, sizeof(Cat*));
    ColorManager* color_manager = makeColorManager(N);

    for (int n = 0; n < N; n++) {
        all_cats[n].idx = n;
        all_cats_ptr[n] = &all_cats[n];
        scanf("%d", &(all_cats[n].appetite));
    }

    for (int n = 0; n < N; n++) {
        scanf("%d", &color);
        all_cats[n].color = color;
    }

    sortCatsPtr(all_cats_ptr, N);
    CatsLinkedList* c_ll = makeCatsLinkedList(all_cats, all_cats_ptr, N);
    buildColorCatCircularArray(color_manager, all_cats_ptr, N);

    show(c_ll, color_manager);
    printf("\nMagic ! !\n\n");
    magic_op(c_ll, color_manager, 1, HIGHEST, SUCCESS);
    show(c_ll, color_manager);

    all_cats = NULL;
    all_cats_ptr = NULL;

    // TODO Sort Color Mapping Array and every Color Cat Circular Array.
}

/**
 *  Define Function
 */

void swap_op(CatsLinkedList* c_ll, ColorManager* cm, int idx)
{
    if (idx >= c_ll->size)
        return;
    Cat* target = getCat(c_ll, idx);
    if (target == NULL)
        return;
    Cat* next = target->next;
    if (next == NULL)
        return;
    swap_on_ll(c_ll, idx);
    if (target->color == next->color)
        swap_on_cca(cm, idx);
}

void magic_op(CatsLinkedList* c_ll, ColorManager* cm, int color, int dir, int success)
{
    CatCircularArray* cca = findColor(cm, color);
    if (cca == NULL)
        return;
    int appetite, idx;
    if (success == SUCCESS)
        appetite = c_ll->max->appetite + 1;
    else
        appetite = c_ll->min->appetite - 1;
    if (dir == LOWEST)
        idx = cca->base[cca->head]->idx;
    else
        idx = cca->base[(cca->head - 1 + cca->size) % cca->size]->idx;
    magic_on_cca(cca, dir, success, appetite);
    magic_on_ll(c_ll, idx, success);
}

/**
 * Methods of Cat
 */

int cmpCat(const void* c1, const void* c2)
{
    Cat** cat1 = (Cat**)c1;
    Cat** cat2 = (Cat**)c2;
    return (*cat1)->appetite - (*cat2)->appetite;
}

void sortCatsPtr(Cat** all_cat_ptr, int N)
{
    qsort(all_cat_ptr, N, sizeof(Cat*), cmpCat);
}

/**
 * Methods of All Cats Linked List
 */

CatsLinkedList* makeCatsLinkedList(Cat* all_cats, Cat** all_cats_ptr, int N)
{
    CatsLinkedList* ll = (CatsLinkedList*)calloc(1, sizeof(CatsLinkedList));
    ll->size = N;
    ll->arr = all_cats;
    ll->min = all_cats_ptr[0];
    ll->max = all_cats_ptr[N - 1];
    all_cats_ptr[0]->next = all_cats_ptr[1];
    for (int n = 1; n < N - 1; n++) {
        all_cats_ptr[n]->next = all_cats_ptr[n + 1];
        all_cats_ptr[n]->pre = all_cats_ptr[n - 1];
    }
    all_cats_ptr[N - 1]->pre = all_cats_ptr[N - 2];
    return ll;
}

Cat* getCat(CatsLinkedList* c_ll, int idx)
{
    if (idx >= c_ll->size)
        return NULL;
    return &(c_ll->arr[idx]);
}

void swap_on_ll(CatsLinkedList* c_ll, int idx)
{
    Cat *target = &(c_ll->arr[idx]), *next = c_ll->arr[idx].next;
    if (next == NULL)
        return;

    swap_appetite(target, next);
    removeCat(c_ll, target);
    insertAfter(c_ll, next, target);
}

void magic_on_ll(CatsLinkedList* c_ll, int idx, int success)
{
    Cat* target = getCat(c_ll, idx);
    if (target == NULL)
        return;
    removeCat(c_ll, target);
    if (success == SUCCESS)
        insertAfter(c_ll, c_ll->max, target);
    else
        insertBefore(c_ll, c_ll->min, target);
}
/**
 *  Methods of Color Cat Circular Array
 */

void removeCat(CatsLinkedList* c_ll, Cat* target)
{
    Cat *next, *pre;
    if (target == NULL)
        return;
    next = target->next;
    pre = target->pre;
    target->next = NULL;
    target->pre = NULL;
    if (target == c_ll->max) {
        c_ll->max = pre;
    } else {
        next->pre = pre;
    }
    if (target == c_ll->min) {
        c_ll->min = next;
    } else {
        pre->next = next;
    }
}

void insertAfter(CatsLinkedList* c_ll, Cat* target, Cat* cat)
{
    Cat* next;
    if (target == NULL)
        target = c_ll->max;
    if (target == NULL) {
        c_ll->max = cat;
        c_ll->min = cat;
        return;
    }
    next = target->next;
    target->next = cat;
    cat->pre = target;
    cat->next = next;

    if (next == NULL)
        c_ll->max = cat;
    if (next != NULL)
        next->pre = cat;
}

void insertBefore(CatsLinkedList* c_ll, Cat* target, Cat* cat)
{
    Cat* pre;
    if (target == NULL)
        target = c_ll->min;
    if (target == NULL) {
        c_ll->max = cat;
        c_ll->min = cat;
        return;
    }
    pre = target->pre;
    target->pre = cat;
    cat->next = target;
    cat->pre = pre;

    if (pre == NULL)
        c_ll->min = cat;
    if (pre != NULL)
        pre->next = cat;
}

void swap_appetite(Cat* c1, Cat* c2)
{
    int tmp = c1->appetite;
    c1->appetite = c2->appetite;
    c2->appetite = tmp;
}

/**
 *  Methods of Color Manager
 */
ColorManager* makeColorManager(int max_size)
{
    ColorManager* color_manager = (ColorManager*)calloc(1, sizeof(ColorManager));
    CatCircularArray* color_mapping = (CatCircularArray*)calloc(max_size, sizeof(CatCircularArray));
    color_manager->cc_arr = color_mapping;
    return color_manager;
}

int cmpColorCat(const void* c1, const void* c2)
{
    Cat** cat1 = (Cat**)c1;
    Cat** cat2 = (Cat**)c2;
    if ((*cat1)->color != (*cat2)->color)
        return (*cat1)->color - (*cat2)->color;
    return (*cat1)->appetite - (*cat2)->appetite;
}

void sortColorCats(Cat** cats, int N)
{
    qsort(cats, N, sizeof(Cat*), cmpColorCat);
}

CatCircularArray* findColor(ColorManager* cm, int color)
{
    int l = 0, r = cm->n_color - 1, mid, mid_color;
    CatCircularArray* cc_arr = cm->cc_arr;
    while (l <= r) {
        mid = (l + r) / 2;
        mid_color = cc_arr[mid].color;
        if (mid_color == color)
            return &cc_arr[mid];
        else if (mid_color < color) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    return NULL;
}

void buildColorCatCircularArray(ColorManager* cm, Cat** all_cats_ptr, int N)
{
    int n_color = 0, cur_color = 0, cur_cca = 0, cur_head = 0, idx = all_cats_ptr[0]->idx, offset = 1;
    sortColorCats(all_cats_ptr, N);

    for (int n = 0; n < N; n++) {
        if (all_cats_ptr[n]->color != cur_color) {
            n_color += 1;
            cur_color = all_cats_ptr[n]->color;
        }
    }
    cm->n_color = n_color;

    cm->cc_arr = (CatCircularArray*)calloc(n_color, sizeof(CatCircularArray));
    cm->cc_arr[cur_cca].color = all_cats_ptr[0]->color;
    cm->cc_arr[cur_cca].base = &(all_cats_ptr[0]);
    cm->cca_idx = (int*)calloc(N, sizeof(int));
    cm->cat_idx_in_cca = (int*)calloc(N, sizeof(int));
    cm->cca_idx[idx] = 0;
    cm->cat_idx_in_cca[idx] = 0;
    cur_color = all_cats_ptr[0]->color;
    for (int n = 1; n < N; n++) {
        if (all_cats_ptr[n]->color != cur_color) {
            cm->cc_arr[cur_cca].size = n - cur_head;
            cur_color = all_cats_ptr[n]->color;
            cur_head = n;
            cur_cca += 1;
            cm->cc_arr[cur_cca].color = cur_color;
            cm->cc_arr[cur_cca].base = &(all_cats_ptr[n]);
            offset = 0;
        }
        idx = all_cats_ptr[n]->idx;
        cm->cca_idx[idx] = cur_cca;
        cm->cat_idx_in_cca[idx] = offset;
        offset += 1;
    }
    cm->cc_arr[n_color - 1].size = N - cur_head;
}

void swap_on_cca(ColorManager* cm, int idx)
{
    CatCircularArray* cca = &(cm->cc_arr[cm->cca_idx[idx]]);
    int idx_in_cca = cm->cat_idx_in_cca[idx];
    if (idx_in_cca >= cca->size - 1)
        return;
    Cat* tmp = cca->base[idx_in_cca];
    cca->base[idx_in_cca] = cca->base[idx_in_cca + 1];
    cca->base[idx_in_cca + 1] = tmp;
}

void magic_on_cca(CatCircularArray* cca, int dir, int success, int appetite)
{
    if (dir == HIGHEST) {
        cca->base[(cca->head - 1 + cca->size) % cca->size]->appetite = appetite;
        if (success != SUCCESS)
            cca->head = (cca->head - 1 + cca->size) % cca->size;
    } else {
        cca->base[cca->head]->appetite = appetite;
        if (success != FAILURE)
            cca->head = (cca->head + 1) % cca->size;
    }
}