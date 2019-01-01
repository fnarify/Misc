#include <stdio.h>
#include <stdlib.h>

#define DIM 32
#define GOB 'G'
#define ELF 'E'
#define OBS '#'
#define FRE '.'

int state[DIM * DIM] = {0};

struct Point {
    int x, y;
};
typedef struct Point Point;

struct Unit {
    char type;
    Point p;
    int att, hp, del;
};
typedef struct Unit Unit;

struct Queue
{
    Point p[DIM * DIM];
    int rear, front;
};
typedef struct Queue Queue;

// add to end of queue
void enq(Point p, Queue *q)
{
    if (q->front == -1) {q->front = 0;}
    q->p[++q->rear] = p;
}

// remove from end of queue
Point deq(Queue *q)
{
    Point p;
    if (q->rear != -1)
    {
        p = q->p[q->front];
        q->front++;
        if (q->front > q->rear) {q->front = q->rear = -1;}
    }
    return p;
}

// is valid point in grid
int isvalid(Point p, char grid[DIM][DIM + 2])
{
    int ret = 0;
    if (p.x >= 0 && p.x < DIM && p.y >= 0 && p.y < DIM)
    {
        if (grid[p.y][p.x] == '.') {ret = 1;}
    }
    return ret;
}

// dest is target to move, source is 4 adjacent squares near target to be attacked
int bfs(Point psrc, Point pdest, char grid[DIM][DIM + 2])
{
    int vis[DIM][DIM] = {{0}}, dist[DIM][DIM];
    Queue *q = malloc(sizeof(struct Queue));
    if (!q) {return -1;}
    q->front = q->rear = -1;
    dist[psrc.y][psrc.x] = 0;
    vis[psrc.y][psrc.x] = 1;


    free(q);
    return 0;
}

int main()
{
    int i, j, nunits, index, xmin, ymin;
    Unit **units;
    char grid[DIM][DIM + 2];

    // parse grid via redirection of "input15.txt"
    i = 0;
    while (fgets(&grid[i++][0], DIM + 2, stdin) != NULL) {;}

    // find number of units
    nunits = 0;
    for (i = 0; i < DIM; i++)
    {
        for (j = 0; j < DIM; j++)
        {
            if (grid[i][j] == ELF || grid[i][j] == GOB)
            {
                nunits++;
            }
        }
    }
    units = malloc(sizeof(*units) * nunits);
    if (!units) {return -1;}

    // setup all units
    index = 0;
    for (i = 0; i < DIM; i++)
    {
        for (j = 0; j < DIM; j++)
        {
            if (grid[i][j] == ELF || grid[i][j] == GOB)
            {
                units[index] = malloc(sizeof(struct Unit));
                if (!units[index]) {return -1;}
                units[index]->type = grid[i][j];
                units[index]->p.x = j;
                units[index]->p.y = i;
                units[index]->att = 3;
                units[index]->hp = 200;
                units[index]->del = 0;
                index++;
            }
        }
    }

    int target = 0, nrounds = 0, xdist, ydist;
    while (1)
    {
        for (i = 0; i < nunits; i++)
        {
            // find closest target
            xmin = ymin = DIM + 1;
            for (j = 0; j < nunits; j++)
            {
                if (i != j && units[i]->type != units[j]->type)
                {
                    xdist = abs(units[i]->p.x - units[j]->p.x);
                    ydist = abs(units[i]->p.y - units[j]->p.y);
                    if (xdist < xmin && ydist < ymin)
                    {
                        xmin = xdist;
                        ymin = ydist;
                        target = j;
                    }
                }
            }

            // breadth-first search on all valid positions adjacent to target
        }

        nrounds++;
        break;
    }

    printf("number of rounds: %d\n", nrounds);

    for (i = 0; i < nunits; i++) {free(units[i]);}
    free(units);

    return 0;
}
