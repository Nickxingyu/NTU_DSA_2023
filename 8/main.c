#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HIGHEST 1
#define LOWEST 0
#define SUCCESS 1
#define FAILURE 0

int q_time = 0;

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
    int* offset_in_cca;
    CatCircularArray* cc_arr;
} ColorManager;

void show(CatsLinkedList* c_ll, ColorManager* cm)
{
    Cat* tmp;
    int idx = 0;
    tmp = c_ll->min;
    printf("-----------------------------------------------\n");
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
        printf("\nColor: %d\n", cm->cc_arr[n].color);
        for (int c = 0; c < cm->cc_arr[n].size; c++) {
            idx = (cm->cc_arr[n].head + c) % cm->cc_arr[n].size;
            printf("%d %d %d %d\n", idx, cm->cc_arr[n].base[idx]->color, cm->cc_arr[n].base[idx]->idx, cm->cc_arr[n].base[idx]->appetite);
        }
    }
}

void buildAllDS(CatsLinkedList** c_ll, ColorManager** cm, Cat* all_cats, Cat** all_cats_ptr, int N);
void swap_op(CatsLinkedList* c_ll, ColorManager* cm, int idx);
void magic_op(CatsLinkedList* c_ll, ColorManager* cm, int color, int dir, int success);
int questioning_op(ColorManager* cm, int color, int lower_b, int upper_b);

int main()
{
    int N, M, color, op, arg1, arg2, arg3;
    scanf("%d %d", &N, &M);
    Cat *all_cats = (Cat*)calloc(N, sizeof(Cat)), *tmp;
    Cat** all_cats_ptr = (Cat**)calloc(N, sizeof(Cat*));
    ColorManager* cm;
    CatsLinkedList* c_ll;

    for (int n = 0; n < N; n++) {
        all_cats[n].idx = n;
        all_cats_ptr[n] = &all_cats[n];
        scanf("%d", &(all_cats[n].appetite));
    }

    for (int n = 0; n < N; n++) {
        scanf("%d", &color);
        all_cats[n].color = color;
    }

    buildAllDS(&c_ll, &cm, all_cats, all_cats_ptr, N);
    all_cats = NULL;
    all_cats_ptr = NULL;

    for (int m = 0; m < M; m++) {
        // show(c_ll, cm);
        scanf("%d", &op);
        if (op == 2) {
            scanf("%d", &arg1);
        } else {
            scanf("%d %d %d", &arg1, &arg2, &arg3);
        }
        if (op == 2) {
            swap_op(c_ll, cm, arg1);
        } else if (op == 3) {
            magic_op(c_ll, cm, arg1, arg2, arg3);
        } else {
            // questioning_op(cm, arg1, arg2, arg3);
            printf("%d\n", questioning_op(cm, arg1, arg2, arg3));
        }
    }
    // show(c_ll, cm);
}

/**
 *  Define Function
 */

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

void swap_appetite(Cat* c1, Cat* c2)
{
    int tmp = c1->appetite;
    c1->appetite = c2->appetite;
    c2->appetite = tmp;
}

/**
 * Methods of Cats Linked List
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

void removeCat(CatsLinkedList* c_ll, Cat* target)
{
    Cat *next, *pre;
    if (target == NULL)
        return;
    next = target->next;
    pre = target->pre;
    target->next = NULL;
    target->pre = NULL;
    if (next == NULL) {
        c_ll->max = pre;
    } else {
        next->pre = pre;
    }
    if (pre == NULL) {
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

void swap_on_ll(CatsLinkedList* c_ll, int idx)
{
    Cat *target = &(c_ll->arr[idx]), *next = c_ll->arr[idx].next;
    if (next == NULL)
        return;

    swap_appetite(target, next);
    removeCat(c_ll, target);
    insertAfter(c_ll, next, target);
}

void magic_on_ll(CatsLinkedList* c_ll, int idx, int success, int appetite)
{
    Cat* target = getCat(c_ll, idx);
    if (target == NULL)
        return;
    target->appetite = appetite;
    removeCat(c_ll, target);
    if (success == SUCCESS)
        insertAfter(c_ll, c_ll->max, target);
    else
        insertBefore(c_ll, c_ll->min, target);
}

/**
 *  Methods of Cat Circular Array
 */
