#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM 50
#define NGEN 1000000000
#define TOLR 500 // tolerance for when to check for cycles needs to be reasonably high > 400
#define OPEN '.'
#define TREE '|'
#define YARD '#'

int adj[] = {[OPEN] = 0, [TREE] = 0, [YARD] = 0, [' '] = 0};

/**
 * Check the interior points only of the 2d array state and record what its adjacent points are
 * (one step in all cartesian directions), do not include the point itself
 * Exterior points are ignore when making decisions, just makes the logic easier to read
 */
char checkadj(int x, int y, char state[DIM + 2][DIM + 2])
{
    char ret = ' ';
    adj[OPEN] = adj[TREE] = adj[YARD] = 0;

    // left
    adj[(int) state[y - 1][x - 1]]++;
    adj[(int) state[y    ][x - 1]]++;
    adj[(int) state[y + 1][x - 1]]++;
    // above/below
    adj[(int) state[y - 1][x]]++;
    adj[(int) state[y + 1][x]]++;
    // right
    adj[(int) state[y - 1][x + 1]]++;
    adj[(int) state[y    ][x + 1]]++;
    adj[(int) state[y + 1][x + 1]]++;

    switch(state[y][x])
    {
        case OPEN:
            ret = adj[TREE] >= 3 ? TREE : OPEN;
            break;
        case TREE:
            ret = adj[YARD] >= 3 ? YARD : TREE;
            break;
        case YARD:
            ret = adj[YARD] && adj[TREE] ? YARD : OPEN;
            break;
    }

    return ret;
}

void solve(char state[DIM + 2][DIM + 2], int gen)
{
    int i, j;
    adj[TREE] = adj[OPEN] = adj[YARD] = 0;
    for (i = 1; i < DIM + 1; i++)
    {
        for (j = 1; j < DIM + 1; j++) {adj[(int) state[i][j]]++;}
    }
    printf("After %d generations: %d trees, %d yards - total = %d\n", gen, adj[TREE], adj[YARD], adj[TREE] * adj[YARD]);
}

/**
 * Don't forget none of these arrays are null-terminated if you want to print them
 * The data does cycle eventually, but requires a few hundred generations before it shows
 * so only start checking after a set TOLR to prevent wasteful memcpy and memcmpy calls
 */
int main()
{
    int i, j, gen, ncycle, found, size;
    char line[DIM + 2];
    // exterior of size 1 around the perimeter to make calculation easier
    char state[DIM + 2][DIM + 2];
    char nstate[DIM][DIM];
    char cycle[DIM][DIM];

    memset(state, ' ', sizeof(state));

    i = 1;
    while (fgets(line, sizeof(line), stdin) != NULL) {memcpy(&state[i++][1], &line, DIM);}

    gen = found = ncycle = 0;
    size = NGEN;
    while (gen++ < size)
    {
        for (i = 0; i < DIM; i++)
        {
            for (j = 0; j < DIM; j++)
            {
                nstate[i][j] = checkadj(j + 1, i + 1, state);
            }
        }

        for (i = 0; i < DIM; i++) {memcpy(&state[i+1][1], &nstate[i][0], DIM);}

        if (gen == 10 && !found) // part 1
        {
            solve(state, gen);
        }
        else if (gen == TOLR) // part 2
        {
            memcpy(&cycle, &nstate, sizeof(cycle));
        }
        else if (gen > TOLR && !found)
        {
            if (!memcmp(&cycle, &nstate, sizeof(cycle)))
            {
                found = 1;
                ncycle = gen - TOLR;
                size = (size - gen) % ncycle;
                gen = 0;
            }
        }
    }

    printf("repeats after: %d cycles\n", ncycle);
    solve(state, gen - 1);

    return 0;
}
