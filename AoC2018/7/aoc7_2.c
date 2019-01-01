#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define N_JOBS    26
#define N_WORKERS 5

/**
 * remove the character exclude from the variable size list steps where each row has length sizes
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
 * simulation for 5 workers on the previous job input, record time that all jobs are finally complete
 */
int main()
{
    int i, j, n, curtime, index, temp;
    char c1, c2;
    char line[51];
    char steps[N_JOBS][N_JOBS] = {{0}};
    int sizes[N_JOBS] = {0};
    char jobs[N_WORKERS] = {0};
    int times[N_WORKERS] = {0};

    while (fgets(line, 51, stdin) != NULL)
    {
        sscanf(line, "Step %c must be finished before step %c can begin.\n", &c1, &c2);
        steps[c2 - 'A'][sizes[c2 - 'A']++] = c1;
    }

    n = curtime = 0;
    while (n < N_JOBS)
    {
        for (i = 0; i < N_JOBS; i++)
        {
            for (j = 0; j < N_WORKERS; j++)
            {
                if (jobs[j] || times[j] || sizes[i]) {continue;}

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
        for (i = 0; i < N_WORKERS; i++)
        {
            if (times[i] < temp && times[i])
            {
                temp = times[i];
                index = i;
            }
        }
        curtime = temp;

        // remove job 
        removech(steps, sizes, jobs[index]);
        jobs[index] = times[index] = 0;

        n++;
    }

    printf("Part 2: %ds\n", curtime);

    return 0;
}
