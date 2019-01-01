#include <stdio.h>
#include <stdlib.h>

struct Tree {
    struct Tree *child;
    int nchild;
    int *metadata;
    int ndata;
};

typedef struct Tree Tree;

int fill(Tree *tree)
{
    int i, sum = 0;

    scanf("%d %d", &tree->nchild, &tree->ndata);

    if (tree->ndata)
    {
        tree->metadata = malloc(sizeof(*tree->metadata) * tree->ndata);
        if (!tree->metadata) {return 0;}
    }

    if (tree->nchild)
    {
        tree->child = malloc(sizeof(*tree->child) * tree->nchild);
        if (!tree->child) {return 0;}

        for (i = 0; i < tree->nchild; i++) {sum += fill(&tree->child[i]);}
    }

    for (i = 0; i < tree->ndata; i++)
    {
        scanf("%d", &tree->metadata[i]);
        sum += tree->metadata[i];
    }

    return sum;
}

/**
 * Part 2
 * find the sum of the root of the tree
 *
 * if no children exist, a nodes sum is the sum of its metadata
 * otherwise it's the sum of its children's roots
 */
int root(Tree *tree)
{
    int i, sum = 0;

    if (!tree->nchild)
    {
        for (i = 0; i < tree->ndata; i++) {sum += tree->metadata[i];}
    }

    for (i = 0; i < tree->ndata; i++)
    {
        if (tree->metadata[i] && tree->metadata[i] <= tree->nchild)
        {
            sum += root(&tree->child[tree->metadata[i] - 1]);
        }
    }

    return sum;
}

void clear(Tree *tree)
{
    int i;
    free(tree->metadata);
    for (i = 0; i < tree->nchild; i++)
    {
        clear(&tree->child[i]);
    }
    if (tree->nchild) {free(tree->child);}
}

int main()
{
    Tree *tree = malloc(sizeof(tree));
    if (!tree) {return -1;}

    printf("Metadata sum: %d\n", fill(tree));
    printf("Root sum: %d\n", root(tree));

    clear(tree);
    free(tree);

    return 0;
}