void magic_on_cca(CatCircularArray* cca, int dir, int success)
{
    if ((dir ^ success) == 0)
        return;
    if (dir == HIGHEST) {
        cca->head = (cca->head - 1 + cca->size) % cca->size;
    } else {
        cca->head = (cca->head + 1) % cca->size;
    }
}

int binarySearchOnCircularArray(Cat** base, int head, int size, int l, int r, int value)
{
    int idx, mid, appetite;
    while (l <= r) {
        idx = (head + l) % size;
        appetite = base[idx]->appetite;
        if (appetite >= value)
            return l;
        mid = (l + r) / 2;
        idx = (head + mid) % size;
        appetite = base[idx]->appetite;
        if (appetite == value)
            return mid;
        else if (appetite > value)
            r = mid;
        else
            l = mid + 1;
    }
    return -1;
}

int binarySearchOnCircularArrayNotInclude(Cat** base, int head, int size, int l, int r, int value)
{
    int idx, mid, appetite;
    while (l <= r) {
        idx = (head + l) % size;
        appetite = base[idx]->appetite;
        if (appetite > value)
            return l;
        mid = (l + r) / 2;
        idx = (head + mid) % size;
        appetite = base[idx]->appetite;
        if (appetite > value)
            r = mid;
        else
            l = mid + 1;
    }
    return -1;
}

int findLowerBound(CatCircularArray* cca, int b)
{
    int l = 0, r = cca->size - 1;
    return binarySearchOnCircularArray(cca->base, cca->head, cca->size, l, r, b);
}

int findUpperBound(CatCircularArray* cca, int b)
{
    int l = 0, r = cca->size - 1;
    return binarySearchOnCircularArrayNotInclude(cca->base, cca->head, cca->size, l, r, b);
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
    int n_color = 0, cur_color = -1, cur_cca = 0, cur_head = 0, idx = all_cats_ptr[0]->idx, offset = 1;
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
    cm->offset_in_cca = (int*)calloc(N, sizeof(int));
    cm->cca_idx[idx] = 0;
    cm->offset_in_cca[idx] = 0;
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
        cm->offset_in_cca[idx] = offset;
        offset += 1;
    }
    cm->cc_arr[n_color - 1].size = N - cur_head;
}

void swap_on_cca(ColorManager* cm, int idx)
{

    CatCircularArray* cca = &(cm->cc_arr[cm->cca_idx[idx]]);
    int offset = cm->offset_in_cca[idx];
    int next_offset = (offset + 1) % cca->size;
    int next_idx = cca->base[next_offset]->idx;
    Cat* tmp = cca->base[offset];
    cca->base[offset] = cca->base[next_offset];
    cca->base[next_offset] = tmp;
    cm->offset_in_cca[idx] = next_offset;
    cm->offset_in_cca[next_idx] = offset;
}

/**
 * Global Operations
 */
void buildAllDS(CatsLinkedList** c_ll, ColorManager** cm, Cat* all_cats, Cat** all_cats_ptr, int N)
{
    sortCatsPtr(all_cats_ptr, N);
    *cm = makeColorManager(N);
    *c_ll = makeCatsLinkedList(all_cats, all_cats_ptr, N);
    buildColorCatCircularArray(*cm, all_cats_ptr, N);
}

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
    magic_on_cca(cca, dir, success);
    magic_on_ll(c_ll, idx, success, appetite);
}

int questioning_op(ColorManager* cm, int color, int lower_b, int upper_b)
{
    q_time += 1;
    int lower_idx, upper_idx;
    CatCircularArray* cca = findColor(cm, color);
    if (cca == NULL)
        return 0;
    lower_idx = findLowerBound(cca, lower_b);
    upper_idx = findUpperBound(cca, upper_b);
    if (lower_idx == -1)
        return 0;
    if (upper_idx == -1)
        upper_idx = cca->size;
    return upper_idx - lower_idx;
}