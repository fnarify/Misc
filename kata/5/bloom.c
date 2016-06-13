#include "bloom.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <openssl/sha.h>

#define BITS          CHAR_BIT                // Bits in a byte (typically 8).
#define FIELD_SIZE    BITS * sizeof(elem_f)   // Size of field in bits.
#define ELEMS         400                     // Number of field elements.
#define SIZE_BYTE     sizeof(elem_f) * ELEMS  // Size of bloom filter in bytes.
#define SIZE_BIT      BITS * SIZE_BYTE        // Size of bloom filter in bits.
#define DICT          "/usr/share/dict/words" // Dictionary file.
#define SAVE          "bloom"                 // Filename to save filter to.
#define MAX_WORD_SIZE 46                      // Max char size of any word.
#define MAX_WORDS     1000                    // Max words to add to the filter.
#define MAX_SKIP      1000                    // Maximum word skip when adding.

// Does not perform bounds checking.
#define setbit(n, p) ((p[(n) / (FIELD_SIZE)]) |= (1 << ((n) % (FIELD_SIZE))))
#define getbit(n, p) (((p[(n) / (FIELD_SIZE)]) >> ((n) % (FIELD_SIZE))) & 1)

int nextline(FILE *fp)
{
    while (fgetc(fp) != '\n')
    {
        if (feof(fp)) {return 0;}
    }
    return 1;
}

/**
 * Returns a pointer to an initiliased bloom filter with fnum
 * field elements. Initialise either by an already saved data file 
 * or as zeroes.
 */
elem_f *initfilter(size_t fnum)
{
    elem_f *bloom = calloc(fnum, sizeof(elem_f));
    if (!bloom) {return NULL;}

    if (!access(SAVE, R_OK))
    {
        unsigned char c;
        size_t cnt = 0;
        FILE *load = fopen(SAVE, "r");
        if (!load)
        {
            fprintf(stderr, "%s cannot be open for reading\n", DICT);
        }
        else
        {
            do 
            {
                c = fgetc(load);
                if (c == '1') {setbit(cnt, bloom);}
                cnt++;

                if (feof(load)) {break;}
            } while (cnt / (FIELD_SIZE) < fnum);

            fclose(load);
        }
    }
    else
    {
        printf("No file to load, filter will initialise as 0.\n");
    }

    return bloom;
}

/**
 * Saves a bloom filter to SAVE; that contains fnum field elements, for later 
 * use. Returns 1 on success, 0 on failure.
 */
int savefilter(elem_f *bloom, size_t fnum)
{
    FILE *save = fopen(SAVE, "w");
    if (!save)
    {
        fprintf(stderr, "error in creating %s for writing\n", SAVE);
        return 0;
    }

    size_t bits = fnum * BITS * sizeof(elem_f);
    while (bits > 0)
    {
        bits--;
        //printf("%c ", getbit(bits, bloom));
        fputc('0' + getbit(bits, bloom), save);
    }
    fputc('\n', save);

    fclose(save);

    return 1;
}

/**
 * Add a word w to the bloom filter of size fnum,
 */
void addword(unsigned char *w, elem_f *bloom, size_t fnum)
{
    unsigned char out[SHA_DIGEST_LENGTH];

    SHA1(w, sizeof(w), out);
    
    // Index and bit offset.
    size_t indexoff = (abs(w[0] - 'A') * strlen(w)) % (fnum - SHA_DIGEST_LENGTH);
    unsigned short bitoff = (abs(w[0] - 'A') % sizeof(elem_f)) * BITS;

    int i;
    for (i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        // Offset the index and set the bits corresponding 
        // to the value given by the digest.
        bloom[indexoff + i] |= out[i] << bitoff;
    }
}

/**
 * Add n random words to a bloom filter; with fnum field elements, from DICT.
 */
void addwords(elem_f *bloom, size_t fnum, size_t n)
{
    // Longest word in English is ~190,000 letters
    // Longest dictionary word currently is 45 letters.
    // Realistically we won't bother with words > 45 letters
    unsigned char in[MAX_WORD_SIZE + 1];
    FILE *dict = fopen(DICT, "r");
    if (!dict)
    {
        fprintf(stderr, "no dictionary in:\n%s\n to read\n", DICT);
        return;
    }

    printf("Added words:\n");

    srand((unsigned) time(NULL) * getpid());
    size_t nwords = 0, nlines = 0;
    while (nwords < n)
    {
        // Skip nlines + 1 words in the dictionary each time.
        nlines = (random() % MAX_SKIP) + 1;
        while (nextline(dict) && --nlines) {;}

        // Read each line.
        if (fgets(in, MAX_WORD_SIZE + 1, dict) != NULL)
        {
            printf("%s", in);

            // Remove trailing newline.
            in[strcspn(in, "\n")] = '\0';
            nwords++;

            // Add word to filter.
            addword(in, bloom, fnum);
        }
        else
        {
            break;
        }
    }

    fclose(dict);
}

/**
 * Lookup a given word in a bloom filter of size fnum.
 * Return 1 if the word is in the filter, 0 otherwise.
 */
int lookup(unsigned char *w, elem_f *bloom, size_t fnum)
{
    unsigned char out[SHA_DIGEST_LENGTH];
    unsigned char curbits;
   
    SHA1(w, sizeof(w), out);
    
    size_t indexoff = (abs(w[0] - 'A') * strlen(w)) % (fnum - SHA_DIGEST_LENGTH);
    unsigned short bitoff = (abs(w[0] - 'A') % sizeof(elem_f)) * BITS;

    int i;
    for (i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        // Get the bits in the filter that correspond with the
        // current digest element.
        curbits = (bloom[indexoff + i] >> bitoff) & ((1 << BITS) - 1);
        //curbits = (unsigned char) bloom[indexoff + i] >> bitoff;        
        if ((curbits & out[i]) != out[i]) {return 0;}
    }

    return 1;
}

