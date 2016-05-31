#include "binary.h"

int chop(int tar, int size, int *p)
{
    return chopr(tar, 0, size - 1, p);
}

/**
 * Function recursive.
 */
int chopr(int tar, int start, int end, int *p)
{
    int mid = (start + end) / 2;

    if (start > end) {return -1;}

    if (tar == p[mid]) {return mid;}
    else if (tar > p[mid]) {return chopr(tar, mid + 1, end, p);}
    else {return chopr(tar, start, mid - 1, p);}
}
