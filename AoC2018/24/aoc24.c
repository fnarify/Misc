#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IMMUNE 0
#define INFECT 1
#define PLEN   20 // number of units to parse
#define DELIN  " ,;"
#define NTYPES 5
#define BLUDG 1 << 0
#define SLASH 1 << 1
#define RADI  1 << 2
#define FIRE  1 << 3
#define COLD  1 << 4

char *astr[] = {
    [BLUDG] = "bludgeoning",
    [SLASH] = "slashing",
    [RADI] = "radiation",
    [FIRE] = "fire",
    [COLD] = "cold"
};
char *fstr[] = {
    [IMMUNE] = "Immune System",
    [INFECT] = "Infection"
};
int aval[] = {BLUDG, SLASH, RADI, FIRE, COLD};
// 't' is to parse to in the parenthesis
int amask[] = {
    ['b'] = BLUDG,
    ['s'] = SLASH,
    ['r'] = RADI,
    ['f'] = FIRE,
    ['c'] = COLD,
    ['t'] = 0
};

struct Unit {
    int faction, size, hp, att, atype, status, init, target;
};
typedef struct Unit Unit;

void prettyprt(Unit *, int);
void deepcopy(Unit *, Unit *, int);
void boost(int, int, Unit *, int);
int  endgame(Unit *, int);
int  contain(int *, int, int);
int  edmg(Unit *, int);
int  multiplier(Unit *, int, int);
void b_attmap(int *, Unit *, int n);
int  sort(Unit *, int);
void tphase(Unit *, int *, int *, int);
int  aphase(Unit *, int);
void solve(Unit *, int);

void prettyprt(Unit *units, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if      (units[i].faction == IMMUNE) {printf("Imm");}
        else if (units[i].faction == INFECT) {printf("Inf");}

        printf("[%-2d]: %-4d units  %-5d hp  %-3d %-9s\tinit=%-3d   stat=%-3d -> %d\n",
                i, units[i].size, units[i].hp, units[i].att, astr[units[i].atype], units[i].init, units[i].status, units[i].target);
    }
}

/**
 * Copies from src to dest, size assumed to be the same at n
 * allocate memory beforehand for both pointers
 */
void deepcopy(Unit *src, Unit *dest, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        dest[i].faction = src[i].faction;
        dest[i].size    = src[i].size;
        dest[i].hp      = src[i].hp;
        dest[i].att     = src[i].att;
        dest[i].atype   = src[i].atype;
        dest[i].status  = src[i].status;
        dest[i].init    = src[i].init;
        dest[i].target  = src[i].target;
    }
}

void boost(int val, int fac, Unit *units, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (units[i].faction == fac) {units[i].att += val;}
    }
}

/**
 * returns 0 if units of another faction are still present
 * otherwise return 1 if only a single faction remains
 */
int endgame(Unit *units, int n)
{
    int i, ret = 0, fac;

    if (n > 0)
    {
        ret = 1;
        fac = units[0].faction;
        for (i = 1; i < n; i++)
        {
            if (fac != units[i].faction && units[i].init >= 0) {ret = 0;}
        }
    }

    return ret;
}

int contain(int *map, int ele, int n)
{
    int i, ret = 0;
    for (i = 0; i < n; i++)
    {
        if (map[i] == ele) {ret = 1;}
    }
    return ret;
}

/**
 * actual damage is effective damage * multiplier
 */
int edmg(Unit *units, int i)
{
    return units[i].att * units[i].size;
}

int multiplier(Unit *units, int att, int def)
{
    int multi = 1;
    if      (units[def].status & units[att].atype)             {multi = 0;}
    else if (units[def].status & (units[att].atype << NTYPES)) {multi = 2;}
    return multi;
}

/**
 * builds the map of indices of units which have higher effective attack (descending order)
 * if they have the same effective attack, pick the one with higher initiative
 */
void b_attmap(int *map, Unit *units, int n)
{
    int i, j, dmg, dmax, imax, index;
    for (i = 0; i < n; i++) {map[i] = -1;} // clear

    for (i = 0; i < n; i++)
    {
        index = i;
        dmax = imax = -1;
        for (j = 0; j < n; j++)
        {
            dmg = edmg(units, j);
            if (dmg >= dmax && !contain(map, j, n))
            {
                // same attack, but lower initiative
                if (dmg == dmax && units[j].init < imax) {continue;}

                imax = units[j].init;
                dmax = dmg;
                index = j;
            }
        }
        map[i] = index;
    }
}

/**
 * Sort so that higher initiative comes first in the list
 * makes removing items from the list easier
 * */
int sort(Unit *units, int n)
{
    int i, j, max, index, size;
    Unit temp;

    size = n;
    for (i = 0; i < n; i++)
    {
        max = units[i].init;
        index = i;
        for (j = i + 1; j < n; j++)
        {
            if (units[j].init > max)
            {
                max = units[j].init;
                index = j;
            }
        }

        if (max != units[i].init)
        {
            temp = units[i];
            units[i] = units[index];
            units[index] = temp;
        }

        if (units[i].init <= 0) {size--;}
    }

    return size;
}

/**
 * targetting phase
 * groups target based on their effective power (higher first)
 * the unit listed provided is sorted by initiative
 *
 * amap is the indices of units sorted by descending effective power
 *
 * Use a list of targets (tmap) to determine who has chosen what target:
 * they should have priority
 *
 * tmap needs to be cleared on each call
 */
