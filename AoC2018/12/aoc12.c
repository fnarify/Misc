#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#define LEN 32
#define GEN 200

/**
 * The size of the grid does expand per generation, the final answer can be bruteforced by noting the solution converges
 * thus taking that length (where the first # starts and ends) + 4 as the maximum size of the two pointers and marking an offset
 * saves on a large amount of memory, but it's built too finely around the input.
 *
 * Otherwise just record where it converges, that is such that the previous state is equivalent to the current state shifted left by 1
 * (or that the change from one state to the next is identical for every state after).
 *
 * The input I have converges after 120 generations.
 */
int main()
{
    int64_t i, j, k, index, lastindex, sum, lastsum, size = 104, nstates;
    char line[120], in[6], out[LEN];
    int map[LEN] = {0};

    // None of these strings are null-terminated, do that before printing
    char *state = malloc(sizeof(*state) * (size + GEN));
    char *nstate = malloc(sizeof(*nstate) * (size + GEN));
    if (!state | !nstate) {return -1;}

    // grab inputs and what they map to, saving as a bitmask to prevent string operations
    index = 0;
    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        if (line[0] == 'i')
        {
            sscanf(line, "initial state: %s\n", &state[2]);
            // prepend and append string with two ellipses
            state[0] = state[1] = state[size - 2] = state[size - 1] = '.';
        }
        else if (line[0] != '\n')
        {
            sscanf(line, "%s => %c", in, &out[index]);
            for (i = 0; i < 5; i++) {map[index] += in[i] << i;}
            index++;
        }
    }
    nstates = index;

    lastsum = lastindex = 0;
    for (i = 0; i < GEN; i++)
    {
        memset(nstate, '.', size * sizeof(*nstate));

        for (j = 0; j < size; j++)
        {
            // need to check 2 spaces left of each entry and 2 right
            sum = 0;
            for (k = j - 2; k < j + 3; k++)
            {
                if (k < 0 || k > size - 1) {sum += '.' << (k - j + 2);}
                else                       {sum += state[k] << (k - j + 2);}
            }

            // update new state
            for (k = 0; k < nstates; k++)
            {
                if (sum == map[k])
                {
                    nstate[j] = out[k];
                    break;
                }
            }
        }
        
        memcpy(state, nstate, size * sizeof(*state));
        size++;

        sum = index = 0;
        for (j = 0; j < size; j++)
        {
            if (state[j] == '#')
            {
                sum += j - 2;
                index++;
            }
        }

        // part 1 solution at generation 20
        if (i == 19) {printf("Gen 20 - sum of pots: %" PRId64 "\n", sum);}

        // values converged; more correct to compare the previous state rather than index offset
        if (sum == lastsum + lastindex) {break;}

        lastsum = sum;
        lastindex = index;
    }

    // part 2, values converged record sum
    sum += index * (50000000000 - i - 1);
    printf("Sum of pots: %" PRId64 "\n", sum);

    return 0;
}
