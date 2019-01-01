#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_GUARDS 5000
#define LINE_LEN 50

/**
 * Find guard ID that sleeps for the most minutes and at which minute
 * Then find the guard that sleeps for the most at a single minute
 *
 * Takes input via piping to stdin.
 */
int main()
{
    int day, month, year, hour, minutes, guard;
    int start, i, j, asleep, max, temp, index;
    char line[LINE_LEN];
    char desc[LINE_LEN];
    // can be made quicker by initialising sparsely, helps with all checks
    char *guards = calloc(NUM_GUARDS * 60, sizeof(*guards));
    if (!guards) {return -1;}

    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        sscanf(line, "[%d-%d-%d %d:%d] %[^\n]", &year, &month, &day, &hour, &minutes, desc);

        if (desc[0] == 'w') // "wakes up"
        {
            for (i = start; i < minutes; i++)
            {
                guards[guard * 60 + i]++;
            }
        }
        else if (desc[0] == 'f') // "falls asleep"
        {
            start = minutes;
        }
        else
        {
            sscanf(desc, "Guard #%d begins shift", &guard);
        }
    }

    // part 1
    asleep = 0;
    for (i = 0; i < NUM_GUARDS; i++)
    {
        temp = 0;
        for (j = 0; j < 60; j++) {temp += guards[i * 60 + j];}

        if (temp > asleep)
        {
            index = i;
            asleep = temp;
        }
    }

    max = 0;
    for (i = 0; i < 60; i++)
    {
        if (guards[index * 60 + i] > max)
        {
            max = guards[index * 60 + i];
            j = i;
        }
    }
    
    printf("Guard %d asleep for %dmins, most at minute %d\nanswer: %d\n", index, asleep, j, index * j);

    // part 2
    max = temp = 0;
    for (i = 0; i < 60; i++)
    {
        for (j = 0; j < NUM_GUARDS; j++)
        {
            if (guards[j * 60 + i] > max)
            {
                max = guards[j * 60 + i];
                guard = j;
                asleep = i;
            }
        }
    }

    printf("Guard %d asleep the most at minute %d\nanswer: %d\n", guard, asleep, guard * asleep);

    free(guards);

    return 0;
}
