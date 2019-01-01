#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 10
#define NUM_LOOP 140 // 140 for first duplicate

/**
 * Advent of Code 1:
 * Prints the first duplicate value from the summation list input1.txt
 * if NUM_LOOP is set to 1 it will print the sum after just 1 iteration.
 *
 * Run it once to find maximum, approximately double it for list length and use the original value as the offset.
 * For simplicity, if the summation is small enough on the initial run, then the maximum will likely be very close
 * to the absolute value of the largest value in the input list.
 */
int main(int argc, char *argv[])
{
    char freq[MAX_LINE];
    char *list = malloc(sizeof(list) * 160000);
    int sum = 0, n = 0;

    if (argc > 1)
    {
        FILE *file = fopen(argv[1], "r");

        if (!file)
        {
            printf("Can't open file: %s\n", argv[1]);
            return -1;
        }

        while (n < NUM_LOOP)
        {
            while (fgets(freq, MAX_LINE, file))
            {
                sum += atoi(freq);
                if (list[sum + 80000] == 1)
                {
                    printf("duplicate: %d\n", sum);
                    fclose(file);
                    return 1;
                }
                list[sum + 80000] = 1;
            }
            rewind(file);
            n++;
        }

        printf("total freq: %d\n", sum);
        fclose(file);
    }

    free(list);

    return 0;
}
