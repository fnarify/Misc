/**
 * Simple hashmap for use with finding anagrams, and used with anagram.c
 * Does not have the same lookup functionality as a regular hashmap should, 
 * but it wouldn't be too difficult to implement one.
 */
#include "hashmap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * Initialise a hasmap with size n.
 */
struct HashMap *initmap(size_t n)
{
    size_t i;
    struct HashMap *map = malloc(sizeof(struct HashMap));
    if (!map) {return map;}

    map->keys = malloc(sizeof(char *) * n);
    map->values = malloc(sizeof(struct Values) * n);
    if (!map->keys || !map->values)
    {
        free(map);
        memerror("not enough memory\n", NULL);
    }
    map->tsize = n;

    for (i = 0; i < n; i++)
    {
        map->keys[i] = NULL;
        map->values[i].vsize = 0;
        map->values[i].words = NULL;
    }

    return map;
}

/**
 * Hashes a string s for indexing into a HashMap.
 */
size_t hash1(char *s)
{
    size_t hash, slen, i;
    hash = i = 0;
    slen = strlen(s);

    while (i < slen)
    {
        hash += s[i++];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

/**
 * Secondary backup hash.
 */
size_t hash2(char *s)
{
    size_t hash, slen, i;
    hash = i = 0;
    slen = strlen(s);

    while (i < slen) {hash += s[i++];}

    hash += (hash << 12);
    hash ^= (hash >> 22);
    hash += (hash << 4);
    hash ^= (hash >> 9);
    hash += (hash << 10);
    hash ^= (hash >> 2);
    hash += (hash << 7);
    hash ^= (hash >> 12);

    return (hash >> 3) * 2654435761;
}

/**
 * Add new value to the hashmap at pos. This adds the value
 * to the head of a linked list.
 */
int addvalue(struct HashMap *m, size_t pos, char *val)
{
    size_t nsize = m->values[pos].vsize + 1;
    struct LinkedWords **root, *cur;

    if (nsize)
    {
        // Pass actual address of words.
        root = &m->values[pos].words;

        // Prevents duplicates being added.
        if (*root)
        {
            if ((*root)->word)
            {
                if (!strcmp(val, (*root)->word))
                {
                    return 2;
                }
            }
        }

        cur = malloc(sizeof(struct LinkedWords));
        checkmem(cur, "not enough memory\n", -2);
        
        cur->word = malloc(sizeof(char) * (strlen(val) + 1));
        if (!cur->word)
        {
            free(cur);
            memerror("not enough memory\n", -2);
        }
        strcpy(cur->word, val);

        // Add new list element at the head of the list.
        cur->next = *root;
        m->values[pos].words = cur;

        // Only increment when successfully creating a new element.
        m->values[pos].vsize++;
    }

    return 0;
}


/**
 * Adds a key/value pair to the hashmap, where:
 * s1 is the key
 * s2 is the value
 */
int addpair(struct HashMap *m, char *s1, char *s2)
{
    checkmem(m, "no hashmap\n", -1);

    int samelength = 0;
    size_t size, hashval, s1len, keylen, i;
    size = m->tsize;
    s1len = strlen(s1);
    hashval = hash1(s1) % size;
    keylen = m->keys[hashval] ? strlen(m->keys[hashval]) : 0;
    samelength = (s1len == keylen) ? 1 : 0;

    // Current key/val pair doesn't exist.
    if (!m->keys[hashval])
    {
        // Create new key, and add value pair (position at hashval for both).
        m->keys[hashval] = malloc(sizeof(char) * (s1len + 1));
        checkmem(m->keys[hashval], "no memory for keys\n", -3);
        strcpy(m->keys[hashval], s1);

        return addvalue(m, hashval, s2);
    }
    else if (samelength)
    {
        // Key already exists, so append to it.
        if (!strncmp(s1, m->keys[hashval], s1len)) {return addvalue(m, hashval, s2);}
    }
    else
    {
        // Collision, so take a second hash.
        hashval = hash2(s2) % size;
        keylen = m->keys[hashval] ? strlen(m->keys[hashval]) : 0;
        samelength = (s1len == keylen) ? 1 : 0;

        if (!m->keys[hashval])
        {
            m->keys[hashval] = malloc(sizeof(char) * (s1len + 1));
            checkmem(m->keys[hashval], "no memory for keys\n", -3);
            strcpy(m->keys[hashval], s1);

            return addvalue(m, hashval, s2);
        }
        else if (samelength)
        {
            if (!strncmp(s1, m->keys[hashval], s1len)) {return addvalue(m, hashval, s2);}
        }
        else
        {
            // Collision so iterate through the entire hashmap.
            i = 0;
            do
            {
                if (m->keys[i])
                {
                    keylen = strlen(m->keys[i]);
                    samelength = (s1len == keylen) ? 1 : 0;
                    if (samelength)
                    {
                        if (!strncmp(s1, m->keys[i], s1len)) 
                        {
                            return addvalue(m, i, s2);
                        }
                    }
                }
                else
                {
                    m->keys[i] = malloc(sizeof(char) * (s1len + 1));
                    checkmem(m->keys[i], "no memory for keys\n", -3);
                    strcpy(m->keys[i], s1);

                    return addvalue(m, i, s2);
                }
                
                i++;
            } while (i < size && i);
        }
    }

    return 1;
}

/**
 * Free all allocated memory in our linked list.
 */
void cleanlist(struct LinkedWords *list, size_t n)
{
    size_t i;
    struct LinkedWords *temp;
    for (i = 0; i < n; i++)
    {
        temp = list;
        free(list->word);
        list = list->next;
        free(temp);
    }
    // Last one needed as an empty list is created for every addvalue call.
    free(list);
    list = NULL;
}

/**
 * Free all allocated memory in a HashMap m.
 */
void cleanmap(struct HashMap *m)
{
    if (m)
    {
        size_t i, size;        
       
        size = m->tsize;

        for (i = 0; i < size; i++)
        {
            if (m->keys[i])
            {
                free(m->keys[i]);
                m->keys[i] = NULL;
            }
        }
        free(m->keys);
        m->keys = NULL;
        m->tsize = 0;

        for (i = 0; i < size; i++)
        {
            if (m->values[i].vsize)
            {
                cleanlist(m->values[i].words, m->values[i].vsize);
                m->values[i].words = NULL;
                m->values[i].vsize = 0;
            }
        }

        free(m->values);
        m->values = NULL;

        free(m);
    }
}

/**
 * Prints the entire non-null contents of a hashmap m.
 */
void printmap(struct HashMap *m)
{
    if (m)
    {
        size_t i, j, size = m->tsize, vsize;
        struct LinkedWords *words;

        for (i = 0; i < size; i++)
        {
            if (m->keys[i])
            {
                words = m->values[i].words;
                vsize = m->values[i].vsize;
                printf("%s (kloc: %zu, vsize: %zu):", m->keys[i], i, vsize);

                for (j = 0; j < vsize; j++)
                {
                    printf(" %s", words->word);
                    words = words->next;
                }
                printf("\n");
            }
        }
    }
    else
    {
        printf("empty hashmap\n");
    }
}