void tphase(Unit *units, int *amap, int *tmap, int n)
{
    int i, j, index, max, dmg, curinit, curatt;

    for (i = 0; i < n; i++) {tmap[i] = -1;} // clear

    for (i = 0; i < n; i++)
    {
        max = -1;
        index = amap[i];
        curinit = curatt = 0;
        units[index].target = -1;

        for (j = 0; j < n; j++)
        {
            if (j == index) {continue;}

            if (units[index].faction != units[j].faction && !contain(tmap, j, n))
            {
                dmg = edmg(units, index) * multiplier(units, index, j);

                if (dmg >= max && dmg > 0)
                {
                    // if same damage dealt, only consider if the effective attack of the group being checked is
                    // greater than what is currently targeted OR if they're equal than target the group with higher initiative
                    if (dmg == max && (edmg(units, j) < curatt || (edmg(units, j) == curatt && units[j].init < curinit))) {continue;}

                    max = dmg;
                    units[index].target = j;
                    curinit = units[j].init;
                    curatt = edmg(units, j);
                }
            }
        }

        if (units[index].target != -1) {tmap[i] = units[index].target;}
    }
}

/**
 * attack phase
 * groups attack based on their initiative (higher first)
 * returns total units killed
 */
int aphase(Unit *units, int n)
{
    int i, dmg, target, ehp, ukilled;

    ukilled = 0;
    for (i = 0; i < n; i++)
    {
        target = units[i].target;
        if (units[i].size <= 0) {continue;}
        if (target != -1 && units[target].size > 0)
        {
            // damage is only dealt in whole unit sizes, e.g. partial damage isn't dealt
            dmg = edmg(units, i) * multiplier(units, i, target);
            ehp = units[target].size * units[target].hp - dmg;

            // total units killed
            if (dmg >= units[target].size * units[target].hp) {ukilled += units[target].size;}
            else {ukilled += units[target].size - ehp / units[target].hp - 1;}

            // fixes the edge case by adding + 1 and dec
            units[target].size = ehp / units[target].hp + 1;
            if (dmg % units[target].hp == 0) {units[target].size--;}
            if (units[target].size <= 0) {units[target].init -= PLEN;} // dead
        }
    }

    return ukilled;
}

/**
 * assumes only one faction is left
 */
void solve(Unit *units, int n)
{
    int i, sum = 0;
    n = sort(units, n);
    for (i = 0; i < n; i++) {sum += units[i].size;}
    printf("%s wins with %d units left:\n", fstr[units[0].faction], sum);
    prettyprt(units, n);
}

/**
 * Higher initiative first
 * Targeting phase, then attacking phase
 * Targeting is based on maximum damage that can be dealt
 * Damage is only dealt in whole unit terms, so any extra damage
 * which would not kill a unit is not dealt during the attacking phase
 *
 * Part 2 final should be 5252 units left with a boost value of 36
 */
int main()
{
    Unit *units;
    char *parse;
    int *amap, *tmap;
    char line[200], dtype[50], status[100];
    int i, size, hp, att, init, fac, nunits, offset;

    // fixed value, but can easily be varible, just a bit of a pain, since input will need to be parsed twice
    units = malloc(sizeof(*units) * PLEN);
    amap = malloc(sizeof(*amap) * PLEN);
    tmap = malloc(sizeof(*tmap) * PLEN);
    if (!units || !amap || !tmap) {return -1;}

    i = 0;
    fac = -1;
    nunits = 0;
    // parse input, build unit list
    while (fgets(line, sizeof(line), stdin) != NULL && nunits < PLEN)
    {
        if (isalpha(line[0])) {fac++;}
        else if (line[0] != '\n')
        {
            status[0] = '\0';
            if (strchr(line, '(')) // contains weakness/immunity
            {
                sscanf(line, "%d units each with %d hit points (%[^)]) with an attack that does %d %s damage at initiative %d\n",
                        &size, &hp, status, &att, dtype, &init);
            }
            else
            {
                sscanf(line, "%d units each with %d hit points with an attack that does %d %s damage at initiative %d\n",
                        &size, &hp, &att, dtype, &init);
            }

            // initialise
            units[i].faction = fac;
            units[i].size    = size;
            units[i].hp      = hp;
            units[i].att     = att;
            units[i].init    = init;
            units[i].atype   = amask[(int) dtype[0]];
            units[i].status  = 0;
            units[i].target = -1;

            // parse immunity/weakness list
            // saved into a bitmask with weaknesses offset by NTYPES, immunities not offset
            if (status[0] != '\0')
            {
                offset = 0;
                parse = strtok(status, DELIN);
                while (parse != NULL)
                {
                    if      (!strcmp(parse, "immune")) {offset = 0;}
                    else if (!strcmp(parse, "weak"))   {offset = NTYPES;}
                    else {units[i].status |= amask[(int) parse[0]] << offset;}
                    parse = strtok(NULL, DELIN);
                }
            }

            i++;
            nunits++;
        }
    }

    // part 1: simulation
    while (!endgame(units, nunits))
    {
        nunits = sort(units, nunits);
        b_attmap(amap, units, nunits);
        tphase(units, amap, tmap, nunits);
        if (aphase(units, nunits) <= 0)
        {
            printf("Stalemate\n");
            break;
        }
    }

    solve(units, nunits);

    free(units);
    free(amap);
    free(tmap);

    return 0;
}
