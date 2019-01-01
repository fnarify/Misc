#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define IN_LEN 50000
#define IN_WID 26

/**
 * AoC 2018 part 2
 * Assumes each line has the same amount of characters
 *
 * Reads the test file via redirection.
 */
int main()
{
    char c, ndiff;
    int i, j, k, dindex;
    char *list = malloc(sizeof(*list) * IN_LEN * IN_WID); // row-major order

    i = 0;
    while (read(STDIN_FILENO, &c, 1))
    {
        if (c != '\n') {list[i++] = c;}
    }


    for (i = 0; i < IN_LEN; i++)
    {
        for (j = i + 1; j < IN_LEN; j++)
        {
            for (k = 0, ndiff = 0; k < IN_WID; k++)
            {
                if (list[i * IN_WID + k] != list[j * IN_WID + k])
                {
                    if (++ndiff > 1) {break;}
                    dindex = k;
                }
            }

            if (ndiff == 1)
            {
                printf("%.*s%.*s", dindex, &list[i * IN_WID], IN_WID - dindex - 1, &list[i * IN_WID + dindex + 1]);
                i = j = IN_LEN;
            }
        }
    }

    free(list);

    return 0;
}
