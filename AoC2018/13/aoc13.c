#include <stdio.h>
#include <stdlib.h>

#define LEFT  '<'
#define RIGHT '>'
#define UP    '^'
#define DOWN  'v'
#define LEN   150

struct Cart {
    int x, y, s, del;
    char dir;
    char prev;
};
typedef struct Cart Cart;

char slash[] = {[LEFT] = DOWN, [RIGHT] = UP, [UP] = RIGHT, [DOWN] = LEFT}; // '/'
char blash[] = {[LEFT] = UP, [RIGHT] = DOWN, [UP] = LEFT, [DOWN] = RIGHT}; // '\'
// map directions to ints
int map[] = {[LEFT] = 0, [RIGHT] = 1, [UP] = 2, [DOWN] = 3};
// turn left, go straight, turn right
char direction[4][3] = {
    {DOWN, LEFT, UP},    // left
    {UP, RIGHT, DOWN},   // right
    {LEFT, UP, RIGHT},   // up
    {RIGHT, DOWN, LEFT}  // down
};


/**
 * Sorts the carts based on their y value, and then on their x value if their y value is identical
 * places all carts with del set to the end of the list
 *
 * returns the size of the list after clearing memory
 */
int sort(Cart *carts[], int csize)
{
    int i, j, min, index;
    Cart *temp;

    // make sure the deleted carts end up at the end
    for (i = 0; i < csize; i++)
    {
        if (carts[i]->del) {carts[i]->y = LEN + 1;}
    }

    // sort by y then x
    for (i = 0; i < csize; i++)
    {
        min = carts[i]->y;
        index = 0;
        for (j = i + 1; j < csize; j++)
        {
            if (carts[j]->y <= min)
            {
                min = carts[j]->y;
                index = j;
            }
        }

        if (min != carts[i]->y || (index && carts[i]->x > carts[index]->x))
        {
            temp = carts[index];
            carts[index] = carts[i];
            carts[i] = temp;
        }
    }

    // free memory
    index = 0;
    for (i = 0; i < csize; i++)
    {
        if (carts[i]->del)
        {
            index++;
            free(carts[i]);
        }
    }

    return csize - index;
}

/**
 * Collisions on minecarts
 * can make csize independent of a set value by just running the loop twice
 */
int main()
{
    int i, j, index, csize = 17;
    char grid[LEN][LEN + 2];
    Cart *carts[csize];
    for (i = 0; i < csize; i++)
    {
        carts[i] = malloc(sizeof(struct Cart));
        if (!carts[i]) {return -1;}
    }
    
    // parse input file
    i = index = 0;
    while (fgets(&grid[i++][0], sizeof(*grid), stdin) != NULL) {;}

    // build carts list
    for (i = 0; i < LEN; i++)
    {
        for (j = 0; j < LEN; j++)
        {
            switch (grid[i][j])
            {
                // let it fall through
                case LEFT: case RIGHT: case UP: case DOWN:
                    carts[index]->x = j;
                    carts[index]->y = i;
                    carts[index]->s = 0;
                    carts[index]->del = 0;
                    carts[index]->dir = grid[i][j];
                    carts[index]->prev = (grid[i][j] == '>' || grid[i][j] == '<' ? '-' : '|');
                    index++;
                    break;
            }
        }
    }

    // move carts
    while (csize > 1)
    {
        // iterate through carts
        for (i = 0; i < csize; i++)
        {
            if (carts[i]->del) {continue;}

            grid[carts[i]->y][carts[i]->x] = carts[i]->prev;
            if (!carts[i]->del)
            {
                switch(carts[i]->dir)
                {
                    case LEFT:
                        carts[i]->x--;
                        break;

                    case RIGHT:
                        carts[i]->x++;
                        break;

                    case UP:
                        carts[i]->y--;
                        break;

                    case DOWN:
                        carts[i]->y++;
                        break;
                }

                switch(grid[carts[i]->y][carts[i]->x])
                {
                    case LEFT: case RIGHT: case UP: case DOWN: // collision
                        printf("Col @ %d,%d\n", carts[i]->x, carts[i]->y);
                        for (j = 0; j < csize; j++)
                        {
                            if (i != j && carts[j]->x == carts[i]->x && carts[j]->y == carts[i]->y) {break;}
                        }
                        carts[i]->dir = carts[j]->prev;
                        carts[i]->del = carts[j]->del = 1;
                        break;
                    case '-':
                        carts[i]->prev = '-';
                        break;
                    case '|':
                        carts[i]->prev = '|';
                        break;
                    case '/':
                        carts[i]->dir = slash[(int) carts[i]->dir];
                        carts[i]->prev = '/';
                        break;
                    case '\\':
                        carts[i]->dir = blash[(int) carts[i]->dir];
                        carts[i]->prev = '\\';
                        break;
                    case '+':
                        carts[i]->dir = direction[map[(int) carts[i]->dir]][carts[i]->s];
                        if (++carts[i]->s > 2) {carts[i]->s = 0;}
                        carts[i]->prev = '+';
                        break;
                }
            }
            grid[carts[i]->y][carts[i]->x] = carts[i]->dir;
        }

        csize = sort(carts, csize);
    }

    printf("%d,%d\n", carts[0]->x, carts[0]->y);
    free(carts[0]);

    return 0;
}
