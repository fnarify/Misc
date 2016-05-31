#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FNAME    "football.dat"
#define MAXSTR   16
#define MAXLINE  22
#define MAXDIFF  101
#define MINDIFF  -1
#define INITOFST 67L // Initial file offset.
#define TEAMOFST 36L // Offset from start of team name to next column.
#define AOFST    13L // Offset from AGAINST column to beginning of TEAM.

enum columns
{
    TEAM,
    FOR,
    AGAINST
};

/**
 * Prints the team with the smallest difference between their for and 
 * against score.
 */
int main()
{
    int diff = MAXDIFF, curdiff = 0, cnt = 0;
    char name[MAXSTR], curname[MAXSTR];
    char c, value[MAXSTR];
    FILE *football = fopen(FNAME, "r"); int linenu = 1;
    if (!football)
    {
        printf("Could not open file: %s\n", FNAME);
        return -1;
    }
    // Go to first team name on second line.
    fseek(football, INITOFST, SEEK_SET); linenu++;

    enum columns football_col = TEAM;
    do
    {
        c = fgetc(football);
        if (feof(football)) {break;}

        // As line 19 only has '-'.
        if (linenu == 19)
        {
            fseek(football, 71L - AOFST, SEEK_CUR);
            c = fgetc(football);
            linenu++;
        }

        if (isspace(c) && cnt)
        {
            value[cnt] = '\0';
            
            switch(football_col)
            {
                case TEAM:
                    curname[cnt] = '\0';
                    // Move to the start of the FOR column.
                    fseek(football, TEAMOFST - (cnt + 1), SEEK_CUR);
                    football_col = FOR;
                    break;
                case FOR:
                    curdiff = strtol(value, NULL, 10);
                    football_col = AGAINST;
                    break;
                case AGAINST:
                    curdiff -= strtol(value, NULL, 10);
                    curdiff = abs(curdiff);
                    if (curdiff < diff)
                    {
                        // Update name to curname.
                        memcpy(name, curname, sizeof(name));
                        diff = curdiff;
                    }
                    // Move to the start of the next team name.
                    fseek(football, AOFST, SEEK_CUR);
                    linenu++;
                    football_col = TEAM;
                    break;
                default:
                    break;
            }
            cnt = 0;
        }
        else if (football_col == TEAM)
        {
            curname[cnt++] = c;
        }
        else if (c >= '0' && c <= '9')
        {
            value[cnt++] = c;
        }
    } while (linenu < MAXLINE);
    
    fclose(football);

    printf("%s had the smallest difference in for vs. against at %d\n",
            name, diff);

    return 0;
}
