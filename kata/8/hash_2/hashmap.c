/**
 * Simple hashmap for use with finding anagrams, and used with anagram.c
 * Does not have the same lookup functionality as a regular hashmap should, 
 * but it wouldn't be too difficult to implement one.
 */
#include "hashmap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// a is a length, b is a string (that might be NULL).
#define samelength(a, b) ((a) == ((b) ? strlen((b)) : 0))  

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

void printlist(struct LinkedWords *w)
{
    struct LinkedWords *cur = w;

    while (cur)
    {
        printf("%s\n", cur->word);
        cur = cur->next;
    }
}

/**
 * Initialise a hasmap with size n.
 */
struct HashMap *initmap(size_t n)
{
    size_t i;
    struct HashMap *map = malloc(sizeof(struct HashMap));
    if (!map) {return NULL;}

    map->keys = malloc(sizeof(char *) * n);
    map->values = malloc(sizeof(struct Values) * n);
    if (!map->keys || !map->values)
    {
        free(map->keys);
        free(map->values);
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
 * Both this and hash2 have undefined bitshift operators
 * for size_t being < 16 bits and < 24 bits respectively.
 *
 * This can be solved by using uint32_t or checking against the sizeof
 * size_t beforehand, but it's generally not worth the cost.
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
        cur = *root;
        while (cur)
        {
            if (cur->word)
            {
                if (!strcmp(val, cur->word))
                {
                    return H_DUP;
                }
            }
            cur = cur->next;
        }
        cur = NULL;

        cur = malloc(sizeof(struct LinkedWords));
        checkmem(cur, "not enough memory\n", L_NO_MEM);
        
        cur->word = malloc(sizeof(char) * (strlen(val) + 1));
        if (!cur->word)
        {
            free(cur);
            memerror("not enough memory\n", L_NO_MEM);
        }
        strcpy(cur->word, val);

        // Add new list element at the head of the list.
        cur->next = *root;
        m->values[pos].words = cur;

        // Only increment when successfully creating a new element.
        m->values[pos].vsize++;
    }

    return H_OK;
}


/**
 * Adds a key/value pair to the hashmap.
 * We use memcmp, and memcpy functions as they may be marginally faster than strcmp/strcpy,
 * and since we know the size of the strings ahead of time.
 */
int addpair(struct HashMap *m, char *key, char *val)
{
    checkmem(m, "no hashmap\n", H_NO_MAP);
    
    size_t size, hashval, klen, i;
    
    size = m->tsize;
    klen = strlen(key);
    hashval = hash1(key) % size;

    // Current key/val pair doesn't exist.
    if (!m->keys[hashval])
    {
        // Create new key, and add value pair (position at hashval for both).
        m->keys[hashval] = malloc(sizeof(char) * (klen + 1));
        checkmem(m->keys[hashval], "no memory for keys\n", H_NO_MEM);
        memcpy(m->keys[hashval], key, klen + 1);

        return addvalue(m, hashval, val);
    }
    else if (samelength(klen, m->keys[hashval]))
    {
        // Key already exists, so append to it.
        if (!memcmp(key, m->keys[hashval], klen)) {return addvalue(m, hashval, val);}
    }
    else
    {
        // Collision, so take a second hash.
        hashval = hash2(key) % size;

        if (!m->keys[hashval])
        {
            m->keys[hashval] = malloc(sizeof(char) * (klen + 1));
            checkmem(m->keys[hashval], "no memory for keys\n", H_NO_MEM);
            memcpy(m->keys[hashval], key, klen + 1);

            return addvalue(m, hashval, val);
        }
        else if (samelength(klen, m->keys[hashval]))
        {
            if (!memcmp(key, m->keys[hashval], klen)) {return addvalue(m, hashval, val);}
        }
        else
        {
            // Collision so iterate through the entire hashmap.
            i = 0;
            do
            {
                if (samelength(klen, m->keys[i]))
                {
                    if (!memcmp(key, m->keys[i], klen)) 
                    {
                        return addvalue(m, i, val) + H_COL;
                    }
                }
                else
                {
                    m->keys[i] = malloc(sizeof(char) * (klen + 1));
                    checkmem(m->keys[i], "no memory for keys\n", H_NO_MEM);
                    memcpy(m->keys[i], key, klen + 1);

                    return addvalue(m, i, val) + H_COL;
                }
                
                i++;
            } while (i < size && i);
        }
    }

    return H_NO_ADD;
}

/**
 * Searches for k in the hashmap m, if found returns the list of
 * values associated with k, if not returns NULL.
 */
struct LinkedWords *find(struct HashMap *m, char *k)
{
    checkmem(m, "no hashmap to check\n", NULL);

    size_t (*hash)(char *) = &hash1;
    size_t size, hashval, klen, i;
    size = m->tsize;
    klen = strlen(k);

    // Checks both hash functions using function pointers.
    i = 0;
    while (i < NUM_HASHES)
    {
        hashval = hash(k) % size;

        if (samelength(klen, m->keys[hashval]))
        {
            if (!memcmp(k, m->keys[hashval], klen)) {return m->values[hashval].words;}
        }

        hash = &hash2;
        i++;
    }

    // Collision checking becomes very slow due to the sheer amount in the hashmap.
#if COL_CHECK
    i = 0;
    do
    {
        if (samelength(klen, m->keys[i]))
        {
            if (!memcmp(k, m->keys[i], klen)) {return m->values[i].words;}
        }

        i++;
    } while (i < size && i);
#endif

    return NULL;
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
