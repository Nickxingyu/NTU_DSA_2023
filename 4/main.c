#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

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
    int16_t head;
    int16_t tail;
} DSA;

DSA *make_DSA(int16_t size)
{
    DSA *dsa = (DSA *)calloc(1, sizeof(DSA));
    dsa->size = size;
    dsa->area = (Player *)calloc(size, sizeof(Player));
    return dsa;
}

int16_t kill_othoer_players(DSA *dsa, int attack_power)
{
    int16_t size = dsa->size;
    int16_t new_tail = (dsa->tail - 1 + size) % size, count = 1;
    while (new_tail != dsa->head)
    {
        new_tail = (new_tail - 1 + size) % size;
        if (dsa->area[new_tail].attack_power >= attack_power)
            break;
        count++;
    }
    return count;
}

void revolution(DSA *dsa)
{
    dsa->head = (dsa->head + 1) % dsa->size;
}

void add_player(DSA *dsa, int index, int attack_power)
{
    Player *target = &(dsa->area[dsa->tail]);
    target->index = index;
    target->attack_power = attack_power;
    // target->score = 0;
    dsa->tail = (dsa->tail + 1) % dsa->size;
}

void *enter_DSA(DSA *dsa, int index, int attack_power)
{
    Player *area = dsa->area;
    uint16_t size = dsa->size, head = dsa->head, tail = dsa->tail;
    int kill_count = 0;
    printf("Round %d:", index);
    if (area[(tail - 1 + size) % size].attack_power < attack_power && area[(tail - 1 + size) % size].index != 0)
    {
        kill_count = kill_othoer_players(dsa, attack_power);
        for (int i = 1; i <= kill_count; i++)
        {
            printf(" %d", area[(tail - i + size) % size].index);
        }
        tail = (tail - kill_count + size) % size;
        dsa->tail = tail;
    }
    else
    {
        if (tail == head && area[head].index != 0)
        {
            revolution(dsa);
            printf(" %d", area[tail].index);
        }
    }
    add_player(dsa, index, attack_power);
    printf("\n");
}

void *final_DSA(DSA *dsa)
{
    int16_t head = dsa->head, target = dsa->tail, size = dsa->size;
    printf("Final:");
    while (target != head)
    {
        target = (target - 1 + size) % size;
        printf(" %d", dsa->area[target].index);
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
    final_DSA(dsa);
    return 0;
}