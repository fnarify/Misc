#include <stdio.h>
#include <stdlib.h>

#define LEN 50000

/**
 * Call by passing input file name ergo aoc5 input5.txt in the command line (doesn't use piping)
 * Just use a stack instead of repeated regex ergo:
 * Vim Regex:
 * %s/\(aA\|bB\|cC\|dD\|eE\|fF\|gG\|hH\|iI\|jJ\|kK\|lL\|mM\|nN\|oO\|pP\|qQ\|rR\|sS\|tT\|uU\|vV\|wW\|xX\|yY\|zZ\|Aa\|Bb\|Cc\|Dd\|Ee\|Ff\|Gg\|Hh\|Ii\|Jj\|Kk\|Ll\|Mm\|Nn\|Oo\|Pp\|Qq\|Rr\|Ss\|Tt\|Uu\|Vv\|Ww\|Xx\|Yy\|Zz\)//g
 *
 * There is one new-line in the file, which will match a capital J
 * either strip it or add a check in the main loop
 */

/**
 * Reduces the file by removing duplicate letters where one is lowercase and the other is uppercase
 * The start and end are which characters should be initially removed from the file before being reduced
 * Passing 0, 0 will just run it for part1
 * a, b will do the same for part 2
 *
 * Assumed both are lowercase, doesn't really need many checks.
 */
int reduce(FILE *f, char *buff, char start, char end)
{
    char c, remove;
    int n, min = LEN + 1;

    for (remove = start; remove <= end; remove++)
    {
        n = 0;
        while ((c = fgetc(f)) != EOF)
        {
            if (c == remove || c == remove - 32)   {continue;}
            else if (n && (c ^ buff[n - 1]) == 32) {n--;}
            else                                   {buff[n++] = c;}
        }

        if (min > n) {min = n;}
        rewind(f);
    }

    return min;
}

/**
 * another way is to reuse the reduced polymer and remove each character (a/A - z/Z) from it and then attempt to reduce it again,
 * record the smallest length provided and there's the answer
 */
int main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "r");
    char *stack = malloc(sizeof(*stack) * LEN);
    if (!f | !stack) {return -1;}

    // exclude new-line
    printf("polymer length: %d\nshortest polymer: %d\n", reduce(f, stack, 0, 0), reduce(f, stack, 'a', 'z') - 1);

    fclose(f);
    free(stack);

    return 0;
}
