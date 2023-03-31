#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef struct Player
{
    int index;
    int attack_power;
    int score;
} Player;

typedef struct DSA
{
    // An array of Players
    Player *area;
    uint16_t size;
    uint16_t head;
    uint16_t tail;
    bool full;
} DSA;

DSA *make_DSA(int16_t size)
{
    DSA *dsa = (DSA *)calloc(1, sizeof(DSA));
    dsa->size = size;
    dsa->area = (Player *)calloc(size, sizeof(Player));
    return dsa;
}

void free_DSA(DSA *dsa)
{
    memset(dsa->area, 0, dsa->size * sizeof(Player));
    free(dsa->area);
    memset(dsa, 0, sizeof(DSA));
    free(dsa);
    dsa = NULL;
}

void kill_other_players(DSA *dsa, int index, int attack_power)
{
    int16_t size = dsa->size, new_tail = dsa->tail, head = dsa->head;
    int target_score = 0;
    while (dsa->tail != head || dsa->full)
    {
        new_tail = (dsa->tail - 1 + size) % size;
        if (dsa->area[new_tail].attack_power >= attack_power)
            break;
        target_score = dsa->area[new_tail].score;
        target_score += (size - (new_tail - head + size) % size) * (index - dsa->area[new_tail].index);
        printf(" %d,%d", dsa->area[new_tail].index, target_score);
        dsa->tail = new_tail;
        dsa->full = false;
    }
}

void check_revolution(DSA *dsa, int index)
{
    int i = (dsa->head + 1) % dsa->size;
    int head_score = dsa->area[dsa->head].score;
    if (dsa->full && dsa->head == dsa->tail)
    {
        head_score += dsa->size * (index - dsa->area[dsa->head].index);
        printf(" %d,%d", dsa->area[dsa->head].index, head_score);
        dsa->head = (dsa->head + 1) % dsa->size;
        dsa->full = false;
        do
        {
            dsa->area[i].score -= index - dsa->area[i].index;
            i = (i + 1) % dsa->size;
        } while (i != dsa->head);
    }
}

void add_player(DSA *dsa, int index, int attack_power)
{
    Player *target = &(dsa->area[dsa->tail]);
    target->index = index;
    target->attack_power = attack_power;
    target->score = 0;
    dsa->tail = (dsa->tail + 1) % dsa->size;
    if (dsa->tail == dsa->head)
        dsa->full = true;
}

void enter_DSA(DSA *dsa, int index, int attack_power)
{
    printf("Round %d:", index);
    kill_other_players(dsa, index, attack_power);
    check_revolution(dsa, index);
    add_player(dsa, index, attack_power);
    printf("\n");
}

void final_DSA(DSA *dsa, int n)
{
    int16_t head = dsa->head, target = dsa->tail, size = dsa->size;
    int target_score = 0;
    printf("Final:");
    while (target != head || dsa->full)
    {
        target = (target - 1 + size) % size;
        target_score = dsa->area[target].score + (n + 1 - dsa->area[target].index) * (size - (target - head + size) % size);
        printf(" %d,%d", dsa->area[target].index, target_score);
        dsa->full = false;
    }
    printf("\n");
}

int main()
{
    int n, m, input;
    scanf("%d %d", &n, &m);

    // Make a "Digital Savage Arena" (DSA)
    DSA *dsa = make_DSA(m);

    // Game Start !
    for (int i = 1; i <= n; i++)
    {
        scanf("%d", &input);
        enter_DSA(dsa, i, input);
    }
    final_DSA(dsa, n);
    free_DSA(dsa);
    return 0;
}