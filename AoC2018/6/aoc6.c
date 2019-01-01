#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define POINTS 50

int main()
{
    
    int xmin, ymin, xmax, ymax;
    int i, j, n, index, manhattan, temp, distances, region = 0;

    char line[26];

    int *grid;
    int *x = malloc(sizeof(*x) * POINTS);
    int *y = malloc(sizeof(*y) * POINTS);

    int *sum  = calloc(POINTS, sizeof(*sum));
    char *inf = calloc(POINTS, sizeof(*inf));

    if (!x | !y | !sum | !inf)
    {
        perror("No memory\n");
        return -1;
    }

    n = xmax = ymax = 0;
    xmin = ymin = INT_MAX;
    while (fgets(line, 26, stdin) != NULL)
    {
        sscanf(line, "%d, %d\n", &x[n], &y[n]);

        if      (x[n] < xmin) {xmin = x[n];}
        else if (x[n] > xmax) {xmax = x[n];}

        if      (y[n] < ymin) {ymin = y[n];}
        else if (y[n] > ymax) {ymax = y[n];}

        n++;
    }

    // can save 4 * (xmax - xmin + ymax - ymin) bytes worth of memory but makes the maths look worse
    grid = calloc(xmax * ymax, sizeof(*grid));
    if (!grid)
    {
        perror("No memory\n");
        return -2;
    }

    // part 1: find the size of the finite area with the most closest neighbours
    for (i = xmin; i < xmax; i++)
    {
        for (j = ymin; j < ymax; j++)
        {
            temp = xmax + 1;
            distances = 0; // part r
            // find minimum manhattan distance
            for (n = 0; n < POINTS; n++)
            {
                manhattan = abs(i - x[n]) + abs(j - y[n]);
                distances += manhattan; // part 2
                if (manhattan < temp)
                {
                    temp = manhattan;
                    index = n;
                }
            }

            if (grid[i * ymax + j]) // duplicate distance
            {
                grid[i * ymax + j] = -1;
            }
            else
            {
                grid[i * ymax +j] = index;
                sum[index]++;

                // remove any points that touch (or if their closest neighbours touch) the edges of the grid
                if (!inf[index] && (i == xmin | i == xmax || j == ymin || j == ymax)) {inf[index] = 1;}
            }

            // part 2: points where the sum of the manhattan distances to all specified points is less than
            if (distances < 10000) {region++;}
        }
    }

    free(grid);
    free(x);
    free(y);

    manhattan = 0;
    for (i = 0; i < POINTS; i++)
    {
        if (sum[i] > manhattan && !inf[i]) {manhattan = sum[i];}
    }
    printf("largest area: %d\nregion size: %d\n", manhattan, region);

    free(sum);
    free(inf);

    return 0;
}
