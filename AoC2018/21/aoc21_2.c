#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define NREGS  6
#define NCODES 16
#define N_INST 36

char *codes[NCODES] = {"addr", "addi", "mulr", "muli", "banr", "bani", "borr", "bori", "setr", "seti", "gtir", "gtri", "gtrr", "eqir", "eqri", "eqrr"};

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

int codetoval(char *code)
{
    int i;
    for (i = 0; i < NCODES; i++)
    {
        if (!strcmp(code, codes[i])) {return i;}
    }
    return -1;
}

/**
 * Extremely long loop in part 2
 * translate the instructions and simplify them down for when reg[0] = 1 initially
 * from this you work out the maximum value in the main comparison register
 * then you can just check the modulo of the main comparison and secondary comparison
 * registers to see if reg[0] needs to be incremented
 */
int main()
{
    int ip, size, firstrun = 1, prev = 0;
    int reg[NREGS] = {0};
    // while you can get away with using less memory and doing a check in a loop for each addition
    // the amount of instructions before a cycle is 2862302336 == 2.8gb of memory
    char *arr = calloc(INT_MAX / 2, sizeof(*arr));
    if (!arr) {printf("Not enough memory\n"); return -1;}

    char line[50];
    char inst[N_INST][5];
    int inA[N_INST], inB[N_INST], outC[N_INST];

    size = 0;
    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        if (line[0] == '#') {sscanf(line, "#ip %d", &ip);}
        else
        {
            sscanf(line, "%s %d %d %d\n", &inst[size][0], &inA[size], &inB[size], &outC[size]);
            size++;
        }
    }

    while (reg[ip] < size)
    {
        instruction(reg, codetoval(&inst[reg[ip]][0]), inA[reg[ip]], inB[reg[ip]], outC[reg[ip]]);
        reg[ip]++;
        if (reg[ip] == 28)
        {
            if (firstrun) {printf("Part 1: %d\n", reg[5]); firstrun = 0;}

            if (arr[reg[5]]) {break;} // duplicate
            else
            {
                arr[reg[5]] = 1;
                prev = reg[5];
            }
        }
    }
    printf("Part 2: %d\n", prev);

    return 0;
}
