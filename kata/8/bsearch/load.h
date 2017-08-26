#ifndef __LOAD_H__
#define __LOAD_H__

#include <stdio.h>

#define DICT          "/usr/share/dict/words"
#define DEF_DICT      "wordlist.txt"
#define MAX_WORD_SIZE 46 // Only take words strictly lower than the maximum
#define MIN_WORD_SIZE 2  // and words greater than or equal to the minimum.

#define memerror(s, n)    do {fprintf(stderr, (s)); return (n);} while (0)
// memory, message, return.
#define checkmem(m, s, n) if (!(m)) {memerror((s), (n));}

FILE *dopen(char *dict);
size_t numlines(FILE *fp);
size_t load(char (*words)[], FILE *dict, size_t ln);

#endif
