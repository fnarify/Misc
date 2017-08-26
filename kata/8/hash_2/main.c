#include "subwords.h"
#include <string.h>
#include <time.h>

#define H_MAX_SIZE 700000 // Max size for the hashmap.

/**
 * Program to find all words in a dictionary that are comprised of two subwords
 * that are also in the dictionary, and prints the results.
 */
int main()
{
    FILE *dict;
    char word[MAX_WORD_SIZE + 1], pword[MAX_WORD_SIZE + 1];
    struct HashMap *map = initmap(H_MAX_SIZE);
    size_t size = map->tsize, slen, midpt, i;

    clock_t start = clock(), diff;

    load(map, size);

    dict = dopen(DICT);
    checkmem(dict, "cannot load default dictionary, exiting...\n", -1);

    // Get each word from a dictionary to compare for subwords.
    while (fgets(word, MAX_WORD_SIZE + 1, dict))
    {
        word[strcspn(word, "\n")] = '\0';
        lstr(word);

        slen = strlen(word);
        midpt = subword(map, word);

        // Print subwords when they exist.
        if (midpt)
        {
            for (i = 0; i < midpt; i++)
            {
                pword[i] = word[i];
            }
            pword[i] = ' ';
            while (i++ < slen)
            {
                pword[i] = word[i - 1];
            }
            pword[i] = '\0';
            
            printf("%-*s => %s [%zu]\n", MAX_WORD_SIZE, pword, word, midpt);
        }
    }

    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %ds %dms\n", msec/1000, msec%1000);

    fclose(dict);

    cleanmap(map);
    return 0;
}
