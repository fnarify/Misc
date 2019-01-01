#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NPTS 1405

struct Point {
    int x, y, z, t;
};
typedef struct Point Point;

struct Group {
    Point *pts;
    int size;
};
typedef struct Group Group;

int mdist(Point a, Point b)
{
    return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z) + abs(a.t - b.t);
}

/**
 * appends p to pts in group g at index i
 */
void addtogroup(Point p, Group *g, int i)
{
    g[i].pts[g[i].size].x = p.x;
    g[i].pts[g[i].size].y = p.y;
    g[i].pts[g[i].size].z = p.z;
    g[i].pts[g[i].size].t = p.t;
    g[i].size++;
}

/**
 * Points can form a constellation after a point that occurs later in our input list is checked
 * so run through a loop until you find a point that can be part of a constellation, otherwise
 * ignore it till later
 */
int main()
{
    int i, j, k, gsize, psize;
    char line[50];

    Point *p = malloc(sizeof(*p) * NPTS);
    Group *group = malloc(sizeof(*group) * NPTS);
    if (!group || !p) {perror("Not enough memory\n"); return -1;}

    gsize = 0;
    for (i = 0; i < NPTS; i++)
    {
        group[i].pts = malloc(sizeof(*group[i].pts) * NPTS);
        if (!group[i].pts) {perror("Not enough memory\n"); return -1;}
        group[i].size = 0;
    }

    psize = 0;
    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        sscanf(line, "%d,%d,%d,%d\n", &p[psize].x, &p[psize].y, &p[psize].z, &p[psize].t);
        psize++;
    }

    // stack like implementation to add stars rather than a union
    // since any possible grouping needs to be checked for all points before a point is added
cont:while (psize)
    {
        for (i = 0; i < psize; i++)
        {
            for (j = 0; j < gsize; j++)
            {
                for (k = 0; k < group[j].size; k++)
                {
                    // if close neighbour found, add and repeat loop
                    if (mdist(p[i], group[j].pts[k]) <= 3)
                    {
                        addtogroup(p[i], group, j);
                        // swap values
                        psize--;
                        p[i].x = p[psize].x;
                        p[i].y = p[psize].y;
                        p[i].z = p[psize].z;
                        p[i].t = p[psize].t;
                        goto cont;
                    }
                }
            }
        }

        // only if it can't add to any group
        addtogroup(p[--psize], group, gsize++);
    }
    printf("Number of constellations: %d\n", gsize);

    for (i = 0; i < NPTS; i++) {free(group[i].pts);}
    free(group);
    free(p);

    return 0;
}
