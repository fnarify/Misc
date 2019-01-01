#include <stdio.h>
#include <stdlib.h>

#define SERIAL 2694
#define G_SIZE 300

/**
 * sums can be calculated nicely by moving left to right then right to left while removing only the 
 * left 3 values or right 3 (if moving R->L), and removing the top 3 values if moving down a row.
 *
 * or by building a summation map, ergo adding like so:
 * a b
 * c d
 * for the first iteration, each letter denotes the sum for size 1:
 * for size 2 the sum would then be a + b + c + d for point 0,0
 *
 * For part 1, just compile and run the program with size set to 3
 */
int main()
{
    int i, j, k, l, sum, max, x, y, size, dim;
    int *grid = malloc(sizeof(*grid) * G_SIZE * G_SIZE);
    if (!grid) {return -1;}

    for (i = 0; i < G_SIZE; i++)
    {
        for (j = 0; j < G_SIZE; j++)
        {
            // x, y are 1-indexed
            // [(y(x + 10) + serial)(x + 10) / 100] % 10 - 5
            grid[i * G_SIZE + j] = (((((j + 11) * (i + 1) + SERIAL) * (j + 11)) / 100 ) % 10) - 5;
        }
    }
    
    // horrible brute force attempt, ignore the first iteration
    x = y = dim = max = 0;
    for (size = 2; size < G_SIZE; size++)
    {
        for (i = 0; i < G_SIZE - size; i++)
        {
            for (j = 0; j < G_SIZE - size; j++)
            {
                sum = 0;
                for (k = i; k < i + size; k++)
                {
                    for (l = j; l < j + size; l++)
                    {
                        sum += grid[k * G_SIZE + l];
                    }
                }

                if (sum > max)
                {
                    max = sum;
                    dim = size;
                    x = j + 1;
                    y = i + 1;
                }
            }
        }
    }

    free(grid);

    printf("%d,%d,%d\n", x, y, dim);

    return 0;
}
