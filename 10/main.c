#include <stdio.h>
#include <stdlib.h>

typedef long long ll_int;

typedef struct Knight {
    ll_int health;
    int set;
    int power;
    int score;
} Knight;

typedef struct KnightSet {
    ll_int power;
    ll_int damage;
    int capacity;
    int size;
    int score;
    Knight** Knights_heap;
} KnightSet;

void init_process(int n, Knight* knights, KnightSet* knight_set);
void fight(int n, int m, Knight* knights, KnightSet* knight_set);
void show(int n, Knight* knights);

int main()
{
    int n, m;
    scanf("%d %d", &n, &m);
    Knight* knights = (Knight*)calloc(n, sizeof(Knight));
    KnightSet* knight_set = (KnightSet*)calloc(n, sizeof(KnightSet));
    init_process(n, knights, knight_set);
    fight(n, m, knights, knight_set);
    show(n, knights);
}

void init_process(int n, Knight* knights, KnightSet* knight_set)
{
    for (int i = 0; i < n; i++) {
        knights[i].set = i;
        scanf(" %lld", &knights[i].health);
        knight_set[i].capacity = 8;
        knight_set[i].size = 1;
        knight_set[i].Knights_heap = (Knight**)calloc(8, sizeof(Knight*));
        knight_set[i].Knights_heap[0] = &knights[i];
    }
    for (int i = 0; i < n; i++) {
        scanf(" %d", &knights[i].power);
        knight_set[i].power = knights[i].power;
    }
}

/**
 *  The operations of union two knight set.
 */
void swap(int l, int r, Knight** knights_heap);
void insert(int s, KnightSet* knight_set, Knight* knight);
Knight* pop(KnightSet* set);
void checkAlive(KnightSet* set);

void unionSet(int s1, int s2, KnightSet* knight_set)
{
    KnightSet *set_1 = &knight_set[s1], *set_2 = &knight_set[s2];
    int total_size = set_1->size + set_2->size;

    // Increase capacity if capacity is not enough.
    if (total_size > set_1->capacity) {
        set_1->capacity *= 2;
        set_1->Knights_heap = realloc(set_1->Knights_heap, set_1->capacity * sizeof(Knight*));
    }

    // Insert the elements of the smaller set to the bigger set
    for (int i = 0; i < set_2->size; i++) {
        insert(s1, knight_set, set_2->Knights_heap[i]);
    }
    set_1->power += set_2->power;
}

void swap(int l, int r, Knight** knights_heap)
{
    Knight* tmp = knights_heap[l];
    knights_heap[l] = knights_heap[r];
    knights_heap[r] = tmp;
}

// min health heap insert
void insert(int s, KnightSet* knight_set, Knight* knight)
{
    KnightSet* set = &knight_set[s];
    int target = set->size;
    set->size += 1;
    set->Knights_heap[target] = knight;

    // heapify
    while (target != 0 && set->Knights_heap[(target - 1) / 2]->health > set->Knights_heap[target]->health) {
        swap((target - 1) / 2, target, set->Knights_heap);
        target = (target - 1) / 2;
    }
}

// min health heap pop
Knight* pop(KnightSet* set)
{
    int root = 0, min;
    swap(0, set->size - 1, set->Knights_heap);
    set->size -= 1;
    while (root < set->size) {

        // Find the knight with min health.
        min = root;
        if (root * 2 + 1 < set->size) {
            if (set->Knights_heap[root * 2 + 1]->health < set->Knights_heap[min]->health) {
                min = root * 2 + 1;
            }
        }
        if (root * 2 + 2 < set->size) {
            if (set->Knights_heap[root * 2 + 2]->health < set->Knights_heap[min]->health) {
                min = root * 2 + 2;
            }
        }

        // If the knight with min health is root, we find the correct position of this knight.
        if (min == root)
            break;

        // Keep finding
        swap(root, min, set->Knights_heap);
        root = min;
    }
    return set->Knights_heap[set->size];
}

/**
 *  The operations about function 'fight'.
 */
void attack(int a, int t, Knight* knights, KnightSet* knight_set);
void adjust(int b_set_idx, int s_set_idx, KnightSet* knight_set);
void settle(int n, Knight* knights, KnightSet* knight_set);

void fight(int n, int m, Knight* knights, KnightSet* knight_set)
{
    int a, t;
    for (int i = 0; i < m; i++) {
        scanf(" %d %d", &a, &t);
        attack(a - 1, t - 1, knights, knight_set);
    }
    settle(n, knights, knight_set);
}

void adjust(int b_set_idx, int s_set_idx, KnightSet* knight_set)
{
    KnightSet *b_set = &knight_set[b_set_idx], *s_set = &knight_set[s_set_idx];
    for (int i = 0; i < s_set->size; i++) {
        s_set->Knights_heap[i]->set = b_set_idx;
        s_set->Knights_heap[i]->health += b_set->damage - s_set->damage;
        s_set->Knights_heap[i]->score += s_set->score - b_set->score;
    }
}

void checkAlive(KnightSet* set)
{
    Knight* target;
    /**
     * If the health of knight is less than the damage of set, this knight is dead.
     *
     * Because Knights_heap is a min heap, we can find the knight with smallest health by checking root.
     *
     */
    while (set->Knights_heap[0]->health <= set->damage) {
        target = pop(set);
        target->set = -1;
        target->score += set->score;
        set->power -= target->power;
    }
}

void attack(int a, int t, Knight* knights, KnightSet* knight_set)
{
    int a_set_idx = knights[a].set;
    int t_set_idx = knights[t].set;
    if (a_set_idx == t_set_idx || a_set_idx == -1 || t_set_idx == -1)
        return;

    KnightSet* a_set = &knight_set[a_set_idx];
    KnightSet* t_set = &knight_set[t_set_idx];
    a_set->score += 1;
    t_set->damage += a_set->power;
    if (a_set->size >= t_set->size) {
        adjust(a_set_idx, t_set_idx, knight_set);
        unionSet(a_set_idx, t_set_idx, knight_set);
        checkAlive(a_set);
    } else {
        adjust(t_set_idx, a_set_idx, knight_set);
        unionSet(t_set_idx, a_set_idx, knight_set);
        checkAlive(t_set);
    }
}

void settle(int n, Knight* knights, KnightSet* knight_set)
{
    Knight* target;
    KnightSet* target_set;
    for (int i = 0; i < n; i++) {
        target = &knights[i];
        target_set = &knight_set[target->set];
        target->score += (target->set != -1) * target_set->score;
    }
}

/**
 *  Show the status of knights.
 */

void show(int n, Knight* knights)
{
    for (int i = 0; i < n - 1; i++) {
        printf("%d ", knights[i].score);
    }
    printf("%d", knights[n - 1].score);
}