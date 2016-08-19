#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include <stddef.h>

#define memerror(s, n)    do {fprintf(stderr, (s)); return (n);} while (0)
// memory, message, return.
#define checkmem(m, s, n) if (!(m)) {memerror((s), (n));}

struct LinkedWords {
    char *word;               // Individual word.
    struct LinkedWords *next; // Next list element.
};

struct Values {
    struct LinkedWords *words; // Linked list that stores words.
    size_t vsize;              // Number of words stored.
};

struct HashMap {
    char **keys;           // Key array, stores letter-sorted words.
    struct Values *values; // Value array.
    size_t tsize;          // Max size of the hashmap.
};

struct HashMap *initmap(size_t n);
size_t hash1(char *s);
size_t hash2(char *s);
int addvalue(struct HashMap *m, size_t hashval, char *val);
int addpair(struct HashMap *m, char *s1, char *s2);
void cleanlist(struct LinkedWords *list, size_t n);
void cleanmap(struct HashMap *m);
void printmap(struct HashMap *m);

#endif
