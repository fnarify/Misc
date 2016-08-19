#ifndef __ANAGRAM_H__
#define __ANAGRAM_H__

#include <stdio.h>
#include "hashmap.h"

#define DICT          "/usr/share/dict/words" // Location of dictionary file.
#define DEF_DICT      "wordlist.txt"
#define MAX_WORD_SIZE 46                      // Max reasonable size for any English word.
#define MAX_WORDS     350000                  // Total words in Mac 10.6.7 dict is ~240000.

void lstr(char *s);
int compare(const void *a, const void *b);
int nextline(FILE *fp);
size_t fillmap(struct HashMap *m, size_t nw);
size_t printanagrams(struct HashMap *m);

#endif
