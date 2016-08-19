#include "anagram.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

/**
 * Example of running our anagram detector.
 */
int main()
{
    clock_t start = clock(), diff;

    struct HashMap *map = initmap(MAX_WORDS);

    printf("%zu words added\n-\n", fillmap(map, map->tsize));

    printf("%zu anagrams found\n-\n", printanagrams(map));

    cleanmap(map);

    diff = clock() - start;

    size_t msec = diff * 1000 / CLOCKS_PER_SEC;

    printf("time taken %zu (s) %zu (ms)\n", msec / 1000, msec % 1000);

    return 0;
}
