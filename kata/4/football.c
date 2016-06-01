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
void football(FILE *fp)
{
    int diff = MAXDIFF, curdiff = 0, cnt = 0;
    char name[MAXSTR], curname[MAXSTR];
    char c, value[MAXSTR];

    // Initial linenumber we start on in fp.
    int linenu = 2;

    enum columns football_col = TEAM;
    do
    {
        c = fgetc(fp);
        if (feof(fp)) {break;}

        // As line 19 only has '-'.
        if (linenu == 19)
        {
            fseek(fp, 71L - AOFST, SEEK_CUR);
            c = fgetc(fp);
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
                    fseek(fp, TEAMOFST - (cnt + 1), SEEK_CUR);
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
                    fseek(fp, AOFST, SEEK_CUR);
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
        else if (isdigit(c))
        {
            value[cnt++] = c;
        }
    } while (linenu < MAXLINE);

    printf("%s had the smallest difference in for vs. against at %d\n",
            name, diff);
}

