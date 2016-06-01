#include <assert.h>
#include <stdio.h>
#include "binary5.c"

int main()
{
    int p[4] = {1, 3, 5, 7};
    assert(chop(3, 0, p) == -1);
    assert(chop(3, 1, p) == -1);
    assert(chop(1, 1, p) == 0);

    assert(chop(1, 3, p) == 0);
    assert(chop(3, 3, p) == 1);
    assert(chop(5, 3, p) == 2);
    assert(chop(0, 3, p) == -1);
    assert(chop(2, 3, p) == -1);
    assert(chop(4, 3, p) == -1);
    assert(chop(6, 3, p) == -1);

    assert(chop(1, 4, p) == 0);
    assert(chop(3, 4, p) == 1);
    assert(chop(5, 4, p) == 2);
    assert(chop(7, 4, p) == 3);
    assert(chop(0, 4, p) == -1);
    assert(chop(2, 4, p) == -1);
    assert(chop(4, 4, p) == -1);
    assert(chop(6, 4, p) == -1);
    assert(chop(8, 4, p) == -1);

    printf("All tests passed without fail\n");
    return 0;
}
