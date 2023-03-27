#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Player
{
    int index;
    int attack_power;
    int score;
} Player;

typedef struct DSA
{
    uint16_t size;
    uint16_t head;
    uint16_t tail;

    // An array of Players
    Player *area;
} DSA;

DSA *make_DSA(int16_t size)
{
    DSA *dsa = (DSA *)calloc(1, sizeof(DSA));
    dsa->size = size;
    dsa->area = (Player *)calloc(size, sizeof(Player));
    return dsa;
}

int kill_othoer_players(DSA *dsa, int attack_power)
{
}

void revolution(DSA *dsa)
{
}

void add_player(DSA *dsa, int index, int attack_power)
{
    Player *target = &(dsa->area[dsa->tail]);
    target->index = index;
    target->attack_power = attack_power;
    target->score = 0;
    dsa->tail = (dsa->tail + 1) % dsa->size;
}

void *enter_DSA(DSA *dsa, int index, int attack_power)
{
    Player *area = dsa->area;
    uint16_t size = dsa->size, head = dsa->head, tail = dsa->tail;
    int kill_count = 0;

    if (area[tail - 1].attack_power < attack_power)
    {
        kill_count = kill_othoer_players(dsa, attack_power);
        for (int i = 1; i <= kill_count; i++)
        {
            printf(" %d,%d", area[(tail - i + size) % size].index, area[(tail - i + size) % size].score);
        }
    }
    else
    {
        if (tail == head && area[head].index != 0)
        {
            revolution(dsa);
            printf(" %d,%d", area[tail].index, area[tail].score);
        }
    }
    add_player(dsa, index, attack_power);
    printf("\n");
}

void *final_DSA(DSA *dsa)
{
    printf("Final:");
    printf("\n");
}

int main()
{
    int n, m, input;
    scanf("%d %d\n", &n, &m);

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