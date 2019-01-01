#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define N_JOBS    26
#define N_WORKERS 5

/**
 * Remove the character exclude from the variable size list steps where each row has length sizes
 */
void removech(char steps[N_JOBS][N_JOBS], int sizes[N_JOBS], char exclude)
{
    int i, j;
    for (i = 0; i < N_JOBS; i++)
    {
        for (j = 0; j < sizes[i] && sizes[i] != -1; j++)
        {
            if (steps[i][j] == exclude)
            {
                sizes[i]--;
                while (steps[i][j] != '\0')
                {
                    steps[i][j] = steps[i][j + 1];
                    j++;
                }
            }
        }
    }
}

/**
 * simulates nw workers picking jobs concurrently on variable list steps with each row
 * having size sizes
 * 
 * Alters the arrays steps and sizes so a deep copy is needed first if running multiple times
 *
 * nw - 1 for part 1
 * nw - 5 for part 2
 */
int simulation(char steps[N_JOBS][N_JOBS], int sizes[N_JOBS], int nw)
{
    int i, j, n, m, curtime, index, temp;
    char jobs[N_WORKERS] = {0};
    int times[N_WORKERS] = {0};
    char output[N_JOBS]  = {0};

    n = m = curtime = 0;
    while (n < N_JOBS)
    {
        for (i = 0; i < N_JOBS; i++)
        {
            for (j = 0; j < nw; j++)
            {
                if (jobs[j] || sizes[i]) {continue;}

                // add job to worker
                jobs[j] = 'A' + i;
                // record what time to complete
                times[j] = 60 + i + 1 + curtime;
                // prevent job being redone
                sizes[i] = -1;
            }
        }

        // find first job to complete
        temp = INT_MAX;
        for (i = 0; i < nw; i++)
        {
            if (times[i] < temp && times[i])
            {
                temp = times[i];
                index = i;
            }
        }
        curtime = temp;

        // record and remove job
        output[m++] = jobs[index];
        removech(steps, sizes, jobs[index]);
        jobs[index] = times[index] = 0;

        n++;
    }

    output[m] = '\0';
    printf("%s\n", output);

    return curtime;
}

void deepcopy(char steps[N_JOBS][N_JOBS], char a[N_JOBS][N_JOBS], int sizes[N_JOBS], int b[N_JOBS])
{
    int i, j;
    for (i = 0; i < N_JOBS; i++)
    {
        b[i] = sizes[i];
        for (j = 0; j < N_JOBS; j++)
        {
            a[i][j] = steps[i][j];
        }
    }
}

/**
 * Runs a job simulation for both part 1 and 2 of AoC18 day 7
 */
int main()
{
    char c1, c2;
    char line[51];
    char steps[N_JOBS][N_JOBS] = {{0}};
    int sizes[N_JOBS] = {0};
    
    char a[N_JOBS][N_JOBS];
    int b[N_JOBS];

    while (fgets(line, 51, stdin) != NULL)
    {
        sscanf(line, "Step %c must be finished before step %c can begin.\n", &c1, &c2);
        steps[c2 - 'A'][sizes[c2 - 'A']++] = c1;
    }

    deepcopy(steps, a, sizes, b);
    printf("Part 1: %d\n", simulation(a , b, 1));
    deepcopy(steps, a, sizes, b);
    printf("Part 2: %ds\n", simulation(a, b, N_WORKERS));

    return 0;
}
