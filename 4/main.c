#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef struct Player
{
    int index;
    int attack_power;
    // int score;
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

void kill_other_players(DSA *dsa, int attack_power)
{
    int16_t size = dsa->size;
    int16_t new_tail = dsa->tail;
    while (dsa->tail != dsa->head || dsa->full)
    {
        new_tail = (dsa->tail - 1 + size) % size;
        if (dsa->area[new_tail].attack_power >= attack_power)
            break;
        printf(" %d", dsa->area[new_tail].index);
        dsa->tail = new_tail;
        dsa->full = false;
    }
}

void check_revolution(DSA *dsa)
{
    if (dsa->full && dsa->head == dsa->tail)
    {
        printf(" %d", dsa->area[dsa->head].index);
        dsa->head = (dsa->head + 1) % dsa->size;
        dsa->full = false;
    }
}

void add_player(DSA *dsa, int index, int attack_power)
{
    Player *target = &(dsa->area[dsa->tail]);
    target->index = index;
    target->attack_power = attack_power;
    // target->score = 0;
    dsa->tail = (dsa->tail + 1) % dsa->size;
    if (dsa->tail == dsa->head)
        dsa->full = true;
}

void *enter_DSA(DSA *dsa, int index, int attack_power)
{
    Player *area = dsa->area;
    uint16_t size = dsa->size, head = dsa->head, tail = dsa->tail;
    printf("Round %d:", index);
    kill_other_players(dsa, attack_power);
    check_revolution(dsa);
    add_player(dsa, index, attack_power);
    printf("\n");
}

void *final_DSA(DSA *dsa)
{
    int16_t head = dsa->head, target = dsa->tail, size = dsa->size;
    printf("Final:");
    while (target != head || dsa->full)
    {
        target = (target - 1 + size) % size;
        printf(" %d", dsa->area[target].index);
        dsa->full = false;
    }
    printf("\n");
    memset(dsa->area, 0, dsa->size * sizeof(Player));
    free(dsa->area);
    memset(dsa, 0, sizeof(DSA));
    free(dsa);
    dsa = NULL;
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
    final_DSA(dsa);
    return 0;
}