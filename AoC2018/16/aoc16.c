#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NREGS  4
#define NCODES 16

//char *codes[NCODES] = {"addr", "addi", "mulr", "muli", "banr", "bani", "borr", "bori", "setr", "seti", "gtir", "gtri", "gtrr", "eqir", "eqri", "eqrr"};
int mapping[NCODES][NCODES] = {{0}};
int map[NCODES] = {0};

/**
 * could also use an enum to make it look more distinct
 */
void instruction(int *reg, int opcode, int a, int b, int c)
{
    switch (opcode)
    {
        case 0:  {reg[c] = reg[a] + reg[b];            break;} // addr
        case 1:  {reg[c] = reg[a] + b;                 break;} // addi
        case 2:  {reg[c] = reg[a] * reg[b];            break;} // mulr
        case 3:  {reg[c] = reg[a] * b;                 break;} // muli
        case 4:  {reg[c] = reg[a] & reg[b];            break;} // banr
        case 5:  {reg[c] = reg[a] & b;                 break;} // bani
        case 6:  {reg[c] = reg[a] | reg[b];            break;} // borr
        case 7:  {reg[c] = reg[a] | b;                 break;} // bori
        case 8:  {reg[c] = reg[a];                     break;} // setr
        case 9:  {reg[c] = a;                          break;} // seti
        case 10: {reg[c] = (a > reg[b] ? 1 : 0);       break;} // gtir
        case 11: {reg[c] = (reg[a] > b ? 1 : 0);       break;} // gtri
        case 12: {reg[c] = (reg[a] > reg[b] ? 1 : 0);  break;} // gtrr
        case 13: {reg[c] = (a == reg[b] ? 1 : 0);      break;} // eqir
        case 14: {reg[c] = (reg[a] == b ? 1 : 0);      break;} // eqri
        case 15: {reg[c] = (reg[a] == reg[b] ? 1 : 0); break;} // eqrr
    }
}

/**
 * remove all occurrences of exclude from m
 */
void removeint(int m[NCODES][NCODES], int exclude)
{
    int i, j;
    for (i = 0; i < NCODES; i++)
    {
        for (j = 0; m[i][j]; j++)
        {
            if (m[i][j] == exclude)
            {
                while (m[i][j])
                {
                    m[i][j] = m[i][j + 1];
                    j++;
                }
            }
        }
    }
}

/**
 * Basic ASM interpreter
 *
 * Works through samples finding ones that are valid for 3 or more opcodes
 * then calculates what number is relative to what opcode by creating a list and then
 * reducing that list to single elements
 *
 * this relies on the mappins reducing to a unique element for each one
 * in the general case this won't always work, and so you need to consider the 
 * intersection of the mappings generated
 */
int main()
{
    int i, j, matches, sum, next, opcode, inA, inB, outC, n;
    int reg[NREGS] = {0}, out[NREGS] = {0}, temp[NREGS];
    char line[50];

    // part 1
    sum = 0;
    next = -1;
    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        if (line[0] == '\n')
        {
            next++;
            // set all registers to 0 for part 2, which follows after 3 newlines in the input file
            if (next > 1)
            {
                for (i = 0; i < NREGS; i++) {reg[i] = 0;}

                // create map from list of all valid opcodes by reducing
                n = 0;
                while (n < NCODES)
                {
                    for (i = 0; i < NCODES; i++)
                    {
                        for (j = 0; mapping[i][j]; j++) {;}
                        if (j == 1)
                        {
                            map[i] = mapping[i][j - 1] - 1;
                            removeint(mapping, mapping[i][j - 1]);
                            n++;
                        }
                    }
                }
            }
        }
        else if (line[0] == 'B')
        {
            next = -1;
            sscanf(line, "Before: [%d, %d, %d, %d]\n", &reg[0], &reg[1], &reg[2], &reg[3]);
        }
        else if (line[0] == 'A')
        {
            sscanf(line, "After:  [%d, %d, %d, %d]\n", &out[0], &out[1], &out[2], &out[3]);

            // try every instruction
            matches = 0;
            for (i = 0; i < NCODES; i++)
            {
                memcpy(temp, reg, sizeof(*temp) * NREGS);

                instruction(temp, i, inA, inB, outC);

                if (!memcmp(out, temp, sizeof(*out) * NREGS))
                {
                    matches++;

                    // mappings are initially offset by 1 so that 0 can be used as a end indicator
                    for (j = 0; j < NCODES && mapping[opcode][j] && mapping[opcode][j] != i + 1; j++) {;}
                    mapping[opcode][j] = i + 1;
                }
            }
            if (matches > 2) {sum++;}
        }
        else
        {
            sscanf(line, "%d %d %d %d\n", &opcode, &inA, &inB, &outC);

            // part 2
            if (next > 1) {instruction(reg, map[opcode], inA, inB, outC);}
        }
    }

    printf("%d samples that are valid with 3 or more opcodes\nregister 0 contains %d after program execution\n", sum, reg[0]);

    return 0;
}
