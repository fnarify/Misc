#include "load.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

/**
 * String comparison used for bsearch.
 */
int strcompare(const void *x, const void *y)
{
    if (!(char *)x || !(char *)y) {return -2;}
    else {return strcmp((char *)x, (char *)y);}
}

/**
 * Checks if a given string s is composed of two subwords and if so
 * returns the index where the second subword begins.
 * Otherwise, returns 0 if two subwords do not exist.
 *
 * Given an array of strings checks if each string contains two subwords of size 
 * MIN_WORD_SIZE or greater. If so then prints the two subwords and the index where the 
 * second subword begins.
 * It will print all subwords for a given word, not just the first subword pair.
 */
void subwords(char (*words)[MAX_WORD_SIZE + 1], size_t max)
{
    int (*cmpfnc)(const void *a, const void *b) = &strcompare;
    size_t midpt, wlen, i, j;
    // Store the largest substring needed to save extra mallocs.
    char w1[MAX_WORD_SIZE - MIN_WORD_SIZE], w2[MAX_WORD_SIZE - MIN_WORD_SIZE];

    for (i = 0; i < max && words[i]; i++)
    {
        wlen = strlen(words[i]);
        midpt = MIN_WORD_SIZE - 1;

        if (wlen >= 2 * MIN_WORD_SIZE)
        {
            while (midpt++ < wlen - MIN_WORD_SIZE)
            {
                for (j = 0; j < midpt; j++)
                {
                    w1[j] = words[i][j];
                }
                w1[j] = '\0';

                for ( ; j < wlen; j++)
                {
                    w2[j - midpt] = words[i][j];
                }
                w2[j - midpt] = '\0';

                // Print subwords if they exist.
                if (bsearch(w1, words, max, MAX_WORD_SIZE + 1, cmpfnc) &&
                    bsearch(w2, words, max, MAX_WORD_SIZE + 1, cmpfnc))
                {
                    printf("%-*s %-*s => %s [%zu]\n", MAX_WORD_SIZE / 2, w1, MAX_WORD_SIZE / 2, w2, words[i], midpt);
                }
            }
        }
    }
}


/**
 * Program to find all words in a dictionary that are comprised of two subwords
 * that are also in the dictionary, and prints the results.
 *
 * Assume dictionary is already sorted.
 */
int main()
{
    clock_t start, diff;
    FILE *dict;
    char (*words)[MAX_WORD_SIZE + 1];
    size_t lines;
    
    dict = dopen(DICT);
    checkmem(dict, "cannot load default dictionary, exiting...\n", -1);

    lines = numlines(dict);

    // Allocates the maximum amount of memory needed,
    // rather than for each individual word.
    words = malloc(sizeof(**words) * lines * (MAX_WORD_SIZE + 1));
    checkmem(words, "not enough memory or empty dictionary\n", -2);

    start = clock();

    lines = load(words, dict, lines);
    fclose(dict);

    subwords(words, lines);

    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %ds %dms\n", msec / 1000, msec % 1000);

    free(words);

    return 0;
}
