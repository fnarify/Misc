/**
 * Simple program to search for anagrams in text files, or as implemented in
 * system dictionaries and wordlists. It one word per line in the file, 
 * sorts it lowercase lexicographically, and places that as the key in a hashmap
 */
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "anagram.h"

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
 * Go to start of next line in file.
 */
int nextline(FILE *fp)
{
    while (fgetc(fp) != '\n')
    {
        if (feof(fp)) {return 0;}
    }
    return 1;
}

/**
 * Fills hashmap m with key/pair values, each string is in lowercase.
 * adds either the max amount of words that can be stored in the hashmap, 
 * or nw amount of words.
 *
 * returns the amount of words succesfully added.
 */
size_t fillmap(struct HashMap *m, size_t nw)
{
    checkmem(m, "no hashmap\n", -1);

    int (*cmpfnc)(const void *, const void *) = &compare;
    char val[MAX_WORD_SIZE + 1], key[MAX_WORD_SIZE];
    size_t nwords = 0, msize = m->tsize, cnt = 0;
    FILE *dict = fopen(DICT, "r");
    if (!dict)
    {
        fprintf(stderr, "no dictionary in %s to read\n", DICT);
        fprintf(stderr, "loading default dictionary file %s\n", DEF_DICT);

        dict = fopen(DEF_DICT, "r");
        if (!dict)
        {
            fprintf(stderr, "cannot load default dictionary, exiting...\n");
            return -2;
        }
    }

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

/**
 * Prints all anagrams in the hasmap, that is
 * all lists that contain more than 1 element.
 *
 * Returns how many sets of anagrams there are.
 */
size_t printanagrams(struct HashMap *m)
{
    checkmem(m, "no hashmap\n", -1);

    size_t cnt = 0;
    size_t i, j, size = m->tsize, vsize;
    struct LinkedWords *words;

    for (i = 0; i < size; i++)
    {
        if (m->keys[i])
        {
            words = m->values[i].words;
            vsize = m->values[i].vsize;
            
            if (vsize > 1)
            {
                for (j = 0; j < vsize; j++)
                {
                    printf("%s ", words->word);
                    words = words->next;
                }
                printf("\n");
                cnt++;
            }
        }
    }

    return cnt;
}
