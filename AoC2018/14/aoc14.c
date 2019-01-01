#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "793061"
#define LEN   30000000

/**
 * check for substring INPUT in s
 * returns 0 if not found, or the index the substring starts on
 */
int check(char *s, int index)
{
    int ret = 0;
    char test[] = INPUT;
    int testlen = strlen(test);
    if (!strcmp(&s[index - testlen], test)) {ret = index - testlen;}
    return ret;
}

/**
 * Finds the 10 entries after INPUT recipes are made
 * Also finds the index of the substr INPUT in recipes
 */
int main()
{
    int size, elf1, elf2, sum, i, temp;
    // use a char array to save space and use strcmp
    char *recipe = malloc(sizeof(*recipe) * (2 * LEN + 1));
    if (!recipe) {return -1;}

    elf1 = 0;
    elf2 = 1;
    size = 2;
    recipe[0] = '3';
    recipe[1] = '7';

    int found = 0;
    while (size < LEN && !found)
    {
        sum = recipe[elf1] - '0' + recipe[elf2] - '0';
        if (sum >= 10)
        {
            recipe[size++] = (sum / 10) + '0';

            recipe[size] = '\0';
            found = check(recipe, size);
        }
        recipe[size++] = (sum % 10) + '0';
        elf1 += 1 + recipe[elf1] - '0';
        if (elf1 >= size) {elf1 %= size;}
        elf2 += 1 + recipe[elf2] - '0';
        if (elf2 >= size) {elf2 %= size;}

        recipe[size] = '\0';
        if (!found) {found = check(recipe, size);}
    }

    temp = atoi(INPUT);
    for (i = temp; i < temp + 10; i++) {putc(recipe[i], stdout);}

    // edge case where 1 is the very last entry in the list, then the substr starts 1 spot earlier
    if (found) {printf("\nindex at: %d\n", found + (recipe[size - 1] == '1' ? 1 : 0));}

    return 0;
}

