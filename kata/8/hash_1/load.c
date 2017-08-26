/**
 * Provides the means for loading a file, in this case a dictionary,
 * and performing what is needed on each word.
 */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "load.h"

/**
 * Converts a string s to all lower case letters.
 */
void lstr(char *s)
{
    for ( ; *s; s++) {*s = tolower(*s);}
}

/**
 * Comparison function for qsort.
 */
int compare(const void *a, const void *b)
{
    return (int) (*(char *)a - *(char *)b);
}

/**
 * Opens a dictionary file given, otherwise load the default
 * dictionary defined in the header file.
 */
FILE *dopen(char *dict)
{
    FILE *temp = fopen(dict, "r");
    if (!temp)
    {
        fprintf(stderr, "no dictionary in %s to read\n", dict);
        fprintf(stderr, "loading default dictionary file %s\n", DEF_DICT);

        temp = fopen(DEF_DICT, "r");
    }

    return temp;
}

/**
 * Loads a hashmap m with nw words from a defined dictionary file,
 * and returns the amount of words succesfully added.
 */
size_t load(struct HashMap *m, size_t nw)
{
    checkmem(m, "no hashmap\n", -1);

    int (*cmpfnc)(const void *, const void *) = &compare;
    char val[MAX_WORD_SIZE + 1], key[MAX_WORD_SIZE];
    size_t nwords = 0, msize = m->tsize, cnt = 0;
    FILE *dict = dopen(DICT);
    checkmem(dict, "cannot load default dictionary, exiting...\n", -1);

    while ((nwords < nw) && (nwords < msize))
    {
        // Read each line.
        if (fgets(val, MAX_WORD_SIZE + 1, dict))
        {
            // Remove trailing newline.
            val[strcspn(val, "\n")] = '\0';

            lstr(val);
            strcpy(key, val);

            qsort(key, strlen(key), sizeof(char), cmpfnc);
            if (!addpair(m, key, val)) {cnt++;}

            nwords++;
        }
        else
        {
            break;
        }
    }

    fclose(dict);

    return cnt;
}

