#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define N_POINTS 341
#define TOLERANCE 16 // might need to be 10 since character height is 10

/**
 * Could start after approximately 10000 iterations initally to speed it up a decent amount
 */
int main()
{
    char line[51];
    char *grid;
    int x[N_POINTS], y[N_POINTS], xv[N_POINTS], yv[N_POINTS];
    int iterations, height, width, xmin, xmax, ymin, ymax, i, j;

    i = 0;
    while (fgets(line, 51, stdin) != NULL)
    {
        sscanf(line, "position=<%d, %d> velocity=<%d, %d>\n", &x[i], &y[i], &xv[i], &yv[i]);
        i++;
    }

    iterations = 0;
    while (1)
    {
        xmax = ymax = 0;
        xmin = ymin = INT_MAX;
        for (i = 0; i < N_POINTS; i++)
        {
            x[i] += xv[i];
            y[i] += yv[i];
            
            if      (x[i] < xmin) {xmin = x[i];}
            else if (x[i] > xmax) {xmax = x[i];}
            if      (y[i] < ymin) {ymin = y[i];}
            else if (y[i] > ymax) {ymax = y[i];}
        }
        iterations++;

        if (ymax - ymin <= TOLERANCE) {break;}
    }

    width = xmax - xmin + 2; // +1 for newline
    height = ymax - ymin + 1;

    grid = malloc(sizeof(*grid) * (width * height + 1));
    if (!grid) {return -1;}

    // fill grid
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width - 1; j++)
        {
            grid[i * width + j] = ' ';
        }
        grid[i * width + j] = '\n';
    }
    grid[width * height] = '\0';

    for (i = 0; i < N_POINTS; i++) {grid[width * (y[i] - ymin) + (x[i] - xmin)] = '#';}

    printf("%sIterations: %ds\n", grid, iterations);

    free(grid);

    return 0;
}
