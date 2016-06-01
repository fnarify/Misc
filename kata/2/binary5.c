#include "binary.h"
#include <stdlib.h>

/**
 * Random value between 0 and lim (inclusive).
 */
int rand_lim(int lim)
{
    int div = RAND_MAX / (lim + 1);
    int val;

    do
    {
        val = rand() / div;
    } while (val > lim);

    return val;
}

/**
 * Performs a binary search using a random value in the range of the array
 * as opposed to the value in the middle.
 */
int chop(int tar, int size, int *p)
{
    int low, mid, high;
    low = 0; high = size - 1;

    while (low <= high)
    {
        mid = low + rand_lim(high - low);

        if (tar == p[mid]) {return mid;}
        else if (tar > p[mid]) {low = mid + 1;}
        else {high = mid - 1;}
    }
    return -1;
}

