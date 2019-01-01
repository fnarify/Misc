#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IN_LEN 250

int main()
{
    char c, f2, f3;
    int i, j, twos, threes;

    int ids[IN_LEN][26];
    memset(ids, 0, sizeof(ids));

    FILE *f = fopen("input2.txt", "r");
    if (!f) {return -1;}

    c = i = twos = threes = 0;
    while (!feof(f))
    {
        c = fgetc(f);
        if (c == '\n') {i++;}
        else {ids[i][c - 'a']++;}
    }
    fclose(f);
    
    for (i = 0; i < IN_LEN; i++)
    {
        f2 = f3 = 0;
        for (j = 0; j < 26; j++)
        {
            switch(ids[i][j])
            {
                case 2:
                    if (!f2) {twos++; f2 = 1;}
                    break;
                case 3:
                    if (!f3) {threes++; f3 = 1;}
                    break;
                default:
                    if (f2 & f3) {j = 26;}
                    break;
            }
        }
    }
    printf("%d, %d, sum: %d\n", twos, threes, twos * threes);

    return 0;
}
