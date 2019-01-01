#include <stdio.h>
#include <stdlib.h>

void spiral(size_t *, size_t, size_t);
void print_spiral(size_t *, size_t, size_t);

int main()
{
    size_t width = 17;
    size_t height = 3;

    size_t *matrix = calloc(width * height, sizeof(size_t));
    if (!matrix) {return 1;}

    spiral(matrix, width, height);
    print_spiral(matrix, width, height);

    free(matrix);
    return 0;
}

/**
 * creates an involute spiral, stored in a 1-d array.
 * Requires an array of size at least w*h.
 * This function does not check the array for nullity etc.
 */
void spiral(size_t *mat, size_t w, size_t h)
{
    size_t size, min_h, min_w;
    size_t x, y, cnt;
    x = y = min_h = min_w = cnt = 0;
    size = h * w;

    while (cnt < size)
    {
        while (x < w && cnt < size)
        {
            mat[(min_w + w) * x++ + y] = cnt++;
        }
        x--;
        y++;
        while (y < h && cnt < size)
        {
            mat[(min_w + w) * x + y++] = cnt++;
        }
        y--;
        while (x > min_w && cnt < size)
        {
            mat[(min_w + w) * --x + y] = cnt++;
        }
        y--;
        while (y > min_h && cnt < size)
        {
            mat[(min_w + w) * x + y--] = cnt++;
        }
        // constrain conditions to start of next spiral loop
        min_h++;
        min_w++;
        x++;
        y++;
        w--;
        h--;
    }
}

void print_spiral(size_t *mat, size_t w, size_t h)
{
    size_t x, y;
    for (y = 0; y < h; y++)
    {
        for (x = 0; x < w; x++)
        {
            printf("%02zu ", mat[w * x + y]);
        }
        printf("\n");
    }
}
