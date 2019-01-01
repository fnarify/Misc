#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define H 1800
#define W 800
#define SRC 500
#define OPEN '.'
#define OBST '#'
#define STAG '~'
#define FLOW '|'

int descend(int x, int y, char *grid)
{
    y++;
    while (grid[y * W + x] != OBST && grid[y * W + x] != STAG)
    {
        if (grid[y * W + x] == FLOW || y > H - 1) {return 0;}
        grid[y * W + x] = FLOW;
        y++;
    }
    return y;
}

/**
 * Recursive function which fills the container if it detects both sides are bounded
 * otherwise it will overflow and descend on the sides that are not bounded
 */
void fill(int x, int y, char *grid)
{
    int i, mid, lbound, rbound, xleft, xright, temp;
    char type = STAG;

    mid = x;
    xleft = xright = 0;
    lbound = rbound = 1;

    // check left side bound
    while (1)
    {
        x--;
        if (grid[y * W + x] == OBST)
        {
            xleft = x + 1;
            break;
        }
        else if (grid[(y + 1) * W + x] == OPEN)
        {
            // edge case when flowing water lands on a container in a container
            xleft = grid[(y + 1) * W + x + 1] == FLOW ? x + 1 : x;
            type = FLOW;
            lbound = 0;
            break;
        }
    }

    // check right side bound
    x = mid;    
    while (1)
    {
        x++;
        if (grid[y * W + x] == OBST)
        {
            xright = x;
            break;
        }
        else if (grid[(y + 1) * W + x] == OPEN)
        {
            xright = grid[(y + 1) * W + x - 1] == FLOW ? x : x + 1;
            type = FLOW;
            rbound = 0;
            break;
        }
    }

    // fill row
    for (i = xleft; i < xright; i++) {grid[y * W + i] = type;}

    temp = y;
    // recursive descend
    if (!lbound)
    {
        y = descend(xleft, y, grid);
        if (y) {fill(xleft, y - 1, grid);}
    }

    y = temp;
    if (!rbound)
    {
        xright--;
        y = descend(xright, y, grid);
        if (y) {fill(xright, y - 1, grid);}
    }

    // bounded both sides so flow up one
    if (rbound && lbound) {fill(mid, y - 1, grid);}
}

/**
 * Water flow simulation into containers
 * doesn't obey actual flow mechanics, and so only fills the side of the container it starts in,
 * if there is an obstacle as it flows upwards.
 */
int main()
{
    int x1, x2, y1, y2, ymin, xmin, ymax, xmax, stagw, runw;
    char line[50];
    char *grid = malloc(sizeof(*grid) * H * W);
    if (!grid) {return -1;}
    memset(grid, '.', H * W);
    grid[SRC] = '+';

    xmin = ymin = INT_MAX;
    xmax = ymax = 0;
    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        if (line[0] == 'x')
        {
            sscanf(line, "x=%d, y=%d..%d\n", &x1, &y1, &y2);

            if (y1 < ymin) {ymin = y1;}
            if (y2 > ymax) {ymax = y2;}
            if      (x1 < xmin) {xmin = x1;}
            else if (x1 > xmax) {xmax = x1;}

            while (y1 <= y2)
            {
                grid[y1 * W + x1] = OBST;
                y1++;
            }
        }
        else
        {
            sscanf(line, "y=%d, x=%d..%d\n", &y1, &x1, &x2);

            if      (y1 < ymin) {ymin = y1;}
            else if (y1 > ymax) {ymax = y1;}
            if (x1 < xmin) {xmin = x1;}
            if (x2 > xmax) {xmax = x2;}

            while (x1 <= x2)
            {
                grid[y1 * W + x1] = OBST;
                x1++;
            }
        }
    }

    fill(SRC, descend(SRC, 0, grid) - 1, grid);

    stagw = runw = 0;
    for (y1 = ymin; y1 <= ymax; y1++)
    {
        for (x1 = xmin - 1; x1 <= xmax + 1; x1++)
        {
            if      (grid[y1 * W + x1] == STAG) {stagw++;}
            else if (grid[y1 * W + x1] == FLOW) {runw++;}
        }
    }

    printf("Flowing water: %d, stagnant water: %d, total water: %d\n", runw, stagw, runw + stagw);

    free(grid);

    return 0;
}
