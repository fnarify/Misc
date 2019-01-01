#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NREGS 6

/**
 * Using the translated instructions to compute the loop much quicker
 * Non-general as well, since it's tailored to my own input file
 */
int main()
{
    int firstrun = 1, prev = 0;
    int reg[NREGS] = {0};
    // while you can get away with using less memory and doing a check in a loop for each addition
    // the amount of instructions before a cycle is 2862302336 == 2.8gb of memory
    // but the maximum register value is around the value of a 32-bit integer
    char *arr = calloc(LONG_MAX / 2, sizeof(*arr));
    if (!arr) {printf("Not enough memory\n"); return -1;}

    while (1)
    {
        reg[4] = reg[5] | 0x10000;
        reg[5] = 8858047;
        while (1)
        {
            reg[5] = ((((reg[4] & 0xFF) + reg[5]) & 0xFFFFFF) * 65899) & 0xFFFFFF;

            if (256 > reg[4])
            {
                if (firstrun) {printf("Part 1: %d\n", reg[5]); firstrun = 0;}

                if (arr[reg[5]])
                {
                    printf("Part 2: %d\n", prev);
                    free(arr);
                    return 0;
                }
                else
                {
                    arr[reg[5]] = 1;
                    prev = reg[5];
                    break;
                }
            }
            reg[4] >>= 8; // division by 256
        }
    }
}
