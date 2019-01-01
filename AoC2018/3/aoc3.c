#include <stdio.h>
#include <stdlib.h>

#define DIM 1000

/**
 * Checking for overlap on a 1000 x 1000 matrix.
 */
int main()
{
    int id, xoff, yoff, width, height, overlaps, i, j;
    char *fabric = calloc(DIM * DIM, sizeof(*fabric));
    if (!fabric) {return -1;}
    FILE *f = fopen("input3.txt", "r");
    if (!f) {return -3;}

    // part 1
    overlaps = 0;
    while (fscanf(f, "#%d @ %d,%d: %dx%d\n", &id, &xoff, &yoff, &width, &height) != EOF)
    {
        for (i = xoff; i < xoff + width; i++)
        {
            for (j = yoff; j < yoff + height; j++)
            {
                if (++fabric[i + j * DIM] == 2) {overlaps++;}
            }
        }
    }
    printf("Overlapping of %d square inches\n", overlaps);

    // part 2
    rewind(f);
next:while (fscanf(f, "#%d @ %d,%d: %dx%d\n", &id, &xoff, &yoff, &width, &height) != EOF)
    {
        for (i = xoff; i < xoff + width; i++)
        {
            for (j = yoff; j < yoff + height; j++)
            {
                if (fabric[i + j * DIM] > 1) {goto next;}
            }
        }

        printf("no overlap at id: %d\n", id);
        break;
    }

    fclose(f);
    free(fabric);

    return 0;
}
