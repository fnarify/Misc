#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define N_PLAYERS 459
#define L_MARBLE  7210300 // divide by 100 for part 1

struct List {
    struct List *next, *prev;
    int64_t val;
};

typedef struct List List;

/**
 * Assuming L_MARBLE - 1 is not divisible wholy by 23 then the list will finish at the final entry
 * so we can free memory by moving backwards until we reach the start
 */
void clear(List *list)
{
    while (list->val)
    {
        list = list->prev;
        free(list->next);
    }
    free(list);
}

/**
 * Doubly linked list implementation with a game of marbles
 */
int main()
{
    int64_t i, j, max, index;
    List *list = malloc(sizeof(struct List));
    if (!list) {return -1;}
    List *new;
    list->val = 0;
    list->next = list->prev = list;
    
    int64_t *players = calloc(N_PLAYERS, sizeof(*players));
    if (!players) {return -1;}

    index = 0;
    for (i = 1; i < L_MARBLE; i++)
    {
        if (i % 23 == 0)
        {
            // if divisible by 23 remove the item 7 steps back add it to the players score as well as the value of i
            for (j = 0; j < 7; j++) {list = list->prev;}

            players[index] += list->val + i;
            list->next->prev = list->prev;
            list->prev->next = list->next;

            List *temp = list->next;
            free(list);
            list = temp;
        }
        else
        {
            // insert new list entry between the next and next next entry
            new = malloc(sizeof(*new));
            if (!new) {return -1;}
            new->val = i;
            new->next = list->next->next;
            new->prev = list->next;
            new->next->prev = new;
            new->prev->next = new;

            list = new;
        }

        if (++index == N_PLAYERS) {index ^= index;}
    }

    max = 0;
    for (i = 0; i < N_PLAYERS; i++)
    {
        if (players[i] > max) {max = players[i];}
    }
    printf("Highest score: %" PRId64 "\n", max);

    free(players);
    clear(list);

    return 0;
}
