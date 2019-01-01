#include <stdio.h>
#include <stdlib.h>

#define ROCKY  '.'
#define WET    '='
#define NARROW '|'

char type[3] = {ROCKY, WET, NARROW};

// if at 0,0 or target point then return 1
#define chk(x, y, tx, ty) ( (((x) == (tx) - 1) && ((y) == (ty) - 1)) || ((x) == 0 && (y) == 0) )

int erosion(int x, int y, int depth, int *smap, int width)
{
    int geo;
    if      (x == 0) {geo = y * 48271;}
    else if (y == 0) {geo = x * 16807;}
    else             {geo = smap[y * width + x - 1] * smap[(y - 1) * width + x];}

    smap[y * width + x] = (geo + depth) % 20183;
    return smap[y * width + x];
}

int main()
{
    char *grid, line[50];
    int *smap, risk, depth, eros, tx, ty, i, j;

    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        if      (line[0] == 'd') {sscanf(line, "depth: %d\n", &depth);}
        else if (line[0] == 't') {sscanf(line, "target: %d,%d\n", &tx, &ty);}
    }

    tx++;
    ty++;
    grid = malloc(sizeof(*grid) * tx * ty);
    smap = malloc(sizeof(*smap) * tx * ty);
    if (!grid || !smap) {return -1;}

    risk = 0;
    for (i = 0; i < ty; i++)
    {
        for (j = 0; j < tx; j++)
        {
            eros = chk(j, i, tx, ty) ? (depth % 20183) : erosion(j, i, depth, smap, tx);
            eros %= 3;

            grid[i * tx + j] = type[eros];
            risk += eros;
        }
    }

    printf("Risk=%d for target=%d,%d with depth=%d\n", risk, tx - 1, ty - 1, depth);

    free(smap);
    free(grid);

    return 0;
}
