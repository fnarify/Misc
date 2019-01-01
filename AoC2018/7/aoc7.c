#include <stdio.h>
#include <stdlib.h>

#define ALPHA_LEN 26

/**
 * remove the character exclude from the variable size list steps where each row has length sizes
 */
void removech(char steps[ALPHA_LEN][ALPHA_LEN], int sizes[ALPHA_LEN], char exclude)
{
    int i, j;
    for (i = 0; i < ALPHA_LEN; i++)
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
 * Output the order of the jobs that must be completed
 */
int main()
{
    int i, n;
    char c1, c2;
    char line[51];
    char output[ALPHA_LEN + 1];
    char steps[ALPHA_LEN][ALPHA_LEN] = {{0}};
    int sizes[ALPHA_LEN] = {0};

    while (fgets(line, 51, stdin) != NULL)
    {
        sscanf(line, "Step %c must be finished before step %c can begin.\n", &c1, &c2);
        steps[c2 - 'A'][sizes[c2 - 'A']++] = c1;
    }

    n = 0;
    while (n < ALPHA_LEN)
    {
        for (i = 0; i < ALPHA_LEN; i++)
        {
            if (!sizes[i])
            {
                output[n] = 'A' + i;
                sizes[i] = -1; // ignore now
                break;
            }
        }

        removech(steps, sizes, output[n]);
        n++;
    }

    output[n] = '\0';

    printf("%s\n", output);

    return 0;
}
