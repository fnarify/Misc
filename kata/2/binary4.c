#include "binary.h"

int chop(int tar, int size, int *p)
{
    if (size == 1)
    {
        return (*p == tar ? 0 : -1);
    }
    else if (size > 1)
    {
        return (size & 1 ? 1 : -1) * choppa(tar, size, p, p);
    }
    else
    {
        return -1;
    }
}

/**
 * Pointer arithmetic version.
 * (relies on ptrdiff_t being one of the integer types, which is not always true).
 * Also doesn't work if the array contains the target element more than once.
 */
ptrdiff_t choppa(int tar, int size, int *p, int *inip)
{
    if (size == 1)
    {
        ptrdiff_t res = p - inip;
        return (*p == tar ? res: -1);
    }
    else if (size > 1)
    {
        int mid;
        mid = (size - 1) / 2;

        return choppa(tar, mid + 1, p, inip) * choppa(tar, size / 2, p + mid + 1, inip);
    }
    else
    {
        return -1;
    }
}
