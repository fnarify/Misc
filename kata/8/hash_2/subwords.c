#include "subwords.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Checks if lw contains the string s.
 * Returns 0 if true and -1 if not.
 */
int contains(struct LinkedWords *lw, char *s)
{
    checkmem(lw, "empty linked list\n", -1);

    struct LinkedWords *cur = lw;
    size_t slen = strlen(s);

    while (cur)
    {
        if (slen == strlen(cur->word))
        {
            if (!memcmp(s, cur->word, slen)) {return 0;}
        }

        cur = cur->next;
    }

    return -1;
}

/**
 * Checks if a given string s is composed of two subwords and if so
 * returns the index where the second subword begins.
 * Otherwise, returns 0 if two subwords do not exist.
 */
size_t subword(struct HashMap *m, char *s)
{
    int (*cmpfnc)(const void *a, const void *b) = &compare;
    size_t slen, midpt, memsize, i;
    char *w1[2], *w2[2];
    struct LinkedWords *v1, *v2;

    slen = s ? strlen(s) : 0;
    midpt = MIN_WORD_SIZE;
    memsize = slen - MIN_WORD_SIZE + 1;

    // Only check for words that contain two subwords both greater than MIN_WORD_SIZE.
    if (slen > 2 * MIN_WORD_SIZE + 1)
    {
        // As quicksort sorts in-place, we need to store
        // the sorted and unsorted string for each.
        w1[0] = malloc(sizeof(char) * memsize);
        w1[1] = malloc(sizeof(char) * memsize);
        w2[0] = malloc(sizeof(char) * memsize);
        w2[1] = malloc(sizeof(char) * memsize);
        if (!w1[0] || !w1[1] || !w2[0] || !w2[1])
        {
            fprintf(stderr, "not enough memory\n");
            // Skip while loop without a jump.
            midpt = slen - MIN_WORD_SIZE;
        }

        // Iterate through the string s taking incrementative
        // midpoints and checking if they are valid words.
        while (midpt++ < slen - MIN_WORD_SIZE)
        {
            // We take and search each string in only lowercase.
            for (i = 0; i < midpt; i++)
            {
                w1[0][i] = w1[1][i] = tolower(s[i]);
            }
            w1[0][i] = w1[1][i] = '\0';

            for ( ; i < slen; i++)
            {
                w2[0][i - midpt] = w2[1][i - midpt] = tolower(s[i]);
            }
            w2[0][i - midpt] = w2[1][i - midpt] = '\0';
 
            qsort(w1[1], midpt, sizeof(char), cmpfnc);
            qsort(w2[1], slen - midpt, sizeof(char), cmpfnc);

            // Attempt to find if both subwords are contained in the hashmap
            // and then check if it is actually contained in the value linked list.
            if ((v1 = find(m, w1[1])) && (v2 = find(m, w2[1])))
            {
                if (!contains(v1, w1[0]) && !contains(v2, w2[0]))
                {
                    return midpt;
                }
            }
        }

        free(w1[0]); free(w1[1]);
        free(w2[0]); free(w2[1]);
    }

    return 0;
}
