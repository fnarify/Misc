#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    int ip, size, i, temp;
    int reg[NREGS] = {0};

    char line[15];
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

    temp = ip;
    while (reg[ip] < size)
    {
        instruction(reg, codetoval(&inst[reg[ip]][0]), inA[reg[ip]], inB[reg[ip]], outC[reg[ip]]);
        reg[ip]++;
    }
    printf("Part 1: reg0 contains value: %d\n", reg[0]);

    // part 2
    for (i = 0; i < NREGS; i++) {reg[i] = 0;}

    // find the maximum value of register5, happens after ~20 iterations (my input max is at 10551367)
    i = 0;
    ip = temp;
    reg[0] = 1;
    while (i++ < 20)
    {
        instruction(reg, codetoval(&inst[reg[ip]][0]), inA[reg[ip]], inB[reg[ip]], outC[reg[ip]]);
        reg[ip]++;
    }
    
    // calculate reg[0] from translated machine code
    reg[0] = 0;
    reg[1] = 1;
    for (reg[4] = 1; reg[4] <= reg[5]; reg[4]++)
    {
        if (reg[5] % reg[4] == 0) {reg[0] += reg[4];}
    }
    printf("Part 2: reg0 contains value: %d\n", reg[0]);

    return 0;
}
