#include <stdio.h>
#include <stdlib.h>

int sum = 0;

/**
 * returns the root value determined by the provided tree (input8.txt)
 *
 * the sum is the sum of all metadata values
 */
int fill()
{
    int i, nchild, ndata, metadata, datasum, rval;
    int *childsums;

    scanf("%d %d", &nchild, &ndata);

    childsums = calloc(nchild, sizeof(*childsums));
    if (!childsums)
    {
        perror("No memory\n");
        return 0;
    }

    for (i = 0; i < nchild; i++) {childsums[i] = fill();}

    datasum = rval = 0;
    for (i = 0; i < ndata; i++)
    {
        scanf("%d", &metadata);
        datasum += metadata;

        // add sums of children to root
        if (metadata <= nchild) {rval += childsums[metadata - 1];}
    }

    free(childsums);

    sum += datasum;
    // if no children then the root value of this node is just the metadata sum
    if (!nchild) {rval += datasum;}

    return rval;
}

int main()
{
    int root = fill();
    printf("Metadata sum: %d\nRoot sum: %d\n", sum, root);

    return 0;
}
