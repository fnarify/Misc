#include "binary.h"
#include <stdio.h>

int chop(int tar, int size, int *p)
{
    if (size > 0) 
    {
        int temp = 0;
        int *acc = &temp;

        chopp(tar, size, p, acc);
        return *acc;
    }
    else 
    {
        return -1;
    }
}

/**
 * Pointer recursive version.
 * Assume the accumulator has already been set to 0 before the first call.
 */
void chopp(int tar, int size, int *p, int *acc)
{
    int mid, high;
    high = size - 1;
    mid = high / 2;
    *acc += mid; 

    if (high < 0) 
    {
        *acc = -1;
    }
    else if (tar > *(p + mid))
    {
        if (mid == 0) {(*acc)++;} else {*acc += mid;}
        chopp(tar, size - (mid + 1), p + mid + 1, acc);
    }
    else if (tar < *(p + mid))
    {
        *acc -= mid;
        chopp(tar, size - (mid + 1), p, acc);
    }
}

