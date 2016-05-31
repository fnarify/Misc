#include "binary.h"

/**
 * Iterative method.
 */
int chop(int tar, int size, int *p)
{
    int low, mid, high;
    low = 0;
    high = size - 1;

    while (low <= high)
    {
        mid = (low + high) / 2;

        if (tar == p[mid]) {return mid;}
        else if (tar > p[mid]) {low = mid + 1;}
        else {high = mid - 1;}
    }

    return -1;
}
