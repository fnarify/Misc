/**
 * Provides the means for loading a file, in this case a dictionary,
 * and performing what is needed on each word.
 *
 * Case-sensitive.
 */
#include <stdlib.h>
#include <string.h>
#include "load.h"

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

size_t numlines(FILE *fp)
{
    if (!fp) {return 0;}

    size_t cnt = 0;
    char c;
    while (!feof(fp))
    {
        c = fgetc(fp);
        if (c == '\n') {cnt++;}
    }

    return cnt;
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
 * Allocate each word in a dictionary into each array element of words up to
 * a number ln.
 */
size_t load(char (*words)[MAX_WORD_SIZE + 1], FILE *dict, size_t ln)
{
    checkmem(*words, "no array\n", 1);
    checkmem(dict, "cannot load default dictionary, exiting...\n", 2);

    rewind(dict); // Set dictionary to beginning.

    char word[MAX_WORD_SIZE + 1];
    size_t nwords = 0, lnsize = 0;

    while (nwords < ln)
    {
        // Read each line.
        if (fgets(word, MAX_WORD_SIZE + 1, dict))
        {
            // Don't read more then MAX_WORD_SIZE characters per line.
            lnsize = strcspn(word, "\n");
            word[lnsize] = '\0';
            // If no new-line read go to next line.
            if (lnsize == MAX_WORD_SIZE) {nextline(dict);}
            // Only add words that are the same size or greater than the minimum.
            if (lnsize > MIN_WORD_SIZE - 1)
            {
                strcpy(words[nwords], word);
                nwords++;
            }
        }
        else
        {
            break;
        }
    }

    fclose(dict);

    return nwords;
}
