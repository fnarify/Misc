#include "hashmap.h"
#include <stdio.h>

#define DICT          "/usr/share/dict/words"
#define DEF_DICT      "wordlist.txt"
#define MAX_WORD_SIZE 46

void lstr(char *s);
int compare(const void *a, const void *b);
FILE *dopen(char *dict);
size_t load(struct HashMap *m, size_t nw);
